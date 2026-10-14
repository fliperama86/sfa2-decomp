"""Seconds, generated pixels only: compile and exercise the intro compositor.

No original program, game assets, window, sound or whole-port build.
"""
from pathlib import Path
import resource
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class VideoTest(unittest.TestCase):
    def test_native_compositor(self):
        with tempfile.TemporaryDirectory(prefix='intro-video-') as directory:
            output = Path(directory) / 'video-test'
            subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined', '-fno-sanitize-recover=all',
                            str(ROOT / 'port/intro/video.c'),
                            str(ROOT / 'port/intro/tests/video_test.c'), '-o', str(output)],
                           check=True, timeout=20)
            subprocess.run([str(output)], check=True, timeout=10)

    def test_negative_controls(self):
        mutations = [
            ('priority[at]|=128u;', 'priority[at]|=0u;'),
            ('((y+16u+v->row_offset)&1023u)', '((y+v->row_offset)&1023u)'),
            ('15u+2u*(color>>12)', '16u+2u*(color>>12)'),
        ]
        source = (ROOT / 'port/intro/video.c').read_text()
        def no_core():
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        for old, new in mutations:
            with self.subTest(mutation=old), tempfile.TemporaryDirectory(prefix='intro-video-') as directory:
                self.assertEqual(source.count(old), 1)
                directory = Path(directory)
                unit, output = directory / 'video.c', directory / 'video-test'
                unit.write_text(source.replace(old, new))
                subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                                '-I' + str(ROOT / 'port/intro'), str(unit),
                                str(ROOT / 'port/intro/tests/video_test.c'), '-o', str(output)],
                               check=True, timeout=20)
                result = subprocess.run([str(output)], cwd=directory, capture_output=True,
                                        preexec_fn=no_core, timeout=10)
                self.assertEqual(result.returncode, -6)
                self.assertIn(b'Assertion', result.stderr)


if __name__ == '__main__':
    unittest.main()
