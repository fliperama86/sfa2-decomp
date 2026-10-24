#!/usr/bin/env python3
"""Seconds: the object grids of the stage modules (port/arcade/stage_objects.py, stage_objects.c, stage_objects.h) on a made-up arcade; no game files.

Python side: the tile list of a stage (object tile runs then the shared chunk's), the grid a record's cells make (columns, rows, palette row, origin, the numbers
of the tiles in the grid's range, a transparent tile showing nothing, a record that cannot be a grid), the finding of grids in a module, the generated table's text.
C side: stage_objects.c, built with the address and undefined-behavior sanitizers and a driver (tests/stage_objects_test.c): its bytes equal the model's for
random records of mode 2 (blocks with flips) and mode 0, the known bytes are exactly those written, a grid that cannot be built is refused with a message and the
others are still built, a grid outside the module is refused, and the checked-in table is sane and compiles.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import fighters as F
import stages as S
import stage_objects as O

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
COORD = 0x9f260
TILES = 0x10000 + 64


def make_world(seed):
    """(program, graphics, blank set): graphics tiles BANK + 0..63, a third of them transparent; the program has room for records."""
    rnd = random.Random(seed)
    gfx = bytearray(rnd.randbytes(TILES * 128))
    blank = set()
    for code in range(64):
        if code % 3 == 0:
            gfx[(0x10000 + code) * 128:(0x10000 + code + 1) * 128] = b'\xff' * 128
            blank.add(0x10000 + code)
    return bytearray(0x300000), bytes(gfx), blank


def put_u16(prog, at, v):
    struct.pack_into('>H', prog, at, v & 0xffff)


def put_record(prog, at, blocks, slot, coords, mode=2, attr=0):
    """An art record at `at`: mode 2 from blocks [(code, attr)] or mode 0 (codes, one shared attr). Coordinates (x, y) at 0x100000 + 64 * slot, the pointer in
    the coordinate table at COORD + 4 * slot. Returns the record's cells count."""
    ptr = 0x100000 + 64 * slot
    struct.pack_into('>I', prog, COORD + 4 * slot, ptr)
    for k, (x, y) in enumerate(coords):
        struct.pack_into('>hh', prog, ptr + 4 * k, x, y)
    if mode == 2:
        cells = sum((((a >> 8) & 15) + 1) * (((a >> 12) & 15) + 1) for _c, a in blocks)
        put_u16(prog, at, 2); put_u16(prog, at + 2, cells); put_u16(prog, at + 4, len(blocks) - 1); put_u16(prog, at + 6, 4 * slot)
        for k, (code, a) in enumerate(blocks):
            put_u16(prog, at + 8 + 4 * k, code); put_u16(prog, at + 10 + 4 * k, a)
        return cells
    put_u16(prog, at, 0); put_u16(prog, at + 2, len(blocks)); put_u16(prog, at + 4, attr); put_u16(prog, at + 6, 4 * slot)
    for k, (code, _a) in enumerate(blocks):
        put_u16(prog, at + 8 + 2 * k, code)
    return len(blocks)


def keys_for(cells, extra=()):
    """The tile runs (one tile each, numbered from 1) that name the (code + BANK, flip) of every cell of a record, then `extra` (code, flip)."""
    seen, runs = [], [(S.RUN_ZERO, 1, 0)]
    for code, x, y, fl, _g, _gp in cells:
        key = (code + 0x10000, (fl >> 5 & 1) | (fl >> 6 & 1) << 1)
        if key not in seen:
            seen.append(key)
            runs.append((key[0], 1, key[1]))
    for code, flip in extra:
        runs.append((code + 0x10000, 1, flip))
    return runs


class Python(unittest.TestCase):
    def test_tile_keys(self):
        keys = O.tile_keys([(S.RUN_ZERO, 1, 0), (0x10005, 2, 1), (S.RUN_DISC, 1, 0), (S.RUN_BLANK, 1, 0)], [(0x10009, 1, 3), (0x10020, 2, 2)])
        self.assertEqual(keys, [None, (0x10005, 1), (0x10006, 1), None, None, (0x10009, 3), (0x10020, 2), (0x10021, 2)])
        self.assertEqual(O.tile_keys([], []), [])

    def record_cells(self, prog, at):
        return F.read_any(F.View(bytes(prog)), at)[1]

    def test_a_record_makes_a_grid(self):
        prog, gfx, blank = make_world(1)
        # a 2 x 2 block with flip x at (-16, -32), palette 3, and a single cell below it at (0, 0) of palette 3
        put_record(prog, 0x1000, [(0x11, 0x1123), (0x20, 0x0003)], 0, [(-16, -32), (0, 0)])
        cells = self.record_cells(prog, 0x1000)
        self.assertEqual(len(cells), 5)
        runs = keys_for(cells)
        keys = O.tile_keys(runs, [])
        grid = O.build_grid(cells, blank, keys, 1, len(keys) - 1)
        # the cells that show something: tile codes 0x11..0x14 are 0x11, 0x12, 0x21, 0x22 (flip x swaps the columns), then 0x20; three codes are transparent
        vis = [c for c in O.grid_cells(cells, blank)]
        self.assertTrue(all(c[0] not in blank for c in vis))
        self.assertIsNotNone(grid)
        cols, zero, rows, pal, ox, oy = struct.unpack_from('<BBBBHH', grid)
        self.assertEqual((zero, pal), (0, 3))
        minx = min(c[1] for c in vis); miny = min(c[2] for c in vis)
        self.assertEqual((cols, rows, ox, oy), ((max(c[1] for c in vis) - minx) // 16 + 1, (max(c[2] for c in vis) - miny) // 16 + 1, -minx & 0xffff, -miny & 0xffff))
        self.assertEqual(len(grid), 8 + 2 * cols * rows)
        cellsout = struct.unpack_from('<%dH' % (cols * rows), grid, 8)
        self.assertEqual(sum(1 for c in cellsout if c), len(vis))
        for c in cellsout:
            self.assertTrue(c == 0 or keys[c] is not None)

    def test_a_record_that_cannot_be_a_grid(self):
        prog, gfx, blank = make_world(2)
        put_record(prog, 0x1000, [(0x11, 0x0001), (0x13, 0x0002)], 0, [(0, 0), (16, 0)])          # two palette rows
        cells = self.record_cells(prog, 0x1000)
        keys = O.tile_keys(keys_for(cells), [])
        self.assertIsNone(O.build_grid(cells, blank, keys, 1, 9))
        put_record(prog, 0x1100, [(0x11, 0x0001), (0x13, 0x0001)], 1, [(0, 0), (8, 0)])           # a cell off the 16 pixel grid
        cells = self.record_cells(prog, 0x1100)
        self.assertIsNone(O.build_grid(cells, blank, O.tile_keys(keys_for(cells), []), 1, 9))
        put_record(prog, 0x1200, [(0x11, 0x0001), (0x11, 0x0001)], 2, [(0, 0), (0, 0)])           # two cells at one place
        cells = self.record_cells(prog, 0x1200)
        self.assertIsNone(O.build_grid(cells, blank, O.tile_keys(keys_for(cells), []), 1, 9))
        put_record(prog, 0x1300, [(0x11, 0x0001), (0x13, 0x0001)], 3, [(0, 0), (16, 0)])
        cells = self.record_cells(prog, 0x1300)
        runs = keys_for(cells)
        keys = O.tile_keys(runs + [(0x10011, 1, 0)], [])                                                      # the same tile a second time inside the range
        self.assertIsNone(O.build_grid(cells, blank, keys, 1, len(keys) - 1))
        self.assertIsNotNone(O.build_grid(cells, blank, keys, 1, len(keys) - 2))
        self.assertIsNone(O.build_grid(cells, blank, keys, 1, 1))                                   # a range that does not hold a tile
        transparent = [(0x0, 0x0001)]                                                               # code 0: its tile BANK + 0 is transparent: no visible cell
        put_record(prog, 0x1400, transparent, 4, [(0, 0)])
        self.assertIsNone(O.build_grid(self.record_cells(prog, 0x1400), blank, [None], 0, 0))

    def test_finding_grids_in_a_module(self):
        mod = bytearray(256)
        grid = struct.pack('<BBBBHH', 2, 0, 1, 4, 16, 16) + struct.pack('<2H', 3, 0)
        mod[40:40 + len(grid)] = grid
        got = O.find_grids(bytes(mod), 10)
        self.assertEqual([(g[0], g[1], g[2], g[3]) for g in got], [(40, 2, 1, (3, 0))])
        self.assertEqual(O.find_grids(bytes(mod), 0), [])                                           # a cell names a tile the stage does not have

    def test_the_generated_table_text(self):
        text = O.c_table([('STAGE00.PAC', [(40, 0x2223b0, 3, 9), (80, 0x2223c8, 3, 9)]), ('STAGE01.PAC', [])])
        self.assertIn('static const struct arcade_stage_grid grids_00[] = {', text)
        self.assertIn('{40u, 0x2223b0u, 3u, 9u},', text)
        self.assertIn('{"STAGE00.PAC", grids_00, 2u},', text)
        self.assertIn('{"STAGE01.PAC", 0, 0u},', text)
        self.assertNotIn(chr(0x2014), text)


class C(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.dir = Path(cls.tmp.name)
        cls.exe = str(cls.dir / 'test')
        subprocess.run(['gcc', *FLAGS, *SAN, '-I', str(ARC), '-o', cls.exe, str(ARC / 'stage_objects.c'), str(ARC / 'stage_objects_table.c'),
                        str(ARC / 'tests/stage_objects_test.c')], check=True, capture_output=True, text=True, timeout=120)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def run_c(self, prog, gfx, spec, size):
        (self.dir / 'prog').write_bytes(bytes(prog))
        (self.dir / 'gfx').write_bytes(gfx)
        (self.dir / 'spec').write_text(spec)
        out = self.dir / 'out'
        out.mkdir(exist_ok=True)
        r = subprocess.run([self.exe, 'build', str(self.dir / 'prog'), str(self.dir / 'gfx'), str(self.dir / 'spec'), str(out)], capture_output=True, text=True, timeout=60)
        self.assertIn(r.returncode, (0, 1), r.stdout + r.stderr)
        return r, (out / 'module.bin').read_bytes(), (out / 'module.known').read_bytes()

    def spec_for(self, runs, shared, grids, size):
        word = lambda f: 'ZERO' if f == S.RUN_ZERO else 'DISC' if f == S.RUN_DISC else 'BLANK' if f == S.RUN_BLANK else str(f)
        return (f'size {size}\n' + ''.join(f'object {word(f)} {c} {fl}\n' for f, c, fl in runs) + ''.join(f'shared {word(f)} {c} {fl}\n' for f, c, fl in shared) +
                ''.join(f'grid {o} {a} {lo} {hi}\n' for o, a, lo, hi in grids))

    def test_random_records_build_as_the_model_says(self):
        for seed in range(12):
            rnd = random.Random(seed)
            prog, gfx, blank = make_world(seed)
            grids, runs_all, expect = [], [(S.RUN_ZERO, 1, 0)], bytearray(4096)
            known_expect = bytearray(4096)
            at = 0x1000
            # records, each with its own tile numbers (one object a record: the ranges are disjoint)
            for slot in range(5):
                mode = rnd.choice([0, 2])
                n = rnd.randrange(1, 4)
                if mode == 2:
                    blocks = [(rnd.randrange(0, 40), (rnd.randrange(3) << 8) | (rnd.randrange(2) << 12) | rnd.choice([0, 0x20, 0x40, 0x60]) | 3) for _ in range(n)]
                    coords = [(rnd.randrange(-3, 3) * 16, rnd.randrange(-3, 3) * 16) for _ in range(n)]
                else:
                    blocks = [(rnd.randrange(0, 40), 0) for _ in range(n)]
                    coords = [(rnd.randrange(-3, 3) * 16, rnd.randrange(-3, 3) * 16) for _ in range(n)]
                put_record(prog, at + 0x40 * slot, blocks, slot, coords, mode, attr=rnd.choice([3, 3, 0x23, 0x43]))
                rec = F.read_any(F.View(bytes(prog)), at + 0x40 * slot)
                lo = len(runs_all)
                runs_all += keys_for(rec[1])[1:]
                hi = len(runs_all) - 1
                keys = O.tile_keys(runs_all, [])
                made = O.build_grid(rec[1], blank, keys, lo, hi)
                off = 64 * slot
                grids.append((off, at + 0x40 * slot, lo, hi))
                if made is not None:
                    expect[off:off + len(made)] = made
                    known_expect[off:off + len(made)] = b'\1' * len(made)
            r, data, known = self.run_c(prog, gfx, self.spec_for(runs_all, [], grids, 4096), 4096)
            self.assertEqual(data, bytes(expect), seed)
            self.assertEqual(known, bytes(known_expect), seed)
            self.assertEqual(r.returncode, 0 if all(O.build_grid(F.read_any(F.View(bytes(prog)), a)[1], blank, O.tile_keys(runs_all, []), lo, hi) for _o, a, lo, hi in grids) else 1)

    def test_a_grid_that_cannot_be_built_is_refused_and_the_others_are_built(self):
        prog, gfx, blank = make_world(7)
        put_record(prog, 0x1000, [(0x11, 0x0003), (0x13, 0x0003)], 0, [(0, 0), (16, 0)])
        put_record(prog, 0x1100, [(0x14, 0x0003), (0x16, 0x0003)], 1, [(0, 0), (16, 0)])
        rec0, rec1 = F.read_any(F.View(bytes(prog)), 0x1000), F.read_any(F.View(bytes(prog)), 0x1100)
        runs = [(S.RUN_ZERO, 1, 0)] + keys_for(rec0[1])[1:] + keys_for(rec1[1])[1:]
        keys = O.tile_keys(runs, [])
        good = O.build_grid(rec1[1], blank, keys, 3, 4)
        self.assertIsNotNone(good)
        # the first grid names a range that does not hold its tiles; the second is fine
        r, data, known = self.run_c(prog, gfx, self.spec_for(runs, [], [(0, 0x1000, 3, 4), (64, 0x1100, 3, 4)], 256), 256)
        self.assertEqual(r.returncode, 1)
        self.assertIn('error: ', r.stdout)
        self.assertEqual(data[64:64 + len(good)], good)
        self.assertEqual(known[:64], bytes(64))
        self.assertEqual(known[64:64 + len(good)], b'\1' * len(good))
        # an art record that is not one, and a grid beyond the module
        r, data, known = self.run_c(prog, gfx, self.spec_for(runs, [], [(0, 0x2000, 1, 2)], 256), 256)
        self.assertEqual((r.returncode, sum(known)), (1, 0))
        r, data, known = self.run_c(prog, gfx, self.spec_for(runs, [], [(250, 0x1100, 3, 4)], 256), 256)
        self.assertEqual((r.returncode, sum(known)), (1, 0))

    def test_the_shared_tiles_follow_the_object_tiles_in_the_numbering(self):
        prog, gfx, blank = make_world(3)
        put_record(prog, 0x1000, [(0x11, 0x0003), (0x13, 0x0003)], 0, [(0, 0), (16, 0)])
        rec = F.read_any(F.View(bytes(prog)), 0x1000)
        objects = [(S.RUN_ZERO, 1, 0), (0x10011, 1, 0)]
        shared = [(0x10013, 1, 0)]
        keys = O.tile_keys(objects, shared)
        made = O.build_grid(rec[1], blank, keys, 1, 2)
        self.assertEqual(struct.unpack_from('<2H', made, 8), (1, 2))                                  # the second cell is tile 2: the shared chunk's first
        r, data, known = self.run_c(prog, gfx, self.spec_for(objects, shared, [(0, 0x1000, 1, 2)], 64), 64)
        self.assertEqual(r.returncode, 0)
        self.assertEqual(data[:len(made)], made)

    def test_the_checked_in_table(self):
        r = subprocess.run([self.exe, 'table'], capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout)
        self.assertIn('20 stage files', r.stdout)

    def test_native_and_32_bit_builds_have_no_warnings(self):
        for extra in ((), ('-m32',)):
            probe = self.dir / 'probe.c'
            probe.write_text('int main(void){return 0;}\n')
            if extra and subprocess.run(['gcc', *extra, '-o', str(self.dir / 'p'), str(probe)], capture_output=True).returncode:
                continue
            for src in (ARC / 'stage_objects.c', ARC / 'stage_objects_table.c'):
                r = subprocess.run(['gcc', *extra, *FLAGS, '-O2', '-I', str(ARC), '-c', str(src), '-o', str(self.dir / 'x.o')], capture_output=True, text=True)
                self.assertEqual((r.returncode, r.stderr), (0, ''), (extra, src))


if __name__ == '__main__':
    start = time.time() if False else None
    result = unittest.main(exit=False, verbosity=0)
    sys.exit(0 if result.result.wasSuccessful() else 1)
