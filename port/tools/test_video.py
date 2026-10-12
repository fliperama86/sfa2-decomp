"""Fast synthetic geometry/policy and dependency-patch controls. No game/GPU.

Checks the exact published patch on invented files, header installation, and
marked versus unmarked backend branches. Pixel readback has a separate generated
fixture (video_gpu_test.c) for an already built SDK, never a game replay.
"""
from pathlib import Path
import subprocess
import tempfile
import unittest
import psyzbuild
import hostbuild

ROOT = Path(__file__).resolve().parents[2]
PATCH = ROOT / 'port/psyz.patch'

class Video(unittest.TestCase):
    def test_stale_library_rejected_before_game_compilation(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            self.assertEqual(psyzbuild.video_api_metadata(root), {})
            psyzbuild.install_video_header(root, PATCH.read_text())
            self.assertEqual(psyzbuild.video_api_metadata(root), {'port_video_api': 1})
            (root / 'gpu.c').write_text('void f(void) { Psyz_Sfa2SetArcadeCanvas(0); }\n')
            with self.assertRaisesRegex(hostbuild.Problem, 'rebuild'):
                hostbuild.require_video_api({}, root)
            hostbuild.require_video_api({'port_video_api': 1}, root)
            (root / 'gpu.c').write_text('/* A custom legacy runtime has no extension dependency. */\n')
            hostbuild.require_video_api({}, root)

    def test_generated_policy(self):
        with tempfile.TemporaryDirectory() as folder:
            exe = Path(folder) / 'test'
            subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror',
                '-I', str(ROOT / 'port/src'), str(ROOT / 'port/tools/video_test.c'),
                '-o', str(exe)], check=True, timeout=15)
            subprocess.run([str(exe)], check=True, timeout=5)

    def test_complete_published_patch(self):
        text = PATCH.read_text()
        patches = psyzbuild.parse_patch(text)
        self.assertEqual({p.path for p in patches}, {'psyz/src/psyz/libgpu.c',
            'psyz/src/platform/sdl3_gpu.c', 'psyz/src/platform/sdl3_gl.c'})
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            for patch in patches:
                target = root / patch.path
                target.parent.mkdir(parents=True, exist_ok=True)
                # Invent neighbours around each required context. No SDK input.
                target.write_text('\n'.join('\n'.join(h.old) + '\n/* fixture neighbour %d */\n' % n
                    for n, h in enumerate(patch.hunks)) + '\n')
            changes = psyzbuild.apply_patch(text, root)
            psyzbuild.install_video_header(root, text)
            self.assertEqual((root / 'psyz/src/platform/port_video.h').read_bytes(),
                (ROOT / 'port/src/video.h').read_bytes())
            self.assertEqual(len(changes), sum(len(p.hunks) for p in patches))
            gpu = (root / 'psyz/src/platform/sdl3_gpu.c').read_text()
            self.assertIn('(tpage & PORT_VIDEO_PIXEL_GRID) && (code & EXTRA_VERTEX)', gpu)
            self.assertIn('if (sfa2_pixel_perfect_ui &&', gpu)
            self.assertIn('if (!fixed_hud) FixupFlipUV', gpu)
            self.assertIn('Psyz_Sfa2ExperimentalVideoAvailable(void) { return 1; }', gpu)
            self.assertIn('port_video_arcade_fit', gpu)
            gl = (root / 'psyz/src/platform/sdl3_gl.c').read_text()
            self.assertIn('Psyz_Sfa2ExperimentalVideoAvailable(void) { return 0; }', gl)
            # Reject applying twice, rather than guessing against stale input.
            with self.assertRaises(psyzbuild.Problem):
                psyzbuild.apply_patch(text, root)

if __name__ == '__main__':
    unittest.main()
