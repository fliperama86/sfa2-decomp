#!/usr/bin/env python3
"""Analysis tool: survey of the disc files that the fighter, stage, sample and program-data tools do not cover, and how much of each the arcade set gives.

What it does. The PS1 disc holds, besides the fighter files (PLnn), the stage files (STAGEnn), the sound banks and the program's data, these
files: ASCII.PAC (fonts and HUD), DEMO.PAC (title, ranking), SELECT.PAC and SELECTA.PAC (character select), FIRSTSND.PAC and GAME_SND.PAC
(sound), CONTnn(X).PAC (continue screen), ENDnn.PAC (endings), BOSSnn, GDEMOnn and RDMnn.PAC (talk before a fight), CDEMOnn.PAC and
NDEMOnn.PAC (a fighter's picture and name plate), the XA music files, the STR movies and the stand-alone PS-X EXE programs. This tool
opens each, splits each archive chunk by what it holds (the loader's table and slot, then the content), and measures for each part how
much of it is the arcade's, by the methods of the other tools in this folder:

  - tiles (compressed sprite tiles, raw 128-byte tiles, and the 16 x 16 cells of the pictures the loader copies to video memory):
    equal to an arcade graphics tile ('tile_arcade', 'c_tile'), with the colour index one higher (arcade 15, transparent, is PS1 0),
    flips allowed (as fighters.py and stages.py); else made of arcade 8 x 8 pieces, flips allowed ('tile_pieces', 'c_pieces'; the scroll
    layers' text tiles and the PS1's repacked backdrops are such pieces); else some pieces are ('part'), or none ('own'). Flat and
    blank pieces count as matching. Arcade tile codes and palette addresses found are listed with --detail;
  - palettes (16 words of 5-bit colours, black as 0 or 0x8000, bit 15 ignored): the arcade's 15 colour words at brightness 0xe
    (stages.Helper.palettes; the doubling rule of fighter_parts.py gives the same colours), found anywhere in the arcade program; the
    rest are counted as filler ramps (fighter_parts.is_ramp), flat palettes or the PS1's own;
  - program modules (the table 0 chunks that hold code): runs of 32 bytes or more equal to the arcade program, 16-bit fields turned
    (program_map.py's search), and chains of cell lists in the fighters' format (program_map.find_cell_chains);
  - sound banks: programs, tones and samples counted (samples.py match has the arcade share of the samples);
  - with --lists, the cell lists of the modules that hold them (fighters.analyse): how many have tiles that are a run of an arcade art record;
  - XA files, STR movies, EXE programs: their structure only (no arcade source is possible for recorded audio and video).

A chunk that mixes kinds (the continue screen's sprite chunk, the fighter picture chunk of CDEMO, the name plate chunk of NDEMO) is
split into regions: compressed tile segments (offset table, then blocks, as tilepack.py reads them), palette runs, zero runs and the
rest (pictures). The loader's own order of things (for a TIM header, the rectangle says how many bytes the loader reads, not the block
size) is followed. Every chunk is reported once for each distinct content, with the number of files that carry it.

The converter that the port runs is C (screens.c, with the table screens_table.c). The model it reproduces, and the finder of each chunk's
recipe (what to build from the arcade, what stays the disc's), are in screens.py (its docstring says what a recipe is). Two options use them:

  --c-table     print port/arcade/screens_table.c: the recipes of every chunk of the select, demo, continue, ending, boss and character-demo
                files that has tiles, palettes or pictures from the arcade (addresses, counts and layouts, no game bytes);
  --compare-c   build screens.c with the checked-in table and, for each file and chunk, compare the C with the model (screens.py's
                model_chunk) and with the disc at the bytes it claims (a packed tile segment is compared as the tiles it decodes to), then print
                the bytes built, the bytes equal to the disc and the bytes the arcade gives alone, by file kind.

Not covered by any recipe (they stay the disc's): the modules (code, and the cell lists of the select screen module are not byte-equal to the
fighters' rules), the sound banks, the four full-colour ending illustrations, the title and ranking screens' own pictures, the select screen's
cell map, the name plate cell map, the PS1's own tiles, palettes and cells inside the chunks that do have a recipe.

Inputs: the disc image (raw 2352-byte sectors, --disc) and the arcade set's ZIP (--arcade, MAME's sfa2u), both only read. It writes nothing
except a temporary folder that holds a small C helper compiled at run time, removed on exit. It prints counts, addresses and sizes,
never game bytes (--c-table prints the C table to the standard output).

    python3 port/arcade/other_files.py --disc BIN --arcade ZIP [--only ASCII] [--no-modules] [--detail] [--lists]
    python3 port/arcade/other_files.py --disc BIN --arcade ZIP --c-table > port/arcade/screens_table.c
    python3 port/arcade/other_files.py --disc BIN --arcade ZIP --compare-c
"""
import argparse
import collections
import ctypes
import hashlib
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import pac            # noqa: E402  the chunk archive reader of the PS1 tools
import tilepack       # noqa: E402  the compressed tile reader of this folder
import stages         # noqa: E402  the disc and arcade readers and the palette search
import program_map    # noqa: E402  the arcade-equal data search and the cell list chains
import fighter_parts  # noqa: E402  the filler ramp test
import samples        # noqa: E402  the sound bank readers
import fighters       # noqa: E402  the cell list reader and its arcade art record index

SECTOR = 2048
JR_RA = 0x03E00008
ARCADE_BRIGHTNESS = 14      # the brightness of the PS1 palettes (stages.py)
MIN_SEGMENT_TILES = 4       # a compressed tile segment has at least this many tiles
MAX_TABLE = 0x20000         # the largest offset table of a segment (bytes)
MIN_PALETTE_RUN = 8         # palette rows in a row that make a palette run (a shorter run inside a picture is chance)
MIN_ZERO_RUN = 256
MIN_PALETTE_ROWS = 4        # palette rows in a row inside a picture
PAGE_BYTES = 32768          # one 256 x 256 picture of 4 bits a pixel
MIN_EQUAL_RUN = 32          # program_map's smallest table
SOUND_TABLES = (3, 4, 5)

# the stand-alone PS1 data that the disc has apart from the archives and what the loader does with each, by file name
# (module slot, tile chunk slot) of the kinds whose module holds cell lists in the fighters' format next to a compressed tile chunk
LIST_PAIRS = {'END': (0x28, 0x29), 'CDEMO': (1, 0x10), 'SELECTA': (0x27, 0x18), 'CONT': (0x12, 0x13), 'DEMO': (0xf, 0xc)}
KIND_ORDER = ('ASCII', 'DEMO', 'SELECT', 'SELECTA', 'FIRSTSND', 'GAME_SND', 'CONT', 'END', 'BOSS', 'RDM', 'GDEMO', 'CDEMO', 'NDEMO')

# What each chunk position is. Key: (kind, table, slot, ordinal among the chunks of that table and slot in the file).
# Value: (name, what it is and who reads it). Found from the loader (ps1/docs/overlays.md) and the consuming C under ps1/src/overlays.
WHAT = {
    ('ASCII', 1, 0, 0): ('palettes', '128 palettes to video memory (128,480): HUD and font colours'),
    ('ASCII', 0, 1, 0): ('raw tiles', '763 uncompressed tiles at 0x80010000, sent as 4 x 16 strips from x 0x340 (load_images_to_vram): HUD, round lettering, icons'),
    ('ASCII', 0, 9, 0): ('picture', 'font page, sent to (0x3c0,0x100) by load_images_to_vram'),
    ('ASCII', 0, 10, 0): ('palettes', '32 palettes at 0x80050000, the first 15 sent to two places at y 0x1f0'),
    ('DEMO', 3, 6, 0): ('sound commands', 'command list of the sound player (8 bytes a command)'),
    ('DEMO', 3, 2, 0): ('VAB header', 'sound bank header (slot 2)'),
    ('DEMO', 4, 2, 0): ('VAB body', 'sound bank samples (slot 2)'),
    ('DEMO', 5, 0x102, 0): ('sequence', 'pQES sequence (music or jingle)'),
    ('DEMO', 0, 0xe, 0): ('pictures', 'title screen: two of the three pages sent from 0x800c7000 to (0x180,0) by init_strips_and_load_three_images; the third page lies in the module'),
    ('DEMO', 1, 0, 0): ('palettes', '32 palettes to video memory (144,480)'),
    ('DEMO', 0, 0xc, 0): ('tile library', '19 compressed tile segments at 0x80061000: the shared sprites of the menus (the same chunk in SELECT and CONT)'),
    ('DEMO', 0, 0xd, 0): ('pictures', 'ranking screen pages at 0x800b7000, sent to (0x280,0) and (0x2c0,0)'),
    ('DEMO', 0, 0xf, 0): ('module', 'attract module (title, ranking, demo) at 0x800df000'),
    ('SELECT', 3, 4, 0): ('sound commands', 'command list'),
    ('SELECT', 3, 0, 0): ('VAB header', 'sound bank header (slot 0)'),
    ('SELECT', 4, 0, 0): ('VAB body', 'sound bank samples (slot 0)'),
    ('SELECT', 5, 0x100, 0): ('sequence', 'pQES sequence'),
    ('SELECT', 5, 0x101, 0): ('sequence', 'pQES sequence'),
    ('SELECT', 0, 0xc, 0): ('tile library', 'the same 19 segments as DEMO, at 0x80061000'),
    ('SELECT', 1, 0, 0): ('palettes', '21 palettes to video memory (336,480)'),
    ('SELECT', 0, 4, 0): ('module', 'select_screen_special module at 0x801b0000 (a fighter module slot)'),
    ('SELECTA', 3, 4, 0): ('sound commands', 'command list'),
    ('SELECTA', 3, 0, 0): ('VAB header', 'sound bank header (slot 0)'),
    ('SELECTA', 4, 0, 0): ('VAB body', 'sound bank samples (slot 0)'),
    ('SELECTA', 5, 0x100, 0): ('sequence', 'pQES sequence'),
    ('SELECTA', 5, 0x101, 0): ('sequence', 'pQES sequence'),
    ('SELECTA', 0, 0x18, 0): ('tiles', 'the select screen sprites: one compressed segment at 0x80038000'),
    ('SELECTA', 0, 0x19, 0): ('palettes', '32 palettes at 0x800e0000 (copied to rows 1 and 6 of the palette buffers)'),
    ('SELECTA', 0, 0x1a, 0): ('palettes', '32 palettes at 0x800e0400 (rows 2 and 7)'),
    ('SELECTA', 0, 0x1b, 0): ('palettes', '32 palettes at 0x800e0800 (rows 3 and 8)'),
    ('SELECTA', 0, 0x1c, 0): ('palettes', '32 palettes at 0x800e0c00 (rows 0 and 5)'),
    ('SELECTA', 0, 0x1d, 0): ('palettes', '126 palettes at 0x800e1000'),
    ('SELECTA', 0, 0x1e, 0): ('palettes', '378 palettes at 0x800e2000'),
    ('SELECTA', 0, 0x1f, 0): ('picture', 'one page at 0x800e5000, sent to (0x380,0x100)'),
    ('SELECTA', 0, 0x20, 0): ('palettes', '8 palettes at 0x800ed000'),
    ('SELECTA', 0, 0x21, 0): ('palettes', '10 palettes at 0x800ed400 (copied into row 2 at entry 0x160)'),
    ('SELECTA', 0, 0x22, 0): ('palettes', '32 palettes at 0x800ed800, sent to (0x40,0x1e0)'),
    ('SELECTA', 0, 0x23, 0): ('palettes', '22 palettes at 0x800edc00'),
    ('SELECTA', 0, 0x24, 0): ('palettes', '19 palettes at 0x800ee000'),
    ('SELECTA', 0, 0x25, 0): ('TIM picture', 'the select screen backdrop at 0x800ee400: a TIM (4 bits a pixel, 768 palettes) whose rectangle (3 pages) reads past the TIM block'),
    ('SELECTA', 0, 0x26, 0): ('cell map', '10,240 cells (texture word, page and palette word) at 0x801b0000 (init_primitives_and_copy_palette_block)'),
    ('SELECTA', 0, 0x27, 0): ('module', 'select_screen module at 0x80010000'),
    ('FIRSTSND', 3, 0, 0): ('VAB header', 'sound bank header (slot 0)'),
    ('FIRSTSND', 4, 0, 0): ('VAB body', 'sound bank samples (slot 0)'),
    ('FIRSTSND', 0, 0xffff, 0): ('zeros', '16,384 bytes the loader reads and discards (no table entry)'),
    ('GAME_SND', 0, 0xffff, 0): ('zeros', '16,384 bytes the loader reads and discards'),
    ('CONT', 0, 0x13, 0): ('sprite blob', 'continue screen art at 0x80040000: tile segments, palettes (0x80042284 on), one page (0x80043344), more segments (0x8004b344, 0x80055598)'),
    ('CONT', 0, 0x14, 0): ('tile library', 'the same 19 segments as DEMO, at 0x8005b000'),
    ('CONT', 0, 0x15, 0): ('pictures', 'two pages at 0x800b1000, sent to (0x280,0) and (0x2c0,0)'),
    ('CONT', 0, 0x12, 0): ('module', 'continue_screen module at 0x80010000'),
    ('CONT', 3, 4, 0): ('sound commands', 'command list'),
    ('END', 0, 0x28, 0): ('module', 'ending module at 0x80010000'),
    ('END', 0, 0x29, 0): ('tiles', 'the ending art of the fighter: one compressed segment at 0x80060000'),
    ('END', 0, 0x2d, 0): ('tiles', '160 compressed tiles at 0x80078000: the Zero 2 and Capcom logos of the last screen'),
    ('END', 0, 0x2e, 0): ('TIM 8-bit', 'a 384 x 240, 256-colour illustration at 0x80080000 (the same four in every ending file)'),
    ('END', 0, 0x2f, 0): ('TIM 8-bit', 'illustration at 0x800a0000'),
    ('END', 0, 0x30, 0): ('TIM 8-bit', 'illustration at 0x800c0000'),
    ('END', 0, 0x31, 0): ('TIM 8-bit', 'illustration at 0x800e0000'),
    ('BOSS', 2, 0, 0): ('portrait page', 'portrait page of the two speakers (256 x 96) with 36 palette rows below, to (704,0)'),
    ('BOSS', 2, 0, 1): ('text page', 'the lines of the talk (Japanese text, 16 x 16 glyph cells), to (768,0)'),
    ('BOSS', 0, 0xffff, 0): ('zeros', '16,384 bytes the loader reads and discards'),
    ('BOSS', 0, 0x2a, 0): ('module', 'boss_demo module at 0x801e0000 (the same in BOSS, GDEMO and RDM)'),
    ('RDM', 2, 0, 0): ('portrait page', 'portrait page of the two speakers with palette rows, to (704,0)'),
    ('RDM', 2, 0, 1): ('text page', 'the lines of the talk, to (768,0)'),
    ('RDM', 0, 0xffff, 0): ('zeros', '16,384 bytes the loader reads and discards'),
    ('RDM', 0, 0x2a, 0): ('module', 'the shared boss_demo module'),
    ('GDEMO', 2, 0, 0): ('portrait page', 'portrait page of the two speakers with palette rows, to (704,0)'),
    ('GDEMO', 2, 0, 1): ('text page', 'the lines of the talk (the same in all 21 files), to (768,0)'),
    ('GDEMO', 0, 0xffff, 0): ('zeros', '16,384 bytes the loader reads and discards'),
    ('GDEMO', 0, 0x2a, 0): ('module', 'the shared boss_demo module'),
    ('CDEMO', 2, 0, 0): ('portrait page', 'the fighter large portrait with 18 palette rows (160 to 177), to (768,0)'),
    ('CDEMO', 0, 0x10, 0): ('sprite blob', "the fighter's sprites at 0x80059000: five tile segments, one 256 x 256 page of the PS1's own (a stripe of 16 cells), 73 palettes, reserved zeros"),
    ('CDEMO', 0, 1, 0): ('module', 'character_demo module at 0x80010000'),
    ('NDEMO', 0, 0x11, 0): ('name plate', 'cell map (0x2000 bytes), 32 palettes, one page, at 0x80093000 (init_primitive_table_and_load_images)'),
}
# chunks that are not self-describing, in the loader's own layout: (kind, table, slot) -> [(region, start, end or None)]
LAYOUTS = {('NDEMO', 0, 0x11): [('map', 0, 0x2000), ('pal', 0x2000, 0x2400), ('image', 0x2400, None)]}
RAW_TILES = {('ASCII', 0, 1)}      # chunks that are a plain list of 128-byte tiles

# ---------------------------------------------------------------- pure functions (tested without game data)


def kind_of(name):
    """The file kind of an archive name ('CONT0AX.PAC' -> 'CONT', 'DEMO.PAC' -> 'DEMO', 'SELECTA.PAC' -> 'SELECTA')."""
    stem = name.upper().removesuffix('.PAC')
    return re.sub(r'[0-9A-F]{2}X?$', '', stem) or stem


def tim_header(d):
    """The TIM at the start of d as a dict (bpp, clut rectangle and rows, image rectangle in halfwords, offsets), or None.

    The loader (load_image_from_stream) sends the image rectangle's w * h halfwords from the data after the image block's header; the
    block size field is not used, and in the select screen's TIM the rectangle is larger than the block."""
    if len(d) < 20 or struct.unpack_from('<I', d, 0)[0] != 0x10:
        return None
    (flags,) = struct.unpack_from('<I', d, 4)
    if flags & ~0xb or (flags & 3) == 3:
        return None
    pos, clut = 8, None
    if flags & 8:
        if pos + 12 > len(d):
            return None
        size, x, y, w, h = struct.unpack_from('<IHHHH', d, pos)
        if size < 12 or pos + 12 + 2 * w * h > len(d):
            return None
        clut = dict(x=x, y=y, w=w, h=h, at=pos + 12)
        pos += size
    if pos + 12 > len(d):
        return None
    size, x, y, w, h = struct.unpack_from('<IHHHH', d, pos)
    if size < 12 or w == 0 or h == 0:
        return None
    return dict(bpp=(4, 8, 16, 24)[flags & 3], clut=clut, x=x, y=y, w=w, h=h, at=pos + 12, size=size, block_end=pos + size)


def flag_words(chunk, p, count):
    """(flag bits as one integer, offset after them) of a tile block's `count` items: ceil(count / 16) words."""
    nw = (count - 1 >> 4) + 1 if count else 0
    return int.from_bytes(chunk[p:p + 2 * nw], 'little'), p + 2 * nw


def tile_block_end(chunk, offset):
    """Offset after the compressed tile block at `offset` (the four forms of tilepack.decode_block)."""
    (n,) = struct.unpack_from('<H', chunk, offset)
    p = offset + 2
    if n == 0:
        return p + 128
    if n >> 8 == 0:
        _flags, p = flag_words(chunk, p, n)
        return p + 2 * n
    if n >> 8 == 0x80:
        flags = int.from_bytes(chunk[p:p + 8], 'little')
        return p + 8 + 2 * bin(flags).count('1')
    k = n & 0xff
    flags, p = flag_words(chunk, p, k)
    for i in range(k):
        p += 2 if flags >> i & 1 else 1
    return p


def tile_segments(d):
    """[(start, tiles, end)] of the compressed tile segments anywhere in d, found at 4-aligned offsets and not overlapping.

    A segment is an offset table (u32 offsets in bytes from the segment's start, the first one the table's own size, increasing),
    then the blocks; every block must decode to 64 words, and `end` is the 4-aligned offset after the last block."""
    out, p, n = [], 0, len(d)
    while p + 16 <= n:
        (first,) = struct.unpack_from('<I', d, p)
        if first % 4 == 0 and 4 * MIN_SEGMENT_TILES <= first <= MAX_TABLE and p + first <= n:
            offs = struct.unpack_from('<%dI' % (first // 4), d, p)
            if offs[0] == first and all(offs[i] < offs[i + 1] for i in range(len(offs) - 1)) and p + offs[-1] < n:
                try:
                    for o in offs:
                        tilepack.decode_block(d, p + o)
                    e = min((tile_block_end(d, p + offs[-1]) + 3) & ~3, n)
                except (ValueError, struct.error):
                    e = None
                if e is not None:
                    out.append((p, first // 4, e))
                    p = e
                    continue
        p += 4
    return out


def segment_tiles(d, segs):
    """The 128-byte tiles (little-endian words as the PS1 stores them) of the segments, in order."""
    out = []
    for s, n, _e in segs:
        for o in struct.unpack_from('<%dI' % n, d, s):
            _form, words = tilepack.decode_block(d, s + o)
            out.append(struct.pack('<64H', *words))
    return out


def palette_row_kind(row):
    """'p' for a 32-byte row that looks like a PS1 palette (colour 0 is 0, at least 11 of the other 15 are not), 'z' for an empty
    row, None otherwise."""
    if len(row) < 32:
        return None
    w = struct.unpack('<16H', row[:32])
    if not any(w):
        return 'z'
    return 'p' if w[0] == 0 and sum(1 for x in w[1:] if x) >= 11 else None


def palette_run_end(d, q, b):
    """End of the palette run that starts at q (empty rows may lie inside; at most 3 in a row), or q when none starts there."""
    r, good, empties = q, 0, 0
    last = q
    while r + 32 <= b:
        k = palette_row_kind(d[r:r + 32])
        if k == 'p':
            good += 1
            empties = 0
            last = r + 32
        elif k == 'z' and empties < 3:
            empties += 1
        else:
            break
        r += 32
    return last if good >= MIN_PALETTE_RUN and palette_row_kind(d[q:q + 32]) == 'p' else q


def chunk_regions(d):
    """[(kind, start, end)] regions of a chunk: 'tiles' (compressed segments), 'pal' (palette runs), 'zero' (runs of zero bytes) and
    'image' (the rest: pictures and what is not recognised, with the zero runs between them left in)."""
    segs = tile_segments(d)
    out, p = [], 0

    def gap(a, b):
        q = a
        rest = None
        while q < b:
            e = palette_run_end(d, q, b)
            if e > q:
                if rest is not None:
                    out.append(('image', rest, q))
                    rest = None
                out.append(('pal', q, e))
                q = e
                continue
            if rest is None:
                r = q
                while r < b and d[r] == 0:
                    r += 1
                if r - q >= MIN_ZERO_RUN:
                    out.append(('zero', q, r))
                    q = r
                    continue
                rest = q
            q += 4 if q + 4 <= b else b - q
        if rest is not None:
            out.append(('image', rest, b))

    for s, _n, e in segs:
        if s > p:
            gap(p, s)
        out.append(('tiles', s, e))
        p = e
    if p < len(d):
        gap(p, len(d))
    # an 'image' region that is only zeros is a zero run; neighbouring image and zero regions between two structured regions are one picture
    merged = []
    for kind, a, b in out:
        if kind == 'image' and not any(d[a:b]):
            kind = 'zero'
        if merged and merged[-1][0] == kind == 'zero' and merged[-1][2] == a:
            merged[-1] = (kind, merged[-1][1], b)
        else:
            merged.append((kind, a, b))
    return merged


def merge_pictures(regions):
    """Zero and 'image' regions that follow each other with no tiles or palettes between become one 'image' region (a picture with
    blank parts); a pure zero run stays a zero run."""
    out = []
    for kind, a, b in regions:
        if out and kind in ('image', 'zero') and out[-1][0] in ('image', 'zero') and out[-1][2] == a and 'image' in (kind, out[-1][0]):
            out[-1] = ('image', out[-1][1], b)
        else:
            out.append((kind, a, b))
    return out


def cells_of(page, width=256):
    """[(cell x, cell y, 128 bytes)] of the 16 x 16 cells of a 4 bits a pixel picture `width` pixels wide (a cell is 16 rows of 8 bytes,
    low nibble first, as a tile is). A partial last cell row is dropped."""
    row = width // 2
    rows = len(page) // row
    out = []
    for cy in range(rows // 16):
        for cx in range(width // 16):
            out.append((cx, cy, b''.join(page[(cy * 16 + y) * row + cx * 8:(cy * 16 + y) * row + cx * 8 + 8] for y in range(16))))
    return out


def pieces_of(cell):
    """The four 8 x 8 pieces (32 bytes each, 8 rows of 4 bytes) of a 128-byte cell: top left, top right, bottom left, bottom right."""
    return [b''.join(cell[(y + (q >> 1) * 8) * 8 + (q & 1) * 4:(y + (q >> 1) * 8) * 8 + (q & 1) * 4 + 4] for y in range(8)) for q in range(4)]


def is_flat(piece):
    """True when every pixel of a packed piece has the same value (blank included)."""
    return len(set(piece)) == 1 and piece[0] >> 4 == piece[0] & 15


def classify_cell(cell, find16, find8):
    """'blank', 'tile' (an arcade tile), 'pieces' (not a tile, but every non-flat 8 x 8 piece is an arcade piece), 'part' (some are) or
    'own'. find16 and find8 return a number (0 or more) for a hit and a negative number for a miss."""
    if cell == bytes(128):
        return 'blank'
    if find16(cell) >= 0:
        return 'tile'
    hits = [is_flat(p) or find8(p) >= 0 for p in pieces_of(cell)]
    nonflat = [not is_flat(p) for p in pieces_of(cell)]
    if all(hits):
        return 'pieces' if any(nonflat) else 'tile'
    return 'part' if any(h and nf for h, nf in zip(hits, nonflat)) else 'own'


def palette_rows(page, width=256, min_run=MIN_PALETTE_ROWS):
    """{y} of the rows of a picture that hold palettes: rows whose 32-byte groups are all palettes or empty (one at least a palette),
    when at least min_run such rows follow each other. Only the pictures of table 2 (the fighters' portrait pages and the talk pages)
    carry palette rows; in other pictures such rows are chance."""
    row = width // 2
    ys, run = set(), []
    for y in range(len(page) // row + 1):
        ks = [palette_row_kind(page[y * row + 32 * i:y * row + 32 * i + 32]) for i in range(row // 32)] if (y + 1) * row <= len(page) else [None]
        if 'p' in ks and all(k is not None for k in ks):
            run.append(y)
        else:
            if len(run) >= min_run:
                ys.update(run)
            run = []
    return ys


def page_stats(page, find16, find8, width=256, with_palettes=False):
    """Counter of a picture's cells: cells, blank, pal (cells that are palette rows), tile, pieces, part, own."""
    c = collections.Counter()
    pal = palette_rows(page, width) if with_palettes else set()
    for cx, cy, cell in cells_of(page, width):
        c['cells'] += 1
        ys = range(cy * 16, cy * 16 + 16)
        if any(y in pal for y in ys) and all(y in pal or not any(cell[(y - cy * 16) * 8:(y - cy * 16) * 8 + 8]) for y in ys):   # palette rows and blank rows only
            c['pal'] += 1
            continue
        c[classify_cell(cell, find16, find8)] += 1
    return c


def normalise_palettes(chunk):
    """The chunk's palettes (32 bytes each) with bit 15 cleared on colours 1 to 15 (semi-transparency; a 0x8000 black becomes 0)."""
    n = len(chunk) // 32
    out = bytearray(chunk[:n * 32])
    for k in range(n):
        for i in range(1, 16):
            out[k * 32 + 2 * i + 1] &= 0x7f
    return bytes(out)


def colours_of(row):
    """Colours 1..15 of a 32-byte palette."""
    return list(struct.unpack('<16H', row)[1:])


def palette_kind(row):
    """'flat' (colours 1..15 all equal), 'ramp' (a filler ramp, fighter_parts.is_ramp) or 'own'."""
    cs = colours_of(row)
    if len(set(cs)) == 1:
        return 'flat'
    return 'ramp' if fighter_parts.is_ramp(cs) is not None else 'own'


def xa_summary(subheaders):
    """{'sectors', 'null', 'audio': {channel: sectors}, 'coding': set} of the sectors' subheaders [(file, channel, submode, coding)].
    Audio sectors have submode bit 2 (0x04) set; a coding byte's bit 0 is stereo, bit 2 the 18.9 kHz rate, bit 4 8 bits."""
    out = dict(sectors=0, null=0, audio=collections.Counter(), coding=set())
    for _f, ch, sub, coding in subheaders:
        out['sectors'] += 1
        if sub & 4:
            out['audio'][ch] += 1
            out['coding'].add(coding)
        else:
            out['null'] += 1
    return out


def xa_coding_text(coding):
    """Text of an XA coding byte: 'stereo 37.8 kHz 4 bit'."""
    return '%s %s kHz %d bit' % ('stereo' if coding & 1 else 'mono', '18.9' if coding & 4 else '37.8', 8 if coding & 16 else 4)


def str_video_header(sector):
    """(frame, chunk, chunks, width, height) of a movie video sector's 32-byte header, or None. The data starts with the magic
    0x80010160, then the chunk number and the chunks in the frame (16 bits each), the frame number (32 bits) and the size of the frame's
    data (32 bits); the width and height (16 bits each) follow at offsets 16 and 18."""
    if len(sector) < 32 or struct.unpack_from('<I', sector, 0)[0] != 0x80010160:
        return None
    chunk, chunks, frame = struct.unpack_from('<HHI', sector, 4)
    return frame, chunk, chunks, struct.unpack_from('<H', sector, 16)[0], struct.unpack_from('<H', sector, 18)[0]


def exe_header(d):
    """Dict (entry, load address, size, stack) of a PS-X EXE header, or None."""
    if d[:8] != b'PS-X EXE' or len(d) < 0x800:
        return None
    entry, gp, load, size = struct.unpack_from('<4I', d, 0x10)
    return dict(entry=entry, load=load, size=size, sp=struct.unpack_from('<I', d, 0x30)[0])


def is_module(d):
    """True when a chunk holds code: at least eight `jr ra` words."""
    return len(d) >= 32 and sum(1 for (w,) in struct.iter_unpack('<I', d[:len(d) // 4 * 4]) if w == JR_RA) >= 8


def cluster_ranges(values, gap):
    """[(low, high, count)] of the sorted distinct values grouped where neighbours are at most `gap` apart (counts distinct values)."""
    out = []
    for v in sorted(set(values)):
        if out and v - out[-1][1] <= gap:
            out[-1] = (out[-1][0], v, out[-1][2] + 1)
        else:
            out.append((v, v, 1))
    return out


def ranges_text(label, values, gap, limit=8):
    """'label 0x10-0x40 (5), ...' from cluster_ranges, the largest `limit` clusters in address order; '' when there are no values."""
    cl = cluster_ranges(values, gap)
    big = sorted(sorted(cl, key=lambda r: -r[2])[:limit])
    if not big:
        return ''
    return '%s in %d groups: %s%s' % (label, len(cl), ', '.join('%#x-%#x (%d)' % r for r in big), ' ...' if len(cl) > limit else '')


def offset_table(d, max_entries=1024):
    """The offsets of a file that starts with a table of u32 offsets (the first the table's own size, all inside the file), or None.
    They need not increase: CAPCOM.BIN's table lists its 32 blocks in two interleaved runs."""
    if len(d) < 16:
        return None
    (first,) = struct.unpack_from('<I', d, 0)
    if first % 4 or not 16 <= first <= 4 * max_entries or first > len(d):
        return None
    offs = struct.unpack_from('<%dI' % (first // 4), d, 0)
    return offs if all(o <= len(d) for o in offs) and len(set(offs)) == len(offs) else None


def table_block_sizes(offs, size):
    """Sizes of the blocks that an offset table lists, each running to the next offset in file order (the last to the end)."""
    order = sorted(offs) + [size]
    nxt = {o: order[i + 1] for i, o in enumerate(order[:-1])}
    return [nxt[o] - o for o in offs]


def share(a, b):
    """'a/b' with a percentage; '-' when b is 0."""
    return '-' if not b else '%d/%d (%d%%)' % (a, b, round(100 * a / b))


def count_text(c):
    """One line of the counters that are not zero, in a fixed order."""
    order = ('segments', 'tiles', 'blank', 'tile_arcade', 'tile_pieces', 'tile_part', 'tile_own', 'pal_n', 'pal_arcade', 'pal_ramp', 'pal_flat',
             'pal_own', 'cells', 'c_tile', 'c_pieces', 'c_part', 'c_own', 'c_pal', 'zero_bytes', 'tim8_bytes', 'map_cells',
             'runs', 'run_bytes', 'chains', 'unknown_bytes', 'tim8_colours', 'programs', 'tones', 'samples', 'sample_bytes')
    return ' '.join('%s=%d' % (k, c[k]) for k in order if c.get(k))


# ---------------------------------------------------------------- inputs

C_SOURCE = r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static uint8_t *ps1t; static int ncodes; static int64_t *tab16, *tab8; static uint32_t sz16, sz8;
static uint32_t spread[256];
static uint64_t fnv(const uint8_t *p, int n) { uint64_t h = 1469598103934665603ULL; for (int i = 0; i < n; i++) { h ^= p[i]; h *= 1099511628211ULL; } return h; }
static void flip(const uint8_t *in, uint8_t *out, int f, int side) {
  uint8_t px[256], o[256]; int bytes = side * side / 2;
  for (int i = 0; i < bytes; i++) { px[2 * i] = in[i] & 15; px[2 * i + 1] = in[i] >> 4; }
  for (int y = 0; y < side; y++) for (int x = 0; x < side; x++) o[y * side + x] = px[((f & 2) ? side - 1 - y : y) * side + ((f & 1) ? side - 1 - x : x)];
  for (int k = 0; k < bytes; k++) out[k] = o[2 * k] | o[2 * k + 1] << 4; }
static void quad(const uint8_t *t, int q, uint8_t *o) { for (int y = 0; y < 8; y++) memcpy(o + y * 4, t + (y + ((q >> 1) & 1) * 8) * 8 + (q & 1) * 4, 4); }
static void get16(int64_t v, uint8_t *u) { int f = v & 3; long c = v >> 2; if (f) flip(ps1t + c * 128, u, f, 16); else memcpy(u, ps1t + c * 128, 128); }
static void get8(int64_t v, uint8_t *u) { int f = v & 3, q = (v >> 2) & 3; long c = v >> 4; uint8_t b[32]; quad(ps1t + c * 128, q, b); if (f) flip(b, u, f, 8); else memcpy(u, b, 32); }
/* the arcade's tiles as the PS1 stores them (colour index plus one, low nibble first), and an index of every tile and 8 x 8 piece with its flips */
int build(const uint8_t *gfx, int n) {
  for (int b = 0; b < 256; b++) { uint32_t v = 0; for (int i = 0; i < 8; i++) v |= ((b >> (7 - i)) & 1u) << (4 * i); spread[b] = v; }
  ncodes = n; ps1t = malloc((size_t)n * 128);
  for (int c = 0; c < n; c++) { const uint8_t *q = gfx + (size_t)c * 128; uint8_t *o = ps1t + (size_t)c * 128;
    for (int y = 0; y < 16; y++) for (int h = 0; h < 2; h++) { const uint8_t *b = q + y * 8 + h * 4;
      uint32_t v = spread[b[0]] | spread[b[1]] << 1 | spread[b[2]] << 2 | spread[b[3]] << 3;
      v = ((v & 0x77777777u) + 0x11111111u) ^ ((v ^ 0x11111111u) & 0x88888888u); memcpy(o + y * 8 + h * 4, &v, 4); } }
  sz16 = 1u << 22; sz8 = 1u << 24; tab16 = malloc((size_t)sz16 * 8); tab8 = malloc((size_t)sz8 * 8);
  memset(tab16, 0xff, (size_t)sz16 * 8); memset(tab8, 0xff, (size_t)sz8 * 8);
  for (int c = n - 1; c >= 0; c--) {
    uint8_t t[128], u[128];
    for (int f = 0; f < 4; f++) { if (f) flip(ps1t + (size_t)c * 128, t, f, 16); else memcpy(t, ps1t + (size_t)c * 128, 128);
      uint32_t i = fnv(t, 128) & (sz16 - 1);
      while (tab16[i] != -1) { get16(tab16[i], u); if (!memcmp(u, t, 128)) break; i = (i + 1) & (sz16 - 1); }
      tab16[i] = (int64_t)c * 4 + f; }
    for (int q = 3; q >= 0; q--) { uint8_t b[32], p[32], w[32]; quad(ps1t + (size_t)c * 128, q, b);
      for (int f = 0; f < 4; f++) { if (f) flip(b, p, f, 8); else memcpy(p, b, 32);
        uint32_t i = fnv(p, 32) & (sz8 - 1);
        while (tab8[i] != -1) { get8(tab8[i], w); if (!memcmp(w, p, 32)) break; i = (i + 1) & (sz8 - 1); }
        tab8[i] = ((int64_t)c * 4 + q) * 4 + f; } } }
  return 0; }
int64_t find16(const uint8_t *q) { uint32_t i = fnv(q, 128) & (sz16 - 1); uint8_t u[128];
  while (tab16[i] != -1) { get16(tab16[i], u); if (!memcmp(u, q, 128)) return tab16[i]; i = (i + 1) & (sz16 - 1); } return -1; }
int64_t find8(const uint8_t *q) { uint32_t i = fnv(q, 32) & (sz8 - 1); uint8_t u[32];
  while (tab8[i] != -1) { get8(tab8[i], u); if (!memcmp(u, q, 32)) return tab8[i]; i = (i + 1) & (sz8 - 1); } return -1; }
'''


class TileIndex:
    """Every arcade 16 x 16 tile and 8 x 8 piece, flips included, as the PS1 stores them (colour index plus one): find16 and find8
    return 4 * code + flip (16 x 16) or the piece's number, or -1."""

    def __init__(self, tmp, gfx):
        src = Path(tmp) / 'index.c'
        src.write_text(C_SOURCE)
        lib = Path(tmp) / 'index.so'
        subprocess.run(['cc', '-O2', '-shared', '-fPIC', '-o', str(lib), str(src)], check=True, capture_output=True, timeout=60)
        self.lib = ctypes.CDLL(str(lib))
        self.lib.build.argtypes = [ctypes.c_char_p, ctypes.c_int]
        for f in (self.lib.find16, self.lib.find8):
            f.restype = ctypes.c_int64
            f.argtypes = [ctypes.c_char_p]
        self.gfx = gfx
        self.lib.build(gfx, len(gfx) // 128)

    def find16(self, cell):
        return self.lib.find16(cell)

    def find8(self, piece):
        return self.lib.find8(piece)


class Context:
    """The arcade views, the tile index and the palette search, built once."""

    def __init__(self, arcade_zip, tmp):
        self.prog, self.gfx = stages.arcade_views(arcade_zip)
        self.index = TileIndex(tmp, self.gfx)
        self.helper = stages.Helper(tmp, self.gfx, self.prog)
        self.stats = program_map.UnitStats(self.prog)
        self.palette_cache = {}
        self.palette_at = []     # the arcade addresses of the palettes found since take_sources
        self.tile_codes = []     # the arcade tile codes of the whole tiles found since take_sources

    def take_sources(self):
        """(palette addresses, tile codes) found since the last call."""
        out = (self.palette_at, self.tile_codes)
        self.palette_at, self.tile_codes = [], []
        return out

    def find16(self, cell):
        r = self.index.find16(cell)
        if r >= 0:
            self.tile_codes.append(r >> 2)
        return r

    def palettes(self, chunk):
        """Counters of the palettes in a chunk (rows of 32 bytes, as the loader stores them) that are not empty: pal_n, and how many
        are the arcade's (found in the arcade program), filler ramps, flat, or the PS1's own."""
        chunk = normalise_palettes(chunk)
        n = len(chunk) // 32
        real = [k for k in range(n) if any(chunk[32 * k:32 * k + 32])]
        c = collections.Counter(pal_n=len(real))
        rows = sorted({chunk[32 * k:32 * k + 32] for k in real})
        todo = [r for r in rows if r not in self.palette_cache]
        if todo:
            _n, at = self.helper.palettes(b''.join(todo), ARCADE_BRIGHTNESS)
            for r, a in zip(todo, at):
                self.palette_cache[r] = a
        for k in real:
            row = chunk[32 * k:32 * k + 32]
            if self.palette_cache[row] is not None:
                c['pal_arcade'] += 1
                self.palette_at.append(self.palette_cache[row])
            else:
                c['pal_' + palette_kind(row)] += 1
        return c

    def tile_stats(self, tiles):
        """Counters of a list of 128-byte tiles: tiles, blank, and the others by class: tile_arcade (an arcade tile, flips allowed),
        tile_pieces (not one, but every non-flat 8 x 8 piece is an arcade piece), tile_part (some are), tile_own."""
        c = collections.Counter(tiles=len(tiles))
        for t in tiles:
            k = classify_cell(t, self.find16, self.index.find8)
            c['blank' if k == 'blank' else 'tile_arcade' if k == 'tile' else 'tile_' + k] += 1
        return c

    def picture(self, page, width=256, with_palettes=False):
        """Counters of a picture: its cells by class (cells counts the non-blank cells that are not palette rows) and the palettes
        that its palette rows hold."""
        c = page_stats(page, self.find16, self.index.find8, width, with_palettes)
        out = collections.Counter(cells=c['cells'] - c['blank'] - c['pal'], c_tile=c['tile'], c_pieces=c['pieces'], c_part=c['part'],
                                  c_own=c['own'], c_pal=c['pal'])
        row = width // 2
        pals = [page[y * row + 32 * i:y * row + 32 * i + 32] for y in sorted(palette_rows(page, width) if with_palettes else ()) for i in range(row // 32)]
        pals = [p for p in pals if palette_row_kind(p) == 'p']
        if pals:
            out.update(self.palettes(b''.join(pals)))
        return out

    def module(self, d, with_runs=True):
        """Counters for a module chunk: arcade-equal runs (>= 32 bytes) and cell list chains."""
        c = collections.Counter()
        if with_runs:
            anchors = program_map.seed_anchors(d, self.prog)
            segs = program_map.find_segments(d, self.prog, self.stats, anchors)
            kept = program_map.select_segments_data(d, self.prog, segs, bytearray(len(d)), self.stats, min_bits=8.0, min_units=MIN_EQUAL_RUN // 2)
            c['runs'] = len(kept)
            c['run_bytes'] = sum(e - s for s, e, *_ in kept)
        c['chains'] = len(program_map.find_cell_chains(d, bytearray(b'\x01' * len(d)), 20, 400))
        return c


# ---------------------------------------------------------------- the analysis of one chunk

def what_of(kind, table, slot, ordinal, d):
    """(name, text) of a chunk position: the table above, else by table (sound chunks) or content (zeros)."""
    w = WHAT.get((kind, table, slot, ordinal))
    if w:
        return w
    if table == 3:
        return ('VAB header', 'sound bank header') if d[:4] == b'pBAV' else ('sound commands', 'command list of the sound player (8 bytes a command)')
    if table == 4:
        return ('VAB body', 'sound bank samples, SPU ADPCM')
    if table == 5:
        return ('sequence', 'pQES sequence')
    if table == 0 and slot == 0xffff and not any(d):
        return ('zeros', '16,384 bytes the loader reads and discards (no table entry)')
    return ('?', 'not read yet')


def analyse_regions(d, ctx, key, detail=False):
    """Counters and detail notes of a chunk by its regions."""
    c = collections.Counter()
    notes = []
    layout = LAYOUTS.get(key)
    regions = [(k, a, len(d) if b is None else b) for k, a, b in layout] if layout else merge_pictures(chunk_regions(d))
    for kind, a, b in regions:
        part = d[a:b]
        r = collections.Counter()
        if kind == 'tiles':
            segs = tile_segments(part)
            r.update(ctx.tile_stats(segment_tiles(part, segs)))
            r['segments'] = len(segs)
        elif kind == 'pal':
            r.update(ctx.palettes(part))
        elif kind == 'zero':
            r['zero_bytes'] += b - a
        elif kind == 'map':
            r['map_cells'] += len(part) // 4
        elif kind == 'image':
            if len(part) % (PAGE_BYTES // 2) == 0:
                for i in range(0, len(part), PAGE_BYTES):
                    r.update(ctx.picture(part[i:i + PAGE_BYTES]))
            else:
                r['unknown_bytes'] += len(part)
        c.update(r)
        notes.append('%s %#x+%#x %s' % (kind, a, b - a, count_text(r)))
    return c, notes


def analyse_chunk(d, table, slot, key, ctx, with_modules=True, name=''):
    """(counters, notes) for one chunk's content. `name` is the chunk's label from WHAT, which says how to read it when the content
    does not."""
    c = collections.Counter()
    if table in (3, 4, 5):
        return c, []
    if table == 1:
        return ctx.palettes(d), []
    if table == 2 or name in ('picture', 'pictures'):
        for i in range(0, len(d), PAGE_BYTES):
            c.update(ctx.picture(d[i:i + PAGE_BYTES], 256, name == 'portrait page'))
        return c, []
    if not any(d):
        c['zero_bytes'] = len(d)
        return c, []
    if name == 'palettes':
        return ctx.palettes(d), []
    if name == 'cell map':
        c['map_cells'] = len(d) // 4
        return c, []
    if key in RAW_TILES:
        return ctx.tile_stats([d[i:i + 128] for i in range(0, len(d) // 128 * 128, 128)]), []
    if is_module(d):
        return ctx.module(d, with_modules), []
    tim = tim_header(d)
    if tim:
        return analyse_tim(d, tim, ctx)
    return analyse_regions(d, ctx, key)


def analyse_tim(d, tim, ctx):
    """Counters of a TIM: its palettes, and its picture when 4 bits a pixel (the loader reads w * h halfwords from the data after the
    image header, past the block's end if the rectangle is larger)."""
    c = collections.Counter()
    notes = ['TIM %d bits, image %dx%d halfwords at (%d,%d), block %#x, rectangle %#x' % (tim['bpp'], tim['w'], tim['h'], tim['x'], tim['y'], tim['size'], 2 * tim['w'] * tim['h'])]
    if tim['clut'] and tim['bpp'] == 4:
        k = tim['clut']
        c.update(ctx.palettes(d[k['at']:k['at'] + 2 * k['w'] * k['h']]))
    elif tim['clut']:
        c['tim8_colours'] = tim['clut']['w'] * tim['clut']['h']
    size = 2 * tim['w'] * tim['h']
    img = d[tim['at']:tim['at'] + size]
    if tim['bpp'] == 4 and tim['w'] % 4 == 0 and len(img) == size:
        c.update(ctx.picture(img, tim['w'] * 4))         # the rectangle is w halfwords of 4 pixels, rows of w * 2 bytes
    else:
        c['tim8_bytes'] += len(img)
    return c, notes


# ---------------------------------------------------------------- reports

def archive_chunks(data):
    return [dict(c, data=data[c['offset']:c['offset'] + c['size']]) for c in pac.chunk_table(data)]


def bank_counts(header, body):
    """Counters of a sound bank: programs, tones and samples of a header, and the bytes of the body's samples."""
    c = collections.Counter()
    try:
        info = samples.parse_vh(header)
    except samples.FormatError:
        return c
    c.update(programs=info['programs'], tones=len(info['tones']), samples=sum(1 for z in info['sizes'] if z))
    if body is not None:
        c['sample_bytes'] = sum(info['sizes'])
    return c


def ratio_text(per):
    """Ranges over the distinct contents of the arcade share of tiles, palettes and cells, as 'tiles 85-92%' (one value when equal)."""
    out = []
    for label, num, den in (('tiles', ('tile_arcade',), ('tiles',)), ('tiles with pieces', ('tile_arcade', 'tile_pieces'), ('tiles',)),
                            ('palettes', ('pal_arcade',), ('pal_n',)), ('cells', ('c_tile', 'c_pieces'), ('cells',))):
        vals = []
        for c in per:
            n = sum(c.get(k, 0) for k in num)
            m = sum(c.get(k, 0) - (c.get('blank', 0) if k == 'tiles' else 0) for k in den)
            if m > 0:
                vals.append(100 * n / m)
        if vals:
            lo, hi = round(min(vals)), round(max(vals))
            out.append('%s %s%%' % (label, lo if lo == hi else '%d-%d' % (lo, hi)))
    return ', '.join(out)


def pac_report(files, sec, ctx, only, with_modules, detail):
    groups = collections.OrderedDict((k, []) for k in KIND_ORDER)
    for path in sorted(files):
        if not path.upper().endswith('.PAC'):
            continue
        name = path.rsplit('/', 1)[1]
        k = kind_of(name)
        if k in groups and (not only or only.upper() == k):
            groups[k].append((name, path))
    for k, members in groups.items():
        if not members:
            continue
        positions = collections.OrderedDict()   # (table, slot, ordinal) -> {sha: [files, data, bank counters]}
        for name, path in members:
            lba, size = files[path]
            data = sec(lba, (size + SECTOR - 1) // SECTOR)[:size]
            seen = collections.Counter()
            chunks = archive_chunks(data)
            headers = {c['slot']: c['data'] for c in chunks if c['table'] == 3 and c['data'][:4] == b'pBAV'}
            bodies = {c['slot']: c['data'] for c in chunks if c['table'] == 4}
            for ch in chunks:
                ordinal = seen[(ch['table'], ch['slot'])]
                seen[(ch['table'], ch['slot'])] += 1
                entry = positions.setdefault((ch['table'], ch['slot'], ordinal), collections.OrderedDict())
                h = hashlib.sha256(ch['data']).digest()
                if h not in entry:
                    bank = bank_counts(headers[ch['slot']], bodies.get(ch['slot'])) if ch['table'] in (3, 4) and ch['slot'] in headers else collections.Counter()
                    if ch['table'] == 3:
                        bank.pop('sample_bytes', None)
                    if ch['table'] == 4:
                        bank = collections.Counter(samples=bank['samples'], sample_bytes=bank['sample_bytes'])
                    entry[h] = [0, ch['data'], bank]
                entry[h][0] += 1
        print('== %s: %d files' % (k, len(members)))
        for (table, slot, ordinal), contents in positions.items():
            per, notes = [], []
            ctx.take_sources()
            for h, (n, d, bank) in contents.items():
                nm = what_of(k, table, slot, ordinal, d)[0]
                cc, nn = analyse_chunk(d, table, slot, (k, table, slot), ctx, with_modules, nm)
                cc.update(bank)
                per.append(cc)
                notes.append(nn)
            pal_at, codes = ctx.take_sources()
            total = collections.Counter()
            for cc in per:
                total.update(cc)
            sizes = sorted({len(d) for n, d, _b in contents.values()})
            first = next(iter(contents.values()))[1]
            name, text = what_of(k, table, slot, ordinal, first)
            nfiles = sum(n for n, d, _b in contents.values())
            extra = ratio_text(per) if len(per) > 1 else ''
            print('  t%d s%#x #%d  %s B  %d file(s), %d distinct  %s  [%s]%s  %s' % (
                table, slot, ordinal, '/'.join('%d' % z for z in sizes[:3]) + ('+' if len(sizes) > 3 else ''), nfiles, len(contents), name,
                count_text(total), ('  per content: ' + extra) if extra else '', text))
            if detail:
                for line in notes[0]:
                    print('       ' + line)
                for text in (ranges_text('arcade palettes (68000 addresses)', pal_at, 0x40), ranges_text('arcade tile codes', codes, 0x100)):
                    if text:
                        print('       ' + text)


def list_report(files, sec, ctx, tmp, only):
    """For the kinds of LIST_PAIRS: the module's cell lists (fighters.analyse) against the arcade's art records, with the first
    compressed segment of the tile chunk as the tiles the lists number. Prints one line a file (CONT and DEMO: the first)."""
    view = fighters.View(ctx.prog)
    index = fighters.Index(view)
    helper = fighters.Helper(tmp, ctx.gfx)
    for kind, (mslot, tslot) in LIST_PAIRS.items():
        if only and only.upper() != kind:
            continue
        names = [p for p in sorted(files) if p.upper().endswith('.PAC') and kind_of(p.rsplit('/', 1)[1]) == kind]
        if kind in ('CONT', 'DEMO'):
            names = names[:1]
        tot = collections.Counter()
        for p in names:
            lba, size = files[p]
            chunks = archive_chunks(sec(lba, (size + SECTOR - 1) // SECTOR)[:size])
            mod = next(c['data'] for c in chunks if c['table'] == 0 and c['slot'] == mslot)
            tiles = next(c['data'] for c in chunks if c['table'] == 0 and c['slot'] == tslot)
            segs = tile_segments(tiles)
            r = fighters.analyse(mod, tiles[:segs[0][2]] if segs else tiles, view, index, helper)
            if 'error' in r:
                print('  %s: %s' % (p.rsplit('/', 1)[1], r['error']))
                continue
            n, ok = len(r['lists']), r['tiles_ok']
            tot.update(files=1, lists=n, matched=ok, equal=r['bytes_equal'])
            if len(names) <= 3:
                print('  %s: %d lists, %d with arcade-record tile runs (of %d), %d byte-equal, differences %s' % (
                    p.rsplit('/', 1)[1], n, ok, r['tiles_of'], r['bytes_equal'], dict(r['kinds'])))
        if len(names) > 3 and tot['files']:
            print('  %s: %d files, %d lists, %d with arcade-record tile runs, %d byte-equal' % (kind, tot['files'], tot['lists'], tot['matched'], tot['equal']))


def raw_sectors(f, lba, count, block=256):
    """The raw 2352-byte sectors lba .. lba + count - 1 of the image, read in blocks."""
    done = 0
    while done < count:
        n = min(block, count - done)
        f.seek((lba + done) * 2352)
        data = f.read(n * 2352)
        for i in range(len(data) // 2352):
            yield data[i * 2352:(i + 1) * 2352]
        done += n


def disc_files_report(files, sec, f, ctx=None, with_modules=True):
    """XA, STR and EXE files, and the other files at the disc's root."""
    exes = {}
    for path in sorted(files):
        lba, size = files[path]
        up = path.upper()
        if up.endswith('.EXE'):
            data = sec(lba, (size + SECTOR - 1) // SECTOR)[:size]
            h = exe_header(data)
            exes[path] = data
            line = '  %-22s %8d  PS-X EXE  entry %#x load %#x size %#x' % (path, size, h['entry'], h['load'], h['size'])
            if ctx is not None and with_modules and not path.upper().endswith(('A2O.EXE', 'BF3.EXE')):
                line += '  [%s]' % count_text(ctx.module(data[0x800:], True))
            print(line)
    base = exes.get('/PAC/Z2O.EXE')
    for path, data in exes.items():
        if base is not None and path != '/PAC/Z2O.EXE' and len(data) == len(base):
            print('  %-22s differs from Z2O.EXE in %d bytes' % (path, sum(1 for a, b in zip(base, data) if a != b)))
    for path in sorted(files):
        lba, size = files[path]
        up = path.upper()
        if up.endswith('.XA'):
            subs = [(s[16], s[17], s[18], s[19]) for s in raw_sectors(f, lba, size // SECTOR)]
            x = xa_summary(subs)
            per = sorted(x['audio'].items())
            secs = [n for _c, n in per]
            print('  %-22s %8d  XA  %d sectors, %d audio channels of %d to %d sectors (%s), %d null sectors; seconds at double speed %d to %d' % (
                path, size, x['sectors'], len(per), min(secs), max(secs), ', '.join(xa_coding_text(c) for c in sorted(x['coding'])), x['null'],
                min(secs) * 8 // 150, max(secs) * 8 // 150))
        elif up.endswith('.STR'):
            frames, res, audio = set(), None, 0
            for s in raw_sectors(f, lba, size // SECTOR):
                if s[18] & 4:
                    audio += 1
                v = str_video_header(s[24:24 + 32])
                if v:
                    frames.add(v[0])
                    res = (v[3], v[4])
            print('  %-22s %8d  STR  %d frames of %s, %d audio sectors, %d sectors (%d s at double speed)' % (path, size, len(frames), res, audio, size // SECTOR, size // SECTOR // 150))
        elif up.endswith('CAPCOM.BIN'):
            data = sec(lba, (size + SECTOR - 1) // SECTOR)[:size]
            offs = offset_table(data)
            sizes = table_block_sizes(offs, len(data)) if offs else []
            print('  %-22s %8d  %s' % (path, size, ('table of %d blocks of %d to %d bytes, not tile blocks (read by LOGO.EXE with CAPCOM.STR)' % (
                len(offs), min(sizes), max(sizes))) if offs else 'no offset table'))
        elif not up.endswith(('.PAC', '.EXE')) and up not in ('/ZNULL.DAT', '/SLPS_004.15'):
            print('  %-22s %8d  disc mastering information (title, date), not read by the game' % (path, size) if up.endswith('.INF') else '  %-22s %8d' % (path, size))
        elif up == '/ZNULL.DAT':
            data = sec(lba, 8)
            print('  %-22s %8d  padding (%s)' % (path, size, 'all zero in its first 16 KB' if not any(data) else 'not zero'))


def screens_main(args, files, sec, ctx, tmp, started):
    """--c-table and --compare-c: the recipes of the screens' chunks (screens.py), as the C table or compared with the C build."""
    import screens
    disc = {}
    for path in sorted(files):
        if not path.upper().endswith('.PAC') or (args.only and kind_of(path.rsplit('/', 1)[1]) != args.only.upper()):
            continue
        k = kind_of(path.rsplit('/', 1)[1])
        if k not in KIND_ORDER:
            continue
        lba, size = files[path]
        data = sec(lba, (size + SECTOR - 1) // SECTOR)[:size]
        seen = collections.Counter()
        chunks = []
        for c in pac.chunk_table(data):
            chunks.append((c['table'], c['slot'], seen[(c['table'], c['slot'])], data[c['offset']:c['offset'] + c['size']]))
            seen[(c['table'], c['slot'])] += 1
        disc[path.rsplit('/', 1)[1]] = (k, chunks)
    env = screens.Env(tmp, ctx.prog, ctx.gfx)
    table = screens.collect(disc, env, sys.modules[__name__])
    text = screens.c_table(table, 'python3 port/arcade/other_files.py --disc DISC --arcade SET --c-table')
    if args.c_table:
        sys.stdout.write(text)
        print('%d files, %d recipes in %.1f s' % (len(table.files), len(table.recipes), time.monotonic() - started), file=sys.stderr)
        return 0
    return screens.compare_report(table, text, disc, env, ctx, tmp, started)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file kind (ASCII, DEMO, SELECT, SELECTA, CONT, END, BOSS, RDM, GDEMO, CDEMO, NDEMO, FIRSTSND, GAME_SND)')
    ap.add_argument('--no-modules', action='store_true', help='skip the search of the modules for arcade-equal runs (it takes seconds a module)')
    ap.add_argument('--detail', action='store_true', help='print the regions of the first content of each chunk and the arcade addresses found')
    ap.add_argument('--lists', action='store_true', help="also compare the modules' cell lists with the arcade's art records (fighters.analyse; about 2 s a file)")
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/screens_table.c (the recipes the runtime C builds the screens from)')
    ap.add_argument('--compare-c', action='store_true', help='build screens.c with the checked-in table and compare it with the model and the disc, per file')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='other-files-')
    try:
        ctx = Context(args.arcade, tmp)
        with open(args.disc, 'rb') as f:
            files, sec = stages.iso_files(f)
            if args.c_table or args.compare_c:
                return screens_main(args, files, sec, ctx, tmp, started)
            pac_report(files, sec, ctx, args.only, not args.no_modules, args.detail)
            if args.lists:
                print('== cell lists of the modules against the arcade art records')
                list_report(files, sec, ctx, tmp, args.only)
            if not args.only:
                print('== disc files')
                disc_files_report(files, sec, f, ctx, not args.no_modules)
        print('done in %.1f s' % (time.monotonic() - started))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
