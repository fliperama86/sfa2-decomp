#!/usr/bin/env python3
"""Seconds: the rest of a fighter module's data (port/arcade/module_data.py and module_data.c); no game files.

Python side, on made-up data: the generated tables' parsers, the five blocks' spans, exact runs of 16-bit and 32-bit fields (as they are,
turned), zero trimming, the layout with the fewest letter changes and its periodic form, the layout text (tokens, groups, malformed
ones), the field letters (n, f, q and the others, with what does not fit), the converted records (templates: the inverse of the field
letters, the search, the pieces, a second side that differs), the search of the arcade program for a module's tables, the pointer words
and their classes, the byte classes, the model of the C and the text of the generated table; the palette letters (k, z, K, Z) against
screens.py's rule, the palette search, the screen modules' analysis on a made-up module (what is code or the loader's is kept out) and
the parsers of the tables that tell which bytes those are.

C side: module_data.c, built with the address and undefined-behavior sanitizers, against the model on made-up programs and layouts
(every letter, groups, a value that does not fit, a layout that makes the wrong size, a piece outside the program, digits that would
overflow), against program.c's piece builder for the letters they share, rows (pieces and words, a bad piece skipped and the others
built, a wrong module size), the validity checks, the checked-in table (45 fighter modules, no piece or word inside a block that
fighter.c or sequence.c builds), and a 32-bit build.
"""
from pathlib import Path
import collections, random, shutil, struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import module_data as M
import program_map as P
import screens as SC

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SOURCES = [ROOT / 'port/arcade' / n for n in ('module_data.c', 'module_data_table.c', 'program.c', 'fighters_table.c', 'sequences_table.c')]
DRIVER = ROOT / 'port/arcade/tests/module_data_test.c'


def turn(b, k):
    return b''.join(b[i:i + k][::-1] for i in range(0, len(b), k))


def arcade(size=0x4000, seed=7):
    """A made-up arcade program of random bytes."""
    r = random.Random(seed)
    return bytes(r.randrange(256) for _ in range(size))


def field_inverse(letter, want):
    """The arcade bytes that give the PS1 bytes `want` under the letter."""
    return M.record_expected(want, letter)


def random_layout(r):
    """(layout text, PS1 bytes, arcade bytes) of a random layout whose fields all fit: letters, tokens and groups."""
    items, want, arc = [], bytearray(), bytearray()

    def field(x):
        n = M.LAYOUT_SIZES[x][0]
        w = bytes(r.randrange(256) for _ in range(n))
        if x == 'e':
            w = bytes([w[0], 0, 0, 0])
        return w, field_inverse(x, w)
    for _ in range(r.randrange(1, 5)):
        if r.random() < 0.35:
            inner = [(r.choice('bwlnfqe'), r.randrange(1, 4)) for _ in range(r.randrange(1, 4))]
            reps = r.randrange(2, 5)
            items.append(('g', inner, reps))
            for _rep in range(reps):
                for x, c in inner:
                    for _c in range(c):
                        w, a = field(x)
                        want += w
                        arc += a
        else:
            x, c = r.choice('bwlnfqe'), r.randrange(1, 5)
            items.append(('t', x, c))
            for _c in range(c):
                w, a = field(x)
                want += w
                arc += a
    return M.items_text(items), bytes(want), bytes(arc)


# ---------------------------------------------------------------- Python side

class Tables(unittest.TestCase):
    def test_parse_fighters_and_sequences_rows(self):
        text = ('{"PL00.PAC", 4u, ARCADE_FIGHTER_SAME_CHUNK, 0x3df0u, 0x5db8u, 8130u, 8742u, ARCADE_FIGHTER_TERMINATOR, segments_pl00, 2u, NULL, 0u},\n'
                '{"PL0E.PAC", 4u, 0x2bu, 0x2a38u, 0x6f60u, 17514u, 24966u, 5u, segments_pl0e, 9u, NULL, 0u},\n')
        t = M.parse_fighters_table(text)
        self.assertEqual(t['PL00.PAC'], dict(positions=0x3df0, lists=0x5db8, positions_room=8130, lists_room=8742, same_chunk=True, positions_slot=None))
        self.assertEqual((t['PL0E.PAC']['positions_slot'], t['PL0E.PAC']['same_chunk'], t['PL0E.PAC']['positions']), (0x2b, False, 0x2a38))
        q = M.parse_sequences_table('{"PL00.PAC", 0x801b0000u, 0x7fe0u, 0xab9cu, 853u, 212u, 240u, 0u, 0u, 0u, groups_pl00, 1u},')
        self.assertEqual(q['PL00.PAC'], dict(load=0x801b0000, steps=0x7fe0, frames=0xab9c, step_count=853, frame_count=212, table_entries=240))

    def test_spans_from_tables(self):
        fr = dict(positions=0x100, lists=0x400, positions_room=0x200, lists_room=0x100, same_chunk=True, positions_slot=None)
        sq = dict(load=0x801b0000, steps=0x500, frames=0x600, step_count=10, frame_count=4, table_entries=3)
        spans = M.spans_from_tables(fr, sq, 0x800)
        self.assertEqual(spans, [(0x100, 0x300, 'positions'), (0x400, 0x500, 'lists'), (0x500, 0x578, 'steps'), (0x578, 0x584, 'steptable'),
                                 (0x600, 0x640, 'frames')])
        self.assertIsNone(M.spans_from_tables(fr, sq, 0x630))                        # does not fit the module
        self.assertIsNone(M.spans_from_tables(dict(fr, lists=0x2f0), sq, 0x800))     # two blocks overlap
        other = M.spans_from_tables(dict(fr, same_chunk=False, positions_slot=0x2b), sq, 0x800)
        self.assertEqual([c for _a, _b, c in other], ['lists', 'steps', 'steptable', 'frames'])

    def test_stems_and_labels(self):
        self.assertEqual([M.stem_of(n) for n in ('PL00.PAC', 'PL00X.PAC', 'PL0A.PAC', 'PL0AX.PAC', 'PL15.PAC')], ['PL00', 'PL00', 'PL0A', 'PL0A', 'PL15'])
        e = dict(name='PL09X.PAC', slot=8, extra=True)
        self.assertEqual(M.entry_label(e), 'PL09X.PAC slot 0x8')
        self.assertEqual(M.cid('PL00X.PAC', 5), 'pl00x')
        self.assertEqual(M.cid('PL00X.PAC', 0xb), 'pl00x_sb')


class Runs(unittest.TestCase):
    def test_units_and_exact_runs(self):
        m = bytes([0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0, 0, 0, 0])
        buf = bytes([0x12, 0x34, 0x78, 0x56, 0xff, 0xff, 0xf0, 0xde, 0xbc, 0x9a])      # as is, turned, wrong, turned, ...
        self.assertTrue(M.unit_ok(buf, m, 0, 0) and M.unit_ok(buf, m, 2, 0))
        self.assertFalse(M.unit_ok(buf, m, 4, 0))
        self.assertFalse(M.unit_ok(buf, m, 20, 0) or M.unit_ok(buf, m, 0, 100) or M.unit_ok(buf, m, -2, 0))
        self.assertEqual(M.exact_runs_in(buf, m, 0, 10, 0), [(0, 4), (6, 8)])
        # a 32-bit field turned is a run on its own; the other halves are not 16-bit units
        buf32 = bytes([0x78, 0x56, 0x34, 0x12, 0x01, 0x02])
        m32 = bytes([0x12, 0x34, 0x56, 0x78, 0x77, 0x77])
        self.assertTrue(M.quad_ok(buf32, m32, 0, 0))
        self.assertEqual(M.exact_runs_in(buf32, m32, 0, 6, 0), [(0, 4)])
        self.assertEqual(M.exact_runs_in(buf32, m32, 0, 2, 0), [])

    def test_trim_zeros(self):
        m = bytes(8)
        buf = bytes([0, 0, 0, 0, 1, 2, 0, 0, 0, 0])
        self.assertEqual(M.trim_zeros(buf, m + bytes(4), 0, 10, 0), (4, 6))
        self.assertIsNone(M.trim_zeros(bytes(10), m + bytes(4), 0, 10, 0))
        # zero in the module but not in the arcade stays: it is part of a 32-bit field
        self.assertEqual(M.trim_zeros(bytes([1, 2, 0, 0]), bytes([0, 0, 2, 1]), 0, 4, 0), (0, 4))

    def test_best_layout_and_periodic(self):
        m = arcade(0x800, 5)
        # 20 records of 8 bytes: two turned units, two as-is units (a unit that is the same either way suits both)
        buf = bytearray()
        for k in range(20):
            src = 0x100 + 8 * k
            buf += turn(m[src:src + 4], 2) + m[src + 4:src + 8]
        buf = bytes(buf)
        lay = M.best_layout(buf, m, 0, len(buf), 0x100)
        self.assertEqual(M.rle([(x, c) for x, c in lay]), M.rle(lay))
        text = M.rle(lay)
        self.assertEqual(M.build_piece(m, 0x100, text), buf)
        items = M.periodic_layout(buf, m, 0, len(buf), 0x100, lay)
        self.assertIsNotNone(items)
        ptext = M.items_text(items)
        self.assertLess(len(ptext), len(text))
        self.assertEqual(M.build_piece(m, 0x100, ptext), buf)
        self.assertIn('(w2b2)', ptext)
        # 32-bit fields turned
        buf2 = turn(m[0x200:0x20c], 4)
        self.assertEqual(M.rle(M.best_layout(buf2, m, 0, 12, 0x200)), 'l3')
        self.assertEqual(M.build_piece(m, 0x200, 'l3'), buf2)
        # not an exact run
        self.assertIsNone(M.best_layout(b'\xee\xee', bytes(8), 0, 2, 0))


class Layout(unittest.TestCase):
    def test_parse_sizes_and_malformed(self):
        self.assertEqual(M.layout_parse('b16w4(w2n2)410'), [('t', 'b', 16), ('t', 'w', 4), ('g', [('w', 2), ('n', 2)], 410)])
        self.assertEqual(M.layout_sizes('b16w4(w2n2)410'), (32 + 8 + 2460, 32 + 8 + 3280))
        self.assertEqual(M.layout_sizes('l2e1f1q1'), (8 + 4 + 2, 8 + 1 + 4))
        self.assertEqual(M.layout_sizes('k2z1K3Z1'), (7 * 32, 7 * 32))
        self.assertEqual(M.layout_parse('K17(w2k1)3'), [('t', 'K', 17), ('g', [('w', 2), ('k', 1)], 3)])
        for bad in ('', 'b', 'b0', 'x3', '(w2n2)', '(w2n2)0', '()3', '(w2(n2))3', 'b2 ', 'b2w', '(w2n2', 'w2)3', 'B2', 'Y1', 'K', 'k0', '(K1)0'):
            self.assertIsNone(M.layout_parse(bad), bad)
            self.assertIsNone(M.layout_sizes(bad), bad)
        self.assertEqual(M.group_text([('w', 2), ('f', 1), ('q', 1)], 27), '(w2f1q1)27')

    def test_field_letters(self):
        b = bytes([0x12, 0x34, 0x56, 0x78])
        self.assertEqual(M.field_bytes('b', b[:2]), bytes([0x12, 0x34]))
        self.assertEqual(M.field_bytes('w', b[:2]), bytes([0x34, 0x12]))
        self.assertEqual(M.field_bytes('l', b), bytes([0x78, 0x56, 0x34, 0x12]))
        self.assertEqual(M.field_bytes('e', b[:1]), bytes([0x12, 0, 0, 0]))
        self.assertEqual(M.field_bytes('n', bytes([0, 200])), bytes([200]))
        self.assertEqual(M.field_bytes('f', bytes([0x80, 0x05])), bytes([0x85]))
        self.assertEqual(M.field_bytes('f', bytes([0x00, 0x7f])), bytes([0x7f]))
        self.assertEqual(M.field_bytes('q', bytes([0x00, 0x60])), bytes([4]))
        self.assertEqual(M.field_bytes('q', bytes([0x17, 0xe8])), bytes([255]))          # 255 * 24
        for letter, arc in (('n', bytes([1, 0])), ('f', bytes([0x00, 0x80])), ('f', bytes([0x01, 0x00])), ('f', bytes([0x40, 0x01])),
                            ('q', bytes([0x00, 0x61])), ('q', bytes([0x18, 0x00]))):
            with self.assertRaises(ValueError):
                M.field_bytes(letter, arc)

    def test_palette_letters(self):
        r = random.Random(3)
        for _ in range(200):
            words = [r.choice((0, 0xf000, 0x0fff, 0x1234, r.randrange(65536), r.randrange(65536))) for _ in range(15)]
            arc = b''.join(struct.pack('>H', w) for w in words) + bytes([r.randrange(256), r.randrange(256)])     # the 16th word is not read
            for letter, flags in (('k', 0), ('z', SC.PAL_BLACK_ZERO), ('K', SC.PAL_BIT15), ('Z', SC.PAL_BIT15 | SC.PAL_BLACK_ZERO)):
                got = M.field_bytes(letter, arc)
                self.assertEqual(len(got), 32)
                self.assertEqual(got, SC.palette_bytes(words, flags), letter)                 # screens.py's rule, the same four flags
                self.assertEqual(got[:2], bytes(2))                                           # colour 0 is transparent, even with bit 15
        # the rule on single colours: brightness nibble ignored, R bits 8 to 11, G bits 4 to 7, B bits 0 to 3, each doubled
        row = lambda w, letter: struct.unpack('<16H', M.field_bytes(letter, struct.pack('>15H', w, *(0x0123,) * 14) + bytes(2)))[1]
        self.assertEqual(row(0x0fff, 'k'), 0x1e | 0x1e << 5 | 0x1e << 10)
        self.assertEqual(row(0x0100, 'k'), 2)
        self.assertEqual(row(0x0010, 'k'), 2 << 5)
        self.assertEqual(row(0x0001, 'k'), 2 << 10)
        for hi in (0x0000, 0x1000, 0x7000, 0xf000):
            self.assertEqual(row(hi | 0x0abc, 'k'), row(0x0abc, 'k'))                         # the brightness nibble is dropped
            self.assertEqual(row(hi | 0x0abc, 'K'), row(0x0abc, 'K'))
        # black: 0x8000 with k, 0 with z; bit 15 on every colour with K and Z (black then is 0x8000 either way)
        self.assertEqual([row(0x0000, x) for x in 'kzKZ'], [0x8000, 0, 0x8000, 0x8000])
        self.assertEqual([row(0xf000, x) for x in 'kzKZ'], [0x8000, 0, 0x8000, 0x8000])      # brightness only is black too
        self.assertEqual([row(0x0123, x) for x in 'kzKZ'], [1 << 1 | 2 << 6 | 3 << 11] * 2 + [0x8000 | (1 << 1 | 2 << 6 | 3 << 11)] * 2)
        self.assertEqual(M.field_bytes('K', bytes(30)), M.field_bytes('Z', bytes(30)))
        with self.assertRaises(ValueError):
            M.field_bytes('k', bytes(29))

    def test_palette_pieces_stride(self):
        # consecutive rows of one piece are 32 arcade bytes apart: the 16th word of a palette is skipped, not read
        m = bytearray(arcade(0x400, 9))
        pieces = M.build_piece(bytes(m), 0x40, 'K3')
        self.assertEqual(len(pieces), 96)
        for k in range(3):
            self.assertEqual(pieces[32 * k:32 * k + 32], SC.palette_bytes(struct.unpack_from('>15H', m, 0x40 + 32 * k), SC.PAL_BIT15))
        m[0x5e:0x60] = b'\xaa\xbb'                                   # the unused word of the first palette
        self.assertEqual(M.build_piece(bytes(m), 0x40, 'K3'), pieces)
        self.assertEqual(M.build_piece(bytes(m), 0x40, 'k1z1')[32:], M.field_bytes('z', bytes(m[0x60:0x80])))
        # a group with a palette, and the bounds: the last row needs its 32 bytes
        self.assertEqual(M.layout_sizes('(w1k1)2'), (68, 68))
        self.assertEqual(len(M.build_piece(bytes(m), 0x3e0, 'k1')), 32)
        with self.assertRaises(ValueError):
            M.build_piece(bytes(m), 0x3e2, 'k1')

    def test_build_piece_refuses(self):
        m = arcade(0x100, 1)
        with self.assertRaises(ValueError):
            M.build_piece(m, 0xf8, 'b8')                  # runs off the program
        with self.assertRaises(ValueError):
            M.build_piece(m, 1, 'b1')                     # odd source
        with self.assertRaises(ValueError):
            M.build_piece(m, 0, 'b0')

    def test_inverse_of_every_letter(self):
        r = random.Random(11)
        for x in 'bwlnfqe':
            for _ in range(20):
                n = M.LAYOUT_SIZES[x][0]
                w = bytes(r.randrange(256) for _ in range(n))
                if x == 'e':
                    w = bytes([w[0], 0, 0, 0])
                a = M.record_expected(w, x)
                self.assertEqual(len(a), M.LAYOUT_SIZES[x][1])
                self.assertEqual(M.field_bytes(x, a), w)
        self.assertIsNone(M.record_expected(bytes([1, 2, 0, 0]), 'e'))


class Converted(unittest.TestCase):
    def world(self):
        """An arcade program with a table of 8-byte records at 0x1000 (x, y, flags, value) and a PS1 module that has it as 6-byte records
        (wwfq) after some other bytes, and the first side and a second side that differs in one record."""
        m = bytearray(arcade(0x3000, 9))
        recs = []
        for k in range(12):
            x, y = (k * 37 - 200) & 0xffff, (k * 11) & 0xffff
            flags = 0x8000 | (k % 3 + 1)
            val = 24 * (k % 5)
            m[0x1000 + 8 * k:0x1008 + 8 * k] = struct.pack('>HHHH', x, y, flags, val)
            recs.append(struct.pack('<hhBB', x - 65536 if x >= 32768 else x, y, 0x80 | (k % 3 + 1), k % 5))
        mod = bytes(r.randrange(256) for r in [random.Random(2)] for _ in range(40)) + b''.join(recs) + bytes(range(1, 30))
        return bytes(m), mod, 40

    def test_template_helpers(self):
        self.assertEqual(M.template_fields('wwfq'), [('w', 2), ('f', 1), ('q', 1)])
        self.assertEqual(M.template_sizes('wwfq'), (6, 8))
        self.assertEqual(M.record_expected(struct.pack('<hhBB', -4, 86, 0x81, 4), 'wwfq'), struct.pack('>hhHH', -4, 86, 0x8001, 96))

    def test_search_and_pieces(self):
        m, mod, at = self.world()
        taken = {'a': bytearray(len(mod))}
        runs = M.template_search([('a', mod)], m, 'wwfq', taken)
        self.assertEqual(runs, [('a', at, at + 72, 0x1000)])
        self.assertEqual(sum(taken['a']), 72)
        stats = P.UnitStats(m)
        pieces = M.template_pieces(mod, m, [(at, at + 72, 0x1000, 'wwfq')], stats, bytearray(len(mod)))
        self.assertEqual([(o, n, s, t) for o, n, s, t, _e in pieces], [(at, 72, 0x1000, '(w2f1q1)12')])
        self.assertEqual(M.build_piece(m, 0x1000, '(w2f1q1)12'), mod[at:at + 72])
        # a second side that differs in the 3rd record: two runs, the short one too short to keep
        twin = bytearray(mod)
        twin[at + 6 * 2] ^= 0xff
        pieces = M.template_pieces(bytes(twin), m, [(at, at + 72, 0x1000, 'wwfq')], stats, bytearray(len(mod)))
        self.assertEqual([(o, n, s, t) for o, n, s, t, _e in pieces], [(at + 18, 54, 0x1000 + 24, '(w2f1q1)9')])
        # bytes another class holds are not taken; a run too short is not a piece
        taken = {'a': bytearray(len(mod))}
        taken['a'][at + 30] = 1
        self.assertEqual(M.template_search([('a', mod)], m, 'wwfq', taken), [('a', at, at + 30, 0x1000), ('a', at + 36, at + 72, 0x1000 + 48)])
        self.assertEqual(M.template_pieces(mod, m, [(at, at + 72, 0x1000, 'wwfq')], stats, bytearray(at + 30 and [0] * at + [1] * 70 + [0] * 100)), [])
        self.assertEqual(M.template_search([('a', mod)], m, 'wwnn', {'a': bytearray(len(mod))}), [])      # another template: no run


class Search(unittest.TestCase):
    def test_tables_found_and_built(self):
        m = arcade(0x4000, 21)
        r = random.Random(4)
        junk = lambda n: bytes(r.randrange(256) for _ in range(n))
        t1 = turn(m[0x1000:0x1080], 2)                         # 16-bit fields turned
        t2 = m[0x2000:0x2060]                                  # as they are
        t3 = turn(m[0x2800:0x2840], 4)                         # 32-bit fields turned
        tail = junk(24) + t1 + junk(20) + t2 + junk(20) + t3 + junk(30)
        ce = 0x100
        mod = bytes(ce) + tail
        stats = P.UnitStats(m)
        cands = M.search_tails([('x', ce, mod[ce:], [])], m, stats)['x']
        self.assertTrue(cands)
        taken = bytearray(len(mod))
        taken[:ce] = b'\1' * ce
        pieces = M.pieces_of(mod, m, cands, stats, taken)
        covered = bytearray(len(mod))
        for off, n, src, text, ev in pieces:
            self.assertGreaterEqual(ev, M.MIN_EVIDENCE)
            self.assertGreaterEqual(n, M.MIN_PIECE)
            self.assertEqual(M.build_piece(m, src, text), mod[off:off + n])
            self.assertFalse(any(covered[off:off + n]))
            covered[off:off + n] = b'\1' * n
        for base, size in ((ce + 24, 0x80), (ce + 24 + 0x80 + 20, 0x60), (ce + 24 + 0x80 + 20 + 0x60 + 20, 0x40)):
            self.assertGreaterEqual(sum(covered[base:base + size]), size - 8, hex(base))
        # the junk between the tables is not a piece
        self.assertEqual(sum(covered[ce:ce + 24]), 0)
        # a masked block (the other builders') is kept out
        mask = [(ce + 24, ce + 24 + 0x80)]
        masked = bytearray(mod)
        masked[ce + 24:ce + 24 + 0x80] = bytes(0x80)
        cands2 = M.search_tails([('x', ce, bytes(masked[ce:]), mask)], m, stats)['x']
        self.assertTrue(all(e <= ce + 24 or s >= ce + 24 + 0x80 for s, e, _d in cands2))
        # a second side whose code is 4 bytes longer: the candidates shift, the deltas move back
        shifted = [(s + 4, e + 4, d - 4) for s, e, d in cands]
        mod2 = bytes(ce + 4) + tail
        pieces2 = M.pieces_of(mod2, m, shifted, stats, bytearray(b'\1' * (ce + 4) + bytes(len(tail))))
        self.assertEqual([(o - 4, n, s, t) for o, n, s, t, _e in pieces2], [(o, n, s, t) for o, n, s, t, _e in pieces])

    def test_zeros_alone_and_low_evidence_are_not_pieces(self):
        m = bytearray(arcade(0x2000, 8))
        m[0x100:0x140] = bytes(0x40)
        m[0x200:0x20a] = bytes([0, 1, 0, 1, 0, 1, 0, 1, 0, 1])           # a short run of one value: little information
        mod = bytes(0x10) + bytes(0x40) + bytes(4) + m[0x200:0x20a]
        stats = P.UnitStats(bytes(m))
        pieces = M.pieces_of(mod, bytes(m), [(0x10, len(mod), 0x100 - 0x10)], stats, bytearray(0x10))
        self.assertEqual(pieces, [])


def palette_options_for(m):
    pindex = SC.PaletteIndex(m)
    return lambda row: SC.palette_options(m, row, pindex.find)


class Palettes(unittest.TestCase):
    def test_runs_of_palette_rows(self):
        m = arcade(0x3000, 31)
        stats = P.UnitStats(m)
        mod = bytearray(0x800)
        mod[0x100:0x100 + 96] = M.build_piece(m, 0x400, 'K3')                  # three consecutive palettes with bit 15
        mod[0x200:0x220] = M.build_piece(m, 0x1000, 'k1')                      # one, alone
        mod[0x220:0x240] = M.build_piece(m, 0x1020, 'k1')
        mod[0x260:0x280] = M.build_piece(m, 0x1c00, 'z1')                      # black stays 0 (the palette has a black colour)
        words = list(struct.unpack_from('>15H', m, 0x1c00))
        self.assertIn(0, [(w >> 8 & 15) | (w >> 4 & 15) | (w & 15) for w in words] + [0])
        m2 = bytearray(m)
        m2[0x1c02:0x1c04] = b'\xf0\x00'                                      # colour 2 of that palette: only brightness, so black
        stats = P.UnitStats(bytes(m2))
        mod[0x260:0x280] = M.build_piece(bytes(m2), 0x1c00, 'z1')
        mod[0x300:0x320] = bytes([0, 0] + [1, 2] * 15)                         # colour 0 is 0 but these are no arcade colours
        taken = bytearray(0x800)
        taken[0x1c0:0x1e0] = b'\1' * 0x20                                     # a byte range that is not free (code, or the loader's)
        got = M.find_palette_runs(bytes(mod), taken, palette_options_for(bytes(m2)), bytes(m2), stats)
        by = {o: (n, s, t) for o, n, s, t, _e in got}
        self.assertEqual(by[0x100], (96, 0x400, 'K3'))
        self.assertEqual(by[0x200], (64, 0x1000, 'k2'))                        # two rows that are consecutive in the arcade are one run
        self.assertEqual(by[0x260], (32, 0x1c00, 'z1'))
        self.assertEqual(sorted(by), [0x100, 0x200, 0x260])
        for o, n, s, t, e in got:
            self.assertGreaterEqual(e, M.MIN_PALETTE_EVIDENCE)
            self.assertEqual(M.build_piece(bytes(m2), s, t), bytes(mod[o:o + n]))
            self.assertTrue(all(taken[o:o + n]))                               # taken is updated
        # rows in taken bytes are not looked at
        taken2 = bytearray(0x800)
        taken2[0x100:0x120] = b'\1' * 0x20
        got2 = M.find_palette_runs(bytes(mod), taken2, palette_options_for(bytes(m2)), bytes(m2), stats)
        self.assertNotIn(0x100, {o for o, *_ in got2})
        self.assertIn(0x120, {o for o, *_ in got2})                            # the other two rows of the run still are a run
        # a row of one value (little information) is no palette
        flat = bytearray(0x40)
        flat[0:32] = struct.pack('<16H', 0, *(0x8000,) * 15)
        mflat = bytes(0x100)
        self.assertEqual(M.find_palette_runs(bytes(flat), bytearray(0x40), palette_options_for(mflat), mflat, P.UnitStats(arcade(0x1000, 2))), [])


class Screens(unittest.TestCase):
    def test_parsers(self):
        pins = ('static const struct arcade_code_range ranges_0[] = {\n    {0x20, 0x17a4},\n};\n'
                'static const struct arcade_code_range ranges_2[] = {\n    {0x4, 0x3d4}, {0x800, 0x40},\n};\n'
                'const struct arcade_code_pin arcade_code_pins[] = {\n    {"slot0", 0x1000,\n     {0x01, 0x02},\n     {0x03, 0x04}, ranges_0, 1},\n'
                '    {"slot1", 0x20,\n     {0x01},\n     {0x03}, NULL, 0},\n    {"slot2", 0x2000,\n     {0x01},\n     {0x03}, ranges_2, 2},\n};\n')
        self.assertEqual(M.parse_code_ranges(pins), {'slot0': [(0x20, 0x17a4)], 'slot1': [], 'slot2': [(4, 0x3d4), (0x800, 0x40)]})
        chains = ('const struct arcade_screen_lists_chain arcade_screen_lists_chains[] = {\n    {ARCADE_SCREEN_LISTS_PAL_SPLIT, 100u, 200u, 0u, 5u, 0u, 0u},\n'
                  '    {0u, 10u, 20u, 5u, 1u, 0u, 0u},\n};\n'
                  'const struct arcade_screen_lists_use arcade_screen_lists_uses[] = {\n    {"END00.PAC", 40u, 0x1000u, 0x2000u, 0u, 1u, 0x2u, 3u, 4u},\n'
                  '    {"END01.PAC", 40u, 0x30u, 0x40u, 1u, 1u, 0x2u, 3u, 4u},\n};\n')
        texts = ('static const struct arcade_text_string strings_a[] = {\n    {0x508cu, 0x8ecaau, 4u, 8u, ARCADE_TEXT_PLAIN},\n'
                 '    {0x50bcu, 0x844f0u, 6u, 12u, ARCADE_TEXT_CODES},\n};\n'
                 'const struct arcade_text_module arcade_text_modules[] = {\n    {"END00.PAC", 40u, 1000u, strings_a, 2u},\n};\n')
        quotes = ('static const struct arcade_quote_fighter fighters[] = {\n    {"CDEMO00.PAC", 0x0, 0xe2780, 0x8b98, 26u, 0x9f6c, 0x530, 0x9e38, 0x134, 0x18668, 0x5158}, /* block 0 */\n};\n'
                  'const struct arcade_quotes arcade_quotes = {1u, 0x10u, 160, 16u, fighters, 1};\n')
        talk = ('static const struct arcade_talk_page pages[] = {\n    {"RDM00.PAC", 0xeea88, 4, {0, 1, 2, 3}, 0x19bc, 5},\n};\n'
                'const struct arcade_talk arcade_talk = {2, 0, 1, 0x2a, pages, 1};\n')
        stages = ('const struct arcade_stage arcade_stages[] = {\n    {"STAGE00.PAC", 0u, {49896u, 98304u, 3072u, 18944u},\n'
                  '     {0xffffffffffffffffull, 0xffffffffull},\n     {\n         {8080u, 64u, 16u, 4u, 0u, 2u, ids_00_0, 0, 0u, ck_00_0, 0},\n'
                  '         {12176u, 64u, 16u, 4u, 0u, 7u, ids_00_1, 0, 0u, 0, 0},\n         {16272u, 32u, 8u, 4u, 0u, 7u, ids_00_2, 0, 0u, 0, 0},\n'
                  '     },\n     0, 0u, 0, 0u,\n     objects_00, 29u,\n     0, 0u},\n};\n')
        got = M.parse_loader_regions(chains, texts, quotes, talk, stages)
        self.assertEqual(sorted(got['END00.PAC', 40]), [(0x1000, 0x1064), (0x2000, 0x20c8), (0x508c, 0x5094), (0x50bc, 0x50c8)])
        self.assertEqual(got['END01.PAC', 40], [(0x30, 0x3a), (0x40, 0x54)])
        self.assertEqual(got['CDEMO00.PAC', 1], [(0x8b98, 0x8b98 + 4 * 26), (0x9f6c, 0x9f6c + 0x530), (0x9e38, 0x9e38 + 0x134)])
        self.assertEqual(got['RDM00.PAC', 0x2a], [(0x19bc, 0x19bc + 30)])
        self.assertEqual(got['STAGE00.PAC', 6], [(8080, 8080 + 4096), (12176, 12176 + 4096), (16272, 16272 + 1024)])
        self.assertEqual(M.merged_ranges([(10, 20), (15, 30), (30, 31), (40, 50), (-5, 3), (60, 60), (90, 200)], 100), [(0, 3), (10, 31), (40, 50), (90, 100)])

    def test_analysis_of_a_module(self):
        m = bytes(arcade(0x8000, 77))
        r = random.Random(5)
        junk = lambda n: bytes(r.randrange(1, 256) for _ in range(n))
        load = 0x80010000
        mod = bytearray(0x1000)
        mod[0x000:0x040] = junk(0x40)                                        # code
        mod[0x100:0x200] = turn(m[0x1000:0x1100], 2)                         # a table of the arcade, turned (a piece)
        mod[0x300:0x300 + 96] = M.build_piece(m, 0x4000, 'K3')               # three palettes
        mod[0x400:0x440] = turn(m[0x2000:0x2040], 2)                         # the loader's: kept out
        mod[0x500:0x520] = M.build_piece(m, 0x4800, 'k1')                    # the loader's: a palette kept out
        struct.pack_into('<IIII', mod, 0x600, load + 0x100, load + 0x300, 0x80118900, load + 0x604)      # four words
        mod[0x700:0x740] = junk(0x40)                                        # nothing the arcade has
        prog = dict(funcs={0x80118900}, code=(0x80118900, 0x8016d91c), data=(0x8016d91c, 0x80183904))
        ent = dict(image='img', slot=0x28, load=load, mod=bytes(mod), files=['END00.PAC', 'END01.PAC'])
        info = M.analyse_screens([ent], m, {'img': [(0, 0x40)]}, {('END00.PAC', 0x28): [(0x400, 0x440)], ('END01.PAC', 0x28): [(0x500, 0x520)]}, prog, set())['img']
        by = {o: (n, s, t) for o, n, s, t, _e in info['pieces']}
        self.assertEqual(by[0x300], (96, 0x4000, 'K3'))
        self.assertTrue(any(o <= 0x100 and o + n >= 0x1f0 for o, n, _s, _t in ((o, n, s, t) for o, (n, s, t) in by.items())))
        self.assertFalse(any(o < 0x440 and o + n > 0x400 or o < 0x520 and o + n > 0x500 or o < 0x40 for o, (n, _s, _t) in by.items()))
        self.assertEqual(info['relocs'], [(0x600, 0x100), (0x604, 0x300), (0x608, 0x80118900), (0x60c, 0x604)])
        img, known = M.model_build(m, len(mod), load, info['pieces'], info['relocs'])
        self.assertTrue(all(img[k] == mod[k] for k in range(len(mod)) if known[k]))
        c = collections.Counter(info['cls'])
        self.assertEqual((c['code'], c['loader'], c['palette'], c['ptr']), (0x40, 0x40 + 0x20, 96, 16))
        self.assertGreaterEqual(c['arcade'], 0xe0)
        self.assertEqual(sum(c.values()), len(mod))
        rows = M.screen_descriptors({'img': info})
        self.assertEqual([(d['file'], d['slot'], d['load'], d['size']) for d in rows], [('END00.PAC', 0x28, load, 0x1000), ('END01.PAC', 0x28, load, 0x1000)])
        self.assertEqual(rows[0]['pieces'], rows[1]['pieces'])
        text = M.c_table(rows)
        self.assertEqual(text.count('static const struct arcade_module_piece '), 1)                 # the two files share their arrays
        self.assertIn('"K3"', text)
        self.assertNotIn('\N{EM DASH}', text)
        self.assertEqual(M.screen_descriptors({'x': dict(info, pieces=[], relocs=[])}), [])        # nothing to build: no row
        self.assertIn('NULL', M.c_table(M.screen_descriptors({'x': dict(info, pieces=[])})))        # pointers only: no piece array


class Pointers(unittest.TestCase):
    def test_classes_and_rows(self):
        load, size, ce = 0x801b0000, 0x400, 0x100
        prog = dict(funcs={0x80118900, 0x80119000}, code=(0x80118900, 0x8016d91c), data=(0x8016d91c, 0x80183904))
        funcs = {0x40, 0x80}
        words = [load + 0x40, load + 0x44, load + 0x200, 0x80118900, 0x8016da00, 0x80100000, 0x801e0020, 0x801b0001, 0x00001234]
        buf = bytearray(size)
        for k, w in enumerate(words):
            struct.pack_into('<I', buf, 0x200 + 4 * k, w)
        free = bytearray(b'\1' * size)
        out = M.find_pointers(bytes(buf), free, load, ce, funcs, prog, {0x80100000})
        got = {o: (w, c) for o, w, c in out}
        self.assertEqual(got[0x200][1], 'ptr_self_code')
        self.assertEqual(got[0x204][1], 'ptr_self_label')        # 0x44 is inside the code and is no function start
        self.assertEqual(got[0x208][1], 'ptr_self_data')
        self.assertEqual(got[0x20c][1], 'ptr_prog_code')
        self.assertEqual(got[0x210][1], 'ptr_prog_data')
        self.assertEqual(got[0x214][1], 'ptr_prog_other')
        self.assertEqual(got[0x218][1], 'ptr_overlay')
        self.assertNotIn(0x21c, got)                              # not 4-aligned as an address
        self.assertNotIn(0x220, got)                              # not a RAM address
        rows = M.reloc_rows(out, load, size)
        self.assertEqual(rows[:3], [(0x200, 0x40), (0x204, 0x44), (0x208, 0x200)])
        self.assertEqual([t for _o, t in rows[3:]], [0x80118900, 0x8016da00, 0x80100000, 0x801e0020])
        # a lone word that names nothing known is not a pointer; two next to each other are
        lone = bytearray(size)
        struct.pack_into('<I', lone, 0x300, 0x80150000)
        self.assertEqual(M.find_pointers(bytes(lone), free, load, ce, funcs, prog, set()), [])
        struct.pack_into('<I', lone, 0x304, 0x80150010)
        self.assertEqual(len(M.find_pointers(bytes(lone), free, load, ce, funcs, prog, set())), 2)
        # claimed bytes are not looked at
        free2 = bytearray(free)
        free2[0x200:0x204] = bytes(4)
        self.assertNotIn(0x200, {o for o, _w, _c in M.find_pointers(bytes(buf), free2, load, ce, funcs, prog, {0x80100000})})


class Classes(unittest.TestCase):
    def test_every_byte_gets_a_class(self):
        m = arcade(0x1000, 2)
        mod = bytearray(0x400)
        mod[0x180:0x190] = turn(m[0x40:0x50], 2)
        mod[0x200:0x204] = struct.pack('<I', 0x801b0100)
        mod[0x300:0x30c] = b'Hello, Ryu!\0'
        mod[0x320:0x324] = bytes([1, 2, 3, 4])
        spans = [(0x100, 0x140, 'lists'), (0x140, 0x180, 'frames')]
        pieces = [(0x180, 0x10, 0x40, 'w8', 30.0)]
        pointers = [(0x200, 0x801b0100, 'ptr_self_data')]
        conv = [(0x1c0, 6, 0x800, '(w2n2)1', 20.0)]
        mod[0x1c0:0x1c6] = bytes(6)
        mod[0x1f0:0x1f4] = bytes([9, 9, 9, 9])
        cls = M.classify(bytes(mod), 0x801b0000, 0x100, spans, pieces, conv, [(0x1e0, 0x1f4, 0)], pointers)
        self.assertEqual(len(cls), 0x400)
        self.assertTrue(all(cls))
        self.assertEqual(set(cls[:0x100]), {'code'})
        self.assertEqual((cls[0x100], cls[0x17f]), ('lists', 'frames'))
        self.assertEqual(set(cls[0x180:0x190]), {'arcade'})
        self.assertEqual(set(cls[0x1c0:0x1c6]), {'arcade_conv'})
        self.assertEqual(set(cls[0x200:0x204]), {'ptr_self_data'})
        self.assertEqual(cls[0x1f0], 'near')
        self.assertEqual(cls[0x1ef], 'zero')
        self.assertEqual(cls[0x300], 'text')
        self.assertEqual(cls[0x320], 'other')
        self.assertEqual(cls[0x3ff], 'zero')
        runs = M.runs_of(cls)
        self.assertEqual([(x, y, n) for x, y, n in runs if x < 0x340], [(0x1f0, 0x1f4, 4), (0x300, 0x324, 15)])    # joined across 24 zero bytes
        self.assertEqual(M.detail_lines(dict(cls=cls), 1), ['    left: 0x300, 36 bytes from there, 15 of them not built'])
        c = M.class_counts(cls)
        self.assertEqual(sum(c.values()), 0x400)
        data, others, rest, ptr, mine = M.split_counts(c)
        self.assertEqual((data, others, ptr), (0x300, 0x80, 4))
        self.assertEqual(mine, c['arcade'] + c['arcade_conv'] + 4 + c['zero'])


class Model(unittest.TestCase):
    def test_model_build(self):
        m = arcade(0x800, 6)
        pieces = [(0x10, 8, 0x100, 'w4', 20.0), (0x20, 6, 0x200, 'b3', 20.0)]
        img, known = M.model_build(m, 0x40, 0x801b0000, pieces, [(0x30, 0x20), (0x34, 0x80118900)])
        self.assertEqual(img[0x10:0x18], turn(m[0x100:0x108], 2))
        self.assertEqual(img[0x20:0x26], m[0x200:0x206])
        self.assertEqual(struct.unpack_from('<II', img, 0x30), (0x801b0020, 0x80118900))
        self.assertEqual(sum(known), 8 + 6 + 8)
        for bad in ([(0x10, 8, 0x100, 'w3', 1.0)], [(0x3c, 8, 0x100, 'w4', 1.0)], [(0x10, 8, 0x7fc, 'w4', 1.0)]):
            with self.assertRaises(ValueError):
                M.model_build(m, 0x40, 0x801b0000, bad, [])
        with self.assertRaises(ValueError):
            M.model_build(m, 0x40, 0x801b0000, [], [(0x32, 1)])

    def test_c_table_text(self):
        d1 = dict(file='PL00.PAC', slot=4, load=0x801b0000, size=100, pieces=[(0x10, 8, 0x100, 'w4')], relocs=[(0x20, 0x40), (0x24, 0x80118900)])
        d2 = dict(d1, file='PL00X.PAC', slot=5, load=0x801c8000)
        d3 = dict(d1, file='PL01.PAC', pieces=[(0x10, 8, 0x102, 'w4')])
        d4 = dict(d1, file='PL00X.PAC', slot=0xb, load=0x801e0000)
        text = M.c_table([d1, d2, d3, d4])
        self.assertTrue(text.startswith('/* GENERATED by'))
        self.assertNotIn('\N{EM DASH}', text)
        self.assertEqual(text.count('static const struct arcade_module_piece '), 2)       # PL00X shares PL00's arrays; PL01 has its own pieces
        self.assertEqual(text.count('static const struct arcade_module_reloc '), 1)
        self.assertIn('{0x10u, 8u, 0x100u, "w4"},', text)
        self.assertIn('{0x20u, 0x40u},', text)
        self.assertIn('{"PL00X.PAC", 5u, 0x801c8000u, 100u, pieces_pl00, 1u, relocs_pl00, 2u},', text)
        self.assertIn('{"PL00X.PAC", 11u, 0x801e0000u, 100u, pieces_pl00, 1u, relocs_pl00, 2u},', text)
        self.assertIn('arcade_module_data_count', text)

    def test_the_checked_in_table(self):
        text = (ROOT / 'port/arcade/module_data_table.c').read_text()
        self.assertTrue(text.startswith('/* GENERATED by'))
        self.assertNotIn('\N{EM DASH}', text)
        rows = [l for l in text.splitlines() if l.startswith('    {"PL')]
        self.assertGreaterEqual(len(rows), 77)
        names = [(l.split('"')[1], int(l.split('",')[1].split('u')[0])) for l in rows]
        self.assertEqual(names, sorted(names))
        self.assertEqual(sum(1 for _n, s in names if s in (4, 5)), 45)
        # the screen and stage rows follow the fighter rows
        tail = text[text.index('arcade_module_data[] = {'):]
        srows = [l for l in tail.splitlines() if l.startswith('    {"') and not l.startswith('    {"PL')]
        self.assertGreaterEqual(len(srows), 160)
        snames = [(l.split('"')[1], int(l.split('",')[1].split('u')[0])) for l in srows]
        self.assertEqual(snames, sorted(snames))
        self.assertLess(max(i for i, l in enumerate(tail.splitlines()) if l.startswith('    {"PL')), min(i for i, l in enumerate(tail.splitlines()) if l in srows))
        self.assertEqual({s for _n, s in snames}, {1, 4, 6, 0xf, 0x12, 0x27, 0x28, 0x2a})
        for l in text.splitlines():
            if l.startswith('    {0x'):
                self.assertRegex(l, r'^    \{0x[0-9a-f]+u, (0x[0-9a-f]+u\},|\d+u, 0x[0-9a-f]+u, "[a-zA-Z0-9()]+"\},)$')


# ---------------------------------------------------------------- C side

class C(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-module-data-')
        d = Path(cls.dir.name)
        cls.exe = d / 'module_data_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES), str(DRIVER), '-o', str(cls.exe)],
                       check=True, timeout=180)
        cls.prog = d / 'prog.bin'

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def piece(self, prog, source, size, layout):
        """(status, bytes) of the C for one piece."""
        self.prog.write_bytes(prog)
        out = Path(self.dir.name) / 'piece.bin'
        out.unlink(missing_ok=True)
        r = subprocess.run([str(self.exe), 'piece', str(self.prog), str(source), str(size), layout, str(out)], capture_output=True, text=True, timeout=60)
        self.assertIn(r.returncode, (0, 1), r.stdout + r.stderr)
        return r.returncode, (out.read_bytes() if r.returncode == 0 else None)

    def test_pieces_equal_the_model_for_every_letter(self):
        r = random.Random(5)
        for _ in range(150):
            text, want, arc = random_layout(r)
            prog = bytearray(arcade(0x400, r.randrange(100)))
            source = 2 * r.randrange(0, 100)
            prog[source:source + len(arc)] = arc
            self.assertEqual(M.layout_sizes(text), (len(want), len(arc)))
            self.assertEqual(M.build_piece(bytes(prog), source, text), want)
            self.assertEqual(self.piece(bytes(prog), source, len(want), text), (0, want), text)

    def test_palette_letters_equal_the_model(self):
        r = random.Random(14)
        for _ in range(60):
            prog = bytearray(arcade(0x400, r.randrange(100)))
            for k in range(0, len(prog) - 1, 2):
                if r.random() < 0.3:
                    prog[k:k + 2] = r.choice((b'\0\0', b'\xf0\x00', b'\x00\x00', b'\x80\x00'))     # blacks, brightness only
            letters = ''.join(r.choice('kzKZ') + str(r.randrange(1, 4)) for _ in range(r.randrange(1, 4)))
            n = M.layout_sizes(letters)[0]
            source = 2 * r.randrange(0, 40)
            self.assertEqual(self.piece(bytes(prog), source, n, letters), (0, M.build_piece(bytes(prog), source, letters)), letters)
        # the stride is 32: a piece needs all 32 bytes of the last row; a palette can sit at the very end of the program
        prog = bytes(range(256)) * 4
        self.assertEqual(self.piece(prog, 992, 32, 'K1')[0], 0)
        self.assertEqual(self.piece(prog, 994, 32, 'K1')[0], 1)
        self.assertEqual(self.piece(prog, 0, 64, 'k1')[0], 1)                                   # the wrong size
        self.assertEqual(self.piece(prog, 0, 32, '(k1)1')[0], 0)
        self.assertEqual(self.piece(prog, 0, 70, '(k1b1)1')[0], 1)
        self.assertEqual(self.piece(prog, 0, 34, '(k1b1)1')[0], 0)
        self.assertEqual(self.piece(prog, 0, 32, 'Y1')[0], 1)                                   # an unknown letter

    def test_screen_module_row(self):
        # a row like a screen module's: palettes, a turned table, pointer words into the module and to the program, at the screen link address
        m = bytearray(arcade(0x1000, 15))
        m[0x400:0x440] = bytes([0x01, 0x23, 0xf4, 0x56] * 16)
        pieces = [(0x20, 96, 0x100, 'K3'), (0x80, 32, 0x200, 'z1'), (0xa0, 32, 0x240, 'k1'), (0x100, 64, 0x400, 'w32')]
        relocs = [(0x180, 0x20), (0x184, 0x80118900), (0x188, 0x300)]
        rc, img, known = self.run_row(bytes(m), 0x80010000, 0x200, pieces, relocs)
        mimg, mknown = M.model_build(bytes(m), 0x200, 0x80010000, [(o, n, s, t, 0.0) for o, n, s, t in pieces], relocs)
        self.assertEqual((rc, img, known), (0, mimg, mknown))
        self.assertEqual(sum(known), 96 + 32 + 32 + 64 + 12)
        self.assertEqual(struct.unpack_from('<III', img, 0x180), (0x80010020, 0x80118900, 0x80010300))
        self.assertEqual(img[0x20:0x40], SC.palette_bytes(struct.unpack_from('>15H', m, 0x100), SC.PAL_BIT15))
        self.assertEqual(img[0x80:0xa0], SC.palette_bytes(struct.unpack_from('>15H', m, 0x200), SC.PAL_BLACK_ZERO))
        self.assertEqual(self.valid(0x80010000, 0x200, pieces, relocs), 0)
        self.assertEqual(self.valid(0x80010000, 0x200, [(0x1f0, 32, 0x100, 'k1')], []), -1)       # a palette off the end of the module
        self.assertEqual(self.valid(0x80010000, 0x200, [(0x20, 31, 0x100, 'k1')], []), -1)        # the wrong size
        self.assertEqual(self.valid(0x80010000, 0x200, [(0x20, 32, 0x100, 'k1')], [(0x30, 0x20)]), -1)   # a word in a palette
        # rows with only pieces, or only words
        self.assertEqual(self.valid(0x80010000, 0x200, pieces, []), 0)
        self.assertEqual(self.valid(0x80010000, 0x200, [], relocs), 0)

    def test_refusals(self):
        prog = bytes(range(256)) * 4
        self.assertEqual(self.piece(prog, 0, 8, 'w4')[0], 0)
        for source, size, layout in ((0, 8, 'w3'), (0, 6, 'w4'), (0, 8, 'w5'), (1, 8, 'w4'), (1020, 8, 'w4'), (1024, 2, 'b1'), (2000, 4, 'b2'),
                                     (0, 8, ''), (0, 8, 'w'), (0, 8, 'w0'), (0, 8, 'x4'), (0, 8, '(w2'), (0, 8, 'w2)2'), (0, 8, '()2'), (0, 8, '(w2)0'),
                                     (0, 8, '(w1(b1)2)2'), (0, 8, 'b99999999999'), (0, 4, '(b1)99999999'), (0, 2, 'n2'),
                                     (0, 8, 'e1'), (0, 8, 'w2w2w2w2w2'), (0, 4, 'q1b1')):
            self.assertEqual(self.piece(prog, source, size, layout)[0], 1, (source, size, layout))
        # values that do not fit: n, f, q
        high = bytes([1, 0, 0, 5, 0x40, 1, 0x00, 0x61])
        for layout, size in (('n1', 1), ('f1', 1), ('q1', 1)):
            self.assertEqual(self.piece(high, 0, size, layout)[0], 1, layout)
        self.assertEqual(self.piece(high, 2, 1, 'n1'), (0, bytes([5])))
        self.assertEqual(self.piece(bytes([0, 0, 0, 1, 0x80, 0x05, 0x01, 0x28, 0]), 4, 1, 'f1'), (0, bytes([0x85])))
        # a group repeats a record; the size is checked before anything is written
        rec = bytes([0x12, 0x34, 0x00, 0x05])
        self.assertEqual(self.piece(rec * 3, 0, 9, '(w1n1)3'), (0, bytes([0x34, 0x12, 0x05]) * 3))
        self.assertEqual(self.piece(rec * 3, 0, 12, '(w1n1)3')[0], 1)
        self.assertEqual(self.piece(rec * 2, 0, 9, '(w1n1)3')[0], 1)           # the program is too short

    def test_the_letters_program_c_has(self):
        r = random.Random(8)
        letters = 'bwlne'
        for _ in range(60):
            n = r.randrange(1, 6)
            toks = [(r.choice(letters), r.randrange(1, 5)) for _ in range(n)]
            text = M.rle(toks)
            units = ''.join(x * c for x, c in toks)
            ps1 = sum(M.LAYOUT_SIZES[x][0] * c for x, c in toks)
            prog = bytes(r.randrange(2) * r.randrange(256) if r.random() < 0.7 else r.randrange(256) for _ in range(0x200))
            source = 2 * r.randrange(0, 40)
            if ps1 % 2:
                continue                                          # program.c's pieces have even sizes
            self.prog.write_bytes(prog)
            res = subprocess.run([str(self.exe), 'same', str(self.prog), str(source), str(ps1), text, units], capture_output=True, text=True, timeout=60)
            self.assertEqual((res.returncode, res.stdout.strip()), (0, 'equal'), (text, units))

    def spec(self, load, size, pieces, relocs):
        return f'{load} {size} {len(pieces)} {len(relocs)}\n' + ''.join(f'{o} {n} {s} {t}\n' for o, n, s, t in pieces) + ''.join(f'{o} {t}\n' for o, t in relocs)

    def run_row(self, prog, load, size, pieces, relocs):
        d = Path(self.dir.name) / 'row'
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        self.prog.write_bytes(prog)
        (d / 'spec.txt').write_text(self.spec(load, size, pieces, relocs))
        r = subprocess.run([str(self.exe), 'row', str(self.prog), str(d / 'spec.txt'), str(d)], capture_output=True, text=True, timeout=60)
        self.assertIn(r.returncode, (0, 1), r.stdout + r.stderr)
        return r.returncode, (d / 'module.bin').read_bytes(), (d / 'known.bin').read_bytes()

    def test_rows_equal_the_model(self):
        m = arcade(0x800, 12)
        pieces = [(0x10, 8, 0x100, 'w4'), (0x20, 6, 0x200, 'b3'), (0x40, 12, 0x300, '(w1n1)4')]
        m = bytearray(m)
        m[0x300:0x320] = bytes([0, 5] * 16)
        relocs = [(0x30, 0x20), (0x34, 0x80118900), (0x3c, 0x0)]
        rc, img, known = self.run_row(bytes(m), 0x801b0000, 0x80, pieces, relocs)
        mimg, mknown = M.model_build(bytes(m), 0x80, 0x801b0000, [(o, n, s, t, 0.0) for o, n, s, t in pieces], relocs)
        self.assertEqual((rc, img, known), (0, mimg, mknown))
        self.assertEqual(sum(known), 8 + 6 + 12 + 12)
        # a piece that cannot be built (a value that does not fit) is skipped, the others are built, the status says so
        bad = list(pieces) + [(0x60, 2, 0x10, 'n2')]
        m2 = bytearray(m)
        m2[0x10:0x14] = bytes([1, 2, 3, 4])
        rc, img, known = self.run_row(bytes(m2), 0x801b0000, 0x80, bad, relocs)
        mimg, mknown = M.model_build(bytes(m2), 0x80, 0x801b0000, [(o, n, s, t, 0.0) for o, n, s, t in pieces], relocs)
        self.assertEqual((rc, img, known), (1, mimg, mknown))

    def valid(self, load, size, pieces, relocs):
        path = Path(self.dir.name) / 'valid.txt'
        path.write_text(self.spec(load, size, pieces, relocs))
        r = subprocess.run([str(self.exe), 'valid', str(path)], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        return int(r.stdout)

    def test_validity(self):
        ok = [(0x10, 8, 0x100, 'w4'), (0x20, 6, 0x200, 'b3')]
        self.assertEqual(self.valid(0x801b0000, 0x80, ok, [(0x30, 1), (0x34, 2)]), 0)
        self.assertEqual(self.valid(0x801b0000, 0x80, [], []), 0)
        cases = {
            'unsorted pieces': ([ok[1], ok[0]], []),
            'overlapping pieces': ([(0x10, 8, 0x100, 'w4'), (0x14, 6, 0x200, 'b3')], []),
            'piece off the end': ([(0x7c, 8, 0x100, 'w4')], []),
            'odd source': ([(0x10, 8, 0x101, 'w4')], []),
            'wrong size': ([(0x10, 6, 0x100, 'w4')], []),
            'bad layout': ([(0x10, 8, 0x100, 'w')], []),
            'empty piece': ([(0x10, 0, 0x100, 'w4')], []),
            'unsorted words': (ok, [(0x38, 1), (0x30, 1)]),
            'words overlap': (ok, [(0x30, 1), (0x32, 1)]),
            'word not aligned': (ok, [(0x32, 1)]),
            'word off the end': (ok, [(0x7e, 1)]),
            'word in a piece': (ok, [(0x14, 1)]),
        }
        for what, (pieces, relocs) in cases.items():
            self.assertEqual(self.valid(0x801b0000, 0x80, pieces, relocs), -1, what)
        self.assertEqual(self.valid(0x801b0002, 0x80, ok, []), -1)          # the link address is not 4-aligned

    def test_the_checked_in_table(self):
        r = subprocess.run([str(self.exe), 'tables'], capture_output=True, text=True, timeout=120)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)

    def test_32_bit_build(self):
        d = Path(self.dir.name)
        for src in (SOURCES[0], SOURCES[2]):
            subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / (src.stem + '32.o'))], check=True, timeout=120)


if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
