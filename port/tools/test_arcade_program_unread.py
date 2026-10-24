#!/usr/bin/env python3
"""Seconds: port/arcade/program_unread.py's pure functions on made-up input; no game files, no build.

The names an object leaves undefined, the addresses of the linked program, the constants of a C source, the pointer words of the table, and the
scan itself on a small made-up data block: a symbol that an object refers to is read, so is one a pointer word of a read symbol points into (also
through a second word), one a constant names is read, the rest is unread and adjacent unread symbols merge into one run. The list that code_pins.py
holds must be sorted, not overlap, lie inside the data block and not be empty.
"""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import program_unread as U   # noqa: E402
import code_pins             # noqa: E402


class Pure(unittest.TestCase):
    def test_nm_lines(self):
        out = '         U ps1_data_8017f3a0\n         U printf\n0000000a T impl_x\n         U ps1_func_80120604\n'
        self.assertEqual(U.undefined_names(out), {'data_8017f3a0', 'func_80120604'})
        self.assertEqual(U.absolute_addresses('8017f3a0 A ps1_data_8017f3a0\n0807d3c0 T impl_f\nzzzz A ps1_bad\n'), {'data_8017f3a0': 0x8017f3a0})

    def test_constants_and_words(self):
        text = 'a = 0x8017eb1cu; b = 0x80190a44; c = 0x1f800000; d = 0x80200000; e = 0x80183904;'
        self.assertEqual(U.constant_addresses(text), {0x8017eb1c})                  # the others lie outside the data block
        words = U.pointer_words('{0x80170000u, 0x80171000u},\n  {0x80170004, 0x80172000},\nnot a row')
        self.assertEqual(words, [(0x80170000, 0x80171000), (0x80170004, 0x80172000)])

    def test_scan(self):
        s, e = 0x1000, 0x1100
        ranges = [(0x1000, 0x1010, 'a'), (0x1010, 0x1020, 'b'), (0x1020, 0x1030, 'c'), (0x1030, 0x1040, 'd'), (0x1040, 0x1050, 'e'), (0x1050, 0x1100, 'f')]
        # 'a' is referred to; a pointer word in 'a' points into 'c'; one in 'c' points into 'e'; a constant names 'f'; b and d are unread
        words = [(0x1004, 0x1024), (0x1028, 0x1044), (0x1034, 0x1000)]   # the third word is in the unread 'd': it makes nothing read
        unread, merged = U.unread_ranges(ranges, {0x1000}, {0x1060}, words, s, e)
        self.assertEqual([n for _, _, n in unread], ['b', 'd'])
        self.assertEqual(merged, [(0x1010, 0x10), (0x1030, 0x10)])
        unread, merged = U.unread_ranges(ranges, set(), set(), [], s, e)
        self.assertEqual(merged, [(0x1000, 0x100)])                     # nothing referred to: one run, merged

    def test_the_committed_list(self):
        runs = code_pins.RESIDENT_UNREAD
        self.assertTrue(runs)
        for (a, n), (b, _) in zip(runs, runs[1:]):
            self.assertLess(a + n, b + 1)                                # sorted, not overlapping
        for a, n in runs:
            self.assertTrue(U.DATA_START <= a and a + n <= U.DATA_END and n > 0)


if __name__ == '__main__':
    import time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
