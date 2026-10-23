#!/usr/bin/env python3
"""Analysis tool: rebuild each fighter's PS1 cell lists from the arcade's art records and compare them with the disc.

What it does. The PS1 game keeps, for every pose of a fighter, a "cell list" (which 16x16 tiles, with which flags)
and a "position run" stream (where the tiles go) in the fighter's code module. Both are a mechanical conversion of
the arcade's art records. For every fighter file on the disc (PLnn.PAC and PLnnX.PAC) this tool finds where the
module's tables are, finds the arcade records the lists were made from (by comparing the pixels of the PS1 tiles
with the arcade's tiles), runs the conversion on those records and prints one line per file: how many lists, which
arcade segments, how many lists agree at the tile level, whether the position bytes and the list bytes equal the
disc's, how many tiles are stored flipped in the PS1 tile data, and how many tiles the numbering uses.

The rules it reproduces (the arcade record is: mode word 2, cell count, block count - 1, a coordinate index;
then blocks of tile code and attribute; the coordinates are blocks of x and y at the pointer found through the
index; see the README of this folder):
  - a block expands to the cells of its tile rectangle, flips included; the cells of a record are sorted by (y, x);
  - one counter per file numbers the tiles, from 1, in the order of first use (a code not seen before gets the
    next number); cells of horizontally adjacent tiles form position runs of three 16-bit values each;
  - identical position streams are stored once, in order of first use;
  - a list is a five-word header and a body in one of four modes (plain tiles, runs of consecutive tiles, tiles
    with flags, runs with flags); the shorter body wins and a tie goes to the run mode.

What it finds for itself, for each file (no debugger): the end of the module's code (after the last `jr ra` and its
delay slot); the chain of lists (the longest run of valid lists whose first position offset is 0); the arcade
record of each list, by pixels (a PS1 tile equals an arcade tile, or an arcade tile flipped); and the position
streams, by searching the module (and the file's other table-0 chunks) for the first converted stream. Beyond the
rules above it also meets, and handles only as far as stated:
  - a constant added to the record's tile codes (the video chip's code high bits, 0 to 3 times 0x10000): found by
    the pixels, one value per run of records;
  - empty lists (one cell, tile 0), which have no record; the last one ends the chain (the terminator);
  - lists with no mode-2 record: skipped (their numbers are reserved so that the counter stays in step, their
    position streams are copied from the module); the tool only says how many match a mode-0 record;
  - tiles stored flipped in the PS1 tile data ("baked flips"): counted and reported, and a list that differs and
    holds one is counted apart. No rule for them is made up.

Inputs: the disc image (raw 2352-byte sectors) and the arcade set's ZIP (MAME's sfa2u), both given as arguments
and both read only. It writes nothing except a temporary folder (a small C helper compiled at run time for the
tile comparison) that it removes. It prints sizes, addresses and counts, never game bytes beyond a few header
numbers. It is an analysis and reference tool; the converter that the port runs will be written in C.

    python3 port/arcade/fighters.py --disc BIN --arcade ZIP [--only NAME] [--detail]
"""
import argparse
import collections
import ctypes
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
from import_scene import lanes, unshuffle   # noqa: E402  the graphics layout the intro importer pins with the real set
import pac                                    # noqa: E402  the chunk archive reader of the PS1 tools

COORD_TABLE = 0x9f260      # arcade: table of pointers to coordinate blocks, indexed by the record's coordinate index
JR_RA = 0x03E00008
MODULE_SLOTS = {4, 5}      # table 0 code slots of the fighter module (first side, second side)
TILE_SLOTS = {1, 2}        # table 0 slots of the sprite tiles (first side, second side)
MAX_CELLS = 512            # a list longer than this is not read as a list
BANKS = (0, 0x10000, 0x20000, 0x30000)   # added to a record's tile codes: the video chip's code high bits
MIN_CHAIN = 20             # lists in a row that make a chain


# ---------------------------------------------------------------- pure functions (tested without game data)

def s16(v):
    return v - 65536 if v >= 32768 else v


class View:
    """The arcade program as the 68000 sees it: big-endian reads with bounds."""

    def __init__(self, data):
        self.data = data

    def u16(self, o):
        return struct.unpack_from('>H', self.data, o)[0]

    def s16(self, o):
        return s16(self.u16(o))

    def u32(self, o):
        return struct.unpack_from('>I', self.data, o)[0]


def expand_block(code, attr, x, y):
    """Cells (code, x, y, flags) of one block: attr bits 8-11 width-1, 12-15 height-1 (tiles), 5 flip x, 6 flip y."""
    w, h = ((attr >> 8) & 15) + 1, ((attr >> 12) & 15) + 1
    out = []
    for j in range(h):
        for i in range(w):
            ci = w - 1 - i if attr & 0x20 else i
            cj = h - 1 - j if attr & 0x40 else j
            out.append((code + ci + 16 * cj, x + 16 * i, y + 16 * j, attr & 0x7f))
    return out


def record_header(view, a):
    """(declared cells, blocks, pointer to the coordinates) of the art record at address a, or None.

    Only mode 2 (the fighters' form) is read; None when the record or its coordinates lie outside the data."""
    n = len(view.data)
    if a < 0 or a & 1 or a + 8 > n or view.u16(a) != 2:
        return None
    cells, nb, idx = view.u16(a + 2), view.u16(a + 4) + 1, view.s16(a + 6)
    if not 1 <= cells <= MAX_CELLS or a + 8 + 4 * nb > n or not 0 <= COORD_TABLE + idx <= n - 4:
        return None
    ptr = view.u32(COORD_TABLE + idx)
    if ptr & 1 or ptr + 4 * nb > n:
        return None
    return cells, nb, ptr


def read_record(view, a):
    """The art record at address a: (declared cells, cells (code, x, y, flags) unsorted, end address), or None."""
    head = record_header(view, a)
    if not head:
        return None
    cells, nb, ptr = head
    out = []
    for k in range(nb):
        out += expand_block(view.u16(a + 8 + 4 * k), view.u16(a + 10 + 4 * k), view.s16(ptr + 4 * k), view.s16(ptr + 4 * k + 2))
    return cells, out, a + 8 + 4 * nb


def block_cells(view, a, nb):
    """Cells the blocks of the record at a expand to, counted from the attributes only."""
    total = 0
    for k in range(nb):
        attr = view.u16(a + 10 + 4 * k)
        total += (((attr >> 8) & 15) + 1) * (((attr >> 12) & 15) + 1)
    return total


def read_record0(view, a):
    """The art record of mode 0 at address a, which the fighters' conversion does not use: (cells, cells, end) in the
    form of read_record, or None. It has a shared attribute word, a coordinate index and one tile code per cell."""
    n = len(view.data)
    if a < 0 or a & 1 or a + 8 > n or view.u16(a) != 0:
        return None
    cells, attr, idx = view.u16(a + 2), view.u16(a + 4), view.s16(a + 6)
    if not 1 <= cells <= MAX_CELLS or a + 8 + 2 * cells > n or not 0 <= COORD_TABLE + idx <= n - 4:
        return None
    ptr = view.u32(COORD_TABLE + idx)
    if ptr & 1 or ptr + 4 * cells > n:
        return None
    out = [(view.u16(a + 8 + 2 * k), view.s16(ptr + 4 * k), view.s16(ptr + 4 * k + 2), attr & 0x7f) for k in range(cells)]
    return cells, out, a + 8 + 2 * cells


def sort_cells(cells):
    """Cells ordered by (y, x); equal positions keep their order."""
    return sorted(cells, key=lambda c: (c[2], c[1]))


class Numbering:
    """One counter per file: a code not seen before gets the next number, starting at 1 (0 is reserved).

    A list that could not be converted still uses numbers; reserve() takes them, filed under the pixel class of
    their tile, and the first code of that class to come takes the reserved number instead of a new one."""

    def __init__(self):
        self.of = {}
        self.next = 1
        self.reserved = {}

    def number(self, code, cls=None):
        if code not in self.of:
            if cls is not None and cls in self.reserved:
                self.of[code] = self.reserved.pop(cls)
            else:
                self.of[code] = self.next
                self.next += 1
        return self.of[code]

    def reserve(self, number, cls):
        """Take `number` (and any below it not yet taken) for a tile of pixel class cls."""
        if number >= self.next:
            self.next = number + 1
            self.reserved[cls] = number


def position_runs(cells):
    """Runs of horizontally adjacent sorted cells: [x, y, length - 1]."""
    runs = []
    for _code, x, y, _fl in cells:
        r = runs[-1] if runs else None
        if r and r[1] == y and x == r[0] + 16 * (r[2] + 1):
            r[2] += 1
        else:
            runs.append([x, y, 0])
    return runs


def position_stream(runs):
    """Little-endian u16 x3 per run: dx from the previous run's last tile (0 first), dy from its y (0 first), length - 1."""
    out = bytearray()
    lx = ly = 0
    for x, y, n in runs:
        out += struct.pack('<hhH', x - lx, y - ly, n)
        lx, ly = x + 16 * n, y
    return bytes(out)


class Positions:
    """The file's position data: identical streams stored once, appended in order of first use."""

    def __init__(self):
        self.data = bytearray()
        self.at = {}

    def add(self, stream):
        if stream not in self.at:
            self.at[stream] = len(self.data)
            self.data += stream
        return self.at[stream]


def encode_list(cells, pos):
    """The list for numbered cells [(number, flags)] in sorted order and its position offset, as bytes.

    No flip bits and one flags value for all cells: mode 0 (a word per cell) or mode 4 (first number and run - 1 of
    consecutive numbers), the shorter wins, a tie goes to 4; the third header word is the palette. Otherwise mode 2
    (number and flags per cell) or mode 6 (first number, flags, run - 1 of consecutive numbers with equal flags), the
    shorter wins, a tie goes to 6; the third header word is cells - 1."""
    n = len(cells)
    if all(f & 0x60 == 0 for _t, f in cells) and len({f for _t, f in cells}) == 1:
        runs = []
        for t, _f in cells:
            if runs and runs[-1][0] + runs[-1][1] + 1 == t:
                runs[-1][1] += 1
            else:
                runs.append([t, 0])
        if n < 2 * len(runs):
            mode, body = 0, b''.join(struct.pack('<H', t) for t, _f in cells)
        else:
            mode, body = 4, b''.join(struct.pack('<HH', t, r) for t, r in runs)
        return struct.pack('<5H', n, mode, cells[0][1] & 0x1f, 0, pos) + body
    runs = []
    for t, f in cells:
        if runs and runs[-1][0] + runs[-1][2] + 1 == t and runs[-1][1] == f:
            runs[-1][2] += 1
        else:
            runs.append([t, f, 0])
    if 2 * n < 3 * len(runs):
        mode, body = 2, b''.join(struct.pack('<HH', t, f) for t, f in cells)
    else:
        mode, body = 6, b''.join(struct.pack('<HHH', t, f, r) for t, f, r in runs)
    return struct.pack('<5H', n, mode, n - 1, 0, pos) + body


def parse_list(mod, o):
    """Read the list at module offset o: dict(n, mode, third, pos, cells [(number word, flags or None)], end), or None.

    The cells are listed in the order of the body. A body that runs past the module or past its count is None."""
    if o < 0 or o & 1 or o + 10 > len(mod):
        return None
    n, mode, third, zero, pos = struct.unpack_from('<5H', mod, o)
    if not 1 <= n <= MAX_CELLS or mode not in (0, 2, 4, 6) or zero:
        return None
    i, cells = o + 10, []
    if mode in (0, 2):
        step = 2 if mode == 0 else 4
        if i + step * n > len(mod):
            return None
        for k in range(n):
            cells.append((struct.unpack_from('<H', mod, i + step * k)[0], None if mode == 0 else struct.unpack_from('<H', mod, i + 4 * k + 2)[0]))
        i += step * n
    else:
        step, count = (4, 0) if mode == 4 else (6, 0)
        while count < n:
            if i + step > len(mod):
                return None
            if mode == 4:
                t, run = struct.unpack_from('<HH', mod, i)
                fl = None
            else:
                t, fl, run = struct.unpack_from('<HHH', mod, i)
            count += run + 1
            if count > n:
                return None
            cells += [(t + r, fl) for r in range(run + 1)]
            i += step
    return dict(n=n, mode=mode, third=third, pos=pos, cells=cells, end=i)


def diff_kind(mine, disc):
    """What differs between two encoded lists (each starting at offset 0 of its bytes): 'numbering' (the tile
    numbers), 'position' (the position offset), 'flips' (flags differing only in the flip bits), 'palette' (other flag
    bits), 'encoding' (same cells, another mode) or 'structure' (one is not a list)."""
    a, b = parse_list(mine, 0), parse_list(disc, 0)
    if not a or not b:
        return 'structure'
    if [t for t, _f in a['cells']] != [t for t, _f in b['cells']]:
        return 'numbering'
    if a['pos'] != b['pos']:
        return 'position'
    bits = 0
    for (_t, fa), (_u, fb) in zip(a['cells'], b['cells']):
        bits |= (a['third'] if fa is None else fa) ^ (b['third'] if fb is None else fb)
    if not bits:
        return 'encoding'
    return 'flips' if not bits & ~0x60 else 'palette'


def code_end(mod):
    """Module offset just after the last `jr ra` and its delay slot, or None when there is none."""
    last = None
    for o in range(0, len(mod) - 3, 4):
        if struct.unpack_from('<I', mod, o)[0] == JR_RA:
            last = o
    return None if last is None else last + 8


def chain_from(mod, s, limit=None):
    """The lists one after another from offset s (each with its offset 'at'). With a limit, a list whose position
    offset is odd or not below the limit ends the chain."""
    lists, o = [], s
    while True:
        lst = parse_list(mod, o)
        if not lst or (limit is not None and (lst['pos'] & 1 or lst['pos'] >= limit)):
            return lists
        lists.append(dict(lst, at=o))
        o = lst['end']


def find_chain(mod, start):
    """The longest chain of at least MIN_CHAIN lists that begins at an even offset >= start with the positions
    before it: the first list's position offset is 0 and every offset is below the distance to `start`, where the
    positions begin. Returns (offset, lists) or None."""
    best = None
    s = start + (start & 1)
    while s < len(mod) - 10:
        first = parse_list(mod, s)
        lists = chain_from(mod, s, s - start) if first and first['pos'] == 0 else []
        if len(lists) >= MIN_CHAIN:
            if best is None or len(lists) > len(best[1]):
                best = (s, lists)
            s = lists[-1]['end']
        else:
            s += 2
    return best


def find_chain_first(mod, start):
    """A chain of at least MIN_CHAIN lists that begins right at the end of the code (the positions follow the
    lists): tried at `start` and at it rounded up to 8 and 16. Returns (offset, lists) or None."""
    best = None
    for s in sorted({start, (start + 7) & ~7, (start + 15) & ~15}):
        first = parse_list(mod, s)
        lists = chain_from(mod, s) if first and first['pos'] == 0 else []
        if len(lists) >= MIN_CHAIN and (best is None or len(lists) > len(best[1])):
            best = (s, lists)
    return best


def multiset_match(disc_tiles, arc_classes):
    """True when the arcade classes are those of the disc list's tiles.

    A disc tile is (class, flipped classes): its own pixels' class (-1 when no arcade tile has them) and the classes
    of arcade tiles that equal it when flipped (the PS1 may store a tile flipped). Tiles go first by their own class,
    then by a flipped class; a tile of no known class stands for one arcade tile of any class."""
    have = collections.Counter(arc_classes)
    rest, unknown = [], 0
    for own, flipped in disc_tiles:
        if own >= 0 and have[own] > 0:
            have[own] -= 1
        elif own < 0 and not flipped:
            unknown += 1
        else:
            rest.append((own, flipped))
    for own, flipped in rest:
        for c in flipped:
            if have[c] > 0:
                have[c] -= 1
                break
        else:
            return False
    return sum(have.values()) == unknown


# ---------------------------------------------------------------- the C helper (tile comparison)

HELPER = r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static const unsigned char *g_end;
static int g_bad;
static uint16_t rd(const unsigned char *b) { if (b + 2 > g_end) { g_bad = 1; return 0; } return b[0] | b[1] << 8; }
static int decode(const unsigned char *data, long size, int idx, unsigned char out[128]) {
    uint32_t off; memcpy(&off, data + 4 * idx, 4);
    if (off + 2 > (uint32_t)size) return -1;
    g_end = data + size; g_bad = 0;
    const unsigned char *p = data + off, *start = p;
    uint16_t n = rd(p); p += 2; int o = 0;
#define PUT(w) do { if (o >= 128) return -1; out[o] = (w) & 0xff; out[o + 1] = (w) >> 8; o += 2; } while (0)
    if (n == 0) { for (int i = 0; i < 64; i++) { PUT(rd(p)); p += 2; } }
    else if ((n & 0xff00) == 0) {
        n &= 0xff; uint16_t fw[16]; int nw = ((n - 1) >> 4) + 1;
        for (int i = 0; i < nw; i++) { fw[i] = rd(p); p += 2; }
        for (int k = 0; k < n; k++) {
            int bit = (fw[k >> 4] >> (k & 15)) & 1; uint16_t w = rd(p); p += 2;
            if (bit) { int len = (w & 3) + 2; const unsigned char *s = start - 2 * (w >> 2); if (s < data) return -1; for (int j = 0; j < len; j++) PUT(rd(s + 2 * j)); }
            else PUT(w);
        }
    } else if ((n & 0xff00) == 0x8000) {
        uint16_t fw[4]; for (int i = 0; i < 4; i++) { fw[i] = rd(p); p += 2; }
        for (int k = 0; k < 64; k++) { if ((fw[k >> 4] >> (k & 15)) & 1) { PUT(rd(p)); p += 2; } else PUT(0); }
    } else {
        n &= 0xff; uint16_t fw[16]; int nw = ((n - 1) >> 4) + 1;
        for (int i = 0; i < nw; i++) { fw[i] = rd(p); p += 2; }
        for (int k = 0; k < n; k++) {
            int bit = (fw[k >> 4] >> (k & 15)) & 1;
            if (p >= g_end) return -1;
            unsigned char v = *p++;
            if (bit) { if (p >= g_end) return -1; int c = *p++; for (int j = 0; j < c; j++) { if (o >= 128) return -1; out[o++] = v; } }
            else { if (o >= 128) return -1; out[o++] = v; }
        }
    }
    if (g_bad) return -1;
    return o == 128 ? 0 : -2;
}
static uint64_t hsh(const unsigned char *px, int fl) {
    uint64_t h = 1469598103934665603ull;
    for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) {
        int sx = (fl & 1) ? 15 - x : x, sy = (fl & 2) ? 15 - y : y;
        h = (h ^ px[sy * 16 + sx]) * 1099511628211ull;
    }
    return h | 1;
}
#define EB 19
static uint64_t *ek, *fk[3]; static uint32_t *ev, *fv[3];
static void put(uint64_t *k, uint32_t *v, int bits, uint64_t key, uint32_t val) {
    uint64_t m = (1ull << bits) - 1, i = (key >> 9) & m;
    while (k[i] && k[i] != key) i = (i + 1) & m;
    if (!k[i]) { k[i] = key; v[i] = val; }
}
static long get(uint64_t *k, uint32_t *v, int bits, uint64_t key) {
    uint64_t m = (1ull << bits) - 1, i = (key >> 9) & m;
    while (k[i]) { if (k[i] == key) return v[i]; i = (i + 1) & m; }
    return -1;
}
/* Arcade tiles of 128 bytes; pixel = (index + 1) & 15 as the PS1 stores it. canon[code] = lowest code with the same pixels. */
int arc_init(const unsigned char *gfx, long ntiles, int32_t *canon) {
    free(ek); free(ev);
    ek = calloc(1ull << EB, 8); ev = calloc(1ull << EB, 4);
    if (!ek || !ev || ntiles > (1l << 18)) return -1;
    for (int f = 0; f < 3; f++) {
        free(fk[f]); free(fv[f]);
        fk[f] = calloc(1ull << EB, 8); fv[f] = calloc(1ull << EB, 4);
        if (!fk[f] || !fv[f]) return -1;
    }
    unsigned char px[256];
    for (long t = 0; t < ntiles; t++) {
        const unsigned char *q = gfx + t * 128;
        for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) {
            int b = 0; for (int pl = 0; pl < 4; pl++) b |= ((q[y * 8 + (x / 8) * 4 + pl] >> (7 - x % 8)) & 1) << pl;
            px[y * 16 + x] = (b + 1) & 15;
        }
        put(ek, ev, EB, hsh(px, 0), (uint32_t)t);
        for (int fl = 1; fl < 4; fl++) put(fk[fl - 1], fv[fl - 1], EB, hsh(px, fl), (uint32_t)t);
        canon[t] = (int32_t)get(ek, ev, EB, hsh(px, 0));
    }
    return 0;
}
/* Per tile of a PS1 sprite chunk: exact[i] = canonical arcade code with equal pixels, else -1; flipped[3 * i + f - 1] = canonical code
 * of the arcade tile whose pixels flipped by f (1 across, 2 down, 3 both) equal it, else -1; -2 everywhere when the tile does not
 * decode. Returns the tile count. */
long ps1_classify(const unsigned char *data, long size, int32_t *exact, int32_t *flipped, long maxn) {
    if (size < 4) return -1;
    uint32_t first; memcpy(&first, data, 4);
    long count = first / 4;
    if (count > maxn || (long)first > size) return -1;
    unsigned char o[128], px[256];
    for (long i = 0; i < count; i++) {
        if (decode(data, size, (int)i, o)) { exact[i] = flipped[3 * i] = flipped[3 * i + 1] = flipped[3 * i + 2] = -2; continue; }
        for (int k = 0; k < 128; k++) { px[2 * k] = o[k] & 15; px[2 * k + 1] = o[k] >> 4; }
        uint64_t h = hsh(px, 0);
        exact[i] = (int32_t)get(ek, ev, EB, h);
        for (int f = 0; f < 3; f++) flipped[3 * i + f] = (int32_t)get(fk[f], fv[f], EB, h);
    }
    return count;
}
'''


class Helper:
    def __init__(self, tmp, gfx):
        src = Path(tmp) / 'tiles.c'
        src.write_text(HELPER)
        lib = Path(tmp) / 'tiles.so'
        subprocess.run(['cc', '-O2', '-shared', '-fPIC', '-o', str(lib), str(src)], check=True, capture_output=True, timeout=60)
        self.lib = ctypes.CDLL(str(lib))
        self.lib.arc_init.argtypes = [ctypes.c_char_p, ctypes.c_long, ctypes.c_void_p]
        self.lib.ps1_classify.argtypes = [ctypes.c_char_p, ctypes.c_long, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_long]
        self.lib.ps1_classify.restype = ctypes.c_long
        self.ntiles = len(gfx) // 128
        canon = (ctypes.c_int32 * self.ntiles)()
        if self.lib.arc_init(gfx, self.ntiles, canon):
            raise RuntimeError('tile helper could not start')
        self.canon = list(canon)

    def classify(self, chunk):
        cap = 1 << 16
        ex, fl = (ctypes.c_int32 * cap)(), (ctypes.c_int32 * (3 * cap))()
        n = self.lib.ps1_classify(chunk, len(chunk), ex, fl, cap)
        if n < 0:
            raise ValueError('tile chunk')
        return list(ex[:n]), list(fl[:3 * n])


# ---------------------------------------------------------------- inputs

def iso_files(f):
    """{path: (lba, size)} of the disc's ISO 9660 files (raw 2352-byte sectors, data at +24)."""
    def sec(lba, n=1):
        out = b''
        for i in range(n):
            f.seek((lba + i) * 2352 + 24)
            out += f.read(2048)
        return out
    files = {}

    def walk(lba, size, path):
        d, i = sec(lba, (size + 2047) // 2048), 0
        while i < len(d):
            ln = d[i]
            if ln == 0:
                i = (i // 2048 + 1) * 2048
                continue
            e = d[i:i + ln]
            i += ln
            el, sz = struct.unpack('<I', e[2:6])[0], struct.unpack('<I', e[10:14])[0]
            nm = e[33:33 + e[32]]
            if nm in (b'\0', b'\1'):
                continue
            p = path + '/' + nm.decode('latin1').split(';')[0]
            if e[25] & 2:
                walk(el, sz, p)
            else:
                files[p] = (el, sz)
    root = sec(16)[156:156 + 34]
    walk(struct.unpack('<I', root[2:6])[0], struct.unpack('<I', root[10:14])[0], '')
    return files, sec


def arcade_views(zip_path):
    """(program as the 68000 sees it, graphics as the video chip reads them) from the set's ZIP."""
    with zipfile.ZipFile(zip_path) as z:
        raw = b''.join(z.read(f'sz2u.{i:02d}') for i in range(3, 9))
        prog = bytearray(len(raw))
        prog[::2], prog[1::2] = raw[1::2], raw[::2]
        gfx = unshuffle(b''.join(lanes([z.read(f'sz2.{i:02d}m') for i in g]) for g in ((13, 15, 17, 19), (14, 16, 18, 20))))
    return bytes(prog), gfx


def split_file(data):
    """(module, tiles, side, other table-0 chunks as (slot, bytes)) of a fighter file, or an error string. Chunks are told
    apart by table and slot."""
    chunks = pac.chunk_table(data)
    mods = [c for c in chunks if c['table'] == 0 and c['slot'] in MODULE_SLOTS]
    tiles = [c for c in chunks if c['table'] == 0 and c['slot'] in TILE_SLOTS]
    if len(mods) != 1 or len(tiles) != 1:
        return f'{len(mods)} module and {len(tiles)} tile chunks'
    cut = lambda c: data[c['offset']:c['offset'] + c['size']]
    others = [(c['slot'], cut(c)) for c in chunks if c['table'] == 0 and c['slot'] not in MODULE_SLOTS | TILE_SLOTS | {0xffff} and c['size'] > 0x1000]
    return cut(mods[0]), cut(tiles[0]), 2 if mods[0]['slot'] == 5 else 1, others


# ---------------------------------------------------------------- the analysis of one file

class Index:
    """Mode-2 art records of the arcade program by declared cell count (only those whose blocks add up to it)."""

    def __init__(self, view):
        self.view = view
        self.by_cells = collections.defaultdict(list)
        self.cache = {}
        self._mode0 = None
        d, pos = view.data, 0
        while True:
            pos = d.find(b'\x00\x02', pos)
            if pos < 0:
                break
            if not pos & 1:
                head = record_header(view, pos)
                if head and block_cells(view, pos, head[1]) == head[0]:
                    self.by_cells[head[0]].append(pos)
            pos += 1

    def record(self, a):
        if a not in self.cache:
            self.cache[a] = read_record(self.view, a)
        return self.cache[a]

    def mode0(self):
        """Mode-0 records by cell count, indexed the first time they are asked for."""
        if self._mode0 is None:
            self._mode0 = collections.defaultdict(list)
            d = self.view.data
            for a in range(0, len(d) - 8, 2):
                if d[a] == 0 and d[a + 1] == 0 and d[a + 2] == 0 and 1 <= d[a + 3] <= 64 and read_record0(self.view, a):
                    self._mode0[self.view.u16(a + 2)].append(a)
        return self._mode0

    def record0(self, a):
        return read_record0(self.view, a)


def is_empty(lst):
    """A list of one cell, tile 0, is an empty list: no record, no position data; its position offset is the length
    of the positions so far. The last list of a file is one (the terminator)."""
    return lst['n'] == 1 and lst['mode'] == 0 and lst['cells'] == [(0, None)]


def list_palettes(lst):
    """The palette of each cell of a disc list: the header's for the plain modes, the flags' low five bits otherwise."""
    if lst['mode'] in (0, 4):
        return [lst['third']] * lst['n']
    return [f & 0x1f for _t, f in lst['cells']]


def stream_end(mod, at, n):
    """End offset of the position stream at `at` that covers n cells, or None."""
    c = 0
    while c < n:
        if at + 6 > len(mod):
            return None
        c += struct.unpack_from('<H', mod, at + 4)[0] + 1
        at += 6
    return at if c == n else None


def analyse(mod, tiles, view, index, helper, others=()):
    """Everything the line and the detail need, as a dict. The key 'error' when the file cannot be read."""
    pend = code_end(mod)
    if pend is None:
        return dict(error='no jr ra in the module')
    a, b = find_chain(mod, pend), find_chain_first(mod, pend)
    if not a and not b:
        return dict(error=f'no list chain after {pend:#x}')
    first = bool(b) and (not a or len(b[1]) > len(a[1]))
    lbase, lists = b if first else a
    r = dict(codeend=pend, lbase=lbase, lists_first=first)
    ex, fl = helper.classify(tiles)
    canon = helper.canon
    ntile = len(ex)

    def ps1_tile(num):
        t = num & 0x3fff
        if t >= ntile or ex[t] == -2:
            return -1, ()
        return ex[t], tuple(c for c in fl[3 * t:3 * t + 3] if c >= 0)

    def record_classes(rec, bank):
        return [canon[c[0] + bank] if c[0] + bank < helper.ntiles else -2 for c in rec[1]]

    def matches(lst, a, bank):
        """(tiles agree, palettes agree, record) of list lst against the record at a."""
        rec = index.record(a)
        if not rec or rec[0] != lst['n'] or len(rec[1]) != lst['n']:
            return False, False, rec
        if not multiset_match([ps1_tile(t) for t, _f in lst['cells']], record_classes(rec, bank)):
            return False, False, rec
        return True, sorted(list_palettes(lst)) == sorted(c[3] & 0x1f for c in rec[1]), rec

    def walk(k, a, bank):
        """How many lists from k on agree with the records from a on, and how many of those in palette too."""
        cnt = same = 0
        while k + cnt < len(lists):
            if is_empty(lists[k + cnt]):        # an empty list has no record: the records go on after it
                cnt += 1
                continue
            ok, pal, rec = matches(lists[k + cnt], a, bank)
            if not ok:
                break
            a = rec[2]
            cnt += 1
            same += pal
        return cnt, same

    # which list belongs to which record: segments of lists whose records follow one another; among records that
    # agree equally far, the one whose palettes agree too
    seg, unmatched, recs = [], [], {}
    k = 0
    while k < len(lists):
        best = ((0, 0), None, 0)
        if not is_empty(lists[k]):
            for a in index.by_cells.get(lists[k]['n'], ()):
                for bank in BANKS:
                    score = walk(k, a, bank)
                    if score > best[0]:
                        best = (score, a, bank)
        if not best[0][0]:
            if not is_empty(lists[k]):
                unmatched.append(k)
            k += 1
            continue
        a, count = best[1], best[0][0]
        seg.append((a, sum(1 for j in range(count) if not is_empty(lists[k + j])), k, best[2]))
        for j in range(count):
            if not is_empty(lists[k + j]):
                rec = index.record(a)
                recs[k + j] = (rec, best[2])
                a = rec[2]
        k += count

    # the terminator is the first empty list after the last list that has a record; what follows it is not a list
    last = max(recs, default=-1)
    stop = next((k for k in range(last + 1, len(lists)) if is_empty(lists[k])), None)
    term = None
    if stop is not None:
        term = lists[stop]
        del lists[stop:]
        unmatched = [k for k in unmatched if k < stop]
    lend = lists[-1]['end'] if term is None else term['end']
    # a list without a record: does a record of mode 0 (not converted here) have its tiles?
    alt0 = []
    for k in unmatched:
        lst = lists[k]
        cls = [ps1_tile(t) for t, _f in lst['cells']]
        pal = sorted(list_palettes(lst))
        for a in index.mode0().get(lst['n'], ()):
            rec = index.record0(a)
            if any(multiset_match(cls, [canon[c[0] + bank] if c[0] + bank < helper.ntiles else -2 for c in rec[1]]) and
                   pal == sorted(c[3] & 0x1f for c in rec[1]) for bank in BANKS):
                alt0.append((k, a))
                break
    r['alt0'] = alt0
    r.update(segments=seg, unmatched=unmatched, lists=lists, term=term, listsend=lend)

    def convert(posbase, posmod):
        """The lists as bytes (None where no record), the numbering and the positions. A list without a record keeps
        the counters in step: its numbers are taken and, when its position stream is new, the stream is copied from
        the module at posbase."""
        num, pos, mine, code_of_num = Numbering(), Positions(), [], {}
        for k, lst in enumerate(lists):
            if is_empty(lst):
                mine.append((struct.pack('<5HH', 1, 0, 0, 0, len(pos.data), 0), []))
                continue
            if k not in recs:
                for t, _f in lst['cells']:
                    if (t & 0x3fff) >= num.next:
                        num.reserve(t & 0x3fff, ps1_tile(t)[0])
                end = None if posbase is None else stream_end(posmod, posbase + lst['pos'], lst['n'])
                if end is not None and lst['pos'] == len(pos.data):
                    pos.data += posmod[posbase + lst['pos']:end]
                mine.append(None)
                continue
            rec, bank = recs[k]
            cells = sort_cells([(code + bank, x, y, f) for code, x, y, f in rec[1]])
            numbered = []
            for code, _x, _y, f in cells:
                before = num.next
                n = num.number(code, canon[code] if code < helper.ntiles else None)
                if n >= before or n not in code_of_num:
                    code_of_num[n] = code
                numbered.append((n, f))
            mine.append((encode_list(numbered, pos.add(position_stream(position_runs(cells)))), [n for n, _f in numbered]))
        return mine, num, pos, code_of_num

    mine, num, pos, code_of_num = convert(None, mod)
    # where the positions are: the first list's stream, converted, is searched for in the module (before the lists, or
    # after them when the lists come first), then in the file's other table-0 chunks
    first_stream = bytes(pos.data[:24]) if 0 in recs else b''
    lo, hi = (lend, len(mod)) if first else (pend, lbase)
    posmod, where = mod, None
    at = mod.find(first_stream, lo, hi) if first_stream else -1
    for slot, chunk in others:
        if at < 0 and first_stream:
            at, posmod, where = chunk.find(first_stream), chunk, f'slot {slot:#x}'
    posbase = at if at >= 0 else None
    if posbase is not None:
        mine, num, pos, code_of_num = convert(posbase, posmod)
    r.update(posbase=posbase, pos_where=where if posbase is not None else None)

    # tile level: every number of a converted list holds the pixels of its code (flipped storage counts as a match)
    baked, wrong = {}, {}
    for n, code in code_of_num.items():
        want = canon[code] if code < helper.ntiles else -2
        if n < ntile and ex[n] >= 0 and ex[n] == want:
            continue
        flips = [f + 1 for f in range(3) if n < ntile and fl[3 * n + f] == want]
        if flips:
            baked[n] = (code, flips[0])
        else:
            wrong[n] = code
    r.update(baked=baked, wrong=wrong, numbers=num.next - 1, chunk_tiles=ntile, tiles_ok=len(recs), tiles_of=sum(1 for lst in lists if not is_empty(lst)))

    # positions
    pbytes = bytes(pos.data)
    disc_pos = b'' if posbase is None else posmod[posbase:posbase + len(pbytes)]
    r['pos_len'] = len(pbytes)
    r['term_ok'] = None if term is None else term['pos'] == len(pbytes)
    r['pos_diff'] = None if pbytes == disc_pos else next((i for i, (x, y) in enumerate(zip(pbytes, disc_pos)) if x != y), len(disc_pos))

    # list bytes: each list against the disc's at its own place; a difference in a list with a tile stored flipped is counted apart
    r.update(bytes_equal=0, baked_diff=0, kinds=collections.Counter(), first_bad=None, first_other=None)
    g = lambda m, i: struct.unpack_from('<H', m, 2 * i)[0] if 2 * i + 2 <= len(m) else None
    for k, lst in enumerate(lists):
        if mine[k] is None:
            continue
        m, numbers = mine[k]
        disc = mod[lst['at']:lst['end']]
        if m == disc:
            r['bytes_equal'] += 1
            continue
        w = next((i for i in range(min(len(m), len(disc)) // 2) if m[2 * i:2 * i + 2] != disc[2 * i:2 * i + 2]), min(len(m), len(disc)) // 2)
        bad = dict(k=k, at=lst['at'], word=w, mine=g(m, w), disc=g(disc, w), mine_len=len(m), disc_len=len(disc), n=lst['n'], mode=lst['mode'])
        if any(n in baked for n in numbers):
            r['baked_diff'] += 1
        else:
            bad['kind'] = diff_kind(m, disc)
            r['kinds'][bad['kind']] += 1
            if r['first_other'] is None:
                r['first_other'] = bad
        if r['first_bad'] is None:
            r['first_bad'] = bad
    r['mine_count'] = sum(1 for m in mine if m is not None)
    return r


def line(name, side, r):
    if 'error' in r:
        return f'{name} side {side}: {r["error"]}'
    names = [f'{a:#x}+{c}' + (f'@{b >> 16}' if b else '') for a, c, _k, b in r['segments']]
    segs = ' '.join(names) if len(names) <= 4 else ' '.join(names[:3]) + f' ... {len(names)} segments'
    s = f'{name} side {side}: {len(r["lists"]) + (r["term"] is not None)} lists, segments {segs or "none"}; tiles {r["tiles_ok"]}/{r["tiles_of"]} lists'
    if r['unmatched']:
        s += f'; unmatched lists {len(r["unmatched"])} (first {r["unmatched"][0]}, {len(r["alt0"])} match a mode 0 record)'
    if r['posbase'] is None:
        s += '; positions not found'
    else:
        s += f'; positions {r["pos_len"]} B ' + ('equal' if r['pos_diff'] is None else f'differ from +{r["pos_diff"]:#x}')
    if r['pos_where']:
        s += f' (in the chunk of {r["pos_where"]})'
    elif r['lists_first'] and r['posbase'] is not None:
        s += ' (after the lists)'
    if r['term'] is None:
        s += '; no terminator'
    elif not r['term_ok']:
        s += '; terminator offset off'
    fb = r['first_bad']
    if not r['mine_count']:
        s += '; no list converted'
    elif fb is None:
        s += '; list bytes all equal'
    else:
        other = sum(r['kinds'].values())
        s += (f'; list bytes equal up to list {fb["k"]} (+{fb["at"]:#x}); {r["bytes_equal"]}/{r["mine_count"]} equal, '
              f'{r["baked_diff"]} with baked tiles, {other} other')
        if other:
            s += ' (' + ', '.join(f'{k} {v}' for k, v in r['kinds'].most_common()) + ')'
    s += f'; baked flips {len(r["baked"])}'
    if r['wrong']:
        s += f'; tile mismatches {len(r["wrong"])}'
    s += f'; numbering {r["numbers"]} of {r["chunk_tiles"]} tiles'
    return s


def details(name, r):
    """At most six short lines: the first mismatch of each kind, as numbers and addresses (module offsets)."""
    out = []
    if 'error' in r:
        return out
    pos = 'not found' if r['posbase'] is None else f'{r["posbase"]:#x}'
    out.append(f'  {name}: module offsets: code end {r["codeend"]:#x}, positions {pos}, lists {r["lbase"]:#x} to {r["listsend"]:#x}')
    if r['unmatched']:
        k = r['unmatched'][0]
        lst = r['lists'][k]
        out.append(f'  {name}: first list without a record: {k} at +{lst["at"]:#x}: {lst["n"]} cells, mode {lst["mode"]}, '
                   f'{len(r["unmatched"])} in all')
    if r['pos_diff'] is not None:
        out.append(f'  {name}: positions differ at +{r["pos_diff"]:#x} of {r["pos_len"]}')
    b = r['first_other']
    if b:
        out.append(f'  {name}: first {b["kind"]} difference without a baked tile: list {b["k"]} at +{b["at"]:#x}: n {b["n"]} mode {b["mode"]}, '
                   f'length mine {b["mine_len"]} disc {b["disc_len"]}, word {b["word"]}: mine {b["mine"]} disc {b["disc"]}')
    if r['baked']:
        n = min(r['baked'])
        code, fl = r['baked'][n]
        out.append(f'  {name}: first baked flip: tile {n} is arcade tile {code} flipped ({fl})')
    if r['wrong']:
        n = min(r['wrong'])
        out.append(f'  {name}: first tile mismatch: tile {n} is not arcade tile {r["wrong"][n]}')
    return out[:6]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00.PAC or PL00)')
    ap.add_argument('--detail', action='store_true', help='up to six extra lines per file for the first mismatch of each kind')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='fighters-')
    try:
        prog, gfx = arcade_views(args.arcade)
        view = View(prog)
        index = Index(view)
        helper = Helper(tmp, gfx)
        with open(args.disc, 'rb') as f:
            files, sec = iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
            if args.only:
                want = args.only.upper().removesuffix('.PAC') + '.PAC'
                names = [p for p in names if p.endswith('/' + want)]
            for p in names:
                lba, size = files[p]
                data = sec(lba, (size + 2047) // 2048)[:size]
                name = p.split('/')[-1]
                try:
                    parts = split_file(data)
                except (pac.FormatError, struct.error) as exc:
                    print(f'{name}: {exc}')
                    continue
                if isinstance(parts, str):
                    print(f'{name}: {parts}')
                    continue
                mod, tiles, side, others = parts
                r = analyse(mod, tiles, view, index, helper, others)
                print(line(name, side, r))
                if args.detail:
                    for d in details(name, r):
                        print(d)
        print(f'{len(names)} files in {time.monotonic() - started:.1f} s')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
