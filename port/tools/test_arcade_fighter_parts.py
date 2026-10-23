#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/fighter_parts.py on synthetic data; no game files.

Colour conversion (doubling, black to 0x8000, brightness ignored, arcade color 15 to PS1 color 0), the portrait palette
(bit 15), ramps, the palette layout's addresses, the tile format round trip against the intro importer's decoder, the page
classifier, and table 1's search and comparison on a made-up arcade program. Then the model of the runtime C (the palette
chunk from a descriptor, the art records, the portrait and shared pages, the derivation of their descriptors from a disc
page, the generated table's text) on a made-up program and graphics. `Arcade` below is also the fixture of the C test.
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


class Arcade:
    """A made-up arcade program (random words from 0x230000 up, so every palette address holds something) and graphics, with
    helpers that write the art records the pages are made from."""
    SIZE, GFX = 0x2a0000, 0x1400000

    def __init__(self, seed=1):
        rnd = random.Random(seed)
        self.rnd = rnd
        self.prog = bytearray(self.SIZE)
        self.prog[0x230000:] = rnd.randbytes(self.SIZE - 0x230000)
        self.gfx = rnd.randbytes(self.GFX)
        self.records = 0

    def portrait(self, fighter, blocks, at=None):
        """A mode 4 record of blocks (code, attribute, x, y) and the step pointer of `fighter` to it; returns its address."""
        at = at or 0x100000 + 0x1000 * fighter
        cells = sum((((a >> 8) & 15) + 1) * (((a >> 12) & 15) + 1) for _c, a, _x, _y in blocks)
        struct.pack_into('>HHH', self.prog, at, 4, cells, len(blocks) - 1)
        for k, (code, attr, x, y) in enumerate(blocks):
            struct.pack_into('>HHhh', self.prog, at + 6 + 8 * k, code, attr, x, y)
        struct.pack_into('>I', self.prog, fp.STEPS + fp.STEP_STRIDE * fighter, at)
        return at

    def shared(self, records):
        """The four mode 2 records in a row from 0x235658; each is a list of blocks (code, attribute, x, y)."""
        at = fp.VS_FIRST
        for blocks in records:
            idx, ptr = 4 * self.records, 0x120000 + 0x100 * self.records
            self.records += 1
            cells = sum((((a >> 8) & 15) + 1) * (((a >> 12) & 15) + 1) for _c, a, _x, _y in blocks)
            struct.pack_into('>HHHh', self.prog, at, 2, cells, len(blocks) - 1, idx)
            struct.pack_into('>I', self.prog, fp.COORD_TABLE + idx, ptr)
            for k, (code, attr, x, y) in enumerate(blocks):
                struct.pack_into('>HH', self.prog, at + 8 + 4 * k, code, attr)
                struct.pack_into('>hh', self.prog, ptr + 4 * k, x, y)
            at += 8 + 4 * len(blocks)

    def standard_shared(self):
        """Four records with cells all over the shared page, the last with cells above and below y = 0."""
        self.shared([[(0x7000, 0x0102, -32, 0), (0x7010, 0x0002, -48, 16)],
                     [(0x7020, 0x1101, 0, -32)],
                     [(0x7030, 0x0201, -64, -32), (0x7040, 0x0001, 0, 0)],
                     [(0x7050, 0x0001, -48, 16), (0x7051, 0x0501, -32, 0), (0x7060, 0x0001, 0, -16), (0x7061, 0x0001, 48, 16)]])

    def pixels(self, code):
        """The PS1 pixels of arcade tile `code`, by the intro importer's own decoder."""
        return bytes((v + 1) & 15 for v in tile(self.gfx, code, 16))

    def cell(self, page, cx, cy):
        return bytes(fp.page_cell(page, cx, cy))


class Palettes(unittest.TestCase):
    def test_ramp_index(self):
        self.assertEqual([fp.ramp_index(n) for n in (33, 35, 63, 60, 62)], [0, 1, 15, 16, 17])
        self.assertEqual([fp.ramp_index(n) for n in (0, 1, 31, 32, 34, 58, 61)], [None, None, None, None, None, None, 14])

    def desc(self, **kw):
        return {**dict(layout='flat', split=16, base=0x2a0000 - 0x2000, keep=0, ramps=None), **kw}

    def test_chunk_follows_the_descriptor(self):
        a = Arcade(2)
        chunk, kept = fp.palette_chunk(3, a.prog, self.desc())
        self.assertEqual((len(chunk), kept), (2048, 0))
        for n, addr in enumerate(fp.palette_addresses(3, 0x2a0000 - 0x2000, 'flat', 16)):
            self.assertEqual(struct.unpack_from('<16H', chunk, 32 * n), tuple(fp.ps1_palette(fp.words_of(a.prog, addr))))
        # kept palettes are zero and reported; ramps take their slots (odd 33 on, and 60 and 62)
        ramps = [None] * 18
        ramps[0], ramps[16] = 0x7a8, 0x3c
        chunk, kept = fp.palette_chunk(3, a.prog, self.desc(keep=1 << 5 | 1 << 40, ramps=ramps))
        self.assertEqual(kept, 1 << 5 | 1 << 40)
        self.assertEqual(chunk[32 * 5:32 * 6], bytes(32))
        self.assertEqual(struct.unpack_from('<16H', chunk, 32 * 33), tuple(fp.ps1_palette(fp.ramp_words(0x7a8))))
        self.assertEqual(struct.unpack_from('<16H', chunk, 32 * 60), tuple(fp.ps1_palette(fp.ramp_words(0x3c))))
        self.assertEqual(struct.unpack_from('<16H', chunk, 32 * 35), tuple(fp.ps1_palette(fp.words_of(a.prog, 0x2a0000 - 0x2000 + 32))))

    def test_an_unaddressed_or_outside_palette_is_refused(self):
        a = Arcade(3)
        with self.assertRaises(ValueError):
            fp.palette_chunk(0, a.prog, self.desc(layout='fives'))          # the fives layout has no palette 12 and up
        fp.palette_chunk(0, a.prog, self.desc(layout='fives', keep=sum(1 << n for n in (57, 59, 61, 63))))
        with self.assertRaises(ValueError):
            fp.palette_chunk(0, a.prog[:0x29c000], self.desc())             # palettes past the end of the program

    def test_derivation_round_trip(self):
        a = Arcade(4)
        # the real data is not random: give the palettes the second block's search needs (words with brightness 0)
        for off in range(0x260000, 0x2a0000, 2):
            struct.pack_into('>H', a.prog, off, a.rnd.randrange(1, 0x1000))
        ramps = [None] * 18
        ramps[3], ramps[17] = 0x7b0, 0x768
        want = self.desc(base=0x290000, keep=1 << 33 | 1 << 20, ramps=ramps)
        chunk, _ = fp.palette_chunk(5, a.prog, want)
        chunk = bytearray(chunk)
        struct.pack_into('<16H', chunk, 32 * 33, *([0] + [0x1234 + k for k in range(15)]))     # the PS1's own colors
        struct.pack_into('<16H', chunk, 32 * 20, *([0] + [0x4321 + k for k in range(15)]))
        got = fp.derive_palettes(5, bytes(chunk), a.prog)
        self.assertEqual((got['layout'], got['split'], got['base'], got['keep']), ('flat', 16, 0x290000, 1 << 33 | 1 << 20))
        self.assertEqual(got['ramps'], ramps)
        again, _ = fp.palette_chunk(5, a.prog, got)
        self.assertEqual([again[32 * n:32 * n + 32] for n in range(64) if n not in (20, 33)], [bytes(chunk[32 * n:32 * n + 32]) for n in range(64) if n not in (20, 33)])


class Pages(unittest.TestCase):
    def test_block_cells(self):
        cells = fp.block_cells(-16, 32, 0x1102, lambda d: 100 + d)       # 2 wide, 2 high
        self.assertEqual(cells, [(-16, 32, 100), (0, 32, 101), (-16, 48, 116), (0, 48, 117)])
        for bad in ((0, 0, 0x0020), (0, 0, 0x0040), (8, 0, 0x0001), (0, 4, 0x0001)):
            with self.assertRaises(ValueError):
                fp.block_cells(*bad[:2], bad[2], lambda d: d)

    def test_records_and_refusals(self):
        a = Arcade(5)
        at = a.portrait(3, [(0x0ba6, 0x0302, -96, -64), (0x0bd6, 0x0002, -80, -48)])
        cells = fp.record4_cells(a.prog, at)
        self.assertEqual(len(cells), 5)
        self.assertEqual(cells[0], (-96, -64, 0x10000 + 0xafa6))
        self.assertEqual(cells[4], (-80, -48, 0x10000 + 0xafd6))
        self.assertEqual(fp.portrait_tiles(a.prog, 3), {(0, 1): 0x1afa6, (1, 1): 0x1afa7, (2, 1): 0x1afa8, (3, 1): 0x1afa9, (1, 2): 0x1afd6})
        with self.assertRaises(ValueError):
            fp.record4_cells(a.prog, at + 2)                              # not a record
        with self.assertRaises(ValueError):
            fp.record4_cells(a.prog[:at + 10], at)                        # runs past the program
        a.portrait(4, [(0x0100, 0x0002, 200, 0)])                         # the cell is right of the page
        with self.assertRaises(ValueError):
            fp.portrait_tiles(a.prog, 4)
        a.portrait(5, [(0x0100, 0x0020, 0, 0)])                           # a flip
        with self.assertRaises(ValueError):
            fp.portrait_tiles(a.prog, 5)
        # the code wraps in 16 bits like the arcade's scroll layer
        a.portrait(6, [(0x5c00, 0x0001, 0, 0)])
        self.assertEqual(fp.portrait_tiles(a.prog, 6), {(6, 5): 0x10000})

    def test_a_portrait_page_is_placed_by_position(self):
        a = Arcade(6)
        a.standard_shared()
        a.portrait(2, [(0x0100, 0x1102, -96, -80), (0x0200, 0x0001, -48, 0), (0x0300, 0x0301, 48, 16)])
        mask = 0b10100101
        page = fp.portrait_page(a.prog, a.gfx, 2, shadow=mask)
        self.assertEqual(len(page), 0x8000)
        want = {(0, 0): 0x100, (1, 0): 0x101, (0, 1): 0x110, (1, 1): 0x111, (3, 5): 0x200, (9, 6): 0x300, (10, 6): 0x301, (11, 6): 0x302, (12, 6): 0x303}
        for (cx, cy), c in want.items():
            if cx < 12:                                                      # (12, 6) is the backdrop's column: the record has no cell there
                self.assertEqual(a.cell(page, cx, cy), a.pixels(0x10000 + ((c + 0xa400) & 0xffff)), (cx, cy))
        # the filler tile covers columns 12 to 15 of rows 0 to 9
        back = a.pixels(0x1a49e)
        self.assertTrue(all(a.cell(page, cx, cy) == back for cx in range(12, 16) for cy in range(10)))
        # the shadow cells are those of the shared page's last record at y >= 0, in record order; the mask picks them
        shadow = fp.shared_tiles(a.prog)[1]
        self.assertEqual(len(shadow), 8)
        self.assertEqual(shadow[0][0], (5, 15))                              # x -48, y 16
        self.assertEqual(shadow[1][0], (6, 14))                              # x -32, y 0, the first of six cells
        for i, ((cx, cy), code) in enumerate(shadow):
            self.assertEqual(a.cell(page, cx, cy) == a.pixels(code), bool(mask >> i & 1), i)

    def test_palette_rows_and_drop(self):
        a = Arcade(7)
        a.standard_shared()
        a.portrait(1, [(0x0100, 0x0101, -96, -80)])
        page = fp.portrait_page(a.prog, a.gfx, 1, 0, drop=(1, 0))
        self.assertEqual(a.cell(page, 1, 0), bytes(256))
        self.assertNotEqual(a.cell(page, 0, 0), bytes(256))
        for i, addr in enumerate(fp.portrait_addresses_of(1)):
            self.assertEqual(struct.unpack_from('<16H', page, (160 + i) * 128), tuple(fp.portrait_palette(fp.words_of(a.prog, addr))))
        self.assertEqual(page[178 * 128:178 * 128 + 32], bytes(32))
        self.assertEqual(fp.portrait_addresses_of(1), fp.portrait_addresses(1))

    def test_the_shared_page(self):
        a = Arcade(8)
        a.standard_shared()
        tiles, shadow = fp.shared_tiles(a.prog)
        self.assertEqual((tiles[(10, 2)], tiles[(11, 2)], tiles[(9, 3)]), (0x17000, 0x17001, 0x17010))     # first record, offset (12, 2)
        self.assertEqual((tiles[(8, 4)], tiles[(9, 4)], tiles[(8, 5)], tiles[(9, 5)]), (0x17020, 0x17021, 0x17030, 0x17031))   # second, (8, 6)
        self.assertEqual((tiles[(0, 0)], tiles[(2, 0)], tiles[(4, 2)]), (0x17030, 0x17032, 0x17040))        # third, (4, 2)
        page = fp.shared_page(a.prog, a.gfx)
        for (cx, cy), code in tiles.items():
            self.assertEqual(a.cell(page, cx, cy), a.pixels(code))
        self.assertEqual(sum(1 for cy in range(16) for cx in range(16) if any(fp.page_cell(page, cx, cy))), len(tiles))   # nothing else is drawn
        self.assertTrue(all(c[1] >= 14 for c, _code in shadow))
        a.prog[fp.VS_FIRST + 1] = 4                                                  # a record of another mode
        with self.assertRaises(ValueError):
            fp.shared_tiles(a.prog)

    def test_derive_portrait_round_trip(self):
        a = Arcade(9)
        a.standard_shared()
        a.portrait(7, [(0x0100, 0x0301, -96, -80), (0x0400, 0x0001, -64, 16)])
        shadow = fp.shared_tiles(a.prog)[1]
        for mask in (0, 5, (1 << len(shadow)) - 1):
            for drop in (None, (2, 0)):
                page = fp.portrait_page(a.prog, a.gfx, 7, mask, drop)
                got = fp.derive_portrait(a.prog, a.gfx, 7, page)
                self.assertEqual((got['mask'], got['drop'], got['equal']), (mask, drop, True), (mask, drop))
        # a page with something else on it is not reproduced; two blank cells are not one dropped cell
        page = bytearray(fp.portrait_page(a.prog, a.gfx, 7))
        page[200 * 128] = 1
        self.assertFalse(fp.derive_portrait(a.prog, a.gfx, 7, bytes(page))['equal'])
        page = fp.portrait_page(a.prog, a.gfx, 7, 0, (0, 0))
        for cy in (0,):
            put = bytearray(page)
            fp.put_cell(put, 1, cy, bytes(256))
            self.assertFalse(fp.derive_portrait(a.prog, a.gfx, 7, bytes(put))['equal'])


class CTable(unittest.TestCase):
    def test_text(self):
        d1 = dict(layout='flat', split=16, base=0x29c300, keep=0, ramps=None)
        d2 = dict(layout='fives', split=14, base=0x29ae20, keep=1 << 40 | 1, ramps=[None] * 12 + [0x38, 0x30, 0x38, 0x40, None, None])
        rows = [dict(file='PL00.PAC', fighter=0, desc=d1, palette_chunks=1, pages='PV', shadow=0x41, drop=None, page_disc=False),
                dict(file='PL02X.PAC', fighter=2, desc=d2, palette_chunks=1, pages='P', shadow=0xfff, drop=(0, 7), page_disc=False),
                dict(file='PL14.PAC', fighter=2, desc=d2, palette_chunks=2, pages='PVP', shadow=0, drop=None, page_disc=True)]
        text = fp.c_table(rows)
        self.assertNotIn(chr(0x2014), text)
        self.assertEqual(text.count('static const uint16_t ramps_'), 1)             # equal lists are written once
        self.assertIn('{"PL00.PAC", 0u, ARCADE_PARTS_FLAT, 16u, 1u, 0x29c300u, 0x0ull, NULL, "PV", 0x41u, 0u, -1, -1},', text)
        self.assertIn('{"PL02X.PAC", 2u, ARCADE_PARTS_FIVES, 14u, 1u, 0x29ae20u, 0x10000000001ull, ramps_0, "P", 0xfffu, 0u, 0, 7},', text)
        self.assertIn('"PVP", 0x0u, ARCADE_PARTS_PAGE_DISC, -1, -1}', text)
        self.assertIn('NR, NR, NR', text)


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
