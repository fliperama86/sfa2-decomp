#!/usr/bin/env python3
"""Seconds: the full animation sets (port/arcade/full_sets.py, full_sets.c, full_sets.h); no game files.

Python side, on made-up data: the relation of two sequences, the mapping of table entries (same, subsequence, exceptions, a run that breaks,
ties, refusals), the runs, the diff of the data after the frames (list offsets, module addresses, padding, refusals), the generated table's text,
the row's text form and the mask of written bytes, the resolution check (--compare-c's) on a good and a damaged result, and the sprite table check.

C side: full_sets.c, built with the address and undefined-behavior sanitizers and a driver (tests/full_sets_test.c), on made-up chunks:
arcade_full_build (a happy case against an independent model: positions copied, steps' list offsets moved, table entries per the runs, the
steps outside the block, the mask of written bytes exactly the written bytes; every refusal) and arcade_full_tiles (a happy case, every
refusal), and the generated table's shape compiled with full_sets.c.
"""
from pathlib import Path
import struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import full_sets as X

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
DRIVER = ARC / 'tests/full_sets_test.c'

LOAD, SLOAD = 0x80100000, 0x80180000
LISTS_AT = 0x80101000
SIZE, SSIZE = 0x300, 0x300


def p32(v):
    return struct.pack('<I', v & 0xffffffff)


def cell_list(k):
    """A valid cell list: one cell, mode 0, `third` = k."""
    return struct.pack('<5H', 1, 0, k, 0, 0) + struct.pack('<H', 0x100 + k)


def make_source():
    """(source chunk, row fields about it): positions at 0x40 (20 bytes), 4 lists at 0x60 (48 bytes), 4 steps at 0x90, a table of 4 at 0xc0,
    a step outside the block at 0x200."""
    s = bytearray(b'\x77' * SSIZE)
    s[0x40:0x54] = bytes(0xa0 + i for i in range(20))
    for k in range(4):
        s[0x60 + 12 * k:0x6c + 12 * k] = cell_list(k)
    for k in range(4):
        fl = 0x8000 if k in (1, 3) else 0
        s[0x90 + 12 * k:0x9c + 12 * k] = struct.pack('<HHII', k + 1, fl, 12 * k | (k & 1), (k << 16) | 0xfffe)
    for e, k in enumerate([0, 2, 1, 3]):
        s[0xc0 + 4 * e:0xc4 + 4 * e] = p32(SLOAD + 0x90 + 12 * k)
    s[0x200:0x20c] = struct.pack('<HHII', 9, 0x8000, 24 | 1, 0x50000)
    return bytes(s)


def make_target():
    t = bytearray(b'\x55' * SIZE)
    t[0x1f0:0x1fc] = struct.pack('<HHII', 9, 0x8000, 0x1234, 0x50000)
    return bytes(t)


def row():
    return dict(file='PL99.PAC', source='PL98.PAC', slot=4, source_slot=4, tiles_slot=1, source_tiles_slot=1, kind=0x10, load=LOAD, source_load=SLOAD,
                source_positions=0x40, positions_size=20, source_lists=0x60, lists_size=48, source_steps=0x90, step_count=4, source_table_entries=4,
                positions=0x20, lists=0x28, steps=0x34, table=0x80, table_entries=6, runs=[(0, 2, 2), (2, 2, 0), (4, 2, 1)], outside=[(0x1f0, 0x200)])


def model(r, module, source, lists_address):
    """(expected chunk, expected mask) of the row, written independently of full_sets.c."""
    out, mask = bytearray(module), bytearray(len(module))
    delta = (lists_address - (r['load'] + r['lists'])) & 0xffffffff

    def put(at, b):
        out[at:at + len(b)] = b
        mask[at:at + len(b)] = b'\1' * len(b)
    put(r['positions'], source[r['source_positions']:r['source_positions'] + r['positions_size']])
    for k in range(r['step_count']):
        a, b, ptr, fr = struct.unpack_from('<HHII', source, r['source_steps'] + 12 * k)
        put(r['steps'] + 12 * k, struct.pack('<HHII', a, b, (ptr + delta) & 0xffffffff, fr))
    table = r['source_steps'] + 12 * r['step_count']
    for first, count, src in r['runs']:
        for k in range(count):
            step = (struct.unpack_from('<I', source, table + 4 * (src + k))[0] - r['source_load'] - r['source_steps']) // 12
            put(r['table'] + 4 * (first + k), p32(r['load'] + r['steps'] + 12 * step))
    for t, s in r['outside']:
        put(t + 4, p32(struct.unpack_from('<I', source, s + 4)[0] + delta))
    return bytes(out), bytes(mask)


class CBase(unittest.TestCase):
    """The driver, linked with a table generated for the made-up row."""

    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory()
        d = Path(cls.dir.name)
        (d / 'table.c').write_text(X.c_table([row()]).replace('#include "full_sets.h"', f'#include "{ARC}/full_sets.h"'))
        cls.exe = d / 'full_sets_test'
        subprocess.run(['gcc', *FLAGS, *SAN, str(ARC / 'full_sets.c'), str(d / 'table.c'), str(DRIVER), '-o', str(cls.exe)], check=True, timeout=180)

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_exe(self, *args):
        return subprocess.run([str(self.exe), *map(str, args)], capture_output=True, text=True, timeout=60)

    def build(self, r, module, source, lists_address=LISTS_AT):
        d = Path(self.dir.name)
        for n, b in (('module', module), ('source', source)):
            (d / n).write_bytes(b)
        (d / 'spec').write_text(X.spec_text(r))
        run = self.run_exe('spec', d / 'spec', d / 'module', d / 'source', lists_address, d / 'out', d / 'changed')
        if run.returncode:
            return run, None, None
        return run, (d / 'out').read_bytes(), (d / 'changed').read_bytes()


class Build(CBase):
    def test_happy_case_against_the_model(self):
        r, module, source = row(), make_target(), make_source()
        run, out, changed = self.build(r, module, source)
        self.assertEqual(run.returncode, 0, run.stdout)
        want, mask = model(r, module, source, LISTS_AT)
        self.assertEqual(out, want)
        self.assertEqual(changed, mask)
        self.assertEqual(changed, bytes(X.expected_changed(r, SIZE)))
        self.assertEqual(sum(changed), 20 + 48 + 24 + 4)
        # unwritten bytes are the module's
        self.assertTrue(all(o == m for o, m, c in zip(out, module, changed) if not c))
        # the steps' list offsets moved by the delta, the odd bit kept
        delta = LISTS_AT - (LOAD + 0x28)
        self.assertEqual(struct.unpack_from('<I', out, 0x34 + 12 * 1 + 4)[0], 12 + 1 + delta)

    def test_the_row_in_the_linked_table_is_the_spec(self):
        run = self.run_exe('table')
        self.assertEqual(run.returncode, 0)
        self.assertEqual(X.parse_spec(run.stdout), row())
        self.assertEqual(X.parse_spec(X.spec_text(row())), row())

    def test_row_mode_builds_the_same_as_spec_mode(self):
        d = Path(self.dir.name)
        r, module, source = row(), make_target(), make_source()
        (d / 'module').write_bytes(module)
        (d / 'source').write_bytes(source)
        run = self.run_exe('row', 0, d / 'module', d / 'source', LISTS_AT, d / 'out', d / 'changed')
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual((d / 'out').read_bytes(), model(r, module, source, LISTS_AT)[0])

    def test_no_outside_steps_and_no_runs(self):
        r = dict(row(), runs=[], outside=[])
        run, out, changed = self.build(r, make_target(), make_source())
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual(sum(changed), 20 + 48)

    def refused(self, why, **fields):
        run, out, _ = self.build(dict(row(), **fields), make_target(), make_source())
        self.assertEqual(run.returncode, 3, f'{why}: {run.stdout} {run.stderr}')
        self.assertTrue(run.stdout.startswith('refused: '), run.stdout)

    def test_refusals(self):
        self.refused('a source block outside the chunk', source_lists=SSIZE - 8)
        self.refused('the source steps outside the chunk', source_steps=SSIZE - 12)
        self.refused('the source table outside the chunk', source_table_entries=60000)
        self.refused('the positions outside the module chunk', positions=SIZE - 10)
        self.refused('the steps outside the module chunk', steps=SIZE - 20)
        self.refused('the table outside the module chunk', table=SIZE - 8)
        self.refused('the positions overlap the steps', positions_size=0x40 - 0)       # 0x20 + 0x40 > 0x34
        self.refused('the steps overlap the table', table=0x34 + 12 * 4 - 4)
        self.refused('a run outside the target table', runs=[(4, 3, 0)])
        self.refused('a run outside the source table', runs=[(0, 2, 3)])
        self.refused('an outside step outside the module', outside=[(SIZE - 6, 0x200)])
        self.refused('an outside step outside the source', outside=[(0x1f0, SSIZE - 6)])

    def test_a_source_entry_that_is_not_a_step(self):
        s = bytearray(make_source())
        for bad in (SLOAD + 0x90 + 4, SLOAD + 0x90 + 12 * 4, SLOAD + 0x90 - 12, 0):
            s[0xc0:0xc4] = p32(bad)
            run, _, _ = self.build(row(), make_target(), bytes(s))
            self.assertEqual(run.returncode, 3, hex(bad))
            self.assertIn('not a step', run.stdout)

    def test_an_odd_delta(self):
        run, _, _ = self.build(row(), make_target(), make_source(), LISTS_AT + 1)
        self.assertEqual(run.returncode, 3)
        self.assertIn('odd', run.stdout)

    def test_a_source_entry_unused_by_the_runs_is_not_read(self):
        s = bytearray(make_source())
        s[0xc4:0xc8] = p32(0)          # entry 1 is in no run of this row
        run, out, _ = self.build(dict(row(), runs=[(0, 1, 0), (1, 2, 2)]), make_target(), bytes(s))
        self.assertEqual(run.returncode, 0, run.stdout)


class Tiles(CBase):
    def tiles(self, chunk, address=0x80206000, base=0x80010000, room=1000):
        d = Path(self.dir.name)
        (d / 'tiles').write_bytes(chunk)
        run = self.run_exe('tiles', d / 'tiles', address, base, room, d / 'tiles_out')
        return run, (d / 'tiles_out').read_bytes() if run.returncode == 0 else None

    @staticmethod
    def chunk(entries, size):
        b = bytearray(b'\xcc' * size)
        for i, e in enumerate(entries):
            b[4 * i:4 * i + 4] = p32(e)
        return bytes(b)

    def test_happy_case(self):
        entries = [16, 20, 26, 40]
        run, table = self.tiles(self.chunk(entries, 64))
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual(run.stdout.strip(), '4')
        self.assertEqual(table, b''.join(p32(0x80206000 + e - 0x80010000) for e in entries))

    def test_base_above_the_address_wraps(self):
        run, table = self.tiles(self.chunk([8, 12], 16), address=0x80010000, base=0x80206000)
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual(table, b''.join(p32(0x80010000 + e - 0x80206000) for e in (8, 12)))

    def test_room_exactly_enough(self):
        run, table = self.tiles(self.chunk([8, 12], 16), room=8)
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual(len(table), 8)

    def test_refusals(self):
        for why, chunk, room in (('shorter than 4 bytes', b'\1\0', 1000), ('entry 0 zero', self.chunk([0, 8], 16), 1000),
                                 ('entry 0 not a multiple of 4', self.chunk([10, 12, 14], 16), 1000),
                                 ('entry 0 past the chunk', self.chunk([20, 8], 16), 1000),
                                 ('an entry outside the chunk', self.chunk([8, 16], 16), 1000),
                                 ('an entry far outside the chunk', self.chunk([8, 0xfffffff0], 16), 1000),
                                 ('an entry inside the table', self.chunk([8, 4], 16), 1000),
                                 ('too little room', self.chunk([16, 20, 24, 28], 32), 15)):
            run, _ = self.tiles(chunk, room=room)
            self.assertEqual(run.returncode, 3, f'{why}: {run.stdout} {run.stderr}')
            self.assertTrue(run.stdout.startswith('refused: '), why)

    def test_refusal_writes_nothing(self):
        d = Path(self.dir.name)
        (d / 'tiles_out').unlink(missing_ok=True)
        run, _ = self.tiles(self.chunk([8, 16], 16))
        self.assertEqual(run.returncode, 3)
        self.assertFalse((d / 'tiles_out').exists())


class GeneratedTable(unittest.TestCase):
    def test_table_text_compiles_with_the_c(self):
        rows = [row(), dict(row(), file='PL99X.PAC', slot=5, tiles_slot=2, load=0x801c8000, outside=[])]
        text = X.c_table(rows)
        self.assertIn('{0u, 2u, 2u},', text)
        self.assertIn('{0x1f0u, 0x200u},', text)
        self.assertIn('NULL, 0u},', text)
        self.assertIn('arcade_full_set_count', text)
        self.assertIn('GENERATED by', text)
        self.assertNotIn(chr(0x2014), text)
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / 't.c').write_text(text.replace('#include "full_sets.h"', f'#include "{ARC}/full_sets.h"'))
            exe = Path(d) / 'x'
            subprocess.run(['gcc', *FLAGS, str(ARC / 'full_sets.c'), str(Path(d) / 't.c'), str(DRIVER), '-o', str(exe)], check=True, timeout=120)
            out = subprocess.run([str(exe), 'table'], capture_output=True, text=True, check=True).stdout
        parts = out.split('\n')
        first = X.parse_spec('\n'.join(parts[:1 + 3 + 1]) + '\n')
        self.assertEqual(first, rows[0])
        self.assertEqual(X.parse_spec('\n'.join(parts[5:5 + 1 + 3]) + '\n'), rows[1])

    def test_checked_in_table_compiles_and_has_the_two_rows(self):
        with tempfile.TemporaryDirectory() as d:
            exe = Path(d) / 'x'
            subprocess.run(['gcc', *FLAGS, '-m32', '-c', str(ARC / 'full_sets.c'), '-o', str(Path(d) / 'a.o')], check=True, timeout=120)
            subprocess.run(['gcc', *FLAGS, str(ARC / 'full_sets.c'), str(ARC / 'full_sets_table.c'), str(DRIVER), '-o', str(exe)], check=True, timeout=120)
            out = subprocess.run([str(exe), 'table'], capture_output=True, text=True, check=True).stdout
        names = [l.split()[0] for l in out.split('\n') if l.startswith('PL')]
        self.assertEqual(names, [t for t, _s in X.TARGETS])


class Pure(unittest.TestCase):
    @staticmethod
    def seq(*steps):
        return [(d, f, 0, fr << 16) for d, f, fr in steps]

    def test_relation(self):
        a = self.seq((3, 0, 1), (4, 0x8000, 2))
        self.assertEqual(X.relation(a, self.seq((3, 0, 1), (4, 0x8000, 2))), 'same')
        self.assertEqual(X.relation(a, self.seq((3, 0, 1), (5, 0, 7), (4, 0x8000, 2))), 'sub')
        self.assertIsNone(X.relation(a, self.seq((3, 0, 2), (4, 0x8000, 2))))
        self.assertIsNone(X.relation(self.seq((3, 0, 1), (3, 0, 1)), self.seq((3, 0, 1), (4, 0x8000, 2))))
        # the list offset word does not take part
        self.assertEqual(X.relation([(3, 0x8000, 5, 0)], [(3, 0x8000, 99, 0)]), 'same')

    def test_walk_stops_at_bit_15(self):
        mod = b''.join(struct.pack('<HHII', 1, f, 0, 0) for f in (0, 0, 0x8000, 0, 0))
        self.assertEqual(len(X.walk(mod, 0)), 3)
        self.assertEqual(len(X.walk(mod, 12)), 2)
        self.assertEqual(len(X.walk(b''.join(struct.pack('<HHII', 1, 0, 0, 0) for _ in range(300)), 0)), X.MAX_STEPS)
        with self.assertRaises(X.Fail):
            X.walk(mod, 5 * 12 - 4)

    def sequences(self, n):
        return [self.seq((i + 1, 0, i), (2, 0x8000, 0)) for i in range(n)]

    def test_map_identity_and_a_shifted_tail(self):
        s = self.sequences(12)
        t = s[:5] + s[7:9]
        js, rels = X.map_entries(t, s, exceptions=())
        self.assertEqual(js, [0, 1, 2, 3, 4, 7, 8])
        self.assertEqual(rels, ['same'] * 7)
        self.assertEqual(X.to_runs(js), [(0, 5, 0), (5, 2, 7)])

    def test_map_subsequence_and_exception(self):
        s = self.sequences(6)
        s[2] = self.seq((3, 0, 2), (1, 0, 9), (2, 0x8000, 0))            # the source's has a step more
        t = list(s)
        t[2] = self.seq((3, 0, 2), (2, 0x8000, 0))
        t[4] = self.seq((7, 0, 40), (2, 0x8000, 0))                       # a variant: neither
        js, rels = X.map_entries(t, s, exceptions=(4,))
        self.assertEqual(js, list(range(6)))
        self.assertEqual(rels, ['same', 'same', 'sub', 'same', 'variant', 'same'])

    def test_map_refusals(self):
        s = self.sequences(6)
        t = list(s)
        t[4] = self.seq((7, 0, 40), (2, 0x8000, 0))
        with self.assertRaises(X.Fail):
            X.map_entries(t, s, exceptions=())                  # no mapping
        with self.assertRaises(X.Fail):
            X.map_entries(list(s), s, exceptions=(4,))          # the exception maps by content
        t2 = list(s)
        t2[4] = self.seq((7, 0, 40), (2, 0, 0))                  # a variant that does not end in a jump
        with self.assertRaises(X.Fail):
            X.map_entries(t2, s, exceptions=(4,))
        with self.assertRaises(X.Fail):
            X.map_entries([s[3], s[3]], s, exceptions=())        # two entries, one source entry (distinct sequences cannot share)

    def test_map_soft_mode_reports_unmatched(self):
        s = self.sequences(6)
        t = list(s)
        t[4] = self.seq((7, 0, 40), (2, 0x8000, 0))
        js, rels = X.map_entries(t, s, exceptions=(), unmatched_cost=50)
        self.assertEqual(rels[4], None)
        self.assertEqual(js, list(range(6)))

    def test_ties_prefer_the_same_index(self):
        one = self.seq((5, 0x8000, 1))
        s = [self.sequences(1)[0], one, one, one, one]
        js, _ = X.map_entries([s[0], one, one], s, exceptions=())
        self.assertEqual(js, [0, 1, 2])

    def test_runs(self):
        self.assertEqual(X.to_runs([0, 1, 2, 5, 6, 3]), [(0, 3, 0), (3, 2, 5), (5, 1, 3)])
        self.assertEqual(X.to_runs([]), [])

    def tails(self, tweak_t=lambda w: w, tweak_s=lambda w: w):
        """Two modules' tails (made up), from word offsets 0: a module address, zero padding in the source, steps outside the block."""
        tl, sl = 0x80000000, 0x80100000
        t = [1, 2, tl + 0x20, 4, 0x20, 5, 6, 7, 0x40, 8]            # target: 0x20 and 0x40 are list offsets (index 4, 8)
        s = [0, 0, 1, 2, sl + 0x20, 4, 0x80, 5, 6, 7, 0x90, 8]
        return tweak_t(t), tweak_s(s), tl, sl

    @staticmethod
    def pack(words, size=64):
        b = b''.join(p32(w) for w in words)
        return b + b'\0' * (size - len(b))

    def test_outside_steps_diff(self):
        t, s, tl, sl = self.tails()
        tm, sm = self.pack(t, 0x28), self.pack(s, 0x30)
        out, extra = X.outside_steps(tm, tl, 0, 0x100, sm, sl, 0, 0x100)
        self.assertEqual(out, [(12, 20), (28, 36)])
        self.assertEqual(extra, 2)

    def test_outside_steps_refusals(self):
        t, s, tl, sl = self.tails()
        tm, sm = self.pack(t, 0x28), self.pack(s, 0x30)
        with self.assertRaises(X.Fail):
            X.outside_steps(tm, tl, 0, 0x30, sm, sl, 0, 0x100)               # a differing word beyond the lists
        t2 = list(t)
        t2[5] = 77
        with self.assertRaises(X.Fail):
            X.outside_steps(self.pack(t2, 0x28), tl, 0, 0x40, sm, sl, 0, 0x100)    # another differing word, not a list offset
        t3 = list(t)
        t3[2] = 99                                                             # a module address against a value
        with self.assertRaises(X.Fail):
            X.outside_steps(self.pack(t3, 0x28), tl, 0, 0x100, sm, sl, 0, 0x100)
        s2 = list(s)
        s2[0] = 9                                                              # padding that is not zero
        with self.assertRaises(X.Fail):
            X.outside_steps(tm, tl, 0, 0x100, self.pack(s2, 0x30), sl, 0, 0x100)

    def test_pointers_into(self):
        mod = self.pack([5, 0x80000100, 0x80000010, 0x80000200], 64)
        self.assertEqual(X.pointers_into(mod, 0x80000000, 0, 0x100, 0x200), [4])

    def test_spec_and_expected_mask(self):
        r = row()
        self.assertEqual(X.parse_spec(X.spec_text(r)), r)
        m = X.expected_changed(r, SIZE)
        self.assertEqual(sum(m), 20 + 48 + 24 + 4)
        self.assertEqual(m[0x1f4:0x1f8], b'\1\1\1\1')
        self.assertEqual(m[0x1f0], 0)

    def test_compare_row_on_a_good_and_a_damaged_result(self):
        r, module, source = row(), make_target(), make_source()
        out, mask = model(r, module, source, LISTS_AT)
        counts, problems = X.compare_row(r, module, source, out, mask, LISTS_AT)
        self.assertEqual(problems, [])
        self.assertEqual(counts['entries'], 6)
        self.assertEqual(counts['outside'], 1)
        # an entry pointing at the wrong step
        bad = bytearray(out)
        bad[0x80:0x84] = p32(LOAD + 0x34 + 12 * 2)
        self.assertTrue(X.compare_row(r, module, source, bytes(bad), mask, LISTS_AT)[1])
        # a list offset not moved
        bad = bytearray(out)
        bad[0x34 + 12 + 4:0x34 + 12 + 8] = p32(struct.unpack_from('<I', source, 0x90 + 12 + 4)[0])
        self.assertTrue(X.compare_row(r, module, source, bytes(bad), mask, LISTS_AT)[1])
        # a byte written outside the mask
        bad = bytearray(out)
        bad[0x2ff] ^= 1
        self.assertTrue(X.compare_row(r, module, source, bytes(bad), mask, LISTS_AT)[1])
        # a mask that claims more
        wide = bytearray(mask)
        wide[0x100] = 1
        self.assertTrue(X.compare_row(r, module, source, out, bytes(wide), LISTS_AT)[1])

    def test_compare_tiles(self):
        chunk = Tiles.chunk([16, 20, 26, 40], 64)
        table = b''.join(p32(0x80206000 + e - 0x80010000) for e in (16, 20, 26, 40))
        self.assertEqual(X.compare_tiles(chunk, 0x80206000, 0x80010000, table, 4), [])
        bad = table[:4] + p32(struct.unpack_from('<I', table, 4)[0] + 2) + table[8:]
        self.assertTrue(X.compare_tiles(chunk, 0x80206000, 0x80010000, bad, 4))
        self.assertTrue(X.compare_tiles(chunk, 0x80206000, 0x80010000, table[:12], 3))

    def test_no_dash(self):
        for p in (ARC / 'full_sets.c', ARC / 'full_sets.h', ARC / 'full_sets.py', ARC / 'tests/full_sets_test.c', Path(__file__)):
            self.assertNotIn(chr(0x2014), p.read_text(), str(p))


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
