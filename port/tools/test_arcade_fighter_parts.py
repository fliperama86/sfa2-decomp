#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/fighter_parts.py on synthetic data; no game files.

Colour conversion (doubling, black to 0x8000, brightness ignored, arcade color 15 to PS1 color 0), the portrait palette
(bit 15), ramps, the palette layout's addresses, the tile format round trip against the intro importer's decoder, the page
classifier, and table 1's search and comparison on a made-up arcade program.
"""
from pathlib import Path
import collections, random, struct, sys, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/intro'))
import fighter_parts as fp
from import_scene import tile


class Colors(unittest.TestCase):
    def test_conversion(self):
        self.assertEqual(fp.ps1_color(0x0fc9), 0x4b1e)      # R 15, G 12, B 9 -> 30, 24, 18
        self.assertEqual(fp.ps1_color(0x0111), 0x0842)
        self.assertEqual(fp.ps1_color(0x0000), 0x8000)      # black must not be the transparent 0
        self.assertEqual(fp.ps1_color(0xf111), fp.ps1_color(0x0111))   # the brightness nibble is dropped
        self.assertEqual(fp.ps1_color(0x0f00), 0x001e)
        self.assertEqual(fp.ps1_color(0x00f0), 0x03c0)
        self.assertEqual(fp.ps1_color(0x000f), 0x7800)

    def test_palette(self):
        words = list(range(0x100, 0x110))
        p = fp.ps1_palette(words)
        self.assertEqual(len(p), 16)
        self.assertEqual(p[0], 0)
        self.assertEqual(p[1:], [fp.ps1_color(w) for w in words[:15]])   # word 15 is not used
        q = fp.portrait_palette([0] * 16)
        self.assertEqual(q, [0] + [0x8000] * 15)
        self.assertEqual(fp.portrait_palette(words)[3], fp.ps1_color(words[2]) | 0x8000)

    def test_ramp(self):
        self.assertEqual(fp.ramp_words(0x7a8)[:5], [0x7a8, 0x7a9, 0x7a9, 0x7aa, 0x7aa])
        self.assertEqual(fp.ramp_words(0x7a8)[14], 0x7af)
        colors = [fp.ps1_color(w) for w in fp.ramp_words(0x7a8)]
        self.assertEqual(fp.is_ramp(colors), 0x7a8)
        self.assertIsNone(fp.is_ramp(colors[:-1] + [colors[-1] ^ 0x40]))
        self.assertEqual(fp.is_ramp([fp.ps1_color(w) for w in fp.ramp_words(0x078)]), 0x078)


class Layout(unittest.TestCase):
    def test_addresses(self):
        a = fp.palette_addresses(1, 0x29c300)
        self.assertEqual(len(a), 64)
        self.assertEqual(a[0], fp.A_BASE + fp.A_STRIDE)
        self.assertEqual(a[2], a[0] + 32)
        self.assertEqual(a[58], fp.A_BASE + fp.A_STRIDE + 29 * 32)
        self.assertEqual((a[60], a[62]), (fp.RAMP_60, fp.RAMP_62))
        self.assertEqual(a[1], fp.B_BASE + fp.B_STRIDE)
        self.assertEqual(a[31], fp.B_BASE + fp.B_STRIDE + 15 * 32)
        self.assertEqual(a[33], 0x29c300)
        self.assertEqual(a[63], 0x29c300 + 15 * 32)

    def test_fighter_numbers(self):
        self.assertEqual([fp.FIGHTER_OF[i] for i in (0, 5, 0x12)], [0, 5, 0x12])
        self.assertEqual((fp.FIGHTER_OF[0x13], fp.FIGHTER_OF[0x15], fp.FIGHTER_OF[0x16], fp.FIGHTER_OF[0x17]), (0x11, 0x0d, 0x10, 0x11))
        self.assertEqual(fp.palette_addresses(0x13, 0)[1], fp.palette_addresses(0x11, 0)[1])

    def test_split_and_fives(self):
        a = fp.palette_addresses(0, 0x100000, 'flat', 8)
        self.assertEqual(a[15], fp.B_BASE + 7 * 32)
        self.assertEqual(a[17], 0x100000)              # p = 8 is the first of the second block
        f = fp.palette_addresses(0, 0x100000, 'fives', 16)
        self.assertEqual([f[33], f[35], f[37], f[39]], [0x100000, 0x100020, 0x1000a0, 0x1000c0])
        self.assertEqual([f[n] for n in (57, 59, 61, 63)], [None] * 4)
        self.assertEqual(fp.c_offset(11, 'fives'), 32 * (25 + 1))
        self.assertIsNone(fp.c_offset(12, 'fives'))

    def test_portrait_addresses(self):
        a = fp.portrait_addresses(2)
        self.assertEqual(len(a), 18)
        self.assertEqual(a[0], 0x29c7c0 + 0x60 * 2)
        self.assertEqual(a[2], a[0] + 64)
        self.assertEqual(a[3], a[0] + 0x720)
        self.assertEqual(a[17], a[0] + 5 * 0x720 + 64)


class Tiles(unittest.TestCase):
    def test_planar_round_trip(self):
        rnd = random.Random(1)
        pix = bytes(rnd.randrange(16) for _ in range(256))
        self.assertEqual(bytes(tile(fp.planar(pix), 0, 16)), pix)    # the importer's decoder reads what planar wrote
        self.assertEqual(len(fp.planar(pix)), 128)

    def page_with(self, cells):
        """A 256 x 256 4-bit page, cells {(cx, cy): 256 pixels}."""
        page = bytearray(0x8000)
        for (cx, cy), pix in cells.items():
            for y in range(16):
                for x in range(16):
                    X, Y = cx * 16 + x, cy * 16 + y
                    i = Y * 128 + X // 2
                    page[i] |= pix[y * 16 + x] << (4 if X & 1 else 0)
        return bytes(page)

    def test_cell_and_classifier(self):
        rnd = random.Random(2)
        arc = bytes(rnd.randrange(16) for _ in range(256))          # arcade pixel indices of a tile
        ps1 = bytes((p + 1) & 15 for p in arc)
        tiles = {fp.planar(arc): 77}
        stray = bytes((rnd.randrange(1, 16)) for _ in range(256))
        page = self.page_with({(3, 2): ps1, (5, 5): stray})
        self.assertEqual(bytes(fp.page_cell(page, 3, 2)), ps1)
        self.assertEqual(fp.tile_of_cell(fp.page_cell(page, 3, 2), tiles), 77)
        grid, rows = fp.classify_page(page, tiles)
        self.assertEqual(grid[2][3], 77)
        self.assertEqual(grid[5][5], -1)
        self.assertIsNone(grid[0][0])
        self.assertEqual(rows, [])
        self.assertEqual(sum(1 for line in grid for e in line if e is None), 254)

    def test_palette_rows(self):
        page = bytearray(0x8000)
        pal = fp.portrait_palette([0x0123 + k for k in range(16)])
        for i in range(2):                                           # two palette rows at y = 160, 161
            struct.pack_into('<16H', page, (160 + i) * 128, *pal)
        grid, rows = fp.classify_page(bytes(page), {})
        self.assertEqual(rows, [160, 161])
        self.assertEqual(grid[10][0], -2)                            # the cells that hold them are taken out of the grid
        self.assertEqual(grid[10][1], -2)
        self.assertIsNone(grid[10][4])


class Table1(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        rnd = random.Random(3)
        cls.prog = bytearray(0x2a0000 + 0x400)
        for a in range(0x200000, len(cls.prog) - 32, 2):             # random palette words with a zero brightness nibble
            struct.pack_into('>H', cls.prog, a, rnd.randrange(1, 0x1000))
        cls.prog = bytes(cls.prog)

    def chunk(self, pl, x_base, layout, split, edit=()):
        addr = fp.palette_addresses(pl, x_base, layout, split)
        pals = [fp.ps1_palette(fp.words_of(self.prog, a)) if a is not None else [0] + [fp.ps1_color(w) for w in fp.ramp_words(0x7a8)] for a in addr]
        for n, pal in edit:
            pals[n] = pal
        return b''.join(struct.pack('<16H', *p) for p in pals)

    def test_all_exact_and_found(self):
        r = fp.table1(4, self.chunk(4, 0x2a0000 - 0x3000, 'flat', 16), self.prog)
        self.assertEqual((r['exact'], r['other'], r['split']), (64, 0, 16))
        self.assertEqual((r['c_base'], r['layout']), (0x2a0000 - 0x3000, 'flat'))

    def test_split_fives_and_filler(self):
        base = 0x290000
        chunk = self.chunk(2, base, 'fives', 16)
        r = fp.table1(2, chunk, self.prog)
        self.assertEqual((r['layout'], r['c_base'], r['split']), ('fives', base, 16))
        self.assertEqual((r['exact'], r['ramp7'], r['other']), (60, 4, 0))   # the 4 palettes the layout names no address for
        r = fp.table1(0, self.chunk(0, 0x280000, 'flat', 8), self.prog)
        self.assertEqual((r['split'], r['exact'], r['other']), (8, 64, 0))

    def test_a_changed_palette_is_counted(self):
        pal = list(fp.ps1_palette(fp.words_of(self.prog, fp.A_BASE + 5 * 32)))
        pal[4] ^= 0x40
        chunk = self.chunk(0, 0x290000, 'flat', 16, edit=[(10, pal)])
        r = fp.table1(0, chunk, self.prog)
        self.assertEqual((r['exact'], r['other']), (63, 1))
        self.assertEqual(r['others'][0][:2], (10, 15))

    def test_summary(self):
        t = collections.Counter(**{'t1 bytes': 100, 't1 exact': 60})
        text = fp.summary(t, 3)
        self.assertIn('totals over 3 files', text)
        self.assertNotIn(chr(0x2014), text)


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
