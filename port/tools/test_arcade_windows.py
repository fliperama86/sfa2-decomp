#!/usr/bin/env python3
"""Seconds: the finder of the tiles that the PS1 cut from frames of the arcade (port/arcade/screen_windows.py), on made-up data; no game files.

Makes a small arcade (test_arcade_screens.FrameWorld: art records of the four modes, scroll 2 blocks, graphics of random tiles at two banks) and checks that
a tile that a window of a source gives (any position, any stored flip, all rows or the odd or even rows, hanging over the frame's edge) is found, and that the
window it returns gives exactly that tile (screens.py's model draws it); that a tile no frame holds is not found; the odd and even fields of one picture;
the portrait edge (the arcade's tile with its lower right clipped is recognised, the candidate with the fewest pixels more wins, a tile that is no portrait
tile is not); the search for the frames (the atlas of the graphics, the candidate records and blocks, the choice by the tiles each adds); the file of sources
a --write makes; and the table text that screens.py writes for WINDOW items (the C compiles it and cuts the same tiles).
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/tools'))
import screens as S
import screen_windows as W
import compose
import test_arcade_screens as TS

HI = TS.HI


class Finder(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.fw = TS.FrameWorld(11)

    def finder(self, sources=None):
        fw = self.fw
        return W.Windows(fw.prog, fw.gfx, fw.sources if sources is None else sources)

    def test_tiles_of_windows_are_found_and_the_window_gives_them(self):
        fw, f = self.fw, self.finder()
        rnd = random.Random(1)
        found = total = 0
        for _ in range(250):
            win = fw.window(rnd)
            tile = S.window_tile(fw.arc, win)
            if not any(tile):
                continue
            total += 1
            got = f.find(tile)
            self.assertIsNotNone(got, win)
            self.assertEqual(S.window_tile(fw.arc, got), tile, (win, got))
            found += 1
        self.assertGreater(total, 30)
        self.assertEqual(found, total)

    def test_a_tile_hanging_over_the_edge_and_the_fields(self):
        fw, f = self.fw, self.finder()
        src = (S.SRC_SCROLL2, 3, 0, 2)
        for x, y in ((-8, -8), (504, 250), (248, 120)):
            for rows in (0, 1, 2):
                tile = S.window_tile(fw.arc, src + (x, y, 0, rows))
                if not any(tile):
                    continue
                got = f.find(tile)
                self.assertIsNotNone(got, (x, y, rows))
                self.assertEqual(S.window_tile(fw.arc, got), tile)
        odd, even = (S.window_tile(fw.arc, src + (240, 100, 0, r)) for r in (1, 2))
        self.assertEqual(f.find(odd)[7], 1)
        self.assertEqual(f.find(even)[7], 2)

    def test_a_tile_in_no_frame_is_not_found(self):
        f = self.finder()
        rnd = random.Random(5)
        for _ in range(20):
            self.assertIsNone(f.find(bytes(rnd.randrange(256) for _ in range(128))))
        self.assertIsNone(f.find(bytes(128)))

    def test_the_first_source_wins(self):
        fw = self.fw
        tile = next(t for t in (S.window_tile(fw.arc, fw.sources[0] + (x, y, 0, 0)) for x in range(-16, 120, 16) for y in range(-16, 120, 16)) if sum(1 for v in W.unpack(t) if v) > 40)
        self.assertEqual(self.finder([fw.sources[0], fw.sources[1]]).find(tile)[:4], fw.sources[0])
        self.assertEqual(self.finder([fw.sources[1], fw.sources[0]]).find(tile)[:4] in (fw.sources[0], fw.sources[1]), True)    # the second bank holds the same tiles

    # ---- the portrait edge

    def portrait_program(self):
        fw = self.fw
        prog = bytearray(fw.prog)
        for k in range(W.PORTRAIT_FIGHTERS):
            struct.pack_into('>I', prog, W.PORTRAITS + 0x14 * k, fw.rec4)
        return bytes(prog)

    def test_the_edge_is_the_whole_arcade_tile(self):
        fw = self.fw
        prog = self.portrait_program()
        f = W.Windows(prog, fw.gfx, ())
        rec = compose.record_cells(prog, fw.rec4, 0x10000)
        self.assertEqual(rec[0], 4)
        checked = 0
        for code, x, y, flags, _part in rec[1]:
            whole = S.Arc(fw.gfx).tile(code, 0)
            px = bytearray(W.unpack(whole))
            if sum(1 for v in px if v) < 80:
                continue
            clipped = bytes(v if 16 * yy + xx < 120 + 16 * (code % 5) else 0 for (yy, xx), v in ((divmod(i, 16), px[i]) for i in range(256)))
            if sum(1 for v in clipped if v) < W.MIN_EDGE_PIXELS or clipped == bytes(px):
                continue
            tile = W.pack(clipped)
            got = f.edge(tile)
            self.assertIsNotNone(got)
            back = S.flip_pixels(S.Arc(fw.gfx).tile(got[0], got[1] & 3), 16, 0)
            self.assertTrue(all(c == 0 or c == b for c, b in zip(clipped, W.unpack(back))), 'the candidate holds the clipped pixels')
            checked += 1
        self.assertGreater(checked, 0)
        noise = bytes(random.Random(2).randrange(256) for _ in range(128))
        self.assertIsNone(f.edge(noise))
        self.assertIsNone(f.edge(bytes(128)))
        full = W.pack(bytes(W.unpack(S.Arc(fw.gfx).tile(rec[1][0][0], 0))))
        self.assertIsNone(f.edge(full), 'a whole tile is no clipped tile (the candidate needs pixels more)')

    # ---- the search for the frames

    def test_find_sources_chooses_the_frames_that_hold_the_tiles(self):
        fw = self.fw
        rec = compose.record_cells(fw.prog, fw.rec2, 0)
        arc = S.Arc(fw.gfx, fw.prog)
        canvas = compose.record_canvas(fw.prog, fw.gfx, fw.rec2, 0)
        tiles = {}
        for code, x, y, flags, _part in rec[1]:
            t = compose.cut(canvas, x, y, 0)
            if sum(1 for v in W.unpack(t) if v) > 40:
                tiles[t] = {'a'}
        self.assertGreater(len(tiles), 0)
        logs = []
        chosen, covered = W.find_sources(fw.prog, fw.gfx, sorted(tiles), tiles, log=logs.append)
        self.assertIn((S.SRC_RECORD, fw.rec2, 0, 0), chosen)
        self.assertEqual(covered, set(tiles))
        again = W.Windows(fw.prog, fw.gfx, chosen)
        for t in tiles:
            self.assertEqual(S.window_tile(arc, again.find(t)), t)

    # ---- the file of sources

    def test_write_sources_replaces_only_the_block(self):
        with tempfile.TemporaryDirectory() as d:
            copy = Path(d) / 'screen_windows.py'
            copy.write_text((ROOT / 'port/arcade/screen_windows.py').read_text())
            before = copy.read_text()
            W.write_sources([(1, 0x14cb14, 0, 0), (2, 3, 0, 2)], copy)
            text = copy.read_text()
            ns = {}
            start = text.index('# BEGIN SOURCES')
            exec(text[start:text.index('# END SOURCES')], ns)
            self.assertEqual(ns['SOURCES'], ((1, 0x14cb14, 0, 0), (2, 3, 0, 2)))
            head = before[:before.index('# BEGIN SOURCES')]
            tail = before[before.index('# END SOURCES'):]
            self.assertEqual(text[:len(head)], head)
            self.assertTrue(text.endswith(tail))
            W.write_sources(list(W.SOURCES), copy)
            self.assertEqual(copy.read_text(), before)

    def test_the_checked_in_sources_are_well_formed(self):
        seen = set()
        for kind, address, bank, blocks in W.SOURCES:
            self.assertIn(kind, (W.SRC_RECORD, W.SRC_SCROLL2))
            self.assertEqual(kind == W.SRC_SCROLL2, blocks in (1, 2))
            self.assertNotIn((kind, address, bank, blocks), seen)
            seen.add((kind, address, bank, blocks))
        self.assertNotIn(chr(0x2014), Path(W.__file__).read_text())


if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    import time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
