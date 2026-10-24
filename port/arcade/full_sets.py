#!/usr/bin/env python3
"""Analysis tool: full animation sets. Derives the table that full_sets.c reads, from the disc, and checks the C against it.

What it does. On the PS1, a fighter's module against another fighter holds a cut animation set (Zangief's PL10.PAC and PL10X.PAC: 890 steps,
272 cell lists); the mirror module PL16.PAC (both players Zangief) holds a fuller one (993 steps, 376 lists) and runs the same code. The
runtime writes PL16's set into PL10's and PL10X's module chunk (full_sets.c; the contract is full_sets.h). This tool finds, for each pair in
TARGETS, every number that code needs, from the bytes of the disc and the other generated tables, and fails loudly where the data is not what
the rules below expect:

  Blocks. Each module's blocks are found by their structure (the chain of cell lists after the code, the position runs in front of it, the
          step run after the lists, the pointer table after the steps and the frame records after that), as module_data.py does. The source's
          blocks are checked against port/arcade/fighters_table.c's and sequences_table.c's rows of the source (they must agree).
  Frames. The frame records of the target and of the source are byte for byte the same, and so is every word of the data after them, except:
          the module addresses (which differ by where each module's blocks lie), 16 zero bytes more in the source, and the words below.
  Table.  The target's step table entry i is the source's entry i + d. Each target sequence (its steps from the entry up to the step with flags
          bit 15, at most 200) is, for the mapped source sequence, the same in duration, flags and frame field ("same"), or its frames (the
          non-jump steps' frame numbers) are a subsequence of the source's ("sub": the source has steps more), or it is one of the EXCEPTIONS (a
          fuller variant of the same sequence that is neither: the sequence differs in a step or two, as the owner established). The offset d is
          the one that gives the fewest runs (dynamic programming over the entries, preferring "same"); the runs are the stretches of constant
          d. Every entry must map; the exceptions must be exactly the entries that do not map by content; two entries never share a source entry.
  Outside steps. 11 steps of the target (and of the source) lie in the data after the frames, outside the step block: the only words there
          that differ and are not module addresses. Each is the list offset (word 1) of a 12-byte step whose other words are equal, and a
          list offset of the module's own lists; found by a word-aligned diff of the two regions.
  Place. The source's position runs go at the target's position runs (where its code reads them), the source's steps right after them,
          rounded up to 4, and must end before the target's step table.

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade, accepted for the same command line as the folder's
other tools; this tool does not read it). The repository's two generated tables (fighters_table.c, sequences_table.c) are read. It writes
nothing but what --c-table names and a temporary folder (--compare-c), and prints numbers, never game bytes. Seconds.

    python3 port/arcade/full_sets.py --disc BIN --arcade ZIP [--c-table FILE | --compare-c]

--c-table FILE writes port/arcade/full_sets_table.c (the rows of TARGETS; numbers only). --compare-c compiles full_sets.c, the checked-in
full_sets_table.c and tests/full_sets_test.c into a temporary folder, runs the C on the disc's real module chunks for each row (the lists at
LISTS_ADDRESS) and checks the result by resolution: for every target table entry it walks the steps in the output chunk and the mapped
entry's steps in the source, step by step (duration, flags, loop/frame field equal, the cell list read in the placed lists equal to the
source's), checks the outside steps resolve the same way and that the bytes the C wrote are exactly the ones the model says, then prints one
line per row; it ends with exit status 0 when every row is equal. A report on the other mirror pairs (OTHERS: Sakura and Gen) is printed with
either option, as a finding only: no row is made for them.
"""
import argparse
import difflib
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import fighters as F        # noqa: E402  read only: the disc reader, the file splitter, the cell list parser
import module_data as M     # noqa: E402  read only: the generated tables' parsers, the load addresses
import sequences as S       # noqa: E402  read only: the step finder
import pac                  # noqa: E402  read only: the chunk table

TARGETS = [("PL10.PAC", "PL16.PAC"), ("PL10X.PAC", "PL16.PAC")]
OTHERS = [("PL0D.PAC", "PL15.PAC"), ("PL0DX.PAC", "PL15.PAC"),     # Sakura
          ("PL11.PAC", "PL17.PAC"), ("PL11X.PAC", "PL17.PAC"), ("PL13.PAC", "PL17.PAC"), ("PL13X.PAC", "PL17.PAC")]   # Gen
KIND = 0x10                         # the player kind that loads the targets (Zangief)
EXCEPTIONS = (34, 37, 38, 66, 102, 119)   # target entries that are fuller variants, not sequences the source's has with extra steps
LISTS_ADDRESS = 0x80200000          # where --compare-c places the source's cell lists
TILES_ADDRESS = 0x80206000          # ... and the source's whole sprite chunk
TILES_BASE = {4: 0x80010000, 5: 0x8007f200}   # where the code of the first-side and second-side module reads its sprite chunk (the second is a placeholder)
STEP = 12
FRAME = 16
MAX_STEPS = 200                     # a walk of one sequence stops here
JUMP = 0x8000
RUN_COST = 1000                     # dynamic programming: cost of a new run
SUB_COST = 1                        # ... of an entry whose mapping is a subsequence
TIE_COST = 0.001                    # ... of an entry per unit of |d|: among mappings of equal content, the one nearest to the same index
FAIL = float('inf')

DISC_HELP = 'disc image, raw 2352-byte sectors'


class Fail(Exception):
    """The data is not what the rules expect."""


# ---------------------------------------------------------------- pure functions (tested without game data)

def u32(b, o):
    return struct.unpack_from('<I', b, o)[0]


def round4(n):
    return (n + 3) & ~3


def find_blocks(mod, load):
    """The blocks of a fighter module found by structure: dict(positions (start of the runs, where the code ends), lists (start, end, count),
    steps (start, count), table (start, entries), frames (start, count), step list (the dicts of sequences.step_at)); Fail when not found."""
    ce = F.code_end(mod)
    if ce is None:
        raise Fail('no code end')
    res = F.find_chain_first(mod, ce) or F.find_chain(mod, ce)
    if not res:
        raise Fail('no chain of cell lists')
    l0, lists = res
    lend = lists[-1]['end']
    st, steps = S.find_steps(mod, lend, lend - l0)
    if not steps:
        raise Fail('no step run')
    table = S.ps1_table_entries(mod, st, len(steps), load)
    if not table:
        raise Fail('no step table')
    fb = st + STEP * len(steps) + 4 * len(table)
    frames = max(p['fi'] for p in steps) + 1
    return dict(positions=ce, positions_end=min(l0, ce + lists[-1]['pos']), lists=(l0, lend, len(lists)), steps=(st, len(steps)),
                table=(st + STEP * len(steps), len(table)), frames=(fb, frames), step_list=steps)


def table_offsets(mod, load, blocks):
    """The module offsets of the steps the step table's entries point to."""
    t, n = blocks['table']
    return [u32(mod, t + 4 * i) - load for i in range(n)]


def walk(mod, o, limit=MAX_STEPS):
    """[(duration, flags, list offset word, loop/frame word)] of the steps from module offset o to the step with flags bit 15 (included), at
    most `limit`; Fail when it leaves the module."""
    out = []
    while len(out) < limit:
        if o < 0 or o + STEP > len(mod):
            raise Fail('a sequence leaves the module')
        dur, fl, ptr, fr = struct.unpack_from('<HHII', mod, o)
        out.append((dur, fl, ptr, fr))
        o += STEP
        if fl & JUMP:
            break
    return out


def relation(a, b):
    """'same' when the steps a equal b in duration, flags and frame field; 'sub' when a's frames (frame numbers of the steps without bit 15) are a
    subsequence of b's; None otherwise."""
    if [(s[0], s[1], s[3]) for s in a] == [(s[0], s[1], s[3]) for s in b]:
        return 'same'
    it = iter([s[3] >> 16 for s in b if not s[1] & JUMP])
    if all(any(x == y for y in it) for x in [s[3] >> 16 for s in a if not s[1] & JUMP]):
        return 'sub'
    return None


def weak_variant(a, b):
    """What an EXCEPTION must satisfy: both sequences end with a step of flags bit 15."""
    return bool(a[-1][1] & JUMP and b[-1][1] & JUMP)


def map_entries(target, source, exceptions=EXCEPTIONS, unmatched_cost=None):
    """(list of source entry per target entry, list of relations 'same'/'sub'/'variant'/None) for the sequences `target` (a list of step lists)
    and `source`. Strict (unmatched_cost None): Fail when an entry has no mapping, when the entries that do not map by content are not
    exactly `exceptions`, or when two entries share a source entry. With unmatched_cost a number, an entry that maps to nothing costs that and
    is reported as None (a finding, not a refusal)."""
    nt, ns = len(target), len(source)
    rel = {}

    def local(i, d):
        j = i + d
        if not 0 <= j < ns:
            return FAIL
        key = (i, j)
        if key not in rel:
            rel[key] = relation(target[i], source[j])
        r = rel[key]
        tie = TIE_COST * abs(d)
        if i in exceptions:
            if r is not None:
                return FAIL
            return tie if weak_variant(target[i], source[j]) else FAIL
        if r == 'same':
            return tie
        if r == 'sub':
            return SUB_COST + tie
        return FAIL if unmatched_cost is None else unmatched_cost + tie

    ds = range(-(nt - 1), ns)
    prev = {d: 0 for d in ds}
    back = []
    for i in range(nt):
        best_d = min(prev, key=lambda d: prev[d])
        best = prev[best_d]
        cur, bp = {}, {}
        for d in ds:
            l = local(i, d)
            if l == FAIL:
                cur[d] = FAIL
                bp[d] = None
                continue
            stay = prev[d]
            move = best + RUN_COST if i else 0
            if stay <= move:
                cur[d], bp[d] = stay + l, d
            else:
                cur[d], bp[d] = move + l, best_d
        if min(cur.values()) == FAIL:
            raise Fail(f'entry {i} maps to no source entry (and is not one of the exceptions)')
        back.append(bp)
        prev = cur
    d = min(prev, key=lambda x: prev[x])
    chosen = [0] * nt
    for i in range(nt - 1, -1, -1):
        chosen[i] = d
        d = back[i][d]
    js = [i + chosen[i] for i in range(nt)]
    rels = []
    for i, j in enumerate(js):
        r = rel[(i, j)] if (i, j) in rel else relation(target[i], source[j])
        if i in exceptions:
            rels.append('variant')
        else:
            rels.append(r)
    if unmatched_cost is None:
        bad = [i for i, r in enumerate(rels) if r == 'variant']
        if bad != sorted(exceptions):
            raise Fail(f'entries that do not map by content: {bad}, expected {sorted(exceptions)}')
        if len(set(js)) != nt:
            raise Fail('two target entries map to the same source entry')
    return js, rels


def to_runs(js):
    """[(first, count, source_first)]: the stretches of the mapping where the source entry grows by one with the target entry."""
    runs = []
    for i, j in enumerate(js):
        if runs and runs[-1][2] + runs[-1][1] == j:
            runs[-1] = (runs[-1][0], runs[-1][1] + 1, runs[-1][2])
        else:
            runs.append((i, 1, j))
    return runs


def tail_words(mod, start):
    return [u32(mod, o) for o in range(start, len(mod) - 3, 4)]


def outside_steps(tmod, tload, tstart, tlists_size, smod, sload, sstart, slists_size):
    """([(target offset, source offset)] of the steps outside the step block, the words of zero padding the source has more) from the diff of the
    words of the data after the frames (target from tstart, source from sstart). Module addresses are not compared (they compare equal as such)
    and a word of a pointer to each module's own range is a pointer. Fail when anything else differs."""
    tw, sw = tail_words(tmod, tstart), tail_words(smod, sstart)
    a = ['P' if tload <= w < tload + len(tmod) else w for w in tw]
    b = ['P' if sload <= w < sload + len(smod) else w for w in sw]
    out, extra = [], 0
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if tag == 'equal':
            continue
        if tag == 'insert' and not any(sw[j1:j2]):
            extra += j2 - j1
            continue
        if tag != 'replace' or i2 - i1 != j2 - j1:
            raise Fail(f'the data after the frames differs ({tag}) at target offset {tstart + 4 * i1:#x}, source {sstart + 4 * j1:#x}')
        for k in range(i2 - i1):
            if a[i1 + k] == 'P' or b[j1 + k] == 'P':
                raise Fail(f'a module address against a value at target offset {tstart + 4 * (i1 + k):#x}')
            if (a[i1 + k] & ~1) >= tlists_size or (b[j1 + k] & ~1) >= slists_size:
                raise Fail(f'a differing word at target offset {tstart + 4 * (i1 + k):#x} is not a list offset')
            out.append((tstart + 4 * (i1 + k) - 4, sstart + 4 * (j1 + k) - 4))
    return out, extra


def pointers_into(mod, load, start, lo, hi):
    """Module addresses among the words from `start` that point into the module offsets [lo, hi)."""
    return [start + 4 * k for k, w in enumerate(tail_words(mod, start)) if load + lo <= w < load + hi]


def plan(target, source, ftable, stable):
    """The table row of a pair (dict) and the facts for the report. `target` and `source` are dicts(name, mod, load, slot, tiles_slot);
    ftable/stable the parsed fighters_table.c and sequences_table.c. Fail when the data is not what the rules expect."""
    tb, sb = find_blocks(target['mod'], target['load']), find_blocks(source['mod'], source['load'])
    fr, sq = ftable.get(source['name']), stable.get(source['name'])
    if not fr or not sq:
        raise Fail(f"{source['name']} is not in fighters_table.c and sequences_table.c")
    sp_start, sp_size = fr['positions'], fr['positions_room']
    sl_start, sl_size = fr['lists'], fr['lists_room']
    found = dict(positions=(sb['positions'], sb['positions_end'] - sb['positions']), lists=(sb['lists'][0], sb['lists'][1] - sb['lists'][0]),
                 steps=(sb['steps'][0], sb['steps'][1]), table=sb['table'][1])
    given = dict(positions=(sp_start, sp_size), lists=(sl_start, sl_size), steps=(sq['steps'], sq['step_count']), table=sq['table_entries'])
    if found != given or not fr['same_chunk'] or sq['load'] != source['load'] or sq['frames'] != sb['frames'][0]:
        raise Fail(f"{source['name']}: the blocks found {found} are not the generated tables' {given}")
    s_steps, s_count = sb['steps']
    t_steps_old, t_count_old = tb['steps']
    # frames: the same records
    tf, nf = tb['frames']
    sf, nsf = sb['frames']
    if nf != nsf or target['mod'][tf:tf + FRAME * nf] != source['mod'][sf:sf + FRAME * nf]:
        raise Fail('the frame records of the target and the source are not the same')
    # sequences
    tt, st = table_offsets(target['mod'], target['load'], tb), table_offsets(source['mod'], source['load'], sb)
    tseq = [walk(target['mod'], o) for o in tt]
    sseq = [walk(source['mod'], o) for o in st]
    js, rels = map_entries(tseq, sseq)
    runs = to_runs(js)
    # outside steps
    outside, extra = outside_steps(target['mod'], target['load'], tf + FRAME * nf, tb['lists'][1] - tb['lists'][0],
                                   source['mod'], source['load'], sf + FRAME * nf, sb['lists'][1] - sb['lists'][0])
    # the place
    positions = tb['positions']
    steps = round4(positions + sp_size)
    steps_end = steps + STEP * s_count
    table = tb['table'][0]
    if steps_end > table:
        raise Fail(f'the source steps end at {steps_end:#x}, after the target step table at {table:#x}')
    if positions + sp_size > steps:
        raise Fail('the positions overlap the steps')
    lists = tb['lists'][0]
    overwritten = pointers_into(target['mod'], target['load'], tf + FRAME * nf, positions, table)
    if overwritten:
        raise Fail(f'module addresses in the data after the frames point into the rewritten range: {[hex(o) for o in overwritten]}')
    if (target['load'] + lists) & 1:
        raise Fail('the target lists base is odd')
    row = dict(file=target['name'], source=source['name'], slot=target['slot'], source_slot=source['slot'], tiles_slot=target['tiles_slot'],
               source_tiles_slot=source['tiles_slot'], kind=KIND, load=target['load'], source_load=source['load'],
               source_positions=sp_start, positions_size=sp_size, source_lists=sl_start, lists_size=sl_size, source_steps=s_steps,
               step_count=s_count, source_table_entries=sb['table'][1], positions=positions, lists=lists, steps=steps, table=table,
               table_entries=tb['table'][1], runs=runs, outside=outside)
    facts = dict(relations=rels, target_steps=t_count_old, target_lists=tb['lists'][2], source_lists=sb['lists'][2], steps_end=steps_end,
                 positions_end=positions + sp_size, extra_zero_words=extra, mapping=js, target_blocks=tb, source_blocks=sb)
    return row, facts


def stem(name):
    return name.removesuffix('.PAC').lower()


def c_table(rows):
    """The text of port/arcade/full_sets_table.c."""
    lines = ['/* GENERATED by `python3 port/arcade/full_sets.py --disc DISC --arcade SET --c-table FILE` (FILE = port/arcade/full_sets_table.c); do not edit.',
             ' * For each fighter module whose animation set the PS1 cut (Zangief\'s PL10 and PL10X), the numbers full_sets.c needs to write the',
             " * fuller set of the mirror module (PL16) into it: the blocks of both modules (module offsets), the runs of the step table (target entries",
             ' * first .. first + count - 1 are the source\'s entries source_first ..) and the steps outside the step block. Only offsets, counts and',
             ' * indices: no byte of either game. */',
             '#include "full_sets.h"', '']
    for r in rows:
        s = stem(r['file'])
        lines.append(f'static const struct arcade_full_run runs_{s}[] = {{')
        lines += [f'    {{{a}u, {b}u, {c}u}},' for a, b, c in r['runs']]
        lines.append('};')
        if r['outside']:
            lines.append(f'static const struct arcade_full_step outside_{s}[] = {{')
            lines += [f'    {{{a:#x}u, {b:#x}u}},' for a, b in r['outside']]
            lines.append('};')
        lines.append('')
    lines.append('const struct arcade_full_set arcade_full_sets[] = {')
    for r in rows:
        s = stem(r['file'])
        out = f'outside_{s}, {len(r["outside"])}u' if r['outside'] else 'NULL, 0u'
        lines.append(f'    {{"{r["file"]}", "{r["source"]}", {r["slot"]}u, {r["source_slot"]}u, {r["tiles_slot"]}u, {r["source_tiles_slot"]}u, {r["kind"]:#x}u, '
                     f'{r["load"]:#x}u, {r["source_load"]:#x}u, {r["source_positions"]:#x}u, {r["positions_size"]}u, {r["source_lists"]:#x}u, '
                     f'{r["lists_size"]}u, {r["source_steps"]:#x}u, {r["step_count"]}u, {r["source_table_entries"]}u, {r["positions"]:#x}u, '
                     f'{r["lists"]:#x}u, {r["steps"]:#x}u, {r["table"]:#x}u, {r["table_entries"]}u, runs_{s}, {len(r["runs"])}u, {out}}},')
    lines.append('};')
    lines.append('const unsigned arcade_full_set_count = sizeof arcade_full_sets / sizeof arcade_full_sets[0];')
    return '\n'.join(lines) + '\n'


SPEC_FIELDS = ('slot', 'source_slot', 'tiles_slot', 'source_tiles_slot', 'kind', 'load', 'source_load', 'source_positions', 'positions_size',
               'source_lists', 'lists_size', 'source_steps', 'step_count', 'source_table_entries', 'positions', 'lists', 'steps', 'table',
               'table_entries')


def spec_text(r):
    """A row in the text form tests/full_sets_test.c reads and prints: `file source` and the SPEC_FIELDS, the run and outside counts, a line per run
    and per outside step."""
    head = [r['file'], r['source']] + [str(r[k]) for k in SPEC_FIELDS] + [str(len(r['runs'])), str(len(r['outside']))]
    return '\n'.join([' '.join(head)] + [' '.join(map(str, x)) for x in r['runs']] + [' '.join(map(str, x)) for x in r['outside']]) + '\n'


def parse_spec(text):
    """The row (dict) of spec_text's text."""
    lines = text.split('\n')
    h = lines[0].split()
    r = dict(file=h[0], source=h[1])
    r.update({k: int(v, 0) for k, v in zip(SPEC_FIELDS, h[2:2 + len(SPEC_FIELDS)])})
    nr, no = int(h[2 + len(SPEC_FIELDS)]), int(h[3 + len(SPEC_FIELDS)])
    r['runs'] = [tuple(int(x, 0) for x in lines[1 + k].split()) for k in range(nr)]
    r['outside'] = [tuple(int(x, 0) for x in lines[1 + nr + k].split()) for k in range(no)]
    return r


def expected_changed(r, size):
    """The mask (bytearray of 0/1 per byte) of the bytes arcade_full_build must write for the row."""
    m = bytearray(size)
    for a, n in ((r['positions'], r['positions_size']), (r['steps'], STEP * r['step_count'])):
        m[a:a + n] = b'\1' * n
    for first, count, _src in r['runs']:
        m[r['table'] + 4 * first:r['table'] + 4 * (first + count)] = b'\1' * (4 * count)
    for t, _s in r['outside']:
        m[t + 4:t + 8] = b'\1' * 4
    return m


def resolve_lists(mod, lists_base, off, placed, placed_at):
    """The bytes of the cell list the list offset word `off` names: read at lists_base + (off & ~1) from `mod`, or from `placed` when that address
    is inside it (placed starts at address placed_at); lists_base is an address in the second case. None when it is not a list."""
    o = off & ~1
    if placed is None:
        lst = F.parse_list(mod, lists_base + o)
        return None if lst is None else mod[lists_base + o:lst['end']]
    a = (lists_base + o) & 0xffffffff
    if not placed_at <= a < placed_at + len(placed):
        return None
    lst = F.parse_list(placed, a - placed_at)
    return None if lst is None else placed[a - placed_at:lst['end']]


def compare_row(r, module, source, out, changed, lists_address=LISTS_ADDRESS):
    """(counts dict, problems) of the C's result `out`/`changed` for the row r against the source, by resolution (see --compare-c)."""
    problems = []
    load, sload = r['load'], r['source_load']
    placed = source[r['source_lists']:r['source_lists'] + r['lists_size']]
    if len(out) != len(module):
        problems.append('the output chunk has another size')
    if bytes(changed) != bytes(expected_changed(r, len(module))):
        problems.append('the bytes written are not the model\'s')
    if any(a != b for k, (a, b, c) in enumerate(zip(out, module, changed)) if not c and a != b):
        problems.append('a byte outside the written ones changed')
    covered = [None] * r['table_entries']
    for first, count, src in r['runs']:
        for k in range(count):
            covered[first + k] = src + k
    counts = dict(entries=0, steps=0, lists=0, outside=0)
    sblocks = dict(steps=r['source_steps'], table=r['source_steps'] + STEP * r['step_count'])
    for i, j in enumerate(covered):
        if j is None:
            problems.append(f'entry {i} is in no run')
            continue
        try:
            a = walk(out, u32(out, r['table'] + 4 * i) - load)
            b = walk(source, u32(source, sblocks['table'] + 4 * j) - sload)
        except Fail as e:
            problems.append(f'entry {i}: {e}')
            continue
        counts['entries'] += 1
        if len(a) != len(b):
            problems.append(f'entry {i}: {len(a)} steps against {len(b)}')
            continue
        for k, (x, y) in enumerate(zip(a, b)):
            if (x[0], x[1], x[3]) != (y[0], y[1], y[3]):
                problems.append(f'entry {i} step {k}: duration, flags or loop/frame differ')
                continue
            la = resolve_lists(out, load + r['lists'], x[2], placed, lists_address)
            lb = resolve_lists(source, r['source_lists'], y[2], None, 0)
            if la is None or la != lb or (x[2] & 1) != (y[2] & 1):
                problems.append(f'entry {i} step {k}: the cell list differs')
                continue
            counts['steps'] += 1
            counts['lists'] += 1
    for t, s in r['outside']:
        x, y = struct.unpack_from('<HHII', out, t), struct.unpack_from('<HHII', source, s)
        la = resolve_lists(out, load + r['lists'], x[2], placed, lists_address)
        lb = resolve_lists(source, r['source_lists'], y[2], None, 0)
        if (x[0], x[1], x[3]) != (y[0], y[1], y[3]) or la is None or la != lb:
            problems.append(f'outside step at {t:#x} does not resolve to the source\'s list')
        else:
            counts['outside'] += 1
    return counts, problems


def compare_tiles(source, address, base, table, count):
    """Problems (list of text) of the offset table `table` (count entries) for the source's sprite chunk placed at `address`, the game reading
    tile i at base + entry i: every entry resolves, in the placed chunk, to the block the source's own entry i names, and the block's extent
    (up to the next higher entry, or the chunk's end) holds the same bytes."""
    problems = []
    if count != u32(source, 0) // 4 or len(table) != 4 * count:
        return ['the table has another number of tiles than the source\'s']
    own = [u32(source, 4 * i) for i in range(count)]
    ends = sorted(set(own) | {len(source)})
    bad = 0
    for i in range(count):
        resolved = (base + u32(table, 4 * i) - address) & 0xffffffff     # offset in the placed chunk
        nxt = ends[ends.index(own[i]) + 1]
        if resolved != own[i] or source[resolved:resolved + nxt - own[i]] != source[own[i]:nxt]:
            bad += 1
    if bad:
        problems.append(f'{bad} of {count} tiles resolve to another block')
    return problems


# ---------------------------------------------------------------- the disc

def read_files(disc, names):
    """{name: dict(name, mod, load, side, slot, tiles_slot)} of the fighter files on the disc."""
    out = {}
    with open(disc, 'rb') as f:
        index, sec = F.iso_files(f)
        for name in sorted(set(names)):
            found = [p for p in index if p.upper().endswith('/' + name)]
            if len(found) != 1:
                raise Fail(f'{name} is not on the disc')
            lba, size = index[found[0]]
            data = sec(lba, (size + 2047) // 2048)[:size]
            res = F.split_file(data)
            if isinstance(res, str):
                raise Fail(f'{name}: {res}')
            mod, _tiles, side, _others = res
            chunks = [c for c in pac.chunk_table(data) if c['table'] == 0]
            slot = [c['slot'] for c in chunks if c['slot'] in F.MODULE_SLOTS]
            tiles = [c['slot'] for c in chunks if c['slot'] in F.TILE_SLOTS]
            out[name] = dict(name=name, mod=mod, tiles=_tiles, load=M.LOAD[side], side=side, slot=slot[0], tiles_slot=tiles[0])
    return out


def report_row(r, facts):
    rel = facts['relations']
    print(f"{r['file']} <- {r['source']}: {len(rel)} table entries ({rel.count('same')} same, {rel.count('sub')} subsequence, "
          f"{rel.count('variant')} variant), runs {r['runs']}")
    print(f"    target {facts['target_steps']} steps, {facts['target_lists']} lists; source {r['step_count']} steps, {facts['source_lists']} lists; "
          f"positions {r['positions']:#x}..{facts['positions_end']:#x}, steps {r['steps']:#x}..{facts['steps_end']:#x}, table {r['table']:#x}; "
          f"{len(r['outside'])} outside steps at {[hex(t) for t, _s in r['outside']]} (source {[hex(s) for _t, s in r['outside']]}); "
          f"{facts['extra_zero_words']} zero words more in the source")


def report_other(files, ftable, stable):
    """A finding for the other mirror pairs: counts and whether a mapping by content exists. No row."""
    print('Other mirror pairs (a finding only, no table row):')
    for tn, sn in OTHERS:
        t, s = files[tn], files[sn]
        try:
            tb, sb = find_blocks(t['mod'], t['load']), find_blocks(s['mod'], s['load'])
        except Fail as e:
            print(f'  {tn} <- {sn}: blocks not found ({e})')
            continue
        tseq = [walk(t['mod'], o) for o in table_offsets(t['mod'], t['load'], tb)]
        sseq = [walk(s['mod'], o) for o in table_offsets(s['mod'], s['load'], sb)]
        text = f'  {tn} <- {sn}: steps {tb["steps"][1]} / {sb["steps"][1]}, lists {tb["lists"][2]} / {sb["lists"][2]}, table entries {tb["table"][1]} / {sb["table"][1]}'
        try:
            js, rels = map_entries(tseq, sseq, exceptions=(), unmatched_cost=50)
        except Fail as e:
            print(text + f'; no mapping ({e})')
            continue
        runs = to_runs(js)
        text += (f'; mapping by content: {rels.count("same")} same, {rels.count("sub")} subsequence, {rels.count(None)} none, '
                 f'{len(runs)} runs {runs[:6]}{"..." if len(runs) > 6 else ""}')
        tf, nf = tb['frames']
        sf, nsf = sb['frames']
        text += f'; frames {"equal" if nf == nsf and t["mod"][tf:tf + FRAME * nf] == s["mod"][sf:sf + FRAME * nf] else "differ"}'
        try:
            out, _x = outside_steps(t['mod'], t['load'], tf + FRAME * nf, tb['lists'][1] - tb['lists'][0],
                                    s['mod'], s['load'], sf + FRAME * nsf, sb['lists'][1] - sb['lists'][0])
            text += f'; {len(out)} outside steps'
        except Fail as e:
            text += f'; after the frames: {e}'
        print(text)


# ---------------------------------------------------------------- the C

def build_driver(tmp):
    exe = str(Path(tmp) / 'full_sets_test')
    subprocess.run(['gcc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-o', exe, str(HERE / 'full_sets.c'), str(HERE / 'full_sets_table.c'),
                    str(HERE / 'tests/full_sets_test.c')], check=True, timeout=120)
    return exe


def compare_c(rows, files, text, tmp):
    """Run the checked-in C table on the disc's chunks; print a line per row. Returns the exit status."""
    status = 0
    table_file = HERE / 'full_sets_table.c'
    print('checked-in full_sets_table.c ' + ('equals' if table_file.exists() and table_file.read_text() == text else 'DIFFERS FROM') + ' the table this tool derives')
    if not table_file.exists() or table_file.read_text() != text:
        status = 1
    exe = build_driver(tmp)
    dumped = subprocess.run([exe, 'table'], check=True, capture_output=True, text=True, timeout=60).stdout.split('\n')
    c_rows, k = [], 0
    while k < len(dumped) and dumped[k]:
        h = dumped[k].split()
        n = int(h[-2]) + int(h[-1])
        c_rows.append(parse_spec('\n'.join(dumped[k:k + 1 + n]) + '\n'))
        k += 1 + n
    if len(c_rows) != len(rows):
        print(f'the C table has {len(c_rows)} rows, the tool derived {len(rows)}')
        return 1
    for idx, (r, c) in enumerate(zip(rows, c_rows)):
        module, source = files[r['file']]['mod'], files[r['source']]['mod']
        if r != c:
            print(f"{r['file']}: the C table's row differs from the derived row")
            status = 1
        paths = {n: str(Path(tmp) / n) for n in ('module', 'source', 'out', 'changed')}
        Path(paths['module']).write_bytes(module)
        Path(paths['source']).write_bytes(source)
        run = subprocess.run([exe, 'row', str(idx), paths['module'], paths['source'], str(LISTS_ADDRESS), paths['out'], paths['changed']],
                             capture_output=True, text=True, timeout=60)
        if run.returncode:
            print(f"{r['file']}: the C refused or failed: {run.stdout.strip()} {run.stderr.strip()}")
            status = 1
            continue
        out, changed = Path(paths['out']).read_bytes(), Path(paths['changed']).read_bytes()
        counts, problems = compare_row(r, module, source, out, changed)
        print(f"{r['file']}: {counts['entries']} of {r['table_entries']} entries, {counts['steps']} steps and cell lists, {counts['outside']} of "
              f"{len(r['outside'])} outside steps resolve equal to {r['source']}; {sum(changed)} bytes written; "
              + ('EQUAL' if not problems else f'{len(problems)} PROBLEMS: ' + '; '.join(problems[:5])))
        if problems:
            status = 1
        base = TILES_BASE[r['slot']]
        spaths = {n: str(Path(tmp) / n) for n in ('tiles_source', 'tiles_out')}
        src_tiles = files[r['source']]['tiles']
        Path(spaths['tiles_source']).write_bytes(src_tiles)
        run = subprocess.run([exe, 'tiles', spaths['tiles_source'], str(TILES_ADDRESS), str(base), str(len(files[r['file']]['tiles'])), spaths['tiles_out']],
                             capture_output=True, text=True, timeout=60)
        if run.returncode:
            print(f"{r['file']} sprite table: the C refused or failed: {run.stdout.strip()} {run.stderr.strip()}")
            status = 1
            continue
        count = int(run.stdout)
        table = Path(spaths['tiles_out']).read_bytes()
        tproblems = compare_tiles(src_tiles, TILES_ADDRESS, base, table, count)
        print(f"{r['file']} sprite table: {count} tiles of {r['source']}'s chunk ({len(src_tiles)} bytes at {TILES_ADDRESS:#x}, base {base:#x}) resolve to the "
              + ('same blocks' if not tproblems else 'PROBLEM: ' + '; '.join(tproblems)))
        if tproblems:
            status = 1
    return status


# ---------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help=DISC_HELP)
    ap.add_argument('--arcade', help="arcade set ZIP (MAME's sfa2u); accepted, not read")
    ap.add_argument('--c-table', metavar='FILE', help='write port/arcade/full_sets_table.c to FILE')
    ap.add_argument('--compare-c', action='store_true', help='run full_sets.c on the disc and check the result by resolution')
    args = ap.parse_args(argv)
    started = time.monotonic()
    names = [n for pair in TARGETS + OTHERS for n in pair]
    try:
        files = read_files(args.disc, names)
        ftable = M.parse_fighters_table((HERE / 'fighters_table.c').read_text())
        stable = M.parse_sequences_table((HERE / 'sequences_table.c').read_text())
        rows = []
        for tn, sn in TARGETS:
            row, facts = plan(files[tn], files[sn], ftable, stable)
            report_row(row, facts)
            rows.append(row)
    except Fail as e:
        print(f'FAILED: {e}')
        return 1
    report_other(files, ftable, stable)
    text = c_table(rows)
    status = 0
    if args.c_table:
        Path(args.c_table).write_text(text)
        print(f'wrote {args.c_table}')
    if args.compare_c:
        tmp = tempfile.mkdtemp(prefix='full-sets-')
        try:
            status = compare_c(rows, files, text, tmp)
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    print(f'done in {time.monotonic() - started:.1f} s')
    return status


if __name__ == '__main__':
    sys.exit(main())
