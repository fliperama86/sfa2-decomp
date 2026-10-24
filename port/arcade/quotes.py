#!/usr/bin/env python3
"""Analysis tool: the win quotes of the PS1's win screen in the arcade's English. Derives the table that quotes.c reads, from the disc and the
arcade set, and checks the C by drawing what it builds as the game does.

What it does. After a match the PS1 draws the winner's quote (CDEMOnn.PAC, nn the winner's kind) as a sprite: an object that follows the
fighters' sprite rules. The module (table 0 slot 1, the same in all 21 files) holds, for each kind, a table of u32 quote offsets (24 to 27
entries; the game picks an entry by the loser, the margin of the win and a random number), a cell list at lists base + offset (five u16: n,
mode, palette row offset, 0, stream offset; then the n 16 x 16 tile numbers, mode 0 plain or mode 4 (tile, run - 1)) and a position stream
at streams base + stream offset (a u16 triple dx, dy, run - 1 for each line: a run of tiles at 16 pixels, dx from where the run before ended).
The tiles are those of the glyph segment (the sprite blob's third compressed tile segment, table 0 slot 0x10, which the module reads from
0x80071668), tile 0 the blank one. quotes.c rewrites, for each file, the kind's offset table, lists, streams and the file's glyph segment with
the arcade's four quotes of that fighter (quotes.h has the layout).

What it finds for itself, from the disc: the module offsets of each kind's offset table and its entries (the table of the tables at 0x800192dc,
tables back to back up to it), of the lists and streams bases (the pairs at 0x80021170: streams, then lists, areas back to back; kinds that
share an area share its room), the glyph segment's offset and room (up to the next tile segment; the gap after it must be zeros), and from the
PS1's own quotes how a line is centred (centre) and how far lines are apart (line height); and the quote of a one-line kind (0x14) for the
placement of one-line quotes. The module and the places must be the same in all 21 files. The arcade's quotes (block b of 19 fighters, 4 quotes,
2 lines of 42 words) are mapped to the kinds: block k to kind k for 0..0x11, kind 0x12 (Chun-Li's other version) takes block 4 and kind 0x13
(Gen's other style) block 0x11, because the module gives them the same tables and area as those kinds; kind 0x14 takes the last block (the
dots), whose one-line quote the PS1's own kind 0x14 matches in size.

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade; MAME's sfa2u). It writes nothing but what --c-table
and --previews name and a temporary folder (--compare-c); it prints numbers, never game bytes (--text prints the first line of each arcade
block, which is game text, to show the mapping).

    python3 port/arcade/quotes.py --disc BIN --arcade ZIP [--c-table FILE] [--compare-c] [--previews DIR] [--text]

--c-table FILE writes port/arcade/quotes_table.c. --compare-c compiles quotes.c, tilepack.c, the checked-in quotes_table.c and
tests/quotes_test.c into a temporary folder, runs the build for every file of the table on the real disc and arcade set, and checks each: the
glyph segment is decoded (by tilepack.py, not by the C); each of the kind's offset table entries is followed to its cell list and stream as the
game does and the quote is drawn; the drawing equals the arcade's four quotes drawn by the font rule at the places of quotes.h, and the
characters found in the drawing (every 8 x 16 cell matched to the font's glyphs) equal the arcade's lines; only the bytes of the offset table,
the lists and the streams changed, and every changed byte is marked; the build equals this file's model. Ends with status 0 only when all of it
holds. --previews DIR writes a PNG a file (the four quotes, drawn with the file's quote palette: the blob's 11th palette, video memory row
0x1ea, which the game's init loads and the quote's cells select with palette offset 0).
"""
import argparse
import hashlib
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import zlib
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import fighters as F        # noqa: E402  read only: the disc reader, the arcade program and graphics views
import other_files as O     # noqa: E402  read only: the tile segments of a chunk
import pac                  # noqa: E402  read only: the chunk table
import tilepack as T        # noqa: E402  read only: the tile reader

BASE = 0x80010000           # where the module is linked
OFFSET_TABLES = 0x800192dc  # data_800192dc_slot01: a u32 address a kind, of its quote offset table
AREAS = 0x80021170          # data_80021170_slot01: two u32 a kind, the streams base and the lists base
KINDS = 0x15                # CDEMO00 to CDEMO14
MODULE_SLOT, BLOB_SLOT = 1, 0x10
SEGMENT_INDEX = 2           # the glyph segment among the blob's compressed tile segments
PALETTE_AT = 0x17d48 + 10 * 32   # the blob's palette for video memory row 0x1ea (the load at 0x80070d48 starts at row 0x1e0)
ARCADE_TEXT = 0xe2780       # the arcade program's first quote
LINE, LINES, PER_FIGHTER = 42, 2, 4
BLOCK_BYTES = PER_FIGHTER * LINES * LINE * 2
BLOCKS = 19
BLOCK_OF = list(range(18)) + [4, 17, 18]    # kind -> arcade block (see the docstring)
MAX_RUN = 20
WIDTH = 320
DISC_HELP = 'disc image, raw 2352-byte sectors'


class Fail(Exception):
    pass


# ---------------------------------------------------------------- the arcade's font and text

def be16(prog, a):
    return prog[a] << 8 | prog[a + 1]


def arcade_px(tile, x, y):
    p = y * 8 + (x >> 3) * 4
    sh = 7 - (x & 7)
    return (tile[p] >> sh & 1) | (tile[p + 1] >> sh & 1) << 1 | (tile[p + 2] >> sh & 1) << 2 | (tile[p + 3] >> sh & 1) << 3


def glyph(gfx, c):
    """The 8 x 16 glyph of character c (0x21..0x7e) as 16 rows of 8 PS1 colours (the arcade's plus one, 15 -> 0); a space is blank."""
    if c == 0x20:
        return [[0] * 8 for _ in range(16)]
    rows = []
    for y in range(16):
        k = 0x80 + (c & 15) + 0x20 * ((c - 0x20) >> 4) + (0x10 if y >= 8 else 0)
        at = (0x10000 + (k >> 1)) * 128
        if at + 128 > len(gfx):
            raise Fail('the glyph of %#x lies outside the graphics' % c)
        t = gfx[at:at + 128]
        row = (k & 1) * 8 + (y & 7)
        rows.append([(arcade_px(t, x, row) + 1) & 15 for x in range(8)])
    return rows


def word_char(prog, a):
    """The character of the 16-bit word at a: 0x20 for a space, 0x21..0x7e, or None for a word that is not a character."""
    hi, lo = prog[a], prog[a + 1]
    if hi == 0 and lo == 0:
        return 0x20
    if hi not in (0, 0x80):
        return None
    if lo == 0:
        return 0x20 if hi == 0x80 else None
    if lo < 0x80:
        return None
    c = lo & 0x7f
    return c if c == 0x20 or 0x21 <= c <= 0x7e else None


def arcade_line(prog, a):
    """The 42 characters of the line at a, as a string (raises Fail for a word outside the font)."""
    out = []
    for i in range(LINE):
        c = word_char(prog, a + 2 * i)
        if c is None:
            raise Fail('character %d of the line at %#x is outside the font' % (i, a))
        out.append(chr(c))
    return ''.join(out)


def arcade_quotes(prog, text):
    """[[line0, line1] x 4] of the fighter whose first quote is at `text`, the lines trimmed."""
    return [[arcade_line(prog, text + (q * LINES + l) * LINE * 2).strip(' ') for l in range(LINES)] for q in range(PER_FIGHTER)]


# ---------------------------------------------------------------- the layout (the model of quotes.c)

def tiles_of(lines):
    """The width in tiles of a trimmed line, and its pairs of characters."""
    pairs = [(lines[i], lines[i + 1] if i + 1 < len(lines) else ' ') for i in range(0, len(lines), 2)]
    return pairs


def model_build(quotes, centre, line_height, glyphs):
    """What quotes.c writes for the fighter's 4 quotes (each a list of 2 trimmed lines): dict(tiles=[tile bytes], lists=bytes, streams=bytes,
    list_at=[offset a quote], stream_at=[...]). glyphs(c) -> 8 x 16 rows (see glyph). Raises Fail like the C does."""
    pairs, numbers = [], {}
    cells, runs = [], []
    for q in quotes:
        shown = sum(1 for l in q if l)
        qc, qr, prev_row, x_end, k = [], [], 0, 0, 0
        for li, l in enumerate(q):
            if not l:
                continue
            ps = tiles_of(l)
            if len(ps) > MAX_RUN:
                raise Fail('a line has more than %d tiles' % MAX_RUN)
            row = 1 if shown == 1 else li
            xs = centre - 16 * ((len(ps) + 1) // 2)
            for a, b in ps:
                if a == ' ' and b == ' ':
                    qc.append(0)
                    continue
                if (a, b) not in numbers:
                    numbers[(a, b)] = len(pairs) + 1
                    pairs.append((a, b))
                qc.append(numbers[(a, b)])
            qr.append(((xs - x_end if k else xs) & 0xffff, ((row - prev_row if k else row) * line_height) & 0xffff, len(ps) - 1))
            x_end = xs + 16 * len(ps) - 16
            prev_row = row
            k += 1
        if not qc:
            raise Fail('a quote has no text')
        cells.append(qc)
        runs.append(qr)
    tiles = [bytes(128)]
    for a, b in pairs:
        ga, gb = glyphs(ord(a)), glyphs(ord(b))
        t = bytearray(128)
        for y in range(16):
            row = ga[y] + gb[y]
            for x in range(0, 16, 2):
                t[y * 8 + x // 2] = row[x] | row[x + 1] << 4
        tiles.append(bytes(t))
    streams, stream_at = bytearray(), []
    for qr in runs:
        body = b''.join(struct.pack('<3H', *r) for r in qr)
        for j in range(len(stream_at)):
            if stream_at[j][1] == body:
                stream_at.append(stream_at[j])
                break
        else:
            stream_at.append((len(streams), body))
            streams += body
    lists, list_at, stored = bytearray(), [], []
    for i, qc in enumerate(cells):
        body = struct.pack('<5H', len(qc), 0, 0, 0, stream_at[i][0]) + b''.join(struct.pack('<H', c) for c in qc)
        for j in range(len(stored)):
            if stored[j][1] == body:
                list_at.append(stored[j][0])
                break
        else:
            stored.append((len(lists), body))
            list_at.append(len(lists))
            lists += body
    return dict(tiles=tiles, lists=bytes(lists), streams=bytes(streams), list_at=list_at, stream_at=[s[0] for s in stream_at])


# ---------------------------------------------------------------- the drawing (the game's way)

def u32(m, a):
    return struct.unpack_from('<I', m, a)[0]


def parse_list(module, lists, entry, streams):
    """(cells, runs) of the cell list at lists + entry as the game reads it: modes 0 and 4; runs = [(dx, dy, tiles)] from the stream at
    streams + the list's stream offset. Raises Fail when anything lies outside the module."""
    q = lists + (entry & ~1)
    if q + 10 > len(module):
        raise Fail('a cell list lies outside the module')
    n, mode, _row, _x, so = struct.unpack_from('<5H', module, q)
    cells, p = [], q + 10
    if mode == 0:
        if p + 2 * n > len(module):
            raise Fail('a cell list lies outside the module')
        cells = list(struct.unpack_from('<%dH' % n, module, p))
    elif mode == 4:
        while len(cells) < n:
            if p + 4 > len(module):
                raise Fail('a cell list lies outside the module')
            w, r = struct.unpack_from('<2H', module, p)
            p += 4
            cells += [w + k for k in range(r + 1)]
    else:
        raise Fail('mode %d is not read here' % mode)
    runs, s, total = [], streams + (so & ~1), 0
    while total < n:
        if s + 6 > len(module):
            raise Fail('a stream lies outside the module')
        dx, dy, r = struct.unpack_from('<3H', module, s)
        s += 6
        runs.append((dx - 0x10000 if dx >= 0x8000 else dx, dy - 0x10000 if dy >= 0x8000 else dy, r + 1))
        total += r + 1
    return cells, runs


def place(runs):
    """[(x of the first tile, y, tiles)] of runs as the game steps them (x2 starts at 0; a run ends one tile short of its last tile's right
    edge, and the next dx counts from there)."""
    out, x, y = [], 0, 0
    for i, (dx, dy, r) in enumerate(runs):
        x += dx
        y += dy
        out.append((x, y, r))
        x += 16 * r - 16
    return out


def segment_tiles(segment):
    return [struct.pack('<64H', *w) for _f, w in T.decode_chunk(segment)]


def draw(module, lists, streams, entry, tiles):
    """The canvas (rows of PS1 colours, WIDTH x 16 * rows) of the quote at lists + entry, tiles from segment_tiles."""
    cells, runs = parse_list(module, lists, entry, streams)
    spots = place(runs)
    height = max(y for _x, y, _r in spots) + 16
    canvas = [[0] * WIDTH for _ in range(height)]
    i = 0
    for x, y, r in spots:
        for k in range(r):
            if cells[i] >= len(tiles):
                raise Fail('a cell names tile %d of %d' % (cells[i], len(tiles)))
            t = tiles[cells[i]]
            i += 1
            for yy in range(16):
                for xx in range(16):
                    b = t[yy * 8 + xx // 2]
                    v = b >> 4 if xx & 1 else b & 15
                    X = x + 16 * k + xx
                    if not 0 <= X < WIDTH:
                        raise Fail('a tile lies outside the screen width')
                    if v:
                        canvas[y + yy][X] = v
    return canvas


def draw_expected(lines, centre, line_height, glyphs):
    """The arcade's quote (two trimmed lines) drawn by the font rule at the places of quotes.h: character by character, 8 pixels apart."""
    shown = sum(1 for l in lines if l)
    canvas = [[0] * WIDTH for _ in range(line_height + 16)]
    for li, l in enumerate(lines):
        if not l:
            continue
        row = 1 if shown == 1 else li
        tiles = (len(l) + 1) // 2
        xs = centre - 16 * ((tiles + 1) // 2)
        for ci, ch in enumerate(l):
            g = glyphs(ord(ch))
            for y in range(16):
                for x in range(8):
                    if g[y][x]:
                        canvas[row * line_height + y][xs + 8 * ci + x] = g[y][x]
    return canvas


def read_back(canvas, rows, line_height, glyph_of):
    """The text of each row of the canvas, found by matching every 8 x 16 cell with the font's glyphs; None for a cell no glyph matches, and a
    set of candidate characters is taken when glyphs look alike (a cell matches when the expected character is among them: see compare)."""
    by_pixels = {}
    for c in range(0x20, 0x7f):
        by_pixels.setdefault(bytes(v for r in glyph_of(c) for v in r), set()).add(chr(c))
    out = []
    for row in range(rows):
        cells = []
        for cx in range(0, WIDTH, 8):
            key = bytes(canvas[row * line_height + y][cx + x] if row * line_height + y < len(canvas) else 0 for y in range(16) for x in range(8))
            cells.append(by_pixels.get(key))
        out.append(cells)
    return out


# ---------------------------------------------------------------- the disc

class Disc:
    """The 21 files' module and blob."""

    def __init__(self, path):
        self.files = {}
        with open(path, 'rb') as f:
            index, sec = F.iso_files(f)
            for k in range(KINDS):
                name = 'CDEMO%02X.PAC' % k
                hits = [p for p in index if p.upper().endswith('/' + name)]
                if len(hits) != 1:
                    raise Fail('%s is not on the disc' % name)
                lba, size = index[hits[0]]
                data = sec(lba, (size + 2047) // 2048)[:size]
                chunks = {(c['table'], c['slot']): data[c['offset']:c['offset'] + c['size']] for c in pac.chunk_table(data)}
                if (0, MODULE_SLOT) not in chunks or (0, BLOB_SLOT) not in chunks:
                    raise Fail('%s lacks its module or its sprite blob' % name)
                self.files[name] = (k, chunks[(0, MODULE_SLOT)], chunks[(0, BLOB_SLOT)])


def module_tables(module):
    """From the module: (offset_tables [(table module offset, entries)] a kind, areas [(streams, streams_room, lists, lists_room)] a kind)."""
    addrs = [u32(module, OFFSET_TABLES - BASE + 4 * k) for k in range(KINDS)]
    ends = sorted(set(addrs) | {OFFSET_TABLES})
    tables = []
    for a in addrs:
        n = (ends[ends.index(a) + 1] - a) // 4
        if not 20 <= n <= 30:
            raise Fail('the quote offset table at %#x has %d entries' % (a, n))
        tables.append((a - BASE, n))
    pa = [(u32(module, AREAS - BASE + 8 * k), u32(module, AREAS - BASE + 8 * k + 4)) for k in range(KINDS)]
    order = sorted(set(pa))
    areas = []
    for s, l in pa:
        nxt = order[order.index((s, l)) + 1][0] if order.index((s, l)) + 1 < len(order) else AREAS
        if not s < l <= nxt:
            raise Fail('the areas at %#x, %#x and %#x are not in order' % (s, l, nxt))
        areas.append((s - BASE, l - s, l - BASE, nxt - l))
    return tables, areas


def own_quotes(module, tables, areas):
    """[(kind, entry, cells, runs)] of every distinct list the module's offset tables name, read as the game does."""
    out, seen = [], set()
    for k in range(KINDS):
        t, n = tables[k]
        s, _sr, l, _lr = areas[k]
        for e in range(n):
            entry = u32(module, t + 4 * e)
            if (l, entry) in seen:
                continue
            seen.add((l, entry))
            cells, runs = parse_list(module, l, entry, s)
            out.append((k, entry, cells, runs))
    return out


def centring(own):
    """(centre, line height, findings) from the PS1's own quotes: lines that are one run, the left edge of the first tile and its width."""
    centres, heights, one_line = Counter(), Counter(), []
    for k, _e, cells, runs in own:
        if any(i and dy == 0 for i, (_dx, dy, _r) in enumerate(runs)):
            continue
        spots = place(runs)
        if len(runs) == 1:
            one_line.append((k, spots[0][1], runs[0][2]))
        for i, (x, y, r) in enumerate(spots):
            centres[x + 8 * (r + (r & 1))] += 1
            if i:
                heights[runs[i][1]] += 1
    if not centres or not heights:
        raise Fail('the PS1 has no quote to take the centring from')
    return centres.most_common(1)[0][0], heights.most_common(1)[0][0], dict(centres=centres, heights=heights, one_line=one_line)


def glyph_segment(blob):
    """(start, tiles, end) of the glyph segment and the start of the next one."""
    segs = O.tile_segments(blob)
    if len(segs) <= SEGMENT_INDEX + 1:
        raise Fail('the sprite blob has %d tile segments' % len(segs))
    return segs[SEGMENT_INDEX], segs[SEGMENT_INDEX + 1][0]


def derive(disc, program):
    """Everything the table holds, from the disc: dict(rows, centre, line_height, notes). Raises Fail."""
    modules = {hashlib.sha256(m).hexdigest() for _k, m, _b in disc.files.values()}
    if len(modules) != 1:
        raise Fail('the 21 modules are not the same')
    module = disc.files['CDEMO00.PAC'][1]
    tables, areas = module_tables(module)
    own = own_quotes(module, tables, areas)
    centre, line_height, notes = centring(own)
    rows, segs = [], set()
    for name, (k, _m, blob) in sorted(disc.files.items()):
        (start, count, end), nxt = glyph_segment(blob)
        if end > nxt or any(blob[end:nxt]):
            raise Fail('%s: the gap after the glyph segment is not zeros' % name)
        segs.add((start, nxt))
        s, sr, l, lr = areas[k]
        t, n = tables[k]
        rows.append(dict(file=name, kind=k, text=ARCADE_TEXT + BLOCK_BYTES * BLOCK_OF[k], offsets=t, count=n, lists=l, lists_room=lr,
                         streams=s, streams_room=sr, segment=start, segment_room=nxt - start, disc_tiles=count))
    if len(segs) != 1:
        raise Fail('the glyph segments are not in the same place in the 21 files')
    notes.update(own=len(own), blocks=BLOCKS)
    if ARCADE_TEXT + BLOCK_BYTES * BLOCKS > len(program):
        raise Fail('the arcade quotes lie outside the program')
    return dict(rows=rows, centre=centre, line_height=line_height, notes=notes)


def c_table(d):
    lines = ['/* GENERATED by `python3 port/arcade/quotes.py --disc DISC --arcade SET --c-table FILE` (FILE = port/arcade/quotes_table.c); do not edit.',
             ' * For each of the 21 CDEMOnn.PAC files: the arcade address of the fighter\'s first quote, the module offsets of the kind\'s quote offset',
             ' * table, lists base and streams base with the bytes each may take, and the glyph segment\'s blob offset and room; and how the PS1\'s',
             ' * own quotes are centred. Only offsets, addresses and counts: no byte of either game. */',
             '#include "quotes.h"', '', 'static const struct arcade_quote_fighter fighters[] = {']
    for r in d['rows']:
        lines.append('    {"%s", %#x, %#x, %#x, %du, %#x, %#x, %#x, %#x, %#x, %#x}, /* block %d */' % (
            r['file'], r['kind'], r['text'], r['offsets'], r['count'], r['lists'], r['lists_room'], r['streams'], r['streams_room'],
            r['segment'], r['segment_room'], BLOCK_OF[r['kind']]))
    lines += ['};', '', 'const struct arcade_quotes arcade_quotes = {%du, %#xu, %d, %du, fighters, sizeof fighters / sizeof fighters[0]};' % (
        MODULE_SLOT, BLOB_SLOT, d['centre'], d['line_height'])]
    return '\n'.join(lines) + '\n'


# ---------------------------------------------------------------- PNG

def png(path, width, height, rgb):
    """A PNG of rgb (bytes, 3 a pixel)."""
    def chunk(tag, body):
        c = tag + body
        return struct.pack('>I', len(body)) + c + struct.pack('>I', zlib.crc32(c) & 0xffffffff)
    raw = b''.join(b'\0' + rgb[y * width * 3:(y + 1) * width * 3] for y in range(height))
    Path(path).write_bytes(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
                           + chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))


def colours(blob):
    """The 16 RGB triples of the quote palette (the blob's 11th), colour 0 shown as a dark blue instead of transparent."""
    out = []
    for i in range(16):
        w = struct.unpack_from('<H', blob, PALETTE_AT + 2 * i)[0]
        out.append((20, 24, 60) if i == 0 else (((w & 31) * 255) // 31, (((w >> 5) & 31) * 255) // 31, (((w >> 10) & 31) * 255) // 31))
    return out


def preview(path, canvases, blob, scale=3, pad=8):
    pal = colours(blob)
    height = sum(len(c) + pad for c in canvases) + pad
    img = [[(0, 0, 0)] * WIDTH for _ in range(height)]
    y = pad
    for c in canvases:
        for r, row in enumerate(c):
            img[y + r] = [pal[v] for v in row]
        y += len(c) + pad
    out = bytearray()
    for row in img:
        line = b''.join(bytes(p) * scale for p in row)
        out += line * scale
    png(path, WIDTH * scale, height * scale, bytes(out))


# ---------------------------------------------------------------- the C

def build_driver(tmp, sanitize=False):
    exe = str(Path(tmp) / 'quotes_test')
    cmd = ['gcc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror']
    if sanitize:
        cmd += ['-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g']
    subprocess.run(cmd + ['-o', exe, str(HERE / 'quotes.c'), str(HERE / 'tilepack.c'), str(HERE / 'quotes_table.c'), str(HERE / 'tests/quotes_test.c')],
                   check=True, timeout=180)
    return exe


def parse_table_dump(text):
    lines = text.strip().split('\n')
    centre, height, count, mslot, bslot = map(int, lines[0].split())
    rows = []
    for ln in lines[1:1 + count]:
        f = ln.split()
        v = [int(x, 0) for x in f[1:]]
        rows.append(dict(file=f[0], kind=v[0], text=v[1], offsets=v[2], count=v[3], lists=v[4], lists_room=v[5], streams=v[6],
                         streams_room=v[7], segment=v[8], segment_room=v[9]))
    return centre, height, mslot, bslot, rows


def compare_c(d, disc, program, gfx, text, tmp, previews):
    status = 0
    same = (HERE / 'quotes_table.c').exists() and (HERE / 'quotes_table.c').read_text() == text
    print('checked-in quotes_table.c ' + ('equals' if same else 'DIFFERS FROM') + ' the table this tool derives')
    if not same:
        return 1
    exe = build_driver(tmp)
    centre, height, mslot, bslot, c_rows = parse_table_dump(subprocess.run([exe, 'table'], check=True, capture_output=True, text=True, timeout=60).stdout)
    keys = ('file', 'kind', 'text', 'offsets', 'count', 'lists', 'lists_room', 'streams', 'streams_room', 'segment', 'segment_room')
    if (centre, height, mslot, bslot) != (d['centre'], d['line_height'], MODULE_SLOT, BLOB_SLOT) or \
            [tuple(r[k] for k in keys) for r in c_rows] != [tuple(r[k] for k in keys) for r in d['rows']]:
        print("the C table differs from the derived one")
        return 1
    prog_f, gfx_f, out = Path(tmp) / 'program', Path(tmp) / 'gfx', Path(tmp) / 'out'
    out.mkdir()
    prog_f.write_bytes(program)
    gfx_f.write_bytes(gfx)
    glyph_cache = {}

    def glyphs(c):
        if c not in glyph_cache:
            glyph_cache[c] = glyph(gfx, c)
        return glyph_cache[c]
    totals = Counter()
    for r in c_rows:
        name = r['file']
        k, module, blob = disc.files[name]
        mod_f = Path(tmp) / 'module'
        mod_f.write_bytes(module)
        args = [str(r[x]) for x in ('kind', 'text', 'offsets', 'count', 'lists', 'lists_room', 'streams', 'streams_room', 'segment', 'segment_room')]
        run = subprocess.run([exe, 'build', name] + args + [str(centre), str(height), str(prog_f), str(gfx_f), str(mod_f), str(out)],
                             capture_output=True, text=True, timeout=120)
        if run.returncode:
            print('%s: the C refused: %s %s' % (name, run.stdout.strip(), run.stderr.strip()))
            status = 1
            continue
        new, changed, segment = (out / 'module').read_bytes(), (out / 'changed').read_bytes(), (out / 'segment').read_bytes()
        problems = []
        # the places written
        allowed = [(r['offsets'], 4 * r['count']), (r['lists'], r['lists_room']), (r['streams'], r['streams_room'])]
        for i in range(len(module)):
            inside = any(a <= i < a + n for a, n in allowed)
            if new[i] != module[i] and not (inside and changed[i]):
                problems.append('byte %#x changed outside the rooms or unmarked' % i)
                break
            if changed[i] and not inside:
                problems.append('byte %#x marked outside the rooms' % i)
                break
        if len(segment) > r['segment_room']:
            problems.append('the segment is longer than its room')
        # the model
        quotes = arcade_quotes(program, r['text'])
        try:
            model = model_build(quotes, centre, height, glyphs)
        except Fail as e:
            problems.append('the model refuses: %s' % e)
            model = None
        if model:
            if new[r['lists']:r['lists'] + len(model['lists'])] != model['lists'] or new[r['streams']:r['streams'] + len(model['streams'])] != model['streams']:
                problems.append('lists or streams differ from the model')
            if [u32(new, r['offsets'] + 4 * e) for e in range(r['count'])] != [model['list_at'][e % PER_FIGHTER] for e in range(r['count'])]:
                problems.append('the offset table differs from the model')
            if [segment_tiles(segment)[i] for i in range(len(model['tiles']))] != model['tiles'] or len(segment_tiles(segment)) != len(model['tiles']):
                problems.append('the segment decodes to other tiles than the model')
        # the drawing
        tiles = segment_tiles(segment)
        canvases = []
        for e in range(r['count']):
            entry = u32(new, r['offsets'] + 4 * e)
            q = e % PER_FIGHTER
            try:
                got = draw(new, r['lists'], r['streams'], entry, tiles)
            except Fail as ex:
                problems.append('entry %d: %s' % (e, ex))
                continue
            want = draw_expected(quotes[q], centre, height, glyphs)
            if len(got) < len(want):
                got = got + [[0] * WIDTH] * (len(want) - len(got))
            if got != want:
                problems.append('entry %d (quote %d) is not the arcade quote as drawn by the font rule' % (e, q))
                continue
            if e < PER_FIGHTER:
                canvases.append(got)
                back = read_back(got, LINES, height, glyphs)
                for li, l in enumerate(quotes[q]):
                    if not l:
                        continue
                    row = 1 if sum(1 for x in quotes[q] if x) == 1 else li
                    xs = centre - 16 * (((len(l) + 1) // 2 + 1) // 2)
                    text_back = back[row][xs // 8:xs // 8 + len(l)]
                    if any(c is None or l[i] not in c for i, c in enumerate(text_back)):
                        problems.append('quote %d line %d: the characters found are not the arcade line' % (q, li))
                    elif any(len(c) > 1 for c in text_back):
                        totals['ambiguous'] += sum(1 for c in text_back if len(c) > 1)
        n_tiles = len(tiles)
        used = lambda a, n: next((i for i in range(n, 0, -1) if changed[a + i - 1]), 0)
        print('%s: %d tiles (disc %d), segment %d of %d bytes, lists %d of %d, streams %d of %d, offset table %d entries: %s' % (
            name, n_tiles, r_disc_tiles(d, name), len(segment), r['segment_room'], used(r['lists'], r['lists_room']), r['lists_room'],
            used(r['streams'], r['streams_room']), r['streams_room'], r['count'], 'OK' if not problems else 'PROBLEMS'))
        for p in problems[:5]:
            print('    ' + p)
        if problems:
            status = 1
        totals['files'] += 1
        totals['ok'] += not problems
        if previews and canvases and not problems:
            Path(previews).mkdir(parents=True, exist_ok=True)
            preview(Path(previews) / name.replace('.PAC', '.png'), canvases, blob)
    print('%d of %d files equal the arcade quotes as drawn (%d glyph cells could be more than one character)' % (totals['ok'], totals['files'], totals['ambiguous']))
    return status


def r_disc_tiles(d, name):
    return next(r['disc_tiles'] for r in d['rows'] if r['file'] == name)


# ---------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help=DISC_HELP)
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--c-table', metavar='FILE', help='write port/arcade/quotes_table.c to FILE')
    ap.add_argument('--compare-c', action='store_true', help='build the checked-in table in C and check what it builds by drawing it')
    ap.add_argument('--previews', metavar='DIR', help='write a PNG a file (private) into DIR; with --compare-c')
    ap.add_argument('--text', action='store_true', help="print the first line of each arcade block (game text) to show the mapping")
    args = ap.parse_args(argv)
    started = time.monotonic()
    try:
        disc = Disc(args.disc)
        program, gfx = F.arcade_views(args.arcade)
        d = derive(disc, program)
    except Fail as e:
        print('FAILED: %s' % e)
        return 1
    n = d['notes']
    print('%d files, one module; %d distinct PS1 quotes read; centre %d, line height %d' % (len(d['rows']), n['own'], d['centre'], d['line_height']))
    print('  centre of the PS1 lines (x of the first tile + 8 tiles rounded up to even): %s' % dict(n['centres'].most_common(4)))
    print('  line steps: %s; one-run quotes of the PS1: %s' % (dict(n['heights']), [(k, 'first dy %d' % y, '%d tiles' % r) for k, y, r in n['one_line']][:6]))
    print('  quote offset tables %d to %d entries; arcade blocks %d' % (min(r['count'] for r in d['rows']), max(r['count'] for r in d['rows']), BLOCKS))
    for r in d['rows']:
        print('  %s kind %#04x block %2d: offsets %#x x%d, lists %#x room %d, streams %#x room %d, segment %#x room %d (disc %d tiles)' % (
            r['file'], r['kind'], BLOCK_OF[r['kind']], r['offsets'], r['count'], r['lists'], r['lists_room'], r['streams'], r['streams_room'],
            r['segment'], r['segment_room'], r['disc_tiles']))
    if args.text:
        for b in range(BLOCKS):
            print('  block %2d: %s' % (b, arcade_line(program, ARCADE_TEXT + BLOCK_BYTES * b).strip()))
    text = c_table(d)
    status = 0
    if args.c_table:
        Path(args.c_table).write_text(text)
        print('wrote %s' % args.c_table)
    if args.compare_c:
        tmp = tempfile.mkdtemp(prefix='quotes-')
        try:
            status = compare_c(d, disc, program, gfx, text, tmp, args.previews)
        except Fail as e:
            print('FAILED: %s' % e)
            status = 1
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    print('done in %.1f s' % (time.monotonic() - started))
    return status


if __name__ == '__main__':
    sys.exit(main())
