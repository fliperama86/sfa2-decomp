"""Widescreen step 4: the overrides of port/src/wide_objects.c with stand-ins of the game's margin test and draw phase.

Built with gcc -m32 and run in under a second; no game, disc or window. Source: port/src/wide_objects.c,
port/tools/wide_objects_test.c (port/widescreen.md).
"""
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class WideObjects(unittest.TestCase):
    def test_objects_in_the_sides(self):
        with tempfile.TemporaryDirectory() as folder:
            exe = Path(folder) / 'test'
            subprocess.run(['gcc', '-m32', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror', '-fno-pie', '-no-pie', '-D_GNU_SOURCE',
                            '-I', str(ROOT / 'port/src'), str(ROOT / 'port/src/wide_objects.c'),
                            str(ROOT / 'port/tools/wide_objects_test.c'), '-o', str(exe)], check=True, timeout=30)
            result = subprocess.run([str(exe)], capture_output=True, text=True, timeout=10)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('wide objects ok', result.stdout)


def main() -> int:
    """Each test as one case: `ok   NAME` or `FAIL NAME: why`, then the summary line that the review reads."""
    cases = [t for suite in unittest.defaultTestLoader.loadTestsFromModule(sys.modules[__name__]) for t in suite]
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
