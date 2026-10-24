#!/usr/bin/env python3
"""Analysis tool and model: the name plates' cell maps (NDEMOnn.PAC, table 0 slot 0x11) built from the arcade's scroll 1, the specification of port/arcade/plates.c.

What it is. After a fight the win screen shows a name plate: the winner's stage in a thumbnail with a ribbon beside it. Its file holds a map of 64 x 32 cells (a
texture word and a palette word each, 8,192 bytes), 32 palettes and one 256 x 256 page of 8 x 8 units that screens.py already builds from the arcade. The map
is a window of the strips that the select screen's backdrop shows (screen_lists.py, BACKDROPS): 8 rows (3 to 10) of the arcade's scroll 1 block sets, the
columns 0 to 6 from set 0, 7 to 31 from set 64 + band and 32 to 63 from set 84, band being the stage's number. A cell's 8 x 8 piece is looked up among the page's
units (rows, then columns: the first unit that equals it names the texture word (22 << 10 | row << 5 | column)), the palette word is 0x7800 + 64 * the attribute's
palette (0x7800 for a blank piece). Every other cell is the constant cell: texture 0x5800 (the blank unit at the page's
top left), palette 0x7800.

The rule was found on the disc: band k is the file's kind for the kinds 0 to 0x13 (kind 0x14 shows band 2), and on every file 490 to 512 of the strip's 512 cells are the
disc's; the others are pieces the page lacks (the PS1 kept those cells some other way): each is written into a free unit of the page (not named by a cell, blank) and the cell names it, so the plate shows the arcade's piece where the disc's map
shows the PS1's own. The map is built whole, served in place of the
disc's (owned: a pack keeps none of it).

    python3 port/arcade/plates.py --disc BIN --arcade ZIP [--c-table | --compare-c]

--c-table prints port/arcade/plates_table.c (each file's band, chosen by the most cells equal to the disc's). --compare-c builds plates.c with that table, builds each file's page
as the loader does (screens.c, with the disc), and checks the C against this model and the cells against the disc. Inputs are read only; it prints counts, never game bytes.
"""
import argparse
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
import screen_lists as SL  # noqa: E402

SLOT = 0x11
COLUMNS, ROWS, FIRST_ROW, BAND_ROWS = 64, 32, 3, 8
FIRST_PAGE, CLUT, FILL_TEXTURE = 22, 0x7800, 0x5800
MAP_BYTES, PAGE_AT, PAGE_BYTES, PAGE_ROW = 0x2000, 0x2400, 32768, 128
BANDS = 20


def set_of(c, band):
    """(scroll 1 set, column in it) of map column c."""
    if c < 7:
        return 0, c
    if c < 32:
        return 64 + band, c
    return 84, c - 32


def model_map(prog, gfx, band, page, known=None):
    """(the map's 8,192 bytes, the cells of the strip left as the constant cell, the page with the new units written, the units written (row * 32 + column)).
    `known` (a byte a page byte, 0 not built) leaves out the units that are not whole. A piece that the page lacks goes to the first free unit (not named by a cell,
    built and blank, rows then columns) and the cell names it; a cell with no free unit stays the constant cell."""
    page = bytearray(page)
    units = {}
    for v in range(32):
        for u in range(32):
            at = [(v * 8 + y) * PAGE_ROW + u * 4 for y in range(8)]
            if known is not None and not all(known[a + i] for a in at for i in range(4)):
                continue
            units.setdefault(b''.join(page[a:a + 4] for a in at), (FIRST_PAGE << 10) | (v << 5) | u)
    out = bytearray(MAP_BYTES)
    for i in range(ROWS * COLUMNS):
        struct.pack_into('<HH', out, 4 * i, FILL_TEXTURE, CLUT)
    filled = 0
    blank = bytes(32)
    misses = []
    for r in range(BAND_ROWS):
        for c in range(COLUMNS):
            s, x = set_of(c, band)
            code, attr = SL.scroll1_cell(prog, s, x, FIRST_ROW + r)
            piece = SL.scroll1_piece_bytes(gfx, code, attr)
            if piece is None:
                filled += 1
                continue
            clut = CLUT if piece == blank else CLUT + 64 * (attr & 31)
            tex = units.get(piece)
            if tex is None:
                misses.append((FIRST_ROW + r, c, piece, clut))
                continue
            struct.pack_into('<HH', out, 4 * ((FIRST_ROW + r) * COLUMNS + c), tex, clut)
    referenced = set()
    for i in range(ROWS * COLUMNS):
        tex = struct.unpack_from('<H', out, 4 * i)[0]
        if tex >> 10 == FIRST_PAGE:
            referenced.add(tex & 1023)
    written = []
    nxt = 0
    for r, c, piece, clut in misses:
        tex = units.get(piece)
        if tex is None:
            while nxt < 1024:
                i = nxt
                nxt += 1
                v, u = divmod(i, 32)
                at = [(v * 8 + y) * PAGE_ROW + u * 4 for y in range(8)]
                if i in referenced or (known is not None and not all(known[a + k] for a in at for k in range(4))) or any(page[a:a + 4] != bytes(4) for a in at):
                    continue
                for y, a in enumerate(at):
                    page[a:a + 4] = piece[4 * y:4 * y + 4]
                written.append(i)
                referenced.add(i)
                tex = units[piece] = (FIRST_PAGE << 10) | (v << 5) | u
                break
        if tex is None:
            filled += 1
            continue
        struct.pack_into('<HH', out, 4 * (r * COLUMNS + c), tex, clut)
    return bytes(out), filled, bytes(page), written


def cells_equal(a, b):
    """How many of the 2,048 cells of two maps are equal."""
    return sum(1 for i in range(0, MAP_BYTES, 4) if a[i:i + 4] == b[i:i + 4])


def best_band(prog, gfx, disc_chunk):
    """(band, cells equal to the disc's) of the strip that matches the file's map best, over its own page."""
    page = disc_chunk[PAGE_AT:PAGE_AT + PAGE_BYTES]
    best = None
    for band in range(BANDS):
        eq = cells_equal(model_map(prog, gfx, band, page)[0], disc_chunk[:MAP_BYTES])
        if best is None or eq > best[1]:
            best = (band, eq)
    return best


# ---------------------------------------------------------------- the table and the check

def disc_files(args):
    import stages
    import pac
    files = {}
    with open(args.disc, 'rb') as f:
        index, sec = stages.iso_files(f)
        for path in sorted(index):
            name = path.rsplit('/', 1)[-1]
            if not re.fullmatch(r'NDEMO[0-9A-F]{2}\.PAC', name.upper()):
                continue
            lba, size = index[path]
            data = sec(lba, (size + 2047) // 2048)[:size]
            for c in pac.chunk_table(data):
                if c['table'] == 0 and c['slot'] == SLOT:
                    files[name.upper()] = data[c['offset']:c['offset'] + c['size']]
    return files


def c_table(rows, command):
    out = ['/* GENERATED by `%s`; do not edit.' % command,
           ' * The strip (set 64 + band of the arcade\'s scroll 1) that each name plate file shows. No byte of either game. */',
           '#include "plates.h"', '', 'const struct arcade_plate arcade_plates[] = {']
    out += ['    {"%s", %d},' % row for row in rows]
    out += ['};', 'const unsigned arcade_plate_count = %d;' % len(rows), '']
    return '\n'.join(out)


class CPlates:
    """plates.c, compose.c and the checked-in plates_table.c as a shared library."""

    def __init__(self, tmp):
        lib = Path(tmp) / 'plates.so'
        subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC', '-o', str(lib)] + [str(HERE / n) for n in ('plates.c', 'compose.c', 'plates_table.c')],
                       check=True, capture_output=True, timeout=120)
        self.lib = ctypes.CDLL(str(lib))
        c = ctypes
        self.lib.arcade_plate_build.argtypes = [c.c_char_p, c.c_size_t, c.c_char_p, c.c_size_t, c.c_uint, c.c_char_p, c.c_char_p, c.c_char_p, c.c_char_p, c.c_void_p, c.c_char_p,
                                                c.c_size_t]

    def build(self, prog, gfx, band, page, known):
        """(map, cells left constant, page with the new units, units written) as the C gives them, or its message."""
        out, filled, err = ctypes.create_string_buffer(MAP_BYTES), ctypes.c_uint(0), ctypes.create_string_buffer(200)
        pg, units = ctypes.create_string_buffer(bytes(page), len(page)), ctypes.create_string_buffer(1024)
        rc = self.lib.arcade_plate_build(prog, len(prog), gfx, len(gfx), band, pg, known, out, units, ctypes.byref(filled), err, len(err))
        return err.value.decode() if rc else (out.raw, filled.value, pg.raw, [i for i in range(1024) if units.raw[i]])


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/plates_table.c')
    ap.add_argument('--compare-c', action='store_true', help='check the C against this model and the disc')
    args = ap.parse_args(argv)
    import fighters
    import screens
    prog, gfx = fighters.arcade_views(args.arcade)
    files = disc_files(args)
    started = time.monotonic()
    if args.c_table:
        rows = [(name, best_band(prog, gfx, d)[0]) for name, d in sorted(files.items())]
        sys.stdout.write(c_table(rows, 'python3 port/arcade/plates.py --disc DISC --arcade SET --c-table'))
        print('%d files in %.1f s' % (len(rows), time.monotonic() - started), file=sys.stderr)
        return 0
    if not args.compare_c:
        ap.error('--c-table or --compare-c')
    tmp = tempfile.mkdtemp(prefix='plates-')
    try:
        cp = CPlates(tmp)
        lib = screens.CLib(tmp, prog, gfx)
        text = (HERE / 'plates_table.c').read_text()
        rows = re.findall(r'\{"(NDEMO[0-9A-F]{2}\.PAC)", (\d+)\}', text)
        problems = eq_total = fill_total = 0
        for name, band in rows:
            d = files[name]
            built = lib.build(name, 0, SLOT, 0, d)
            if isinstance(built, str):
                print('%s: the screens build says %s' % (name, built))
                problems += 1
                continue
            data, known = built[0], built[1]
            page, pk = data[PAGE_AT:PAGE_AT + PAGE_BYTES], known[PAGE_AT:PAGE_AT + PAGE_BYTES]
            m, mf, mp, mu = model_map(prog, gfx, int(band), page, pk)
            got = cp.build(prog, gfx, int(band), page, bytes(1 if k else 0 for k in pk))
            if isinstance(got, str):
                print('%s: %s' % (name, got))
                problems += 1
                continue
            same = got == (m, mf, mp, mu)
            eq = cells_equal(m, d[:MAP_BYTES])
            eq_total += eq
            fill_total += mf
            if not same:
                problems += 1
            print('%s: band %s, C %s model, %d of 2048 cells equal to the disc, %d of the strip\'s 512 cells the constant cell, %d units written to the page' % (name, band, '==' if same else '!=', eq, mf, len(mu)))
        print('%d files, %d problems; the strips\' cells that fell to the constant cell: %d' % (len(rows), problems, fill_total))
        return 1 if problems else 0
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == '__main__':
    sys.exit(main())
