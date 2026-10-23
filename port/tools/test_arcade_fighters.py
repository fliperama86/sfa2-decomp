#!/usr/bin/env python3
"""Seconds: the fighter conversion tool's pure functions on made-up data; no disc, no arcade set.

Checks port/arcade/fighters.py: block expansion with flips, the art record reader, cell order, the tile numbering,
position runs and their streams (identical streams stored once), list encoding in all four modes with both ties,
list reading, the matching of tile classes, the kinds of difference, the code end and the chain finders, and the
C tile helper on random tiles. One small made-up world (an arcade program, its tiles, a PS1 module and a PS1 tile
chunk) then goes through the whole analysis, with and without a tile bank, and must come out equal.
"""
from pathlib import Path
import os, random, struct, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import fighters as F

JR_RA = struct.pack('<I', 0x03E00008)


def tile_bytes(px):
    """The arcade's 128 bytes for 256 pixel values 0..15 as the PS1 stores them (arcade index + 1, modulo 16)."""
    out = bytearray(128)
    for y in range(16):
        for x in range(16):
            b = (px[y * 16 + x] - 1) & 15
            for pl in range(4):
                out[y * 8 + (x // 8) * 4 + pl] |= ((b >> pl) & 1) << (7 - x % 8)
    return bytes(out)


def ps1_chunk(tiles_px):
    """A PS1 sprite chunk of literal tiles (word count 0 then 64 words), with its table of offsets."""
    head, body = bytearray(), bytearray()
    base = 4 * len(tiles_px)
    for px in tiles_px:
        head += struct.pack('<I', base + len(body))
        body += struct.pack('<H', 0) + bytes(px[2 * k] | px[2 * k + 1] << 4 for k in range(128))
    return bytes(head + body)


def flipped(px, fl):
    return [px[(15 - y if fl & 2 else y) * 16 + (15 - x if fl & 1 else x)] for y in range(16) for x in range(16)]


class Pure(unittest.TestCase):
    def test_expand_block_with_flips(self):
        # a block of 2 x 2 tiles (attribute bits 8-11 = 1, 12-15 = 1) at code 100, position (-16, 32)
        plain = F.expand_block(100, 0x1100, -16, 32)
        self.assertEqual(plain, [(100, -16, 32, 0), (101, 0, 32, 0), (116, -16, 48, 0), (117, 0, 48, 0)])
        fx = F.expand_block(100, 0x1120, -16, 32)      # flip x reverses the codes in a row, the positions stay
        self.assertEqual([c[0] for c in fx], [101, 100, 117, 116])
        self.assertEqual([(c[1], c[2]) for c in fx], [(-16, 32), (0, 32), (-16, 48), (0, 48)])
        self.assertTrue(all(c[3] == 0x20 for c in fx))
        fy = F.expand_block(100, 0x1140, 0, 0)
        self.assertEqual([c[0] for c in fy], [116, 117, 100, 101])
        both = F.expand_block(100, 0x1161, 0, 0)       # flags keep the palette and both flip bits
        self.assertEqual([c[0] for c in both], [117, 116, 101, 100])
        self.assertTrue(all(c[3] == 0x61 for c in both))
        wide = F.expand_block(0, 0x0200, 0, 0)         # 3 x 1
        self.assertEqual([(c[0], c[1]) for c in wide], [(0, 0), (1, 16), (2, 32)])

    def make_view(self):
        data = bytearray(0xa0000)
        def put(o, *words):
            for i, w in enumerate(words):
                struct.pack_into('>H', data, o + 2 * i, w & 0xffff)
        # record at 0x1000: mode 2, 5 cells, 2 blocks, coordinate index 8; blocks: a 1 x 1 and a 2 x 2
        put(0x1000, 2, 5, 1, 8, 0x0123, 0x0003, 0x0200, 0x1102)
        struct.pack_into('>I', data, F.COORD_TABLE + 8, 0x2000)
        put(0x2000, -16, -32, 24, 8)
        # a record of the wrong mode and one whose coordinates lie outside
        put(0x1100, 4, 5, 1, 8, 0, 0, 0, 0)
        put(0x1200, 2, 1, 0, 12, 0, 0)
        struct.pack_into('>I', data, F.COORD_TABLE + 12, 0xffffff0)
        return F.View(bytes(data))

    def test_read_record(self):
        v = self.make_view()
        cells, out, end = F.read_record(v, 0x1000)
        self.assertEqual((cells, end), (5, 0x1010))
        self.assertEqual(out[0], (0x0123, -16, -32, 3))
        self.assertEqual([c[0] for c in out[1:]], [0x0200, 0x0201, 0x0210, 0x0211])
        self.assertEqual([(c[1], c[2]) for c in out[1:]], [(24, 8), (40, 8), (24, 24), (40, 24)])
        self.assertEqual(F.block_cells(v, 0x1000, 2), 5)
        self.assertIsNone(F.read_record(v, 0x1100))
        self.assertIsNone(F.read_record(v, 0x1200))
        self.assertIsNone(F.read_record(v, 0x1001))
        self.assertIsNone(F.read_record(v, len(v.data)))

    def test_sort_cells_by_y_then_x_and_keeps_ties(self):
        cells = [(1, 16, 0, 0), (2, -16, 16, 0), (3, 0, 0, 0), (4, 0, 0, 0), (5, -16, 0, 0)]
        self.assertEqual([c[0] for c in F.sort_cells(cells)], [5, 3, 4, 1, 2])

    def test_numbering(self):
        n = F.Numbering()
        self.assertEqual([n.number(c) for c in (50, 60, 50, 70, 60)], [1, 2, 1, 3, 2])
        self.assertEqual(n.next, 4)
        # numbers taken by a list that could not be converted are claimed by the first code of the same pixels
        n.reserve(6, 'a')
        self.assertEqual(n.next, 7)
        self.assertEqual(n.number(80, 'b'), 7)
        self.assertEqual(n.number(81, 'a'), 6)
        self.assertEqual(n.number(81, 'a'), 6)
        self.assertEqual(n.next, 8)

    def test_position_runs_and_stream(self):
        cells = [(1, -32, -16, 0), (2, -16, -16, 0), (3, 0, -16, 0), (4, 32, -16, 0), (5, -32, 0, 0)]
        runs = F.position_runs(cells)
        self.assertEqual(runs, [[-32, -16, 2], [32, -16, 0], [-32, 0, 0]])
        # dx from the last tile of the previous run (0 for the first), dy from its y, then length - 1
        self.assertEqual(F.position_stream(runs), struct.pack('<hhHhhHhhH', -32, -16, 2, 32, 0, 0, -64, 16, 0))
        self.assertEqual(F.position_runs([(1, 0, 0, 0), (2, 16, 16, 0)]), [[0, 0, 0], [16, 16, 0]])

    def test_positions_are_stored_once_in_order_of_first_use(self):
        p = F.Positions()
        a, b = b'\x01' * 6, b'\x02' * 12
        self.assertEqual([p.add(a), p.add(b), p.add(a), p.add(b)], [0, 6, 0, 6])
        self.assertEqual(bytes(p.data), a + b)

    def words(self, b):
        return list(struct.unpack('<%dH' % (len(b) // 2), b))

    def test_encode_plain_modes_and_ties(self):
        # shorter wins between mode 0 (n words) and mode 4 (2 words a run); the third word is the palette
        self.assertEqual(self.words(F.encode_list([(1, 3), (3, 3), (5, 3)], 9)), [3, 0, 3, 0, 9, 1, 3, 5])
        self.assertEqual(self.words(F.encode_list([(1, 3), (2, 3), (3, 3), (4, 3)], 0)), [4, 4, 3, 0, 0, 1, 3])
        # a tie (4 cells, 2 runs: 4 words each) goes to mode 4
        self.assertEqual(self.words(F.encode_list([(1, 2), (2, 2), (5, 2), (6, 2)], 0)), [4, 4, 2, 0, 0, 1, 1, 5, 1])
        self.assertEqual(self.words(F.encode_list([(7, 0)], 0)), [1, 0, 0, 0, 0, 7])    # one cell: 1 word against 2

    def test_encode_flag_modes_and_ties(self):
        fl = 0x20
        # shorter wins between mode 2 (2 words a cell) and mode 6 (3 words a run); the third word is cells - 1
        self.assertEqual(self.words(F.encode_list([(1, fl), (2, fl), (3, fl)], 4)), [3, 6, 2, 0, 4, 1, fl, 2])
        self.assertEqual(self.words(F.encode_list([(1, fl), (5, 0x21)], 0)), [2, 2, 1, 0, 0, 1, fl, 5, 0x21])
        # a tie (3 cells, 2 runs: 6 words each) goes to mode 6
        self.assertEqual(self.words(F.encode_list([(1, fl), (2, fl), (7, fl)], 0)), [3, 6, 2, 0, 0, 1, fl, 1, 7, fl, 0])
        # equal numbers in a run but another palette split the run; no flip bit but two flag values is a flag mode
        self.assertEqual(self.words(F.encode_list([(1, 1), (2, 2)], 0)), [2, 2, 1, 0, 0, 1, 1, 2, 2])
        # a flip bit alone (vertical) is a flag mode
        self.assertEqual(self.words(F.encode_list([(1, 0x40)], 0))[:2], [1, 2])

    def test_parse_list_round_trip_and_refusals(self):
        for cells in ([(1, 3), (3, 3), (5, 3)], [(1, 3), (2, 3), (3, 3), (4, 3)], [(1, 0x20), (2, 0x20), (3, 0x20)],
                      [(1, 0x20), (5, 0x21)], [(1, 0x20), (2, 0x20), (7, 0x20)]):
            lst = F.parse_list(F.encode_list(cells, 6), 0)
            self.assertEqual([t for t, _f in lst['cells']], [t for t, _f in cells])
            self.assertEqual((lst['n'], lst['pos']), (len(cells), 6))
        good = bytearray(F.encode_list([(1, 3), (2, 3)], 0))
        for i, v in ((2, 3), (6, 1)):                      # a mode that is not 0, 2, 4 or 6; the zero word not zero
            bad = bytearray(good)
            struct.pack_into('<H', bad, i, v)
            self.assertIsNone(F.parse_list(bytes(bad), 0))
        self.assertIsNone(F.parse_list(bytes(good[:-2]), 0))                      # truncated
        over = F.encode_list([(1, 3), (2, 3), (3, 3)], 0)
        bad = bytearray(over)
        struct.pack_into('<H', bad, 0, 2)                                         # a run longer than the count
        self.assertIsNone(F.parse_list(bytes(bad), 0))
        self.assertIsNone(F.parse_list(struct.pack('<5H', 0, 0, 0, 0, 0), 0))      # no cells

    def test_empty_lists_and_palettes(self):
        e = F.parse_list(struct.pack('<6H', 1, 0, 0, 0, 40, 0), 0)
        self.assertTrue(F.is_empty(e))
        self.assertFalse(F.is_empty(F.parse_list(F.encode_list([(1, 0)], 0), 0)))
        self.assertEqual(F.list_palettes(F.parse_list(F.encode_list([(1, 3), (2, 3)], 0), 0)), [3, 3])
        self.assertEqual(F.list_palettes(F.parse_list(F.encode_list([(1, 0x21), (2, 0x02)], 0), 0)), [1, 2])

    def test_multiset_match(self):
        m = F.multiset_match
        self.assertTrue(m([(5, ()), (6, ())], [6, 5]))
        self.assertFalse(m([(5, ()), (6, ())], [6, 6]))
        self.assertFalse(m([(5, ())], [5, 5]))                     # one arcade tile too many
        self.assertTrue(m([(-1, ()), (5, ())], [5, 99]))           # a tile of unknown class stands for any one
        self.assertFalse(m([(-1, ())], [5, 99]))
        self.assertTrue(m([(-1, (7,))], [7]))                      # not its own class, but flipped it is arcade tile 7
        self.assertFalse(m([(-1, (7,))], [8]))                     # a tile with flipped candidates must match one
        self.assertTrue(m([(5, (7,)), (-1, (5,))], [5, 5]))        # the second goes by its flipped class

    def test_diff_kind(self):
        base = F.encode_list([(1, 3), (2, 3), (3, 3)], 6)
        self.assertEqual(F.diff_kind(base, F.encode_list([(1, 3), (2, 3), (4, 3)], 6)), 'numbering')
        self.assertEqual(F.diff_kind(base, F.encode_list([(1, 3), (2, 3), (3, 3)], 12)), 'position')
        flipped_ = F.encode_list([(1, 0x23), (2, 0x23), (3, 0x23)], 6)
        self.assertEqual(F.diff_kind(flipped_, base), 'flips')
        self.assertEqual(F.diff_kind(F.encode_list([(1, 4), (2, 4), (3, 4)], 6), base), 'palette')
        same_cells = F.encode_list([(1, 3), (2, 3), (3, 3)], 6)
        plain0 = struct.pack('<5H3H', 3, 0, 3, 0, 6, 1, 2, 3)
        self.assertEqual(F.diff_kind(plain0, same_cells), 'encoding')
        self.assertEqual(F.diff_kind(b'\x00' * 12, base), 'structure')

    def test_code_end_and_stream_end(self):
        mod = bytes(8) + JR_RA + bytes(4) + bytes(4) + JR_RA + bytes(8) + b'\x01\x02\x03\x04'
        self.assertEqual(F.code_end(mod), 28)                                # the last `jr ra` is at 20; its delay slot ends at 28
        self.assertIsNone(F.code_end(bytes(64)))
        s = struct.pack('<hhHhhH', 0, 0, 2, 16, 16, 0)
        self.assertEqual(F.stream_end(s, 0, 4), 12)
        self.assertEqual(F.stream_end(s, 0, 3), 6)                           # the first run alone covers 3 cells
        self.assertIsNone(F.stream_end(s, 0, 5))
        self.assertIsNone(F.stream_end(s, 0, 2))                             # no run ends on 2 cells

    def chain_module(self, count, positions_first):
        code = bytes(4) + JR_RA + bytes(4)
        pos = bytearray()
        lists = bytearray()
        for i in range(count):
            at = len(pos) // 6 * 6
            pos += struct.pack('<hhH', -16, 16 * i, 1)
            lists += F.encode_list([(2 * i + 1, 1), (2 * i + 2, 1)], at)
        if positions_first:
            return code + bytes(pos) + bytes(lists), len(code), len(code) + len(pos)
        return code + bytes(lists) + bytes(pos), len(code), len(code)

    def test_chain_finders(self):
        mod, start, lbase = self.chain_module(30, True)
        self.assertEqual(F.code_end(mod), start)
        found = F.find_chain(mod, start)
        self.assertEqual((found[0], len(found[1])), (lbase, 30))
        self.assertEqual([l['at'] for l in found[1]][:2], [lbase, lbase + 14])
        self.assertIsNone(F.find_chain_first(mod, start))                # positions first: nothing at the code end
        mod, start, lbase = self.chain_module(30, False)                 # lists first, the positions after them
        first = F.find_chain_first(mod, start)
        self.assertEqual((first[0], len(first[1])), (start, 30))
        self.assertEqual(F.chain_from(mod, start)[-1]['end'], start + 30 * 14)
        short, start, _ = self.chain_module(F.MIN_CHAIN - 1, True)
        self.assertIsNone(F.find_chain(short, start))


class Helper(unittest.TestCase):
    def test_tile_classes_and_flipped_tiles(self):
        rnd = random.Random(7)
        arc = [[rnd.randrange(16) for _ in range(256)] for _ in range(5)]
        arc[3] = list(arc[1])                                            # a duplicate: both belong to class 1
        gfx = b''.join(tile_bytes(px) for px in arc)
        tiles = [[rnd.randrange(16) for _ in range(256)],               # tile 0: in no arcade tile
                 arc[1],                                                  # equal to arcade tiles 1 and 3
                 flipped(arc[2], 2),                                      # arcade tile 2 flipped down
                 flipped(arc[4], 1),                                      # arcade tile 4 flipped across
                 flipped(arc[0], 3)]                                      # arcade tile 0 flipped both ways
        with tempfile.TemporaryDirectory(prefix='fighters-test-') as tmp:
            h = F.Helper(tmp, gfx)
            self.assertEqual(h.canon, [0, 1, 2, 1, 4])
            ex, fl = h.classify(ps1_chunk(tiles))
        self.assertEqual(ex, [-1, 1, -1, -1, -1])
        self.assertEqual(fl[3:6], [-1, -1, -1])
        self.assertEqual(fl[6:9], [-1, 2, -1])
        self.assertEqual(fl[9:12], [4, -1, -1])
        self.assertEqual(fl[12:15], [-1, -1, 0])


class World(unittest.TestCase):
    """A made-up fighter: 24 art records of one 2 x 1 block, the arcade tiles, a PS1 module and a PS1 tile chunk."""

    def build(self, bank):
        rnd = random.Random(3)
        rec_codes = [2 * r for r in range(24)]
        ntiles = bank + 64
        px = {c: [rnd.randrange(16) for _ in range(256)] for c in range(48)}
        gfx = bytearray(ntiles * 128)
        for c, p in px.items():
            gfx[(bank + c) * 128:(bank + c + 1) * 128] = tile_bytes(p)
        # the rest of the arcade tiles must not equal each other, or the classes would merge; random bytes do not
        rest = random.Random(5).randbytes(128 * 16)
        gfx[(bank + 48) * 128:(bank + 64) * 128] = rest
        if bank:
            gfx[:128 * 64] = random.Random(9).randbytes(128 * 64)
        prog = bytearray(0xa0000)
        for r, c in enumerate(rec_codes):
            a = 0x1000 + 12 * r
            struct.pack_into('>HHHhHH', prog, a, 2, 2, 0, 4 * r, c, 0x0100)
            struct.pack_into('>I', prog, F.COORD_TABLE + 4 * r, 0x3000 + 4 * r)
            struct.pack_into('>hh', prog, 0x3000 + 4 * r, -16, 16 * (r % 3))
        # the PS1 tiles: number n is the arcade tile n - 1
        tiles = [[rnd.randrange(16) for _ in range(256)]] + [px[n - 1] for n in range(1, 49)]
        # the module: code, positions (three streams), lists with an empty one after the fifth, the terminator
        code = bytes(8) + JR_RA + bytes(4)
        streams = [struct.pack('<hhH', -16, 16 * y, 1) for y in range(3)]
        positions = b''.join(streams)
        lists, empties = b'', 0
        for r in range(24):
            lists += struct.pack('<7H', 2, 4, 0, 0, 6 * (r % 3), 2 * r + 1, 1)
            if r == 4:
                lists += struct.pack('<6H', 1, 0, 0, 0, len(positions), 0)
                empties += 1
        lists += struct.pack('<6H', 1, 0, 0, 0, len(positions), 0)
        pad = bytes(-(len(code) + len(positions)) % 8)
        return F.View(bytes(prog)), bytes(gfx), ps1_chunk(tiles), code + positions + pad + lists, len(code), len(code) + len(positions) + len(pad)

    def run_world(self, bank):
        view, gfx, tiles, mod, pbase, lbase = self.build(bank)
        with tempfile.TemporaryDirectory(prefix='fighters-test-') as tmp:
            helper = F.Helper(tmp, gfx)
            r = F.analyse(mod, tiles, view, F.Index(view), helper)
        self.assertNotIn('error', r)
        self.assertEqual((r['codeend'], r['posbase'], r['lbase']), (pbase, pbase, lbase))
        self.assertEqual(len(r['lists']), 25)                               # 24 and the empty one in the middle; the terminator is apart
        self.assertIsNotNone(r['term'])
        self.assertTrue(r['term_ok'])
        self.assertEqual(r['segments'], [(0x1000, 24, 0, bank)])
        self.assertEqual((r['tiles_ok'], r['tiles_of']), (24, 24))
        self.assertEqual((r['bytes_equal'], r['first_bad'], r['pos_diff']), (25, None, None))
        self.assertEqual((r['baked'], r['wrong'], r['unmatched'], r['numbers']), ({}, {}, [], 48))
        return r

    def test_whole_analysis_without_a_bank(self):
        self.run_world(0)

    def test_whole_analysis_with_a_bank(self):
        self.run_world(0x10000)

    def test_report_lines_are_short_and_plain(self):
        r = self.run_world(0)
        line = F.line('PL99.PAC', 1, r)
        self.assertIn('26 lists, segments 0x1000+24', line)
        self.assertIn('positions 18 B equal', line)
        self.assertIn('list bytes all equal', line)
        self.assertNotIn(chr(0x2014), line)
        self.assertLessEqual(len(F.details('PL99.PAC', r)), 6)


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
