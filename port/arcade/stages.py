#!/usr/bin/env python3
"""Analysis tool: compare the PS1 stage files (PAC/STAGEnn.PAC) with the arcade set and print what agrees.

What it does. A stage file of the disc holds eight chunks (seven for the last stage): a palette chunk, the
tile pages of the three scroll layers, the shared effect module and its tiles, the stage's own object tiles,
seven object palettes and the stage module (code and the three layer maps). This tool opens every stage file,
finds the three layer maps in the stage module, and for each layer compares its cells with the arcade's own
tile maps, cell by cell, by pixel content and palette row:
  - layer 1 (16 pixel tiles) and layer 2 (32 pixel tiles) are blocks of the arcade's scroll maps; the tool
    reports which arcade blocks they are (the map tables at 0x2d1f00, 154 blocks of 16x16 cells, and at
    0x2f8700, 101 blocks of 8x8 cells, both column by column) and how many cells agree;
  - layer 0 (16 pixel cells) is the arcade's scroll 1 layer (8x8 cells): four consecutive block sets of 32 x 32 cells
    from the tables at 0x2ad300 (sets of 8 x 8 ids) and 0x2b1a80 (groups of 4 x 4 cells), cell code n being the
    left half of the 8 rows of the 16x16 tile 0x10000 + n // 2 (top rows when n is even). The tool reports which
    sets they are and how many of the 1,024 pieces of each block agree in pixels and in palette row (a PS1 cell is
    two by two pieces with one palette row, so when its pieces have different rows the PS1 keeps one);
  - the object tiles (table 0 slot 9) and the shared effect tiles (slot 7) are lists of 128-byte tiles: it
    reports how many equal an arcade tile with the colour index one higher (the arcade's 15, transparent,
    is the PS1's 0);
  - the palette chunk is searched in the arcade program, each PS1 palette (16 words of 5-bit colours) being
    the arcade's 15 colour words of a palette row at brightness 0xe, shifted by one entry: it reports how many
    palettes were found, and how many palettes of the three layer tables equal the arcade rows that the stage
    number predicts (one table of 32 rows per stage and layer, 0x400 bytes apart), and the same for the seven
    object palettes (stage s: the 7 rows from 0x275c40 + 0xe0 * s).
It writes nothing except a temporary folder that holds a small C helper compiled at run time, removed on exit.
It prints counts, addresses and block numbers, never game bytes. It is an analysis and reference tool; the
converter that the port runs will be written in C.

Rules it checks (all measured on the supplied disc and set, see the README of this folder):
  - a layer cell of the PS1 is two 16-bit words, a tile code and a palette word. Tile code bits 0-4 times 8
    are the tile's u position in the page, bits 5-9 times 8 its v position, bits 10-15 the texture page,
    22 for the first of the stage's pages (chunk table 2, 32 KB per page, 128 bytes a row, 4 bits a pixel).
    The palette word is 0x7800 + layer + 64 * row: the layer's column and the row of the arcade cell's palette
    (attribute bits 0-4). Flips of the arcade cell (attribute bits 5 and 6) are baked into the page's tile;
  - a 16 pixel layer's map holds 32 by 16 cells to the page of 2,048 bytes, pages side by side, then below;
    a 32 pixel layer's map holds 16 by 8 cells to the page;
  - the PS1 colour of an arcade palette word is each 4-bit channel n scaled as n * 17 * 43 / 45 and cut to
    5 bits (brightness 0xe in the video chip's 0x0f + 2 * brightness formula);
  - the palette chunk of a stage holds its tables row by row: the even entries of the first 64 are layer 0's
    32 palettes, the odd ones layer 1's, entries 64 to 95 are layer 2's (in a chunk of 128 palettes or more
    entries 64 to 127 alternate layer 2's rows with a table of placeholder colours). Layer n's row r of stage s is the
    arcade palette row at 0x27c1c0, 0x2865c0 or 0x2909c0 (layers 0, 1, 2) + 0x400 * s + 0x20 * r. A colour that
    converts to black is 0x8000 in the layer palettes (0 means transparent to the PS1).

    python3 port/arcade/stages.py --disc BIN --arcade ZIP [--only 00] [--detail]

The loader's model (below the analysis; stage.c reproduces it byte for byte). A stage is built from the arcade alone, with the
descriptor in stages_table.c (made from the disc by --c-table: addresses, counts, flags and bit masks, no byte of either game):
  - which blocks each layer is: the arcade lists a stage's block ids as bytes in three tables of the program, layer 0's sets
    at 0x1d000 (u16 offsets a stage at 0x1cff4, entries from table + 4), layer 1's blocks at 0x1d0f8 (offsets at 0x1d0ec, entries
    from table + 0xc; one row of 4 ids at the entry), layer 2's blocks at 0x1d2cc + 0x40 * stage (16 rows of 4; row 14). The
    descriptor names, for each id the PS1 map uses, the program address of its byte (found by looking for the row of ids near the
    entry; the PS1 reads some rows rotated, turned about or from a neighbouring entry, and a few ids of 0 are constants);
  - the maps: the scan goes block by block (row by row of blocks, row by row of cells in a block). Layer 1 and layer 2 cells are
    the arcade cells (tile, flips, palette row = attribute bits 0-4); a layer 0 cell is 2 x 2 scroll 1 cells. Layer 0's palette row
    (l0_row) is what the PS1's palette merge leaves of its rule: the palette rows are tried in the order of the number of cells that
    have a piece in them, most first (lower number among equals), and a cell takes the first row whose colours are the ones its
    opaque pixels ask for (an index asks for the colour of its piece's row; a cell whose pieces ask for different colours of one
    index, or that no row fits, takes the row of its first opaque piece). That is the PS1's row for 93% of the cells that ask for one
    row; the PS1's merges of rows (rows made of colours of several rows, moved or approximated, run-time looking changes of the
    entries a row's cells do not use) are not reproduced;
  - the pages: a layer's different tiles (by pixels, flips baked in) take page slots in the order the scan first uses them, the
    blank (all transparent) tile first in the layer's first slot. 16 pixel slots run down a column of 16 then along the page
    (16 columns); 32 pixel slots 8 down, 8 along. Where the disc differs from that (the PS1 cleaned a cell to blank, gave a merged
    cell an earlier cell's tile, started a new slot, left slots to tiles of its own) the descriptor lists the cell and its slot
    (`exc`; place_layer, find_exceptions), and the packing is in step again after it;
  - the layer starts: layer 0 at slot 0, layer 1 at the next page column after layer 0's tiles and whatever extras or holes follow
    them without a gap, layer 2 at the next free 32 pixel column after layer 1's tiles and the extras and holes that follow them
    without a gap (START_RULE; l1_start_rule, l2_start_rule): the rule holds for layer 1 in 17 of 20 stages and for layer 2 in 19;
    the others keep their start in the descriptor. The runs of extras (arcade tiles the stage module animates, in the arcade's
    last 16 pixel tiles, one stage after another) and holes (tiles the arcade lacks) are in the descriptor;
  - which cells and tiles are known: a layer's flags (MAP_EXACT, TILES_EXACT) say that all of them equal the disc's; otherwise the
    layer's cell_known and tile_known bit masks name those that do (the layer 0 palette rows and merges are why the others differ);
  - the layer palettes: the arcade's palette row of the stage, layer and row, each 4-bit channel scaled (pal_exact: those that equal
    the disc's). The chunk's other palettes (the fourth table, and the tables beyond 128) are in runs (`more`): arcade rows that the
    PS1 copied, 0x20 apart, from addresses in the palette region (0x260000 to 0x2ad000, where the effects' rows lie), or filler rows
    (fifteen times the colour 0x322); the PS1's mixes of rows stay unknown;
  - the object tiles: runs of arcade tiles (the PS1 stores each flip as a tile); the object palettes: 7 arcade rows; the shared
    tiles: runs.
Options: --model reports, per stage and piece, how many bytes the model proves equal to the disc ("known"); --c-table prints
stages_table.c; --compare-c builds stage.c with the checked-in table and compares it with the model and the disc.
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

MODULE_BASE = 0x801e8000    # where the stage module (table 0 slot 6) is loaded
FIRST_PAGE = 22             # texture page number (code >> 10) of the stage's first 256x256 page
PAGE_BYTES = 32768          # one page: 256 rows of 128 bytes
BRIGHTNESS = 14             # the brightness the PS1 palettes were made at
BLANK_L0 = 0x5800           # the cell code layer 0's setup writes as "no tile"
BLANK_L2 = 0x20             # the same for layer 2
PALETTE_TABLES = (0x27c1c0, 0x2865c0, 0x2909c0)   # the arcade's palette rows of layer 0, 1, 2 of stage 0; 0x400 more a stage
PALETTE_OBJECTS = 0x275c40                         # the arcade's 7 object palette rows of stage 0; 7 * 0x20 more a stage
SETS1 = 0x2ad300            # the arcade's scroll 1 block sets: 143 sets of 128 bytes (8 x 8 ids of 4 x 4 cell groups), 32 x 32 cells each
GROUPS1 = 0x2b1a80          # the 4 x 4 cell groups of scroll 1: 64 bytes each, column by column
TABLE16 = 0x2d1f00          # the arcade's 16x16 cell blocks: 154 blocks of 1,024 bytes
TABLE32 = 0x2f8700          # the arcade's 32x32 cell blocks: 101 blocks of 256 bytes
LAYER_NAMES = ('layer 0', 'layer 1', 'layer 2')
KINDS = {  # (table, slot) of a chunk -> its name in the report
    (1, 0): 'palettes', (2, 0): 'pages', (0, 3): 'effects', (0, 7): 'effect tiles',
    (0, 9): 'object tiles', (0, 10): 'object palettes', (0, 6): 'module'}


# ---------------------------------------------------------------- pure functions (tested without game data)

def sx16(v):
    return v - 65536 if v & 0x8000 else v


def arcade_color_to_ps1(word, brightness=BRIGHTNESS):
    """The 15-bit PS1 colour (red in bits 0-4) of an arcade palette word (red, green, blue nibbles) at a brightness."""
    bright = 0x0f + (brightness << 1)
    out = 0
    for shift, nib in ((0, (word >> 8) & 15), (5, (word >> 4) & 15), (10, word & 15)):
        out |= ((nib * 0x11 * bright // 0x2d) >> 3) << shift
    return out


def ps1_palette_from_arcade(words, brightness=BRIGHTNESS):
    """The 16 PS1 colours of an arcade palette row of 16 words: entry 0 transparent (0), entry k the arcade's
    k-1 converted; a colour that converts to black is stored as 0x8000 (black with the opaque bit), as 0 means
    transparent to the PS1."""
    if len(words) < 15:
        raise ValueError('a palette row has 15 colours')
    return [0] + [arcade_color_to_ps1(w, brightness) or 0x8000 for w in words[:15]]


def is_flat_palette(chunk, k):
    """True when palette k of a chunk has the same colour in all of entries 1 to 15 (it then matches many rows)."""
    row = struct.unpack_from('<16H', chunk, 32 * k)
    return len(set(row[1:])) == 1


def palette_group(k, count):
    """Which table palette k of a stage's palette chunk belongs to: (layer, row), or ('more', index) for a table
    that no layer of the tests uses. The first 64 entries alternate layer 0 and layer 1 rows. A chunk of fewer
    than 128 palettes then holds layer 2's 32 rows; a chunk of 128 or more alternates layer 2's rows with the
    rows of a fourth table (placeholder colours), and may go on with more tables."""
    if k < 64 and k < count:
        return (k & 1, k >> 1)
    if 64 <= k < 96 and count < 128:
        return (2, k - 64)
    if 64 <= k < 128 and count >= 128:
        return (2, (k - 64) >> 1) if k % 2 == 0 else ('more', k)
    return ('more', k)


def decode_cell(code, clut):
    """(page index in the stage's chunk, u, v, layer column, palette row) of a layer cell's two words."""
    return ((code >> 10) - FIRST_PAGE, (code & 0x1f) * 8, ((code >> 5) & 0x1f) * 8, clut & 63, (clut - 0x7800) >> 6)


def map_word_index(cx, cy, cells_across, side):
    """Index of cell (cx, cy)'s first 16-bit word in a layer map (two words a cell), for 16 or 32 pixel cells."""
    if side == 16:
        return (cy >> 4) * ((cells_across >> 5) << 10) + ((cx >> 5) << 10) + ((cy & 15) << 6) + ((cx & 31) << 1)
    if side == 32:
        return (cy >> 3) * ((cells_across >> 4) << 8) + ((cx >> 4) << 8) + ((cy & 7) << 5) + ((cx & 15) << 1)
    raise ValueError('cell size')


def arcade_cell_tile(code, attr, side):
    """(gfx tile index, flip x, flip y, palette row) of an arcade map cell; the tile index counts 128 or 512 bytes."""
    if side == 16:
        return (((code + 0xa400) & 0xffff) + 0x10000, bool(attr & 0x20), bool(attr & 0x40), attr & 31)
    return (((code + 0x900) & 0x3fff) + 0x4000, bool(attr & 0x20), bool(attr & 0x40), attr & 31)


def scroll1_set_cells(prog, number):
    """The 32 x 32 cells (code, attribute) of the arcade's scroll 1 block set `number`, as {(x, y): (code, attribute)}.

    The set is 64 big-endian ids, 8 columns of 8 rows; id m names the 4 x 4 cells at GROUPS1 + 64 * m, stored column by
    column (4 cells down, then the next column)."""
    ids = struct.unpack_from('>64H', prog, SETS1 + 128 * number)
    cells = {}
    for col in range(8):
        for row in range(8):
            at = GROUPS1 + 64 * ids[col * 8 + row]
            for c in range(4):
                for r in range(4):
                    cells[(col * 4 + c, row * 4 + r)] = struct.unpack_from('>HH', prog, at + 4 * (c * 4 + r))
    return cells


def scroll1_piece(code):
    """(index of the 16x16 gfx tile, first pixel row) of a scroll 1 cell code: 8x8 pixels, the left half of the tile's
    top (even code) or bottom (odd code) eight rows, the tile being 0x10000 + code // 2."""
    return 0x10000 + (code >> 1), (code & 1) * 8


def arcade_block_cell(k, cells):
    """(x, y) of the k-th cell of an arcade block: column by column."""
    return (k // cells, k % cells)


def find_layers(module):
    """The three layer maps that the stage module's setup writes: [(offset, blank code, width)] for layers 0, 1, 2.

    The setup functions store, in straight-line code, the map pointer at +0x50 of a layer record, its width at
    +0x58 and its blank code (a halfword) at +0x1e. The scan follows lui/addiu/ori/addu constants word by word.
    Returns None when the three are not all found. Layer 0's blank code is 0x5800, layer 2's 0x20."""
    regs = {0: 0}
    found = {}
    cur = None
    for i in range(0, len(module) - 3, 4):
        w = struct.unpack_from('<I', module, i)[0]
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xffff
        if op == 0xf:
            regs[rt] = imm << 16 if rt else 0
        elif op == 0x9 and regs.get(rs) is not None:
            regs[rt] = (regs[rs] + sx16(imm)) & 0xffffffff
        elif op == 0xd and regs.get(rs) is not None:
            regs[rt] = regs[rs] | imm
        elif op == 0 and (w & 0x3f) == 0x21 and regs.get(rs) is not None and regs.get(rt) is not None:
            regs[(w >> 11) & 31] = (regs[rs] + regs[rt]) & 0xffffffff if (w >> 11) & 31 else 0
        elif op in (0x9, 0xa, 0xb, 0xc, 0xd, 0xe) or 0x20 <= op <= 0x26:
            regs[rt] = None if rt else 0
        elif op == 0 and (w & 0x3f) not in (0x08, 0x18, 0x19, 0x1a, 0x1b):
            regs[(w >> 11) & 31] = None if (w >> 11) & 31 else 0
        elif op == 0x2b and imm == 0x50 and regs.get(rt) is not None:
            ptr = regs[rt] - MODULE_BASE
            cur = found.setdefault(ptr, {}) if 0 <= ptr < len(module) else None
        elif op == 0x2b and imm == 0x58 and cur is not None and regs.get(rt) is not None:
            cur['width'] = regs[rt]
        elif op == 0x29 and imm == 0x1e and cur is not None and regs.get(rt) is not None:
            cur['blank'] = regs[rt]
    layers = [None, None, None]
    for ptr in sorted(found):
        blank = found[ptr].get('blank')
        if blank == BLANK_L0 and layers[0] is None:
            layers[0] = ptr
        elif blank == BLANK_L2 and layers[2] is None:
            layers[2] = ptr
        elif blank is not None and BLANK_L0 < blank <= 0x6400 and layers[1] is None:
            layers[1] = ptr
    if None in layers:
        return None
    return [(p, found[p]['blank'], found[p].get('width') if found[p].get('width') in (512, 1024, 2048) else 1024)
            for p in layers], sorted(found)


def layer_extents(layers, pointers):
    """[(offset, cells across, cells down, cell size)] of the three layers: heights of the 16 pixel layers come
    from the distance to the next map pointer (at most 64 cells down), layer 2 is 8 cells of 32 pixels down."""
    out = []
    for n, (ptr, _blank, width) in enumerate(layers):
        if n == 2:
            out.append((ptr, width // 32, 8, 32))
            continue
        later = [p for p in pointers if p > ptr]
        gap = (later[0] - ptr) if later else 0x1000
        across = width // 16
        rows = min(64, max(16, (gap // 4 // across) // 16 * 16))
        out.append((ptr, across, rows, 16))
    return out


def split_stage(data):
    """{kind: bytes} of a stage file's chunks, or an error string. The second palette chunk is 'shared palettes'."""
    out = {}
    for c in pac.chunk_table(data):
        kind = KINDS.get((c['table'], c['slot']))
        if kind is None:
            return f"unknown chunk table {c['table']} slot {c['slot']}"
        if kind == 'palettes' and kind in out:
            kind = 'shared palettes'
        if kind in out:
            return f'two {kind} chunks'
        out[kind] = data[c['offset']:c['offset'] + c['size']]
    for need in ('palettes', 'pages', 'module', 'effects', 'effect tiles', 'object palettes', 'shared palettes'):
        if need not in out:
            return f'no {need} chunk'
    return out


def predicted_palette(prog, stage, layer, row):
    """The PS1 palette of a layer's palette row predicted from the arcade program: 32 rows of 16 words a table,
    one table of 0x400 bytes per stage and layer, the three layers' tables at PALETTE_TABLES."""
    at = PALETTE_TABLES[layer] + 0x400 * stage + 0x20 * row
    return ps1_palette_from_arcade(struct.unpack_from('>16H', prog, at))


def palette_agreement(prog, stage, chunk, used=None):
    """([equal, total] for layers 0, 1, 2) of the chunk's palettes of those three tables against predicted_palette.

    used: optional [set of rows, ...] per layer; then only those rows count (the rows the layer's cells use)."""
    out = [[0, 0], [0, 0], [0, 0]]
    for k in range(min(128, len(chunk) // 32)):
        layer, row = palette_group(k, len(chunk) // 32)
        if layer == 'more' or (used is not None and row not in used[layer]):
            continue
        have = list(struct.unpack_from('<16H', chunk, 32 * k))
        out[layer][1] += 1
        out[layer][0] += have == predicted_palette(prog, stage, layer, row)
    return out


def rows_used(module, layers, extents):
    """[set of palette rows] that the non-blank cells of the three layer maps use."""
    out = []
    for n, (off, across, down, side) in enumerate(extents):
        rows = set()
        for cy in range(down):
            for cx in range(across):
                at = off + 2 * map_word_index(cx, cy, across, side)
                if at + 4 > len(module):
                    continue
                code, clut = struct.unpack_from('<HH', module, at)
                if code != layers[n][1]:
                    rows.add(decode_cell(code, clut)[4])
        out.append(rows)
    return out


def row_classes(prog, stage, chunk, layer, rows):
    """(equal, a few entries differ (1 to 8), the whole row differs) over the given rows of one layer's table."""
    out = [0, 0, 0]
    for row in rows:
        k = row * 2 + layer if layer < 2 else 64 + row * (2 if len(chunk) // 32 >= 128 else 1)
        have = struct.unpack_from('<16H', chunk, 32 * k)
        diff = sum(1 for a, b in zip(have, predicted_palette(prog, stage, layer, row)) if a != b)
        out[0 if diff == 0 else 1 if diff <= 8 else 2] += 1
    return tuple(out)


def same_palette(a, b):
    """Two 16-entry PS1 palettes equal, black counting the same as 0 or as 0x8000 (the object palettes use 0)."""
    return all(x == y or (x in (0, 0x8000) and y in (0, 0x8000)) for x, y in zip(a, b))


def object_palette_agreement(prog, stage, chunk):
    """(equal, total) of the 7 object palettes of a stage against the arcade rows 7 * 0x20 * stage after PALETTE_OBJECTS."""
    equal = 0
    for k in range(len(chunk) // 32):
        at = PALETTE_OBJECTS + 7 * 0x20 * stage + 0x20 * k
        equal += same_palette(struct.unpack_from('<16H', chunk, 32 * k), ps1_palette_from_arcade(struct.unpack_from('>16H', prog, at)))
    return equal, len(chunk) // 32


def first_palette_difference(prog, stage, chunk):
    """For layers 0, 1, 2: (row, entries that differ) of the first row of the chunk that differs from
    predicted_palette, or None when all of the layer's rows are equal."""
    out = [None, None, None]
    for k in range(min(128, len(chunk) // 32)):
        layer, row = palette_group(k, len(chunk) // 32)
        if layer == 'more':
            continue
        have = struct.unpack_from('<16H', chunk, 32 * k)
        want = predicted_palette(prog, stage, layer, row)
        if out[layer] is None and list(have) != want:
            out[layer] = (row, sum(1 for a, b in zip(have, want) if a != b))
    return out


# ---------------------------------------------------------------- the C helper (compiled at run time)

HELPER = r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef uint64_t u64;
static const uint8_t *G; static long GN; static const uint8_t *P; static long PN;
static u64 *A16, *A32; static uint8_t *Q16, *Q32;   /* hashes and palette rows of the arcade blocks */
static u64 *t16k; static int32_t *t16v; static u64 *t8k; static int32_t *t8v;
#define FLAT(v) (0xf1a7000000000000ull | (v))
static u64 fnv(const uint8_t *px, int side, int fx, int fy) {
  u64 h = 1469598103934665603ull; int flat = 1;
  for (int y = 0; y < side; y++) for (int x = 0; x < side; x++) {
    unsigned v = px[(fy ? side - 1 - y : y) * side + (fx ? side - 1 - x : x)];
    if (v != px[0]) flat = 0;
    h = (h ^ v) * 1099511628211ull; }
  return flat ? FLAT(px[0]) : h; }
static void arc_pixels(long idx, int side, int qx0, int qy0, int n, uint8_t *px) {   /* n x n pixels at (qx0, qy0) of a tile */
  long sz = (long)side * side / 2; const uint8_t *q = (idx >= 0 && (idx + 1) * sz <= GN) ? G + idx * sz : 0;
  for (int y = 0; y < n; y++) for (int x = 0; x < n; x++) {
    int b = 0, xx = qx0 + x, yy = qy0 + y;
    if (q) for (int pl = 0; pl < 4; pl++) b |= ((q[yy * side / 2 + xx / 8 * 4 + pl] >> (7 - xx % 8)) & 1) << pl; else b = 15;
    px[y * n + x] = (b + 1) & 15; } }
static const int NB16 = 154, NB32 = 101, NSET1 = 143;
static u64 *A8; static uint8_t *Q8;   /* the scroll 1 block sets: 143 sets of 32 x 32 cells, column by column */
int arc_init(const uint8_t *gfx, long gfx_len, const uint8_t *prog, long prog_len) {
  G = gfx; GN = gfx_len; P = prog; PN = prog_len;
  if (prog_len < 0x2f8700 + 101 * 256) return -1;
  A16 = malloc(sizeof(u64) * NB16 * 256); Q16 = malloc(NB16 * 256); A32 = malloc(sizeof(u64) * NB32 * 64); Q32 = malloc(NB32 * 64);
  uint8_t px[1024];
  for (int b = 0; b < NB16; b++) for (int k = 0; k < 256; k++) {
    const uint8_t *e = prog + 0x2d1f00 + b * 1024 + k * 4; unsigned code = e[0] << 8 | e[1], attr = e[2] << 8 | e[3];
    arc_pixels((long)((code + 0xa400) & 0xffff) + 0x10000, 16, 0, 0, 16, px);
    A16[b * 256 + k] = fnv(px, 16, !!(attr & 0x20), !!(attr & 0x40)); Q16[b * 256 + k] = attr & 31; }
  for (int b = 0; b < NB32; b++) for (int k = 0; k < 64; k++) {
    const uint8_t *e = prog + 0x2f8700 + b * 256 + k * 4; unsigned code = e[0] << 8 | e[1], attr = e[2] << 8 | e[3];
    arc_pixels((long)((code + 0x900) & 0x3fff) + 0x4000, 32, 0, 0, 32, px);
    A32[b * 64 + k] = fnv(px, 32, !!(attr & 0x20), !!(attr & 0x40)); Q32[b * 64 + k] = attr & 31; }
  A8 = malloc(sizeof(u64) * NSET1 * 1024); Q8 = malloc(NSET1 * 1024);
  for (int b = 0; b < NSET1; b++) for (int col = 0; col < 8; col++) for (int row = 0; row < 8; row++) {
    unsigned m = prog[0x2ad300 + b * 128 + (col * 8 + row) * 2] << 8 | prog[0x2ad300 + b * 128 + (col * 8 + row) * 2 + 1];
    for (int c = 0; c < 4; c++) for (int r = 0; r < 4; r++) {
      const uint8_t *e = prog + 0x2b1a80 + (long)m * 64 + (c * 4 + r) * 4; unsigned code = e[0] << 8 | e[1], attr = e[2] << 8 | e[3];
      if (0x2b1a80 + (long)m * 64 + 64 > prog_len) code = 0x20;
      arc_pixels(0x10000 + (code >> 1), 16, 0, (code & 1) * 8, 8, px);
      int k = (col * 4 + c) * 32 + row * 4 + r; A8[b * 1024 + k] = fnv(px, 8, !!(attr & 0x20), !!(attr & 0x40)); Q8[b * 1024 + k] = attr & 31; } }
  return 0; }
static int page_pixels(const uint8_t *pages, long plen, unsigned code, int side, int dx, int dy, int n, uint8_t *px) {
  long pg = (long)(code >> 10) - 22; int u = (code & 0x1f) * 8 + dx, v = ((code >> 5) & 0x1f) * 8 + dy;
  if (pg < 0 || (pg + 1) * 32768 > plen || u + n > 256 || v + n > 256) return 0;
  for (int y = 0; y < n; y++) for (int x = 0; x < n; x++) { int uu = u + x; uint8_t b = pages[pg * 32768 + (v + y) * 128 + uu / 2]; px[y * n + x] = (uu & 1) ? b >> 4 : b & 15; }
  return 1; }
static int cell_words(const uint8_t *mod, long mlen, long off, int wc, int side, int cx, int cy, unsigned *code, unsigned *clut) {
  long i16 = side == 16 ? (long)(cy >> 4) * ((wc >> 5) << 10) + ((cx >> 5) << 10) + ((cy & 15) << 6) + ((cx & 31) << 1)
                        : (long)(cy >> 3) * ((wc >> 4) << 8) + ((cx >> 4) << 8) + ((cy & 7) << 5) + ((cx & 15) << 1);
  if (off + i16 * 2 + 4 > mlen) return 0;
  const uint8_t *e = mod + off + i16 * 2; *code = e[0] | e[1] << 8; *clut = e[2] | e[3] << 8; return 1; }
/* out: per block of the layer, 5 ints: non-flat cells, best arcade block, matching cells, ties, matching non-flat cells */
int match_layer(const uint8_t *pages, long plen, const uint8_t *mod, long mlen, long off, int wc, int hc, int side, int32_t *out) {
  int bc = side == 16 ? 16 : 8, nb = side == 16 ? NB16 : NB32; const u64 *A = side == 16 ? A16 : A32; const uint8_t *Q = side == 16 ? Q16 : Q32;
  int n = 0; uint8_t px[1024]; u64 ph[256]; uint8_t pp[256];
  for (int by = 0; by < hc / bc; by++) for (int bx = 0; bx < wc / bc; bx++) {
    int nonflat = 0;
    for (int y = 0; y < bc; y++) for (int x = 0; x < bc; x++) {
      unsigned code, clut; u64 h;
      if (!cell_words(mod, mlen, off, wc, side, bx * bc + x, by * bc + y, &code, &clut) || !page_pixels(pages, plen, code, side, 0, 0, side, px)) h = 0xdead000000000000ull | code;
      else { h = fnv(px, side, 0, 0); if ((h >> 48) != 0xf1a7) nonflat++; }
      ph[y * bc + x] = h; pp[y * bc + x] = ((clut - 0x7800) >> 6) & 31; }
    int best = 0, bestn = -1, ties = 0, bestnf = 0;
    for (int b = 0; b < nb; b++) {
      int m = 0, mn = 0;
      for (int y = 0; y < bc; y++) for (int x = 0; x < bc; x++) {
        int k = x * bc + y; u64 a = A[b * bc * bc + k];
        if (a == ph[y * bc + x] && ((a >> 48) == 0xf1a7 || Q[b * bc * bc + k] == pp[y * bc + x])) { m++; if ((a >> 48) != 0xf1a7) mn++; } }
      if (m > bestn) { bestn = m; best = b; ties = 0; bestnf = mn; } else if (m == bestn) ties++; }
    out[n * 5 + 0] = nonflat; out[n * 5 + 1] = best; out[n * 5 + 2] = bestn; out[n * 5 + 3] = ties; out[n * 5 + 4] = bestnf; n++; }
  return n; }
/* layer 0: per block of 16 x 16 PS1 cells (32 x 32 pieces of 8 x 8), the best scroll 1 set; 5 ints a block: non-flat pieces, set,
   pieces equal in pixels and palette row, pieces equal in pixels, non-flat pieces equal in both */
int match_layer0(const uint8_t *pages, long plen, const uint8_t *mod, long mlen, long off, int wc, int hc, int32_t *out) {
  int n = 0; uint8_t px[64]; static u64 ph[1024]; static uint8_t pp[1024];
  for (int by = 0; by < hc / 16; by++) for (int bx = 0; bx < wc / 16; bx++) {
    int nonflat = 0;
    for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) {
      unsigned code, clut; int ok = cell_words(mod, mlen, off, wc, 16, bx * 16 + x, by * 16 + y, &code, &clut);
      for (int q = 0; q < 4; q++) {
        u64 h = 0xdead000000000000ull | code;
        if (ok && page_pixels(pages, plen, code, 16, (q & 1) * 8, (q >> 1) * 8, 8, px)) { h = fnv(px, 8, 0, 0); if ((h >> 48) != 0xf1a7) nonflat++; }
        int k = (x * 2 + (q & 1)) * 32 + y * 2 + (q >> 1); ph[k] = h; pp[k] = ((clut - 0x7800) >> 6) & 31; } }
    int best = 0, bestn = -1, bestpix = 0, bestnf = 0;
    for (int b = 0; b < NSET1; b++) {
      int m = 0, mn = 0, mp = 0;
      for (int k = 0; k < 1024; k++) { u64 a = A8[b * 1024 + k]; if (a == ph[k]) { mp++; if ((a >> 48) == 0xf1a7 || Q8[b * 1024 + k] == pp[k]) { m++; if ((a >> 48) != 0xf1a7) mn++; } } }
      if (m > bestn || (m == bestn && mp > bestpix)) { bestn = m; best = b; bestpix = mp; bestnf = mn; } }
    out[n * 5 + 0] = nonflat; out[n * 5 + 1] = best; out[n * 5 + 2] = bestn; out[n * 5 + 3] = bestpix; out[n * 5 + 4] = bestnf; n++; }
  return n; }
static u64 hash8(const uint8_t *px, int fl) { return fnv(px, 8, fl & 1, (fl >> 1) & 1); }
#define H8 23
static void build8(void) {
  if (t8k) return;
  t8k = calloc(1ull << H8, 8); t8v = malloc((1ull << H8) * 4); uint8_t px[64]; long nt = GN / 128;
  for (long t = 0; t < nt; t++) for (int q = 0; q < 4; q++) {
    arc_pixels(t, 16, (q & 1) * 8, (q >> 1) * 8, 8, px);
    for (int fl = 0; fl < 4; fl++) { u64 k = hash8(px, fl), i = (k >> 9) & ((1ull << H8) - 1); while (t8k[i] && t8k[i] != k) i = (i + 1) & ((1ull << H8) - 1); if (!t8k[i]) { t8k[i] = k; t8v[i] = (int32_t)t; } } } }
static int has8(u64 k) { u64 m = (1ull << H8) - 1, i = (k >> 9) & m; while (t8k[i]) { if (t8k[i] == k) return 1; i = (i + 1) & m; } return 0; }
/* out[0] non-flat 8x8 pieces of a 16 pixel layer, out[1] of them that are pieces of an arcade tile */
int quadrants(const uint8_t *pages, long plen, const uint8_t *mod, long mlen, long off, int wc, int hc, int32_t *out) {
  build8(); uint8_t px[64]; out[0] = out[1] = 0;
  for (int cy = 0; cy < hc; cy++) for (int cx = 0; cx < wc; cx++) {
    unsigned code, clut; if (!cell_words(mod, mlen, off, wc, 16, cx, cy, &code, &clut)) continue;
    for (int q = 0; q < 4; q++) {
      if (!page_pixels(pages, plen, code, 16, (q & 1) * 8, (q >> 1) * 8, 8, px)) continue;
      u64 h = hash8(px, 0); if ((h >> 48) == 0xf1a7) continue;
      out[0]++; if (has8(h)) out[1]++; } }
  return 0; }
/* tiles: out[i] = 4 * arcade tile index + flip (1 x, 2 y) of the first arcade tile equal to PS1 tile i (colour one higher), or -1 */
int match_tiles(const uint8_t *chunk, long n, int32_t *out) {
  if (!t16k) { t16k = calloc(1ull << 23, 8); t16v = malloc((1ull << 23) * 4); uint8_t px[256]; long nt = GN / 128, m = (1ull << 23) - 1;
    for (long t = 0; t < nt; t++) { arc_pixels(t, 16, 0, 0, 16, px);
      for (int fl = 0; fl < 4; fl++) { u64 k = fnv(px, 16, fl & 1, fl >> 1), i = (k >> 9) & m; while (t16k[i] && t16k[i] != k) i = (i + 1) & m; if (!t16k[i]) { t16k[i] = k; t16v[i] = (int32_t)(t * 4 + fl); } } } }
  uint8_t px[256]; long m = (1ull << 23) - 1; int found = 0;
  for (long i = 0; i < n; i++) {
    for (int k = 0; k < 128; k++) { px[2 * k] = chunk[i * 128 + k] & 15; px[2 * k + 1] = chunk[i * 128 + k] >> 4; }
    u64 k = fnv(px, 16, 0, 0), j = (k >> 9) & m; out[i] = -1;
    while (t16k[j]) { if (t16k[j] == k) { out[i] = t16v[j]; found++; break; } j = (j + 1) & m; } }
  return found; }
/* palettes: pals = np rows of 16 little-endian words; out[i] = offset of the first arcade row whose 15 words give entries 1-15, or -1
   (a colour that converts to black may be stored as 0 or as 0x8000) */
int find_palettes(const uint16_t *pals, int np, int brightness, int32_t *out) {
  static uint16_t cv[65536]; int bright = 0x0f + (brightness << 1);
  for (int w = 0; w < 65536; w++) { int r = ((w >> 8) & 15) * 0x11 * bright / 0x2d, g = ((w >> 4) & 15) * 0x11 * bright / 0x2d, b = (w & 15) * 0x11 * bright / 0x2d; cv[w] = (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10); }
  int found = 0;
  for (int k = 0; k < np; k++) { out[k] = -1;
    for (long o = 0; o + 30 <= PN; o += 2) {
      int ok = 1; for (int i = 1; i < 16 && ok; i++) { int c = cv[(P[o + 2 * (i - 1)] << 8) | P[o + 2 * (i - 1) + 1]], p = pals[k * 16 + i]; if (c ? c != p : (p != 0 && p != 0x8000)) ok = 0; }
      if (ok) { out[k] = (int32_t)o; found++; break; } } }
  return found; }
'''


class Helper:
    def __init__(self, tmp, gfx, prog):
        src = Path(tmp) / 'stages.c'
        src.write_text(HELPER)
        lib = Path(tmp) / 'stages.so'
        subprocess.run(['cc', '-O2', '-shared', '-fPIC', '-o', str(lib), str(src)], check=True, capture_output=True, timeout=60)
        self.lib = ctypes.CDLL(str(lib))
        self.gfx, self.prog = gfx, prog      # kept alive: the helper reads them in place
        c = ctypes
        self.lib.arc_init.argtypes = [c.c_char_p, c.c_long, c.c_char_p, c.c_long]
        self.lib.match_layer.argtypes = [c.c_char_p, c.c_long, c.c_char_p, c.c_long, c.c_long, c.c_int, c.c_int, c.c_int, c.c_void_p]
        self.lib.match_layer0.argtypes = [c.c_char_p, c.c_long, c.c_char_p, c.c_long, c.c_long, c.c_int, c.c_int, c.c_void_p]
        self.lib.quadrants.argtypes = [c.c_char_p, c.c_long, c.c_char_p, c.c_long, c.c_long, c.c_int, c.c_int, c.c_void_p]
        self.lib.match_tiles.argtypes = [c.c_char_p, c.c_long, c.c_void_p]
        self.lib.find_palettes.argtypes = [c.c_char_p, c.c_int, c.c_int, c.c_void_p]
        if self.lib.arc_init(gfx, len(gfx), prog, len(prog)):
            raise RuntimeError('stage helper could not start (program too short)')

    def match_layer(self, pages, module, off, cells_across, cells_down, side):
        bc = 16 if side == 16 else 8
        n = (cells_across // bc) * (cells_down // bc)
        out = (ctypes.c_int32 * (5 * n))()
        if self.lib.match_layer(pages, len(pages), module, len(module), off, cells_across, cells_down, side, out) != n:
            raise ValueError('layer')
        return [tuple(out[5 * i:5 * i + 5]) for i in range(n)]

    def match_layer0(self, pages, module, off, cells_across, cells_down):
        n = (cells_across // 16) * (cells_down // 16)
        out = (ctypes.c_int32 * (5 * n))()
        if self.lib.match_layer0(pages, len(pages), module, len(module), off, cells_across, cells_down, out) != n:
            raise ValueError('layer')
        return [tuple(out[5 * i:5 * i + 5]) for i in range(n)]

    def quadrants(self, pages, module, off, cells_across, cells_down):
        out = (ctypes.c_int32 * 2)()
        self.lib.quadrants(pages, len(pages), module, len(module), off, cells_across, cells_down, out)
        return out[0], out[1]

    def tiles(self, chunk):
        n = len(chunk) // 128
        out = (ctypes.c_int32 * max(n, 1))()
        found = self.lib.match_tiles(chunk, n, out)
        return n, found, list(out[:n])

    def palettes(self, chunk, brightness=BRIGHTNESS):
        n = len(chunk) // 32
        rows = sorted({chunk[32 * k:32 * k + 32] for k in range(n)})     # equal rows are searched once
        out = (ctypes.c_int32 * max(len(rows), 1))()
        self.lib.find_palettes(b''.join(rows), len(rows), brightness, out)
        at = {row: (None if out[i] < 0 else out[i]) for i, row in enumerate(rows)}
        return n, [at[chunk[32 * k:32 * k + 32]] for k in range(n)]


# ---------------------------------------------------------------- inputs (the same readers as fighters.py)

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


# ---------------------------------------------------------------- the analysis of one stage file

def analyse(parts, helper, stage, prog):
    """Counts for one stage file (a dict from split_stage) as a dict, or an error string."""
    module, pages = parts['module'], parts['pages']
    r = {'pages': len(pages) // PAGE_BYTES, 'layers': []}
    found = find_layers(module)
    if found is None:
        return 'layer maps not found in the module'
    layers, pointers = found
    extents = layer_extents(layers, pointers)
    for n, (off, across, down, side) in enumerate(extents):
        item = {'offset': off, 'cells': (across, down), 'side': side}
        if n == 0:
            item['pieces'] = helper.quadrants(pages, module, off, across, down)
        blocks = helper.match_layer0(pages, module, off, across, down) if n == 0 else helper.match_layer(pages, module, off, across, down, side)
        item['blocks'] = blocks
        r['layers'].append(item)
    for kind in ('object tiles', 'effect tiles'):
        r[kind] = tile_summary(helper.tiles(parts[kind])) if kind in parts else None
    for kind in ('palettes', 'shared palettes', 'object palettes'):
        count, offs = helper.palettes(parts[kind])
        r[kind] = (count, offs, sum(1 for k in range(count) if is_flat_palette(parts[kind], k)))
    used = rows_used(module, layers, extents)
    r['rows used'] = [len(u) for u in used]
    r['layer 0 rows'] = row_classes(prog, stage, parts['palettes'], 0, sorted(used[0]))
    r['rule'] = palette_agreement(prog, stage, parts['palettes'], rows_used(module, layers, extents))
    r['object rule'] = object_palette_agreement(prog, stage, parts['object palettes'])
    r['first_difference'] = first_palette_difference(prog, stage, parts['palettes'])
    return r


def block_cells_total(item, n):
    """Cells (pieces for layer 0) in one block of a layer's result."""
    return 1024 if n == 0 else 256 if item['side'] == 16 else 64


def layer_text(item, n):
    blocks = item['blocks']
    exact = sum(1 for b in blocks if b[2] == block_cells_total(item, n))
    nonflat, nonflat_ok = sum(b[0] for b in blocks), sum(b[4] for b in blocks)
    text = f'L{n} {item["cells"][0] * item["side"]}x{item["cells"][1] * item["side"]}'
    if n == 0:
        text += f' sets exact {exact}/{len(blocks)} 8x8 pieces {nonflat_ok}/{nonflat} (pixels equal in {sum(b[3] for b in blocks)} of {1024 * len(blocks)}, rows in {sum(b[2] for b in blocks)})'
    else:
        text += f' blocks {exact}/{len(blocks)} cells {nonflat_ok}/{nonflat}'
    return text


def tile_summary(result):
    """(tiles, found, found as a flipped copy) from Helper.tiles."""
    n, found, idx = result
    return n, found, sum(1 for v in idx if v >= 0 and v & 3)


def line(name, r):
    if isinstance(r, str):
        return f'{name}: {r}'
    parts = [name, f'pages {r["pages"]}'] + [layer_text(it, n) for n, it in enumerate(r['layers'])]
    for kind, label in (('object tiles', 'objtiles'), ('effect tiles', 'fxtiles')):
        parts.append(f'{label} {r[kind][1]}/{r[kind][0]} ({r[kind][2]} flipped)' if r[kind] else f'{label} none')
    for kind, label in (('palettes', 'palettes'), ('shared palettes', 'shared'), ('object palettes', 'objpal')):
        count, offs, flat = r[kind]
        parts.append(f'{label} {sum(1 for o in offs if o is not None)}/{count}' + (f' ({flat} flat)' if flat else ''))
    parts.append('palette rows used that equal the arcade L0 L1 L2 ' + ' '.join(f'{e}/{t}' for e, t in r['rule']) + ' | objpal rows equal %d/%d' % r['object rule'])
    return ' | '.join(parts)


def details(name, r):
    out = []
    if isinstance(r, str):
        return out
    for n, it in enumerate(r['layers']):
        ids = [b[1] for b in it['blocks']]
        total = block_cells_total(it, n)
        bad = [(i, b) for i, b in enumerate(it['blocks']) if b[2] != total]
        out.append(f'  {name} L{n} arcade {"scroll 1 sets" if n == 0 else "blocks"} {ids}' + (f'; first block that differs: #{bad[0][0]} {bad[0][1][2]} equal of {total}' if bad else ''))
    out.append(f'  {name} layer 0 palette rows used: equal / a few entries differ / whole row differs: {r["layer 0 rows"]}')
    out.append(f'  {name} first palette row that differs from the arcade table, per layer (row, entries): {r["first_difference"]}')
    return out


# ---------------------------------------------------------------- the model: what the loader builds from the arcade
#
# stage.c reproduces this section byte for byte (`--compare-c` checks it). A stage is described by a descriptor (a dict,
# printed as C by `--c-table`): where the three layer maps lie in the module, which arcade bytes name each layer's block
# sets, where the layers start in the pages, which page slots hold arcade tiles apart from the layers, the runs of arcade
# tiles of the object tiles, and which parts of the build were found equal to the disc (flags): those bytes are `known`.

LIST_REGION = ((0x1d000, 0x1d0c0), (0x1d0f8, 0x1d2a0), (0x1d2cc, 0x1d7cc))   # where the arcade lists the block ids of layers 0, 1, 2
LIST_TABLES = (0x1cff4, 0x1d0ec)           # u16 offsets a stage, from the table + 4 (layer 0) and + 0xc (layer 1)
LIST2_ROW = (0x1d2cc, 0x40, 14)            # layer 2: 16 rows of 4 ids a stage, 0x40 bytes apart; the stage's row is 14
SET_COUNT, BLOCK16_COUNT, BLOCK32_COUNT = 143, 154, 101
RUN_ZERO, RUN_DISC, RUN_BLANK = 0xfffffffe, 0xffffffff, 0xfffffffd   # a run of tiles: transparent ones, ones the arcade does not have (the disc's), or those blank by design

# Object tiles (table 0 slot 9) that no arcade tile makes, blank by the owner's rule ("rebuild arcade-style": blank what the arcade does
# not show). {file: (tiles, evidence)}; describe turns exactly that many tiles of RUN_DISC into RUN_BLANK and refuses a stage whose count
# differs, so nothing is blanked silently. The chunk is then built whole from the arcade and served to every read (owned).
BLANK_BY_DESIGN = {
    # STAGE04 (Chun-Li's stage): 937 of the chunk's 960 tiles, three runs (418, 360 and 159) of bicycle riders drawn with hats, wheels and
    # spokes. None is an arcade tile: not under the 4 flips, not under any relabelling of colour indexes, not a quarter of a 32 x 32 scroll 3
    # tile, and only 6 % of their 8 x 8 pieces occur at any offset inside an arcade 16 x 16 tile (measured 2026-10-24). The arcade's own
    # riders are the animation frames of its layer 2 (blocks 9, 25, 43, 67 and 68, built as the extra map of the stage), which show
    # (inferred) the same scene; these sprites are the PS1's own redrawn ones, so the arcade shows nothing in their place.
    'STAGE04.PAC': (937, 'bicycle riders, no arcade tile; the arcade has its riders in the layer 2 animation blocks'),
    # STAGE01: tile 275, one corner of a gradient (colours 11 to 14) that is no arcade tile under any flip or relabelling.
    'STAGE01.PAC': (1, 'one gradient corner tile, no arcade tile'),
}

MAP_EXACT, TILES_EXACT, START_RULE = 1, 2, 4   # flags of a layer: its map, its tiles in the pages, equal the disc's; its start is the packing rule's
FORCE_NEW = 0xffffffff                      # an exception slot: the cell takes a new slot though an equal tile exists
SCROLL3_FLIP, QUARTER_SHIFT = 8, 4          # flip bits of an extras run beyond the flips: its codes are 32 pixel tiles, a quarter (bits 4 and 5) each (sprite_tiles.py's rule)
XKEY = 1 << 20                              # cells of the extra maps of a layer have x = XKEY * (map number + 1) + x of the map, so keys never meet a layer cell's
MORE_ROW, MORE_FILL, MORE_APPROX, MORE_BLANK = 0, 1, 2, 3   # kinds of run of the 'more' palettes: arcade rows from an address, filler rows; by design (not the disc's,
                                            # `own`): the arcade row that stands for a row of the disc that differs from it, or a transparent row
APPROX_MAX_DIFF = 6                         # a 'more' palette of a stage's first chunk is the nearest arcade row when at most this many of its 15 entries differ, else blank
FILL_COLOUR = 0x322                         # the colour of a filler palette (all 15 entries), as an arcade word
OBJECT_PALETTES_BYTES = 224


class Arc:
    """The arcade program and graphics with a cache of the converted tiles."""

    def __init__(self, prog, gfx):
        self.prog, self.gfx = prog, gfx
        self._rows, self._tiles, self._sets, self._pieces, self._palettes = {}, {}, {}, {}, {}

    def palette(self, stage, layer, row):
        """The 16 PS1 colours of a layer's palette row of a stage (predicted_palette), cached. struct.error when the row lies outside."""
        key = (stage, layer, row)
        if key not in self._palettes:
            self._palettes[key] = predicted_palette(self.prog, stage, layer, row)
        return self._palettes[key]

    def rows(self, index, side):
        """side rows of side PS1 colour values (the arcade's plus one, mod 16) of graphics tile `index` (128 bytes for 16,
        512 for 32). A tile outside the graphics is transparent."""
        key = (index, side)
        if key not in self._rows:
            size = side * side // 2
            if index < 0 or (index + 1) * size > len(self.gfx):
                rows = [[0] * side for _ in range(side)]
            else:
                g, base, rows = self.gfx, index * size, []
                for y in range(side):
                    r = []
                    for x in range(side):
                        at, sh = base + y * side // 2 + x // 8 * 4, 7 - x % 8
                        v = ((g[at] >> sh) & 1) | (((g[at + 1] >> sh) & 1) << 1) | (((g[at + 2] >> sh) & 1) << 2) | (((g[at + 3] >> sh) & 1) << 3)
                        r.append((v + 1) & 15)
                    rows.append(r)
            self._rows[key] = rows
        return self._rows[key]

    def tile(self, index, side, flip_x=False, flip_y=False):
        """The PS1 bytes of a tile (rows of side / 2 bytes, the left pixel in the low nibble), flipped."""
        key = (index, side, bool(flip_x), bool(flip_y))
        if key not in self._tiles:
            rows = self.rows(index, side)
            if flip_y:
                rows = rows[::-1]
            if flip_x:
                rows = [r[::-1] for r in rows]
            self._tiles[key] = pack_rows(rows)
        return self._tiles[key]

    def quarter(self, index, q, flip_x=False, flip_y=False):
        """The PS1 bytes of quarter q (0 top left, 1 top right, 2 bottom left, 3 bottom right) of the 32 x 32 pixel graphics tile `index`
        (512 bytes), taken first and then flipped: a 16 x 16 tile (sprite_tiles.py's quarter_raw is the same rule on the bytes)."""
        key = (index, 'q', q, bool(flip_x), bool(flip_y))
        if key not in self._tiles:
            rows = [r[(q & 1) * 16:(q & 1) * 16 + 16] for r in self.rows(index, 32)[(q >> 1) * 16:(q >> 1) * 16 + 16]]
            if flip_y:
                rows = rows[::-1]
            if flip_x:
                rows = [r[::-1] for r in rows]
            self._tiles[key] = pack_rows(rows)
        return self._tiles[key]

    def has_tile(self, index, side=16):
        """True when graphics tile `index` lies inside the graphics."""
        size = side * side // 2
        return 0 <= index and (index + 1) * size <= len(self.gfx)

    def scroll1_set(self, number):
        if number not in self._sets:
            self._sets[number] = scroll1_set_cells(self.prog, number)
        return self._sets[number]

    def piece(self, code, attr):
        """8 rows of 8 PS1 values of a scroll 1 cell: the left half of the top (even code) or bottom eight rows of its 16x16 tile, flipped."""
        key = (code, attr & 0x60)
        if key not in self._pieces:
            index, first = scroll1_piece(code)
            rows = [r[:8] for r in self.rows(index, 16)[first:first + 8]]
            if attr & 0x40:
                rows = rows[::-1]
            if attr & 0x20:
                rows = [r[::-1] for r in rows]
            self._pieces[key] = rows
        return self._pieces[key]


def pack_rows(rows):
    """Bytes of rows of PS1 values: two to a byte, the left one low."""
    return bytes(r[x] | r[x + 1] << 4 for r in rows for x in range(0, len(r), 2))


def slot_code(slot, side):
    """The tile code (page, u, v) of a page slot: 16 pixel tiles 16 to a column and 16 columns to a page (column by column),
    32 pixel tiles 8 and 8."""
    page, rest = divmod(slot, 256 if side == 16 else 64)
    if page + FIRST_PAGE > 63:
        raise ValueError(f'slot {slot} is beyond the tile codes')
    if side == 16:
        return ((page + FIRST_PAGE) << 10) | (((rest % 16) * 2) << 5) | ((rest // 16) * 2)
    return ((page + FIRST_PAGE) << 10) | (((rest % 8) * 4) << 5) | ((rest // 8) * 4)


def slot_of_code(code, side):
    """The page slot of a tile code (None for a code that is not on the stage's pages or not on the slot grid)."""
    page, u, v = (code >> 10) - FIRST_PAGE, (code & 31) * 8, ((code >> 5) & 31) * 8
    if page < 0 or u % side or v % side:
        return None
    return page * (256 if side == 16 else 64) + (u // side) * (16 if side == 16 else 8) + v // side


def slot_place(slot, side):
    """The byte offset in the pages of the first row of a page slot's tile (rows are 128 bytes apart)."""
    per_page, columns = (256, 16) if side == 16 else (64, 8)
    page, rest = divmod(slot, per_page)
    col, row = rest // columns, rest % columns
    return page * PAGE_BYTES + row * side * 128 + col * side // 2


def put_slot(pages, slot, side, tile):
    """Write a tile's bytes (side rows of side / 2 bytes) at a page slot; False when it lies outside the pages."""
    if not slot_ok(pages, slot, side):
        return False
    at = slot_place(slot, side)
    per = side // 2
    for y in range(side):
        pages[at + y * 128:at + y * 128 + per] = tile[y * per:(y + 1) * per]
    return True


def get_slot(pages, slot, side):
    at, per = slot_place(slot, side), side // 2
    return b''.join(bytes(pages[at + y * 128:at + y * 128 + per]) for y in range(side))


def l0_cell(arc, stage, quad):
    """(tile bytes, rows, needs) of a layer 0 cell from its four scroll 1 cells [(code, attribute)] in the order top left, top right,
    bottom left, bottom right. rows: the distinct palette rows (attribute bits 0-4) of the pieces that have an opaque pixel, in piece
    order. needs: {colour index: PS1 colour} that the opaque pixels ask of the stage's layer 0 palette, or None when two pieces ask for
    different colours at one index (then the cell cannot use any one row without a loss)."""
    pcs = [arc.piece(c, a) for c, a in quad]
    rows_, needs, conflict = [], {}, False
    for pc, (_c, a) in zip(pcs, quad):
        used = {v for r in pc for v in r if v}
        if not used:
            continue
        row = a & 31
        if row not in rows_:
            rows_.append(row)
        pal = arc.palette(stage, 0, row)
        for j in used:
            if needs.setdefault(j, pal[j]) != pal[j]:
                conflict = True
    rows8 = [r0 + r1 for r0, r1 in zip(pcs[0], pcs[1])] + [r0 + r1 for r0, r1 in zip(pcs[2], pcs[3])]
    return pack_rows(rows8), rows_, None if conflict else needs


def l0_order(infos):
    """The order in which layer 0 tries the stage's palette rows: the rows by the number of cells that have a piece in them, most first,
    the lower row number first among equals. infos: [(rows, needs)] of all the layer's cells."""
    count = {}
    for rows_, _needs in infos:
        for r in rows_:
            count[r] = count.get(r, 0) + 1
    return sorted(count, key=lambda r: (-count[r], r))


def l0_row(rows_, needs, order, palettes):
    """The palette row of a layer 0 cell: the first row of `order` whose colours are the ones the cell's pixels ask for (the row the PS1
    gives to the most cells that fit it); a cell whose pieces disagree, or that no row fits, takes the row of its first piece with an
    opaque pixel; a blank cell row 0. palettes[row] is predicted_palette of the row."""
    if not rows_:
        return 0
    if needs is not None:
        for x in order:
            pal = palettes[x]
            if all(pal[j] == c for j, c in needs.items()):
                return x
    return rows_[0]


def layer_cells(arc, n, ids, across, down, stage=0):
    """[(cx, cy, tile bytes, palette row)] of layer n in the PS1's scan order (blocks row by row, the cells of a block row by row),
    from the arcade block ids: layer 0 sets of 32 x 32 scroll 1 cells (2 x 2 to a PS1 cell, rows by l0_row from the stage's layer 0
    table), layer 1 blocks of 16 x 16 cells, layer 2 blocks of 8 x 8 cells of 32 pixels. A block that does not exist raises ValueError."""
    side = (16, 16, 32)[n]
    bc = 16 if n < 2 else 8
    if not across or not down or across % bc or down % bc or len(ids) != (across // bc) * (down // bc):
        raise ValueError(f'layer {n} has {len(ids)} block ids for {across} x {down} cells')
    blocks_across = across // bc
    out, infos = [], []
    for by in range(down // bc):
        for bx in range(blocks_across):
            b = ids[by * blocks_across + bx]
            if b >= (SET_COUNT, BLOCK16_COUNT, BLOCK32_COUNT)[n]:
                raise ValueError(f'layer {n} block {b} does not exist')
            if n == 0:
                cs = arc.scroll1_set(b)
            for y in range(bc):
                for x in range(bc):
                    if n == 0:
                        tile, rows_, needs = l0_cell(arc, stage, [cs[(2 * x + q % 2, 2 * y + q // 2)] for q in range(4)])
                        out.append((bx * 16 + x, by * 16 + y, tile, None))
                        infos.append((rows_, needs))
                    else:
                        base = (TABLE16 + b * 1024, TABLE32 + b * 256)[n - 1]
                        code, attr = struct.unpack_from('>HH', arc.prog, base + (x * bc + y) * 4)
                        index, fx, fy, row = arcade_cell_tile(code, attr, side)
                        out.append((bx * bc + x, by * bc + y, arc.tile(index, side, fx, fy), row))
    if n == 0:
        order = l0_order(infos)
        palettes = [arc.palette(stage, 0, r) for r in range(32)]
        out = [(cx, cy, tile, l0_row(rows_, needs, order, palettes)) for (cx, cy, tile, _r), (rows_, needs) in zip(out, infos)]
    return out


def xmap_across(side):
    """The width in cells that the page layout of an extra map is addressed with (map_word_index): two blocks side by side a page."""
    return 32 if side == 16 else 16


def xmap_cells(arc, n, xi, ids):
    """[(x, y, tile bytes, palette row)] of extra map `xi` of layer n (1 or 2), in the order the PS1's packing scans them: the
    map is a stack of pages of two blocks side by side (block h is page h // 2, the left or right half), and a block's cells row
    by row. x carries XKEY * (xi + 1); y counts down the stack. The blocks are the arcade's, the same as the layer's own cells
    (arcade animation frames of the layer: the PS1 keeps them as further maps the stage module switches to)."""
    if n not in (1, 2):
        raise ValueError(f'layer {n} has no extra maps')
    side, bc = (16, 16, 32)[n], (16, 16, 8)[n]
    table, stride, nblocks = ((TABLE16, 1024, BLOCK16_COUNT), (TABLE32, 256, BLOCK32_COUNT))[n - 1]
    out = []
    for h, b in enumerate(ids):
        if b >= nblocks:
            raise ValueError(f'layer {n} extra block {b} does not exist')
        page, half = divmod(h, 2)
        for y in range(bc):
            for x in range(bc):
                code, attr = struct.unpack_from('>HH', arc.prog, table + b * stride + (x * bc + y) * 4)
                index, fx, fy, row = arcade_cell_tile(code, attr, side)
                out.append((XKEY * (xi + 1) + half * bc + x, page * bc + y, arc.tile(index, side, fx, fy), row))
    return out


# ---------------------------------------------------------------- our own layer 0 (E2, E3)
#
# The PS1 draws layer 0 as 16 x 16 sprites, one palette word (a row of the layer's palette column) a cell; the arcade's cells are 8 x 8 pieces
# with a row each. Where the PS1's tool merged the rows of a cell's pieces it changed tiles and palette rows (which stages.py cannot rebuild:
# l0_row), and the result differs from what the arcade draws. With the layer's flag L0_OWN the loader builds the layer's map cells, its
# tiles (in the pages' first slots) and its 32 palette rows itself, by this rule, and serves them owned (a pack keeps none):
#   a blank cell is the blank tile (slot 0, code 0x5800, which the drawing code skips);
#   a cell whose pieces share one arcade row keeps that row and the arcade's indexes (the tile is the arcade's);
#   a cell with pieces in several rows (the `colours` set C of its opaque pixels) takes, in the order of these steps:
#     1. the first row (the rows of its own pieces in piece order, then rows 0 to 31) that holds all of C: its pixels are re-indexed to that
#        row (an unused arcade row taken this way is claimed: no merged row is made on it);
#     2. else, if C has at most 15 colours, a merged row: the existing merged row with room (at most 15 colours with C) that shares most colours
#        with C (the lowest row among equals), else a new one on the lowest unclaimed row that no piece of the stage uses; the colours are
#        added in ascending value, and kept in place, so tiles made earlier stay valid;
#     3. else the nearest colour: the row (0 to 31, arcade, claimed or merged, that has a colour) whose nearest colours, pixel by pixel, differ
#        least from the cell's (the sum of the channel differences of 5 bits; a transparent pixel against an opaque one counts 60); the lowest
#        row among equals;
#   when the different tiles are more than the slots the layer has (the pages' first slots up to layer 1's start, less those a list of the
#   module points at), the least used tile (fewest cells, then fewest opaque pixels, then the latest in first use) is folded into the tile
#   (another or the blank) that its cells draw nearest to the arcade's, until they fit.
# The row numbers of the arcade's pieces are kept (the stage's code and lists name rows of layer 0 by their arcade numbers); only rows no
# piece uses become merged rows.

L0_OWN = 8                               # flag of layer 0: built by the rule above


def l0_cdist(a, b):
    """Difference of two PS1 colours (0: transparent) as the sum of the differences of their 5-bit channels; transparent against opaque is 60."""
    if not a and not b:
        return 0
    if not a or not b:
        return 60
    return abs((a & 31) - (b & 31)) + abs(((a >> 5) & 31) - ((b >> 5) & 31)) + abs(((a >> 10) & 31) - ((b >> 10) & 31))


def l0_scan(arc, d):
    """[((cx, cy), quad)] of layer 0's cells in scan order: its own blocks (sets of 32 x 32 scroll 1 cells, row by row of blocks, then of cells) then
    those of its extra maps (layer['xmaps']: sets, two to a page of 32 x 16 cells, the keys carrying XKEY * (map number + 1))."""
    layer = d['layers'][0]
    ids = ids_of(arc, layer)
    out = []
    across = layer['across'] // 16
    for by in range(layer['down'] // 16):
        for bx in range(across):
            b = ids[by * across + bx]
            if b >= SET_COUNT:
                raise ValueError(f'layer 0 block {b} does not exist')
            cs = arc.scroll1_set(b)
            for y in range(16):
                for x in range(16):
                    out.append(((bx * 16 + x, by * 16 + y), [cs[(2 * x + q % 2, 2 * y + q // 2)] for q in range(4)]))
    for xi, xm in enumerate(layer.get('xmaps', ())):
        for h, b in enumerate(xm['ids']):
            if b >= SET_COUNT:
                raise ValueError(f'layer 0 extra block {b} does not exist')
            cs = arc.scroll1_set(b)
            page, half = divmod(h, 2)
            for y in range(16):
                for x in range(16):
                    out.append(((XKEY * (xi + 1) + half * 16 + x, page * 16 + y), [cs[(2 * x + q % 2, 2 * y + q // 2)] for q in range(4)]))
    return out


def l0_pixels(arc, stage, quad):
    """(colours of the cell's 256 pixels (0 transparent), the cell's rows in piece order): each piece's pixels in its own arcade row."""
    px, rows_ = [0] * 256, []
    for q, (code, attr) in enumerate(quad):
        pc, row = arc.piece(code, attr), attr & 31
        pal = arc.palette(stage, 0, row)
        used = False
        for yy in range(8):
            for xx in range(8):
                v = pc[yy][xx]
                if v:
                    px[((q >> 1) * 8 + yy) * 16 + (q & 1) * 8 + xx] = pal[v]
                    used = True
        if used and row not in rows_:
            rows_.append(row)
    return px, rows_


def l0_tile(px, colours):
    """The tile bytes of 256 colours against a row's colours (entries 1 to 15 in `colours`: the first entry with the colour; 0 transparent), or None
    when a colour is not there."""
    index = {}
    for i, c in enumerate(colours):
        index.setdefault(c, i + 1)
    out = bytearray(128)
    for i, c in enumerate(px):
        if c:
            v = index.get(c)
            if v is None:
                return None
            out[i >> 1] |= v << (4 * (i & 1))
    return bytes(out)


def l0_near(px, colours):
    """(error, tile bytes) of 256 colours drawn with the nearest colour of a row's colours (the lowest entry among equals)."""
    out, err = bytearray(128), 0
    for i, c in enumerate(px):
        if c:
            best = None
            for k, e in enumerate(colours):
                dist = l0_cdist(c, e)
                if best is None or dist < best[0]:
                    best = (dist, k + 1)
            err += best[0]
            out[i >> 1] |= best[1] << (4 * (i & 1))
    return err, bytes(out)


def l0_draw(tile, colours):
    """The 256 colours a tile draws in a row (0: transparent, or an entry the row does not have)."""
    return [(colours[v - 1] if 0 < v <= len(colours) else 0) for v in (((tile[i >> 1] >> (4 * (i & 1))) & 15) for i in range(256))]


def l0_own_build(arc, d):
    """What the loader builds of layer 0 by the rule above: a dict with
       words {key: (code, clut)} of the cells that are built (not those in layer['l0own']['skip']),
       tiles {slot: 128 bytes} of the slots used, zero_slots [slots made blank], rows {row: 16 PS1 colours} of the 32 palette rows,
       stats {tier: cells} and the counts folded. layer['l0own'] = {'skip': [(first, count)] of cell scan indexes, 'reserved': [(slot, count)]
       of the slots of the region [0, layer 1's start) that stay the disc's}. ValueError when the layer's tiles cannot fit or an input is bad."""
    stage, layer = d['stage'], d['layers'][0]
    own = layer['l0own']
    region = d['layers'][1]['start']
    skip = {i for a, c in own['skip'] for i in range(a, a + c)}
    reserved = {s for a, c in own['reserved'] for s in range(a, a + c)}
    if 0 in reserved or region > 0xffff:
        raise ValueError('layer 0 has no slot 0 for its blank tile')
    free_slots = [s for s in range(region) if s not in reserved]
    scan_all = [(i, x) for i, x in enumerate(l0_scan(arc, d)) if i not in skip]
    scan = [x for _i, x in scan_all]
    cells = []                                   # (key, px, rows_, arcade tile bytes of a single-row cell)
    used = set()
    for key, quad in scan:
        px, rows_ = l0_pixels(arc, stage, quad)
        cells.append((key, px, rows_, quad))
        used.update(rows_)
    arcade = [list(arc.palette(stage, 0, r)[1:]) for r in range(32)]
    mixed, claimed, stats = {}, set(used), collections.Counter()
    def colours(r):
        return mixed[r] if r in mixed else arcade[r]
    chosen = []                                  # (row, tile bytes) per cell
    for key, px, rows_, quad in cells:
        C = {c for c in px if c}
        if not C:
            chosen.append((0, bytes(128)))
            stats['blank'] += 1
            continue
        if len(rows_) == 1:
            chosen.append((rows_[0], l0_tile(px, colours(rows_[0]))))
            stats['one row'] += 1
            continue
        found = None
        for r in rows_ + [r for r in range(32) if r not in rows_]:
            cols = colours(r)
            if cols and C <= set(cols):
                found = r
                break
        if found is not None:
            claimed.add(found)
            chosen.append((found, l0_tile(px, colours(found))))
            stats['re-indexed'] += 1
            continue
        if len(C) <= 15:
            best = None
            for r in sorted(mixed):
                if len(set(mixed[r]) | C) <= 15:
                    score = len(C & set(mixed[r]))
                    if best is None or score > best[0]:
                        best = (score, r)
            if best is None:
                fresh = [r for r in range(32) if r not in claimed]
                if fresh:
                    mixed[fresh[0]] = []
                    claimed.add(fresh[0])
                    best = (0, fresh[0])
            if best is not None:
                r = best[1]
                mixed[r] += sorted(C - set(mixed[r]))
                chosen.append((r, l0_tile(px, mixed[r])))
                stats['merged'] += 1
                continue
        best = None
        for r in range(32):
            cols = colours(r)
            if not cols:
                continue
            err, t = l0_near(px, cols)
            if best is None or err < best[0]:
                best = (err, r, t)
        claimed.add(best[1])
        chosen.append((best[1], best[2]))
        stats['nearest'] += 1
        stats['nearest error'] += best[0]
    # the different tiles, then folding to the room the region has
    room = len(free_slots)
    order, cellsof = [], collections.defaultdict(list)
    for i, (row, t) in enumerate(chosen):
        if t not in cellsof:
            order.append(t)
        cellsof[t].append(i)
    if bytes(128) not in cellsof:
        order.insert(0, bytes(128))
    elif order[0] != bytes(128):
        order.remove(bytes(128))
        order.insert(0, bytes(128))
    folded = 0
    while len(order) > room:
        if room < 1 or len(order) < 2:
            raise ValueError('layer 0 has no room for its tiles')
        def weight(t):
            return (len(cellsof.get(t, ())), sum(1 for i in range(256) if (t[i >> 1] >> (4 * (i & 1))) & 15), -order.index(t))
        victims = [t for t in order[1:]] or []
        a = min(victims, key=weight)
        best = None
        for bi, b in enumerate(order):
            if b == a:
                continue
            err = 0
            for i in cellsof[a]:
                row = chosen[i][0]
                err += sum(l0_cdist(x, y) for x, y in zip(cells[i][1], l0_draw(b, colours(row))))
            if best is None or err < best[0]:
                best = (err, b)
        stats['folding error'] += best[0]
        b = best[1]
        for i in cellsof[a]:
            chosen[i] = (chosen[i][0], b)
        cellsof[b] = cellsof.get(b, []) + cellsof.pop(a)
        order.remove(a)
        folded += 1
    slot_of = {t: free_slots[k] for k, t in enumerate(order)}
    words = {}
    for (key, _px, _r, _q), (row, t) in zip(cells, chosen):
        slot = slot_of[t]
        words[key] = (slot_code(slot, 16), 0x7800 + 64 * (0 if slot == 0 else row))
    rows = {}
    for r in range(32):
        if r in mixed:
            rows[r] = [0] + mixed[r] + [0] * (15 - len(mixed[r]))
        else:
            rows[r] = predicted_palette(arc.prog, stage, 0, r)
    stats['folded'] = folded
    stats['merged rows'] = len(mixed)
    return {'words': words, 'tiles': {slot_of[t]: t for t in order}, 'zero_slots': free_slots[len(order):], 'rows': rows, 'stats': dict(stats),
            'index': {key: i for (i, _x), (key, _q) in zip(scan_all, scan)}}


def l0_skip_cells(d):
    """[(first, count)] of the scan indexes of layer 0's own cells whose four bytes lie in another layer's map (a stage whose layer 0 map is
    longer than the room before layer 1's: those bytes are layer 1's, built with it)."""
    layer = d['layers'][0]
    ranges = []
    for n in (1, 2):
        other, side = d['layers'][n], (16, 16, 32)[n]
        ranges.append((other['map'], other['map'] + 4 * other['across'] * other['down']))
    bad = []
    across = layer['across'] // 16
    for i in range((layer['across'] // 16) * (layer['down'] // 16) * 256):
        blk, k = divmod(i, 256)
        bx, by = blk % across, blk // across
        y, x = divmod(k, 16)
        at = layer['map'] + 2 * map_word_index(bx * 16 + x, by * 16 + y, layer['across'], 16)
        if any(a < at + 4 and at < b for a, b in ranges):
            bad.append(i)
    return index_spans(bad)


def l0_list_slots(parts, d):
    """[(slot, count)] of the page slots before layer 1's start that cells in the stage module outside the layers' maps point at (lists of the
    module's own: they name these slots by code and keep doing so): the layer 0 packing leaves them alone."""
    module, pages = parts['module'], parts['pages']
    npages = len(pages) // PAGE_BYTES
    taken = bytearray(len(module))
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        for cy in range(layer['down']):
            for cx in range(layer['across']):
                at = layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side)
                taken[at:at + 4] = b'\1\1\1\1'
        for xm in layer.get('xmaps', ()):
            pages_n = len(xm['ids']) // 2
            for page in range(pages_n):
                for y in range(16 if n < 2 else 8):
                    for x in range(32 if n < 2 else 16):
                        at = xm['map'] + 2 * map_word_index(x, page * (16 if n < 2 else 8) + y, xmap_across(side), side)
                        taken[at:at + 4] = b'\1\1\1\1'
    region = d['layers'][1]['start']
    slots = set()
    for at in range(0, len(module) - 3, 4):
        if taken[at] or not cell_like_any(module, at, npages):
            continue
        slot = slot_of_code(struct.unpack_from('<H', module, at)[0], 16)
        if slot is not None and slot < region:
            slots.add(slot)
    return index_spans(sorted(slots))


def cell_like_any(module, at, npages):
    """True when the four bytes at `at` look like a cell of one of the layers (a tile code on the stage's pages, a palette word 0x7800 + column + 64 * row, column 0 to 3)."""
    code, clut = struct.unpack_from('<HH', module, at)
    return 0x7800 <= clut < 0x8000 and (clut & 63) < 4 and FIRST_PAGE <= code >> 10 < FIRST_PAGE + npages


def l0_result(arc, d):
    """l0_own_build for a descriptor, kept in the descriptor (it takes a moment on a stage with many cells)."""
    if '_l0' not in d:
        d['_l0'] = l0_own_build(arc, d)
    return d['_l0']


def layer_cells_all(arc, d, n):
    """The cells of layer n of descriptor d in scan order: its own (layer_cells), then those of its extra maps (layer['xmaps'],
    each {'map': byte offset in the module, 'ids': arcade block numbers, two to a page})."""
    layer = d['layers'][n]
    cells = layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], d['stage'])
    for xi, xm in enumerate(layer.get('xmaps', ())):
        cells += xmap_cells(arc, n, xi, xm['ids'])
    return cells


def layer_cell_total(layer, n):
    """Cells of a layer counting its extra maps (the domain of its cell_known and tile_known bits)."""
    bc = (16, 16, 8)[n]
    return layer['across'] * layer['down'] + sum(len(xm['ids']) * bc * bc for xm in layer.get('xmaps', ()))


def cell_offset(layer, n, cx, cy):
    """The byte offset in the module of the cell with key (cx, cy) of layer n (a layer cell or one of its extra maps')."""
    side = (16, 16, 32)[n]
    if cx < XKEY:
        return layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side)
    xi = cx // XKEY - 1
    return layer['xmaps'][xi]['map'] + 2 * map_word_index(cx - XKEY * (xi + 1), cy, xmap_across(side), side)


def place_layer(cells, n, start, exc=None):
    """({(cx, cy): (tile code, palette word)}, [tile bytes]) of a layer's cells whose tiles take page slots from `start`: the blank
    tile first, then each different tile in the order the cells first use it. A blank cell has palette row 0.
    exc: {cell index: slot offset} where the disc differs from that rule. The cell takes slot start + offset: a slot of a tile already
    made (a smaller offset than the number of slots so far) is only shared; a larger offset is a new slot for this tile, the slots
    between left without a tile (None in the list: the PS1's own); FORCE_NEW takes the next slot though an equal tile exists."""
    exc = exc or {}
    side = (16, 16, 32)[n]
    blank = bytes(side * side // 2)
    index, tiles, words = {blank: 0}, [blank], {}
    for i, (cx, cy, tile, row) in enumerate(cells):
        e = exc.get(i)
        if e is None:
            j = index.get(tile)
            if j is None:
                j = index[tile] = len(tiles)
                tiles.append(tile)
        elif e == FORCE_NEW:
            j = len(tiles)
            tiles.append(tile)
        elif e < len(tiles):
            j = e
        else:
            if e > 0xffff:
                raise ValueError(f'layer {n} cell {i} is given slot offset {e}, beyond the pages')
            j = e
            tiles.extend([None] * (e - len(tiles)))
            tiles.append(tile)
            index[tile] = e
        words[(cx, cy)] = (slot_code(start + j, side), 0x7800 + n + 64 * (0 if j == 0 else row))
    return words, tiles


def ids_of(arc, layer):
    """The block ids of a descriptor's layer: the arcade program's bytes at its addresses (address 0 is a constant 0)."""
    if any(a >= len(arc.prog) for a in layer['ids']):
        raise ValueError('an id address lies outside the arcade program')
    return [arc.prog[a] if a else 0 for a in layer['ids']]


def tile_run_bytes(arc, runs, total):
    """(bytes, known) of a list of tile runs (first, count, flip) as the PS1's tile list of `total` bytes. A run of transparent tiles
    is zeros (known), a run of RUN_DISC or RUN_BLANK tiles is zeros (not known); a run of arcade tiles is those tiles, flipped."""
    out, known, at = bytearray(total), bytearray(total), 0
    for first, count, flip in runs:
        if at + 128 * count > total:
            raise ValueError('tile runs longer than the chunk')
        if first in (RUN_DISC, RUN_BLANK):
            at += 128 * count
            continue
        for k in range(count):
            if first != RUN_ZERO:
                if not arc.has_tile(first + k):
                    raise ValueError(f'tile {first + k} lies outside the arcade graphics')
                out[at:at + 128] = arc.tile(first + k, 16, flip & 1, flip & 2)
            known[at:at + 128] = b'\1' * 128
            at += 128
    if at != total:
        raise ValueError('tile runs shorter than the chunk')
    return out, known


def runs_owned(runs):
    """True when a tile run list is built whole from the arcade (stage.c sets `owned`): some run is blank by design and none is the disc's."""
    return any(f == RUN_BLANK for f, _c, _fl in runs) and not any(f == RUN_DISC for f, _c, _fl in runs)


# Page slots that no arcade tile makes (16 x 16 nor a quarter of a 32 x 32 pixel one) and that are blank by the same rule, as the object
# tiles above. {file: (slots, evidence)}; describe refuses a file whose holes (slots with art no arcade tile makes, outside the layers) do not
# add up to the count, and the table text says so. The slots stay zero in the build and unknown (`holes`) until the pages chunk is served whole.
# Stage 06's holes are not in this table: they are layer 0 tiles of its extra sets, which the own layer 0 packing makes.
HOLES_BLANK_BY_DESIGN = {
    # STAGE0D: slots 207 to 222, one column of the first page: four frames of four tiles (a figure at a window, in all 15 colours); no
    # arcade tile at 16 or 32 pixels (measured 2026-10-24: none under 4 flips, none as a quarter of a 32 x 32 tile). With layer 0 built by the own
    # rule the four slots 130, 131, 140 and 141 that the same list names (layer 0 tiles the PS1 merged) are held too: 20 slots in all.
    'STAGE0D.PAC': (20, 'four 4-tile frames of a figure at a window (slots 207 to 222) and the four layer 0 slots (130, 131, 140, 141) its list names, no arcade tile'),
    # STAGE03: slots 749 to 752, four tiles of the strip the PS1 added to layer 1 (DESIGN_BY_DESIGN) that no arcade tile makes (the other 184 of the
    # strip's 188 distinct tiles are arcade tiles); the strip itself is built blank.
    'STAGE03.PAC': (4, 'four tiles of the road strip the PS1 added to layer 1, no arcade tile'),
}


def holes_by_design(name, holes):
    """None when the holes (runs (slot, count)) are as HOLES_BLANK_BY_DESIGN says for the file (no entry: any holes are accepted, as the
    ones of stage 06), else the refusal's text."""
    if name not in HOLES_BLANK_BY_DESIGN:
        return None
    have, want = sum(c for _s, c in holes), HOLES_BLANK_BY_DESIGN[name][0]
    return None if have == want else f'{name}: {have} page slots have no arcade source, the blank-by-design table says {want}'


# Cells and tiles of layers 1 and 2 where the PS1 differs from the arcade and the arcade's version is built (served owned, not equal to the
# disc's), by the owner's rule ("rebuild arcade-style"). {(file, layer): (blank cells, row cells, retouched tiles, evidence)}; describe
# finds them (see design_of) and refuses a layer whose counts differ from the table's, so none is taken unseen:
#   blank cells: cells where the arcade's tile is blank and the PS1's is art (the PS1 filled a blank part of the layer): built blank;
#   row cells: cells at the arcade's tile (the same slot) but in another palette row: built with the arcade's row;
#   retouched tiles: tiles of cells that equal the disc's where the PS1's pixels differ from the arcade's tile: built as the arcade's tile.
DESIGN_BY_DESIGN = {
    # STAGE03 layer 1: the arcade's blocks leave rows 20 to 31 of x 16 to 63 blank; the PS1 fills rows 28 to 31 there (192 cells) with
    # cells copied from 16 rows above (159 of the 192 are the same word as the cell 16 rows up; the others point at tiles of their own, 184 of
    # the 188 distinct tiles being arcade tiles). The arcade shows nothing there. Measured 2026-10-24.
    ('STAGE03.PAC', 1): (192, 0, 0, 'the PS1 filled the blank bottom rows of layer 1 with copies of the rows 16 above'),
    # STAGE03 layer 2: six 32 x 32 pixel tiles (cells 14 and 15 of rows 0 to 2) where the PS1's pixels differ from the arcade's in 4 to
    # 663 of 1,024 pixels: retouched by the PS1's art tool.
    ('STAGE03.PAC', 2): (0, 0, 6, 'six tiles of the top of layer 2 retouched'),
    # STAGE10 layer 1: seventeen tiles of cells 16 to 63 of rows 6 to 11 (the fence, a sign and the foreground fist) where 2 to 9 of 256 pixels
    # differ from the arcade's (retouched), and three cells whose tile is the arcade's but that use another palette row (16, not 8).
    ('STAGE10.PAC', 1): (0, 3, 17, 'seventeen tiles retouched by a few pixels, three cells in another row'),
}


def design_of(arc, d, module, pages, n, start, tiles, words):
    """(blank cells, row cells, retouched tiles) of layer n of descriptor d, each a list of indices (own cells in scan order; tiles of the
    layer), found as DESIGN_BY_DESIGN says, from the model's `words` ({(cx, cy): (code, clut)}), `tiles` and `start` against the disc's
    `module` and `pages`. Only the layer's own cells count, not its extra maps'."""
    layer, side = d['layers'][n], (16, 16, 32)[n]
    blank, row, retouched = [], [], []
    for i, (cx, cy, tile, _r) in enumerate(layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], d['stage'])):
        have = struct.unpack_from('<HH', module, layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side))
        want = words[(cx, cy)]
        if have == want:
            continue
        slot = slot_of_code(have[0], side)
        ps1 = get_slot(pages, slot, side) if slot is not None and slot_ok(pages, slot, side) else None
        if ps1 is None:
            continue
        if not any(tile) and any(ps1):
            blank.append(i)
        elif have[0] == want[0] and (have[1] - 0x7800) >> 6 != (want[1] - 0x7800) >> 6:
            row.append(i)
    for j, t in enumerate(tiles):
        if t is not None and slot_ok(pages, start + j, side):
            ps1 = get_slot(pages, start + j, side)
            if ps1 != t and any(ps1) and any(t):
                retouched.append(j)
    return blank, row, retouched


def blank_by_design(name, runs):
    """The runs with the RUN_DISC ones made RUN_BLANK, as BLANK_BY_DESIGN says for the file; the string of a refusal when the count of tiles
    the arcade does not have differs from the table's (a table entry for a file with none, or none for a file with some, is no match
    either), so a stage that gains or loses own tiles is never blanked unseen."""
    have = sum(c for f, c, _fl in runs if f == RUN_DISC)
    want = BLANK_BY_DESIGN.get(name, (0, ''))[0]
    if have != want:
        return f'{name}: {have} object tiles have no arcade source, the blank-by-design table says {want}'
    return [(RUN_BLANK if f == RUN_DISC else f, c, fl) for f, c, fl in runs]


def model_more(prog, out, known, count, more, own=None):
    """Write into `out` the runs (k, step, count, kind, arg) of palettes: arcade rows from address arg on (0x20 apart, MORE_ROW) or filler
    rows (MORE_FILL), marked in `known`; or by design, marked in `own` instead: MORE_APPROX arcade rows that stand for rows of the disc that
    differ, MORE_BLANK transparent rows. A run that leaves the chunk of `count` palettes raises ValueError."""
    for k0, step, n, kind, arg in more:
        if kind not in (MORE_ROW, MORE_FILL, MORE_APPROX, MORE_BLANK):
            raise ValueError('a palette run has an unknown kind')
        for i in range(n):
            k = k0 + step * i
            if k >= count:
                raise ValueError('a palette run leaves the chunk')
            if kind == MORE_BLANK:
                out[32 * k:32 * k + 32] = bytes(32)
                own[32 * k:32 * k + 32] = b'\1' * 32
                continue
            if kind == MORE_FILL:
                words = [FILL_COLOUR] * 15
            else:
                if arg + 0x20 * i + 32 > len(prog):
                    raise ValueError('a palette row lies outside the arcade program')
                words = struct.unpack_from('>15H', prog, arg + 0x20 * i)
            struct.pack_into('<16H', out, 32 * k, 0, *[arcade_color_to_ps1(w) or 0x8000 for w in words])
            (own if kind == MORE_APPROX else known)[32 * k:32 * k + 32] = b'\1' * 32


def model_shared_palettes(prog, size, more, own=None):
    """(bytes, known) of the second palette chunk of a stage file (the same in every stage file): `size` bytes of palettes that are
    all the runs `more`; the others are zero and not known. `own` (a bytearray of `size`) gets the palettes built by design."""
    out, known = bytearray(size), bytearray(size)
    own = own if own is not None else bytearray(size)
    model_more(prog, out, known, size // 32, more, own)
    own[0:4] = bytes(4)                                   # the chunk's first word stays the disc's (stage.c first_words_left_alone)
    return out, known


def model_palettes(prog, stage, count, exact, more=(), own=None):
    """(bytes, known) of the layer palettes of a stage's palette chunk (count palettes of 16 words): the rows of the three layer
    tables where palette_group names them, converted. Palette k is known when bit k of `exact` is set. more: runs (k, step, count,
    kind, arg) of the other palettes: palettes k, k + step, ... are arcade rows from address arg on (0x20 apart, MORE_ROW) or filler
    rows (MORE_FILL); they are known. A run that leaves the chunk raises ValueError."""
    out, known = bytearray(count * 32), bytearray(count * 32)
    for k in range(min(count, 128)):
        layer, row = palette_group(k, count)
        if layer == 'more':
            continue
        struct.pack_into('<16H', out, 32 * k, *predicted_palette(prog, stage, layer, row))
        if (exact >> k) & 1:
            known[32 * k:32 * k + 32] = b'\1' * 32
    model_more(prog, out, known, count, more, own if own is not None else bytearray(count * 32))
    return out, known


def model_object_palettes(prog, stage):
    """The 7 object palettes: arcade rows converted, entry 0 transparent, black 0."""
    out = bytearray()
    for k in range(7):
        words = struct.unpack_from('>16H', prog, PALETTE_OBJECTS + 7 * 0x20 * stage + 0x20 * k)
        out += struct.pack('<16H', 0, *[arcade_color_to_ps1(w) for w in words[:15]])
    return bytes(out)


def map_bytes(module, offset, across, side, words, known=None, known_cells=None):
    """Write a layer's cells (two words each) into the module at its map, marking them in `known` when given (all of them, or only the
    cells (cx, cy) in known_cells); False when a cell lies outside the module (then nothing is promised about what was written)."""
    for (cx, cy), (code, clut) in words.items():
        at = offset + 2 * map_word_index(cx, cy, across, side)
        if at + 4 > len(module):
            return False
        struct.pack_into('<HH', module, at, code, clut)
        if known is not None and (known_cells is None or (cx, cy) in known_cells):
            known[at:at + 4] = b'\1\1\1\1'
    return True


def map_equal(module, offset, across, side, words):
    """True when the module holds exactly these cells at its map."""
    for (cx, cy), (code, clut) in words.items():
        at = offset + 2 * map_word_index(cx, cy, across, side)
        if at + 4 > len(module) or struct.unpack_from('<HH', module, at) != (code, clut):
            return False
    return True


def model_stage(arc, d):
    """{'module', 'pages', 'palettes', 'object tiles', 'object palettes': (bytes, known)} for a descriptor `d` (see describe()).
    Only the three layer maps of the module are built; its other bytes are zero and not known. Raises ValueError where the C refuses
    (a block that does not exist, an address outside the arcade views, a layer that does not fit the module or the pages)."""
    try:
        return model_stage_pieces(arc, d)
    except (struct.error, IndexError) as exc:
        raise ValueError(f'an address lies outside the arcade views ({exc})') from exc


def chain_end(end, extras, holes):
    """The end of the run of page slots that starts at `end` and goes on through the extras and holes that touch it one after the other."""
    runs = sorted([(a, c) for a, c, _f, _fl in extras] + [tuple(h) for h in holes])
    changed = True
    while changed:
        changed = False
        for a, c in runs:
            if a <= end < a + c:
                end, changed = a + c, True
    return end


def l1_start_rule(count0, extras, holes):
    """Where layer 1 starts by the packing rule: at the next page column after layer 0's tiles and whatever the extras and holes put
    straight after them."""
    return (chain_end(count0, extras, holes) + 15) // 16 * 16


def l2_start_rule(end1, extras, holes):
    """Where layer 2 starts by the packing rule: at the next free 32 pixel column after layer 1's tiles (which end at `end1`) and the
    extras and holes that follow them without a gap."""
    return l2_start_after(chain_end(end1, extras, holes))


def layer_start(d, n, counts):
    """The page slot layer n starts at: the descriptor's, or the packing rule's when its flags say so. counts: tiles of the layers before."""
    layer = d['layers'][n]
    if layer['flags'] & START_RULE:
        if n == 1:
            return l1_start_rule(counts[0], d['extras'], d['holes'])
        if n == 2:
            return l2_start_rule(counts[1] + counts[3], d['extras'], d['holes'])
    return layer['start']


def known_cell_set(layer, cells):
    """The cells (cx, cy) of a layer whose words are known: all (None) when the map flag is set, else those named by the cell_known bits."""
    if layer['flags'] & MAP_EXACT:
        return None
    mask = layer.get('cell_known', 0)
    return {(cx, cy) for i, (cx, cy, _t, _r) in enumerate(cells) if (mask >> i) & 1}


def write_layer_maps(module, known, layer, side, words, cs):
    """Write the cells of a layer and of its extra maps into the module (the extra maps' cells have x from XKEY on, see xmap_cells),
    marking them known as map_bytes does: all when `cs` is None and the layer's map flag is set, else the cells in `cs`.
    False when a cell lies outside the module."""
    all_known = bool(layer['flags'] & MAP_EXACT or cs)
    parts = [(layer['map'], layer['across'], 0)] + [(xm['map'], xmap_across(side), XKEY * (xi + 1)) for xi, xm in enumerate(layer.get('xmaps', ()))]
    for offset, across, lo in parts:
        hi = lo + XKEY if lo else XKEY
        mine = {(cx - lo, cy): v for (cx, cy), v in words.items() if lo <= cx < hi}
        mine_cs = None if cs is None else {(cx - lo, cy) for cx, cy in cs if lo <= cx < hi}
        if not map_bytes(module, offset, across, side, mine, known if all_known else None, mine_cs):
            return False
    return True


def model_stage_pieces(arc, d):
    sizes = d['sizes']
    module, module_known = bytearray(sizes['module']), bytearray(sizes['module'])
    pages, pages_known = bytearray(sizes['pages']), bytearray(b'\1' * sizes['pages'])   # known, but for the holes and the tiles that differ
    counts = [0, 0, 0, 0]               # tiles of the layers; counts[3] the start of layer 1 (for layer 2's rule)
    module_own, pages_own = bytearray(sizes['module']), bytearray(sizes['pages'])
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        if n == 0 and layer['flags'] & L0_OWN:           # built by the own rule: all owned, nothing known
            r = l0_result(arc, d)
            ck, tk = layer.get('cell_known', 0), layer.get('tile_known', 0)       # what equals the disc's: known; the rest owned
            for (cx, cy), (code, clut) in r['words'].items():
                at = cell_offset(layer, 0, cx, cy)
                if at + 4 > len(module):
                    raise ValueError('layer 0 map does not fit the module')
                struct.pack_into('<HH', module, at, code, clut)
                if (ck >> r['index'][(cx, cy)]) & 1:
                    module_known[at:at + 4] = b'\1\1\1\1'
                else:
                    module_own[at:at + 4] = b'\1\1\1\1'
            for slot, tile in list(r['tiles'].items()) + [(x, None) for x in r['zero_slots']]:
                if tile is not None and not put_slot(pages, slot, 16, tile):
                    raise ValueError('a layer 0 tile does not fit the pages')
                if not slot_ok(pages, slot, 16):
                    continue
                equal = (tk >> slot) & 1
                mark_slot(pages_known, slot, 16, 1 if equal else 0)
                mark_slot(pages_own, slot, 16, 0 if equal else 1)
            counts[0] = len(r['tiles'])
            continue
        cells = layer_cells_all(arc, d, n)
        start = layer_start(d, n, counts) if n else layer['start']
        if n == 1:
            counts[3] = start
        words, tiles = place_layer(cells, n, start, dict(layer.get('exc', ())))
        counts[n] = len(tiles)
        cs = known_cell_set(layer, cells)
        if not write_layer_maps(module, module_known, layer, side, words, cs):
            raise ValueError(f'layer {n} map does not fit the module')
        for i in layer.get('own_cells', ()):     # cells built by design: served owned, never known
            at = cell_offset(layer, n, cells[i][0], cells[i][1])
            module_own[at:at + 4] = b'\1\1\1\1'
            module_known[at:at + 4] = bytes(4)
        tmask = layer.get('tile_known', 0)
        for j, tile in enumerate(tiles):
            if tile is None:
                continue
            if not put_slot(pages, start + j, side, tile):
                raise ValueError(f'layer {n} tile {j} does not fit the pages')
            if not layer['flags'] & TILES_EXACT and not (j <= len(cells) and (tmask >> j) & 1):
                mark_slot(pages_known, start + j, side, 0)
        for j in layer.get('own_tiles', ()):     # tiles the PS1 retouched, built as the arcade's: served owned, never known
            if j >= len(tiles) or tiles[j] is None:
                raise ValueError(f'layer {n} retouched tile {j} is not a tile')
            mark_slot(pages_known, start + j, side, 0)
            mark_slot(pages_own, start + j, side, 1)
    for slot, count, first, flip in d['extras']:
        for k in range(count):
            if flip & ~(3 | SCROLL3_FLIP | 3 << QUARTER_SHIFT) or (not flip & SCROLL3_FLIP and flip >> QUARTER_SHIFT):
                raise ValueError('an extra run has a bad flip')
            if flip & SCROLL3_FLIP:
                tile = arc.quarter(first + k, (flip >> QUARTER_SHIFT) & 3, flip & 1, flip & 2) if arc.has_tile(first + k, 32) else None
            else:
                tile = arc.tile(first + k, 16, flip & 1, flip & 2) if arc.has_tile(first + k) else None
            if tile is None or not put_slot(pages, slot + k, 16, tile):
                raise ValueError('an extra tile does not fit the pages or the arcade graphics')
    for slot, count in d['holes']:
        for k in range(count):
            if slot_ok(pages, slot + k, 16):
                mark_slot(pages_known, slot + k, 16, 0)
    objects = tile_run_bytes(arc, d['object_runs'], sizes['object tiles'])
    if d.get('grids'):                                   # the object grids of the module that the arcade builds equal to the disc's (stage_objects.py)
        import fighters
        import stage_objects as O
        if not hasattr(arc, '_blank'):
            arc._blank = {t for t in range(len(arc.gfx) // 128) if arc.gfx[t * 128:(t + 1) * 128] == b'\xff' * 128}
            arc._view = fighters.View(arc.prog)
        keys = O.tile_keys(d['object_runs'], d['shared_runs'])
        for off, addr, lo, hi in d['grids']:
            rec = fighters.read_any(arc._view, addr)
            made = O.build_grid(rec[1], arc._blank, keys, lo, hi) if rec else None
            if made is None or off + len(made) > len(module):
                raise ValueError(f'the grid at {off} cannot be built from the record at {addr:#x}')
            module[off:off + len(made)] = made
            module_known[off:off + len(made)] = b'\1' * len(made)
    pal_own = bytearray(sizes['palettes'])
    palettes = model_palettes(arc.prog, d['stage'], sizes['palettes'] // 32, d['pal_exact'], d.get('more', ()), pal_own)
    if d['layers'][0]['flags'] & L0_OWN:                 # the layer's 32 rows by the own rule (arcade rows and merged ones), served owned
        for r, words in l0_result(arc, d)['rows'].items():
            struct.pack_into('<16H', palettes[0], 64 * r, *words)
            equal = (d['pal_exact'] >> (2 * r)) & 1
            palettes[1][64 * r:64 * r + 32] = (b'\1' if equal else b'\0') * 32
            pal_own[64 * r:64 * r + 32] = (b'\0' if equal else b'\1') * 32
    result = {'module': (module, module_known), 'pages': (pages, pages_known),
            'palettes': palettes,
            'object tiles': objects,
            'object palettes': (model_object_palettes(arc.prog, d['stage']), b'\1' * OBJECT_PALETTES_BYTES),
            'owned': {'object tiles'} if runs_owned(d['object_runs']) else set(),
            'own': {'palettes': pal_own, 'module': module_own, 'pages': pages_own}}
    for label in ('module', 'pages', 'palettes'):         # the first word of a chunk stays the disc's (stage.c first_words_left_alone)
        result['own'][label][0:4] = bytes(4)
    return result


def mark_slot(known, slot, side, value=1):
    at, per = slot_place(slot, side), side // 2
    for y in range(side):
        known[at + y * 128:at + y * 128 + per] = bytes([value]) * per


# ---------------------------------------------------------------- the descriptors (made from the disc and the arcade)

def locate_ids(prog, ids, per_row, lo, hi, near):
    """Addresses in prog[lo:hi] whose bytes are `ids`: each row of `per_row` ids is looked for as a whole (the arcade lists a block
    row as consecutive bytes) and the nearest to `near` wins; a row that is not there is taken id by id; an id 0 with no
    arcade byte near stays a constant (address 0). Raises ValueError for an id the region does not have."""
    out = []
    for j in range(0, len(ids), per_row):
        row = list(ids[j:j + per_row])
        whole = [a for a in range(lo, hi - len(row) + 1) if list(prog[a:a + len(row)]) == row] if any(row) else []
        if whole:
            best = min(whole, key=lambda a: (abs(a - near), a))
            out += [best + k for k in range(len(row))]
            continue
        for v in row:
            if v == 0:
                out.append(0)
                continue
            found = [a for a in range(lo, hi) if prog[a] == v]
            if not found:
                raise ValueError(f'id {v} is not in the arcade list at {lo:#x}')
            out.append(min(found, key=lambda a: (abs(a - near), a)))
    return out


def list_pointers(prog, stage):
    """The arcade list addresses of a stage for layers 0, 1, 2: the first byte of the stage's entry."""
    o0 = struct.unpack_from('>H', prog, LIST_TABLES[0] + 2 * stage)[0]
    o1 = struct.unpack_from('>H', prog, LIST_TABLES[1] + 2 * stage)[0]
    return (LIST_TABLES[0] + 4 + o0, LIST_TABLES[1] + 0xc + o1, LIST2_ROW[0] + LIST2_ROW[1] * stage + 4 * LIST2_ROW[2])


def tile_runs(chunk, matches):
    """Runs (first, count, flip) over the 128-byte tiles of a chunk, given matches[i] = 4 * arcade tile + flip or -1: tiles of zeros
    are RUN_ZERO, tiles with no match RUN_DISC, others consecutive arcade tiles with the same flip."""
    runs = []
    for i in range(len(chunk) // 128):
        if not any(chunk[128 * i:128 * i + 128]):
            kind = (RUN_ZERO, 0)
        elif matches[i] < 0:
            kind = (RUN_DISC, 0)
        else:
            kind = (matches[i] >> 2, matches[i] & 3)
        if runs:
            f, c, fl = runs[-1]
            if kind[0] in (RUN_ZERO, RUN_DISC) and f == kind[0]:
                runs[-1] = (f, c + 1, fl)
                continue
            if kind[0] not in (RUN_ZERO, RUN_DISC) and f not in (RUN_ZERO, RUN_DISC) and f + c == kind[0] and fl == kind[1]:
                runs[-1] = (f, c + 1, fl)
                continue
        runs.append((kind[0], 1, kind[1]))
    return runs


def hole_runs(slots):
    """Runs (slot, count) of consecutive ascending page slots."""
    runs = []
    for s in slots:
        if runs and runs[-1][0] + runs[-1][1] == s:
            runs[-1] = (runs[-1][0], runs[-1][1] + 1)
        else:
            runs.append((s, 1))
    return runs


def slot_runs(slots, matches):
    """Runs (slot, count, first, flip) over page slots (ascending) whose tiles match arcade tile matches[i] = 4 * tile + flip (-1: none,
    left out): consecutive slots, consecutive tiles, one flip."""
    runs = []
    for s, m in zip(slots, matches):
        if m < 0:
            continue
        if runs:
            a, c, f, fl = runs[-1]
            if a + c == s and f + c == m >> 2 and fl == m & 3:
                runs[-1] = (a, c + 1, f, fl)
                continue
        runs.append((s, 1, m >> 2, m & 3))
    return runs


MORE_ROWS_FROM, MORE_ROWS_TO = 0x260000, 0x2ad000     # where the arcade's palette rows lie (the tables, the object rows and the effects' rows)


def row_index(arc):
    """{the 16 PS1 colours of an arcade row: [addresses]} of the rows (0x20 apart) in the palette region; entry 0 is 0, black 0x8000."""
    if not hasattr(arc, '_row_index'):
        index = {}
        for at in range(MORE_ROWS_FROM, min(MORE_ROWS_TO, len(arc.prog) - 30), 0x20):
            key = (0,) + tuple(arcade_color_to_ps1(w) or 0x8000 for w in struct.unpack_from('>15H', arc.prog, at))
            index.setdefault(key, []).append(at)
        arc._row_index = index
    return arc._row_index


def more_runs(arc, chunk, all_more=False):
    """Runs (k, step, count, kind, arg) that make the palettes of a chunk that no layer table names ('more') from the arcade: palettes
    k, k + step, ... are the arcade rows at arg, arg + 0x20, ... (MORE_ROW), or filler rows (MORE_FILL). Only palettes that equal the
    build byte for byte are covered; the others (the PS1's mixes of rows) are left out. Longest runs first. With all_more every palette
    of the chunk is looked at, not only those that no layer table names (the second palette chunk of a stage file has no layer tables)."""
    count = len(chunk) // 32
    cands = {}
    fill = (0,) + (arcade_color_to_ps1(FILL_COLOUR) or 0x8000,) * 15
    index = row_index(arc)
    for k in range(count):
        if not all_more and palette_group(k, count)[0] != 'more':
            continue
        have = struct.unpack_from('<16H', chunk, 32 * k)
        found = [(MORE_ROW, a) for a in index.get(tuple(have), [])]
        if tuple(have) == fill:
            found.append((MORE_FILL, 0))
        if found:
            cands[k] = found
    left, runs = set(cands), []
    while left:
        best = None
        for k in sorted(left):
            for kind, a in cands[k]:
                for step in (1, 2):
                    n = 1
                    while (k + step * n) in left and (kind, a + 0x20 * n if kind == MORE_ROW else 0) in cands[k + step * n]:
                        n += 1
                    if best is None or n > best[0]:
                        best = (n, k, step if n > 1 else 1, kind, a)
        n, k, step, kind, a = best
        runs.append((k, step, n, kind, a))
        for m in range(n):
            left.discard(k + step * m)
    return sorted(runs)


# The second palette chunk's 18 palettes that no arcade row makes (the other 110 are arcade rows). {palette: (arcade row address or None, why)}:
# built by design, not equal to the disc's (MORE_APPROX, or MORE_BLANK for None); describe_shared_palettes refuses a chunk whose palettes
# that are no arcade row are not exactly these. Measured 2026-10-24 on all 20 files (the chunk is the same in each).
SHARED_PALETTES_BY_DESIGN = {
    # 10 and 93: an arcade row but for one entry (the PS1 changed an entry: a flash colour).
    10: (0x277040, 'one entry differs'), 93: (0x275be0, 'one entry differs'),
    # 65 to 89 (odd): thirteen rows of four groups, each group the same fifteen colours as one of four consecutive arcade rows (0x29c240 to
    # 0x29c2a0) laid out on other indexes, one entry a ramp of colours over the rows of a group (a fade of the PS1's making); the arcade tiles
    # that use them are the arcade's own, so the arcade's row is the one that shows them as the arcade does (the ramp is not the arcade's).
    65: (0x29c240, 'group 1'),
    67: (0x29c260, 'group 2'), 69: (0x29c260, 'group 2'), 71: (0x29c260, 'group 2'), 73: (0x29c260, 'group 2'),
    75: (0x29c280, 'group 3'), 77: (0x29c280, 'group 3'), 79: (0x29c280, 'group 3'), 81: (0x29c280, 'group 3'),
    83: (0x29c2a0, 'group 4'), 85: (0x29c2a0, 'group 4'), 87: (0x29c2a0, 'group 4'), 89: (0x29c2a0, 'group 4'),
    # 123, 125, 127: rows of two near-black tints (translucent shading of the PS1's own): no arcade row is near (7 or more entries differ),
    # the arcade shows nothing like it: blank.
    123: (None, 'near-black tint, no arcade row'), 125: (None, 'near-black tint, no arcade row'), 127: (None, 'near-black tint, no arcade row'),
}


def rows_list(arc):
    """[(address, the 16 PS1 colours)] of the arcade's palette rows (0x20 apart in the palette region), cached."""
    if not hasattr(arc, '_rows_list'):
        arc._rows_list = [(at, (0,) + tuple(arcade_color_to_ps1(w) or 0x8000 for w in struct.unpack_from('>15H', arc.prog, at)))
                          for at in range(MORE_ROWS_FROM, min(MORE_ROWS_TO, len(arc.prog) - 30), 0x20)]
    return arc._rows_list


def nearest_row(arc, have):
    """(entries that differ, address) of the arcade row nearest to a palette of 16 colours (the lowest address among equals)."""
    return min((sum(1 for a, b in zip(have, row) if a != b), at) for at, row in rows_list(arc))


def design_runs(arc, chunk, runs, all_more=False, explicit=None):
    """(runs by design, counts) for the palettes the exact runs leave out among those that no layer table names (all of them with all_more):
    each is MORE_APPROX of the nearest arcade row when at most APPROX_MAX_DIFF entries differ, else MORE_BLANK. With `explicit` ({k: (address or
    None, why)}) the palettes are exactly its keys and take its rows; otherwise the string of a refusal. counts: (approximated, blank)."""
    count = len(chunk) // 32
    done = {k + st * i for k, st, n, _kind, _arg in runs for i in range(n)}
    left = [k for k in range(count) if k not in done and (all_more or palette_group(k, count)[0] == 'more')]
    if explicit is not None:
        if set(left) != set(explicit):
            return f'the palettes no arcade row makes are {sorted(left)}, the table says {sorted(explicit)}'
        out = [(k, 1, 1, MORE_BLANK if explicit[k][0] is None else MORE_APPROX, explicit[k][0] or 0) for k in left]
    else:
        out = []
        for k in left:
            diff, at = nearest_row(arc, struct.unpack_from('<16H', chunk, 32 * k))
            out.append((k, 1, 1, MORE_APPROX, at) if diff <= APPROX_MAX_DIFF else (k, 1, 1, MORE_BLANK, 0))
    return out, (sum(1 for r in out if r[3] == MORE_APPROX), sum(1 for r in out if r[3] == MORE_BLANK))


def find_exceptions(cells, side, wants):
    """[(cell index, slot offset)] where the disc's cells differ from the first-use packing of place_layer: wants[i] is the slot offset
    (slot minus the layer's start) the disc gives cell i, or None when it is not on the pages. A cell whose tile the packing would put
    elsewhere gets its offset (the disc shares a slot, or skips slots for a tile of its own), or FORCE_NEW when the disc starts a new
    slot for it. After an exception the packing is in step again. A cell with a tile the packing has already, which the disc puts
    beyond the next free slot, is left alone (the PS1's own cells): nothing is promised about it."""
    index, count, exc = {bytes(side * side // 2): 0}, 1, []
    for i, (_cx, _cy, tile, _row) in enumerate(cells):
        have = index.get(tile)
        got = count if have is None else have
        if wants[i] is None or wants[i] == got:
            if have is None:
                index[tile] = count
                count += 1
        elif wants[i] == count:
            exc.append((i, FORCE_NEW))
            count += 1
        elif wants[i] < count:
            exc.append((i, wants[i]))
        elif have is None:
            exc.append((i, wants[i]))
            index[tile] = wants[i]
            count = wants[i] + 1
    return exc


def cell_like(module, at, npages):
    """True when the four bytes at `at` look like a layer 1 or layer 2 cell: a tile code on one of the stage's pages, a palette word
    0x7800 + layer + 64 * row."""
    code, clut = struct.unpack_from('<HH', module, at)
    return 0x7800 <= clut < 0x8000 and (clut & 63) in (1, 2) and FIRST_PAGE <= code >> 10 < FIRST_PAGE + npages


def identify_block(arc, parts, start, n, page, half):
    """The arcade block (of the tables of layer n: 16 pixel blocks of 16 x 16 cells or 32 pixel blocks of 8 x 8) that the half `half`
    of page `page` of the extra map at byte `start` of the module is, or None: every cell must show the pixels of the arcade
    cell (flips applied) and, but for a blank one, its palette row. The lowest block that does so, so a blank half is block 0."""
    module, pages = parts['module'], parts['pages']
    side, bc = (16, 16, 32)[n], (16, 16, 8)[n]
    table, stride, nblocks = ((TABLE16, 1024, BLOCK16_COUNT), (TABLE32, 256, BLOCK32_COUNT))[n - 1]
    have = []
    for k in range(bc * bc):
        x, y = divmod(k, bc)
        at = start + 2 * map_word_index(half * bc + x, page * bc + y, xmap_across(side), side)
        code, clut = struct.unpack_from('<HH', module, at)
        slot = slot_of_code(code, side)
        if slot is None or not slot_ok(pages, slot, side):
            return None
        have.append((get_slot(pages, slot, side), (clut - 0x7800) >> 6))
    for b in range(nblocks):
        for k in range(bc * bc):
            x, y = divmod(k, bc)
            code, attr = struct.unpack_from('>HH', arc.prog, table + b * stride + (x * bc + y) * 4)
            index, fx, fy, row = arcade_cell_tile(code, attr, side)
            tile = arc.tile(index, side, fx, fy)
            if tile != have[k][0] or (any(tile) and row != have[k][1]):
                break
        else:
            return b
    return None


def find_xmaps(arc, parts, d):
    """[(layer, {'map': byte offset in the module, 'ids': [block numbers]})]: the maps in the stage module, outside the three layer
    maps, that are runs of whole pages of layer 1 or layer 2 cells (two blocks side by side a page: 32 x 16 cells of 16 pixels, or
    16 x 8 of 32 pixels) all of whose halves are blocks of the arcade (identify_block). They are the arcade's animation frames of the
    layer (a waterfall, cyclists, waves): the PS1 keeps each frame as a further map. A run (cells of one layer: the palette word's
    column) with a half that is no block is left out."""
    module, pages = parts['module'], parts['pages']
    npages = len(pages) // PAGE_BYTES
    taken = bytearray(len(module))
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        for cy in range(layer['down']):
            for cx in range(layer['across']):
                at = layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side)
                taken[at:at + 4] = b'\1\1\1\1'
    out, at = [], 0
    while at + 4 <= len(module):
        if taken[at] or not cell_like(module, at, npages):
            at += 4
            continue
        col = struct.unpack_from('<H', module, at + 2)[0] & 63
        end = at
        while end + 4 <= len(module) and not taken[end] and cell_like(module, end, npages) and struct.unpack_from('<H', module, end + 2)[0] & 63 == col:
            end += 4
        page_bytes = (2048, 512)[col - 1]
        ids = []
        for page in range((end - at) // page_bytes):
            pair = [identify_block(arc, parts, at, col, page, half) for half in (0, 1)]
            if None in pair:
                ids = None
                break
            ids += pair
        if ids:
            out.append((col, {'map': at, 'ids': ids}))
        at = end
    return out


def quarter_extras(arc, slots):
    """[(slot, 1, 32 pixel tile, flip)] for the page slots [(slot, tile bytes)] whose tile is a quarter of one of the arcade's 32 x 32 pixel
    tiles (sprite_tiles.py's index; the lowest tile, then quarter, then flip that makes it): the flip carries SCROLL3_FLIP and the quarter."""
    if not slots:
        return []
    import sprite_tiles as X
    if not hasattr(arc, '_index32'):
        arc._index32 = X.tile_index32(arc.gfx)
    out = []
    for slot, tile in slots:
        keys = X.candidates(tile, {}, None, arc._index32)
        if keys:
            k = keys[0] - X.BASE32
            out.append((slot, 1, k >> 4, (k & 3) | SCROLL3_FLIP | (k >> 2 & 3) << QUARTER_SHIFT))
    return out


def exceptions_of(arc, d, n, module, skip=()):
    """The packing exceptions of layer n (find_exceptions) against the disc's module; the cells in `skip` (scan indices) are taken as not on the
    pages, so that the PS1's slots for them are not followed (cells built by design)."""
    layer, side = d['layers'][n], (16, 16, 32)[n]
    cells = layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], d['stage'])
    wants = []
    for i, (cx, cy, _t, _r) in enumerate(cells):
        code = struct.unpack_from('<H', module, layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side))[0]
        slot = slot_of_code(code, side)
        wants.append(None if i in skip or slot is None or slot < layer['start'] else slot - layer['start'])
    return find_exceptions(cells, side, wants)


# Layer 0's extra maps (the layer's animation frames, scroll 1 sets) that the PS1 keeps in the stage module like the animation blocks of layers 1 and
# 2 (xmaps), found by hand because the PS1's merged tiles are not the arcade's. {file: (map offset, sets)}, checked against the disc by
# describe (the sets must be the ones the best match names for each block, and most of their pieces must agree). STAGE06: 4 blocks of 16 x 16 cells
# at 9112: a blank set and the arcade's sets 49, 50 and 51 (measured 2026-10-24: 907, 676 and 918 of 1,024 pieces equal in pixels and row).
L0_EXTRA_MAPS = {'STAGE06.PAC': (9112, [0, 49, 50, 51])}


def decide_l0_own(arc, d, parts, helper):
    """Make layer 0 of descriptor d built by the own rule (L0_OWN) when the model's does not equal the disc's: some cell outside the bytes
    of the other layers' maps is not known, or its tiles or its palette rows are not exact. True when it is (the layer's flags, extra map, skip
    and reserved spans and the palette bits are set), False when not, or the text of a refusal."""
    layer, name = d['layers'][0], d['file']
    skip = l0_skip_cells(d)
    skipped = {i for a, c in skip for i in range(a, a + c)}
    unknown = any(not (layer['flags'] & MAP_EXACT or (layer['cell_known'] >> i) & 1) for i in range(layer['across'] // 16 * (layer['down'] // 16) * 256) if i not in skipped)
    rows_exact = all((d['pal_exact'] >> (2 * r)) & 1 for r in range(32))
    if name in L0_EXTRA_MAPS:
        off, sets = L0_EXTRA_MAPS[name]
        found = helper.match_layer0(parts['pages'], parts['module'], off, 32, 32)
        if [b[1] for b in found[1:]] != sets[1:] or any(b[2] * 2 < b[0] for b in found):
            return f'{name}: the extra layer 0 map at {off} is not the sets {sets}'
        layer['xmaps'] = [{'map': off, 'ids': sets}]
    elif not unknown and layer['flags'] & TILES_EXACT and rows_exact:
        return False
    layer['l0own'] = {'skip': skip, 'reserved': []}
    layer['flags'] = L0_OWN
    layer['cell_known'] = layer['tile_known'] = 0
    layer.pop('own_cells', None)
    layer.pop('own_tiles', None)
    layer['l0own']['reserved'] = l0_list_slots(parts, d)
    d['layers'][1]['flags'] &= ~START_RULE
    d.pop('_l0', None)
    try:
        res = l0_result(arc, d)
    except ValueError as exc:
        return f'{name}: {exc}'
    module, pages, chunk = parts['module'], parts['pages'], parts['palettes']
    ck = tk = 0
    for key, (code, clut) in res['words'].items():           # what the rule builds equal to the disc's: known, the rest is owned
        if struct.unpack_from('<HH', module, cell_offset(layer, 0, *key)) == (code, clut):
            ck |= 1 << res['index'][key]
    for slot, tile in res['tiles'].items():
        if slot_ok(pages, slot, 16) and get_slot(pages, slot, 16) == tile:
            tk |= 1 << slot
    for slot in res['zero_slots']:
        if slot_ok(pages, slot, 16) and not any(get_slot(pages, slot, 16)):
            tk |= 1 << slot
    layer['cell_known'], layer['tile_known'] = ck, tk
    for r in range(32):
        d['pal_exact'] &= ~(1 << (2 * r))
        if list(struct.unpack_from('<16H', chunk, 64 * r)) == res['rows'][r]:
            d['pal_exact'] |= 1 << (2 * r)
    return True


def describe(name, stage, parts, arc, helper, r):
    """The descriptor of one stage file: `parts` from split_stage, `r` from analyse (which blocks the layers are). Flags are set
    where the model equals the disc. Returns a dict, or a string when the file cannot be described."""
    module, pages = parts['module'], parts['pages']
    found = find_layers(module)
    if found is None:
        return 'layer maps not found in the module'
    layers, pointers = found
    extents = layer_extents(layers, pointers)
    ptrs = list_pointers(arc.prog, stage)
    d = {'file': name, 'stage': stage, 'layers': [], 'extras': [], 'holes': [],
         'sizes': {'module': len(module), 'pages': len(pages), 'palettes': len(parts['palettes']),
                   'object tiles': len(parts.get('object tiles', b'')), 'object palettes': len(parts['object palettes'])}}
    starts = [0, None, None]
    for n, (off, across, down, side) in enumerate(extents):
        codes = []
        for cy in range(down):
            for cx in range(across):
                at = off + 2 * map_word_index(cx, cy, across, side)
                code = struct.unpack_from('<H', module, at)[0]
                slot = slot_of_code(code, side)
                if slot is not None:
                    codes.append(slot)
        if n and not codes:
            return f'layer {n} has no tile on the pages'
        if n:
            starts[n] = min(codes)
        blocks = r['layers'][n]['blocks']
        bc = 16 if n < 2 else 8
        try:
            at = locate_ids(arc.prog, [b[1] for b in blocks], across // bc, LIST_REGION[n][0], LIST_REGION[n][1], ptrs[n])
        except ValueError as exc:
            return str(exc)
        d['layers'].append({'map': off, 'across': across, 'down': down, 'ids': at, 'start': starts[n], 'flags': 0})
    d['object_runs'] = []
    if 'object tiles' in parts:
        n, found_n, idx = helper.tiles(parts['object tiles'])
        d['object_runs'] = blank_by_design(name, tile_runs(parts['object tiles'], idx))
        if isinstance(d['object_runs'], str):
            return d['object_runs']
    d['pal_exact'] = 0
    # where the disc's cells differ from the first-use packing, per layer: exceptions; then what the model builds equals the disc's
    for n, layer in enumerate(d['layers']):
        try:
            layer['exc'] = exceptions_of(arc, d, n, module)
        except ValueError as exc:
            return str(exc)
    for n, xm in find_xmaps(arc, parts, d):
        d['layers'][n].setdefault('xmaps', []).append(xm)
    def measure():
        """The model's maps and tiles against the disc's: sets each layer's flags and known bits; (maps, tiles, extra maps with a cell that differs)."""
        first = model_stage_raw(arc, d)
        if isinstance(first, str):
            return first
        maps, tiles = first
        bad = []
        for n, layer in enumerate(d['layers']):
            side = (16, 16, 32)[n]
            if n == 0 and layer['flags'] & L0_OWN:
                continue
            cells = layer_cells_all(arc, d, n)
            bits = 0
            for i, (cx, cy, _t, _r) in enumerate(cells):
                if struct.unpack_from('<HH', module, cell_offset(layer, n, cx, cy)) == maps[n][(cx, cy)]:
                    bits |= 1 << i
                elif cx >= XKEY:
                    bad.append((n, cx // XKEY - 1))
            tbits = 0
            for j, t in enumerate(tiles[n]):
                if t is not None and j <= len(cells) and slot_ok(pages, layer['start'] + j, side) and get_slot(pages, layer['start'] + j, side) == t:
                    tbits |= 1 << j
            every = sum(1 << j for j, t in enumerate(tiles[n]) if t is not None)
            layer['flags'] = (MAP_EXACT if bits == 2 ** len(cells) - 1 else 0) | (TILES_EXACT if tbits == every else 0)
            layer['cell_known'] = 0 if layer['flags'] & MAP_EXACT else bits
            layer['tile_known'] = 0 if layer['flags'] & TILES_EXACT else tbits
        return maps, tiles, bad

    while True:                           # an extra map with a cell that differs is left out
        got = measure()
        if isinstance(got, str):
            return got
        maps, tiles, bad = got
        if not bad:
            break
        for n, xi in sorted(set(bad), reverse=True):
            del d['layers'][n]['xmaps'][xi]
    # cells and tiles of layers 1 and 2 built as the arcade has them where the PS1 differs (DESIGN_BY_DESIGN): found, counted against the table
    starts = [d['layers'][0]['start'], layer_start(d, 1, [len(tiles[0]), 0, 0, 0]), None]
    starts[2] = layer_start(d, 2, [len(tiles[0]), len(tiles[1]), 0, starts[1]])
    found_design = False
    for n in (1, 2):
        blank, row, _ret = design_of(arc, d, module, pages, n, starts[n], tiles[n], maps[n])
        if blank:
            d['layers'][n]['exc'] = exceptions_of(arc, d, n, module, set(blank))
            found_design = True
    if found_design:
        got = measure()
        if isinstance(got, str):
            return got
        maps, tiles, bad = got
        starts = [d['layers'][0]['start'], layer_start(d, 1, [len(tiles[0]), 0, 0, 0]), None]
        starts[2] = layer_start(d, 2, [len(tiles[0]), len(tiles[1]), 0, starts[1]])
    for n in (1, 2):
        layer = d['layers'][n]
        blank, row, ret = design_of(arc, d, module, pages, n, starts[n], tiles[n], maps[n])
        want = DESIGN_BY_DESIGN.get((name, n), (0, 0, 0, ''))
        if (len(blank), len(row), len(ret)) != want[:3]:
            return f'{name} layer {n}: {len(blank)} blank cells, {len(row)} cells in another row, {len(ret)} retouched tiles; the by-design table says {want[:3]}'
        if blank or row or ret:
            layer['own_cells'] = sorted(blank + row)
            layer['own_tiles'] = ret
            for i in layer['own_cells']:            # built by design: not known (they differ from the disc's by what the table counts)
                layer['cell_known'] &= ~(1 << i)
            for j in ret:
                layer['tile_known'] &= ~(1 << j)
    # palettes
    chunk = parts['palettes']
    count = len(chunk) // 32
    for k in range(min(count, 128)):
        layer, row = palette_group(k, count)
        if layer != 'more' and list(struct.unpack_from('<16H', chunk, 32 * k)) == predicted_palette(arc.prog, stage, layer, row):
            d['pal_exact'] |= 1 << k
    d['more'] = more_runs(arc, chunk)
    design = design_runs(arc, chunk, d['more'])
    d['more'] = sorted(d['more'] + design[0])
    d['more_design'] = design[1]
    # layer 0 by the own rule where the PS1's differs from what the model builds (E2, E3)
    own0 = decide_l0_own(arc, d, parts, helper)
    if isinstance(own0, str):
        return own0
    if own0:
        maps, tiles, _bad = measure()                 # the new layer 0 tiles for the outside slots below
    # page slots outside the model's layers that hold arcade tiles
    covered = set()
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        if n == 0 and own0:                            # the region before layer 1 is layer 0's, but for the slots a list of the module names
            reserved = {x for a, c in layer['l0own']['reserved'] for x in range(a, a + c)}
            covered.update(x for x in range(d['layers'][1]['start']) if x not in reserved)
            continue
        for j, t in enumerate(tiles[n]):
            if t is None:
                continue
            s = layer['start'] + j
            if side == 16:
                covered.add(s)
            else:
                pg, rest = divmod(s, 64)
                col, row = rest // 8, rest % 8
                covered.update(pg * 256 + (col * 2 + dx) * 16 + row * 2 + dy for dx in range(2) for dy in range(2))
    slots = [s for s in range(len(pages) // 128) if s not in covered and any(get_slot(pages, s, 16))]
    d['holes'] = []
    if slots:
        chunk = b''.join(get_slot(pages, s, 16) for s in slots)
        matches = helper.tiles(chunk)[2]
        d['extras'] = slot_runs(slots, matches)
        left = [s for s, m in zip(slots, matches) if m < 0]
        quarters = quarter_extras(arc, [(s, get_slot(pages, s, 16)) for s in left])
        d['extras'] = sorted(d['extras'] + quarters)
        d['holes'] = hole_runs([s for s in left if s not in {a for a, _c, _f, _fl in quarters}])
    refusal = holes_by_design(name, d['holes'])
    if refusal:
        return refusal
    # the packing rule for the starts of layers 1 and 2
    s1, s2 = starts_by_rule(arc, d)
    if s1 == d['layers'][1]['start'] and not own0:
        d['layers'][1]['flags'] |= START_RULE
    if s2 == d['layers'][2]['start']:
        d['layers'][2]['flags'] |= START_RULE
    return d


def slot_ok(pages, slot, side):
    """True when the tile of a page slot lies inside the pages and has a tile code."""
    if slot < 0 or slot // (256 if side == 16 else 64) + FIRST_PAGE > 63:
        return False
    return slot_place(slot, side) + (side - 1) * 128 + side // 2 <= len(pages)


def model_stage_raw(arc, d):
    """(maps, tiles) of a descriptor's three layers: [{(cx, cy): (code, clut)}] and [[tile bytes]]; or a message."""
    maps, tiles = [], []
    counts = [0, 0, 0, 0]
    try:
        for n, layer in enumerate(d['layers']):
            if n == 0 and layer['flags'] & L0_OWN:
                r = l0_result(arc, d)
                words, t = r['words'], [r['tiles'].get(j) for j in range(max(r['tiles']) + 1)]
            else:
                cells = layer_cells_all(arc, d, n)
                start = layer_start(d, n, counts) if n else layer['start']
                if n == 1:
                    counts[3] = start
                words, t = place_layer(cells, n, start, dict(layer.get('exc', ())))
            counts[n] = len(t)
            maps.append(words)
            tiles.append(t)
    except ValueError as exc:
        return str(exc)
    return maps, tiles


def l2_start_after(end16):
    """The first slot of 32 pixel tiles that lies wholly after 16 pixel slot `end16`: the next free 32 pixel column of the pages."""
    page, rest = divmod(end16, 256)
    return page * 64 + (((rest + 15) // 16 + 1) // 2) * 8


def starts_by_rule(arc, d):
    """(layer 1 start, layer 2 start) the packing rule gives for a descriptor, from the model's tile counts: layer 1 starts at the next
    page column after layer 0's tiles (and what the extras and holes put straight after them); layer 2 at the next free 32 pixel column
    after layer 1's tiles (from the descriptor's start) and the extras and holes that follow them."""
    maps, tiles = model_stage_raw(arc, d)
    return (l1_start_rule(len(tiles[0]), d['extras'], d['holes']),
            l2_start_rule(d['layers'][1]['start'] + len(tiles[1]), d['extras'], d['holes']))


def shared_runs(parts_list, helper):
    """The runs of the shared effect tiles (every stage file holds the same chunk); a message when two differ."""
    chunk = parts_list[0]
    if any(p != chunk for p in parts_list):
        return 'the stages\' shared tiles differ'
    return tile_runs(chunk, helper.tiles(chunk)[2])


# ---------------------------------------------------------------- the C table

def index_spans(indexes):
    """[(first, count)] of the runs of consecutive numbers in a sorted list of indexes."""
    out = []
    for i in indexes:
        if out and out[-1][0] + out[-1][1] == i:
            out[-1] = (out[-1][0], out[-1][1] + 1)
        else:
            out.append((i, 1))
    return out


def c_array(name, values, per_line=8, fmt='{:#x}u', ctype='uint32_t'):
    out = [f'static const {ctype} {name}[] = {{']
    for i in range(0, len(values), per_line):
        out.append('    ' + ', '.join(fmt.format(v) for v in values[i:i + per_line]) + ',')
    out.append('};')
    return out


def mask_words(mask, bits):
    """The 64-bit words of a bit mask of `bits` bits."""
    return [(mask >> (64 * i)) & (2 ** 64 - 1) for i in range((bits + 63) // 64)]


def c_table(descs, shared, omitted, shared_pal=None):
    """The text of stages_table.c: for each described stage its layers' map offsets, sizes and lists of arcade addresses, the exceptions
    of the packing and the known cells and tiles, the page slots of arcade tiles, the object tile runs, the runs of palettes and the
    flags; then the shared tile runs and the runs of palettes of the second palette chunk, which every stage file holds alike
    (shared_pal: (bytes of the chunk, runs as more_runs makes them), or None). No byte of either game."""
    out = ['/* GENERATED by `python3 port/arcade/stages.py --disc DISC --arcade SET --c-table`; do not edit.',
           ' * For each stage file: where the three layer maps lie in the module chunk, the arcade program addresses of the bytes that',
           ' * name each layer\'s block sets, the page slot each layer starts at (unless the packing rule gives it), the cells where the',
           ' * disc differs from the first-use packing of tiles, which cells and tiles the build reproduces, the page slots that hold',
           ' * arcade tiles apart from the layers, the runs of arcade tiles of the object tile chunk, the runs of palettes that are arcade',
           ' * rows, the sizes of the chunks and the flags of what the build reproduces. Only addresses, counts and offsets: no byte of',
           ' * either game.']
    for name, why in omitted:
        out.append(f' * Left out: {name}: {why}')
    out += [' */', '#include "stage.h"', '']
    for d in descs:
        s = f'{d["stage"]:02x}'
        for n, layer in enumerate(d['layers']):
            out += c_array(f'ids_{s}_{n}', layer['ids'], 8, '{:#x}u')
            if layer.get('exc'):
                out.append(f'static const struct arcade_stage_exc exc_{s}_{n}[] = {{')
                out += [f'    {{{c}u, {"ARCADE_STAGE_FORCE_NEW" if sl == FORCE_NEW else str(sl) + "u"}}},' for c, sl in layer['exc']]
                out.append('};')
            cells = layer_cell_total(layer, n)
            for what, key in (('oc', 'own_cells'), ('ot', 'own_tiles')):
                if layer.get(key):
                    if what == 'oc':
                        out.append(f'/* {d["file"]} layer {n}: {len(layer[key])} cells are built as the arcade has them, not as the disc (by design, the owner\'s rule "rebuild arcade-style"; stages.py DESIGN_BY_DESIGN: '
                                   f'{DESIGN_BY_DESIGN[(d["file"], n)][3]}); served owned. */')
                    else:
                        out.append(f'/* {d["file"]} layer {n}: {len(layer[key])} tiles the PS1 retouched are built as the arcade has them (by design; stages.py DESIGN_BY_DESIGN); served owned. */')
                    spans = index_spans(layer[key])
                    out.append(f'static const struct arcade_stage_span {what}_{s}_{n}[] = {{')
                    out += [f'    {{{a}u, {c}u}},' for a, c in spans]
                    out.append('};')
            if layer.get('l0own'):
                lo = layer['l0own']
                out.append(f'/* {d["file"]} layer 0: built by the own rule (stages.py "our own layer 0"): the cells, tiles and 32 palette rows are the loader\'s, served owned; '
                           f'{sum(c for _a, c in lo["skip"])} cells lie in other layers\' maps and are not built, {sum(c for _a, c in lo["reserved"])} slots of the region stay the disc\'s '
                           f'(lists of the module name them). */')
                for what, key in (('ls', 'skip'), ('lr', 'reserved')):
                    if lo[key]:
                        out.append(f'static const struct arcade_stage_span {what}_{s}[] = {{')
                        out += [f'    {{{a}u, {c}u}},' for a, c in lo[key]]
                        out.append('};')
            if layer.get('xmaps'):
                for xi, xm in enumerate(layer['xmaps']):
                    out += c_array(f'xids_{s}_{n}_{xi}', xm['ids'], 16, '{}u', 'uint8_t')
                out.append(f'static const struct arcade_stage_xmap xmaps_{s}_{n}[] = {{')
                out += [f'    {{{xm["map"]}u, {len(xm["ids"])}u, xids_{s}_{n}_{xi}}},' for xi, xm in enumerate(layer['xmaps'])]
                out.append('};')
            if layer.get('cell_known'):
                out += c_array(f'ck_{s}_{n}', mask_words(layer['cell_known'], cells), 4, '{:#x}ull', 'uint64_t')
            if layer.get('tile_known'):
                out += c_array(f'tk_{s}_{n}', mask_words(layer['tile_known'], cells + 1), 4, '{:#x}ull', 'uint64_t')
        if d['extras']:
            out.append(f'static const struct arcade_stage_slots extras_{s}[] = {{')
            out += [f'    {{{a}u, {c}u, {f}u, {fl}u}},' for a, c, f, fl in d['extras']]
            out.append('};')
        if d['holes']:
            if d['file'] in HOLES_BLANK_BY_DESIGN:
                out.append(f'/* {d["file"]}: {sum(c for _s, c in d["holes"])} page slots are blank by design, the owner\'s rule "rebuild arcade-style": '
                           f'{HOLES_BLANK_BY_DESIGN[d["file"]][1]} (stages.py HOLES_BLANK_BY_DESIGN has the evidence). */')
            out.append(f'static const struct arcade_stage_hole holes_{s}[] = {{')
            out += [f'    {{{a}u, {c}u}},' for a, c in d['holes']]
            out.append('};')
        if d['object_runs']:
            blank = sum(c for f, c, _fl in d['object_runs'] if f == RUN_BLANK)
            if blank:
                out.append(f'/* {d["file"]}: {blank} of the {sum(c for _f, c, _fl in d["object_runs"])} object tiles are blank by design, the owner\'s rule "rebuild arcade-style":')
                out.append(f' * {BLANK_BY_DESIGN[d["file"]][1]} (stages.py BLANK_BY_DESIGN has the evidence). The chunk is served whole, built from the arcade alone. */')
            out.append(f'static const struct arcade_stage_run objects_{s}[] = {{')
            out += [f'    {{{c_first(f)}, {c}u, {fl}u}},' for f, c, fl in d['object_runs']]
            out.append('};')
        if d.get('more'):
            if d.get('more_design') and sum(d['more_design']):
                out.append(f'/* {d["file"]}: {d["more_design"][0]} of the extra palettes are the nearest arcade row (at most {APPROX_MAX_DIFF} of 15 entries differ from the disc\'s row) and '
                           f'{d["more_design"][1]} are blank (no arcade row is that near), by design, the owner\'s rule "rebuild arcade-style"; those are served owned. */')
            out.append(f'static const struct arcade_stage_more more_{s}[] = {{')
            out += [f'    {{{k}u, {c}u, {st}u, {MORE_NAMES[kind]}, {arg:#x}u}},' for k, st, c, kind, arg in d['more']]
            out.append('};')
        out.append('')
    out.append('static const struct arcade_stage_run shared_runs[] = {')
    out += [f'    {{{c_first(f)}, {c}u, {fl}u}},' for f, c, fl in shared]
    sp = shared_pal or (0, [], (0, 0))
    pal_size, pal_runs, pal_design = sp if len(sp) == 3 else (sp[0], sp[1], (0, 0))
    if pal_runs:
        if any(r[3] in (MORE_APPROX, MORE_BLANK) for r in pal_runs):
            out += ['};', '', f'/* The second palette chunk ({pal_size // 32} palettes, the same in every stage file): {sum(r[2] for r in pal_runs if r[3] in (MORE_ROW, MORE_FILL))} are arcade rows or filler rows, equal to the disc\'s;',
                    f' * {pal_design[0]} are the arcade row that stands for a row of the disc that differs from it and {pal_design[1]} are blank, by design, the owner\'s rule "rebuild arcade-style"',
                    ' * (stages.py SHARED_PALETTES_BY_DESIGN has the rows and the evidence); those are served owned, the rest equal. */']
            out.append('static const struct arcade_stage_more shared_pal_more[] = {')
        else:
            out += ['};', '', 'static const struct arcade_stage_more shared_pal_more[] = {']
        out += [f'    {{{k}u, {c}u, {st}u, {MORE_NAMES[kind]}, {arg:#x}u}},' for k, st, c, kind, arg in pal_runs]
    out += ['};', '', 'const struct arcade_stage_run *const arcade_stage_shared = shared_runs;',
            f'const unsigned arcade_stage_shared_count = {len(shared)}u;',
            f'const struct arcade_stage_more *const arcade_stage_shared_pal = {"shared_pal_more" if pal_runs else "0"};',
            f'const unsigned arcade_stage_shared_pal_count = {len(pal_runs)}u;',
            f'const unsigned arcade_stage_shared_pal_bytes = {pal_size}u;', '',
            'const struct arcade_stage arcade_stages[] = {']
    for d in descs:
        s = f'{d["stage"]:02x}'
        sz = d['sizes']
        out.append(f'    {{"{d["file"]}", {d["stage"]}u, {{{sz["module"]}u, {sz["pages"]}u, {sz["palettes"]}u, {sz["object tiles"]}u}},')
        out.append(f'     {{{d["pal_exact"] & (2 ** 64 - 1):#x}ull, {d["pal_exact"] >> 64:#x}ull}},')
        out.append('     {')
        for n, layer in enumerate(d['layers']):
            exc, ck, tk = layer.get('exc'), layer.get('cell_known'), layer.get('tile_known')
            start = 0 if layer['flags'] & START_RULE else layer['start']
            xms = layer.get('xmaps') or []
            out.append(f'         {{{layer["map"]}u, {layer["across"]}u, {layer["down"]}u, {len(layer["ids"])}u, {start}u, {layer["flags"]}u, ids_{s}_{n},'
                       f' {f"exc_{s}_{n}" if exc else "0"}, {len(exc or [])}u, {f"ck_{s}_{n}" if ck else "0"}, {f"tk_{s}_{n}" if tk else "0"},'
                       f' {f"xmaps_{s}_{n}" if xms else "0"}, {len(xms)}u,'
                       f' {f"oc_{s}_{n}" if layer.get("own_cells") else "0"}, {len(index_spans(layer.get("own_cells", [])))}u,'
                       f' {f"ot_{s}_{n}" if layer.get("own_tiles") else "0"}, {len(index_spans(layer.get("own_tiles", [])))}u,'
                       f' {f"ls_{s}" if n == 0 and layer.get("l0own", {}).get("skip") else "0"}, {len(layer.get("l0own", {}).get("skip", [])) if n == 0 else 0}u,'
                       f' {f"lr_{s}" if n == 0 and layer.get("l0own", {}).get("reserved") else "0"}, {len(layer.get("l0own", {}).get("reserved", [])) if n == 0 else 0}u}},')
        out.append('     },')
        out.append(f'     {"extras_" + s if d["extras"] else "0"}, {len(d["extras"])}u, {"holes_" + s if d["holes"] else "0"}, {len(d["holes"])}u,')
        out.append(f'     {"objects_" + s if d["object_runs"] else "0"}, {len(d["object_runs"])}u,')
        out.append(f'     {"more_" + s if d.get("more") else "0"}, {len(d.get("more") or [])}u}},')
    out += ['};', f'const unsigned arcade_stage_count = {len(descs)}u;', '']
    return '\n'.join(out)


MORE_NAMES = {MORE_ROW: 'ARCADE_STAGE_MORE_ROW', MORE_FILL: 'ARCADE_STAGE_MORE_FILL', MORE_APPROX: 'ARCADE_STAGE_MORE_APPROX', MORE_BLANK: 'ARCADE_STAGE_MORE_BLANK'}


def c_first(f):
    return {RUN_ZERO: 'ARCADE_STAGE_ZERO', RUN_DISC: 'ARCADE_STAGE_DISC', RUN_BLANK: 'ARCADE_STAGE_BLANK'}.get(f, f'{f}u')


# ---------------------------------------------------------------- the C build, compared (--compare-c)

class CPiece(ctypes.Structure):
    _fields_ = [('data', ctypes.POINTER(ctypes.c_uint8)), ('known', ctypes.POINTER(ctypes.c_uint8)), ('size', ctypes.c_uint32), ('owned', ctypes.c_int), ('own', ctypes.POINTER(ctypes.c_uint8))]


class COut(ctypes.Structure):
    _fields_ = [('module', CPiece), ('pages', CPiece), ('palettes', CPiece), ('object_tiles', CPiece), ('object_palettes', CPiece)]


PIECE_NAMES = (('module', 'module'), ('pages', 'pages'), ('palettes', 'palettes'), ('object tiles', 'object_tiles'), ('object palettes', 'object_palettes'))


class CLib:
    """stage.c and the checked-in stages_table.c built as a shared library (warnings are errors)."""

    def __init__(self, tmp, prog, gfx):
        here = Path(__file__).resolve().parent
        lib = Path(tmp) / 'stage.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC', '-o', str(lib),
                        str(here / 'stage.c'), str(here / 'stages_table.c'), str(here / 'stage_objects.c'), str(here / 'stage_objects_table.c')], check=True, capture_output=True, timeout=120)
        self.lib, self.prog, self.gfx = ctypes.CDLL(str(lib)), prog, gfx
        c = ctypes
        self.lib.arcade_stage_find.restype = c.c_void_p
        self.lib.arcade_stage_find.argtypes = [c.c_char_p]
        self.lib.arcade_stage_build.argtypes = [c.c_char_p, c.c_size_t, c.c_char_p, c.c_size_t, c.c_void_p, c.c_void_p, c.c_char_p, c.c_size_t]
        self.lib.arcade_stage_free.argtypes = [c.c_void_p]
        self.lib.arcade_stage_build_shared.argtypes = [c.c_char_p, c.c_size_t, c.c_void_p, c.c_char_p, c.c_size_t]
        self.lib.arcade_stage_piece_free.argtypes = [c.c_void_p]
        self.lib.arcade_stage_build_shared_palettes.argtypes = [c.c_char_p, c.c_size_t, c.c_void_p, c.c_char_p, c.c_size_t]

    @staticmethod
    def take(piece):
        n = piece.size
        return ctypes.string_at(piece.data, n), ctypes.string_at(piece.known, n)

    def build(self, name):
        """({piece: (data, known)}, seconds) of the file's table row, or a message (also when the table has no such file)."""
        row = self.lib.arcade_stage_find(name.encode())
        if not row:
            return 'not in the C table'
        out, err = COut(), ctypes.create_string_buffer(256)
        t0 = time.perf_counter()
        rc = self.lib.arcade_stage_build(self.prog, len(self.prog), self.gfx, len(self.gfx), row, ctypes.byref(out), err, len(err))
        took = time.perf_counter() - t0
        if rc:
            return err.value.decode()
        try:
            res = {label: self.take(getattr(out, field)) for label, field in PIECE_NAMES}
            res['owned'] = {label for label, field in PIECE_NAMES if getattr(out, field).owned}
            res['own'] = {label: ctypes.string_at(getattr(out, field).own, getattr(out, field).size) for label, field in PIECE_NAMES if label in ('palettes', 'module', 'pages')}
            return res, took
        finally:
            self.lib.arcade_stage_free(ctypes.byref(out))

    def shared_palettes(self):
        """((data, known), seconds) of the second palette chunk the C builds, or a message."""
        out, err = CPiece(), ctypes.create_string_buffer(256)
        t0 = time.perf_counter()
        if self.lib.arcade_stage_build_shared_palettes(self.prog, len(self.prog), ctypes.byref(out), err, len(err)):
            return err.value.decode()
        try:
            return self.take(out), time.perf_counter() - t0, ctypes.string_at(out.own, out.size)
        finally:
            self.lib.arcade_stage_piece_free(ctypes.byref(out))

    def shared(self):
        out, err = CPiece(), ctypes.create_string_buffer(256)
        t0 = time.perf_counter()
        if self.lib.arcade_stage_build_shared(self.gfx, len(self.gfx), ctypes.byref(out), err, len(err)):
            return err.value.decode()
        try:
            return self.take(out), time.perf_counter() - t0
        finally:
            self.lib.arcade_stage_piece_free(ctypes.byref(out))


def known_equal(piece, disc):
    """(known bytes, True when every known byte equals the disc's) of a built (data, known) piece."""
    data, known = piece
    if len(data) != len(disc):
        return sum(known), False
    return sum(known), all(a == b for a, b, k in zip(data, disc, known) if k)


def xmap_report(d, built, parts):
    """(cells, cells the build marks known and equal to the disc's, the different page tiles those cells use, of them known and equal
    to the disc's) over the extra maps of all of a descriptor's layers, for a build's pieces and the disc's."""
    data, known = built['module']
    pdata, pknown = built['pages']
    disc, dpages = parts['module'], parts['pages']
    cells = equal = 0
    slots = {}
    for n, layer in enumerate(d['layers']):
        side, bc = (16, 16, 32)[n], (16, 16, 8)[n]
        for xi, xm in enumerate(layer.get('xmaps', ())):
            for h in range(len(xm['ids'])):
                page, half = divmod(h, 2)
                for y in range(bc):
                    for x in range(bc):
                        at = cell_offset(layer, n, XKEY * (xi + 1) + half * bc + x, page * bc + y)
                        cells += 1
                        if all(known[at:at + 4]) and data[at:at + 4] == disc[at:at + 4]:
                            equal += 1
                            slot = slot_of_code(struct.unpack_from('<H', data, at)[0], side)
                            if slot is not None and slot_ok(pdata, slot, side):
                                slots[(slot, side)] = 1
    good = 0
    for slot, side in slots:
        at, per = slot_place(slot, side), side // 2
        good += all(pknown[at + y * 128 + k] and pdata[at + y * 128 + k] == dpages[at + y * 128 + k] for y in range(side) for k in range(per))
    return cells, equal, len(slots), good


def xmap_text(d, built, parts):
    cells, equal, slots, good = xmap_report(d, built, parts)
    if not cells:
        return ''
    ids = '; '.join(f'layer {n} map {xm["map"]} blocks {xm["ids"]}' for n, layer in enumerate(d['layers']) for xm in layer.get('xmaps', ()))
    return f'; extra maps: {equal}/{cells} cells known and equal to the disc\'s, {good}/{slots} of their page slots ({ids})'


def owned_text(built, parts, d):
    """What is served whole as the loader's own: the pieces in built['owned'], with how many of their bytes are blank by design."""
    text = ''
    if built['owned']:
        blank = 128 * sum(c for f, c, _fl in d['object_runs'] if f == RUN_BLANK) if d else 0
        text += '; owned (built whole from the arcade, served to every read): ' + ', '.join(
            f'{label} {len(parts[label])} bytes, {blank} of them blank by design' for label in sorted(built['owned']))
    for label, what in (('palettes', 'the nearest arcade row or blank'), ('module', 'cells as the arcade has them'), ('pages', 'tiles as the arcade has them')):
        own = sum(1 for b in built['own'][label] if b)
        if own:
            text += f'; {label} by design ({what}, served owned): {own} bytes'
    return text


def model_line(name, built, parts, rule=None, d=None):
    """One line: for each piece how many bytes the model builds and knows to be the disc's, and whether those bytes are."""
    items, ok = [], True
    for label, _field in PIECE_NAMES:
        disc = parts.get(label, b'')
        k, same = known_equal(built[label], disc)
        ok &= same
        items.append(f'{label} {k}/{len(disc)}')
    return (f'{name} model: ' + ', '.join(items) + ('; the known bytes equal the disc' if ok else '; KNOWN BYTES DIFFER from the disc') +
            (f'; layer starts by the packing rule: layer 1 {"yes" if rule[0] else "no"}, layer 2 {"yes" if rule[1] else "no"}' if rule else '') +
            (xmap_text(d, built, parts) if d else '') + owned_text(built, parts, d))


def compare_line(name, built, model, parts, took, d=None):
    """One line: the C build against the model (data and known, every piece) and, at the known bytes, against the disc."""
    if isinstance(built, str):
        return f'{name}: C refused: {built}'
    same_model = [label for label, _f in PIECE_NAMES if built[label] != model[label]] + (['owned'] if built['owned'] != model['owned'] else []) + (['own'] if {k: bytes(v) for k, v in built['own'].items()} != {k: bytes(v) for k, v in model['own'].items()} else [])
    items, ok = [], True
    for label, _field in PIECE_NAMES:
        disc = parts.get(label, b'')
        k, same = known_equal(built[label], disc)
        ok &= same
        items.append(f'{label} {k}/{len(disc)}' + ('' if same else ' DIFFERS'))
    return (f'{name} C: ' + ('= model (all pieces, bytes and known)' if not same_model else 'DIFFERS from the model in ' + ', '.join(same_model)) +
            '; = disc at the known bytes: ' + ', '.join(items) + (xmap_text(d, built, parts) if d else '') + owned_text(built, parts, d) + f'; {took * 1000:.1f} ms')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one stage, by number in hex (00 to 13)')
    ap.add_argument('--detail', action='store_true', help='extra lines: arcade block numbers, first block that differs, palette tables')
    ap.add_argument('--model', action='store_true', help='build every stage from the arcade with the model and report, per piece, how many bytes are proved equal to the disc')
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/stages_table.c (the descriptors of the stages the runtime C builds)')
    ap.add_argument('--compare-c', action='store_true', help='build stage.c with the checked-in table and compare it with the model and the disc, per stage')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='stages-')
    build = args.model or args.c_table or args.compare_c
    if args.c_table and args.only:
        ap.error('--c-table writes the whole table; drop --only')
    try:
        prog, gfx = arcade_views(args.arcade)
        helper = Helper(tmp, gfx, prog)
        arc = Arc(prog, gfx)
        rows, omitted, shared_parts, pal_parts = [], [], [], []
        with open(args.disc, 'rb') as f:
            files, sec = iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/STAGE') and p.endswith('.PAC'))
            if args.only:
                names = [p for p in names if p.endswith('STAGE' + args.only.upper() + '.PAC')]
            for p in names:
                lba, size = files[p]
                data = sec(lba, (size + 2047) // 2048)[:size]
                name = p.split('/')[-1][:-4]
                try:
                    parts = split_stage(data)
                except (pac.FormatError, struct.error) as exc:
                    print(f'{name}: {exc}')
                    continue
                if isinstance(parts, str):
                    print(f'{name}: {parts}')
                    continue
                r = analyse(parts, helper, int(name[5:], 16), prog)
                if build:
                    d = describe(name + '.PAC', int(name[5:], 16), parts, arc, helper, r)
                    shared_parts.append(parts['effect tiles'])
                    pal_parts.append(parts['shared palettes'])
                    if isinstance(d, str):
                        omitted.append((name + '.PAC', d))
                        print(f'{name}: not described: {d}', file=sys.stderr if args.c_table else sys.stdout)
                    else:
                        rows.append((d, parts))
                    continue
                print(line(name, r))
                if args.detail:
                    for d in details(name, r):
                        print(d)
        if build:
            shared = shared_runs(shared_parts, helper)
            if isinstance(shared, str):
                print(shared, file=sys.stderr)
                return 1
            import stage_objects                      # the grids of each module that the arcade builds equal to the disc's: the model builds them too
            octx = stage_objects.context(prog, gfx)
            for d, parts in rows:
                d['shared_runs'] = shared
                d['grids'] = stage_objects.grid_rows(parts['module'], d['object_runs'], shared, octx[1], octx[2], octx[3])[0]
            pal_chunk = pal_parts[0] if pal_parts and all(p == pal_parts[0] for p in pal_parts) else None
            pal_runs, pal_design = [], (0, 0)
            if pal_chunk:
                pal_runs = more_runs(arc, pal_chunk, all_more=True)
                design = design_runs(arc, pal_chunk, pal_runs, True, SHARED_PALETTES_BY_DESIGN)
                if isinstance(design, str):
                    omitted.append(('the second palette chunk', design))
                    pal_chunk = None
                else:
                    pal_runs, pal_design = sorted(pal_runs + design[0]), design[1]
            if pal_parts and pal_chunk is None:
                omitted.append(('the second palette chunk', "the stage files' chunks differ"))
            text = c_table([d for d, _p in rows], shared, omitted, (len(pal_chunk), pal_runs, pal_design) if pal_chunk else None)
            if args.c_table:
                sys.stdout.write(text)
            else:
                shared_total = 128 * sum(c for _f, c, _fl in shared)
                shared_model = tile_run_bytes(arc, shared, shared_total)
                shared_disc = shared_parts[0] if shared_parts else b''
                k, same = known_equal(shared_model, shared_disc)
                print(f'shared tiles model: {k}/{len(shared_disc)} bytes from the arcade' + ('' if same else '; DIFFERS from the disc'))
                if pal_chunk:
                    pal_own = bytearray(len(pal_chunk))
                    pal_model = model_shared_palettes(prog, len(pal_chunk), pal_runs, pal_own)
                    k, same = known_equal(pal_model, pal_chunk)
                    print(f'shared palettes model: {k}/{len(pal_chunk)} bytes from the arcade ({k // 32} of {len(pal_chunk) // 32} palettes), '
                          f'{sum(pal_own)} bytes by design ({pal_design[0]} the nearest arcade row, {pal_design[1]} blank), served owned' + ('' if same else '; DIFFERS from the disc'))
                clib = None
                if args.compare_c:
                    here = Path(__file__).resolve().parent / 'stages_table.c'
                    print('stages_table.c ' + ('is the generated one' if here.read_text() == text else 'DIFFERS from the generated one'))
                    clib = CLib(tmp, prog, gfx)
                total = 0.0
                for d, parts in rows:
                    model = model_stage(arc, d)
                    if not args.compare_c:
                        rs = starts_by_rule(arc, d)
                        print(model_line(d['file'][:-4], model, parts, (rs[0] == d['layers'][1]['start'], rs[1] == d['layers'][2]['start']), d))
                        continue
                    built = clib.build(d['file'])
                    if not isinstance(built, str):
                        total += built[1]
                    print(compare_line(d['file'][:-4], built if isinstance(built, str) else built[0], model, parts, 0 if isinstance(built, str) else built[1], d))
                if args.compare_c:
                    c_shared = clib.shared()
                    if isinstance(c_shared, str):
                        print(f'shared tiles: C refused: {c_shared}')
                    else:
                        k, same = known_equal(c_shared[0], shared_disc)
                        print(f'shared tiles C: ' + ('= model' if c_shared[0] == shared_model else 'DIFFERS from the model') +
                              f'; = disc at the known bytes: {k}/{len(shared_disc)}' + ('' if same else ' DIFFERS') + f'; {c_shared[1] * 1000:.1f} ms')
                    if pal_chunk:
                        c_pal = clib.shared_palettes()
                        if isinstance(c_pal, str):
                            print(f'shared palettes: C refused: {c_pal}')
                        else:
                            k, same = known_equal(c_pal[0], pal_chunk)
                            print('shared palettes C: ' + ('= model' if c_pal[0] == pal_model and c_pal[2] == bytes(pal_own) else 'DIFFERS from the model') +
                                  f'; = disc at the known bytes: {k}/{len(pal_chunk)}' + ('' if same else ' DIFFERS') + f'; {c_pal[1] * 1000:.1f} ms')
                    print(f'C time for {len(rows)} stages: {total * 1000:.1f} ms')
        print(f'{len(names)} files in {time.monotonic() - started:.1f} s', file=sys.stderr if args.c_table else sys.stdout)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
