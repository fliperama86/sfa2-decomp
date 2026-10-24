#!/usr/bin/env python3
"""Seconds: the art record parse and the canvas drawing of the loader (port/arcade/compose.c) against the Python model (port/arcade/compose.py);
no game files.

Builds compose.c and a driver with the address and undefined-behavior sanitizers, makes small arcade programs (art records of mode 2, mode 0,
mode 4 and mode 10 with flips, parts, palettes and banks; scroll 2 blocks) and graphics of random tiles (some of them flipped copies of
others), and checks that the C gives, cell for cell, the cells the model gives (with and without the palette part rule), the same canvases
(bounding box and every pixel, later cells over earlier ones where they are not transparent), the same 16 x 16 cuts (inside, across the
edge, outside, with all rows, the odd rows or the even rows), the scroll 2 blocks drawn one and two wide, and the single tiles and
scroll 2 cells. It checks that every malformed input is refused by both (a record outside the program, odd or short, of an unknown mode, with
too many or too few cells, coordinates outside the program, a grid with a size, a canvas wider than the limit, a tile outside the graphics,
a block that does not exist, too many blocks), never an overflow or a leak, and that the C compiles warning-free natively and for 32 bits.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/tools'))
import compose as M
import screen_lists as S
import test_arcade_screen_lists as T

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic')
ARC = ROOT / 'port/arcade'
SOURCES = [ARC / 'compose.c']
DRIVER = ARC / 'tests/compose_test.c'
NT, HI = T.NT, T.HI


def mapcode(tile):
    """The code of a scroll 2 cell or a mode 4 or 10 block that draws graphics tile `tile` (0 to NT - 1) of the high bank."""
    return (tile - S.SCROLL_ADD) & 0xffff


class Case:
    """One made-up arcade program and its graphics, written to a folder the driver reads."""

    def __init__(self, folder, prog_bytes, gfx):
        self.dir = Path(folder)
        self.prog, self.gfx = bytes(prog_bytes), gfx
        (self.dir / 'prog.bin').write_bytes(self.prog)
        (self.dir / 'gfx.bin').write_bytes(gfx)


class ComposeC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.exe = Path(cls.tmp.name) / 'compose_test'
        r = subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES), str(DRIVER),
                            '-o', str(cls.exe)], capture_output=True, text=True, timeout=120)
        if r.returncode:
            raise RuntimeError(r.stderr)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def setUp(self):
        self.work = tempfile.TemporaryDirectory()

    def tearDown(self):
        self.work.cleanup()

    # ---- driver

    def run_c(self, *args):
        r = subprocess.run([str(self.exe), *map(str, args)], capture_output=True, text=True, timeout=60)
        self.assertNotIn('Sanitizer', r.stderr, r.stderr)
        self.assertNotIn('runtime error', r.stderr, r.stderr)
        self.assertIn(r.returncode, (0, 1), r.stderr)
        return r

    def c_record(self, case, address, bank, split):
        """(info, cells) as the C gives them, or an error string."""
        r = self.run_c('record', case.dir / 'prog.bin', address, bank, int(split))
        lines = r.stdout.splitlines()
        if lines[0].startswith('error: '):
            return lines[0][7:]
        info = tuple(int(v) for v in lines[0].split()[1:])
        return info, [tuple(int(v) for v in ln.split()) for ln in lines[1:]]

    def c_canvas(self, case, command, args, cuts):
        """(canvas, cuts) as the C gives them, or an error string. canvas is (x0, y0, w, h, pixels)."""
        out = Path(self.work.name)
        r = self.run_c(command, case.dir / 'prog.bin', case.dir / 'gfx.bin', *args, out, *[v for c in cuts for v in c])
        line = r.stdout.splitlines()[0]
        if line.startswith('error: '):
            return line[7:]
        x0, y0, w, h = (int(v) for v in line.split()[1:])
        data = (out / 'cuts.bin').read_bytes()
        return (x0, y0, w, h, (out / 'canvas.bin').read_bytes()), [data[128 * i:128 * i + 128] for i in range(len(cuts))]

    # ---- scenes

    def scene(self, seed):
        rng = random.Random(seed)
        pix, gfx = T.made_up_graphics(rng)
        prog = T.Prog()
        tile = lambda: rng.randrange(NT)
        attr = lambda: rng.randrange(4) << 5 | rng.choice((0, 3, 5, 12))
        recs = []
        for _ in range(4):
            recs.append(('rec2', prog.record(prog.random_record(rng))))
        shared = rng.randrange(4) << 5 | rng.choice((0, 3, 5))
        recs.append(('rec0', prog.record([(rng.randrange(0, 20), 16 * rng.randrange(-3, 4) + rng.choice((0, 0, 5)), 16 * rng.randrange(-3, 4)) for _ in range(rng.randrange(1, 6))],
                                         shared=shared)))
        blocks = [(mapcode(rng.randrange(NT - 18)), 0x0100 | 0x1000 | rng.choice((0, 3, 12)) | rng.randrange(4) << 5, 16 * rng.randrange(-3, 3), 16 * rng.randrange(-2, 5))
                  for _ in range(rng.randrange(1, 5))]
        recs.append(('rec4', prog.record4(blocks)))
        h, w = rng.randrange(1, 4), rng.randrange(1, 5)
        recs.append(('rec10', prog.record10(h, w, [(mapcode(tile()), attr()) for _ in range(h * w)])))
        for n in (3, 4, 221):
            prog.block(n, {(c, r): (mapcode(tile()), attr()) for c in range(16) for r in range(16)})
        case = Case(self.work.name, prog.data, gfx)
        return rng, pix, gfx, prog, case, recs

    # ---- the parse

    def test_cells_equal_the_model(self):
        cases = 0
        for seed in range(30):
            rng, pix, gfx, prog, case, recs = self.scene(seed)
            for kind, a in recs:
                for bank in (0, HI, 2 * HI):
                    for split in (0, 1):
                        model = M.record_cells(case.prog, a, bank, split)
                        got = self.c_record(case, a, bank, split)
                        self.assertNotIsInstance(got, str, f'seed {seed} {kind}: {got}')
                        mode, cells, end = model
                        self.assertEqual(got[0], (mode, len(cells), len(cells), end), f'seed {seed} {kind}')
                        self.assertEqual(got[1], [tuple(c) for c in cells], f'seed {seed} {kind} bank {bank:#x} split {split}')
                        cases += 1
        self.assertEqual(cases, 30 * 7 * 6)

    def test_parts_follow_the_palette_rule_only_for_modes_0_and_2(self):
        prog = T.Prog()
        blocks = [(1, 0x0000 | 0x00, 0, 0), (2, 0x0000 | 0x03, 16, 0), (3, 0x0000 | 0x03, 32, 0), (4, 0x0000 | 0x03, 40, 0)]
        a = prog.record(blocks)
        case = Case(self.work.name, prog.data, bytes(128))
        plain = self.c_record(case, a, 0, 0)[1]
        split = self.c_record(case, a, 0, 1)[1]
        self.assertEqual([c[4] for c in plain], [0, 0, 0, 1])          # 40 - 32 is not a multiple of 16
        self.assertEqual([c[4] for c in split], [0, 1, 1, 2])          # the palette changes at the second block
        top = prog.record([(1, 0x0003, 0, 0), (2, 0x0013, 16, 0), (3, 0x0073, 32, 0)])         # palette bit 4 counts, the flip bits do not
        case = Case(self.work.name, prog.data, bytes(128))
        self.assertEqual([c[4] for c in self.c_record(case, top, 0, 1)[1]], [0, 1, 1])
        self.assertEqual([c[4] for c in self.c_record(case, top, 0, 0)[1]], [0, 0, 0])
        portrait = prog.record4([(mapcode(0), 0x0000, 0, 0), (mapcode(1), 0x0003, 16, 0)])
        case = Case(self.work.name, prog.data, bytes(128))
        self.assertEqual([c[4] for c in self.c_record(case, portrait, 0, 1)[1]], [0, 0])     # the rule is for modes 0 and 2

    def test_flipped_blocks_mirror_their_codes(self):
        prog = T.Prog()
        a = prog.record([(10, 0x0200 | 0x1000 | 0x20, 0, 0), (50, 0x0100 | 0x1000 | 0x40, 64, 0)])    # 3 x 2 flipped across, 2 x 2 flipped down
        case = Case(self.work.name, prog.data, bytes(128))
        cells = self.c_record(case, a, 0, 0)[1]
        self.assertEqual([(c[0], c[1], c[2]) for c in cells[:6]], [(12, 0, 0), (11, 16, 0), (10, 32, 0), (28, 0, 16), (27, 16, 16), (26, 32, 16)])
        self.assertEqual([(c[0], c[1], c[2]) for c in cells[6:]], [(66, 64, 0), (67, 80, 0), (50, 64, 16), (51, 80, 16)])

    # ---- the canvas

    def cut_positions(self, rng, canvas):
        x0, y0, w, h, _ = canvas
        out = []
        for _ in range(8):
            out.append((x0 + rng.randrange(-24, w + 8), y0 + rng.randrange(-24, h + 8), rng.randrange(3)))
        out.append((x0, y0, 0))
        out.append((x0 + w - 16, y0 + h - 16, 1))
        out.append((x0 - 100, y0, 2))
        return out

    def test_canvases_and_cuts_equal_the_model(self):
        cases = 0
        for seed in range(30):
            rng, pix, gfx, prog, case, recs = self.scene(100 + seed)
            for kind, a in recs:
                bank = 0 if kind in ('rec2', 'rec0') else HI
                if seed % 2 and kind in ('rec2', 'rec0'):
                    bank = HI                       # the same tiles at the high bank
                model = M.record_canvas(case.prog, case.gfx, a, bank)
                self.assertIsNotNone(model, f'seed {seed} {kind}')
                cuts = self.cut_positions(rng, model)
                got = self.c_canvas(case, 'canvas', (a, bank), cuts)
                self.assertNotIsInstance(got, str, f'seed {seed} {kind}: {got}')
                self.assertEqual(got[0], model, f'seed {seed} {kind}')
                self.assertEqual(got[1], [M.cut(model, *c) for c in cuts], f'seed {seed} {kind}')
                cases += 1
        self.assertEqual(cases, 30 * 7)

    def test_later_cells_cover_earlier_ones_only_where_opaque(self):
        pix = [bytes(256) for _ in range(NT)]
        pix[0] = bytes((i % 15) + 1 if i % 5 else 0 for i in range(256))            # some pixels transparent
        pix[1] = bytes((i % 7) + 8 if i % 3 else 0 for i in range(256))
        gfx = b''.join(T.encode_tile(t) for t in pix)
        prog = T.Prog()
        a = prog.record([(0, 0x0000, 0, 0), (1, 0x0000, 8, 8)])
        case = Case(self.work.name, prog.data, gfx)
        got = self.c_canvas(case, 'canvas', (a, 0), [])
        canvas = got[0]
        self.assertEqual(canvas, M.record_canvas(case.prog, gfx, a, 0))
        x0, y0, w, h, px = canvas
        self.assertEqual((x0, y0, w, h), (0, 0, 24, 24))
        for y in range(16):
            for x in range(16):
                a_ = pix[0][y * 16 + x]
                b_ = pix[1][(y - 8) * 16 + (x - 8)] if 8 <= x < 24 and 8 <= y < 24 else 0
                if 8 <= x and 8 <= y and b_:
                    self.assertEqual(px[y * w + x], b_)
                elif a_:
                    self.assertEqual(px[y * w + x], a_)

    def test_scroll2_blocks_and_cells(self):
        cases = 0
        for seed in range(12):
            rng, pix, gfx, prog, case, recs = self.scene(200 + seed)
            for first, blocks in ((3, 1), (3, 2), (4, 1), (221, 1)):
                model = M.scroll2_canvas(case.prog, gfx, first, blocks)
                self.assertIsNotNone(model)
                cuts = [(rng.randrange(-20, 256 * blocks + 4), rng.randrange(-20, 260), rng.randrange(3)) for _ in range(8)] + [(0, 0, 0), (160, 96, 1), (16, 16, 2)]
                got = self.c_canvas(case, 'scroll2', (first, blocks), cuts)
                self.assertNotIsInstance(got, str, got)
                self.assertEqual(got[0], model)
                self.assertEqual(got[1], [M.cut(model, *c) for c in cuts])
                cases += 1
            for c, r in ((0, 0), (15, 15), (3, 9)):
                lines = self.run_c('cell', case.dir / 'prog.bin', 3, c, r).stdout.split()
                want = M.scroll2_cell(case.prog, 3, c, r)
                self.assertEqual((int(lines[1]), int(lines[2])), want)
        self.assertEqual(cases, 48)

    def test_cuts_in_the_pose_style(self):
        # a canvas of two blocks side by side is cut at odd offsets and with the odd or even rows: the pieces of the two fields rebuild the whole
        rng, pix, gfx, prog, case, recs = self.scene(300)
        model = M.scroll2_canvas(case.prog, gfx, 3, 2)
        for x, y in ((160, 96), (250, 100), (3, 5)):
            whole = M.cut(model, x, y, 0)
            odd, even = M.cut(model, x, y, 1), M.cut(model, x, y, 2)
            self.assertEqual(bytes(a | b for a, b in zip(odd, even)), whole)
            self.assertTrue(all(a == 0 for r in range(0, 16, 2) for a in odd[r * 8:r * 8 + 8]))
            got = self.c_canvas(case, 'scroll2', (3, 2), [(x, y, 0), (x, y, 1), (x, y, 2)])
            self.assertEqual(got[1], [whole, odd, even])

    def test_single_tiles(self):
        rng = random.Random(7)
        pix, gfx = T.made_up_graphics(rng)
        case = Case(self.work.name, bytes(16), gfx)
        out = Path(self.work.name)
        for code in (0, 5, NT - 1, HI, HI + NT - 1):
            for flip in range(4):
                self.run_c('tile', case.dir / 'gfx.bin', code, flip, out)
                self.assertEqual((out / 'tile.bin').read_bytes(), M.tile_pixels(gfx, code, flip))
        for code in (HI + NT, 0x7fffffff):
            self.assertEqual(self.run_c('tile', case.dir / 'gfx.bin', code, 0, out).stdout.strip(), 'refused')
            self.assertIsNone(M.tile_pixels(gfx, code, 0))

    # ---- malformed input: refused by the model and the C alike, and never an overflow

    def raw_program(self, size=0x9f400):
        return bytearray(size)

    def test_malformed_records_are_refused(self):
        def put(d, a, *words, fmt='>H'):
            for k, w in enumerate(words):
                struct.pack_into(fmt, d, a + 2 * k, w & 0xffff)
        coord = T.F.COORD_TABLE
        d = self.raw_program()
        bad = {}
        bad['odd address'] = 0x1001
        bad['beyond the program'] = len(d) + 8
        bad['short at the end'] = len(d) - 6
        put(d, 0x1100, 3, 1, 0, 0)
        bad['unknown mode'] = 0x1100
        put(d, 0x1200, 2, 0, 0, 0)
        bad['no cells'] = 0x1200
        put(d, 0x1300, 2, 513, 0, 0)
        bad['too many cells'] = 0x1300
        put(d, 0x1400, 2, 4, 0, 0)                           # one block of no attribute: 1 cell, not 4
        struct.pack_into('>I', d, coord, 0x20000)
        bad['cells that do not add up'] = 0x1400
        put(d, 0x1500, 2, 1, 0x7fff, 0x7fff)                 # coordinate index far outside
        bad['coordinate index outside'] = 0x1500
        put(d, 0x1600, 2, 1, 0, 0)
        struct.pack_into('>I', d, coord, 0x20001)
        bad['odd coordinate pointer'] = 0x1600
        put(d, 0x1700, 2, 1, 0, 0)
        struct.pack_into('>I', d, coord, len(d) - 2)
        bad['coordinates past the end'] = 0x1700
        put(d, 0x1800, 4, 1, 0)                              # a portrait whose 2 x 2 block says 4 cells but whose header says 1
        put(d, 0x1806, 0, 0x0100 | 0x1000, 0, 0)
        bad['portrait cells that do not add up'] = 0x1800
        put(d, 0x1900, 10, 1, 1)                             # a grid of 2 x 2
        put(d, 0x1906, 0, 0, 0, 0x0100, 0, 0, 0, 0)          # one block with a size
        bad['grid block with a size'] = 0x1900
        put(d, 0x1a00, 10, 63, 63)                           # 64 x 64 cells
        bad['grid over the cell limit'] = 0x1a00
        put(d, 0x1b00, 4, 5, 0x7fff)                         # 32768 blocks
        bad['blocks outside the program'] = 0x1b00
        case = Case(self.work.name, d, bytes(128))
        for name, a in bad.items():
            got = self.c_record(case, a, 0, 0)
            self.assertIsInstance(got, str, name)
            self.assertIsNone(M.record_cells(bytes(d), a, 0), name)
            self.assertTrue(got, name)                            # an error line, whatever its text
        # a good record in the same program still parses
        prog = T.Prog()
        good = prog.record([(1, 0x0000, 0, 0)])
        case = Case(self.work.name, prog.data, bytes(128))
        self.assertNotIsInstance(self.c_record(case, good, 0, 0), str)

    def test_malformed_canvases_are_refused(self):
        prog = T.Prog()
        rng = random.Random(3)
        pix, gfx = T.made_up_graphics(rng)
        wide = prog.record([(1, 0x0000, 0, 0), (2, 0x0000, 600, 0)])          # 616 pixels wide: over the limit
        tall = prog.record([(1, 0x0000, 0, 0), (2, 0x0000, 0, -500)])
        edge = prog.record([(1, 0x0000, 0, 0), (2, 0x0000, 496, 0)])          # exactly 512 wide
        outside = prog.record([(1, 0x0000, 0, 0)])                            # with bank 2 * HI: a tile past the graphics
        outside_flip = prog.record([(NT - 1, 0x0200 | 0x1000, 0, 0)])         # with bank HI: 3 x 2 cells reach codes above the last tile
        for block in (3, 4):
            prog.block(block, {(c, r): (mapcode(1), 0) for c in range(16) for r in range(16)})
        prog.block(5, {(0, 0): (mapcode(NT + 5), 0)})                         # a cell of a tile past the graphics
        case = Case(self.work.name, prog.data, gfx)
        for name, a, bank, fine in (('wide', wide, 0, False), ('tall', tall, 0, False), ('edge', edge, 0, True), ('outside', outside, 2 * HI, False),
                                    ('outside flipped', outside_flip, HI, False)):
            model = M.record_canvas(case.prog, gfx, a, bank)
            got = self.c_canvas(case, 'canvas', (a, bank), [(0, 0, 0)])
            if fine:
                self.assertNotIsInstance(got, str, name)
                self.assertEqual(got[0], model)
                self.assertEqual(got[0][2], 512)
            else:
                self.assertIsNone(model, name)
                self.assertIsInstance(got, str, name)
        for first, blocks, ok in ((221, 2, False), (222, 1, False), (3, 3, False), (3, 0, False), (3, 2, True), (5, 1, False)):
            model = M.scroll2_canvas(case.prog, gfx, first, blocks)
            got = self.c_canvas(case, 'scroll2', (first, blocks), [])
            self.assertEqual(model is None, isinstance(got, str), (first, blocks))
            if ok:
                self.assertEqual(got[0], model)
        self.assertEqual(self.run_c('cell', case.dir / 'prog.bin', 222, 0, 0).stdout.strip(), 'refused')
        self.assertEqual(self.run_c('cell', case.dir / 'prog.bin', 3, 16, 0).stdout.strip(), 'refused')
        self.assertEqual(self.run_c('cell', case.dir / 'prog.bin', 3, 0, 16).stdout.strip(), 'refused')

    def test_an_empty_canvas_cuts_to_nothing(self):
        # the model: no cell, no canvas; its cut is 128 zero bytes. (The C draws nothing for no cell; a record always has one.)
        self.assertEqual(M.cut(M.draw(b'', []), 3, 4, 0), bytes(128))


class Builds(unittest.TestCase):
    def test_compiles_warning_free_natively_and_for_32_bits(self):
        d = Path(tempfile.mkdtemp())
        for src in SOURCES + [DRIVER]:
            r = subprocess.run(['gcc', *FLAGS, '-c', str(src), '-o', str(d / 'x.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stderr)
        probe = d / 'probe.c'
        probe.write_text('int main(void){return 0;}\n')
        if subprocess.run(['gcc', '-m32', '-o', str(d / 'probe'), str(probe)], capture_output=True).returncode:
            self.skipTest('no 32-bit toolchain')
        for src in SOURCES + [DRIVER]:
            r = subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / 'x32.o')], capture_output=True, text=True, timeout=60)
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
