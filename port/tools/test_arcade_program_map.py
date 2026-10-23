#!/usr/bin/env python3
"""Seconds: the pure functions of port/arcade/program_map.py on synthetic data; no game files.

Made-up arcade programs (seeded random bytes) and made-up PS1 data: the unit kinds (as it is, two bytes turned,
four bytes turned), growth and exact runs of a segment, the evidence, the ramp and fragment filters, the value
search across widths, pointers, text, cell-list chains, the whole classification of a small block, the ISO
reader on a tiny image, and the readers of the repository's tables.
"""
from pathlib import Path
import random, struct, sys, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import program_map as P


def arcade(n=8192, seed=1):
    return random.Random(seed).randbytes(n)


def turn2(b):
    return b''.join(b[i:i + 2][::-1] for i in range(0, len(b), 2))


def turn4(b):
    return b''.join(b[i:i + 4][::-1] for i in range(0, len(b), 4))


class Readers(unittest.TestCase):
    def test_symbols_inventory_pieces_build_ranges(self):
        syms = P.parse_symbols('a = 0x80170000;\nb = 0x80170004; // c\nbad line\nd=0x80170000;\n')
        self.assertEqual(syms, {0x80170000: ['a', 'd'], 0x80170004: ['b']})
        inv = P.parse_inventory('80118900\t8\tnoop\t-\t-\t0\t0\tclosed\nxx\t1\tbad\n80157090\t228\tf\n')
        self.assertEqual(inv, [(0x80118900, 8, 'noop'), (0x80157090, 228, 'f')])
        pieces = P.parse_pieces('    {0x8016ea70u,  2688u, 0x099500u, "b", "data_8016ea70+0x0"},\n  {0x801701bcu,    84u, 0x02508eu, "wb", "x"},\n')
        self.assertEqual(pieces, [(0x8016ea70, 2688, 0x99500, 'b', 'data_8016ea70+0x0'), (0x801701bc, 84, 0x2508e, 'wb', 'x')])
        toml = b'[[unit]]\nname="u"\nsource="sdk/libcd/bios.c"\nrodata = { address = 0x8016df14, size = 576 }\ndata = { address = 0x80181b34, size = 816 }\n[[unit]]\nname="v"\nsource="resident/x.c"\n'
        self.assertEqual(P.parse_build_ranges(toml), [(0x8016df14, 576, 'rodata', 'u', 'sdk/libcd/bios.c'), (0x80181b34, 816, 'data', 'u', 'sdk/libcd/bios.c')])

    def test_iso_reader_walks_directories(self):
        def sector(lba):
            return image.get(lba, bytes(2048))

        def rec(name, lba, size, flags):
            nm = name.encode()
            body = bytes([0, 0]) + struct.pack('<I', lba) + struct.pack('>I', lba) + struct.pack('<I', size) + struct.pack('>I', size)
            body += bytes(7) + bytes([flags]) + bytes(6) + bytes([len(nm)]) + nm
            body = bytes([34 + len(nm) - 1 + (len(nm) % 2 == 0)]) + body[1:]
            return body.ljust(body[0], b'\0')
        file_data = b'PS-X' + bytes(range(60)) + bytes(2100)
        root_dir = rec('\0', 20, 2048, 2) + rec('SUB', 21, 2048, 2)
        sub_dir = rec('\0', 21, 2048, 2) + rec('SLPS_004.15;1', 22, len(file_data), 0)
        pvd = bytes(156) + rec('\0', 20, 2048, 2)[:34]
        image = {16: pvd.ljust(2048, b'\0'), 20: root_dir.ljust(2048, b'\0'), 21: sub_dir.ljust(2048, b'\0'),
                 22: file_data[:2048], 23: file_data[2048:].ljust(2048, b'\0')}
        self.assertEqual(P.iso_read_file(sector, 'SLPS_004.15'), file_data)
        self.assertIsNone(P.iso_read_file(sector, 'NOPE.BIN'))

    def test_swap_pairs(self):
        self.assertEqual(P.swap_pairs(b'\x01\x02\x03\x04'), b'\x02\x01\x04\x03')


class Search(unittest.TestCase):
    def test_unit_kinds(self):
        m = bytes([0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0])
        self.assertEqual(P.unit_kind(bytes([0x12, 0x34]), m, 0, 0), ('b', 2))
        self.assertEqual(P.unit_kind(bytes([0x34, 0x12]), m, 0, 0), ('w', 2))
        self.assertEqual(P.unit_kind(bytes([0x78, 0x56, 0x34, 0x12]), m, 0, 0), ('l', 4))
        self.assertIsNone(P.unit_kind(bytes([1, 2]), m, 0, 0))
        self.assertIsNone(P.unit_kind(bytes([0x34, 0x12]), m, 0, 100))      # outside
        # four bytes turned only at a multiple of 4: at offset 2 the unit is not 'l'
        q = bytes([0, 0, 0x78, 0x56, 0x34, 0x12])
        self.assertNotEqual(P.unit_kind(q, m, 2, 0), ('l', 4))

    def make(self):
        m = arcade()
        q = bytearray(random.Random(2).randbytes(256))
        src = 1000
        # 64 bytes (16 words) of 32-bit fields turned, one word changed, then 32 bytes with 16-bit fields turned
        q[64:128] = turn4(m[src:src + 64])
        q[100:104] = b'\xde\xad\xbe\xef'
        q[128:160] = turn2(m[src + 64:src + 96])
        return bytes(q), m, src

    def test_segments_exact_runs_and_rebuild(self):
        q, m, src = self.make()
        stats = P.UnitStats(m)
        segs = P.find_segments(q, m, stats)
        best = max(segs, key=lambda s: s[4])
        s, e, d = best[0], best[1], best[2]
        self.assertEqual((s, e, d), (64, 160, src - 64))
        self.assertEqual(best[5], 2)                       # the changed word is two 16-bit units
        self.assertGreater(best[3], 40)
        runs = P.exact_runs(best[6], s)
        self.assertEqual([(o, n) for o, n, _l in runs], [(64, 36), (104, 56)])
        for o, n, _lay in runs:
            units = P.simplify_layout(q, m, o, n, d)
            lay = units or P.minimal_period(_lay)
            self.assertEqual(P.build_run(m, o + d, n, lay), q[o:o + n])
        self.assertEqual(P.simplify_layout(q, m, 64, 36, d), 'l')
        self.assertEqual(P.simplify_layout(q, m, 128, 32, d), 'w')
        self.assertEqual(P.minimal_period('lblblb'), 'lb')
        self.assertEqual(P.minimal_period('lbl'), 'lbl')

    def test_random_data_finds_nothing(self):
        m = arcade()
        q = random.Random(9).randbytes(512)
        stats = P.UnitStats(m)
        self.assertEqual([s for s in P.find_segments(q, m, stats) if s[3] >= 8], [])

    def test_selection_keeps_the_strongest_and_updates_claims(self):
        q, m, src = self.make()
        stats = P.UnitStats(m)
        segs = P.find_segments(q, m, stats)
        claimed = bytearray(len(q))
        claimed[64:72] = b'\1' * 8                       # a piece already owns the first 8 bytes
        kept = P.select_segments_data(q, m, segs, claimed, stats)
        self.assertTrue(kept)
        self.assertGreaterEqual(min(k[0] for k in kept), 72)
        self.assertTrue(all(claimed[k[0]:k[1]] == b'\1' * (k[1] - k[0]) for k in kept))

    def test_tiers_and_plausibility(self):
        self.assertEqual(P.tier(50), 'arcade_hi')
        self.assertEqual(P.tier(25), 'arcade_med')
        self.assertEqual(P.tier(10), 'arcade_lo')
        self.assertEqual(P.tier(14, matched=161, mismatching=0, distinct=6), 'arcade_hi')    # long exact run, low entropy
        self.assertEqual(P.tier(14, matched=161, mismatching=0, distinct=3), 'arcade_lo')    # but not a degenerate one
        syms = {0x80170000, 0x80170010}
        self.assertTrue(P.plausible(0, 40, 0x80170000, syms))
        self.assertTrue(P.plausible(0, 16, 0x80170000, syms))        # a whole small symbol
        self.assertFalse(P.plausible(4, 20, 0x80170000, syms))       # a fragment inside one
        self.assertFalse(P.plausible(0, 8, 0x80170000, syms))

    def test_ramps(self):
        self.assertTrue(P.is_ramp([0x4f + i for i in range(15)]))
        self.assertTrue(P.is_ramp([0x47, 0, 0x49, 0, 0x4b, 0, 0x4d, 0]))
        self.assertFalse(P.is_ramp([5, 6, 10, 7, 8, 12, 10, 15, 7]))
        q = b''.join(struct.pack('<H', 0x4f + i) for i in range(16))
        self.assertTrue(P.segment_is_ramp(q, 0, len(q)))
        r = random.Random(3).randbytes(32)
        self.assertFalse(P.segment_is_ramp(r, 0, len(r)))

    def test_value_search_across_widths(self):
        m = bytearray(arcade(4096, seed=5))
        vals = [0x0d, 0x0c, 0x00, 0x0e, 0x11, 0x0a, 0x01, 0x09, 0x0f, 0x02, 0x03, 0x05, 0x07, 0x0b, 0x06, 0x10]
        m[600:600 + len(vals)] = bytes(vals)                        # arcade bytes
        q = bytearray(random.Random(6).randbytes(256))
        q[64:64 + 4 * len(vals)] = b''.join(struct.pack('<I', v) for v in vals)    # PS1 32-bit values
        usable = bytearray(b'\1' * len(q))
        out = P.find_value_segments(bytes(q), bytes(m), usable, P.value_stats(bytes(m)))
        hit = [o for o in out if o[2] == 4 and o[3] == 1 and o[0] <= 64 and o[1] >= 64 + 4 * len(vals) - 8]
        self.assertTrue(hit, out)
        self.assertGreater(hit[0][5], 20)
        # a ramp is not reported
        m2 = bytearray(arcade(4096, seed=5)); m2[600:616] = bytes(range(0x40, 0x50))
        q2 = bytearray(random.Random(6).randbytes(256)); q2[64:64 + 64] = b''.join(struct.pack('<I', 0x40 + i) for i in range(16))
        out2 = P.find_value_segments(bytes(q2), bytes(m2), usable, P.value_stats(bytes(m2)))
        self.assertEqual([o for o in out2 if o[2] == 4 and o[3] == 1 and o[0] <= 64 + 8 and o[1] >= 64 + 32], [])


class Structures(unittest.TestCase):
    def test_pointers(self):
        words = [0x80120000, 0x801206c4, 0x80010008, 0x00001000, 0x80078008, 0x00002000, 0x801b0000, 0x801b0004]
        q = b''.join(struct.pack('<I', w) for w in words)
        cands = P.pointer_candidates(q, 0x8016e000)
        self.assertEqual(cands, [0, 4, 8, 16, 24, 28])
        func_starts = {0x80120000}
        strong = P.strong_pointers(q, cands, func_starts, set())
        self.assertEqual(strong, [0, 4, 8, 24, 28])        # 0x80078008 (offset 16) is alone and no symbol: weak
        fa = [0x80120000]

        def func_at(w):
            return 0x80120000 <= w < 0x80120800
        self.assertEqual(P.pointer_kind(0x80120000, func_starts, func_at, set()), 'ptr_code')
        self.assertEqual(P.pointer_kind(0x801206c4, func_starts, func_at, set()), 'jtbl')
        self.assertEqual(P.pointer_kind(0x80170100, func_starts, func_at, set()), 'ptr_data')
        self.assertEqual(P.pointer_kind(0x801b0000, func_starts, func_at, set()), 'ptr_module')
        self.assertEqual(P.pointer_kind(0x80010008, func_starts, func_at, set()), 'ptr_other')

    def test_text(self):
        self.assertTrue(P.is_text(b'HIT COMBO       /'))
        self.assertTrue(P.is_text(b'        @@'))
        self.assertTrue(P.is_text(b'FONT.TIM'))
        self.assertFalse(P.is_text(b'BCDDDDDDDD'))
        self.assertFalse(P.is_text(b'(((((((((((('))
        self.assertFalse(P.is_text(b'abc'))
        data = b'\0\0HIT COMBO\0\0\0\0\x1d\x90REVERSAL  \0\0' + bytes(40) + b'NORMAL MODE  @@'
        runs = P.text_runs(data)
        self.assertEqual([(a, b) for a, b in runs], [(2, data.index(b'  \0\0') + 2), (data.index(b'NORMAL'), len(data))])
        self.assertEqual(P.text_runs(b'\0BCDDDDDDDD\0((((((((\0'), [])

    def lists(self, count):
        out = b''
        tile = 1
        for k in range(count):
            n = 3 + k % 4
            out += struct.pack('<5H', n, 4, 0x15, 0, k * 6) + struct.pack('<HH', tile, n - 1)
            tile += n
        return out

    def test_cell_lists(self):
        one = struct.pack('<5H', 3, 4, 0x15, 0, 0) + struct.pack('<HH', 1, 2)
        self.assertEqual(P.parse_cell_list(one, 0), (3, 4, 0, len(one)))
        self.assertIsNone(P.parse_cell_list(struct.pack('<5H', 3, 5, 0x15, 0, 0) + bytes(8), 0))        # mode 5
        self.assertIsNone(P.parse_cell_list(struct.pack('<5H', 3, 4, 0x15, 1, 0) + bytes(8), 0))        # nonzero word
        self.assertIsNone(P.parse_cell_list(struct.pack('<5H', 4, 4, 0x15, 0, 0) + struct.pack('<HH', 1, 2), 0))   # run says 3 cells, header 4
        mode0 = struct.pack('<5H', 2, 0, 3, 0, 0) + struct.pack('<2H', 7, 8)
        self.assertEqual(P.parse_cell_list(mode0, 0), (2, 0, 0, len(mode0)))
        chain = self.lists(40)
        q = bytes(16) + chain + bytes(16)
        usable = bytearray(b'\1' * len(q))
        found = P.find_cell_chains(q, usable)
        self.assertEqual(len(found), 1)
        self.assertEqual((found[0][0], found[0][1], found[0][2]), (16, 16 + len(chain), 40))
        self.assertEqual(found[0][3], 39 * 6)
        self.assertEqual(P.find_cell_chains(q, usable, min_lists=50), [])
        self.assertEqual(P.find_cell_chains(bytes(400), bytearray(b'\1' * 400)), [])

    def test_classify_a_small_block(self):
        base = P.DATA_START
        m = arcade()
        syms = {base + 0: ['first'], base + 0x100: ['table_a'], base + 0x200: ['handlers']}
        funcs = [(0x80120000, 64, 'fn')]
        q = bytearray(0x300)
        q[0x00:0x40] = turn2(m[1000:1064])                    # a piece (given below)
        q[0x100:0x140] = turn4(m[2000:2064])                  # an arcade table at another width, not a piece
        q[0x140:0x150] = b'HIT COMBO  REVERSAL'[:16]          # text
        struct.pack_into('<4I', q, 0x200, 0x80120000, 0x80120000, 0x801b0000, 0x801b0100)     # pointers
        q[0x240:0x250] = random.Random(11).randbytes(16)      # other
        pieces = [(base, 0x40, 1000, 'w', 'first')]
        ranges = [(base + 0x2f0, 8, 'rodata', 'u', 'sdk/libcd/bios.c')]
        cls, details = P.classify(bytes(q), base, m, pieces, syms, funcs, ranges)
        self.assertEqual(set(cls[0x00:0x40]), {'piece'})
        self.assertEqual(set(cls[0x100:0x140]), {'arcade_hi'})
        self.assertEqual(set(cls[0x140:0x150]), {'text'})
        self.assertEqual(cls[0x200:0x20c], ['ptr_code'] * 8 + ['ptr_module'] * 4)
        self.assertEqual(set(cls[0x2f0:0x2f8]), {'sdk'})
        self.assertEqual(set(cls[0x240:0x250]), {'other'})
        self.assertEqual(cls[0x2a0], 'zero')
        self.assertEqual(len(details['segments']), 1)
        counts = dict(P.summarize(cls))
        self.assertEqual(sum(counts.values()), len(q))
        self.assertEqual(counts['piece'], 0x40)

    def test_symbol_ranges_and_tables(self):
        syms = {0x80170000: ['a'], 0x80170010: ['b'], 0x80170018: ['c'], 0x90000000: ['far']}
        self.assertEqual(P.symbol_ranges(syms, 0x8016fffc, 0x80170020),
                         [(0x8016fffc, 0x80170000, '(start of data)'), (0x80170000, 0x80170010, 'a'),
                          (0x80170010, 0x80170018, 'b'), (0x80170018, 0x80170020, 'c')])
        cls = ['zero'] * 4 + ['text'] * 4
        rows = P.table_rows(cls, [(0x100, 0x104, 'x'), (0x104, 0x108, 'y')], 0x100)
        self.assertEqual([(a, size, n, dict(c)) for a, size, n, c in rows], [(0x100, 4, 'x', {'zero': 4}), (0x104, 4, 'y', {'text': 4})])
        self.assertEqual(P.zero_run_bytes(['zero'] * 20 + ['other'] * 4, bytes(20) + b'\1' * 4), 20)
        self.assertEqual(P.zero_run_bytes(['zero'] * 8, bytes(8)), 0)

    def test_nearest_symbol(self):
        self.assertEqual(P.nearest_symbol({0x80170000: ['a'], 0x80170010: ['b']}, 0x80170014), 'b+0x4')


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
