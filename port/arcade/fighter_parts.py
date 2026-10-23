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
                            Where the arcade has no such palette the PS1 file holds a ramp of 15 steps (a counter from a
                            start value, red 7); those are counted apart (rule from constants, no arcade data). The
                            rest differs from the arcade set (PL09, PL0B, PL0E, PL14): the Japanese game's own
                            colors, or the set's other region.
  table 2, 0x8000 bytes     a 4 bits a pixel, 256 x 256 picture sent to video memory (the loader sends it in 16
                            sectors of 64 x 16 halfwords, at x 0x280, 0x2c0 for the second side, 0x300 for the shared
                            page): the fighter's portrait page, and a second page (the "VS" lettering) that is the same
                            in 24 files. Every 16 x 16 cell of them that is not blank is an arcade
                            graphics tile (color index + 1); the portrait page also holds 18 palettes of 16 colors with
                            bit 15 set on every color (rows 160 to 177), arcade palettes at 0x29c7c0 + 0x720 * button
                            + 0x60 * fighter + 0x20 * k (button 0..5, k 0..2). Which cells are present and where is
                            not derived here.
  table 3 slot 4 or 5       sound command lists (8 bytes a command: read by the sound player, func_80120604 in
                            ps1/src/resident/sound), one per sound of the fighter's bank; the arcade drives QSound
                            with its own command codes, so it has no source for them.
  table 5 slot 0x100/0x101  a PS1 sequence file (magic pQES: a short run of MIDI-like notes). No arcade source.
  table 0 slot 0xffff       16,384 bytes of zeros that the loader reads and discards (PL15 to PL17, two each).

Inputs: the disc image (raw 2352-byte sectors) and the arcade set's ZIP (MAME's sfa2u), both given as arguments and
both read only. It writes nothing. It prints sizes, addresses and counts, never game bytes. It is an analysis and
reference tool; the converter that the port runs will be written in C.

    python3 port/arcade/fighter_parts.py --disc BIN --arcade ZIP [--only NAME] [--detail]
"""
import argparse
import collections
import struct
import sys
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


def portrait_addresses(pl):
    """Arcade address of each of the 18 palettes in the portrait page, in row order."""
    k = FIGHTER_OF[pl]
    return [PORTRAIT_BASE + PORTRAIT_BUTTON * j + PORTRAIT_FIGHTER * k + PALETTE_BYTES * m for j in range(6) for m in range(3)]


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
    t1 = t['t1 bytes']
    t2 = t['t2 bytes']
    cell = 128
    lines = [f'totals over {nfiles} files (bytes):',
             f'  table 1 palettes {t1}: exact from the arcade by the layout {t["t1 exact"]}, arcade ramps found by search only {t["t1 ramp red 0"]}, '
             f'red-7 ramps (rule, no arcade data) {t["t1 ramp red 7"]}, not reproduced {t["t1 left"]} ({t["t1 colors of other palettes equal"]} of their colors equal)',
             f'  table 2 pages {t2} in {t["t2 cells"]} cells: blank {t["t2 blank"] * cell}, arcade tiles at positions not derived {t["t2 tiles"] * cell} '
             f'({t["t2 distinct"]} distinct pages hold {t["t2 distinct tiles"]} tiles), palette cells {t["t2 palette cells"] * cell} '
             f'of which palettes exact by the layout {t["t2 palette bytes exact"]} and the rest zero, other cells {t["t2 unknown"] * cell}',
             f'  table 3 slot 4/5 sound command lists {t["t3 sound command bytes"]}: no arcade source',
             f'  table 5 sequences {t["t5 sequence bytes"]} ({t["t5 sequence magic ok"]} of {t["t5 sequences"]} start with pQES): no arcade source',
             f'  table 0 slot 0xffff {t["zeros bytes"]} ({t["zero chunks all zero"]} of {t["zero chunks"]} chunks all zero): padding read and discarded',
             f'  not treated here: sound bank headers {t["sound bank header bytes (not here)"]}, sound bank bodies {t["t4 bytes (not here)"]}, '
             f'table 0 code and tiles {t["code or tile bytes (not here)"]}']
    return '\n'.join(lines)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00.PAC or PL00)')
    ap.add_argument('--detail', action='store_true', help='one extra line per file for the first palette that differs')
    args = ap.parse_args(argv)
    started = time.monotonic()
    prog, gfx = arcade_views(args.arcade)
    tiles = {}
    for t in range(len(gfx) // 128):
        tiles.setdefault(gfx[t * 128:t * 128 + 128], t)
    total = collections.Counter()
    seen_pages = {}
    with open(args.disc, 'rb') as f:
        files, sec = iso_files(f)
        names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
        if args.only:
            want = args.only.upper().removesuffix('.PAC') + '.PAC'
            names = [p for p in names if p.endswith('/' + want)]
        for p in names:
            lba, size = files[p]
            data = sec(lba, (size + 2047) // 2048)[:size]
            name = p.split('/')[-1][:-4]
            pl = int(name[2:4], 16)
            second = name.endswith('X')
            try:
                chunks = pac.chunk_table(data)
            except (pac.FormatError, struct.error) as exc:
                print(f'{name}: {exc}')
                continue
            cut = lambda c: data[c['offset']:c['offset'] + c['size']]
            bits = []
            for c in chunks:
                t, s = c['table'], c['slot']
                total['file bytes'] += c['size']
                if t == 1:
                    r = table1(pl, cut(c), prog)
                    total['t1 bytes'] += c['size']
                    total['t1 exact'] += 32 * r['exact']
                    total['t1 ramp red 0'] += 32 * r['ramp0']
                    total['t1 ramp red 7'] += 32 * r['ramp7']
                    total['t1 left'] += 32 * r['other']
                    total['t1 colors of other palettes equal'] += sum(k for _n, k, _a in r['others'])
                    bits.append(f'table 1: {r["exact"]} exact, {r["ramp0"]} arcade ramps, {r["ramp7"]} red-7 ramps, {r["other"]} other of 64 palettes '
                                f'(odd blocks split at {r["split"]}, second {r["layout"]} at {r["c_base"]:#x})')
                    if args.detail and r['others']:
                        bits.append('palettes not reproduced (number:equal colors of 16): ' + ' '.join(f'{n}:{k}' for n, k, _a in r['others']))
                elif t == 2:
                    page = cut(c)
                    key = bytes(page[:0x400]) + bytes(page[-0x400:]) + struct.pack('<I', sum(page) & 0xffffffff)
                    r = table2(pl, second, page, prog, tiles)
                    total['t2 bytes'] += c['size']
                    total['t2 cells'] += 256
                    total['t2 blank'] += r['blank']
                    total['t2 tiles'] += r['tiles']
                    total['t2 palette bytes exact'] += PALETTE_BYTES * r['pal_ok']
                    total['t2 palette cells'] += r['palette_cells']
                    total['t2 unknown'] += r['unknown']
                    if key not in seen_pages:
                        seen_pages[key] = name
                        total['t2 distinct'] += 1
                        total['t2 distinct tiles'] += r['tiles']
                        total['t2 distinct unknown'] += r['unknown']
                    bits.append(f'table 2: {r["blank"]} blank, {r["tiles"]} arcade tiles, {r["unknown"]} other cells, '
                                f'{r["palette_cells"]} palette cells with {r["pal_ok"]}/{r["pal_rows"]} palettes exact')
                elif t == 3 and s in (4, 5):
                    total['t3 sound command bytes'] += c['size']
                elif t == 5:
                    total['t5 sequence bytes'] += c['size']
                    total['t5 sequence magic ok'] += 1 if cut(c)[:4] == b'pQES' else 0
                    total['t5 sequences'] += 1
                elif t == 3:
                    total['sound bank header bytes (not here)'] += c['size']
                elif t == 0 and s == 0xffff:
                    total['zeros bytes'] += c['size']
                    total['zero chunks'] += 1
                    total['zero chunks all zero'] += 1 if not any(cut(c)) else 0
                elif t == 0:
                    total['code or tile bytes (not here)'] += c['size']
                elif t == 4:
                    total['t4 bytes (not here)'] += c['size']
            print(f'{name}: ' + '; '.join(bits))
    print(summary(total, len(names)))
    print(f'{time.monotonic() - started:.1f} s')
    return 0


if __name__ == '__main__':
    sys.exit(main())
