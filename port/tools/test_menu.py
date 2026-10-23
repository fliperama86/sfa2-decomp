"""Subsecond native fixtures of the settings menu (port/src/menu.c, port/tools/menu_test.c): value lists, texts, the input
state machine and the picture, on a generated video memory. No game, graphics or private inputs; the C is built with
gcc and, where this machine can run 32-bit programs (the port is 32-bit), with gcc -m32 as well.
"""
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
FLAGS = ['-std=c11', '-O1', '-Wall', '-Wextra', '-Werror']
SOURCES = [str(ROOT / 'port/src/menu.c'), str(ROOT / 'port/tools/menu_test.c')]


def build_and_run(folder, extra):
    exe = Path(folder) / 'menu_test'
    subprocess.run(['gcc', *extra, *FLAGS, '-I', str(ROOT / 'port/src'), *SOURCES, '-o', str(exe)], check=True, timeout=30)
    return subprocess.run([str(exe)], capture_output=True, text=True, timeout=10)


def can_run_32bit():
    with tempfile.TemporaryDirectory() as folder:
        source = Path(folder) / 'probe.c'
        source.write_text('int main(void) { return 0; }\n')
        try:
            subprocess.run(['gcc', '-m32', str(source), '-o', str(Path(folder) / 'probe')], check=True, timeout=15,
                           capture_output=True)
            return subprocess.run([str(Path(folder) / 'probe')], timeout=5).returncode == 0
        except (subprocess.CalledProcessError, OSError):
            return False


class Menu(unittest.TestCase):
    def check(self, extra):
        with tempfile.TemporaryDirectory() as folder:
            result = build_and_run(folder, extra)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('menu ok', result.stdout)

    def test_native(self):
        self.check([])

    def test_32bit(self):
        if not can_run_32bit():
            self.skipTest('no 32-bit gcc runtime here')
        self.check(['-m32'])

    def test_pure(self):
        # menu.c is a pure unit: it includes only its own header and the C library's stdio and string.
        includes = [line.strip() for line in (ROOT / 'port/src/menu.c').read_text().splitlines()
                    if line.startswith('#include')]
        self.assertEqual(includes, ['#include "menu.h"', '#include <stdio.h>', '#include <string.h>'])


def main() -> int:
    """Each test as one case: `ok   NAME` or `FAIL NAME: why`, then the summary line that the review reads."""
    tests = list(unittest.defaultTestLoader.loadTestsFromModule(sys.modules[__name__]))
    cases = [t for suite in tests for t in suite]
    bad = 0
    for case in cases:
        result = unittest.TestResult()
        case.run(result)
        problems = result.failures + result.errors
        if problems:
            bad += 1
            print(f"FAIL {case.id().rsplit('.', 1)[-1]}: {problems[0][1].strip().splitlines()[-1]}")
        else:
            print(f"ok   {case.id().rsplit('.', 1)[-1]}")
    if bad:
        print(f"{bad} case(s) behaved wrongly")
        return 1
    count = len(cases)
    print(f"all {count} cases behaved as required")
    return 0


if __name__ == '__main__':
    sys.exit(main())
