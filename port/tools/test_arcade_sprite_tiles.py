#!/usr/bin/env python3
"""Seconds: the sprite tile runs of the fighter files (port/arcade/sprite_tiles.py, sprite_tiles.c, sprite_tiles.h); no game files.

Python side, on a made-up graphics buffer: the flips of a tile, the index and the candidates of a tile under the 4 flips, the runs of each
step (1, 0, -1) and each flip, BLANK and OWN runs, the choice among tiles found under several (code, flip) (the one that continues the run, the
longest run), the split of a run at 65535 tiles, the expansion of runs back to the tiles, and the generated table's text (identical runs shared).

C side: sprite_tiles.c, built with the address and undefined-behavior sanitizers with a driver (tests/sprite_tiles_test.c) and a table that
the generator writes for made-up rows: arcade_sprite_chunk_find, arcade_sprite_tiles_build (agrees with the Python expansion for every step,
flip and BLANK; refuses an OWN run, a code outside the graphics, a run list that adds up to more or fewer tiles than the chunk has, a bad
step or flip), the packing check of the driver, and that the generated table and the checked-in one compile with sprite_tiles.c.
"""
from pathlib import Path
import random, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import sprite_tiles as X

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
DRIVER = ARC / 'tests/sprite_tiles_test.c'
N = 40


def make_gfx():
    """40 made-up arcade tiles (128 random bytes each); the PS1 forms of all of them differ under every flip."""
    rnd = random.Random(2)
    return bytes(rnd.randrange(256) for _ in range(N * 128))


GFX = make_gfx()


def tile(code, flip=0):
    return X.flip_bytes(X.T.arcade_tile(GFX, code), flip)


class Python(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.index = X.tile_index(GFX)

    def test_flips(self):
        t = X.T.arcade_tile(GFX, 3)
        for f in range(4):
            self.assertEqual(X.flip_bytes(X.flip_bytes(t, f), f), t)
            self.assertEqual(X.flip_bytes(t, f), X.T.flip_tile(t, f))
        self.assertEqual(X.flip_bytes(t, 3), X.flip_bytes(X.flip_bytes(t, 1), 2))
        self.assertNotEqual(X.flip_bytes(t, 1), t)

    def test_candidates(self):
        for code, flip in ((0, 0), (5, 1), (6, 2), (39, 3)):
            self.assertEqual(X.candidates(tile(code, flip), self.index), [code * 4 + flip])
        self.assertEqual(X.candidates(bytes([0x37]) * 128, self.index), [])

    def test_candidates_of_symmetric_and_duplicate_tiles(self):
        # a tile that is its own mirror image is found under both flips; two equal arcade tiles are both found
        gfx = bytearray(GFX)
        gfx[9 * 128:10 * 128] = gfx[8 * 128:9 * 128]
        index = X.tile_index(bytes(gfx))
        t = X.T.arcade_tile(bytes(gfx), 8)
        self.assertEqual(X.candidates(t, index), [8 * 4, 9 * 4])

    def test_the_blank_tile_is_not_indexed(self):
        gfx = b'\xff' * 128 + GFX     # all bit planes set: every pixel is the arcade's transparent 15, the PS1's 0
        index = X.tile_index(gfx)
        self.assertNotIn(X.BLANK_TILE, index)
        self.assertEqual(len(index), N)

    def runs(self, tiles):
        return X.make_runs(tiles, self.index)

    def test_steps(self):
        self.assertEqual(self.runs([tile(c) for c in (3, 4, 5, 6)]), [(3, 4, 1, 0)])
        self.assertEqual(self.runs([tile(7)] * 3), [(7, 3, 0, 0)])
        self.assertEqual(self.runs([tile(c) for c in (9, 8, 7)]), [(9, 3, -1, 0)])
        self.assertEqual(self.runs([tile(12)]), [(12, 1, 0, 0)])

    def test_flips_in_runs(self):
        for f in range(4):
            self.assertEqual(self.runs([tile(c, f) for c in (20, 21, 22)]), [(20, 3, 1, f)])
            self.assertEqual(self.runs([tile(c, f) for c in (22, 21, 20)]), [(22, 3, -1, f)])
        self.assertEqual(self.runs([tile(20, 1), tile(21, 2)]), [(20, 1, 0, 1), (21, 1, 0, 2)])

    def test_blank_and_own(self):
        own = bytes([0x37]) * 128
        runs = self.runs([X.BLANK_TILE, X.BLANK_TILE, tile(1), own, own, own, tile(2), X.BLANK_TILE])
        self.assertEqual(runs, [(X.BLANK, 2, 0, 0), (1, 1, 0, 0), (X.OWN, 3, 0, 0), (2, 1, 0, 0), (X.BLANK, 1, 0, 0)])
        self.assertEqual(X.own_tiles(runs), 3)
        self.assertEqual(X.tiles_of(runs), 8)
        self.assertEqual(self.runs([]), [])

    def test_a_tile_found_twice_continues_the_run(self):
        gfx = bytearray(GFX)
        gfx[30 * 128:31 * 128] = gfx[5 * 128:6 * 128]      # tile 30 equals tile 5
        index = X.tile_index(bytes(gfx))
        t = lambda c: X.T.arcade_tile(bytes(gfx), c)
        # 4, 5(=30), 6: the run goes through 5, not through 30
        self.assertEqual(X.make_runs([t(4), t(5), t(6)], index), [(4, 3, 1, 0)])
        # 29, 30(=5), 31: the run goes through 30
        self.assertEqual(X.make_runs([t(29), t(30), t(31)], index), [(29, 3, 1, 0)])
        # the longest run wins at the start: 5, 6, 7 beats 30 alone
        self.assertEqual(X.make_runs([t(30), t(6), t(7)], index), [(5, 3, 1, 0)])

    def test_the_same_flip_as_before_breaks_a_tie(self):
        gfx = bytearray(GFX)
        gfx[30 * 128:31 * 128] = gfx[5 * 128:6 * 128]
        index = X.tile_index(bytes(gfx))
        t = lambda c, f: X.flip_bytes(X.T.arcade_tile(bytes(gfx), c), f)
        # tile 5 mirrored is also tile 30 mirrored; after a run with flip 1 the next single tile keeps flip 1
        runs = X.make_runs([t(10, 1), t(5, 1)], index)
        self.assertEqual(runs[0], (10, 1, 0, 1))
        self.assertEqual(runs[1][3], 1)

    def test_long_runs_split(self):
        runs = X.make_runs([X.BLANK_TILE] * (X.MAX_COUNT + 5), {})
        self.assertEqual(runs, [(X.BLANK, X.MAX_COUNT, 0, 0), (X.BLANK, 5, 0, 0)])

    def test_expand_agrees(self):
        tiles = [X.BLANK_TILE, tile(3), tile(4), tile(4), tile(9, 3), tile(8, 3), tile(30, 2), X.BLANK_TILE]
        runs = self.runs(tiles)
        self.assertEqual(X.expand_runs(runs, GFX), tiles)
        with self.assertRaises(ValueError):
            X.expand_runs([(X.OWN, 1, 0, 0)], GFX)
        with self.assertRaises(ValueError):
            X.expand_runs([(N - 1, 2, 1, 0)], GFX)
        with self.assertRaises(ValueError):
            X.expand_runs([(0, 2, -1, 0)], GFX)

    def test_redraw_and_own_numbers(self):
        tiles = [X.BLANK_TILE, tile(3), tile(4), tile(5)]
        runs = X.make_runs([tiles[0], tile(3), bytes([0x37]) * 128, bytes([0x38]) * 128], self.index)
        self.assertEqual(X.own_numbers(runs), {2, 3})
        saved = X.REDRAWN
        try:
            X.REDRAWN = ((2, 7, 1), (3, None, 0))
            out, changed = X.redraw(tiles, {2, 3}, GFX)
            self.assertEqual(changed, {2, 3})
            self.assertEqual(out, [tiles[0], tiles[1], tile(7, 1), X.BLANK_TILE])
            with self.assertRaises(ValueError):    # the numbers must be exactly the chunk's own tiles
                X.redraw(tiles, {2}, GFX)
            X.REDRAWN = ((2, N, 0), (3, None, 0))
            with self.assertRaises(ValueError):    # a code outside the graphics
                X.redraw(tiles, {2, 3}, GFX)
        finally:
            X.REDRAWN = saved

    def test_mark_splits_the_redrawn_tiles(self):
        runs = [(10, 4, 1, 1), (X.BLANK, 2, 0, 0), (20, 3, -1, 0)]
        self.assertEqual(X.mark(runs, set()), runs)
        self.assertEqual(X.mark(runs, {1, 2, 4, 8}), [
            (10, 1, 0, 1), (11, 2, 1, 1 | X.REDRAWN_FLIP), (13, 1, 0, 1),
            (X.BLANK, 1, 0, X.REDRAWN_FLIP), (X.BLANK, 1, 0, 0),
            (20, 2, -1, 0), (18, 1, 0, X.REDRAWN_FLIP)])
        self.assertEqual(X.expand_runs(X.mark(runs, {1, 2, 4, 8}), GFX), X.expand_runs(runs, GFX))

    def test_redrawn_table_is_the_gen_files(self):
        numbers = [t for t, _c, _f in X.REDRAWN]
        self.assertEqual(len(numbers), 62)
        self.assertEqual(len(set(numbers)), 62)
        self.assertEqual(numbers, sorted(numbers))
        self.assertEqual(numbers[:32], list(range(3180, 3212)))
        self.assertTrue(all(f in range(4) for _t, _c, f in X.REDRAWN))
        self.assertEqual(X.REDRAWN_FILES, {'PL11.PAC', 'PL11X.PAC', 'PL13.PAC', 'PL13X.PAC'})

    def test_compare_numbering(self):
        tiles = [X.BLANK_TILE, tile(3), tile(4, 1), tile(5)]
        runs = self.runs(tiles)
        self.assertEqual(X.compare_numbering(tiles, runs, {1: 3, 2: 4, 3: 5}, {1: 0, 2: 1, 3: 0}, GFX), (3, 3, 0, 0))
        self.assertEqual(X.compare_numbering(tiles, runs, {1: 3, 2: 4, 3: 6}, {1: 0, 2: 1, 3: 0}, GFX), (3, 2, 0, 1))
        self.assertEqual(X.compare_numbering(tiles, runs, {1: 3}, {1: 2}, GFX), (1, 0, 0, 1))

    def test_c_table_text(self):
        a = [(1, 2, 1, 0), (X.BLANK, 1, 0, 0)]
        b = [(X.OWN, 4, 0, 0)]
        rows = [dict(file='PL00.PAC', slot=1, size=100, tiles=3, runs=a), dict(file='PL00X.PAC', slot=2, size=100, tiles=3, runs=list(a)),
                dict(file='PL01.PAC', slot=1, size=7, tiles=4, runs=b)]
        text = X.c_table(rows)
        self.assertEqual(text.count('static const struct arcade_sprite_run'), 2)       # identical runs are shared
        self.assertIn('{"PL00X.PAC", 2u, 100u, 3u, runs_pl00, 2u},', text)
        self.assertIn('{ARCADE_SPRITE_RUN_BLANK, 1u, 0, 0u},', text)
        self.assertIn('{ARCADE_SPRITE_RUN_OWN, 4u, 0, 0u},', text)
        self.assertIn('{1u, 2u, 1, 0u},', text)
        self.assertNotIn(chr(0x2014), text)


def rows_for_c():
    tiles = [X.BLANK_TILE, tile(3), tile(4), tile(5), tile(5), tile(9, 1), tile(8, 1), tile(7, 2), tile(39, 3), X.BLANK_TILE]
    good = X.make_runs(tiles, X.tile_index(GFX))
    n = len(tiles)
    return tiles, [
        dict(file='good', slot=1, size=20000, tiles=n, runs=good),
        dict(file='gooX', slot=2, size=20000, tiles=n, runs=list(good)),
        dict(file='own', slot=1, size=20000, tiles=3, runs=[(1, 1, 0, 0), (X.OWN, 2, 0, 0)]),
        dict(file='outside', slot=1, size=20000, tiles=3, runs=[(N - 2, 3, 1, 0)]),
        dict(file='below', slot=1, size=20000, tiles=3, runs=[(1, 3, -1, 0)]),
        dict(file='more', slot=1, size=20000, tiles=3, runs=[(1, 2, 1, 0), (5, 2, 1, 0)]),
        dict(file='fewer', slot=1, size=20000, tiles=5, runs=[(1, 2, 1, 0), (5, 2, 1, 0)]),
        dict(file='step', slot=1, size=20000, tiles=2, runs=[(1, 2, 2, 0)]),
        dict(file='flip', slot=1, size=20000, tiles=2, runs=[(1, 2, 1, 8)]),
        dict(file='huge', slot=1, size=20000, tiles=2, runs=[(0xfffffff0, 2, 1, 0)]),
        dict(file='empty', slot=1, size=20000, tiles=0, runs=[]),
        dict(file='tight', slot=1, size=60, tiles=n, runs=good),
    ]


class C(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.dir = Path(cls.tmp.name)
        cls.tiles, cls.rows = rows_for_c()
        (cls.dir / 'table.c').write_text(X.c_table(cls.rows))
        (cls.dir / 'gfx').write_bytes(GFX)
        cls.exe = str(cls.dir / 'test')
        subprocess.run(['gcc', *FLAGS, *SAN, '-I', str(ARC), '-o', cls.exe, str(ARC / 'sprite_tiles.c'), str(cls.dir / 'table.c'), str(ARC / 'tilepack.c'),
                        str(DRIVER)], check=True, capture_output=True, text=True, timeout=120)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def run_driver(self, *args):
        return subprocess.run([self.exe, *args], capture_output=True, text=True, timeout=60)

    def build(self, name):
        out = self.dir / 'out'
        out.unlink(missing_ok=True)
        run = self.run_driver('build', name, str(self.dir / 'gfx'), str(out))
        return run, (out.read_bytes() if out.exists() else None)

    def test_list_and_find(self):
        out = self.run_driver('list').stdout.split('\n')
        self.assertEqual(out[0], 'count %d' % len(self.rows))
        self.assertEqual(out[1].split(), ['good', '1', '20000', str(len(self.tiles)), str(len(self.rows[0]['runs']))])
        self.assertEqual(self.run_driver('find', 'good').returncode, 0)
        self.assertEqual(self.run_driver('find', 'gooX').returncode, 0)
        self.assertEqual(self.run_driver('find', 'goo').returncode, 1)
        self.assertEqual(self.run_driver('find', 'good2').returncode, 1)
        self.assertEqual(self.run_driver('find', '').returncode, 1)

    def test_build_agrees_with_python(self):
        for name in ('good', 'gooX'):
            run, got = self.build(name)
            self.assertEqual(run.returncode, 0, run.stderr)
            self.assertEqual(got, b''.join(self.tiles))
        run, got = self.build('good')
        self.assertEqual(got, b''.join(X.expand_runs(self.rows[0]['runs'], GFX)))

    def test_the_runs_of_each_step_and_flip(self):
        # one row per step and flip is not in the table; the good row has steps 1, 0 and -1 and flips 0 to 3 (checked above), and an empty row builds nothing
        run, got = self.build('empty')
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(got, b'')
        steps = {r[2] for r in self.rows[0]['runs']}
        flips = {r[3] for r in self.rows[0]['runs']}
        self.assertEqual(steps, {0, 1, -1})
        self.assertEqual(flips, {0, 1, 2, 3})

    def test_refusals(self):
        for name, what in (('own', 'does not have'), ('outside', 'outside the graphics'), ('below', 'outside the graphics'), ('huge', 'outside the graphics'),
                           ('more', 'more tiles'), ('fewer', 'fewer tiles'), ('step', 'bad step'), ('flip', 'bad step or flip')):
            run, got = self.build(name)
            self.assertEqual(run.returncode, 3, name)
            self.assertIn(what, run.stderr, name)
            self.assertIn(name, run.stderr)
            self.assertIsNone(got, name)
        self.assertEqual(self.build('nope')[0].returncode, 4)

    def test_check_compares_and_packs(self):
        disc = self.dir / 'disc'
        disc.write_bytes(b''.join(self.tiles))
        run = self.run_driver('check', 'good', str(self.dir / 'gfx'), str(disc))
        fields = run.stdout.split()
        self.assertEqual(run.returncode, 0, run.stderr)
        self.assertEqual(fields[:5], ['built', '0', '0', '-', fields[4]])
        self.assertTrue(fields[4].isdigit() and int(fields[4]) <= 20000)
        # a tile that differs is counted and the first one named
        bad = bytearray(b''.join(self.tiles))
        bad[4 * 128] ^= 1
        disc.write_bytes(bytes(bad))
        self.assertEqual(self.run_driver('check', 'good', str(self.dir / 'gfx'), str(disc)).stdout.split()[:4], ['built', '0', '1', '4'])
        # a chunk that does not fit its room is reported as such
        disc.write_bytes(b''.join(self.tiles))
        out = self.run_driver('check', 'tight', str(self.dir / 'gfx'), str(disc)).stdout.split()
        self.assertEqual(out[:2] + out[4:5], ['built', '0', 'unpacked'])
        # an OWN run is counted, its tiles taken as the disc's
        own_disc = tile(1) + self.tiles[1] + self.tiles[2]
        disc.write_bytes(own_disc)
        self.assertEqual(self.run_driver('check', 'own', str(self.dir / 'gfx'), str(disc)).stdout.split()[:4], ['built', '2', '0', '-'])
        # a code outside the graphics still refuses
        self.assertEqual(self.run_driver('check', 'outside', str(self.dir / 'gfx'), str(disc)).returncode, 3)

    def test_generated_table_compiles(self):
        # the made-up rows are built above; here the checked-in table (the real rows) must compile with sprite_tiles.c
        table = ARC / 'sprite_tiles_table.c'
        self.assertTrue(table.exists(), 'port/arcade/sprite_tiles_table.c is not checked in')
        for src in (self.dir / 'table.c', table):
            run = subprocess.run(['gcc', *FLAGS, '-I', str(ARC), '-c', '-o', str(self.dir / 'table.o'), str(src)], capture_output=True, text=True, timeout=120)
            self.assertEqual(run.returncode, 0, run.stderr)
        text = table.read_text()
        self.assertIn('arcade_sprite_chunks[]', text)
        self.assertNotIn(chr(0x2014), text)


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
