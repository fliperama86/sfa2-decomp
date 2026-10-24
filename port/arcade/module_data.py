#!/usr/bin/env python3
"""Analysis tool: map every data byte of the 45 fighter modules, and say which of it the arcade set gives.

What it does. A fighter file (PLnn.PAC first side, PLnnX.PAC second side) holds a code module (table 0, slot 4 or 5). After the
code come the position runs, the cell lists, the animation steps, the step pointer table and the frame records, which fighters.py and
sequences.py convert from the arcade's art and sequence records (fighter.c and sequence.c build them), and then everything else: the
attack boxes, hurt boxes, box tables, move and command tables, tables of pointers, and the PS1's own tables. Some files carry more
code modules (slot 0xb in 24 files, the same bytes; slots 0 and 8 in PL09; 0x2b and 0x2c in PL0E, with its position runs; 0x16 and
0x17 in PL11 and PL13): the tool maps those too, and reports them apart. It puts every byte of a module in one class and builds,
from the arcade program, the classes the arcade holds:

  code        before the end of the module's last function (the port's C replaces it)
  positions, lists, steps, steptable, frames
              the blocks that fighter.c and sequence.c build, taken from fighters_table.c and sequences_table.c (the files that
              neither covers, PL04, PL10, PL11 and PL13 with their X twins, have them found by their structure and are marked)
  arcade      an exact run of the arcade program: the same values, each 16-bit field as it is ('b') or with its two bytes turned
              ('w'), or 32-bit fields turned ('l'). A run is at least MIN_PIECE bytes with at least MIN_EVIDENCE bits of evidence
              (program_map.py's measure: the information in its distinct non-zero values, minus the log2 of the places it could
              have matched); zeros at its ends are not part of it. Its layout is stored with it, in the form module_data.h says
  arcade_conv records of the arcade's tables that the PS1 narrowed: 8 arcade bytes (two 16-bit values, then two more) make 6
              PS1 bytes (the two values turned, then two bytes: the low byte of a value, or a flags byte whose bit 15 became
              bit 7, or a value divided by 0x18). These are the hurt boxes (struct Box6) and the tables of the same shape; the
              templates are TEMPLATES, a run needs MIN_RECORDS records and MIN_EVIDENCE_CONV bits
  near        inside a stretch that agrees with the arcade program but not byte for byte (a changed field, or a run too short to
              keep): not built
  ptr_*       a 32-bit word that is an address: ptr_self_code (a function of the module), ptr_self_label (inside a function: a
              jump table entry), ptr_self_data (another place of the module), ptr_prog_code, ptr_prog_data, ptr_prog_other (the
              resident program: a function, its data, anything else) and ptr_overlay (another overlay: the shared module at
              0x801e0000, the stage module, the other side). The words are built from a table of the module's offset and target
              (module_data_table.c): a target inside the module is stored as an offset (the link address is added), any other
              as the address, as program_relocs_table.c does
  celllist    a chain of cell lists beyond the main chain (not built)
  text        printable text (not built)
  zero        bytes that are zero and in no other class; a zero-filled image has them
  other       the PS1's own tables (not built)

The arcade search is program_map.py's: the same seeds, growth, scoring and evidence (imported, read only). It runs once over the data
of the first-side modules (the second side is the same source linked higher, so its pieces are the first side's at the same offsets,
plus the shift of its code when that differs in length) and once more, for the converted records, over what the exact pieces leave. A
module whose bytes another file has too is analysed once. Each file's pieces are then exact runs of that file's own bytes, so the
table is equal to the disc by construction; --compare-c shows that the C, the model and the table agree on that.

Inputs: the disc image (raw 2352-byte sectors, --disc) and the arcade set's ZIP (--arcade, MAME's sfa2u). The repository's own files
are read for the layout of the blocks other tools build (port/arcade/fighters_table.c and sequences_table.c), for the PS1 program's
functions and symbols (ps1/inventory/*.tsv, ps1/src/symbols.ld) and for the modules' functions (ps1/inventory/modules.tsv). It
writes NOTHING except a temporary folder (--compare-c compiles the C there and removes it) and prints numbers, addresses and class
names, never game bytes. It is an analysis and reference tool; the loader that builds the bytes at run time is the C (module_data.c).
About 45 seconds.

    python3 port/arcade/module_data.py --disc BIN --arcade ZIP [--only NAME] [--detail] [--c-table | --compare-c]

The report has one line per module (data bytes, what fighter.c and sequence.c build, the rest by class, the share this table
builds) and totals; --detail adds the largest stretches left. --c-table prints port/arcade/module_data_table.c: per module the
pieces (module offset, size, arcade address, layout) and the pointer words (module offset, target). --compare-c builds module_data.c
with the checked-in table into a temporary folder and prints, per module, the bytes the C built, whether they equal the model's and
the disc's, and the totals. (With --only the tables hold just that file, so --compare-c says the checked-in one differs.)
"""
import argparse
import bisect
import collections
import ctypes
import re
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
import program_map as P     # noqa: E402  read only: the arcade search and its evidence, pointer and text finders
import sequences as S       # noqa: E402  read only: the step finder (for the files no table covers)

LOAD = {1: 0x801b0000, 2: 0x801c8000}      # where the first-side and second-side modules are linked (ps1/docs/overlays.md)
EXTRA_LOAD = {0xb: 0x801e0000, 0x0: 0x80075e00, 0x8: 0x8008bf00, 0x2b: 0x80077000, 0x2c: 0x8008bf00, 0x16: 0x8007bc00, 0x17: 0x8008bf00}
FIRST_OF = {5: 4, 8: 0, 0x2c: 0x2b, 0x17: 0x16}   # the first-side slot of each second-side slot (ps1/docs/overlays.md)
RELATIVE = 0x80000000                       # a table target below this is an offset in the module, else an address
MIN_PIECE = 8                               # bytes of the smallest piece
MIN_EVIDENCE = 8.0                          # bits: the least information a piece's values must carry
STEP, FRAME = 12, 16
TEMPLATES = ('wwnn', 'wwfq', 'wwfn')         # record layouts of the converted tables (PS1 6 bytes from the arcade's 8), see template_search
MIN_RECORDS = 4                             # records of the shortest converted piece
WINDOW = 3                                  # records that must agree for a converted piece to be looked for
MIN_EVIDENCE_CONV = 12.0                    # bits for a converted piece
SENTINEL = b'\xa5\x5a' * 8
CLASSES = ['code', 'positions', 'lists', 'steps', 'steptable', 'frames', 'arcade', 'arcade_conv', 'near', 'ptr_self_code', 'ptr_self_label',
           'ptr_self_data', 'ptr_prog_code', 'ptr_prog_data', 'ptr_prog_other', 'ptr_overlay', 'celllist', 'text', 'zero', 'other']
BUILT_ELSEWHERE = ('positions', 'lists', 'steps', 'steptable', 'frames')
MINE = ('arcade', 'arcade_conv', 'zero') + tuple(c for c in CLASSES if c.startswith('ptr_'))   # classes this tool's table builds
LAYOUT_SIZES = {'b': (2, 2), 'w': (2, 2), 'l': (4, 4), 'n': (1, 2), 'e': (4, 1), 'f': (1, 2), 'q': (1, 2),   # letter -> (PS1 bytes, arcade bytes)
                'k': (32, 32), 'z': (32, 32), 'K': (32, 32), 'Z': (32, 32)}   # a palette row: 15 arcade words read, 16 words (32 bytes) of stride
PAL_BLACK_ZERO, PAL_BIT15 = 1, 2             # screens.py's palette flags
PALETTE_FLAGS = {'k': 0, 'z': PAL_BLACK_ZERO, 'K': PAL_BIT15, 'Z': PAL_BIT15 | PAL_BLACK_ZERO}   # the palette letters


# ---------------------------------------------------------------- pure functions (tested without game data)

def stem_of(name):
    """'PL0A.PAC' and 'PL0AX.PAC' are both 'PL0A'; the first side and the second side of one fighter."""
    s = name.removesuffix('.PAC')
    return s[:-1] if s.endswith('X') and len(s) > 2 and s != 'PL0X' else s


def parse_fighters_table(text):
    """{file: dict(positions, lists, positions_room, lists_room, same_chunk, positions_slot)} from port/arcade/fighters_table.c."""
    out = {}
    for m in re.finditer(r'\{"(PL\w+\.PAC)",\s*(\w+),\s*(\w+),\s*(0x[0-9a-f]+)u,\s*(0x[0-9a-f]+)u,\s*(\d+)u,\s*(\d+)u', text):
        same_chunk = m.group(3) == 'ARCADE_FIGHTER_SAME_CHUNK'
        out[m.group(1)] = dict(positions=int(m.group(4), 16), lists=int(m.group(5), 16), positions_room=int(m.group(6)),
                               lists_room=int(m.group(7)), same_chunk=same_chunk,
                               positions_slot=None if same_chunk else int(m.group(3).rstrip('u'), 0))
    return out


def parse_sequences_table(text):
    """{file: dict(load, steps, frames, step_count, frame_count, table_entries)} from port/arcade/sequences_table.c."""
    out = {}
    for m in re.finditer(r'\{"(PL\w+\.PAC)",\s*(0x[0-9a-f]+)u,\s*(0x[0-9a-f]+)u,\s*(0x[0-9a-f]+)u,\s*(\d+)u,\s*(\d+)u,\s*(\d+)u', text):
        out[m.group(1)] = dict(load=int(m.group(2), 16), steps=int(m.group(3), 16), frames=int(m.group(4), 16),
                               step_count=int(m.group(5)), frame_count=int(m.group(6)), table_entries=int(m.group(7)))
    return out


def spans_from_tables(fr, sq, size):
    """[(start, end, class)] of the five blocks the other builders make, from their table rows; None when a block does not fit the
    module or two overlap."""
    out = []
    if fr['same_chunk']:
        out.append((fr['positions'], fr['positions'] + fr['positions_room'], 'positions'))
    out.append((fr['lists'], fr['lists'] + fr['lists_room'], 'lists'))
    steps_end = sq['steps'] + STEP * sq['step_count']
    out.append((sq['steps'], steps_end, 'steps'))
    out.append((steps_end, steps_end + 4 * sq['table_entries'], 'steptable'))
    out.append((sq['frames'], sq['frames'] + FRAME * sq['frame_count'], 'frames'))
    out.sort()
    for (a, b, _c), (c, _d, _e) in zip(out, out[1:]):
        if b > c:
            return None
    return out if out[-1][1] <= size else None


def structural_spans(mod, load):
    """The same five blocks found by their structure (no table): positions from the end of the code to the last list's position
    offset, the longest chain of cell lists, the step run after it, the pointer table after the steps and the frame records after
    that. [(start, end, class)] or None when no chain or no step run is found. The frames start right after the table and
    their count is the highest frame number a step uses, plus one."""
    ce = F.code_end(mod)
    if ce is None:
        return None
    res = F.find_chain_first(mod, ce) or F.find_chain(mod, ce)
    if not res:
        return None
    l0, lists = res
    lend = lists[-1]['end']
    st, steps = S.find_steps(mod, lend, lend - l0)
    if not steps:
        return None
    table = S.ps1_table_entries(mod, st, len(steps), load)
    steps_end = st + STEP * len(steps)
    fb = steps_end + 4 * len(table)
    fe = fb + FRAME * (max(p['fi'] for p in steps) + 1)
    out = []
    if l0 > ce:
        out.append((ce, min(l0, ce + lists[-1]['pos']), 'positions'))
    out += [(l0, lend, 'lists'), (st, steps_end, 'steps'), (steps_end, fb, 'steptable'), (fb, min(fe, len(mod)), 'frames')]
    return [s for s in out if s[1] > s[0]]


def unit_ok(buf, m, i, d):
    """Two bytes of buf at i equal the arcade's at i + d as they are or turned."""
    if i < 0 or i + 2 > len(buf) or i + d < 0 or i + d + 2 > len(m):
        return False
    a, b = buf[i:i + 2], m[i + d:i + d + 2]
    return a == b or a[::-1] == b


def quad_ok(buf, m, i, d):
    """Four bytes of buf at i equal the arcade's at i + d turned (a 32-bit field)."""
    if i < 0 or i + 4 > len(buf) or i + d < 0 or i + d + 4 > len(m):
        return False
    return buf[i:i + 4][::-1] == m[i + d:i + d + 4]


def exact_runs_in(buf, m, lo, hi, d):
    """Maximal exact runs inside [lo, hi) of buf against the arcade program m at +d: [(start, end)]. A run is a stretch every
    byte of which is made of 16-bit fields equal as they are or turned and 32-bit fields turned, steps of 2 or 4 bytes."""
    out = []
    i = lo
    while i + 2 <= hi:
        if not (unit_ok(buf, m, i, d) or (i + 4 <= hi and quad_ok(buf, m, i, d))):
            i += 2
            continue
        reach = {i}
        end = i
        p = i
        while p <= end:
            if p in reach:
                if p + 2 <= hi and unit_ok(buf, m, p, d):
                    reach.add(p + 2)
                    end = max(end, p + 2)
                if p + 4 <= hi and quad_ok(buf, m, p, d):
                    reach.add(p + 4)
                    end = max(end, p + 4)
            p += 2
        out.append((i, end))
        i = end
    return out


def trim_zeros(buf, m, a, b, d):
    """[a, b) without the 16-bit units at its two ends that are zero in the module and in the arcade (None when nothing is left)."""
    while a + 2 <= b and not any(buf[a:a + 2]) and not any(m[a + d:a + d + 2]):
        a += 2
    while b - 2 >= a and not any(buf[b - 2:b]) and not any(m[b - 2 + d:b + d]):
        b -= 2
    return (a, b) if b > a and any(buf[a:b]) else None


def best_layout(buf, m, a, b, d):
    """The layout of the exact run [a, b) against m at +d with the fewest letter changes, as a list of (letter, count): 'b' two
    bytes as they are, 'w' two bytes turned, 'l' four bytes turned; counts are fields. A unit equal both ways takes the letter that
    saves a change. None when [a, b) is not an exact run."""
    n = (b - a) // 2
    INF = 1 << 30
    # cost[k][letter]: fewest tokens to cover the first k units, the last token being `letter`
    cost = [{'b': INF, 'w': INF, 'l': INF} for _ in range(n + 1)]
    back = [dict() for _ in range(n + 1)]
    for k in range(n):
        p = a + 2 * k
        prev = [(0, None)] if k == 0 else [(cost[k][x], x) for x in 'bwl' if cost[k][x] < INF]
        if not prev:
            continue
        if p + 2 <= b and unit_ok(buf, m, p, d):
            ba, mb = buf[p:p + 2], m[p + d:p + d + 2]
            for x, good in (('b', ba == mb), ('w', ba[::-1] == mb)):
                if not good:
                    continue
                for c, x0 in prev:
                    nc = c + (0 if x0 == x else 1)
                    if nc < cost[k + 1][x] or (nc == cost[k + 1][x] and x0 == x):
                        cost[k + 1][x] = nc
                        back[k + 1][x] = x0
        if k + 2 <= n and quad_ok(buf, m, p, d):
            for c, x0 in prev:
                nc = c + (0 if x0 == 'l' else 1)
                if nc < cost[k + 2]['l'] or (nc == cost[k + 2]['l'] and x0 == 'l'):
                    cost[k + 2]['l'] = nc
                    back[k + 2]['l'] = x0
    best = min(('b', 'w', 'l'), key=lambda x: (cost[n][x], x))
    if cost[n][best] >= INF:
        return None
    letters, k, x = [], n, best
    while k > 0:
        letters.append(x)
        x0 = back[k][x]
        k -= 2 if x == 'l' else 1
        x = x0
    letters.reverse()
    out = []
    for x in letters:
        if out and out[-1][0] == x:
            out[-1][1] += 1
        else:
            out.append([x, 1])
    return [(x, c) for x, c in out]


def rle(layout):
    """A flat layout [(letter, count)] as text: letters and field counts, 'b16w4l2'."""
    return ''.join(f'{x}{c}' for x, c in layout)


def group_text(fields, repeat):
    """A record layout [(letter, count)] repeated `repeat` times, as text: '(w2n2)410'."""
    return f'({rle(fields)}){repeat}'


def layout_items(tokens):
    """Items ('t', letter, count) for flat tokens [(letter, count)]."""
    return [('t', x, c) for x, c in tokens]


def items_text(items):
    return ''.join(f'{it[1]}{it[2]}' if it[0] == 't' else group_text(it[1], it[2]) for it in items)


def periodic_layout(buf, m, a, b, d, flat):
    """A shorter layout of the exact run [a, b) than the flat one `flat` ([(letter, count)]) when its units repeat with a period: a head
    of flat tokens, a group of up to 32 units repeated, a tail. A unit equal both ways takes the letter that the record's other
    copies need. Returns the layout items, or None when nothing is shorter. Only 'b' and 'w' fields take part in the group."""
    n = (b - a) // 2
    sets = []
    for u in range(n):
        ba, mb = buf[a + 2 * u:a + 2 * u + 2], m[a + 2 * u + d:a + 2 * u + d + 2]
        sets.append((1 if ba == mb else 0) | (2 if ba[::-1] == mb else 0))
    best, best_cost = None, len(flat)
    for period in range(1, 33):
        for head in range(min(period, n)):
            reps = (n - head) // period
            if reps < 3:
                break
            allowed = [3] * period
            for k in range(reps * period):
                allowed[k % period] &= sets[head + k]
            if not all(allowed):
                continue
            letters, prev = [], None
            for v in allowed:
                x = prev if prev and v & (1 if prev == 'b' else 2) else ('b' if v & 1 else 'w')
                letters.append(x)
                prev = x
            inner = []
            for x in letters:
                if inner and inner[-1][0] == x:
                    inner[-1][1] += 1
                else:
                    inner.append([x, 1])
            inner = [(x, c) for x, c in inner]
            lo, mid, hi = a, a + 2 * (head + reps * period), b
            h = best_layout(buf, m, lo, a + 2 * head, d) if head else []
            t = best_layout(buf, m, mid, hi, d) if mid < hi else []
            if h is None or t is None:
                continue
            cost = len(h) + len(inner) + len(t) + 1
            if cost < best_cost:
                best_cost = cost
                best = layout_items(h) + [('g', inner, reps)] + layout_items(t)
    return best


TOKEN = re.compile(r'\(((?:[a-zA-Z]\d+)+)\)(\d+)|([a-zA-Z])(\d+)')


def layout_parse(text):
    """The items of a layout text: ('t', letter, count) or ('g', [(letter, count)], repeat). A layout is tokens, a letter and a field
    count each ('b16w4'), and groups, tokens in parentheses and a repeat count ('(w2n2)410'). None when it is malformed: an unknown
    letter, a zero or missing count, a group inside a group, text left over."""
    items, at = [], 0
    while at < len(text):
        m = TOKEN.match(text, at)
        if not m:
            return None
        if m.group(1) is not None:
            inner = [(x, int(c)) for x, c in re.findall(r'([a-zA-Z])(\d+)', m.group(1))]
            if int(m.group(2)) == 0 or any(x not in LAYOUT_SIZES or c == 0 for x, c in inner):
                return None
            items.append(('g', inner, int(m.group(2))))
        else:
            if m.group(3) not in LAYOUT_SIZES or int(m.group(4)) == 0:
                return None
            items.append(('t', m.group(3), int(m.group(4))))
        at = m.end()
    return items or None


def layout_sizes(text):
    """(PS1 bytes, arcade bytes) a layout text covers; None when malformed."""
    items = layout_parse(text)
    if items is None:
        return None
    ps1 = arc = 0
    for it in items:
        if it[0] == 't':
            ps1 += LAYOUT_SIZES[it[1]][0] * it[2]
            arc += LAYOUT_SIZES[it[1]][1] * it[2]
        else:
            ps1 += it[2] * sum(LAYOUT_SIZES[x][0] * c for x, c in it[1])
            arc += it[2] * sum(LAYOUT_SIZES[x][1] * c for x, c in it[1])
    return ps1, arc


def palette_row(words, flags):
    """The 32 PS1 bytes of a palette row from the arcade palette's first 15 words (screens.palette_bytes' rule): colour 0 is 0 (transparent);
    colours 1 to 15 are the words' 12-bit colours (brightness nibble, bits 12 to 15, ignored; R bits 8 to 11, G 4 to 7, B 0 to 3) as
    R * 2 | (G * 2) << 5 | (B * 2) << 10; a black result is 0x8000 unless PAL_BLACK_ZERO; with PAL_BIT15 bit 15 is set on all of them."""
    out = [0]
    for w in words:
        c = (w >> 8 & 15) << 1 | (w >> 4 & 15) << 6 | (w & 15) << 11
        if not c and not flags & PAL_BLACK_ZERO:
            c = 0x8000
        out.append(c | (0x8000 if flags & PAL_BIT15 else 0))
    return struct.pack('<16H', *out)


def field_bytes(letter, arc):
    """The PS1 bytes of one field from its arcade bytes (2 for b, w, n, f and q, 4 for l, 1 for e, a palette row's 32 for k, z, K and Z: they
    read the first 30 of 32 arcade bytes, 15 big-endian words; palette_row says how). 'b' keeps two bytes, 'w' turns
    two bytes, 'l' turns four bytes; 'n' is the low byte of a 16-bit value whose high byte is zero; 'f' a flags byte from a 16-bit
    value (bit 15 becomes bit 7, bits 0 to 6 stay, nothing else may be set); 'q' a byte that is the 16-bit value divided by 0x18
    (a multiple); 'e' a 32-bit little-endian value from one byte. ValueError when the value does not fit."""
    if letter == 'b':
        return bytes(arc)
    if letter == 'w':
        return bytes(arc[::-1])
    if letter == 'l':
        return bytes(arc[::-1])
    if letter == 'e':
        return bytes([arc[0], 0, 0, 0])
    if letter in PALETTE_FLAGS:
        if len(arc) < 30:
            raise ValueError('a palette row needs 30 bytes')
        return palette_row(struct.unpack('>15H', bytes(arc[:30])), PALETTE_FLAGS[letter])
    v = (arc[0] << 8) | arc[1]
    if letter == 'n' and v <= 255:
        return bytes([v])
    if letter == 'f' and not v & 0x7f80:
        return bytes([((v >> 8) & 0x80) | (v & 0x7f)])
    if letter == 'q' and v % 24 == 0 and v // 24 <= 255:
        return bytes([v // 24])
    raise ValueError(f'{v:#x} does not fit the letter {letter}')


def build_piece(m, source, text):
    """The module's bytes of one piece: the arcade program m from `source`, read as the layout text says (field_bytes' letters).
    ValueError when it does not fit the program, is malformed or a value does not fit its letter."""
    items = layout_parse(text)
    if items is None:
        raise ValueError('malformed layout')
    if source < 0 or source & 1 or source + layout_sizes(text)[1] > len(m):
        raise ValueError('outside the program')
    out = bytearray()
    at = source
    for it in items:
        for _ in range(it[2] if it[0] == 'g' else 1):
            for x, c in (it[1] if it[0] == 'g' else [(it[1], it[2])]):
                n = LAYOUT_SIZES[x][1]
                for _k in range(c):
                    out += field_bytes(x, m[at:at + n])
                    at += n
    return bytes(out)


def run_evidence(m, source, size, stats):
    """Bits of information in the arcade values the run reads (each distinct non-zero 16-bit value once), minus the log2 of the
    places it could have been found: program_map.segment_stats' measure."""
    seen = set()
    ev = 0.0
    for k in range(0, size, 2):
        u = m[source + k:source + k + 2]
        if u != b'\0\0' and u not in seen:
            seen.add(u)
            ev += stats.bits(u)
    return ev - stats.places


def pieces_of(buf, m, cands, stats, claimed=None):
    """The pieces of one module: [(offset, size, arcade source, layout text, evidence)] sorted by offset. cands are the stretches
    that agree with the arcade, (start, end, delta) in the module's offsets, delta added to an offset gives the arcade address.
    claimed (bytearray, 1 = taken) keeps a piece off bytes another class holds."""
    out = []
    taken = bytearray(len(buf)) if claimed is None else bytearray(claimed)
    for s, e, d in sorted(cands):
        for a, b in exact_runs_in(buf, m, s, e, d):
            t = trim_zeros(buf, m, a, b, d)
            if t is None:
                continue
            a, b = t
            if any(taken[a:b]):
                continue
            for a2, b2 in exact_runs_in(buf, m, a, b, d):
                if b2 - a2 < MIN_PIECE:
                    continue
                ev = run_evidence(m, a2 + d, b2 - a2, stats)
                if ev < MIN_EVIDENCE:
                    continue
                lay = best_layout(buf, m, a2, b2, d)
                if lay is None:
                    continue
                items = periodic_layout(buf, m, a2, b2, d, lay) if len(lay) > 6 else None
                out.append((a2, b2 - a2, a2 + d, items_text(items) if items else rle(lay), ev))
                taken[a2:b2] = b'\1' * (b2 - a2)
    return sorted(out)


def template_fields(t):
    """The layout [(letter, count)] of a record template ('wwnn' is [('w', 2), ('n', 2)])."""
    out = []
    for x in t:
        if out and out[-1][0] == x:
            out[-1][1] += 1
        else:
            out.append([x, 1])
    return [(x, c) for x, c in out]


def template_sizes(t):
    """(PS1 bytes, arcade bytes) of one record of the template."""
    return sum(LAYOUT_SIZES[x][0] for x in t), sum(LAYOUT_SIZES[x][1] for x in t)


def record_expected(rec, t):
    """The arcade bytes that the PS1 record `rec` (bytes) is made from under template t, the inverse of field_bytes; None when the
    record cannot be made by it."""
    out = bytearray()
    k = 0
    for x in t:
        n = LAYOUT_SIZES[x][0]
        f = rec[k:k + n]
        k += n
        if x == 'b':
            out += f
        elif x in 'wl':
            out += f[::-1]
        elif x == 'n':
            out += bytes([0, f[0]])
        elif x == 'f':
            out += bytes([f[0] & 0x80, f[0] & 0x7f])
        elif x == 'q':
            out += (f[0] * 24).to_bytes(2, 'big')
        else:
            if any(f[1:]):
                return None
            out += f[:1]
    return bytes(out)


def template_search(items, m, t, taken):
    """Where records of template t lie in the free bytes: [(key, start, end, source)] (module offsets; source the arcade address of the
    first record). items: [(key, buf)]; taken: {key: bytearray, 1 = not free}, updated with the runs found. A run needs WINDOW records
    that agree to be found (an index of the windows' arcade bytes is looked up in the program), is extended record by record both
    ways while the records agree, and the longest runs are taken first."""
    p, a = template_sizes(t)
    span = WINDOW * p
    want = {}
    for key, buf in items:
        free = taken[key]
        pre = [0]
        for b in free:
            pre.append(pre[-1] + b)
        for s in range(0, len(buf) - span + 1, 2):
            if pre[s + span] - pre[s] or not any(buf[s:s + span]):
                continue
            parts = [record_expected(buf[s + r * p:s + (r + 1) * p], t) for r in range(WINDOW)]
            if None in parts:
                continue
            k = b''.join(parts)
            if len(set(k)) >= 3:
                want.setdefault(k, []).append((key, s))
    hits = []
    L = WINDOW * a
    for j in range(0, len(m) - L + 1, 2):
        k = m[j:j + L]
        if k in want:
            hits += [(key, s, j) for key, s in want[k]]
    bufs = dict(items)
    runs = {}
    for key, s, j in hits:
        buf, free = bufs[key], taken[key]
        e, je = s + span, j + L
        while e + p <= len(buf) and not any(free[e:e + p]) and je + a <= len(m) and record_expected(buf[e:e + p], t) == m[je:je + a]:
            e, je = e + p, je + a
        b, jb = s, j
        while b - p >= 0 and not any(free[b - p:b]) and jb - a >= 0 and record_expected(buf[b - p:b], t) == m[jb - a:jb]:
            b, jb = b - p, jb - a
        if (key, b) not in runs or runs[(key, b)][0] < e:
            runs[(key, b)] = (e, jb)
    out = []
    for (key, b), (e, jb) in sorted(runs.items(), key=lambda kv: (-(kv[1][0] - kv[0][1]), kv[0])):
        free = taken[key]
        if e - b < MIN_RECORDS * p or any(free[b:e]):
            continue
        free[b:e] = b'\1' * (e - b)
        out.append((key, b, e, jb))
    return sorted(out)


def template_pieces(buf, m, cands, stats, claimed):
    """The converted pieces of one module from candidates (start, end, source, template): the maximal runs of records inside each that
    still agree with the arcade (the second side's bytes are checked here), of at least MIN_RECORDS records and MIN_EVIDENCE_CONV bits,
    off the bytes claimed. [(offset, size, source, layout text, evidence)]."""
    out = []
    taken = bytearray(claimed)
    for s, e, src, t in sorted(cands):
        p, a = template_sizes(t)
        fields = template_fields(t)
        r, n = 0, (e - s) // p
        while r < n:
            if record_expected(buf[s + r * p:s + (r + 1) * p], t) != m[src + r * a:src + (r + 1) * a]:
                r += 1
                continue
            r2 = r
            while r2 < n and record_expected(buf[s + r2 * p:s + (r2 + 1) * p], t) == m[src + r2 * a:src + (r2 + 1) * a]:
                r2 += 1
            lo, hi = s + r * p, s + r2 * p
            ev = run_evidence(m, src + r * a, (r2 - r) * a, stats)
            if r2 - r >= MIN_RECORDS and ev >= MIN_EVIDENCE_CONV and not any(taken[lo:hi]):
                out.append((lo, hi - lo, src + r * a, group_text(fields, r2 - r), ev))
                taken[lo:hi] = b'\1' * (hi - lo)
            r = r2
    return sorted(out)


def search_tails(tails, m, stats):
    """The stretches of the tails that agree with the arcade program m (program_map's search): {key: [(start, end, delta)]} in each
    tail's own module offsets. tails: [(key, offset of the tail's first byte in its module, bytes, masked)]; the bytes of the blocks
    that are built otherwise (masked: [(start, end)] in module offsets) are zero in the bytes and kept out of the result. The tails are put one after another in one buffer (each keeping its offset modulo 4, so
    that 32-bit fields stay aligned) and searched once."""
    q = bytearray()
    where = []
    claimed = bytearray()
    for key, fo, data, masked in tails:
        while len(q) % 4 != fo % 4:
            q.append(0)
        where.append((key, fo, len(q), len(q) + len(data)))
        claimed += bytes(len(q) - len(claimed))
        q += data
        q += SENTINEL
        claimed += bytes(len(q) - len(claimed))
        claimed[len(q) - len(SENTINEL):len(q)] = b'\1' * len(SENTINEL)
        for a, b in masked:
            lo, hi = max(a, fo) - fo + where[-1][2], max(b, fo) - fo + where[-1][2]
            claimed[lo:hi] = b'\1' * (hi - lo)
    q = bytes(q)
    anchors = P.seed_anchors(q, m)
    segs = P.find_segments(q, m, stats, anchors)
    kept = P.select_segments_data(q, m, segs, claimed, stats)
    out = {key: [] for key, _fo, _s, _e in where}
    starts = [w[2] for w in where]
    for s, e, d, _ev, _ma, _mi, _lay in kept:
        k = bisect.bisect_right(starts, s) - 1
        key, fo, qs, qe = where[k]
        if e > qe or P.segment_is_ramp(q, s, e):
            continue
        out[key].append((fo + s - qs, fo + e - qs, d + qs - fo))
    return out


def pointer_class(w, load, size, code_end, funcs, prog):
    """The class of a pointer word: 'ptr_self_code' (a function start of the module), 'ptr_self_label' (inside the module's code),
    'ptr_self_data'; 'ptr_prog_code' (a function of the resident program), 'ptr_prog_data' (its data), 'ptr_prog_other'; or
    'ptr_overlay'. funcs: module offsets of the module's functions; prog: dict(funcs=set of addresses, code=(lo, hi), data=(lo, hi))."""
    if load <= w < load + size:
        rel = w - load
        if rel < code_end:
            return 'ptr_self_code' if rel in funcs else 'ptr_self_label'
        return 'ptr_self_data'
    if w >= P.MODULE_START:
        return 'ptr_overlay'
    if w in prog['funcs']:
        return 'ptr_prog_code'
    if prog['data'][0] <= w < prog['data'][1]:
        return 'ptr_prog_data'
    return 'ptr_prog_other'


def find_pointers(buf, free, load, code_end, funcs, prog, symbols):
    """[(offset, word, class)] of the pointer words in the free bytes (free: bytearray, 1 = unclaimed). A word counts when it is
    4-aligned, a RAM address, and either an address inside the module, a function or a symbol of the program, or next to another
    such word (program_map's strong_pointers rule)."""
    size = len(buf)
    cands = set()
    for i in range(0, size - 3, 4):
        if all(free[i:i + 4]):
            w = struct.unpack_from('<I', buf, i)[0]
            if P.RAM_LO <= w <= P.RAM_HI and not w & 3:
                cands.add(i)
    out = []
    for i in sorted(cands):
        w = struct.unpack_from('<I', buf, i)[0]
        own = load <= w < load + size
        known = own or w in prog['funcs'] or w in symbols
        if known or (i - 4) in cands or (i + 4) in cands:
            out.append((i, w, pointer_class(w, load, size, code_end, funcs, prog)))
    return out


def reloc_rows(pointers, load, size):
    """[(offset, target)] for the table: an offset inside the module is stored as that offset, any other address as it is."""
    return [(o, w - load if load <= w < load + size else w) for o, w, _c in pointers]


def model_build(m, size, load, pieces, relocs):
    """What module_data.c builds for one file, as (bytes, known): the pieces from the arcade program, then the pointer words. The
    module is `size` zero bytes first; known is 1 for every byte a piece or a word wrote. ValueError when a piece does not fit."""
    img, known = bytearray(size), bytearray(size)
    for off, n, src, text, *_ in pieces:
        if layout_sizes(text) is None or layout_sizes(text)[0] != n or off < 0 or off + n > size:
            raise ValueError('malformed piece')
        img[off:off + n] = build_piece(m, src, text)
        known[off:off + n] = b'\1' * n
    for off, target in relocs:
        if off < 0 or off + 4 > size or off & 3:
            raise ValueError('malformed relocation')
        w = load + target if target < RELATIVE else target
        img[off:off + 4] = struct.pack('<I', w)
        known[off:off + 4] = b'\1\1\1\1'
    return bytes(img), bytes(known)


def classify(mod, load, ce, spans, pieces, conv, cands, pointers):
    """One class per byte of the module. Order: the code, the five blocks, the pieces, the pointer words, the stretches that
    almost agree with the arcade ('near'), cell lists, text, then zero and other."""
    n = len(mod)
    cls = [''] * n

    def put(a, b, c):
        for k in range(max(a, 0), min(b, n)):
            if not cls[k]:
                cls[k] = c
    put(0, ce, 'code')
    for a, b, c in spans:
        put(a, b, c)
    for off, size, *_ in pieces:
        put(off, off + size, 'arcade')
    for off, size, *_ in conv:
        put(off, off + size, 'arcade_conv')
    for o, _w, c in pointers:
        put(o, o + 4, c)
    for s, e, _d in cands:
        for k in range(s, min(e, n)):
            if not cls[k] and mod[k]:
                cls[k] = 'near'
    free = bytes(1 if not cls[k] else 0 for k in range(n))
    for s, e, *_ in P.find_cell_chains(mod, free):
        put(s, e, 'celllist')
    for a, b in P.text_runs(mod):
        if all(not cls[k] for k in range(a, b)):
            put(a, b, 'text')
    for k in range(n):
        if not cls[k]:
            cls[k] = 'zero' if mod[k] == 0 else 'other'
    return cls


def class_counts(cls):
    c = collections.Counter(cls)
    return {k: c.get(k, 0) for k in CLASSES}


# ---------------------------------------------------------------- the descriptor and the C table

def cid(name, slot):
    """The C identifier part of a module: the file name, and the slot for the modules other than slot 4 or 5."""
    return name.removesuffix('.PAC').lower() + ('' if slot in (4, 5) else f'_s{slot:x}')


def descriptor(i):
    """What the C table holds for one module (an entry of analyse_all's result)."""
    return dict(file=i['name'], slot=i['slot'], load=i['load'], size=len(i['mod']),
                pieces=[(o, n, s, t) for o, n, s, t, *_ in i['pieces']], relocs=list(i['relocs']))


def c_table(descs):
    """The text of module_data_table.c: per file the pieces and the pointer words. A second side whose pieces or words equal its
    first side's shares its arrays."""
    out = ['/* GENERATED by `python3 port/arcade/module_data.py --disc DISC --arcade SET --c-table`; do not edit.',
           ' * For each fighter file (the code module of table 0, slot 4 or 5) and each file of the screen and stage modules (END, CDEMO, CONT, DEMO,',
           ' * SELECT, SELECTA, BOSS, RDM, GDEMO, STAGE: their slots), the pieces of the module that are an exact run of the arcade',
           ' * program (module offset, size, arcade address, layout: a letter and a field count each, b = 16-bit as it is, w = 16-bit turned,',
           ' * l = 32-bit turned, k z K Z = palette rows), and the pointer words (module offset, target: below 0x80000000 an offset in the module, to which the',
           ' * link address is added, else an address). Only offsets, addresses and layouts: no byte of either game. */',
           '#include "module_data.h"', '']
    owner_p, owner_r = {}, {}
    names = []
    for d in descs:
        names.append(d['file'])
        key_p, key_r = tuple(d['pieces']), tuple(d['relocs'])
        if key_p in owner_p:
            d['_pieces'] = owner_p[key_p]
        else:
            d['_pieces'] = owner_p[key_p] = f'pieces_{cid(d["file"], d["slot"])}' if d['pieces'] else 'NULL'
            if d['pieces']:
                out.append(f'static const struct arcade_module_piece {d["_pieces"]}[] = {{')
                out += [f'    {{0x{o:x}u, {n}u, 0x{s:x}u, "{t}"}},' for o, n, s, t in d['pieces']]
                out += ['};', '']
        if key_r in owner_r:
            d['_relocs'] = owner_r[key_r]
        else:
            d['_relocs'] = owner_r[key_r] = f'relocs_{cid(d["file"], d["slot"])}' if d['relocs'] else 'NULL'
            if d['relocs']:
                out.append(f'static const struct arcade_module_reloc {d["_relocs"]}[] = {{')
                out += [f'    {{0x{o:x}u, 0x{t:x}u}},' for o, t in d['relocs']]
                out += ['};', '']
    out.append('const struct arcade_module_data arcade_module_data[] = {')
    for d in descs:
        out.append(f'    {{"{d["file"]}", {d["slot"]}u, 0x{d["load"]:x}u, {d["size"]}u, {d["_pieces"]}, {len(d["pieces"])}u, '
                   f'{d["_relocs"]}, {len(d["relocs"])}u}},')
    out += ['};', 'const unsigned arcade_module_data_count = sizeof arcade_module_data / sizeof arcade_module_data[0];', '']
    return '\n'.join(out)


# ---------------------------------------------------------------- reading the inputs

def load_inventory_functions(text, name, load, size):
    """Module offsets of the functions that the inventory lists for archive `name` inside [load, load + size)."""
    out = set()
    for line in text.splitlines():
        p = line.split('\t')
        if len(p) >= 4 and p[0] == name and re.fullmatch(r'[0-9a-f]{8}', p[2]):
            a = int(p[2], 16)
            if load <= a < load + size:
                out.add(a - load)
    return out


def read_modules(disc, only=None):
    """[dict(key, name, slot, side, mod, load, extra)] of the code modules of the fighter files on the disc, in name order: the module of
    table 0 slot 4 or 5 of each file, and the other code chunks some files carry (EXTRA_LOAD). A module that two files carry with the same
    bytes is listed for each."""
    out = []
    with open(disc, 'rb') as f:
        files, sec = F.iso_files(f)
        for p in sorted(x for x in files if x.startswith('/PAC/PL') and x.endswith('.PAC')):
            name = p.split('/')[-1]
            if only and name.removesuffix('.PAC') != only.upper().removesuffix('.PAC'):
                continue
            lba, size = files[p]
            parts = F.split_file(sec(lba, (size + 2047) // 2048)[:size])
            if isinstance(parts, str):
                print(f'{name}: {parts}')
                continue
            mod, _tiles, side, others = parts
            slot = 4 if side == 1 else 5
            out.append(dict(key=entry_key(name, slot), name=name, slot=slot, side=side, mod=mod, load=LOAD[side], extra=False))
            for oslot, data in sorted(others):
                if oslot in EXTRA_LOAD:
                    out.append(dict(key=entry_key(name, oslot), name=name, slot=oslot, side=2 if oslot in FIRST_OF else 1, mod=data,
                                    load=EXTRA_LOAD[oslot], extra=True))
    return out


def entry_key(name, slot):
    return f'{name}:{slot:x}'


def entry_label(e):
    return e['name'] if not e['extra'] else f'{e["name"]} slot {e["slot"]:#x}'


def entry_spans(e, fr_table, sq_table):
    """(spans, built) of a module: the five blocks for the main module (from the tables when both cover the file), the positions for the
    extra chunk that holds them, none for other extras."""
    name, mod = e['name'], e['mod']
    if e['extra']:
        fr = fr_table.get(name)
        if fr and not fr['same_chunk'] and fr['positions_slot'] == e['slot']:
            return [(fr['positions'], fr['positions'] + fr['positions_room'], 'positions')], True
        return [], True
    if name in fr_table and name in sq_table:
        return spans_from_tables(fr_table[name], sq_table[name], len(mod)), True
    return structural_spans(mod, e['load']), False


def analyse_all(entries, m, fr_table, sq_table, inventory_text, prog, symbols, log=None):
    """Everything per module: spans, code end, pieces, pointers, classes. Returns {key: dict}. A module whose bytes another one of the
    same slot has is analysed once; a second-side module is searched through its first side (its pieces are that module's at the same
    offsets, plus the shift of its code when the code differs in length)."""
    stats = P.UnitStats(m)
    info = {}
    canon, same = {}, {}
    for e in entries:
        ident = (e['slot'], e['mod'])
        if ident in canon:
            same[e['key']] = canon[ident]
            continue
        canon[ident] = e['key']
        spans, built = entry_spans(e, fr_table, sq_table)
        if spans is None:
            raise SystemExit(f'{e["key"]}: the blocks of the fighter format are not found')
        info[e['key']] = dict(e, ce=F.code_end(e['mod']), spans=spans, built=built)
    # one search over the first sides (and the modules with no twin)
    first = {(stem_of(i['name']), i['slot']): k for k, i in info.items() if i['side'] == 1}
    tails, twin = [], {}
    for k, i in info.items():
        g = first.get((stem_of(i['name']), FIRST_OF.get(i['slot'])))
        if i['side'] == 2 and g:
            shift = i['ce'] - info[g]['ce']
            if shift % 4 == 0 and len(i['mod']) - len(info[g]['mod']) == shift:
                twin[k] = (g, shift)
                continue
        mask = bytearray(i['mod'])
        for a, b, _c in i['spans']:
            mask[a:b] = bytes(b - a)
        tails.append((k, i['ce'], bytes(mask[i['ce']:]), [(a, b) for a, b, _c in i['spans']]))
    cands = search_tails(tails, m, stats)
    for k, (g, shift) in twin.items():
        cands[k] = [(s + shift, e + shift, d - shift) for s, e, d in cands[g]]
    # the pieces that are exact runs, then (in the bytes they leave) the converted records
    exact, taken = {}, {}
    for k, i in info.items():
        mod, ce = i['mod'], i['ce']
        t = bytearray(len(mod))
        t[:ce] = b'\1' * ce
        for a, b, _c in i['spans']:
            t[a:b] = b'\1' * (b - a)
        exact[k] = pieces_of(mod, m, cands[k], stats, t)
        for off, n, *_ in exact[k]:
            t[off:off + n] = b'\1' * n
        taken[k] = t
    items = [(k, i['mod']) for k, i in info.items() if k not in twin]
    tcands = {k: [] for k in info}
    work = {k: bytearray(taken[k]) for k, _b in items}
    for t in TEMPLATES:
        for key, b, e, src in template_search(items, m, t, work):
            tcands[key].append((b, e, src, t))
    for k, (g, shift) in twin.items():
        tcands[k] = [(b + shift, e + shift, src, t) for b, e, src, t in tcands[g]]
    for k, i in info.items():
        mod, ce = i['mod'], i['ce']
        conv = template_pieces(mod, m, tcands[k], stats, taken[k])
        pieces = sorted(exact[k] + conv)
        free = bytearray(1 if not t else 0 for t in taken[k])
        for off, n, *_ in conv:
            free[off:off + n] = bytes(n)
        funcs = load_inventory_functions(inventory_text, i['name'], i['load'], len(mod))
        pointers = find_pointers(mod, free, i['load'], ce, funcs, prog, symbols)
        relocs = reloc_rows(pointers, i['load'], len(mod))
        cls = classify(mod, i['load'], ce, i['spans'], exact[k], conv, cands[k], pointers)
        i.update(cands=cands[k], pieces=pieces, pointers=pointers, relocs=relocs, cls=cls, same=[])
        if log:
            log(k)
    for k, g in same.items():
        info[g]['same'].append(k)
    entry_of = {e['key']: e for e in entries}
    for k, g in same.items():
        info[k] = dict(info[g], **{x: entry_of[k][x] for x in ('key', 'name', 'slot', 'side', 'load')}, same_as=g)
    return info


def program_context():
    symbols = P.parse_symbols((ROOT / 'ps1/src/symbols.ld').read_text())
    funcs = P.parse_inventory((ROOT / 'ps1/inventory/game.tsv').read_text()) + P.parse_inventory((ROOT / 'ps1/inventory/library.tsv').read_text())
    prog = dict(funcs={a for a, _s, _n in funcs}, code=(P.CODE_START, P.DATA_START), data=(P.DATA_START, P.DATA_END))
    return prog, set(symbols)


# ---------------------------------------------------------------- the screen and stage modules

# The code chunks of the screen modules (END, CDEMO, CONT, DEMO, SELECT, SELECTA, BOSS/RDM/GDEMO) and of the 20 stage files are analysed
# as the fighter modules are, with data only: there is no code (code_pins_table.c's ranges), no block of the fighter format, and the bytes
# the other builders make (screen_lists, texts, quotes, talk, stage) are kept out. What is left is searched for palette rows (the letters
# k, z, K, Z), runs of the arcade program (b, w, l and the converted records) and pointer words. A module that many files carry (the 21
# endings, the 42 continue screens) is one image: analysed once, one row for each file that carries it, the rows sharing their arrays.

MIN_PALETTE_EVIDENCE = MIN_EVIDENCE        # bits a palette run must carry (the information of its distinct non-zero words minus log2 of the places)
PALETTE_STRIDE = 32


def parse_code_ranges(text):
    """{image name: [(offset, size)]} from port/arcade/code_pins_table.c."""
    blocks = {int(m.group(1)): [(int(a, 16), int(b, 16)) for a, b in re.findall(r'\{(0x[0-9a-f]+), (0x[0-9a-f]+)\}', m.group(2))]
              for m in re.finditer(r'ranges_(\d+)\[\] = \{(.*?)\};', text, re.S)}
    out = {}
    for m in re.finditer(r'\{"([\w.]+)", 0x[0-9a-f]+,\s*\{[^}]*\},\s*\{[^}]*\},\s*(?:ranges_(\d+)|NULL), \d+\}', text):
        out[m.group(1)] = blocks[int(m.group(2))] if m.group(2) else []
    return out


def parse_loader_regions(screen_lists, texts, quotes, talk, stages):
    """{(file, slot): [(start, end)]} of the module bytes that the loader's other builders make or keep (the texts of their tables): the
    chains of cell lists and position runs of screen_lists_table.c (the own lists in them are the disc's), the strings of texts_table.c,
    the quote tables of quotes_table.c, the scripts of talk_table.c and the layer maps of stages_table.c."""
    out = collections.defaultdict(list)

    def num(x):
        x = x.rstrip('u')
        return 1 if x == 'ARCADE_SCREEN_LISTS_PAL_SPLIT' else int(x, 0)
    cs = screen_lists[screen_lists.index('arcade_screen_lists_chains[]'):screen_lists.index('arcade_screen_lists_uses[]')]
    chains = [tuple(num(x) for x in m.groups()) for m in re.finditer(r'\{(\w+), (\w+), (\w+), (\w+), (\w+), (\w+), (\w+)\},', cs)]
    for f, slot, pos, lst, ch in re.findall(r'\{"(\w+\.PAC)", (\d+)u, (0x\w+)u, (0x\w+)u, (\d+)u, \d+u, 0x\w+u, \d+u, \d+u\}', screen_lists):
        _rules, ps, ls, *_ = chains[int(ch)]
        out[f, int(slot)] += [(int(pos, 16), int(pos, 16) + ps), (int(lst, 16), int(lst, 16) + ls)]
    arrays = {m.group(1): [(int(o, 16), int(o, 16) + int(sz)) for o, _s, _c, sz in
                           re.findall(r'\{(0x[0-9a-f]+)u, (0x[0-9a-f]+)u, (\d+)u, (\d+)u, ARCADE_TEXT_\w+\}', m.group(2))]
              for m in re.finditer(r'static const struct arcade_text_string (\w+)\[\] = \{(.*?)\};', texts, re.S)}
    for f, slot, _size, arr, _n in re.findall(r'\{"(\w+\.PAC)", (\d+)u, (\d+)u, (\w+), (\d+)u\}', texts):
        out[f, int(slot)] += arrays[arr]
    qslot = int(re.search(r'arcade_quotes = \{(\d+)u', quotes).group(1))
    for f, off, cnt, lst, lroom, st, sroom in re.findall(
            r'\{"(\w+\.PAC)", 0x[0-9a-f]+, 0x[0-9a-f]+, (0x[0-9a-f]+), (\d+)u, (0x[0-9a-f]+), (0x[0-9a-f]+), (0x[0-9a-f]+), (0x[0-9a-f]+), '
            r'0x[0-9a-f]+, 0x[0-9a-f]+\}', quotes):
        off, cnt = int(off, 16), int(cnt)
        out[f, qslot] += [(off, off + 4 * cnt), (int(lst, 16), int(lst, 16) + int(lroom, 16)), (int(st, 16), int(st, 16) + int(sroom, 16))]
    tslot = int(re.search(r'arcade_talk = \{\d+, \d+, \d+, (0x[0-9a-f]+|\d+),', talk).group(1), 0)
    for f, script, recs in re.findall(r'\{"(\w+\.PAC)", 0x[0-9a-f]+, \d+, \{[^}]*\}, (0x[0-9a-f]+), (\d+)\}', talk):
        out[f, tslot].append((int(script, 16), int(script, 16) + 6 * int(recs)))
    for f, body in re.findall(r'\{"(STAGE\w+\.PAC)", \d+u, \{[^}]*\},\s*\{[^}]*\},\s*\{\s*((?:\{[^{}]*\},\s*){3})\}', stages):
        for mp, across, down in re.findall(r'\{(\d+)u, (\d+)u, (\d+)u,', body):
            out[f, 6].append((int(mp), int(mp) + 4 * int(across) * int(down)))
    return out


def merged_ranges(ranges, size):
    """Sorted, joined [(start, end)] clipped to [0, size)."""
    out = []
    for a, b in sorted((max(a, 0), min(b, size)) for a, b in ranges):
        if b <= a:
            continue
        if out and a <= out[-1][1]:
            out[-1] = (out[-1][0], max(out[-1][1], b))
        else:
            out.append((a, b))
    return out


def palette_evidence(m, source, rows, stats):
    """Bits of information in the 15 words of each of `rows` consecutive arcade palettes from `source` (each distinct non-zero word once),
    minus the log2 of the places it could have been found."""
    seen = set()
    ev = 0.0
    for r in range(rows):
        for k in range(15):
            u = m[source + 32 * r + 2 * k:source + 32 * r + 2 * k + 2]
            if u != b'\0\0' and u not in seen:
                seen.add(u)
                ev += stats.bits(u)
    return ev - stats.places


def find_palette_runs(buf, taken, options, m, stats):
    """The palette rows of a module that are the arcade's: [(offset, size, arcade source, layout text, evidence)] sorted by offset. A row is 32
    bytes at an even offset in untaken bytes whose colour 0 is 0 and that one arcade palette (15 words at an address) gives under one of the
    four palette letters; consecutive rows of the module that are consecutive palettes of the arcade (32 bytes apart) with the same letter are
    one run (the longest first, the lowest address and letter among equals). A run carries MIN_PALETTE_EVIDENCE bits. options(row) gives the
    row's [(address, flags)] (screens.palette_options). taken (bytearray, 1 = not free) is updated with the runs."""
    n = len(buf)
    opts = {}
    for o in range(0, n - PALETTE_STRIDE + 1, 2):
        if buf[o] or buf[o + 1] or any(taken[o:o + PALETTE_STRIDE]) or not any(buf[o + 2:o + PALETTE_STRIDE]):
            continue
        row = bytes(buf[o:o + PALETTE_STRIDE])
        got = options(row)
        if got:
            opts[o] = set(got)
    out = []
    for o in sorted(opts):
        if any(taken[o:o + PALETTE_STRIDE]):
            continue
        best = (0, None)
        for a, flags in sorted(opts[o], key=lambda t: (t[0], t[1])):
            k = 1
            while (o + PALETTE_STRIDE * k) in opts and (a + PALETTE_STRIDE * k, flags) in opts[o + PALETTE_STRIDE * k] \
                    and not any(taken[o + PALETTE_STRIDE * k:o + PALETTE_STRIDE * (k + 1)]):
                k += 1
            if k > best[0]:
                best = (k, (a, flags))
        if best[1] is None:
            continue
        k, (a, flags) = best
        ev = palette_evidence(m, a, k, stats)
        if ev < MIN_PALETTE_EVIDENCE:
            continue
        letter = {0: 'k', PAL_BLACK_ZERO: 'z', PAL_BIT15: 'K', PAL_BIT15 | PAL_BLACK_ZERO: 'K'}[flags]   # Z builds what K builds: K is the letter
        out.append((o, PALETTE_STRIDE * k, a, f'{letter}{k}', ev))
        taken[o:o + PALETTE_STRIDE * k] = b'\1' * (PALETTE_STRIDE * k)
    return sorted(out)


def screen_classes(mod, claimed, loader, pal, exact, conv, pointers):
    """One class per byte of a screen module: 'code' (claimed as code), 'loader' (bytes the other builders make), 'palette', 'arcade',
    'arcade_conv', 'ptr', then 'zero' for the rest that is zero and 'left'. claimed: (code, loader) bytearrays."""
    code, load = claimed
    cls = ['left'] * len(mod)
    for k in range(len(mod)):
        if code[k]:
            cls[k] = 'code'
        elif load[k]:
            cls[k] = 'loader'
        elif not mod[k]:
            cls[k] = 'zero'
    for lst, name in ((pal, 'palette'), (exact, 'arcade'), (conv, 'arcade_conv')):
        for off, n, *_ in lst:
            cls[off:off + n] = [name] * n
    for o, _w, _c in pointers:
        cls[o:o + 4] = ['ptr'] * 4
    return cls


def analyse_screens(entries, m, code_ranges, loader, prog, symbols):
    """Everything per image of the screen and stage modules. entries: [dict(image, load, mod, files, slot)]; code_ranges: {image: [(offset,
    size)]}; loader: parse_loader_regions' result. Returns {image: dict(entry..., pieces, relocs, cls, palettes, exact, conv, pointers)}.
    The pieces are the palette runs, the exact runs and the converted records of the bytes that are neither code nor made by the loader."""
    stats = P.UnitStats(m)
    import screens as SC
    pindex = SC.PaletteIndex(m)

    def options(row):
        return SC.palette_options(m, row, pindex.find)
    info = {}
    for e in entries:
        mod = e['mod']
        n = len(mod)
        code = bytearray(n)
        for o, s in code_ranges[e['image']]:
            code[o:o + s] = b'\1' * min(s, n - o)
        load = bytearray(n)
        for f in e['files']:
            for a, b in merged_ranges(loader.get((f, e['slot']), []), n):
                load[a:b] = b'\1' * (b - a)
        taken = bytearray(a | b for a, b in zip(code, load))
        pal = find_palette_runs(mod, taken, options, m, stats)
        info[e['image']] = dict(e, code=code, loader=load, taken=taken, palettes=pal)
    tails = []
    for k, i in info.items():
        mod = i['mod']
        z = bytearray(mod)
        masked = merged_ranges([(a, b) for a, b in runs_of_ones(i['taken'])], len(mod))
        for a, b in masked:
            z[a:b] = bytes(b - a)
        tails.append((k, 0, bytes(z), masked))
    cands = search_tails(tails, m, stats)
    work = {}
    for k, i in info.items():
        mod = i['mod']
        t = bytearray(i['taken'])
        i['exact'] = pieces_of(mod, m, cands[k], stats, t)
        for off, n, *_ in i['exact']:
            t[off:off + n] = b'\1' * n
        i['after_exact'] = t
        work[k] = bytearray(t)
    items = [(k, i['mod']) for k, i in info.items()]
    tcands = {k: [] for k in info}
    for t in TEMPLATES:
        for key, b, e, src in template_search(items, m, t, work):
            tcands[key].append((b, e, src, t))
    for k, i in info.items():
        mod = i['mod']
        taken = i['after_exact']
        i['conv'] = template_pieces(mod, m, tcands[k], stats, taken)
        free = bytearray(1 if not x else 0 for x in taken)
        for off, n, *_ in i['conv']:
            free[off:off + n] = bytes(n)
        i['pointers'] = find_pointers(mod, free, i['load'], 0, set(), prog, symbols)
        i['relocs'] = reloc_rows(i['pointers'], i['load'], len(mod))
        i['pieces'] = sorted(i['palettes'] + i['exact'] + i['conv'])
        i['cls'] = screen_classes(mod, (i['code'], i['loader']), None, i['palettes'], i['exact'], i['conv'], i['pointers'])
    return info


def runs_of_ones(b):
    """[(start, end)] of the runs of non-zero bytes of b."""
    out, s = [], None
    for k, x in enumerate(list(b) + [0]):
        if x and s is None:
            s = k
        elif not x and s is not None:
            out.append((s, k))
            s = None
    return out


def screen_descriptors(info):
    """The table rows of the screen modules: one for each file that carries an image that has something to build, sorted by file name and slot."""
    out = []
    for i in info.values():
        if not i['pieces'] and not i['relocs']:
            continue                                  # nothing to build: no row
        for f in i['files']:
            out.append(dict(file=f, slot=i['slot'], load=i['load'], size=len(i['mod']), pieces=[(o, n, s, t) for o, n, s, t, *_ in i['pieces']],
                            relocs=list(i['relocs'])))
    return sorted(out, key=lambda d: (d['file'], d['slot']))


def read_chunk(sec, lba, size, slot):
    """The bytes of table 0 chunk `slot` of the archive at sector lba (size bytes), reading only its header sector and the chunk."""
    head = sec(lba, 1)
    count = struct.unpack_from('<I', head, 0)[0]
    at = 2048
    for k in range(count):
        kind, n, _first = struct.unpack_from('<III', head, 0x20 + 32 * k)
        if kind == slot:
            return sec(lba + at // 2048, (n + 2047) // 2048)[:n]
        at += (n + 2047) // 2048 * 2048
    return None


def read_screen_modules(disc, only=None):
    """[dict(image, slot, load, files, mod)] of the screen and stage images of build.toml (every image whose archive is not a fighter's), the
    chunk read from the first file that carries it and checked against the build's SHA-256 in each of the files (contents.tsv)."""
    import code_pins as CP
    images, _funcs = CP.read_build()
    _rows, archives = CP.read_inventory()
    out = []
    with open(disc, 'rb') as f:
        files, sec = F.iso_files(f)
        by_name = {}
        for p, v in files.items():
            by_name.setdefault(p.rsplit('/', 1)[-1].upper(), []).append(v)
        for im in images:
            archive = Path(im['archive']).name
            if archive.upper().startswith('PL'):
                continue
            names = archives[(im['slot'], archive)]
            if only and not any(x.upper().removesuffix('.PAC') == only.upper().removesuffix('.PAC') for x in names):
                continue
            mod = None
            for name in names:
                hits = by_name.get(name.upper(), [])
                if len(hits) != 1:
                    raise SystemExit(f'{name}: {len(hits)} files of that name on the disc')
                chunk = read_chunk(sec, hits[0][0], hits[0][1], im['slot'])
                if chunk is None or CP.sha(chunk) != im['sha256'].lower():
                    raise SystemExit(f'{name}: the chunk of slot {im["slot"]:#x} is not the pinned image {im["name"]}')
                mod = chunk
            out.append(dict(image=im['name'], slot=im['slot'], load=im['address'], files=[n.upper() for n in names], mod=mod))
    return out


def screen_context():
    """The texts of the tables that tell what is not code and what the loader's other builders make."""
    def read(name):
        return (HERE / name).read_text()
    code = parse_code_ranges(read('code_pins_table.c'))
    loader = parse_loader_regions(read('screen_lists_table.c'), read('texts_table.c'), read('quotes_table.c'), read('talk_table.c'),
                                  read('stages_table.c'))
    return code, loader


SCREEN_KINDS = ('code', 'loader', 'palette', 'arcade', 'arcade_conv', 'ptr', 'zero', 'left')


def screen_line(i):
    c = collections.Counter(i['cls'])
    mine = c['palette'] + c['arcade'] + c['arcade_conv'] + c['ptr']
    rest = len(i['mod']) - c['code'] - c['loader']
    return (f'{i["image"]} ({i["files"][0]} slot {i["slot"]:#x}, {len(i["files"])} files, {len(i["mod"])} bytes at {i["load"]:#x}): code {c["code"]}, '
            f'made by the loader {c["loader"]}, rest {rest}: palettes {c["palette"]} ({sum(1 for p in i["palettes"] for _ in range(p[1] // 32))} rows in {len(i["palettes"])} runs), '
            f'arcade pieces {c["arcade"]} + converted {c["arcade_conv"]} in {len(i["exact"]) + len(i["conv"])}, pointer words {c["ptr"]} ({len(i["relocs"])}), '
            f'zero {c["zero"]}, left {c["left"]} ({100.0 * mine / (rest - c["zero"]) if rest > c["zero"] else 100.0:.1f}% of the non-zero rest built)')


def screen_report(info, log=print):
    tot = collections.Counter()
    for i in sorted(info.values(), key=lambda i: (i['files'][0], i['slot'])):
        log(screen_line(i))
        c = collections.Counter(i['cls'])
        for k in SCREEN_KINDS:
            tot[k] += c[k] * len(i['files'])
    log('screens and stages, counted once for each file that carries an image: ' + ', '.join(f'{k} {tot[k]}' for k in SCREEN_KINDS))


# ---------------------------------------------------------------- the report

def split_counts(c):
    """(data bytes, bytes the other builders make, rest, pointer bytes, bytes this table builds) of a class count."""
    data = sum(c.values()) - c['code']
    others = sum(c[k] for k in BUILT_ELSEWHERE)
    ptr = sum(c[k] for k in CLASSES if k.startswith('ptr_'))
    return data, others, data - others, ptr, c['arcade'] + c['arcade_conv'] + ptr + c['zero']


def file_line(label, i):
    c = class_counts(i['cls'])
    data, others, rest, ptr, mine = split_counts(c)
    unbuilt = '' if i['built'] else ' (blocks found by structure, not built)'
    left = c['near'] + c['celllist'] + c['text'] + c['other']
    more = f'; the same bytes in {len(i["same"])} more files' if i.get('same') else ''
    return (f'{label}: module {sum(c.values())}, code {c["code"]}, data {data} = blocks of fighter.c and sequence.c {others} + rest {rest}{unbuilt}; '
            f'rest: arcade {c["arcade"]} + converted {c["arcade_conv"]} in {len(i["pieces"])} pieces, pointers {ptr} ({len(i["pointers"])} words), zero {c["zero"]}, '
            f'left {left} (near {c["near"]}, celllist {c["celllist"]}, text {c["text"]}, other {c["other"]}); '
            f'{100.0 * mine / rest if rest else 100.0:.1f}% of the rest built{more}')


def totals(infos):
    tot = collections.Counter()
    for i in infos:
        tot.update(class_counts(i['cls']))
    return tot


def target_report(infos):
    """Lines: the pointer words by class, and for the words into the module, by the kind of block they point at."""
    by = collections.Counter()
    into = collections.Counter()
    for i in infos:
        for _o, w, c in i['pointers']:
            by[c] += 1
            if c == 'ptr_self_data':
                rel = w - i['load']
                kind = next((cl for a, b, cl in i['spans'] if a <= rel < b), 'data after the blocks')
                into[kind] += 1
    lines = ['pointer words by class: ' + ', '.join(f'{k} {by[k]}' for k in CLASSES if k in by)]
    if into:
        lines.append('words into the module\'s data, by what they point at: ' + ', '.join(f'{k} {v}' for k, v in sorted(into.items())))
    return lines


def runs_of(cls, names=('other', 'near', 'text'), gap=32):
    """The stretches of left bytes: [(start, end, bytes in the class names)]. Runs of those classes are joined when at most `gap` bytes
    (zeros, as a rule) lie between them."""
    out, s = [], None
    for k, c in enumerate(list(cls) + ['']):
        if c in names:
            if s is None:
                s = k
            e = k + 1
        elif s is not None and k - e >= gap:
            out.append([s, e])
            s = None
    out = [(a, b, sum(1 for c in cls[a:b] if c in names)) for a, b in out]
    return out


def detail_lines(i, count=6):
    runs = sorted(runs_of(i['cls']), key=lambda r: -r[2])[:count]
    return [f'    left: {s:#x}, {e - s} bytes from there, {n} of them not built' for s, e, n in runs]


def summary_lines(title, infos):
    tot = totals(infos)
    data, others, rest, ptr, mine = split_counts(tot)
    lines = [f'{title}: {sum(tot.values())} bytes, code {tot["code"]}, data {data} = blocks of fighter.c and sequence.c {others} + rest {rest}']
    if not rest:
        return lines
    lines.append('  rest by class: ' + ', '.join(f'{k} {tot[k]} ({100.0 * tot[k] / rest:.1f}%)' for k in CLASSES
                                               if k not in ('code',) + BUILT_ELSEWHERE and tot[k]))
    lines.append(f'  built by the table: arcade {tot["arcade"]} + converted {tot["arcade_conv"]} + pointer words {ptr} + zero fill {tot["zero"]}'
                 f' = {mine} of {rest} ({100.0 * mine / rest:.1f}%); left {rest - mine}')
    lines += ['  ' + x for x in target_report(infos)]
    return lines


def ordered(info, extra, copies):
    """The modules of the main (extra False) or the extra kind in file order; with copies=False only those analysed (a module whose bytes
    another has is a copy)."""
    return [i for k, i in sorted(info.items(), key=lambda kv: (kv[1]['name'], kv[1]['slot'])) if i['extra'] == extra and (copies or 'same_as' not in i)]


def print_report(info, detail=False, log=print):
    for extra in (False, True):
        for i in ordered(info, extra, True):
            if 'same_as' in i:
                log(f'{entry_label(i)}: the same bytes as {entry_label(info[i["same_as"]])}')
                continue
            log(file_line(entry_label(i), i))
            if detail:
                for line in detail_lines(i):
                    log(line)
    mains = ordered(info, False, True)
    for line in summary_lines(f'all {len(mains)} fighter modules (slot 4 or 5)', mains):
        log(line)
    extras = ordered(info, True, False)
    if extras:
        for line in summary_lines(f'the {len(extras)} other code modules the files carry (each once)', extras):
            log(line)


# ---------------------------------------------------------------- the C, compared

class CLib:
    """module_data.c and the generated table compiled (warnings are errors) into a shared library in a temporary folder."""

    class Piece(ctypes.Structure):
        _fields_ = [('offset', ctypes.c_uint32), ('size', ctypes.c_uint32), ('source', ctypes.c_uint32), ('layout', ctypes.c_char_p)]

    class Reloc(ctypes.Structure):
        _fields_ = [('offset', ctypes.c_uint32), ('target', ctypes.c_uint32)]

    class Row(ctypes.Structure):
        pass

    def __init__(self, tmp):
        lib = Path(tmp) / 'module_data.so'
        built = subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC', '-o', str(lib),
                        str(HERE / 'module_data.c'), str(HERE / 'module_data_table.c')], capture_output=True, timeout=120, text=True)
        if built.returncode:
            raise SystemExit('module_data.c does not build:\n' + built.stderr)
        self.lib = ctypes.CDLL(str(lib))
        self.Row._fields_ = [('file', ctypes.c_char_p), ('slot', ctypes.c_uint16), ('load', ctypes.c_uint32), ('size', ctypes.c_uint32),
                             ('pieces', ctypes.POINTER(self.Piece)), ('piece_count', ctypes.c_uint), ('relocs', ctypes.POINTER(self.Reloc)),
                             ('reloc_count', ctypes.c_uint)]
        n = ctypes.c_uint.in_dll(self.lib, 'arcade_module_data_count').value
        self.rows = (self.Row * n).in_dll(self.lib, 'arcade_module_data')
        self.lib.arcade_module_data_build.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(self.Row), ctypes.c_void_p, ctypes.c_void_p,
                                                      ctypes.c_size_t]
        self.lib.arcade_module_data_valid.argtypes = [ctypes.POINTER(self.Row)]
        self.lib.arcade_module_data_find.argtypes = [ctypes.c_char_p, ctypes.c_uint]
        self.lib.arcade_module_data_find.restype = ctypes.POINTER(self.Row)

    def build(self, prog, name, slot):
        """(bytes, known, seconds) or an error string."""
        row = self.lib.arcade_module_data_find(name.encode(), slot)
        if not row:
            return 'not in the table'
        size = row.contents.size
        img, known = ctypes.create_string_buffer(size), ctypes.create_string_buffer(size)
        t0 = time.perf_counter()
        rc = self.lib.arcade_module_data_build(prog, len(prog), row, img, known, size)
        took = time.perf_counter() - t0
        if rc:
            return 'the C refused'
        return img.raw, known.raw, took


def first_diff(a, b):
    return next((k for k, (x, y) in enumerate(zip(a, b)) if x != y), min(len(a), len(b)))


def compare_screens(clib, m, sinfo, log=print):
    """The screen and stage rows of the checked-in table through the C: for every file that carries an image, the C's bytes equal the model's,
    and the bytes built equal the disc's. Returns the status (1 on any difference)."""
    status = 0
    tot = collections.Counter()
    rows = 0
    took_all = 0.0
    for i in sorted(sinfo.values(), key=lambda i: (i['files'][0], i['slot'])):
        mod = i['mod']
        if not i['pieces'] and not i['relocs']:
            log(f'{i["image"]} ({i["files"][0]} slot {i["slot"]:#x}, {len(i["files"])} files): nothing to build, no row')
            continue
        mimg, mknown = model_build(m, len(mod), i['load'], i['pieces'], i['relocs'])
        diff = next((k for k in range(len(mod)) if mknown[k] and mimg[k] != mod[k]), None)
        by = collections.Counter(cl for cl, kn in zip(i['cls'], mknown) if kn)
        elsewhere = sum(v for k, v in by.items() if k not in ('palette', 'arcade', 'arcade_conv', 'ptr'))
        took_img = 0.0
        bad_rows = 0
        for f in i['files']:
            built = clib.build(m, f, i['slot'])
            if isinstance(built, str):
                log(f'{f} slot {i["slot"]:#x}: {built}')
                status = 1
                continue
            img, known, took = built
            rows += 1
            took_all += took
            took_img = max(took_img, took)
            if img != mimg or known != mknown:
                bad_rows += 1
        status |= bad_rows != 0 or diff is not None or elsewhere != 0
        tot.update({k: v * len(i['files']) for k, v in by.items()})
        log(f'{i["image"]} ({i["files"][0]} slot {i["slot"]:#x}, {len(i["files"])} rows): C built palettes {by["palette"]} + arcade {by["arcade"] + by["arcade_conv"]} '
            f'+ pointers {by["ptr"]} = {sum(mknown)} bytes (slowest row {took_img * 1000:.2f} ms); '
            + ('C = model in every row' if not bad_rows else f'C != model in {bad_rows} rows') + '; '
            + ('equal to the disc' if diff is None else f'DIFFERS from the disc (from +{diff:#x})')
            + (f'; {elsewhere} bytes built outside the classes' if elsewhere else ''))
    log(f'screen and stage modules: {rows} rows, built palettes {tot["palette"]} + arcade {tot["arcade"] + tot["arcade_conv"]} + pointers {tot["ptr"]} bytes '
        f'(counted once for each file that carries an image); C time for all rows {took_all * 1000:.1f} ms')
    return status


def compare_c(info, m, text, sinfo=None, log=print):
    """Build the C with the checked-in table; per module: C against the model and the disc. Returns the status."""
    status = 0
    here = HERE / 'module_data_table.c'
    same = here.read_text() == text
    log('module_data_table.c ' + ('is the generated one' if same else 'DIFFERS from the generated one'))
    status |= not same
    tmp = tempfile.mkdtemp(prefix='module-data-')
    try:
        clib = CLib(tmp)
        total = {False: collections.Counter(), True: collections.Counter()}
        took_all = 0.0
        for k in range(len(clib.rows)):
            if clib.lib.arcade_module_data_valid(ctypes.byref(clib.rows[k])):
                log(f'{clib.rows[k].file.decode()} slot {clib.rows[k].slot}: the table row is malformed')
                status = 1
        copies = collections.Counter()
        for key in sorted(info, key=lambda k: (info[k]['extra'], info[k]['name'], info[k]['slot'])):
            i = info[key]
            label = entry_label(i)
            built = clib.build(m, i['name'], i['slot'])
            if isinstance(built, str):
                log(f'{label}: {built}')
                status = 1
                continue
            img, known, took = built
            mimg, mknown = model_build(m, len(i['mod']), i['load'], i['pieces'], i['relocs'])
            disc = i['mod']
            eq_model = img == mimg and known == mknown
            built_n = sum(known)
            eq_disc = sum(1 for k, (a, b) in enumerate(zip(img, disc)) if known[k] and a == b)
            c = class_counts(i['cls'])
            by = collections.Counter()
            for k, cl in enumerate(i['cls']):
                if known[k]:
                    by['arcade' if cl in ('arcade', 'arcade_conv') else 'pointers' if cl.startswith('ptr_') else 'elsewhere'] += 1
            _data, _others, rest, ptr, _mine = split_counts(c)
            status |= (not eq_model) or built_n != eq_disc or by['elsewhere'] != 0
            if 'same_as' in i:
                copies[i['same_as']] += 1
                if not i['extra']:
                    total[False].update(dict(built=built_n, equal=eq_disc, zeros=c['zero'], rest=rest, arcade=by['arcade'], pointers=by['pointers']))
                continue
            took_all += took
            total[i['extra']].update(dict(built=built_n, equal=eq_disc, zeros=c['zero'], rest=rest, arcade=by['arcade'], pointers=by['pointers']))
            log(f'{label}: C built {built_n} bytes (arcade {by["arcade"]} of {c["arcade"] + c["arcade_conv"]}, pointers {by["pointers"]} of {ptr}) in {took * 1000:.2f} ms; C '
                + ('=' if eq_model else '!= (from +%#x)' % first_diff(img, mimg)) + ' model; '
                + ('equal to the disc' if built_n == eq_disc else f'{built_n - eq_disc} bytes DIFFER from the disc (from +%#x)' %
                   next(k for k, (a, b) in enumerate(zip(img, disc)) if known[k] and a != b))
                + f'; with {c["zero"]} zero bytes {100.0 * (built_n + c["zero"]) / rest if rest else 100.0:.1f}% of the rest')
        for extra, what in ((False, 'fighter modules'), (True, 'other code modules')):
            t = total[extra]
            if t['rest']:
                log(f'{what}: built {t["built"]} bytes, equal to the disc {t["equal"]} (zero fill adds {t["zeros"]}) of {t["rest"]} in the rest')
        log(f'C time for {sum(1 for i in info.values() if "same_as" not in i)} distinct modules: {took_all * 1000:.1f} ms; '
            f'{sum(copies.values())} more rows share their bytes with another and were checked equal too')
        if sinfo:
            status |= compare_screens(clib, m, sinfo, log)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return status


# ---------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00 or PL00.PAC); the tables then hold only it')
    ap.add_argument('--detail', action='store_true', help='per file, the largest runs left')
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/module_data_table.c')
    ap.add_argument('--compare-c', action='store_true', help='build module_data.c with the checked-in table and compare it with the model and the disc')
    args = ap.parse_args(argv)
    t0 = time.monotonic()
    m = P.load_arcade_program(args.arcade)
    files = read_modules(args.disc, args.only)
    fr = parse_fighters_table((HERE / 'fighters_table.c').read_text())
    sq = parse_sequences_table((HERE / 'sequences_table.c').read_text())
    prog, symbols = program_context()
    inventory = (ROOT / 'ps1/inventory/modules.tsv').read_text()
    info = analyse_all(files, m, fr, sq, inventory, prog, symbols) if files else {}
    sentries = read_screen_modules(args.disc, args.only)
    code, loader = screen_context()
    sinfo = analyse_screens(sentries, m, code, loader, prog, symbols) if sentries else {}
    descs = [descriptor(info[k]) for k in sorted(info, key=lambda k: (info[k]['name'], info[k]['slot']))] + screen_descriptors(sinfo)
    text = c_table(descs)
    if args.c_table:
        sys.stdout.write(text)
        print(f'{len(info)} fighter files, {len(sinfo)} screen and stage images in {time.monotonic() - t0:.1f} s', file=sys.stderr)
        return 0
    if args.compare_c:
        status = compare_c(info, m, text, sinfo)
        print(f'{len(info)} fighter files, {len(sinfo)} screen and stage images in {time.monotonic() - t0:.1f} s')
        return status
    if info:
        print_report(info, args.detail)
    if sinfo:
        screen_report(sinfo)
    print(f'{len(info)} fighter files, {len(sinfo)} screen and stage images in {time.monotonic() - t0:.1f} s')
    return 0


if __name__ == '__main__':
    sys.exit(main())
