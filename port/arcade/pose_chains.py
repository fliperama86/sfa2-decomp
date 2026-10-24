#!/usr/bin/env python3
"""Analysis tool and model: the PS1's pose lists as a cut of an arcade frame on a 16-pixel grid (the specification of the chain builder that is to come).

What it is. The continue screen (CONT files) and the tile library that CONT, DEMO and SELECT share show the fighters in poses. The module chunk of those
files holds for each fighter, and for each library segment, a CHAIN of cell lists: one list a pose, the lists one after another, the position runs in front.
Two kinds of list are in a chain. The first is the existing rule (screen_lists.py): the list is the cells of an arcade art record, the tiles are the arcade's
(an "arcade cut"). The second is what this module models: the PS1 drew the arcade's frame (the record's canvas, compose.py) and cut it again on the absolute
16-pixel grid of its own list coordinates, so its tiles are not the arcade's although every pixel is (screen_windows.py finds the tiles; this module derives
the list). The rule, found on the disc and checked list by list here:

  frame   the canvas of an arcade record (compose.record_canvas) and an origin (ox, oy) that moves the record's coordinates to the list's (0, 0 for most
          lists; the frame can be placed anywhere, a few lists have it a few pixels off);
  cells   the 16 x 16 windows of a grid of the list's coordinates, x = qx + 16 k and y = qy + 16 j (compose.cut takes them), that have a non-zero pixel, in
          row order (y, then x). The phase (qx, qy) is 0, 0 for nearly every list; for a few it is another (the origin's own, or the phase of the
          disc's cells, which the spec then records); it is part of the spec;
  numbers the chain's tile numbers are given by first use over its lists in order: a window whose pixels (or those flipped across or down) were met gets
          that number again, else the next number; the first use's orientation is the stored tile and a later use gets the flip bits that turn it into the
          window (g = d(window) xor d(stored), d the lowest flip that gives the canonical form, as screen_lists.pixel_class and Pool use);
  list    fighters.encode_list over the numbers and flip bits with the list's palette; the position runs are fighters.position_stream over the cells.

A list's SPEC is (record address, bank, ox, oy, qx, qy, palette): addresses and offsets, no byte of either game. `find` looks the record up from the disc's list (the
pixel values of a record's cells are counted, the candidates are drawn and compared pixel for pixel); `build` makes the list from a spec alone; `check` compares
the build with the disc's bytes. The chain's numbering continues after a list that this rule does not make: the numbers of such a list (an arcade cut, or
the few lists it cannot reproduce) are taken from the disc, so the report says which lists the rule made and which it did not, and why.

Where the chains are. The module chunk of a CONT file (table 0, slot 0x12) holds the fighters' pose chains, entry k of the table at 0x11b38 (8 bytes an entry: the
chain's first list, its position runs) for fighter k, numbering into the file's slot 0x13 segment 1; and the library chains, entry k of the table at 0x1814c,
numbering into segment k of the slot 0x14 chunk. DEMO.PAC holds the same library chains (table at 0x1097c, module slot 0xf, tile library slot 0xc).

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade), read only. It writes nothing but a temporary folder (the C helper of
screen_lists.py). It prints counts, addresses and offsets, never game bytes.

    python3 port/arcade/pose_chains.py --disc BIN --arcade ZIP [--only CONT05] [--detail] [--spec]
"""
import argparse
import collections
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
import fighters  # noqa: E402
import other_files as OF  # noqa: E402
import screen_lists as SL  # noqa: E402
import pac  # noqa: E402

GRID = 16
BANKS = (0, 0x10000, 0x20000, 0x30000)
MIN_PIXELS = 200            # a frame with fewer opaque pixels is not looked up by its pixel values (a list of a few cells)
POSE_TABLE, LIBRARY_TABLE = 0x11b38, 0x1814c         # in the CONT module chunk (slot 0x12); DEMO's library table is at 0x1097c (slot 0xf)
DEMO_LIBRARY_TABLE = 0x1097c
FIGHTERS = 21                # entries of the pose table (some chains are shared: the second sides of a few fighters)
LIBRARY_CHAINS = 19


# ---------------------------------------------------------------- pure functions (tested without game data)

def nibbles(tile):
    """The 256 pixels of a 128-byte PS1 tile (8 bytes a row, the left pixel of a pair in the low nibble)."""
    out = bytearray(256)
    for i, b in enumerate(tile):
        out[2 * i], out[2 * i + 1] = b & 15, b >> 4
    return bytes(out)


def window_pixels(tile, flags):
    """The 256 pixels of a tile drawn with list flags (0x20 across, 0x40 down)."""
    return SL.flip_pixels(nibbles(tile), (flags >> 5) & 3)


def list_canvas(tiles, cells):
    """(canvas, problem): the picture of a list's cells [(number, flags, x, y)] over the chain's `tiles` (128 bytes each), as compose.py's canvas
    (x0, y0, w, h, pixels); problem is a string when a number is outside the tiles."""
    for n, _f, _x, _y in cells:
        if not 0 <= n < len(tiles):
            return None, 'tile number %d is outside the segment of %d tiles' % (n, len(tiles))
    draw = []
    px = {}
    for n, f, x, y in cells:
        t = window_pixels(tiles[n], f)
        for ty in range(16):
            for tx in range(16):
                v = t[ty * 16 + tx]
                if v:
                    px[(x + tx, y + ty)] = v
    return px, None


def opaque_box(px):
    xs = [k[0] for k in px]
    ys = [k[1] for k in px]
    return min(xs), min(ys), max(xs), max(ys)


def canvas_points(canvas):
    """{(x, y): value} of the opaque pixels of a compose.py canvas (x0, y0, w, h, pixels)."""
    x0, y0, w, h, px = canvas
    return {(x0 + i % w, y0 + i // w): v for i, v in enumerate(px) if v}


def move(canvas, ox, oy):
    return (canvas[0] + ox, canvas[1] + oy) + tuple(canvas[2:])


def grid_cells(canvas, qx=0, qy=0):
    """[(x, y, 128 bytes)] of the non-blank windows of the 16-pixel grid of phase (qx, qy) over a canvas, in row order."""
    x0, y0, w, h, _px = canvas
    out = []
    for gy in range(y0 - (y0 - qy) % GRID, y0 + h, GRID):
        for gx in range(x0 - (x0 - qx) % GRID, x0 + w, GRID):
            t = compose.cut(canvas, gx, gy)
            if any(t):
                out.append((gx, gy, t))
    return out


class Numbering:
    """The chain's tile numbers: first use, one number a pixel class (a tile and its flipped copies), the first use's orientation stored."""

    def __init__(self):
        self.of = {}        # class key -> (number, d of the stored tile)
        self.next = 1

    def seed(self, number, pixels):
        """A number that the disc gives (a list the rule does not make): the tile stored as `pixels` (256) under that number."""
        key, d = SL.pixel_class(pixels)
        self.of.setdefault(key, (number, d))
        self.next = max(self.next, number + 1)

    def number(self, pixels):
        """(number, flip bits) of a window's 256 pixels; a new number for a class not met before."""
        key, d = SL.pixel_class(pixels)
        if key not in self.of:
            self.of[key] = (self.next, d)
            self.next += 1
        n, d0 = self.of[key]
        return n, d ^ d0


def build_list(canvas, ox, oy, qx, qy, numbering, palette, pos):
    """(list bytes, position stream, [(x, y)]) of a frame's cut: the frame moved by (ox, oy), cut on the grid of phase (qx, qy), fighters.encode_list over
    the numbered cells, the position runs of the cells."""
    cells = grid_cells(move(canvas, ox, oy), qx, qy)
    numbered = []
    for _x, _y, t in cells:
        n, g = numbering.number(nibbles(t))
        numbered.append((n, (g << 5) | palette))
    stream = fighters.position_stream(fighters.position_runs([(n, x, y) for (x, y, _t), (n, _f) in zip(cells, numbered)]))
    return fighters.encode_list(numbered, pos), stream, [(x, y) for x, y, _t in cells]


# ---------------------------------------------------------------- the frames of the arcade (analysis)

class Frames:
    """The arcade's art records as frames: an index from a frame's pixel value counts to the records that have them, and the drawn canvas of a record."""

    def __init__(self, prog, gfx, records):
        self.prog, self.gfx = prog, gfx
        self.hists = {}
        self.index = collections.defaultdict(list)
        self.canvases = {}
        for a in sorted(records):
            for bank in BANKS:
                r = compose.record_cells(prog, a, bank)
                if r is None or len(r[1]) < 4:
                    continue
                h = [0] * 16
                for code, *_ in r[1]:
                    t = self.tile_hist(code)
                    if t is None:
                        break
                    for k in range(1, 16):
                        h[k] += t[k]
                else:
                    if sum(h) >= MIN_PIXELS:
                        self.index[tuple(h)].append((a, bank))

    def tile_hist(self, code):
        if code not in self.hists:
            p = compose.tile_pixels(self.gfx, code, 0)
            if p is None:
                self.hists[code] = None
            else:
                c = collections.Counter(p)
                self.hists[code] = tuple(c.get(k, 0) for k in range(16))
        return self.hists[code]

    def canvas(self, address, bank):
        key = (address, bank)
        if key not in self.canvases:
            self.canvases[key] = compose.record_canvas(self.prog, self.gfx, address, bank)
        return self.canvases[key]

    def find(self, points):
        """[(address, bank, ox, oy)] of the records whose canvas, moved by (ox, oy), is exactly `points` {(x, y): value} (frames that are drawn
        alike are several)."""
        h = [0] * 16
        for v in points.values():
            h[v] += 1
        want = tuple(h)
        bx, by = opaque_box(points)[:2]
        out = []
        for a, bank in self.index.get(want, ()):
            c = self.canvas(a, bank)
            if c is None:
                continue
            cp = canvas_points(c)
            if len(cp) != len(points):
                continue
            cx, cy = opaque_box(cp)[:2]
            ox, oy = bx - cx, by - cy
            if all(points.get((x + ox, y + oy)) == v for (x, y), v in cp.items()):
                out.append((a, bank, ox, oy))
        return out


    def nearest(self, points):
        """(distance, address, bank) of the record whose pixel value counts are nearest to `points` (the sum of the differences of the counts)."""
        h = [0] * 16
        for v in points.values():
            h[v] += 1
        best = None
        for key, recs in self.index.items():
            d = sum(abs(a - b) for a, b in zip(h, key))
            if best is None or d < best[0]:
                best = (d, recs[0][0], recs[0][1])
        return best


# ---------------------------------------------------------------- the chains

def position_base(mod, start, lists):
    """(offset of the chain's position runs, end of its last stream) or None: the NEAREST offset to the chain from which every list's stream fits and ends
    at or before the chain. (screen_lists.position_base takes the lowest such offset; where several fit, a lower one is a different run of the same
    kind of data, the position runs lie right in front of the chain: the right one ends within a few bytes of it.)"""
    maxpos = max(l['pos'] for l in lists)
    best = None
    for base in range(max(0, start - maxpos - 6 - 0x2000) & ~1, start - maxpos - 5, 2):
        ends = SL.stream_ends(mod, base, lists)
        if ends is not False:
            end = max((e for e in ends if e), default=base)
            if end <= start:
                best = (base, end)
    return best


class Chain:
    """One chain of a module chunk: its lists with their cells, the tiles of its segment. `positions` is the offset of its position runs."""

    def __init__(self, name, mod, start, tiles):
        self.name, self.mod, self.start, self.tiles = name, mod, start, tiles
        self.lists = fighters.chain_from(mod, start)
        pb = position_base(mod, start, self.lists)
        self.positions = pb[0] if pb else None
        self.cells = [SL.disc_cells(mod, self.positions, lst) if pb else None for lst in self.lists]


def table_chains(mod, table, count, base):
    """[chain start] of entry k of the pointer table at `table` of the module chunk (the first word of an entry, minus the module's load address)."""
    return [struct.unpack_from('<I', mod, table + 8 * k)[0] - base for k in range(count)]


def classify_other(env, prog, gfx, cells, tiles):
    """Why a list is not made by the grid rule: 'empty' for one cell of tile 0, 'arcade cut' when an arcade record has these cell positions and the
    list's tiles are its tiles, else None."""
    if len(cells) == 1 and cells[0][0] == 0:
        return 'empty'
    key = tuple(sorted((x, y) for _n, _f, x, y in cells))
    for a in env.by_xy.get(key, ()):
        for bank in BANKS:
            r = compose.record_cells(prog, a, bank)
            if r is None or len(r[1]) != len(cells):
                continue
            by = {(x, y): (code, flags) for code, x, y, flags, _p in r[1]}
            ok = True
            for n, f, x, y in cells:
                code, flags = by.get((x, y), (None, 0))
                t = compose.tile_pixels(gfx, code, (flags >> 5) & 3) if code is not None else None
                if t is None or n >= len(tiles) or t != window_pixels(tiles[n], f):
                    ok = False
                    break
            if ok:
                return 'arcade cut'
    return None


def model_chain(frames, env, chain):
    """The report of one chain: a list of dicts, one a list, in order: kind ('grid' reproduced, 'grid differs', 'arcade cut', 'empty', 'no frame'), the
    spec (address, bank, ox, oy, palette) for a grid list, and a reason for what is not reproduced. The numbering continues from the disc after a list
    this rule does not make."""
    numbering = Numbering()
    out = []
    mod = chain.mod
    for i, (lst, cells) in enumerate(zip(chain.lists, chain.cells)):
        row = dict(index=i, cells=len(cells or ()))
        out.append(row)
        if not cells:
            row.update(kind='no frame', why='the list does not parse')
            continue
        canvas, why = list_canvas(chain.tiles, cells)
        if why:
            row.update(kind='no frame', why=why)
            continue
        other = classify_other(env, frames.prog, frames.gfx, cells, chain.tiles)
        points = canvas
        found = frames.find(points) if len(points) >= MIN_PIXELS else []
        seed = True
        if found and other is None:
            pals = {f & 0x1f for _n, f, _x, _y in cells}
            palette = collections.Counter(f & 0x1f for _n, f, _x, _y in cells).most_common(1)[0][0]
            disc_list = mod[lst['at']:lst['end']]
            end = fighters.stream_end(mod, chain.positions + lst['pos'], lst['n'])
            disc_stream = mod[chain.positions + lst['pos']:end] if end else b''
            first = None
            for a, bank, ox, oy in found:
                for qx, qy in dict.fromkeys(((0, 0), (ox % GRID, oy % GRID), (cells[0][2] % GRID, cells[0][3] % GRID))):
                    trial = Numbering()
                    trial.of, trial.next = dict(numbering.of), numbering.next
                    data, stream, grid = build_list(frames.canvas(a, bank), ox, oy, qx, qy, trial, palette, lst['pos'])
                    if first is None:
                        first = (a, bank, ox, oy, qx, qy, data, stream, grid)
                    if data == disc_list and stream == disc_stream and len(pals) == 1:
                        row['kind'] = 'grid'
                        row['spec'] = (a, bank, ox, oy, qx, qy, palette)
                        numbering = trial
                        seed = False
                        break
                if not seed:
                    break
            if seed:
                a, bank, ox, oy, qx, qy, data, stream, grid = first
                row['spec'] = (a, bank, ox, oy, qx, qy, palette)
                row['kind'] = 'grid differs'
                row['why'] = why_differs(grid, cells, data, disc_list, stream, disc_stream, pals)
        elif other:
            row.update(kind=other)
        elif not found:
            if len(points) < MIN_PIXELS:
                row.update(kind='no frame', why='too few pixels to look up')
            else:
                d, a, bank = frames.nearest(points)
                row.update(kind='no frame', why='no record draws these pixels at any offset; the nearest (%#x) has %s pixel value counts'
                           % (a, 'the same' if d == 0 else 'counts that differ by %d in' % d))
        else:
            row.update(kind='no frame', why='a record draws these pixels but the list is also an arcade cut')
        if seed:
            for n, f, _x, _y in cells:
                if 0 <= n < len(chain.tiles) and chain.tiles[n] != bytes(128):
                    numbering.seed(n, nibbles(chain.tiles[n]))
    return out


def why_differs(grid, cells, data, disc_list, stream, disc_stream, pals):
    disc_cells = [(x, y) for _n, _f, x, y in cells]
    if grid != disc_cells:
        return 'the cells differ (%d cut, %d on the disc)' % (len(grid), len(disc_cells))
    if len(pals) != 1:
        return 'the list has %d different palettes' % len(pals)
    if stream != disc_stream:
        return 'the position runs differ'
    if len(data) == len(disc_list) and data[:2] == disc_list[:2]:
        return 'the numbers or flips differ'
    return 'the encoding differs (mode)'


def layout_ok(chain):
    """True when the chain's position runs are its lists' streams stored once each in order of first use, the first list's offset 0, and the runs end where
    the chain's last stream ends (the rule the builder assembles a chain by; an empty list, one cell of tile 0, has the size of the runs so far)."""
    seen, size = {}, 0
    for lst in chain.lists:
        if lst['n'] == 1 and lst['cells'] == [(0, None)]:
            if lst['pos'] != size:
                return False
            continue
        end = fighters.stream_end(chain.mod, chain.positions + lst['pos'], lst['n'])
        if end is None:
            return False
        stream = chain.mod[chain.positions + lst['pos']:end]
        if stream not in seen:
            if lst['pos'] != size:
                return False
            seen[stream] = lst['pos']
            size += len(stream)
        elif seen[stream] != lst['pos']:
            return False
    return True


def summarize(rows):
    c = collections.Counter(r['kind'] for r in rows)
    return c


def report_line(name, label, rows, detail, layout=None):
    c = summarize(rows)
    parts = ['%d lists: %d made by the grid rule' % (len(rows), c['grid'])]
    for k in ('arcade cut', 'empty', 'grid differs', 'no frame'):
        if c[k]:
            parts.append('%d %s' % (c[k], k))
    if layout is not None:
        parts.append('position runs %s' % ('laid out by the rule' if layout else 'NOT laid out by the rule'))
    print('%s %s: %s' % (name, label, ', '.join(parts)))
    if detail:
        for r in rows:
            if r['kind'] in ('grid differs', 'no frame') or detail > 1:
                print('    list %2d %-12s cells %3d %s %s' % (r['index'], r['kind'], r['cells'], r.get('spec', ''), r.get('why', '')))
    return c


# ---------------------------------------------------------------- the disc

def read_file(files, sec, name):
    lba, size = files['/PAC/' + name]
    data = sec(lba, (size + 2047) // 2048)[:size]
    chunks, seen = {}, collections.Counter()
    for c in pac.chunk_table(data):
        key = (c['table'], c['slot'])
        chunks[key + (seen[key],)] = data[c['offset']:c['offset'] + c['size']]
        seen[key] += 1
    return chunks


def segment_tiles(chunk, k):
    segs = OF.tile_segments(chunk)
    return OF.segment_tiles(chunk, [segs[k]]) if k < len(segs) else []


def run(args):
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='pose-chains-')
    try:
        prog, gfx = fighters.arcade_views(args.arcade)
        env = SL.Env(prog, gfx, tmp)
        records = set()
        for v in env.by_xy.values():
            records.update(v)
        frames = Frames(prog, gfx, records)
        print('%d frames indexed by pixel counts in %.1f s' % (sum(len(v) for v in frames.index.values()), time.monotonic() - started))
        with open(args.disc, 'rb') as f:
            files, sec = fighters.iso_files(f)
            total = collections.Counter()
            jobs = []
            for k in range(FIGHTERS):
                jobs.append(('CONT%02X.PAC' % k, 'pose %d' % k, k))
            jobs.append(('CONT00.PAC', 'library', None))
            jobs.append(('DEMO.PAC', 'library', None))
            for name, label, k in jobs:
                if args.only and not name.startswith(args.only.upper()):
                    continue
                chunks = read_file(files, sec, name)
                if name == 'DEMO.PAC':
                    mod, lib, base, table = chunks[(0, 0xf, 0)], chunks[(0, 0xc, 0)], 0x800df000, DEMO_LIBRARY_TABLE
                else:
                    mod, base = chunks[(0, 0x12, 0)], 0x80010000
                    lib, table = chunks[(0, 0x14, 0)], LIBRARY_TABLE
                if k is not None:
                    starts = table_chains(mod, POSE_TABLE, FIGHTERS, base)
                    tiles = segment_tiles(chunks[(0, 0x13, 0)], 1)
                    chain = Chain(name, mod, starts[k], tiles)
                    rows = model_chain(frames, env, chain)
                    total.update(report_line(name, label + ' (chain %#x)' % chain.start, rows, args.detail, layout_ok(chain)))
                    if args.spec:
                        for r in rows:
                            if 'spec' in r:
                                print('    spec', name, r['index'], '%#x %#x %d %d %d %d %d' % r['spec'])
                else:
                    starts = table_chains(mod, table, LIBRARY_CHAINS, base)
                    for j, start in enumerate(starts):
                        tiles = segment_tiles(lib, j)
                        chain = Chain(name, mod, start, tiles)
                        rows = model_chain(frames, env, chain)
                        total.update(report_line(name, '%s %d (chain %#x)' % (label, j, start), rows, args.detail, layout_ok(chain)))
                        if args.spec:
                            for r in rows:
                                if 'spec' in r:
                                    print('    spec', name, j, r['index'], '%#x %#x %d %d %d %d %d' % r['spec'])
        print('total: %s in %.1f s' % (dict(total), time.monotonic() - started))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='files whose name starts with this (CONT05, DEMO)')
    ap.add_argument('--detail', action='count', default=0, help='the lists that are not reproduced (twice: every list)')
    ap.add_argument('--spec', action='store_true', help='print the spec of every list the grid rule makes')
    return run(ap.parse_args(argv))


if __name__ == '__main__':
    sys.exit(main())
