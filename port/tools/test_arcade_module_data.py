#!/usr/bin/env python3
"""Seconds: the rest of a fighter module's data (port/arcade/module_data.py and module_data.c); no game files.

Python side, on made-up data: the generated tables' parsers, the five blocks' spans, exact runs of 16-bit and 32-bit fields (as they are,
turned), zero trimming, the layout with the fewest letter changes and its periodic form, the layout text (tokens, groups, malformed
ones), the field letters (n, f, q and the others, with what does not fit), the converted records (templates: the inverse of the field
letters, the search, the pieces, a second side that differs), the search of the arcade program for a module's tables, the pointer words
and their classes, the byte classes, the model of the C and the text of the generated table.

C side: module_data.c, built with the address and undefined-behavior sanitizers, against the model on made-up programs and layouts
(every letter, groups, a value that does not fit, a layout that makes the wrong size, a piece outside the program, digits that would
overflow), against program.c's piece builder for the letters they share, rows (pieces and words, a bad piece skipped and the others
built, a wrong module size), the validity checks, the checked-in table (45 fighter modules, no piece or word inside a block that
fighter.c or sequence.c builds), and a 32-bit build.
"""
from pathlib import Path
import random, shutil, struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import module_data as M
import program_map as P

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
        for bad in ('', 'b', 'b0', 'x3', '(w2n2)', '(w2n2)0', '()3', '(w2(n2))3', 'b2 ', 'b2w', '(w2n2', 'w2)3', 'B2'):
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
        for l in text.splitlines():
            if l.startswith('    {0x'):
                self.assertRegex(l, r'^    \{0x[0-9a-f]+u, (0x[0-9a-f]+u\},|\d+u, 0x[0-9a-f]+u, "[a-z0-9()]+"\},)$')


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
