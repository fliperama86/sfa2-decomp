#!/usr/bin/env python3
"""Seconds: the runtime C of the screens' tiles, palettes and pictures (port/arcade/screens.c) against the Python model (port/arcade/screens.py)
and the recipe finders, on made-up data; no game files.

Builds screens.c, tilepack.c, the generated recipe table and a driver with the address and undefined-behavior sanitizers, makes a small arcade
(graphics with transparent, flat and symmetric tiles, palette words) and made-up regions of every kind (compressed segments, plain tile lists,
palettes with flags and strides, pictures with every op), and checks that the C gives, byte for byte and known byte for known byte, what
model_chunk gives (a packed segment is compared as the tiles it decodes to), that the pixels follow an independent reading of the arcade's
bit planes, that a missing disc or a room too small leaves a segment unbuilt, and that malformed input (a code outside the graphics, a region
outside its chunk, ops outside the picture, items that do not fit) is refused by both. It also checks the finders (describe_tiles,
describe_palettes, describe_picture: the model redraws what they were given), the table writer, the checked-in table's shape, and that the C
compiles warning-free natively and for 32 bits.
"""
from pathlib import Path
import random, shutil, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import screens as S
import tilepack as T

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic')
SOURCES = [ROOT / 'port/arcade' / n for n in ('screens.c', 'screens_table.c', 'tilepack.c')]
TILES = 48                  # tiles of the made-up graphics
KINDS = {S.SEG: 'SEG', S.RAW: 'RAW', S.PALS: 'PALS', S.PIC: 'PIC'}
OPS = {S.OP_TILE: 'TILE', S.OP_PIECE: 'PIECE', S.OP_FLAT: 'FLAT', S.OP_DISC: 'DISC', S.OP_BLANK: 'BLANK'}
NAMES = {S.ZERO: 'ZERO', S.DISC: 'DISC', S.PIECES: 'PIECES'}


def arcade_bytes_of(pixels):
    """The 128 bytes of an arcade tile from a 16 x 16 list of values 0..15: row y, half h of 8 pixels, four plane bytes (plane p has bit 7 - x of the
    pixel's bit p), the way the graphics view stores them."""
    out = bytearray()
    for y in range(16):
        for h in range(2):
            for p in range(4):
                out.append(sum(((pixels[y][8 * h + x] >> p) & 1) << (7 - x) for x in range(8)))
    return bytes(out)


def ps1_bytes_of(pixels):
    """The PS1 form of a 16 x 16 tile of arcade values: colour plus one modulo 16, low nibble first."""
    return bytes(((pixels[y][x] + 1) & 15) | (((pixels[y][x + 1] + 1) & 15) << 4) for y in range(16) for x in range(0, 16, 2))


class World:
    """A small made-up arcade: tiles, palette words, and the model over them."""

    def __init__(self, seed):
        self.rnd = rnd = random.Random(seed)
        self.pix = []
        for c in range(TILES):
            kind = c % 6
            if kind == 0:
                px = [[15] * 16 for _ in range(16)]                       # transparent
            elif kind == 1:
                v = rnd.randrange(15)
                px = [[v] * 16 for _ in range(16)]                        # flat
            elif kind == 2:                                               # symmetric across
                px = [[0] * 16 for _ in range(16)]
                for y in range(16):
                    for x in range(8):
                        px[y][x] = px[y][15 - x] = rnd.randrange(16)
            else:
                px = [[rnd.randrange(16) for _ in range(16)] for _ in range(16)]
            self.pix.append(px)
        self.gfx = b''.join(arcade_bytes_of(p) for p in self.pix)
        self.prog = bytearray(rnd.randbytes(0x800))
        for k in range(4):                                                # palettes with black words, so the black rule shows
            struct.pack_into('>15H', self.prog, 0x100 + 32 * k, *[0 if (k + i) % 5 == 0 else rnd.randrange(0x10000) for i in range(15)])
        self.prog = bytes(self.prog)
        self.arc = S.Arc(self.gfx)


def spec_text(region, extra_pieces=()):
    kind, offset, size = region[:3]
    width = region[3] if kind == S.PIC else region[4] if kind == S.PALS else 0
    lines = ['region %s %d %d %d' % (KINDS[kind], offset, size, width)]
    if kind == S.PIC:
        for k, x, y, w, h, first, sx, sy, flip in region[4]:
            lines.append('op %s %d %d %d %d %d %d %d %d' % (OPS[k], x, y, w, h, first, sx, sy, flip))
    else:
        for first, count, flip in region[3]:
            lines.append('item %s %d %d' % (NAMES.get(first, first), count, flip))
        if kind in (S.SEG, S.RAW):
            for q in region[4]:
                lines.append('piece ' + ' '.join(str(v) for v in q))
    return '\n'.join(lines) + '\n'


class ScreensC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-screens-')
        d = Path(cls.dir.name)
        cls.exe = d / 'screens_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES),
                        str(ROOT / 'port/arcade/tests/screens_test.c'), '-o', str(cls.exe)], check=True, timeout=180)
        cls.world = World(7)
        (d / 'prog.bin').write_bytes(cls.world.prog)
        (d / 'tiles.bin').write_bytes(cls.world.gfx)
        cls.n = 0

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_c(self, region, disc=None, world=None):
        """(data, known, unbuilt) of the C (self.design: the units it counts blank by design), or the refusal message."""
        base = Path(self.dir.name)
        out = base / f'out{self.n}'
        ScreensC.n += 1
        out.mkdir()
        (base / 'spec.txt').write_text(spec_text(region))
        args = [str(self.exe), 'build', str(base / 'prog.bin'), str(base / 'tiles.bin'), str(base / 'spec.txt'), str(out)]
        if disc is not None:
            (base / 'disc.bin').write_bytes(disc)
            args.append(str(base / 'disc.bin'))
        r = subprocess.run(args, capture_output=True, text=True, timeout=120)
        if r.returncode == 1 and r.stdout.startswith('error: '):
            return r.stdout[7:].strip()
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        lines = r.stdout.splitlines()
        self.verify = lines[1] if len(lines) > 1 else None
        self.design = int(lines[0].split()[3])
        return (out / 'out.bin').read_bytes(), (out / 'out.known').read_bytes(), int(lines[0].split()[1])

    def model(self, region, disc=None):
        return S.model_chunk(self.world.arc, self.world.prog, (0, 0, 0, region[1] + region[2], [region]), disc)

    def same(self, region, disc=None):
        """The C equals the model on a region (a segment as the tiles it decodes to); returns (C result, model result)."""
        got = self.run_c(region, disc)
        want = self.model(region, disc)
        self.assertIsInstance(got, tuple, got)
        self.assertEqual(self.design, S.units_of([region]), 'the units blank by design')
        data, known, unbuilt = got
        mdata, mknown, msegs = want
        if region[0] == S.SEG:
            off, size = region[1], region[2]
            if mknown[off] == 2 and unbuilt == 0:
                self.assertEqual(known[off:off + size], b'\2' * size)
                self.assertEqual(S.segment_tiles_of(data[off:off + size]), msegs[off])
            else:
                self.assertEqual(known[off:off + size], bytes(size))
                self.assertEqual(unbuilt, 1)
            self.assertEqual(data[:off], mdata[:off])
        else:
            self.assertEqual(data, mdata)
            self.assertEqual(known, mknown)
            self.assertEqual(unbuilt, 0)
        return got, want

    # ---- the pixels

    def test_tile_pixels_follow_the_bit_planes(self):
        w = self.world
        for code in (3, 7, 8, 13):
            expect = ps1_bytes_of(w.pix[code])
            self.assertEqual(w.arc.tile(code), expect)
            region = (S.RAW, 0, 128, [(code, 1, 0)], [])
            self.assertEqual(self.run_c(region)[0], expect)
            for flip in (1, 2, 3):
                px = [[w.pix[code][15 - y if flip & 2 else y][15 - x if flip & 1 else x] for x in range(16)] for y in range(16)]
                self.assertEqual(self.run_c((S.RAW, 0, 128, [(code, 1, flip)], []))[0], ps1_bytes_of(px), flip)

    def test_a_transparent_arcade_tile_is_zero(self):
        self.assertEqual(self.run_c((S.RAW, 0, 128, [(0, 1, 0)], []))[0], bytes(128))

    def test_pieces_are_halves_of_tiles(self):
        w = self.world
        # piece k = left half of tile k >> 1, top rows for even k, bottom for odd
        for k in (6, 7, 14, 15):
            px = w.pix[k >> 1]
            rows = range((k & 1) * 8, (k & 1) * 8 + 8)
            want = bytes(((px[y][x] + 1) & 15) | (((px[y][x + 1] + 1) & 15) << 4) for y in rows for x in range(0, 8, 2))
            self.assertEqual(w.arc.piece(k), want)
            region = (S.PIC, 0, 16 * 8, 16, [(S.OP_PIECE, 0, 0, 1, 1, k, 0, 0, 0)])
            data = self.run_c(region)[0]
            self.assertEqual(b''.join(data[y * 8:y * 8 + 4] for y in range(8)), want)
            self.assertEqual(b''.join(data[y * 8 + 4:y * 8 + 8] for y in range(8)), bytes(32))

    def test_a_tile_of_four_pieces(self):
        pieces = [(14 << 2 | 0, 15 << 2 | 1, 12 << 2 | 2, 13 << 2 | 3)]
        region = (S.RAW, 0, 256, [(S.PIECES, 1, 0), (S.PIECES, 1, 0)], pieces)
        got, want = self.same(region)
        t = got[0][:128]
        self.assertEqual(got[0][128:], t)
        self.assertEqual(S.tile_of_pieces([S.piece_of(self.world.arc, p) for p in pieces[0]]), t)
        flat = [(S.FLAT_BASE | 5, S.FLAT_BASE | 0, S.FLAT_BASE | 15, S.FLAT_BASE | 1)]
        got, _ = self.same((S.RAW, 0, 128, [(S.PIECES, 1, 0)], flat))
        self.assertEqual(got[0][0], 0x55)
        self.assertEqual(got[0][4], 0x00)
        self.assertEqual(got[0][8 * 8], 0xff)

    # ---- tile lists

    def test_raw_tiles_items_and_known(self):
        items = [(S.ZERO, 2, 0), (3, 2, 0), (S.DISC, 1, 0), (9, 1, 3), (20, 3, 1)]
        region = (S.RAW, 64, 9 * 128 + 50, items, [])
        got, _ = self.same(region)
        data, known, _u = got
        self.assertEqual(known[:64], bytes(64))
        self.assertEqual(known[64:64 + 128 * 2], b'\1' * 256)
        self.assertEqual(known[64 + 4 * 128:64 + 5 * 128], bytes(128))          # the DISC tile stays unknown
        self.assertEqual(known[64 + 9 * 128:], bytes(50))

    def test_random_tile_lists(self):
        rnd = random.Random(3)
        for _ in range(12):
            items, pieces, total = [], [], 0
            for _ in range(rnd.randrange(1, 8)):
                r = rnd.randrange(5)
                if r == 0:
                    items.append((S.ZERO, rnd.randrange(1, 4), 0)); total += items[-1][1]
                elif r == 1:
                    items.append((S.DISC, rnd.randrange(1, 4), 0)); total += items[-1][1]
                elif r == 2:
                    pieces.append(tuple(rnd.choice([S.FLAT_BASE | rnd.randrange(16), rnd.randrange(TILES * 2) << 2 | rnd.randrange(4)]) for _ in range(4)))
                    items.append((S.PIECES, 1, len(pieces) - 1)); total += 1
                else:
                    n = rnd.randrange(1, 5)
                    items.append((rnd.randrange(TILES - n), n, rnd.randrange(4))); total += n
            self.same((S.RAW, 0, total * 128, items, pieces))

    # ---- segments

    def seg_region(self, items, room=None, offset=0x40, pieces=()):
        total = sum(c for _f, c, _fl in items)
        return (S.SEG, offset, room if room else 4 * total + 130 * total, items, list(pieces))

    def test_a_segment_is_packed_and_decodes_to_the_tiles(self):
        items = [(S.ZERO, 1, 0), (3, 5, 0), (9, 4, 2), (S.ZERO, 2, 0), (20, 7, 1), (S.PIECES, 1, 0)]
        region = self.seg_region(items, pieces=[(14 << 2, 15 << 2 | 1, 12 << 2 | 2, S.FLAT_BASE | 3)])
        got, want = self.same(region)
        self.assertEqual(len(want[2][0x40]), 20)
        self.assertEqual(got[2], 0)

    def test_a_segment_needs_the_disc_for_its_missing_tiles(self):
        items = [(S.ZERO, 1, 0), (3, 3, 0), (S.DISC, 2, 0), (9, 2, 0)]
        region = self.seg_region(items)
        w = self.world
        own = [bytes(random.Random(i).randrange(256) for _ in range(128)) for i in range(2)]
        tiles = [bytes(128), w.arc.tile(3), w.arc.tile(4), w.arc.tile(5), own[0], own[1], w.arc.tile(9), w.arc.tile(10)]
        packed = T.encode_chunk_small([T.tile_words(t) for t in tiles])[0]
        disc = bytes(0x40) + packed + bytes(100)
        region = (S.SEG, 0x40, len(packed) + 100, items, [])
        got, want = self.same(region, disc)
        self.assertEqual(S.segment_tiles_of(got[0][0x40:0x40 + region[2]]), tiles)
        # without the disc, or with a disc whose segment has another tile count, the segment stays unbuilt
        data, known, unbuilt = self.run_c(region)
        self.assertEqual((unbuilt, known), (1, bytes(len(known))))
        other = T.encode_chunk_small([T.tile_words(t) for t in tiles[:5]])[0]
        data, known, unbuilt = self.run_c(region, bytes(0x40) + other + bytes(400))
        self.assertEqual((unbuilt, known), (1, bytes(len(known))))
        self.assertEqual(self.model(region, bytes(0x40) + other + bytes(400))[1][0x40], 0)

    def test_a_segment_that_does_not_fit_its_room_is_left_to_the_disc(self):
        items = [(S.ZERO, 1, 0)] + [(c, 1, 0) for c in range(3, 30)]
        region = self.seg_region(items, room=200)
        data, known, unbuilt = self.run_c(region)
        self.assertEqual((unbuilt, known, data), (1, bytes(len(known)), bytes(len(data))))

    def test_the_packed_segment_never_passes_its_room(self):
        items = [(S.ZERO, 1, 0)] + [(c, 1, c & 3) for c in range(3, 40)]
        chunk = T.encode_chunk_small([T.tile_words(self.world.arc.tile(c, c & 3)) for c in range(3, 40)])[0]
        region = self.seg_region(items, room=len(chunk) + 64, offset=0x10)
        data, known, unbuilt = self.run_c(region)
        self.assertEqual(unbuilt, 0)
        room = region[2]
        self.assertEqual(known[0x10:0x10 + room], b'\2' * room)
        self.assertEqual(data[:0x10], bytes(0x10))
        self.assertEqual(len(data), 0x10 + room)

    def test_verify_checks_bytes_and_packed_tiles_against_the_disc(self):
        w = self.world
        region = (S.RAW, 16, 3 * 128, [(3, 2, 0), (S.DISC, 1, 0)], [])
        disc = bytearray(16) + w.arc.tile(3) + w.arc.tile(4) + bytes(range(128))
        self.run_c(region, bytes(disc))
        self.assertEqual(self.verify, 'verify ok')
        disc[16 + 128 + 5] ^= 1
        self.run_c(region, bytes(disc))
        self.assertTrue(self.verify.startswith('verify: byte 0x'), self.verify)
        self.assertIn('0x95', self.verify)                              # offset 16 + 128 + 5
        self.run_c(region, bytes(disc)[:100])
        self.assertTrue(self.verify.startswith('verify: the disc'), self.verify)
        items = [(S.ZERO, 1, 0), (3, 3, 0), (S.DISC, 2, 0), (9, 2, 0)]
        own = [bytes(random.Random(i).randrange(256) for _ in range(128)) for i in range(2)]
        tiles = [bytes(128), w.arc.tile(3), w.arc.tile(4), w.arc.tile(5), own[0], own[1], w.arc.tile(9), w.arc.tile(10)]
        packed = T.encode_chunk_small([T.tile_words(t) for t in tiles])[0]
        region = (S.SEG, 0x40, len(packed) + 100, items, [])
        self.run_c(region, bytes(0x40) + packed + bytes(100))
        self.assertEqual(self.verify, 'verify ok')
        tiles[6] = w.arc.tile(9, 1)                                      # a disc whose tile is not the arcade's
        other = T.encode_chunk_small([T.tile_words(t) for t in tiles])[0]
        self.run_c(region, bytes(0x40) + other + bytes(100 + len(packed) - len(other)))
        self.assertTrue(self.verify.startswith('verify: tile 6 of the segment'), self.verify)

    # ---- palettes

    def test_palettes_flags_strides_zero_and_disc(self):
        w = self.world
        items = [(0x100, 2, 0), (S.ZERO, 1, 0), (0x100, 1, S.PAL_BLACK_ZERO), (0x120, 1, S.PAL_BIT15), (S.DISC, 1, 0), (0x100, 1, 3)]
        for stride in (1, 4):
            region = (S.PALS, 32, 32 * stride * 7 + 32, items, stride)
            got, _ = self.same(region)
            data, known, _u = got
            for i, state in enumerate([1, 1, 1, 1, 1, 0, 1]):
                a = 32 + 32 * stride * i
                self.assertEqual(known[a:a + 32], bytes([state]) * 32, (stride, i))
            if stride == 4:
                self.assertEqual(known[32 + 32:32 + 128], b'\1' * 96)
        # palette 0 of the first item: colour 0 is 0, the words' channels doubled, black (a zero word) is 0x8000 unless the flag says otherwise
        pal = struct.unpack('<16H', S.palette_bytes(struct.unpack_from('>15H', w.prog, 0x100), 0))
        self.assertEqual(pal[0], 0)
        words = struct.unpack_from('>15H', w.prog, 0x100)
        self.assertEqual(words[0], 0)
        self.assertEqual(pal[1], 0x8000)
        self.assertEqual(struct.unpack('<16H', S.palette_bytes(words, S.PAL_BLACK_ZERO))[1], 0)
        self.assertEqual(struct.unpack('<16H', S.palette_bytes(words, S.PAL_BIT15))[2], pal[2] | 0x8000)
        w2 = words[1]
        self.assertEqual(pal[2], (w2 >> 8 & 15) << 1 | (w2 >> 4 & 15) << 6 | (w2 & 15) << 11)
        data = self.run_c((S.PALS, 0, 32, [(0x100, 1, 0)], 1))[0]
        self.assertEqual(data, S.palette_bytes(words, 0))

    # ---- pictures

    def pic(self, ops, width=64, rows=64):
        return (S.PIC, 16, width // 2 * rows, width, ops)

    def test_picture_ops_equal_the_model(self):
        ops = [(S.OP_TILE, 0, 0, 2, 2, 3, 1, 4, 0), (S.OP_TILE, 4, 0, 1, 1, 9, 0, 0, 3), (S.OP_PIECE, 0, 4, 3, 2, 6, 2, 1, 1),
               (S.OP_FLAT, 3, 6, 2, 1, 5, 0, 0, 0), (S.OP_PIECE, 5, 5, 2, 2, 20, 32, 1, 2), (S.OP_DISC, 4, 4, 1, 2, 0, 0, 0, 0)]
        got, _ = self.same(self.pic(ops))
        data, known, _u = got
        row = 32
        # the DISC rect is zero and unknown; the rest is known
        self.assertEqual(known[16 + 32 * row + 16:16 + 32 * row + 20], bytes(4))
        self.assertEqual(known[16 + 36 * row + 16:16 + 36 * row + 20], bytes(4))
        self.assertEqual(known[:16], bytes(16))
        # the tile at (0, 0) is arcade tile 3 and the next one, 16 pixels right, tile 4 (x step 1)
        def block(x, y, side):
            return b''.join(data[16 + (y + j) * row + x // 2:16 + (y + j) * row + (x + side) // 2] for j in range(side))
        self.assertEqual(block(0, 0, 16), self.world.arc.tile(3))
        self.assertEqual(block(16, 0, 16), self.world.arc.tile(4))
        self.assertEqual(block(0, 16, 16), self.world.arc.tile(3 + 4))              # y step 4
        self.assertEqual(block(32, 0, 16), self.world.arc.tile(9, 3))
        self.assertEqual(block(24, 48, 8)[:32], bytes([0x55]) * 32)                 # flat 5 (the op at x 3, y 6 in units of 8)

    def test_random_pictures(self):
        rnd = random.Random(9)
        for _ in range(15):
            ops = []
            for _ in range(rnd.randrange(1, 7)):
                kind = rnd.choice([S.OP_TILE, S.OP_PIECE, S.OP_FLAT, S.OP_DISC])
                w, h = rnd.randrange(1, 4), rnd.randrange(1, 3)
                unit = 2 if kind == S.OP_TILE else 1
                x, y = rnd.randrange(0, 8 - unit * w + 1), rnd.randrange(0, 8 - unit * h + 1)
                first = rnd.randrange(15) if kind == S.OP_FLAT else rnd.randrange(TILES // 2) if kind == S.OP_TILE else rnd.randrange(TILES)
                ops.append((kind, x, y, w, h, first, rnd.choice([0, 1, 2, -1]), rnd.choice([0, 1, 3]), rnd.randrange(4)))
            region = self.pic(ops)
            try:
                self.model(region)
            except S.ModelError:
                self.assertIsInstance(self.run_c(region), str)
                continue
            self.same(region)

    # ---- blank by design

    def test_by_design_tiles_are_blank_known_2_and_counted(self):
        w = self.world
        items = [(3, 2, 0), (S.ZERO, 2, S.BY_DESIGN), (S.ZERO, 1, 0), (9, 1, 1), (S.ZERO, 1, S.BY_DESIGN)]
        got, _ = self.same((S.RAW, 16, 7 * 128, items, []))
        data, known, _u = got
        self.assertEqual(self.design, 3)
        for i, state in enumerate([1, 1, 2, 2, 1, 1, 2]):
            self.assertEqual(known[16 + 128 * i:16 + 128 * i + 128], bytes([state]) * 128, i)
        self.assertEqual(data[16 + 2 * 128:16 + 4 * 128], bytes(256))
        # a segment with no DISC tile left is built whole with no disc, and the verify skips what is by design but not the rest
        items = [(S.ZERO, 1, 0), (3, 3, 0), (S.ZERO, 2, S.BY_DESIGN), (9, 2, 0)]
        region = self.seg_region(items)
        got, want = self.same(region)
        self.assertEqual((got[2], self.design), (0, 2))
        tiles = [bytes(128), w.arc.tile(3), w.arc.tile(4), w.arc.tile(5), bytes(128), bytes(128), w.arc.tile(9), w.arc.tile(10)]
        self.assertEqual(want[2][0x40], tiles)
        own = [bytes(random.Random(i).randrange(256) for _ in range(128)) for i in range(2)]
        on_disc = list(tiles)
        on_disc[4:6] = own                                              # the disc has art where the build is blank by design
        packed = T.encode_chunk_small([T.tile_words(t) for t in on_disc])[0]
        disc = bytes(0x40) + packed + bytes(region[2] - len(packed))
        self.run_c(region, disc)
        self.assertEqual(self.verify, 'verify ok')
        on_disc[6] = w.arc.tile(9, 1)                                   # and a tile that is not by design must still agree
        packed = T.encode_chunk_small([T.tile_words(t) for t in on_disc])[0]
        self.run_c(region, bytes(0x40) + packed + bytes(region[2] - len(packed)))
        self.assertTrue(self.verify.startswith('verify: tile 6 of the segment'), self.verify)

    def test_by_design_palettes_and_picture_rects(self):
        items = [(0x100, 1, 0), (S.ZERO, 2, S.BY_DESIGN), (S.DISC, 1, 0), (S.ZERO, 1, 0)]
        got, _ = self.same((S.PALS, 32, 32 * 5, items, 1))
        known = got[1]
        for i, state in enumerate([1, 2, 2, 0, 1]):
            self.assertEqual(known[32 + 32 * i:64 + 32 * i], bytes([state]) * 32, i)
        self.assertEqual(self.design, 2)
        ops = [(S.OP_TILE, 0, 0, 2, 2, 3, 1, 4, 0), (S.OP_BLANK, 1, 1, 2, 3, 0, 0, 0, 0), (S.OP_DISC, 5, 6, 2, 1, 0, 0, 0, 0), (S.OP_FLAT, 0, 5, 2, 1, 5, 0, 0, 0)]
        got, _ = self.same(self.pic(ops))
        data, known, _u = got
        row = 32
        self.assertEqual(self.design, 6)
        self.assertEqual(known[16 + 8 * row + 4:16 + 8 * row + 12], b'\2' * 8)           # the blank rect, over the tile that was drawn there
        self.assertEqual(data[16 + 8 * row + 4:16 + 8 * row + 12], bytes(8))
        self.assertEqual(known[16 + 48 * row + 20:16 + 48 * row + 28], bytes(8))        # the DISC rect stays unknown
        self.assertEqual(known[16 + 40 * row:16 + 40 * row + 8], b'\1' * 8)
        # verify: bytes known 2 outside a segment are not compared with the disc, bytes known 1 are
        disc = bytes(16) + (bytes(range(1, 256)) * 9)[:32 * 64]
        self.run_c(self.pic([(S.OP_BLANK, 0, 0, 8, 8, 0, 0, 0, 0)]), disc)
        self.assertEqual(self.verify, 'verify ok')

    def test_the_by_design_mark_is_for_blank_items_only(self):
        for first in (3, S.DISC, S.PIECES):
            region = (S.RAW, 0, 128, [(first, 1, S.BY_DESIGN | (0 if first != S.PIECES else 0))], [(14 << 2,) * 4])
            with self.subTest(first=first):
                with self.assertRaises(S.ModelError):
                    self.model(region)
                self.assertIsInstance(self.run_c(region), str)
        pal = (S.PALS, 0, 32, [(0x100, 1, S.BY_DESIGN | S.PAL_BIT15)], 1)
        with self.assertRaises(S.ModelError):
            self.model(pal)
        self.assertIsInstance(self.run_c(pal), str)

    # ---- malformed input

    def test_malformed_input_is_refused_by_both(self):
        n = TILES
        cases = {
            'tile outside the graphics': (S.RAW, 0, 128, [(n, 1, 0)], []),
            'run past the graphics': (S.RAW, 0, 512, [(n - 2, 3, 0)], []),
            'raw tiles longer than the region': (S.RAW, 0, 128, [(3, 2, 0)], []),
            'palette outside the program': (S.PALS, 0, 64, [(len(self.world.prog) - 20, 1, 0)], 1),
            'palettes longer than the region': (S.PALS, 0, 32, [(0x100, 2, 0)], 1),
            'picture op outside the picture': self.pic([(S.OP_TILE, 7, 0, 1, 1, 3, 1, 1, 0)]),
            'picture rows outside': self.pic([(S.OP_PIECE, 0, 8, 1, 1, 3, 0, 0, 0)]),
            'piece outside the graphics': self.pic([(S.OP_PIECE, 0, 0, 1, 1, 2 * n, 0, 0, 0)]),
            'tile of a rect outside the graphics': self.pic([(S.OP_TILE, 0, 0, 2, 1, n - 1, 1, 0, 0)]),
            'picture width that does not divide': (S.PIC, 0, 100, 48, []),
            'picture of zero width': (S.PIC, 0, 128, 0, []),
            'unknown piece quad': (S.RAW, 0, 128, [(S.PIECES, 1, 5)], []),
        }
        for name, region in cases.items():
            with self.subTest(name):
                with self.assertRaises(S.ModelError):
                    self.model(region)
                got = self.run_c(region)
                self.assertIsInstance(got, str, f'{name}: the C built it')
        self.same((S.RAW, 0, 128, [(3, 1, 0)], []))        # still fine after the refusals (the sanitizers would say otherwise)

    def test_the_generated_table_is_sane(self):
        r = subprocess.run([str(self.exe), 'table'], capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout)
        self.assertIn(': ok', r.stdout)

    def test_every_recipe_of_the_table_builds_with_a_blank_arcade(self):
        r = subprocess.run([str(self.exe), 'all'], capture_output=True, text=True, timeout=300)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn(' 0 refused', r.stdout)

    def test_compiles_warning_free_for_32_bits(self):
        d = Path(self.dir.name)
        probe = d / 'probe.c'
        probe.write_text('int main(void){return 0;}\n')
        if subprocess.run(['gcc', '-m32', '-o', str(d / 'probe'), str(probe)], capture_output=True).returncode:
            self.skipTest('no 32-bit toolchain')
        for src in SOURCES:
            r = subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / 'x32.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stderr)

    def test_native_build_has_no_warnings(self):
        d = Path(self.dir.name)
        for src in SOURCES:
            r = subprocess.run(['gcc', *FLAGS, '-O2', '-c', str(src), '-o', str(d / 'x.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual((r.returncode, r.stderr), (0, ''))


class Finders(unittest.TestCase):
    """The recipe finders against the model: what they describe, the model redraws."""

    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-screens-find-')
        cls.world = World(21)
        cls.cand = S.Candidates(cls.dir.name, cls.world.gfx)

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def test_tiles_become_runs_zero_pieces_and_disc(self):
        w, arc = self.world, self.world.arc
        own = bytes(random.Random(1).randrange(1, 256) for _ in range(128))
        parts = [S.piece_of(arc, 14 << 2), S.piece_of(arc, 15 << 2 | 1), S.flat_piece(4), S.piece_of(arc, 17 << 2 | 3)]
        tiles = [bytes(128), arc.tile(3), arc.tile(4), arc.tile(5), arc.tile(9, 2), bytes(128), bytes(128), own, S.tile_of_pieces(parts), arc.tile(11, 1), own]
        items, pieces = S.describe_tiles(tiles, arc, self.cand)
        got = S.tile_items(arc, items, pieces)
        self.assertEqual([t for t in got if t is not None], [t for t, g in zip(tiles, got) if g is not None])
        self.assertEqual([i for i, g in enumerate(got) if g is None], [7, 10])
        self.assertEqual(items[0], (S.ZERO, 1, 0))
        self.assertEqual(items[1], (3, 3, 0))                      # one run of three consecutive tiles
        self.assertIn((S.ZERO, 2, 0), items)
        self.assertEqual(sum(1 for f, _c, _fl in items if f == S.PIECES), 1)
        self.assertEqual(len(pieces), 1)
        self.assertEqual(pieces[0][2], S.FLAT_BASE | 4)

    def test_palettes_become_runs_with_flags(self):
        w = self.world
        index = S.PaletteIndex(w.prog)
        words = [struct.unpack_from('>15H', w.prog, 0x100 + 32 * k) for k in range(4)]
        rows = [S.palette_bytes(words[0], 0), S.palette_bytes(words[1], 0), bytes(32), S.palette_bytes(words[2], S.PAL_BLACK_ZERO),
                S.palette_bytes(words[3], S.PAL_BIT15), bytes([1]) * 32]
        items = S.describe_palettes(rows, w.prog, index)
        self.assertEqual(items[0], (0x100, 2, 0))
        self.assertEqual(items[1], (S.ZERO, 1, 0))
        self.assertEqual(items[-1], (S.DISC, 1, 0))
        got = S.palette_items(w.prog, items)
        self.assertEqual([g for g in got if g], [r for r, g in zip(rows, got) if g])
        self.assertEqual(sum(1 for g in got if g is None), 1)

    def test_a_picture_is_redrawn_from_its_ops(self):
        arc = self.world.arc
        width, rows = 64, 64
        img = bytearray(width // 2 * rows)
        S.blit(img, width // 2, 0, 0, arc.tile(3), 16)
        S.blit(img, width // 2, 16, 0, arc.tile(4), 16)
        S.blit(img, width // 2, 0, 16, arc.tile(19), 16)                    # +16 down from tile 3
        S.blit(img, width // 2, 16, 16, arc.tile(20), 16)
        S.blit(img, width // 2, 40, 8, arc.piece(10), 8)
        S.blit(img, width // 2, 40, 16, arc.piece(11), 8)                   # k + 1 down
        S.blit(img, width // 2, 48, 8, arc.piece(14), 8)                    # k + 4 right
        S.blit(img, width // 2, 56, 40, S.flat_piece(7), 8)
        S.blit(img, width // 2, 16, 40, bytes(random.Random(2).randrange(1, 256) for _ in range(32)), 8)       # the PS1's own
        ops = S.describe_picture(bytes(img), width, arc, self.cand)
        data, known = S.picture_ops(arc, ops, width, len(img))
        self.assertTrue(all(a == b for a, b, k in zip(data, img, known) if k))
        kinds = [o[0] for o in ops]
        self.assertEqual(kinds.count(S.OP_DISC), 1)
        self.assertEqual(sum(1 for o in ops if o[0] == S.OP_TILE and o[3] * o[4] == 4), 1)      # one rect of 2 by 2: tiles 3, 4, 19 and 20
        self.assertEqual(sum(1 for k in known if not k), 32)                  # only the own piece is unknown
        built, disc, nonblank = S.picture_stats(bytes(img), width, ops)
        self.assertEqual(disc, 1)
        self.assertEqual(built + disc, nonblank)

    def test_candidates_find_flips_and_pieces(self):
        arc = self.world.arc
        for code, flip in ((3, 0), (4, 1), (9, 2), (10, 3)):
            ids = self.cand.c16(arc.tile(code, flip))
            self.assertTrue(any(i >> 2 == code and i & 3 == flip for i in ids) or ids, (code, flip))
            for i in ids:
                self.assertEqual(arc.tile(i >> 2, i & 3), arc.tile(code, flip))
        for k, flip in ((7, 0), (8, 2), (13, 1)):
            ids = self.cand.c8(arc.piece(k, flip))
            self.assertTrue(ids)
            for i in ids:
                self.assertEqual(arc.piece(i >> 2, i & 3), arc.piece(k, flip))
        self.assertEqual(self.cand.c16(bytes([1]) * 128), [])


class TableText(unittest.TestCase):
    def table(self):
        t = S.Table()
        quad = (14 << 2, 15 << 2 | 1, S.FLAT_BASE | 2, 3 << 2)
        r1 = t.add_recipe(1000, [(S.SEG, 0, 600, [(S.ZERO, 1, 0), (3, 4, 1), (S.PIECES, 1, 0), (S.DISC, 2, 0)], [quad]),
                                 (S.PALS, 600, 64, [(0x100, 1, 3), (S.DISC, 1, 0)], 1)])
        r2 = t.add_recipe(512, [(S.PIC, 0, 512, 64, [(S.OP_TILE, 0, 0, 2, 1, 3, 1, 16, 0), (S.OP_DISC, 4, 0, 2, 1, 0, 0, 0, 0)])])
        self.assertEqual(t.add_recipe(1000, [(S.SEG, 0, 600, [(S.ZERO, 1, 0), (3, 4, 1), (S.PIECES, 1, 0), (S.DISC, 2, 0)], [quad]),
                                             (S.PALS, 600, 64, [(0x100, 1, 3), (S.DISC, 1, 0)], 1)]), r1)       # equal recipes are one
        t.add_use('A00.PAC', 0, 0x18, 0, r1)
        t.add_use('A00.PAC', 2, 0, 0, r2)
        t.add_use('B.PAC', 0, 0x18, 0, r1)
        return t, (r1, r2)

    def test_table_interns_and_models(self):
        t, (r1, r2) = self.table()
        self.assertEqual((len(t.recipes), len(t.pieces)), (2, 1))
        size, regions = t.model_regions(r1)
        self.assertEqual(size, 1000)
        self.assertEqual(regions[0][3], [(S.ZERO, 1, 0), (3, 4, 1), (S.PIECES, 1, 0), (S.DISC, 2, 0)])
        self.assertEqual(regions[0][4], [(14 << 2, 15 << 2 | 1, S.FLAT_BASE | 2, 3 << 2)])
        size, regions = t.model_regions(r2)
        self.assertEqual(regions[0][3:], (64, [(S.OP_TILE, 0, 0, 2, 1, 3, 1, 16, 0), (S.OP_DISC, 4, 0, 2, 1, 0, 0, 0, 0)]))

    def test_table_text_compiles_and_looks_up(self):
        t, _ = self.table()
        text = S.c_table(t, 'python3 port/arcade/other_files.py --c-table')
        self.assertNotIn(chr(0x2014), text)
        self.assertIn('{"A00.PAC", 0, 2}', text)
        self.assertIn('{"B.PAC", 2, 1}', text)
        self.assertIn('OD, 0}', text)
        d = Path(tempfile.mkdtemp(prefix='arcade-screens-table-'))
        try:
            (d / 't.c').write_text(text)
            (d / 'main.c').write_text('#include "screens.h"\n#include <stdio.h>\nint main(void){const struct arcade_screens_file*f=arcade_screens_find("B.PAC");'
                                      'const struct arcade_screens_recipe*r=arcade_screens_recipe_of(f,0,0x18,0);'
                                      'printf("%u %u %u %d\\n",r->size,r->count,arcade_screens_piece_count,arcade_screens_recipe_of(f,2,0,0)==0);return 0;}\n')
            r = subprocess.run(['gcc', *FLAGS, '-I', str(ROOT / 'port/arcade'), str(d / 't.c'), str(d / 'main.c'), str(ROOT / 'port/arcade/screens.c'),
                                str(ROOT / 'port/arcade/tilepack.c'), '-o', str(d / 't')], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stderr)
            self.assertEqual(subprocess.run([str(d / 't')], capture_output=True, text=True).stdout.split(), ['1000', '2', '1', '1'])
        finally:
            shutil.rmtree(d)


class DesignRows(unittest.TestCase):
    """The explicit by-design table: what it turns into blank units, and what it refuses."""

    def regions(self):
        ops = [(S.OP_TILE, 0, 0, 2, 2, 3, 1, 4, 0), (S.OP_DISC, 5, 6, 2, 1, 0, 0, 0, 0), (S.OP_DISC, 0, 40, 1, 1, 0, 0, 0, 0), (S.OP_FLAT, 2, 4, 2, 1, 5, 0, 0, 0)]
        return [(S.SEG, 0, 600, [(S.ZERO, 1, 0), (3, 4, 1), (S.DISC, 2, 0), (S.DISC, 3, 0)], []), (S.PIC, 600, 64 * 64, 128, ops)]

    def run_rows(self, rows, kind='END', slot=0x29, regions=None):
        old = S.DESIGN_ROWS
        S.DESIGN_ROWS = rows
        try:
            used = []
            return S.apply_design(regions or self.regions(), kind, 0, slot, 0, used), used
        finally:
            S.DESIGN_ROWS = old

    def test_disc_units_of_a_segment_become_blank_by_design(self):
        out, used = self.run_rows((dict(kind='END', table=0, slot=0x29, ordinal=0, offset=0, units='disc'),))
        self.assertEqual(out[0][3], [(S.ZERO, 1, 0), (3, 4, 1), (S.ZERO, 2, S.BY_DESIGN), (S.ZERO, 3, S.BY_DESIGN)])
        self.assertEqual(out[1], self.regions()[1])
        self.assertEqual(used, [(0, 5)])
        self.assertEqual(S.units_of(out), 5)

    def test_a_row_names_its_kind_chunk_and_region(self):
        for kind, slot in (('CONT', 0x29), ('END', 0x2a)):
            out, used = self.run_rows((dict(kind='END', table=0, slot=0x29, ordinal=0, offset=0, units='disc'),), kind, slot)
            self.assertEqual((out, used), (self.regions(), []))
        out, used = self.run_rows((dict(kind='END', table=0, slot=0x29, ordinal=0, offset=600, units='disc', rows=(0, 128)),))
        self.assertEqual(out[0], self.regions()[0])                  # the region at offset 600 is the picture; the segment is untouched

    def test_picture_rows_blank_the_disc_rects_or_everything_inside(self):
        row = dict(kind='END', table=0, slot=0x29, ordinal=0, offset=600, units='disc', rows=(0, 128))
        out, used = self.run_rows((row,))
        ops = out[1][4]
        self.assertEqual(ops[1], (S.OP_BLANK, 5, 6, 2, 1, 0, 0, 0, 0))          # inside the rows: blanked in place
        self.assertEqual(ops[2], (S.OP_DISC, 0, 40, 1, 1, 0, 0, 0, 0))          # below them: the disc's
        self.assertEqual(used, [(0, 2)])
        out, used = self.run_rows((dict(row, units='all', rows=(0, 128)),))
        ops = out[1][4]
        self.assertEqual([o[0] for o in ops], [S.OP_BLANK, S.OP_DISC])           # the tile, the flat and the DISC rect inside are one blank rect
        self.assertEqual(ops[0], (S.OP_BLANK, 0, 0, 16, 16, 0, 0, 0, 0))
        self.assertEqual(used, [(0, 256)])
        with self.assertRaises(S.ModelError):                                    # an op that crosses the rows' edge is not guessed at
            self.run_rows((dict(row, units='all', rows=(0, 12)),))
        with self.assertRaises(S.ModelError):
            self.run_rows((dict(row, offset=0, units='all'),))                   # 'all' is for a picture

    def test_the_checked_in_rows_are_explicit(self):
        for row in S.DESIGN_ROWS:
            self.assertEqual(set(row) - {'rows'}, {'kind', 'table', 'slot', 'ordinal', 'offset', 'units', 'expect', 'why'})
            self.assertIn(row['units'], ('disc', 'all'))
            self.assertGreater(row['expect'], 0)
            self.assertGreater(len(row['why']), 40)
            self.assertNotIn(chr(0x2014), row['why'])

    def test_the_table_text_writes_the_mark_and_the_op(self):
        t = S.Table()
        t.add_use('A00.PAC', 0, 0x29, 0, t.add_recipe(1000, [(S.SEG, 0, 600, [(S.ZERO, 3, S.BY_DESIGN), (3, 1, 0)], []),
                                                           (S.PIC, 600, 512, 64, [(S.OP_BLANK, 0, 0, 2, 2, 0, 0, 0, 0)])]))
        text = S.c_table(t, 'cmd')
        self.assertIn('{Z, 3, BD}', text)
        self.assertIn('OB, 0}', text)
        self.assertEqual(S.units_of(t.model_regions(0)[1]), 7)


class Small(unittest.TestCase):
    def test_flip_pixels(self):
        data = bytes(range(8))                                   # 4 x 4, 2 bytes a row
        self.assertEqual(S.flip_pixels(data, 4, 0), data)
        px = [b >> (4 * i) & 15 for b in data for i in (0, 1)]
        across = S.flip_pixels(data, 4, 1)
        down = S.flip_pixels(data, 4, 2)
        both = S.flip_pixels(data, 4, 3)
        unpack = lambda d: [b >> (4 * i) & 15 for b in d for i in (0, 1)]
        self.assertEqual(unpack(across), [px[y * 4 + 3 - x] for y in range(4) for x in range(4)])
        self.assertEqual(unpack(down), [px[(3 - y) * 4 + x] for y in range(4) for x in range(4)])
        self.assertEqual(S.flip_pixels(across, 4, 2), both)

    def test_tilepack_tile_and_count(self):
        tiles = [bytes([i]) * 128 for i in range(1, 6)]
        chunk, _ = T.encode_chunk_small([T.tile_words(t) for t in tiles])
        self.assertEqual(S.tilepack_count(chunk), 5)
        self.assertEqual(S.segment_tiles_of(chunk), tiles)
        self.assertEqual(S.tilepack_tile(chunk, 5), None)
        self.assertEqual(S.tilepack_count(bytes(3)), 0)
        self.assertEqual(S.tilepack_count(struct.pack('<I', 6) + bytes(20)), 0)

    def test_the_checked_in_table_has_files(self):
        text = (ROOT / 'port/arcade/screens_table.c').read_text()
        self.assertIn('GENERATED by', text)
        self.assertNotIn(chr(0x2014), text)
        self.assertIn('{"SELECTA.PAC"', text)


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
