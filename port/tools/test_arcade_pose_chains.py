#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/pose_chains.py on made-up frames; no game files.

Checks the grid cut (cells of a canvas on a 16-pixel grid of any phase, in row order, blank windows left out), the numbering (a new class a new number,
a flipped copy the old number with the flip bits, the numbers that a seed gives), the list a frame builds (fighters.parse_list and screen_lists.disc_cells
read the bytes back to the cells that were cut), the frame lookup by pixel counts (an offset, a frame that is drawn twice, a frame that is not there), and
the nearest position base (the one that ends at the chain, where a lower one fits too).
"""
from pathlib import Path
import random, struct, sys, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import pose_chains as PC
import fighters as F
import screen_lists as SL


def canvas_of(points):
    """A compose.py canvas (x0, y0, w, h, pixels) of {(x, y): value}."""
    xs, ys = [k[0] for k in points], [k[1] for k in points]
    x0, y0 = min(xs), min(ys)
    w, h = max(xs) - x0 + 1, max(ys) - y0 + 1
    px = bytearray(w * h)
    for (x, y), v in points.items():
        px[(y - y0) * w + x - x0] = v
    return (x0, y0, w, h, bytes(px))


def blob(rnd, x0, y0, w, h):
    return {(x, y): rnd.randrange(1, 16) for x in range(x0, x0 + w) for y in range(y0, y0 + h)}


class Cut(unittest.TestCase):
    def test_cells_follow_the_grid_in_row_order_and_skip_blank_windows(self):
        rnd = random.Random(1)
        pts = {**blob(rnd, -20, -20, 12, 10), **blob(rnd, 20, 5, 6, 6), **blob(rnd, -20, 30, 5, 5)}
        cells = PC.grid_cells(canvas_of(pts))
        got = [(x, y) for x, y, _t in cells]
        self.assertEqual(got, sorted(got, key=lambda c: (c[1], c[0])))
        self.assertTrue(all(x % 16 == 0 and y % 16 == 0 for x, y in got))
        want = {(x // 16 * 16, y // 16 * 16) for x, y in pts}
        self.assertEqual(set(got), want)              # a window with a pixel is a cell, the gap in the middle is not
        self.assertNotIn((0, -16), got)
        for x, y, t in cells:                         # every pixel is in exactly one window, with its value
            px = PC.nibbles(t)
            for ty in range(16):
                for tx in range(16):
                    self.assertEqual(px[ty * 16 + tx], pts.get((x + tx, y + ty), 0))

    def test_a_phase_moves_the_grid(self):
        rnd = random.Random(2)
        pts = blob(rnd, 3, 5, 20, 20)
        for qx, qy in ((0, 0), (15, 2), (7, 9)):
            cells = PC.grid_cells(canvas_of(pts), qx, qy)
            self.assertTrue(all(x % 16 == qx and y % 16 == qy for x, y, _t in cells), (qx, qy))
            self.assertEqual(sum(bin(sum(PC.nibbles(t))).count('1') > 0 for _x, _y, t in cells), len(cells))
            total = {}
            for x, y, t in cells:
                px = PC.nibbles(t)
                for i, v in enumerate(px):
                    if v:
                        total[(x + i % 16, y + i // 16)] = v
            self.assertEqual(total, pts)

    def test_moving_a_frame_moves_the_cells(self):
        rnd = random.Random(3)
        c = canvas_of(blob(rnd, 0, 0, 20, 20))
        a = PC.grid_cells(PC.move(c, 32, -16))
        b = PC.grid_cells(c)
        self.assertEqual([(x - 32, y + 16, t) for x, y, t in a], b)


class Numbers(unittest.TestCase):
    def tile(self, seed):
        rnd = random.Random(seed)
        return bytes(rnd.randrange(1, 16) for _ in range(256))

    def test_a_class_is_one_number_and_a_flip_is_a_flip_bit(self):
        n = PC.Numbering()
        a, b = self.tile(1), self.tile(2)
        self.assertEqual(n.number(a), (1, 0))
        self.assertEqual(n.number(b), (2, 0))
        self.assertEqual(n.number(a), (1, 0))
        for f in (1, 2, 3):
            got, g = n.number(SL.flip_pixels(a, f))
            self.assertEqual(got, 1)
            self.assertEqual(SL.flip_pixels(a, g), SL.flip_pixels(a, f))     # the bits turn the stored tile into the window
        sym = bytes(v for y in range(16) for v in [y % 5] * 16)               # the same across: a flip across is no flip
        self.assertEqual(PC.Numbering().number(sym), (1, 0))
        m = PC.Numbering()
        m.number(sym)
        self.assertEqual(m.number(SL.flip_pixels(sym, 1)), (1, 0))

    def test_seeded_numbers_continue(self):
        n = PC.Numbering()
        n.seed(7, self.tile(5))
        n.seed(9, self.tile(6))
        self.assertEqual(n.number(self.tile(6)), (9, 0))
        self.assertEqual(n.number(self.tile(8)), (10, 0))
        self.assertEqual(n.number(self.tile(5)), (7, 0))


class Lists(unittest.TestCase):
    def test_a_built_list_reads_back_as_the_cells_that_were_cut(self):
        rnd = random.Random(4)
        pts = {**blob(rnd, -30, -40, 40, 12), **blob(rnd, -10, -20, 20, 40)}
        canvas = canvas_of(pts)
        for ox, oy, qx, qy in ((0, 0, 0, 0), (16, 0, 0, 0), (-14, -1, 0, 0), (15, 2, 15, 2)):
            num = PC.Numbering()
            stream_at = 6                                           # the position runs lie in front of the list, as in a module chunk
            data, stream, cells = PC.build_list(canvas, ox, oy, qx, qy, num, 26, 0)
            mod = bytes(stream_at) + stream + data
            at = stream_at + len(stream)
            lst = F.parse_list(mod, at)
            self.assertIsNotNone(lst)
            self.assertEqual((lst['n'], lst['end']), (len(cells), len(mod)))
            read = SL.disc_cells(mod, stream_at, lst)
            self.assertEqual([(x, y) for _n, _f, x, y in read], cells)
            self.assertTrue(all(f & 0x1f == 26 for _n, f, _x, _y in read))
            self.assertEqual(sorted({n for n, _f, _x, _y in read}), list(range(1, num.next)))     # numbers by first use, no gap

    def test_two_lists_share_numbers_by_class(self):
        rnd = random.Random(5)
        c = canvas_of(blob(rnd, 0, 0, 33, 33))
        num = PC.Numbering()
        PC.build_list(c, 0, 0, 0, 0, num, 0, 0)
        first = num.next
        PC.build_list(c, 0, 0, 0, 0, num, 0, 0)                     # the same frame again: nothing new
        self.assertEqual(num.next, first)


class Layout(unittest.TestCase):
    def test_layout_rule_accepts_first_use_order_and_refuses_another(self):
        rnd = random.Random(8)
        a, b = canvas_of(blob(rnd, 0, 0, 33, 17)), canvas_of(blob(rnd, 0, 0, 17, 33))
        stream_a = PC.build_list(a, 0, 0, 0, 0, PC.Numbering(), 0, 0)[1]
        stream_b = PC.build_list(b, 0, 0, 0, 0, PC.Numbering(), 0, 0)[1]
        self.assertNotEqual(stream_a, stream_b)
        for order, ok in (((stream_a, stream_b, stream_a), True), ((stream_b, stream_a), False)):
            positions = stream_a + stream_b
            offs = {stream_a: 0, stream_b: len(stream_a)}
            lists = b''.join(PC.build_list(a if s_ is stream_a else b, 0, 0, 0, 0, PC.Numbering(), 0, offs[s_])[0] for s_ in order)
            mod = positions + lists
            chain = PC.Chain.__new__(PC.Chain)
            chain.mod, chain.positions = mod, 0
            chain.lists = F.chain_from(mod, len(positions))
            chain.cells = []
            self.assertEqual(len(chain.lists), len(order))
            self.assertEqual(PC.layout_ok(chain), ok)


class Lookup(unittest.TestCase):
    def frames(self, canvases):
        f = PC.Frames.__new__(PC.Frames)
        f.canvases, f.index = {}, {}
        for k, (key, c) in enumerate(canvases):
            f.canvases[key] = c
            h = [0] * 16
            for v in c[4]:
                h[v] += 1
            h[0] = 0
            f.index.setdefault(tuple(h), []).append(key)
        return f

    def test_a_frame_is_found_at_its_offset_and_a_twin_is_listed(self):
        rnd = random.Random(6)
        pts = blob(rnd, 5, 7, 30, 20)
        other = blob(rnd, 0, 0, 30, 20)
        f = self.frames([((0x100, 0), canvas_of(pts)), ((0x200, 0x10000), canvas_of(other)), ((0x300, 0), canvas_of({(x + 3, y - 4): v for (x, y), v in pts.items()}))])
        moved = {(x + 40, y - 9): v for (x, y), v in pts.items()}
        found = f.find(moved)
        self.assertEqual(sorted((a, b, ox, oy) for a, b, ox, oy in found), [(0x100, 0, 40, -9), (0x300, 0, 37, -5)])
        self.assertEqual(f.find(blob(random.Random(7), 0, 0, 30, 20)), [])
        pts2 = dict(pts)
        pts2[(5, 7)] = 1 if pts[(5, 7)] != 1 else 2                  # one pixel off: the counts differ and nothing is found
        self.assertEqual(f.find(pts2), [])


class Positions(unittest.TestCase):
    def test_the_base_nearest_the_chain_is_taken(self):
        # a one-cell list needs a stream of one run, any three words fit, so lower bases fit as well; the stream that ends at the list is the right one
        stream = struct.pack('<hhH', 0, 0, 0)
        lst = struct.pack('<5H', 1, 0, 0, 0, 0) + struct.pack('<H', 1)
        for lead in (40, 100):
            mod = bytes(lead) + stream + lst
            start = lead + len(stream)
            lists = F.chain_from(mod, start)
            self.assertEqual(len(lists), 1)
            base, end = PC.position_base(mod, start, lists)
            self.assertEqual((base, end), (lead, start))
            low = SL.position_base(mod, start, lists)
            self.assertLessEqual(low[0], base)


if __name__ == '__main__':
    import time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    if result.wasSuccessful():
        print('all %d tests passed in %.1f seconds' % (result.testsRun, took))
    else:
        print('%d of %d tests FAILED in %.1f seconds' % (len(result.failures) + len(result.errors), result.testsRun, took))
    sys.exit(0 if result.wasSuccessful() else 1)
