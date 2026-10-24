#!/usr/bin/env python3
"""Analysis tool: full animation sets. Derives the table that full_sets.c reads, from the disc, and checks the C against it.

What it does. On the PS1, a fighter's module against another fighter holds a cut animation set (Zangief's PL10.PAC and PL10X.PAC: 890 steps,
272 cell lists); the mirror module PL16.PAC (both players Zangief) holds a fuller one (993 steps, 376 lists) and runs the same code. The
same goes for Gen: PL11, PL11X, PL13 and PL13X hold two blocks of steps (1849 steps, 351 lists), the mirror module PL17.PAC two fuller ones
(2095 steps, 613 lists). The runtime writes the mirror module's set into the module chunk (full_sets.c; the contract is full_sets.h). This
tool finds, for each pair in TARGETS, every number that code needs, from the bytes of the disc and the other generated tables, and fails
loudly where the data is not what the rules below expect:

  Blocks. Each module's blocks are found by their structure (the chain of cell lists after the code, the position runs in front of it, the
          step run after the lists, the pointer table after the steps and the frame records after that, and, when valid steps follow the
          frame records, a second block of steps with its own table and frame records), as module_data.py does. The source's blocks are
          checked against port/arcade/fighters_table.c's and sequences_table.c's rows of the source (they must agree).
  Frames. The frame records of the target and of the source are the same up to the source's count, except the records the C overwrites
          (Gen's differ in one record); the target has the same number or more (its extra records are the PS1's own and
          no step of the source uses them).
  Table.  The target's step table entry i is the source's entry i + d. Each target sequence (its steps from the entry up to the step with flags
          bit 15, at most 200) is, for the mapped source sequence, the same in duration, flags and frame field ("same"), or its frames (the
          non-jump steps' frame numbers) are a subsequence of the source's ("sub": the source has steps more), or it is one of the EXCEPTIONS (a
          fuller variant of the same sequence that is neither: the sequence differs in a step or two, as the owner established). The offset d is
          the one that gives the fewest runs (dynamic programming over the entries, preferring "same"); the runs are the stretches of constant
          d. Every entry must map; the exceptions must be exactly the entries that do not map by content; two entries never share a source entry.
  Outside steps. Steps of the target (and of the source) lie in the data after the frames, outside the step blocks, each with a list offset
          of the module's own lists (word 1). Zangief's 11 are found by a word-aligned diff of the data after the frames (the only words there
          that differ and are not module addresses). Gen's 8 are found by their form (a hold of 4095 ticks with flags 0x8000, no loop or
          frame) in the data after the step blocks and paired in order; each pair's lists have the same size.
  Place. The source's position runs go at the target's position runs (where its code reads them); the steps of each block go into the
          room the module has free: the room after the position runs up to the first step table (where the target's cell lists and first
          step block were, which the full set moves or replaces), and the room of the target's other step blocks, first fit by size.
          No written region may overlap another (the C checks it).

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade, accepted for the same command line as the folder's
other tools; this tool does not read it). The repository's two generated tables (fighters_table.c, sequences_table.c) are read. It writes
nothing but what --c-table names and a temporary folder (--compare-c), and prints numbers, never game bytes. Seconds.

    python3 port/arcade/full_sets.py --disc BIN --arcade ZIP [--c-table FILE | --compare-c]

--c-table FILE writes port/arcade/full_sets_table.c (the rows of TARGETS; numbers only). --compare-c compiles full_sets.c, the checked-in
full_sets_table.c and tests/full_sets_test.c into a temporary folder, runs the C on the disc's real module chunks for each row (the lists at
LISTS_ADDRESS) and checks the result by resolution: for every target table entry it walks the steps in the output chunk and the mapped
entry's steps in the source, step by step (duration, flags, loop/frame field equal, the cell list read in the placed lists equal to the
source's), checks the outside steps resolve the same way and that the bytes the C wrote are exactly the ones the model says, then prints one
line per row; it ends with exit status 0 when every row is equal. A report on the other mirror pairs (OTHERS: Sakura) is printed with
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

TARGETS = [("PL10.PAC", "PL16.PAC"), ("PL10X.PAC", "PL16.PAC"),                                      # Zangief
           ("PL11.PAC", "PL17.PAC"), ("PL11X.PAC", "PL17.PAC"), ("PL13.PAC", "PL17.PAC"), ("PL13X.PAC", "PL17.PAC")]   # Gen
OTHERS = [("PL0D.PAC", "PL15.PAC"), ("PL0DX.PAC", "PL15.PAC")]     # Sakura
KINDS = {"PL10": 0x10, "PL11": 0x11, "PL13": 0x13}   # the player kind that loads the target (its file is entry 2 * kind or 2 * kind + 1)
FAMILY = {"PL13": "PL11"}           # PL13 is Gen's other version: its module is PL11's byte for byte
# Per fighter, per block of steps: the target table entries that are fuller variants, not sequences the source's has with extra steps.
# Zangief's: one block. Gen's: the first block (PL11's entries 36 and 38 are shorter variants of the same arcade sequences, and 56, 57, 61 and 64
# have other frames in the middle) and the second (the same kinds of variants, and entries that point into the first block's steps in the PS1,
# where the arcade has sequences of its own).
EXCEPTIONS = {"PL10": [(34, 37, 38, 66, 102, 119)],
              "PL11": [(36, 38, 56, 57, 61, 64),
                       (2, 3, 30, 31, 32, 34, 36, 38, 63, 64, 188, 191, 195, 196, 197, 227, 230, 234, 235)]}
# How the steps outside the blocks are found: 'diff' (the words after the frames differ) or 'holds' (steps of the form of a hold)
OUTSIDE = {"PL10": "diff", "PL11": "holds"}
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


def pointing_table(mod, load, at, blocks):
    """The module offsets of the pointer table at `at`: words that point to a step of one of `blocks` (dicts with 'steps' (start, count)),
    up to the first word that does not."""
    out, o = [], at
    while o + 4 <= len(mod):
        v = u32(mod, o) - load
        if not any(b0 <= v < b0 + STEP * n and (v - b0) % STEP == 0 for b0, n in (b['steps'] for b in blocks)):
            break
        out.append(v)
        o += 4
    return out


def next_block(mod, load, blocks, list_size):
    """The block of steps that follows the frame records of the last of `blocks` (a run of steps, a pointer table after it and its frame
    records), or None. A few frame records may lie unused between the two."""
    fstart, fcount = blocks[-1]['frames']
    for gap in range(9):
        at, steps = S.run_of_steps(mod, fstart + FRAME * (fcount + gap), list_size)
        if len(steps) < S.MIN_RUN:
            continue
        end = at + STEP * len(steps)
        table = pointing_table(mod, load, end, blocks + [dict(steps=(at, len(steps)))])
        if table:
            return dict(steps=(at, len(steps)), table=(end, len(table)), frames=(end + 4 * len(table), max(p['fi'] for p in steps) + 1), step_list=steps)
    return None


def find_blocks(mod, load):
    """The blocks of a fighter module found by structure: dict(positions (start of the runs, where the code ends), lists (start, end, count),
    steps (start, count), table (start, entries), frames (start, count), step list (the dicts of sequences.step_at), blocks (a list of
    dicts(steps, table, frames, step_list), the first one the one above and, when steps follow its frame records, the later ones)); Fail when
    not found."""
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
    first = dict(steps=(st, len(steps)), table=(st + STEP * len(steps), len(table)), frames=(fb, frames), step_list=steps)
    blocks = [first]
    while True:
        more = next_block(mod, load, blocks, lend - l0)
        if more is None:
            break
        blocks.append(more)
    return dict(positions=ce, positions_end=min(l0, ce + lists[-1]['pos']), lists=(l0, lend, len(lists)), steps=first['steps'], table=first['table'],
                frames=first['frames'], step_list=steps, blocks=blocks)


def table_offsets(mod, load, block):
    """The module offsets of the steps the step table of `block` (or of the dict find_blocks returns, for its first block) points to."""
    t, n = block['table']
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


def pointers_into(mod, load, start, lo, hi, skip=()):
    """Module addresses among the words from `start` that point into the module offsets [lo, hi), but for the words that lie in a range of
    `skip` ((start, end) module offsets)."""
    return [start + 4 * k for k, w in enumerate(tail_words(mod, start))
            if load + lo <= w < load + hi and not any(a <= start + 4 * k < b for a, b in skip)]


def list_extent(mod, lists, off):
    """The size in bytes of the cell list the list offset word `off` names (lists: the module offset of the lists block), or None."""
    lst = F.parse_list(mod, lists + (off & ~1))
    return None if lst is None else lst['end'] - (lists + (off & ~1))


def hold_steps(mod, lists, lists_size, skip):
    """[(module offset, list offset word)] of the steps of the form of a hold (4095 ticks, flags 0x8000, no loop or frame) whose list offset
    names a cell list of the module's lists, at 4-aligned offsets outside the ranges `skip` ((start, end))."""
    out = []
    for o in range(0, len(mod) - STEP + 1, 4):
        if any(a <= o < b for a, b in skip):
            continue
        dur, fl, ptr, fr = struct.unpack_from('<HHII', mod, o)
        if dur == 4095 and fl == JUMP and fr == 0 and (ptr & ~1) < lists_size and list_extent(mod, lists, ptr) is not None:
            out.append((o, ptr))
    return out


def outside_holds(tmod, tlists, tlists_size, tskip, smod, slists, slists_size, sskip):
    """[(target offset, source offset)] of the hold steps outside the step blocks, paired in order. Fail when the numbers differ or a pair's
    lists are not of the same size."""
    t, s = hold_steps(tmod, tlists, tlists_size, tskip), hold_steps(smod, slists, slists_size, sskip)
    if len(t) != len(s) or not t:
        raise Fail(f'{len(t)} steps of the form of a hold outside the blocks in the target, {len(s)} in the source')
    for (to, tp), (so, sp) in zip(t, s):
        if list_extent(tmod, tlists, tp) != list_extent(smod, slists, sp):
            raise Fail(f'the hold steps at target offset {to:#x} and source offset {so:#x} name lists of another size')
    return [(to, so) for (to, _tp), (so, _sp) in zip(t, s)]


def place_blocks(sizes, regions):
    """[start of each block] for blocks of the given sizes in bytes, put first fit in decreasing size into the free `regions` ((start, end)),
    each block at the region's next free 4-aligned address. Fail when one does not fit."""
    free = [[a, b] for a, b in regions]
    out = [None] * len(sizes)
    for k in sorted(range(len(sizes)), key=lambda k: (-sizes[k], k)):
        for r in free:
            at = round4(r[0])
            if at + sizes[k] <= r[1]:
                out[k] = at
                r[0] = at + sizes[k]
                break
        else:
            raise Fail(f'a block of {sizes[k]} bytes does not fit in the free room {regions}')
    return out


def plan(target, source, ftable, stable):
    """The table row of a pair (dict) and the facts for the report. `target` and `source` are dicts(name, mod, load, slot, tiles_slot);
    ftable/stable the parsed fighters_table.c and sequences_table.c. Fail when the data is not what the rules expect."""
    tb, sb = find_blocks(target['mod'], target['load']), find_blocks(source['mod'], source['load'])
    fam = family(target['name'])
    fr, sq = ftable.get(source['name']), stable.get(source['name'])
    if not fr or not sq:
        raise Fail(f"{source['name']} is not in fighters_table.c and sequences_table.c")
    sp_start, sp_size = fr['positions'], fr['positions_room']
    sl_start, sl_size = fr['lists'], fr['lists_room']
    srows = [sq] + sq.get('more', [])
    found = dict(positions=sb['positions'], lists=(sb['lists'][0], sb['lists'][1] - sb['lists'][0]),
                 blocks=[(b['steps'], b['table'][1], b['frames'][0]) for b in sb['blocks']])
    given = dict(positions=sp_start, lists=(sl_start, sl_size),
                 blocks=[((r['steps'], r['step_count']), r['table_entries'], r['frames']) for r in srows])
    # the position runs end at the last stream the last list names (the structure's end of them) or later, before the lists
    if found != given or not fr['same_chunk'] or any(r['load'] != source['load'] for r in srows) or \
            not sb['positions_end'] - sb['positions'] <= sp_size <= sb['lists'][0] - sb['positions']:
        raise Fail(f"{source['name']}: the blocks found {found} (positions end {sb['positions_end']:#x}) are not the generated tables' {given} "
                   f"(positions room {sp_size})")
    if len(tb['blocks']) != len(sb['blocks']):
        raise Fail(f"{target['name']} has {len(tb['blocks'])} blocks of steps, {source['name']} {len(sb['blocks'])}")
    exceptions = EXCEPTIONS.get(fam)
    if exceptions is None or len(exceptions) != len(tb['blocks']):
        raise Fail(f'{fam}: no exceptions listed for its {len(tb["blocks"])} blocks')
    blocks, rels_all, mappings = [], [], []
    for k, (tk, sk) in enumerate(zip(tb['blocks'], sb['blocks'])):
        # frames: the source's records over the target's where they differ; the target has no fewer records
        tf, nf = tk['frames']
        sf, nsf = sk['frames']
        if nf < nsf:
            raise Fail(f'block {k}: the target has {nf} frame records, the source {nsf}')
        differing = [i for i in range(nsf) if target['mod'][tf + FRAME * i:tf + FRAME * (i + 1)] != source['mod'][sf + FRAME * i:sf + FRAME * (i + 1)]]
        copy = differing[-1] + 1 if differing else 0
        # sequences
        tt, st = table_offsets(target['mod'], target['load'], tk), table_offsets(source['mod'], source['load'], sk)
        tseq = [walk(target['mod'], o) for o in tt]
        sseq = [walk(source['mod'], o) for o in st]
        js, rels = map_entries(tseq, sseq, exceptions=exceptions[k])
        rels_all.append(rels)
        mappings.append(js)
        blocks.append(dict(source_steps=sk['steps'][0], step_count=sk['steps'][1], source_table_entries=sk['table'][1], steps=None, table=tk['table'][0],
                           table_entries=tk['table'][1], source_frames=sf if copy else 0, frames=tf if copy else 0, frame_count=copy, runs=to_runs(js)))
    # outside steps
    method = OUTSIDE[fam]
    t_all = [(b['steps'][0], b['steps'][0] + STEP * b['steps'][1]) for b in tb['blocks']]
    s_all = [(b['steps'][0], b['steps'][0] + STEP * b['steps'][1]) for b in sb['blocks']]
    tf, nf = tb['frames']
    sf, nsf = sb['frames']
    if method == 'diff':
        outside, extra = outside_steps(target['mod'], target['load'], tf + FRAME * nf, tb['lists'][1] - tb['lists'][0],
                                       source['mod'], source['load'], sf + FRAME * nsf, sb['lists'][1] - sb['lists'][0])
    else:
        outside = outside_holds(target['mod'], tb['lists'][0], tb['lists'][1] - tb['lists'][0], t_all,
                                source['mod'], sb['lists'][0], sb['lists'][1] - sb['lists'][0], s_all)
        extra = 0
    # the place: the position runs where the target's code reads them; each block of steps in the free room
    positions = tb['positions']
    table0 = tb['blocks'][0]['table'][0]
    room = [(positions + sp_size, table0)] + [(b['steps'][0], b['table'][0]) for b in tb['blocks'][1:]]
    sizes = [STEP * b['step_count'] for b in blocks]
    for b, at in zip(blocks, place_blocks(sizes, room)):
        b['steps'] = at
    if positions + sp_size > table0:
        raise Fail('the positions overlap the first step table')
    lists = tb['lists'][0]
    rewritten = [(positions, table0)] + [(b['steps'][0], b['table'][0]) for b in tb['blocks'][1:]]
    skip = [(b['table'][0], b['table'][0] + 4 * b['table'][1]) for b in tb['blocks']] + t_all[1:]
    overwritten = []
    for lo, hi in rewritten:
        overwritten += pointers_into(target['mod'], target['load'], tf, lo, hi, skip)
    if overwritten:
        raise Fail(f'module addresses in the data after the first frames point into the rewritten range: {[hex(o) for o in overwritten]}')
    if (target['load'] + lists) & 1:
        raise Fail('the target lists base is odd')
    row = dict(file=target['name'], source=source['name'], slot=target['slot'], source_slot=source['slot'], tiles_slot=target['tiles_slot'],
               source_tiles_slot=source['tiles_slot'], kind=KINDS[target['name'].removesuffix('.PAC').removesuffix('X')], load=target['load'],
               source_load=source['load'], source_positions=sp_start, positions_size=sp_size, source_lists=sl_start, lists_size=sl_size,
               positions=positions, lists=lists, blocks=blocks, outside=outside)
    facts = dict(relations=rels_all, mappings=mappings, target_steps=sum(b['steps'][1] for b in tb['blocks']), target_lists=tb['lists'][2],
                 source_lists=sb['lists'][2], extra_zero_words=extra, target_blocks=tb, source_blocks=sb)
    return row, facts


def family(name):
    """The fighter of a target file: 'PL10' for PL10.PAC and PL10X.PAC, 'PL11' for the files of PL11 and of its twin PL13."""
    f = name.removesuffix('.PAC').removesuffix('X')
    return FAMILY.get(f, f)


def stem(name):
    return name.removesuffix('.PAC').lower()


def c_table(rows):
    """The text of port/arcade/full_sets_table.c."""
    lines = ['/* GENERATED by `python3 port/arcade/full_sets.py --disc DISC --arcade SET --c-table FILE` (FILE = port/arcade/full_sets_table.c); do not edit.',
             " * For each fighter module whose animation set the PS1 cut (Zangief's PL10 and PL10X, Gen's PL11, PL11X, PL13 and PL13X), the numbers",
             " * full_sets.c needs to write the fuller set of the mirror module (PL16, PL17) into it: the blocks of both modules (module offsets), for each",
             " * block of steps the runs of its step table (target entries first .. first + count - 1 are the source's entries source_first ..) and the",
             ' * frame records to replace, and the steps outside the blocks. Only offsets, counts and indices: no byte of either game. */',
             '#include "full_sets.h"', '']
    for r in rows:
        s = stem(r['file'])
        for k, b in enumerate(r['blocks']):
            lines.append(f'static const struct arcade_full_run runs_{s}_{k}[] = {{')
            lines += [f'    {{{a}u, {n}u, {c}u}},' for a, n, c in b['runs']]
            lines.append('};')
        lines.append(f'static const struct arcade_full_block blocks_{s}[] = {{')
        for k, b in enumerate(r['blocks']):
            lines.append(f'    {{{b["source_steps"]:#x}u, {b["step_count"]}u, {b["source_table_entries"]}u, {b["steps"]:#x}u, {b["table"]:#x}u, '
                         f'{b["table_entries"]}u, {b["source_frames"]:#x}u, {b["frames"]:#x}u, {b["frame_count"]}u, runs_{s}_{k}, {len(b["runs"])}u}},')
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
                     f'{r["lists_size"]}u, {r["positions"]:#x}u, {r["lists"]:#x}u, blocks_{s}, {len(r["blocks"])}u, {out}}},')
    lines.append('};')
    lines.append('const unsigned arcade_full_set_count = sizeof arcade_full_sets / sizeof arcade_full_sets[0];')
    return '\n'.join(lines) + '\n'


SPEC_FIELDS = ('slot', 'source_slot', 'tiles_slot', 'source_tiles_slot', 'kind', 'load', 'source_load', 'source_positions', 'positions_size',
               'source_lists', 'lists_size', 'positions', 'lists')
BLOCK_FIELDS = ('source_steps', 'step_count', 'source_table_entries', 'steps', 'table', 'table_entries', 'source_frames', 'frames', 'frame_count')


def spec_text(r):
    """A row in the text form tests/full_sets_test.c reads and prints: a line `file source` and the SPEC_FIELDS, the block and outside counts;
    then for each block a line of the BLOCK_FIELDS and its run count with a line `first count source_first` per run; then a line
    `target source` per outside step."""
    lines = [' '.join([r['file'], r['source']] + [str(r[k]) for k in SPEC_FIELDS] + [str(len(r['blocks'])), str(len(r['outside']))])]
    for b in r['blocks']:
        lines.append(' '.join([str(b[k]) for k in BLOCK_FIELDS] + [str(len(b['runs']))]))
        lines += [' '.join(map(str, x)) for x in b['runs']]
    lines += [' '.join(map(str, x)) for x in r['outside']]
    return '\n'.join(lines) + '\n'


def parse_specs(text):
    """The rows (dicts) of spec_text's text, one after another."""
    lines = [l for l in text.split('\n') if l]
    rows, i = [], 0
    while i < len(lines):
        h = lines[i].split()
        r = dict(file=h[0], source=h[1])
        r.update({k: int(v, 0) for k, v in zip(SPEC_FIELDS, h[2:2 + len(SPEC_FIELDS)])})
        nb, no = int(h[2 + len(SPEC_FIELDS)]), int(h[3 + len(SPEC_FIELDS)])
        i += 1
        r['blocks'] = []
        for _ in range(nb):
            v = [int(x, 0) for x in lines[i].split()]
            b = dict(zip(BLOCK_FIELDS, v))
            i += 1
            b['runs'] = [tuple(int(x, 0) for x in lines[i + k].split()) for k in range(v[len(BLOCK_FIELDS)])]
            i += v[len(BLOCK_FIELDS)]
            r['blocks'].append(b)
        r['outside'] = [tuple(int(x, 0) for x in lines[i + k].split()) for k in range(no)]
        i += no
        rows.append(r)
    return rows


def parse_spec(text):
    """The row (dict) of spec_text's text."""
    rows = parse_specs(text)
    if len(rows) != 1:
        raise Fail(f'{len(rows)} rows in a spec')
    return rows[0]


def expected_changed(r, size):
    """The mask (bytearray of 0/1 per byte) of the bytes arcade_full_build must write for the row."""
    m = bytearray(size)
    m[r['positions']:r['positions'] + r['positions_size']] = b'\1' * r['positions_size']
    for b in r['blocks']:
        n = STEP * b['step_count']
        m[b['steps']:b['steps'] + n] = b'\1' * n
        for first, count, _src in b['runs']:
            m[b['table'] + 4 * first:b['table'] + 4 * (first + count)] = b'\1' * (4 * count)
        n = FRAME * b['frame_count']
        m[b['frames']:b['frames'] + n] = b'\1' * n
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
    if out[r['positions']:r['positions'] + r['positions_size']] != source[r['source_positions']:r['source_positions'] + r['positions_size']]:
        problems.append('the position runs are not the source\'s')
    counts = dict(entries=0, steps=0, lists=0, outside=0, frames=0)
    for n, b in enumerate(r['blocks']):
        covered = [None] * b['table_entries']
        for first, count, src in b['runs']:
            for k in range(count):
                covered[first + k] = src + k
        stable = b['source_steps'] + STEP * b['step_count']
        for k in range(b['step_count']):    # every step written: the same but for the list offset, which names the same list
            x, y = struct.unpack_from('<HHII', out, b['steps'] + STEP * k), struct.unpack_from('<HHII', source, b['source_steps'] + STEP * k)
            la = resolve_lists(out, load + r['lists'], x[2], placed, lists_address)
            lb = resolve_lists(source, r['source_lists'], y[2], None, 0)
            if (x[0], x[1], x[3]) != (y[0], y[1], y[3]) or la is None or la != lb or (x[2] & 1) != (y[2] & 1):
                problems.append(f'block {n} step {k}: not the source\'s or its cell list differs')
            else:
                counts['steps'] += 1
        for i, j in enumerate(covered):
            if j is None:
                problems.append(f'block {n} entry {i} is in no run')
                continue
            try:
                a = walk(out, u32(out, b['table'] + 4 * i) - load)
                c = walk(source, u32(source, stable + 4 * j) - sload)
            except Fail as e:
                problems.append(f'block {n} entry {i}: {e}')
                continue
            if len(a) != len(c) or any(x[:2] != y[:2] or x[3] != y[3] for x, y in zip(a, c)):
                problems.append(f'block {n} entry {i}: not the source\'s sequence')
                continue
            counts['entries'] += 1
            counts['lists'] += len(a)
        if b['frame_count']:
            if out[b['frames']:b['frames'] + FRAME * b['frame_count']] != source[b['source_frames']:b['source_frames'] + FRAME * b['frame_count']]:
                problems.append(f'block {n}: the frame records are not the source\'s')
            else:
                counts['frames'] += b['frame_count']
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
    print(f"{r['file']} <- {r['source']}: {len(r['blocks'])} block(s) of steps, {len(r['outside'])} outside steps at {[hex(t) for t, _s in r['outside']]} "
          f"(source {[hex(s) for _t, s in r['outside']]}); {facts['extra_zero_words']} zero words more in the source")
    print(f"    target {facts['target_steps']} steps, {facts['target_lists']} lists; source {sum(b['step_count'] for b in r['blocks'])} steps, "
          f"{facts['source_lists']} lists; positions {r['positions']:#x}..{r['positions'] + r['positions_size']:#x}")
    for k, (b, rel) in enumerate(zip(r['blocks'], facts['relations'])):
        print(f"    block {k}: {len(rel)} table entries ({rel.count('same')} same, {rel.count('sub')} subsequence, {rel.count('variant')} variant), "
              f"runs {b['runs']}; {b['step_count']} steps at {b['steps']:#x}..{b['steps'] + STEP * b['step_count']:#x}, table {b['table']:#x}, "
              f"{b['frame_count']} frame records replaced at {b['frames']:#x}")


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
        text = f'  {tn} <- {sn}: {len(tb["blocks"])} / {len(sb["blocks"])} blocks, first block steps {tb["steps"][1]} / {sb["steps"][1]}, lists {tb["lists"][2]} / {sb["lists"][2]}, table entries {tb["table"][1]} / {sb["table"][1]}'
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
    c_rows = parse_specs(subprocess.run([exe, 'table'], check=True, capture_output=True, text=True, timeout=60).stdout)
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
        print(f"{r['file']}: {counts['entries']} of {sum(b['table_entries'] for b in r['blocks'])} entries, {counts['steps']} steps (with their cell lists), "
              f"{counts['frames']} frame records and {counts['outside']} of {len(r['outside'])} outside steps resolve equal to {r['source']}; "
              f"{sum(changed)} bytes written; "
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
