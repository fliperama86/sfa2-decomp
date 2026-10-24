#!/usr/bin/env python3
"""Analysis tool: the animation steps of the stage modules, rebuilt from the arcade's sequence records (the table that stage_steps.c reads).

What it does. A stage module (table 0, slot 6) holds runs of 12-byte animation steps (struct SequenceStep: s16 duration, s16 flags, u32 pointer to a grid, s16 loop
offset, u16 frame index; the same struct as the fighters', see sequences.py) that the objects of the stage play. They are the PS1 tool's conversion of the
arcade objects' sequence records, and the pointer is the module address of the grid (stage_objects.py) that the record's art record became:

  An arcade record of a stage object is 16 bytes: a big-endian 16-bit countdown, a 16-bit flags word, a 32-bit pointer to the art record, 8 bytes of frame data;
  when bit 15 of the flags is set, 4 more bytes follow: the address of the record the sequence jumps to. The next record follows directly.
  The step: duration = the countdown; flags = the flags word (bit 15 stays as the jump, the rest as it is); pointer = 0x801e8000 + the offset of the grid that
  the art record became; loop offset = when the jump flag is set, the step index of the target record minus this step's (0 otherwise; the jump is back or
  to the step itself); frame index = 0 (the stages have no frame records).

The pairing is the one sequences.py uses (pair_records: by countdown, flags and grid, then by grid alone, then in order inside a gap of the same size on both
sides). This tool finds the step runs of a module (consecutive steps that point at a grid), finds where in the arcade program the first step with a known
grid comes from, walks the records from there and pairs. It keeps the steps that the rule above rebuilds to the byte. Nothing is invented: a step that
differs (the PS1 changed a countdown, flags or loop) or has no record is left to the disc.

The grid of an art record. A grid (stage_objects.py) is found for an art record by the cells; where the grid is not one the arcade builds, or several grids have
the same cells, the art record is mapped to the grid by the paired step's own pointer: a layout fact (which module offset the object's art lies at) that the
table carries as `arts` (art record address -> grid offset). A step whose pointer is not its art's usual grid (the art is drawn by two grids) is left to the disc.

    python3 port/arcade/stage_steps.py --disc BIN --arcade ZIP [--only 0E] [--c-table FILE]

--c-table writes port/arcade/stage_steps_table.c. `stages.py --compare-c` builds the C with the checked-in table and checks that its module bytes equal the
model's (build_steps) and the disc's at the known bytes.
"""
import argparse
import collections
import shutil
import struct
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
sys.path.insert(0, str(ROOT / 'port/intro'))
import sequences as Q          # noqa: E402  read only: the pairing of records and steps
import stage_objects as O      # noqa: E402  read only: the grids
import stages as S             # noqa: E402  read only: the stage files

BASE = 0x801e8000              # where the stage module is linked
STEP = 12
REC = 16                       # an arcade record of a stage object (20 with a jump)
JUMP = 0x8000
HOLD = 4095
ART_LO, ART_HI = Q.ART_LO, Q.ART_HI


# ---------------------------------------------------------------- pure functions (tested without game data)

def valid_record(prog, a):
    if a < 0 or a & 1 or a + REC > len(prog):
        return False
    dur, fl, art = struct.unpack_from('>HHI', prog, a)
    return 1 <= dur <= HOLD and not art & 1 and ART_LO <= art < ART_HI and (not fl & JUMP or a + REC + 4 <= len(prog))


def record_at(prog, a):
    """(record, next address): record = dict(a, dur, fl, art, tgt)."""
    dur, fl, art = struct.unpack_from('>HHI', prog, a)
    tgt = struct.unpack_from('>I', prog, a + REC)[0] if fl & JUMP else None
    return dict(a=a, dur=dur, fl=fl, art=art, tgt=tgt), a + REC + (4 if fl & JUMP else 0)


def walk_records(prog, a, limit=4000):
    out = []
    while len(out) < limit and valid_record(prog, a):
        rec, a = record_at(prog, a)
        out.append(rec)
    return out


def step_runs(mod, grid_addrs):
    """[[step dict]]: runs of consecutive 12-byte steps (2-aligned) whose pointer is a grid address and whose countdown and frame index are plausible."""
    steps = []
    for i in range(0, len(mod) - STEP + 1, 2):
        dur, fl, ptr, lo, fi = struct.unpack_from('<hhIhH', mod, i)
        if ptr in grid_addrs and 0 <= dur <= HOLD and fi == 0:
            steps.append(dict(o=i, dur=dur, fl=fl & 0xffff, dp=ptr, lo=lo))
    runs, cur = [], []
    for s in steps:
        if cur and s['o'] == cur[-1]['o'] + STEP:
            cur.append(s)
        else:
            if cur:
                runs.append(cur)
            cur = [s]
    if cur:
        runs.append(cur)
    return runs


def pack(rec, grid, loop):
    """The 12 bytes of the step of an arcade record (a dict as record_at makes) that points at the grid with module address `grid`."""
    return struct.pack('<hhIhH', rec['dur'], rec['fl'] - 65536 if rec['fl'] >= 32768 else rec['fl'], grid, loop, 0)


def build_steps(prog, runs, art_grid):
    """{step module offset: 12 bytes} for the rows `runs` [(step offset, record address, count)]: the step k of a run is the k-th record from its address.
    art_grid maps an art record address to the grid's module offset. A jump whose target is no record of a run, or an art with no grid, is an error
    (ValueError)."""
    where, recs = {}, []
    for off, addr, count in runs:
        a = addr
        for k in range(count):
            if not valid_record(prog, a):
                raise ValueError(f'no record at {a:#x}')
            rec, nxt = record_at(prog, a)
            where[a] = off + STEP * k
            recs.append((off + STEP * k, rec))
            a = nxt
    out = {}
    for o, rec in recs:
        grid = art_grid.get(rec['art'])
        if grid is None:
            raise ValueError(f'no grid for the art record {rec["art"]:#x}')
        loop = 0
        if rec['fl'] & JUMP:
            if rec['tgt'] not in where:
                raise ValueError(f'the jump target {rec["tgt"]:#x} is in no run')
            loop = (where[rec['tgt']] - o) // STEP
        out[o] = pack(rec, BASE + grid, loop)
    return out


# ---------------------------------------------------------------- finding the pairs on the disc

def find_anchor(prog, run, cands):
    """(k, record address) of the first step of the run with a known art record whose record has its countdown and flags, or None. cands: grid address ->
    [art record addresses]."""
    for k, s in enumerate(run):
        for art in cands.get(s['dp'], ()):
            pat = struct.pack('>I', art)
            h = prog.find(pat)
            while h >= 0:
                r = h - 4
                if r >= 0 and not r & 1 and struct.unpack_from('>HH', prog, r) == (s['dur'], s['fl']):
                    return k, r
                h = prog.find(pat, h + 1)
    return None


def walk_back(prog, a, n):
    """The address n records before the one at a (a record that ends exactly at a), as far as that is unambiguous."""
    for _ in range(n):
        for size in (REC, REC + 4):
            p = a - size
            if valid_record(prog, p):
                rec, nxt = record_at(prog, p)
                if nxt == a:
                    a = p
                    break
        else:
            break
    return a


def pair_run(prog, run, amap):
    """[(arc, [(i, j)])] for one run of steps: arc = the arcade records walked from the anchor, j indexes into run (offset by the part paired)."""
    cands = collections.defaultdict(list)
    for art, g in amap.items():
        cands[g].append(art)
    out, done = [], 0
    while done < len(run):
        rest = run[done:]
        anchor = find_anchor(prog, rest, cands)
        if not anchor:
            break
        k, r = anchor
        arc = walk_records(prog, walk_back(prog, r, k))[:len(rest) + k + 40]
        prs = Q.pair_records(arc, [dict(dur=s['dur'], fl=s['fl'], dp=s['dp']) for s in rest], amap) if arc else []
        if not prs:
            done += k + 1
            continue
        out.append((arc, [(i, done + j) for i, j in prs]))
        done += max(j for _i, j in prs) + 1
    return out


def cell_map(mod, found, keys, blank, rec_key, index):
    """{art record address: grid address} for the grids whose cells and palette row an art record has (the first grid in the module when several do)."""
    amap = {}
    for at, cols, _nrows, cells in found:
        _lo, _hi, hits, _why = O.grid_hits(mod, at, cols, cells, keys, blank, rec_key)
        for a, _c0 in hits or ():
            rec = index.record(a)
            if rec and mod[at + 3] in {c[3] & 31 for c in rec[1]}:
                amap.setdefault(a, BASE + at)
    return amap


def analyse_stage(prog, mod, amap, found_addrs):
    """The table row for one module: dict(runs, arts, counts). amap: art record -> grid address, from the cells (cell_map)."""
    counts = collections.Counter()
    sruns = step_runs(mod, found_addrs)
    counts['steps'] = sum(len(r) for r in sruns)
    pairs = []                                                     # (record, step) for every pairing, consecutive records of one walk
    known = dict(amap)
    for _round in range(2):                                        # the second round pairs with what the first round learned of the grids
        pairs = []
        for run in sruns:
            for arc, prs in pair_run(prog, run, known):
                pairs += [(arc, i, run[j]) for i, j in prs]
        learned = collections.defaultdict(collections.Counter)
        for arc, i, step in pairs:
            learned[arc[i]['art']][step['dp']] += 1
        known = dict(amap)
        for art, c in learned.items():
            known.setdefault(art, c.most_common(1)[0][0])
    counts['paired'] = len(pairs)
    default = {art: max(c, key=lambda g: (c[g], g == amap.get(art))) for art, c in learned.items()}
    where = {arc[i]['a']: step['o'] for arc, i, step in pairs}
    kept = []
    for arc, i, step in pairs:
        rec = arc[i]
        loop = 0
        if rec['fl'] & JUMP:
            if rec['tgt'] not in where:
                counts['jump target unknown'] += 1
                continue
            loop = (where[rec['tgt']] - step['o']) // STEP
        if step['dp'] != default[rec['art']]:
            counts['grid not the usual one'] += 1
            continue
        want = pack(rec, step['dp'], loop)
        if want != bytes(mod[step['o']:step['o'] + STEP]):
            for name, ok in (('countdown', rec['dur'] == step['dur']), ('flags', rec['fl'] == step['fl']), ('loop', loop == step['lo'])):
                if not ok:
                    counts['differs in ' + name] += 1
            continue
        kept.append((arc, i, step))
    while True:                                                    # a jump must lead to a step that is built too (the C finds the target in the runs)
        built = {arc[i]['a'] for arc, i, _s in kept}
        keep = [t for t in kept if not t[0][t[1]]['fl'] & JUMP or t[0][t[1]]['tgt'] in built]
        counts['jump target not built'] += len(kept) - len(keep)
        if len(keep) == len(kept):
            break
        kept = keep
    # runs: consecutive kept steps of consecutive records
    rows, last = [], None
    for arc, i, step in sorted(kept, key=lambda t: t[2]['o']):
        a = arc[i]['a']
        if rows and last == (arc[i - 1]['a'] if i else None) and rows[-1][0] + STEP * rows[-1][2] == step['o']:
            rows[-1][2] += 1
        else:
            rows.append([step['o'], a, 1])
        last = a
    arts = sorted({(arc[i]['art'], default[arc[i]['art']] - BASE) for arc, i, _s in kept})
    counts['equal'] = len(kept)
    return dict(runs=[tuple(r) for r in rows], arts=arts, counts=counts, learned=len(set(known) - set(amap)))


# ---------------------------------------------------------------- the table

def c_table(rows_by_file):
    out = ['/* GENERATED by `python3 port/arcade/stage_steps.py --disc DISC --arcade SET --c-table FILE`; do not edit.',
           ' * For each stage file: the runs of animation steps of its module that the arcade\'s sequence records build equal to the disc\'s (the module offset of the first',
           ' * step, the address of its record in the arcade program, the count), the grid each art record became (art record address, grid offset).',
           ' * Only offsets, addresses and counts: no byte of either game. */',
           '#include "stage_steps.h"', '']
    for file, row in rows_by_file:
        tag = file[5:7].lower()
        if not row['runs']:
            continue
        for name, kind, items, fmt in (('runs', 'arcade_stage_step_run', row['runs'], '{{{0}u, {1:#x}u, {2}u}}'),
                                       ('arts', 'arcade_stage_step_art', row['arts'], '{{{0:#x}u, {1}u}}')):
            if items:
                out.append(f'static const struct {kind} {name}_{tag}[] = {{')
                out += ['    ' + fmt.format(*it) + ',' for it in items]
                out += ['};', '']
    out.append('const struct arcade_stage_steps arcade_stage_steps[] = {')
    for file, row in rows_by_file:
        tag = file[5:7].lower()
        cells = []
        for name in ('runs', 'arts'):
            n = len(row[name]) if row['runs'] else 0
            cells.append(f'{name}_{tag}, {n}u' if n else '0, 0u')
        out.append(f'    {{"{file}", {", ".join(cells)}}},')
    out += ['};', 'const unsigned arcade_stage_steps_count = sizeof arcade_stage_steps / sizeof arcade_stage_steps[0];', '']
    return '\n'.join(out)


def stage_row(prog, mod, parts_runs, shared_runs, ctx):
    """The table row (analyse_stage's) of one stage module. ctx is stage_objects.context's tuple (view, index, blank, rec_key)."""
    _view, index, blank, rec_key = ctx
    keys = O.tile_keys(parts_runs, shared_runs)
    found = O.find_grids(mod, len(keys))
    amap = cell_map(mod, found, keys, blank, rec_key, index)
    return analyse_stage(prog, mod, amap, {BASE + at for at, _c, _r, _cells in found})


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True)
    ap.add_argument('--arcade', required=True)
    ap.add_argument('--only')
    ap.add_argument('--c-table', metavar='FILE')
    args = ap.parse_args(argv)
    started = time.monotonic()
    prog, gfx, *ctx = O.load_arcade(args.arcade)
    tmp = tempfile.mkdtemp(prefix='stage-steps-')
    try:
        helper = S.Helper(tmp, gfx, prog)
        with open(args.disc, 'rb') as f:
            files, sec = S.iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/STAGE') and p.endswith('.PAC'))
            datas = {}
            for p in names:
                lba, size = files[p]
                datas[p.split('/')[-1]] = sec(lba, (size + 2047) // 2048)[:size]
        first = S.split_stage(next(iter(datas.values())))
        shared_runs = S.tile_runs(first['effect tiles'], helper.tiles(first['effect tiles'])[2])
        total, table = collections.Counter(), []
        for name, data in datas.items():
            if args.only and not name.endswith('STAGE' + args.only.upper() + '.PAC'):
                continue
            parts = S.split_stage(data)
            runs = S.tile_runs(parts['object tiles'], helper.tiles(parts['object tiles'])[2]) if 'object tiles' in parts else []
            row = stage_row(prog, parts['module'], runs, shared_runs, tuple(ctx))
            built = build_steps(prog, row['runs'], dict(row['arts']))
            mod = parts['module']
            assert all(mod[o:o + STEP] == b for o, b in built.items()), name
            row['counts']['bytes'] = STEP * len(built)
            table.append((name, row))
            total.update(row['counts'])
            print(f'{name}: runs {len(row["runs"])}, arts {len(row["arts"])}, learned {row["learned"]}, {dict(row["counts"])}')
        print(dict(total))
        if args.c_table:
            Path(args.c_table).write_text(c_table(table))
            print(f'wrote {args.c_table}')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    print(f'done in {time.monotonic() - started:.1f} s')
    return 0


if __name__ == '__main__':
    sys.exit(main())
