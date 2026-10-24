#!/usr/bin/env python3
"""Seconds: the code ranges and data pins of the module chunks (port/arcade/code_pins.py, code_pins.c, code_pins.h); no game files.

Python side, on made-up data: the merge of ranges, the established functions (a plain image, a `like` image moved by the difference of the
addresses, leave_out, a function without a size), the derivation of the ranges (the established ones outside the chunk are refused; inventory
rows already covered, accepted by the `jr ra` check, and skipped for each reason), zeroing and the data pin, the whole derivation over made-up
images (a wrong pin is refused, a `like` image's rows compared with its base's), the classes of images, and the generated table's text.

C side: code_pins.c, built with the address and undefined-behavior sanitizers with a driver (tests/code_pins_test.c) and a table that the
generator writes for made-up rows: arcade_code_pin_find (found, not found), arcade_code_zero (zeroes exactly the ranges and agrees with the
Python zeroing and data pin; refuses a wrong size, a range outside the chunk, a range whose end overflows; a row with no ranges), and that
the generated table compiles with code_pins.c.
"""
from pathlib import Path
import struct, subprocess, sys, tempfile, time, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import code_pins as X

ARC = ROOT / 'port/arcade'
FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
DRIVER = ARC / 'tests/code_pins_test.c'
JR, NOP = struct.pack('<I', X.JR_RA), b'\0\0\0\0'


def words(n, fill=0x11):
    return bytes([fill]) * (4 * n)


def func(n, fill=0x11, ends_jr=True):
    """A made-up function of n words (n >= 2): filler, then `jr ra` and its delay slot, or filler."""
    return words(n - 2, fill) + (JR + NOP if ends_jr else words(2, fill))


def make_chunk():
    """(chunk, address, est, rows): A (6 words, established), B (4 words, inventory, jr ra), C (5 words, inventory, no jr ra), a gap, D (3 words,
    established, adjacent to A), filler at the end. Addresses from 0x80100000."""
    parts = [func(6, 0x21), func(3, 0x22, False), func(4, 0x23), func(5, 0x24, False), words(2, 0x25), func(3, 0x26)]
    chunk = b''.join(parts)
    base = 0x80100000
    return chunk, base


def pin_rows(chunk, ranges, name='img'):
    return dict(name=name, size=len(chunk), pin=X.sha(chunk), data_pin=X.sha(X.zeroed(chunk, ranges)), ranges=ranges)


class Python(unittest.TestCase):
    def test_merge(self):
        self.assertEqual(X.merge([]), [])
        self.assertEqual(X.merge([(8, 4), (0, 4)]), [(0, 4), (8, 4)])
        self.assertEqual(X.merge([(0, 4), (4, 4)]), [(0, 8)])
        self.assertEqual(X.merge([(0, 10), (4, 2), (8, 8)]), [(0, 16)])
        self.assertEqual(X.merge([(0, 0), (8, 4)]), [(8, 4)])
        self.assertEqual(X.total([(0, 8), (20, 4)]), 12)

    def test_established(self):
        images = [dict(name='a', address=0x1000), dict(name='b', like='a', address=0x3000), dict(name='c', like='a', address=0x5000, leave_out=['u2'])]
        by = {i['name']: i for i in images}
        funcs = {'a': [('f', 'u1', 0x1000, 16), ('g', 'u2', 0x1040, 8)], None: [('h', 'u9', 0x80, 4)]}
        self.assertEqual(X.established(by['a'], by, funcs), [(0x1000, 16), (0x1040, 8)])
        self.assertEqual(X.established(by['b'], by, funcs), [(0x3000, 16), (0x3040, 8)])
        self.assertEqual(X.established(by['c'], by, funcs), [(0x5000, 16)])
        with self.assertRaises(X.Fail):
            X.established(by['a'], by, {'a': [('f', 'u1', 0x1000, 0)]})
        by['d'] = dict(name='d', like='b', address=0x7000)
        with self.assertRaises(X.Fail):
            X.established(by['d'], by, funcs)

    def test_the_chunk_first_word_is_never_zeroed(self):
        chunk = bytearray(32)
        struct.pack_into('<I', chunk, 0, 0x27bdffe8)
        r = X.derive_ranges(0x1000, bytes(chunk), [(0x1000, 4), (0x1008, 8)], [])
        self.assertEqual(r['ranges'], [(8, 8)])   # a function of the first word alone leaves nothing
        r = X.derive_ranges(0x1000, bytes(chunk), [(0x1000, 12)], [])
        self.assertEqual(r['ranges'], [(4, 8)])

    def test_derive(self):
        chunk, base = make_chunk()
        a, d = (base, 24), (base + 24 + 12 + 16 + 20 + 8, 12)
        rows = [(base, 24), (base + 24, 12), (base + 36, 16), (base + 52, 20), (base + 72, 8), (base + 80, 12), (base + 80, 4),
                (base + 1000, 16), (base - 4, 8), (base + 26, 8), (base + 36, 0)]
        r = X.derive_ranges(base, chunk, [a, d], rows)
        # A and D are established; row (base+24, 12) is 3 words with no jr ra at its end (func(3, ..., False)); (base+36, 16) is B with jr ra
        # (accepted); (base+52, 20) is C without jr ra; (base+72, 8) filler; (base+80, 12) is D (covered); (base+80, 4) inside D (covered).
        self.assertEqual(r['ranges'], [(4, 20), (36, 16), (80, 12)])
        self.assertEqual((r['est_bytes'], r['added_bytes'], r['covered'], r['checked']), (32, 16, 3, 1))
        reasons = {(x[0] - base, x[1]): x[2] for x in r['skipped']}
        self.assertEqual(reasons, {(24, 12): 'not jr ra', (52, 20): 'not jr ra', (72, 8): 'not jr ra', (1000, 16): 'outside the chunk',
                                   (-4, 8): 'outside the chunk', (26, 8): 'unaligned', (36, 0): 'outside the chunk'})
        # too short: 4 bytes outside the established ranges
        r = X.derive_ranges(base, chunk, [a], [(base + 52, 4)])
        self.assertEqual(r['skipped'], [(base + 52, 4, 'too short')])
        # an inventory row that touches an established range is joined with it
        chunk2 = func(6, 0x31) + func(3, 0x32)
        r = X.derive_ranges(base, chunk2, [(base, 24)], [(base + 24, 12)])
        self.assertEqual(r['ranges'], [(4, 32)])
        # an established function outside the chunk is refused
        for bad in ((base + len(chunk) - 4, 8), (base - 4, 8)):
            with self.assertRaises(X.Fail):
                X.derive_ranges(base, chunk, [bad], [])

    def test_like_move_and_bounds(self):
        chunk, base = make_chunk()
        shift = 0x10000
        images = [dict(name='a', address=base), dict(name='b', like='a', address=base + shift)]
        by = {i['name']: i for i in images}
        est = X.established(by['b'], by, {'a': [('f', 'u', base, 24), ('g', 'u', base + 80, 12)]})
        r = X.derive_ranges(base + shift, chunk, est, [])
        self.assertEqual(r['ranges'], [(4, 20), (80, 12)])
        # the same functions in a chunk too short for the moved range
        with self.assertRaises(X.Fail):
            X.derive_ranges(base + shift, chunk[:90], est, [])

    def test_zero_and_pin(self):
        chunk, _ = make_chunk()
        z = X.zeroed(chunk, [(0, 24), (36, 16)])
        self.assertEqual(len(z), len(chunk))
        self.assertEqual(z[:24], bytes(24))
        self.assertEqual(z[24:36], chunk[24:36])
        self.assertEqual(z[36:52], bytes(16))
        self.assertEqual(z[52:], chunk[52:])
        self.assertNotEqual(X.sha(z), X.sha(chunk))
        self.assertEqual(X.sha(X.zeroed(chunk, [])), X.sha(chunk))

    def test_derive_all(self):
        chunk, base = make_chunk()
        other = chunk[:60] + b'\x77' * (len(chunk) - 60)
        shift = 0x10000
        images = [dict(name='pl00', archive='../x/PL00.PAC', slot=4, sha256=X.sha(chunk), address=base),
                  dict(name='pl00x', like='pl00', archive='../x/PL00X.PAC', slot=5, sha256=X.sha(chunk), address=base + shift),
                  dict(name='stage', archive='../x/STAGE00.PAC', slot=6, sha256=X.sha(other), address=base)]
        funcs = {'pl00': [('f', 'u', base, 24), ('g', 'u', base + 80, 12)], 'stage': [('s', 'u2', base, 24)]}
        rows = {(4, 'PL00.PAC'): [(base, 24), (base + 36, 16), (base + 80, 12)], (5, 'PL00X.PAC'): [(base + shift, 24), (base + shift + 36, 16), (base + shift + 80, 12)],
                (6, 'STAGE00.PAC'): [(base, 24)]}
        archives = {(4, 'PL00.PAC'): ['PL00.PAC'], (5, 'PL00X.PAC'): ['PL00X.PAC', 'PL01X.PAC'], (6, 'STAGE00.PAC'): ['STAGE00.PAC']}
        data = {('PL00.PAC', 4): chunk, ('PL00X.PAC', 5): chunk, ('STAGE00.PAC', 6): other}
        res = X.derive_all(lambda a, s: data[(a, s)], images, funcs, rows, archives)
        self.assertEqual([r['name'] for r in res], ['pl00', 'pl00x', 'stage'])
        self.assertEqual(res[0]['ranges'], [(4, 20), (36, 16), (80, 12)])
        self.assertEqual(res[1]['ranges'], res[0]['ranges'])
        self.assertEqual((res[0]['cls'], res[1]['cls'], res[2]['cls']), ('fighters', 'fighters', 'stages'))
        self.assertEqual((res[0]['copies'], res[1]['copies']), (1, 2))
        self.assertEqual((res[0].get('like_differs'), res[1]['like_differs']), (None, 0))
        self.assertEqual(res[0]['data_pin'], X.sha(X.zeroed(chunk, res[0]['ranges'])))
        self.assertEqual(res[0]['pin'], X.sha(chunk))
        # a second side whose inventory rows are not the base's rows moved
        rows2 = dict(rows)
        rows2[(5, 'PL00X.PAC')] = [(base + shift, 24), (base + shift + 40, 16)]
        res = X.derive_all(lambda a, s: data[(a, s)], images, funcs, rows2, archives)
        self.assertEqual(res[1]['like_differs'], 3)
        # a chunk that is not the pinned one, an image that is not a content of the inventory
        bad = dict(data)
        bad[('PL00.PAC', 4)] = chunk[:-4] + b'\1\0\0\0'
        with self.assertRaises(X.Fail):
            X.derive_all(lambda a, s: bad[(a, s)], images, funcs, rows, archives)
        with self.assertRaises(X.Fail):
            X.derive_all(lambda a, s: data[(a, s)], images, funcs, {}, archives)

    def test_classes(self):
        self.assertEqual([X.image_class(n) for n in ('PL00.PAC', 'PL0EX.PAC', 'STAGE0A.PAC', 'END03.PAC', 'BOSS00.PAC', 'CONT00.PAC', 'SELECT.PAC')],
                         ['fighters', 'fighters', 'stages', 'endings', 'screens', 'screens', 'screens'])

    def test_table_text(self):
        chunk, _ = make_chunk()
        text = X.c_table([pin_rows(chunk, [(0, 24), (36, 16)], 'a'), pin_rows(chunk, [], 'b')])
        self.assertIn('static const struct arcade_code_range ranges_0[] = {', text)
        self.assertNotIn('ranges_1[]', text)
        self.assertIn('{0x0, 0x18}', text)
        self.assertIn('{0x24, 0x10}', text)
        self.assertIn('{"a", %#x,' % len(chunk), text)
        self.assertIn('NULL, 0}', text)
        self.assertIn('arcade_code_pin_count = 2;', text)
        self.assertIn('0x' + X.sha(chunk)[:2], text)
        self.assertEqual(text, X.c_table([pin_rows(chunk, [(0, 24), (36, 16)], 'a'), pin_rows(chunk, [], 'b')]))

    def test_no_dash(self):
        for p in (ARC / 'code_pins.c', ARC / 'code_pins.h', ARC / 'code_pins.py', DRIVER, Path(__file__)):
            self.assertNotIn(chr(0x2014), p.read_text(), str(p))


class CSide(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.dir = Path(cls.tmp.name)
        cls.chunk, _ = make_chunk()
        n = len(cls.chunk)
        rows = [pin_rows(cls.chunk, [(4, 20), (36, 16), (80, 12)], 'good'), pin_rows(cls.chunk, [], 'none'),
                pin_rows(cls.chunk, [(0, 4), (n - 4, 4)], 'ends'),
                # rows the generator would never write: a range past the end, one whose end overflows
                pin_rows(cls.chunk, [(0, 8)], 'outside'), pin_rows(cls.chunk, [(0, 8)], 'overflow')]
        rows[3]['ranges'] = [(0, 8), (n - 4, 8)]
        rows[4]['ranges'] = [(8, 0xfffffffc)]
        (cls.dir / 'table.c').write_text(X.c_table(rows))
        cls.exe = str(cls.dir / 'test')
        subprocess.run(['gcc', *FLAGS, *SAN, '-I', str(ARC), '-o', cls.exe, str(ARC / 'code_pins.c'), str(cls.dir / 'table.c'), str(DRIVER)],
                       check=True, capture_output=True, text=True, timeout=120)
        cls.rows = rows
        (cls.dir / 'in').write_bytes(cls.chunk)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def run_driver(self, *args):
        return subprocess.run([self.exe, *args], capture_output=True, text=True, timeout=60)

    def test_list_and_find(self):
        out = self.run_driver('list').stdout.split('\n')
        self.assertEqual(out[0], 'count 5')
        row = out[1].split()
        self.assertEqual(row, ['good', str(len(self.chunk)), self.rows[0]['pin'], self.rows[0]['data_pin'], '3'])
        self.assertEqual(self.run_driver('find', 'good').returncode, 0)
        self.assertEqual(self.run_driver('find', 'ends').returncode, 0)
        self.assertEqual(self.run_driver('find', 'nope').returncode, 1)
        self.assertEqual(self.run_driver('find', 'goo').returncode, 1)
        self.assertEqual(self.run_driver('find', 'good2').returncode, 1)
        self.assertEqual(self.run_driver('ranges', 'good').stdout.split('\n')[:3], ['4 20', '36 16', '80 12'])

    def test_zero_agrees_with_python(self):
        for r in self.rows[:3]:
            out = self.dir / 'out'
            run = self.run_driver('zero', r['name'], str(self.dir / 'in'), str(out))
            self.assertEqual(run.returncode, 0, run.stderr)
            got = out.read_bytes()
            self.assertEqual(got, X.zeroed(self.chunk, r['ranges']))
            self.assertEqual(X.sha(got), r['data_pin'])
        # zeroes exactly the ranges: every other byte is as it was
        out = (self.dir / 'out').read_bytes()
        self.assertEqual(out[:4], bytes(4))
        self.assertEqual(out[4:-4], self.chunk[4:-4])
        self.assertEqual(out[-4:], bytes(4))

    def test_refusals(self):
        out = self.dir / 'refused'
        self.assertEqual(self.run_driver('zero', 'good', str(self.dir / 'in'), str(out), 'short').returncode, 3)
        self.assertEqual(self.run_driver('zero', 'outside', str(self.dir / 'in'), str(out)).returncode, 3)
        self.assertEqual(self.run_driver('zero', 'overflow', str(self.dir / 'in'), str(out)).returncode, 3)
        self.assertEqual(self.run_driver('zero', 'nope', str(self.dir / 'in'), str(out)).returncode, 4)
        short = self.dir / 'short'
        short.write_bytes(self.chunk[:-4])
        self.assertEqual(self.run_driver('zero', 'good', str(short), str(out)).returncode, 3)
        self.assertFalse(out.exists())


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
