"""Fast synthetic geometry/policy and dependency-patch controls. No game/GPU.

Checks the exact published patch on invented files, header installation, and
marked versus unmarked backend branches. Pixel readback has a separate generated
fixture (video_gpu_test.c) for an already built SDK, never a game replay.
"""
from pathlib import Path
import subprocess
import re
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
            self.assertEqual(psyzbuild.video_api_metadata(root), {'port_video_api': 5})
            (root / 'gpu.c').write_text('void f(void) { Psyz_Sfa2SetArcadeCanvas(0); }\n')
            with self.assertRaisesRegex(hostbuild.Problem, 'rebuild'):
                hostbuild.require_video_api({}, root)
            with self.assertRaisesRegex(hostbuild.Problem, 'rebuild'):
                hostbuild.require_video_api({'port_video_api': 1}, root)
            with self.assertRaisesRegex(hostbuild.Problem, 'rebuild'):
                hostbuild.require_video_api({'port_video_api': 2}, root)
            with self.assertRaisesRegex(hostbuild.Problem, 'rebuild'):
                hostbuild.require_video_api({'port_video_api': 3}, root)
            with self.assertRaisesRegex(hostbuild.Problem, 'rebuild'):
                hostbuild.require_video_api({'port_video_api': 4}, root)
            hostbuild.require_video_api({'port_video_api': 5}, root)
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
            'psyz/src/platform/sdl3_gpu.c', 'psyz/src/platform/sdl3_common.h', 'psyz/src/platform/sdl3_gl.c', 'psyz/src/platform/sdl3_audio.c'})
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
            self.assertIn('port_video_life_fill(x, &y, w, &h)', gpu)
            self.assertIn('(code & PORT_VIDEO_LIFE_FILL)', gpu)
            # Filters: sharp bilinear asks the shared policy for the enlargement, the linear filter follows it (or bilinear alone), and teardown frees its texture.
            self.assertIn('!port_video_sharp_fit((int)src->w,(int)src->h,dst.w,dst.h,&pre)) return 0;', gpu)
            self.assertIn('if (sfa2_filter != 2 || debug_show_vram ||', gpu)
            self.assertIn('const int sfa2_linear = Sfa2SharpPrescale(cmd, &src, dst) || (sfa2_filter == 1 && !debug_show_vram);', gpu)
            self.assertIn('.filter = sfa2_linear ? SDL_GPU_FILTER_LINEAR : SDL_GPU_FILTER_NEAREST,', gpu)
            self.assertNotIn('SetSharpBilinear', gpu)
            # Screen: the window size replaces PsyZ's default at the first show, the fullscreen is entered there, and PsyZ's own flag follows.
            self.assertIn('SetWindowSizeInPixels(sfa2_win_w, sfa2_win_h);', gpu)
            self.assertIn('        Sfa2ApplyPendingScreen();\n        SDL_ShowWindow(sdl3_window);', gpu)
            self.assertIn('is_fullscreen = true;', gpu)
            self.assertIn('SDL_GetClosestFullscreenDisplayMode(display, w, h, 0.0f, false, &closest)', gpu)
            self.assertIn('SDL_SetWindowFullscreenMode(sdl3_window, mode)', gpu)
            # Host keys: recorded by the event pump of the common header, without quitting or toggling.
            common = (root / 'psyz/src/platform/sdl3_common.h').read_text()
            self.assertIn('if (sfa2_host_keys && (event.key.scancode == SDL_SCANCODE_ESCAPE ||', common)
            self.assertIn('case SDL_EVENT_GAMEPAD_BUTTON_DOWN:', common)
            self.assertIn('event.gbutton.button == SDL_GAMEPAD_BUTTON_GUIDE', common)
            self.assertIn('SDL_ReleaseGPUTexture(device, sfa2_sharp_texture);\n            sfa2_sharp_texture = NULL;', gpu)
            gl = (root / 'psyz/src/platform/sdl3_gl.c').read_text()
            self.assertIn('Psyz_Sfa2ExperimentalVideoAvailable(void) { return 0; }', gl)
            # Every function the port declares for a backend is defined by both backends (the OpenGL fallback must link too).
            declared = re.findall(r'^\w+ (Psyz_Sfa2\w+)\(', (ROOT / 'port/src/video.h').read_text(), re.M)
            self.assertIn('Psyz_Sfa2SetBatchedUploads', declared)
            self.assertIn('Psyz_Sfa2SetFilter', declared)
            for name in ('SetScreen', 'GetScreen', 'DesktopSize', 'FullscreenModes', 'SetHostKeys', 'TakeKeys', 'SetWide', 'ReadWide'):
                self.assertIn('Psyz_Sfa2' + name, declared)
            # Wide canvas (port_video_api 5): a draw of the batch into each frame area's canvas through the shared helper, moved by the shader offset,
            # clears and loads that meet the area, the canvas shown in place of the frame, and the canvases released with the other textures.
            self.assertIn('Sfa2DrawBatch(cmd, GetRenderTarget(), NULL);', gpu)
            self.assertIn('Sfa2DrawBatch(cmd, sfa2_wide_canvas->tex, sfa2_wide_canvas);', gpu)
            self.assertIn('static void Sfa2DrawBatch(SDL_GPUCommandBuffer* cmd, SDL_GPUTexture* texture, const Sfa2Canvas* canvas) {', gpu)
            self.assertIn('(float)(sfa2_wide_extra - canvas->x) / GetDrawGridXScale()', gpu)
            self.assertIn('Sfa2ClearCanvases(cmd, rect, r, g, b);', gpu)
            self.assertIn('Sfa2StaleCanvases(rect->x, rect->y, rect->w, rect->h);', gpu)
            self.assertIn('Sfa2StaleCanvases(dst_x, dst_y, copy_w, copy_h);', gpu)
            self.assertIn('port_video_wide_fit(win.w,win.h,&wide_fit)', gpu)
            self.assertIn('Sfa2WideShown(sfa2_wide);\n    SubmitFrame();', gpu)
            self.assertIn('sfa2_sharp_texture = NULL;\n        }\n        Sfa2ReleaseCanvases();', gpu)
            self.assertIn('Psyz_Sfa2ReadWide(uint16_t *pixels, int max_w, int max_h, int *w, int *h) { (void)pixels; (void)max_w; (void)max_h; (void)w; (void)h; return -1; }', gl)
            self.assertNotIn('Psyz_Sfa2SetSharpBilinear', declared)
            for name in declared:
                for backend, source in (('sdl3_gpu.c', gpu), ('sdl3_gl.c', gl)):
                    self.assertRegex(source, r'(?m)^\w+ %s\([^)]*\) *\{' % name, '%s lacks %s' % (backend, name))
            # Reject applying twice, rather than guessing against stale input.
            with self.assertRaises(psyzbuild.Problem):
                psyzbuild.apply_patch(text, root)

if __name__ == '__main__':
    unittest.main()
