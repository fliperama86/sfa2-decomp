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
RUN_ZERO, RUN_DISC = 0xfffffffe, 0xffffffff   # a run of tiles: transparent ones, or ones the arcade does not have
MAP_EXACT, TILES_EXACT, START_RULE = 1, 2, 4   # flags of a layer: its map, its tiles in the pages, equal the disc's; its start is the packing rule's
FORCE_NEW = 0xffffffff                      # an exception slot: the cell takes a new slot though an equal tile exists
MORE_ROW, MORE_FILL = 0, 1                  # kinds of run of the 'more' palettes: arcade rows from an address, or filler rows
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
    is zeros (known), a run of RUN_DISC tiles is zeros (not known); a run of arcade tiles is those tiles, flipped."""
    out, known, at = bytearray(total), bytearray(total), 0
    for first, count, flip in runs:
        if at + 128 * count > total:
            raise ValueError('tile runs longer than the chunk')
        if first == RUN_DISC:
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


def model_palettes(prog, stage, count, exact, more=()):
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
    for k0, step, n, kind, arg in more:
        for i in range(n):
            k = k0 + step * i
            if k >= count:
                raise ValueError('a palette run leaves the chunk')
            if kind == MORE_FILL:
                words = [FILL_COLOUR] * 15
            else:
                if arg + 0x20 * i + 32 > len(prog):
                    raise ValueError('a palette row lies outside the arcade program')
                words = struct.unpack_from('>15H', prog, arg + 0x20 * i)
            struct.pack_into('<16H', out, 32 * k, 0, *[arcade_color_to_ps1(w) or 0x8000 for w in words])
            known[32 * k:32 * k + 32] = b'\1' * 32
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


def model_stage_pieces(arc, d):
    sizes = d['sizes']
    module, module_known = bytearray(sizes['module']), bytearray(sizes['module'])
    pages, pages_known = bytearray(sizes['pages']), bytearray(b'\1' * sizes['pages'])   # known, but for the holes and the tiles that differ
    counts = [0, 0, 0, 0]               # tiles of the layers; counts[3] the start of layer 1 (for layer 2's rule)
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        cells = layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], d['stage'])
        start = layer_start(d, n, counts) if n else layer['start']
        if n == 1:
            counts[3] = start
        words, tiles = place_layer(cells, n, start, dict(layer.get('exc', ())))
        counts[n] = len(tiles)
        cs = known_cell_set(layer, cells)
        if not map_bytes(module, layer['map'], layer['across'], side, words, module_known if (layer['flags'] & MAP_EXACT or cs) else None, cs):
            raise ValueError(f'layer {n} map does not fit the module')
        tmask = layer.get('tile_known', 0)
        for j, tile in enumerate(tiles):
            if tile is None:
                continue
            if not put_slot(pages, start + j, side, tile):
                raise ValueError(f'layer {n} tile {j} does not fit the pages')
            if not layer['flags'] & TILES_EXACT and not (j <= len(cells) and (tmask >> j) & 1):
                mark_slot(pages_known, start + j, side, 0)
    for slot, count, first, flip in d['extras']:
        for k in range(count):
            if not arc.has_tile(first + k) or not put_slot(pages, slot + k, 16, arc.tile(first + k, 16, flip & 1, flip & 2)):
                raise ValueError('an extra tile does not fit the pages or the arcade graphics')
    for slot, count in d['holes']:
        for k in range(count):
            if slot_ok(pages, slot + k, 16):
                mark_slot(pages_known, slot + k, 16, 0)
    objects = tile_run_bytes(arc, d['object_runs'], sizes['object tiles'])
    return {'module': (module, module_known), 'pages': (pages, pages_known),
            'palettes': model_palettes(arc.prog, d['stage'], sizes['palettes'] // 32, d['pal_exact'], d.get('more', ())),
            'object tiles': objects,
            'object palettes': (model_object_palettes(arc.prog, d['stage']), b'\1' * OBJECT_PALETTES_BYTES)}


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


def more_runs(arc, chunk):
    """Runs (k, step, count, kind, arg) that make the palettes of a chunk that no layer table names ('more') from the arcade: palettes
    k, k + step, ... are the arcade rows at arg, arg + 0x20, ... (MORE_ROW), or filler rows (MORE_FILL). Only palettes that equal the
    build byte for byte are covered; the others (the PS1's mixes of rows) are left out. Longest runs first."""
    count = len(chunk) // 32
    cands = {}
    fill = (0,) + (arcade_color_to_ps1(FILL_COLOUR) or 0x8000,) * 15
    index = row_index(arc)
    for k in range(count):
        if palette_group(k, count)[0] != 'more':
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
        d['object_runs'] = tile_runs(parts['object tiles'], idx)
    d['pal_exact'] = 0
    # where the disc's cells differ from the first-use packing, per layer: exceptions; then what the model builds equals the disc's
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        try:
            cells = layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], stage)
        except ValueError as exc:
            return str(exc)
        wants = []
        for cx, cy, _t, _r in cells:
            code = struct.unpack_from('<H', module, layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side))[0]
            slot = slot_of_code(code, side)
            wants.append(None if slot is None or slot < layer['start'] else slot - layer['start'])
        layer['exc'] = find_exceptions(cells, side, wants)
    first = model_stage_raw(arc, d)       # the layers' tiles and maps, to compare
    if isinstance(first, str):
        return first
    maps, tiles = first
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
        cells = layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], stage)
        bits = 0
        for i, (cx, cy, _t, _r) in enumerate(cells):
            at = layer['map'] + 2 * map_word_index(cx, cy, layer['across'], side)
            if struct.unpack_from('<HH', module, at) == maps[n][(cx, cy)]:
                bits |= 1 << i
        tbits = 0
        for j, t in enumerate(tiles[n]):
            if t is not None and j <= len(cells) and slot_ok(pages, layer['start'] + j, side) and get_slot(pages, layer['start'] + j, side) == t:
                tbits |= 1 << j
        every = sum(1 << j for j, t in enumerate(tiles[n]) if t is not None)
        layer['flags'] = (MAP_EXACT if bits == 2 ** len(cells) - 1 else 0) | (TILES_EXACT if tbits == every else 0)
        layer['cell_known'] = 0 if layer['flags'] & MAP_EXACT else bits
        layer['tile_known'] = 0 if layer['flags'] & TILES_EXACT else tbits
    # page slots outside the model's layers that hold arcade tiles
    covered = set()
    for n, layer in enumerate(d['layers']):
        side = (16, 16, 32)[n]
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
        d['holes'] = hole_runs([s for s, m in zip(slots, matches) if m < 0])
    # palettes
    chunk = parts['palettes']
    count = len(chunk) // 32
    for k in range(min(count, 128)):
        layer, row = palette_group(k, count)
        if layer != 'more' and list(struct.unpack_from('<16H', chunk, 32 * k)) == predicted_palette(arc.prog, stage, layer, row):
            d['pal_exact'] |= 1 << k
    d['more'] = more_runs(arc, chunk)
    # the packing rule for the starts of layers 1 and 2
    s1, s2 = starts_by_rule(arc, d)
    if s1 == d['layers'][1]['start']:
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
            cells = layer_cells(arc, n, ids_of(arc, layer), layer['across'], layer['down'], d['stage'])
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

def c_array(name, values, per_line=8, fmt='{:#x}u', ctype='uint32_t'):
    out = [f'static const {ctype} {name}[] = {{']
    for i in range(0, len(values), per_line):
        out.append('    ' + ', '.join(fmt.format(v) for v in values[i:i + per_line]) + ',')
    out.append('};')
    return out


def mask_words(mask, bits):
    """The 64-bit words of a bit mask of `bits` bits."""
    return [(mask >> (64 * i)) & (2 ** 64 - 1) for i in range((bits + 63) // 64)]


def c_table(descs, shared, omitted):
    """The text of stages_table.c: for each described stage its layers' map offsets, sizes and lists of arcade addresses, the exceptions
    of the packing and the known cells and tiles, the page slots of arcade tiles, the object tile runs, the runs of palettes and the
    flags; then the shared tile runs. No byte of either game."""
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
            cells = layer['across'] * layer['down']
            if layer.get('cell_known'):
                out += c_array(f'ck_{s}_{n}', mask_words(layer['cell_known'], cells), 4, '{:#x}ull', 'uint64_t')
            if layer.get('tile_known'):
                out += c_array(f'tk_{s}_{n}', mask_words(layer['tile_known'], cells + 1), 4, '{:#x}ull', 'uint64_t')
        if d['extras']:
            out.append(f'static const struct arcade_stage_slots extras_{s}[] = {{')
            out += [f'    {{{a}u, {c}u, {f}u, {fl}u}},' for a, c, f, fl in d['extras']]
            out.append('};')
        if d['holes']:
            out.append(f'static const struct arcade_stage_hole holes_{s}[] = {{')
            out += [f'    {{{a}u, {c}u}},' for a, c in d['holes']]
            out.append('};')
        if d['object_runs']:
            out.append(f'static const struct arcade_stage_run objects_{s}[] = {{')
            out += [f'    {{{c_first(f)}, {c}u, {fl}u}},' for f, c, fl in d['object_runs']]
            out.append('};')
        if d.get('more'):
            out.append(f'static const struct arcade_stage_more more_{s}[] = {{')
            out += [f'    {{{k}u, {c}u, {st}u, {"ARCADE_STAGE_MORE_FILL" if kind == MORE_FILL else "ARCADE_STAGE_MORE_ROW"}, {arg:#x}u}},' for k, st, c, kind, arg in d['more']]
            out.append('};')
        out.append('')
    out.append('static const struct arcade_stage_run shared_runs[] = {')
    out += [f'    {{{c_first(f)}, {c}u, {fl}u}},' for f, c, fl in shared]
    out += ['};', '', 'const struct arcade_stage_run *const arcade_stage_shared = shared_runs;',
            f'const unsigned arcade_stage_shared_count = {len(shared)}u;', '',
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
            out.append(f'         {{{layer["map"]}u, {layer["across"]}u, {layer["down"]}u, {len(layer["ids"])}u, {start}u, {layer["flags"]}u, ids_{s}_{n},'
                       f' {f"exc_{s}_{n}" if exc else "0"}, {len(exc or [])}u, {f"ck_{s}_{n}" if ck else "0"}, {f"tk_{s}_{n}" if tk else "0"}}},')
        out.append('     },')
        out.append(f'     {"extras_" + s if d["extras"] else "0"}, {len(d["extras"])}u, {"holes_" + s if d["holes"] else "0"}, {len(d["holes"])}u,')
        out.append(f'     {"objects_" + s if d["object_runs"] else "0"}, {len(d["object_runs"])}u,')
        out.append(f'     {"more_" + s if d.get("more") else "0"}, {len(d.get("more") or [])}u}},')
    out += ['};', f'const unsigned arcade_stage_count = {len(descs)}u;', '']
    return '\n'.join(out)


def c_first(f):
    return {RUN_ZERO: 'ARCADE_STAGE_ZERO', RUN_DISC: 'ARCADE_STAGE_DISC'}.get(f, f'{f}u')


# ---------------------------------------------------------------- the C build, compared (--compare-c)

class CPiece(ctypes.Structure):
    _fields_ = [('data', ctypes.POINTER(ctypes.c_uint8)), ('known', ctypes.POINTER(ctypes.c_uint8)), ('size', ctypes.c_uint32)]


class COut(ctypes.Structure):
    _fields_ = [('module', CPiece), ('pages', CPiece), ('palettes', CPiece), ('object_tiles', CPiece), ('object_palettes', CPiece)]


PIECE_NAMES = (('module', 'module'), ('pages', 'pages'), ('palettes', 'palettes'), ('object tiles', 'object_tiles'), ('object palettes', 'object_palettes'))


class CLib:
    """stage.c and the checked-in stages_table.c built as a shared library (warnings are errors)."""

    def __init__(self, tmp, prog, gfx):
        here = Path(__file__).resolve().parent
        lib = Path(tmp) / 'stage.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC', '-o', str(lib),
                        str(here / 'stage.c'), str(here / 'stages_table.c')], check=True, capture_output=True, timeout=120)
        self.lib, self.prog, self.gfx = ctypes.CDLL(str(lib)), prog, gfx
        c = ctypes
        self.lib.arcade_stage_find.restype = c.c_void_p
        self.lib.arcade_stage_find.argtypes = [c.c_char_p]
        self.lib.arcade_stage_build.argtypes = [c.c_char_p, c.c_size_t, c.c_char_p, c.c_size_t, c.c_void_p, c.c_void_p, c.c_char_p, c.c_size_t]
        self.lib.arcade_stage_free.argtypes = [c.c_void_p]
        self.lib.arcade_stage_build_shared.argtypes = [c.c_char_p, c.c_size_t, c.c_void_p, c.c_char_p, c.c_size_t]
        self.lib.arcade_stage_piece_free.argtypes = [c.c_void_p]

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
            return {label: self.take(getattr(out, field)) for label, field in PIECE_NAMES}, took
        finally:
            self.lib.arcade_stage_free(ctypes.byref(out))

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


def model_line(name, built, parts, rule=None):
    """One line: for each piece how many bytes the model builds and knows to be the disc's, and whether those bytes are."""
    items, ok = [], True
    for label, _field in PIECE_NAMES:
        disc = parts.get(label, b'')
        k, same = known_equal(built[label], disc)
        ok &= same
        items.append(f'{label} {k}/{len(disc)}')
    return (f'{name} model: ' + ', '.join(items) + ('; the known bytes equal the disc' if ok else '; KNOWN BYTES DIFFER from the disc') +
            (f'; layer starts by the packing rule: layer 1 {"yes" if rule[0] else "no"}, layer 2 {"yes" if rule[1] else "no"}' if rule else ''))


def compare_line(name, built, model, parts, took):
    """One line: the C build against the model (data and known, every piece) and, at the known bytes, against the disc."""
    if isinstance(built, str):
        return f'{name}: C refused: {built}'
    same_model = [label for label, _f in PIECE_NAMES if built[label] != model[label]]
    items, ok = [], True
    for label, _field in PIECE_NAMES:
        disc = parts.get(label, b'')
        k, same = known_equal(built[label], disc)
        ok &= same
        items.append(f'{label} {k}/{len(disc)}' + ('' if same else ' DIFFERS'))
    return (f'{name} C: ' + ('= model (all pieces, bytes and known)' if not same_model else 'DIFFERS from the model in ' + ', '.join(same_model)) +
            '; = disc at the known bytes: ' + ', '.join(items) + f'; {took * 1000:.1f} ms')


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
        rows, omitted, shared_parts = [], [], []
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
            text = c_table([d for d, _p in rows], shared, omitted)
            if args.c_table:
                sys.stdout.write(text)
            else:
                shared_total = 128 * sum(c for _f, c, _fl in shared)
                shared_model = tile_run_bytes(arc, shared, shared_total)
                shared_disc = shared_parts[0] if shared_parts else b''
                k, same = known_equal(shared_model, shared_disc)
                print(f'shared tiles model: {k}/{len(shared_disc)} bytes from the arcade' + ('' if same else '; DIFFERS from the disc'))
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
                        print(model_line(d['file'][:-4], model, parts, (rs[0] == d['layers'][1]['start'], rs[1] == d['layers'][2]['start'])))
                        continue
                    built = clib.build(d['file'])
                    if not isinstance(built, str):
                        total += built[1]
                    print(compare_line(d['file'][:-4], built if isinstance(built, str) else built[0], model, parts, 0 if isinstance(built, str) else built[1]))
                if args.compare_c:
                    c_shared = clib.shared()
                    if isinstance(c_shared, str):
                        print(f'shared tiles: C refused: {c_shared}')
                    else:
                        k, same = known_equal(c_shared[0], shared_disc)
                        print(f'shared tiles C: ' + ('= model' if c_shared[0] == shared_model else 'DIFFERS from the model') +
                              f'; = disc at the known bytes: {k}/{len(shared_disc)}' + ('' if same else ' DIFFERS') + f'; {c_shared[1] * 1000:.1f} ms')
                    print(f'C time for {len(rows)} stages: {total * 1000:.1f} ms')
        print(f'{len(names)} files in {time.monotonic() - started:.1f} s', file=sys.stderr if args.c_table else sys.stdout)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
