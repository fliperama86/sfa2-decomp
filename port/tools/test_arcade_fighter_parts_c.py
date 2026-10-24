#!/usr/bin/env python3
"""Seconds: the runtime C of the fighter files' palettes and pages (port/arcade/fighter_parts.c) against the Python model; no game files.

Builds fighter_parts.c, the generated descriptor table and a driver with the address and undefined-behavior sanitizers, makes small
arcade programs and graphics (random, with art records written by the fixture of test_arcade_fighter_parts.py) and checks that the C
gives, byte for byte, the palette chunk and the portrait and shared pages that fighter_parts.py's model gives, for random descriptors
(both block layouts, splits, kept palettes, filler ramps), for every shadow mask and the dropped cell, and that every malformed
input (a palette or record past the program, a flipped or off-grid block, a cell outside the page, a tile past the graphics, a record
of the wrong mode, too many cells) is refused and never overflows. It also checks the generated table's sanity and that the C compiles
warning-free for 32 bits.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'port/tools'))
import fighter_parts as fp
from test_arcade_fighter_parts import Arcade

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SOURCES = [ROOT / 'port/arcade' / n for n in ('fighter_parts.c', 'fighter_parts_table.c')]
DRIVER = ROOT / 'port/arcade/tests/fighter_parts_test.c'


class FighterPartsC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-fighter-parts-')
        d = Path(cls.dir.name)
        cls.exe = d / 'fighter_parts_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES), str(DRIVER),
                        '-o', str(cls.exe)], check=True, timeout=120)
        cls.gfx = Arcade(1).gfx
        (d / 'tiles.bin').write_bytes(cls.gfx)
        cls.n = 0

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_c(self, *args):
        r = subprocess.run([str(self.exe), *map(str, args)], capture_output=True, text=True, timeout=60)
        self.assertNotIn('Sanitizer', r.stderr)
        self.assertNotIn('runtime error', r.stderr)
        return r.returncode, r.stdout + r.stderr

    def case(self, prog, name):
        """Write a program file for one case; returns (case directory, program path)."""
        FighterPartsC.n += 1
        d = Path(self.dir.name)
        (d / 'prog.bin').write_bytes(bytes(prog))
        return d, d / 'prog.bin'

    def arcade(self, seed):
        a = Arcade.__new__(Arcade)
        rnd = random.Random(seed)
        a.rnd, a.records = rnd, 0
        a.prog = bytearray(Arcade.SIZE)
        a.prog[0x230000:] = rnd.randbytes(Arcade.SIZE - 0x230000)
        a.gfx = self.gfx
        return a

    # ------------------------------------------------------------ palettes

    def palettes(self, prog, fighter, desc):
        """(status, text, bytes, kept) of the C for the descriptor."""
        d, p = self.case(prog, 'palettes')
        ramps = desc['ramps']
        spec = f"{fighter} {1 if desc['layout'] == 'fives' else 0} {desc['split']} {desc['keep']:x} {0 if ramps is None else 18} {desc['base']:x}"
        if ramps is not None:
            spec += ' ' + ' '.join(f'{0xffff if v is None else v:x}' for v in ramps)
        (d / 'pspec.txt').write_text(spec + '\n')
        status, text = self.run_c('palettes', p, d / 'pspec.txt', d / 'pal.bin')
        if status:
            return status, text, None, None
        return 0, text, (d / 'pal.bin').read_bytes(), int(text.split()[1], 16)

    def same_palettes(self, prog, fighter, desc):
        try:
            want = fp.palette_chunk(fighter, bytes(prog), desc)
        except ValueError:
            status, text, *_ = self.palettes(prog, fighter, desc)
            self.assertEqual(status, 1, text)
            self.assertIn('has no arcade address', text)
            return None
        status, text, got, kept = self.palettes(prog, fighter, desc)
        self.assertEqual(status, 0, text)
        self.assertEqual((got, kept), want)
        return want

    def test_palettes_equal_the_model_for_random_descriptors(self):
        rnd = random.Random(11)
        a = self.arcade(11)
        built = refused = 0
        for _ in range(30):
            layout = rnd.choice(['flat', 'fives'])
            split = rnd.choice([8, 14, 16, 16, 16])
            fighter = rnd.randrange(0x13)
            keep = sum(1 << rnd.randrange(64) for _ in range(rnd.randrange(8)))
            ramps = None if rnd.random() < 0.3 else [rnd.choice([None, None, rnd.randrange(0x30, 0x7e0)]) for _ in range(18)]
            desc = dict(layout=layout, split=split, base=rnd.randrange(0x270000, 0x29e000, 32), keep=keep, ramps=ramps)
            if layout == 'fives' and rnd.random() < 0.8:
                desc['keep'] |= sum(1 << n for n in range(1, 64, 2) if (n >> 1) - split >= 12)
            if self.same_palettes(a.prog, fighter, desc) is None:
                refused += 1
            else:
                built += 1
        self.assertTrue(built >= 10 and refused >= 1, (built, refused))

    def test_colors_black_brightness_and_the_transparent_color(self):
        a = self.arcade(12)
        base = fp.A_BASE + fp.A_STRIDE * 4
        words = [0x0000, 0xf000, 0x0fff, 0xffff, 0x0001, 0x0010, 0x0100, 0x1000, 0x0f0f, 0x0ab5, 0x8421, 0x0000, 0x0888, 0x0777, 0x0666, 0x0abc]
        struct.pack_into('>16H', a.prog, base, *words)
        desc = dict(layout='flat', split=16, base=0x29c300, keep=0, ramps=None)
        chunk, _ = self.same_palettes(a.prog, 4, desc)
        got = struct.unpack_from('<16H', chunk, 0)
        self.assertEqual(got[0], 0)                          # the arcade's color 15 is the transparent one, and is not read
        self.assertEqual(got[1], 0x8000)                     # black is opaque black
        self.assertEqual(got[2], 0x8000)                     # the brightness nibble is dropped
        self.assertEqual(got[3], fp.ps1_color(0x0fff))
        self.assertEqual(got[12], fp.ps1_color(0x0000))

    def test_filler_ramps_and_kept_palettes(self):
        a = self.arcade(13)
        ramps = [None] * 18
        ramps[0], ramps[15], ramps[16], ramps[17] = 0x7a8, 0x770, 0x3c, 0x0f8
        desc = dict(layout='flat', split=16, base=0x29c300, keep=1 << 3 | 1 << 62, ramps=ramps)
        chunk, kept = self.same_palettes(a.prog, 2, desc)
        self.assertEqual(kept, 1 << 3 | 1 << 62)
        self.assertEqual(chunk[96:128], bytes(32))
        self.assertEqual(chunk[62 * 32:63 * 32], bytes(32))
        self.assertEqual(struct.unpack_from('<16H', chunk, 33 * 32), tuple(fp.ps1_palette(fp.ramp_words(0x7a8))))
        self.assertEqual(struct.unpack_from('<16H', chunk, 63 * 32), tuple(fp.ps1_palette(fp.ramp_words(0x770))))
        self.assertEqual(struct.unpack_from('<16H', chunk, 60 * 32), tuple(fp.ps1_palette(fp.ramp_words(0x3c))))

    def test_palettes_outside_the_program_or_without_an_address_are_refused(self):
        a = self.arcade(14)
        desc = dict(layout='flat', split=16, base=0x29c300, keep=0, ramps=None)
        for end in (0x26d000, 0x26d0e0 + 0x3c0 * 3 + 20 * 32 + 10, 0x28b150):
            short = bytes(a.prog[:end])
            status, text, *_ = self.palettes(short, 3, desc)
            self.assertEqual(status, 1, end)
            self.assertIn('has no arcade address in the program', text)
            with self.assertRaises(ValueError):
                fp.palette_chunk(3, short, desc)
        fives = dict(desc, layout='fives', base=0x29ae20)
        status, text, *_ = self.palettes(a.prog, 3, fives)
        self.assertEqual((status, 'palette 57' in text), (1, True))      # the fives layout has no palette 12 and up
        status, text, *_ = self.palettes(a.prog, 3, dict(fives, keep=sum(1 << n for n in (57, 59, 61, 63))))
        self.assertEqual(status, 0, text)
        status, text, *_ = self.palettes(a.prog, 3, dict(desc, base=0x29fff0))     # the second block runs past the program
        self.assertEqual(status, 1)

    # ------------------------------------------------------------ pages

    def portrait(self, prog, fighter, shadow=0, flags=0, drop=(-1, -1), cells=()):
        """(status, text, bytes)."""
        d, p = self.case(prog, 'portrait')
        (d / 'gspec.txt').write_text(f'{fighter} {shadow:x} {flags} {drop[0]} {drop[1]}\n' + ''.join(f'{cx} {cy} {vx} {vy}\n' for cx, cy, vx, vy in cells))
        status, text = self.run_c('portrait', p, d / 'tiles.bin', d / 'gspec.txt', d / 'page.bin')
        return status, text, (d / 'page.bin').read_bytes() if status == 0 else None

    def shared(self, prog):
        d, p = self.case(prog, 'shared')
        status, text = self.run_c('shared', p, d / 'tiles.bin', d / 'page.bin')
        return status, text, (d / 'page.bin').read_bytes() if status == 0 else None

    def standard(self, seed, fighter=5):
        a = self.arcade(seed)
        a.standard_shared()
        a.portrait(fighter, [(0x0100, 0x1102, -96, -80), (0x0200, 0x0001, -48, 0), (0x0300, 0x0301, 48, 16), (0x0b9a, 0x0001, -32, -80),
                             (0x0bd6, 0x0902, -96, -16), (0x0100, 0x1102, -64, -64)])         # the last overlaps the first and the fourth's row
        return a

    def test_portrait_pages_equal_the_model(self):
        a = self.standard(21)
        shadow = fp.shared_tiles(a.prog)[1]
        rnd = random.Random(21)
        masks = [0, (1 << len(shadow)) - 1, 1, 1 << (len(shadow) - 1)] + [rnd.randrange(1 << len(shadow)) for _ in range(4)]
        for mask in masks:
            for drop in (None, (0, 0), (3, 5)):
                status, text, page = self.portrait(a.prog, 5, mask, 0, drop or (-1, -1))
                self.assertEqual(status, 0, text)
                self.assertEqual(page, fp.portrait_page(a.prog, a.gfx, 5, mask, drop), (mask, drop))

    def test_a_page_flagged_as_the_discs_is_not_built(self):
        a = self.standard(22)
        status, text, _ = self.portrait(a.prog, 5, 3, 1)
        self.assertEqual((status, text), (2, ''))                # status 2 and nothing written

    def test_cells_of_the_shared_page_placed_besides(self):
        a = self.standard(26)
        shared = fp.shared_tiles(a.prog)[0]
        some = sorted(shared)[:3]
        cells = [(c[0], 15 - c[1] if c[1] < 8 else c[1] - 1, c[0], c[1]) for c in some] + [(15, 15, some[0][0], some[0][1])]
        status, text, page = self.portrait(a.prog, 5, 3, 2, cells=cells)                    # flag 2: owned
        self.assertEqual(status, 0, text)
        self.assertEqual(page, fp.portrait_page(a.prog, a.gfx, 5, 3, None, cells))
        self.assertNotEqual(page, fp.portrait_page(a.prog, a.gfx, 5, 3, None))
        status, text, _ = self.portrait(a.prog, 5, 0, 2, cells=[(1, 1, 15, 15)])             # the shared page has no cell there
        self.assertEqual(status, 1)
        self.assertIn('no cell', text)

    def test_the_shared_page_equals_the_model(self):
        a = self.standard(23)
        status, text, page = self.shared(a.prog)
        self.assertEqual(status, 0, text)
        self.assertEqual(page, fp.shared_page(a.prog, a.gfx))

    def test_a_portrait_with_blocks_at_the_corners(self):
        a = self.arcade(24)
        a.standard_shared()
        a.portrait(0, [(0x0000, 0x0001, -96, -80), (0xffff, 0x0001, 144, 160), (0x0123, 0xff02, -96, -80)])  # the last covers 16 x 16 cells
        status, text, page = self.portrait(a.prog, 0, 0x55, 0)
        self.assertEqual(status, 0, text)
        self.assertEqual(page, fp.portrait_page(a.prog, a.gfx, 0, 0x55))

    def test_malformed_portraits_are_refused(self):
        def refused(prog, fighter, text, shadow=0, tiles=None):
            if tiles is not None:
                (Path(self.dir.name) / 'tiles.bin').write_bytes(tiles)
            try:
                status, out, _ = self.portrait(prog, fighter, shadow)
            finally:
                (Path(self.dir.name) / 'tiles.bin').write_bytes(self.gfx)
            self.assertEqual(status, 1, out)
            self.assertIn(text, out)
        a = self.standard(25)
        refused(a.prog[:0x5e6c0], 5, 'has no portrait step')
        refused(a.prog[:0x100200], 5, 'no mode 4 art record')                         # fighter 5's record at 0x105000 is gone
        b = self.standard(26)
        b.portrait(6, [(0x0100, 0x0001, -88, -80)])                                   # off the cell grid
        refused(b.prog, 6, 'flip or off the cell grid')
        b.portrait(6, [(0x0100, 0x0041, -96, -80)])                                   # flipped down
        refused(b.prog, 6, 'flip or off the cell grid')
        b.portrait(6, [(0x0100, 0x0001, 160, -80)])                                   # cell 16: right of the page
        refused(b.prog, 6, 'outside the page')
        b.portrait(6, [(0x0100, 0x0001, -112, -80)])
        refused(b.prog, 6, 'outside the page')
        b.portrait(6, [(0x0100, 0x0001, -96, 176)])
        refused(b.prog, 6, 'outside the page')
        struct.pack_into('>I', b.prog, fp.STEPS + fp.STEP_STRIDE * 6, 0x2fffff0)      # the step names an address past the program
        refused(b.prog, 6, 'no mode 4 art record')
        struct.pack_into('>I', b.prog, fp.STEPS + fp.STEP_STRIDE * 6, 0x100001)       # odd
        refused(b.prog, 6, 'no mode 4 art record')
        struct.pack_into('>I', b.prog, fp.STEPS + fp.STEP_STRIDE * 6, 0x0f0000)       # zeros: mode 0
        refused(b.prog, 6, 'no mode 4 art record')
        struct.pack_into('>I', b.prog, fp.STEPS + fp.STEP_STRIDE * 6, Arcade.SIZE - 6)   # a record whose blocks lie past the end
        struct.pack_into('>HHH', b.prog, Arcade.SIZE - 6, 4, 1, 0)
        refused(b.prog, 6, 'runs past the program')
        struct.pack_into('>I', b.prog, fp.STEPS + fp.STEP_STRIDE * 6, 0x140000)       # 65536 blocks of 1 x 1 in zeros
        struct.pack_into('>HHH', b.prog, 0x140000, 4, 0, 0xffff)
        refused(b.prog, 6, 'too many cells')
        c = self.standard(27)
        refused(c.prog, 5, 'is outside the graphics', tiles=self.gfx[:0x100000])        # the graphics end before the portrait's tiles
        refused(c.prog[:0x29c800], 5, 'portrait palette', tiles=None)
        d = self.standard(28)
        status, out, _ = self.portrait(d.prog, 5, 0x1fff)                              # a shadow cell that does not exist
        self.assertEqual(status, 1, out)
        self.assertIn('shadow cell', out)

    def test_malformed_shared_pages_are_refused(self):
        def bad(a, text):
            status, out, _ = self.shared(a.prog)
            self.assertEqual(status, 1, out)
            self.assertIn(text, out)
        a = self.arcade(31)
        bad(a, 'no mode 2 art record')                                                 # zeros
        a.standard_shared()
        self.assertEqual(self.shared(a.prog)[0], 0)
        short = Arcade.__new__(Arcade)
        short.prog = bytearray(a.prog[:fp.VS_FIRST + 12])
        bad(short, 'runs past the program')
        b = self.arcade(32)
        b.standard_shared()
        struct.pack_into('>I', b.prog, fp.COORD_TABLE, 0x2fffff0)                      # the first record's coordinates lie past the program
        bad(b, 'lie past the program')
        c = self.arcade(33)
        c.shared([[(0x7000, 0x0001, -32, 0)], [(0x7000, 0x0021, 0, 0)], [(0x7000, 0x0001, 0, 0)], [(0x7000, 0x0001, 0, 0)]])    # the second has a flip
        bad(c, 'flip or off the cell grid')
        d = self.arcade(34)
        d.shared([[(0x7000, 0x0001, 640, 0)], [(0x7000, 0x0001, 0, 0)], [(0x7000, 0x0001, 0, 0)], [(0x7000, 0x0001, 0, 0)]])   # past the page
        bad(d, 'outside the page')
        status, _out = self.run_c('shared', Path(self.dir.name) / 'prog.bin', Path(self.dir.name) / 'nothing.bin', Path(self.dir.name) / 'x.bin')
        self.assertEqual(status, 3)                                                    # a missing graphics file is a setup failure

    # ------------------------------------------------------------ the table and the build

    def test_the_generated_table_is_sane(self):
        status, out = self.run_c('table')
        self.assertEqual((status, out.strip()), (0, 'ok'))
        text = SOURCES[1].read_text()
        self.assertNotIn(chr(0x2014), text)
        self.assertEqual(text.count('GENERATED by'), 1)

    def test_it_compiles_for_32_bits_warning_free(self):
        out = Path(self.dir.name) / 'fp32.o'
        r = subprocess.run(['gcc', '-m32', *FLAGS, '-pedantic', '-c', str(SOURCES[0]), '-o', str(out)], capture_output=True, text=True, timeout=60)
        if 'sys/cdefs.h' in r.stderr or 'bits/' in r.stderr or 'gnu/stubs-32.h' in r.stderr:
            self.skipTest('no 32-bit headers')
        self.assertEqual(r.returncode, 0, r.stderr)
        r = subprocess.run(['gcc', *FLAGS, '-pedantic', '-c', str(SOURCES[1]), '-o', str(Path(self.dir.name) / 'tab.o')], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 0, r.stderr)


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
