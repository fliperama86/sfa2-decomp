#!/usr/bin/env python3
"""Reference model of the screens' arcade-built data (the specification of port/arcade/screens.c), and the tools that describe the disc's files in it.

What it is. The select, demo, continue, ending, boss and character-demo screens read from their archives compressed sprite tiles, palettes, and
pictures of 16 x 16 cells. Most of these are arcade tiles and palettes (other_files.py measures how much). This module is the model of how the
runtime builds them from the arcade set, and the analysis that finds, for each chunk of a disc file, the recipe that rebuilds what the arcade has.
A chunk's recipe is a list of REGIONS (byte ranges of the chunk); what lies outside the regions stays the disc's (headers, code, the PS1's own art).

  seg   a compressed tile segment (offset table, then a block a tile; tilepack.py): a list of tile ITEMS builds the tiles, the runtime packs them
        no larger than the room (the disc segment's size) with tilepack.c. Items: RUN (first arcade tile, count, flip), ZERO (blank tiles),
        PIECES (a tile of four 8 x 8 pieces of the arcade's scroll 1 graphics) and DISC (tiles the arcade does not have: the runtime takes them
        from the disc's segment). Known bytes of a seg region are 2 ("the tiles equal the disc's") when no tile is missing or the disc was given.
  raw   a plain list of 128-byte tiles (the same items; DISC tiles stay unknown).
  pals  palettes of 16 colours, 32 bytes each: runs of arcade palettes at a stride of 32 bytes, with flags (colour 0 is 0, then the 15 colours with
        each channel doubled; black is 0x8000 unless flag 1; flag 2 sets bit 15 of every colour), ZERO palettes, DISC palettes.
  pic   a picture of 4 bits a pixel, `width` pixels wide, in rows: blank unless drawn by OPS: tile rects (16 x 16 arcade tiles a cell, the code
        stepping by stepx across and stepy down), piece rects (8 x 8 pieces of scroll 1: piece k is the left half of arcade tile k >> 1, the top
        rows for an even k, the bottom for an odd one), flat rects, DISC rects (stay the disc's, unknown) and BLANK rects (zero by design).

BY DESIGN. A ZERO item whose flip carries BY_DESIGN, and a BLANK op, are blank on purpose where the disc is not: the units differ from the disc's and are
        taken from nothing. They are listed in BY_DESIGN below, one explicit row each (what, where, how many units, why), never found by a heuristic. Their bytes are
        known 2 (served as built, not compared with the disc) outside segments, and a segment that has no DISC tile left is built whole; the C counts them.

Colour index: the PS1's is the arcade's plus one (the arcade's 15, transparent, is 0). Flip bit 0 mirrors across, bit 1 down.

Inputs of the tools: the disc image (--disc) and the arcade set's ZIP (--arcade), read only. They write nothing but a temporary folder with a small
C helper that is removed on exit. The runtime converter is C (screens.c); this file is the model it reproduces and the way its table is made.
"""
import collections
import ctypes
import hashlib
import struct
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import tilepack  # noqa: E402

ZERO = 0xfffffffe          # an item: count blank tiles / palettes
DISC = 0xffffffff          # an item: count tiles / palettes the arcade does not have
PIECES = 0xfffffffd        # an item: one tile of four pieces, flip = index into the pieces list
WINDOW = 0xfffffffc        # an item: one tile cut from a drawn frame of the arcade, flip = a window (kind, address, bank, blocks, x, y, flip, rows) or its index
SRC_RECORD, SRC_SCROLL2 = 1, 2   # the kinds of source of a window
PAL_BLACK_ZERO = 1         # palette flag: a black colour stays 0
PAL_BIT15 = 2              # palette flag: bit 15 of colours 1 to 15 is set
FLAT_BASE = 0xfffffff0     # a piece id (k << 2 | flip) at or above this is a flat piece of value id & 15

BY_DESIGN = 0x80000000     # a flip bit of an item: blank on purpose (ZERO), or the arcade's tile where the PS1 edited its own (a run, a WINDOW)
OP_TILE, OP_PIECE, OP_FLAT, OP_DISC, OP_BLANK = 1, 2, 3, 4, 5
SEG, RAW, PALS, PIC = 1, 2, 3, 4
KIND_NAMES = {SEG: 'seg', RAW: 'raw', PALS: 'pals', PIC: 'pic'}

TILE_BYTES, PIECE_BYTES, PALETTE_BYTES = 128, 32, 32
MAX_CAND = 6               # candidates tried for a seed


# ---------------------------------------------------------------- pixels (the model of the C)

def flip_pixels(data, side, flip):
    """The packed pixels of a side x side block (side / 2 bytes a row, low nibble first) flipped across (bit 0) and/or down (bit 1)."""
    if not flip:
        return bytes(data)
    row = side // 2
    px = [b >> (4 * i) & 15 for b in data for i in (0, 1)]
    out = [px[(side - 1 - y if flip & 2 else y) * side + (side - 1 - x if flip & 1 else x)] for y in range(side) for x in range(side)]
    return bytes(out[2 * k] | out[2 * k + 1] << 4 for k in range(side * row))


class Arc:
    """The arcade graphics as the screens use them: tile(code, flip) the 128 PS1 bytes of arcade tile `code` (colour plus one), piece(k, flip) the
    32 bytes of scroll 1 piece k, None when the code lies outside the graphics."""

    def __init__(self, gfx, prog=None):
        self.gfx, self.prog = gfx, prog         # prog: the arcade program as the 68000 sees it, for the frames the WINDOW items cut from
        self.count = len(gfx) // TILE_BYTES
        self._tile, self._piece, self._canvas = {}, {}, {}

    def tile(self, code, flip=0):
        key = code, flip
        r = self._tile.get(key)
        if r is None and 0 <= code < self.count:
            base = self._tile.get((code, 0))
            if base is None:
                base = self._tile[(code, 0)] = tilepack.arcade_tile(self.gfx, code)
            r = self._tile[key] = base if not flip else tilepack.flip_tile(base, flip)
        return r

    def piece(self, k, flip=0):
        key = k, flip
        r = self._piece.get(key)
        if r is None:
            t = self.tile(k >> 1)
            if t is None:
                return None
            r = b''.join(t[(y + (k & 1) * 8) * 8:(y + (k & 1) * 8) * 8 + 4] for y in range(8))
            r = self._piece[key] = flip_pixels(r, 8, flip)
        return r


def flat_piece(v):
    return bytes([(v & 15) * 17]) * PIECE_BYTES


def piece_of(arc, pid):
    """The 32 bytes of a piece id (k << 2 | flip, or a flat id), or None outside the graphics."""
    if pid >= FLAT_BASE:
        return flat_piece(pid)
    return arc.piece(pid >> 2, pid & 3)


def tile_of_pieces(parts):
    """A 128-byte tile from four 32-byte pieces: top left, top right, bottom left, bottom right."""
    return b''.join(parts[(y >> 3) * 2][(y & 7) * 4:(y & 7) * 4 + 4] + parts[(y >> 3) * 2 + 1][(y & 7) * 4:(y & 7) * 4 + 4] for y in range(16))


def palette_bytes(words, flags):
    """The 32 bytes of the PS1 palette of an arcade palette's first 15 words (the 12-bit colours; the brightness nibble is dropped)."""
    out = [0]
    for w in words:
        c = (w >> 8 & 15) << 1 | (w >> 4 & 15) << 6 | (w & 15) << 11
        if not c and not flags & PAL_BLACK_ZERO:
            c = 0x8000
        out.append(c | (0x8000 if flags & PAL_BIT15 else 0))
    return struct.pack('<16H', *out)


# ---------------------------------------------------------------- the model of a region (items and ops as the C table holds them)

class ModelError(ValueError):
    pass


def check_design(items, palettes=False):
    """Refuse the by-design mark on a DISC or PIECES item, and on a palette item that is not a ZERO item."""
    for first, _count, flip in items:
        if not isinstance(flip, tuple) and flip & BY_DESIGN and (first in (DISC, PIECES) or (palettes and first != ZERO)):
            raise ModelError('the by-design mark is on an item that is not blank or an arcade tile')


def design_kinds(items):
    """[0, 1 or 2] for each tile or palette of a list of items: 0 the disc's, 1 blank by design (it must be blank), 2 an arcade tile by design (not
    compared with the disc)."""
    return [(1 if first == ZERO else 2) if not isinstance(flip, tuple) and flip & BY_DESIGN else 0 for first, count, flip in items for _ in range(count)]


def design_flags(items):
    """[bool] for each tile or palette of a list of items: by design (blank, or the arcade's tile)."""
    return [bool(k) for k in design_kinds(items)]


class Pieces(list):
    """The quads of the PIECES items of a table; `windows` are the (kind, address, bank, blocks, x, y, flip, rows) of its WINDOW items."""
    windows = ()


def window_tile(arc, win):
    """The 128 bytes of the tile a window cuts: the source frame drawn (compose.py), the 16 x 16 cut at (x, y) keeping the rows, then the stored flip."""
    import compose
    kind, address, bank, blocks, x, y, flip, rows = win
    key = (kind, address, bank, blocks)
    if key not in arc._canvas:
        if arc.prog is None:
            raise ModelError('a window needs the arcade program')
        if kind == SRC_RECORD:
            canvas = compose.record_canvas(arc.prog, arc.gfx, address, bank)
        elif kind == SRC_SCROLL2:
            canvas = compose.scroll2_canvas(arc.prog, arc.gfx, address, blocks)
        else:
            raise ModelError('source kind %d' % kind)
        if canvas is None:
            raise ModelError('the frame of a window cannot be drawn')
        arc._canvas[key] = canvas
    if rows not in (0, 1, 2):
        raise ModelError('window keeps rows %d' % rows)
    return flip_pixels(compose.cut(arc._canvas[key], x, y, rows), 16, flip & 3)


def tile_items(arc, items, pieces):
    """[(128 bytes or None for a DISC tile)] of a list of items (first, count, flip)."""
    check_design(items)
    out = []
    for first, count, flip in items:
        if first == DISC:
            out += [None] * count
        elif first == ZERO:
            out += [bytes(TILE_BYTES)] * count
        elif first == PIECES:
            if flip >= len(pieces):
                raise ModelError('tile of pieces %d is not in the table' % flip)
            parts = [piece_of(arc, p) for p in pieces[flip]]
            if None in parts:
                raise ModelError('a piece lies outside the graphics')
            out.append(tile_of_pieces(parts))
        elif first == WINDOW:
            windows = getattr(pieces, 'windows', ())
            win = flip if isinstance(flip, tuple) else (windows[flip & ~BY_DESIGN] if (flip & ~BY_DESIGN) < len(windows) else None)
            if count != 1 or win is None:
                raise ModelError('window %s is not in the table' % (flip,))
            out.append(window_tile(arc, win))
        else:
            for j in range(count):
                t = arc.tile(first + j, flip & 3)
                if t is None:
                    raise ModelError('tile %#x lies outside the graphics' % (first + j))
                out.append(t)
    return out


def palette_items(prog, items):
    """[(32 bytes or None for a DISC palette)] of palette items (address, count, flags)."""
    check_design(items, True)
    out = []
    for first, count, flags in items:
        if first == DISC:
            out += [None] * count
        elif first == ZERO:
            out += [bytes(PALETTE_BYTES)] * count
        else:
            for j in range(count):
                a = first + PALETTE_BYTES * j
                if a + 30 > len(prog):
                    raise ModelError('palette at %#x lies outside the program' % a)
                out.append(palette_bytes(struct.unpack_from('>15H', prog, a), flags))
    return out


def blit(buf, row_bytes, x_px, y_px, data, side):
    """Write a side x side block of packed pixels into a picture (side must keep the block in whole bytes: x_px even)."""
    per = side // 2
    for y in range(side):
        a = (y_px + y) * row_bytes + x_px // 2
        buf[a:a + per] = data[y * per:(y + 1) * per]


def picture_ops(arc, ops, width, size):
    """(data, known) of a picture of `size` bytes `width` pixels wide from its ops (kind, x, y, w, h, first, stepx, stepy, flip); x, y in
    units of 8 pixels, w, h in tiles or pieces."""
    row = width // 2
    if width <= 0 or width % 16 or size % row:
        raise ModelError('picture width %d does not fit %d bytes' % (width, size))
    rows = size // row
    data, known = bytearray(size), bytearray(b'\x01') * size
    for kind, x, y, w, h, first, sx, sy, flip in ops:
        unit = 16 if kind == OP_TILE else 8
        if kind not in (OP_TILE, OP_PIECE, OP_FLAT, OP_DISC, OP_BLANK):
            raise ModelError('picture op kind %d' % kind)
        if (x * 8 + unit * w > width) or (y * 8 + unit * h > rows):
            raise ModelError('picture op outside the picture')
        if kind in (OP_DISC, OP_BLANK):     # in order, as the C does: zero, and unknown (DISC) or by design (BLANK, known 2)
            for yy in range(y * 8, (y + h) * 8):
                a = yy * row + x * 4
                data[a:a + w * 4] = bytes(w * 4)
                known[a:a + w * 4] = bytes([2 if kind == OP_BLANK else 0]) * (w * 4)
            continue
        for j in range(h):
            for i in range(w):
                if kind == OP_TILE:
                    t = arc.tile(first + i * sx + j * sy, flip)
                    if t is None:
                        raise ModelError('tile outside the graphics')
                    blit(data, row, x * 8 + 16 * i, y * 8 + 16 * j, t, 16)
                else:
                    p = flat_piece(first) if kind == OP_FLAT else arc.piece(first + i * sx + j * sy, flip)
                    if p is None:
                        raise ModelError('piece outside the graphics')
                    blit(data, row, x * 8 + 8 * i, y * 8 + 8 * j, p, 8)
    return bytes(data), bytes(known)


# region: (kind, offset, size, extra...) as below
#   (SEG, offset, room, items, pieces)   (RAW, offset, size, items, pieces)   (PALS, offset, size, items, stride)   (PIC, offset, size, width, ops)
# chunk recipe: (table, slot, ordinal, size, regions)

def model_chunk(arc, prog, recipe, disc=None, head_word=None):
    """(data, known, segments) of a chunk recipe. data and known are the chunk's size; a seg region's data is zero (the packer is the C's) and
    its entry in `segments` {offset: tiles} holds the tiles (a DISC tile taken from the disc's segment, None without a disc). known is 1 for
    bytes built equal to the disc's, 2 over a seg region whose tiles are all given and over bytes blank by design, 0 elsewhere. With `head_word` (the chunk's first
    word in the archive header, which the game's disc stream checks the chunk's own against) the bytes 0 to 3 that are 2 and not in a segment become its bytes, known 1."""
    table, slot, ordinal, size, regions = recipe
    data, known, segments = bytearray(size), bytearray(size), {}
    for region in regions:
        kind, offset, rsize = region[:3]
        if offset + rsize > size:
            raise ModelError('region outside the chunk')
        if kind in (SEG, RAW):
            tiles = tile_items(arc, region[3], region[4])
            if kind == SEG:
                got = list(tiles)
                if disc is not None and tilepack_count(disc[offset:offset + rsize]) == len(got):
                    for i, t in enumerate(got):
                        if t is None:
                            d = tilepack_tile(disc[offset:offset + rsize], i)
                            if d is None:
                                break
                            got[i] = d
                segments[offset] = got
                if None not in got:
                    known[offset:offset + rsize] = b'\x02' * rsize
                continue
            if len(tiles) * TILE_BYTES > rsize:
                raise ModelError('raw tiles longer than the region')
            by_design = design_flags(region[3])
            for i, t in enumerate(tiles):
                if t is not None:
                    data[offset + 128 * i:offset + 128 * i + 128] = t
                    known[offset + 128 * i:offset + 128 * i + 128] = bytes([2 if by_design[i] else 1]) * 128
        elif kind == PALS:
            stride = region[4]
            pals = palette_items(prog, region[3])
            if stride < 1 or (len(pals) - 1) * stride * PALETTE_BYTES + PALETTE_BYTES > rsize and pals:
                raise ModelError('palettes longer than the region')
            known[offset:offset + rsize] = b'\x01' * rsize
            by_design = design_flags(region[3])
            for i, p in enumerate(pals):
                a = offset + 32 * stride * i
                if p is None:
                    known[a:a + 32] = bytes(32)
                else:
                    data[a:a + 32] = p
                    if by_design[i]:
                        known[a:a + 32] = b'\x02' * 32
        elif kind == PIC:
            d, k = picture_ops(arc, region[4], region[3], rsize)
            data[offset:offset + rsize] = d
            known[offset:offset + rsize] = k
        else:
            raise ModelError('region kind %d' % kind)
    if head_word is not None and size >= 4:
        for r in regions:
            if r[0] == SEG and r[1] == 0 and known[0] == 2 and 4 * len(segments.get(0, ())) != head_word:
                known[0:r[2]] = bytes(r[2])
        for k in range(4):
            if known[k] == 2 and not any(r[0] == SEG and r[1] <= k < r[1] + r[2] for r in regions):
                data[k] = head_word >> (8 * k) & 255
                known[k] = 1
    return bytes(data), bytes(known), segments


def tilepack_count(chunk):
    """The tile count of a compressed segment's offset table (0 when it is not plausible: the first offset must be the table's own size)."""
    if len(chunk) < 8:
        return 0
    first = struct.unpack_from('<I', chunk, 0)[0]
    return first // 4 if first % 4 == 0 and 4 <= first <= len(chunk) and first // 4 <= 16384 else 0


def tilepack_tile(chunk, index):
    """Tile `index` (128 bytes) of a compressed segment, or None when the segment does not hold it."""
    try:
        n = struct.unpack_from('<I', chunk, 0)[0] // 4
        if index >= n:
            return None
        off = struct.unpack_from('<I', chunk, 4 * index)[0]
        _form, words = tilepack.decode_block(chunk, off)
        return struct.pack('<64H', *words)
    except (ValueError, struct.error):
        return None


# ---------------------------------------------------------------- finding the recipe of a disc chunk (analysis; not part of the runtime)

CAND_SOURCE = r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static uint8_t *ps1t; static int32_t *head16, *next16, *head8, *next8; static uint32_t sz16, sz8;
static uint32_t spread[256];
static uint64_t fnv(const uint8_t *p, int n) { uint64_t h = 1469598103934665603ULL; for (int i = 0; i < n; i++) { h ^= p[i]; h *= 1099511628211ULL; } return h; }
static void flip(const uint8_t *in, uint8_t *out, int f, int side) {
  uint8_t px[256], o[256]; int bytes = side * side / 2;
  for (int i = 0; i < bytes; i++) { px[2 * i] = in[i] & 15; px[2 * i + 1] = in[i] >> 4; }
  for (int y = 0; y < side; y++) for (int x = 0; x < side; x++) o[y * side + x] = px[((f & 2) ? side - 1 - y : y) * side + ((f & 1) ? side - 1 - x : x)];
  for (int k = 0; k < bytes; k++) out[k] = o[2 * k] | o[2 * k + 1] << 4; }
static void half(const uint8_t *t, int q, uint8_t *o) { for (int y = 0; y < 8; y++) memcpy(o + y * 4, t + (y + (q & 1) * 8) * 8, 4); }
static void get16(int32_t id, uint8_t *u) { int f = id & 3; long c = id >> 2; if (f) flip(ps1t + c * 128, u, f, 16); else memcpy(u, ps1t + c * 128, 128); }
static void get8(int32_t id, uint8_t *u) { int f = id & 3; long k = id >> 2; uint8_t b[32]; half(ps1t + (k >> 1) * 128, (int)(k & 1), b); if (f) flip(b, u, f, 8); else memcpy(u, b, 32); }
int build(const uint8_t *gfx, int n) {
  for (int b = 0; b < 256; b++) { uint32_t v = 0; for (int i = 0; i < 8; i++) v |= ((b >> (7 - i)) & 1u) << (4 * i); spread[b] = v; }
  ps1t = malloc((size_t)n * 128);
  for (int c = 0; c < n; c++) { const uint8_t *q = gfx + (size_t)c * 128; uint8_t *o = ps1t + (size_t)c * 128;
    for (int y = 0; y < 16; y++) for (int h = 0; h < 2; h++) { const uint8_t *b = q + y * 8 + h * 4;
      uint32_t v = spread[b[0]] | spread[b[1]] << 1 | spread[b[2]] << 2 | spread[b[3]] << 3;
      v = ((v & 0x77777777u) + 0x11111111u) ^ ((v ^ 0x11111111u) & 0x88888888u); memcpy(o + y * 8 + h * 4, &v, 4); } }
  sz16 = 1u << 20; sz8 = 1u << 22;
  head16 = malloc(sz16 * 4); next16 = malloc((size_t)n * 16); memset(head16, 0xff, sz16 * 4);
  head8 = malloc(sz8 * 4); next8 = malloc((size_t)n * 32); memset(head8, 0xff, sz8 * 4);
  for (int c = n - 1; c >= 0; c--) { uint8_t t[128];
    for (int f = 3; f >= 0; f--) { if (f) flip(ps1t + (size_t)c * 128, t, f, 16); else memcpy(t, ps1t + (size_t)c * 128, 128);
      uint32_t i = fnv(t, 128) & (sz16 - 1); int32_t id = c * 4 + f; next16[id] = head16[i]; head16[i] = id; }
    for (int q = 1; q >= 0; q--) { uint8_t b[32], p[32]; half(ps1t + (size_t)c * 128, q, b);
      for (int f = 3; f >= 0; f--) { if (f) flip(b, p, f, 8); else memcpy(p, b, 32);
        uint32_t i = fnv(p, 32) & (sz8 - 1); int32_t id = (c * 2 + q) * 4 + f; next8[id] = head8[i]; head8[i] = id; } } }
  return 0; }
int cands16(const uint8_t *q, int32_t *out, int max) { int k = 0; uint8_t u[128]; uint32_t i = fnv(q, 128) & (sz16 - 1);
  for (int32_t id = head16[i]; id >= 0; id = next16[id]) { get16(id, u); if (!memcmp(u, q, 128) && k < max) out[k++] = id; } return k; }
int cands8(const uint8_t *q, int32_t *out, int max) { int k = 0; uint8_t u[32]; uint32_t i = fnv(q, 32) & (sz8 - 1);
  for (int32_t id = head8[i]; id >= 0; id = next8[id]) { get8(id, u); if (!memcmp(u, q, 32) && k < max) out[k++] = id; } return k; }
'''


class Candidates:
    """Every arcade tile and scroll 1 piece with its flips, looked up by pixels: c16(tile) lists tile code * 4 + flip, c8(piece) lists piece k * 4 +
    flip (the lowest codes first, at most MAX_CAND). Built from a C helper compiled into `tmp`."""

    def __init__(self, tmp, gfx):
        src = Path(tmp) / 'cand.c'
        src.write_text(CAND_SOURCE)
        lib = Path(tmp) / 'cand.so'
        subprocess.run(['cc', '-O2', '-shared', '-fPIC', '-o', str(lib), str(src)], check=True, capture_output=True, timeout=60)
        self.lib = ctypes.CDLL(str(lib))
        self.lib.build.argtypes = [ctypes.c_char_p, ctypes.c_int]
        for f in (self.lib.cands16, self.lib.cands8):
            f.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int]
        self.gfx = gfx
        self.lib.build(gfx, len(gfx) // TILE_BYTES)
        self.buf = (ctypes.c_int32 * MAX_CAND)()
        self.cache16, self.cache8 = {}, {}

    def c16(self, tile):
        r = self.cache16.get(tile)
        if r is None:
            r = self.cache16[tile] = list(self.buf[:self.lib.cands16(tile, self.buf, MAX_CAND)])
        return r

    def c8(self, piece):
        r = self.cache8.get(piece)
        if r is None:
            r = self.cache8[piece] = list(self.buf[:self.lib.cands8(piece, self.buf, MAX_CAND)])
        return r


def pieces_of(tile):
    """The four 32-byte pieces of a 128-byte tile (top left, top right, bottom left, bottom right)."""
    return [b''.join(tile[(y + (q >> 1) * 8) * 8 + (q & 1) * 4:(y + (q >> 1) * 8) * 8 + (q & 1) * 4 + 4] for y in range(8)) for q in range(4)]


def is_flat(piece):
    return len(set(piece)) == 1 and piece[0] >> 4 == piece[0] & 15


def describe_tiles(tiles, arc, cand, windows=None, edge=False):
    """(items, pieces) that rebuild the list of 128-byte tiles from the arcade as far as it has them: runs of consecutive arcade tiles with one flip,
    blank tiles, tiles of four scroll 1 pieces, WINDOW items (a tile cut from a frame of the arcade: windows.find, screen_windows.py), and DISC runs for the
    rest. Every item but DISC is exactly the disc's tile. With `edge` the tiles that are the arcade's portrait tiles clipped (windows.edge) become runs of
    one tile marked BY_DESIGN: the arcade's whole tile (the chunk must be named by a DESIGN_ROWS row of units 'portrait', which counts them)."""
    n, zero = len(tiles), bytes(TILE_BYTES)
    items, pieces = [], []
    i = 0

    def emit(first, count, flip):
        if first == DISC and items and items[-1][0] == DISC:
            items[-1] = (DISC, items[-1][1] + count, 0)
        else:
            items.append((first, count, flip))

    while i < n:
        t = tiles[i]
        if t == zero:
            j = i
            while j < n and tiles[j] == zero:
                j += 1
            emit(ZERO, j - i, 0)
            i = j
            continue
        best = (0, None)
        for cid in cand.c16(t):
            code, f = cid >> 2, cid & 3
            k = 1
            while i + k < n and arc.tile(code + k, f) == tiles[i + k]:
                k += 1
            if k > best[0]:
                best = (k, cid)
        if best[1] is not None:
            emit(best[1] >> 2, best[0], best[1] & 3)
            i += best[0]
            continue
        ids = []
        for p in pieces_of(t):
            if is_flat(p):
                ids.append(FLAT_BASE | p[0] & 15)
            else:
                c = cand.c8(p)
                if not c:
                    break
                ids.append(c[0])
        if len(ids) == 4:
            pieces.append(tuple(ids))
            emit(PIECES, 1, len(pieces) - 1)
        else:
            win = windows.find(t) if windows is not None else None
            clipped = windows.edge(t) if win is None and edge and windows is not None else None
            if win is not None:
                emit(WINDOW, 1, win)
            elif clipped is not None:
                items.append((clipped[0], 1, clipped[1] | BY_DESIGN))
            else:
                emit(DISC, 1, 0)
        i += 1
    return items, pieces


def palette_options(prog, row, find):
    """[(address, flags)] of the arcade palettes that build exactly this 32-byte row, lowest address first."""
    out = []
    for a in find(row):
        words = struct.unpack_from('>15H', prog, a)
        for flags in (0, PAL_BLACK_ZERO, PAL_BIT15, PAL_BLACK_ZERO | PAL_BIT15):
            if palette_bytes(words, flags) == row:
                out.append((a, flags))
    return out


class PaletteIndex:
    """Finds the arcade palettes (15 words at an even address) that give a PS1 palette, ignoring bit 15 and the black rule."""

    def __init__(self, prog):
        n = len(prog) // 2
        words = struct.unpack('>%dH' % n, prog[:n * 2])
        lut = [(w >> 8 & 15) << 1 | (w >> 4 & 15) << 6 | (w & 15) << 11 for w in range(4096)]
        self.cw = [lut[w & 0xfff] for w in words]
        self.idx = collections.defaultdict(list)
        cw = self.cw
        for i in range(n - 15):
            self.idx[(cw[i], cw[i + 1], cw[i + 2])].append(i)
        self.cache = {}

    def find(self, row):
        r = self.cache.get(row)
        if r is None:
            r = []
            w = struct.unpack('<16H', row)
            if w[0] == 0:
                want = [x & 0x7fff for x in w[1:]]
                for i in self.idx.get((want[0], want[1], want[2]), ()):
                    if self.cw[i:i + 15] == want:
                        r.append(2 * i)
            self.cache[row] = r
        return r


def describe_palettes(rows, prog, pindex):
    """Palette items (address, count, flags) for a list of 32-byte rows: runs at a stride of 32 bytes of arcade palettes, ZERO, DISC."""
    n = len(rows)
    opts = [palette_options(prog, r, pindex.find) if any(r) else None for r in rows]
    items = []
    i = 0
    while i < n:
        if opts[i] is None:
            j = i
            while j < n and opts[j] is None:
                j += 1
            items.append((ZERO, j - i, 0))
            i = j
            continue
        best = (0, None)
        for a, flags in opts[i]:
            k = 1
            while i + k < n and opts[i + k] is not None and (a + 32 * k, flags) in opts[i + k]:
                k += 1
            if k > best[0]:
                best = (k, (a, flags))
        if best[1] is None:
            if items and items[-1][0] == DISC:
                items[-1] = (DISC, items[-1][1] + 1, 0)
            else:
                items.append((DISC, 1, 0))
            i += 1
        else:
            items.append((best[1][0], best[0], best[1][1]))
            i += best[0]
    return items


def unit_piece(img, row, ux, uy):
    """The 32-byte 8 x 8 piece at unit (ux, uy) of a picture whose rows are `row` bytes."""
    return b''.join(img[(uy * 8 + y) * row + ux * 4:(uy * 8 + y) * row + ux * 4 + 4] for y in range(8))


def cell_tile(img, row, cx, cy):
    return b''.join(img[(cy * 16 + y) * row + cx * 8:(cy * 16 + y) * row + cx * 8 + 8] for y in range(16))


def describe_picture(img, width, arc, cand):
    """Ops that redraw a picture of 4 bits a pixel from the arcade (see the module docstring), greedy: tile rects (the code stepping +1 across,
    +16 down) over the cells that are arcade tiles, piece rects over the 8 x 8 units that are scroll 1 pieces (the steps are taken from what
    the neighbours' candidates allow), FLAT rects for flat units and DISC rects for the rest. Blank units need no op."""
    row = width // 2
    rows = len(img) // row
    cw, chh = width // 16, rows // 16
    uw, uh = width // 8, rows // 8
    cov = [[False] * uw for _ in range(uh)]
    ops = []
    blank16, blank8 = bytes(TILE_BYTES), bytes(PIECE_BYTES)
    cells = {(cx, cy): cell_tile(img, row, cx, cy) for cy in range(chh) for cx in range(cw)}
    units = {(ux, uy): unit_piece(img, row, ux, uy) for uy in range(uh) for ux in range(uw)}

    def cell_free(cx, cy):
        return cx < cw and cy < chh and not cov[2 * cy][2 * cx]

    for cy in range(chh):
        for cx in range(cw):
            if not cell_free(cx, cy) or cells[cx, cy] == blank16:
                continue
            best = (0, None)
            for cid in cand.c16(cells[cx, cy]):
                code, f = cid >> 2, cid & 3
                w = 1
                while cell_free(cx + w, cy) and arc.tile(code + w, f) == cells[cx + w, cy]:
                    w += 1
                h = 1
                while cy + h < chh and all(cell_free(cx + i, cy + h) and arc.tile(code + i + 16 * h, f) == cells[cx + i, cy + h] for i in range(w)):
                    h += 1
                if w * h > best[0]:
                    best = (w * h, (cx, cy, w, h, code, f))
            if best[1]:
                x, y, w, h, code, f = best[1]
                ops.append((OP_TILE, 2 * x, 2 * y, w, h, code, 1, 16, f))
                for j in range(2 * h):
                    for i in range(2 * w):
                        cov[2 * y + j][2 * x + i] = True
    def unit_free(ux, uy):
        return ux < uw and uy < uh and not cov[uy][ux]

    for uy in range(uh):
        for ux in range(uw):
            if not unit_free(ux, uy) or is_flat(units[ux, uy]):
                continue
            seeds = cand.c8(units[ux, uy])
            best = (0, None)
            for sid in seeds:
                k, f = sid >> 2, sid & 3
                sxs = {0} | {(c >> 2) - k for c in cand.c8(units[ux + 1, uy]) if c & 3 == f} if unit_free(ux + 1, uy) and not is_flat(units[ux + 1, uy]) else {0}
                sys_ = {0} | {(c >> 2) - k for c in cand.c8(units[ux, uy + 1]) if c & 3 == f} if unit_free(ux, uy + 1) and not is_flat(units[ux, uy + 1]) else {0}
                for sx in sxs:
                    w = 1
                    while unit_free(ux + w, uy) and arc.piece(k + w * sx, f) == units[ux + w, uy]:
                        w += 1
                    for sy in sys_:
                        h = 1
                        while uy + h < uh and all(unit_free(ux + i, uy + h) and arc.piece(k + i * sx + h * sy, f) == units[ux + i, uy + h] for i in range(w)):
                            h += 1
                        if w * h > best[0]:
                            best = (w * h, (ux, uy, w, h, k, sx, sy, f))
            if best[1]:
                x, y, w, h, k, sx, sy, f = best[1]
                ops.append((OP_PIECE, x, y, w, h, k, sx, sy, f))
                for j in range(h):
                    for i in range(w):
                        cov[y + j][x + i] = True
    flats, discs = [], []
    for uy in range(uh):
        ux = 0
        while ux < uw:
            if cov[uy][ux] or units[ux, uy] == blank8:
                ux += 1
                continue
            flat = is_flat(units[ux, uy])
            v = units[ux, uy][0] & 15
            e = ux + 1
            while e < uw and not cov[uy][e] and units[e, uy] != blank8 and is_flat(units[e, uy]) == flat and (not flat or units[e, uy][0] & 15 == v):
                e += 1
            (flats if flat else discs).append([uy, ux, e - ux, v])
            ux = e
    for group, kind in ((flats, OP_FLAT), (discs, OP_DISC)):
        merged = []
        for y, x, w, v in group:
            if merged and merged[-1][1] == x and merged[-1][2] == w and merged[-1][3] == v and merged[-1][0] + merged[-1][4] == y:
                merged[-1][4] += 1
            else:
                merged.append([y, x, w, v, 1])
        for y, x, w, v, h in merged:
            ops.append((kind, x, y, w, h, v if kind == OP_FLAT else 0, 0, 0, 0))
    return ops


def picture_stats(img, width, ops):
    """(units built, units left to the disc, non-blank units) of a picture and its ops."""
    row = width // 2
    uw, uh = width // 8, len(img) // row // 8
    blank = bytes(PIECE_BYTES)
    nonblank = sum(1 for uy in range(uh) for ux in range(uw) if unit_piece(img, row, ux, uy) != blank)
    disc = sum(w * h for kind, x, y, w, h, *_ in ops if kind == OP_DISC)
    return nonblank - disc, disc, nonblank


# ---------------------------------------------------------------- recipes of whole chunks

PAGE_ROWS = 256
BUILT_MIN = 0.25          # a picture region is kept when at least this share of its non-blank units is built


class Env:
    """What the recipe finder needs: the model's arcade, the candidate index, the program and its palette index."""

    def __init__(self, tmp, prog, gfx):
        self.prog, self.arc = prog, Arc(gfx, prog)
        self.cand = Candidates(tmp, gfx)
        self.pindex = PaletteIndex(prog)
        import screen_windows
        self.windows = screen_windows.Windows(prog, gfx)     # the finder of the tiles cut from frames of the arcade
        self.edge = False                                    # set for the chunks that a DESIGN_ROWS row of units 'portrait' names
        self.blank_offsets = set()                           # the offsets of pictures that a row of units 'all' of the chunk names


def palette_rows_of(d, offset, size):
    return [d[offset + 32 * i:offset + 32 * i + 32] for i in range(size // 32)]


def pals_region(d, offset, size, env, stride=1):
    """A PALS region of the 32-byte rows at offset (every `stride`-th one is a palette, the rest must be zero), or None."""
    rows = palette_rows_of(d, offset, size)
    if stride > 1:
        if any(any(r) for i, r in enumerate(rows) if i % stride):
            return None
        rows = rows[::stride]
    return (PALS, offset, size, describe_palettes(rows, env.prog, env.pindex), stride)


def tiles_region(kind, d, offset, size, env):
    tiles = [d[offset + 128 * i:offset + 128 * i + 128] for i in range(size // 128)]
    items, pieces = describe_tiles(tiles, env.arc, env.cand, env.windows, env.edge)
    return (kind, offset, size, items, pieces)


def pic_region(d, offset, size, width, env):
    """A PIC region for d[offset:offset + size], or None when too little of it comes from the arcade."""
    row = width // 2
    size = size // row * row
    if size < row:
        return None
    img = d[offset:offset + size]
    ops = describe_picture(img, width, env.arc, env.cand)
    built, disc, nonblank = picture_stats(img, width, ops)
    if nonblank and built < BUILT_MIN * nonblank:
        return None
    return (PIC, offset, size, width, ops)


def pic_or_disc(d, offset, size, width, env):
    """pic_region, or for a place that a DESIGN_ROWS row of units 'all' names (env.blank_offsets) a picture whose only op leaves the whole to the disc: the
    row then makes it blank by design. None for any other picture that is too little the arcade's."""
    r = pic_region(d, offset, size, width, env)
    if r is None and offset in env.blank_offsets and size % (width // 2) == 0:
        r = (PIC, offset, size, width, [(OP_DISC, 0, 0, width // 8, size // (width // 2) // 8, 0, 0, 0, 0)])
    return r


def check_picture(env, region, d):
    data, known = picture_ops(env.arc, region[4], region[3], region[2])
    img = d[region[1]:region[1] + region[2]]
    return all(a == b for a, b, k in zip(data, img, known) if k)


def describe_chunk(d, table, name, env, key, of):
    """The recipe regions for a chunk's bytes `d`, by what the chunk is (`name`, from other_files.WHAT; `key` = (kind, table, slot)). Returns a
    list of regions (possibly empty)."""
    regions = []
    env.edge = any(row['units'] == 'portrait' and (row['kind'], row['table'], row['slot']) == tuple(key) for row in DESIGN_ROWS)
    env.blank_offsets = {row['offset'] for row in DESIGN_ROWS if row['units'] == 'all' and (row['kind'], row['table'], row['slot']) == tuple(key)}
    if table == 1 or name == 'palettes':
        r = pals_region(d, 0, len(d) // 32 * 32, env)
        return [r] if r else []
    if key in of.RAW_TILES:
        return [tiles_region(RAW, d, 0, len(d) // 128 * 128, env)]
    if table == 2 or name in ('picture', 'pictures', 'portrait page', 'text page'):
        pages = [d[i:i + of.PAGE_BYTES] for i in range(0, len(d), of.PAGE_BYTES)]
        ys = set()
        if name == 'portrait page':
            for pi, page in enumerate(pages):
                ys |= {pi * PAGE_ROWS + y for y in of.palette_rows(page)}
        row, total = 128, len(d) // 128
        y = 0
        while y < total:
            e = y
            pal = y in ys
            while e < total and (e in ys) == pal:
                e += 1
            if pal:
                r = pals_region(d, y * row, (e - y) * row, env, 4) or pals_region(d, y * row, (e - y) * row, env)
            else:
                r = pic_or_disc(d, y * row, (e - y) * row, 256, env)
            if r:
                regions.append(r)
            y = e
        return regions
    if name == 'name plate':
        r = pals_region(d, 0x2000, 0x400, env)
        regions += [r] if r else []
        r = pic_region(d, 0x2400, len(d) - 0x2400, 256, env)
        return regions + ([r] if r else [])
    tim = of.tim_header(d)
    if tim and tim['bpp'] == 4:
        if tim['clut']:
            k = tim['clut']
            r = pals_region(d, k['at'], 2 * k['w'] * k['h'] // 32 * 32, env)
            regions += [r] if r else []
        size = min(2 * tim['w'] * tim['h'], len(d) - tim['at'])
        r = pic_region(d, tim['at'], size, 4 * tim['w'], env) if tim['w'] % 4 == 0 else None
        return regions + ([r] if r else [])
    if tim:
        return []
    for kind, a, b in of.merge_pictures(of.chunk_regions(d)):
        if kind == 'tiles':
            for s, n, e in of.tile_segments(d[a:b]):
                tiles = of.segment_tiles(d[a:b], [(s, n, e)])
                items, pieces = describe_tiles(tiles, env.arc, env.cand, env.windows, env.edge)
                regions.append((SEG, a + s, e - s, items, pieces))
        elif kind == 'pal':
            r = pals_region(d, a, (b - a) // 32 * 32, env)
            if r:
                regions.append(r)
        elif kind == 'image' and (b - a) % 128 == 0:
            r = pic_or_disc(d, a, b - a, 256, env)
            if r:
                regions.append(r)
    return regions


def check_region(env, region, d):
    """True when the region's known bytes equal d (a model check of the recipe)."""
    kind, offset, size = region[:3]
    if kind == SEG or kind == RAW:
        tiles = tile_items(env.arc, region[3], region[4])
        if kind == SEG:
            return all(t is None or t == tilepack_tile(d[offset:offset + size], i) for i, t in enumerate(tiles))
        return all(t is None or t == d[offset + 128 * i:offset + 128 * i + 128] for i, t in enumerate(tiles))
    if kind == PALS:
        stride = region[4]
        pals = palette_items(env.prog, region[3])
        return all(p is None or p == d[offset + 32 * stride * i:offset + 32 * stride * i + 32] for i, p in enumerate(pals))
    return check_picture(env, region, d)


# ---------------------------------------------------------------- the table (screens_table.c)

class Table:
    """The recipes of all files in the shape of the C table: shared item lists, ops, pieces, regions and recipes, and for each file its chunks.
    Recipes are made once for equal chunks; a region's pieces are renumbered into one global list (the PIECES items' flip)."""

    def __init__(self):
        self.pieces, self.piece_idx = [], {}
        self.windows, self.window_idx = [], {}     # the distinct windows (kind, address, bank, blocks, x, y, flip, rows)
        self.items, self.item_idx = [], {}
        self.ops, self.op_idx = [], {}
        self.regions = []                  # (kind, offset, size, width, first, count)
        self.recipes, self.recipe_idx = [], {}     # (size, first, count)
        self.files = collections.OrderedDict()     # name -> [(table, slot, ordinal, recipe)]
        self.design = {}                           # name -> units blank by design in the file, as DESIGN_ROWS give them

    def _seq(self, seq, store, index):
        key = tuple(seq)
        if key not in index:
            index[key] = len(store)
            store.extend(key)
        return index[key]

    def add_recipe(self, size, regions):
        """The index of the recipe for a chunk of `size` bytes with `regions` (as describe_chunk returns them)."""
        out = []
        for region in regions:
            kind, offset, rsize = region[:3]
            if kind in (SEG, RAW):
                items = []
                for first, count, flip in region[3]:
                    if first == PIECES:
                        quad = tuple(region[4][flip])
                        if quad not in self.piece_idx:
                            self.piece_idx[quad] = len(self.pieces)
                            self.pieces.append(quad)
                        flip = self.piece_idx[quad]
                    elif first == WINDOW and isinstance(flip, tuple):
                        if flip not in self.window_idx:
                            self.window_idx[flip] = len(self.windows)
                            self.windows.append(flip)
                        flip = self.window_idx[flip]
                    items.append((first, count, flip))
                out.append((kind, offset, rsize, 0, self._seq(items, self.items, self.item_idx), len(items)))
            elif kind == PALS:
                items = [tuple(i) for i in region[3]]
                out.append((kind, offset, rsize, region[4], self._seq(items, self.items, self.item_idx), len(items)))
            else:
                ops = [tuple(o) for o in region[4]]
                out.append((kind, offset, rsize, region[3], self._seq(ops, self.ops, self.op_idx), len(ops)))
        key = (size, tuple(out))
        if key not in self.recipe_idx:
            self.recipe_idx[key] = len(self.recipes)
            first = len(self.regions)
            self.regions.extend(out)
            self.recipes.append((size, first, len(out)))
        return self.recipe_idx[key]

    def add_use(self, file, table, slot, ordinal, recipe):
        self.files.setdefault(file, []).append((table, slot, ordinal, recipe))

    def model_regions(self, recipe):
        """The recipe as model_chunk takes it: regions with global item lists and the global pieces."""
        size, first, count = self.recipes[recipe]
        out = []
        for kind, offset, rsize, aux, f, n in self.regions[first:first + count]:
            if kind in (SEG, RAW):
                pieces = Pieces(self.pieces)
                pieces.windows = self.windows
                out.append((kind, offset, rsize, self.items[f:f + n], pieces))
            elif kind == PALS:
                out.append((kind, offset, rsize, self.items[f:f + n], aux))
            else:
                out.append((kind, offset, rsize, aux, self.ops[f:f + n]))
        return size, out


def c_rows(rows, fmt, per_line=2):
    lines = []
    for i in range(0, len(rows), per_line):
        lines.append('    ' + ' '.join(fmt(r) + ',' for r in rows[i:i + per_line]))
    return '\n'.join(lines)


SPECIAL = {ZERO: 'ARCADE_SCREENS_ZERO', DISC: 'ARCADE_SCREENS_DISC', PIECES: 'ARCADE_SCREENS_PIECES', WINDOW: 'ARCADE_SCREENS_WINDOW'}
KIND_C = {SEG: 'ARCADE_SCREENS_SEG', RAW: 'ARCADE_SCREENS_RAW', PALS: 'ARCADE_SCREENS_PALS', PIC: 'ARCADE_SCREENS_PIC'}
OP_C = {OP_TILE: 'ARCADE_SCREENS_OP_TILE', OP_PIECE: 'ARCADE_SCREENS_OP_PIECE', OP_FLAT: 'ARCADE_SCREENS_OP_FLAT', OP_DISC: 'ARCADE_SCREENS_OP_DISC',
        OP_BLANK: 'ARCADE_SCREENS_OP_BLANK'}


def c_table(t, command):
    """The text of screens_table.c for a Table."""
    def num(v):
        return {ZERO: 'Z', DISC: 'D', PIECES: 'P', WINDOW: 'W'}.get(v, '%#x' % v)

    out = ['/* GENERATED by `%s`; do not edit.' % command,
           ' * For each disc file of the select, demo, continue, ending, boss and character-demo screens: the chunks (table, slot, ordinal) that have a',
           ' * recipe, and for each recipe its regions: compressed tile segments, plain tile lists, palettes and pictures, as lists of items (runs of',
           ' * consecutive arcade tiles, blank tiles, tiles of scroll 1 pieces, arcade palettes at an address, what stays the disc) and picture ops',
           ' * (rects of tiles or pieces by arcade code). Only addresses, counts and layouts: no byte of either game. */',
           '#include "screens.h"', '',
           '#define Z ARCADE_SCREENS_ZERO', '#define D ARCADE_SCREENS_DISC', '#define P ARCADE_SCREENS_PIECES', '#define W ARCADE_SCREENS_WINDOW',
           '#define BD ARCADE_SCREENS_BY_DESIGN',
           '#define OT ARCADE_SCREENS_OP_TILE', '#define OP ARCADE_SCREENS_OP_PIECE', '#define OF ARCADE_SCREENS_OP_FLAT', '#define OD ARCADE_SCREENS_OP_DISC',
           '#define OB ARCADE_SCREENS_OP_BLANK',
           '#define SEG ARCADE_SCREENS_SEG', '#define RAW ARCADE_SCREENS_RAW', '#define PALS ARCADE_SCREENS_PALS', '#define PIC ARCADE_SCREENS_PIC', '',
           '/* {first, count, flip}: a tile or palette item */',
           'const struct arcade_screens_item arcade_screens_items[] = {']
    out.append(c_rows(t.items, lambda i: '{%s, %d, %s}' % (num(i[0]), i[1], 'BD' if i[2] == BY_DESIGN else (str(i[2]) if i[2] < BY_DESIGN else '%#x' % i[2])), 5))
    out.append('};\n')
    out.append('/* {first, stepx, stepy, x, y, w, h, kind, flip} */')
    out.append('const struct arcade_screens_op arcade_screens_ops[] = {')
    out.append(c_rows(t.ops or [(OP_DISC, 0, 0, 0, 0, 0, 0, 0, 0)], lambda o: '{%#x, %d, %d, %d, %d, %d, %d, %s, %d}' % (o[5], o[6], o[7], o[1], o[2], o[3], o[4], {OP_TILE: 'OT', OP_PIECE: 'OP', OP_FLAT: 'OF', OP_DISC: 'OD', OP_BLANK: 'OB'}[o[0]], o[8]), 3))
    out.append('};\n')
    out.append('const uint32_t arcade_screens_pieces[][4] = {')
    out.append(c_rows(t.pieces or [(0, 0, 0, 0)], lambda q: '{' + ', '.join('%#x' % v for v in q) + '}', 3))      # an empty array is not C
    out.append('};')
    out.append('const unsigned arcade_screens_piece_count = %d;\n' % len(t.pieces))
    sources = []
    for w in t.windows:
        if w[:4] not in sources:
            sources.append(w[:4])
    out.append('/* {address, bank, blocks, kind}: a frame of the arcade (an art record, or scroll 2 blocks) that WINDOW items cut tiles from */')
    out.append('const struct arcade_screens_source arcade_screens_sources[] = {')
    out.append(c_rows(sources or [(0, 0, 0, 0)], lambda q: '{%#x, %#x, %d, %s}' % (q[1], q[2], q[3], {SRC_RECORD: 'ARCADE_SCREENS_SRC_RECORD', SRC_SCROLL2: 'ARCADE_SCREENS_SRC_SCROLL2', 0: '0'}[q[0]]), 3))
    out.append('};')
    out.append('const unsigned arcade_screens_source_count = %d;\n' % len(sources))
    out.append('/* {source, x, y, flip, rows}: the 16 x 16 tile with its top left at (x, y) of the source frame, the rows kept (0 all, 1 odd, 2 even), stored with the flip */')
    out.append('const struct arcade_screens_window arcade_screens_windows[] = {')
    out.append(c_rows(t.windows or [(0, 0, 0, 0, 0, 0, 0, 0)], lambda w: '{%d, %d, %d, %d, %d}' % ((sources.index(w[:4]) if t.windows else 0), w[4], w[5], w[6], w[7]), 4))
    out.append('};')
    out.append('const unsigned arcade_screens_window_count = %d;\n' % len(t.windows))
    out.append('/* {kind, offset, size, width or stride, first, count} */')
    out.append('const struct arcade_screens_region arcade_screens_regions[] = {')
    out.append(c_rows(t.regions, lambda r: '{%s, %#x, %#x, %d, %d, %d}' % ({SEG: 'SEG', RAW: 'RAW', PALS: 'PALS', PIC: 'PIC'}[r[0]], r[1], r[2], r[3], r[4], r[5]), 3))
    out.append('};\n')
    out.append('/* {size, first region, regions} */')
    out.append('const struct arcade_screens_recipe arcade_screens_recipes[] = {')
    out.append(c_rows(t.recipes, lambda r: '{%d, %d, %d}' % r, 6))
    out.append('};')
    out.append('const unsigned arcade_screens_recipe_count = %d;\n' % len(t.recipes))
    uses, files = [], []
    for name, chunks in t.files.items():
        files.append((name, len(uses), len(chunks)))
        uses += chunks
    out.append('/* {table, slot, ordinal, recipe} */')
    out.append('const struct arcade_screens_use arcade_screens_uses[] = {')
    out.append(c_rows(uses, lambda u: '{%d, %#x, %d, %d}' % u, 6))
    out.append('};\n')
    out.append('/* {file, first use, uses} */')
    out.append('const struct arcade_screens_file arcade_screens_files[] = {')
    out.append(c_rows(files, lambda f: '{"%s", %d, %d}' % f, 4))
    out.append('};')
    out.append('const unsigned arcade_screens_file_count = %d;' % len(files))
    out.append('\n' + '\n'.join('#undef ' + m for m in ('Z', 'D', 'P', 'W', 'BD', 'OT', 'OP', 'OF', 'OD', 'OB', 'SEG', 'RAW', 'PALS', 'PIC')) + '\n')
    return '\n'.join(out)


# ---------------------------------------------------------------- what is blank by design (the explicit table)

# One row for each place where the build is blank on purpose (the owner's rule of 2026-10-24, "rebuild arcade-style": where the arcade shows the screen its
# own way the screen is built from the arcade's art; the rest is blank). A row names the file kind, the chunk (table, slot, ordinal) and the region by its
# offset, and says which units: 'disc' = every unit the arcade does not have (the DISC items of a segment, list or palettes, the DISC rects of a picture),
# 'all' = the whole of the picture rows `rows` (pixel rows, a picture only). `expect` is the number of units over all the files of the kind that have the
# chunk (tiles, palettes, or 8 x 8 pieces of a picture): the generator refuses a table whose count differs, so a row cannot drift over the disc's data.
# `why` is the evidence. There is no heuristic here: a unit not named by a row is the disc's.
DESIGN_ROWS = (
    dict(kind='CONT', table=0, slot=0x15, ordinal=0, offset=0, units='all', rows=(0, 256), expect=43008,
         why="the score ranking's backdrop, a dark villain bust (the PS1's own art: no 16 x 16 window of the arcade's tile mosaic and no value-permuted "
             "arcade tile matches its own cells). The arcade has no such backdrop. The whole of page 0 (32 x 32 pieces in each of the 42 files) is "
             "blank, since the arcade-equal cells that are left (flat fill) would be a dark blob without its outline."),
    dict(kind='SELECTA', table=0, slot=0x18, ordinal=0, offset=0, units='portrait', expect=170,
         why="the select screen's big portraits are clipped by the PS1 along a curve at their lower right (a few cells a portrait). The arcade shows them whole: the cell is "
             "the arcade's tile, unclipped. Each such tile is found as a portrait tile of the arcade whose pixels contain the clipped tile's (screen_windows.py, edge)."),
    dict(kind='SELECTA', table=0, slot=0x1f, ordinal=0, offset=0, units='all', rows=(0, 256), expect=1024,
         why="a 256 x 32 gradient of dithered noise that the PS1 made itself (the same page in every file that has it); the arcade has no such texture. The chunk's first "
         "word stays the archive header's (the game's disc stream compares it), see arcade_screens_head_word."),
    dict(kind='CDEMO', table=0, slot=0x10, ordinal=0, offset=0xc478, units='all', rows=(0, 256), expect=21504,
         why="the same dithered gradient page as the select screen's, the PS1's own."),
    dict(kind='END', table=0, slot=0x2d, ordinal=0, offset=0, units='disc', expect=2646,
         why="the last screen's Zero 2 logo: 126 of the 160 tiles in each of the 21 files are the PS1's own or only partly the arcade's. The US set's logo says Alpha 2 and has other "
             "cells, so it is not drawn here. The arcade CAPCOM tiles and pieces stay."),
    dict(kind='END', table=0, slot=0x29, ordinal=0, offset=0, units='disc', expect=607,
         why="the ending art's own tiles: scene pieces that the PS1 composed (a landscape, an arena, backdrops), pixel for pixel not the arcade's (a 16 x 16 "
             "window search over the arcade's tile mosaic covers 46 of 607 own tiles at 90%). The arcade's ending art around them stays."),
)


def units_of(regions):
    """The by-design units of a list of regions: ZERO items marked BY_DESIGN (tiles, palettes), 8 x 8 pieces of BLANK ops."""
    n = 0
    for r in regions:
        if r[0] == PIC:
            n += sum(o[3] * o[4] for o in r[4] if o[0] == OP_BLANK)
        else:
            n += sum(c for f, c, fl in r[3] if not isinstance(fl, tuple) and fl & BY_DESIGN)
    return n


def op_rows(o):
    """(first, end) pixel rows of a picture op."""
    return o[2] * 8, o[2] * 8 + (16 if o[0] == OP_TILE else 8) * o[4]


def design_picture(region, row):
    """The picture region with the row's units made blank by design (DISC rects, or every op, inside the row's pixel rows)."""
    lo, hi = row.get('rows', (0, region[2] // (region[3] // 2)))
    kept, first = [], None
    for o in region[4]:
        a, b = op_rows(o)
        if a < hi and b > lo and not (lo <= a and b <= hi):
            raise ModelError('a picture op crosses the rows of a by-design row')
        if lo <= a and b <= hi and (row['units'] == 'all' or o[0] == OP_DISC):
            if first is None:
                first = len(kept)
            if row['units'] == 'disc':
                kept.append((OP_BLANK, o[1], o[2], o[3], o[4], 0, 0, 0, 0))
            continue
        kept.append(o)
    if row['units'] == 'all':
        kept.insert(len(kept) if first is None else first, (OP_BLANK, 0, lo // 8, region[3] // 8, (hi - lo) // 8, 0, 0, 0, 0))
    return (region[0], region[1], region[2], region[3], kept)


def apply_design(regions, kind, table, slot, ordinal, used):
    """The regions of a chunk of a file of `kind` with the rows of DESIGN_ROWS applied; `used` collects (row index, units) of every row that matched."""
    out = []
    for r in regions:
        for k, row in enumerate(DESIGN_ROWS):
            if (row['kind'], row['table'], row['slot'], row['ordinal'], row['offset']) != (kind, table, slot, ordinal, r[1]):
                continue
            before = units_of([r])
            if row['units'] == 'portrait':      # the tiles describe_tiles made BY_DESIGN runs of the portraits' edge: counted, not changed
                if r[0] not in (SEG, RAW):
                    raise ModelError("units 'portrait' is for a tile list")
                used.append((k, before))
                continue
            if r[0] == PIC:
                r = design_picture(r, row)
            elif row['units'] == 'disc':
                r = r[:3] + ([(ZERO, c, BY_DESIGN) if f == DISC else (f, c, fl) for f, c, fl in r[3]],) + r[4:]
            else:
                raise ModelError("units 'all' is for a picture")
            used.append((k, units_of([r]) - before))
        out.append(r)
    return out


# ---------------------------------------------------------------- collecting the recipes of the disc's files

SKIP_NAMES = ('module', 'VAB header', 'VAB body', 'sequence', 'sound commands', 'zeros', 'TIM 8-bit', '?')


def collect(files, env, of, label=None, check=True):
    """A Table with the recipe of every chunk of the disc files that has one. `files` maps a file name ("BOSS00.PAC") to (kind, [(table, slot,
    ordinal, bytes)])."""
    t = Table()
    made = {}
    totals = collections.Counter()
    for name, (kind, chunks) in sorted(files.items()):
        for table, slot, ordinal, d in chunks:
            what = of.what_of(kind, table, slot, ordinal, d)[0]
            if what in SKIP_NAMES:
                continue
            key = hashlib.sha1(d).digest() + bytes([table]) + what.encode()
            if key not in made:
                if label:
                    label((kind, table, slot, name))
                made[key] = describe_chunk(d, table, what, env, (kind, table, slot), of)
            if made[key]:
                used = []
                regions = apply_design(made[key], kind, table, slot, ordinal, used)
                for k, n in used:
                    totals[k] += n
                    t.design[name] = t.design.get(name, 0) + n
                t.add_use(name, table, slot, ordinal, t.add_recipe(len(d), regions))
    kinds = {kind for kind, _chunks in files.values()}
    wrong = ['row %d (%s slot %#x) blanks %d units, the table declares %d' % (k, row['kind'], row['slot'], totals[k], row['expect'])
             for k, row in enumerate(DESIGN_ROWS) if row['kind'] in kinds and totals[k] != row['expect']]
    if wrong and check:
        raise ModelError('by design: ' + '; '.join(wrong))
    return t


# ---------------------------------------------------------------- the C build, compared (--compare-c)

class COut(ctypes.Structure):
    _fields_ = [('data', ctypes.POINTER(ctypes.c_uint8)), ('known', ctypes.POINTER(ctypes.c_uint8)), ('size', ctypes.c_uint32), ('unbuilt', ctypes.c_uint), ('design', ctypes.c_uint)]


class CLib:
    """screens.c, the checked-in screens_table.c and tilepack.c as a shared library (warnings are errors)."""

    def __init__(self, tmp, prog, gfx):
        lib = Path(tmp) / 'screens.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC', '-o', str(lib), str(HERE / 'compose.c'), str(HERE / 'screens.c'),
                        str(HERE / 'screens_table.c'), str(HERE / 'tilepack.c')], check=True, capture_output=True, timeout=180)
        self.lib, self.prog, self.gfx = ctypes.CDLL(str(lib)), prog, gfx
        c = ctypes
        self.lib.arcade_screens_find.restype = c.c_void_p
        self.lib.arcade_screens_find.argtypes = [c.c_char_p]
        self.lib.arcade_screens_recipe_of.restype = c.c_void_p
        self.lib.arcade_screens_recipe_of.argtypes = [c.c_void_p, c.c_uint, c.c_uint, c.c_uint]
        self.lib.arcade_screens_build.argtypes = [c.c_char_p, c.c_size_t, c.c_char_p, c.c_size_t, c.c_void_p, c.c_char_p, c.c_size_t, c.c_void_p,
                                                  c.c_char_p, c.c_size_t]
        self.lib.arcade_screens_free.argtypes = [c.c_void_p]
        self.lib.arcade_screens_head_word.argtypes = [c.c_void_p, c.c_void_p, c.c_uint32]
        self.lib.arcade_screens_verify.argtypes = [c.c_void_p, c.c_void_p, c.c_char_p, c.c_size_t, c.c_char_p, c.c_size_t]

    def build(self, name, table, slot, ordinal, disc):
        """(data, known, unbuilt, seconds, verify message or None, units blank by design) of the chunk's recipe built by the C with the disc's chunk, or a
        message."""
        f = self.lib.arcade_screens_find(name.encode())
        r = self.lib.arcade_screens_recipe_of(f, table, slot, ordinal) if f else None
        if not r:
            return 'not in the C table'
        out, err = COut(), ctypes.create_string_buffer(256)
        t0 = time.perf_counter()
        rc = self.lib.arcade_screens_build(self.prog, len(self.prog), self.gfx, len(self.gfx), r, disc, len(disc), ctypes.byref(out), err, len(err))
        took = time.perf_counter() - t0
        if rc:
            return err.value.decode()
        self.lib.arcade_screens_head_word(r, ctypes.byref(out), struct.unpack_from('<I', disc, 0)[0] if len(disc) >= 4 else 0)
        try:
            verify = self.lib.arcade_screens_verify(r, ctypes.byref(out), disc, len(disc), err, len(err))
            return (ctypes.string_at(out.data, out.size), ctypes.string_at(out.known, out.size), out.unbuilt, took,
                    None if verify == 0 else 'C verify: ' + err.value.decode(), out.design)
        finally:
            self.lib.arcade_screens_free(ctypes.byref(out))


def segment_tiles_of(chunk):
    """All tiles (128 bytes) of a packed segment, or None when one does not decode."""
    n = tilepack_count(chunk)
    out = []
    for i in range(n):
        t = tilepack_tile(chunk, i)
        if t is None:
            return None
        out.append(t)
    return out


def arcade_bytes(recipe):
    """How many bytes of a chunk the arcade alone gives (recipe = (size, regions)): a segment's room in proportion to its tiles that are not DISC,
    plain tiles and palettes by count, pictures by the units not left to the disc."""
    total = 0.0
    for r in recipe[1]:
        kind, rsize = r[0], r[2]
        if kind in (SEG, RAW):
            n = sum(c for _f, c, _fl in r[3])
            miss = sum(c for f, c, _fl in r[3] if f == DISC)
            total += (rsize if kind == SEG else 128 * (n - miss)) * ((n - miss) / n if kind == SEG and n else 1)
        elif kind == PALS:
            total += rsize - 32 * sum(c for f, c, _fl in r[3] if f == DISC)
        else:
            total += rsize - sum(w * h * 32 for k, x, y, w, h, *_ in r[4] if k == OP_DISC)
    return total


def compare_chunk(recipe, model, built, disc):
    """The comparison of one chunk: (C == model?, C == disc at the known bytes?, bytes served, notes, segments the C could not pack). `model` =
    (data, known, segments), `built` = (data, known, unbuilt, seconds) of the C. Packed segments are compared as tiles. A segment the model can
    build but the C's packer cannot fit into the room stays the disc's: it is counted, not an error."""
    mdata, mknown, msegs = model
    cdata, cknown, unbuilt, _t, verify, design = built
    notes = []
    if verify:
        notes.append(verify)
    same_model = len(cdata) == len(mdata)
    ok_disc = True
    served = 0
    skipped = 0
    seg_regions = {r[1]: r for r in recipe[1] if r[0] == SEG}
    mask = bytearray(b'\x01') * len(mdata)
    for off, r in seg_regions.items():
        mask[off:off + r[2]] = bytes(r[2])
    if same_model:
        same_model = all(a == b for a, b, m in zip(cknown, mknown, mask) if m) and all(a == b for a, b, m in zip(cdata, mdata, mask) if m)
    for off, r in seg_regions.items():
        if cknown[off] == 2:
            if mknown[off] != 2:
                same_model = False
                notes.append('segment %#x built where the model has it unknown' % off)
            tiles = segment_tiles_of(cdata[off:off + r[2]])
            if tiles is None or tiles != msegs.get(off):
                same_model = False
                notes.append('segment %#x differs from the model' % off)
            dtiles = segment_tiles_of(disc[off:off + r[2]])
            by_design = design_kinds(r[3])
            if dtiles is not None and tiles is not None and len(dtiles) == len(tiles) == len(by_design):
                # the tiles by design are not the disc's (the blank ones must be blank), and the rest must be the disc's
                if all(t == bytes(TILE_BYTES) for t, flag in zip(tiles, by_design) if flag == 1):
                    dtiles = [t if flag else d for t, d, flag in zip(tiles, dtiles, by_design)]
            if dtiles is None or tiles is None or tiles != dtiles:
                ok_disc = False
                notes.append('segment %#x differs from the disc' % off)
            served += r[2]
        elif mknown[off] == 2:
            skipped += 1
        elif cknown[off] != 0:
            same_model = False
    if skipped != unbuilt - sum(1 for off in seg_regions if mknown[off] != 2):
        same_model = False
        notes.append('unbuilt segments %d, expected %d' % (unbuilt, skipped))
    if design != units_of(recipe[1]):
        same_model = False
        notes.append('the C counts %d units blank by design, the table has %d' % (design, units_of(recipe[1])))
    for i in range(len(cdata)):
        if cknown[i] == 1:
            served += 1
            if cdata[i] != disc[i]:
                ok_disc = False
                if len(notes) < 3:
                    notes.append('byte %#x differs from the disc' % i)
    return same_model, ok_disc and not verify, served, notes, skipped


def compare_report(table, text, disc, env, ctx, tmp, started):
    """Print one line a file: the C build against the model and the disc, per chunk; then the totals by file kind."""
    here = HERE / 'screens_table.c'
    print('screens_table.c ' + ('is the generated one' if here.exists() and here.read_text() == text else 'DIFFERS from the generated one (or is missing)'))
    clib = CLib(tmp, ctx.prog, ctx.gfx)
    arc = env.arc
    memo = {}
    totals = collections.OrderedDict()
    bad = 0
    spent = 0.0
    design_total = 0
    for name, uses in table.files.items():
        kind, chunks = disc[name]
        by_key = {(t, s, o): d for t, s, o, d in chunks}
        parts, flags = [], []
        file_design = [0, 0]      # units blank by design: the C's, and the generator's rows'
        for t, s, o, recipe_id in uses:
            d = by_key[t, s, o]
            key = (recipe_id, hashlib.sha1(d).digest())
            if key not in memo:
                size, regions = table.model_regions(recipe_id)
                model = model_chunk(arc, env.prog, (t, s, o, size, regions), d, struct.unpack_from('<I', d, 0)[0] if len(d) >= 4 else None)
                built = clib.build(name, t, s, o, d)
                if isinstance(built, str):
                    memo[key] = (built, None, 0, 0.0, 0, 0, 0.0, 0, 0)
                else:
                    same_model, same_disc, got, notes, skipped = compare_chunk((size, regions), model, built, d)
                    memo[key] = (None, (same_model, same_disc, notes), got, built[3], skipped, 0, arcade_bytes((size, regions)), built[5], units_of(regions))
            err, verdict, got, took, skipped, _x, arcade, c_design, m_design = memo[key]
            design_room = sum(r[2] for r in table.model_regions(recipe_id)[1] if r[0] == SEG and any(design_flags(r[3]))) if verdict else 0   # segments that are not wholly the disc's
            file_design[0] += c_design
            file_design[1] += m_design
            tot = totals.setdefault(kind, collections.Counter())
            tot['chunks'] += 1
            tot['disc_bytes'] += len(d)
            tot['served'] += got
            tot['skipped'] += skipped
            tot['arcade'] += arcade
            if err:
                flags.append('C refused t%d s%#x: %s' % (t, s, err))
                bad += 1
                continue
            spent += took
            if not verdict[0]:
                flags.append('t%d s%#x C DIFFERS from the model %s' % (t, s, '; '.join(verdict[2])))
                bad += 1
            if not verdict[1]:
                flags.append('t%d s%#x built bytes DIFFER from the disc %s' % (t, s, '; '.join(verdict[2])))
                bad += 1
            tot['equal'] += (got - design_room) if verdict[1] else 0
            parts.append('t%d s%#x %d/%d%s' % (t, s, got, len(d), ' (%d segment left to the disc: does not pack into its room)' % skipped if skipped else ''))
        declared = table.design.get(name, 0)
        if file_design[0] != declared or file_design[1] != declared:
            flags.append('by design: the C counts %d units, the model %d, the generator\'s rows %d' % (file_design[0], file_design[1], declared))
            bad += 1
        design_total += file_design[0]
        by_design = ' (%d units blank by design)' % file_design[0] if file_design[0] else ''
        print('%s: %s%s%s' % (name, ', '.join(parts), by_design, ('; ' + '; '.join(flags)) if flags else '; C = model, = disc at the known bytes'))
    print('--- by file kind (chunks that have a recipe): disc bytes; served = bytes the C builds that equal the disc (packed segments count as decoding to '
          'the disc\'s tiles); from the arcade alone = served minus what comes from the disc (tiles, palettes and cells the arcade lacks)')
    for kind, c in totals.items():
        print('%-9s %3d chunks %10d disc bytes, served %10d (%d%%) equal to the disc %10d, from the arcade alone %10d (%d%%), segments left %d' % (
            kind, c['chunks'], c['disc_bytes'], c['served'], round(100 * c['served'] / max(1, c['disc_bytes'])), c['equal'], c['arcade'],
            round(100 * c['arcade'] / max(1, c['disc_bytes'])), c['skipped']))
    kinds = {k for k, _c in disc.values()}
    declared = sum(r['expect'] for r in DESIGN_ROWS if r['kind'] in kinds)
    print('blank by design: %d units over %d files (the rows of DESIGN_ROWS declare %d)' % (design_total, len(table.design), declared))
    if design_total != declared:
        print('by-design total DIFFERS from the declared one')
        bad += 1
    print('C time %.1f ms for %d distinct builds; %d problems; %.1f s' % (spent * 1000, len(memo), bad, time.monotonic() - started))
    return 1 if bad else 0
