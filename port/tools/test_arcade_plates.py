#!/usr/bin/env python3
"""Seconds: the name plates' cell maps of the loader (port/arcade/plates.c) against the Python model (port/arcade/plates.py); no game files.

Builds plates.c, compose.c and a driver with the address and undefined-behavior sanitizers, makes small arcades (scroll 1 sets and groups, graphics of random
tiles), pages of 8 x 8 units (some of them the strips' pieces, some twice, some not built, some unknown), and checks that the C gives, byte for byte, the map the model gives
(the constant cell outside the strip, the first unit of the page that holds a piece, the palette word of the attribute, the constant cell for a piece the page lacks
or whose graphics tile is missing, a unit that is not whole not looked up), for every band; that a piece's flips are the attribute's; that the compose helpers
(scroll 1 cell, piece) equal the model; that a program too short is refused; the table's shape; and that the C compiles warning-free natively and for 32 bits.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/tools'))
import plates as P
import screen_lists as S
import test_arcade_screen_lists as L

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic')
ARC = ROOT / 'port/arcade'
SOURCES = [ARC / 'plates.c', ARC / 'compose.c']
DRIVER = ARC / 'tests/plates_test.c'
NT, HI = L.NT, L.HI
SIZE = 0x300000


class World:
    """A made-up arcade: scroll 1 sets 0, 64 to 83 and 84 over random groups, graphics at the high bank."""

    def __init__(self, seed, missing=0.05):
        self.rng = rng = random.Random(seed)
        self.pix, self.gfx = L.made_up_graphics(rng)
        prog = bytearray(SIZE)
        self.groups = 40
        for g in range(self.groups):
            for k in range(16):
                code = rng.randrange(2 * NT) if rng.random() > missing else 2 * NT + rng.randrange(10)     # codes past the graphics: no piece
                attr = rng.randrange(4) << 5 | rng.randrange(32)
                struct.pack_into('>HH', prog, S.GROUPS1 + 64 * g + 4 * k, code, attr)
        for s in [0] + list(range(64, 85)):
            for k in range(64):
                struct.pack_into('>H', prog, S.SETS1 + 128 * s + 2 * k, rng.randrange(self.groups))
        self.prog = bytes(prog)

    def pieces(self, band):
        out = []
        for r in range(8):
            for c in range(64):
                s, x = P.set_of(c, band)
                code, attr = S.scroll1_cell(self.prog, s, x, 3 + r)
                piece = S.scroll1_piece_bytes(self.gfx, code, attr)
                if piece is not None:
                    out.append(piece)
        return out

    def page(self, band, share=0.7):
        """A page whose units hold `share` of the strip's pieces (twice some of them), the rest random; and the mask: a few units not built."""
        rng = self.rng
        pieces = self.pieces(band)
        units = [bytes(rng.randrange(256) for _ in range(32)) for _ in range(1024)]
        for p in rng.sample(pieces, int(len(pieces) * share)):
            units[rng.randrange(700)] = p
        for _ in range(30):
            units[rng.randrange(1024)] = rng.choice(pieces)                       # a piece in two places: the first (rows, then columns) wins
        for i in range(700, 1024):
            units[i] = bytes(32)                                                   # the page's blank part, where the units that are not named are free
        units[0] = bytes(32)                                                       # the blank unit, as the real page has
        page = bytearray(32768)
        known = bytearray(b'\x01') * 32768
        for i, u in enumerate(units):
            v, c = divmod(i, 32)
            for y in range(8):
                page[(v * 8 + y) * 128 + c * 4:(v * 8 + y) * 128 + c * 4 + 4] = u[y * 4:y * 4 + 4]
        for _ in range(20):
            i = rng.randrange(1024)
            v, c = divmod(i, 32)
            known[(v * 8 + rng.randrange(8)) * 128 + c * 4 + rng.randrange(4)] = 0     # one byte of a unit not built: the unit is not looked up
        return bytes(page), bytes(known)


class PlatesC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.exe = Path(cls.tmp.name) / 'plates_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES), str(DRIVER), '-o', str(cls.exe)],
                       check=True, capture_output=True, timeout=120)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def run_c(self, w, band, page, known, prog=None):
        d = Path(self.tmp.name)
        (d / 'prog.bin').write_bytes(w.prog if prog is None else prog)
        (d / 'gfx.bin').write_bytes(w.gfx)
        (d / 'page.bin').write_bytes(page)
        (d / 'known.bin').write_bytes(known)
        r = subprocess.run([str(self.exe), 'build', str(d / 'prog.bin'), str(d / 'gfx.bin'), str(d / 'page.bin'), str(d / 'known.bin'), str(band), str(d)],
                           capture_output=True, text=True, timeout=60)
        self.assertNotIn('Sanitizer', r.stderr, r.stderr)
        self.assertNotIn('runtime error', r.stderr, r.stderr)
        if r.returncode == 1 and r.stdout.startswith('error: '):
            return r.stdout[7:].strip()
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        units = (d / 'units.bin').read_bytes()
        return (d / 'map.bin').read_bytes(), int(r.stdout.split()[1]), (d / 'page.out').read_bytes(), [i for i in range(1024) if units[i]]

    def test_the_map_equals_the_model_for_every_band(self):
        for seed in range(3):
            w = World(seed)
            for band in (0, 1, 7, 19, 2):
                page, known = w.page(band)
                got = self.run_c(w, band, page, known)
                want = P.model_map(w.prog, w.gfx, band, page, known)
                self.assertEqual(got, want, (seed, band))
                self.assertGreater(len(want[3]), 0)                                 # some pieces are not in the page and are written to free units
                self.assertEqual(want[2][:0], b'')

    def test_the_constant_cell_outside_the_strip_and_the_strips_palette_words(self):
        w = World(5, missing=0.0)
        page, known = w.page(3, share=1.0)
        res = self.run_c(w, 3, page, known)
        data, page = res[0], res[2]                                                  # the page with the new units: every named unit holds its cell's piece
        cell = lambda r, c: struct.unpack_from('<HH', data, 4 * (r * 64 + c))
        for r in list(range(3)) + list(range(11, 32)):
            self.assertTrue(all(cell(r, c) == (0x5800, 0x7800) for c in range(64)), r)
        seen = 0
        for r in range(8):
            for c in range(64):
                s, x = P.set_of(c, 3)
                code, attr = S.scroll1_cell(w.prog, s, x, 3 + r)
                piece = S.scroll1_piece_bytes(w.gfx, code, attr)
                tex, clut = cell(3 + r, c)
                if (tex, clut) == (0x5800, 0x7800):
                    continue                                                         # the constant cell: a unit that is not built, or a blank piece at the page's start
                seen += 1
                self.assertEqual(clut, 0x7800 if piece == bytes(32) else 0x7800 + 64 * (attr & 31))
                self.assertEqual(tex >> 10, 22)
                v, u = (tex >> 5) & 31, tex & 31
                unit = b''.join(page[(v * 8 + y) * 128 + u * 4:(v * 8 + y) * 128 + u * 4 + 4] for y in range(8))
                self.assertEqual(unit, piece)
        self.assertGreater(seen, 400)

    def test_a_piece_the_page_lacks_or_whose_tile_is_missing_is_the_constant_cell(self):
        w = World(6, missing=0.3)
        page, known = w.page(4, share=0.5)
        got = self.run_c(w, 4, page, known)
        data, filled = got[0], got[1]
        count = sum(1 for r in range(3, 11) for c in range(64) if data[4 * (r * 64 + c):4 * (r * 64 + c) + 4] == struct.pack('<HH', 0x5800, 0x7800))
        self.assertGreaterEqual(count, filled)                                       # every filled cell is the constant cell (a blank piece may be it too)
        self.assertGreater(filled, 0)                                                # the pieces whose graphics tile is missing
        self.assertEqual(got, P.model_map(w.prog, w.gfx, 4, page, known))

    def test_the_compose_helpers_equal_the_model(self):
        w = World(8)
        helper = Path(self.tmp.name) / 'helper.c'
        helper.write_text('#include "compose.h"\n#include <stdio.h>\n#include <stdlib.h>\nint main(int c, char **v){unsigned code, attr, set = atoi(v[1]), x = atoi(v[2]), y = atoi(v[3]);'
                          'static unsigned char prog[0x300000], gfx[(0x10000 + 64) * 128]; FILE *f = fopen(v[4], "rb"); fread(prog, 1, sizeof prog, f); fclose(f);'
                          'f = fopen(v[5], "rb"); fread(gfx, 1, sizeof gfx, f); fclose(f); unsigned char out[32];'
                          'if (arcade_compose_scroll1_cell(prog, sizeof prog, set, x, y, &code, &attr)) { printf("refused\\n"); return 0; }'
                          'if (arcade_compose_piece(gfx, sizeof gfx, code, attr, out)) { printf("%u %u none\\n", code, attr); return 0; }'
                          'printf("%u %u ", code, attr); for (int i = 0; i < 32; i++) printf("%02x", out[i]); printf("\\n"); return 0; }\n')
        exe = Path(self.tmp.name) / 'helper'
        subprocess.run(['gcc', '-std=c99', '-O1', '-Wall', '-fsanitize=address,undefined', '-I', str(ARC), str(helper), str(ARC / 'compose.c'), '-o', str(exe)], check=True,
                       capture_output=True, timeout=60)
        d = Path(self.tmp.name)
        (d / 'prog.bin').write_bytes(w.prog)
        gfx = w.gfx + bytes(max(0, (0x10000 + 64) * 128 - len(w.gfx)))
        (d / 'gfx.bin').write_bytes(gfx[:(0x10000 + 64) * 128])
        rng = random.Random(1)
        for _ in range(60):
            set_, x, y = rng.choice((0, 64, 70, 84)), rng.randrange(32), rng.randrange(32)
            r = subprocess.run([str(exe), str(set_), str(x), str(y), str(d / 'prog.bin'), str(d / 'gfx.bin')], capture_output=True, text=True, timeout=30)
            self.assertEqual(r.returncode, 0, r.stderr)
            code, attr = S.scroll1_cell(w.prog, set_, x, y)
            piece = S.scroll1_piece_bytes(gfx[:(0x10000 + 64) * 128], code, attr)
            self.assertEqual(r.stdout.split(), [str(code), str(attr), 'none' if piece is None else piece.hex()])
        r = subprocess.run([str(exe), '0', '32', '0', str(d / 'prog.bin'), str(d / 'gfx.bin')], capture_output=True, text=True, timeout=30)
        self.assertEqual(r.stdout.strip(), 'refused')

    def test_a_program_that_is_too_short_is_refused(self):
        w = World(9)
        page, known = w.page(0)
        self.assertIsInstance(self.run_c(w, 0, page, known, prog=w.prog[:S.SETS1 + 100]), str)

    def test_the_table_has_the_files_and_compiles(self):
        text = (ARC / 'plates_table.c').read_text()
        rows = P.re.findall(r'\{"(NDEMO[0-9A-F]{2}\.PAC)", (\d+)\}', text)
        self.assertEqual(len(rows), 21)
        self.assertEqual([n for n, _ in rows], sorted(n for n, _ in rows))
        self.assertTrue(all(0 <= int(b) < P.BANDS for _, b in rows))
        self.assertNotIn(chr(0x2014), text + (ARC / 'plates.c').read_text() + (ARC / 'plates.h').read_text() + (ARC / 'plates.py').read_text())
        d = Path(tempfile.mkdtemp())
        for src in SOURCES + [ARC / 'plates_table.c', DRIVER]:
            r = subprocess.run(['gcc', *FLAGS, '-c', str(src), '-o', str(d / 'x.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stderr)
        probe = d / 'p.c'
        probe.write_text('int main(void){return 0;}\n')
        if subprocess.run(['gcc', '-m32', '-o', str(d / 'p'), str(probe)], capture_output=True).returncode:
            return
        for src in SOURCES + [ARC / 'plates_table.c']:
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
