#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/stages.py and its C helper on made-up data; no game files.

Colour conversion, palette rows, the layer map's cell addressing, the arcade cells' tile codes, the scan that
finds the three layer maps in a module's setup code, the chunk split of a stage file, and (with the helper
compiled and a tiny made-up arcade program and graphics) the matching of a layer block, of tiles, of 8x8 pieces
and of palette rows. Then the model that stage.c reproduces (page slots, tile conversion and flips, the scan and
first-use packing of a layer, run lists, palettes) and the descriptor made from a made-up stage file: its layer
starts, flags, extras and holes.
"""
from pathlib import Path
import random, struct, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import stages  # noqa: E402
from test_arcade_stage import World  # noqa: E402  the made-up arcade the C test uses


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

    def test_row_classes(self):
        prog = bytearray(0x300000)
        base = stages.PALETTE_TABLES[0] + 0x400
        for row in range(3):
            struct.pack_into('>16H', prog, base + 0x20 * row, *range(0x0100 + row * 0x40, 0x0110 + row * 0x40))
        chunk = bytearray(96 * 32)
        for row in range(3):
            struct.pack_into('<16H', chunk, 32 * 2 * row, *stages.predicted_palette(bytes(prog), 1, 0, row))
        struct.pack_into('<H', chunk, 64 + 6, 0x1234)             # row 1: one entry differs
        struct.pack_into('<16H', chunk, 128, *([0x4321] * 16))     # row 2: the whole row differs
        self.assertEqual(stages.row_classes(bytes(prog), 1, bytes(chunk), 0, [0, 1, 2]), (1, 1, 1))

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

    def test_scroll1_sets(self):
        prog = bytearray(0x300000)
        for m in (0, 5):                                         # two cell groups: codes 100 * m + 4 * c + r, attribute m
            for c in range(4):
                for r in range(4):
                    struct.pack_into('>HH', prog, stages.GROUPS1 + 64 * m + 4 * (c * 4 + r), 100 * m + 4 * c + r, m)
        ids = [0] * 64
        ids[1 * 8 + 2] = 5                                       # column 1, row 2 of the set
        struct.pack_into('>64H', prog, stages.SETS1 + 128 * 3, *ids)
        cells = stages.scroll1_set_cells(bytes(prog), 3)
        self.assertEqual(len(cells), 1024)
        self.assertEqual(cells[(4 + 2, 8 + 3)], (500 + 4 * 2 + 3, 5))        # group (1, 2) holds cell (c, r) = (2, 3) at x 6, y 11
        self.assertEqual(cells[(0, 0)], (0, 0))
        self.assertEqual(stages.scroll1_set_cells(bytes(prog), 0)[(5, 9)], (5, 0))                   # every id 0: group 0 repeated, cell (1, 1) of it
        self.assertEqual(stages.scroll1_piece(10), (0x10005, 0))
        self.assertEqual(stages.scroll1_piece(11), (0x10005, 8))

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


class Model(unittest.TestCase):
    """The model that stage.c reproduces: page slots, tile conversion, layer packing, run lists."""

    def test_slot_codes_and_places(self):
        # 16 pixel tiles: 16 to a column (v fastest), 16 columns to a page; the code's u and v are in units of 8 pixels
        self.assertEqual(stages.slot_code(0, 16), 22 << 10)
        self.assertEqual(stages.slot_code(1, 16), 22 << 10 | 2 << 5)
        self.assertEqual(stages.slot_code(16, 16), 22 << 10 | 2)
        self.assertEqual(stages.slot_code(256, 16), 23 << 10)
        self.assertEqual(stages.slot_code(1, 32), 22 << 10 | 4 << 5)
        self.assertEqual(stages.slot_code(8, 32), 22 << 10 | 4)
        self.assertEqual(stages.slot_code(64, 32), 23 << 10)
        for side in (16, 32):
            for slot in (0, 1, 7, 8, 15, 16, 63, 64, 255, 256, 700):
                self.assertEqual(stages.slot_of_code(stages.slot_code(slot, side), side), slot)
        self.assertIsNone(stages.slot_of_code(5 << 10, 16))               # not on the stage's pages
        self.assertIsNone(stages.slot_of_code(22 << 10 | 1, 16))          # not on the 16 pixel grid
        self.assertEqual(stages.slot_place(0, 16), 0)
        self.assertEqual(stages.slot_place(1, 16), 16 * 128)             # the next tile down
        self.assertEqual(stages.slot_place(16, 16), 8)                   # the next column: 16 pixels of 4 bits
        self.assertEqual(stages.slot_place(256, 16), 32768)
        self.assertEqual(stages.slot_place(1, 32), 32 * 128)
        self.assertEqual(stages.slot_place(8, 32), 16)
        self.assertEqual(stages.slot_place(64, 32), 32768)
        with self.assertRaises(ValueError):
            stages.slot_code(42 * 256, 16)                                # page 64 has no code

    def test_put_and_get_slot_with_bounds(self):
        pages = bytearray(32768 * 2)
        tile = bytes(range(128))
        self.assertTrue(stages.put_slot(pages, 17, 16, tile))
        self.assertEqual(stages.get_slot(pages, 17, 16), tile)
        self.assertEqual(pages[stages.slot_place(17, 16)], 0)
        self.assertEqual(pages[stages.slot_place(17, 16) + 128], 8)       # the second row of the tile is 128 bytes down
        self.assertTrue(stages.put_slot(pages, 511, 16, tile))
        self.assertFalse(stages.put_slot(pages, 512, 16, tile))           # the third page does not exist
        self.assertFalse(stages.put_slot(pages, -1, 16, tile))
        big = bytes(range(256)) * 2
        self.assertTrue(stages.put_slot(pages, 127, 32, big))
        self.assertEqual(stages.get_slot(pages, 127, 32), big)
        self.assertFalse(stages.put_slot(pages, 128, 32, big))
        self.assertFalse(stages.put_slot(bytearray(3000), 1, 16, tile))    # the tile's last row must fit too

    def test_tile_conversion_and_flips(self):
        gfx = bytearray(0x10000 * 128 + 4 * 128)
        pixels = [(3 * x + y) % 16 for y in range(16) for x in range(16)]
        gfx[0x10000 * 128:0x10000 * 128 + 128] = tile_bytes(pixels)
        gfx[(0x10000 + 1) * 128:(0x10000 + 2) * 128] = b'\xff' * 128
        arc = stages.Arc(bytes(gfx), bytes(gfx)) if False else stages.Arc(b'', bytes(gfx))
        rows = arc.rows(0x10000, 16)
        self.assertEqual([v for r in rows for v in r], [(p + 1) & 15 for p in pixels])
        plain = arc.tile(0x10000, 16)
        self.assertEqual(len(plain), 128)
        for y in range(16):
            for x in range(16):
                self.assertEqual((plain[y * 8 + x // 2] >> (4 * (x % 2))) & 15, rows[y][x])      # the left pixel of a pair is the low nibble
        fx, fy = arc.tile(0x10000, 16, True, False), arc.tile(0x10000, 16, False, True)
        for y in range(16):
            for x in range(16):
                self.assertEqual((fx[y * 8 + x // 2] >> (4 * (x % 2))) & 15, rows[y][15 - x])
                self.assertEqual((fy[y * 8 + x // 2] >> (4 * (x % 2))) & 15, rows[15 - y][x])
        self.assertEqual(arc.tile(0x10001, 16), bytes(128))                # arcade colour 15 (transparent) is the PS1's 0
        self.assertEqual(arc.tile(0x20000, 16), bytes(128))                # a tile outside the graphics is transparent
        self.assertTrue(arc.has_tile(0x10003) and not arc.has_tile(0x10004) and not arc.has_tile(-1))
        self.assertEqual(stages.pack_rows([[1, 2, 3, 4]]), bytes([0x21, 0x43]))

    def test_32_pixel_tiles(self):
        gfx = bytearray(8 * 512)
        for y in range(32):
            for x in range(32):
                for pl in range(4):
                    gfx[512 + y * 16 + x // 8 * 4 + pl] |= (((x + 2 * y) // 3 >> pl) & 1) << (7 - x % 8)
        arc = stages.Arc(b'', bytes(gfx))
        rows = arc.rows(1, 32)
        self.assertEqual([[((x + 2 * y) // 3 + 1) & 15 for x in range(32)] for y in range(32)], rows)
        self.assertEqual(len(arc.tile(1, 32)), 512)

    def test_scroll1_pieces(self):
        gfx = bytearray(0x10000 * 128 + 8 * 128)
        pixels = [(x + 16 * y) % 15 for y in range(16) for x in range(16)]
        gfx[0x10002 * 128:0x10003 * 128] = tile_bytes(pixels)
        arc = stages.Arc(b'', bytes(gfx))
        top = [[(pixels[y * 16 + x] + 1) & 15 for x in range(8)] for y in range(8)]
        bottom = [[(pixels[(8 + y) * 16 + x] + 1) & 15 for x in range(8)] for y in range(8)]
        self.assertEqual(arc.piece(4, 0), top)                              # code 4 = tile 0x10002, top rows
        self.assertEqual(arc.piece(5, 0), bottom)
        self.assertEqual(arc.piece(4, 0x20), [r[::-1] for r in top])
        self.assertEqual(arc.piece(4, 0x40), top[::-1])
        self.assertEqual(arc.piece(5, 0x60), [r[::-1] for r in bottom[::-1]])
        self.assertEqual(arc.piece(4, 0x1f), top)                           # the palette row is not part of the piece

    def test_layer_scan_packing_and_blank(self):
        w = World(21)
        arc = w.arc
        ids = [1, 0, 2, 0]
        cells = stages.layer_cells(arc, 1, ids, 64, 16)
        self.assertEqual(len(cells), 1024)
        self.assertEqual([c[:2] for c in cells[:3]], [(0, 0), (1, 0), (2, 0)])         # the cells of a block row by row
        self.assertEqual(cells[16][:2], (0, 1))
        self.assertEqual(cells[256][:2], (16, 0))                                      # then the next block along
        words, tiles = stages.place_layer(cells, 1, 40)
        self.assertEqual(tiles[0], bytes(128))
        self.assertEqual(len(set(tiles)), len(tiles))
        order = []
        for cx, cy, tile, row in cells:
            if tile not in order and any(tile):
                order.append(tile)
        self.assertEqual(tiles[1:], order)                                              # the blank first, then each tile as the scan first meets it
        for cx, cy, tile, row in cells:
            code, clut = words[(cx, cy)]
            self.assertEqual(code, stages.slot_code(40 + tiles.index(tile), 16))
            self.assertEqual(clut, 0x7801 + 64 * (0 if not any(tile) else row))
        with self.assertRaises(ValueError):
            stages.layer_cells(arc, 1, [154, 0, 0, 0], 64, 16)                         # block 154 does not exist
        with self.assertRaises(ValueError):
            stages.layer_cells(arc, 2, [101], 8, 8)
        with self.assertRaises(ValueError):
            stages.layer_cells(arc, 0, [143, 0, 0, 0], 64, 16)
        with self.assertRaises(ValueError):
            stages.layer_cells(arc, 1, [0, 0, 0], 64, 16)                              # three ids for four blocks

    def test_extra_maps_are_pages_of_two_blocks_and_continue_the_scan(self):
        w = World(21)
        arc = w.arc
        cells = stages.xmap_cells(arc, 1, 0, [1, 2])
        self.assertEqual(len(cells), 512)
        self.assertEqual([c[:2] for c in cells[:3]], [(stages.XKEY, 0), (stages.XKEY + 1, 0), (stages.XKEY + 2, 0)])    # a block's cells row by row
        self.assertEqual(cells[16][:2], (stages.XKEY, 1))
        self.assertEqual(cells[256][:2], (stages.XKEY + 16, 0))                         # the second block is the right half of the first page
        cells2 = stages.xmap_cells(arc, 1, 1, [0, 1, 2, 0])
        self.assertEqual(cells2[512][:2], (2 * stages.XKEY, 16))                        # the second page is 16 cells down
        self.assertEqual(stages.xmap_cells(arc, 2, 0, [2, 1])[64][:2], (stages.XKEY + 8, 0))
        own = stages.layer_cells(arc, 1, [1, 0, 2, 0], 64, 16)
        self.assertEqual([c[2:] for c in cells[:256]], [c[2:] for c in stages.layer_cells(arc, 1, [1, 0, 0, 0], 64, 16)[:256]])     # block 1 as in a layer
        self.assertEqual(len(own), 1024)
        d = {'stage': 3, 'layers': [{'across': 64, 'down': 16, 'ids': [0x1d020, 0x1d021, 0x1d022, 0x1d023], 'flags': 0}, {'across': 64, 'down': 16, 'ids': [0x1d120, 0x1d121, 0x1d122, 0x1d123], 'flags': 0, 'map': 0x1200, 'xmaps': [{'map': 0x3000, 'ids': [1, 2]}]},
                                                    {'across': 32, 'down': 8, 'ids': [0x1d304 + k for k in range(4)], 'flags': 0}]}
        all_ = stages.layer_cells_all(arc, d, 1)
        self.assertEqual(len(all_), 1024 + 512)
        self.assertEqual(all_[:1024], stages.layer_cells(arc, 1, stages.ids_of(arc, d['layers'][1]), 64, 16, 3))
        self.assertEqual(all_[1024:], cells)
        self.assertEqual(stages.layer_cell_total(d['layers'][1], 1), 1536)
        self.assertEqual(stages.layer_cell_total(d['layers'][2], 2), 256)
        # a cell's place in the module: the layer's own, or the extra map's page layout
        self.assertEqual(stages.cell_offset(d['layers'][1], 1, 33, 2), 0x1200 + 2 * stages.map_word_index(33, 2, 64, 16))
        self.assertEqual(stages.cell_offset(d['layers'][1], 1, stages.XKEY + 17, 3), 0x3000 + 2 * stages.map_word_index(17, 3, 32, 16))
        # the tiles are packed in first-use order through the extra map: those it adds that the layer has already take the layer's slots
        words, tiles = stages.place_layer(all_, 1, 40)
        words0, tiles0 = stages.place_layer(all_[:1024], 1, 40)
        self.assertEqual(tiles, tiles0)                                                 # the extra blocks 1 and 2 are the layer's own blocks 1 and 2
        for cx, cy, _t, _r in cells:
            self.assertIn(words[(cx, cy)][0], [v[0] for v in words0.values()])
        # writing: the extra cells go to the extra map, marked known with the layer's
        module, known = bytearray(0x4000), bytearray(0x4000)
        self.assertTrue(stages.write_layer_maps(module, known, dict(d['layers'][1], flags=stages.MAP_EXACT), 16, words, None))
        at = 0x3000 + 2 * stages.map_word_index(17, 3, 32, 16)
        self.assertEqual(struct.unpack_from('<HH', module, at), words[(stages.XKEY + 17, 3)])
        self.assertEqual(known[at:at + 4], b'\1' * 4)
        self.assertEqual(sum(known) // 4, 1536)
        only = {(stages.XKEY + 17, 3)}                                                  # known cells named singly, among the extra map's
        module2, known2 = bytearray(0x4000), bytearray(0x4000)
        self.assertTrue(stages.write_layer_maps(module2, known2, d['layers'][1], 16, words, only))
        self.assertEqual(sum(known2) // 4, 1)
        self.assertEqual(known2[at:at + 4], b'\1' * 4)
        self.assertFalse(stages.write_layer_maps(bytearray(0x2800), bytearray(0x2800), d['layers'][1], 16, words, None))         # the extra map does not fit
        for bad_layer, ids in ((1, [154, 0]), (2, [101, 0])):
            with self.assertRaises(ValueError):
                stages.xmap_cells(arc, bad_layer, 0, ids)
        with self.assertRaises(ValueError):
            stages.xmap_cells(arc, 0, 0, [0, 1])

    def test_a_layer_zero_cell_asks_for_colours_and_rows(self):
        w = World(21)
        arc = w.arc
        quad = [(0, 0x02), (1, 0x05), (0, 0x02), (2, 0x05)]
        tile, rows, needs = stages.l0_cell(arc, 3, quad)
        self.assertEqual(len(tile), 128)
        pcs = [arc.piece(c, a) for c, a in quad]
        self.assertEqual(tile, stages.pack_rows([r0 + r1 for r0, r1 in zip(pcs[0], pcs[1])] + [r0 + r1 for r0, r1 in zip(pcs[2], pcs[3])]))
        opaque = [any(v for r in p for v in r) for p in pcs]
        self.assertEqual(rows, [a & 31 for (c, a), o in zip(quad, opaque) if o][:1] + [r for r in dict.fromkeys(a & 31 for (c, a), o in zip(quad, opaque) if o)][1:])
        if needs is not None:
            for j, colour in needs.items():
                self.assertTrue(any(arc.palette(3, 0, a & 31)[j] == colour for (c, a), o in zip(quad, opaque) if o))
        # a made-up cell with known pieces: tile 0x10000 is transparent, so its pieces ask for nothing
        blank = stages.l0_cell(arc, 3, [(0, 3), (0, 3), (0, 3), (0, 3)])
        self.assertEqual(blank[0], bytes(128))
        self.assertEqual(blank[1], [])
        self.assertEqual(blank[2], {})

    def test_layer_zero_rows_are_tried_most_popular_first(self):
        infos = [([2, 5], {}), ([5], {}), ([5, 9], {}), ([2], {}), ([], {}), ([7], {})]
        self.assertEqual(stages.l0_order(infos), [5, 2, 7, 9])                 # 5 in three cells, 2 in two, then 7 and 9 by number
        pal = {r: [0] + [r * 16 + j for j in range(15)] for r in range(32)}
        pal[4] = list(pal[2])
        pal[6] = list(pal[5])
        pal[6][3] = 999
        order = [5, 2, 6]
        self.assertEqual(stages.l0_row([5], {1: pal[5][1], 3: pal[5][3]}, order, pal), 5)        # the first row that has the colours
        self.assertEqual(stages.l0_row([2], {1: pal[2][1]}, order, pal), 2)
        self.assertEqual(stages.l0_row([7, 5], {1: pal[5][1], 3: 999}, order, pal), 6)         # a row of no piece may do
        self.assertEqual(stages.l0_row([7, 5], {1: 12345}, order, pal), 7)                      # no row has the colour: the first piece's row
        self.assertEqual(stages.l0_row([7, 5], None, order, pal), 7)                            # the pieces disagree: the first piece's row
        self.assertEqual(stages.l0_row([], {}, order, pal), 0)                                  # a blank cell: row 0

    def test_exceptions_to_first_use_packing(self):
        blank = bytes(128)
        a, b, c = bytes([1]) * 128, bytes([2]) * 128, bytes([3]) * 128
        cells = [(i, 0, t, 5) for i, t in enumerate([a, b, a, c, b, c, a])]
        words, tiles = stages.place_layer(cells, 0, 10)
        self.assertEqual(tiles, [blank, a, b, c])
        self.assertEqual([stages.slot_of_code(words[(i, 0)][0], 16) - 10 for i in range(7)], [1, 2, 1, 3, 2, 3, 1])
        # the disc started a new slot for the second a, made both c cells blank (slot 0) and put the last a on that new slot
        wants = [1, 2, 3, 0, 2, 0, 3]
        exc = stages.find_exceptions(cells, 16, wants)
        self.assertEqual(exc, [(2, stages.FORCE_NEW), (3, 0), (5, 0), (6, 3)])
        words, tiles = stages.place_layer(cells, 0, 10, dict(exc))
        self.assertEqual([stages.slot_of_code(words[(i, 0)][0], 16) - 10 for i in range(7)], wants)
        self.assertEqual(tiles, [blank, a, b, a])                    # FORCE_NEW added a copy of the tile; explicit slots added none
        self.assertEqual(words[(3, 0)][1], 0x7800)                   # a cell on slot 0 is blank: row 0
        self.assertEqual(words[(0, 0)][1], 0x7800 + 64 * 5)
        # a tile the packing has not made yet that the disc puts beyond the next free slot gets that slot (the slots between stay empty)
        exc = stages.find_exceptions(cells, 16, [1, 2, 1, 9, 2, 9, None])
        self.assertEqual(exc, [(3, 9)])
        words, tiles = stages.place_layer(cells, 0, 10, dict(exc))
        self.assertEqual([stages.slot_of_code(words[(i, 0)][0], 16) - 10 for i in range(7)], [1, 2, 1, 9, 2, 9, 1])
        self.assertEqual(tiles, [blank, a, b] + [None] * 6 + [c])
        # a tile it has made already is left alone when the disc puts the cell beyond the next free slot, and so is a cell off the pages
        self.assertEqual(stages.find_exceptions(cells, 16, [1, 2, 9, 3, 2, 3, None]), [])
        self.assertEqual(stages.find_exceptions(cells, 16, [1, 2, 1, 3, 2, 3, 1]), [])
        with self.assertRaises(ValueError):
            stages.place_layer(cells, 0, 10, {3: 0x10000})

    def test_the_starts_the_packing_rule_gives(self):
        self.assertEqual(stages.chain_end(141, [], []), 141)
        self.assertEqual(stages.chain_end(207, [], [(207, 16)]), 223)
        self.assertEqual(stages.chain_end(100, [(100, 5, 7, 0), (105, 1, 8, 1)], [(106, 4)]), 110)       # runs that touch one after the other
        self.assertEqual(stages.chain_end(100, [(101, 5, 7, 0)], []), 100)                              # a gap ends it
        self.assertEqual(stages.chain_end(100, [(90, 5, 7, 0)], []), 100)                               # a run that ends before it does not count
        self.assertEqual(stages.l1_start_rule(141, [], []), 144)             # stage 00 of the examined disc
        self.assertEqual(stages.l1_start_rule(112, [], []), 112)
        self.assertEqual(stages.l1_start_rule(207, [], [(207, 16)]), 224)
        self.assertEqual(stages.l2_start_rule(441, [], []), 112)
        self.assertEqual(stages.l2_start_rule(749, [], [(749, 4)]) , stages.l2_start_after(753))
        d = {'extras': [(10, 2, 3, 0)], 'holes': [], 'layers': [{'flags': 0, 'start': 0}, {'flags': stages.START_RULE, 'start': 99}, {'flags': stages.START_RULE, 'start': 99}]}
        self.assertEqual(stages.layer_start(d, 1, [33, 0, 0, 0]), 48)
        self.assertEqual(stages.layer_start(d, 2, [33, 100, 0, 48]), stages.l2_start_after(148))
        d['layers'][1]['flags'] = 0
        self.assertEqual(stages.layer_start(d, 1, [33, 0, 0, 0]), 99)

    def test_the_other_palettes_are_arcade_rows_or_filler(self):
        prog = bytearray(0x300000)
        for row in range(6):
            struct.pack_into('>16H', prog, 0x2a9000 + 0x20 * row, *[(row * 16 + c * 3 + 1) & 0xfff for c in range(16)])
        for layer in range(3):
            for row in range(32):
                struct.pack_into('>16H', prog, stages.PALETTE_TABLES[layer] + 0x400 * 2 + 0x20 * row, *[(layer * 32 + row) * 7 + c for c in range(16)])
        prog = bytes(prog)
        more = [(65, 2, 3, stages.MORE_ROW, 0x2a9020), (71, 2, 2, stages.MORE_FILL, 0), (128, 1, 2, stages.MORE_ROW, 0x2a9000)]
        data, known = stages.model_palettes(prog, 2, 160, 0, more)
        for i, k in enumerate((65, 67, 69)):
            words = struct.unpack_from('>15H', prog, 0x2a9020 + 0x20 * i)
            self.assertEqual(list(struct.unpack_from('<16H', data, 32 * k)), [0] + [stages.arcade_color_to_ps1(x) or 0x8000 for x in words])
            self.assertEqual(known[32 * k:32 * k + 32], b'\1' * 32)
        fill = [0] + [stages.arcade_color_to_ps1(stages.FILL_COLOUR)] * 15
        self.assertEqual(list(struct.unpack_from('<16H', data, 71 * 32)), fill)
        self.assertEqual(list(struct.unpack_from('<16H', data, 73 * 32)), fill)
        self.assertEqual(sum(1 for k in range(160) if known[32 * k]), 3 + 2 + 2)
        for bad in ([(70, 2, 50, stages.MORE_FILL, 0)], [(65, 2, 1, stages.MORE_ROW, 0x2fffe0 + 0x20)], [(128, 1, 40, stages.MORE_FILL, 0)]):
            with self.assertRaises(ValueError):
                stages.model_palettes(prog, 2, 160, 0, bad)

    def test_finding_the_runs_of_other_palettes(self):
        w = World(7)
        arc = stages.Arc(bytearray(w.arc.prog), w.arc.gfx)
        prog = arc.prog
        for row in range(8):
            struct.pack_into('>16H', prog, 0x2a9000 + 0x20 * row, *[0x100 + 16 * row + c for c in range(16)])
        chunk = bytearray(160 * 32)
        for i, row in enumerate((2, 3, 4)):                                        # palettes 65, 67, 69 are rows 2 to 4
            struct.pack_into('<16H', chunk, 32 * (65 + 2 * i), 0, *[stages.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', prog, 0x2a9000 + 0x20 * row)])
        struct.pack_into('<16H', chunk, 32 * 128, 0, *[stages.arcade_color_to_ps1(stages.FILL_COLOUR)] * 15)        # palette 128 is filler
        struct.pack_into('<16H', chunk, 32 * 129, 0, *[1] * 15)                    # palette 129 is neither
        arc._row_index = {}
        for at in range(0x2a9000, 0x2a9000 + 0x20 * 8, 0x20):
            arc._row_index.setdefault((0,) + tuple(stages.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', prog, at)), []).append(at)
        runs = stages.more_runs(arc, bytes(chunk))
        self.assertEqual(runs, [(65, 2, 3, stages.MORE_ROW, 0x2a9040), (128, 1, 1, stages.MORE_FILL, 0)])
        data, known = stages.model_palettes(bytes(prog), 3, 160, 0, runs)
        for k in (65, 67, 69, 128):
            self.assertEqual(data[32 * k:32 * k + 32], bytes(chunk[32 * k:32 * k + 32]))
            self.assertEqual(known[32 * k:32 * k + 32], b'\1' * 32)
        self.assertEqual(known[129 * 32:130 * 32], bytes(32))

    def test_the_second_palette_chunk_has_no_layer_tables(self):
        w = World(7)
        arc = stages.Arc(bytearray(w.arc.prog), w.arc.gfx)
        prog = arc.prog
        for row in range(8):
            struct.pack_into('>16H', prog, 0x2a9000 + 0x20 * row, *[0x100 + 16 * row + c for c in range(16)])
        chunk = bytearray(128 * 32)
        for k, row in ((0, 2), (1, 3), (64, 4), (65, 5)):                           # palettes that a layer chunk would give to the layer tables
            struct.pack_into('<16H', chunk, 32 * k, 0, *[stages.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', prog, 0x2a9000 + 0x20 * row)])
        struct.pack_into('<16H', chunk, 32 * 10, 0, *[7] * 15)                      # no arcade row
        arc._row_index = {}
        for at in range(0x2a9000, 0x2a9000 + 0x20 * 8, 0x20):
            arc._row_index.setdefault((0,) + tuple(stages.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', prog, at)), []).append(at)
        self.assertEqual(stages.more_runs(arc, bytes(chunk)), [(65, 1, 1, stages.MORE_ROW, 0x2a9000 + 0x20 * 5)])    # a chunk of 128 palettes names 0, 1 and 64 as layer rows
        runs = stages.more_runs(arc, bytes(chunk), all_more=True)
        self.assertEqual(sorted((k, c) for k, st, c, kind, a in runs), [(0, 2), (64, 2)])
        data, known = stages.model_shared_palettes(bytes(prog), len(chunk), runs)
        for k in (0, 1, 64, 65):
            self.assertEqual(data[32 * k:32 * k + 32], bytes(chunk[32 * k:32 * k + 32]))
            self.assertEqual(known[32 * k:32 * k + 32], b'\1' * 32)
        self.assertEqual(sum(known), 4 * 32)
        with self.assertRaises(ValueError):
            stages.model_shared_palettes(bytes(prog), 64, [(0, 1, 3, stages.MORE_FILL, 0)])

    def test_palettes_by_design(self):
        w = World(7)
        arc = stages.Arc(bytearray(w.arc.prog), w.arc.gfx)
        prog = arc.prog
        for row in range(8):
            struct.pack_into('>16H', prog, 0x2a9000 + 0x20 * row, *[0x100 + 16 * row + c for c in range(16)])
        for at in range(stages.MORE_ROWS_FROM, stages.MORE_ROWS_TO, 0x20):               # rows outside the made-up ones: far from anything
            if not 0x2a9000 <= at < 0x2a9100:
                struct.pack_into('>16H', prog, at, *[0xf00 + (at >> 5) % 7] * 16)
        want = lambda row: [0] + [stages.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', prog, 0x2a9000 + 0x20 * row)]
        chunk = bytearray(160 * 32)
        struct.pack_into('<16H', chunk, 32 * 65, *want(2))                           # exact: not by design
        near = want(3)
        near[4] ^= 1; near[9] ^= 1                                                  # 2 entries differ
        struct.pack_into('<16H', chunk, 32 * 67, *near)
        edge = want(4)
        for i in range(1, 7): edge[i] ^= 1                                          # 6 entries differ: still near
        struct.pack_into('<16H', chunk, 32 * 69, *edge)
        far = want(5)
        for i in range(1, 8): far[i] ^= 1                                           # 7 entries differ: blank
        struct.pack_into('<16H', chunk, 32 * 71, *far)
        chunk = bytes(chunk)
        arc._row_index = None
        arc._row_index = {}
        for row in range(8):
            arc._row_index.setdefault(tuple(want(row)), []).append(0x2a9000 + 0x20 * row)
        runs = stages.more_runs(arc, chunk)
        self.assertEqual(runs, [(65, 1, 1, stages.MORE_ROW, 0x2a9040)])
        design, counts = stages.design_runs(arc, chunk, runs)
        by_k = {r[0]: r for r in design}
        self.assertEqual(by_k[67], (67, 1, 1, stages.MORE_APPROX, 0x2a9060))
        self.assertEqual(by_k[69], (69, 1, 1, stages.MORE_APPROX, 0x2a9080))
        self.assertEqual(by_k[71][3], stages.MORE_BLANK)
        self.assertEqual(counts, (sum(1 for r in design if r[3] == stages.MORE_APPROX), sum(1 for r in design if r[3] == stages.MORE_BLANK)))
        self.assertNotIn(65, by_k)
        self.assertTrue(all(palette % 2 == 1 or palette >= 128 for palette in by_k))    # only palettes that no layer table names
        # every palette is then built: rows, approximations (own) and blanks (own)
        own = bytearray(len(chunk))
        data, known = stages.model_palettes(bytes(prog), 3, 160, 0, sorted(runs + design), own)
        self.assertEqual(list(struct.unpack_from('<16H', data, 32 * 67)), want(3))      # the arcade row, not the disc's two-entry variant
        self.assertEqual(data[32 * 71:32 * 72], bytes(32))
        self.assertEqual([k for k in range(160) if own[32 * k]], sorted(by_k))
        self.assertEqual(known[32 * 65:32 * 66], b'\1' * 32)
        # explicit rows (the second chunk's table): the palettes that no run makes are exactly the keys
        small = chunk[32 * 65:32 * 65 + 32] + chunk[32 * 67:32 * 67 + 32] + chunk[32 * 71:32 * 71 + 32]
        runs3 = stages.more_runs(arc, small, all_more=True)
        self.assertEqual(runs3, [(0, 1, 1, stages.MORE_ROW, 0x2a9040)])
        d2, counts2 = stages.design_runs(arc, small, runs3, True, {1: (0x2a9060, 'x'), 2: (None, 'y')})
        self.assertEqual(d2, [(1, 1, 1, stages.MORE_APPROX, 0x2a9060), (2, 1, 1, stages.MORE_BLANK, 0)])
        self.assertEqual(counts2, (1, 1))
        self.assertIn('the table says [1]', stages.design_runs(arc, small, runs3, True, {1: (0x2a9060, 'x')}))
        self.assertIn('the table says [1, 2, 0]'.replace('[1, 2, 0]', '[0, 1, 2]'), stages.design_runs(arc, small, runs3, True, {0: (None, ''), 1: (None, ''), 2: (None, '')}))
        self.assertNotIn(chr(0x2014), ''.join(v[1] for v in stages.SHARED_PALETTES_BY_DESIGN.values()))
        self.assertEqual(len(stages.SHARED_PALETTES_BY_DESIGN), 18)

    def test_locating_ids(self):
        prog = bytearray(0x100)
        prog[0x20:0x28] = bytes([0, 5, 6, 7, 0, 0, 9, 9])
        prog[0x40:0x44] = bytes([5, 6, 7, 8])
        self.assertEqual(stages.locate_ids(prog, [5, 6, 7, 8], 4, 0, 0x100, 0x40), [0x40, 0x41, 0x42, 0x43])
        self.assertEqual(stages.locate_ids(prog, [0, 5, 6, 7], 4, 0, 0x100, 0x20), [0x20, 0x21, 0x22, 0x23])
        self.assertEqual(stages.locate_ids(prog, [0, 0, 0, 0], 4, 0, 0x100, 0x40), [0, 0, 0, 0])      # a row of zeros stays constant
        self.assertEqual(stages.locate_ids(prog, [9, 5, 9, 5], 4, 0, 0x100, 0x30), [0x27, 0x21, 0x27, 0x21])   # no such row: id by id, nearest
        self.assertEqual(stages.locate_ids(prog, [5, 6, 0, 0], 2, 0, 0x100, 0x41)[:2], [0x40, 0x41])   # rows of the layer's width (2): the near one wins
        with self.assertRaises(ValueError):
            stages.locate_ids(prog, [200, 1, 1, 1], 4, 0, 0x100, 0)
        self.assertEqual(stages.list_pointers(bytes(0x300000), 3), (0x1cff8, 0x1d0f8, 0x1d2cc + 3 * 0x40 + 56))

    def test_run_lists(self):
        def tile(b):
            return bytes([b]) * 128
        chunk = tile(0) + tile(1) + tile(2) + tile(3) + tile(4) + tile(0) + tile(0) + tile(5) + tile(6)
        matches = [0, 400, 404, 409, -1, 0, 0, 413, 413]
        # tile 1 = arcade 100 flip 0; tile 2 = arcade 101 flip 0 (consecutive); tile 3 = arcade 102 flip 1 (new run); no match; zeros twice; 103 flip 1 then 103 flip 1
        matches = [0, 400, 404, 409, -1, 0, 0, 413, 417]
        self.assertEqual(stages.tile_runs(chunk, matches),
                         [(stages.RUN_ZERO, 1, 0), (100, 2, 0), (102, 1, 1), (stages.RUN_DISC, 1, 0), (stages.RUN_ZERO, 2, 0), (103, 2, 1)])
        self.assertEqual(stages.tile_runs(b'', []), [])
        self.assertEqual(stages.slot_runs([5, 6, 7, 9, 10, 20], [400, 404, 408, 413, 417, -1]), [(5, 3, 100, 0), (9, 2, 103, 1)])
        self.assertEqual(stages.slot_runs([5, 6], [400, 400]), [(5, 1, 100, 0), (6, 1, 100, 0)])
        self.assertEqual(stages.hole_runs([3, 4, 5, 9, 11, 12]), [(3, 3), (9, 1), (11, 2)])
        self.assertEqual(stages.hole_runs([]), [])

    def test_where_layer_two_starts(self):
        self.assertEqual(stages.l2_start_after(0), 0)
        self.assertEqual(stages.l2_start_after(1), 8)                        # the first 32 pixel column holds two 16 pixel columns
        self.assertEqual(stages.l2_start_after(16), 8)
        self.assertEqual(stages.l2_start_after(17), 8)
        self.assertEqual(stages.l2_start_after(33), 16)
        self.assertEqual(stages.l2_start_after(441), 112)                    # stage 00 of the examined disc: 144 + 297 tiles end in page 1, column 11
        self.assertEqual(stages.l2_start_after(256 * 3 - 1), 3 * 64)             # the end of a page: the next page
        self.assertEqual(stages.l2_start_after(256 * 2 + 40), 2 * 64 + 16)         # 40 slots: two full columns and a half, so the third 32 pixel column is next

    def test_palettes_of_the_model(self):
        prog = bytearray(0x300000)
        for layer in range(3):
            for row in range(32):
                struct.pack_into('>16H', prog, stages.PALETTE_TABLES[layer] + 0x400 * 2 + 0x20 * row, *[(layer * 32 + row) * 7 + c for c in range(16)])
        data, known = stages.model_palettes(bytes(prog), 2, 96, 1 | 1 << 2 | 1 << 70)
        self.assertEqual(list(struct.unpack_from('<16H', data, 0)), stages.predicted_palette(bytes(prog), 2, 0, 0))
        self.assertEqual(list(struct.unpack_from('<16H', data, 32)), stages.predicted_palette(bytes(prog), 2, 1, 0))
        self.assertEqual(list(struct.unpack_from('<16H', data, 70 * 32)), stages.predicted_palette(bytes(prog), 2, 2, 6))
        self.assertEqual([k for k in range(96) if known[k * 32]], [0, 2, 70])
        data, known = stages.model_palettes(bytes(prog), 2, 192, 2 ** 192 - 1)
        self.assertEqual(sum(1 for k in range(192) if known[k * 32]), 64 + 32)            # no row of the tables beyond 128, and every second one of 64 to 127
        self.assertEqual(data[65 * 32:66 * 32], bytes(32))
        self.assertEqual(data[130 * 32:131 * 32], bytes(32))
        obj = stages.model_object_palettes(bytes(prog), 0)
        self.assertEqual(len(obj), 224)
        self.assertEqual(struct.unpack_from('<H', obj, 0)[0], 0)

    def test_blank_by_design_object_tiles(self):
        arc = stages.Arc(b'', bytes(0x10000 * 128 + 8 * 128))
        runs = [(stages.RUN_ZERO, 1, 0), (0x10002, 2, 0), (stages.RUN_DISC, 4, 0)]
        self.assertFalse(stages.runs_owned(runs))
        # the table says how many tiles may be blanked: exactly that many, else a refusal that names both counts
        saved = dict(stages.BLANK_BY_DESIGN)
        try:
            stages.BLANK_BY_DESIGN['T.PAC'] = (4, 'made up')
            got = stages.blank_by_design('T.PAC', runs)
            self.assertEqual(got, [(stages.RUN_ZERO, 1, 0), (0x10002, 2, 0), (stages.RUN_BLANK, 4, 0)])
            self.assertTrue(stages.runs_owned(got))
            self.assertIn('3 object tiles have no arcade source', stages.blank_by_design('T.PAC', runs[:2] + [(stages.RUN_DISC, 3, 0)]))
            self.assertIn('the blank-by-design table says 0', stages.blank_by_design('U.PAC', runs))
            self.assertEqual(stages.blank_by_design('U.PAC', runs[:2]), runs[:2])        # a file with none and no entry is unchanged
            self.assertIn('the blank-by-design table says 4', stages.blank_by_design('T.PAC', runs[:2]))
        finally:
            stages.BLANK_BY_DESIGN.clear()
            stages.BLANK_BY_DESIGN.update(saved)
        data, known = stages.tile_run_bytes(arc, got, 7 * 128)
        self.assertEqual(known, b'\1' * 384 + b'\0' * 512)
        self.assertEqual(data[384:], bytes(512))
        self.assertTrue(all(isinstance(v[0], int) and v[0] > 0 and v[1] for v in stages.BLANK_BY_DESIGN.values()))
        self.assertNotIn(chr(0x2014), ''.join(v[1] for v in stages.BLANK_BY_DESIGN.values()))
        # the generated table says so, with its comment
        text = stages.c_table([dict(World(3).descriptor(objects=[(stages.RUN_ZERO, 1, 0), (stages.RUN_BLANK, 2, 0)]), file='STAGE04.PAC', stage=4)],
                              [(0x10002, 1, 0)], [])
        self.assertIn('{ARCADE_STAGE_BLANK, 2u, 0u},', text)
        self.assertIn('2 of the 3 object tiles are blank by design', text)

    def test_run_bytes(self):
        gfx = bytearray(0x10000 * 128 + 8 * 128)
        for k in range(8):
            gfx[(0x10000 + k) * 128:(0x10000 + k + 1) * 128] = bytes([k * 17 + 3] * 128)
        arc = stages.Arc(b'', bytes(gfx))
        data, known = stages.tile_run_bytes(arc, [(stages.RUN_ZERO, 1, 0), (0x10002, 2, 0), (stages.RUN_DISC, 1, 0), (0x10005, 1, 3)], 5 * 128)
        self.assertEqual(known, b'\1' * 384 + b'\0' * 128 + b'\1' * 128)
        self.assertEqual(data[:128], bytes(128))
        self.assertEqual(data[128:256], arc.tile(0x10002, 16))
        self.assertEqual(data[512:], arc.tile(0x10005, 16, True, True))
        with self.assertRaises(ValueError):
            stages.tile_run_bytes(arc, [(0x10002, 2, 0)], 128)
        with self.assertRaises(ValueError):
            stages.tile_run_bytes(arc, [(0x10002, 1, 0)], 256)
        with self.assertRaises(ValueError):
            stages.tile_run_bytes(arc, [(0x20000, 1, 0)], 128)


def make_stage_file(world, d, extra_tile_slots=(), hole_slots=(), quarter_slots=()):
    """The parts of a made-up stage file as a disc would hold it: the model's own maps, pages and palettes, plus extra arcade tiles
    and tiles that are no arcade tile at some page slots; the module has layer setup functions find_layers can read."""
    built = stages.model_stage(world.arc, d)
    module = bytearray(built['module'][0])
    for n, (blank, width) in enumerate(((0x5800, 1024), (0x5810, 1024), (0x20, 1024))):
        at = 0x3000 + 0x40 * n
        module[at:at + 48] = words(setup(0x801e8000 + d['layers'][n]['map'], blank, width))
    pages = bytearray(built['pages'][0])
    for slot, index, fx, fy in extra_tile_slots:
        stages.put_slot(pages, slot, 16, world.arc.tile(index, 16, fx, fy))
    for slot, index, q, fx, fy in quarter_slots:
        stages.put_slot(pages, slot, 16, world.arc.quarter(index, q, fx, fy))
    for slot in hole_slots:
        stages.put_slot(pages, slot, 16, bytes((slot * 7 + k * 13) % 251 | 1 for k in range(128)))
    palettes = bytearray(built['palettes'][0])
    parts = {'module': bytes(module), 'pages': bytes(pages), 'palettes': bytes(palettes), 'object tiles': bytes(built['object tiles'][0]),
             'object palettes': bytes(built['object palettes'][0]), 'effect tiles': bytes(128), 'effects': bytes(16), 'shared palettes': bytes(32)}
    return parts


class Describe(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='stages-describe-')
        cls.world = World(31)
        cls.helper = stages.Helper(cls.dir.name, cls.world.arc.gfx, cls.world.arc.prog)

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def test_a_stage_the_model_made_is_described_as_exact(self):
        w = self.world
        d0 = w.descriptor(extras=[], holes=[], objects=[(0x10006, 4, 0), (0x10009, 2, 3)], count=96)
        d0['sizes']['module'] = 0x4000
        parts = make_stage_file(w, d0, extra_tile_slots=[(300, 0x1000a, False, False), (301, 0x1000c, True, False), (302, 0x1000d, False, True)], hole_slots=[310, 311])
        r = stages.analyse(parts, self.helper, 3, w.arc.prog)
        d = stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r)
        self.assertIsInstance(d, dict, d)
        self.assertEqual([l['flags'] & 3 for l in d['layers']], [3, 3, 3])
        self.assertEqual([bool(l['flags'] & stages.START_RULE) for l in d['layers']], [False, True, True])      # the made-up stage follows the packing rule
        self.assertEqual([l['exc'] for l in d['layers']], [[], [], []])
        self.assertEqual([l['start'] for l in d['layers']], [l['start'] for l in d0['layers']])
        self.assertEqual([(l['map'], l['across'], l['down']) for l in d['layers']], [(0x100, 64, 16), (0x1200, 64, 16), (0x2400, 32, 8)])
        self.assertEqual(d['layers'][0]['ids'][:4], [0x1d020, 0x1d021, 0x1d022, 0x1d023])
        self.assertEqual(d['layers'][1]['ids'][:1], [0x1d120])
        self.assertEqual(d['layers'][2]['ids'][:1], [0x1d304])
        slots = {a + k: (f + k, fl) for a, c, f, fl in d['extras'] for k in range(c)}
        self.assertEqual(slots, {300: (0x1000a, 0), 301: (0x1000c, 1), 302: (0x1000d, 2)})
        self.assertEqual(d['holes'], [(310, 2)])
        self.assertEqual(d['pal_exact'] & (2 ** 96 - 1), sum(1 << k for k in range(96)))
        self.assertEqual(d['object_runs'], [(0x10006, 4, 0), (0x10009, 2, 3)])
        self.assertEqual(d['sizes']['pages'], len(parts['pages']))
        # the descriptor builds the same pieces the file holds, but for the holes
        model = stages.model_stage(w.arc, d)
        for label in ('module', 'palettes', 'object tiles', 'object palettes'):
            data, known = model[label]
            self.assertTrue(all(a == b for a, b, k in zip(data, parts[label], known) if k), label)
        data, known = model['pages']
        self.assertTrue(all(a == b for a, b, k in zip(data, parts['pages'], known) if k))
        self.assertEqual(len(known) - sum(known), 2 * 128)

    def test_page_slots_of_quarters_are_found_and_holes_counted(self):
        w = self.world
        d0 = w.descriptor(objects=[(0x10003, 2, 0)], count=96)
        d0['sizes']['module'] = 0x4000
        parts = make_stage_file(w, d0, extra_tile_slots=[(300, 0x1000a, False, False)], hole_slots=[310, 311],
                                quarter_slots=[(302, 7, 1, False, False), (303, 7, 2, True, False), (304, 9, 3, False, True), (306, 9, 0, True, True)])
        r = stages.analyse(parts, self.helper, 3, w.arc.prog)
        d = stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r)
        self.assertIsInstance(d, dict, d)
        f = lambda q, fl: stages.SCROLL3_FLIP | q << stages.QUARTER_SHIFT | fl
        self.assertEqual(d['extras'], [(300, 1, 0x1000a, 0), (302, 1, 7, f(1, 0)), (303, 1, 7, f(2, 1)), (304, 1, 9, f(3, 2)), (306, 1, 9, f(0, 3))])
        self.assertEqual(d['holes'], [(310, 2)])
        data, known = stages.model_stage(w.arc, d)['pages']
        self.assertTrue(all(a == b for a, b, k in zip(data, parts['pages'], known) if k))
        self.assertEqual(len(known) - sum(known), 2 * 128)             # only the holes are unknown
        # a file whose holes the table counts: the count must match
        stages.HOLES_BLANK_BY_DESIGN['TEST.PAC'] = (3, 'made up')
        try:
            self.assertIn('2 page slots have no arcade source, the blank-by-design table says 3', stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r))
            stages.HOLES_BLANK_BY_DESIGN['TEST.PAC'] = (2, 'made up')
            self.assertIsInstance(stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r), dict)
            text = stages.c_table([dict(d, file='TEST.PAC')], [(0x10002, 1, 0)], [])
            self.assertIn('2 page slots are blank by design', text)
        finally:
            del stages.HOLES_BLANK_BY_DESIGN['TEST.PAC']
        self.assertIsNone(stages.holes_by_design('NOPE.PAC', [(1, 5)]))
        self.assertIsNone(stages.holes_by_design('STAGE0D.PAC', [(207, 16)]))
        self.assertIn('15 page slots', stages.holes_by_design('STAGE0D.PAC', [(207, 15)]))

    def test_the_quarter_of_a_32_pixel_tile_is_the_rule_of_sprite_tiles(self):
        import sprite_tiles as X
        w = self.world
        for t, q in ((5, 0), (5, 1), (9, 2), (11, 3)):
            for fx in (False, True):
                for fy in (False, True):
                    want = X.flip_bytes(X.T.arcade_tile(X.quarter_raw(bytes(w.arc.gfx), t, q), 0), (1 if fx else 0) | (2 if fy else 0))
                    self.assertEqual(w.arc.quarter(t, q, fx, fy), want, (t, q, fx, fy))

    def test_extra_maps_are_found_and_checked(self):
        w = self.world
        xm = ((), ({'map': 0x3400, 'ids': [2, 0]},), ({'map': 0x3c00, 'ids': [1, 2, 0, 1]},))
        d0 = w.descriptor(xmaps=xm, objects=[(0x10003, 2, 0)], count=96)
        d0['sizes']['module'] = 0x4000
        parts = make_stage_file(w, d0)
        r = stages.analyse(parts, self.helper, 3, w.arc.prog)
        d = stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r)
        self.assertIsInstance(d, dict, d)
        self.assertEqual([l.get('xmaps') for l in d['layers']], [None, [{'map': 0x3400, 'ids': [2, 0]}], [{'map': 0x3c00, 'ids': [1, 2, 0, 1]}]])
        self.assertEqual([l['flags'] & 3 for l in d['layers']], [3, 3, 3])              # the extra maps' cells and tiles are exact
        model = stages.model_stage(w.arc, d)
        data, known = model['module']
        self.assertTrue(all(a == b for a, b, k in zip(data, parts['module'], known) if k))
        self.assertGreaterEqual(sum(known), 4 * (1024 + 1024 + 256 + 512 + 256))
        # a cell of the extra map that is not the arcade's block's: the map is not found (no block fits), the rest is described as before
        module = bytearray(parts['module'])
        struct.pack_into('<H', module, 0x3c00 + 8, struct.unpack_from('<H', module, 0x3c00 + 8)[0] ^ 0x40)
        bad = dict(parts, module=bytes(module))
        d2 = stages.describe('TEST.PAC', 3, bad, w.arc, self.helper, stages.analyse(bad, self.helper, 3, w.arc.prog))
        self.assertIsInstance(d2, dict, d2)
        self.assertEqual([l.get('xmaps') for l in d2['layers']], [None, [{'map': 0x3400, 'ids': [2, 0]}], None])
        # the C table text carries the extra maps and the sizes of the known masks count them
        text = stages.c_table([dict(d, file='STAGE03.PAC')], [(0x10002, 1, 0)], [], (4096, [(0, 2, 5, stages.MORE_ROW, 0x275c40), (1, 1, 2, stages.MORE_FILL, 0)]))
        self.assertIn('static const uint8_t xids_03_2_0[]', text)
        self.assertIn('{15360u, 4u, xids_03_2_0}', text)
        self.assertIn('static const struct arcade_stage_xmap xmaps_03_1[]', text)
        self.assertIn('static const struct arcade_stage_more shared_pal_more[]', text)
        self.assertIn('{0u, 5u, 2u, ARCADE_STAGE_MORE_ROW, 0x275c40u}', text)
        self.assertIn('arcade_stage_shared_pal_bytes = 4096u;', text)
        self.assertNotIn(chr(0x2014), text)

    def test_a_layer_that_differs_loses_its_flags_and_a_palette_row_its_bit(self):
        w = self.world
        d0 = w.descriptor(objects=[(0x10003, 2, 0)], count=96)
        d0['sizes']['module'] = 0x4000
        parts = make_stage_file(w, d0)
        module, pages, palettes = bytearray(parts['module']), bytearray(parts['pages']), bytearray(parts['palettes'])
        struct.pack_into('<H', module, 0x1200 + 8, struct.unpack_from('<H', module, 0x1200 + 8)[0] ^ 0x40)       # a layer 1 cell with another tile code
        struct.pack_into('<H', palettes, 5 * 32 + 4, 0x1234)                                                 # palette 5 (layer 1 row 2) differs
        stages.put_slot(pages, d0['layers'][2]['start'] + 1, 32, bytes(512))                              # a layer 2 tile is gone
        parts.update({'module': bytes(module), 'pages': bytes(pages), 'palettes': bytes(palettes)})
        r = stages.analyse(parts, self.helper, 3, w.arc.prog)
        d = stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r)
        self.assertIsInstance(d, dict, d)
        self.assertEqual([l['flags'] & 3 for l in d['layers']], [3, 3, 1])           # layer 1's other tile code is an exception of the packing: still exact
        self.assertEqual(d['layers'][1]['exc'][0][0], 2)                            # the cell at byte 8 of the map comes first
        self.assertGreater(len(d['layers'][1]['exc']), 1)                           # the tile it first made lies on a slot it no longer uses: later cells say so
        self.assertEqual(d['layers'][2]['cell_known'], 0)                           # layer 2's map is exact
        self.assertEqual(bin(d['layers'][2]['tile_known']).count('1'), len(stages.model_stage_raw(w.arc, d)[1][2]) - 1)     # but one of its tiles is gone
        self.assertEqual((d['pal_exact'] >> 5) & 1, 0)
        self.assertEqual(bin(d['pal_exact']).count('1'), 95)

    def test_own_object_tiles_are_refused_unless_the_table_blanks_them(self):
        w = self.world
        d0 = w.descriptor(objects=[(0x10003, 2, 0)], count=96)
        d0['sizes']['module'] = 0x4000
        parts = make_stage_file(w, d0)
        own = dict(parts, **{'object tiles': parts['object tiles'] + bytes([0x37]) * 128})
        r = stages.analyse(own, self.helper, 3, w.arc.prog)
        refusal = stages.describe('TEST.PAC', 3, own, w.arc, self.helper, r)
        self.assertIn('1 object tiles have no arcade source, the blank-by-design table says 0', refusal)
        stages.BLANK_BY_DESIGN['TEST.PAC'] = (1, 'made up')
        try:
            d = stages.describe('TEST.PAC', 3, own, w.arc, self.helper, r)
        finally:
            del stages.BLANK_BY_DESIGN['TEST.PAC']
        self.assertIsInstance(d, dict, d)
        self.assertEqual(d['object_runs'][-1], (stages.RUN_BLANK, 1, 0))
        self.assertEqual(stages.model_stage(w.arc, d)['owned'], {'object tiles'})

    def test_cells_and_tiles_the_ps1_changed_are_found_and_counted(self):
        w = self.world
        d0 = w.descriptor(objects=[(0x10003, 2, 0)], count=96)
        d0['sizes']['module'] = 0x4000
        parts = make_stage_file(w, d0)
        layer = d0['layers'][1]
        cells = stages.layer_cells(w.arc, 1, stages.ids_of(w.arc, layer), 64, 16, 3)
        blank = [i for i, c in enumerate(cells) if not any(c[2])][:2]
        solid = [i for i, c in enumerate(cells) if any(c[2])]
        module, pages = bytearray(parts['module']), bytearray(parts['pages'])
        start = layer['start']

        def at(i):
            return layer['map'] + 2 * stages.map_word_index(cells[i][0], cells[i][1], 64, 16)
        other = struct.unpack_from('<H', module, at(solid[0]))[0]
        for i in blank:                                                        # the PS1 filled two blank cells with art
            struct.pack_into('<H', module, at(i), other)
        code, clut = struct.unpack_from('<HH', module, at(solid[1]))           # one cell in another palette row
        struct.pack_into('<HH', module, at(solid[1]), code, clut ^ (3 << 6))
        j = stages.slot_of_code(struct.unpack_from('<H', module, at(solid[2]))[0], 16) - start
        stages.put_slot(pages, start + j, 16, bytes(b ^ 0x11 for b in stages.get_slot(pages, start + j, 16)))      # one tile retouched
        bad = dict(parts, module=bytes(module), pages=bytes(pages))
        r = stages.analyse(bad, self.helper, 3, w.arc.prog)
        refusal = stages.describe('TEST.PAC', 3, bad, w.arc, self.helper, r)
        self.assertIn('layer 1: 2 blank cells, 1 cells in another row, 1 retouched tiles; the by-design table says (0, 0, 0)', refusal)
        stages.DESIGN_BY_DESIGN[('TEST.PAC', 1)] = (2, 1, 1, 'made up')
        try:
            d = stages.describe('TEST.PAC', 3, bad, w.arc, self.helper, r)
            self.assertIsInstance(d, dict, d)
            l1 = d['layers'][1]
            self.assertEqual((l1['own_cells'], l1['own_tiles']), (sorted(blank + [solid[1]]), [j]))
            # the model builds the arcade's cells there, owned, and none of them known; the rest of the layer equals the disc's
            model = stages.model_stage(w.arc, d)
            data, known = model['module']
            own = model['own']['module']
            for i in blank + [solid[1]]:
                self.assertEqual((known[at(i):at(i) + 4], own[at(i):at(i) + 4]), (bytes(4), b'\1' * 4))
            self.assertTrue(all(a == b for a, b, k in zip(data, bad['module'], known) if k))
            self.assertEqual(sum(model['own']['pages']), 128)
            self.assertEqual(sum(model['own']['pages']), sum(1 for o in model['own']['pages'] if o))
            text = stages.c_table([dict(d, file='TEST.PAC')], [(0x10002, 1, 0)], [])
            self.assertIn('3 cells are built as the arcade has them, not as the disc', text)
            self.assertIn('1 tiles the PS1 retouched are built as the arcade has them', text)
            self.assertIn('static const struct arcade_stage_span oc_', text)
        finally:
            del stages.DESIGN_BY_DESIGN[('TEST.PAC', 1)]
        self.assertEqual(stages.index_spans([1, 2, 3, 7, 9, 10]), [(1, 3), (7, 1), (9, 2)])
        self.assertTrue(all(len(v) == 4 and v[3] and chr(0x2014) not in v[3] for v in stages.DESIGN_BY_DESIGN.values()))

    def test_an_unreadable_module_is_reported(self):
        w = self.world
        d0 = w.descriptor(count=96)
        parts = make_stage_file(w, d0)
        parts['module'] = bytes(len(parts['module']))
        r = stages.analyse(make_stage_file(w, d0), self.helper, 3, w.arc.prog)
        self.assertEqual(stages.describe('TEST.PAC', 3, parts, w.arc, self.helper, r), 'layer maps not found in the module')


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
