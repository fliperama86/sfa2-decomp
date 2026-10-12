"""Subsecond native settings fixtures, generated files only, no game/hardware.

The C fixture exercises the public load/save API, transactional errors and all
flag combinations. Source: port/src/settings.c/.h and settings_test.c.
"""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class Settings(unittest.TestCase):
    def test_generated_settings(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror',
                '-I', str(ROOT / 'port/src'), str(ROOT / 'port/src/settings.c'),
                str(ROOT / 'port/tools/settings_test.c'), '-o', str(path / 'test')], check=True, timeout=15)
            subprocess.run([str(path / 'test'), str(path / 'settings.ini')], check=True, timeout=5)

    def test_real_startup_parser_without_game(self):
        # Execute the real startup/CLI parsing section alone. The remainder of
        # main is deliberately excluded: no memory map, GPU, audio or disc access.
        main = (ROOT / 'port/src/main.c').read_text()
        prefix = main.split('int main(int argc, char **argv)', 1)[1].split(
            '    if (list && !bad', 1)[0]
        source = '''#include "port.h"
#include "settings.h"
#include <stdlib.h>
#include <string.h>
int port_skip_programs;
static int refuse(const char *why) { puts(why);return 2; }
int main(int argc,char **argv)''' + prefix + '\nreturn 2;\n}\n'
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            (path / 'test.c').write_text(source)
            exe = path / 'startup'
            subprocess.run(['gcc', '-std=c11', '-O1', '-I', str(ROOT / 'port/src'),
                str(path / 'test.c'), str(ROOT / 'port/src/settings.c'), '-o', str(exe)], check=True, timeout=15)
            def run(args, expected, ui=None, canvas=None, center=False, fast=False):
                result = subprocess.run([str(exe), '--no-audio', '--show-settings', *args],
                    cwd=path, capture_output=True, text=True, timeout=3)
                self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
                if expected == 0:
                    self.assertIn('pixel_perfect_ui = ' + ('true' if ui else 'false'), result.stdout)
                    self.assertIn('arcade_video = ' + ('true' if canvas else 'false'), result.stdout)
                    self.assertIn('center_ui = ' + ('true' if center else 'false'), result.stdout)
                    self.assertIn('fast_loads = ' + ('true' if fast else 'false'), result.stdout)
            run([], 0, False, False)
            def volume(args, expected, line=None):
                result = subprocess.run([str(exe), '--no-audio', '--show-settings', *args],
                    cwd=path, capture_output=True, text=True, timeout=3)
                self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
                if line: self.assertIn(line, result.stdout.splitlines())
            volume([], 0, 'volume = 100')
            volume(['--volume', '20'], 0, 'volume = 20')
            volume(['--volume', '0'], 0, 'volume = 0')
            volume(['--volume', '100'], 0, 'volume = 100')
            volume(['--volume', '20', '--volume', '55'], 0, 'volume = 55')
            for bad in (['--volume'], ['--volume', '101'], ['--volume', '-1'], ['--volume', 'x'], ['--volume', ''], ['--volume', '20x'], ['--volume', '1.5']):
                volume(bad, 2)
            config = path / 'settings.ini'
            config.write_text('[experimental]\npixel_perfect_ui = true\narcade_video = false\n')
            run([], 0, True, False)
            run(['--arcade-video'], 0, True, True)
            run(['--ps1-rendering'], 0, False, False)
            run(['--arcade-video', '--ps1-video'], 0, True, False)
            run(['--pixel-perfect-ui', '--ps1-rendering'], 0, False, False)
            run(['--ps1-rendering', '--settings', str(config)], 0, False, False)
            run(['--settings', str(config), '--arcade-video'], 0, True, True)
            run(['--center-ui'], 0, True, False, True)
            run(['--center-ui', '--ps1-ui-layout'], 0, True, False, False)
            run(['--fast-loads'], 0, True, False, False, True)
            run(['--fast-loads', '--ps1-loading'], 0, True, False, False, False)
            config.write_text('[experimental]\nfast_loads = true\n')
            run([], 0, False, False, False, True)
            run(['--ps1-loading'], 0, False, False, False, False)
            config.write_text('[experimental]\npixel_perfect_ui = true\ncenter_ui = true\n')
            run(['--ps1-ui-layout', '--settings', str(config)], 0, True, False, False)
            run(['--settings', str(config), '--arcade-video'], 0, True, True, True)
            run(['--settings', 'missing.ini'], 2)
            config.write_text('[experimental]\narcade_video = typo\n')
            run([], 2)
            config.write_text('[experimental]\narcade_video = true\narcade_video = false\n')
            run([], 2)

if __name__ == '__main__':
    unittest.main()
