#!/usr/bin/env python3
"""Analysis tool: classify every byte of the PS1 program's data and say which of it the arcade program also holds.

What it does. The PS1 program (SLPS_004.15) has 90,088 bytes of data between the end of its last function
(0x8016d91c) and its last byte that is not zero (0x80183903). port/arcade/program.c builds 48 pieces of it from
the arcade program (runs of 32 bytes or more, 16-bit fields byte-reversed). This tool looks at ALL of the data
and puts each byte in one class, so that one can plan how to build the rest without the disc:

  piece        built already by program.c (the pieces are read from that file)
  arcade_hi    equal to the arcade program, found by the relaxed search below, evidence 40 bits or more
  arcade_med   the same, evidence 20 to 40 bits
  arcade_lo    the same, evidence 8 to 20 bits (short or low-entropy; plausible, not safe)
  arcade_frag  short equal fragments (under 32 bytes, not a whole symbol): real equalities of rare values, but
               scattered over unrelated places of the arcade program (tile-code lists); not a table's source
  arcade_conv  the same VALUES at another width (a byte table the PS1 widened to 32 bits, or the reverse)
  sdk          Sony's library: data ranges that the matching build gives to library units (ps1/src/build.toml)
               and the symbols the library owns (D_800..., _spu..., gpu...); not read by the port if the
               library is replaced
  jtbl         jump tables of the game's own switch statements (read-only data ranges of game units in
               build.toml, and pointers into the middle of a function): the port compiles its own
  ptr_code     a 32-bit word that is the address of a function (a handler table)
  ptr_data     ... of data in this same block (string, table)
  ptr_module   ... of an overlay module's load area (0x801ae900 and above)
  ptr_other    ... of the rest of RAM (buffers, the area below the code, bss)
  text         ASCII text and the records around it (menus, messages, names)
  celllist     chains of cell lists (the format fighters.py reads) and the position streams they use
  zero         bytes that are zero and in no other class
  other        the rest (the PS1's own tables)

The arcade search is a relaxed form of the one that made the pieces. A unit is 2 bytes, equal to the arcade's as it
is ('b') or with its two bytes turned ('w'), or 4 bytes turned ('l', a 32-bit field). Seeds are 8-byte windows
with at least 3 different values, found with an index of the arcade program; each seed grows both ways while
units agree, and a mismatching unit costs 1.5 matched units (the growth stops when the score falls 6 below its
best). The evidence of a segment is the information in the arcade values it matches (each distinct non-zero 16-bit
value counted once, -log2 of its frequency in the arcade program) minus the log2 of the number of places it could
have matched: a segment with 40 bits or more cannot be chance. A second search compares VALUES of different widths
(PS1 u8/u16/u32 little-endian against the arcade's u8/u16/u32 big-endian) and drops runs that are mostly
consecutive numbers (tile-index ramps match any arcade ramp). Both searches only look at bytes that nothing
stronger has claimed.

Inputs: the disc image (raw 2352-byte sectors, --disc) or an extracted SLPS_004.15 (--exe), and the arcade
set's ZIP (--arcade, MAME's sfa2u). The repository's own files are read for symbols and function addresses:
ps1/src/symbols.ld, ps1/inventory/game.tsv and library.tsv, ps1/src/build.toml, ps1/src/**/*.c (who uses a
table), port/arcade/program.c. It writes NOTHING and prints only numbers, addresses and symbol names, never
game bytes. It is an analysis and reference tool; the loader that builds the data at run time is written in C.

    python3 port/arcade/program_map.py --disc BIN --arcade ZIP [--top N] [--all] [--pointers] [--candidates]

--top N shows the N largest tables of each class (default 12); --all prints every symbol range with its class
mix; --pointers lists every pointer word with its target (symbol and offset); --candidates prints the arcade
segments that program.c does not have yet, as exact runs in program.c's piece format, with 'l' as a new layout
letter (a 32-bit field, four bytes turned) and the evidence in bits.
"""
import argparse
import bisect
import collections
import math
import re
import struct
import sys
import time
import tomllib
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

DATA_START = 0x8016d91c
DATA_END = 0x80183904
CODE_START = 0x80118900
MODULE_START = 0x801ae900          # the resident image ends here; overlay modules load at and above it
RAM_LO, RAM_HI = 0x80010000, 0x801fffff
SEED_WINDOW = 8                    # bytes
XDROP = 6.0                        # growth stops this far below the best score
MISMATCH_COST = 1.5
ZERO_GAIN = 0.2                    # an equal unit of zeros says little
SDK_NAME = re.compile(r'^(D_[0-9A-Fa-f]{8}|D_last|D_intr_\w+|_spu\w*|_qin|_qout|_sn\w*|gpu\w*|GPU\w*|dev_\w+|hw|DMA\d_\w+)$')

CLASSES = ['piece', 'arcade_hi', 'arcade_med', 'arcade_lo', 'arcade_frag', 'arcade_conv', 'sdk', 'jtbl', 'ptr_code', 'ptr_data',
           'ptr_module', 'ptr_other', 'text', 'celllist', 'zero', 'other']


# ---------------------------------------------------------------- readers of the inputs

def iso_read_file(read_sector, name):
    """The bytes of the first file with this name in the ISO 9660 tree. read_sector(lba) gives 2048 bytes."""
    def sectors(lba, size):
        return b''.join(read_sector(lba + i) for i in range((size + 2047) // 2048))[:size]
    pvd = read_sector(16)
    root = pvd[156:190]
    stack = [(struct.unpack('<I', root[2:6])[0], struct.unpack('<I', root[10:14])[0])]
    while stack:
        lba, size = stack.pop()
        data = sectors(lba, size)
        i = 0
        while i < len(data):
            length = data[i]
            if length == 0:
                i = (i // 2048 + 1) * 2048
                continue
            e = data[i:i + length]
            i += length
            nl = e[32]
            nm = e[33:33 + nl]
            if nm in (b'\0', b'\1'):
                continue
            el = struct.unpack('<I', e[2:6])[0]
            sz = struct.unpack('<I', e[10:14])[0]
            if e[25] & 2:
                stack.append((el, sz))
            elif nm.decode('latin1').split(';')[0].upper() == name.upper():
                return sectors(el, sz)
    return None


def load_exe_from_disc(path):
    """(load address, bytes after the 0x800 header) of SLPS_004.15 on a raw 2352-byte-sector image."""
    with open(path, 'rb') as f:
        def read_sector(lba):
            f.seek(lba * 2352 + 24)
            return f.read(2048)
        exe = iso_read_file(read_sector, 'SLPS_004.15')
    if exe is None:
        raise SystemExit('SLPS_004.15 is not on the disc image')
    return struct.unpack('<I', exe[0x18:0x1c])[0], exe[0x800:]


def load_exe_file(path):
    exe = Path(path).read_bytes()
    return struct.unpack('<I', exe[0x18:0x1c])[0], exe[0x800:]


def swap_pairs(raw):
    out = bytearray(len(raw))
    out[0::2] = raw[1::2]
    out[1::2] = raw[0::2]
    return bytes(out)


def load_arcade_program(zip_path):
    """The 68000's view of the arcade program: the members sz2u.03..08 joined, every byte pair swapped."""
    with zipfile.ZipFile(zip_path) as a:
        raw = b''.join(a.read(f'sz2u.{i:02d}') for i in range(3, 9))
    return swap_pairs(raw)


def parse_symbols(text):
    """{address: [names]} from the linker symbol file (lines like `name = 0x8016ea70;`)."""
    out = {}
    for line in text.splitlines():
        m = re.match(r'\s*(\w+)\s*=\s*(0x[0-9a-fA-F]+)\s*;', line)
        if m:
            out.setdefault(int(m.group(2), 16), []).append(m.group(1))
    return out


def parse_inventory(text):
    """[(address, size, name)] from an inventory table (tab separated: address in hex, size, name)."""
    out = []
    for line in text.splitlines():
        p = line.split('\t')
        if len(p) >= 3 and re.fullmatch(r'[0-9a-f]{8}', p[0]):
            out.append((int(p[0], 16), int(p[1]), p[2]))
    return out


def parse_pieces(text):
    """[(address, size, source, units, name)] from the piece table of port/arcade/program.c."""
    out = []
    for line in text.splitlines():
        m = re.match(r'\s*\{(0x[0-9a-f]+)u,\s*(\d+)u,\s*(0x[0-9a-f]+)u,\s*"(\w+)",\s*"([^"]+)"', line)
        if m:
            out.append((int(m.group(1), 16), int(m.group(2)), int(m.group(3), 16), m.group(4), m.group(5)))
    return out


def parse_build_ranges(toml_bytes):
    """[(address, size, 'rodata' or 'data', unit name, source file)] from the matching build's configuration."""
    cfg = tomllib.loads(toml_bytes.decode())
    out = []
    for u in cfg.get('unit', []):
        for kind in ('rodata', 'data'):
            r = u.get(kind)
            if r:
                out.append((r['address'], r['size'], kind, u.get('name', ''), u.get('source', '')))
    return sorted(out)


# ---------------------------------------------------------------- the byte-level arcade search

def unit_kind(q, m, i, d):
    """How the PS1 data at offset i equals the arcade program at i+d: ('b', 2), ('w', 2), ('l', 4) or None.
    'b' as it is, 'w' two bytes turned, 'l' four bytes turned (a 32-bit field; only at a multiple of 4)."""
    if i < 0 or i + 2 > len(q) or i + d < 0 or i + d + 2 > len(m):
        return None
    a = q[i:i + 2]
    b = m[i + d:i + d + 2]
    if a == b:
        return ('b', 2)
    if a[::-1] == b:
        return ('w', 2)
    if i % 4 == 0 and i + 4 <= len(q) and i + d + 4 <= len(m) and q[i:i + 4][::-1] == m[i + d:i + d + 4]:
        return ('l', 4)
    return None


def grow(q, m, i0, d, direction):
    """Grow a segment from offset i0 at delta d; returns the best end (direction 1) or start (direction -1)."""
    best = cur = 0.0
    best_pos = pos = i0
    n, size = len(q), len(m)
    while True:
        if direction > 0:
            if pos + 2 > n or pos + d + 2 > size:
                break
            u = unit_kind(q, m, pos, d)
            if u is None:
                cur -= MISMATCH_COST
                pos += 2
            else:
                zero = not any(q[pos:pos + u[1]])
                cur += (ZERO_GAIN if zero else 1.0) * (u[1] // 2)
                pos += u[1]
        else:
            if pos - 2 < 0 or pos - 2 + d < 0:
                break
            u4 = unit_kind(q, m, pos - 4, d) if pos >= 4 and (pos - 4) % 4 == 0 else None
            u2 = unit_kind(q, m, pos - 2, d)
            if u4 and u4[0] == 'l' and not u2:
                zero = not any(q[pos - 4:pos])
                cur += (ZERO_GAIN if zero else 1.0) * 2
                pos -= 4
            elif u2:
                zero = not any(q[pos - 2:pos])
                cur += ZERO_GAIN if zero else 1.0
                pos -= 2
            else:
                cur -= MISMATCH_COST
                pos -= 2
        if cur > best + 1e-9:
            best = cur
            best_pos = pos
        if cur < best - XDROP:
            break
    return best_pos


class UnitStats:
    """Frequencies of the 16-bit values of the arcade program, for the evidence of a match."""

    def __init__(self, m):
        self.count = collections.Counter(m[i:i + 2] for i in range(0, len(m) - 1, 2))
        self.total = max(len(m) // 2, 1)
        self.places = math.log2(self.total)

    def bits(self, unit):
        c = self.count[unit] + (self.count[unit[::-1]] if unit != unit[::-1] else 0)
        return -math.log2((c + 1) / self.total)


def segment_stats(q, m, s, e, d, stats):
    """(evidence in bits, matched units, mismatching units, layout string) of the segment [s, e) at delta d."""
    seen = set()
    ev = 0.0
    matched = mis = 0
    layout = []
    i = s
    while i < e:
        u = unit_kind(q, m, i, d)
        if u is None:
            mis += 1
            layout.append('-')
            i += 2
            continue
        kind, ln = u
        layout.append(kind if kind == 'l' else ('b' if kind == 'b' else 'w'))
        for h in range(0, ln, 2):
            au = m[i + d + h:i + d + h + 2]
            if au != b'\0\0' and au not in seen:
                seen.add(au)
                ev += stats.bits(au)
        matched += ln // 2
        i += ln
    return ev - stats.places, matched, mis, ''.join(layout)


def seed_anchors(q, m, window=SEED_WINDOW):
    """{(delta, transform): [offsets in q]} where an 8-byte window of q occurs in m (as it is, 2- or 4-byte turned)."""
    def turn(b, k):
        return b if k == 1 else b''.join(b[i:i + k][::-1] for i in range(0, len(b), k))

    def good(b):
        return len(set(b)) >= 3 and b.count(0) <= 5

    want = {1: {}, 2: {}, 4: {}}
    for t in (1, 2, 4):
        step = 4 if t == 4 else 2
        for i in range(0, len(q) - window + 1, step):
            k = q[i:i + window]
            if good(k):
                want[t].setdefault(turn(k, t), []).append(i)
    anchors = {}
    for t in (1, 2, 4):
        w = want[t]
        for j in range(0, len(m) - window + 1, 2):
            k = m[j:j + window]
            if k in w:
                for i in w[k]:
                    anchors.setdefault((j - i, t), []).append(i)
    return anchors


def find_segments(q, m, stats, anchors=None):
    """All grown segments (start, end, delta, evidence, matched, mismatching, layout) of q that agree with m."""
    if anchors is None:
        anchors = seed_anchors(q, m)
    segs = {}
    for (d, _t), offs in anchors.items():
        end_seen = -1
        for j in sorted(offs):
            i = j - (j & 1)
            if i < end_seen:
                continue
            e = grow(q, m, i, d, 1)
            s = grow(q, m, i, d, -1)
            end_seen = max(end_seen, e)
            if e > s:
                segs[(s, e, d)] = 1
    out = []
    for (s, e, d) in segs:
        ev, ma, mi, lay = segment_stats(q, m, s, e, d, stats)
        out.append((s, e, d, ev, ma, mi, lay))
    return out


def select_segments_data(q, m, segs, claimed, stats, min_bits=8.0, min_units=4):
    """Greedy by evidence; each segment keeps the parts no stronger segment claimed (claimed is updated).
    Returns [(start, end, delta, evidence, matched, mismatching, layout)]."""
    kept = []
    for s, e, d, ev, ma, mi, lay in sorted(segs, key=lambda r: -r[3]):
        if ev < min_bits:
            break
        i = s
        while i < e:
            if claimed[i]:
                i += 2
                continue
            j = i
            while j < e and not claimed[j]:
                j += 2
            ev2, ma2, mi2, lay2 = segment_stats(q, m, i, j, d, stats)
            if ev2 >= min_bits and ma2 >= min_units:
                kept.append((i, j, d, ev2, ma2, mi2, lay2))
                for k in range(i, j):
                    claimed[k] = 1
            i = j
    return sorted(kept)


def tier(bits, matched=0, mismatching=0, distinct=0):
    """Confidence class of a segment. Evidence in bits counts each distinct value once, which undervalues a long
    exact run of low-entropy 32-bit records; so a long run with few mismatches and enough different values also
    counts (48 matched units for high, 24 for medium)."""
    long_ok = mismatching * 20 <= matched and distinct >= 6
    if bits >= 40 or (long_ok and matched >= 48):
        return 'arcade_hi'
    if bits >= 20 or (long_ok and matched >= 24):
        return 'arcade_med'
    return 'arcade_lo'


def segment_is_ramp(q, s, e):
    """True when the segment's 16-bit or 32-bit values (either byte order) are mostly a ramp: lists of consecutive
    tile numbers equal any arcade tile list by chance, and say nothing about the table's source."""
    n2 = (e - s) // 2 * 2
    n4 = (e - s) // 4 * 4
    for fmt, n, w in (('<%dH', n2 // 2, 2), ('>%dH', n2 // 2, 2), ('<%dI', n4 // 4, 4)):
        if n >= 4 and is_ramp(struct.unpack(fmt % n, q[s:s + n * w])):
            return True
    return False


def plausible(start, end, base, symbol_addrs, min_span=32):
    """A segment is structurally plausible when it spans at least min_span bytes, or is exactly one symbol range
    (a small table whole). Shorter pieces inside a bigger table are fragments."""
    if end - start >= min_span:
        return True
    a, b = base + start, base + end
    if a not in symbol_addrs:
        return False
    return b in symbol_addrs or b == DATA_END


def simplify_layout(q, m, off, nbytes, d):
    """The shortest uniform layout ('b', 'w' or 'l') under which q[off:off+nbytes] equals m at off+d, else the
    per-unit layout. (A unit whose two bytes are equal is 'b' as well as 'w'; a table of 32-bit fields has many.)"""
    run = q[off:off + nbytes]
    src = m[off + d:off + d + nbytes]
    if run == src:
        return 'b'
    if len(run) % 2 == 0 and b''.join(run[i:i + 2][::-1] for i in range(0, len(run), 2)) == src:
        return 'w'
    if len(run) % 4 == 0 and off % 4 == 0 and b''.join(run[i:i + 4][::-1] for i in range(0, len(run), 4)) == src:
        return 'l'
    return None


def exact_runs(layout, start, min_units=4):
    """The maximal runs of matching units of a segment: [(offset, bytes, layout of the run)]. A run is exact."""
    out = []
    i = 0
    off = start
    cur = None
    while i < len(layout):
        c = layout[i]
        if c == '-':
            if cur:
                out.append(cur)
                cur = None
            off += 2
            i += 1
            continue
        step = 4 if c == 'l' else 2
        if cur is None:
            cur = [off, 0, '']
        cur[1] += step
        cur[2] += c
        off += step
        i += 1
    if cur:
        out.append(cur)
    return [(o, n, lay) for o, n, lay in out if n >= 2 * min_units]


def build_run(m, src, size, units):
    """The PS1 bytes of a run built from the arcade program m at src: the layout letters repeat over the run's
    16-bit units; 'b' keeps a unit, 'w' turns its two bytes, 'l' turns four bytes (and takes two units)."""
    out = bytearray()
    i = 0
    k = 0
    while i < size:
        c = units[k % len(units)]
        k += 1
        if c == 'l':
            out += m[src + i:src + i + 4][::-1]
            i += 4
        elif c == 'w':
            out += m[src + i:src + i + 2][::-1]
            i += 2
        else:
            out += m[src + i:src + i + 2]
            i += 2
    return bytes(out)


def minimal_period(units):
    """The shortest string whose repetition gives `units` (program.c repeats the layout string over the piece)."""
    n = len(units)
    for p in range(1, n + 1):
        if n % p == 0 and units[:p] * (n // p) == units:
            return units[:p]
    return units


# ---------------------------------------------------------------- the value-level search (other widths)

def _arr(buf, width, phase, big):
    import array
    data = buf[phase:]
    data = data[:len(data) // width * width]
    a = array.array({1: 'B', 2: 'H', 4: 'I'}[width], data)
    if big and width > 1:
        a.byteswap()
    return a


def is_ramp(values):
    """True when most steps between the non-zero values are one small constant (a run of consecutive numbers, or
    every other number, matches any arcade ramp by chance)."""
    values = [v for v in values if v]
    steps = [b - a for a, b in zip(values, values[1:])]
    if not steps:
        return False
    step, count = collections.Counter(steps).most_common(1)[0]
    return abs(step) <= 4 and count * 10 >= len(steps) * 6


def find_value_segments(q, m, usable, stats_by_width):
    """Runs where the PS1 values (width w, little-endian) equal the arcade's (width v, big-endian), w != v.
    usable: bytearray, 1 where q may be claimed. Returns [(start byte, end byte, w, v, arcade byte of the start,
    evidence, mismatching values, values)]."""
    win = {1: 12, 2: 8, 4: 6}
    seeds = collections.defaultdict(set)
    for w in (1, 2, 4):
        t = _arr(q, w, 0, False)
        L = win[w]
        index = {}
        for i in range(0, len(t) - L + 1):
            if not all(usable[i * w:(i + L) * w]):
                continue
            key = tuple(t[i:i + L])
            if len(set(key)) >= 3 and len(set(b - a for a, b in zip(key, key[1:]))) >= 3:
                index.setdefault(key, []).append(i)
        for v in (1, 2, 4):
            if v == w:
                continue
            for ph in ((0,) if v != 4 else (0, 2)):
                a = _arr(m, v, ph, True)
                its = [a[i:] for i in range(L)]
                for j, key in enumerate(zip(*its)):
                    if key in index:
                        for i in index[key]:
                            seeds[(w, v, j * v + ph - i * v)].add(i)

    def pv(i, w):
        return int.from_bytes(q[i * w:i * w + w], 'little')

    def av(base, i, v):
        return int.from_bytes(m[base + i * v:base + i * v + v], 'big')

    def ok(w, v, base, i):
        if i < 0 or (i + 1) * w > len(q) or base + i * v < 0 or base + (i + 1) * v > len(m):
            return None
        return pv(i, w) == av(base, i, v)

    found = {}
    for (w, v, base), idx in seeds.items():
        for i0 in sorted(idx):
            best = cur = 0.0
            e = i = i0
            while True:
                r = ok(w, v, base, i)
                if r is None:
                    break
                cur += (ZERO_GAIN if pv(i, w) == 0 else 1.0) if r else -MISMATCH_COST
                i += 1
                if cur > best + 1e-9:
                    best, e = cur, i
                if cur < best - XDROP:
                    break
            best = cur = 0.0
            s = i = i0
            i -= 1
            while True:
                r = ok(w, v, base, i)
                if r is None:
                    break
                cur += (ZERO_GAIN if pv(i, w) == 0 else 1.0) if r else -MISMATCH_COST
                if cur > best + 1e-9:
                    best, s = cur, i
                i -= 1
                if cur < best - XDROP:
                    break
            if e - s >= 4:
                found[(w, v, base, s, e)] = 1
    out = []
    for (w, v, base, s, e) in found:
        vals = [pv(i, w) for i in range(s, e)]
        if is_ramp(vals) or not all(usable[s * w:e * w]):
            continue
        st = stats_by_width[v]
        seen = set()
        ev = 0.0
        mis = 0
        for i in range(s, e):
            a = av(base, i, v)
            if pv(i, w) == a:
                if a and a not in seen:
                    seen.add(a)
                    ev += -math.log2((st[a] + 1) / st['total'])
            else:
                mis += 1
        ev -= math.log2(st['total'])
        out.append((s * w, e * w, w, v, base + s * v, ev, mis, vals))
    return out


def value_stats(m):
    out = {}
    for v in (1, 2, 4):
        c = collections.Counter(_arr(m, v, 0, True))
        c = collections.defaultdict(int, c)
        c['total'] = len(m) // v
        out[v] = c
    return out


# ---------------------------------------------------------------- pointers, text, cell lists

def pointer_candidates(q, base):
    """Offsets of aligned 32-bit words that are RAM addresses (0x80010000..0x801fffff)."""
    out = []
    for i in range(0, len(q) - 3, 4):
        w = struct.unpack_from('<I', q, i)[0]
        if RAM_LO <= w <= RAM_HI:
            out.append(i)
    return out


def pointer_kind(w, func_starts, func_at, symbol_addrs):
    """Class of a pointer value: ptr_code (a function's start), jtbl (inside a function), ptr_data (this block),
    ptr_module (a module load area) or ptr_other."""
    if CODE_START <= w < DATA_START:
        if w in func_starts:
            return 'ptr_code'
        return 'jtbl' if func_at(w) else 'ptr_other'
    if DATA_START <= w < DATA_END:
        return 'ptr_data'
    if w >= MODULE_START:
        return 'ptr_module'
    return 'ptr_other'


def strong_pointers(q, cands, func_starts, symbol_addrs):
    """Candidates that look like pointers: a function start or a symbol address, or a neighbour is a candidate
    too. A lone word that is neither (a pair of small 16-bit values such as 0x8007 0x8008) is weak."""
    cs = set(cands)
    out = []
    for i in cands:
        w = struct.unpack_from('<I', q, i)[0]
        neighbour = (i - 4) in cs or (i + 4) in cs
        if (w & 3) == 0 and (w in func_starts or w in symbol_addrs or neighbour):
            out.append(i)
    return out


TEXT_RUN = re.compile(rb'[\x20-\x7e]{4,}')


def is_text(s):
    """A printable run that reads as text: it ends the game's way (`@@`, `@c`), or has a space, a lower-case letter
    or `%`, `:` or `.` and mostly letters, so that tables of letters or symbols used as numbers (BCDDD..., ((((...) are not."""
    if len(s) < 4:
        return False
    if s.endswith(b'@'):
        return True
    letters = sum(1 for c in s if 65 <= c <= 90 or 97 <= c <= 122)
    others = len(s) - s.count(b' ')
    return any(c in s for c in b' abcdefghijklmnopqrstuvwxyz%:.') and others > 0 and letters * 2 >= others


def text_runs(q, gap=12):
    """Spans [(start, end)] of text: printable runs that read as text, joined when the bytes between them are
    at most `gap` (the control codes and record headers of the menus)."""
    runs = [m.span() for m in TEXT_RUN.finditer(q) if is_text(m.group())]
    out = []
    for a, b in runs:
        if out and a - out[-1][1] <= gap:
            out[-1] = (out[-1][0], b)
        else:
            out.append((a, b))
    return out


def parse_cell_list(mod, o, max_cells=512):
    """The cell list at offset o (five 16-bit words: cells, mode 0/2/4/6, third, 0, position offset; then a body
    by mode, as port/arcade/fighters.py reads it): (cells, mode, position, end) or None."""
    if o + 10 > len(mod):
        return None
    n, mode, _third, zero, pos = struct.unpack_from('<5H', mod, o)
    if not 1 <= n <= max_cells or mode not in (0, 2, 4, 6) or zero:
        return None
    i = o + 10
    if mode in (0, 2):
        step = 2 if mode == 0 else 4
        end = i + step * n
        return (n, mode, pos, end) if end <= len(mod) else None
    step = 4 if mode == 4 else 6
    count = 0
    while count < n:
        if i + step > len(mod):
            return None
        count += struct.unpack_from('<H', mod, i + (2 if mode == 4 else 4))[0] + 1
        i += step
    return (n, mode, pos, i) if count == n else None


def find_cell_chains(q, usable, min_lists=20, min_bytes=400):
    """Chains of at least min_lists cell lists back to back in usable bytes: [(start, end, lists, max position)]."""
    out = []
    i = 0
    while i + 10 <= len(q):
        if not usable[i]:
            i += 2
            continue
        o = i
        lists = cells = 0
        maxpos = 0
        while True:
            r = parse_cell_list(q, o)
            if r is None or not all(usable[o:r[3]]):
                break
            lists += 1
            cells += r[0]
            maxpos = max(maxpos, r[2])
            o = r[3]
        if lists >= min_lists and o - i >= min_bytes and cells >= 3 * lists:
            out.append((i, o, lists, maxpos))
            i = o
        else:
            i += 2
    return out


# ---------------------------------------------------------------- the classification

def classify(q, base, m, pieces, symbols, funcs, build_ranges, stats=None, value_stats_=None, log=None):
    """One class per byte of q (q is the data from `base`). Returns (classes list, details dict)."""
    n = len(q)
    cls = [''] * n
    details = {'segments': [], 'convs': [], 'chains': [], 'pointers': {}}

    def put(a, b, c, only_free=True):
        for k in range(max(a, 0), min(b, n)):
            if not only_free or not cls[k]:
                cls[k] = c

    for a, size, _src, _u, _name in pieces:
        put(a - base, a - base + size, 'piece')
    # Sony's library: ranges the matching build gives to library units, and the symbols the library owns
    for a, size, _kind, _unit, source in build_ranges:
        if source.startswith('sdk/'):
            put(a - base, a - base + size, 'sdk')
        elif DATA_START <= a < DATA_END:
            put(a - base, a - base + size, 'jtbl')
    names = sorted(symbols)
    for idx, a in enumerate(names):
        if DATA_START <= a < DATA_END and any(SDK_NAME.match(x) for x in symbols[a]):
            nxt = names[idx + 1] if idx + 1 < len(names) else DATA_END
            put(a - base, min(nxt, DATA_END) - base, 'sdk')
    # chains of cell lists (before the arcade search: a short coincidence inside a list is not an arcade table)
    usable = bytearray(1 if not cls[k] else 0 for k in range(n))
    for s, e, lists, maxpos in find_cell_chains(q, usable):
        put(s, e, 'celllist')
        details['chains'].append((s, e, lists, maxpos))
        # the position streams: the symbol range just before the chain, sized for the largest position used
        prev = [a for a in sorted(symbols) if base + s - 0x4000 <= a < base + s]
        if prev:
            a = prev[-1]
            if maxpos + 2 <= (base + s - a) <= maxpos + 64:
                put(a - base, s, 'celllist')
    # the arcade search over the bytes nothing claimed
    if m is not None:
        stats = stats or UnitStats(m)
        claimed = bytearray(1 if cls[k] else 0 for k in range(n))
        segs = find_segments(q, m, stats)
        kept = select_segments_data(q, m, segs, claimed, stats)
        sym_addrs0 = set(symbols)
        for s, e, d, ev, ma, mi, lay in kept:
            if segment_is_ramp(q, s, e):
                continue
            if plausible(s, e, base, sym_addrs0):
                put(s, e, tier(ev, ma, mi, len({q[k:k + 2] for k in range(s, e, 2)})))
            else:
                put(s, e, 'arcade_frag')
            details['segments'].append((s, e, d, ev, ma, mi, lay))
        usable = bytearray(1 if not cls[k] else 0 for k in range(n))
        vs = value_stats_ or value_stats(m)
        conv = [c for c in find_value_segments(q, m, usable, vs) if c[5] >= 20]
        conv.sort(key=lambda c: -c[5])
        for s, e, w, v, arc, ev, mis, _vals in conv:
            if all(not cls[k] for k in range(s, e)):
                put(s, e, 'arcade_conv')
                details['convs'].append((s, e, w, v, arc, ev, mis))
    # pointers
    func_starts = set(a for a, _s, _n in funcs)
    fa = sorted(func_starts)
    fe = {a: a + s for a, s, _n in funcs}

    def func_at(w):
        i = bisect.bisect_right(fa, w) - 1
        return i >= 0 and w < fe[fa[i]]
    sym_addrs = set(symbols)
    for i in strong_pointers(q, pointer_candidates(q, base), func_starts, sym_addrs):
        if any(cls[i:i + 4]):
            continue
        w = struct.unpack_from('<I', q, i)[0]
        k = pointer_kind(w, func_starts, func_at, sym_addrs)
        put(i, i + 4, k)
        details['pointers'][i] = (w, k)
    # text
    for a, b in text_runs(q):
        put(a, b, 'text')
    for k in range(n):
        if not cls[k]:
            cls[k] = 'zero' if q[k] == 0 else 'other'
    return cls, details


# ---------------------------------------------------------------- the report

def symbol_ranges(symbols, base, end):
    """[(start, end, name)] of the symbol-bounded ranges inside [base, end)."""
    starts = sorted(a for a in symbols if base <= a < end)
    if not starts or starts[0] != base:
        starts = [base] + starts
    out = []
    for i, a in enumerate(starts):
        b = starts[i + 1] if i + 1 < len(starts) else end
        out.append((a, b, symbols.get(a, ['(start of data)'])[0]))
    return out


def find_uses(src_root, names):
    """{symbol: sorted names of the source files that use it}, by whole-word search of ps1/src (not sdk/)."""
    if not names:
        return {}
    pat = re.compile(r'\b(' + '|'.join(re.escape(x) for x in sorted(names, key=len, reverse=True)) + r')\b')
    uses = collections.defaultdict(set)
    for p in sorted(Path(src_root).rglob('*.c')):
        if 'sdk' in p.parts:
            continue
        try:
            text = p.read_text(errors='replace')
        except OSError:
            continue
        for sym in set(pat.findall(text)):
            uses[sym].add(p.stem)
    return {k: sorted(v) for k, v in uses.items()}


def zero_run_bytes(cls, q, min_len=16):
    """Bytes of class zero that lie in runs of at least min_len zero bytes (of any class)."""
    total = 0
    i = 0
    n = len(q)
    while i < n:
        if q[i] == 0:
            j = i
            while j < n and q[j] == 0:
                j += 1
            if j - i >= min_len:
                total += sum(1 for k in range(i, j) if cls[k] == 'zero')
            i = j
        else:
            i += 1
    return total


def summarize(cls):
    c = collections.Counter(cls)
    return [(k, c.get(k, 0)) for k in CLASSES]


def table_rows(cls, ranges, base):
    rows = []
    for a, b, name in ranges:
        c = collections.Counter(cls[a - base:b - base])
        rows.append((a, b - a, name, c))
    return rows


def print_report(cls, q, base, details, ranges, uses, symbols, top, show_all, candidates, arcade, pointers=False, log=print):
    n = len(q)
    log(f'PS1 program data: {n} bytes, {base:#x} to {base + n:#x}')
    log('')
    log('bytes per class')
    for k, v in summarize(cls):
        log(f'  {k:12s} {v:7d} {100.0 * v / n:5.1f}%')
    log(f'  (zero bytes in runs of 16 or more: {zero_run_bytes(cls, q)})')
    rows = table_rows(cls, ranges, base)
    for k in CLASSES:
        if k == 'piece':
            continue
        sel = sorted((r for r in rows if r[3].get(k)), key=lambda r: -r[3][k])[:top]
        if not sel:
            continue
        log('')
        log(f'top tables of class {k}')
        for a, size, name, c in sel:
            u = uses.get(name, [])
            used = (u[0] + (f' +{len(u) - 1}' if len(u) > 1 else '')) if u else '-'
            log(f'  {a:#x} {size:6d} {name:28s} {k} {c[k]:6d} of {size:6d}  used by {used}')
    if show_all:
        log('')
        log('every symbol range: address size name, then bytes per class')
        for a, size, name, c in rows:
            log(f'  {a:#x} {size:6d} {name:28s} ' + ' '.join(f'{k}={v}' for k, v in c.items() if v))
    ptr = collections.Counter(k for _w, k in details['pointers'].values())
    log('')
    log('pointer words (aligned, strong): ' + ', '.join(f'{k} {v}' for k, v in sorted(ptr.items())))
    log('cell-list chains: ' + (', '.join(f'{base + s:#x} {e - s} bytes {lists} lists' for s, e, lists, _p in details['chains']) or 'none'))
    segs = details['segments']
    log(f'arcade segments found beyond program.c: {len(segs)}; units that differ inside them: {sum(s[5] for s in segs)}')
    log(f'value-converted runs: {len(details["convs"])}')
    if pointers:
        log('')
        log('pointer words: location, class, target as symbol+offset')
        for i in sorted(details['pointers']):
            w, k = details['pointers'][i]
            log(f'  {base + i:#x} {k:10s} {w:#x} {nearest_symbol(symbols, w)}')
    if candidates:
        log('')
        log('candidate pieces (address, size, arcade address, layout, symbol), exact runs only; "diff" counts mismatching units')
        sym_addrs = set(symbols)
        for s, e, d, ev, ma, mi, lay in segs:
            if not plausible(s, e, base, sym_addrs):
                continue
            for off, nb, run in exact_runs(lay, s):
                units = simplify_layout(q, arcade, off, nb, d) or minimal_period(run)
                if build_run(arcade, off + d, nb, units) != q[off:off + nb]:
                    log(f'  (a run at {base + off:#x} does not rebuild; layout {units!r})')
                    continue
                sym = symbols.get(base + off) or [nearest_symbol(symbols, base + off)]
                log(f'  {{{base + off:#010x}u, {nb:5d}u, {off + d:#08x}u, "{units}", "{sym[0]}"}},  /* {ev:.0f} bits, {mi} diff */')


def nearest_symbol(symbols, addr):
    names = sorted(symbols)
    i = bisect.bisect_right(names, addr) - 1
    return f'{symbols[names[i]][0]}+{addr - names[i]:#x}' if i >= 0 else '?'


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--disc', help='the disc image (raw 2352-byte sectors)')
    g.add_argument('--exe', help='an extracted SLPS_004.15')
    ap.add_argument('--arcade', required=True, help="the arcade set's ZIP (sfa2u)")
    ap.add_argument('--top', type=int, default=12)
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--candidates', action='store_true')
    ap.add_argument('--pointers', action='store_true')
    a = ap.parse_args(argv)
    t0 = time.time()
    load, body = load_exe_from_disc(a.disc) if a.disc else load_exe_file(a.exe)
    m = load_arcade_program(a.arcade)
    q = body[DATA_START - load:DATA_END - load]
    symbols = parse_symbols((ROOT / 'ps1/src/symbols.ld').read_text())
    funcs = parse_inventory((ROOT / 'ps1/inventory/game.tsv').read_text()) + parse_inventory((ROOT / 'ps1/inventory/library.tsv').read_text())
    pieces = parse_pieces((ROOT / 'port/arcade/program.c').read_text())
    ranges = parse_build_ranges((ROOT / 'ps1/src/build.toml').read_bytes())
    cls, details = classify(q, DATA_START, m, pieces, symbols, funcs, ranges)
    sr = symbol_ranges(symbols, DATA_START, DATA_END)
    uses = find_uses(ROOT / 'ps1/src', {name for _a, _b, name in sr})
    print_report(cls, q, DATA_START, details, sr, uses, symbols, a.top, a.all, a.candidates, m, a.pointers)
    print(f'\nran in {time.time() - t0:.1f} seconds')
    return 0


if __name__ == '__main__':
    sys.exit(main())
