#!/usr/bin/env python3
"""Seconds: the runtime C of the fighter lists (port/arcade/fighter.c) against the Python model; no game files.

Builds fighter.c, the generated descriptor table and a driver with the address and undefined-behavior sanitizers,
makes small arcade programs (art records of mode 2 and mode 0 with flips, parts, palettes, banks, repeated records,
empty lists) and checks that the C gives, byte for byte, the lists, the position runs and the tile numbering with
stored flips that fighters.py's model_build gives, for each rule and each list encoding, then for random programs.
It also checks that a block that does not fit its room, a record outside the program and a record whose blocks do not
add up are refused (never an overflow), that the generated table is sane, that the C compiles warning-free for 32
bits, and the helpers that write the table.
"""
from pathlib import Path
import random, shutil, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import fighters as F

PALETTE_PARTS, NUMBER_BY_RUN, TERMINATOR = 1, 2, 4
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SOURCES = [ROOT / 'port/arcade' / n for n in ('fighter.c', 'fighters_table.c')]


class Prog:
    """A made-up arcade program: art records one after the other from 0x1000, their coordinate blocks far away."""

    def __init__(self):
        self.data = bytearray(0xa0000)
        self.at, self.coord, self.n = 0x1000, 0x40000, 0

    def record(self, blocks, shared=None):
        """Mode 2 record of blocks (code, attr, x, y), or with `shared` an attribute word a mode 0 record of blocks
        (code, x, y). Returns its address."""
        a, idx = self.at, 4 * self.n
        self.n += 1
        d = self.data
        if shared is None:
            cells = sum((((attr >> 8) & 15) + 1) * (((attr >> 12) & 15) + 1) for _c, attr, _x, _y in blocks)
            struct.pack_into('>HHHh', d, a, 2, cells, len(blocks) - 1, idx)
            for k, (code, attr, _x, _y) in enumerate(blocks):
                struct.pack_into('>HH', d, a + 8 + 4 * k, code, attr)
            self.at = a + 8 + 4 * len(blocks)
        else:
            struct.pack_into('>HHHh', d, a, 0, len(blocks), shared, idx)
            for k, (code, _x, _y) in enumerate(blocks):
                struct.pack_into('>H', d, a + 8 + 2 * k, code)
            self.at = a + 8 + 2 * len(blocks)
        struct.pack_into('>I', d, F.COORD_TABLE + idx, self.coord)
        for k, b in enumerate(blocks):
            x, y = (b[2], b[3]) if shared is None else (b[1], b[2])
            struct.pack_into('>hh', d, self.coord + 4 * k, x, y)
        self.coord += 4 * len(blocks)
        return a

    def view(self):
        return F.View(bytes(self.data))


def parse_lists(blob):
    out, o = [], 0
    while o < len(blob):
        lst = F.parse_list(blob, o)
        assert lst, f'not a list at {o}'
        out.append(lst)
        o = lst['end']
    return out


class FighterC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-fighter-')
        d = Path(cls.dir.name)
        cls.exe = d / 'fighter_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES),
                        str(ROOT / 'port/arcade/tests/fighter_test.c'), '-o', str(cls.exe)], check=True, timeout=120)
        cls.cases = 0

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_c(self, prog, segs, rules=0, positions_room=1 << 16, lists_room=1 << 16):
        """(status, text, lists, positions, tiles {number: (code, flip bits)}) of the C for the segments."""
        FighterC.cases += 1
        d = Path(self.dir.name) / 'case'
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        (d / 'prog.bin').write_bytes(bytes(prog.data))
        (d / 'spec.txt').write_text(f'{rules} {positions_room} {lists_room} {len(segs)}\n' + ''.join(f'{a} {c} {b}\n' for a, c, b in segs))
        r = subprocess.run([str(self.exe), 'build', str(d / 'prog.bin'), str(d / 'spec.txt'), str(d)], capture_output=True, text=True, timeout=60)
        if r.returncode:
            return r.returncode, r.stdout + r.stderr, None, None, None
        tiles = {}
        for line in (d / 'tiles.txt').read_text().split('\n'):
            if line:
                n, code, flip = map(int, line.split())
                tiles[n] = (code, flip)
        return 0, r.stdout, (d / 'lists.bin').read_bytes(), (d / 'positions.bin').read_bytes(), tiles

    def model(self, prog, segs, rules=0):
        return F.model_build(prog.view(), segs, bool(rules & PALETTE_PARTS), bool(rules & NUMBER_BY_RUN), bool(rules & TERMINATOR))

    def same(self, prog, segs, rules=0):
        """The C equals the model in lists, positions and numbering; the exact room is enough and one byte less is refused."""
        m_lists, m_pos, m_codes, m_bake = self.model(prog, segs, rules)
        status, text, lists, pos, tiles = self.run_c(prog, segs, rules)
        self.assertEqual(status, 0, text)
        self.assertEqual(lists, m_lists)
        self.assertEqual(pos, m_pos)
        flips = {c: (1 if f & 0x20 else 0) | (2 if f & 0x40 else 0) for c, f in m_bake.items()}
        self.assertEqual(tiles, {n: (code, flips[code]) for n, code in m_codes.items()})
        self.assertEqual(sorted(tiles), list(range(1, len(tiles) + 1)))
        status, text, *_ = self.run_c(prog, segs, rules, len(m_pos), len(m_lists))
        self.assertEqual(status, 0, text)
        status, text, *_ = self.run_c(prog, segs, rules, len(m_pos), len(m_lists) - 1)
        self.assertEqual(status, 1)
        self.assertIn('lists need more than their room', text)
        if m_pos:
            status, text, *_ = self.run_c(prog, segs, rules, len(m_pos) - 1, len(m_lists))
            self.assertEqual(status, 1)
            self.assertIn('positions need more than their room', text)
        return m_lists, m_pos, m_codes, m_bake

    # ------------------------------------------------------------ the encodings and the basic rules

    def test_all_four_encodings_and_both_ties(self):
        p = Prog()
        rec = lambda *cells: p.record([(code, attr, 16 * i if gap is None else gap, 32) for i, (code, attr, gap) in enumerate(cells)])
        one = lambda code, attr, x: (code, attr, x)
        first = p.record([(1, 0x0701, 0, 0)])                                              # eight new tiles (1 to 8) in a row: one run
        steps = [
            ('run', first),
            ('plain, apart', rec(one(1, 0x0001, 0), one(3, 0x0001, 32))),                  # tiles 1 and 3: 2 cells, 2 runs: mode 0
            ('two palettes, apart', rec(one(1, 0x0001, 0), one(3, 0x0002, 32))),           # mode 2
            ('run tie', rec(one(1, 0x0001, 0), one(2, 0x0001, 16), one(5, 0x0001, 64), one(6, 0x0001, 80))),    # 4 cells, 2 runs: tie, mode 4
            ('flag tie', rec(one(1, 0x0021, 0), one(2, 0x0021, 16), one(7, 0x0021, 32))),                       # 3 cells, 2 runs: tie, mode 6
            ('flag run', rec(one(1, 0x0021, 0), one(2, 0x0021, 16), one(3, 0x0021, 32))),                       # 3 cells, 1 run: mode 6
            ('palettes alternate', p.record([(1 + i, 0x0001 + (i % 2), 16 * i, 0) for i in range(6)])),         # 6 cells, 6 runs: mode 2
        ]
        segs = [(a, 1, 0) for _name, a in steps]
        lists, *_ = self.same(p, segs)
        modes = [l['mode'] for l in parse_lists(lists)]
        self.assertEqual(modes, [4, 0, 2, 4, 6, 6, 2])
        self.assertEqual(set(modes), {0, 2, 4, 6})

    def test_flips_in_blocks_and_stored_flips(self):
        p = Prog()
        # a 2 x 2 block drawn plain, flipped across, flipped down, both; tiles first used flipped are stored flipped
        recs = [p.record([(100, 0x1100 | fl, 0, 0)]) for fl in (0x00, 0x20, 0x40, 0x60)]
        recs.append(p.record([(200, 0x0120, -16, 16), (300, 0x0140, 16, 16), (400, 0x0160, 48, 16)]))
        recs.append(p.record([(200, 0x0100, -16, 16), (300, 0x0120, 16, 16), (400, 0x0160, 48, 16)]))   # later uses with other flips
        segs = [(a, 1, 0) for a in recs]
        _lists, _pos, codes, bake = self.same(p, segs)
        self.assertEqual(bake[200], 0x20)
        self.assertEqual(bake[300], 0x40)
        self.assertEqual(bake[400], 0x60)
        self.assertEqual(bake[100], 0)

    def test_mode_zero_records_and_banks_and_empty_lists_and_terminator(self):
        p = Prog()
        m0 = p.record([(50, 0, 0), (51, 16, 0), (60, 0, 16)], shared=0x0023)          # three 1 x 1 blocks, shared palette 3, flip across
        m2 = p.record([(70, 0x0100, 0, 0)])
        segs = [(0, 2, 0), (m0, 1, 0x10000), (0, 1, 0), (m2, 1, 0x30000), (m0, 1, 0)]
        lists, pos, codes, bake = self.same(p, segs, TERMINATOR)
        ls = parse_lists(lists)
        self.assertEqual([l['n'] for l in ls], [1, 1, 3, 1, 2, 3, 1])
        self.assertEqual(ls[-1]['pos'], len(pos))                                       # the terminator's offset is the positions' size
        self.assertEqual(codes[1], 60 + 0x10000)                                        # the part that comes second in the record comes first
        self.assertEqual(bake[60 + 0x10000], 0x20)
        self.assertEqual(self.same(p, segs, 0)[0], lists[:-12])                           # without the flag, no terminator

    def test_parts_run_in_reverse_and_palette_changes_split_only_with_the_rule(self):
        p = Prog()
        # x steps back: two parts; the second part's cells come first
        a = p.record([(1, 0x0001, 0, 16), (2, 0x0001, 16, 0), (3, 0x0001, -32, 32), (4, 0x0001, -16, 0)])
        # a palette change inside a part
        b = p.record([(5, 0x0001, 0, 0), (6, 0x0002, 16, 0), (7, 0x0002, 32, 0), (8, 0x0001, 48, 0)])
        segs = [(a, 1, 0), (b, 1, 0)]
        for rules in (0, PALETTE_PARTS):
            self.same(p, segs, rules)
        m0, m1 = self.model(p, segs, 0), self.model(p, segs, PALETTE_PARTS)
        self.assertNotEqual(m0[1:3], m1[1:3])                                           # the rule changes the positions and the codes, and C follows

    def test_numbering_by_run_of_multi_part_records(self):
        p = Prog()
        recs = []
        for r in range(6):
            blocks = [(10 * r + 1, 0x0001, 0, 0), (10 * r + 2, 0x0001, 16, 0), (10 * r + 3, 0x0001, 32, 0),
                      (10 * r + 4, 0x0002, -16, 16), (10 * r + 5, 0x0002, 0, 16), (10 * r + 6, 0x0001, -48, 32)]
            recs.append(p.record(blocks))
        single = p.record([(500, 0x0001, 0, 0), (501, 0x0001, 16, 0)])
        segs = [(recs[0], 3, 0x20000), (single, 1, 0x20000), (recs[3], 3, 0x20000), (0, 1, 0), (recs[0], 2, 0x20000)]
        rules = PALETTE_PARTS | NUMBER_BY_RUN
        self.same(p, segs, rules)
        self.same(p, segs, NUMBER_BY_RUN)
        self.assertNotEqual(self.model(p, segs, rules)[2], self.model(p, segs, PALETTE_PARTS)[2])

    def test_a_record_drawn_again_far_left_is_moved_and_its_stream_stored_apart(self):
        p = Prog()
        a = p.record([(1, 0x0001, -300, 0), (2, 0x0001, -284, 0)])
        b = p.record([(3, 0x0001, -100, 0), (4, 0x0001, -84, 0)])
        segs = [(a, 1, 0), (a, 1, 0), (b, 1, 0), (b, 1, 0), (a, 1, 0)]
        lists, pos, *_ = self.same(p, segs)
        ls = parse_lists(lists)
        # a, a again (moved to x = 0, stored apart), b, b again (not far left: the stream is shared), a again (moved, apart again)
        self.assertEqual([l['pos'] for l in ls], [0, 6, 12, 12, 18])
        self.assertEqual(len(pos), 24)
        self.assertEqual(struct.unpack_from('<h', pos, 0)[0], -300)
        self.assertEqual(struct.unpack_from('<h', pos, 6)[0], 0)
        self.assertEqual(struct.unpack_from('<h', pos, 18)[0], 0)

    def test_the_far_left_limit_is_exclusive(self):
        p = Prog()
        edge, past = p.record([(1, 0x0001, -256, 0)]), p.record([(2, 0x0001, -257, 0)])
        _l, pos, *_ = self.same(p, [(edge, 1, 0), (edge, 1, 0), (past, 1, 0), (past, 1, 0)])
        self.assertEqual(len(pos), 18)                       # edge shared (6), past stored (6) and drawn again moved apart (6)

    def test_identical_streams_are_stored_once(self):
        p = Prog()
        a = p.record([(1, 0x0001, 0, 0), (2, 0x0001, 16, 0)])
        b = p.record([(3, 0x0001, 0, 0), (4, 0x0001, 16, 0)])
        c = p.record([(5, 0x0001, 0, 0), (6, 0x0001, 32, 0)])
        _l, pos, *_ = self.same(p, [(a, 1, 0), (b, 1, 0), (c, 1, 0)])
        self.assertEqual(len(pos), 6 + 12)

    # ------------------------------------------------------------ refusals

    def refused(self, p, segs, text, **room):
        status, out, *_ = self.run_c(p, segs, **room)
        self.assertEqual(status, 1, out)
        self.assertIn(text, out)
        self.assertNotIn('leak', out)

    def test_malformed_input_is_refused(self):
        p = Prog()
        ok = p.record([(1, 0x0001, 0, 0)])
        self.assertEqual(self.run_c(p, [(ok, 1, 0)])[0], 0)
        self.refused(p, [(ok + 1, 1, 0)], 'outside the program')                                    # odd address
        self.refused(p, [(len(p.data) - 4, 1, 0)], 'outside the program')
        self.refused(p, [(ok, 2, 0)], '0 cells')                                                    # the second record is zeros: no cells
        bad = bytearray(p.data)
        struct.pack_into('>H', bad, ok, 5)
        q = Prog()
        q.data[:] = bad
        self.refused(q, [(ok, 1, 0)], 'not an art record')
        q = Prog()
        q.data[:] = p.data
        struct.pack_into('>H', q.data, ok + 2, 9)                                                   # declared cells that the blocks do not add up to
        self.refused(q, [(ok, 1, 0)], 'blocks add up to 1 of its 9')
        q = Prog()
        q.data[:] = p.data
        struct.pack_into('>H', q.data, ok + 2, 1)
        struct.pack_into('>H', q.data, ok + 10, 0x0101)                                             # blocks that add up to more than the cells
        self.refused(q, [(ok, 1, 0)], 'more than its 1 cells')
        q = Prog()
        q.data[:] = p.data
        struct.pack_into('>H', q.data, ok + 2, 600)
        self.refused(q, [(ok, 1, 0)], '600 cells')
        q = Prog()
        q.data[:] = p.data
        struct.pack_into('>I', q.data, F.COORD_TABLE, 0xfffffff0)                                   # coordinates outside the program
        self.refused(q, [(ok, 1, 0)], 'coordinates')
        q = Prog()
        q.data[:] = p.data
        struct.pack_into('>h', q.data, ok + 6, 0x7000)                                              # a coordinate index outside the program
        self.refused(q, [(ok, 1, 0)], 'coordinate index')
        q = Prog()
        q.data[:] = p.data
        struct.pack_into('>H', q.data, ok + 4, 0xffff)                                              # 65536 blocks, the second one empty
        self.refused(q, [(ok, 1, 0)], 'more than its 1 cells')
        small = bytes(p.data[:ok + 4])                                                              # a program that ends inside the record
        d = Path(self.dir.name)
        (d / 'small.bin').write_bytes(small)
        (d / 'small.txt').write_text(f'0 100 100 1\n{ok} 1 0\n')
        (d / 'o').mkdir(exist_ok=True)
        r = subprocess.run([str(self.exe), 'build', str(d / 'small.bin'), str(d / 'small.txt'), str(d / 'o')], capture_output=True, text=True, timeout=30)
        self.assertEqual(r.returncode, 1)
        self.assertIn('outside the program', r.stdout)

    def test_no_room_is_an_error_not_an_overflow(self):
        p = Prog()
        a = p.record([(c, 0x0001, 16 * c, 0) for c in range(1, 40)])
        segs = [(a, 1, 0)]
        for pos_room, lists_room in ((0, 1 << 16), (1 << 16, 0), (5, 1 << 16), (1 << 16, 9), (1 << 16, 11)):
            status, out, *_ = self.run_c(p, segs, 0, pos_room, lists_room)
            self.assertEqual(status, 1, out)
            self.assertIn('room', out)
        self.assertEqual(self.run_c(p, [(0, 1, 0)], TERMINATOR, 0, 12 * 2)[0], 0)                   # two empty lists fit 24 bytes exactly
        self.assertEqual(self.run_c(p, [(0, 1, 0)], TERMINATOR, 0, 12 * 2 - 1)[0], 1)

    def test_values_that_do_not_fit_16_bits_are_refused(self):
        p = Prog()
        a = p.record([(1, 0x0001, -30000, 0), (2, 0x0001, 30000, 0)])        # a step of 60000 pixels
        self.refused(p, [(a, 1, 0)], 'does not fit 16 bits')

    # ------------------------------------------------------------ random programs

    def random_program(self, rnd):
        p = Prog()
        addrs = []
        for _ in range(rnd.randrange(3, 25)):
            kind = rnd.random()
            nb = rnd.randrange(1, 7)
            x, y = rnd.choice((-400, -300, -120, 0, 16, 48)), rnd.randrange(-4, 6) * 16
            if kind < 0.2:
                blocks = []
                for _b in range(nb):
                    blocks.append((rnd.randrange(60), x, y))
                    x += 16 * rnd.randrange(-2, 4)
                    y += 16 * rnd.randrange(0, 2)
                addrs.append(p.record(blocks, shared=rnd.randrange(0x80) | rnd.choice((0, 0x100))))
                continue
            blocks = []
            for _b in range(nb):
                w, h = rnd.randrange(3), rnd.randrange(3)
                attr = rnd.randrange(4) | (w << 8) | (h << 12) | rnd.choice((0, 0x20, 0x40, 0x60))
                blocks.append((rnd.randrange(60), attr, x, y))
                x += 16 * rnd.randrange(-3, 5)
                y += 16 * rnd.randrange(0, 3)
            addrs.append(p.record(blocks))
        return p, addrs

    def test_random_programs_equal_the_model(self):
        rnd = random.Random(11)
        done, kinds = 0, set()
        for case in range(60):
            p, addrs = self.random_program(rnd)
            segs = []
            for _ in range(rnd.randrange(1, 8)):
                if rnd.random() < 0.15:
                    segs.append((0, rnd.randrange(1, 3), 0))
                else:
                    i = rnd.randrange(len(addrs))
                    segs.append((addrs[i], min(rnd.randrange(1, 4), len(addrs) - i), rnd.choice((0, 0, 0x10000, 0x20000, 0x30000))))
            rules = rnd.randrange(8)
            try:
                self.model(p, segs, rules)
            except (ValueError, struct.error):
                continue
            with self.subTest(case=case, rules=rules):
                lists, pos, *_ = self.same(p, segs, rules)
                done += 1
                kinds |= {l['mode'] for l in parse_lists(lists)}
        self.assertGreaterEqual(done, 50)
        self.assertEqual(kinds, {0, 2, 4, 6})

    # ------------------------------------------------------------ the table, the build, the helpers

    def test_the_generated_table_is_sane(self):
        r = subprocess.run([str(self.exe), 'table'], capture_output=True, text=True, timeout=30)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn('table: ', r.stdout)

    def test_compiles_warning_free_for_32_bits(self):
        d = Path(self.dir.name)
        probe = d / 'probe.c'
        probe.write_text('int main(void) { return 0; }\n')
        if subprocess.run(['gcc', '-m32', '-o', str(d / 'probe'), str(probe)], capture_output=True).returncode:
            self.skipTest('no 32-bit toolchain')
        for src in SOURCES:
            r = subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / 'x32.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stderr)

    def test_segments_of_and_the_table_text(self):
        def lst(empty):
            return dict(n=1, mode=0, cells=[(0, None)]) if empty else dict(n=2, mode=4, cells=[(1, None), (2, None)])
        rec = lambda end: (None, None, end)
        lists = [lst(False), lst(False), lst(True), lst(False), lst(False), lst(False), lst(True), lst(True)]
        recs = {0: (rec(0x1010), 0, 0x1000), 1: (rec(0x1020), 0, 0x1010), 3: (rec(0x1030), 0, 0x1020),
                4: (rec(0x1040), 0x20000, 0x1030), 5: (rec(0x1050), 0x20000, 0x1040)}
        self.assertEqual(F.segments_of(lists, recs), [(0x1000, 2, 0), (0, 1, 0), (0x1020, 1, 0), (0x1030, 2, 0x20000), (0, 2, 0)])
        del recs[4]
        self.assertIsNone(F.segments_of(lists, recs))                                    # a list with no record: the C cannot build it
        row = dict(file='PL99.PAC', module_slot=4, positions_slot=None, positions=0x10, lists=0x200, positions_room=100, lists_room=400,
                   rules=['NUMBER_BY_RUN', 'TERMINATOR'], segments=[(0x1000, 2, 0), (0, 1, 0)])
        other = dict(row, file='PL99X.PAC', module_slot=5, positions_slot=0x2b)
        text = F.c_table([row, other], [('PL04.PAC', 'a list with no record'), ('PL10.PAC', 'PS1-only art')])
        self.assertEqual(text.count('segments_pl99[] = {'), 1)                           # identical segments are written once
        self.assertIn('{"PL99.PAC", 4u, ARCADE_FIGHTER_SAME_CHUNK, 0x10u, 0x200u, 100u, 400u, ARCADE_FIGHTER_NUMBER_BY_RUN | ARCADE_FIGHTER_TERMINATOR, segments_pl99, 2u, NULL, 0u},', text)
        self.assertIn('{"PL99X.PAC", 5u, 0x2bu,', text)
        self.assertIn('PL04.PAC: a list with no record', text)
        self.assertIn('PL10, PL11 and PL13', text)
        self.assertNotIn(chr(0x2014), text)
        d, why = F.descriptor('PL10.PAC', 1, {'error': 'x'})
        self.assertIsNone(d)
        self.assertIn('PS1-only art', why)
        d, why = F.descriptor('PL04.PAC', 1, dict(unmatched=[3], posbase=0, fit={}))
        self.assertIsNone(d)
        self.assertIn('no arcade record', why)


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
