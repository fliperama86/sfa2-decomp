#!/usr/bin/env python3
"""Analysis tool and specification: the cell lists and position runs of the screen modules, rebuilt from the arcade's art records.

What it does. The select (SELECTA), character-demo (CDEMO), continue (CONT), ending (END) and attract (DEMO) files of the PS1 disc each hold, in
their module chunk, blocks of cell lists with the position runs in front of them (the format of the fighters' modules, read by
unpack_cell_list_into_stream_cells and build_sprite_quads_from_stream). A block is a "chain": the position runs, then lists one after another, the
first list's position offset 0. A module has one chain for the art of one tile segment, so a file has many (CDEMO 33, END 18, CONT 43). The tool
finds every chain, the tile segment it numbers into, the arcade art record behind each list, and prints one line per file with how many lists it
rebuilds byte for byte. The fighters' rules (fighters.py) carry over with these differences, all found from the data:

  - Numbers. A list's tile numbers are the indexes of tiles in the tile segment the chain belongs to (any segment of the file's chunks; the
    shared library of DEMO, SELECT and CONT has one segment per chain). The PS1 numbers a tile by first use, one number a pixel class (the arcade
    tile, or the same pixels flipped), so the number of a cell is found by looking its tile up in the segment: the lowest number (from 1) whose
    tile is in the cell's class. The segment's tiles are the screens recipe's (screens_table.c: item k of the segment is number k, an arcade tile
    with a stored flip, a blank tile, or not the arcade's). The class of a tile is its pixels or those flipped across and down, whichever bytes
    sort lowest ("canonical"); d is the lowest flip that turns the tile into the canonical one. A tile stored with flip s has stored flip
    e = s xor d(tile); a cell of code c and flip bits u (flags 0x20 across, 0x40 down) gets number n and flip bits g = u xor d(c) xor e(n).
    Exact for every matched list of the five kinds (fighters.py's counter rule needed a first-use order that the segment now gives).
  - Parts. A record's blocks are in parts; the PS1 lists the parts last first, each part by (y, x). A block starts a new part when its x differs
    from the previous block's x by something that is not a multiple of 16 (the two blocks cannot form a run of adjacent tiles), and, in the
    chains with the rule PAL_SPLIT (SELECTA's), also when its palette differs. (fighters.py's "x less than the previous block's" is not needed.)
  - Palettes. The list's palette numbers are the PS1's, not the arcade's: the same cell has another number where the screen loads its colours
    into other rows of the PS1's palette memory. Per run of records one map (arcade number to PS1 number; numbers not in it stay) gives them
    (CDEMO: 20, 21, 28, 29 all become 0; SELECTA: 28 becomes 0 in the name plates, 29 becomes 30 in the second player's copies; END: 6 becomes 23
    or 0). A few cells keep their arcade number against the map (the cap of a name plate): exceptions, by list and cell. They have no source.
  - Own lists. Lists with no arcade record (text lines, pictures cut from scroll data, poses and pictures of the PS1's own) are "own": the table
    keeps their byte length and the bytes they add to the position runs, the loader gets their bytes from the disc. So do the lists
    with n = 1 and tile 0 whose palette or position offset is not the "empty list" form. An empty list (one cell, tile 0, palette 0, position
    offset the size of the runs so far) is built.
  - Position runs: identical streams stored once, in order of first use (as fighters.py); streams of own lists count only by length.
    A record drawn a second time far left (fighters.py's "again" rule) did not occur.

For each chain the tool needs the position runs' offset (found: the distance to the chain where every list's stream fits and the last one ends at
the chain), the tile segment (found: the one whose lookups reproduce the most lists' numbers), and for each list the record: the records with
the same cell positions (sorted) are tried, in the tile banks 0 to 3 times 0x10000, and the one whose lookups give the list's numbers and flips
wins (the record that follows the previous list's, if several do). The palette map and the exceptions are what is left. With the part rule
chosen per chain by the number of lists that then come out equal, lists that still differ become own lists, until none differs.

Options. With --c-table it prints port/arcade/screen_lists_table.c (per chain: rooms, rule flags, items, maps, exceptions; per file: where
each chain lies in the module and which segment it numbers into; no game byte). With --compare-c it builds screen_lists.c with the checked-in
table and prints one line per file: the C against the model and the C against the disc on the bytes the C claims.

Inputs: the disc image (raw 2352-byte sectors) and the arcade set's ZIP (MAME's sfa2u), both read only. It writes nothing except a temporary
folder (a small C helper compiled at run time for the tile classes) that it removes. It prints counts, addresses and sizes, never game bytes.
It is an analysis and reference tool; the converter that the port runs is C (screen_lists.c).

    python3 port/arcade/screen_lists.py --disc BIN --arcade ZIP [--only NAME] [--detail] [--c-table | --compare-c]
"""
import argparse
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

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import pac        # noqa: E402  the chunk archive reader of the PS1 tools
import fighters   # noqa: E402  the arcade views, the disc reader, the list reader and encoder (the format is the fighters')

# the kinds of file and the table 0 slot of the module chunk that holds their cell lists
KINDS = (('SELECTA', 0x27), ('CDEMO', 1), ('CONT', 0x12), ('DEMO', 0xf), ('END', 0x28))
NAME_RE = re.compile(r'^(SELECTA|CDEMO|CONT|DEMO|END)([0-9A-F]{2}X?)?\.PAC$')
BANKS = (0, 0x10000, 0x20000, 0x30000)
PAL_SPLIT = 1                 # rule: a change of palette between blocks starts a part (SELECTA)
RULE_NAMES = ((PAL_SPLIT, 'PAL_SPLIT'),)
REC, EMPTY, OWN = 0, 1, 2     # item kinds
MIN_CHAIN = 2                 # lists in a row that make a chain
MAX_CELLS = 512
Z, D, P = 0xfffffffe, 0xffffffff, 0xfffffffd   # screens items: blank tiles, tiles the arcade lacks, tiles of pieces
KIND_OF_REGION = {'PALS': 3, 'RAW': 2, 'SEG': 1, 'PIC': 4}


# ---------------------------------------------------------------- pure functions (tested without game data)

def flip_pixels(px, f):
    """The 256 pixels of a 16 x 16 tile flipped across (bit 0 of f) and/or down (bit 1)."""
    out = bytearray(256)
    for y in range(16):
        sy = 15 - y if f & 2 else y
        for x in range(16):
            out[y * 16 + x] = px[sy * 16 + (15 - x if f & 1 else x)]
    return bytes(out)


def arcade_pixels(q):
    """The PS1 pixels (arcade value plus one, modulo 16) of the 128 bytes of an arcade tile: four planes, 8 pixels to a byte, 4 bytes to 8 pixels."""
    out = bytearray(256)
    for y in range(16):
        for x in range(16):
            v = 0
            for pl in range(4):
                v |= ((q[y * 8 + (x // 8) * 4 + pl] >> (7 - x % 8)) & 1) << pl
            out[y * 16 + x] = (v + 1) & 15
    return bytes(out)


def fnv64(data):
    h = 14695981039346656037
    for b in data:
        h = ((h ^ b) * 1099511628211) & 0xffffffffffffffff
    return h


def pixel_class(px):
    """(key, d) of a tile's 256 pixels: the key is the FNV-1a hash of the lowest of the four flipped versions, d the lowest flip giving it."""
    best = min((flip_pixels(px, f), f) for f in range(4))
    return fnv64(best[0]), best[1]


def record_blocks(data, a, coord_table=fighters.COORD_TABLE):
    """(blocks [(code, attr, x, y)], end address) of the art record of mode 2 or 0 at address a of the 68000's view `data`, or None.

    Mode 2: blocks of code and attribute (width - 1 in bits 8-11, height - 1 in 12-15, palette 0-4, flips 5, 6). Mode 0: a shared attribute
    word and one code a cell, each a 1 x 1 block. The coordinates are at the pointer found through the record's index."""
    view = fighters.View(data)
    n = len(data)
    if a < 0 or a & 1 or a + 8 > n:
        return None
    mode = view.u16(a)
    if mode not in (0, 2):
        return None
    cells, third, idx = view.u16(a + 2), view.u16(a + 4), view.s16(a + 6)
    nb, stride = (third + 1, 4) if mode == 2 else (cells, 2)
    if not 1 <= cells <= MAX_CELLS or a + 8 + stride * nb > n or not 0 <= coord_table + idx <= n - 4:
        return None
    ptr = view.u32(coord_table + idx)
    if ptr & 1 or ptr + 4 * nb > n:
        return None
    blocks = []
    for k in range(nb):
        code = view.u16(a + 8 + stride * k)
        attr = view.u16(a + 10 + 4 * k) if mode == 2 else third & 0x7f
        blocks.append((code, attr, view.s16(ptr + 4 * k), view.s16(ptr + 4 * k + 2)))
    total = sum((((at >> 8) & 15) + 1) * (((at >> 12) & 15) + 1) for _c, at, _x, _y in blocks)
    return (blocks, a + 8 + stride * nb) if total == cells else None


def block_parts(blocks, pal_split):
    """Part number of each block: a block starts a new part when its x differs from the previous block's by a non-multiple of 16, or, with
    pal_split, when its palette (attribute bits 0-4) differs."""
    out, g = [], 0
    for i, (_c, attr, x, _y) in enumerate(blocks):
        if i and ((x - blocks[i - 1][2]) % 16 or (pal_split and (attr ^ blocks[i - 1][1]) & 0x1f)):
            g += 1
        out.append(g)
    return out


def emitted_cells(blocks, bank, pal_split):
    """The record's cells (code + bank, x, y, flags) in the PS1's order: the parts last first, each part by (y, x), equal positions in record order."""
    parts = block_parts(blocks, pal_split)
    cells = []
    for b, (code, attr, x, y) in enumerate(blocks):
        w, h = ((attr >> 8) & 15) + 1, ((attr >> 12) & 15) + 1
        for j in range(h):
            for i in range(w):
                ci = w - 1 - i if attr & 0x20 else i
                cj = h - 1 - j if attr & 0x40 else j
                cells.append((code + ci + 16 * cj + bank, x + 16 * i, y + 16 * j, attr & 0x7f, parts[b]))
    return [c[:4] for c in sorted(cells, key=lambda c: (-c[4], c[2], c[1]))]


class Pool:
    """The numbers of a tile segment: tiles [(arcade code, stored flip) or 'zero' or None], number k being tiles[k]. lookup() finds a cell's number.

    cls(code) gives (key, d) of an arcade tile; zero_key is the key of the blank tile (value 0 everywhere); a tile of None (the PS1's own) has no class."""

    def __init__(self, tiles, cls, zero_key):
        self.of = {}
        self.tiles = tiles
        for k, t in enumerate(tiles):
            if k == 0 or t is None:
                continue
            if t == 'zero':
                key, e = zero_key, 0
            else:
                c = cls(t[0])
                if c is None:
                    continue
                key, e = c[0], t[1] ^ c[1]
            self.of.setdefault(key, (k, e))
        self.cls = cls

    def lookup(self, code, u):
        """(number, flip bits g) of a cell of arcade tile `code` drawn with flip bits u (1 across, 2 down), or None when the segment lacks the tile."""
        c = self.cls(code)
        if c is None or c[0] not in self.of:
            return None
        n, e = self.of[c[0]]
        return n, u ^ c[1] ^ e


def position_stream_of(cells):
    """The position runs of a list's cells: fighters.py's runs of horizontally adjacent tiles, three 16-bit values each."""
    return fighters.position_stream(fighters.position_runs(cells))


class Built:
    """What the model builds of a chain: the position runs and the lists with a mask of the bytes that are built (the rest is the disc's)."""

    def __init__(self):
        self.positions, self.pos_known = bytearray(), bytearray()
        self.lists, self.lists_known = bytearray(), bytearray()
        self.list_at = []            # offset of each list in `lists`
        self.problems = []           # (list index, why) for a record or tile that cannot be used


def model_chain(prog, pool, items, rules, maps, exceptions):
    """The specification of screen_lists.c: build a chain from the arcade program `prog` (68000 view), the segment `pool`, and
    items [(REC, address, count, bank, map) | (EMPTY, count) | (OWN, list_bytes, stream_bytes)], the chain's rule flags, the maps
    [[(arcade palette, PS1 palette)]] (items refer to index + 1; 0 is no map) and the exceptions {(list index, cell index): palette}."""
    out = Built()
    seen = {}
    index = 0

    def put_stream(stream):
        if stream not in seen:
            seen[stream] = len(out.positions)
            out.positions += stream
            out.pos_known += b'\x01' * len(stream)
        return seen[stream]

    for item in items:
        if item[0] == OWN:
            out.list_at.append(len(out.lists))
            out.lists += bytes(item[1])
            out.lists_known += bytes(item[1])
            out.positions += bytes(item[2])
            out.pos_known += bytes(item[2])
            index += 1
        elif item[0] == EMPTY:
            for _ in range(item[1]):
                out.list_at.append(len(out.lists))
                data = struct.pack('<5HH', 1, 0, 0, 0, len(out.positions), 0)
                out.lists += data
                out.lists_known += b'\x01' * len(data)
                index += 1
        else:
            _k, address, count, bank, map_id = item
            palmap = dict(maps[map_id - 1]) if map_id else {}
            a = address
            for _ in range(count):
                rec = record_blocks(prog, a)
                if rec is None:
                    out.problems.append((index, f'no record at {a:#x}'))
                    return out
                cells = emitted_cells(rec[0], bank, rules & PAL_SPLIT)
                numbered = []
                for k, (code, _x, _y, flags) in enumerate(cells):
                    r = pool.lookup(code, (flags >> 5) & 3)
                    if r is None:
                        out.problems.append((index, f'tile {code:#x} is not in the segment'))
                        return out
                    pal = exceptions.get((index, k), palmap.get(flags & 0x1f, flags & 0x1f))
                    numbered.append((r[0], pal | (r[1] << 5)))
                data = fighters.encode_list(numbered, put_stream(position_stream_of(cells)))
                out.list_at.append(len(out.lists))
                out.lists += data
                out.lists_known += b'\x01' * len(data)
                index += 1
                a = rec[1]
    return out


def merge_maps(current, new):
    """The union of two palette maps (dicts) or None when they disagree about a palette."""
    for a, b in new.items():
        if current.get(a, b) != b:
            return None
    return {**current, **new}


# ---------------------------------------------------------------- the C helper (tile classes) and the screens table

HELPER = r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static void px(const unsigned char *q, unsigned char out[256]) {
    for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) {
        int b = 0; for (int pl = 0; pl < 4; pl++) b |= ((q[y * 8 + (x / 8) * 4 + pl] >> (7 - x % 8)) & 1) << pl;
        out[y * 16 + x] = (b + 1) & 15;
    }
}
static void fl(const unsigned char *a, int f, unsigned char *o) {
    for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) o[y * 16 + x] = a[((f & 2) ? 15 - y : y) * 16 + ((f & 1) ? 15 - x : x)];
}
static uint64_t canon(const unsigned char *q, unsigned char *best, int *d) {
    unsigned char a[256], t[256];
    px(q, a);
    fl(a, 0, best); *d = 0;
    for (int f = 1; f < 4; f++) { fl(a, f, t); if (memcmp(t, best, 256) < 0) { memcpy(best, t, 256); *d = f; } }
    uint64_t h = 14695981039346656037ull;
    for (int i = 0; i < 256; i++) h = (h ^ best[i]) * 1099511628211ull;
    return h;
}
typedef struct { uint64_t key; uint32_t code; } ent;
static int cmp(const void *x, const void *y) {
    const ent *a = x, *b = y;
    return a->key < b->key ? -1 : a->key > b->key ? 1 : a->code < b->code ? -1 : a->code > b->code;
}
/* key and d of every tile; returns the number of tiles that share a key without sharing canonical pixels (0 expected). */
long tile_keys(const unsigned char *gfx, long n, uint64_t *key, unsigned char *dd) {
    unsigned char b[256], c[256];
    int d, e;
    ent *v = malloc((size_t)n * sizeof *v);
    long bad = 0;
    if (!v) return -1;
    for (long t = 0; t < n; t++) { key[t] = canon(gfx + t * 128, b, &d); dd[t] = (unsigned char)d; v[t].key = key[t]; v[t].code = (uint32_t)t; }
    qsort(v, (size_t)n, sizeof *v, cmp);
    for (long i = 1; i < n; i++) if (v[i].key == v[i - 1].key) {
        canon(gfx + (long)v[i].code * 128, b, &d); canon(gfx + (long)v[i - 1].code * 128, c, &e);
        if (memcmp(b, c, 256)) bad++;
    }
    free(v);
    return bad;
}
'''


class Classes:
    """Tile classes of the arcade graphics (key and d of every tile), from the C helper."""

    def __init__(self, tmp, gfx):
        src, lib = Path(tmp) / 'classes.c', Path(tmp) / 'classes.so'
        src.write_text(HELPER)
        subprocess.run(['cc', '-O2', '-shared', '-fPIC', '-o', str(lib), str(src)], check=True, capture_output=True, timeout=60)
        so = ctypes.CDLL(str(lib))
        so.tile_keys.argtypes = [ctypes.c_char_p, ctypes.c_long, ctypes.c_void_p, ctypes.c_void_p]
        so.tile_keys.restype = ctypes.c_long
        self.count = len(gfx) // 128
        keys = (ctypes.c_uint64 * self.count)()
        dd = (ctypes.c_ubyte * self.count)()
        bad = so.tile_keys(gfx, self.count, keys, dd)
        if bad:
            raise RuntimeError(f'{bad} tiles share a class key without sharing pixels')
        self.keys, self.d = list(keys), bytes(dd)
        self.zero_key = pixel_class(bytes(256))[0]

    def cls(self, code):
        return (self.keys[code], self.d[code]) if 0 <= code < self.count else None


class ScreensTable:
    """screens_table.c read as text: the recipes of the disc files' chunks (the screens' tiles, palettes and pictures)."""

    def __init__(self, path=HERE / 'screens_table.c'):
        text = path.read_text()
        names = {'Z': Z, 'D': D, 'P': P, 'SEG': 1, 'RAW': 2, 'PALS': 3, 'PIC': 4}

        def array(name):
            m = re.search(r'\b' + name + r'\[\]\s*=\s*\{(.*?)\n\};', text, re.S)
            return re.findall(r'\{([^{}]*)\}', m.group(1))

        def row(s):
            return [names[t] if t in names else int(t, 0) for t in (x.strip() for x in s.split(',')) if t]
        self.items = [row(s) for s in array('arcade_screens_items')]
        self.regions = [row(s) for s in array('arcade_screens_regions')]
        self.recipes = [row(s) for s in array('arcade_screens_recipes')]
        self.uses = [row(s) for s in array('arcade_screens_uses')]
        self.files = {}
        for s in array('arcade_screens_files'):
            name, rest = s.split(',', 1)
            self.files[name.strip().strip('"')] = row(rest)

    def segments(self, file):
        """[(table, slot, ordinal, k, tiles)] of the file: the tile segments of its chunks with recipes, k the segment's rank in its recipe, tiles
        as Pool takes them. Empty when the file has no recipe."""
        if file not in self.files:
            return []
        first, count = self.files[file]
        out = []
        for table, slot, ordinal, recipe in self.uses[first:first + count]:
            _size, rfirst, rcount = self.recipes[recipe]
            k = 0
            for kind, _off, _size2, _width, ifirst, icount in self.regions[rfirst:rfirst + rcount]:
                if kind != 1:
                    continue
                tiles = []
                for code, n, flip in self.items[ifirst:ifirst + icount]:
                    if code == Z:
                        tiles += ['zero'] * n
                    elif code in (D, P):
                        tiles += [None] * n
                    else:
                        tiles += [(code + j, flip & 3) for j in range(n)]
                out.append((table, slot, ordinal, k, tiles))
                k += 1
        return out




# ---------------------------------------------------------------- the analysis of one file

class Env:
    """The arcade side: program, graphics, tile classes and the records indexed by the positions of their cells."""

    def __init__(self, prog, gfx, tmp):
        self.prog = prog
        self.classes = Classes(tmp, gfx)
        self.by_xy = collections.defaultdict(list)
        self.cache = {}
        index = fighters.Index(fighters.View(prog))
        for addrs in index.by_cells.values():
            for a in addrs:
                rec = self.record(a)
                if rec:
                    self.by_xy[tuple(sorted(rec[2]))].append(a)
        for v in self.by_xy.values():
            v.sort()

    def record(self, a):
        """(blocks, end, {(x, y): (code, flags)} of the record's cells) or None."""
        if a not in self.cache:
            rec = record_blocks(self.prog, a)
            self.cache[a] = None if rec is None else (rec[0], rec[1], {(c[1], c[2]): (c[0], c[3]) for c in emitted_cells(rec[0], 0, 0)})
        return self.cache[a]


def chains_of(mod):
    """[(offset, lists)] of every chain in the module chunk: MIN_CHAIN or more lists one after another from a list whose position offset is 0."""
    out, s = [], 0
    while s < len(mod) - 10:
        first = fighters.parse_list(mod, s)
        if first and first['pos'] == 0:
            lists = fighters.chain_from(mod, s)
            if len(lists) >= MIN_CHAIN:
                out.append((s, lists))
                s = lists[-1]['end']
                continue
        s += 2
    return out


def stream_ends(mod, base, lists):
    """End offset of each list's position stream (None for a list without one: a one-cell list of tile 0), or False when a stream does not fit."""
    out = []
    for lst in lists:
        if lst['n'] == 1 and lst['cells'] == [(0, None)]:
            out.append(None)
            continue
        e = fighters.stream_end(mod, base + lst['pos'], lst['n'])
        if e is None:
            return False
        out.append(e)
    return out


def position_base(mod, start, lists):
    """(offset of the chain's position runs, end of the last stream): the lowest offset below `start` from which every list's stream fits and the
    last ends at or before the chain (the runs lie right in front of it). None when there is none."""
    maxpos = max(l['pos'] for l in lists)
    for base in range(max(0, start - maxpos - 6 - 0x2000) & ~1, start - maxpos - 5, 2):
        ends = stream_ends(mod, base, lists)
        if ends is not False and max((e for e in ends if e), default=base) <= start:
            return base, max((e for e in ends if e), default=base)
    return None


def disc_cells(mod, base, lst):
    """[(number, flags, x, y)] of a disc list in its body order (the third word's palette for the plain modes), or None."""
    pos, c, lx, ly, xy = base + lst['pos'], 0, 0, 0, []
    while c < lst['n']:
        if pos + 6 > len(mod):
            return None
        dx, dy, r = struct.unpack_from('<hhH', mod, pos)
        x, y = lx + dx, ly + dy
        xy += [(x + 16 * i, y) for i in range(r + 1)]
        lx, ly = x + 16 * r, y
        c += r + 1
        pos += 6
    if c != lst['n']:
        return None
    return [(t & 0x3fff, (lst['third'] & 0x1f) if f is None else f, xy[k][0], xy[k][1]) for k, (t, f) in enumerate(lst['cells'])]


def match_record(env, pool, cells, prefer=None, prefer_bank=None):
    """(address, bank) of the arcade record whose cells, looked up in the pool, give exactly the disc list's numbers and flip bits (palettes
    aside), or None. A record that follows the previous list's (prefer) is tried first, then the lowest address."""
    cand = env.by_xy.get(tuple(sorted((x, y) for _n, _f, x, y in cells)), ())
    if prefer in cand:
        cand = [prefer] + [a for a in cand if a != prefer]
    banks = BANKS if prefer_bank is None else (prefer_bank,) + tuple(b for b in BANKS if b != prefer_bank)
    for a in cand:
        at = env.record(a)[2]
        for bank in banks:
            for num, f, x, y in cells:
                code, flags = at[(x, y)]
                r = pool.lookup(code + bank, (flags >> 5) & 3)
                if r is None or r[0] != num or r[1] != (f >> 5) & 3:
                    break
            else:
                return a, bank
    return None


def classify_lists(env, pool, mod, base, lists):
    """For each list (info): ('rec', (address, bank), {arcade palette: disc palette}, {(x, y): disc palette}), ('empty',) or ('own',)."""
    out, prev, prev_bank, plen = [], None, None, 0
    ends = stream_ends(mod, base, lists)
    for lst, e in zip(lists, ends):
        cells = None if e is None else disc_cells(mod, base, lst)
        if e is None and lst['third'] == 0 and lst['pos'] == plen:
            out.append(('empty',))
        elif cells and (m := match_record(env, pool, cells, prev, prev_bank)):
            at = env.record(m[0])[2]
            seen = collections.defaultdict(collections.Counter)
            for _n, f, x, y in cells:
                seen[at[(x, y)][1] & 0x1f][f & 0x1f] += 1
            out.append(('rec', m, {a: c.most_common(1)[0][0] for a, c in seen.items()}, {(x, y): f & 0x1f for _n, f, x, y in cells}))
            prev, prev_bank = env.record(m[0])[1], m[1]
        else:
            out.append(('own',))
        if e is not None:
            plen = max(plen, e - base)
    return out


def assemble(env, mod, base, lists, info, rules):
    """(items, maps, exceptions) for the lists' classification: consecutive records with one bank and compatible palette maps make one item, an
    own list is one item with its byte length and the position bytes it adds, the cells whose disc palette is not their item's map's are the exceptions."""
    pal_split = rules & PAL_SPLIT
    ends = stream_ends(mod, base, lists)
    items, maps, exceptions = [], [], {}
    run = None            # [address, count, bank, merged map dict, next address]
    plen = 0

    def close():
        nonlocal run
        if run:
            m = tuple(sorted((a, b) for a, b in run[3].items() if a != b))
            if m and m not in maps:
                maps.append(m)
            items.append((REC, run[0], run[1], run[2], maps.index(m) + 1 if m else 0))
            run = None
    runmap = []           # per list: the map dict of its item (filled when the item is closed): done below by two passes
    groups = []           # (first list index, last list index) of each REC item
    for i, (lst, inf) in enumerate(zip(lists, info)):
        if inf[0] == 'rec':
            (a, bank), pal = inf[1], inf[2]
            merged = merge_maps(run[3], pal) if run and run[4] == a and run[2] == bank else None
            if merged is not None:
                run[1] += 1
                run[3] = merged
                groups[-1][1] = i
            else:
                close()
                run = [a, 1, bank, dict(pal), None]
                groups.append([i, i])
            run[4] = env.record(a)[1]
        else:
            close()
            if inf[0] == 'empty':
                if items and items[-1][0] == EMPTY:
                    items[-1] = (EMPTY, items[-1][1] + 1)
                else:
                    items.append((EMPTY, 1))
            else:
                e = ends[i]
                new = 0 if e is None else max(0, e - base - plen)
                items.append((OWN, lst['end'] - lst['at'], new))
        if ends[i] is not None:
            plen = max(plen, ends[i] - base)
    close()
    # the exceptions: the cells of a record's list whose disc palette is not what the item's map makes of the arcade's
    k = 0
    gi = 0
    item_maps = [dict(maps[it[4] - 1]) if it[4] else {} for it in items if it[0] == REC]
    for g, (first, last) in enumerate(groups):
        for i in range(first, last + 1):
            (a, bank), disc = info[i][1], info[i][3]
            palmap = item_maps[g]
            for ci, (_code, x, y, flags) in enumerate(emitted_cells(env.record(a)[0], bank, pal_split)):
                want = palmap.get(flags & 0x1f, flags & 0x1f)
                if disc[(x, y)] != want:
                    exceptions[(i, ci)] = disc[(x, y)]
    return items, maps, exceptions


def analyse_chain(env, pool, mod, start, lists, base, rules):
    """Items, maps, exceptions and the kind of every list for one setting of the rules; lists the model does not reproduce become own lists
    (and the model runs again) until none differs. Returns a dict."""
    info = classify_lists(env, pool, mod, base, lists)
    first = sum(1 for inf in info if inf[0] != 'own')
    for _round in range(12):
        items, maps, exceptions = assemble(env, mod, base, lists, info, rules)
        built = model_chain(env.prog, pool, items, rules, maps, exceptions)
        if built.problems:
            bad = {i for i, _why in built.problems}
        else:
            bad = {i for i, (lst, inf) in enumerate(zip(lists, info))
                   if inf[0] != 'own' and bytes(built.lists[built.list_at[i]:built.list_at[i] + lst['end'] - lst['at']]) != mod[lst['at']:lst['end']]}
        if not bad:
            break
        for i in bad:
            info[i] = ('own',)
    equal = sum(1 for inf in info if inf[0] != 'own')
    return dict(items=items, maps=maps, exceptions=exceptions, info=info, built=built, equal=equal, rules=rules, demoted=first - equal)


def own_tiles_arcade(pool, lst):
    """True when every tile number of the list is an arcade tile (or blank) of the pool: such a list is text or a picture laid out from the
    arcade's tiles with no art record behind it."""
    return all((t & 0x3fff) < len(pool.tiles) and pool.tiles[t & 0x3fff] is not None for t, _f in lst['cells'])


def choose_pool(env, pools, mod, base, lists):
    """The pool (index) whose lookups reproduce the numbers of the most lists, among the pools big enough for the chain's tile numbers."""
    top = max(max(t & 0x3fff for t, _f in lst['cells']) for lst in lists)
    best = None
    for pi, pool in enumerate(pools):
        if len(pool[4]) <= top:
            continue
        p = pool[5]
        sample = [lst for lst in lists[:80] if lst['n'] > 1 or lst['cells'] != [(0, None)]]
        hits = sum(1 for lst in sample if (c := disc_cells(mod, base, lst)) and match_record(env, p, c))
        if hits and (best is None or hits > best[0]):
            best = (hits, pi)
    return None if best is None else best[1]


def analyse_file(env, table, name, data):
    """The chains of one disc file: a list of dicts (see chain rows), or an error string. pools: the file's tile segments from the screens table."""
    kind = NAME_RE.match(name).group(1)
    slot = dict(KINDS)[kind]
    chunks = pac.chunk_table(data)
    ordinal, seen = {}, collections.Counter()
    for c in chunks:
        key = (c['table'], c['slot'])
        ordinal[id(c)] = seen[key]
        seen[key] += 1
    mods = [c for c in chunks if c['table'] == 0 and c['slot'] == slot]
    if len(mods) != 1:
        return f'{len(mods)} module chunks'
    mod = data[mods[0]['offset']:mods[0]['offset'] + mods[0]['size']]
    pools = [(t, s, o, k, tiles, Pool(tiles, env.classes.cls, env.classes.zero_key)) for t, s, o, k, tiles in table.segments(name)]
    rows = []
    for start, lists in chains_of(mod):
        pb = position_base(mod, start, lists)
        row = dict(start=start, nlists=len(lists), base=None, mod=mod)
        if pb is None:
            row['why'] = 'positions not found'
            rows.append(row)
            continue
        base, pend = pb
        pi = choose_pool(env, pools, mod, base, lists)
        if pi is None:
            row.update(base=base, why='no pool')
            rows.append(dict(row, own_bytes=sum(l['end'] - l['at'] for l in lists)))
            continue
        pool = pools[pi][5]
        best = None
        for rules in (0, PAL_SPLIT):
            r = analyse_chain(env, pool, mod, start, lists, base, rules)
            if best is None or r['equal'] > best['equal']:
                best = r
        built = best['built']
        row.update(best, base=base, pos_size=pend - base, lists_size=lists[-1]['end'] - start, pool=pools[pi][:4], module_slot=slot,
                   own_bytes=sum(l['end'] - l['at'] for l, inf in zip(lists, best['info']) if inf[0] == 'own'))
        row['own_arcade'] = sum(1 for l, inf in zip(lists, best['info']) if inf[0] == 'own' and own_tiles_arcade(pool, l))
        row['sizes_ok'] = len(built.positions) == pend - base and len(built.lists) == lists[-1]['end'] - start
        rows.append(row)
    return rows


def descriptor_key(row):
    return (row['rules'], row['pos_size'], row['lists_size'], tuple(row['items']), tuple(sorted(row['exceptions'].items())),
            tuple(tuple(row['maps'][m - 1]) if m else () for it in row['items'] if it[0] == REC for m in (it[4],)))


# ---------------------------------------------------------------- the report and the C table

def line(name, rows):
    if isinstance(rows, str):
        return f'{name}: {rows}'
    nl = rec = emp = own = 0
    kinds = collections.Counter()
    for r in rows:
        nl += r['nlists']
        if 'info' not in r:
            own += r['nlists']
            kinds[r['why']] += 1
            continue
        for inf in r['info']:
            if inf[0] == 'rec':
                rec += 1
            elif inf[0] == 'empty':
                emp += 1
            else:
                own += 1
    built = [r for r in rows if 'info' in r]
    own_arc = sum(r['own_arcade'] for r in built)
    pos = sum(r['pos_size'] for r in built)
    pos_known = sum(sum(r['built'].pos_known) for r in built)
    exc = sum(len(r['exceptions']) for r in built)
    maps = sum(1 for r in built for it in r['items'] if it[0] == REC and it[4])
    bad = sum(1 for r in built if not r['sizes_ok'])
    demoted = sum(r['demoted'] for r in built)
    s = (f'{name}: {len(rows)} chains ({len(built)} with records), {nl} lists: {rec} from records, {emp} empty, {own} own ({own_arc} of them in chains with a tile segment and only arcade tiles); positions {pos_known} of {pos} B built; '
         f'{maps} record runs with a palette map, {exc} palette exceptions')
    if kinds:
        s += '; chains left out: ' + ', '.join(f'{v} {k}' for k, v in sorted(kinds.items()))
    if demoted:
        s += f'; {demoted} lists matched a record but not byte for byte (made own)'
    if bad:
        s += f'; {bad} chains with sizes that differ from the disc'
    return s


def c_table(files):
    """The text of screen_lists_table.c for {file name: rows}."""
    items, item_at = [], {}
    excs, pals, pal_at, maps_c, chains, chain_at, uses = [], [], {}, [], [], {}, []
    for name, rows in sorted(files.items()):
        if isinstance(rows, str):
            continue
        for r in rows:
            if 'info' not in r or not any(it[0] != OWN for it in r['items']):
                continue
            mp = []
            for it in r['items']:
                if it[0] == REC and it[4]:
                    m = tuple(r['maps'][it[4] - 1])
                    if m not in pal_at:
                        pal_at[m] = len(maps_c)
                        maps_c.append((len(pals), len(m)))
                        pals += list(m)
                    mp.append(pal_at[m] + 1)
                elif it[0] == REC:
                    mp.append(0)
            it_c = []
            k = 0
            for it in r['items']:
                if it[0] == REC:
                    it_c.append((REC, it[2], it[1], it[3], mp[k], 0, 0))
                    k += 1
                elif it[0] == EMPTY:
                    it_c.append((EMPTY, it[1], 0, 0, 0, 0, 0))
                else:
                    it_c.append((OWN, 1, 0, 0, 0, it[1], it[2]))
            exc_c = sorted((i, c, p) for (i, c), p in r['exceptions'].items())
            key = (r['rules'], r['pos_size'], r['lists_size'], tuple(it_c), tuple(exc_c))
            if key not in chain_at:
                chain_at[key] = len(chains)
                chains.append((r['rules'], r['pos_size'], r['lists_size'], len(items), len(it_c), len(excs), len(exc_c)))
                items += it_c
                excs += exc_c
            t, s, o, kk = r['pool']
            uses.append((name, r['module_slot'], r['base'], r['start'], chain_at[key], t, s, o, kk))
    out = ['/* GENERATED by `python3 port/arcade/screen_lists.py --disc DISC --arcade SET --c-table`; do not edit.',
           ' * The cell lists and position runs of the screen modules (select, character demo, continue, ending, attract) that the arcade\'s art records',
           ' * give: per chain of lists its sizes, rule flags, items (runs of records, empty lists, own lists that only keep their size), the palette maps',
           ' * and the cells that keep another palette; per file where each chain lies in its module and which tile segment of the screens table it',
           ' * numbers into. Only addresses, counts and offsets: no byte of either game. */',
           '#include "screen_lists.h"', '']
    out.append('/* {kind, count, address, bank, map (index + 1, 0: none), list bytes, stream bytes} */')
    out.append('const struct arcade_screen_lists_item arcade_screen_lists_items[] = {')
    for kind, count, a, bank, m, lb, sb in items:
        out.append(f'    {{{kind}, {count}u, {a:#x}u, {bank:#x}u, {m}u, {lb}u, {sb}u}},')
    out += ['};', '', '/* {list, cell, palette} */', 'const struct arcade_screen_lists_exception arcade_screen_lists_exceptions[] = {']
    out += [f'    {{{i}u, {c}u, {p}u}},' for i, c, p in excs] or ['    {0u, 0u, 0u},']
    out += ['};', '', '/* {from, to} */', 'const struct arcade_screen_lists_pal arcade_screen_lists_pals[] = {']
    out += [f'    {{{a}u, {b}u}},' for a, b in pals] or ['    {0u, 0u},']
    out += ['};', '', '/* {first pair, pairs} */', 'const struct arcade_screen_lists_map arcade_screen_lists_maps[] = {']
    out += [f'    {{{f}u, {n}u}},' for f, n in maps_c] or ['    {0u, 0u},']
    out += ['};', '', '/* {rules, position bytes, list bytes, first item, items, first exception, exceptions} */',
            'const struct arcade_screen_lists_chain arcade_screen_lists_chains[] = {']
    for rules, ps, ls, fi, ni, fe, ne in chains:
        rn = ' | '.join('ARCADE_SCREEN_LISTS_' + n for b, n in RULE_NAMES if rules & b) or '0u'
        out.append(f'    {{{rn}, {ps}u, {ls}u, {fi}u, {ni}u, {fe}u, {ne}u}},')
    out += ['};', '', '/* {file, module slot, positions offset, lists offset, chain, pool table, slot, ordinal, segment} */',
            'const struct arcade_screen_lists_use arcade_screen_lists_uses[] = {']
    for name, ms, pb, ls, ch, t, s, o, k in uses:
        out.append(f'    {{"{name}", {ms}u, {pb:#x}u, {ls:#x}u, {ch}u, {t}u, {s:#x}u, {o}u, {k}u}},')
    out += ['};', f'const unsigned arcade_screen_lists_item_count = {len(items)}u;',
            f'const unsigned arcade_screen_lists_map_count = {len(maps_c)}u;',
            f'const unsigned arcade_screen_lists_chain_count = {len(chains)}u;',
            f'const unsigned arcade_screen_lists_use_count = {len(uses)}u;', '']
    return '\n'.join(out)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='files whose name starts with this (SELECTA, CDEMO0F, ...)')
    ap.add_argument('--detail', action='store_true', help='one line per chain')
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/screen_lists_table.c')
    ap.add_argument('--compare-c', action='store_true', help='build screen_lists.c with the checked-in table and compare it with the model and the disc')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='screen-lists-')
    try:
        prog, gfx = fighters.arcade_views(args.arcade)
        env = Env(prog, gfx, tmp)
        table = ScreensTable()
        files = {}
        with open(args.disc, 'rb') as f:
            index, sec = fighters.iso_files(f)
            names = sorted(p for p in index if p.startswith('/PAC/') and NAME_RE.match(p.split('/')[-1]))
            for p in names:
                name = p.split('/')[-1]
                if args.only and not name.startswith(args.only.upper()):
                    continue
                lba, size = index[p]
                data = sec(lba, (size + 2047) // 2048)[:size]
                try:
                    files[name] = analyse_file(env, table, name, data)
                except (pac.FormatError, struct.error) as exc:
                    files[name] = str(exc)
                if not args.c_table and not args.compare_c:
                    print(line(name, files[name]), flush=True)
                    if args.detail and not isinstance(files[name], str):
                        for r in files[name]:
                            print(f'    chain {r["start"]:#x}: {r["nlists"]} lists; ' + (r['why'] if 'info' not in r else
                                  f'pool {r["pool"][0]}/{r["pool"][1]:#x}/{r["pool"][2]} segment {r["pool"][3]}, positions {r["base"]:#x}, '
                                  f'{r["equal"]} lists built, rules {r["rules"]}, {len(r["maps"])} maps, {len(r["exceptions"])} exceptions'))
        if args.c_table:
            sys.stdout.write(c_table(files))
        elif args.compare_c:
            compare_c(files, prog, gfx, tmp)
        print(f'{len(files)} files in {time.monotonic() - started:.1f} s', file=sys.stderr if args.c_table else sys.stdout)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


class CLib:
    """screen_lists.c with the checked-in table (and the screens table it reads), compiled into a temporary folder and called through ctypes."""

    class Use(ctypes.Structure):
        _fields_ = [('file', ctypes.c_char_p), ('module_slot', ctypes.c_uint16), ('positions', ctypes.c_uint32), ('lists', ctypes.c_uint32),
                    ('chain', ctypes.c_uint16), ('pool_table', ctypes.c_uint16), ('pool_slot', ctypes.c_uint16), ('pool_ordinal', ctypes.c_uint16),
                    ('pool_segment', ctypes.c_uint16)]

    class Out(ctypes.Structure):
        _fields_ = [('positions', ctypes.c_void_p), ('positions_known', ctypes.c_void_p), ('positions_size', ctypes.c_uint32),
                    ('lists', ctypes.c_void_p), ('lists_known', ctypes.c_void_p), ('lists_size', ctypes.c_uint32)]

    def __init__(self, tmp):
        lib = Path(tmp) / 'screen_lists.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-shared', '-fPIC', '-o', str(lib)] + [str(HERE / n) for n in
                       ('screen_lists.c', 'screen_lists_table.c', 'screens.c', 'screens_table.c', 'tilepack.c')], check=True, capture_output=True, timeout=120)
        self.lib = ctypes.CDLL(str(lib))
        count = ctypes.c_uint.in_dll(self.lib, 'arcade_screen_lists_use_count').value
        self.uses = (self.Use * count).in_dll(self.lib, 'arcade_screen_lists_uses')
        self.lib.arcade_screen_lists_build.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(self.Use),
                                                       ctypes.POINTER(self.Out), ctypes.c_char_p, ctypes.c_size_t]
        self.lib.arcade_screen_lists_free.argtypes = [ctypes.POINTER(self.Out)]

    def build(self, prog, gfx, use):
        """(positions, positions_known, lists, lists_known, seconds) or an error string."""
        out, err = self.Out(), ctypes.create_string_buffer(256)
        t0 = time.perf_counter()
        rc = self.lib.arcade_screen_lists_build(prog, len(prog), gfx, len(gfx), ctypes.byref(use), ctypes.byref(out), err, len(err))
        took = time.perf_counter() - t0
        if rc:
            return err.value.decode()
        try:
            return (ctypes.string_at(out.positions, out.positions_size), ctypes.string_at(out.positions_known, out.positions_size),
                    ctypes.string_at(out.lists, out.lists_size), ctypes.string_at(out.lists_known, out.lists_size), took)
        finally:
            self.lib.arcade_screen_lists_free(ctypes.byref(out))


def compare_c(files, prog, gfx, tmp):
    """One line per file: the checked-in table's chains built by the C against the model and against the disc (on the bytes the C says it built)."""
    here = HERE / 'screen_lists_table.c'
    print('screen_lists_table.c ' + ('is the generated one' if here.read_text() == c_table(files) else 'DIFFERS from the generated one'))
    clib = CLib(tmp)
    per = collections.OrderedDict()
    total = 0.0
    for use in clib.uses:
        name = use.file.decode()
        rows = files.get(name)
        row = next((r for r in rows if not isinstance(rows, str) and r['start'] == use.lists and 'info' in r), None) if rows else None
        st = per.setdefault(name, collections.Counter())
        if row is None:
            st['no analysis row'] += 1
            continue
        built = clib.build(prog, gfx, use)
        if isinstance(built, str):
            st['refused'] += 1
            print(f'{name}: chain at {use.lists:#x}: C refused: {built}')
            continue
        pos, pk, lst, lk, took = built
        total += took
        m = row['built']
        st['chains'] += 1
        st['same as model'] += int((pos, bytes(m.pos_known), lst, bytes(m.lists_known)) == (bytes(m.positions), bytes(m.pos_known), bytes(m.lists), bytes(m.lists_known)) or
                                   (pk == bytes(m.pos_known) and lk == bytes(m.lists_known) and
                                    all(a == b for a, b, k in zip(pos, m.positions, pk) if k) and all(a == b for a, b, k in zip(lst, m.lists, lk) if k)))
        mod = row['mod']
        dpos = mod[use.positions:use.positions + len(pos)]
        dlst = mod[use.lists:use.lists + len(lst)]
        eq = sum(1 for a, b, k in zip(pos, dpos, pk) if k and a == b) + sum(1 for a, b, k in zip(lst, dlst, lk) if k and a == b)
        known = sum(pk) + sum(lk)
        st['known bytes'] += known
        st['bytes equal to the disc'] += eq
        st['bytes'] += len(pos) + len(lst)
        st['ms'] += took * 1000
    tot = collections.Counter()
    for name, st in per.items():
        print(f'{name}: {st["chains"]} chains, C == model on {st["same as model"]}; built {st["known bytes"]} of {st["bytes"]} B, '
              f'{st["bytes equal to the disc"]} equal to the disc' + (f', {st["no analysis row"]} chains without an analysis row' if st['no analysis row'] else '') +
              f'; {st["ms"]:.1f} ms')
        tot.update(st)
    print(f'C total: {tot["chains"]} chains, C == model on {tot["same as model"]}, {tot["known bytes"]} B built of {tot["bytes"]}, '
          f'{tot["bytes equal to the disc"]} equal to the disc; {total * 1000:.1f} ms')


if __name__ == '__main__':
    sys.exit(main())
