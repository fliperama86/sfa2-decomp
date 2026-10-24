#!/usr/bin/env python3
"""Seconds: the animation steps of the stage modules (port/arcade/stage_steps.py, stage_steps.c, stage_steps.h) on a made-up arcade; no game files.

Python side: the arcade record (16 bytes, 20 with a jump), the step it makes (countdown, flags, grid address, loop offset by the target's step index, frame 0),
the refusals (a jump to no run, an art with no grid), the finding of step runs in a module, the pairing and the choice of the steps that equal the disc's
(a step that differs is left out and splits its run), the generated table's text.
C side: stage_steps.c, built with the address and undefined-behavior sanitizers and a driver (tests/stage_steps_test.c): its bytes equal the model's for random
chains with jumps, the known bytes are exactly those written, a bad run is refused with a message and the others are still built, and the checked-in table
is sane and compiles.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import stage_steps as T

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
BASE = T.BASE
ART0 = 0x200000


def put_chain(prog, at, specs):
    """Records at `at` from specs [(countdown, flags, art, jump target index or None)]: returns their addresses (a jump's target index is a record index)."""
    addrs, a = [], at
    for _d, fl, _art, _t in specs:
        addrs.append(a)
        a += 20 if fl & 0x8000 else 16
    a = at
    for (d, fl, art, t), addr in zip(specs, addrs):
        struct.pack_into('>HHI', prog, a, d, fl, art)
        if fl & 0x8000:
            struct.pack_into('>I', prog, a + 16, addrs[t])
        a = addr + (20 if fl & 0x8000 else 16)
    return addrs


def random_chain(rnd, n, arts):
    specs = []
    for k in range(n):
        jump = rnd.random() < 0.25
        specs.append((rnd.choice([1, 2, 8, 10, 4095]), (0x8000 if jump else 0) | rnd.choice([0, 1, 4, 0x100]), rnd.choice(arts), rnd.randrange(0, k + 1) if jump else None))
    return specs


class Python(unittest.TestCase):
    def test_a_record_is_16_bytes_and_20_with_a_jump(self):
        prog = bytearray(0x300000)
        addrs = put_chain(prog, ART0, [(8, 0, 0x100010, None), (9, 0x8000, 0x100020, 0), (10, 0x4, 0x100030, None)])
        self.assertEqual([a - ART0 for a in addrs], [0, 16, 36])
        recs = T.walk_records(bytes(prog), ART0)
        self.assertEqual([(r['dur'], r['fl'], r['art'], r['tgt']) for r in recs], [(8, 0, 0x100010, None), (9, 0x8000, 0x100020, ART0), (10, 4, 0x100030, None)])

    def test_a_record_must_be_plausible(self):
        prog = bytearray(0x300000)
        struct.pack_into('>HHI', prog, ART0, 0, 0, 0x100010)
        struct.pack_into('>HHI', prog, ART0 + 16, 5, 0, 0x100011)
        struct.pack_into('>HHI', prog, ART0 + 32, 5, 0, 0x500000)
        self.assertEqual([T.valid_record(bytes(prog), ART0 + 16 * k) for k in range(3)], [False, False, False])
        struct.pack_into('>HHI', prog, ART0, 4096, 0, 0x100010)
        self.assertFalse(T.valid_record(bytes(prog), ART0))
        self.assertFalse(T.valid_record(bytes(prog), len(prog) - 8))

    def test_the_step_of_a_record(self):
        prog = bytearray(0x300000)
        addrs = put_chain(prog, ART0, [(8, 0, 0x100010, None), (9, 4, 0x100020, None), (4095, 0x8000, 0x100030, 1), (7, 0x8000, 0x100010, 2)])
        built = T.build_steps(bytes(prog), [(100, addrs[0], 4)], {0x100010: 2000, 0x100020: 2100, 0x100030: 2200})
        self.assertEqual(sorted(built), [100, 112, 124, 136])
        self.assertEqual(struct.unpack('<hhIhH', built[100]), (8, 0, BASE + 2000, 0, 0))
        self.assertEqual(struct.unpack('<hhIhH', built[112]), (9, 4, BASE + 2100, 0, 0))
        self.assertEqual(struct.unpack('<hhIhH', built[124]), (4095, -32768, BASE + 2200, -1, 0))   # jump to the step before
        self.assertEqual(struct.unpack('<hhIhH', built[136]), (7, -32768, BASE + 2000, -1, 0))      # to the step before it

    def test_a_jump_to_itself_has_loop_zero(self):
        prog = bytearray(0x300000)
        addrs = put_chain(prog, ART0, [(8, 0x8000, 0x100010, 0)])
        built = T.build_steps(bytes(prog), [(0, addrs[0], 1)], {0x100010: 64})
        self.assertEqual(struct.unpack('<hhIhH', built[0])[3], 0)

    def test_refusals(self):
        prog = bytearray(0x300000)
        addrs = put_chain(prog, ART0, [(8, 0, 0x100010, None), (9, 0x8000, 0x100020, 0)])
        with self.assertRaises(ValueError):
            T.build_steps(bytes(prog), [(0, addrs[0], 2)], {0x100010: 64})                  # no grid for the second art
        with self.assertRaises(ValueError):
            T.build_steps(bytes(prog), [(0, addrs[1], 1)], {0x100020: 64})                  # the jump leads to a record no run builds
        with self.assertRaises(ValueError):
            T.build_steps(bytes(prog), [(0, ART0 + 0x1000, 1)], {0x100010: 64})             # no record there

    def world(self, specs, grid_of, fix=lambda mod: None):
        """(program, module, art map, grid addresses): a module whose steps are what build_steps makes from the chain, then fix(mod) changes it."""
        prog = bytearray(0x300000)
        addrs = put_chain(prog, ART0, specs)
        mod = bytearray(4096)
        built = T.build_steps(bytes(prog), [(256, addrs[0], len(specs))], grid_of)
        for o, b in built.items():
            mod[o:o + 12] = b
        fix(mod)
        return bytes(prog), bytes(mod), {art: BASE + g for art, g in grid_of.items()}, {BASE + g for g in grid_of.values()}

    def test_step_runs(self):
        specs = [(8, 0, 0x100010, None), (9, 4, 0x100020, None), (10, 0x8000, 0x100010, 0)]
        prog, mod, amap, addrs = self.world(specs, {0x100010: 64, 0x100020: 96})
        runs = T.step_runs(mod, addrs)
        self.assertEqual([[s['o'] for s in r] for r in runs], [[256, 268, 280]])
        self.assertEqual(runs[0][2]['lo'], -2)

    def test_the_steps_that_equal_the_disc_are_kept_and_a_differing_one_splits_its_run(self):
        rnd = random.Random(5)
        arts = [0x100010 + 0x10 * k for k in range(6)]
        specs = random_chain(rnd, 14, arts)
        grid_of = {a: 64 + 32 * k for k, a in enumerate(arts)}
        prog, mod, amap, addrs = self.world(specs, grid_of)
        row = T.analyse_stage(prog, mod, amap, addrs)
        self.assertEqual(row['counts']['equal'], 14)
        self.assertEqual(len(row['runs']), 1)
        self.assertEqual(row['runs'][0][0], 256)
        built = T.build_steps(prog, row['runs'], dict(row['arts']))
        self.assertEqual({o: mod[o:o + 12] for o in built}, built)
        # the disc has a changed countdown in step 5: it is left out, the run is split in two, the others are still built
        bad = 256 + 12 * 5

        def fix(m):
            struct.pack_into('<h', m, bad, struct.unpack_from('<h', m, bad)[0] + 3)
        prog, mod, amap, addrs = self.world(specs, grid_of, fix)
        row = T.analyse_stage(prog, mod, amap, addrs)
        built = T.build_steps(prog, row['runs'], dict(row['arts']))
        self.assertNotIn(bad, built)
        self.assertTrue(all(mod[o:o + 12] == b for o, b in built.items()))
        self.assertGreaterEqual(len(row['runs']), 2)
        self.assertEqual(len(built), 13 - sum(1 for k, (d, fl, art, t) in enumerate(specs) if fl & 0x8000 and t == 5 and k != 5))

    def test_the_table_text(self):
        text = T.c_table([('STAGE00.PAC', dict(runs=[(256, 0x200000, 3)], arts=[(0x100010, 64)])), ('STAGE01.PAC', dict(runs=[], arts=[]))])
        self.assertIn('{256u, 0x200000u, 3u},', text)
        self.assertIn('{0x100010u, 64u},', text)
        self.assertIn('{"STAGE00.PAC", runs_00, 1u, arts_00, 1u},', text)
        self.assertIn('{"STAGE01.PAC", 0, 0u, 0, 0u},', text)
        self.assertNotIn(chr(0x2014), text)


class C(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.dir = Path(cls.tmp.name)
        cls.exe = str(cls.dir / 'test')
        subprocess.run(['gcc', *FLAGS, *SAN, '-I', str(ARC), '-o', cls.exe, str(ARC / 'stage_steps.c'), str(ARC / 'stage_steps_table.c'),
                        str(ARC / 'tests/stage_steps_test.c')], check=True, capture_output=True, text=True, timeout=120)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def run_c(self, prog, spec):
        (self.dir / 'prog').write_bytes(bytes(prog))
        (self.dir / 'spec').write_text(spec)
        out = self.dir / 'out'
        out.mkdir(exist_ok=True)
        r = subprocess.run([self.exe, 'build', str(self.dir / 'prog'), str(self.dir / 'spec'), str(out)], capture_output=True, text=True, timeout=60)
        self.assertIn(r.returncode, (0, 1), r.stdout + r.stderr)
        return r, (out / 'module.bin').read_bytes(), (out / 'module.known').read_bytes()

    @staticmethod
    def spec_for(size, runs, grid_of):
        return f'size {size}\n' + ''.join(f'run {o} {a} {n}\n' for o, a, n in runs) + ''.join(f'art {a} {g}\n' for a, g in grid_of.items())

    def test_random_chains_build_as_the_model_says(self):
        for seed in range(15):
            rnd = random.Random(seed)
            arts = [0x100010 + 0x10 * k for k in range(8)]
            grid_of = {a: 64 + 32 * k for k, a in enumerate(arts)}
            prog = bytearray(0x300000)
            specs_a, specs_b = random_chain(rnd, rnd.randrange(1, 20), arts), random_chain(rnd, rnd.randrange(1, 20), arts)
            addrs_a, addrs_b = put_chain(prog, ART0, specs_a), put_chain(prog, ART0 + 0x1000, specs_b)
            runs = [(0, addrs_a[0], len(specs_a)), (512, addrs_b[0], len(specs_b))]
            built = T.build_steps(bytes(prog), runs, grid_of)
            r, data, known = self.run_c(prog, self.spec_for(1024, runs, grid_of))
            self.assertEqual(r.returncode, 0, (seed, r.stdout))
            expect, kexp = bytearray(1024), bytearray(1024)
            for o, b in built.items():
                expect[o:o + 12] = b
                kexp[o:o + 12] = b'\1' * 12
            self.assertEqual(data, bytes(expect), seed)
            self.assertEqual(known, bytes(kexp), seed)

    def test_a_bad_run_is_refused_and_the_others_are_built(self):
        prog = bytearray(0x300000)
        a = put_chain(prog, ART0, [(8, 0, 0x100010, None), (9, 0, 0x100020, None)])
        b = put_chain(prog, ART0 + 0x1000, [(7, 0, 0x100010, None)])
        good = T.build_steps(bytes(prog), [(100, b[0], 1)], {0x100010: 64})[100]
        # the first run needs a grid for 0x100020, which the row lacks
        r, data, known = self.run_c(prog, self.spec_for(256, [(0, a[0], 2), (100, b[0], 1)], {0x100010: 64}))
        self.assertEqual(r.returncode, 1)
        self.assertIn('error: ', r.stdout)
        self.assertEqual(data[100:112], good)
        self.assertEqual((sum(known[:100]), known[100:112]), (0, b'\1' * 12))
        # no record at the address, a run beyond the module, a jump to no run
        r, data, known = self.run_c(prog, self.spec_for(256, [(0, ART0 + 0x2000, 1)], {0x100010: 64}))
        self.assertEqual((r.returncode, sum(known)), (1, 0))
        r, data, known = self.run_c(prog, self.spec_for(256, [(250, b[0], 1)], {0x100010: 64}))
        self.assertEqual((r.returncode, sum(known)), (1, 0))
        j = put_chain(prog, ART0 + 0x2000, [(8, 0, 0x100010, None), (9, 0x8000, 0x100010, 0)])
        r, data, known = self.run_c(prog, self.spec_for(256, [(0, j[1], 1)], {0x100010: 64}))
        self.assertEqual((r.returncode, sum(known)), (1, 0))

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
            for src in (ARC / 'stage_steps.c', ARC / 'stage_steps_table.c'):
                r = subprocess.run(['gcc', *extra, *FLAGS, '-O2', '-I', str(ARC), '-c', str(src), '-o', str(self.dir / 'x.o')], capture_output=True, text=True)
                self.assertEqual((r.returncode, r.stderr), (0, ''), (extra, src))


if __name__ == '__main__':
    result = unittest.main(exit=False, verbosity=0)
    sys.exit(0 if result.result.wasSuccessful() else 1)
