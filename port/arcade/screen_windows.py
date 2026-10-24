#!/usr/bin/env python3
"""Analysis tool and finder: the screens' tiles that the PS1 cut on its own grid from frames of the arcade (the WINDOW items of screens.py), and the portrait
tiles it clipped.

What it is. The PS1's select, continue and character-demo screens show fighters in poses and some pictures that the arcade has as frames: art records
of the fighters' sprites, or blocks of the scroll 2 layer. The PS1 did not keep the arcade's cells: it drew the frame and cut it again on a 16-pixel
grid of its own (so the tiles are not the arcade's, but every pixel is), sometimes in two interlaced fields (the odd rows in one list, the even rows in
another). A WINDOW item says: draw this frame (compose.py is the model, compose.c the loader's code), cut the 16 x 16 tile with its top left at (x, y) of the
frame's coordinates, keep all rows, the odd rows or the even rows, and store it flipped. The finder (Windows.find) looks a disc tile up in the frames of
SOURCES, at any position, and returns the window that gives exactly the disc's pixels. A tile in no frame stays the disc's, or blank by design.

SOURCES is the list of frames the finder tries, in order (the loader's table holds the ones that windows use). `--find` makes it: it collects the tiles
of the disc's files that the arcade does not give as runs of its tiles or pieces, finds the arcade frames where their pixels lie (a search of the graphics as a
picture, then the records and blocks that draw those tiles), and chooses frames one after another by how many tiles each adds. It prints the result;
`--write` puts it in this file between the markers below. Nothing is a guess that the build keeps: the finder returns a window only when the cut is
equal to the disc's tile, pixel for pixel.

The portrait edge (Windows.edge). The PS1 clipped the lower right of each big portrait of the select screen (the arcade's records of mode 4) along a curve;
the tiles on the curve are the arcade's tile with pixels removed. edge(tile) gives the arcade tile (code, flip) of the portraits whose pixels contain the
tile's, the one with the fewest pixels more. Screens that use it are named by a row of screens.DESIGN_ROWS (units 'portrait'): the arcade's whole tile is
built there, not the clipped one, by design.

Inputs of --find: the disc image (--disc) and the arcade set's ZIP (--arcade), read only. It writes nothing but a temporary folder (screens.py's C helper) and,
with --write, SOURCES below. It prints counts and addresses, never game bytes.

    python3 port/arcade/screen_windows.py --disc BIN --arcade ZIP --find [--write]
"""
import argparse
import collections
import re
import shutil
import struct
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import compose  # noqa: E402
import screen_lists as SL  # noqa: E402

SRC_RECORD, SRC_SCROLL2 = 1, 2
PAD = 16                    # zero margin round a frame when it is searched: a tile may hang over the frame's edge
MIN_ROW_COLOURS = 3         # a row to look a tile up by has at least this many different non-zero values
BANKS = (0, 0x10000, 0x20000, 0x30000)
PORTRAITS = 0x5e6c0         # the arcade's table of the select portraits: fighter k's record pointer at PORTRAITS + 0x14 * k
PORTRAIT_FIGHTERS = 19
MIN_EDGE_PIXELS = 5         # a clipped tile has at least this many pixels
NEIGHBOURS = 0x800          # --find also tries the records this near (in address) to a frame it chose: a pose's frames lie one after another

# BEGIN SOURCES (written by --find --write): (kind, address, bank, blocks), kind 1 an art record, kind 2 scroll 2 blocks from `address` side by side
SOURCES = (
    (2, 0x9e, 0x0, 2),
    (1, 0x19380e, 0x0, 0),
    (1, 0x1b1718, 0x0, 0),
    (1, 0x179d88, 0x20000, 0),
    (1, 0x1937ba, 0x0, 0),
    (1, 0x1797bc, 0x20000, 0),
    (1, 0x1e28f8, 0x0, 0),
    (1, 0x1e247c, 0x0, 0),
    (1, 0x1e352c, 0x0, 0),
    (1, 0x1af710, 0x0, 0),
    (1, 0x1e34cc, 0x0, 0),
    (1, 0x1966ca, 0x0, 0),
    (1, 0x1e34fc, 0x0, 0),
    (1, 0x1e3498, 0x0, 0),
    (1, 0x1af6e8, 0x0, 0),
    (1, 0x1afa08, 0x0, 0),
    (1, 0x1a0d26, 0x0, 0),
    (1, 0x1965d2, 0x0, 0),
    (1, 0x1d04a2, 0x20000, 0),
    (1, 0x19465e, 0x0, 0),
    (1, 0x1a80fc, 0x0, 0),
    (1, 0x180490, 0x20000, 0),
    (1, 0x15eb9c, 0x0, 0),
    (1, 0x1946ca, 0x0, 0),
    (1, 0x17a4ac, 0x20000, 0),
    (1, 0x2b876e, 0x10000, 0),
    (1, 0x1b7a9c, 0x0, 0),
    (1, 0x1d84e0, 0x0, 0),
    (1, 0x1c1232, 0x0, 0),
    (1, 0x19384a, 0x0, 0),
    (1, 0x14e16c, 0x0, 0),
    (1, 0x14c1d4, 0x0, 0),
    (1, 0x1f77b8, 0x0, 0),
    (1, 0x1ceac8, 0x20000, 0),
    (1, 0x14c3ac, 0x0, 0),
    (1, 0x1b6ad0, 0x0, 0),
    (1, 0x1a125e, 0x0, 0),
    (1, 0x14e1a0, 0x0, 0),
    (1, 0x1c1256, 0x0, 0),
    (1, 0x17a518, 0x20000, 0),
    (1, 0x179ca4, 0x20000, 0),
    (1, 0x14e208, 0x0, 0),
    (1, 0x14cb68, 0x0, 0),
    (1, 0x1d0326, 0x20000, 0),
    (1, 0x15e610, 0x0, 0),
    (1, 0x14e230, 0x0, 0),
    (1, 0x14c1b8, 0x0, 0),
    (1, 0x1d039e, 0x20000, 0),
    (1, 0x1b7b9c, 0x0, 0),
    (1, 0x153f52, 0x0, 0),
    (1, 0x14bb58, 0x0, 0),
    (1, 0x1f7074, 0x0, 0),
    (1, 0x1e2334, 0x0, 0),
    (1, 0x1d87c8, 0x0, 0),
    (1, 0x1d038a, 0x20000, 0),
    (1, 0x1bf8ae, 0x0, 0),
    (1, 0x14c190, 0x0, 0),
    (1, 0x1c11d6, 0x0, 0),
    (1, 0x1c11aa, 0x0, 0),
    (1, 0x14e1d8, 0x0, 0),
    (1, 0x14e144, 0x0, 0),
    (1, 0x1d85dc, 0x0, 0),
    (1, 0x1c1276, 0x0, 0),
    (1, 0x1afa28, 0x0, 0),
    (1, 0x1b6bd8, 0x0, 0),
    (1, 0x1b6bac, 0x0, 0),
    (1, 0x15f970, 0x0, 0),
    (1, 0x1c120a, 0x0, 0),
    (1, 0x179cc8, 0x20000, 0),
    (1, 0x1b17bc, 0x0, 0),
    (1, 0x1afbb4, 0x0, 0),
    (1, 0x179d54, 0x20000, 0),
    (1, 0x1b84f4, 0x0, 0),
    (1, 0x14cb14, 0x0, 0),
    (1, 0x1d85b0, 0x0, 0),
    (1, 0x1938a2, 0x0, 0),
    (1, 0x1a019e, 0x10000, 0),
    (1, 0x1f77f4, 0x0, 0),
    (1, 0x1b180c, 0x0, 0),
    (1, 0x19422a, 0x0, 0),
    (1, 0x14ba88, 0x0, 0),
    (1, 0x1b1770, 0x0, 0),
    (1, 0x1938fe, 0x0, 0),
    (1, 0x2b8752, 0x10000, 0),
    (1, 0x1f757c, 0x0, 0),
    (1, 0x1b6b80, 0x0, 0),
    (1, 0x14cb40, 0x0, 0),
    (1, 0x14cae8, 0x0, 0),
    (1, 0x153b2a, 0x0, 0),
    (1, 0x168cba, 0x10000, 0),
    (1, 0x1f777c, 0x0, 0),
    (1, 0x1af9e8, 0x0, 0),
    (1, 0x19395a, 0x0, 0),
    (1, 0x1b829c, 0x0, 0),
    (1, 0x1a0126, 0x10000, 0),
    (1, 0x14c364, 0x0, 0),
    (1, 0x1a01c6, 0x10000, 0),
    (1, 0x1a0702, 0x10000, 0),
    (1, 0x168ce2, 0x10000, 0),
    (1, 0x14c320, 0x0, 0),
    (1, 0x14c2d4, 0x0, 0),
    (1, 0x14c28c, 0x0, 0),
    (1, 0x1b714c, 0x0, 0),
    (1, 0x1a0446, 0x10000, 0),
    (1, 0x1a022e, 0x10000, 0),
    (1, 0x1a04fa, 0x10000, 0),
    (1, 0x1a01ea, 0x10000, 0),
    (1, 0x14c244, 0x0, 0),
    (1, 0x14c1fc, 0x0, 0),
    (1, 0x1a04c2, 0x10000, 0),
    (1, 0x1a046a, 0x10000, 0),
    (1, 0x1a05e2, 0x10000, 0),
    (1, 0x1a031a, 0x10000, 0),
    (1, 0x1a0206, 0x10000, 0),
    (1, 0x179d0c, 0x20000, 0),
    (1, 0x1e1f04, 0x0, 0),
    (1, 0x1af63a, 0x0, 0),
    (1, 0x1a097a, 0x10000, 0),
    (1, 0x1a092a, 0x10000, 0),
    (1, 0x1a08fe, 0x10000, 0),
    (1, 0x1a08c6, 0x10000, 0),
    (1, 0x1a084e, 0x10000, 0),
    (1, 0x1a062e, 0x10000, 0),
    (1, 0x1a059e, 0x10000, 0),
    (1, 0x1a052a, 0x10000, 0),
    (1, 0x1a0492, 0x10000, 0),
    (1, 0x1a0422, 0x10000, 0),
    (1, 0x1a0156, 0x10000, 0),
    (1, 0x1a0ec6, 0x10000, 0),
    (1, 0x1a10ae, 0x10000, 0),
    (1, 0x1a108a, 0x10000, 0),
    (1, 0x1a0e6e, 0x10000, 0),
    (1, 0x1a0ae2, 0x10000, 0),
    (1, 0x1a10da, 0x10000, 0),
    (1, 0x1a1062, 0x10000, 0),
    (1, 0x1a1042, 0x10000, 0),
    (1, 0x1a1022, 0x10000, 0),
    (1, 0x1a0c9e, 0x10000, 0),
    (1, 0x1a0a4e, 0x10000, 0),
    (1, 0x1a09ba, 0x10000, 0),
    (1, 0x1a1132, 0x10000, 0),
    (1, 0x1a0ffe, 0x10000, 0),
    (1, 0x1a0fca, 0x10000, 0),
    (1, 0x1a0f92, 0x10000, 0),
    (1, 0x1a0f6a, 0x10000, 0),
    (1, 0x1a0b42, 0x10000, 0),
    (1, 0x1a09fa, 0x10000, 0),
    (1, 0x1a1182, 0x10000, 0),
    (1, 0x1a125e, 0x10000, 0),
    (1, 0x1a129e, 0x10000, 0),
    (1, 0x1a1474, 0x10000, 0),
    (1, 0x1a143a, 0x10000, 0),
    (1, 0x1a13fe, 0x10000, 0),
    (1, 0x1a12e2, 0x10000, 0),
    (1, 0x1a12ba, 0x10000, 0),
    (1, 0x1a127a, 0x10000, 0),
    (1, 0x1a1222, 0x10000, 0),
)
# END SOURCES


# ---------------------------------------------------------------- pixels

def unpack(tile):
    """The 256 pixel values of a 128-byte PS1 tile."""
    px = bytearray(256)
    px[0::2] = bytes(b & 15 for b in tile)
    px[1::2] = bytes(b >> 4 for b in tile)
    return bytes(px)


def pack(px):
    return bytes(px[2 * k] | px[2 * k + 1] << 4 for k in range(128))


def flip_px(px, f):
    return SL.flip_pixels(px, f)


class Frame:
    """A drawn source, padded with zeros so that a tile hanging over its edge is found, searched by rows."""

    def __init__(self, canvas):
        self.x0, self.y0, w, h, px = canvas
        self.w, self.h = w + 2 * PAD, h + 2 * PAD
        rows = [bytes(self.w)] * PAD + [bytes(PAD) + px[y * w:(y + 1) * w] + bytes(PAD) for y in range(h)] + [bytes(self.w)] * PAD
        self.px = b''.join(rows)
        self.hist = [self.px.count(v) for v in range(16)]      # how many pixels of each value: a tile with more of a value than the frame is not in it

    def holds(self, hist):
        """False when a tile of the pixel counts `hist` cannot lie in the frame."""
        return all(h <= f for h, f in zip(hist[1:], self.hist[1:]))

    def find(self, rows, kept):
        """(x, y) in the frame's coordinates of the 16 x 16 window whose kept rows equal rows[y] (16 values each), or None. `kept` is a list of the kept rows."""
        best = max(kept, key=lambda y: len(set(rows[y]) - {0}))
        needle = rows[best]
        if len(set(needle) - {0}) < 1:
            return None
        pos = self.px.find(needle)
        while pos >= 0:
            x, yy = pos % self.w, pos // self.w
            top = yy - best
            if x + 16 <= self.w and 0 <= top and top + 16 <= self.h:
                if all(self.px[(top + y) * self.w + x:(top + y) * self.w + x + 16] == rows[y] for y in kept):
                    return x - PAD + self.x0, top - PAD + self.y0
            pos = self.px.find(needle, pos + 1)
        return None


def draw(prog, gfx, src):
    kind, address, bank, blocks = src
    if kind == SRC_RECORD:
        return compose.record_canvas(prog, gfx, address, bank)
    return compose.scroll2_canvas(prog, gfx, address, blocks)


ROWS_KEPT = {0: list(range(16)), 1: list(range(1, 16, 2)), 2: list(range(0, 16, 2))}


def row_kinds(px):
    """The row masks a tile can be a window with: 0 always, 1 when its even rows are all zero, 2 when its odd rows are."""
    out = [0]
    if not any(px[y * 16 + x] for y in range(0, 16, 2) for x in range(16)):
        out.append(1)
    if not any(px[y * 16 + x] for y in range(1, 16, 2) for x in range(16)):
        out.append(2)
    return out


class Windows:
    """The finder: windows of the frames of `sources`, and the portrait edge."""

    def __init__(self, prog, gfx, sources=None):
        self.prog, self.gfx = prog, gfx
        self.sources = list(SOURCES if sources is None else sources)
        self._frames = {}
        self._found = {}
        self._edge = None
        self._edge_cache = {}

    def frame(self, k):
        if k not in self._frames:
            canvas = draw(self.prog, self.gfx, self.sources[k])
            self._frames[k] = Frame(canvas) if canvas and canvas[2] else None
        return self._frames[k]

    def find(self, tile):
        """(kind, address, bank, blocks, x, y, flip, rows) of the first window of the sources that gives exactly `tile` (128 bytes), or None."""
        hit = self._found.get(tile, 0)
        if hit != 0:
            return hit
        px = unpack(tile)
        result = None
        hist = [px.count(v) for v in range(16)]
        if any(px):
            for k, src in enumerate(self.sources):
                fr = self.frame(k)
                if fr is None or not fr.holds(hist):
                    continue
                for f in range(4):
                    cut = flip_px(px, f)
                    rows = [cut[y * 16:y * 16 + 16] for y in range(16)]
                    for kind in row_kinds(cut):
                        at = fr.find(rows, ROWS_KEPT[kind])
                        if at:
                            result = src + (at[0], at[1], f, kind)
                            break
                    if result:
                        break
                if result:
                    break
        self._found[tile] = result
        return result

    # ---- the portraits' edge

    def edge_candidates(self):
        """[(code, flip, pixels)] of the tiles of the select portraits (records of mode 4, bank 0x10000), each with its four flips."""
        if self._edge is None:
            prog, out, seen = self.prog, [], set()
            for k in range(PORTRAIT_FIGHTERS):
                a = struct.unpack_from('>I', prog, PORTRAITS + 0x14 * k)[0]
                rec = compose.record_cells(prog, a, 0x10000)
                if rec is None or rec[0] != 4:
                    continue
                for code, *_ in rec[1]:
                    if code in seen:
                        continue
                    seen.add(code)
                    t = compose.tile_pixels(self.gfx, code, 0)
                    if t is None or not any(t):
                        continue
                    for f in range(4):
                        px = flip_px(t, f)
                        out.append((code, f, int.from_bytes(px, 'big'), sum(1 for v in px if v)))
            self._edge = out
        return self._edge

    def edge(self, tile):
        """(code, flip) of the portrait tile that the PS1's clipped `tile` comes from, or None: the tile's pixels equal the candidate's wherever the tile has one, and
        the candidate has more; of several the one with the fewest pixels more, then the lowest code."""
        if tile in self._edge_cache:
            return self._edge_cache[tile]
        px = unpack(tile)
        n = sum(1 for v in px if v)
        best = None
        if n >= MIN_EDGE_PIXELS:
            t = int.from_bytes(px, 'big')
            mask = int.from_bytes(bytes(255 if v else 0 for v in px), 'big')
            for code, f, cand, count in self.edge_candidates():
                if count > n and (best is None or count < best[0]) and (t ^ cand) & mask == 0:
                    best = (count, code, f)
        result = None if best is None else (best[1], best[2])
        self._edge_cache[tile] = result
        return result


class Recorder(Windows):
    """For --find: a finder that finds nothing and records the tiles it was asked for, with a label of where they came from."""

    def __init__(self, prog, gfx):
        super().__init__(prog, gfx, ())
        self.tiles = collections.defaultdict(set)      # tile -> labels
        self.label = None

    def find(self, tile):
        self.tiles[tile].add(self.label)
        return None


# ---------------------------------------------------------------- --find: the frames where the tiles lie

def build_atlas(gfx):
    """The graphics as one picture 256 pixels wide (16 tiles a row, tile code c at column c % 16, row c // 16), one byte a pixel."""
    lo, hi = bytes(i & 15 for i in range(256)), bytes(i >> 4 for i in range(256))
    import tilepack
    rows = len(gfx) // 128 // 16
    atlas = bytearray(rows * 16 * 256)
    for r in range(rows):
        for c in range(16):
            t = tilepack.arcade_tile(gfx, r * 16 + c)
            px = bytearray(256)
            px[0::2], px[1::2] = t.translate(lo), t.translate(hi)
            for y in range(16):
                o = (r * 16 + y) * 256 + c * 16
                atlas[o:o + 16] = px[y * 16:y * 16 + 16]
    return bytes(atlas)


def atlas_codes(atlas, tile):
    """The arcade tile codes round the places of the atlas where the tile lies (any flip; a tile of the odd or even rows only is looked for by those rows), or []."""
    px = unpack(tile)
    out = []
    for f in range(4):
        cut = flip_px(px, f)
        rows = [cut[y * 16:y * 16 + 16] for y in range(16)]
        for kind in row_kinds(cut):
            kept = ROWS_KEPT[kind]
            best = max(kept, key=lambda y: len(set(rows[y]) - {0}))
            if len(set(rows[best]) - {0}) < MIN_ROW_COLOURS + 1:
                continue
            pos = atlas.find(rows[best])
            n = 0
            while pos >= 0 and n < 3000:
                n += 1
                x, yy = pos % 256, pos // 256
                top = yy - best
                if x <= 240 and top >= 0 and all(atlas[(top + y) * 256 + x:(top + y) * 256 + x + 16] == rows[y] for y in kept):
                    c = (top // 16) * 16 + x // 16
                    out += [c, c + 1, c + 16, c + 17]
                    break
                pos = atlas.find(rows[best], pos + 1)
    return out


def record_addresses(prog):
    """The sorted addresses of the arcade's art records of modes 0, 2, 4 and 10."""
    import fighters
    view = fighters.View(prog)
    index = fighters.Index(view)
    addrs = {a for v in index.by_cells.values() for a in v}
    for m in re.finditer(b'(?=\x00[\x04\x0a])', prog):
        if not m.start() & 1:
            addrs.add(m.start())
    return sorted(addrs)


def candidate_sources(prog, gfx, hits, addrs):
    """Counter of candidate sources (kind, address, bank, blocks) -> the number of distinct tile codes of the hits that they draw."""
    by_low = collections.defaultdict(set)              # a record's tile code without its bank -> the records
    for a in addrs:
        rec = compose.record_cells(prog, a, 0)
        if rec is None:
            continue
        for low in {c[0] & 0xffff for c in rec[1]}:
            by_low[low].add(a)
    blocks_of = collections.defaultdict(set)           # a scroll 2 cell's tile code -> blocks
    for b in range(SL.SCROLL_BLOCKS):
        for c in range(16):
            for r in range(16):
                cell = compose.scroll2_cell(prog, b, c, r)
                if cell:
                    blocks_of[cell[0]].add(b)
    out = collections.defaultdict(set)
    for code in hits:
        for a in by_low.get(code & 0xffff, ()):
            out[(SRC_RECORD, a, code & ~0xffff, 0)].add(code)
        for b in blocks_of.get(code, ()):
            for first, n in ((b, 1), (b, 2), (b - 1, 2)):
                if first >= 0:
                    out[(SRC_SCROLL2, first, 0, n)].add(code)
    return out


def covers_of(prog, gfx, src, pool):
    """The tiles of `pool` that lie in the frame `src`."""
    canvas = draw(prog, gfx, src)
    if not canvas or not canvas[2]:
        return set()
    w = Windows(prog, gfx, [src])
    w._frames[0] = Frame(canvas)
    return {t for t in pool if w.find(t)}


def find_sources(prog, gfx, tiles, groups, log=print):
    """The frames, chosen one after another by the tiles each adds, that hold the pixels of `tiles` (a list of 128-byte tiles); `groups` maps a tile to a set of
    labels (the segments it came from): a frame is tried only on the tiles of the groups of the tiles that hint at it. Then the records near a chosen one (a pose's
    frames lie one after another, and some have no whole tile in the graphics picture to hint at them) are tried on the tiles that are left in the groups of the
    covered ones. Returns (sources, covered tiles)."""
    import bisect
    t0 = time.perf_counter()
    atlas = build_atlas(gfx)
    hint = {}                                           # tile -> codes
    for t in tiles:
        c = atlas_codes(atlas, t)
        if c:
            hint[t] = c
    log('%d of %d tiles lie whole in the graphics picture (%.0f s)' % (len(hint), len(tiles), time.perf_counter() - t0))
    addrs = record_addresses(prog)
    cands = candidate_sources(prog, gfx, {c for cs in hint.values() for c in cs}, addrs)
    log('%d candidate frames' % len(cands))
    by_label = collections.defaultdict(list)
    for t in tiles:
        for lab in groups[t]:
            by_label[lab].append(t)
    ranked = sorted(cands, key=lambda s: (-len(cands[s]), s))[:600]
    covers = {}
    for src in ranked:
        labels = {lab for t in tiles if t in hint and any(c in cands[src] for c in hint[t]) for lab in groups[t]}
        covers[src] = covers_of(prog, gfx, src, {t for lab in labels for t in by_label[lab]})
    tried = set(covers)
    log('frames tried: %d (%.0f s)' % (len(covers), time.perf_counter() - t0))
    chosen, covered = [], set()

    def choose():
        while True:
            src = max(covers, key=lambda s: (len(covers[s] - covered), s), default=None)
            if src is None or not covers[src] - covered:
                return
            chosen.append(src)
            covered.update(covers[src])
            del covers[src]

    choose()
    while True:
        seen_labels = {lab for t in covered for lab in groups[t]}
        pool = [t for t in tiles if t not in covered and groups[t] & seen_labels]
        new = set()
        for src in chosen:
            if src[0] != SRC_RECORD:
                continue
            i = bisect.bisect_left(addrs, src[1] - NEIGHBOURS)
            while i < len(addrs) and addrs[i] <= src[1] + NEIGHBOURS:
                c = (SRC_RECORD, addrs[i], src[2], 0)
                if c not in tried:
                    new.add(c)
                i += 1
        if not new or not pool:
            break
        gained = 0
        for src in sorted(new):
            tried.add(src)
            got = covers_of(prog, gfx, src, pool)
            if got:
                covers[src] = got
                gained += 1
        log('neighbours: %d tried, %d hold tiles (%.0f s)' % (len(new), gained, time.perf_counter() - t0))
        before = len(chosen)
        choose()
        if len(chosen) == before:
            break
    return chosen, covered


# ---------------------------------------------------------------- the command line

def write_sources(sources, path=None):
    path = Path(path) if path else HERE / 'screen_windows.py'
    text = path.read_text()
    body = 'SOURCES = (\n' + ''.join('    (%d, %#x, %#x, %d),\n' % s for s in sources) + ')\n'
    new = re.sub(r'(# BEGIN SOURCES[^\n]*\n).*?(# END SOURCES)', lambda m: m.group(1) + body + m.group(2), text, flags=re.S)
    path.write_text(new)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--find', action='store_true', help='find the frames that the disc tiles are cut from')
    ap.add_argument('--write', action='store_true', help='write SOURCES into this file')
    args = ap.parse_args(argv)
    if not args.find:
        ap.error('--find is the only command')
    import other_files as of
    import screens
    import stages
    import collections as C
    tmp = tempfile.mkdtemp(prefix='screen-windows-')
    try:
        ctx = of.Context(args.arcade, tmp)
        env = screens.Env(tmp, ctx.prog, ctx.gfx)
        rec = Recorder(ctx.prog, ctx.gfx)
        env.windows = rec
        with open(args.disc, 'rb') as f:
            files, sec = stages.iso_files(f)
            import pac
            disc = {}
            for path in sorted(files):
                if not path.upper().endswith('.PAC'):
                    continue
                k = of.kind_of(path.rsplit('/', 1)[1])
                if k not in of.KIND_ORDER:
                    continue
                lba, size = files[path]
                data = sec(lba, (size + 2047) // 2048)[:size]
                seen = C.Counter()
                chunks = []
                for c in pac.chunk_table(data):
                    chunks.append((c['table'], c['slot'], seen[(c['table'], c['slot'])], data[c['offset']:c['offset'] + c['size']]))
                    seen[(c['table'], c['slot'])] += 1
                disc[path.rsplit('/', 1)[1]] = (k, chunks)
        screens.collect(disc, env, of, label=lambda lab: setattr(rec, 'label', lab), check=False)     # nothing is found yet: the by-design counts mean nothing here
        tiles = sorted(rec.tiles)
        print('%d distinct tiles are in no run or piece of the arcade' % len(tiles))
        chosen, covered = find_sources(ctx.prog, ctx.gfx, tiles, rec.tiles)
        print('%d frames cover %d of %d tiles' % (len(chosen), len(covered), len(tiles)))
        for s in chosen:
            print('    (%d, %#x, %#x, %d),' % s)
        if args.write:
            write_sources(chosen)
            print('written to', HERE / 'screen_windows.py')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
