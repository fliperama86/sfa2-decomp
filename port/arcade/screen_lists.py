#!/usr/bin/env python3
"""Analysis tool and specification: the cell lists and position runs of the screen modules, rebuilt from the arcade's art records and scroll blocks,
and the select screen's backdrop cell map, rebuilt from the arcade's scroll 1 sets.

What it does. The select (SELECTA), character-demo (CDEMO), continue (CONT), ending (END) and attract (DEMO) files of the PS1 disc each hold, in
their module chunk, blocks of cell lists with the position runs in front of them (the format of the fighters' modules, read by
unpack_cell_list_into_stream_cells and build_sprite_quads_from_stream). A block is a "chain": the position runs, then lists one after another, the
first list's position offset 0. A module has one chain for the art of one tile segment, so a file has many (CDEMO 33, END 18, CONT 43). The tool
finds every chain, the tile segment it numbers into, the arcade source behind each list, and prints one line per file with how many lists it
rebuilds byte for byte. The fighters' rules (fighters.py) carry over with these differences, all found from the data:

  - Numbers. A list's tile numbers are the indexes of tiles in the tile segment the chain belongs to (any segment of the file's chunks; the
    shared library of DEMO, SELECT and CONT has one segment per chain). The PS1 numbers a tile by first use, one number a pixel class (the arcade
    tile, or the same pixels flipped), so the number of a cell is found by looking its tile up in the segment: the lowest number (from 1) whose
    tile is in the cell's class. The segment's tiles are the screens recipe's (screens_table.c: item k of the segment is number k, an arcade
    tile with a stored flip, a blank tile, or not the arcade's). The class of a tile is its pixels or those flipped across and down, whichever bytes
    sort lowest ("canonical"); d is the lowest flip that turns the tile into the canonical one. A tile stored with flip s has stored flip
    e = s xor d(tile); a cell of code c and flip bits u (flags 0x20 across, 0x40 down) gets number n and flip bits g = u xor d(c) xor e(n).
    Exact for every matched list of the five kinds (fighters.py's counter rule needed a first-use order that the segment now gives).
  - Parts. A record's blocks are in parts; the PS1 lists the parts last first, each part by (y, x). A block starts a new part when its x differs
    from the previous block's x by something that is not a multiple of 16 (the two blocks cannot form a run of adjacent tiles), and, in the
    chains with the rule PAL_SPLIT (SELECTA's), for the records of mode 0 and 2 also when its palette differs.
  - Sources. A list is rebuilt from an arcade art record of mode 0 or 2 (the fighters'), of mode 4 (the select screen's portraits: blocks of code,
    attribute, x, y inline; tile = ((code + 0xa400) & 0xffff) + 0x10000; the PS1 leaves out the blank cells and the shadow cells, x from -32 and
    y from 64) or of mode 10 (a grid of 1 x 1 blocks, header mode, height - 1, width - 1, then code and attribute pairs column by column, the
    lower left corner at 0, 0; the same tile rule; blank cells left out), or from a WINDOW of the arcade's scroll 2 layer: blocks of 16 x 16 cells of
    code and attribute (column by column, 1,024 bytes from 0x2c0300; the blocks of the stages' layers 1 start at 0x2d1f00 of the same run), the
    list being the non-blank cells of a rectangle of the map across up to four blocks, drawn at an origin of the PS1's choice.
  - Exceptions. A cell the PS1 left out of a list (DROP), a cell that shows a tile of the PS1's own, which the PS1 made from the arcade's tile by
    cleaning it (OWN_TILE: its number in the segment, flip bits and palette are the exception's; a number 0 means the next of the segment's own
    tiles after the previous such cell), and a cell that keeps another palette than its map gives, are exceptions by list and cell. They are PS1
    choices and have no source.
  - Palettes. The list's palette numbers are the PS1's, not the arcade's: the same cell has another number where the screen loads its colours
    into other rows of the PS1's palette memory. Per run of records one map (arcade number to PS1 number; numbers not in it stay) gives them
    (CDEMO: 20, 21, 28, 29 all become 0; SELECTA: 28 becomes 0 in the name plates, 29 becomes 30 in the second player's copies; END: 6 becomes 23
    or 0).
  - Own lists. Lists with no arcade source (text lines, pictures the PS1 composed from several frames or cut from scroll data, poses of the PS1's
    own) are "own": the table keeps their byte length and the bytes they add to the position runs, the loader gets their bytes from the disc. So do
    the lists with n = 1 and tile 0 whose palette or position offset is not the "empty list" form. An empty list (one cell, tile 0, palette 0,
    position offset the size of the runs so far) is built.
  - Position runs: identical streams stored once, in order of first use (as fighters.py); streams of own lists count only by length.

For each chain the tool needs the position runs' offset (found: the distance to the chain where every list's stream fits and the last one ends at
the chain), the tile segment (found: the one whose lookups reproduce the most lists' numbers, else the one with most arcade tiles), and for each
list the source: the records with the same cell positions (sorted) are tried, in the tile banks 0 to 3 times 0x10000, then (lists of 20 cells or more)
the portraits that have all of the list's positions and at most MAX_DROPS more, then a window of the scroll 2 blocks (anchored at the list's rarest
tile, the lowest blocks that fit, the cells of the rectangle that the list lacks being blank); the one whose lookups give the list's numbers and flips
(the cells whose number is one of the segment's own tiles being exceptions) wins (the record that follows the previous list's, if several do). The
palette map and the exceptions are what is left. With the part rule chosen per chain by the number of lists that then come out equal, lists that
still differ become own lists, until none differs.

The backdrop cell map (BACKDROPS, model_backdrop). SELECTA's chunk 0x26 is 10,240 cells (160 rows of 64) of a texture word and a palette word: 8 x 8
cells of the arcade's scroll 1 (stages.py's block sets), 20 bands of 8 rows (rows 3 to 10 of the sets), the columns of band k from the set 64 + k
(25 columns from x 7), the set 84 (32 columns) and the set 0 (7 columns). A cell's 8 x 8 piece (the left half of a graphics tile's top or bottom rows,
flipped by the attribute) is looked up among the 8 x 8 units of the backdrop's picture (chunk 0x25, which screens.py builds from the arcade: 3 pages
of 32 x 32 units) for the texture word; the palette word is 0x4024 + 64 * the cell's palette (0x4024 for a blank piece).

Options. With --c-table it prints port/arcade/screen_lists_table.c (per chain: rooms, rule flags, items, windows, maps, exceptions; per file: where
each chain lies in the module and which segment it numbers into; the backdrops; no game byte). With --compare-c it builds screen_lists.c with the
checked-in table and prints one line per file (the C against the model and the C against the disc on the bytes the C claims) and one for the backdrop.

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
REC, EMPTY, OWN, WIN = 0, 1, 2, 3     # item kinds
DROP = 0x100                  # an exception's value that removes the cell instead (a cell the PS1 left out of its list)
OWN_TILE = 0x200              # an exception's flag: the cell shows a tile of the PS1's own (the arcade's, cleaned): bits 0-4 the palette, bits 10-11 the
                              # flip bits, bits 16 and up the tile's number in the segment (0: the next of the PS1's own numbers after the
                              # previous such cell of the list)
MAX_DROPS = 8                 # cells of an arcade record the PS1 left out of a list, at most (each is an exception)
SHADOW_X, SHADOW_Y = -32, 64  # mode 4 (the select portraits): the cells from here on right and down are the PS1's shadow cells, not in the lists
SCROLL_FIRST = 0x2c0300       # the arcade's scroll 2 layer blocks: 16 x 16 cells of code and attribute, column by column, 1,024 bytes each
SCROLL_BLOCK, SCROLL_BLOCKS = 1024, 222
MIN_CHAIN = 2                 # lists in a row that make a chain
MAX_CELLS = 512
SCROLL_ADD = 0xa400           # modes 4 and 10: the tile code is the scroll 2 layer's, plus this (16 bits), in the bank 0x10000
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
    """(blocks [(code, attr, x, y)], end address) of the art record of mode 2, 0, 4 or 10 at address a of the 68000's view `data`, or None.

    Mode 2: blocks of code and attribute (width - 1 in bits 8-11, height - 1 in 12-15, palette 0-4, flips 5, 6). Mode 0: a shared attribute
    word and one code a cell, each a 1 x 1 block. The coordinates are at the pointer found through the record's index. Mode 4 (the select
    screen's portraits): blocks of code, attribute, x, y, the tile code being the scroll 2 layer's (plus SCROLL_ADD). Mode 10 (a grid of
    1 x 1 blocks of code and attribute: header mode, height - 1, width - 1, the blocks column by column; the grid's lower left corner is
    at x 0, y 0, so block (i, j) is at (16 i, 16 j - 16 h))."""
    view = fighters.View(data)
    n = len(data)
    if a < 0 or a & 1 or a + 8 > n:
        return None
    mode = view.u16(a)
    if mode == 10:
        h, w = view.u16(a + 2) + 1, view.u16(a + 4) + 1
        if w > 64 or h > 64 or w * h > MAX_CELLS or a + 6 + 4 * w * h > n:
            return None
        blocks = [((view.u16(a + 6 + 4 * (i * h + j)) + SCROLL_ADD) & 0xffff, view.u16(a + 8 + 4 * (i * h + j)), 16 * i, 16 * j - 16 * h)
                  for i in range(w) for j in range(h)]
        if any(at & 0xff80 for _c, at, _x, _y in blocks):
            return None
        return blocks, a + 6 + 4 * w * h
    if mode == 4:
        cells, nb = view.u16(a + 2), view.u16(a + 4) + 1
        if not 1 <= cells <= MAX_CELLS or a + 6 + 8 * nb > n:
            return None
        blocks = [((view.u16(a + 6 + 8 * k) + SCROLL_ADD) & 0xffff, view.u16(a + 8 + 8 * k), view.s16(a + 10 + 8 * k), view.s16(a + 12 + 8 * k))
                  for k in range(nb)]
        total = sum((((at >> 8) & 15) + 1) * (((at >> 12) & 15) + 1) for _c, at, _x, _y in blocks)
        return (blocks, a + 6 + 8 * nb) if total == cells else None
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


def emitted_cells(blocks, bank, pal_split, drop=None):
    """The record's cells (code + bank, x, y, flags) in the PS1's order: the parts last first, each part by (y, x), equal positions in record order.
    drop(code, x, y) true (code without the bank) leaves the cell out."""
    parts = block_parts(blocks, pal_split)
    cells = []
    for b, (code, attr, x, y) in enumerate(blocks):
        w, h = ((attr >> 8) & 15) + 1, ((attr >> 12) & 15) + 1
        for j in range(h):
            for i in range(w):
                ci = w - 1 - i if attr & 0x20 else i
                cj = h - 1 - j if attr & 0x40 else j
                if drop is None or not drop(code + ci + 16 * cj, x + 16 * i, y + 16 * j):
                    cells.append((code + ci + 16 * cj + bank, x + 16 * i, y + 16 * j, attr & 0x7f, parts[b]))
    return [c[:4] for c in sorted(cells, key=lambda c: (-c[4], c[2], c[1]))]


def record_drop(data, a, bank, blank):
    """The cells the PS1 leaves out of the lists of the record at a: None for the records of mode 0 and 2; for the grids (mode 10) the blank
    tiles; for the portraits (mode 4) the blank tiles and the shadow cells (x from SHADOW_X, y from SHADOW_Y). blank(tile) says whether a tile
    (with its bank) has no opaque pixel."""
    mode = data[a] << 8 | data[a + 1] if a + 2 <= len(data) else -1
    if mode == 10:
        return lambda c, x, y: blank(c + bank)
    if mode == 4:
        return lambda c, x, y: blank(c + bank) or (x >= SHADOW_X and y >= SHADOW_Y)
    return None


def record_cells(data, a, bank, pal_split, blank):
    """(cells in the PS1's order, end address) of the art record at a, or None when there is no record. The palette part rule is for the records
    of mode 0 and 2."""
    rec = record_blocks(data, a)
    if rec is None:
        return None
    return emitted_cells(rec[0], bank, pal_split and data[a + 1] in (0, 2), record_drop(data, a, bank, blank)), rec[1]


def window_cells(data, win, blank):
    """The cells (tile, x, y, flags) of a window of the scroll 2 blocks in the PS1's order (rows, left to right), the blank tiles left out.
    win is (x0, y0, c0, r0, w, h, blocks {(rx, ry): block}): the cell (i, j) of the window is cell (c0 + i, r0 + j) of the map whose blocks
    are named by region (rx, ry) = ((c0 + i) // 16, (r0 + j) // 16); None when a block is missing or lies outside the program."""
    x0, y0, c0, r0, w, h, blocks = win
    blocks = dict(blocks)
    out = []
    for j in range(h):
        for i in range(w):
            mc, mr = c0 + i, r0 + j
            blk = blocks.get((mc // 16, mr // 16))
            if blk is None or not 0 <= blk < SCROLL_BLOCKS:
                return None
            at = SCROLL_FIRST + SCROLL_BLOCK * blk + 64 * (mc % 16) + 4 * (mr % 16)
            code, attr = struct.unpack_from('>HH', data, at)
            tile = ((code + SCROLL_ADD) & 0xffff) + 0x10000
            if not blank(tile):
                out.append((tile, x0 + 16 * i, y0 + 16 * j, attr & 0x7f))
    return out


class Pool:
    """The numbers of a tile segment: tiles [(arcade code, stored flip) or 'zero' or None], number k being tiles[k]. lookup() finds a cell's number.

    cls(code) gives (key, d) of an arcade tile; zero_key is the key of the blank tile (value 0 everywhere); a tile of None (the PS1's own) has no class."""

    def __init__(self, tiles, cls, zero_key):
        self.zero_key = zero_key
        self.of = {}
        self.tiles = tiles
        self.kind2 = [k for k, t in enumerate(tiles) if k and t is None]     # the numbers of the PS1's own tiles, in order
        self.kind2_set = set(self.kind2)
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

    def is_blank(self, code):
        """True when arcade tile `code` (with its bank) has no opaque pixel."""
        c = self.cls(code)
        return c is not None and c[0] == self.zero_key

    def successor(self, n):
        """The next of the PS1's own numbers after n (a number that is one of them), or 0."""
        if n not in self.kind2_set:
            return 0
        j = self.kind2.index(n) + 1
        return self.kind2[j] if j < len(self.kind2) else 0

    def number_cells(self, cells, own=None):
        """[(number, flip bits g)] of cells [(tile, x, y, flags)], or a string saying why not. The cells at the indexes in `own` {index: (number,
        flip bits)} show tiles of the PS1's own; every other cell is looked up in the segment."""
        out = []
        for k, (tile, _x, _y, flags) in enumerate(cells):
            if own and k in own:
                out.append(own[k])
                continue
            r = self.lookup(tile, (flags >> 5) & 3)
            if r is None:
                return f'tile {tile:#x} is not in the segment'
            out.append(r)
        return out


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


def model_chain(prog, pool, items, rules, maps, exceptions, windows=()):
    """The specification of screen_lists.c: build a chain from the arcade program `prog` (68000 view), the segment `pool`, and
    items [(REC, address, count, bank, map) | (WIN, window, 1, 0x10000, map) | (EMPTY, count) | (OWN, list_bytes, stream_bytes)], the chain's rule flags, the maps [[(arcade palette, PS1 palette)]] (items refer to index + 1; 0 is no map), the
    exceptions {(list index, cell index): palette or DROP} and the windows [(x0, y0, c0, r0, w, h, {(rx, ry): block})]."""
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
            kind, address, count, bank, map_id = item[:5]
            palmap = dict(maps[map_id - 1]) if map_id else {}
            a = address
            for _ in range(count):
                if kind == WIN:
                    cells = window_cells(prog, windows[address], lambda c: pool.is_blank(c)) if address < len(windows) else None
                    end = None
                else:
                    r = record_cells(prog, a, bank, rules & PAL_SPLIT, pool.is_blank)
                    cells, end = r if r else (None, None)
                if cells is None:
                    out.problems.append((index, f'no record or window at {a:#x}'))
                    return out
                keep = [k for k in range(len(cells)) if exceptions.get((index, k)) != DROP]
                kept = [cells[k] for k in keep]
                own, prev = {}, 0
                for i, k in enumerate(keep):
                    ex = exceptions.get((index, k), 0)
                    if ex & OWN_TILE:
                        num = ex >> 16 or pool.successor(prev)
                        if not num:
                            out.problems.append((index, 'no number follows the previous tile of the PS1\'s own'))
                            return out
                        own[i], prev = (num, (ex >> 10) & 3), num
                nums = pool.number_cells(kept, own)
                if isinstance(nums, str):
                    out.problems.append((index, nums))
                    return out
                numbered = []
                for k, cell, (num, g) in zip(keep, kept, nums):
                    pal = exceptions.get((index, k), palmap.get(cell[3] & 0x1f, cell[3] & 0x1f)) & 0x1f
                    numbered.append((num, pal | (g << 5)))
                data = fighters.encode_list(numbered, put_stream(position_stream_of(kept)))
                out.list_at.append(len(out.lists))
                out.lists += data
                out.lists_known += b'\x01' * len(data)
                index += 1
                a = end
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
        names = {'Z': Z, 'D': D, 'P': P, 'W': 0xfffffffc, 'BD': 0x80000000, 'OT': 1, 'OP': 2, 'OF': 3, 'OD': 4, 'OB': 5, 'SEG': 1, 'RAW': 2, 'PALS': 3, 'PIC': 4}

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

    def picture_offset(self, file, table, slot, ordinal=0):
        """(offset, size) in its chunk of the first picture region of the chunk (table, slot, ordinal) of the file, or None."""
        first, count = self.files.get(file, (0, 0))
        for t, sl_, o, recipe in self.uses[first:first + count]:
            if (t, sl_, o) == (table, slot, ordinal):
                _size, rfirst, rcount = self.recipes[recipe]
                for kind, off, size, _width, _ifirst, _icount in self.regions[rfirst:rfirst + rcount]:
                    if kind == 4:
                        return off, size
        return None

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
                    if code == Z and not flip & 0x80000000:
                        tiles += ['zero'] * n
                    elif code in (Z, D, P, 0xfffffffc) or flip & 0x80000000:      # a by-design tile, a window: still the PS1's own tile for the lists' numbering
                        tiles += [None] * n
                    else:
                        tiles += [(code + j, flip & 3) for j in range(n)]
                out.append((table, slot, ordinal, k, tiles))
                k += 1
        return out




# ---------------------------------------------------------------- the select screen's backdrop cell map

SETS1, GROUPS1 = 0x2ad300, 0x2b1a80        # the arcade's scroll 1 (8 x 8 cell) block sets of 32 x 32 cells, and their 4 x 4 cell groups (stages.py)
# The cell maps of the screen modules that are windows of the arcade's scroll 1 sets (found 2026-10-23): 20 bands of 8 rows (rows y0 and on) of 64
# cells; band k takes its columns from three runs of columns: set 64 + k from x 7, set 84 from x 0, set 0 from x 0. The map holds a cell as two 16-bit
# words, the texture word and the palette word. segments: (columns, first set, set step per band, first x).
BACKDROPS = {'SELECTA.PAC': dict(slot=0x26, picture_slot=0x25, columns=64, rows=160, band=8, y0=3, first_page=22, clut=0x4024,
                                 segments=((25, 64, 1, 7), (32, 84, 0, 0), (7, 0, 0, 0)))}
UNIT_PAGES = 3                            # the picture: 3 pages of 32 x 32 units of 8 x 8 pixels (4 bits a pixel)
PICTURE_ROW = 384                         # bytes a row of the 768 pixel wide picture


def scroll1_cell(prog, set_number, x, y):
    """(code, attribute) of cell (x, y) of the arcade's scroll 1 block set `set_number` (32 x 32 cells, stages.py's scroll1_set_cells)."""
    ident = struct.unpack_from('>H', prog, SETS1 + 128 * set_number + ((x // 4) * 8 + y // 4) * 2)[0]
    return struct.unpack_from('>HH', prog, GROUPS1 + 64 * ident + 4 * ((x % 4) * 4 + y % 4))


def scroll1_piece_bytes(gfx, code, attr):
    """The 32 bytes of the 8 x 8 piece that a scroll 1 cell draws: the left half of the top (even code) or bottom (odd code) eight rows of
    graphics tile 0x10000 + code // 2, the colour index plus one, flipped by attribute bits 5 (across) and 6 (down), two pixels to a byte (the left
    one in the low nibble); None when the tile lies outside the graphics."""
    tile = 0x10000 + (code >> 1)
    if (tile + 1) * 128 > len(gfx):
        return None
    px = arcade_pixels(gfx[tile * 128:tile * 128 + 128])
    rows = [list(px[(y + (code & 1) * 8) * 16:(y + (code & 1) * 8) * 16 + 8]) for y in range(8)]
    if attr & 0x40:
        rows.reverse()
    if attr & 0x20:
        rows = [r[::-1] for r in rows]
    return bytes(r[x] | r[x + 1] << 4 for r in rows for x in range(0, 8, 2))


def picture_units(picture, known, first_page, pages=UNIT_PAGES):
    """{unit bytes: texture word} of the 8 x 8 units of a picture that are all known, the first unit (pages, then columns, then rows) of equal
    pixels naming the word: ((first_page + page) << 10) | (row << 5) | column."""
    units = {}
    for page in range(pages):
        for u in range(32):
            for v in range(32):
                at = [(v * 8 + yy) * PICTURE_ROW + page * 128 + u * 4 for yy in range(8)]
                if any(a + 4 > len(picture) or not all(known[a:a + 4]) for a in at):
                    continue
                units.setdefault(b''.join(picture[a:a + 4] for a in at), ((first_page + page) << 10) | (v << 5) | u)
    return units


def model_backdrop(prog, gfx, spec, picture, known):
    """(map bytes, mask) of a backdrop cell map: for each cell of the window the arcade's scroll 1 cell, its 8 x 8 piece looked up among the picture's
    units for the texture word, and the palette word clut + 64 * (attribute bits 0-4) (clut for a blank piece); a cell whose piece the picture does
    not hold is unknown (mask 0). Cells are rows of `columns`, two 16-bit little-endian words each."""
    units = picture_units(picture, known, spec['first_page'])
    out, mask = bytearray(spec['columns'] * spec['rows'] * 4), bytearray(spec['columns'] * spec['rows'] * 4)
    blank = bytes(32)
    for r in range(spec['rows']):
        band = r // spec['band']
        for c in range(spec['columns']):
            edge = 0
            for cols, first, step, x0 in spec['segments']:
                if c < edge + cols:
                    set_number, x = first + band * step, x0 + c - edge
                    break
                edge += cols
            code, attr = scroll1_cell(prog, set_number, x, spec['y0'] + r % spec['band'])
            piece = scroll1_piece_bytes(gfx, code, attr)
            tex = units.get(piece) if piece is not None else None
            if tex is None:
                continue
            clut = spec['clut'] if piece == blank else spec['clut'] + 64 * (attr & 31)
            at = (r * spec['columns'] + c) * 4
            struct.pack_into('<HH', out, at, tex, clut)
            mask[at:at + 4] = b'\x01' * 4
    return bytes(out), bytes(mask)


# ---------------------------------------------------------------- the analysis of one file

class Env:
    """The arcade side: program, graphics, tile classes, the records indexed by the positions of their cells, and the scroll 2 blocks' cells
    indexed by tile class."""

    def __init__(self, prog, gfx, tmp):
        self.prog = prog
        self.classes = Classes(tmp, gfx)
        self.by_xy = collections.defaultdict(list)
        self.cache = {}
        self.mode4 = []
        index = fighters.Index(fighters.View(prog))
        for addrs in index.by_cells.values():
            for a in addrs:
                rec = self.record(a)
                if rec:
                    self.by_xy[tuple(sorted(rec[2]))].append(a)
        for m in re.finditer(b'(?=\x00[\x04\x0a])', prog):
            if not m.start() & 1 and (rec := self.record(m.start())):
                self.by_xy[tuple(sorted(rec[2]))].append(m.start())
                if prog[m.start() + 1] == 4:
                    self.mode4.append(m.start())
        for v in self.by_xy.values():
            v.sort()
        # the scroll 2 blocks: per block 256 cells (class key, class d, attribute), cell (c, r) at c * 16 + r, and (key, c, r) -> blocks
        self.cell_info, self.cell_at = [], collections.defaultdict(list)
        self.key_cells = collections.defaultdict(list)
        for b in range(SCROLL_BLOCKS):
            info = []
            for c in range(16):
                for r in range(16):
                    code, attr = struct.unpack_from('>HH', prog, SCROLL_FIRST + SCROLL_BLOCK * b + 64 * c + 4 * r)
                    k = self.classes.cls(((code + SCROLL_ADD) & 0xffff) + 0x10000)
                    info.append((k[0], k[1], attr))
                    self.cell_at[(k[0], c, r)].append(b)
                    self.key_cells[k[0]].append((b, c, r))
            self.cell_info.append(info)

    def blank(self, tile):
        c = self.classes.cls(tile)
        return c is not None and c[0] == self.classes.zero_key

    def record(self, a):
        """(blocks, end, {(x, y): (code, flags)} of the record's cells) or None."""
        if a not in self.cache:
            rec = record_blocks(self.prog, a)
            self.cache[a] = None if rec is None else (rec[0], rec[1], {(c[1], c[2]): (c[0], c[3]) for c in
                                                                       emitted_cells(rec[0], 0, 0, record_drop(self.prog, a, 0x10000, self.blank))})
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


def position_base_prefix(mod, start, lists):
    """(base, end of the last stream, count) for a chain whose last lists are not lists of the chain (the scan took data after it for lists): the
    offset below `start` from which the most lists, one after another from the first, have streams that fit and end at or before the chain; count is at
    least 2 and at least half of the lists. None when there is none."""
    best = None
    for base in range(max(0, start - 0x3000) & ~1, start, 2):
        n, end = 0, base
        for lst in lists:
            if lst['n'] == 1 and lst['cells'] == [(0, None)]:
                n += 1
                continue
            e = fighters.stream_end(mod, base + lst['pos'], lst['n'])
            if e is None or e > start:
                break
            end = max(end, e)
            n += 1
        if n >= 2 and 2 * n >= len(lists) and (best is None or n > best[2]):
            best = (base, end, n)
    return best


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


def fit_cells(pool, cells, disc):
    """Fit the model's cells [(tile, x, y, flags)] (in the PS1's order) to a disc list {(x, y): (number, flags)}: the cells the disc lacks are
    drops (at most MAX_DROPS); a cell whose disc number is one of the PS1's own tiles is an own cell; every other cell must be looked up in the
    segment to the disc's number and flip bits. Returns {'drops': [model cell indexes], 'own': {index among the kept cells: (number, flip bits)}}
    when all agree, else None."""
    drops = [k for k, c in enumerate(cells) if (c[1], c[2]) not in disc]
    if len(drops) > MAX_DROPS or len(cells) - len(drops) != len(disc):
        return None
    dropped = set(drops)
    kept = [c for k, c in enumerate(cells) if k not in dropped]
    own = {i: (disc[(x, y)][0], (disc[(x, y)][1] >> 5) & 3) for i, (_t, x, y, _f) in enumerate(kept) if disc[(x, y)][0] in pool.kind2_set}
    nums = pool.number_cells(kept, own)
    if isinstance(nums, str):
        return None
    for (tile, x, y, fl), (n, g) in zip(kept, nums):
        if (n, g) != (disc[(x, y)][0], (disc[(x, y)][1] >> 5) & 3):
            return None
    return {'drops': drops, 'own': own}


def _tile_of(env, b, mc, mr):
    """The graphics tile (with its bank) of cell (mc, mr) of scroll 2 block b."""
    code, _attr = struct.unpack_from('>HH', env.prog, SCROLL_FIRST + SCROLL_BLOCK * b + 64 * mc + 4 * mr)
    return ((code + SCROLL_ADD) & 0xffff) + 0x10000


def find_windows(env, pool, disc):
    """The window (x0, y0, c0, r0, w, h, {(rx, ry): block}) of the scroll 2 blocks whose non-blank cells are the disc list {(x, y): (number, flags)},
    or None. The window is the list's bounding box; its cells are found by looking each block's cells up in the segment (a cell whose number is one
    of the PS1's own is not checked) and the cells of the box that the list lacks must be blank. The first window found, the blocks the lowest."""
    xs, ys = [p[0] for p in disc], [p[1] for p in disc]
    x0, y0 = min(xs), min(ys)
    if any((x - x0) % 16 or (y - y0) % 16 for x, y in disc):
        return None
    w, h = (max(xs) - x0) // 16 + 1, (max(ys) - y0) // 16 + 1
    cells = {((x - x0) // 16, (y - y0) // 16): (n, (f >> 5) & 3) for (x, y), (n, f) in disc.items()}
    if len(cells) < 3 or any(n >= len(pool.tiles) or pool.tiles[n] == 'zero' for n, _g in cells.values()):
        return None
    arc = {k: v for k, v in cells.items() if isinstance(pool.tiles[v[0]], tuple)}
    if not arc:
        return None
    key_of = {k: env.classes.cls(pool.tiles[v[0]][0])[0] for k, v in arc.items()}
    anchor = min(arc, key=lambda k: len(env.key_cells.get(key_of[k], ())))
    spots = env.key_cells.get(key_of[anchor], ())
    if len(spots) > 400:
        return None
    zero = env.classes.zero_key
    for b0, c, r in spots:
        xo, yo = 64 + c - anchor[0], 64 + r - anchor[1]      # the window's origin in cells, the anchor's block being region (4, 4)
        regions = collections.defaultdict(list)
        for dy in range(h):
            for dx in range(w):
                regions[((xo + dx) // 16, (yo + dy) // 16)].append((dx, dy, (xo + dx) % 16, (yo + dy) % 16))
        blocks = {}
        for reg, members in regions.items():
            found = None
            for b in ([b0] if reg == (4, 4) else range(SCROLL_BLOCKS)):
                info = env.cell_info[b]
                for dx, dy, mc, mr in members:
                    key, _d, attr = info[mc * 16 + mr]
                    if (dx, dy) not in cells:
                        if key != zero:
                            break
                    elif (dx, dy) in arc:
                        n, g = cells[(dx, dy)]
                        if key != key_of[(dx, dy)]:
                            break
                        rr = pool.lookup(_tile_of(env, b, mc, mr), (attr >> 5) & 3)
                        if rr is None or rr[0] != n or rr[1] != g:
                            break
                else:
                    found = b
                    break
            if found is None:
                break
            blocks[(reg[0] - xo // 16, reg[1] - yo // 16)] = found
        else:
            return (x0, y0, xo % 16, yo % 16, w, h, blocks)
    return None


def match_source(env, pool, cells, rules, prefer=None, prefer_bank=None):
    """The arcade source of a disc list: ('rec', (address, bank), fit, model cells), ('win', window, fit, model cells) or None. A record whose
    cells are at the disc list's positions is tried first (the record that follows the previous list's, then the lowest address); then, for the
    lists of 20 cells or more, the portraits (mode 4) that have all of the list's positions; then a window of the scroll 2 blocks."""
    disc = {(x, y): (n, f) for n, f, x, y in cells}
    cand = list(env.by_xy.get(tuple(sorted(disc)), ()))
    if prefer in cand:
        cand = [prefer] + [a for a in cand if a != prefer]
    if len(cells) >= 20:
        cand += [a for a in env.mode4 if a not in cand and disc.keys() <= env.record(a)[2].keys() and len(env.record(a)[2]) - len(disc) <= MAX_DROPS]
    banks = BANKS if prefer_bank is None else (prefer_bank,) + tuple(b for b in BANKS if b != prefer_bank)
    for a in cand:
        for bank in banks:
            r = record_cells(env.prog, a, bank, rules & PAL_SPLIT, env.blank)
            if r and (fit := fit_cells(pool, r[0], disc)) is not None:
                return 'rec', (a, bank), fit, r[0]
    win = find_windows(env, pool, disc)
    if win:
        mc = window_cells(env.prog, win, env.blank)
        if mc is not None and (fit := fit_cells(pool, mc, disc)) is not None:
            return 'win', win, fit, mc
    return None


def classify_lists(env, pool, mod, base, lists, rules):
    """For each list (info): ('rec', (address, bank), {arcade palette: disc palette}, {(x, y): disc palette}, fit, model cells), ('win', window, ...
    the same), ('empty',) or ('own',)."""
    out, prev, prev_bank, plen = [], None, None, 0
    ends = stream_ends(mod, base, lists)
    for lst, e in zip(lists, ends):
        cells = None if e is None else disc_cells(mod, base, lst)
        if e is None and lst['third'] == 0 and lst['pos'] == plen:
            out.append(('empty',))
        elif cells and (m := match_source(env, pool, cells, rules, prev, prev_bank)):
            kind, src, fit, mcells = m
            disc = {(x, y): f & 0x1f for _n, f, x, y in cells}
            seen = collections.defaultdict(collections.Counter)
            for k, (_t, x, y, fl) in enumerate(mcells):
                if k not in fit['drops']:
                    seen[fl & 0x1f][disc[(x, y)]] += 1
            out.append((kind, src, {a: c.most_common(1)[0][0] for a, c in seen.items()}, disc, fit, mcells))
            if kind == 'rec':
                prev, prev_bank = env.record(src[0])[1], src[1]
        else:
            out.append(('own',))
        if e is not None:
            plen = max(plen, e - base)
    return out


def assemble(env, pool, mod, base, lists, info, rules):
    """(items, maps, exceptions, windows) for the lists' classification: consecutive records with one bank, compatible palette maps and no tile
    of the PS1's own make one item; a list from a window is an item of its own; an own list is one item with its byte length and the
    position bytes it adds; the cells whose disc palette is not their item's map's, and the cells the PS1 left out, are the exceptions."""
    ends = stream_ends(mod, base, lists)
    items, maps, exceptions, windows = [], [], {}, []
    run = None            # [address, count, bank, merged map dict, next address]
    plen = 0
    groups = []           # (first list index, last list index, map dict) of each REC/WIN item, in the order of the items

    def close():
        nonlocal run
        if run:
            m = tuple(sorted((a, b) for a, b in run[3].items() if a != b))
            if m and m not in maps:
                maps.append(m)
            items.append((REC, run[0], run[1], run[2], maps.index(m) + 1 if m else 0))
            groups[-1].append(dict(m))
            run = None

    def single(i, inf):
        close()
        pal = inf[2]
        m = tuple(sorted((a, b) for a, b in pal.items() if a != b))
        if m and m not in maps:
            maps.append(m)
        if inf[0] == 'win':
            win = (inf[1][0], inf[1][1], inf[1][2], inf[1][3], inf[1][4], inf[1][5], tuple(sorted(inf[1][6].items())))
            if win not in windows:
                windows.append(win)
            items.append((WIN, windows.index(win), 1, 0x10000, maps.index(m) + 1 if m else 0))
        else:
            items.append((REC, inf[1][0], 1, inf[1][1], maps.index(m) + 1 if m else 0))
        groups.append([i, i, dict(m)])

    for i, (lst, inf) in enumerate(zip(lists, info)):
        if inf[0] == 'rec' and run is not None and run[4] == inf[1][0] and run[2] == inf[1][1] and (
                merged := merge_maps(run[3], inf[2])) is not None:
            run[1] += 1
            run[3] = merged
            groups[-1][1] = i
            run[4] = env.record(inf[1][0])[1]
        elif inf[0] == 'rec':
            close()
            run = [inf[1][0], 1, inf[1][1], dict(inf[2]), env.record(inf[1][0])[1]]
            groups.append([i, i])
        elif inf[0] == 'win':
            single(i, inf)
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
    # the exceptions: the cells the disc lacks, the cells that show tiles of the PS1's own, and the cells whose disc palette is not what the
    # item's map makes of the arcade's
    for first, last, palmap in groups:
        for i in range(first, last + 1):
            inf = info[i]
            mcells, fit, disc = inf[5], inf[4], inf[3]
            kept_index, prev = 0, 0
            for ci, (_t, x, y, flags) in enumerate(mcells):
                if ci in fit['drops']:
                    exceptions[(i, ci)] = DROP
                    continue
                if kept_index in fit['own']:
                    number, g = fit['own'][kept_index]
                    exceptions[(i, ci)] = OWN_TILE | g << 10 | (0 if number == pool.successor(prev) else number) << 16 | disc[(x, y)]
                    prev = number
                elif disc[(x, y)] != palmap.get(flags & 0x1f, flags & 0x1f):
                    exceptions[(i, ci)] = disc[(x, y)]
                kept_index += 1
    return items, maps, exceptions, windows


def analyse_chain(env, pool, mod, start, lists, base, rules):
    """Items, maps, exceptions, windows and the kind of every list for one setting of the rules; lists the model does not reproduce become own lists
    (and the model runs again) until none differs. Returns a dict."""
    info = classify_lists(env, pool, mod, base, lists, rules)
    first = sum(1 for inf in info if inf[0] != 'own')
    for _round in range(12):
        items, maps, exceptions, windows = assemble(env, pool, mod, base, lists, info, rules)
        built = model_chain(env.prog, pool, items, rules, maps, exceptions, windows)
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
    return dict(items=items, maps=maps, exceptions=exceptions, windows=windows, info=info, built=built, equal=equal, rules=rules,
                demoted=first - equal)


def own_tiles_arcade(pool, lst):
    """True when every tile number of the list is an arcade tile (or blank) of the pool: such a list is text or a picture laid out from the
    arcade's tiles with no art record behind it."""
    return all((t & 0x3fff) < len(pool.tiles) and pool.tiles[t & 0x3fff] is not None for t, _f in lst['cells'])


def choose_pool(env, pools, mod, base, lists):
    """The pool (index) whose lookups reproduce the numbers of the most lists (records at the lists' own positions), among the pools big enough for
    the chain's tile numbers; failing that, the one in which most cells are arcade tiles (the lists then come from windows). None when no pool is
    big enough."""
    top = max(max(t & 0x3fff for t, _f in lst['cells']) for lst in lists)
    best, arc = None, None
    sample = [lst for lst in lists[:80] if lst['n'] > 1 or lst['cells'] != [(0, None)]]
    for pi, pool in enumerate(pools):
        if len(pool[4]) <= top:
            continue
        p = pool[5]
        hits = 0
        for lst in sample:
            c = disc_cells(mod, base, lst)
            if c and any(fit_cells(p, record_cells(env.prog, a, bank, 0, env.blank)[0], {(x, y): (n, f) for n, f, x, y in c})
                         for a in env.by_xy.get(tuple(sorted((x, y) for _n, _f, x, y in c)), ()) for bank in BANKS
                         if record_cells(env.prog, a, bank, 0, env.blank)):
                hits += 1
        if hits and (best is None or hits > best[0]):
            best = (hits, pi)
        n = sum(1 for lst in lists for t, _f in lst['cells'] if t and isinstance(p.tiles[t & 0x3fff], tuple))
        if arc is None or n > arc[0]:
            arc = (n, pi)
    if best is not None:
        return best[1]
    return None if arc is None else arc[1]


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
        if pb is None and (pp := position_base_prefix(mod, start, lists)):
            lists = lists[:pp[2]]
            pb = pp[:2]
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
            tuple(tuple(row['maps'][m - 1]) if m else () for it in row['items'] if it[0] in (REC, WIN) for m in (it[4],)))


# ---------------------------------------------------------------- the report and the C table

def line(name, rows):
    if isinstance(rows, str):
        return f'{name}: {rows}'
    nl = rec = emp = own = win = 0
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
            elif inf[0] == 'win':
                win += 1
            elif inf[0] == 'empty':
                emp += 1
            else:
                own += 1
    built = [r for r in rows if 'info' in r]
    own_arc = sum(r['own_arcade'] for r in built)
    pos = sum(r['pos_size'] for r in built)
    pos_known = sum(sum(r['built'].pos_known) for r in built)
    exc = sum(len(r['exceptions']) for r in built)
    maps = sum(1 for r in built for it in r['items'] if it[0] in (REC, WIN) and it[4])
    bad = sum(1 for r in built if not r['sizes_ok'])
    demoted = sum(r['demoted'] for r in built)
    s = (f'{name}: {len(rows)} chains ({len(built)} with records), {nl} lists: {rec} from records, {win} from scroll windows, {emp} empty, {own} own ({own_arc} of them in chains with a tile segment and only arcade tiles); positions {pos_known} of {pos} B built; '
         f'{maps} record runs with a palette map, {exc} palette exceptions')
    if kinds:
        s += '; chains left out: ' + ', '.join(f'{v} {k}' for k, v in sorted(kinds.items()))
    if demoted:
        s += f'; {demoted} lists matched a record but not byte for byte (made own)'
    if bad:
        s += f'; {bad} chains with sizes that differ from the disc'
    return s


def c_table(files, picture_offsets=None):
    """The text of screen_lists_table.c for {file name: rows}; picture_offsets {file name: offset of the backdrop's picture in its chunk}."""
    items, wins, win_at, win_blocks = [], [], {}, []
    excs, pals, pal_at, maps_c, chains, chain_at, uses = [], [], {}, [], [], {}, []
    for name, rows in sorted(files.items()):
        if isinstance(rows, str):
            continue
        for r in rows:
            if 'info' not in r or not any(it[0] != OWN for it in r['items']):
                continue
            mp = []
            for it in r['items']:
                if it[0] in (REC, WIN) and it[4]:
                    m = tuple(r['maps'][it[4] - 1])
                    if m not in pal_at:
                        pal_at[m] = len(maps_c)
                        maps_c.append((len(pals), len(m)))
                        pals += list(m)
                    mp.append(pal_at[m] + 1)
                elif it[0] in (REC, WIN):
                    mp.append(0)
            it_c = []
            k = 0
            for it in r['items']:
                if it[0] == REC:
                    it_c.append((REC, it[2], it[1], it[3], mp[k], 0, 0))
                    k += 1
                elif it[0] == WIN:
                    w = r['windows'][it[1]]
                    if w not in win_at:
                        win_at[w] = len(wins)
                        wins.append((w[0], w[1], w[2], w[3], w[4], w[5], len(win_blocks), len(w[6])))
                        win_blocks += [(rx, ry, b) for (rx, ry), b in w[6]]
                    it_c.append((WIN, 1, win_at[w], it[3], mp[k], 0, 0))
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
           ' * and scroll blocks give: per chain of lists its sizes, rule flags, items (runs of records, windows of the scroll 2 blocks, empty lists,',
           ' * own lists that only keep their size), the palette maps and the cells that are left out, keep another palette or show a tile of the',
           ' * PS1\'s own; per file where each chain lies in its module and which tile segment of the screens table it numbers into. Only addresses,',
           ' * counts, offsets and numbers of tiles: no byte of either game. */',
           '#include "screen_lists.h"', '']
    out.append('/* {kind, count, address (REC) or window (WIN), bank, map (index + 1, 0: none), list bytes, stream bytes} */')
    out.append('const struct arcade_screen_lists_item arcade_screen_lists_items[] = {')
    for kind, count, a, bank, m, lb, sb in items:
        out.append(f'    {{{kind}, {count}u, {a:#x}u, {bank:#x}u, {m}u, {lb}u, {sb}u}},')
    out += ['};', '', '/* {x, y, first column, first row, columns, rows, first block, blocks}: the cell (i, j) of a window is at (x + 16 i, y + 16 j) */',
            'const struct arcade_screen_lists_window arcade_screen_lists_windows[] = {']
    out += [f'    {{{x}, {y}, {c}u, {r}u, {w}u, {h}u, {fb}u, {nb}u}},' for x, y, c, r, w, h, fb, nb in wins] or ['    {0, 0, 0u, 0u, 0u, 0u, 0u, 0u},']
    out += ['};', '', '/* {region column, region row, scroll 2 block} */', 'const struct arcade_screen_lists_win_block arcade_screen_lists_win_blocks[] = {']
    out += [f'    {{{rx}u, {ry}u, {b}u}},' for rx, ry, b in win_blocks] or ['    {0u, 0u, 0u},']
    out += ['};', '', '/* {list, cell, value}: a palette, or DROP, or OWN_TILE with flip bits, number and palette (screen_lists.h) */',
            'const struct arcade_screen_lists_exception arcade_screen_lists_exceptions[] = {']
    out += [f'    {{{i}u, {c}u, {p:#x}u}},' for i, c, p in excs] or ['    {0u, 0u, 0u},']
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
    out += ['};', '', '/* {file, cell map slot, picture slot, picture offset, columns, rows, rows a band, first row, first texture page, palette word, runs, {{columns, set, step, x}}} */',
            'const struct arcade_screen_lists_backdrop arcade_screen_lists_backdrops[] = {']
    bds = [(n, BACKDROPS[n], o) for n, o in sorted((picture_offsets or {}).items())]
    for n, bd, o in bds:
        runs = ', '.join(f'{{{c}u, {st}u, {sp}u, {x}u}}' for c, st, sp, x in bd['segments'])
        out.append(f'    {{"{n}", {bd["slot"]:#x}u, {bd["picture_slot"]:#x}u, {o}u, {bd["columns"]}u, {bd["rows"]}u, {bd["band"]}u, {bd["y0"]}u, {bd["first_page"]}u, '
                   f'{bd["clut"]:#x}u, {len(bd["segments"])}u, {{{runs}}}}},')
    if not bds:
        out.append('    {NULL, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, {{0u, 0u, 0u, 0u}}},')
    out += ['};', f'const unsigned arcade_screen_lists_backdrop_count = {len(bds)}u;',
            f'const unsigned arcade_screen_lists_item_count = {len(items)}u;',
            f'const unsigned arcade_screen_lists_window_count = {len(wins)}u;',
            f'const unsigned arcade_screen_lists_win_block_count = {len(win_blocks)}u;',
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
        files, backdrops = {}, {}
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
                if name in BACKDROPS:
                    backdrops[name] = backdrop_chunks(table, name, data)
                if not args.c_table and not args.compare_c:
                    print(line(name, files[name]), flush=True)
                    if name in BACKDROPS:
                        print(backdrop_line(name, prog, gfx, backdrops[name]), flush=True)
                    if args.detail and not isinstance(files[name], str):
                        for r in files[name]:
                            print(f'    chain {r["start"]:#x}: {r["nlists"]} lists; ' + (r['why'] if 'info' not in r else
                                  f'pool {r["pool"][0]}/{r["pool"][1]:#x}/{r["pool"][2]} segment {r["pool"][3]}, positions {r["base"]:#x}, '
                                  f'{r["equal"]} lists built, rules {r["rules"]}, {len(r["maps"])} maps, {len(r["exceptions"])} exceptions'))
        if args.c_table:
            sys.stdout.write(c_table(files, {n: b[0][0] for n, b in backdrops.items() if b}))
        elif args.compare_c:
            compare_c(files, prog, gfx, tmp, backdrops)
        print(f'{len(files)} files in {time.monotonic() - started:.1f} s', file=sys.stderr if args.c_table else sys.stdout)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


def backdrop_chunks(table, name, data):
    """((offset of the picture in its chunk, picture size), picture bytes, cell map bytes) of a file with a backdrop, or None."""
    spec = BACKDROPS[name]
    chunks = {(c['table'], c['slot']): c for c in pac.chunk_table(data)}
    pic = table.picture_offset(name, 0, spec['picture_slot'])
    if pic is None or (0, spec['picture_slot']) not in chunks or (0, spec['slot']) not in chunks:
        return None
    c = chunks[(0, spec['picture_slot'])]
    m = chunks[(0, spec['slot'])]
    return pic, data[c['offset'] + pic[0]:c['offset'] + pic[0] + pic[1]], data[m['offset']:m['offset'] + m['size']]


def backdrop_line(name, prog, gfx, chunks):
    """The model's backdrop map against the disc's, built from the disc's own picture."""
    if chunks is None:
        return f'{name} backdrop: no picture or map chunk'
    (_off, _size), picture, disc = chunks
    got, mask = model_backdrop(prog, gfx, BACKDROPS[name], picture, bytes([1]) * len(picture))
    return (f'{name} backdrop map: {sum(mask)} of {len(disc)} bytes built from the arcade\'s scroll 1 sets and the disc\'s picture, '
            f'{sum(1 for a, b, k in zip(got, disc, mask) if k and a == b)} equal to the disc\'s')


class CLib:
    """screen_lists.c with the checked-in table (and the screens table it reads), compiled into a temporary folder and called through ctypes."""

    class Use(ctypes.Structure):
        _fields_ = [('file', ctypes.c_char_p), ('module_slot', ctypes.c_uint16), ('positions', ctypes.c_uint32), ('lists', ctypes.c_uint32),
                    ('chain', ctypes.c_uint16), ('pool_table', ctypes.c_uint16), ('pool_slot', ctypes.c_uint16), ('pool_ordinal', ctypes.c_uint16),
                    ('pool_segment', ctypes.c_uint16)]

    class Out(ctypes.Structure):
        _fields_ = [('positions', ctypes.c_void_p), ('positions_known', ctypes.c_void_p), ('positions_size', ctypes.c_uint32),
                    ('lists', ctypes.c_void_p), ('lists_known', ctypes.c_void_p), ('lists_size', ctypes.c_uint32)]

    class Run(ctypes.Structure):
        _fields_ = [('columns', ctypes.c_uint16), ('set', ctypes.c_uint16), ('step', ctypes.c_uint16), ('x', ctypes.c_uint16)]

    class Backdrop(ctypes.Structure):
        pass

    class ScreensOut(ctypes.Structure):
        _fields_ = [('data', ctypes.c_void_p), ('known', ctypes.c_void_p), ('size', ctypes.c_uint32), ('unbuilt', ctypes.c_uint)]

    def __init__(self, tmp):
        lib = Path(tmp) / 'screen_lists.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-shared', '-fPIC', '-o', str(lib)] + [str(HERE / n) for n in
                       ('compose.c', 'screen_lists.c', 'screen_lists_table.c', 'screens.c', 'screens_table.c', 'tilepack.c')], check=True, capture_output=True, timeout=120)
        self.lib = ctypes.CDLL(str(lib))
        count = ctypes.c_uint.in_dll(self.lib, 'arcade_screen_lists_use_count').value
        self.uses = (self.Use * count).in_dll(self.lib, 'arcade_screen_lists_uses')
        self.lib.arcade_screen_lists_build.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(self.Use),
                                                       ctypes.POINTER(self.Out), ctypes.c_char_p, ctypes.c_size_t]
        self.lib.arcade_screen_lists_free.argtypes = [ctypes.POINTER(self.Out)]
        self.Backdrop._fields_ = [('file', ctypes.c_char_p), ('slot', ctypes.c_uint16), ('picture_slot', ctypes.c_uint16), ('picture_offset', ctypes.c_uint32),
                                  ('columns', ctypes.c_uint16), ('rows', ctypes.c_uint16), ('band', ctypes.c_uint16), ('y0', ctypes.c_uint16),
                                  ('first_page', ctypes.c_uint16), ('clut', ctypes.c_uint16), ('run_count', ctypes.c_uint), ('runs', self.Run * 4)]
        n = ctypes.c_uint.in_dll(self.lib, 'arcade_screen_lists_backdrop_count').value
        self.backdrops = (self.Backdrop * n).in_dll(self.lib, 'arcade_screen_lists_backdrops')
        self.lib.arcade_screens_find.restype = ctypes.c_void_p
        self.lib.arcade_screens_find.argtypes = [ctypes.c_char_p]
        self.lib.arcade_screens_recipe_of.restype = ctypes.c_void_p
        self.lib.arcade_screens_recipe_of.argtypes = [ctypes.c_void_p, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint]
        self.lib.arcade_screens_build.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t, ctypes.c_void_p, ctypes.c_char_p,
                                                  ctypes.c_size_t, ctypes.POINTER(self.ScreensOut), ctypes.c_char_p, ctypes.c_size_t]
        self.lib.arcade_screens_free.argtypes = [ctypes.POINTER(self.ScreensOut)]
        self.lib.arcade_screen_lists_backdrop_build.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(self.Backdrop),
                                                                ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_char_p,
                                                                ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t]

    def picture(self, prog, gfx, name, slot):
        """(chunk bytes, mask) of the screens table's build of chunk (0, slot, 0) of the file, or an error string."""
        f = self.lib.arcade_screens_find(name.encode())
        r = self.lib.arcade_screens_recipe_of(f, 0, slot, 0) if f else None
        if not r:
            return 'no recipe'
        out, err = self.ScreensOut(), ctypes.create_string_buffer(256)
        if self.lib.arcade_screens_build(prog, len(prog), gfx, len(gfx), r, None, 0, ctypes.byref(out), err, len(err)):
            return err.value.decode()
        try:
            return ctypes.string_at(out.data, out.size), ctypes.string_at(out.known, out.size)
        finally:
            self.lib.arcade_screens_free(ctypes.byref(out))

    def backdrop(self, prog, gfx, bd, picture, known, size):
        """(map bytes, mask) built by the C, or an error string."""
        out, mask = ctypes.create_string_buffer(size), ctypes.create_string_buffer(size)
        err = ctypes.create_string_buffer(256)
        if self.lib.arcade_screen_lists_backdrop_build(prog, len(prog), gfx, len(gfx), ctypes.byref(bd), picture, known, len(picture), out, mask, size, err, len(err)):
            return err.value.decode()
        return out.raw, mask.raw

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


def compare_c(files, prog, gfx, tmp, backdrops=None):
    """One line per file: the checked-in table's chains built by the C against the model and against the disc (on the bytes the C says it built)."""
    here = HERE / 'screen_lists_table.c'
    offsets = {n: b[0][0] for n, b in (backdrops or {}).items() if b}
    print('screen_lists_table.c ' + ('is the generated one' if here.read_text() == c_table(files, offsets) else 'DIFFERS from the generated one'))
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
    for bd in clib.backdrops:
        name = bd.file.decode()
        chunks = (backdrops or {}).get(name)
        spec = BACKDROPS[name]
        built = clib.picture(prog, gfx, name, bd.picture_slot)
        if chunks is None or isinstance(built, str):
            print(f'{name} backdrop: {built if isinstance(built, str) else "no chunks"}')
            continue
        (_off, size), _disc_picture, disc = chunks
        chunk, mask = built
        picture, pmask = chunk[bd.picture_offset:bd.picture_offset + size], mask[bd.picture_offset:bd.picture_offset + size]
        got = clib.backdrop(prog, gfx, bd, picture, pmask, len(disc))
        if isinstance(got, str):
            print(f'{name} backdrop: C refused: {got}')
            continue
        data, known = got
        model, mmask = model_backdrop(prog, gfx, spec, picture, pmask)
        print(f'{name} backdrop map: C == model {data == model and known == mmask}; built {sum(known)} of {len(disc)} bytes, '
              f'{sum(1 for a, b, k in zip(data, disc, known) if k and a == b)} equal to the disc\'s (the picture is {sum(1 for k in pmask if k)} of {len(pmask)} bytes built)')
    print(f'C total: {tot["chains"]} chains, C == model on {tot["same as model"]}, {tot["known bytes"]} B built of {tot["bytes"]}, '
          f'{tot["bytes equal to the disc"]} equal to the disc; {total * 1000:.1f} ms')


if __name__ == '__main__':
    sys.exit(main())
