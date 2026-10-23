#!/usr/bin/env python3
"""Seconds: the effects and relocation code of port/arcade (program_more.c and its Python model in program_map.py), and the new
layout letters of program.c; no game files.

Python side: the width-changing piece letters ('n', 'e') and the pieces found for them, the decoding of a list's cell positions,
the matching of PS1 lists to arcade art records (in order, skipping records, tiles the arcade lacks as wildcards, flipped
classes), the cells a list leaves out, the descriptor (segments, drops, the gap of pictures), the model, the text of both
generated tables, and the per-class report. One made-up world (an arcade program, the PS1's lists made from it with cells left
out and two pictures of the PS1's own) goes through the whole derivation and must come out equal.

C side: program_more.c, built with the address and undefined-behavior sanitizers, against the model on made-up programs (flips,
parts, banks, repeated records, drops, a gap, no gap, random programs), refusals of what does not fit or is malformed (never an
overflow), the relocation writer's order and bounds, program.c's two new letters, the checked-in tables, and a 32-bit build.
"""
from pathlib import Path
import random, shutil, struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import fighters as F
import program_map as P

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SOURCES = [ROOT / 'port/arcade' / n for n in ('fighter.c', 'program.c', 'program_more.c', 'program_effects_table.c', 'program_relocs_table.c')]
DRIVER = ROOT / 'port/arcade/tests/program_more_test.c'


class Prog:
    """A made-up arcade program: art records one after the other from 0x1000, their coordinate blocks far away."""

    def __init__(self):
        self.data = bytearray(0xa0000)
        self.at, self.coord, self.n = 0x1000, 0x40000, 0

    def record(self, blocks):
        """Mode 2 record of blocks (code, attr, x, y). Returns its address."""
        a, idx = self.at, 4 * self.n
        self.n += 1
        d = self.data
        cells = sum((((attr >> 8) & 15) + 1) * (((attr >> 12) & 15) + 1) for _c, attr, _x, _y in blocks)
        struct.pack_into('>HHHh', d, a, 2, cells, len(blocks) - 1, idx)
        for k, (code, attr, _x, _y) in enumerate(blocks):
            struct.pack_into('>HH', d, a + 8 + 4 * k, code, attr)
        self.at = a + 8 + 4 * len(blocks)
        struct.pack_into('>I', d, F.COORD_TABLE + idx, self.coord)
        for k, b in enumerate(blocks):
            struct.pack_into('>hh', d, self.coord + 4 * k, b[2], b[3])
        self.coord += 4 * len(blocks)
        return a

    def view(self):
        return F.View(bytes(self.data))


def turn(b, k):
    return b''.join(b[i:i + k][::-1] for i in range(0, len(b), k))


def desc_of(segments, drops, positions_size, lists_size, gap=0, cols=0, rows=0, address=0x80172a48):
    return dict(address=address, positions_size=positions_size, lists_size=lists_size, segments=segments, drops=drops,
                gap_size=gap, picture_columns=cols, picture_rows=rows)


# ---------------------------------------------------------------- Python side

class Widths(unittest.TestCase):
    def test_narrow_and_widen(self):
        m = bytes([0, 5, 0, 200, 0, 0, 1, 2])
        self.assertEqual(P.build_run(m, 0, 3, 'n'), bytes([5, 200, 0]))
        with self.assertRaises(ValueError):
            P.build_run(m, 4, 2, 'n')                                  # 0x0001 fits, 0x0102 does not
        self.assertEqual(P.build_run(m, 1, 8, 'e'), bytes([5, 0, 0, 0, 0, 0, 0, 0]))
        self.assertEqual(P.build_run(m, 6, 8, 'e'), bytes([1, 0, 0, 0, 2, 0, 0, 0]))
        # the old letters and the new ones in one layout: b (2), e (4), n (1) is 7 PS1 bytes from 5 arcade bytes
        m2 = bytes([9, 8, 7, 0, 6, 1, 2, 3, 0, 4])
        self.assertEqual(P.build_run(m2, 0, 14, 'ben'), bytes([9, 8, 7, 0, 0, 0, 6, 1, 2, 3, 0, 0, 0, 4]))
        self.assertEqual(P.build_run(m2, 0, 4, 'w'), bytes([8, 9, 0, 7]))        # unchanged: 'w' turns two bytes
        self.assertEqual(P.build_run(m2, 0, 4, 'l'), bytes([0, 7, 8, 9]))

    def test_conv_pieces(self):
        m = bytes(8) + bytes([0, 1, 0, 2, 0, 3, 0, 255]) + bytes([7, 8, 9])
        q = bytes([1, 2, 3, 255]) + bytes([7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0])
        syms = {0x80170000: ['table_a']}
        convs = [(0, 4, 1, 2, 8, 100.0, 0), (4, 16, 4, 1, 16, 90.0, 0)]
        rows = P.conv_pieces(q, m, 0x80170000, convs, syms)
        self.assertEqual(rows, [(0x80170000, 4, 8, 'n', 'table_a'), (0x80170004, 12, 16, 'e', 'table_a+0x4')])
        # a run with a mismatch, of another width pair, or that does not rebuild is left out
        self.assertEqual(P.conv_pieces(q, m, 0x80170000, [(0, 4, 1, 2, 8, 100.0, 1)], syms), [])
        self.assertEqual(P.conv_pieces(q, m, 0x80170000, [(0, 4, 2, 4, 8, 100.0, 0)], syms), [])
        self.assertEqual(P.conv_pieces(bytes([9, 9, 9, 9]), m, 0x80170000, [(0, 4, 1, 2, 8, 100.0, 0)], syms), [])
        self.assertEqual(P.conv_pieces(q, m, 0x80170000, [(0, 4, 1, 2, 9, 100.0, 0)], syms), [])          # odd arcade address


class Relocs(unittest.TestCase):
    def test_rows_and_table(self):
        details = {'pointers': {8: (0x80118cd4, 'ptr_code'), 0: (0x801e9f44, 'ptr_module'), 4: (0x8016dc00, 'ptr_data'), 12: (0x80010000, 'ptr_other'),
                                16: (0x80118e00, 'jtbl')}}
        rows = P.reloc_rows(details, 0x8016d91c)
        self.assertEqual(rows, [(0x8016d91c, 0x801e9f44, 'ptr_module'), (0x8016d920, 0x8016dc00, 'ptr_data'),
                                (0x8016d924, 0x80118cd4, 'ptr_code'), (0x8016d928, 0x80010000, 'ptr_other')])
        text = P.relocs_c_table(rows, {0x80118cd4: ['handler']})
        self.assertIn('{0x8016d924u, 0x80118cd4u},  /* ptr_code handler+0x0 */', text)
        self.assertEqual(text.count('{0x80'), 4)
        self.assertIn('arcade_reloc_count', text)
        self.assertNotIn('\N{EM DASH}', text)


class World:
    """A made-up arcade program, the PS1's lists made from some of its records (cells left out, a gap of two pictures, a
    terminator), and everything the derivation takes."""

    def __init__(self):
        p = Prog()
        self.prog = p
        rec = p.record
        self.records = [
            rec([(0x100, 0x0001, -16, -16)]),                                             # 1 cell
            rec([(0x110, 0x1101, -32, -32), (0x120, 0x0002, 16, -32)]),                   # 4 + 1 cells
            rec([(0x130, 0x0201, 0, 0), (0x140, 0x0021, 48, 0), (0x150, 0x0041, 16, 32)]),   # 3 + 1 + 1
            rec([(0x160, 0x0001, -16, -16), (0x161, 0x0001, 0, -16)]),                    # two cells
            rec([(0x170, 0x0101, -16, 0)]),                                               # 2 cells
            rec([(0x180, 0x0001, -16, 0), (0x190, 0x0001, -32, 0)]),                      # two parts (x steps back)
            rec([(0x1a0, 0x0001, -8, 8)]),
        ]
        # records 1, 2, 3 and 5 lose cells (indexes in the record's own order); record 4 is skipped (the PS1 has no list for it)
        self.drops = {self.records[1]: {1}, self.records[2]: {0, 4}, self.records[3]: {1}, self.records[5]: {0}}
        self.used = [r for i, r in enumerate(self.records) if i != 4]
        view = p.view()
        conv = F.Converter()
        self.truth_segments = []
        for a in self.used:
            r = F.read_any(view, a)
            conv.record([c for i, c in enumerate(r[1]) if i not in self.drops.get(a, ())], 0, a)
        built = b''.join(m[0] for m in conv.mine)
        pos = bytes(conv.pos.data)
        self.code_of_num = dict(conv.code_of_num)
        self.cols, self.rows = 3, 2
        stream = struct.pack('<hhH', 0, 0, 2) + struct.pack('<hhH', -32, 16, 2)
        pic_pos = len(pos)
        pics = b''.join(F.encode_list([(600 + 6 * k + i, 0x21) for i in range(6)], pic_pos) for k in range(2))
        term = struct.pack('<5HH', 1, 0, 0, 0, len(pos) + len(stream), 0)
        self.positions = pos + stream + bytes(6)
        self.lists_blob = built + pics + term
        self.gap = len(pics)
        self.built_len = len(built)
        self.view = view
        self.sorted_records = sorted((a, F.read_any(view, a)) for a in self.records)

    def lists(self):
        out, o = [], 0
        while o < len(self.lists_blob):
            lst = F.parse_list(self.lists_blob, o)
            out.append(dict(lst, at=o))
            o = lst['end']
        return out

    def tile_classes(self, t):
        return (self.code_of_num.get(t & 0x3fff, -1), ())

    def match(self):
        return P.match_effect_lists(self.lists(), self.positions, self.tile_classes, self.sorted_records, lambda c: c, F.BANKS, F.is_empty)


class Effects(unittest.TestCase):
    def test_cell_positions(self):
        positions = struct.pack('<hhH', 4, -8, 2) + struct.pack('<hhH', -30, 16, 0)
        lst = dict(n=4, pos=0)
        self.assertEqual(P.effect_cell_positions(positions, lst), [(4, -8), (20, -8), (36, -8), (6, 8)])
        self.assertEqual(P.effect_cell_positions(positions, dict(n=2, pos=0)), None)          # the first run holds 3 cells
        self.assertEqual(P.effect_cell_positions(positions, dict(n=3, pos=0)), [(4, -8), (20, -8), (36, -8)])
        self.assertIsNone(P.effect_cell_positions(positions, dict(n=5, pos=0)))              # runs out of positions
        self.assertEqual(P.effect_cell_positions(positions, dict(n=1, pos=12)), None)

    def test_dropped_cells(self):
        cells = [(1, 0, 0), (2, 16, 0), (3, 0, 16), (4, 16, 16)]
        self.assertEqual(P.dropped_cells(cells, [(0, 0), (0, 16)]), [1, 3])
        self.assertEqual(P.dropped_cells(cells, [(0, 0), (16, 0), (0, 16), (16, 16)]), [])
        self.assertIsNone(P.dropped_cells(cells, [(0, 0), (99, 99)]))
        self.assertEqual(P.dropped_cells([(1, 0, 0), (2, 0, 0)], [(0, 0)]), [1])               # equal positions: the later one is left out

    def test_the_world_end_to_end(self):
        w = World()
        matches = w.match()
        lists = w.lists()
        self.assertEqual(len(lists), len(w.used) + 2 + 1)
        self.assertEqual(matches[:len(w.used)], [(a, 0) for a in w.used])
        self.assertEqual(matches[len(w.used):], [None, None, None])                            # the pictures and the terminator
        records = dict(w.sorted_records)
        desc = P.effects_descriptor(lists, w.positions, matches, records, F.is_empty, len(w.positions), len(w.lists_blob), 0x80172a48)
        # consecutive records make one segment: the records 0..3 are consecutive in the program, 4 is skipped, 5..6 follow
        self.assertEqual(desc['segments'], [(w.records[0], 4, 0), (w.records[5], 2, 0)])
        self.assertEqual(sorted(desc['drops']), sorted((a, i) for a, s in w.drops.items() for i in s))
        self.assertEqual((desc['gap_size'], desc['picture_columns'], desc['picture_rows']), (w.gap, 3, 2))
        data, known = P.effects_model(w.view, desc)
        truth = w.positions + w.lists_blob
        self.assertEqual(len(data), len(truth))
        self.assertEqual(bytes(d for d, k in zip(data, known) if k), bytes(t for t, k in zip(truth, known) if k))
        self.assertEqual(sum(known), len(truth) - w.gap)
        self.assertEqual(data[len(w.positions) + w.built_len:][:w.gap], bytes(w.gap))          # the gap is zero and not known
        text = P.effects_c_table(desc)
        self.assertIn(f'{{{w.records[0]:#x}u, 4u, 0x0u}},', text)
        self.assertIn('arcade_effects = {', text)
        self.assertIn(f'{w.gap}u, 3u, 2u,', text)
        self.assertNotIn('\N{EM DASH}', text)

    def test_the_matching(self):
        w = World()
        lists = w.lists()
        # the lists follow the records in order; when none follows, the search starts again from the first
        reorder = [lists[2], lists[0], lists[1]] + lists[3:]
        got = P.match_effect_lists(reorder, w.positions, w.tile_classes, w.sorted_records, lambda c: c, F.BANKS, F.is_empty)
        self.assertEqual(got[:3], [(w.used[2], 0), (w.used[0], 0), (w.used[1], 0)])
        # a tile the arcade has not (class -1) is a wildcard; a wrong class is not
        wild = lambda t: (-1, ()) if t == lists[0]['cells'][0][0] else w.tile_classes(t)
        self.assertEqual(P.match_effect_lists(lists[:1], w.positions, wild, w.sorted_records, lambda c: c, F.BANKS, F.is_empty), [(w.used[0], 0)])
        wrong = lambda t: (0x7777, ())
        self.assertEqual(P.match_effect_lists(lists[:1], w.positions, wrong, w.sorted_records, lambda c: c, F.BANKS, F.is_empty), [None])
        # a flipped class counts: the tile is stored flipped, and the class of its flipped pixels is the record's
        flipped = lambda t: (0x7777, (w.tile_classes(t)[0],)) if t == lists[0]['cells'][0][0] else w.tile_classes(t)
        self.assertEqual(P.match_effect_lists(lists[:1], w.positions, flipped, w.sorted_records, lambda c: c, F.BANKS, F.is_empty), [(w.used[0], 0)])
        # the bank: the record's codes plus the bank are the classes
        bank = lambda t: (w.tile_classes(t)[0] + 0x20000, ())
        self.assertEqual(P.match_effect_lists(lists[:1], w.positions, bank, w.sorted_records, lambda c: c, F.BANKS, F.is_empty), [(w.used[0], 0x20000)])

    def test_descriptor_refuses_what_it_cannot_describe(self):
        w = World()
        lists, records = w.lists(), dict(w.sorted_records)
        m = w.match()
        d = lambda lists_, m_: P.effects_descriptor(lists_, w.positions, m_, records, F.is_empty, len(w.positions), len(w.lists_blob), 0x80172a48)
        with self.assertRaises(ValueError):                                                   # a list with a record after one without
            d(lists, [m[0], None] + m[2:])
        with self.assertRaises(ValueError):                                                   # no empty list at the end
            d(lists[:-1], m[:-1])
        with self.assertRaises(ValueError):                                                   # an empty list in the middle
            d(lists[:2] + [lists[-1]] + lists[2:], m[:2] + [None] + m[2:])
        bad = [dict(lst) for lst in lists]
        bad[-3]['n'] = 5                                                                      # pictures that differ
        with self.assertRaises(ValueError):
            d(bad, m)
        # a block with no gap needs no pictures
        nogap = P.effects_descriptor(lists[:len(w.used)] + [lists[-1]], w.positions, m[:len(w.used)] + [None], records, F.is_empty, 0, 0, 1)
        self.assertEqual((nogap['gap_size'], nogap['picture_columns'], nogap['picture_rows']), (0, 0, 0))

    def test_model_checks_its_sizes(self):
        w = World()
        desc = P.effects_descriptor(w.lists(), w.positions, w.match(), dict(w.sorted_records), F.is_empty, len(w.positions), len(w.lists_blob), 1)
        P.effects_model(w.view, desc)
        for key, delta in (('positions_size', -7), ('lists_size', 1), ('lists_size', -1), ('gap_size', 1)):
            bad = dict(desc, **{key: desc[key] + delta})
            with self.assertRaises(ValueError):
                P.effects_model(w.view, bad)
        with self.assertRaises(ValueError):                                                   # a record that is not there
            P.effects_model(w.view, dict(desc, segments=[(0x8, 1, 0)]))


class Report(unittest.TestCase):
    def test_class_report(self):
        cls = ['piece', 'piece', 'zero', 'ptr_code', 'ptr_code', 'other']
        q = bytes([1, 2, 0, 5, 6, 9])
        image = bytearray([1, 2, 0, 5, 7, 0])
        built = bytearray([1, 1, 0, 1, 1, 0])
        lines, nb, ne = P.class_report(cls, q, image, built)
        self.assertEqual((nb, ne), (4, 3))
        self.assertTrue(any('piece' in ln and 'DIFFER' not in ln for ln in lines))
        self.assertTrue(any('ptr_code' in ln and 'DIFFER 1' in ln for ln in lines))
        self.assertFalse(any('arcade_hi' in ln for ln in lines))


# ---------------------------------------------------------------- C side

def spec_text(desc):
    segs, drops = desc['segments'], sorted(desc['drops'])
    return (f'{desc["address"]} {desc["positions_size"]} {desc["lists_size"]} {desc["gap_size"]} {desc["picture_columns"]} {desc["picture_rows"]} '
            f'{len(segs)} {len(drops)}\n' + ''.join(f'{a} {c} {b}\n' for a, c, b in segs) + ''.join(f'{a} {i}\n' for a, i in drops))


class EffectsC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-more-')
        d = Path(cls.dir.name)
        cls.exe = d / 'program_more_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES), str(DRIVER), '-o', str(cls.exe)],
                       check=True, timeout=120)
        cls.cases = 0

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_c(self, prog, desc):
        """(status, text, data, known) of the C for the descriptor."""
        EffectsC.cases += 1
        d = Path(self.dir.name) / 'case'
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        (d / 'prog.bin').write_bytes(bytes(prog.data))
        (d / 'spec.txt').write_text(spec_text(desc))
        r = subprocess.run([str(self.exe), 'effects', str(d / 'prog.bin'), str(d / 'spec.txt'), str(d)], capture_output=True, text=True, timeout=60)
        if r.returncode:
            return r.returncode, r.stdout + r.stderr, None, None
        return 0, r.stdout, (d / 'data.bin').read_bytes(), (d / 'known.bin').read_bytes()

    def same(self, prog, desc):
        """The C equals the model, byte for byte, in data and in what it knows."""
        data, known = P.effects_model(prog.view(), desc)
        status, text, cdata, cknown = self.run_c(prog, desc)
        self.assertEqual(status, 0, text)
        self.assertEqual(cdata, data)
        self.assertEqual(cknown, known)
        return data, known

    def test_the_world(self):
        w = World()
        desc = P.effects_descriptor(w.lists(), w.positions, w.match(), dict(w.sorted_records), F.is_empty, len(w.positions), len(w.lists_blob), 0x80172a48)
        data, known = self.same(w.prog, desc)
        truth = w.positions + w.lists_blob
        self.assertEqual(bytes(d for d, k in zip(data, known) if k), bytes(t for t, k in zip(truth, known) if k))

    def sized(self, prog, segments, drops, gap=0, cols=0, rows=0, extra_positions=0):
        """A descriptor whose sizes are what the model needs."""
        conv = F.Converter()
        skip = set(drops)
        view = prog.view()
        for a, count, bank in segments:
            for _ in range(count):
                if a == 0:
                    conv.empty()
                    continue
                r = F.read_any(view, a)
                conv.record([c for i, c in enumerate(r[1]) if (a, i) not in skip], bank, a)
                a = r[2]
        built = sum(len(m[0]) for m in conv.mine)
        pos = len(conv.pos.data) + (6 * rows if cols else 0)
        return desc_of(segments, sorted(drops), pos + extra_positions, built + gap + 12, gap, cols, rows)

    def test_encodings_flips_parts_banks_and_repeats(self):
        p = Prog()
        a = [p.record([(0x100, 0x0701, 0, 0)]),                                    # eight tiles in a row: one run
             p.record([(0x100, 0x0001, 0, 0), (0x103, 0x0001, 32, 0)]),            # plain, apart
             p.record([(0x100, 0x0021, 0, 0), (0x102, 0x0041, 16, 0), (0x105, 0x0061, 32, 0)]),   # flips
             p.record([(0x200, 0x0301, 0, 0), (0x220, 0x0301, -32, 16)]),           # two parts (x steps back)
             p.record([(0x300, 0x0002, -300, 0)])]                                  # far left
        segs = [(a[0], 1, 0), (a[1], 1, 0x10000), (a[2], 3, 0x20000), (a[0], 1, 0x30000), (a[4], 1, 0), (a[4], 1, 0), (a[3], 1, 0)]
        self.same(p, self.sized(p, segs, set()))
        self.same(p, self.sized(p, [(a[3], 1, 0), (a[3], 1, 0)], set()))

    def test_drops_gap_and_pictures(self):
        p = Prog()
        a = [p.record([(0x100, 0x1101, 0, 0), (0x120, 0x0003, 48, 16)]), p.record([(0x140, 0x0301, -16, 0)])]
        drops = {(a[0], 1), (a[0], 4), (a[1], 3)}
        segs = [(a[0], 2, 0)]
        base = self.sized(p, segs, drops)
        self.same(p, base)
        self.same(p, self.sized(p, segs, drops, gap=100, cols=24, rows=14))
        self.same(p, self.sized(p, segs, drops, gap=0, cols=1, rows=1))
        self.same(p, self.sized(p, segs, drops, gap=7, extra_positions=40))             # zeros after the positions
        self.same(p, self.sized(p, [(0, 3, 0), (a[0], 1, 0), (0, 1, 0)], set()))       # empty lists in the middle
        d = self.same(p, self.sized(p, segs, drops, gap=100))
        self.assertEqual(sum(d[1]), len(d[1]) - 100)

    def test_refusals(self):
        p = Prog()
        a = [p.record([(0x100, 0x1101, 0, 0)]), p.record([(0x140, 0x0001, 0, 0)])]
        good = self.sized(p, [(a[0], 2, 0)], {(a[0], 1)}, gap=20, cols=2, rows=2)
        self.same(p, good)
        refused = lambda desc, text: (lambda r: (self.assertEqual(r[0], 1, r[1]), self.assertIn(text, r[1]), self.assertNotIn('left allocated', r[1])))(self.run_c(p, desc))
        refused(dict(good, lists_size=good['lists_size'] + 1), 'the lists come to')
        refused(dict(good, lists_size=good['lists_size'] - 1), 'lists need more than their room')
        refused(dict(good, positions_size=good['positions_size'] - 1), 'positions')
        refused(dict(good, gap_size=good['lists_size']), 'gap does not fit')
        refused(dict(good, gap_size=good['lists_size'] - 11), 'gap does not fit')
        refused(dict(good, drops=[(a[0], 1), (a[0], 4)]), 'not among its 4 cells')
        refused(dict(good, drops=[(a[0], i) for i in range(4)]), 'every cell is left out')
        refused(dict(good, segments=[(a[0], 2, 0), (0x9ffff0, 1, 0)]), 'outside the program')
        struct.pack_into('>H', p.data, 0x30, 5)
        refused(dict(good, segments=[(a[0], 2, 0), (0x30, 1, 0)]), 'not an art record')
        refused(dict(good, picture_rows=65), 'picture rows')
        # a record whose blocks do not add up to its cells
        struct.pack_into('>H', p.data, a[1] + 2, 3)
        refused(dict(good, drops=[]), 'blocks add up')
        # a position step that does not fit 16 bits
        q = Prog()
        b = q.record([(0x100, 0x0001, -30000, 0), (0x101, 0x0001, 30000, 0), (0x102, 0x0001, 29999, 16)])
        r = self.run_c(q, desc_of([(b, 1, 0)], [], 100, 100))
        self.assertEqual(r[0], 1)
        self.assertNotIn('left allocated', r[1])

    def test_random_programs(self):
        rnd = random.Random(7)
        for _case in range(25):
            p = Prog()
            recs = []
            for _ in range(rnd.randint(2, 7)):
                blocks = []
                for _b in range(rnd.randint(1, 4)):
                    w_, h_ = rnd.randint(1, 3), rnd.randint(1, 2)
                    attr = ((h_ - 1) << 12) | ((w_ - 1) << 8) | rnd.choice([0, 0x20, 0x40, 0x60]) | rnd.randint(0, 3)
                    blocks.append((rnd.randint(0x100, 0x2f0), attr, rnd.randint(-200, 200) // 16 * 16, rnd.randint(-100, 100) // 16 * 16))
                recs.append(p.record(blocks))
            segs, drops = [], set()
            for _ in range(rnd.randint(1, 5)):
                a = rnd.choice(recs)
                n = 1 + sum(1 for r in recs if r > a) if rnd.random() < 0.5 else 1
                segs.append((a, min(n, 1 + sum(1 for r in recs if r >= a) - 1), rnd.choice([0, 0x10000, 0x20000])))
            view = p.view()
            for a, count, _bank in segs:
                x = a
                for _ in range(count):
                    r = F.read_any(view, x)
                    for i in range(len(r[1])):
                        if rnd.random() < 0.15 and sum(1 for (aa, _i) in drops if aa == x) + 1 < len(r[1]):
                            drops.add((x, i))
                    x = r[2]
            desc = self.sized(p, segs, drops, gap=rnd.choice([0, 0, 30]), cols=rnd.choice([0, 3]), rows=rnd.choice([1, 4]))
            self.same(p, desc)
        self.assertGreater(EffectsC.cases, 25)

    def test_pieces_relocs_and_tables(self):
        for what in ('pieces', 'relocs', 'tables'):
            r = subprocess.run([str(self.exe), what], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stdout + r.stderr)

    def test_32_bit_build_has_no_warnings(self):
        have = subprocess.run(['gcc', '-m32', '-x', 'c', '-', '-c', '-o', '/dev/null'], input='int x;', capture_output=True, text=True)
        if have.returncode:
            self.skipTest('no 32-bit gcc')
        d = Path(self.dir.name)
        for src in SOURCES:
            subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / (src.stem + '32.o'))], check=True, timeout=120)


class Tables(unittest.TestCase):
    def test_checked_in_tables_have_no_game_bytes_and_the_pieces_parse(self):
        for name in ('program_effects_table.c', 'program_relocs_table.c'):
            text = (ROOT / 'port/arcade' / name).read_text()
            self.assertTrue(text.startswith('/* GENERATED by'))
            self.assertNotIn('\N{EM DASH}', text)
        pieces = P.parse_pieces((ROOT / 'port/arcade/program.c').read_text())
        self.assertGreaterEqual(len(pieces), 78)
        self.assertEqual(pieces, sorted(pieces))
        self.assertTrue({'n', 'e'} <= {p[3] for p in pieces})
        for a, size, _src, _u, _n in pieces:
            self.assertTrue(P.DATA_START <= a and a + size <= P.DATA_END)
        for (a, size, *_), (b, *_rest) in zip(pieces, pieces[1:]):
            self.assertLessEqual(a + size, b)


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
