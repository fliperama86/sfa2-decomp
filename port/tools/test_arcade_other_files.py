#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/other_files.py on synthetic data; no game files.

File kinds from names, the TIM reader (its rectangle larger than its block), the compressed tile segment finder (against the tile
encoder of tilepack.py), palette rows and runs, the region splitter of a mixed chunk, the 16 x 16 cells, the 8 x 8 pieces and the
cell classifier (with made-up lookups), palette rows inside a picture, the XA, STR and EXE header readers, an offset table, and the
text helpers.
"""
from pathlib import Path
import collections, random, struct, sys, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import other_files as of
import tilepack


def tiles(n, seed=1):
    """n random 64-word tiles that are not blank, as lists of words."""
    rnd = random.Random(seed)
    return [[rnd.randrange(1, 65536) for _ in range(64)] for _ in range(n)]


def segment(n, seed=1):
    """(chunk bytes of a compressed tile segment of n tiles, the tiles' 128-byte forms)."""
    t = tiles(n, seed)
    chunk, _forms = tilepack.encode_chunk(t)
    return chunk, [struct.pack('<64H', *w) for w in t]


def palette(seed, black=False):
    rnd = random.Random(seed)
    return struct.pack('<16H', 0, *[0x8000 if black and i == 3 else rnd.randrange(1, 0x7fff) for i in range(15)])


class Names(unittest.TestCase):
    def test_kind_of(self):
        self.assertEqual(of.kind_of('CONT0AX.PAC'), 'CONT')
        self.assertEqual(of.kind_of('CONT00.PAC'), 'CONT')
        self.assertEqual(of.kind_of('DEMO.PAC'), 'DEMO')
        self.assertEqual(of.kind_of('SELECTA.PAC'), 'SELECTA')
        self.assertEqual(of.kind_of('SELECT.PAC'), 'SELECT')
        self.assertEqual(of.kind_of('END0E.PAC'), 'END')
        self.assertEqual(of.kind_of('CDEMO14.PAC'), 'CDEMO')
        self.assertEqual(of.kind_of('GAME_SND.PAC'), 'GAME_SND')
        self.assertEqual(of.kind_of('FIRSTSND.PAC'), 'FIRSTSND')
        self.assertEqual(of.kind_of('RDM0F.PAC'), 'RDM')
        self.assertEqual(of.kind_of('ASCII.PAC'), 'ASCII')


class Tim(unittest.TestCase):
    def make(self, flags, clut=None, image=(0, 0, 4, 2), extra=0):
        d = struct.pack('<II', 0x10, flags)
        if clut:
            x, y, w, h = clut
            d += struct.pack('<IHHHH', 12 + 2 * w * h, x, y, w, h) + bytes(2 * w * h)
        x, y, w, h = image
        return d + struct.pack('<IHHHH', 12 + 2 * w * h - extra, x, y, w, h) + bytes(2 * w * h)

    def test_four_bits_with_clut(self):
        d = self.make(8, clut=(576, 256, 16, 4), image=(384, 256, 8, 3))
        t = of.tim_header(d)
        self.assertEqual((t['bpp'], t['x'], t['y'], t['w'], t['h']), (4, 384, 256, 8, 3))
        self.assertEqual((t['clut']['x'], t['clut']['y'], t['clut']['w'], t['clut']['h']), (576, 256, 16, 4))
        self.assertEqual(t['at'], 8 + 12 + 128 + 12)
        self.assertEqual(len(d) - t['at'], 2 * 8 * 3)

    def test_eight_bits_no_clut(self):
        t = of.tim_header(self.make(1))
        self.assertEqual((t['bpp'], t['clut']), (8, None))

    def test_rectangle_may_be_larger_than_the_block(self):
        # the block says 12 bytes of image, the rectangle 4 x 2 halfwords: the reader takes the rectangle
        d = self.make(0, image=(0, 0, 4, 2), extra=16)
        t = of.tim_header(d)
        self.assertEqual(t['size'], 12)
        self.assertEqual(2 * t['w'] * t['h'], 16)

    def test_rejects(self):
        self.assertIsNone(of.tim_header(b'\x11\0\0\0' + bytes(40)))
        self.assertIsNone(of.tim_header(struct.pack('<II', 0x10, 3) + bytes(40)))      # a reserved pixel mode
        self.assertIsNone(of.tim_header(struct.pack('<II', 0x10, 0x20) + bytes(40)))
        self.assertIsNone(of.tim_header(self.make(8, clut=(0, 0, 16, 1))[:40]))        # cut short
        self.assertIsNone(of.tim_header(b'short'))


class TileSegments(unittest.TestCase):
    def test_one_segment(self):
        chunk, expect = segment(8)
        segs = of.tile_segments(chunk)
        self.assertEqual(segs, [(0, 8, (len(chunk) + 3) & ~3)])
        self.assertEqual(of.segment_tiles(chunk, segs), expect)

    def test_block_end_of_every_form(self):
        rnd = random.Random(5)
        words = [[0] * 64, [rnd.randrange(1, 65536) for _ in range(64)], [1, 2] * 32, [0] * 60 + [1, 2, 3, 4]]
        chunk, forms = tilepack.encode_chunk(words)
        offs = struct.unpack_from('<4I', chunk, 0)
        ends = [of.tile_block_end(chunk, o) for o in offs]
        self.assertEqual(ends[:3], list(offs[1:]))
        self.assertEqual(ends[3], len(chunk))
        self.assertGreaterEqual(len(set(forms)), 2)

    def test_two_segments_with_a_gap(self):
        a, ta = segment(6, 2)
        b, tb = segment(5, 3)
        d = a + bytes(8 - len(a) % 4 if len(a) % 4 else 0) + bytes(64) + b + bytes(32)
        segs = of.tile_segments(d)
        self.assertEqual(len(segs), 2)
        self.assertEqual([n for _s, n, _e in segs], [6, 5])
        self.assertEqual(of.segment_tiles(d, segs), ta + tb)

    def test_not_a_segment(self):
        rnd = random.Random(9)
        self.assertEqual(of.tile_segments(bytes(rnd.randrange(256) for _ in range(4000))), [])
        self.assertEqual(of.tile_segments(bytes(4000)), [])
        a, _t = segment(3)       # fewer tiles than the least a segment has
        self.assertEqual(of.tile_segments(a), [])


class Palettes(unittest.TestCase):
    def test_row_kind(self):
        self.assertEqual(of.palette_row_kind(palette(1)), 'p')
        self.assertEqual(of.palette_row_kind(bytes(32)), 'z')
        self.assertIsNone(of.palette_row_kind(b'\x01\x00' + palette(2)[2:]))        # colour 0 not 0
        self.assertIsNone(of.palette_row_kind(struct.pack('<16H', 0, *([0] * 10 + [1] * 5))))   # too few colours
        self.assertEqual(of.palette_row_kind(palette(3, black=True)), 'p')
        self.assertIsNone(of.palette_row_kind(bytes(10)))

    def test_run_end(self):
        rows = b''.join(palette(i) for i in range(10)) + bytes(64) + palette(20) + bytes(32 * 4) + palette(30)
        # ten palettes, two empty rows, one palette: the run goes to the 11th palette and stops at the four empty rows
        self.assertEqual(of.palette_run_end(rows, 0, len(rows)), 32 * 13)
        self.assertEqual(of.palette_run_end(rows[:32 * 5], 0, 32 * 5), 0)           # under the least run
        self.assertEqual(of.palette_run_end(bytes(400), 0, 400), 0)

    def test_normalise(self):
        row = struct.pack('<16H', 0, *([0x8001] * 14 + [0x8000]))
        out = of.normalise_palettes(row + b'xx')
        self.assertEqual(len(out), 32)
        self.assertEqual(struct.unpack('<16H', out), (0, *([1] * 14 + [0])))

    def test_kind(self):
        self.assertEqual(of.palette_kind(struct.pack('<16H', 0, *([0x1234] * 15))), 'flat')
        self.assertEqual(of.palette_kind(palette(4)), 'own')
        start = 0x0700 | 0x0070 | 0x0007
        ramp = []
        for k in range(15):
            w = start + (k + 1) // 2
            ramp.append(((w >> 8 & 15) << 1) | ((w >> 4 & 15) << 6) | ((w & 15) << 11))
        self.assertEqual(of.palette_kind(struct.pack('<16H', 0, *ramp)), 'ramp')


class Regions(unittest.TestCase):
    def test_mixed_chunk(self):
        seg, _t = segment(6, 7)
        seg += bytes(-len(seg) % 4)
        pal = b''.join(palette(i) for i in range(9))
        zero = bytes(600)
        pic = bytes(random.Random(3).randrange(1, 256) for _ in range(512))
        d = seg + pal + zero + pic + seg
        regions = of.merge_pictures(of.chunk_regions(d))
        kinds = [k for k, _a, _b in regions]
        self.assertEqual(kinds, ['tiles', 'pal', 'image', 'tiles'])      # the zero run before a picture is part of the picture
        self.assertEqual(regions[0], ('tiles', 0, len(seg)))
        self.assertEqual(regions[1], ('pal', len(seg), len(seg) + len(pal)))
        self.assertEqual(regions[2], ('image', len(seg) + len(pal), len(seg) + len(pal) + len(zero) + len(pic)))
        self.assertEqual(regions[-1][2], len(d))
        # regions tile the chunk with no gaps
        self.assertEqual(regions[0][1], 0)
        for x, y in zip(regions, regions[1:]):
            self.assertEqual(x[2], y[1])

    def test_all_zero_and_picture(self):
        self.assertEqual(of.chunk_regions(bytes(5000)), [('zero', 0, 5000)])
        pic = bytes(random.Random(4).randrange(1, 256) for _ in range(5000))
        self.assertEqual(of.merge_pictures(of.chunk_regions(pic)), [('image', 0, 5000)])

    def test_zero_gap_inside_a_picture_stays_in_it(self):
        rnd = random.Random(8)
        pic = bytes(rnd.randrange(1, 256) for _ in range(400)) + bytes(300) + bytes(rnd.randrange(1, 256) for _ in range(400))
        self.assertEqual(of.merge_pictures(of.chunk_regions(pic)), [('image', 0, len(pic))])


class Cells(unittest.TestCase):
    def picture(self, width=32, rows=32, seed=2):
        rnd = random.Random(seed)
        return bytes(rnd.randrange(1, 256) for _ in range(width // 2 * rows))

    def test_cells_of(self):
        page = self.picture()
        cells = of.cells_of(page, 32)
        self.assertEqual([(cx, cy) for cx, cy, _c in cells], [(0, 0), (1, 0), (0, 1), (1, 1)])
        self.assertTrue(all(len(c) == 128 for _x, _y, c in cells))
        # cell (1, 0): pixel row 0 starts at byte 8 of the picture's first row, row 1 at byte 16 + 8
        self.assertEqual(cells[1][2][:8], page[8:16])
        self.assertEqual(cells[1][2][8:16], page[16 + 8:16 + 16])
        self.assertEqual(cells[2][2][:8], page[16 * 16:16 * 16 + 8])
        self.assertEqual(len(of.cells_of(page[:-8], 32)), 2)       # a partial last cell row is dropped

    def test_pieces(self):
        cell = bytes(range(128))
        p = of.pieces_of(cell)
        self.assertEqual(len(p), 4)
        self.assertEqual(p[0][:4], cell[0:4])
        self.assertEqual(p[1][:4], cell[4:8])
        self.assertEqual(p[2][:4], cell[64:68])
        self.assertEqual(p[3][-4:], cell[124:128])
        self.assertEqual(b''.join(p) and len(b''.join(p)), 128)

    def test_flat(self):
        self.assertTrue(of.is_flat(bytes(32)))
        self.assertTrue(of.is_flat(b'\x55' * 32))
        self.assertFalse(of.is_flat(b'\x51' * 32))      # two pixel values in a byte
        self.assertFalse(of.is_flat(b'\x55' * 31 + b'\x56'))

    def test_classify(self):
        rnd = random.Random(6)
        cell = bytes(rnd.randrange(1, 256) for _ in range(128))
        pieces = of.pieces_of(cell)
        miss = lambda q: -1
        self.assertEqual(of.classify_cell(bytes(128), miss, miss), 'blank')
        self.assertEqual(of.classify_cell(cell, lambda q: 7 if q == cell else -1, miss), 'tile')
        self.assertEqual(of.classify_cell(cell, miss, lambda q: 3 if q in pieces else -1), 'pieces')
        self.assertEqual(of.classify_cell(cell, miss, lambda q: 3 if q == pieces[0] else -1), 'part')
        self.assertEqual(of.classify_cell(cell, miss, miss), 'own')
        # flat pieces match by themselves: a cell of one flat and three arcade pieces is 'pieces'
        mixed = pieces[0] + pieces[1] + bytes(0)
        half = bytearray(cell)
        for y in range(8):
            half[y * 8:y * 8 + 4] = b'\x55' * 4
        hp = of.pieces_of(bytes(half))
        self.assertEqual(of.classify_cell(bytes(half), miss, lambda q: 1 if q in hp[1:] else -1), 'pieces')
        self.assertEqual(len(mixed), 64)
        # a cell that is all flat pieces of one colour is a tile
        self.assertEqual(of.classify_cell(b'\x55' * 128, miss, miss), 'tile')

    def test_palette_rows_in_a_picture(self):
        rnd = random.Random(12)
        width = 256
        rows = []
        for y in range(48):
            if 16 <= y < 21:      # five rows of four palettes
                rows.append(b''.join(palette(100 + y * 4 + i) for i in range(4)))
            elif 21 <= y < 32:
                rows.append(bytes(128))
            else:
                rows.append(bytes(rnd.randrange(1, 256) for _ in range(128)))
        page = b''.join(rows)
        self.assertEqual(sorted(of.palette_rows(page, width)), [16, 17, 18, 19, 20])
        self.assertEqual(of.palette_rows(page, width, 6), set())
        c = of.page_stats(page, lambda q: -1, lambda q: -1, width, True)
        # cell row 1 (pixel rows 16 to 31) is five palette rows and eleven blank rows: palette cells, all 16
        self.assertEqual((c['cells'], c['pal'], c['own']), (16 * 3, 16, 32))
        c = of.page_stats(page, lambda q: -1, lambda q: -1, width, False)
        self.assertEqual((c['pal'], c['blank'], c['own']), (0, 0, 48))     # the palette rows are pixels to it
        self.assertEqual(c['part'], 0)
        self.assertEqual(c['cells'], 48)


class Headers(unittest.TestCase):
    def test_xa(self):
        subs = [(1, ch, 0x64, 1) for ch in (0, 1, 2, 0)] + [(1, 0, 0, 0)] * 3
        x = of.xa_summary(subs)
        self.assertEqual((x['sectors'], x['null']), (7, 3))
        self.assertEqual(dict(x['audio']), {0: 2, 1: 1, 2: 1})
        self.assertEqual(x['coding'], {1})
        self.assertEqual(of.xa_coding_text(1), 'stereo 37.8 kHz 4 bit')
        self.assertEqual(of.xa_coding_text(0x15), 'stereo 18.9 kHz 8 bit')
        self.assertEqual(of.xa_coding_text(0), 'mono 37.8 kHz 4 bit')

    def test_str(self):
        hdr = struct.pack('<IHHII', 0x80010160, 3, 5, 77, 5000) + struct.pack('<HH', 320, 240) + bytes(12)
        self.assertEqual(of.str_video_header(hdr), (77, 3, 5, 320, 240))
        self.assertIsNone(of.str_video_header(bytes(32)))
        self.assertIsNone(of.str_video_header(hdr[:20]))

    def test_exe(self):
        d = bytearray(0x900)
        d[:8] = b'PS-X EXE'
        struct.pack_into('<4I', d, 0x10, 0x80011ac8, 0, 0x80010000, 0x1f800)
        struct.pack_into('<I', d, 0x30, 0x801ffff0)
        self.assertEqual(of.exe_header(bytes(d)), dict(entry=0x80011ac8, load=0x80010000, size=0x1f800, sp=0x801ffff0))
        self.assertIsNone(of.exe_header(bytes(0x900)))

    def test_is_module(self):
        code = struct.pack('<8I', *([of.JR_RA] * 8))
        self.assertTrue(of.is_module(code))
        self.assertFalse(of.is_module(code[:28] + bytes(4)))
        self.assertFalse(of.is_module(bytes(64)))

    def test_offset_table(self):
        d = struct.pack('<4I', 16, 100, 40, 60) + bytes(100)
        offs = of.offset_table(d)
        self.assertEqual(offs, (16, 100, 40, 60))
        self.assertEqual(of.table_block_sizes(offs, 116), [24, 16, 20, 40])     # in file order: 16-40, 40-60, 60-100, 100-116
        self.assertIsNone(of.offset_table(struct.pack('<4I', 16, 16, 40, 60) + bytes(100)))     # repeated offset
        self.assertIsNone(of.offset_table(struct.pack('<4I', 16, 900, 40, 60) + bytes(100)))    # outside the file
        self.assertIsNone(of.offset_table(b'abc'))


class Text(unittest.TestCase):
    def test_cluster_ranges(self):
        self.assertEqual(of.cluster_ranges([5, 1, 2, 2, 100, 101, 300], 10), [(1, 5, 3), (100, 101, 2), (300, 300, 1)])
        self.assertEqual(of.cluster_ranges([], 10), [])
        text = of.ranges_text('codes', [1, 2, 3, 500, 900, 901], 10, 2)
        self.assertIn('3 groups', text)
        self.assertIn('0x1-0x3 (3)', text)
        self.assertIn('0x384-0x385 (2)', text)
        self.assertNotIn('0x1f4', text)         # the smallest group is cut by the limit
        self.assertTrue(text.endswith('...'))
        self.assertEqual(of.ranges_text('codes', [], 10), '')

    def test_count_text(self):
        c = collections.Counter(tiles=10, tile_arcade=7, runs=0, pal_n=2)
        self.assertEqual(of.count_text(c), 'tiles=10 tile_arcade=7 pal_n=2')
        self.assertEqual(of.count_text(collections.Counter(tile_pieces=3, tile_own=1, blank=2)), 'blank=2 tile_pieces=3 tile_own=1')
        self.assertEqual(of.share(1, 4), '1/4 (25%)')
        self.assertEqual(of.share(0, 0), '-')

    def test_ratio_text(self):
        a = collections.Counter(tiles=11, blank=1, tile_arcade=5, tile_pieces=1, pal_n=4, pal_arcade=4)
        b = collections.Counter(tiles=21, blank=1, tile_arcade=10, tile_pieces=2, pal_n=2, pal_arcade=1, cells=10, c_tile=3, c_pieces=2)
        text = of.ratio_text([a, b])
        self.assertIn('tiles 50%', text)             # 5 of 10 and 10 of 20
        self.assertIn('tiles with pieces 60%', text)  # 6 of 10 and 12 of 20
        self.assertIn('palettes 50-100%', text)
        self.assertIn('cells 50%', text)
        self.assertNotIn(chr(0x2014), text)

    def test_what_of(self):
        self.assertEqual(of.what_of('ASCII', 0, 9, 0, b'')[0], 'picture')
        self.assertEqual(of.what_of('CONT', 3, 0, 0, b'pBAV')[0], 'VAB header')
        self.assertEqual(of.what_of('CONT', 3, 5, 0, b'xxxx')[0], 'sound commands')
        self.assertEqual(of.what_of('CONT', 4, 1, 0, b'')[0], 'VAB body')
        self.assertEqual(of.what_of('X', 0, 0xffff, 3, bytes(16))[0], 'zeros')
        self.assertEqual(of.what_of('X', 0, 0x99, 0, b'a')[0], '?')

    def test_no_em_dash_in_the_source(self):
        text = (ROOT / 'port/arcade/other_files.py').read_text()
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
