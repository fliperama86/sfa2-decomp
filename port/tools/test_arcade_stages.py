#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/stages.py and its C helper on made-up data; no game files.

Colour conversion, palette rows, the layer map's cell addressing, the arcade cells' tile codes, the scan that
finds the three layer maps in a module's setup code, the chunk split of a stage file, and (with the helper
compiled and a tiny made-up arcade program and graphics) the matching of a layer block, of tiles, of 8x8 pieces
and of palette rows.
"""
from pathlib import Path
import random, struct, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import stages  # noqa: E402


def lui(rt, imm): return 0x3c000000 | rt << 16 | imm
def addiu(rt, rs, imm): return 0x24000000 | rs << 21 | rt << 16 | (imm & 0xffff)
def ori(rt, rs, imm): return 0x34000000 | rs << 21 | rt << 16 | imm
def sw(rt, base, off): return 0xac000000 | base << 21 | rt << 16 | off
def sh(rt, base, off): return 0xa4000000 | base << 21 | rt << 16 | off
def lw(rt, base, off): return 0x8c000000 | base << 21 | rt << 16 | off


def setup(address, blank, width):
    """The words of a layer setup function: map pointer twice, blank code, width, height, then a return."""
    return [lui(2, (address >> 16) + (1 if address & 0x8000 else 0)), addiu(2, 2, address & 0xffff), sw(2, 4, 0x50), sw(2, 4, 0x54),
            ori(2, 0, blank), sh(2, 4, 0x1e), ori(2, 0, width), sw(2, 4, 0x58), ori(2, 0, 256), 0x03e00008, sw(2, 4, 0x5c), 0]


def words(ws): return b''.join(struct.pack('<I', w) for w in ws)


def pac_file(chunks):
    """A stage-file-shaped archive: [(table, slot, bytes)] with the sector padding and the first-word copies."""
    head = bytearray(2048)
    struct.pack_into('<I', head, 0, len(chunks))
    body = b''
    for i, (table, slot, data) in enumerate(chunks):
        struct.pack_into('<III', head, 0x20 + 32 * i, table << 16 | slot, len(data), struct.unpack_from('<I', data)[0])
        body += data + bytes(-len(data) % 2048)
    return bytes(head) + body


class Pure(unittest.TestCase):
    def test_colour_conversion(self):
        self.assertEqual(stages.arcade_color_to_ps1(0x0fff), 30 | 30 << 5 | 30 << 10)      # 15 * 17 * 43 // 45 = 243, >> 3
        self.assertEqual(stages.arcade_color_to_ps1(0x0fff, 15), 0x7fff)                  # full brightness: 255 >> 3
        self.assertEqual(stages.arcade_color_to_ps1(0x0000), 0)
        self.assertEqual(stages.arcade_color_to_ps1(0x0f00), 30)                          # red is the low five bits
        self.assertEqual(stages.arcade_color_to_ps1(0x000f), 30 << 10)
        self.assertEqual(stages.arcade_color_to_ps1(0x1fff), stages.arcade_color_to_ps1(0x0fff))   # a word's own brightness is ignored

    def test_palette_row(self):
        row = stages.ps1_palette_from_arcade([0x0fff, 0, 0x0100] + [0x0888] * 12 + [0x0abc])
        self.assertEqual(len(row), 16)
        self.assertEqual(row[0], 0)
        self.assertEqual(row[1], 0x7bde)
        self.assertEqual(row[2], 0x8000)               # black is the opaque black
        self.assertEqual(row[3], stages.arcade_color_to_ps1(0x0100))
        self.assertEqual(row[15], stages.arcade_color_to_ps1(0x0888))   # the 16th arcade word is not used
        with self.assertRaises(ValueError):
            stages.ps1_palette_from_arcade([0] * 14)

    def test_palette_groups(self):
        self.assertEqual(stages.palette_group(0, 96), (0, 0))
        self.assertEqual(stages.palette_group(5, 96), (1, 2))
        self.assertEqual(stages.palette_group(62, 96), (0, 31))
        self.assertEqual(stages.palette_group(63, 96), (1, 31))
        self.assertEqual(stages.palette_group(64, 96), (2, 0))
        self.assertEqual(stages.palette_group(95, 96), (2, 31))
        self.assertEqual(stages.palette_group(66, 128), (2, 1))                  # 128 or more: layer 2's rows alternate with another table
        self.assertEqual(stages.palette_group(67, 128), ('more', 67))
        self.assertEqual(stages.palette_group(126, 192), (2, 31))
        self.assertEqual(stages.palette_group(130, 192), ('more', 130))

    def test_palette_prediction(self):
        prog = bytearray(0x300000)
        at = stages.PALETTE_TABLES[1] + 0x400 * 3 + 0x20 * 2
        struct.pack_into('>16H', prog, at, *range(0x0100, 0x0110))
        want = stages.ps1_palette_from_arcade(list(range(0x0100, 0x0110)))
        self.assertEqual(stages.predicted_palette(bytes(prog), 3, 1, 2), want)
        chunk = bytearray(96 * 32)
        struct.pack_into('<16H', chunk, 32 * 5, *want)          # palette 5 is layer 1, row 2
        self.assertEqual(stages.palette_agreement(bytes(prog), 3, bytes(chunk))[1], [1, 32])
        self.assertEqual(stages.palette_agreement(bytes(prog), 3, bytes(chunk), [set(), {2, 7}, set()])[1], [1, 2])
        diff = stages.first_palette_difference(bytes(prog), 3, bytes(chunk))
        self.assertEqual(diff[1], (0, 15))                      # layer 1's first row (palette 1) is all zero, the arcade's is not
        self.assertTrue(stages.is_flat_palette(bytes(chunk), 7))
        self.assertFalse(stages.is_flat_palette(bytes(chunk), 5))

    def test_object_palettes(self):
        prog = bytearray(0x300000)
        for k in range(7):
            struct.pack_into('>16H', prog, 0x275c40 + 0xe0 * 4 + 0x20 * k, *([0x0123 * (k + 1) & 0xfff] * 16))
        chunk = b''.join(struct.pack('<16H', *stages.ps1_palette_from_arcade([0x0123 * (k + 1) & 0xfff] * 16)) for k in range(7))
        self.assertEqual(stages.object_palette_agreement(bytes(prog), 4, chunk), (7, 7))
        self.assertEqual(stages.object_palette_agreement(bytes(prog), 5, chunk), (0, 7))
        self.assertTrue(stages.same_palette([0, 0x8000, 5], [0, 0, 5]))
        self.assertFalse(stages.same_palette([0, 1, 5], [0, 2, 5]))

    def test_cell_decoding(self):
        code = 22 << 10 | 7 << 5 | 9
        self.assertEqual(stages.decode_cell(code, 0x7800 + 2 + 64 * 5), (0, 72, 56, 2, 5))
        self.assertEqual(stages.decode_cell(24 << 10, 0x7801), (2, 0, 0, 1, 0))

    def test_map_addressing_matches_the_pages_of_the_drawing_code(self):
        # the drawing code: line = map + (row >> 4) * ((width >> 9) << 10) + ((row & 15) << 6); e = line + ((c >> 5) << 10) + ((c & 31) << 1)
        for width in (512, 1024, 2048):
            seen = set()
            for cy in range(48):
                for cx in range(width // 16):
                    want = (cy >> 4) * ((width >> 9) << 10) + ((cy & 15) << 6) + ((cx >> 5) << 10) + ((cx & 31) << 1)
                    self.assertEqual(stages.map_word_index(cx, cy, width // 16, 16), want)
                    seen.add(want)
            self.assertEqual(len(seen), 48 * (width // 16))
            self.assertEqual(max(seen) + 2, 3 * (width // 512) * 1024)      # the map is dense: 1,024 words a 32 by 16 page
        for cy in range(16):
            for cx in range(32):
                want = (cy >> 3) * (2 << 8) + ((cy & 7) << 5) + ((cx >> 4) << 8) + ((cx & 15) << 1)
                self.assertEqual(stages.map_word_index(cx, cy, 32, 32), want)
        with self.assertRaises(ValueError):
            stages.map_word_index(0, 0, 32, 8)

    def test_arcade_cells(self):
        self.assertEqual(stages.arcade_cell_tile(0x2271, 0x0c2, 16), (((0x2271 + 0xa400) & 0xffff) + 0x10000, False, True, 2))
        self.assertEqual(stages.arcade_cell_tile(0x0000, 0x0020, 16)[:3], (0xa400 + 0x10000, True, False))
        self.assertEqual(stages.arcade_cell_tile(0x3fff, 0, 32)[0], ((0x3fff + 0x900) & 0x3fff) + 0x4000)
        self.assertEqual(stages.arcade_cell_tile(0, 0x7f, 32)[1:], (True, True, 31))
        self.assertEqual([stages.arcade_block_cell(k, 16) for k in (0, 1, 15, 16, 255)], [(0, 0), (0, 1), (0, 15), (1, 0), (15, 15)])

    def test_finding_the_layers(self):
        m = bytearray(0x8000)
        for at, ws in ((0x100, setup(0x801e9f90, 0x5800, 1024)), (0x200, setup(0x801eaf90, 0x5812, 512)), (0x300, setup(0x801ecf90, 0x20, 1024))):
            m[at:at + 48] = words(ws)
        layers, pointers = stages.find_layers(bytes(m))
        self.assertEqual(layers, [(0x1f90, 0x5800, 1024), (0x2f90, 0x5812, 512), (0x4f90, 0x20, 1024)])
        self.assertEqual(pointers, [0x1f90, 0x2f90, 0x4f90])
        self.assertEqual(stages.layer_extents(layers, pointers), [(0x1f90, 64, 16, 16), (0x2f90, 32, 64, 16), (0x4f90, 32, 8, 32)])
        # a load into the register between the address and the store forgets the constant
        spoiled = bytearray(m)
        ws = setup(0x801e9f90, 0x5800, 1024)
        ws.insert(2, lw(2, 4, 0))
        spoiled[0x100:0x100 + 52] = words(ws)
        self.assertIsNone(stages.find_layers(bytes(spoiled)))
        self.assertIsNone(stages.find_layers(bytes(0x1000)))

    def test_rows_used(self):
        module = bytearray(0x4000)
        layers = [(0, 0x5800, 1024), (0x1000, 0x5810, 1024), (0x2000, 0x20, 1024)]
        extents = stages.layer_extents(layers, [0, 0x1000, 0x2000])
        for i in range(0, 0x1000, 4):                          # layer 0 is blank everywhere ...
            struct.pack_into('<HH', module, i, 0x5800, 0x7800)
        for cx, cy, code, row in ((0, 0, 0x5c40, 3), (1, 0, 0x5c40, 3), (40, 9, 0x5c80, 9), (2, 2, 0x5800, 5)):     # ... but for three cells
            struct.pack_into('<HH', module, 2 * stages.map_word_index(cx, cy, 64, 16), code, 0x7800 + 64 * row)
        self.assertEqual(stages.rows_used(bytes(module), layers, extents)[0], {3, 9})

    def test_layer_extents_clamps(self):
        layers = [(0x1000, 0x5800, 1024), (0x1400, 0x5810, 1024), (0x1800, 0x20, 1024)]
        self.assertEqual(stages.layer_extents(layers, [0x1000, 0x1400, 0x1800])[0][2], 16)           # too short: at least 16 rows
        far = [(0x1000, 0x5800, 1024), (0x9000, 0x5810, 1024), (0xa000, 0x20, 1024)]
        self.assertEqual(stages.layer_extents(far, [0x1000, 0x9000, 0xa000])[0][2], 64)               # far: at most 64 rows
        self.assertEqual(stages.layer_extents(far, [0x1000, 0x9000, 0xa000])[2], (0xa000, 32, 8, 32))

    def test_split_stage(self):
        pal, pages = bytes([1, 2, 3, 4]) * 8, bytes([5, 6, 7, 8]) * 16
        chunks = [(1, 0, pal), (2, 0, pages), (0, 3, b'effe'), (1, 0, b'shar' * 4), (0, 7, b'tile' * 32), (0, 9, b'objt' * 32), (0, 10, b'opal' * 8), (0, 6, b'modu' * 8)]
        parts = stages.split_stage(pac_file(chunks))
        self.assertEqual(parts['palettes'], pal)
        self.assertEqual(parts['shared palettes'], b'shar' * 4)
        self.assertEqual(parts['object tiles'], b'objt' * 32)
        self.assertEqual(sorted(parts), sorted(['palettes', 'shared palettes', 'pages', 'effects', 'effect tiles', 'object tiles', 'object palettes', 'module']))
        without = stages.split_stage(pac_file([c for c in chunks if c[1] != 9]))
        self.assertNotIn('object tiles', without)
        self.assertEqual(stages.split_stage(pac_file(chunks[:2])), 'no module chunk')
        self.assertEqual(stages.split_stage(pac_file(chunks + [(0, 6, b'more')])), 'two module chunks')
        self.assertEqual(stages.split_stage(pac_file(chunks + [(3, 1, b'xxxx')])), 'unknown chunk table 3 slot 1')


# ---------------------------------------------------------------- the helper, on a made-up arcade

def tile_bytes(pixels):
    """128 bytes of the arcade layout (planes, 8 pixels to a byte) of 16x16 pixels with the arcade's colour values."""
    out = bytearray(128)
    for y in range(16):
        for x in range(16):
            for pl in range(4):
                out[y * 8 + x // 8 * 4 + pl] |= ((pixels[y * 16 + x] >> pl) & 1) << (7 - x % 8)
    return bytes(out)


def ps1_tile(pixels, flip_x=False, flip_y=False):
    """The 16x16 PS1 pixel values of arcade pixels: flipped, then one added (mod 16)."""
    return [(pixels[(15 - y if flip_y else y) * 16 + (15 - x if flip_x else x)] + 1) & 15 for y in range(16) for x in range(16)]


def put_tile(pages, page, u, v, px):
    for y in range(16):
        for x in range(16):
            at = page * 32768 + (v + y) * 128 + (u + x) // 2
            pages[at] = (pages[at] & (0x0f if (u + x) & 1 else 0xf0)) | (px[y * 16 + x] << (4 if (u + x) & 1 else 0))


class Helper(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='stages-test-')
        rnd = random.Random(7)
        cls.gfx = bytearray(0x10000 * 128 + 40 * 128)                  # tiles 0x10000 and up are the 16 pixel scroll tiles
        cls.pixels = []
        for k in range(40):
            px = [rnd.randrange(16) for _ in range(256)]
            if k == 39:
                px = [15] * 256                                         # the transparent tile
            cls.pixels.append(px)
            cls.gfx[(0x10000 + k) * 128:(0x10000 + k + 1) * 128] = tile_bytes(px)
        cls.prog = bytearray(0x300000)
        cls.block = 5
        cls.cells = {}
        base = stages.TABLE16 + cls.block * 1024
        for x in range(16):
            for y in range(16):
                k = (x * 3 + y) % 40
                attr = (x + y) % 8 | (0x20 if x % 5 == 0 else 0) | (0x40 if y % 7 == 0 else 0)
                code = (0x10000 + k - 0x10000 - 0xa400) & 0xffff
                struct.pack_into('>HH', cls.prog, base + (x * 16 + y) * 4, code, attr)
                cls.cells[(x, y)] = (k, attr)
        pal_at = 0x27c1c0 + 0x400 * 2 + 0x20 * 3
        cls.arc_words = list(range(0x0123, 0x0123 + 16 * 0x0111, 0x0111))[:16]
        cls.arc_words[4] = 0                                           # a black entry
        struct.pack_into('>16H', cls.prog, pal_at, *cls.arc_words)
        cls.helper = stages.Helper(cls.dir.name, bytes(cls.gfx), bytes(cls.prog))

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def ps1_layer(self, change=None):
        """Pages and a module holding a 16 by 16 cell layer that equals the arcade block, in the PS1's own layout."""
        pages = bytearray(32768 * 2)
        module = bytearray(0x2000)
        slots = {}
        for (x, y), (k, attr) in self.cells.items():
            key = (k, bool(attr & 0x20), bool(attr & 0x40))
            if key not in slots:
                n = len(slots) + 1
                slots[key] = n
                put_tile(pages, n // 256, n % 16 * 16, (n // 16 % 16) * 16, ps1_tile(self.pixels[k], key[1], key[2]))
            n = slots[key]
            code = (22 + n // 256) << 10 | ((n // 16 % 16) * 2) << 5 | (n % 16 * 2)
            clut = 0x7800 + 1 + 64 * (attr & 31)
            if change == (x, y):
                clut += 64
            struct.pack_into('<HH', module, 0x40 + stages.map_word_index(x, y, 16, 16) * 2, code, clut)
        return bytes(pages), bytes(module)

    def test_block_match(self):
        pages, module = self.ps1_layer()
        (nonflat, best, match, ties, nf_match), = self.helper.match_layer(pages, module, 0x40, 16, 16, 16)
        self.assertEqual((best, match), (self.block, 256))
        self.assertEqual(nonflat, nf_match)
        self.assertGreater(nonflat, 200)
        pages, module = self.ps1_layer(change=(3, 4))             # one cell with another palette row
        (_n, best, match, _t, _m), = self.helper.match_layer(pages, module, 0x40, 16, 16, 16)
        self.assertEqual((best, match), (self.block, 255))
        # a layer two blocks wide gives two results; an all-blank half is the arcade's blank block 0
        _, module = self.ps1_layer()
        self.assertEqual(len(self.helper.match_layer(pages, module, 0x40, 32, 16, 16)), 2)

    def test_tiles_and_flips(self):
        chunk = bytearray()
        for k, fx, fy in ((3, False, False), (4, True, False), (5, False, True), (6, True, True)):
            chunk += bytes(sum(ps1_tile(self.pixels[k], fx, fy)[2 * i + j] << (4 * j) for j in range(2)) for i in range(128))
        chunk += bytes(128)                                      # a tile of PS1 zeros is the arcade's transparent tile
        chunk += bytes([0x57]) * 128                             # not any arcade tile
        n, found, idx = self.helper.tiles(bytes(chunk))
        self.assertEqual((n, found), (6, 5))
        self.assertEqual(idx[:4], [(0x10000 + 3) * 4, (0x10000 + 4) * 4 + 1, (0x10000 + 5) * 4 + 2, (0x10000 + 6) * 4 + 3])
        self.assertEqual(idx[4] // 4, 0x10000 + 39)
        self.assertEqual(idx[5], -1)
        self.assertEqual(stages.tile_summary((n, found, idx)), (6, 5, 3))

    def test_pieces(self):
        pages, module = self.ps1_layer()
        nonflat, found = self.helper.quadrants(pages, module, 0x40, 16, 16)
        self.assertGreater(nonflat, 800)
        self.assertEqual(found, nonflat)
        broken = bytearray(pages)
        for y in range(8):
            broken[y * 128 + 8:y * 128 + 12] = bytes([0x12, 0x34, 0x56, 0x78])   # the top left quarter of the page's second tile changed
        nonflat2, found2 = self.helper.quadrants(bytes(broken), module, 0x40, 16, 16)
        self.assertLess(found2, nonflat2)

    def test_palette_search(self):
        row = stages.ps1_palette_from_arcade(self.arc_words)
        self.assertEqual(row[5], 0x8000)
        zero = list(row)
        zero[5] = 0                                              # black may be stored as 0 too
        other = [0] + [0x1234] * 15
        chunk = b''.join(struct.pack('<16H', *r) for r in (row, other, zero, row))
        n, offs = self.helper.palettes(chunk)
        self.assertEqual(n, 4)
        self.assertEqual(offs[0], 0x27c1c0 + 0x400 * 2 + 0x20 * 3)
        self.assertEqual((offs[2], offs[3]), (offs[0], offs[0]))
        self.assertIsNone(offs[1])
        self.assertEqual(self.helper.palettes(chunk, 13)[1], [None] * 4)      # another brightness finds nothing


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
