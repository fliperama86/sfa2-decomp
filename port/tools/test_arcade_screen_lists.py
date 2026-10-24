#!/usr/bin/env python3
"""Seconds: the runtime C of the screens' cell lists (port/arcade/screen_lists.c) against the Python model; no game files.

Builds screen_lists.c, the generated table, the screens table it reads and a driver with the address and undefined-behavior sanitizers, makes
small arcade programs (art records of mode 2, mode 0, mode 4 and mode 10 with flips, parts, palettes, banks; blocks of the scroll 2 layer and
of scroll 1), graphics of random tiles (some of them the flipped copies of others, so that one class has several codes) and a tile segment
that stores tiles with flips, and checks that the C gives, byte for byte and mask for mask, the lists and position runs that screen_lists.py's
model_chain gives, for record runs, windows of the scroll 2 blocks, empty lists, own lists, palette maps and exceptions (a palette, a dropped
cell, a tile of the PS1's own), with and without the palette part rule, and the same for the backdrop's cell map (model_backdrop). It also checks the pure functions (flips, classes, parts, the segment lookup with
stored flips against the pixels), that a record outside the program, a tile the segment lacks and sizes that do not fit are refused (never an
overflow), the table's sanity, the screens table reader, and that the C compiles warning-free for 32 bits.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import fighters as F
import screen_lists as S

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
ARC = ROOT / 'port/arcade'
SOURCES = [ARC / n for n in ('compose.c', 'screen_lists.c', 'screen_lists_table.c', 'screens.c', 'screens_table.c', 'tilepack.c')]
NT = 48          # tiles of the made-up graphics
HI = 0x10000     # the scroll layers' tiles lie from here: the graphics hold the same NT tiles at 0 and at HI
SIZE = 0x2f9000  # bytes of the made-up program: the scroll 2 blocks (0x2c0300 and on) and the scroll 1 sets and groups lie in it


def encode_tile(px):
    """The 128 bytes of the arcade tile that decodes to the PS1 pixels px (the inverse of S.arcade_pixels)."""
    q = bytearray(128)
    for y in range(16):
        for x in range(16):
            v = (px[y * 16 + x] - 1) & 15
            for pl in range(4):
                if v >> pl & 1:
                    q[y * 8 + (x // 8) * 4 + pl] |= 1 << (7 - x % 8)
    return bytes(q)


def made_up_graphics(rng):
    """NT tiles: 32 random ones, then flipped copies of some of them (and one blank tile)."""
    base = [bytes(rng.randrange(16) if rng.random() < 0.6 else 0 for _ in range(256)) for _ in range(32)]
    tiles = list(base)
    for _ in range(NT - 33):
        tiles.append(S.flip_pixels(rng.choice(base), rng.randrange(4)))
    tiles.append(bytes(256))
    one = b''.join(encode_tile(t) for t in tiles)
    return tiles, one + bytes(HI * 128 - len(one)) + one      # the same tiles again at HI


class Prog:
    """A made-up arcade program: art records one after the other from 0x1000, their coordinate blocks far away."""

    def __init__(self):
        self.data = bytearray(SIZE)
        self.at, self.coord, self.n = 0x1000, 0x40000, 0

    def record(self, blocks, shared=None):
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

    def record4(self, blocks):
        """A record of mode 4 (the portraits): blocks (code, attribute, x, y); the cells it covers are counted from the attributes."""
        a, d = self.at, self.data
        cells = sum((((at >> 8) & 15) + 1) * (((at >> 12) & 15) + 1) for _c, at, _x, _y in blocks)
        struct.pack_into('>HHH', d, a, 4, cells, len(blocks) - 1)
        for k, (code, attr, x, y) in enumerate(blocks):
            struct.pack_into('>HHhh', d, a + 6 + 8 * k, code, attr, x, y)
        self.at = a + 6 + 8 * len(blocks)
        return a

    def record10(self, h, w, pairs):
        """A record of mode 10 (a grid of h rows and w columns): pairs (code, attribute) column by column."""
        a, d = self.at, self.data
        struct.pack_into('>HHH', d, a, 10, h - 1, w - 1)
        for k, (code, attr) in enumerate(pairs):
            struct.pack_into('>HH', d, a + 6 + 4 * k, code, attr)
        self.at = a + 6 + 4 * w * h
        return a

    def block(self, number, cells):
        """Scroll 2 block `number`: cells {(column, row): (code, attribute)}; the rest stays zero (code 0 is tile 0xa400 + 0x10000, not blank)."""
        for (c, r), (code, attr) in cells.items():
            struct.pack_into('>HH', self.data, S.SCROLL_FIRST + S.SCROLL_BLOCK * number + 64 * c + 4 * r, code, attr)

    def random_record(self, rng):
        blocks, y = [], rng.randrange(-4, 4) * 16
        x = rng.randrange(-4, 4) * 16
        for _ in range(rng.randrange(1, 6)):
            w, h = rng.randrange(1, 3), rng.randrange(1, 3)
            attr = (w - 1) << 8 | (h - 1) << 12 | rng.randrange(4) << 5 | rng.choice((0, 0, 3, 5, 12))
            code = rng.randrange(0, NT - 40)
            x += rng.choice((16, 16, 32, 0, -16, 3, 19)) if blocks else 0
            blocks.append((code, attr, x, y + rng.choice((0, 0, 16, -16))))
        return blocks


def scene(rng):
    """(program bytes, tile bytes, tile pixels, Pool, [record addresses], model Pool tiles) for a random made-up arcade set."""
    pix, gfx = made_up_graphics(rng)
    cls = lambda code: S.pixel_class(pix[code % HI]) if 0 <= code % HI < NT and code < HI + NT else None
    seen, entries = set(), []
    for code in range(NT):
        key = cls(code)[0]
        if key not in seen:
            seen.add(key)
            entries.append((code, rng.randrange(4)))
    rng.shuffle(entries)
    seg = [None] + entries[:3] + ['zero'] + [None, None, None] + entries[3:]    # number 0 is never used; some numbers are the PS1's own
    pool = S.Pool(seg, cls, S.pixel_class(bytes(256))[0])
    return pix, gfx, pool, seg


class Pure(unittest.TestCase):
    def test_flip_and_class(self):
        rng = random.Random(1)
        px = bytes(rng.randrange(16) for _ in range(256))
        self.assertEqual(S.flip_pixels(S.flip_pixels(px, 3), 3), px)
        self.assertEqual(S.flip_pixels(px, 1)[1], px[14])
        self.assertEqual(S.flip_pixels(px, 2)[0], px[240])
        key, d = S.pixel_class(px)
        for f in range(4):
            k2, d2 = S.pixel_class(S.flip_pixels(px, f))
            self.assertEqual(k2, key)
            self.assertEqual(S.flip_pixels(S.flip_pixels(px, f), d2), S.flip_pixels(px, d))
        self.assertEqual(S.arcade_pixels(encode_tile(px)), px)

    def test_parts_and_order(self):
        blocks = [(1, 0x0c, 0, 0), (2, 0x0c, 16, 0), (3, 0x0c, 24, 0), (4, 0x0d, 40, 16)]
        self.assertEqual(S.block_parts(blocks, 0), [0, 0, 1, 1])             # 24 - 16 is not a multiple of 16
        self.assertEqual(S.block_parts(blocks, 1), [0, 0, 1, 2])             # the palette changes at the last block
        cells = S.emitted_cells(blocks, 0x10000, 0)
        self.assertEqual([c[0] for c in cells], [0x10003, 0x10004, 0x10001, 0x10002][:2] + [0x10001, 0x10002])
        self.assertEqual(cells[0][1:3], (24, 0))                              # the later part comes first
        flip = S.emitted_cells([(10, 0x0300 | 0x20 | 1, 0, 0)], 0, 0)       # 4 wide, 1 tall, flipped across: codes in reverse
        self.assertEqual([c[0] for c in flip], [13, 12, 11, 10])

    def test_lookup_flips(self):
        rng = random.Random(2)
        for _ in range(20):
            pix, _gfx, pool, seg = scene(rng)
            for code in range(NT):
                for u in range(4):
                    r = pool.lookup(code, u)
                    self.assertIsNotNone(r)
                    n, g = r
                    # the cell shows the tile flipped by u; the list shows pool tile n stored flipped, drawn flipped by g
                    stored = seg[n]
                    shown = S.flip_pixels(pix[code], u)
                    base = bytes(256) if stored == 'zero' else S.flip_pixels(pix[stored[0]], stored[1])
                    self.assertEqual(S.flip_pixels(base, g), shown)

    def test_position_base_and_prefix(self):
        # a module: position runs (two streams), three lists that use them, then two lists after the chain whose streams do not fit
        cells1 = [(1, 0, 0, 0), (2, 16, 0, 0), (3, 0, 16, 0)]
        cells2 = [(4, 0, 0, 0), (5, 16, 0, 0)]
        s1, s2 = S.position_stream_of(cells1), S.position_stream_of(cells2)
        runs = s1 + s2
        lists = [F.encode_list([(1, 0), (2, 0), (3, 0)], 0), F.encode_list([(4, 0), (5, 0)], len(s1)), F.encode_list([(1, 0), (2, 0), (3, 0)], 0)]
        junk = [F.encode_list([(7, 0)] * 40, 9000), F.encode_list([(8, 0)] * 40, 9000)]
        mod = b'\xff\x7f' * 32 + runs + b''.join(lists)
        start = 64 + len(runs)
        parsed = S.chains_of(mod + bytes(40))
        self.assertTrue(parsed)
        s0, ls = parsed[0]
        self.assertEqual(s0, start)
        self.assertEqual(S.position_base(mod + bytes(40), s0, ls)[0], 64)
        mod2 = mod + b''.join(junk)
        s2_, ls2 = S.chains_of(mod2 + bytes(40))[0]
        self.assertEqual(len(ls2), 5)
        self.assertIsNone(S.position_base(mod2 + bytes(40), s2_, ls2))
        base, end, count = S.position_base_prefix(mod2 + bytes(40), s2_, ls2)
        self.assertEqual((base, count), (64, 3))
        self.assertEqual(end, 64 + len(runs))

    def test_streams_and_encoding(self):
        cells = [(1, 0, 0, 0), (2, 16, 0, 0), (3, 48, 0, 0), (4, 0, 16, 0)]
        s = S.position_stream_of(cells)
        self.assertEqual(struct.unpack('<9h', s), (0, 0, 1, 32, 0, 0, -48, 16, 0))
        self.assertEqual(struct.unpack('<5H', F.encode_list([(k, 0) for k in range(5, 11)], 12)[:10]), (6, 4, 0, 0, 12))   # one run of six: mode 4


class FromC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-screen-lists-')
        d = Path(cls.dir.name)
        cls.exe = d / 'screen_lists_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES),
                        str(ARC / 'tests/screen_lists_test.c'), '-o', str(cls.exe)], check=True, timeout=180)
        cls.cases = 0

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_c(self, prog, gfx, seg, rules, ps, ls, items, exc, pairs, maps, windows=()):
        d = Path(self.dir.name)
        (d / 'prog.bin').write_bytes(prog)
        if getattr(self, 'gfx_seen', None) is not gfx:
            (d / 'gfx.bin').write_bytes(gfx)
            self.gfx_seen = gfx
        blocks = [(rx, ry, b) for w in windows for (rx, ry), b in sorted(dict(w[6]).items())]
        lines = [f'{rules} {ps} {ls} {len(seg)} {len(items)} {len(exc)} {len(pairs)} {len(maps)} {len(windows)} {len(blocks)}']
        for t in seg:
            lines.append('2 0 0' if t is None else '1 0 0' if t == 'zero' else f'0 {t[0]} {t[1]}')
        for it in items:
            if it[0] == S.REC:
                lines.append(f'0 {it[2]} {it[1]} {it[3]} {it[4]} 0 0')
            elif it[0] == S.WIN:
                lines.append(f'3 1 {it[1]} {it[3]} {it[4]} 0 0')
            elif it[0] == S.EMPTY:
                lines.append(f'1 {it[1]} 0 0 0 0 0')
            else:
                lines.append(f'2 1 0 0 0 {it[1]} {it[2]}')
        lines += [f'{i} {c} {p}' for (i, c), p in sorted(exc.items())]
        lines += [f'{a} {b}' for a, b in pairs]
        lines += [f'{a} {b}' for a, b in maps]
        first = 0
        for w in windows:
            lines.append(f'{w[0]} {w[1]} {w[2]} {w[3]} {w[4]} {w[5]} {first} {len(dict(w[6]))}')
            first += len(dict(w[6]))
        lines += [f'{rx} {ry} {b}' for rx, ry, b in blocks]
        (d / 'spec.txt').write_text('\n'.join(lines) + '\n')
        out = d / 'out'
        out.mkdir(exist_ok=True)
        r = subprocess.run([str(self.exe), 'build', str(d / 'prog.bin'), str(d / 'gfx.bin'), str(d / 'spec.txt'), str(out)], capture_output=True, text=True, timeout=60)
        if r.returncode:
            return r.stdout.strip() or f'status {r.returncode} {r.stderr[-200:]}'
        return tuple((out / n).read_bytes() for n in ('lists.bin', 'lists_known.bin', 'positions.bin', 'positions_known.bin'))

    def test_random_chains(self):
        for seed in range(12):
            rng = random.Random(100 + seed)
            pix, gfx, pool, seg = scene(rng)
            prog = Prog()
            addrs = [prog.record(prog.random_record(rng)) for _ in range(14)]
            addrs.append(prog.record([(rng.randrange(0, NT - 40), 16 * k, 0) for k in range(3)], shared=rng.choice((0, 5, 12))))
            maps = [((5, 0), (12, 7)), ((3, 30),)]
            pairs = [p for m in maps for p in m]
            map_rows, at = [], 0
            for m in maps:
                map_rows.append((at, len(m)))
                at += len(m)
            rules = rng.choice((0, S.PAL_SPLIT))
            items, pos, nlists = [], 0, 0
            while pos < len(addrs):
                c = min(rng.randrange(1, 4), len(addrs) - pos)
                if rng.random() < 0.25:
                    items.append((S.OWN, 2 * rng.randrange(5, 30), 6 * rng.randrange(0, 3)))
                    nlists += 1
                if rng.random() < 0.25:
                    items.append((S.EMPTY, rng.randrange(1, 3)))
                    nlists += items[-1][1]
                items.append((S.REC, addrs[pos], c, rng.choice((0, 8)), rng.choice((0, 0, 1, 2))))
                nlists += c
                pos += c
            # the run of records must really be consecutive: records were placed one after another, so item pos..pos+c is
            exc = {(rng.randrange(nlists), rng.randrange(3)): rng.randrange(32) for _ in range(rng.randrange(0, 4))}
            model = S.model_chain(bytes(prog.data), pool, items, rules, maps, exc)
            if model.problems:
                continue
            res = self.run_c(bytes(prog.data), gfx, seg, rules, len(model.positions), len(model.lists), items, exc, pairs, map_rows)
            self.assertNotIsInstance(res, str, f'seed {seed}: {res}')
            self.assertEqual(res[0], bytes(model.lists), f'seed {seed}')
            self.assertEqual(res[1], bytes(model.lists_known))
            self.assertEqual(res[2], bytes(model.positions))
            self.assertEqual(res[3], bytes(model.pos_known))
            FromC.cases += 1
        self.assertGreater(FromC.cases, 5)

    def mapcode(self, tile):
        """The code of a scroll 2 cell or mode 4/10 block that draws graphics tile `tile` (0 to NT - 1) of the high bank."""
        return (tile - S.SCROLL_ADD) & 0xffff

    def test_grids_portraits_windows_and_exceptions(self):
        cases = 0
        for seed in range(10):
            rng = random.Random(500 + seed)
            pix, gfx, pool, seg = scene(rng)
            prog = Prog()
            tile = lambda: rng.randrange(NT)                                   # the last tile is blank
            attr = lambda: rng.randrange(4) << 5 | rng.choice((0, 3, 5, 12))
            h, w = rng.randrange(1, 4), rng.randrange(1, 5)
            grid = prog.record10(h, w, [(self.mapcode(tile()), attr()) for _ in range(h * w)])
            blocks = [(self.mapcode(rng.randrange(NT - 18)), 0x0100 | 0x1000 | rng.choice((0, 3, 12)), 16 * rng.randrange(-3, 3), 16 * rng.randrange(-2, 5)) for _ in range(4)]
            blocks.append((self.mapcode(rng.randrange(NT - 18)), 0x0100, S.SHADOW_X, S.SHADOW_Y))                            # a shadow cell pair: left out
            blocks.append((self.mapcode(rng.randrange(NT - 18)), 0x0000, 16 * rng.randrange(0, 3), S.SHADOW_Y - 16))     # not a shadow cell: y is under SHADOW_Y
            portrait = prog.record4(blocks)
            ordinary = prog.record(prog.random_record(rng))
            for n in (3, 4):
                prog.block(n, {(c, r): (self.mapcode(tile() if rng.random() < 0.7 else NT - 1), attr()) for c in range(16) for r in range(16)})
            wins = [(-32, 16, 12, 5, 6, 4, {(0, 0): 3, (1, 0): 4}), (0, 0, 0, 0, 3, 2, {(0, 0): 3})]
            items = [(S.REC, grid, 1, HI, 0), (S.REC, portrait, 1, HI, 0), (S.WIN, 0, 1, HI, 0), (S.REC, ordinary, 1, 0, 0), (S.WIN, 1, 1, HI, 1)]
            maps = [((5, 0), (12, 7))]
            exc = {}
            # a dropped cell, a palette, and cells that show the PS1's own tiles (an explicit number, then the next one, then another explicit one)
            exc[(2, 1)] = S.DROP
            exc[(2, 2)] = 9
            own = pool.kind2
            exc[(4, 0)] = S.OWN_TILE | 1 << 10 | own[0] << 16 | 3
            exc[(4, 1)] = S.OWN_TILE | 0 << 10 | 0 << 16 | 4            # the next after own[0]
            exc[(4, 2)] = S.OWN_TILE | own[0] << 16 | 5
            model = S.model_chain(bytes(prog.data), pool, items, 0, maps, exc, wins)
            self.assertEqual(model.problems, [], f'seed {seed}')
            res = self.run_c(bytes(prog.data), gfx, seg, 0, len(model.positions), len(model.lists), items, exc, [p for m in maps for p in m], [(0, 2)], wins)
            self.assertNotIsInstance(res, str, f'seed {seed}: {res}')
            self.assertEqual(res[0], bytes(model.lists), f'seed {seed}')
            self.assertEqual(res[1], bytes(model.lists_known))
            self.assertEqual(res[2], bytes(model.positions))
            self.assertEqual(res[3], bytes(model.pos_known))
            cases += 1
        self.assertEqual(cases, 10)

    def test_window_and_grid_rules(self):
        # the model: a grid drops blank cells, a portrait drops blank and shadow cells, a window drops blank cells and reads across blocks
        rng = random.Random(9)
        pix, gfx, pool, seg = scene(rng)
        prog = Prog()
        blank, solid = NT - 1, 0
        grid = prog.record10(2, 2, [(self.mapcode(solid), 3), (self.mapcode(blank), 3), (self.mapcode(solid + 1), 3), (self.mapcode(solid + 2), 3)])
        cells = S.record_cells(bytes(prog.data), grid, HI, 0, pool.is_blank)[0]
        self.assertEqual([c[1:3] for c in cells], [(0, -32), (16, -32), (16, -16)])        # (0, -16) is the blank one
        portrait = prog.record4([(self.mapcode(solid), 0x0100 | 0x1000, -48, 48), (self.mapcode(solid + 3), 0x0000, 0, S.SHADOW_Y)])
        cells = S.record_cells(bytes(prog.data), portrait, HI, 0, pool.is_blank)[0]
        self.assertEqual(sorted((c[1], c[2]) for c in cells), [(-48, 48), (-48, 64), (-32, 48)])         # (-32, 64) and (0, 64) are shadow cells
        prog.block(3, {(15, 2): (self.mapcode(solid), 0), (15, 3): (self.mapcode(blank), 0)})
        prog.block(4, {(0, 2): (self.mapcode(solid + 1), 0x20), (0, 3): (self.mapcode(solid + 2), 0)})
        win = (0, 0, 15, 2, 2, 2, {(0, 0): 3, (1, 0): 4})
        got = S.window_cells(bytes(prog.data), win, pool.is_blank)
        self.assertEqual([(c[1], c[2]) for c in got], [(0, 0), (16, 0), (16, 16)])          # (0, 16) is blank
        self.assertIsNone(S.window_cells(bytes(prog.data), (0, 0, 15, 2, 2, 2, {(0, 0): 3}), pool.is_blank))     # block (1, 0) is missing
        self.assertEqual(pool.successor(pool.kind2[0]), pool.kind2[1])
        self.assertEqual(pool.successor(pool.kind2[-1]), 0)
        self.assertEqual(pool.successor(0), 0)

    def test_backdrop(self):
        for seed in range(4):
            rng = random.Random(700 + seed)
            pix, gfx, pool, seg = scene(rng)
            prog = Prog()
            for ident in range(8):
                for k in range(16):
                    struct.pack_into('>HH', prog.data, S.GROUPS1 + 64 * ident + 4 * k, rng.randrange(2 * NT), rng.choice((0, 0x20, 0x40, 0x60)) | rng.randrange(32))
            for st in range(6):
                for k in range(64):
                    struct.pack_into('>H', prog.data, S.SETS1 + 128 * st + 2 * k, rng.randrange(8))
            spec = dict(slot=0x26, picture_slot=0x25, columns=10, rows=8, band=4, y0=1, first_page=22, clut=0x4024, segments=((4, 0, 1, 2), (6, 3, 0, 5)))
            picture, known = bytearray(98304), bytearray(98304)
            pieces = {}
            for r in range(spec['rows']):
                for c in range(spec['columns']):
                    st, x = (r // 4, 2 + c) if c < 4 else (3, 5 + c - 4)
                    code, at = S.scroll1_cell(bytes(prog.data), st, x, 1 + r % 4)
                    pieces[S.scroll1_piece_bytes(gfx, code, at)] = None
            slots = [(p, u, v) for p in range(3) for u in range(32) for v in range(32)]
            rng.shuffle(slots)
            for piece, (p, u, v) in zip(list(pieces), slots):
                for yy in range(8):
                    at = (v * 8 + yy) * S.PICTURE_ROW + p * 128 + u * 4
                    picture[at:at + 4] = piece[yy * 4:yy * 4 + 4]
            for p, u, v in slots:
                if rng.random() < 0.9:                                                      # a tenth of the units are not known
                    for yy in range(8):
                        at = (v * 8 + yy) * S.PICTURE_ROW + p * 128 + u * 4
                        known[at:at + 4] = b'\x01\x02\x01\x01'
            model, mask = S.model_backdrop(bytes(prog.data), gfx, spec, bytes(picture), bytes(known))
            d = Path(self.dir.name)
            (d / 'prog.bin').write_bytes(bytes(prog.data))
            (d / 'gfx.bin').write_bytes(gfx)
            (d / 'out').mkdir(exist_ok=True)
            (d / 'out/picture.bin').write_bytes(bytes(picture))
            (d / 'out/picture_known.bin').write_bytes(bytes(known))
            lines = [f'{spec["columns"]} {spec["rows"]} {spec["band"]} {spec["y0"]} {spec["first_page"]} {spec["clut"]} {len(spec["segments"])}']
            lines += [f'{c} {st} {sp} {x}' for c, st, sp, x in spec['segments']]
            (d / 'spec.txt').write_text('\n'.join(lines) + '\n')
            r = subprocess.run([str(self.exe), 'backdrop', str(d / 'prog.bin'), str(d / 'gfx.bin'), str(d / 'spec.txt'), str(d / 'out')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
            self.assertEqual((d / 'out/map.bin').read_bytes(), model, f'seed {seed}')
            self.assertEqual((d / 'out/map_known.bin').read_bytes(), mask)
            self.assertGreater(sum(mask), 100)
            self.assertLess(sum(mask), len(mask))

    def test_refusals(self):
        rng = random.Random(7)
        pix, gfx, pool, seg = scene(rng)
        prog = Prog()
        a = prog.record([(1, 0, 0, 0), (2, 0, 16, 0)])
        items = [(S.REC, a, 1, 0, 0)]
        model = S.model_chain(bytes(prog.data), pool, items, 0, [], {})
        ls, ps = len(model.lists), len(model.positions)
        ok = self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, items, {}, [], [])
        self.assertNotIsInstance(ok, str)
        self.assertIn('table says', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls + 2, items, {}, [], []))        # sizes that differ
        self.assertIn('need more', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls - 2, items, {}, [], []))
        self.assertIn('outside the program', self.run_c(bytes(prog.data)[:0x1000], gfx, seg, 0, ps, ls, items, {}, [], []))
        self.assertIn('0 cells', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, [(S.REC, 0x9fff0, 1, 0, 0)], {}, [], []))
        self.assertIn('0 cells', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, [(S.REC, 0x2000, 1, 0, 0)], {}, [], []))
        self.assertIn('not in the segment', self.run_c(bytes(prog.data), gfx, [None] * 4, 0, ps, ls, items, {}, [], []))
        self.assertIn('outside the graphics', self.run_c(bytes(prog.data), gfx[:128], seg, 0, ps, ls, items, {}, [], []))
        self.assertIn('palette map', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, [(S.REC, a, 1, 0, 3)], {}, [], []))
        self.assertIn('window 0 is not in the table', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, [(S.WIN, 0, 1, HI, 0)], {}, [], []))
        self.assertIn('no block for the cell', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, [(S.WIN, 0, 1, HI, 0)], {}, [], [], [(0, 0, 15, 0, 2, 1, {(0, 0): 1})]))
        self.assertIn('no number follows', self.run_c(bytes(prog.data), gfx, seg, 0, ps, ls, items, {(0, 0): S.OWN_TILE}, [], []))

    def test_generated_table(self):
        r = subprocess.run([str(self.exe), 'table'], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 0, r.stdout)
        self.assertIn('0 problems', r.stdout)


class Table(unittest.TestCase):
    def test_screens_table_reader(self):
        t = S.ScreensTable()
        text = (ARC / 'screens_table.c').read_text()
        import re
        self.assertEqual(len(t.files), int(re.search(r'arcade_screens_file_count = (\d+)', text).group(1)))
        segs = t.segments('SELECTA.PAC')
        self.assertTrue(segs)
        self.assertEqual(segs[0][:2], (0, 0x18))

    def test_c_table_text(self):
        rows = {'CDEMO99.PAC': [dict(start=0x100, nlists=3, base=0x20, info=[('own',)], items=[(S.REC, 0x1000, 2, 0x10000, 1), (S.OWN, 14, 6)], maps=[((5, 0),)],
                                      exceptions={(1, 2): 28}, rules=S.PAL_SPLIT, pos_size=64, lists_size=200, pool=(0, 0x10, 0, 0), module_slot=1)],
                'CDEMO98.PAC': 'no module'}
        text = S.c_table(rows)
        self.assertIn('{0, 2u, 0x1000u, 0x10000u, 1u, 0u, 0u},', text)
        self.assertIn('{2, 1u, 0x0u, 0x0u, 0u, 14u, 6u},', text)
        self.assertIn('{1u, 2u, 0x1cu},', text)
        self.assertIn('{"CDEMO99.PAC", 1u, 0x20u, 0x100u, 0u, 0u, 0x10u, 0u, 0u},', text)
        self.assertIn('ARCADE_SCREEN_LISTS_PAL_SPLIT, 64u, 200u,', text)
        self.assertNotIn(chr(0x2014), text)

    def test_32_bit_and_no_dash(self):
        with tempfile.TemporaryDirectory() as d:
            for n in ('compose.c', 'screen_lists.c', 'screen_lists_table.c'):
                subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(ARC / n), '-o', str(Path(d) / (n + '.o'))], check=True, timeout=120)
        for p in (ARC / 'screen_lists.c', ARC / 'screen_lists.h', ARC / 'screen_lists.py', ARC / 'tests/screen_lists_test.c', Path(__file__)):
            self.assertNotIn(chr(0x2014), p.read_text(), str(p))


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
