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
  - layer 0 (16 pixel cells) is made of the arcade's 8x8 pieces: it reports how many of its non-flat 8x8
    pieces are pieces of the arcade's tiles (a piece being a quarter of a 16x16 tile, flips included);
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
static const int NB16 = 154, NB32 = 101;
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
        blocks = helper.match_layer(pages, module, off, across, down, side)
        item['blocks'] = blocks
        r['layers'].append(item)
    for kind in ('object tiles', 'effect tiles'):
        r[kind] = tile_summary(helper.tiles(parts[kind])) if kind in parts else None
    for kind in ('palettes', 'shared palettes', 'object palettes'):
        count, offs = helper.palettes(parts[kind])
        r[kind] = (count, offs, sum(1 for k in range(count) if is_flat_palette(parts[kind], k)))
    r['rows used'] = [len(u) for u in rows_used(module, layers, extents)]
    r['rule'] = palette_agreement(prog, stage, parts['palettes'], rows_used(module, layers, extents))
    r['object rule'] = object_palette_agreement(prog, stage, parts['object palettes'])
    r['first_difference'] = first_palette_difference(prog, stage, parts['palettes'])
    return r


def layer_text(item, n):
    blocks = item['blocks']
    exact = sum(1 for b in blocks if b[2] == (256 if item['side'] == 16 else 64))
    nonflat, nonflat_ok = sum(b[0] for b in blocks), sum(b[4] for b in blocks)
    text = f'L{n} {item["cells"][0] * item["side"]}x{item["cells"][1] * item["side"]}'
    if n == 0:
        text += f' 8x8 pieces {item["pieces"][1]}/{item["pieces"][0]} cells equal {nonflat_ok}/{nonflat}'
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
        if n == 0:
            continue
        ids = [b[1] for b in it['blocks']]
        bad = [(i, b) for i, b in enumerate(it['blocks']) if b[2] != (256 if it['side'] == 16 else 64)]
        out.append(f'  {name} L{n} arcade blocks {ids}' + (f'; first block that differs: #{bad[0][0]} {bad[0][1][2]} cells equal of '
                                                          f'{256 if it["side"] == 16 else 64}' if bad else ''))
    out.append(f'  {name} first palette row that differs from the arcade table, per layer (row, entries): {r["first_difference"]}')
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one stage, by number in hex (00 to 13)')
    ap.add_argument('--detail', action='store_true', help='extra lines: arcade block numbers, first block that differs, palette tables')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='stages-')
    try:
        prog, gfx = arcade_views(args.arcade)
        helper = Helper(tmp, gfx, prog)
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
                print(line(name, r))
                if args.detail:
                    for d in details(name, r):
                        print(d)
        print(f'{len(names)} files in {time.monotonic() - started:.1f} s')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
