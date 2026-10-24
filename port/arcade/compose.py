#!/usr/bin/env python3
"""Model of the art record parse and the canvas drawing (the specification of port/arcade/compose.c). Not a tool: it has no command line.

It reads the records through screen_lists.py (record_blocks: modes 0, 2, 4 and 10; block_parts: the parts) and draws with that module's
pixel functions (arcade_pixels, flip_pixels), so the specification of what a record is stays in one place. What it adds is the cells of
a record in record order with no drops and no sort, the canvas (the bounding box of the cells, later cells over earlier ones where their
pixel is not 0), the scroll 2 blocks side by side, and the 16 x 16 cut of a canvas as the 128 bytes the PS1 stores.

A cell is (code, x, y, flags, part), code with the bank added; a canvas is (x0, y0, w, h, pixels) with one pixel value a byte.
"""
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import screen_lists as S  # noqa: E402

MAX_DIM = 512


def record_cells(data, address, bank, pal_split=False):
    """(mode, cells, end) of the art record at `address` of the 68000's view `data`, or None when it is not a record (C: an error line)."""
    rec = S.record_blocks(data, address)
    if rec is None:
        return None
    blocks, end = rec
    mode = data[address] << 8 | data[address + 1]
    parts = S.block_parts(blocks, bool(pal_split and mode in (0, 2)))
    cells = []
    for b, (code, attr, x, y) in enumerate(blocks):
        w, h = ((attr >> 8) & 15) + 1, ((attr >> 12) & 15) + 1
        for j in range(h):
            for i in range(w):
                ci = w - 1 - i if attr & 0x20 else i
                cj = h - 1 - j if attr & 0x40 else j
                cells.append((code + ci + 16 * cj + bank, x + 16 * i, y + 16 * j, attr & 0x7f, parts[b]))
    return mode, cells, end


def tile_pixels(gfx, code, flip):
    """The 256 PS1 pixels of arcade tile `code` flipped by `flip`, or None outside the graphics."""
    if code < 0 or (code + 1) * 128 > len(gfx):
        return None
    return S.flip_pixels(S.arcade_pixels(gfx[code * 128:code * 128 + 128]), flip)


def draw(gfx, cells):
    """The canvas of cells (code, x, y, flags, ...): None for a tile outside the graphics or a box over MAX_DIM, (0, 0, 0, 0, b'') for no cell."""
    if not cells:
        return (0, 0, 0, 0, b'')
    x0, y0 = min(c[1] for c in cells), min(c[2] for c in cells)
    w, h = max(c[1] for c in cells) + 16 - x0, max(c[2] for c in cells) + 16 - y0
    if w > MAX_DIM or h > MAX_DIM:
        return None
    px = bytearray(w * h)
    for code, x, y, flags, *_ in cells:
        t = tile_pixels(gfx, code, (flags >> 5) & 3)
        if t is None:
            return None
        for ty in range(16):
            for tx in range(16):
                if t[ty * 16 + tx]:
                    px[(y - y0 + ty) * w + (x - x0 + tx)] = t[ty * 16 + tx]
    return (x0, y0, w, h, bytes(px))


def record_canvas(data, gfx, address, bank):
    """The record drawn, or None."""
    r = record_cells(data, address, bank)
    return None if r is None else draw(gfx, r[1])


def scroll2_cell(data, block, c, r):
    """(tile, attribute) of cell (c, r) of scroll 2 block `block`, or None outside the blocks or the program."""
    if not 0 <= block < S.SCROLL_BLOCKS or not (0 <= c < 16 and 0 <= r < 16):
        return None
    at = S.SCROLL_FIRST + S.SCROLL_BLOCK * block + 64 * c + 4 * r
    if at + 4 > len(data):
        return None
    code, attr = data[at] << 8 | data[at + 1], data[at + 2] << 8 | data[at + 3]
    return ((code + S.SCROLL_ADD) & 0xffff) + 0x10000, attr


def scroll2_canvas(data, gfx, first, blocks):
    """`blocks` (1 or 2) scroll 2 blocks side by side, every cell drawn, or None."""
    if not 1 <= blocks <= MAX_DIM // 256:
        return None
    cells = []
    for b in range(blocks):
        for c in range(16):
            for r in range(16):
                cell = scroll2_cell(data, first + b, c, r)
                if cell is None:
                    return None
                cells.append((cell[0], 256 * b + 16 * c, 16 * r, cell[1] & 0x7f, 0))
    return draw(gfx, cells)


def cut(canvas, x, y, rows=0):
    """The 128 bytes of the 16 x 16 tile with its top left at (x, y): 8 a row, the left pixel of a pair in the low nibble; rows 1 keeps only
    the odd rows of the tile, rows 2 only the even rows."""
    x0, y0, w, h, px = canvas
    out = bytearray(128)
    for ty in range(16):
        if (rows == 1 and not ty & 1) or (rows == 2 and ty & 1):
            continue
        for tx in range(16):
            cx, cy = x + tx - x0, y + ty - y0
            if 0 <= cx < w and 0 <= cy < h:
                out[ty * 8 + tx // 2] |= px[cy * w + cx] << (4 * (tx & 1))
    return bytes(out)
