#!/usr/bin/env python3
"""Analysis tool: what the fighter files hold besides the code module, the sprite tiles and the sound bank, and how much of it the arcade set gives.

What it does. Every fighter file (PLnn.PAC, PLnnX.PAC) of the PS1 disc holds, beside the code module (table 0), the sprite
tiles (table 0) and the sound bank (tables 3 and 4), these chunks. This tool says what each one is (found from the PS1
loader and the code that reads it) and, for the ones the arcade has, rebuilds them from the arcade set and compares with
the disc byte for byte.

  table 1, 0x800 bytes      64 sprite palettes (CLUTs of 16 colors), sent to video memory when the fight loads
                            (32 x 32 halfwords at x 0x100, or 0x120 for the second side, y 0x1e0).
                            Rule: arcade palette words (4 bits R, G, B; the brightness nibble is ignored, it is 0 in all
                            the 40,740 matched words, so a nonzero one is untested) become PS1 colors
                            R*2 | G*2 << 5 | B*2 << 10 (not *31/15); 0 becomes 0x8000 (opaque black, since 0 is
                            transparent); arcade color 15 (the transparent one) is PS1 color 0 and arcade colors 0..14
                            are PS1 colors 1..15. Layout, palette n of the file, p = n >> 1, fighter k (FIGHTER_OF):
                            even n < 60: arcade 0x26d0e0 + 0x3c0 * k + 0x20 * p (30 palettes);
                            n = 60 and 62: the ramps at 0x28b140 and 0x28b160;
                            odd n: p < split: 0x272ae0 + 0x200 * k + 0x20 * p (split is 16, 8 for PL0A, 14 for PL08);
                            p >= split: a block that has an address of its own for most fighters (0x29c300 for the rest),
                            palettes in a row ("flat") or, for PL02 and PL14, runs of 5 of which the first 2 are used.
                            Where the arcade has no such palette the PS1 file holds a filler ramp of 15 steps (a counter
                            that steps every second word, from a start word; red 7, or red 0 in PL02): a list of 18
                            start words a file (ramp_index: odd n from 33 take slot p - 16, n = 60 and 62 take 16 and 17),
                            7 distinct lists in 23 files, no arcade data. 31 palettes of PL09, PL0B, PL0E and PL14 (6, 1, 8 and
                            16; 62 with the PLnnX files) differ from this arcade set: the Japanese game's own colors,
                            or the set's other region; they stay the disc's (the descriptor's keep mask).
  table 2, 0x8000 bytes     a 4 bits a pixel, 256 x 256 picture sent to video memory (the loader sends it in 16
                            sectors of 64 x 16 halfwords, at x 0x280, 0x2c0 for the second side, 0x300 for the shared
                            page). Per file the chunks are: the fighter's portrait page; the shared "VS" page (the same
                            in the 24 files that have one, 65 tiles); and, in PL15 to PL17 (one file for both sides), the
                            portrait page again. Every 16 x 16 cell that is not blank is an arcade graphics tile (color
                            index + 1). The layout is the arcade's own, found 2026-10-23:
                            portrait page: the arcade draws the select screen's portrait as a sprite of 9 to 30 blocks
                            (art record of mode 4: code, attribute, x, y) which the sequence steps at 0x5e6c0 + 0x14 * k
                            point to (32-bit pointer a step, fighter k, 19 used). Tile code 0x10000 + code + 0xa400 (16
                            bits) + column + 16 * row of the block, as for the arcade's scroll 2 layer; cell (x + 96) / 16,
                            (y + 80) / 16 of the page. Rows 160 to 177 hold 18 palettes (see portrait_addresses). Beside
                            the record: the backdrop tile 0x1a49e fills cells 12 to 15 of rows 0 to 9, and up to 12 cells
                            of the shared page's shadow (tiles of the VS records' last record, cells 6 to 11 of row 14
                            and 5 to 10 of row 15), which of them a page holds being the PS1's own (a 12 bit mask per file); one cell of PL0F (Dhalsim) is blank where the arcade has a sliver of a tile.
                            PL14's page (the alternate Gouki) is the PS1's own (other palettes, 28 more cells) and stays
                            the disc's.
                            VS page: the four art records of mode 2 in a row from 0x235658 (14, 15, 18 and 18 cells, tile
                            bank 0x10000), each placed at a cell offset ((12, 2), (8, 6), (4, 2), (4, 6)) plus x / 16, y / 16.
  table 3 slot 4 or 5       sound command lists (8 bytes a command: read by the sound player, func_80120604 in
                            ps1/src/resident/sound), one per sound of the fighter's bank; the arcade drives QSound
                            with its own command codes, so it has no source for them.
  table 5 slot 0x100/0x101  a PS1 sequence file (magic pQES: a short run of MIDI-like notes). No arcade source.
  table 0 slot 0xffff       16,384 bytes of zeros that the loader reads and discards (PL15 to PL17, two each).

Inputs: the disc image (raw 2352-byte sectors) and the arcade set's ZIP (MAME's sfa2u), both given as arguments and
both read only. It writes nothing (--compare-c uses a temporary folder, removed). It prints sizes, addresses and counts, never game bytes. It is an analysis and
reference tool; the converter that the port runs will be written in C.

With --c-table it prints instead the C file port/arcade/fighter_parts_table.c: for each of the 45 fighter files the rules the
runtime C (fighter_parts.c) builds the palettes (table 1) and pages (table 2) from: the arcade fighter, where the odd
palettes' second block lies, the filler ramps' start words, the palettes and pages that stay the disc's, which of the
shadow cells a page holds; no game byte. With --compare-c it builds fighter_parts.c with the checked-in table into a
temporary folder and prints one line per file: C against the model and the disc, per chunk, and which palettes stay the
disc's.

    python3 port/arcade/fighter_parts.py --disc BIN --arcade ZIP [--only NAME] [--detail] [--c-table | --compare-c]
"""
import argparse
import collections
import ctypes
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

# arcade program addresses (68000 view)
A_BASE, A_STRIDE = 0x26d0e0, 0x3c0       # 30 palettes of 32 bytes per fighter
B_BASE, B_STRIDE = 0x272ae0, 0x200       # 16 palettes per fighter
C_DEFAULT = 0x29c300                     # 16 palettes shared by the fighters that have none of their own
RAMP_60, RAMP_62 = 0x28b140, 0x28b160    # two shared ramps: the palettes 60 and 62 of the file
PORTRAIT_BASE, PORTRAIT_BUTTON, PORTRAIT_FIGHTER = 0x29c7c0, 0x720, 0x60
PORTRAIT_FIRST_ROW = 160                 # first row of the 18 palettes in the portrait page
PALETTE_BYTES = 32

# PL number of a file -> the arcade's fighter number (the two files of PL13 and PL17 and of PL15/PL16 use another one's)
FIGHTER_OF = {**{i: i for i in range(0x13)}, 0x13: 0x11, 0x14: 2, 0x15: 0x0d, 0x16: 0x10, 0x17: 0x11}


# ---------------------------------------------------------------- pure functions (tested without game data)

def ps1_color(word):
    """The PS1 color of an arcade palette word (4 bits each of R, G, B in bits 8-11, 4-7, 0-3; the brightness nibble is dropped).

    Each channel is doubled (not scaled to 31). Black would be 0, which the PS1 draws as transparent, so it is 0x8000."""
    c = ((word >> 8) & 15) << 1 | ((word >> 4) & 15) << 6 | (word & 15) << 11
    return c or 0x8000


def ps1_palette(words):
    """The 16 PS1 colors of 16 arcade palette words: arcade color 15 is PS1 color 0 (transparent), 0..14 are 1..15."""
    return [0] + [ps1_color(w) for w in words[:15]]


def portrait_palette(words):
    """As ps1_palette, with the semi-transparency bit (bit 15) set on every color but color 0."""
    return [0] + [ps1_color(w) | 0x8000 for w in words[:15]]


def ramp_words(start):
    """The 15 arcade words of a ramp: start, start + 1, start + 1, start + 2, start + 2, ... (a counter that steps every second word)."""
    return [start + (k + 1) // 2 for k in range(15)]


def is_ramp(colors):
    """The start word of the arcade form of PS1 colors 1..15 when they are a ramp (any red), else None.

    The arcade has ramps with red 0; the PS1 files' own fillers have red 7 (start word 0x7xx)."""
    words = [((c & 31) >> 1) << 8 | (((c >> 5) & 31) >> 1) << 4 | ((c >> 10) & 31) >> 1 for c in colors]
    return words[0] if words == ramp_words(words[0]) else None


def c_offset(q, layout):
    """Byte offset from the odd second block's address of its palette number q (the odd palette p = split + q of the file).

    Layout 'flat': palettes in a row. Layout 'fives': six runs of 5 palettes (one per button), of which a run's first 2
    are used (q 0..11); q 12 and up are not in the block, and None."""
    if layout == 'flat':
        return PALETTE_BYTES * q
    return PALETTE_BYTES * (5 * (q // 2) + q % 2) if q < 12 else None


def palette_addresses(pl, x_base, layout='flat', split=16):
    """Arcade address of each of the 64 palettes of file PLpl (None where the layout names none), by the layout in the docstring.

    The odd palettes p = n >> 1 below split are at 0x272ae0 + 0x200 * fighter + 0x20 * p; those from split up are in a
    second block at x_base, laid out as c_offset says (split is 16 for most fighters, less where the first block ends
    early and the second begins with the rest)."""
    k = FIGHTER_OF[pl]
    out = []
    for n in range(64):
        p = n >> 1
        if n & 1:
            if p < split:
                out.append(B_BASE + B_STRIDE * k + PALETTE_BYTES * p)
            else:
                off = c_offset(p - split, layout)
                out.append(None if off is None else x_base + off)
        elif p < 30:
            out.append(A_BASE + A_STRIDE * k + PALETTE_BYTES * p)
        else:
            out.append(RAMP_60 if n == 60 else RAMP_62)
    return out


def portrait_addresses_of(k):
    """Arcade address of each of the 18 palettes in the portrait page of arcade fighter k, in row order."""
    return [PORTRAIT_BASE + PORTRAIT_BUTTON * j + PORTRAIT_FIGHTER * k + PALETTE_BYTES * m for j in range(6) for m in range(3)]


def portrait_addresses(pl):
    """Arcade address of each of the 18 palettes in the portrait page of file PLpl, in row order."""
    return portrait_addresses_of(FIGHTER_OF[pl])


def ramp_index(n):
    """Index (0..17) of palette n of a file in its list of filler ramp starts, or None where a ramp cannot stand.

    The odd palettes from 33 on (p = n >> 1 from 16) take p - 16; the two even ones past the 30 per-fighter palettes, 60
    and 62, take 16 and 17."""
    if n & 1 and n >= 33:
        return (n >> 1) - 16
    return {60: 16, 62: 17}.get(n)


def palette_chunk(pl, prog, desc):
    """(2048 bytes, kept) of a file's palette chunk from its descriptor, the model the runtime C (fighter_parts.c) follows.

    desc has: layout ('flat' or 'fives'), split, base (the second odd block), keep (bit n set: palette n stays the disc's)
    and ramps (18 start words or None). A kept palette is written as zeros; kept is desc['keep']. Any other palette is a filler
    ramp (when ramps names its index) or the arcade's at the layout's address (ValueError where it names none)."""
    addr = palette_addresses(pl, desc['base'], desc['layout'], desc['split'])
    out = bytearray(2048)
    for n in range(64):
        if desc['keep'] >> n & 1:
            continue
        idx = ramp_index(n)
        start = desc['ramps'][idx] if idx is not None and desc['ramps'] else None
        if start is not None:
            pal = ps1_palette(ramp_words(start))
        elif addr[n] is None or addr[n] + 32 > len(prog):
            raise ValueError(f'palette {n} has no arcade address')
        else:
            pal = ps1_palette(words_of(prog, addr[n]))
        struct.pack_into('<16H', out, 32 * n, *pal)
    return bytes(out), desc['keep']


def derive_palettes(pl, chunk, prog):
    """The descriptor (see palette_chunk) that reproduces the disc's palette chunk as far as the rules go.

    The second odd block comes from table1's search. A palette that equals the arcade's at its address is the arcade's; one
    that is a filler ramp (and has a slot in the ramp list) is a ramp; any other stays the disc's (its bit is set in keep)."""
    r = table1(pl, chunk, prog)
    addr = palette_addresses(pl, r['c_base'], r['layout'], r['split'])
    pals = [struct.unpack_from('<16H', chunk, 32 * n) for n in range(64)]
    keep, ramps = 0, [None] * 18
    for n, (p, a) in enumerate(zip(pals, addr)):
        if a is not None and a + 32 <= len(prog) and list(p) == ps1_palette(words_of(prog, a)):
            continue
        start, idx = is_ramp(p[1:]), ramp_index(n)
        if start is not None and idx is not None and list(p) == ps1_palette(ramp_words(start)):
            ramps[idx] = start
        else:
            keep |= 1 << n
    return dict(layout=r['layout'], split=r['split'], base=r['c_base'], keep=keep, ramps=ramps if any(v is not None for v in ramps) else None)


# ---------------------------------------------------------------- the pages: art records to cells

STEPS, STEP_STRIDE = 0x5e6c0, 0x14       # per fighter k, the sequence step whose first long is the pointer to the portrait's art record
ORIGIN_X, ORIGIN_Y = 96, 80              # a record's (x, y) is the page cell ((x + 96) / 16, (y + 80) / 16)
SCROLL_ADD, TILE_BANK = 0xa400, 0x10000  # the portrait's tile code: ((code + 0xa400) & 0xffff) + 0x10000
BACKDROP_TILE, BACKDROP_CELLS = 0x1a49e, (12, 0, 4, 10)    # cell column, row, columns, rows of the filler tile
VS_FIRST, VS_PLACES = 0x235658, ((12, 2), (8, 6), (4, 2), (4, 6))   # the shared page: four mode 2 records in a row and their cell offsets
SHADOW_COLUMN, SHADOW_ROW = 8, 14        # the cells of the last record with y >= 0 are the portrait page's shadow cells at (x / 16 + 8, y / 16 + 14)
COORD_TABLE = 0x9f260                    # the arcade's table of pointers to coordinate blocks (the fighters' records use it too)
PAGE_BYTES = 0x8000


def block_cells(x, y, attr, code_of):
    """Cells (x, y, tile code) of one art block; attr bits 8-11 are the width - 1 and 12-15 the height - 1 in tiles.

    ValueError for a flip bit (bits 5 and 6) or a position that is not on the 16 pixel grid: no portrait record has either."""
    if attr & 0x60 or x & 15 or y & 15:
        raise ValueError('art block with a flip or off the cell grid')
    w, h = ((attr >> 8) & 15) + 1, ((attr >> 12) & 15) + 1
    return [(x + 16 * i, y + 16 * j, code_of(i + 16 * j)) for j in range(h) for i in range(w)]


def record4_cells(prog, a, add=SCROLL_ADD):
    """Cells (x, y, tile code) of the art record of mode 4 at a (header: mode, cells, blocks - 1; blocks: code, attribute, x, y)."""
    if a < 0 or a & 1 or a + 6 > len(prog) or struct.unpack_from('>H', prog, a)[0] != 4:
        raise ValueError(f'no mode 4 art record at {a:#x}')
    nb = struct.unpack_from('>H', prog, a + 4)[0] + 1
    if a + 6 + 8 * nb > len(prog):
        raise ValueError('art record runs past the program')
    out = []
    for k in range(nb):
        code, attr, x, y = struct.unpack_from('>HHhh', prog, a + 6 + 8 * k)
        out += block_cells(x, y, attr, lambda d, code=code: TILE_BANK + ((code + d + add) & 0xffff))
    return out


def record2_cells(prog, a):
    """(cells (x, y, tile code with bank 0x10000), end address) of the art record of mode 2 at a.

    Header: mode, cells, blocks - 1, coordinate index (signed, into the pointer table at 0x9f260); blocks: code, attribute;
    the coordinates (x, y) are 16-bit pairs at the pointer."""
    if a < 0 or a & 1 or a + 8 > len(prog) or struct.unpack_from('>H', prog, a)[0] != 2:
        raise ValueError(f'no mode 2 art record at {a:#x}')
    nb, idx = struct.unpack_from('>H', prog, a + 4)[0] + 1, struct.unpack_from('>h', prog, a + 6)[0]
    if a + 8 + 4 * nb > len(prog) or not 0 <= COORD_TABLE + idx <= len(prog) - 4:
        raise ValueError('art record runs past the program')
    ptr = struct.unpack_from('>I', prog, COORD_TABLE + idx)[0]
    if ptr & 1 or ptr + 4 * nb > len(prog):
        raise ValueError('coordinates past the program')
    out = []
    for k in range(nb):
        code, attr = struct.unpack_from('>HH', prog, a + 8 + 4 * k)
        x, y = struct.unpack_from('>hh', prog, ptr + 4 * k)
        out += block_cells(x, y, attr, lambda d, code=code: TILE_BANK + ((code + d) & 0xffff))
    return out, a + 8 + 4 * nb


def page_cell_of(x, y, ox, oy):
    cx, cy = x // 16 + ox, y // 16 + oy
    if not (0 <= cx < 16 and 0 <= cy < 16):
        raise ValueError('cell outside the page')
    return cx, cy


def portrait_tiles(prog, k):
    """{(cell column, cell row): tile code} of the portrait of arcade fighter k (a later block over an earlier one)."""
    if STEPS + STEP_STRIDE * k + 4 > len(prog):
        raise ValueError('fighter outside the step table')
    a = struct.unpack_from('>I', prog, STEPS + STEP_STRIDE * k)[0]
    return {page_cell_of(x, y, ORIGIN_X // 16, ORIGIN_Y // 16): code for x, y, code in record4_cells(prog, a)}


def shared_tiles(prog):
    """({cell: tile code} of the shared page, [(cell, tile code)] of the shadow cells of a portrait page in record order)."""
    out, shadow, a = {}, [], VS_FIRST
    for n, (ox, oy) in enumerate(VS_PLACES):
        cells, a = record2_cells(prog, a)
        for x, y, code in cells:
            out[page_cell_of(x, y, ox, oy)] = code
            if n == len(VS_PLACES) - 1 and y >= 0:
                shadow.append((page_cell_of(x, y, SHADOW_COLUMN, SHADOW_ROW), code))
    return out, shadow


def unplanar(tile):
    """The 256 pixel indices (row by row) of the arcade's 16 x 16 tile of 128 bytes (planar's inverse)."""
    return [sum(((tile[y * 8 + x // 8 * 4 + p] >> (7 - x % 8)) & 1) << p for p in range(4)) for y in range(16) for x in range(16)]


def cell_pixels(gfx, code):
    """The PS1 pixels (arcade index + 1, modulo 16) of arcade tile `code`; ValueError outside the graphics."""
    if code < 0 or code * 128 + 128 > len(gfx):
        raise ValueError(f'tile {code:#x} outside the graphics')
    return bytearray((v + 1) & 15 for v in unplanar(gfx[code * 128:code * 128 + 128]))


def put_cell(page, cx, cy, pix):
    """Write the 256 pixels of cell (cx, cy) into a 256 pixel wide 4 bits a pixel page (low nibble first)."""
    for y in range(16):
        for x in range(0, 16, 2):
            page[(cy * 16 + y) * 128 + cx * 8 + x // 2] = pix[y * 16 + x] | pix[y * 16 + x + 1] << 4


def shared_page(prog, gfx):
    """The shared page (the "VS" lettering and its shadow shapes), 0x8000 bytes."""
    page = bytearray(PAGE_BYTES)
    for (cx, cy), code in shared_tiles(prog)[0].items():
        put_cell(page, cx, cy, cell_pixels(gfx, code))
    return bytes(page)


def portrait_page(prog, gfx, k, shadow=0, drop=None, extras=()):
    """The portrait page of arcade fighter k, 0x8000 bytes: the record's tiles (minus the cell `drop`), the backdrop, the
    cells of the shared page that `extras` [(cx, cy, vx, vy)] place (the tile at cell (vx, vy) of the shared page at (cx, cy)), the
    shadow cells whose bit is set in `shadow` (bit i: the i-th shadow cell of shared_tiles), and the 18 palettes at rows 160 to 177."""
    page = bytearray(PAGE_BYTES)
    for (cx, cy), code in portrait_tiles(prog, k).items():
        if (cx, cy) != drop:
            put_cell(page, cx, cy, cell_pixels(gfx, code))
    bx, by, bw, bh = BACKDROP_CELLS
    back = cell_pixels(gfx, BACKDROP_TILE)
    for cy in range(by, by + bh):
        for cx in range(bx, bx + bw):
            put_cell(page, cx, cy, back)
    if extras:
        shared = shared_tiles(prog)[0]
        for cx, cy, vx, vy in extras:
            put_cell(page, cx, cy, cell_pixels(gfx, shared[(vx, vy)]))
    cells = shared_tiles(prog)[1] if shadow else []
    if shadow >> len(cells):
        raise ValueError(f'shadow cell {len(cells)} does not exist')
    for i, ((cx, cy), code) in enumerate(cells):
        if shadow >> i & 1:
            put_cell(page, cx, cy, cell_pixels(gfx, code))
    for i, a in enumerate(portrait_addresses_of(k)):
        if a + 32 > len(prog):
            raise ValueError('portrait palette outside the program')
        struct.pack_into('<16H', page, (PORTRAIT_FIRST_ROW + i) * 128, *portrait_palette(words_of(prog, a)))
    return bytes(page)


def derive_portrait(prog, gfx, k, disc_page):
    """The page descriptor (shadow mask, dropped cell or None, equal) that reproduces the disc's portrait page of fighter k.

    The mask has a bit for each shadow cell the disc page holds; the dropped cell is the one cell of the record the disc has
    blank (more than one: not reproduced); equal says the rebuilt page is the disc's, byte for byte."""
    try:
        cells = portrait_tiles(prog, k)
        shadow = shared_tiles(prog)[1]
        mask = sum(1 << i for i, ((cx, cy), code) in enumerate(shadow) if page_cell(disc_page, cx, cy) == cell_pixels(gfx, code))
        blank = [c for c, code in sorted(cells.items()) if not any(page_cell(disc_page, *c)) and any(cell_pixels(gfx, code))]
        if len(blank) > 1:
            return dict(mask=mask, drop=None, equal=False)
        drop = blank[0] if blank else None
        return dict(mask=mask, drop=drop, equal=portrait_page(prog, gfx, k, mask, drop) == disc_page)
    except ValueError:
        return dict(mask=0, drop=None, equal=False)


# Files whose portrait page the PS1 made its own in a way the arcade explains but for the palettes: the page is built the arcade's way and
# differs from the disc's in the cells that hold the 18 portrait palettes (the PS1's own colours there), served owned. PL14 (the alternate
# Gouki): its page is Gouki's portrait (arcade fighter 2), the backdrop, all 12 shadow cells and 28 cells of the shared (VS) page at three
# cell offsets (the VS letters), measured 2026-10-24: against the page built for fighter 2 with all the shadow cells, the disc's differs in 8
# cells, which are the palette rows' (cells 0 to 3, rows 10 and 11), and in 28 cells that are tiles of the shared records.
OWNED_PAGES = {'PL14'}
PALETTE_CELLS = {(cx, cy) for cx in range(4) for cy in (10, 11)}


def derive_extras(prog, gfx, k, disc_page, shadow, drop):
    """The cells [(cx, cy, vx, vy)] that the disc's portrait page holds besides the record, the backdrop and the shadow cells of `shadow`, each a tile
    of the shared page's cell (vx, vy) (the last cell of the shared page with those pixels), or None when a cell is neither that nor a palette
    cell."""
    base = portrait_page(prog, gfx, k, shadow, drop)
    shared = shared_tiles(prog)[0]
    by_pixels = {}
    for cell, code in shared.items():
        by_pixels[bytes(cell_pixels(gfx, code))] = cell
    out = []
    for cy in range(16):
        for cx in range(16):
            mine, theirs = page_cell(base, cx, cy), page_cell(disc_page, cx, cy)
            if mine == theirs or (cx, cy) in PALETTE_CELLS:
                continue
            v = by_pixels.get(bytes(theirs))
            if v is None:
                return None
            out.append((cx, cy, v[0], v[1]))
    return out


def planar(pixels):
    """The 128 bytes of the arcade's 16 x 16 tile format for 256 pixel indices (inverse of port/intro/import_scene.tile)."""
    out = bytearray(128)
    for y in range(16):
        for x in range(16):
            v = pixels[y * 16 + x]
            for plane in range(4):
                if (v >> plane) & 1:
                    out[y * 8 + x // 8 * 4 + plane] |= 1 << (7 - x % 8)
    return bytes(out)


def page_cell(page, cx, cy, width=256):
    """The 256 4-bit pixels (low nibble first) of cell (cx, cy) of a 4 bits a pixel picture, width in pixels."""
    pix = bytearray(256)
    for y in range(16):
        row = (cy * 16 + y) * (width // 2)
        for x in range(16):
            b = page[row + (cx * 16 + x) // 2]
            pix[y * 16 + x] = b >> 4 if (cx * 16 + x) & 1 else b & 15
    return pix


def tile_of_cell(pix, tiles):
    """Arcade tile number of a PS1 cell (its pixel indices are the arcade's plus one), or None."""
    return tiles.get(planar([(p - 1) & 15 for p in pix]))


def classify_page(page, tiles):
    """(grid of 16 rows of 16 entries, palette rows) of a 256 x 256 4-bit page.

    Entries: None for a blank cell, an arcade tile number for a cell that is one, -1 for any other cell. Palette rows
    are the rows y (of the first 16 halfwords) whose colors 1..15 all have bit 15 set and color 0 is 0: the entry of
    those cells is taken out of the grid (-2)."""
    rows = [y for y in range(256) if (lambda hw: hw[0] == 0 and all(v & 0x8000 for v in hw[1:]))(struct.unpack_from('<16H', page, y * 128))]
    grid = []
    for cy in range(16):
        line = []
        for cx in range(16):
            pix = page_cell(page, cx, cy)
            if not any(pix):
                line.append(None)
            elif cx < 4 and all(cy * 16 + y in rows or not any(page[(cy * 16 + y) * 128:(cy * 16 + y) * 128 + 128]) for y in range(16)):
                line.append(-2)
            else:
                t = tile_of_cell(pix, tiles)
                line.append(-1 if t is None else t)
        grid.append(line)
    return grid, rows


# ---------------------------------------------------------------- inputs

def iso_files(f):
    """{path: (lba, size)} of the disc's ISO 9660 files (raw 2352-byte sectors, data at +24), and the sector reader."""
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


def words_of(prog, address):
    """16 big-endian words at an arcade program address."""
    return struct.unpack_from('>16H', prog, address)


# ---------------------------------------------------------------- the analysis

def split_of(pl, pals, prog):
    """Where the first odd block ends: the number of leading odd palettes (of 32) that equal the arcade's at its place."""
    k = FIGHTER_OF[pl]
    for p in range(32):
        if list(pals[2 * p + 1]) != ps1_palette(words_of(prog, B_BASE + B_STRIDE * k + PALETTE_BYTES * p)):
            return p
    return 32


def x_candidates(prog, pals, split):
    """(address, layout) pairs that may hold the second odd block, most voted first (the default 0x29c300 is always last).

    Votes: where the first two colors of one of the odd palettes from split up are found, less its offset in the layout."""
    want = [pals[2 * p + 1] for p in range(split, 32)]
    votes = collections.Counter()
    firsts = collections.defaultdict(list)
    for i, p in enumerate(want):
        firsts[tuple(p[1:3])].append(i)
    for a in range(0, len(prog) - 32, 32):
        w = struct.unpack_from('>2H', prog, a)
        for i in firsts.get((ps1_color(w[0]), ps1_color(w[1])), ()):
            for layout in ('flat', 'fives'):
                off = c_offset(i, layout)
                if off is not None and a - off >= 0:
                    votes[(a - off, layout)] += 1
    out = [k for k, _v in votes.most_common(16) if k[0] + 32 * 32 <= len(prog)]
    return out + [(C_DEFAULT, 'flat')]


def compare_palettes(pl, pals, prog, x_base, layout, split):
    """(exact, ramps with red 0, ramps with red 7, others) of the 64 palettes against the layout.

    Each other is (number, equal colors of 16, address tried or None)."""
    addr = palette_addresses(pl, x_base, layout, split)
    exact = ramp0 = ramp7 = 0
    others = []
    for n, (p, a) in enumerate(zip(pals, addr)):
        mine = ps1_palette(words_of(prog, a)) if a is not None else None
        if mine is not None and list(p) == mine:
            exact += 1
            continue
        start = is_ramp(p[1:])
        if start is None:
            others.append((n, 0 if mine is None else sum(1 for x, y in zip(p, mine) if x == y), a))
        elif start >> 8 == 7:
            ramp7 += 1
        else:
            ramp0 += 1
    return exact, ramp0, ramp7, others


def table1(pl, chunk, prog):
    """Compare one table 1 chunk (64 palettes) with the arcade.

    The second odd block is the candidate (address and layout) that reproduces most palettes, then has fewest others, then
    most equal colors, then the lowest address. Returns the counts of palettes that equal the arcade's at the layout's
    address (exact), that are ramps with red 0 (in the arcade, address not in the layout) or with red 7 (no arcade
    source), and the others, with each other palette's number and how many of its 16 colors equal the arcade's."""
    pals = [struct.unpack_from('<16H', chunk, 32 * n) for n in range(64)]
    split = split_of(pl, pals, prog)
    best = None
    for base, layout in x_candidates(prog, pals, split):
        exact, ramp0, ramp7, others = compare_palettes(pl, pals, prog, base, layout, split)
        score = (exact, -len(others), sum(k for _n, k, _a in others), -base)
        if best is None or score > best[0]:
            best = (score, base, layout, exact, ramp0, ramp7, others)
    _score, base, layout, exact, ramp0, ramp7, others = best
    return dict(exact=exact, ramp0=ramp0, ramp7=ramp7, other=len(others), others=others, c_base=base, layout=layout, split=split)


def table2(pl, side, page, prog, tiles):
    """Classify one table 2 page. Returns counts of cells and, for a portrait page, how many of its 18 palettes the layout reproduces."""
    grid, rows = classify_page(page, tiles)
    flat = [e for line in grid for e in line]
    r = dict(blank=sum(1 for e in flat if e is None), tiles=sum(1 for e in flat if e is not None and e >= 0),
             palette_cells=sum(1 for e in flat if e == -2), unknown=sum(1 for e in flat if e == -1), pal_rows=0, pal_ok=0)
    if r['palette_cells']:
        want = portrait_addresses(pl)
        for i, a in enumerate(want):
            y = PORTRAIT_FIRST_ROW + i
            hw = list(struct.unpack_from('<16H', page, y * 128))
            r['pal_rows'] += 1
            if hw == portrait_palette(words_of(prog, a)):
                r['pal_ok'] += 1
    return r


def summary(t, nfiles):
    """The closing lines: bytes of each kind and how much of it the rules above reproduce."""
    lines = [f'totals over {nfiles} files (bytes):',
             f'  table 1 palettes {t["t1 bytes"]}: arcade {t["t1 arcade"]}, filler ramps {t["t1 ramp"]}, stay the disc\'s {t["t1 kept"]} '
             f'(the PS1\'s own colors), model differs from the disc where it builds {t["t1 wrong"]}',
             f'  table 2 pages {t["t2 bytes"]}: built equal to the disc\'s {t["t2 equal"]} ({t["t2 portrait"]} bytes of portrait pages, {t["t2 shared"]} of shared pages), '
             f'stay the disc\'s {t["t2 kept"]}, built but different {t["t2 wrong"]}',
             f'  table 3 slot 4/5 sound command lists {t["t3 sound command bytes"]}: no arcade source',
             f'  table 5 sequences {t["t5 sequence bytes"]} ({t["t5 sequence magic ok"]} of {t["t5 sequences"]} start with pQES): no arcade source',
             f'  table 0 slot 0xffff {t["zeros bytes"]} ({t["zero chunks all zero"]} of {t["zero chunks"]} chunks all zero): padding read and discarded',
             f'  not treated here: sound bank headers {t["sound bank header bytes (not here)"]}, sound bank bodies {t["t4 bytes (not here)"]}, '
             f'table 0 code and tiles {t["code or tile bytes (not here)"]}']
    return '\n'.join(lines)


# ---------------------------------------------------------------- the descriptors of the runtime C

def parts_row(name, chunks, prog, gfx, shared):
    """The descriptor row of one fighter file from its chunks [(table, bytes)] in file order, with what the model builds.

    Keys: file, pl, fighter, desc (palette_chunk's), palette_chunks, pages (a string of 'P' and 'V' in chunk order),
    shadow, drop, page_disc (the portrait page stays the disc's), t1 and t2 (the disc's chunks)."""
    pl = int(name[2:4], 16)
    k = FIGHTER_OF[pl]
    t1 = [d for t, d in chunks if t == 1 and len(d) == 2048]
    t2 = [d for t, d in chunks if t == 2]
    row = dict(file=name + '.PAC', pl=pl, fighter=k, t1=t1, t2=t2, palette_chunks=len(t1), shadow=0, drop=None, page_disc=False)
    row['desc'] = derive_palettes(pl, t1[0], prog) if t1 else None
    row['pages'] = ''.join('V' if d == shared else 'P' for d in t2)
    portraits = [d for d, kind in zip(t2, row['pages']) if kind == 'P']
    row.update(extras=[], owned=False)
    if portraits:
        r = derive_portrait(prog, gfx, k, portraits[0])
        row.update(shadow=r['mask'], drop=r['drop'], page_disc=not r['equal'] or any(d != portraits[0] for d in portraits))
        if row['page_disc'] and name.removesuffix('.PAC').removesuffix('X') in OWNED_PAGES and all(d == portraits[0] for d in portraits):
            extras = derive_extras(prog, gfx, k, portraits[0], r['mask'], r['drop'])
            if extras is not None:
                row.update(extras=extras, owned=True, page_disc=False)
        if row['page_disc']:
            row.update(shadow=0, drop=None)
    return row


def chunk_report(row, prog, gfx, shared):
    """Per chunk, what the model builds and how it compares with the disc: ([(kind, built, equal, bytes of the chunk)], palettes):
    kind 'palette', 'portrait' or 'shared'; built is False for a chunk that stays the disc's; equal says the built part is the disc's;
    palettes is (arcade, ramp, kept) counts of the palette chunk."""
    out, counts = [], (0, 0, 0)
    if row['desc'] is not None:
        built, kept = palette_chunk(row['pl'], prog, row['desc'])
        ramps = row['desc']['ramps']
        n_ramp = sum(1 for n in range(64) if not kept >> n & 1 and ramps and ramp_index(n) is not None and ramps[ramp_index(n)] is not None)
        n_kept = bin(kept).count('1')
        counts = (64 - n_ramp - n_kept, n_ramp, n_kept)
        for d in row['t1']:
            same = all(built[32 * n:32 * n + 32] == d[32 * n:32 * n + 32] for n in range(64) if not kept >> n & 1)
            out.append(('palette', True, same, len(d)))
    for d, kind in zip(row['t2'], row['pages']):
        if kind == 'V':
            out.append(('shared', True, shared_page(prog, gfx) == d, len(d)))
        elif row['page_disc']:
            out.append(('portrait', False, True, len(d)))
        else:
            out.append(('portrait', True, portrait_page(prog, gfx, row['fighter'], row['shadow'], row['drop'], row['extras']) == d or row['owned'], len(d)))
    return out, counts


def flags_text(row):
    parts = (['ARCADE_PARTS_PAGE_DISC'] if row['page_disc'] else []) + (['ARCADE_PARTS_PAGE_OWNED'] if row['owned'] else [])
    return ' | '.join(parts) or '0u'


def c_table(rows):
    """The text of fighter_parts_table.c for descriptor rows from parts_row."""
    out = ['/* GENERATED by `python3 port/arcade/fighter_parts.py --disc DISC --arcade SET --c-table`; do not edit.',
           ' * For each of the 45 fighter files: the arcade fighter whose palettes and portrait it holds, where the second block of',
           " * odd palettes lies and how it is laid out, the palettes that stay the disc's (a bit each) and the start words of the",
           " * PS1's own filler ramps, the file's table 2 chunks in order (P portrait, V shared), which of the 12 shadow cells the",
           ' * portrait page holds, whether the page stays the disc\'s or is built the arcade\'s way (owned), the one cell it leaves blank and the',
           ' * cells of the shared page it holds besides (cell, source cell). Only addresses, counts',
           ' * and bits: no byte of either game. */',
           '#include "fighter_parts.h"', '', '#define NR ARCADE_PARTS_NO_RAMP', '']
    ramp_names, order = {}, []
    for row in rows:
        r = row['desc'] and row['desc']['ramps']
        if r and tuple(r) not in ramp_names:
            ramp_names[tuple(r)] = f'ramps_{len(ramp_names)}'
            order.append(tuple(r))
    for key in order:
        out.append(f'static const uint16_t {ramp_names[key]}[ARCADE_PARTS_RAMPS] = {{')
        cells = ['NR' if v is None else f'{v:#x}u' for v in key]
        out += ['    ' + ', '.join(cells[i:i + 9]) + ',' for i in range(0, len(cells), 9)]
        out.append('};')
        out.append('')
    cell_names = {}
    for row in rows:
        key = tuple(row['extras'])
        if key and key not in cell_names:
            cell_names[key] = 'cells_' + row['file'].removesuffix('.PAC').lower()
            out.append(f'static const struct arcade_parts_cell {cell_names[key]}[] = {{')
            out += [f'    {{{cx}u, {cy}u, {vx}u, {vy}u}},' for cx, cy, vx, vy in key]
            out += ['};', '']
    out.append('const struct arcade_parts_file arcade_parts_files[] = {')
    for row in rows:
        d = row['desc']
        cells_ref = f'{cell_names[tuple(row["extras"])]}, {len(row["extras"])}u' if row['extras'] else 'NULL, 0u'
        ramps = 'NULL' if not (d and d['ramps']) else ramp_names[tuple(d['ramps'])]
        dx, dy = row['drop'] if row['drop'] else (-1, -1)
        out.append(f'    {{"{row["file"]}", {row["fighter"]}u, ARCADE_PARTS_{"FIVES" if d and d["layout"] == "fives" else "FLAT"}, '
                   f'{d["split"] if d else 16}u, {row["palette_chunks"]}u, {d["base"] if d else 0:#x}u, {d["keep"] if d else 0:#x}ull, {ramps}, '
                   f'"{row["pages"]}", {row["shadow"]:#x}u, {flags_text(row)}, {dx}, {dy}, {cells_ref}}},')
    out += ['};', 'const unsigned arcade_parts_count = sizeof arcade_parts_files / sizeof arcade_parts_files[0];', '', '#undef NR', '']
    return '\n'.join(out)


class CLib:
    """fighter_parts.c with the checked-in table, compiled into a temporary folder and called through ctypes."""

    class File(ctypes.Structure):
        pass

    def __init__(self, tmp):
        here = Path(__file__).resolve().parent
        lib = Path(tmp) / 'fighter_parts.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-shared', '-fPIC', '-o', str(lib),
                        str(here / 'fighter_parts.c'), str(here / 'fighter_parts_table.c')], check=True, capture_output=True, timeout=60)
        self.lib = ctypes.CDLL(str(lib))
        self.File._fields_ = [('file', ctypes.c_char_p), ('fighter', ctypes.c_uint8), ('layout', ctypes.c_uint8), ('split', ctypes.c_uint8),
                              ('palette_chunks', ctypes.c_uint8), ('base', ctypes.c_uint32), ('keep', ctypes.c_uint64),
                              ('ramps', ctypes.POINTER(ctypes.c_uint16)), ('pages', ctypes.c_char_p), ('shadow', ctypes.c_uint16),
                              ('flags', ctypes.c_uint8), ('drop_x', ctypes.c_int8), ('drop_y', ctypes.c_int8), ('cells', ctypes.c_void_p),
                              ('cell_count', ctypes.c_uint)]
        count = ctypes.c_uint.in_dll(self.lib, 'arcade_parts_count').value
        self.rows = (self.File * count).in_dll(self.lib, 'arcade_parts_files')
        fp, cp = ctypes.POINTER(self.File), ctypes.c_char_p
        self.lib.arcade_parts_palettes.argtypes = [cp, ctypes.c_size_t, fp, ctypes.c_char_p, ctypes.POINTER(ctypes.c_uint64), cp, ctypes.c_size_t]
        self.lib.arcade_parts_portrait.argtypes = [cp, ctypes.c_size_t, cp, ctypes.c_size_t, fp, ctypes.c_char_p, cp, ctypes.c_size_t]
        self.lib.arcade_parts_shared.argtypes = [cp, ctypes.c_size_t, cp, ctypes.c_size_t, ctypes.c_char_p, cp, ctypes.c_size_t]

    def row(self, name):
        return next((r for r in self.rows if r.file.decode() == name), None)

    def palettes(self, prog, row):
        """(bytes, kept mask) or an error string."""
        out, kept, err = ctypes.create_string_buffer(2048), ctypes.c_uint64(), ctypes.create_string_buffer(256)
        if self.lib.arcade_parts_palettes(prog, len(prog), ctypes.byref(row), out, ctypes.byref(kept), err, len(err)):
            return err.value.decode()
        return out.raw, kept.value

    def portrait(self, prog, gfx, row):
        """bytes, None (stays the disc's) or an error string."""
        out, err = ctypes.create_string_buffer(PAGE_BYTES), ctypes.create_string_buffer(256)
        rc = self.lib.arcade_parts_portrait(prog, len(prog), gfx, len(gfx), ctypes.byref(row), out, err, len(err))
        return err.value.decode() if rc < 0 else (None if rc else out.raw)

    def shared(self, prog, gfx):
        out, err = ctypes.create_string_buffer(PAGE_BYTES), ctypes.create_string_buffer(256)
        if self.lib.arcade_parts_shared(prog, len(prog), gfx, len(gfx), out, err, len(err)):
            return err.value.decode()
        return out.raw


def compare_line(name, row, clib, prog, gfx, shared):
    """One line: the C's palettes and pages against the model and the disc, and the palettes that stay the disc's."""
    crow = clib.row(name + '.PAC')
    if crow is None:
        return f'{name}: not in the C table'
    parts, t = [], time.perf_counter()
    if row['desc'] is not None:
        built, kept = palette_chunk(row['pl'], prog, row['desc'])
        got = clib.palettes(prog, crow)
        if isinstance(got, str):
            parts.append(f'palettes: C refused: {got}')
        else:
            cb, ck = got
            same_model = cb == built and ck == kept
            equal = [all(cb[32 * n:32 * n + 32] == d[32 * n:32 * n + 32] for n in range(64) if not ck >> n & 1) for d in row['t1']]
            kept_list = [n for n in range(64) if ck >> n & 1]
            parts.append(f'palettes C {"=" if same_model else "!="} model, C {"=" if all(equal) else "!="} disc ({64 - len(kept_list)} built'
                         + (f'; the disc\'s: {" ".join(map(str, kept_list))}' if kept_list else '') + ')')
    pages = []
    portrait_seen = False
    for d, kind in zip(row['t2'], row['pages']):
        if kind == 'V':
            got = clib.shared(prog, gfx)
            model = shared_page(prog, gfx)
        else:
            got = clib.portrait(prog, gfx, crow)
            model = None if row['page_disc'] else portrait_page(prog, gfx, row['fighter'], row['shadow'], row['drop'], row['extras'])
        if isinstance(got, str):
            pages.append(f'{kind} C refused: {got}')
        elif got is None:
            pages.append(f"{kind} stays the disc's" + (' (model agrees)' if model is None else ' (model builds it!)'))
        else:
            by_design = row['owned'] and got != d
            pages.append(f'{kind} C {"=" if got == model else "!="} model, C ' + (f'differs from the disc by design ({sum(1 for cx in range(16) for cy in range(16) if page_cell(got, cx, cy) != page_cell(d, cx, cy))} cells)'
                                                                                 if by_design else f'{"=" if got == d else "!="} disc'))
    if pages:
        parts.append('pages ' + '; '.join(pages))
    return f'{name}: ' + '; '.join(parts) + f'; {(time.perf_counter() - t) * 1000:.1f} ms'


def load(disc, only=None):
    """[(name, [(table, slot, bytes)] in file order)] of the fighter files of the disc."""
    out = []
    with open(disc, 'rb') as f:
        files, sec = iso_files(f)
        names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
        if only:
            want = only.upper().removesuffix('.PAC') + '.PAC'
            names = [p for p in names if p.endswith('/' + want)]
        for p in names:
            lba, size = files[p]
            data = sec(lba, (size + 2047) // 2048)[:size]
            name = p.split('/')[-1][:-4]
            try:
                chunks = pac.chunk_table(data)
            except (pac.FormatError, struct.error) as exc:
                print(f'{name}: {exc}')
                continue
            out.append((name, [(c['table'], c['slot'], data[c['offset']:c['offset'] + c['size']]) for c in chunks]))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00.PAC or PL00)')
    ap.add_argument('--detail', action='store_true', help='one extra line per file for the palettes that stay the disc\'s and the shadow mask')
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/fighter_parts_table.c (the descriptors of the files the runtime C builds)')
    ap.add_argument('--compare-c', action='store_true', help='build fighter_parts.c with the checked-in table and compare it with the model and the disc, per file')
    args = ap.parse_args(argv)
    started = time.monotonic()
    prog, gfx = arcade_views(args.arcade)
    shared = shared_page(prog, gfx)
    loaded = load(args.disc, args.only)
    total = collections.Counter()
    rows = []
    for name, chunks in loaded:
        row = parts_row(name, [(t, d) for t, _s, d in chunks], prog, gfx, shared)
        rows.append(row)
    if args.c_table:
        print(c_table(rows))
        return 0
    if args.compare_c:
        with tempfile.TemporaryDirectory(prefix='fighter-parts-') as tmp:
            clib = CLib(tmp)
            bad = 0
            for row in rows:
                text = compare_line(row['file'][:-4], row, clib, prog, gfx, shared)
                bad += ' != ' in text or 'refused' in text or 'not in the C table' in text
                print(text)
            print(f'{len(rows)} files, {bad} with a difference' + f', {time.monotonic() - started:.1f} s')
        return 1 if bad else 0
    for (name, chunks), row in zip(loaded, rows):
        report, counts = chunk_report(row, prog, gfx, shared)
        bits = []
        if row['desc'] is not None:
            d = row['desc']
            bits.append(f'table 1: {counts[0]} arcade, {counts[1]} ramps, {counts[2]} the disc\'s of 64 palettes '
                        f'(odd blocks split at {d["split"]}, second {d["layout"]} at {d["base"]:#x})')
            if args.detail and d['keep']:
                bits.append('palettes that stay the disc\'s: ' + ' '.join(str(n) for n in range(64) if d['keep'] >> n & 1))
        for kind, built, equal, size in report:
            if kind == 'palette':
                total['t1 bytes'] += size
                total['t1 arcade'] += 32 * counts[0]
                total['t1 ramp'] += 32 * counts[1]
                total['t1 kept'] += 32 * counts[2]
                total['t1 wrong'] += 0 if equal else size
            else:
                total['t2 bytes'] += size
                if not built:
                    total['t2 kept'] += size
                elif equal:
                    total['t2 equal'] += size
                    total['t2 portrait' if kind == 'portrait' else 't2 shared'] += size
                else:
                    total['t2 wrong'] += size
        if row['pages']:
            bits.append('table 2: ' + ' '.join(f'{kind} ' + ('the disc\'s' if not built else 'equal' if equal else 'DIFFERENT') for kind, built, equal, _s in report if kind != 'palette')
                        + (f'; shadow cells {row["shadow"]:#05x}' + (f', cell {row["drop"]} blank' if row['drop'] else '') if args.detail and 'P' in row['pages'] else ''))
        print(f'{name}: ' + '; '.join(bits))
        for c in chunks:
            t, s, d = c
            total['file bytes'] += len(d)
            if t == 3 and s in (4, 5):
                total['t3 sound command bytes'] += len(d)
            elif t == 5:
                total['t5 sequence bytes'] += len(d)
                total['t5 sequence magic ok'] += 1 if d[:4] == b'pQES' else 0
                total['t5 sequences'] += 1
            elif t == 3:
                total['sound bank header bytes (not here)'] += len(d)
            elif t == 0 and s == 0xffff:
                total['zeros bytes'] += len(d)
                total['zero chunks'] += 1
                total['zero chunks all zero'] += 1 if not any(d) else 0
            elif t == 0:
                total['code or tile bytes (not here)'] += len(d)
            elif t == 4:
                total['t4 bytes (not here)'] += len(d)
    print(summary(total, len(loaded)))
    print(f'{time.monotonic() - started:.1f} s')
    return 0


if __name__ == '__main__':
    sys.exit(main())
