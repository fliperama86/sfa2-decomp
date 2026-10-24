#!/usr/bin/env python3
"""Seconds: the talk before a fight in the arcade's English (port/arcade/talk.py, talk.c, talk.h); no game files.

Python side, on made-up data: the arcade's boxes and line counts, the model of the page and of the script, the reading of a module's scripts
(records, end bit, refusals), the derivation of the table from a made-up disc (script rows, alias pages, the GDEMO rows, refusals), the generated
table's text, the palette read from the source, the PNG and the drawing check on a good and a damaged page.

C side: talk.c, built with the address and undefined-behavior sanitizers and a driver (tests/talk_test.c), on a made-up program, graphics and
module: arcade_talk_page_build against the model (pixel placement, blank words, a talk of fewer boxes, the GDEMO box order) and
arcade_talk_script_build (only the w2s written, the line counts, the mask), and every refusal.
"""
from pathlib import Path
import random, struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import talk as T
import quotes as Q

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
DRIVER = ARC / 'tests/talk_test.c'

PROGRAM_SIZE = 0x4000
TEXT = 0x400                           # the made-up talk: 4 boxes of 3 lines of 32 words
MODULE_SIZE = 0x200
SCRIPT, RECORDS = 0x40, 5


def make_gfx():
    rnd = random.Random(11)
    g = bytearray((0x10000 + 0xa0) * 128)
    g[0x10000 * 128:] = rnd.randbytes(0xa0 * 128)
    return bytes(g)


GFX = make_gfx()


def glyphs(c):
    return Q.glyph(GFX, c)


def word(c):
    return b'\0\0' if c == ' ' else struct.pack('>H', ord(c) | 0x80)


def line_words(s, lead=False):
    s = s[:32] + ' ' * (32 - len(s))
    b = b''.join(word(c) for c in s)
    return (b'\x80\x00' + b[2:]) if lead and s[0] == ' ' else b


def make_program(boxes, text=TEXT):
    """boxes: list of (up to 3) strings a box, '\\n' between lines."""
    p = bytearray(PROGRAM_SIZE)
    for b, box in enumerate(boxes):
        lines = box.split('\n') if box else []
        for l in range(3):
            at = text + b * 192 + l * 64
            p[at:at + 64] = line_words(lines[l] if l < len(lines) else '', lead=(b + l) % 2 == 1)
    return bytes(p)


BOXES = ['Hi there\nfriend', 'A', 'x\n\nlast line  ', '  indented\nRight edge is col 32...........']


def make_module(boxes_in_order=(0, 1, 2, 3), w2=7):
    m = bytearray(random.Random(5).randbytes(MODULE_SIZE))
    for i, b in enumerate(boxes_in_order):
        m[SCRIPT + 6 * i:SCRIPT + 6 * i + 6] = struct.pack('<3H', b | (i & 1) << 8, 0xf0 | (i & 1) << 15, w2)
    m[SCRIPT + 6 * len(boxes_in_order):SCRIPT + 6 * len(boxes_in_order) + 6] = struct.pack('<3H', 0x8000, 0, 0x100)
    return bytes(m)


def row(**kw):
    r = dict(file='RDM99.PAC', text=TEXT, boxes=4, box_of=(0, 1, 2, 3), script=SCRIPT, records=RECORDS)
    r.update(kw)
    return r


class Model(unittest.TestCase):
    def test_line_counts(self):
        self.assertEqual(T.box_line_count(['a', '', '']), 1)
        self.assertEqual(T.box_line_count(['a', 'b', '']), 2)
        self.assertEqual(T.box_line_count(['', 'b', '']), 2)          # a blank line before text counts
        self.assertEqual(T.box_line_count(['a', '', 'c']), 3)
        self.assertEqual(T.box_line_count(['  ', ' ', '']), 1)        # at least 1

    def test_arcade_box_repeats_the_last(self):
        r = row(boxes=3)
        self.assertEqual([T.arcade_box(r, b) for b in range(4)], [0, 1, 2, 2])
        r = row(boxes=3, box_of=(0, 1, 2, 0))
        self.assertEqual([T.arcade_box(r, b) for b in range(4)], [0, 1, 2, 0])

    def test_model_page_places_glyphs(self):
        prog = make_program(BOXES)
        page = T.model_page(row(), prog, glyphs)
        px = T.page_pixels(page)
        # box 1 is 'A' at column 0 of line 0: rows 48 to 63, x 0 to 7
        g = glyphs(ord('A'))
        for y in range(16):
            self.assertEqual(px[48 + y][:8], g[y])
        # box 0 line 1 is 'friend' from column 0: row 16 and column 0 hold 'f'
        for y in range(16):
            self.assertEqual(px[16 + y][:8], glyphs(ord('f'))[y])
        # the rest of the page is 0 below the 4 boxes
        self.assertFalse(any(page[48 * 4 * 128:]))
        # a space draws nothing and a 0x8000 word is a space
        self.assertEqual(Q.word_char(b'\x80\x00', 0), 0x20)

    def test_boxes_beyond_the_talk_repeat(self):
        prog = make_program(BOXES)
        a = T.page_pixels(T.model_page(row(boxes=3), prog, glyphs))
        self.assertEqual(a[144:192], a[96:144])                       # box 3 is box 2 again
        b = T.page_pixels(T.model_page(row(boxes=3, box_of=(0, 1, 2, 0)), prog, glyphs))
        self.assertEqual(b[144:192], b[0:48])                         # the GDEMO order: box 3 is box 0

    def test_read_back_finds_the_characters(self):
        prog = make_program(BOXES)
        page = T.model_page(row(), prog, glyphs)
        back = T.read_back(page, glyphs)
        s = T.checked_box(prog, TEXT, 0)
        for l in range(3):
            self.assertTrue(all(c is not None and s[l][k] in c for k, c in enumerate(back[l])))

    def test_model_script_writes_only_w2(self):
        prog, module = make_program(BOXES), make_module()
        out, mask = T.model_script(row(), prog, module)
        counts = [T.box_line_count(T.checked_box(prog, TEXT, b)) for b in range(4)]
        self.assertEqual(counts, [2, 1, 3, 2])
        for i in range(4):
            self.assertEqual(struct.unpack_from('<H', out, SCRIPT + 6 * i + 4)[0], counts[i])
        self.assertEqual(sum(mask), 8)
        self.assertEqual(out[:SCRIPT], module[:SCRIPT])
        self.assertEqual(out[SCRIPT + 24:], module[SCRIPT + 24:])      # the end record is not written

    def test_model_script_ends_early(self):
        prog, module = make_program(BOXES), make_module()
        out, mask = T.model_script(row(boxes=3), prog, module)
        self.assertEqual(sum(mask), 7)
        self.assertEqual(out[SCRIPT + 19], module[SCRIPT + 19] | 0x80)
        self.assertEqual(mask[SCRIPT + 19], 1)
        self.assertEqual(sum(mask[SCRIPT + 20:]), 0)
        out, mask = T.model_script(row(boxes=2, box_of=(0, 1, 0, 1)), prog, module)    # every page box exists: no early end
        self.assertEqual(sum(mask), 8)
        self.assertTrue(all(not (out[SCRIPT + 6 * i + 1] & 0x80) for i in range(4)))

    def test_checked_box_refuses_outside_the_font(self):
        p = bytearray(make_program(BOXES))
        p[TEXT + 4:TEXT + 6] = b'\x12\xc1'
        with self.assertRaises(T.Fail):
            T.checked_box(bytes(p), TEXT, 0)
        with self.assertRaises(T.Fail):
            T.checked_box(bytes(p), PROGRAM_SIZE - 10, 0)


class Reading(unittest.TestCase):
    def test_parse_script(self):
        m = make_module()
        recs, count = T.parse_script(m, SCRIPT)
        self.assertEqual([r[0] for r in recs], [0, 1, 2, 3])
        self.assertEqual(count, 5)
        self.assertEqual([r[1] for r in recs], [0, 1, 0, 1])
        self.assertEqual(T.parse_script(make_module((3, 1, 2)), SCRIPT)[1], 4)

    def test_parse_script_refusals(self):
        with self.assertRaises(T.Fail):
            T.parse_script(make_module((0, 1, 4)), SCRIPT)             # box 4
        with self.assertRaises(T.Fail):
            T.parse_script(bytes(0x100), 0)                            # never ends
        with self.assertRaises(T.Fail):
            T.parse_script(b'\0' * 0x40, 0x3c)                         # runs past the module

    def test_japanese_rows(self):
        page = bytearray(T.PAGE_BYTES)
        page[(48 * 1 + 0) * 128] = 1                                   # a pixel in box 1 row 0
        page[(48 * 1 + 40) * 128 + 5] = 3                              # and in box 1 row 40 (line 2)
        page[(48 * 2 + 16) * 128] = 2
        self.assertEqual([T.japanese_rows(bytes(page), b) for b in range(4)], [0, 2, 1, 0])

    def test_palette_is_read_from_the_source(self):
        pal = T.module_palette()
        self.assertEqual(len(pal), 16)
        self.assertEqual(pal[0], (20, 24, 60))
        self.assertTrue(all(0 <= c <= 255 for p in pal for c in p))


def make_disc():
    """A made-up Disc: 63 files with one module (the table of 44 pointers at 0x1bdc, the scripts, GDEMO's two) and pages whose rows per box equal
    each script's w2; files 12, 13 and 14 carry the pages of 04, 11 and 02."""
    m = bytearray(0x2000)
    base = T.MODULE_BASE
    scripts = []
    rnd = random.Random(2)
    at = 0x1000
    for i in range(T.SCRIPT_ENTRIES):
        if i in (18, 19, 20) or i in (39, 40, 41):
            continue
        lines = [rnd.randint(1, 3) for _ in range(4)] if i not in T.GDEMO_SCRIPTS else [1, 1, 2]
        boxes = (0, 1, 2, 3) if i != 43 else (3, 1, 2)
        if i == 42:
            boxes = (0, 1, 2)
        recs = b''.join(struct.pack('<3H', b | (j & 1) << 8, 0xf0, n) for j, (b, n) in enumerate(zip(boxes, lines)))
        recs += struct.pack('<3H', 0x8000, 0, 0)
        m[at:at + len(recs)] = recs
        scripts.append((i, at, dict(zip(boxes, lines))))
        at += len(recs)
    where = {i: a for i, a, _l in scripts}
    for i in (18, 19, 20):
        where[i] = where[{18: 4, 19: 17, 20: 2}[i]]
    for i in (39, 40, 41):
        where[i] = where[{39: 25, 40: 38, 41: 23}[i]]
    for i in range(T.SCRIPT_ENTRIES):
        struct.pack_into('<I', m, T.SCRIPT_TABLE + 4 * i, base + where[i])
    lines_of = {i: l for i, _a, l in scripts}
    for i, l in ((18, lines_of[4]), (19, lines_of[17]), (20, lines_of[2]), (39, lines_of[25]), (40, lines_of[38]), (41, lines_of[23])):
        lines_of[i] = l

    def page_of(lines):
        p = bytearray(T.PAGE_BYTES)
        for b, n in lines.items():
            for r in range(n):
                p[(48 * b + 16 * r) * 128] = 1
        return bytes(p)

    class D:
        pass
    d = D()
    d.files = {}
    for fam, off in (('BOSS', 0), ('RDM', 0x15)):
        for k in range(0x15):
            d.files['%s%02X.PAC' % (fam, k)] = (bytes(m), page_of(lines_of[off + k]))
    for k in range(0x15):
        d.files['GDEMO%02X.PAC' % k] = (bytes(m), page_of({0: 1, 1: 1, 2: 2, 3: 1}))
    return d


class Derive(unittest.TestCase):
    def prog(self):
        return bytes(0xf6000)

    def test_rows_of_a_made_up_disc(self):
        d = T.derive(make_disc(), self.prog())
        rows = d['rows']
        self.assertEqual(len(rows), 21 * 2 + 21 * 2)
        self.assertEqual([r['file'] for r in rows[:2]], ['RDM00.PAC', 'RDM01.PAC'])
        self.assertEqual(rows[21]['file'], 'BOSS00.PAC')
        r0, b0, b2, bd = rows[0], rows[21], rows[21 + 2], rows[21 + 0x0d]
        # RDM00 is the pair 0's first talk, BOSS00 its second (4 boxes each), the starts 4 boxes apart
        self.assertEqual((r0['text'], b0['text']), (T.BOX0 + 192, T.BOX0 + 192 * 5))
        self.assertEqual((r0['boxes'], r0['box_of'], r0['records']), (4, (0, 1, 2, 3), 5))
        # BOSS02 is T23: 3 boxes; BOSS0D is T35: 5 boxes of which 4 are drawn
        self.assertEqual((b2['boxes'], bd['boxes']), (3, 4))
        self.assertEqual(bd['text'], T.BOX0 + 192 * 140)
        # the next talk after BOSS02's three starts 3 boxes later: RDM0C is T24 (box 96)
        self.assertEqual(rows[0x0c]['text'], T.BOX0 + 192 * 96)
        # aliases take the talk of their kind
        self.assertEqual(rows[0x12]['text'], rows[0x04]['text'])
        self.assertEqual(rows[0x13]['text'], rows[0x11]['text'])
        self.assertEqual(rows[21 + 0x14]['text'], rows[21 + 2]['text'])
        # GDEMO: two rows a file, the Akuma talk, box order 0 1 2 0, the two scripts
        g = rows[42:]
        self.assertEqual(len(g), 42)
        self.assertEqual((g[0]['file'], g[1]['file'], g[2]['file']), ('GDEMO00.PAC', 'GDEMO00.PAC', 'GDEMO01.PAC'))
        self.assertEqual((g[0]['text'], g[0]['boxes'], g[0]['box_of']), (T.BOX0 + 192 * 153, 3, (0, 1, 2, 0)))
        self.assertNotEqual(g[0]['script'], g[1]['script'])
        self.assertEqual((g[0]['records'], g[1]['records']), (4, 4))

    def test_starts_end_at_box_144(self):
        g, starts = 1, []
        for t in range(T.TALKS):
            starts.append(g)
            g += T.BOXES_OF_TALK.get(t, 4)
        self.assertEqual(g, 145)
        self.assertEqual(starts[23], 93)
        self.assertEqual(starts[24], 96)
        self.assertEqual(starts[35], 140)

    def test_refusals(self):
        d = make_disc()
        mod, page = d.files['RDM03.PAC']
        d.files['RDM03.PAC'] = (mod, bytes(len(page)))                      # a page whose rows differ from its script
        with self.assertRaises(T.Fail):
            T.derive(d, self.prog())
        d = make_disc()
        d.files['RDM12.PAC'] = (d.files['RDM12.PAC'][0], bytes(T.PAGE_BYTES))     # an alias that is not its kind's page
        with self.assertRaises(T.Fail):
            T.derive(d, self.prog())
        d = make_disc()
        m, p = d.files['BOSS05.PAC']
        d.files['BOSS05.PAC'] = (m[:-1] + b'\1', p)                               # a module that differs
        with self.assertRaises(T.Fail):
            T.derive(d, self.prog())
        d = make_disc()
        d.files['GDEMO07.PAC'] = (d.files['GDEMO07.PAC'][0], d.files['RDM00.PAC'][1])
        with self.assertRaises(T.Fail):
            T.derive(d, self.prog())
        with self.assertRaises(T.Fail):
            T.derive(make_disc(), bytes(0x1000))                                  # the talks lie outside the program

    def test_c_table_text(self):
        d = T.derive(make_disc(), self.prog())
        t = T.c_table(d)
        self.assertIn('{"RDM00.PAC", 0xeea88, 4, {0, 1, 2, 3}, ', t)
        self.assertIn('{"GDEMO00.PAC", 0xf5c88, 3, {0, 1, 2, 0}, ', t)
        self.assertIn('arcade_talk = {2, 0, 1, 0x2a, pages, sizeof pages / sizeof pages[0]};', t)
        self.assertEqual(t.count('GDEMO05.PAC'), 2)
        self.assertNotIn(chr(0x2014), t)

    def test_table_dump_parse(self):
        head, rows = T.parse_table_dump('2 0 1 42 1\nRDM00.PAC 0xeea88 4 0 1 2 3 0x19bc 5\n')
        self.assertEqual(head, (2, 0, 1, 42))
        self.assertEqual(rows, [dict(file='RDM00.PAC', text=0xeea88, boxes=4, box_of=(0, 1, 2, 3), script=0x19bc, records=5)])


class CBase(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory()
        d = Path(cls.dir.name)
        cls.exe = d / 'talk_test'
        subprocess.run(['gcc', *FLAGS, *SAN, str(ARC / 'talk.c'), str(ARC / 'talk_table.c'), str(DRIVER), '-o', str(cls.exe)], check=True, timeout=240)
        (d / 'gfx').write_bytes(GFX)

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_exe(self, *args):
        return subprocess.run([str(self.exe), *map(str, args)], capture_output=True, text=True, timeout=120)

    def args(self, r):
        return [r['file'], r['text'], r['boxes']] + list(r['box_of']) + [r['script'], r['records']]

    def page(self, program=None, gfx=None, **kw):
        d = Path(self.dir.name)
        (d / 'program').write_bytes(program if program is not None else make_program(BOXES))
        if gfx is not None:
            (d / 'gfx_short').write_bytes(gfx)
        out = d / 'out'
        out.mkdir(exist_ok=True)
        (out / 'page').unlink(missing_ok=True)
        run = self.run_exe('page', *self.args(row(**kw)), d / 'program', d / ('gfx_short' if gfx is not None else 'gfx'), out)
        return run, (out / 'page').read_bytes() if (out / 'page').exists() else None

    def script(self, program=None, module=None, **kw):
        d = Path(self.dir.name)
        (d / 'program').write_bytes(program if program is not None else make_program(BOXES))
        (d / 'module').write_bytes(module if module is not None else make_module())
        out = d / 'out'
        out.mkdir(exist_ok=True)
        for n in ('module', 'changed'):
            (out / n).unlink(missing_ok=True)
        run = self.run_exe('script', *self.args(row(**kw)), d / 'program', d / 'module', out)
        got = ((out / 'module').read_bytes(), (out / 'changed').read_bytes()) if (out / 'module').exists() else (None, None)
        return (run,) + got


class Page(CBase):
    def test_happy_case_against_the_model(self):
        run, page = self.page()
        self.assertEqual(run.returncode, 0, run.stdout + run.stderr)
        self.assertEqual(page, T.model_page(row(), make_program(BOXES), glyphs))
        self.assertEqual(len(page), T.PAGE_BYTES)
        self.assertTrue(any(page))
        self.assertFalse(any(page[48 * 4 * 128:]))

    def test_few_boxes_and_the_gdemo_order(self):
        for kw in (dict(boxes=3), dict(boxes=1), dict(boxes=3, box_of=(0, 1, 2, 0)), dict(boxes=2, box_of=(3, 2, 1, 0))):
            run, page = self.page(**kw)
            self.assertEqual(run.returncode, 0, kw)
            self.assertEqual(page, T.model_page(row(**kw), make_program(BOXES), glyphs), kw)

    def test_blank_talk_is_a_zero_page(self):
        run, page = self.page(program=bytes(PROGRAM_SIZE))
        self.assertEqual(run.returncode, 0)
        self.assertEqual(page, bytes(T.PAGE_BYTES))

    def test_characters_outside_the_font(self):
        p = bytearray(make_program(BOXES))
        for bad in (b'\x00\xff', b'\x12\xc1', b'\x00\x41', b'\x80\x7f', b'\x01\x00'):
            p[TEXT + 20:TEXT + 22] = bad
            run, _ = self.page(program=bytes(p))
            self.assertEqual(run.returncode, 3, bad)
            self.assertIn('outside the font', run.stdout)
        # a bad word in a box that the page does not draw (box 3 with boxes=3 and box_of 0 1 2 2) is not read
        p[TEXT + 20:TEXT + 22] = b'\x12\xc1'
        q = bytearray(make_program(BOXES))
        q[TEXT + 3 * 192 + 4:TEXT + 3 * 192 + 6] = b'\x12\xc1'
        run, _ = self.page(program=bytes(q), boxes=3)
        self.assertEqual(run.returncode, 0, run.stdout)

    def test_text_outside_the_program(self):
        run, _ = self.page(text=PROGRAM_SIZE - 100)
        self.assertEqual(run.returncode, 3)
        self.assertIn('outside the program', run.stdout)
        run, _ = self.page(text=PROGRAM_SIZE - 3 * 192 - 1, boxes=4)
        self.assertEqual(run.returncode, 3)
        run, _ = self.page(text=PROGRAM_SIZE - 3 * 192, boxes=3, box_of=(0, 1, 2, 2))      # exactly fits
        self.assertEqual(run.returncode, 0, run.stdout)

    def test_bad_box_counts_and_orders(self):
        for kw in (dict(boxes=0), dict(boxes=5), dict(box_of=(0, 1, 2, 4)), dict(box_of=(9, 1, 2, 3))):
            run, _ = self.page(**kw)
            self.assertEqual(run.returncode, 3, kw)

    def test_graphics_too_short(self):
        run, _ = self.page(gfx=GFX[:(0x10000 + 0x40) * 128])
        self.assertEqual(run.returncode, 3)
        self.assertIn('outside the graphics', run.stdout)


class Script(CBase):
    def test_happy_case_against_the_model(self):
        module = make_module()
        run, new, changed = self.script(module=module)
        self.assertEqual(run.returncode, 0, run.stdout + run.stderr)
        want, mask = T.model_script(row(), make_program(BOXES), module)
        self.assertEqual(new, want)
        self.assertEqual(changed, mask)
        counts = [struct.unpack_from('<H', new, SCRIPT + 6 * i + 4)[0] for i in range(4)]
        self.assertEqual(counts, [2, 1, 3, 2])
        self.assertTrue(all(a == b for a, b, c in zip(new, module, changed) if not c))

    def test_the_box_a_record_shows(self):
        module = make_module((3, 1, 2))
        run, new, _ = self.script(module=module, records=4, boxes=3, box_of=(0, 1, 2, 0))
        self.assertEqual(run.returncode, 0, run.stdout)
        counts = [struct.unpack_from('<H', new, SCRIPT + 6 * i + 4)[0] for i in range(3)]
        self.assertEqual(counts, [2, 1, 3])                    # record 0 shows page box 3, drawn from arcade box 0: no early end

    def test_the_talk_ends_early_at_a_box_the_arcade_lacks(self):
        module = make_module()
        run, new, changed = self.script(module=module, boxes=3)       # page box 3 has no arcade box (box_of 3 >= 3): record 3 ends the talk
        self.assertEqual(run.returncode, 0, run.stdout)
        want, mask = T.model_script(row(boxes=3), make_program(BOXES), module)
        self.assertEqual((new, changed), (want, mask))
        counts = [struct.unpack_from('<H', new, SCRIPT + 6 * i + 4)[0] for i in range(3)]
        self.assertEqual(counts, [2, 1, 3])
        end = SCRIPT + 6 * 3
        self.assertTrue(new[end + 1] & 0x80)
        self.assertEqual(new[end], module[end])                         # the box byte is kept
        self.assertEqual(new[end + 2:end + 6], module[end + 2:end + 6])  # w1 and w2 are left
        self.assertEqual(sum(changed), 3 * 2 + 1)

    def test_only_the_first_such_record_ends_the_talk_and_the_rest_is_left(self):
        module = make_module()
        run, new, changed = self.script(module=module, boxes=2)       # boxes 2 and 3 are lacking: record 2 ends it, record 3 is not touched
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual(sum(changed), 2 * 2 + 1)
        self.assertTrue(new[SCRIPT + 6 * 2 + 1] & 0x80)
        self.assertEqual(new[SCRIPT + 18:SCRIPT + 36], module[SCRIPT + 18:SCRIPT + 36])
        run, new, changed = self.script(module=module, boxes=1, box_of=(0, 0, 0, 0))   # every box exists: nothing ends early
        self.assertEqual(run.returncode, 0)
        self.assertEqual(sum(changed), 8)
        run, new, changed = self.script(module=module, boxes=1)        # box_of 0 1 2 3 with 1 box: record 1 ends it
        self.assertEqual(sum(changed), 2 + 1)

    def test_the_end_bit_is_set_in_a_record_already_marked_by_side(self):
        module = make_module()
        self.assertTrue(module[SCRIPT + 6 + 1] & 1)                      # record 1 has the side bit in w0's high byte
        run, new, _ = self.script(module=module, boxes=1)
        self.assertEqual(new[SCRIPT + 6 + 1], module[SCRIPT + 6 + 1] | 0x80)

    def test_a_blank_box_has_one_line(self):
        run, new, _ = self.script(program=make_program(['a', '', 'b', 'c']))
        self.assertEqual(run.returncode, 0)
        self.assertEqual(struct.unpack_from('<H', new, SCRIPT + 6 + 4)[0], 1)

    def test_refusals_leave_the_module_alone(self):
        cases = [dict(script=MODULE_SIZE - 12), dict(records=0), dict(records=RECORDS + 1), dict(records=RECORDS - 1)]
        for kw in cases:
            run, new, _ = self.script(**kw)
            self.assertEqual(run.returncode, 3, (kw, run.stdout))
            self.assertIsNone(new)
        run, new, _ = self.script(module=make_module((0, 1, 4, 3)))                # a box past 3
        self.assertEqual(run.returncode, 3)
        self.assertIn('past 3', run.stdout)
        m = bytearray(make_module())
        m[SCRIPT + 6 + 1] |= 0x80                                                  # an end bit before the last record
        run, new, _ = self.script(module=bytes(m))
        self.assertEqual(run.returncode, 3)
        m = bytearray(make_module())
        m[SCRIPT + 6 * 4 + 1] = 0                                                  # the last record without it
        run, new, _ = self.script(module=bytes(m))
        self.assertEqual(run.returncode, 3)
        self.assertIn('no end bit', run.stdout)

    def test_a_character_outside_the_font_in_a_counted_box(self):
        p = bytearray(make_program(BOXES))
        p[TEXT + 2 * 192 + 8:TEXT + 2 * 192 + 10] = b'\x12\xc1'
        run, new, _ = self.script(program=bytes(p))
        self.assertEqual(run.returncode, 3)
        self.assertIsNone(new)


class Table(CBase):
    def test_table_dump_is_the_linked_table(self):
        run = self.run_exe('table')
        self.assertEqual(run.returncode, 0)
        head, rows = T.parse_table_dump(run.stdout)
        self.assertEqual(head, (2, 0, 1, 0x2a))
        self.assertEqual(len(rows), 84)
        self.assertEqual(rows[0]['file'], 'RDM00.PAC')
        self.assertEqual(sum(1 for r in rows if r['file'].startswith('GDEMO')), 42)
        for r in rows:
            self.assertTrue(r['text'] >= T.BOX0 + 192 and 1 <= r['boxes'] <= 4 and 1 <= r['records'] <= 6, r)
            self.assertEqual(r['box_of'], (0, 1, 2, 0) if r['file'].startswith('GDEMO') else (0, 1, 2, 3))

    def test_no_dash(self):
        for p in (ARC / 'talk.c', ARC / 'talk.h', ARC / 'talk.py', ARC / 'talk_table.c', DRIVER, Path(__file__)):
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
