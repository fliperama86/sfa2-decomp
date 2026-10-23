"""The settings menu's host keys in the input script: input.c alone, built natively without PsyZ, in under a second.

Source: port/src/input.c and port/tools/input_keys_test.c (port/settings-menu.md). No game, disc or window.
"""
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class InputKeys(unittest.TestCase):
    def test_host_keys(self):
        for flags in ([], ['-m32']):
            with tempfile.TemporaryDirectory() as folder:
                path = Path(folder)
                subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror', '-D_GNU_SOURCE', *flags,
                    '-I', str(ROOT / 'port/src'), str(ROOT / 'port/src/input.c'),
                    str(ROOT / 'port/tools/input_keys_test.c'), '-o', str(path / 'test')], check=True, timeout=15)
                result = subprocess.run([str(path / 'test'), str(path / 'script.txt')], capture_output=True, text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('input keys ok', result.stdout)


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
