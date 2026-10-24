#!/usr/bin/env python3
"""Seconds: the text strings of the screen modules (port/arcade/texts.py, texts.c, texts.h); no game files.

Python side, on made-up data: the strings of a chunk (padding of one to three zeros, padding that is not zeros, leftover bytes in front,
what is not a string), the CODES view of a program (space, both flag bytes, words that are no character), the finder (CODES before PLAIN,
even addresses only, none), the derivation (rows, statistics, refusals), the file kind and summary, the generated table's text (sharing,
header, no game bytes) and the parser of the driver's table dump.

C side: texts.c, built with the address and undefined-behavior sanitizers and a driver (tests/texts_test.c): arcade_text_build on both
forms against an independent model (the 0x80 flag, padding, no byte written past the size) and every refusal (outside the program, odd
CODES source, a word or byte that is no character, chars + 3 > size, a bad form, NULL), arcade_text_module_valid's refusals (unsorted,
overlapping, outside the chunk, ...), and the generated table compiled with texts.c and built row by row; the checked-in table compiles too.
"""
from pathlib import Path
import re, struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import texts as X

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
DRIVER = ARC / 'tests/texts_test.c'


def codes(text, flag=0x00):
    """The CODES form of text (independent of texts.py): a space 0x0000, else flag then ASCII | 0x80, a word each."""
    return b''.join(b'\0\0' if c == ' ' else bytes([flag, ord(c) | 0x80]) for c in text)


def ps1(text, size=None):
    """text + "@@" + zeros up to size (default: round4(len + 3))."""
    b = text.encode() + b'@@'
    return b + b'\0' * ((size if size is not None else (len(text) + 3 + 3) // 4 * 4) - len(b))


PROGRAM = (b'\x12\x34' * 8 + codes('Hello World!') + b'\xff\xee' + codes('Flag', 0x80) + b'plain text' + b'\0' * 6 +
           codes('Both') + b'Both' + b'\0\0')


def at(needle):
    return PROGRAM.index(needle)


class Scan(unittest.TestCase):
    def test_padding_of_one_to_three_zeros(self):
        for text in ('abcde', 'abcdef', 'abcdefg', 'abcdefgh'):
            chunk = ps1(text) + ps1('zz')
            got = X.scan_strings(chunk)
            self.assertEqual(got[0], (0, text.encode(), len(ps1(text))), text)
            self.assertEqual(got[1][0], len(ps1(text)))
            self.assertEqual(len(ps1(text)) % 4, 0)
            self.assertEqual(len(got), 2)

    def test_start_after_chunk_start_and_zeros(self):
        chunk = b'\0\0\0\0' + ps1('Next one') + b'\0' * 8 + ps1('Third one')
        self.assertEqual([(o, c) for o, c, _ in X.scan_strings(chunk)], [(4, b'Next one'), (4 + len(ps1('Next one')) + 8, b'Third one')])

    def test_padding_that_is_not_zeros_ends_the_string_at_its_zeros(self):
        chunk = b'abcd@@\0\x88' + b'Next@@\0\0\0\0'                    # leftovers after the first zero: the string is 7 bytes
        self.assertEqual(X.scan_strings(chunk), [(0, b'abcd', 7), (8, b'Next', 8)])

    def test_leftover_bytes_in_front_of_a_string_are_not_part_of_it(self):
        for lead in (1, 2, 3):
            chunk = b'\0' * (8 - lead) + b'xyz'[:lead] + b'Next time@@\0\0\0\0'
            self.assertEqual(X.scan_strings(chunk), [(8, b'Next time', 12)], lead)

    def test_what_is_not_a_string(self):
        for why, chunk in (('no zero after @@', b'abc@@xyz\0\0\0\0'), ('empty', b'\0\0\0\0@@\0\0'), ('no @@', b'abcdef\0\0'),
                           ('one @', b'abcde@\0\0\0\0\0\0'), ('at the chunk end, no zero', b'\0\0\0\0abc@@'),
                           ('@@ only, after a start', b'\0\0\0@@\0\0\0\0'), ('nothing', b''), ('zeros', b'\0' * 16)):
            self.assertEqual(X.scan_strings(chunk), [], why)

    def test_text_split_by_a_control_byte_gives_its_last_piece_from_a_multiple_of_4(self):
        self.assertEqual(X.scan_strings(b'ab\x07cd@@\0\0\0\0'), [(4, b'd', 4)])

    def test_a_string_at_the_chunk_end_with_its_zero(self):
        self.assertEqual(X.scan_strings(b'abcde@@\0'), [(0, b'abcde', 8)])
        self.assertEqual(X.scan_strings(b'abcd@@\0'), [(0, b'abcd', 7)])             # chunk ends before the multiple of 4

    def test_at_signs_inside_the_text(self):
        self.assertEqual(X.scan_strings(ps1('a@@b@c')), [(0, b'a@@b@c', 12)])

    def test_size_never_passes_the_next_places(self):
        chunk = ps1('abc', 8) + b'\0' * 12
        self.assertEqual(X.scan_strings(chunk), [(0, b'abc', 8)])


class View(unittest.TestCase):
    def test_codes_view(self):
        v = X.codes_view(codes('Hi there!') + codes('Ok', 0x80) + b'\x01\xc8' + b'\x00\xa0' + b'\x80\x00' + b'\x00\xff' + b'\x80\xa1')
        self.assertEqual(v, b'Hi there!' + b'Ok' + b'\0\0\0\0' + b'!')
        self.assertEqual(X.codes_view(b'\0\0\0'), b'\x20')

    def test_finder_forms_and_addresses(self):
        f = X.Finder(PROGRAM)
        self.assertEqual(f.find(b'Hello World!'), (X.CODES, at(codes('Hello World!'))))
        self.assertEqual(f.find(b'Flag'), (X.CODES, at(codes('Flag', 0x80))))
        self.assertEqual(f.find(b'plain text'), (X.PLAIN, at(b'plain text')))
        self.assertEqual(f.find(b'Both'), (X.CODES, at(codes('Both'))))              # CODES before PLAIN
        self.assertIsNone(f.find(b'not there'))
        self.assertEqual(f.find(b'Hello World!'), (X.CODES, at(codes('Hello World!'))))   # cached

    def test_codes_only_at_even_addresses(self):
        odd = b'\0' + codes('Odd one') + b'\0'
        self.assertIsNone(X.Finder(odd).find(b'Odd one'))
        self.assertEqual(X.Finder(odd[1:]).find(b'Odd one'), (X.CODES, 0))
        # an odd occurrence falls to PLAIN when the bytes are there as such
        self.assertEqual(X.Finder(odd + b'Odd one').find(b'Odd one'), (X.PLAIN, len(odd)))

    def test_a_space_is_a_zero_word(self):
        self.assertEqual(X.Finder(codes('a b')).find(b'a b'), (X.CODES, 0))
        self.assertIsNone(X.Finder(b'\x00' + b'\xa0' + b'\x00\xe1').find(b' a'))     # 0x00a0 is no space


class Derive(unittest.TestCase):
    def chunks(self):
        a = ps1('Hello World!') + ps1('PS1 only text') + ps1('plain text')
        b = ps1('Flag')
        c = b'\0' * 32
        return [('B.PAC', 5, b), ('A.PAC', 0x28, a), ('A.PAC', 2, c)]

    def setUp(self):
        self.min_strings = X.MIN_STRINGS
        X.MIN_STRINGS = 1   # the fixtures' chunks hold one or two strings found

    def tearDown(self):
        X.MIN_STRINGS = self.min_strings

    def test_a_chunk_with_fewer_strings_found_than_the_minimum_is_no_row(self):
        X.MIN_STRINGS = 3
        rows, stats = X.derive(self.chunks(), PROGRAM)
        self.assertEqual(rows, [])
        self.assertEqual([(s['file'], s['found']) for s in stats], [('A.PAC', 2), ('B.PAC', 1)])

    def test_rows_and_statistics(self):
        rows, stats = X.derive(self.chunks(), PROGRAM)
        self.assertEqual([(r['file'], r['slot']) for r in rows], [('A.PAC', 0x28), ('B.PAC', 5)])
        a = rows[0]
        self.assertEqual(a['size'], len(self.chunks()[1][2]))
        hello = len(ps1('Hello World!'))
        self.assertEqual(a['strings'], [dict(offset=0, source=at(codes('Hello World!')), chars=12, size=hello, form=X.CODES),
                                         dict(offset=hello + len(ps1('PS1 only text')), source=at(b'plain text'), chars=10, size=len(ps1('plain text')),
                                              form=X.PLAIN)])
        self.assertEqual([(s['file'], s['total'], s['found'], s['bytes'], s['found_bytes'], s['missing']) for s in stats],
                         [('A.PAC', 3, 2, hello + len(ps1('PS1 only text')) + len(ps1('plain text')), hello + len(ps1('plain text')), [b'PS1 only text']),
                          ('B.PAC', 1, 1, 8, 8, [])])

    def test_two_chunks_in_one_slot_are_refused(self):
        with self.assertRaises(X.Fail):
            X.derive([('A.PAC', 1, b''), ('A.PAC', 1, b'')], PROGRAM)

    def test_kind_and_summary(self):
        for name, kind in (('END00.PAC', 'END'), ('END0A.PAC', 'END'), ('CONT0AX.PAC', 'CONT'), ('CDEMO13.PAC', 'CDEMO'), ('CONT00X.PAC', 'CONT'),
                           ('SELECT.PAC', 'SELECT'), ('SELECTA.PAC', 'SELECTA'), ('DEMO.PAC', 'DEMO')):
            self.assertEqual(X.kind_of(name), kind, name)
        a = ps1('Hello World!') + ps1('PS1 only text') + ps1('plain text')
        _rows, stats = X.derive([('END00.PAC', 0x28, a), ('END01.PAC', 0x28, ps1('Flag'))], PROGRAM)
        total = len(a) + 8
        found = len(ps1('Hello World!')) + len(ps1('plain text')) + 8
        self.assertEqual(X.summary_lines(stats), [f'END: 2 files, 3 of 4 strings found, {found} of {total} bytes'])


class TableText(unittest.TestCase):
    def rows(self):
        s1 = [dict(offset=0, source=0x20, chars=5, size=8, form=X.CODES), dict(offset=8, source=0x31, chars=3, size=8, form=X.PLAIN)]
        s2 = [dict(offset=4, source=0x40, chars=4, size=8, form=X.PLAIN)]
        return [dict(file='END00.PAC', slot=0x28, size=64, strings=s1), dict(file='END01.PAC', slot=0x28, size=64, strings=s1),
                dict(file='SELECT.PAC', slot=4, size=64, strings=s2)]

    def test_text(self):
        t = X.c_table(self.rows())
        self.assertEqual(t.count('static const struct arcade_text_string'), 2)            # the two ending rows share one array
        self.assertIn('{"END01.PAC", 40u, 64u, strings_end00_28, 2u},', t)
        self.assertIn('{0x8u, 0x31u, 3u, 8u, ARCADE_TEXT_PLAIN},', t)
        self.assertIn('arcade_text_module_count', t)
        self.assertTrue(t.startswith('/* GENERATED by `python3 port/arcade/texts.py --disc DISC --arcade SET --c-table FILE`'))
        self.assertNotIn(chr(0x2014), t)
        self.assertNotRegex(t, r'/home|/run/|/media|\.bin|\.zip')

    def test_equal_strings_in_unequal_sizes_do_not_share(self):
        rows = self.rows()
        rows[1] = dict(rows[1], size=128)
        self.assertEqual(X.c_table(rows).count('static const struct arcade_text_string'), 3)

    def test_dump_parser(self):
        dump = 'END00.PAC 40 64 2\n0 32 5 8 0\n8 49 3 8 1\nSELECT.PAC 4 64 1\n4 64 4 8 1\n'
        self.assertEqual(X.parse_dump(dump), self.rows()[:1] + self.rows()[2:])


_SHARED = {}


def shared():
    """The driver linked with a table generated for made-up rows, built once: (folder, exe)."""
    if not _SHARED:
        tmp = tempfile.TemporaryDirectory()
        d = Path(tmp.name)
        (d / 'table.c').write_text(X.c_table(TableC.rows()).replace('#include "texts.h"', f'#include "{ARC}/texts.h"'))
        subprocess.run(['gcc', *FLAGS, *SAN, str(ARC / 'texts.c'), str(d / 'table.c'), str(DRIVER), '-o', str(d / 'texts_test')], check=True, timeout=180)
        (d / 'program').write_bytes(PROGRAM)
        _SHARED.update(tmp=tmp, dir=d)
    return _SHARED['dir']


class CBase(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = shared()
        cls.exe = cls.dir / 'texts_test'
        cls.table_rows = TableC.rows()

    def run_exe(self, *args):
        return subprocess.run([str(self.exe), *map(str, args)], capture_output=True, text=True, timeout=60)

    def build(self, form, source, chars, size, program=None):
        d = self.dir
        if program is not None:
            (d / 'prog2').write_bytes(program)
        out = d / 'out'
        out.unlink(missing_ok=True)
        run = self.run_exe('string', d / ('prog2' if program is not None else 'program'), form, source, chars, size, out)
        return run, out.read_bytes() if run.returncode == 0 else None


class TableC(CBase):
    @staticmethod
    def rows():
        hello, flag, plain = at(codes('Hello World!')), at(codes('Flag', 0x80)), at(b'plain text')
        chunk_len = 64
        s1 = [dict(offset=0, source=hello, chars=12, size=16, form=X.CODES), dict(offset=16, source=flag, chars=4, size=8, form=X.CODES),
              dict(offset=24, source=plain, chars=10, size=16, form=X.PLAIN)]
        return [dict(file='END00.PAC', slot=0x28, size=chunk_len, strings=s1), dict(file='END01.PAC', slot=0x28, size=chunk_len, strings=s1)]

    def test_linked_table_is_the_rows(self):
        run = self.run_exe('table')
        self.assertEqual(run.returncode, 0)
        self.assertEqual(X.parse_dump(run.stdout), self.table_rows)

    def test_row_builds_the_strings(self):
        d = self.dir
        for idx in (0, 1):
            run = self.run_exe('row', idx, d / 'program', d / 'rowout')
            self.assertEqual(run.returncode, 0, run.stdout)
            self.assertEqual(run.stdout.strip(), '...')
            self.assertEqual((d / 'rowout').read_bytes(), ps1('Hello World!', 16) + ps1('Flag', 8) + ps1('plain text', 16))

    def test_row_marks_a_refused_string(self):
        d = self.dir
        (d / 'short').write_bytes(PROGRAM[:at(b'plain text') + 5])
        run = self.run_exe('row', 0, d / 'short', d / 'rowout')
        self.assertEqual(run.returncode, 0)
        self.assertEqual(run.stdout.strip(), '..x')
        self.assertEqual((d / 'rowout').read_bytes()[24:], b'\xee' * 16)

class Build(CBase):
    def test_codes_and_plain(self):
        run, out = self.build(X.CODES, at(codes('Hello World!')), 12, 16)
        self.assertEqual(run.returncode, 0, run.stdout)
        self.assertEqual(out, ps1('Hello World!', 16))
        run, out = self.build(X.PLAIN, at(b'plain text'), 10, 16)
        self.assertEqual(out, ps1('plain text', 16))

    def test_the_flag_byte_is_not_kept(self):
        run, out = self.build(X.CODES, at(codes('Flag', 0x80)), 4, 8)
        self.assertEqual(out, ps1('Flag', 8))

    def test_a_space_in_codes(self):
        run, out = self.build(X.CODES, at(codes('Hello World!')) + 2 * 5, 3, 8)       # " Wo"
        self.assertEqual(out, ps1(' Wo', 8))

    def test_padding_is_all_zeros_and_exactly_size_bytes(self):
        for size in (7, 8, 9, 11, 12):
            run, out = self.build(X.PLAIN, at(b'plain text'), 4, size)
            self.assertEqual(run.returncode, 0, size)
            self.assertEqual(out, b'plai' + b'@@' + b'\0' * (size - 6))

    def test_one_zero_is_enough_and_chars_plus_3_is_the_least_size(self):
        run, out = self.build(X.PLAIN, at(b'plain text'), 4, 7)
        self.assertEqual(out, b'plai@@\0')
        self.assertEqual(self.build(X.PLAIN, at(b'plain text'), 4, 6)[0].returncode, 3)

    def test_the_last_character_of_the_program(self):
        prog = b'\0' * 10 + b'xyz'
        run, out = self.build(X.PLAIN, 10, 3, 8, prog)
        self.assertEqual(out, b'xyz@@\0\0\0')

    def refused(self, why, *args, **kw):
        run, out = self.build(*args, **kw)
        self.assertEqual(run.returncode, 3, f'{why}: {run.stdout} {run.stderr}')
        self.assertEqual(run.stdout.strip(), 'refused', why)

    def test_refusals(self):
        h, p = at(codes('Hello World!')), at(b'plain text')
        self.refused('past the program end (codes)', X.CODES, len(PROGRAM) - 4, 3, 16)
        self.refused('past the program end (plain)', X.PLAIN, len(PROGRAM) - 2, 4, 16)
        self.refused('source beyond the program', X.PLAIN, len(PROGRAM) + 100, 1, 8)
        self.refused('source at 2^32 - 1', X.PLAIN, 0xffffffff, 4, 16)
        self.refused('a codes span that would overflow 32 bits', X.CODES, 0xfffffffe, 0xffff, 0xffff)
        self.refused('a plain span that would overflow 32 bits', X.PLAIN, 0xfffffff0, 0xffff, 0xffff)
        self.refused('odd codes source', X.CODES, h + 1, 3, 8)
        self.refused('odd codes source that would read as characters', X.CODES, 1, 3, 8, program=b'\0' + codes('abc') + b'\0')
        self.refused('a plain byte below space', X.PLAIN, at(b'\x12\x34'), 2, 8)
        self.refused('a plain byte above ~', X.PLAIN, at(b'\xff\xee'), 2, 8)
        self.refused('a plain zero byte', X.PLAIN, at(b'\0' * 6), 3, 8)
        self.refused('a codes word with the wrong high byte', X.CODES, at(b'\x12\x34'), 2, 8)
        self.refused('a codes low byte below 0xa1', X.CODES, 0, 1, 8, program=b'\x00\xa0' * 4)
        self.refused('a codes low byte 0xff', X.CODES, 0, 1, 8, program=b'\x00\xff' * 4)
        self.refused('a codes low byte 0x41 (ascii)', X.CODES, 0, 1, 8, program=b'\x00A' * 4)
        self.refused('a codes high byte 0x01', X.CODES, 0, 1, 8, program=b'\x01\xc1' * 4)
        self.refused('a codes word 0x8000', X.CODES, 0, 1, 8, program=b'\x80\x00' * 4)
        self.refused('chars + 3 > size', X.PLAIN, p, 10, 12)
        self.refused('chars + 3 > size by a lot', X.PLAIN, p, 10, 5)
        self.refused('size 0', X.PLAIN, p, 0, 0)
        self.refused('chars 65535 and size 65535', X.PLAIN, 0, 0xffff, 0xffff)
        self.refused('a bad form', 2, p, 4, 8)
        self.refused('a bad form 255', 255, p, 4, 8)

    def test_accepts_the_edges(self):
        self.assertEqual(self.build(X.PLAIN, at(b'plain text'), 10, 13)[0].returncode, 0)
        self.assertEqual(self.build(X.PLAIN, at(b'plain text'), 10, 16)[0].returncode, 0)
        self.assertEqual(self.build(X.CODES, at(codes('Both')), 4, 8)[0].returncode, 0)
        # zero characters: only "@@" and the zeros
        self.assertEqual(self.build(X.PLAIN, 0, 0, 4)[1], b'@@\0\0')
        self.assertEqual(self.build(X.PLAIN, 0, 0, 3)[1], b'@@\0')

    def test_the_end_of_the_program_is_usable(self):
        prog = codes('abc')
        run, out = self.build(X.CODES, 0, 3, 8, prog)
        self.assertEqual(out, ps1('abc', 8))
        self.refused('one word more', X.CODES, 0, 4, 8, program=prog)

    def test_nulls(self):
        run = self.run_exe('nulls')
        self.assertEqual(run.stdout.strip(), 'ok')


class Valid(CBase):
    def valid(self, size, strings, count=None):
        d = self.dir
        text = f'X.PAC 1 {size} {len(strings) if count is None else count}\n' + ''.join(' '.join(map(str, s)) + '\n' for s in strings)
        (d / 'spec').write_text(text)
        run = self.run_exe('valid', d / 'spec')
        self.assertIn(run.returncode, (0, 3), run.stderr)
        return run.returncode == 0

    def test_valid(self):
        self.assertTrue(self.valid(64, [(0, 0, 4, 8, 1), (8, 0, 5, 8, 1), (16, 4, 3, 8, 0), (24, 0, 12, 16, 1), (40, 0, 2, 8, 1)]))
        self.assertTrue(self.valid(64, []))
        self.assertTrue(self.valid(64, [(56, 0, 5, 8, 1)]))                         # to the last byte
        self.assertTrue(self.valid(64, [(0, 0, 4, 7, 1), (7, 0, 4, 7, 1)]))         # sizes need no multiple of 4 (a string ends at its zeros)

    def test_refusals(self):
        for why, strings in (('unsorted', [(16, 0, 4, 8, 1), (0, 0, 4, 8, 1)]),
                             ('overlapping', [(0, 0, 4, 8, 1), (7, 0, 4, 8, 1)]),
                             ('the same place twice', [(0, 0, 4, 8, 1), (0, 0, 4, 8, 1)]),
                             ('outside the chunk', [(60, 0, 4, 8, 1)]),
                             ('starting past the chunk', [(65, 0, 1, 4, 1)]),
                             ('offset near 2^32', [(0xfffffffc, 0, 1, 4, 1)]),
                             ('a bad form', [(0, 0, 4, 8, 2)]),
                             ('chars + 3 > size', [(0, 0, 6, 8, 1)]),
                             ('an odd codes source', [(0, 1, 2, 8, 0)])):
            self.assertFalse(self.valid(64, strings), why)

    def test_a_row_without_a_strings_array(self):
        # the driver builds the array from the spec; a count of 0 with a valid module is fine, and the NULL cases are the `nulls` command's
        self.assertTrue(self.valid(0, []))


class Sources(unittest.TestCase):
    def test_checked_in_table_compiles_with_the_c(self):
        with tempfile.TemporaryDirectory() as d:
            exe = Path(d) / 'x'
            subprocess.run(['gcc', *FLAGS, str(ARC / 'texts.c'), str(ARC / 'texts_table.c'), str(DRIVER), '-o', str(exe)], check=True, timeout=120)
            out = subprocess.run([str(exe), 'table'], capture_output=True, text=True, check=True).stdout
        rows = X.parse_dump(out)
        self.assertTrue(rows)
        for r in rows:
            self.assertEqual([s['offset'] for s in r['strings']], sorted(s['offset'] for s in r['strings']))
            self.assertTrue(all(s['offset'] + s['size'] <= r['size'] for s in r['strings']))
        ends = [r for r in rows if r['file'].startswith('END') and r['slot'] == 0x28]
        self.assertEqual(len(ends), 21)
        self.assertTrue(all(e['strings'] == ends[0]['strings'] for e in ends))

    def test_checked_in_table_has_no_game_bytes_or_paths(self):
        text = (ARC / 'texts_table.c').read_text()
        self.assertTrue(text.startswith('/* GENERATED by `python3 port/arcade/texts.py'))
        self.assertNotRegex(text, r'/home|/run/|/media|\.bin\b|\.zip')
        body = text[text.index('*/') + 2:]
        allowed = (r'', r'#include "texts.h"', r'static const struct arcade_text_string strings_\w+\[\] = \{', r'\};',
                   r'    \{0x[0-9a-f]+u, 0x[0-9a-f]+u, \d+u, \d+u, ARCADE_TEXT_(CODES|PLAIN)\},',
                   r'const struct arcade_text_module arcade_text_modules\[\] = \{',
                   r'    \{"[A-Z0-9]+\.PAC", \d+u, \d+u, strings_\w+, \d+u\},',
                   r'const unsigned arcade_text_module_count = sizeof arcade_text_modules / sizeof arcade_text_modules\[0\];')
        for line in body.split('\n'):
            self.assertTrue(any(re.fullmatch(a, line) for a in allowed), line)

    def test_no_dash(self):
        for p in (ARC / 'texts.c', ARC / 'texts.h', ARC / 'texts.py', ARC / 'tests/texts_test.c', Path(__file__)):
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
