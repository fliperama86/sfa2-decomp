#!/usr/bin/env python3
"""Seconds: the win quotes in the arcade's English (port/arcade/quotes.py, quotes.c, quotes.h); no game files.

Python side, on made-up data: the font rule, the layout model (pairing, numbering, centring, one-line quotes, stored-once lists and streams),
the reading of a module's tables and of the PS1's own quotes (the centring derived from them), the game's way of reading a cell list and a
stream (modes 0 and 4, refusals) and of placing runs, the drawing check on a good and a damaged module, the generated table's text and the PNG.

C side: quotes.c, built with the address and undefined-behavior sanitizers and a driver (tests/quotes_test.c), on a made-up program, graphics
and module: arcade_quotes_build against the model (module bytes, mask of written bytes, glyph segment decoded by tilepack.py, the quotes drawn
as the game does), and every refusal (a character outside the font, a line of more than 20 tiles, an empty quote, rooms too small, tables
outside the module, overlapping tables, text outside the program, graphics too short).
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, time, unittest, zlib
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import quotes as Q

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
DRIVER = ARC / 'tests/quotes_test.c'

TEXT = 0x400                        # the made-up program: 4 quotes at 0x400
PROGRAM_SIZE = 0x1000
MSIZE = 0x600                       # the made-up module
OFFSETS, COUNT = 0x20, 6
LISTS, LISTS_ROOM = 0x100, 0x100
STREAMS, STREAMS_ROOM = 0x80, 0x40
SEGMENT, SEGMENT_ROOM = 0x18668, 0x3000
CENTRE, HEIGHT = 160, 16


def make_gfx():
    """Graphics with the font's tiles (0x10000 + (0 .. 0x9f)) random, everything else zero; as short as it may be."""
    rnd = random.Random(7)
    g = bytearray((0x10000 + 0xa0) * 128)
    g[0x10000 * 128:] = rnd.randbytes(0xa0 * 128)
    return bytes(g)


GFX = make_gfx()


def word(c):
    return b'\0\0' if c == ' ' else struct.pack('>H', ord(c) | 0x80)


def line_bytes(text, lead=None):
    """A 42-word line of the text centred with spaces (0x0000), the first word 0x8000 when lead is set."""
    pad = (Q.LINE - len(text)) // 2
    s = ' ' * pad + text
    s += ' ' * (Q.LINE - len(s))
    b = b''.join(word(c) for c in s)
    return (struct.pack('>H', 0x8000) + b[2:]) if lead and s[0] == ' ' else b


def make_program(quotes):
    p = bytearray(PROGRAM_SIZE)
    for q, lines in enumerate(quotes):
        for l, text in enumerate(lines):
            at = TEXT + (q * 2 + l) * 84
            p[at:at + 84] = line_bytes(text, lead=(q + l) % 2 == 1)
    return bytes(p)


QUOTES = [['Hello there', 'friend'], ['Hello again', ''], ['', 'Ha  ha ha'], ['WIN WIN', 'x']]


def glyphs(c):
    return Q.glyph(GFX, c)


def make_module():
    m = bytearray(random.Random(3).randbytes(MSIZE))
    return bytes(m)


def row_args(**kw):
    r = dict(file='CDEMO99.PAC', kind=0x99, text=TEXT, offsets=OFFSETS, count=COUNT, lists=LISTS, lists_room=LISTS_ROOM, streams=STREAMS,
             streams_room=STREAMS_ROOM, segment=SEGMENT, segment_room=SEGMENT_ROOM)
    r.update(kw)
    return r


def u32(m, a):
    return struct.unpack_from('<I', m, a)[0]


class Font(unittest.TestCase):
    def test_glyph_rule(self):
        # 'A' = 0x41: TOP = 0x80 + 1 + 0x20 * 2 = 0xc1 (odd: rows 8 to 15 of tile 0x10000 + 0x60), bottom piece 0xd1 (tile 0x10068, rows 8 to 15)
        g = glyphs(0x41)
        t = GFX[(0x10000 + 0x60) * 128:(0x10000 + 0x61) * 128]
        self.assertEqual(g[3][5], (Q.arcade_px(t, 5, 8 + 3) + 1) & 15)
        t = GFX[(0x10000 + 0x68) * 128:(0x10000 + 0x69) * 128]
        self.assertEqual(g[8 + 2][1], (Q.arcade_px(t, 1, 8 + 2) + 1) & 15)
        # 'a' = 0x61: TOP = 0x80 + 1 + 0x20 * 4 = 0x101 (even index 0x100 + 1 = odd...), the rule is the same arithmetic
        g = glyphs(0x61)
        t = GFX[(0x10000 + 0x80)  * 128:(0x10000 + 0x81) * 128]
        self.assertEqual(g[4][2], (Q.arcade_px(t, 2, 8 + 4) + 1) & 15)
        self.assertEqual(glyphs(0x20), [[0] * 8 for _ in range(16)])
        self.assertTrue(all(0 <= v < 16 for r in g for v in r))

    def test_word_char(self):
        for w, c in ((b'\0\0', 0x20), (b'\x80\0', 0x20), (b'\0\xc1', 0x41), (b'\x80\xc1', 0x41), (b'\0\xa0', 0x20)):
            self.assertEqual(Q.word_char(w, 0), c, w)
        for w in (b'\x12\xc1', b'\0\x41', b'\0\xff', b'\x01\0', b'\x80\x7f'):
            self.assertIsNone(Q.word_char(w, 0), w)

    def test_arcade_line_trims_and_refuses(self):
        p = make_program(QUOTES)
        self.assertEqual(Q.arcade_quotes(p, TEXT)[0], ['Hello there', 'friend'])
        self.assertEqual(Q.arcade_quotes(p, TEXT)[2], ['', 'Ha  ha ha'])
        bad = bytearray(p)
        bad[TEXT + 4:TEXT + 6] = b'\x12\xc1'
        with self.assertRaises(Q.Fail):
            Q.arcade_quotes(bytes(bad), TEXT)


class Layout(unittest.TestCase):
    def test_pairing_numbering_and_places(self):
        m = Q.model_build(QUOTES, CENTRE, HEIGHT, glyphs)
        # tiles: 0 blank, then the distinct pairs in order of first use
        pairs = []
        for q in QUOTES:
            for l in q:
                for i in range(0, len(l), 2):
                    p = (l[i], l[i + 1] if i + 1 < len(l) else ' ')
                    if p != (' ', ' ') and p not in pairs:
                        pairs.append(p)
        self.assertEqual(len(m['tiles']), 1 + len(pairs))
        self.assertEqual(m['tiles'][0], bytes(128))
        # quote 0: 'Hello there' (11 characters: 6 tiles) and 'friend' (3 tiles)
        n0 = struct.unpack_from('<5H', m['lists'], m['list_at'][0])
        self.assertEqual(n0[:4], (9, 0, 0, 0))
        s = m['streams'][m['stream_at'][0]:m['stream_at'][0] + 12]
        # 6 tiles: x = 160 - 16 * 3 = 112; 3 tiles: x = 160 - 16 * 2 = 128, from the end of the first run (112 + 96 - 16 = 192): -64
        self.assertEqual(struct.unpack('<6H', s), (112, 0, 5, (128 - 192) & 0xffff, 16, 2))
        cells = struct.unpack_from('<9H', m['lists'], m['list_at'][0] + 10)
        self.assertEqual(cells[:6], tuple(range(1, 7)))
        # the second 'Hello again' shares the first pair
        c1 = struct.unpack_from('<6H', m['lists'], m['list_at'][1] + 10)
        self.assertEqual(c1[0], 1)

    def test_one_line_quotes_are_on_the_second_row(self):
        m = Q.model_build([['Hi', '']] * 4, CENTRE, HEIGHT, glyphs)
        self.assertEqual(struct.unpack('<3H', m['streams']), (160 - 16, 16, 0))
        m = Q.model_build([['', 'Hi']] * 4, CENTRE, HEIGHT, glyphs)
        self.assertEqual(struct.unpack('<3H', m['streams']), (160 - 16, 16, 0))

    def test_odd_and_even_widths_are_centred_as_the_ps1(self):
        for chars, tiles in ((1, 1), (2, 1), (3, 2), (4, 2), (5, 3), (6, 3), (40, 20)):
            m = Q.model_build([['a' * chars, '']] * 4, CENTRE, HEIGHT, glyphs)
            x, _y, r = struct.unpack('<3H', m['streams'])
            self.assertEqual(r, tiles - 1)
            self.assertEqual(x + 8 * (tiles + (tiles & 1)), CENTRE, chars)

    def test_blank_pair_is_tile_zero_and_lone_trailing_is_half_blank(self):
        m = Q.model_build([['a  b', ''], ['a', ''], ['b a', ''], ['x', '']], CENTRE, HEIGHT, glyphs)
        cells = struct.unpack_from('<2H', m['lists'], m['list_at'][0] + 10)
        self.assertEqual(cells, (1, 2))                              # 'a ' then ' b': a pair of spaces is only the whole pair
        m = Q.model_build([['a   b', ''], ['a', ''], ['b', ''], ['x', '']], CENTRE, HEIGHT, glyphs)
        self.assertEqual(struct.unpack_from('<3H', m['lists'], m['list_at'][0] + 10), (1, 0, 2))   # 'a ', '  ', 'b '
        t = m['tiles'][1]
        for y in range(16):
            for x in range(8, 16):
                self.assertEqual((t[y * 8 + x // 2] >> (4 if x & 1 else 0)) & 15, 0)

    def test_identical_quotes_are_stored_once(self):
        m = Q.model_build([['Hi', 'there']] * 4, CENTRE, HEIGHT, glyphs)
        self.assertEqual(m['list_at'], [0, 0, 0, 0])
        self.assertEqual(m['stream_at'], [0, 0, 0, 0])
        self.assertEqual(len(m['lists']), 10 + 2 * 4)

    def test_refusals(self):
        with self.assertRaises(Q.Fail):
            Q.model_build([['a' * 41, '']] * 4, CENTRE, HEIGHT, glyphs)
        with self.assertRaises(Q.Fail):
            Q.model_build([['', '']] + [['a', '']] * 3, CENTRE, HEIGHT, glyphs)


def make_own_module():
    """A module with the PS1 layout's tables: 19 own offset tables back to back up to the table of the tables (18 kinds and 0x14), kinds 0x12
    and 0x13 sharing those of 4 and 0x11; areas back to back (streams, then lists), the aliases sharing; every table entry 0 (a quote a kind: a
    one-line quote of 1 + k % 7 tiles, kind 0 of two lines, kind 0x14 the 9 tiles at dy 16)."""
    m = bytearray(0x12000)
    own = list(range(18)) + [20]
    tab, addr = {}, Q.BASE + 0x8b98
    for i, k in enumerate(own):
        n = 25 if i < 9 else 24
        tab[k] = (addr, n)
        addr += 4 * n
    assert addr == Q.OFFSET_TABLES
    alias = {18: 4, 19: 17}
    for k in range(Q.KINDS):
        struct.pack_into('<I', m, Q.OFFSET_TABLES - Q.BASE + 4 * k, tab[alias.get(k, k)][0])
    areas, a = {}, Q.BASE + 0x9e38
    for k in own:
        areas[k] = (a, a + 0x40) if k != 20 else (a, a + 8)
        a += 0x140
    assert a < Q.AREAS
    for k in range(Q.KINDS):
        struct.pack_into('<2I', m, Q.AREAS - Q.BASE + 8 * k, *areas[alias.get(k, k)])
    for k in own:
        s, l = areas[k]
        if k == 20:
            struct.pack_into('<3H', m, s - Q.BASE, 80, 16, 8)
            cells = [1] * 9
        elif k == 0:
            struct.pack_into('<6H', m, s - Q.BASE, 128, 0, 2, 0x10000 - 16, 16, 1)
            cells = [1, 2, 3, 4, 5]
        else:
            tiles = 1 + k % 7
            struct.pack_into('<3H', m, s - Q.BASE, 160 - 16 * ((tiles + 1) // 2), 0, tiles - 1)
            cells = list(range(1, tiles + 1))
        struct.pack_into('<5H', m, l - Q.BASE, len(cells), 0, 0, 0, 0)
        for i, c in enumerate(cells):
            struct.pack_into('<H', m, l - Q.BASE + 10 + 2 * i, c)
    return bytes(m), areas, tab


class OwnQuotes(unittest.TestCase):
    def test_module_tables_and_rooms(self):
        m, areas, tab = make_own_module()
        tables, got = Q.module_tables(m)
        self.assertEqual(len(tables), Q.KINDS)
        self.assertEqual(tables[3], (tab[3][0] - Q.BASE, 25))
        self.assertEqual(tables[17], (tab[17][0] - Q.BASE, 24))
        self.assertEqual(tables[18], tables[4])          # an alias shares its table
        self.assertEqual(tables[19], tables[17])
        self.assertEqual(tables[20], (tab[20][0] - Q.BASE, 24))
        self.assertEqual(got[3], (areas[3][0] - Q.BASE, 0x40, areas[3][1] - Q.BASE, 0x100))   # streams, room up to the lists, lists, room up to the next area
        self.assertEqual(got[20], (areas[20][0] - Q.BASE, 8, areas[20][1] - Q.BASE, Q.AREAS - areas[20][1]))
        self.assertEqual(got[18], got[4])                # an alias shares its area and room

    def test_a_table_of_the_wrong_size_is_refused(self):
        m, _areas, tab = make_own_module()
        m = bytearray(m)
        struct.pack_into('<I', m, Q.OFFSET_TABLES - Q.BASE + 4 * 5, tab[5][0] + 4 * 8)   # kind 5 starts 8 entries in: its table has 17, kind 4's 33
        with self.assertRaises(Q.Fail):
            Q.module_tables(bytes(m))

    def test_areas_out_of_order_are_refused(self):
        m, _areas, _tab = make_own_module()
        m = bytearray(m)
        s, l = struct.unpack_from('<2I', m, Q.AREAS - Q.BASE)
        struct.pack_into('<2I', m, Q.AREAS - Q.BASE, l, s)
        with self.assertRaises(Q.Fail):
            Q.module_tables(bytes(m))

    def test_own_quotes_and_the_centring(self):
        m, _areas, _tab = make_own_module()
        tables, areas = Q.module_tables(m)
        own = Q.own_quotes(m, tables, areas)
        self.assertEqual(len(own), 19)                   # aliases are read once
        centre, height, notes = Q.centring(own)
        self.assertEqual((centre, height), (160, 16))
        self.assertEqual(notes['one_line'][-1], (20, 16, 9))     # the one-line kind: first dy 16, 9 tiles
        self.assertEqual(dict(notes['heights']), {16: 1})

    def test_centring_refuses_without_quotes(self):
        with self.assertRaises(Q.Fail):
            Q.centring([])


class Reading(unittest.TestCase):
    def test_modes_and_places(self):
        m = bytearray(0x200)
        struct.pack_into('<5H', m, 0x100, 5, 0, 0, 0, 0x20)                 # mode 0, 5 cells
        for i, c in enumerate((7, 8, 9, 1, 2)):
            struct.pack_into('<H', m, 0x10a + 2 * i, c)
        struct.pack_into('<5H', m, 0x120, 5, 4, 0, 0, 0x20)                  # mode 4: (7, run 2), (1, run 1)
        struct.pack_into('<4H', m, 0x12a, 7, 2, 1, 1)
        struct.pack_into('<6H', m, 0x20, 48, 0, 2, 0x10000 - 48, 16, 1)      # runs of 3 and 2 tiles
        cells, runs = Q.parse_list(bytes(m), 0, 0x100, 0)
        self.assertEqual((cells, runs), ([7, 8, 9, 1, 2], [(48, 0, 3), (-48, 16, 2)]))
        cells4, runs4 = Q.parse_list(bytes(m), 0, 0x120, 0)
        self.assertEqual((cells4, runs4), ([7, 8, 9, 1, 2], runs))
        # the game's stepping: a run ends one tile short of its right edge, the next dx counts from there
        self.assertEqual(Q.place(runs), [(48, 0, 3), (48 + 32 - 48, 16, 2)])
        with self.assertRaises(Q.Fail):
            Q.parse_list(bytes(m), 0, 0x1fc, 0)
        struct.pack_into('<H', m, 0x102, 2)
        with self.assertRaises(Q.Fail):
            Q.parse_list(bytes(m), 0, 0x100, 0)

    def test_draw_and_compare_detect_damage(self):
        r = row_args()
        lines = [['AB', 'CD']] * 4
        mod = bytearray(MSIZE)
        model = Q.model_build(lines, CENTRE, HEIGHT, glyphs)
        mod[LISTS:LISTS + len(model['lists'])] = model['lists']
        mod[STREAMS:STREAMS + len(model['streams'])] = model['streams']
        # the segment as tiles
        tiles = model['tiles']
        canvas = Q.draw(bytes(mod), LISTS, STREAMS, model['list_at'][0], tiles)
        self.assertEqual(canvas, Q.draw_expected(lines[0], CENTRE, HEIGHT, glyphs))
        # a damaged cell, a damaged tile and a shifted stream all differ
        bad = bytearray(mod)
        struct.pack_into('<H', bad, LISTS + 10, 2)
        self.assertNotEqual(Q.draw(bytes(bad), LISTS, STREAMS, 0, tiles), canvas)
        t2 = list(tiles)
        t2[1] = bytes([t2[1][0] ^ 1]) + t2[1][1:]
        self.assertNotEqual(Q.draw(bytes(mod), LISTS, STREAMS, 0, t2), canvas)
        bad = bytearray(mod)
        bad[STREAMS] += 8
        self.assertNotEqual(Q.draw(bytes(bad), LISTS, STREAMS, 0, tiles), canvas)
        # read_back finds the characters
        back = Q.read_back(canvas, 2, HEIGHT, glyphs)
        xs = CENTRE - 16
        self.assertTrue(all(c is not None and ch in c for c, ch in zip(back[0][xs // 8:xs // 8 + 2], 'AB')))
        self.assertTrue(all(c is not None and ch in c for c, ch in zip(back[1][xs // 8:xs // 8 + 2], 'CD')))
        # a tile number past the segment
        with self.assertRaises(Q.Fail):
            Q.draw(bytes(mod), LISTS, STREAMS, 0, tiles[:2])
        del r


class Table(unittest.TestCase):
    def test_c_table_text(self):
        rows = [dict(file='CDEMO00.PAC', kind=0, text=0xe2780, offsets=0x8b98, count=26, lists=0x9f6c, lists_room=1328, streams=0x9e38, streams_room=308,
                     segment=0x18668, segment_room=20824, disc_tiles=208)]
        t = Q.c_table(dict(rows=rows, centre=160, line_height=16))
        self.assertIn('{"CDEMO00.PAC", 0x0, 0xe2780, 0x8b98, 26u, 0x9f6c, 0x530, 0x9e38, 0x134, 0x18668, 0x5158}', t)
        self.assertIn('arcade_quotes = {1u, 0x10u, 160, 16u, fighters', t)
        self.assertNotIn(chr(0x2014), t)

    def test_png(self):
        d = tempfile.TemporaryDirectory()
        p = Path(d.name) / 'a.png'
        Q.png(p, 2, 1, bytes([1, 2, 3, 4, 5, 6]))
        b = p.read_bytes()
        self.assertEqual(b[:8], b'\x89PNG\r\n\x1a\n')
        self.assertEqual(struct.unpack('>II', b[16:24]), (2, 1))
        i = b.index(b'IDAT')
        n = struct.unpack('>I', b[i - 4:i])[0]
        self.assertEqual(zlib.decompress(b[i + 4:i + 4 + n]), b'\0' + bytes([1, 2, 3, 4, 5, 6]))
        d.cleanup()


class CBase(unittest.TestCase):
    """The driver, linked with the generated table (a real one is checked in; only its shape matters here)."""

    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory()
        d = Path(cls.dir.name)
        cls.exe = d / 'quotes_test'
        subprocess.run(['gcc', *FLAGS, *SAN, str(ARC / 'quotes.c'), str(ARC / 'tilepack.c'), str(ARC / 'quotes_table.c'), str(DRIVER), '-o', str(cls.exe)],
                       check=True, timeout=240)
        (d / 'gfx').write_bytes(GFX)

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_exe(self, *args):
        return subprocess.run([str(self.exe), *map(str, args)], capture_output=True, text=True, timeout=120)

    def build(self, quotes=QUOTES, program=None, module=None, gfx=None, **kw):
        d = Path(self.dir.name)
        r = row_args(**kw)
        (d / 'program').write_bytes(program if program is not None else make_program(quotes))
        (d / 'module').write_bytes(module if module is not None else make_module())
        if gfx is not None:
            (d / 'gfx_short').write_bytes(gfx)
        out = d / 'out'
        out.mkdir(exist_ok=True)
        for n in ('module', 'changed', 'segment'):
            (out / n).unlink(missing_ok=True)
        args = [r[k] for k in ('kind', 'text', 'offsets', 'count', 'lists', 'lists_room', 'streams', 'streams_room', 'segment', 'segment_room')]
        run = self.run_exe('build', r['file'], *args, CENTRE, HEIGHT, d / 'program', d / ('gfx_short' if gfx is not None else 'gfx'), d / 'module', out)
        return run, out


class Build(CBase):
    def test_happy_case_against_the_model(self):
        module = make_module()
        run, out = self.build(module=module)
        self.assertEqual(run.returncode, 0, run.stdout + run.stderr)
        new, changed, segment = (out / 'module').read_bytes(), (out / 'changed').read_bytes(), (out / 'segment').read_bytes()
        model = Q.model_build(QUOTES, CENTRE, HEIGHT, glyphs)
        want, mask = bytearray(module), bytearray(MSIZE)

        def put(at, b):
            want[at:at + len(b)] = b
            mask[at:at + len(b)] = b'\1' * len(b)
        put(LISTS, model['lists'])
        put(STREAMS, model['streams'])
        for e in range(COUNT):
            put(OFFSETS + 4 * e, struct.pack('<I', model['list_at'][e % 4]))
        self.assertEqual(new, bytes(want))
        self.assertEqual(changed, bytes(mask))
        self.assertTrue(all(a == b for a, b, c in zip(new, module, changed) if not c))
        self.assertEqual(Q.segment_tiles(segment), model['tiles'])
        self.assertEqual(run.stdout.split(), ['ok', str(len(segment))])
        # every entry is drawn as the arcade quote of its place
        tiles = Q.segment_tiles(segment)
        for e in range(COUNT):
            got = Q.draw(new, LISTS, STREAMS, u32(new, OFFSETS + 4 * e), tiles)
            self.assertEqual(got, Q.draw_expected(QUOTES[e % 4], CENTRE, HEIGHT, glyphs), e)

    def test_stored_once_and_exact_rooms(self):
        quotes = [['Hi', 'there']] * 4
        m = Q.model_build(quotes, CENTRE, HEIGHT, glyphs)
        run, out = self.build(quotes=quotes, lists_room=len(m['lists']), streams_room=len(m['streams']))
        self.assertEqual(run.returncode, 0, run.stdout)
        new = (out / 'module').read_bytes()
        self.assertEqual([u32(new, OFFSETS + 4 * e) for e in range(COUNT)], [0] * COUNT)
        run, _ = self.build(quotes=quotes, lists_room=len(m['lists']) - 1)
        self.assertEqual(run.returncode, 3)
        run, _ = self.build(quotes=quotes, streams_room=len(m['streams']) - 1)
        self.assertEqual(run.returncode, 3)

    def test_spaces_inside_and_lone_characters(self):
        quotes = [['a  b', 'c'], ['  x  ', ''], ['m n o', 'p'], ['Q', 'R']]
        run, out = self.build(quotes=quotes)
        self.assertEqual(run.returncode, 0, run.stdout)
        new, segment = (out / 'module').read_bytes(), (out / 'segment').read_bytes()
        tiles = Q.segment_tiles(segment)
        model = Q.model_build(quotes, CENTRE, HEIGHT, glyphs)
        self.assertEqual(tiles, model['tiles'])
        for e in range(4):
            self.assertEqual(Q.draw(new, LISTS, STREAMS, u32(new, OFFSETS + 4 * e), tiles), Q.draw_expected(quotes[e], CENTRE, HEIGHT, glyphs))

    def test_a_character_outside_the_font(self):
        p = bytearray(make_program(QUOTES))
        for bad in (b'\x00\xff', b'\x12\xc1', b'\x00\x41', b'\x80\x7f'):
            p[TEXT + 20:TEXT + 22] = bad
            run, _ = self.build(program=bytes(p))
            self.assertEqual(run.returncode, 3, bad)
            self.assertIn('outside the font', run.stdout)

    def test_a_line_of_more_than_20_tiles(self):
        run, _ = self.build(quotes=[['a' * 41, ''], ['a', ''], ['a', ''], ['a', '']])
        self.assertEqual(run.returncode, 3)
        self.assertIn('more than 20 tiles', run.stdout)
        run, _ = self.build(quotes=[['a' * 40, ''], ['a', ''], ['a', ''], ['a', '']])
        self.assertEqual(run.returncode, 0, run.stdout)

    def test_an_empty_quote(self):
        run, _ = self.build(quotes=[['a', ''], ['', ''], ['a', ''], ['a', '']])
        self.assertEqual(run.returncode, 3)
        self.assertIn('no text', run.stdout)

    def test_segment_room(self):
        run, out = self.build()
        size = len((out / 'segment').read_bytes())
        run, _ = self.build(segment_room=size)
        self.assertEqual(run.returncode, 0, run.stdout)
        run, _ = self.build(segment_room=size - 1)
        self.assertEqual(run.returncode, 3)
        self.assertIn('pack', run.stdout)

    def test_tables_outside_the_module(self):
        for kw in (dict(offsets=MSIZE - 4 * COUNT + 4), dict(lists=MSIZE - LISTS_ROOM + 1), dict(streams=MSIZE - STREAMS_ROOM + 1),
                   dict(offsets=0xffffff00), dict(count=0)):
            run, _ = self.build(**kw)
            self.assertEqual(run.returncode, 3, kw)
        run, _ = self.build(offsets=MSIZE - 4 * COUNT, lists=0x100, lists_room=0x100, streams=0x80, streams_room=0x40)
        self.assertEqual(run.returncode, 0, run.stdout)

    def test_overlapping_tables(self):
        for kw in (dict(lists=OFFSETS + 4), dict(streams=LISTS + 8), dict(streams=OFFSETS)):
            run, _ = self.build(**kw)
            self.assertEqual(run.returncode, 3, kw)
            self.assertIn('overlap', run.stdout)

    def test_text_outside_the_program(self):
        run, _ = self.build(text=PROGRAM_SIZE - 100)
        self.assertEqual(run.returncode, 3)
        self.assertIn('outside the program', run.stdout)

    def test_graphics_too_short(self):
        run, _ = self.build(gfx=GFX[:(0x10000 + 0x40) * 128])
        self.assertEqual(run.returncode, 3)
        self.assertIn('outside the graphics', run.stdout)

    def test_table_dump_is_the_linked_table(self):
        run = self.run_exe('table')
        self.assertEqual(run.returncode, 0)
        centre, height, mslot, bslot, rows = Q.parse_table_dump(run.stdout)
        self.assertEqual((mslot, bslot), (Q.MODULE_SLOT, Q.BLOB_SLOT))
        self.assertEqual(len(rows), Q.KINDS)
        self.assertEqual([r['kind'] for r in rows], list(range(Q.KINDS)))
        self.assertEqual([r['file'] for r in rows], ['CDEMO%02X.PAC' % k for k in range(Q.KINDS)])
        for r in rows:
            self.assertEqual(r['text'], Q.ARCADE_TEXT + Q.BLOCK_BYTES * Q.BLOCK_OF[r['kind']])
            self.assertEqual(r['segment'], 0x18668)
        self.assertEqual((centre, height), (160, 16))

    def test_no_dash(self):
        for p in (ARC / 'quotes.c', ARC / 'quotes.h', ARC / 'quotes.py', ARC / 'quotes_table.c', DRIVER, Path(__file__)):
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
