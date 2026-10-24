"""Fast generated UI-layout and real packet-copy seam controls, no game/SDK/GPU.

Reads only project C/header sources. Native fixture time is under one second;
a short-lived anonymous map models Sony RAM, never a game/disc image.
"""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class UiLayout(unittest.TestCase):
    def compile_run(self, source, extra=()):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            if isinstance(source, str):
                (path / 'fixture.c').write_text(source)
                source = path / 'fixture.c'
            exe = path / 'fixture'
            subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror',
                '-I', str(ROOT / 'port/src'), str(source), *map(str, extra),
                '-o', str(exe)], check=True, timeout=30)
            run = subprocess.run([str(exe)], timeout=15, stderr=subprocess.PIPE, text=True)   # the walker's log lines are for the game's run
            self.assertEqual(run.returncode, 0, run.stderr[-2000:])

    def test_geometry_and_exact_buffer_policy(self):
        self.compile_run(ROOT / 'port/tools/ui_layout_test.c')

    def test_real_packet_copy_seam(self):
        gpu = (ROOT / 'port/src/gpu.c').read_text()
        seam = gpu.split('/* ---- seam: the list walker ---- */', 1)[1].split(
            '/* ---- routines that go to PsyZ ---- */', 1)[0]
        fixture = (ROOT / 'port/tools/ui_layout_seam_test.c').read_text()
        self.assertEqual(fixture.count('/* REAL_GPU_SEAM */'), 1)
        self.compile_run(fixture.replace('/* REAL_GPU_SEAM */', seam))

    def test_no_sdk_refuses_enabled_layout(self):
        self.compile_run('#include "gpu.h"\n#include <assert.h>\nint main(void) {'
            'assert(!port_gpu_set_ui_layout(0));assert(port_gpu_set_ui_layout(1)==-1);return 0;}\n',
            [ROOT / 'port/src/gpu.c'])

if __name__ == '__main__':
    unittest.main()
