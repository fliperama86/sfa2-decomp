#!/usr/bin/env python3
"""Analysis tool: rebuild each fighter's PS1 animation steps and frame records from the arcade's sequence records.

What it does. A fighter's PS1 module holds, after the cell lists, a run of 12-byte animation steps
(duration, flags, data pointer, loop offset, frame index; `struct SequenceStep` in ps1/src/types.fields) and a
table of 16-byte frame records (`struct FrameRecord`) that the steps point at through the frame index. Both are a
mechanical conversion of the arcade's sequence records. For every fighter file on the disc (PLnn.PAC and
PLnnX.PAC) this tool finds the step run in the module, finds the arcade's run of records, pairs the two, runs the
conversion rules below and prints one line per file: how many steps, how many are paired, how many of the paired
steps and of the frame records the rules rebuild to the byte, and where the PS1 differs from the arcade.

The arcade side. A record is 24 bytes: a big-endian 16-bit countdown, a 16-bit flags word, a 32-bit pointer to the
art record (the one that fighters.py converts to the PS1 cell list), then 16 bytes of frame data. When bit 15 of
the flags is set, 4 more bytes follow: the absolute address of the record the sequence jumps to (a loop). The next
record follows directly. (A few records in some fighters have the 4 bytes with bit 15 clear; the tool takes them as
a jump too, and counts them.) The records of a fighter lie in one run, in front of its art records, after the
tables of 16-bit displacements that select a sequence.

The rules (arcade record i, paired with PS1 step j):
  - duration = the countdown; flags = the flags word (the PS1 keeps bit 15 as "jump" and the rest as they are);
  - data pointer = the byte offset of the art record's cell list from the list base (fighters.py finds it);
  - loop offset = when the jump flag is set, the PS1 step index of the target record minus j (0 otherwise);
  - frame record = the arcade's 16 frame bytes with each group of 4 bytes reversed (a 32-bit byte swap);
  - frame index = 0 for an all-zero frame, else the frames are numbered from 1 in the order of first use down the
    step run, and a frame seen before keeps its number.
Beyond the rules the tool reports, and does not invent rules for: steps with no arcade record (the PS1's own),
arcade records the PS1 left out (reductions), and paired steps that differ in one field (the PS1's changes).

Inputs: the disc image (raw 2352-byte sectors) and the arcade set's ZIP (MAME's sfa2u), both given as arguments and
both read only. It writes nothing except a temporary folder (fighters.py compiles a small C helper there) that it
removes. It prints sizes, addresses and counts, never game bytes. It is an analysis and reference tool; the
converter that the port runs will be written in C.

    python3 port/arcade/sequences.py --disc BIN --arcade ZIP [--only NAME] [--detail]
"""
import argparse
import collections
import difflib
import shutil
import struct
import sys
import tempfile
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fighters as F   # noqa: E402  read-only: the art records, the lists, the arcade views

STEP = 12
FRAME = 16
REC = 24                  # an arcade record without a jump
JUMP = 0x8000
HOLD = 4095               # the longest countdown: a hold
ART_LO, ART_HI = 0x100000, 0x400000   # where the arcade's art records lie (a plausibility bound for a pointer)
LOAD = {1: 0x801b0000, 2: 0x801c8000}   # where the first-side and second-side modules are linked (ps1/docs/overlays.md)
MIN_RUN = 50              # steps in a row that make a step run
SCAN = 0x4000             # how far after the lists the step run is looked for


# ---------------------------------------------------------------- pure functions (tested without game data)

def convert_frame(fr):
    """The PS1 frame record of the arcade's 16 frame bytes: each group of 4 bytes reversed."""
    return b''.join(fr[i:i + 4][::-1] for i in range(0, len(fr) - len(fr) % 4, 4))


def pack_step(dur, flags, ptr, loop, frame):
    return struct.pack('<hhIhH', dur, flags - 65536 if flags >= 32768 else flags, ptr, loop, frame)


def step_at(mod, o):
    dur, fl, ptr, loop, fi = struct.unpack_from('<hhIhH', mod, o)
    return dict(o=o, dur=dur, fl=fl & 0xffff, dp=ptr & ~1, odd=ptr & 1, lo=loop, fi=fi)


def valid_step(mod, o, list_size):
    """Whether the 12 bytes at o can be a step: a countdown up to 4095, a pointer inside the lists, a small loop."""
    if o < 0 or o + STEP > len(mod):
        return False
    dur, _fl, ptr, loop, fi = struct.unpack_from('<hhIhH', mod, o)
    return 0 <= dur <= HOLD and ptr < list_size and fi < 2048 and abs(loop) < 2048


def find_steps(mod, lo, list_size, min_run=MIN_RUN, scan=SCAN):
    """(start, steps) of the first run of at least min_run steps that starts at a 4-aligned offset from lo on."""
    for s in range(lo & ~3, min(len(mod), lo + scan), 4):
        if not valid_step(mod, s, list_size):
            continue
        o = s
        while valid_step(mod, o, list_size):
            o += STEP
        if (o - s) // STEP >= min_run:
            return s, [step_at(mod, s + STEP * k) for k in range((o - s) // STEP)]
    return None, []


def valid_record(prog, a):
    """Whether an arcade record can start at a: an even address, a countdown 1..4095, an even art pointer in range."""
    if a < 0 or a & 1 or a + REC > len(prog):
        return False
    dur, _fl, art = struct.unpack_from('>HHI', prog, a)
    return 1 <= dur <= HOLD and not art & 1 and ART_LO <= art < ART_HI


def record_at(prog, a):
    """(record, next address) of the record at a; the record's 'quirk' is set when a jump has bit 15 clear."""
    dur, fl, art = struct.unpack_from('>HHI', prog, a)
    size, quirk = REC, False
    if fl & JUMP:
        size = REC + 4
    elif not valid_record(prog, a + REC) and valid_record(prog, a + REC + 4):
        size, quirk = REC + 4, True
    tgt = struct.unpack_from('>I', prog, a + REC)[0] if size > REC and a + size <= len(prog) else None
    return dict(a=a, dur=dur, fl=fl | (JUMP if quirk else 0), art=art, tgt=tgt, fr=prog[a + 8:a + 24], quirk=quirk), a + size


def walk_records(prog, a, limit=4000):
    out = []
    while len(out) < limit and valid_record(prog, a):
        rec, a = record_at(prog, a)
        out.append(rec)
    return out


def extend_back(prog, a, limit=400):
    """The address of the first record of the run that holds the record at a (records are walked back from a)."""
    for _ in range(limit):
        for size in (REC, REC + 4):
            p = a - size
            if valid_record(prog, p):
                rec, nxt = record_at(prog, p)
                if nxt == a:
                    a = p
                    break
        else:
            return a
    return a


def find_run(prog, steps, inverse):
    """Address of the arcade record the first steps with a known art record come from, or None.

    inverse maps a cell list offset to the arcade address of its art record."""
    for q in steps[:12]:
        art = inverse.get(q['dp'])
        if art is None:
            continue
        pat = struct.pack('>I', art)
        h = prog.find(pat)
        while h >= 0:
            a = h - 4
            if a >= 0 and not a & 1 and struct.unpack_from('>HH', prog, a) == (q['dur'], q['fl'] & 0xffff):
                return a
            h = prog.find(pat, h + 1)
    return None


def _match_blocks(ka, kb):
    return difflib.SequenceMatcher(None, ka, kb, autojunk=False).get_opcodes()


def pair_records(arc, steps, amap, frame_of=None):
    """[(i, j)]: arcade record i belongs to PS1 step j.

    First by (countdown, flags, list offset) over the whole run. Then, in each gap between matched blocks, by the
    list offset alone (the PS1 changed a countdown or flags), then, when frame_of(j) gives the PS1's frame bytes
    of step j, by (countdown, converted frame bytes), and last, in a gap of the same size on both sides, in order,
    when no pair disagrees in a list offset that both know. A step the PS1 gave another list than the arcade's
    record has is not paired: it is the PS1's own."""
    pairs = []
    unique = iter(range(-1, -10 ** 9, -1))
    # a key function per tier: (arcade record) -> key, (step index) -> key; a key never seen on the other side is harmless
    tiers = [(lambda i: amap.get(arc[i]['art'], next(unique)), lambda j: steps[j]['dp'])]
    if frame_of:
        tiers.append((lambda i: (arc[i]['dur'], convert_frame(arc[i]['fr'])),
                      lambda j: (steps[j]['dur'], frame_of(j) or next(unique))))

    def gap(i1, i2, j1, j2, t):
        if i1 >= i2 or j1 >= j2:
            return
        if t == len(tiers):
            if i2 - i1 == j2 - j1 and all(amap.get(arc[i]['art']) in (None, steps[j]['dp']) for i, j in zip(range(i1, i2), range(j1, j2))):
                pairs.extend(zip(range(i1, i2), range(j1, j2)))
            return
        ka = [tiers[t][0](i) for i in range(i1, i2)]
        kb = [tiers[t][1](j) for j in range(j1, j2)]
        for tag, a1, a2, b1, b2 in _match_blocks(ka, kb):
            if tag == 'equal':
                pairs.extend(zip(range(i1 + a1, i1 + a2), range(j1 + b1, j1 + b2)))
            else:
                gap(i1 + a1, i1 + a2, j1 + b1, j1 + b2, t + 1)

    ka = [(a['dur'], a['fl'], amap.get(a['art'])) for a in arc]
    kb = [(p['dur'], p['fl'], p['dp']) for p in steps]
    for tag, a1, a2, b1, b2 in _match_blocks(ka, kb):
        if tag == 'equal':
            pairs.extend(zip(range(a1, a2), range(b1, b2)))
        else:
            gap(a1, a2, b1, b2, 0)
    pairs.sort()
    return pairs


def frame_numbers(frames, actual, own=()):
    """Expected frame index of each step, given the frame bytes of each step (None where unknown) and the actual indices.

    0 for an all-zero frame; a frame seen before has the index of its first use; a new one gets one more than the
    highest index so far. The actual indices are followed, so one difference does not carry on. Steps in `own` (the
    PS1's own, whose frames the PS1 numbered its own way) are not given an expected index and do not move the count."""
    seen, top, out = {}, 0, []
    for k, (fb, got) in enumerate(zip(frames, actual)):
        if fb is None or k in own:
            out.append(None)
            continue
        if not any(fb):
            out.append(0)
        elif fb in seen:
            out.append(seen[fb])
        else:
            out.append(top + 1)
        if any(fb) and fb not in seen:
            seen[fb] = got
        top = max(top, got)
    return out


def scale_flags(fl):
    """The flags some fighters' PS1 steps have for an arcade flags word whose low byte counts in units of 0x18.

    The low byte divided by 0x18, bits 8 and 15 kept; None when the low byte is not a multiple of 0x18 (or zero)."""
    low = fl & 0xff
    if not low or low % 0x18:
        return None
    return (fl & 0x8100) | low // 0x18


def run_length(prog, recs, p, stop):
    """Entries of the table at p (16-bit displacements from p) that point at a record, up to stop."""
    n, o = 0, p
    while o + 2 <= stop and (p + struct.unpack_from('>h', prog, o)[0]) in recs:
        n += 1
        o += 2
    return n


def find_tables(prog, recs, first, end, back=0x900, min_run=1):
    """The arcade's sequence tables around the run of records: [(start, [record addresses])], by address.

    A table is a list of 16-bit displacements from its own start to a record, ended by a zero word; the tables lie
    one after another, either right before the first record or right after the last."""
    out = []
    for p in range(max(0, first - back), first, 2):
        q, tabs = p, []
        while q < first:
            n = run_length(prog, recs, q, first)
            if n < min_run:
                break
            tabs.append((q, n))
            q += 2 * (n + 1)
        if tabs and q == first:
            out = tabs
            break
    q = end
    while q + 2 <= len(prog):
        n = run_length(prog, recs, q, len(prog))
        if n < min_run:
            break
        out.append((q, n))
        q += 2 * (n + 1)
    return [(q, [q + struct.unpack_from('>h', prog, q + 2 * k)[0] for k in range(n)]) for q, n in out]


def ps1_table_entries(mod, s0, n, load):
    """The step indices of the pointer table after the n steps that start at s0, for a module linked at load."""
    out, o = [], s0 + STEP * n
    while o + 4 <= len(mod):
        v = struct.unpack_from('<I', mod, o)[0] - load - s0
        if not 0 <= v < STEP * n or v % STEP:
            break
        out.append(v // STEP)
        o += 4
    return out


def find_frame_base(mod, wanted):
    """Offset of the frame table: the offset that most (index, frame bytes) pairs agree on, as (base, votes), or None.

    wanted is {frame index: converted bytes}."""
    votes = collections.Counter()
    for fi, fb in wanted.items():
        if fi == 0 or not any(fb):
            continue
        k = mod.find(fb)
        while k >= 0:
            if not k & 3 and k - FRAME * fi > 0:
                votes[k - FRAME * fi] += 1
            k = mod.find(fb, k + 1)
    return votes.most_common(1)[0] if votes else None


def ranges(idx):
    """'a-b c d-e' from a sorted list of integers."""
    out, k = [], 0
    while k < len(idx):
        m = k
        while m + 1 < len(idx) and idx[m + 1] == idx[m] + 1:
            m += 1
        out.append(str(idx[k]) if m == k else f'{idx[k]}-{idx[m]}')
        k = m + 1
    return out


# ---------------------------------------------------------------- the analysis of one file

def analyse(mod, r, prog, view, load=0x801b0000, min_run=MIN_RUN):
    """Everything the line and the detail need, as a dict. The key 'error' when the file cannot be read.

    r is fighters.analyse's result for the module (lists, list base and end, segments)."""
    amap = list_offsets(r, view)
    inverse = {v: k for k, v in amap.items()}
    lsize = r['listsend'] - r['lbase']
    start, steps = find_steps(mod, r['listsend'], lsize, min_run)
    if not steps:
        return dict(error='no step run after the lists')
    a0 = find_run(prog, steps, inverse)
    if a0 is None:
        return dict(error='no arcade record for the first steps')
    a0 = extend_back(prog, a0)
    arc = walk_records(prog, a0)
    pairs = pair_records(arc, steps, amap)
    # the frame table, from the frames of the paired steps; then the pairing again, with the PS1's frames as a key
    wanted = {}
    for i, j in pairs:
        wanted.setdefault(steps[j]['fi'], convert_frame(arc[i]['fr']))
    fb = find_frame_base(mod, wanted)
    base = fb[0] if fb else None
    if base is not None:
        def frame_of(j):
            o = base + FRAME * steps[j]['fi']
            return mod[o:o + FRAME] if o + FRAME <= len(mod) else None
        pairs = pair_records(arc, steps, amap, frame_of)
    pj = {j: i for i, j in pairs}
    pi = dict(pairs)
    nframes = max(p['fi'] for p in steps) + 1
    frames = []
    for j, p in enumerate(steps):
        if j in pj:
            frames.append(convert_frame(arc[pj[j]]['fr']))
        elif base is not None and base + FRAME * (p['fi'] + 1) <= len(mod):
            frames.append(mod[base + FRAME * p['fi']:base + FRAME * (p['fi'] + 1)])
        else:
            frames.append(None)
    expect_fi = frame_numbers(frames, [p['fi'] for p in steps], set(range(len(steps))) - set(pj))
    # the steps
    c = collections.Counter()
    at = {x['a']: k for k, x in enumerate(arc)}
    first = {}
    exact_steps, scaled_steps = set(), set()
    for i, j in pairs:
        a, p = arc[i], steps[j]
        got = []
        if a['dur'] != p['dur']:
            got.append('dur')
        scaled = False
        if a['fl'] != p['fl']:
            if scale_flags(a['fl']) == p['fl']:
                scaled = True
            else:
                got.append('flags')
        want_dp = amap.get(a['art'])
        decided = True
        want_lo = 0
        if want_dp is None:
            c['dp unknown'] += 1
            decided = False
        elif want_dp != p['dp']:
            got.append('list')
        if a['fl'] & JUMP:
            ti = at.get(a['tgt'])
            if ti is None or ti not in pi:
                c['loop target unpaired'] += 1
                decided = False
            else:
                want_lo = pi[ti] - j
                if want_lo != p['lo']:
                    got.append('loop')
        elif p['lo']:
            got.append('loop')
        if expect_fi[j] is not None and expect_fi[j] != p['fi']:
            got.append('frame index')
        for g in got:
            c[g] += 1
            first.setdefault(g, (i, j))
        # the rules build the 12 bytes; a step is exact when they equal the module's
        if decided and expect_fi[j] is not None:
            want = pack_step(a['dur'], scale_flags(a['fl']) if scaled else a['fl'], want_dp, want_lo, expect_fi[j])
            if want == mod[p['o']:p['o'] + STEP]:
                (scaled_steps if scaled else exact_steps).add(j)
    # the frame records
    ref = {}
    for i, j in pairs:
        ref.setdefault(steps[j]['fi'], set()).add(convert_frame(arc[i]['fr']))
    fr_exact = fr_diff = 0
    if base is not None:
        for fi, variants in ref.items():
            if base + FRAME * (fi + 1) > len(mod):
                continue
            if len(variants) == 1 and mod[base + FRAME * fi:base + FRAME * (fi + 1)] == next(iter(variants)):
                fr_exact += 1
            else:
                fr_diff += 1
    # the sequence tables
    end = arc[-1]['a'] + REC + (4 if arc[-1]['tgt'] is not None else 0)
    tabs = find_tables(prog, {x['a'] for x in arc}, a0, end)
    flat = [pi.get(at[a], ('arcade only', at[a])) for _q, ents in tabs for a in ents]
    mine = ps1_table_entries(mod, start, len(steps), load)
    tab_match = sum(b.size for b in difflib.SequenceMatcher(None, flat, mine, autojunk=False).get_matching_blocks())
    only_arc = [i for i in range(len(arc)) if i not in pi]
    only_ps1 = [j for j in range(len(steps)) if j not in pj]
    return dict(start=start, steps=steps, a0=a0, arc=arc, pairs=pairs, exact=len(exact_steps), scaled=len(scaled_steps), counts=c, first=first,
                base=base, nframes=nframes, fr_exact=fr_exact, fr_diff=fr_diff, fr_own=nframes - len(set(ref) | {0}),
                tables=[len(e) for _q, e in tabs], table_arcade=len(flat), table_ps1=len(mine), table_match=tab_match,
                only_arc=only_arc, only_ps1=only_ps1, quirks=sum(1 for a in arc if a['quirk']),
                odd=sum(1 for p in steps if p['odd']))


def read_any(view, a):
    """The art record at a, of mode 2 or (when fighters.py reads those) of mode 0: (cells, cells, end address), or None."""
    return getattr(F, 'read_any', F.read_record)(view, a)


def list_offsets(r, view):
    """{arcade address of an art record: cell list offset from the list base} for the lists fighters.py converted."""
    lists, out = r['lists'], {}
    for a, count, k, _bank in r['segments']:
        placed = 0
        j = k
        while placed < count and j < len(lists):
            if F.is_empty(lists[j]):
                j += 1
                continue
            out[a] = lists[j]['at'] - r['lbase']
            rec = read_any(view, a)
            if rec is None:
                break
            a = rec[2]
            j += 1
            placed += 1
    return out


def line(name, r):
    if 'error' in r:
        return f'{name}: {r["error"]}'
    c, n = r['counts'], len(r['steps'])
    diffs = ', '.join(f'{k} {v}' for k, v in c.most_common() if k in ('dur', 'flags', 'list', 'loop', 'frame index'))
    unknown = ', '.join(f'{k} {v}' for k, v in c.most_common() if k in ('dp unknown', 'loop target unpaired'))
    s = (f'{name}: steps {n} at +{r["start"]:#x} ({STEP * n} B), arcade run {len(r["arc"])} at {r["a0"]:#x}; paired {len(r["pairs"])}, '
         f'exact {r["exact"]} ({STEP * r["exact"]} B)')
    if r['scaled']:
        s += f' + {r["scaled"]} with scaled flags'
    if diffs:
        s += f'; differ: {diffs}'
    if unknown:
        s += f'; not decidable: {unknown}'
    s += f'; PS1 only {len(r["only_ps1"])}, arcade only {len(r["only_arc"])}'
    if r['base'] is None:
        s += '; frame table not found'
    else:
        s += (f'; frames {r["nframes"]} at +{r["base"]:#x}: exact {r["fr_exact"]} ({FRAME * r["fr_exact"]} B), differ {r["fr_diff"]}, '
              f'PS1 only {r["fr_own"]}')
    s += f'; tables: arcade {r["table_arcade"]} entries in {len(r["tables"])}, PS1 {r["table_ps1"]}, equal in order {r["table_match"]}'
    if r['quirks'] or r['odd']:
        s += f'; jumps without bit 15 {r["quirks"]}, odd pointers {r["odd"]}'
    return s


def details(name, r):
    """A few short lines: the runs of PS1-only steps and arcade-only records, and the first step of each kind of difference."""
    if 'error' in r:
        return []
    out = []
    for label, idx in (('PS1-only steps', r['only_ps1']), ('arcade-only records', r['only_arc'])):
        if idx:
            rs = ranges(idx)
            out.append(f'  {name}: {label} {len(idx)} in {len(rs)} runs: ' + ' '.join(rs[:6]) + (' ...' if len(rs) > 6 else ''))
    for kind, (i, j) in list(r['first'].items())[:5]:
        a, p = r['arc'][i], r['steps'][j]
        out.append(f'  {name}: first {kind} difference: arcade record {i} ({a["a"]:#x}) step {j}: arcade dur {a["dur"]} flags {a["fl"]:#x}, '
                   f'PS1 dur {p["dur"]} flags {p["fl"]:#x} list {p["dp"]} loop {p["lo"]} frame {p["fi"]}')
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00.PAC or PL00)')
    ap.add_argument('--detail', action='store_true', help='extra lines per file: the runs only one side has, the first difference of each kind')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='sequences-')
    try:
        prog_be, gfx = F.arcade_views(args.arcade)
        view = F.View(prog_be)
        index = F.Index(view)
        helper = F.Helper(tmp, gfx)
        tot = collections.Counter()
        with open(args.disc, 'rb') as f:
            files, sec = F.iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
            if args.only:
                want = args.only.upper().removesuffix('.PAC') + '.PAC'
                names = [p for p in names if p.endswith('/' + want)]
            for p in names:
                lba, size = files[p]
                data = sec(lba, (size + 2047) // 2048)[:size]
                name = p.split('/')[-1]
                try:
                    parts = F.split_file(data)
                except (F.pac.FormatError, struct.error) as exc:
                    print(f'{name}: {exc}')
                    continue
                if isinstance(parts, str):
                    print(f'{name}: {parts}')
                    continue
                mod, tiles, _side, others = parts
                # fighters.py may want to know which files order their parts by palette (PALETTE_PARTS, when it has it)
                kw = dict(palette_parts=name.removesuffix('.PAC').removesuffix('X') in F.PALETTE_PARTS) if hasattr(F, 'PALETTE_PARTS') else {}
                fr = F.analyse(mod, tiles, view, index, helper, others, **kw)
                if 'error' in fr:
                    print(f'{name}: {fr["error"]}')
                    continue
                r = analyse(mod, fr, prog_be, view, LOAD[_side])
                print(line(name, r))
                if 'error' not in r:
                    tot['steps'] += len(r['steps'])
                    tot['exact'] += r['exact']
                    tot['scaled'] += r['scaled']
                    tot['tables'] += r['table_ps1']
                    tot['tables equal'] += r['table_match']
                    tot['frames'] += r['nframes']
                    tot['frames exact'] += r['fr_exact']
                if args.detail:
                    for d in details(name, r):
                        print(d)
        if tot['steps']:
            print(f'all: steps {tot["steps"]} ({STEP * tot["steps"]} B), exact {tot["exact"]} ({STEP * tot["exact"]} B) '
                  f'+ {tot["scaled"]} with scaled flags ({STEP * tot["scaled"]} B); '
                  f'frames {tot["frames"]} ({FRAME * tot["frames"]} B), exact {tot["frames exact"]} ({FRAME * tot["frames exact"]} B); '
                  f'table entries {tot["tables"]}, equal in order {tot["tables equal"]}')
        print(f'{len(names)} files in {time.monotonic() - started:.1f} s')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
