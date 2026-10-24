"""Subsecond native settings fixtures, generated files only, no game/hardware.

The C fixture exercises the public load/save API, transactional errors, the [experimental] and
[display] keys and all flag combinations. The startup test compiles main.c's own CLI section.
Source: port/src/settings.c/.h, port/src/main.c, port/settings.ini and settings_test.c.
"""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class Settings(unittest.TestCase):
    def test_generated_settings(self):
        # Twice: as a build without PsyZ compiles settings.c (the graphics defaults off) and as one with it (on).
        for flags in ([], ['-DPORT_HAVE_PSYZ']):
            with tempfile.TemporaryDirectory() as folder:
                path = Path(folder)
                subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror', *flags,
                    '-I', str(ROOT / 'port/src'), str(ROOT / 'port/src/settings.c'),
                    str(ROOT / 'port/tools/settings_test.c'), '-o', str(path / 'test')], check=True, timeout=15)
                subprocess.run([str(path / 'test'), str(path / 'settings.ini'), str(ROOT / 'port/settings.ini')], check=True, timeout=5)

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
static void dump(const char *tag,const struct port_settings *s)
{
    printf("%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\\n",tag,s->pixel_perfect_ui,s->arcade_video,s->center_ui,s->fast_loads,s->memory_card,
        s->arcade_intro,s->arcade_assets,s->settings_menu,s->widescreen,s->screen,s->window_w,s->window_h,s->fullscreen_w,s->fullscreen_h,s->filter);
}
int main(int argc,char **argv)''' + prefix + '''
if(!bad) { dump("file",&port_settings_file);dump("effective",&port_settings);printf("path %s\\n",port_settings_path);return 0; }
return 2;
}
'''
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            (path / 'test.c').write_text(source)
            exe = path / 'startup'
            subprocess.run(['gcc', '-std=c11', '-O1', '-I', str(ROOT / 'port/src'),
                str(path / 'test.c'), str(ROOT / 'port/src/settings.c'), '-o', str(exe)], check=True, timeout=15)
            def run(args, expected, ui=None, canvas=None, center=False, fast=True, card=True):
                result = subprocess.run([str(exe), '--no-audio', '--show-settings', *args],
                    cwd=path, capture_output=True, text=True, timeout=3)
                self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
                if expected == 0:
                    self.assertIn('pixel_perfect_ui = ' + ('true' if ui else 'false'), result.stdout)
                    self.assertIn('arcade_video = ' + ('true' if canvas else 'false'), result.stdout)
                    self.assertIn('center_ui = ' + ('true' if center else 'false'), result.stdout)
                    self.assertIn('fast_loads = ' + ('true' if fast else 'false'), result.stdout)
                    self.assertIn('memory_card = ' + ('true' if card else 'false'), result.stdout)
            run([], 0, False, False)
            def shows(args):
                result = subprocess.run([str(exe), '--no-audio', '--show-settings', *args], cwd=path, capture_output=True, text=True, timeout=3)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                return result.stdout
            # The defaults of a build without PsyZ, in the file's own form (what port_settings_save writes), then the volume line.
            self.assertEqual(shows([]), '[experimental]\npixel_perfect_ui = false\narcade_video = false\ncenter_ui = false\nfast_loads = true\n'
                'memory_card = true\narcade_intro = false\narcade_assets = false\nsettings_menu = false\nwidescreen = false\n\n[display]\nscreen = window\n'
                'window_size = 1280x960\nfullscreen_size = desktop\nfilter = nearest\nvolume = 100\n')
            def volume(args, expected, line=None):
                result = subprocess.run([str(exe), '--no-audio', '--show-settings', *args],
                    cwd=path, capture_output=True, text=True, timeout=3)
                self.assertEqual(result.returncode, expected, result.stdout + result.stderr)
                if line: self.assertIn(line, result.stdout.splitlines())
            volume([], 0, 'volume = 100')
            run(['--memory-card'], 0, False, False, False, True, True)
            run(['--memory-card', '--no-memory-card'], 0, False, False, False, True, False)
            run(['--card1', 'a.mcr', '--card2', 'b.mcr'], 0, False, False, False, True, True)
            volume([], 0, 'arcade_intro = false')
            volume(['--arcade-intro'], 0, 'arcade_intro = true')
            volume(['--arcade-intro','--ps1-intro'], 0, 'arcade_intro = false')
            volume([], 0, 'arcade_assets = false')
            volume(['--arcade-assets'], 0, 'arcade_assets = true')
            volume(['--arcade-assets', '--ps1-assets'], 0, 'arcade_assets = false')
            volume(['--arcade-rom'], 2)
            volume(['--intro-data'], 2)
            volume(['--intro-data',''], 2)
            volume([], 0, 'filter = nearest')
            volume(['--sharp-bilinear'], 0, 'filter = sharp_bilinear')
            volume(['--sharp-bilinear', '--nearest-scaling'], 0, 'filter = nearest')
            volume(['--nearest-scaling', '--sharp-bilinear'], 0, 'filter = sharp_bilinear')
            volume([], 0, 'widescreen = false')
            volume(['--widescreen'], 0, 'widescreen = true')
            volume(['--widescreen', '--no-widescreen'], 0, 'widescreen = false')
            volume(['--no-widescreen', '--widescreen'], 0, 'widescreen = true')
            volume(['--widescreen=1'], 2)
            volume(['--no-widescreen=1'], 2)
            volume([], 0, 'settings_menu = false')
            volume(['--settings-menu'], 0, 'settings_menu = true')
            volume(['--settings-menu', '--no-settings-menu'], 0, 'settings_menu = false')
            volume(['--no-settings-menu', '--settings-menu'], 0, 'settings_menu = true')
            volume(['--settings-menu=1'], 2)
            volume(['--card1'], 2)
            volume(['--card2'], 2)
            volume(['--card1', ''], 2)
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
            run(['--ps1-loading'], 0, True, False, False, False)
            run(['--fast-loads'], 0, True, False, False, True)
            run(['--fast-loads', '--ps1-loading'], 0, True, False, False, False)
            config.write_text('[experimental]\nfast_loads = true\n')
            run([], 0, False, False, False, True)
            run(['--ps1-loading'], 0, False, False, False, False)
            config.write_text('[experimental]\nfast_loads = false\n')
            run([], 0, False, False, False, False)
            run(['--fast-loads'], 0, False, False, False, True)
            config.write_text('[experimental]\npixel_perfect_ui = true\ncenter_ui = true\n')
            run(['--ps1-ui-layout', '--settings', str(config)], 0, True, False, False)
            run(['--settings', str(config), '--arcade-video'], 0, True, True, True)
            # The old key is read as filter and never shown.
            config.write_text('[experimental]\nsharp_bilinear = true\n')
            volume([], 0, 'filter = sharp_bilinear')
            volume(['--nearest-scaling'], 0, 'filter = nearest')
            config.write_text('[experimental]\nsharp_bilinear = false\n')
            volume([], 0, 'filter = nearest')
            self.assertNotIn('sharp_bilinear =', shows([]))
            config.write_text('[experimental]\nsharp_bilinear = true\n[display]\nfilter = nearest\n')
            volume([], 2)
            config.write_text('[display]\nfilter = nearest\n[experimental]\nsharp_bilinear = true\n')
            volume([], 2)
            config.write_text('[experimental]\nscreen = window\n')
            volume([], 2)
            config.write_text('[display]\nfast_loads = true\n')
            volume([], 2)
            for text in ('window_size = 1280X960', 'window_size = 01280x960', 'window_size = 1280x', 'window_size = x960', 'window_size = 1280 x 960',
                         'window_size = 319x960', 'window_size = 1280x16385', 'fullscreen_size = 0x0', 'fullscreen_size = Desktop', 'screen = full',
                         'filter = linear', 'screen = window\nscreen = window'):
                config.write_text('[display]\n' + text + '\n')
                volume([], 2)
            # The [display] section, in the file's own form, with the effective values.
            config.write_text('[display]\nscreen = fullscreen\nwindow_size = 1600x1200\nfullscreen_size = 1920x1080\nfilter = bilinear\n')
            self.assertEqual(shows([]).split('[display]\n')[1], 'screen = fullscreen\nwindow_size = 1600x1200\nfullscreen_size = 1920x1080\nfilter = bilinear\nvolume = 100\n')
            config.write_text('[display]\nscreen = borderless\nfullscreen_size = desktop\n')
            self.assertIn('screen = borderless\nwindow_size = 1280x960\nfullscreen_size = desktop\n', shows([]))
            # What --show-settings prints is a settings file: it reads back to the same output (without the volume line).
            config.write_text('[display]\nscreen = fullscreen\nwindow_size = 800x600\nfullscreen_size = 1024x768\nfilter = bilinear\n[experimental]\nsettings_menu = true\nfast_loads = false\n')
            first = shows(['--center-ui', '--volume', '7'])
            again = path / 'again.ini'
            again.write_text(first.rsplit('volume = ', 1)[0])
            self.assertEqual(shows(['--settings', str(again), '--volume', '7']), first)
            self.assertEqual(shows(['--settings', str(again), '--volume', '7', '--center-ui']), first)
            # The model: port_settings_file is what the file gave, port_settings the file with the command line over it.
            config.unlink()
            fields = ('pixel_perfect_ui', 'arcade_video', 'center_ui', 'fast_loads', 'memory_card', 'arcade_intro', 'arcade_assets', 'settings_menu', 'widescreen',
                      'screen', 'window_w', 'window_h', 'fullscreen_w', 'fullscreen_h', 'filter')
            def model(args):
                result = subprocess.run([str(exe), '--no-audio', *args], cwd=path, capture_output=True, text=True, timeout=3)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                out = {}
                for line in result.stdout.splitlines():
                    tag, *rest = line.split(' ', 1)
                    out[tag] = rest[0] if tag == 'path' else dict(zip(fields, map(int, rest[0].split())))
                return out['file'], out['effective'], out['path']
            file, effective, where = model([])
            self.assertEqual((file, where), (effective, 'settings.ini'))
            self.assertEqual((file['settings_menu'], file['screen'], file['window_w'], file['window_h'], file['fullscreen_w'], file['fullscreen_h'], file['filter']),
                             (0, 0, 1280, 960, 0, 0, 0))
            file, effective, where = model(['--settings-menu', '--sharp-bilinear', '--fast-loads', '--arcade-video'])
            self.assertEqual((file['settings_menu'], file['filter'], file['arcade_video']), (0, 0, 0))
            self.assertEqual((effective['settings_menu'], effective['filter'], effective['arcade_video']), (1, 2, 1))
            # the override of widescreen stays out of the file's model, and the file's value out of the other keys
            file, effective, where = model(['--widescreen'])
            self.assertEqual((file['widescreen'], effective['widescreen']), (0, 1))
            config.write_text('[experimental]\nwidescreen = true\n')
            file, effective, where = model(['--settings', str(config)])
            self.assertEqual((file['widescreen'], effective['widescreen']), (1, 1))
            file, effective, where = model(['--settings', str(config), '--no-widescreen'])
            self.assertEqual((file['widescreen'], effective['widescreen']), (1, 0))
            for key in fields:
                if key != 'widescreen':
                    self.assertEqual(file[key], effective[key], key)
            config.unlink()
            config.write_text('[experimental]\nsettings_menu = true\nfast_loads = false\n[display]\nfilter = bilinear\nscreen = borderless\nwindow_size = 640x480\nfullscreen_size = 1024x768\n')
            file, effective, where = model(['--settings', str(config), '--no-settings-menu', '--nearest-scaling', '--fast-loads'])
            self.assertEqual(where, str(config))
            self.assertEqual((file['settings_menu'], file['fast_loads'], file['filter'], file['screen'], file['window_w'], file['window_h'], file['fullscreen_w'], file['fullscreen_h']),
                             (1, 0, 1, 2, 640, 480, 1024, 768))
            self.assertEqual((effective['settings_menu'], effective['fast_loads'], effective['filter'], effective['screen'], effective['window_w'], effective['fullscreen_w']),
                             (0, 1, 0, 2, 640, 1024))
            for key in fields:
                if key not in ('settings_menu', 'fast_loads', 'filter'):
                    self.assertEqual(file[key], effective[key], key)
            # the legacy key lands in the file model as filter
            config.write_text('[experimental]\nsharp_bilinear = true\n')
            file, effective, where = model([])
            self.assertEqual((file['filter'], effective['filter']), (2, 2))
            file, effective, where = model(['--nearest-scaling'])
            self.assertEqual((file['filter'], effective['filter']), (2, 0))
            # a refusal prints no model
            result = subprocess.run([str(exe), '--no-audio', '--settings', 'missing.ini'], cwd=path, capture_output=True, text=True, timeout=3)
            self.assertEqual(result.returncode, 2)
            self.assertNotIn('file ', result.stdout)
            config.unlink()
            run(['--settings', 'missing.ini'], 2)
            # A build with PsyZ (settings.c compiled with PORT_HAVE_PSYZ): with no settings file every setting is on but the arcade intro.
            psyz = path / 'startup-psyz'
            subprocess.run(['gcc', '-std=c11', '-O1', '-DPORT_HAVE_PSYZ', '-I', str(ROOT / 'port/src'),
                str(path / 'test.c'), str(ROOT / 'port/src/settings.c'), '-o', str(psyz)], check=True, timeout=15)
            empty = path / 'empty'
            empty.mkdir()
            shown = subprocess.run([str(psyz), '--no-audio', '--show-settings'], cwd=empty, capture_output=True, text=True, timeout=3)
            self.assertEqual(shown.returncode, 0, shown.stdout + shown.stderr)
            for line in ('pixel_perfect_ui = true', 'arcade_video = true', 'center_ui = true', 'fast_loads = true', 'memory_card = true',
                         'arcade_intro = false', 'arcade_assets = false', 'settings_menu = false', 'widescreen = false', 'screen = window', 'window_size = 1280x960',
                         'fullscreen_size = desktop', 'filter = sharp_bilinear'):
                self.assertIn(line, shown.stdout.splitlines())
            self.assertNotIn('sharp_bilinear =', shown.stdout)
            # The template port/settings.ini is those defaults.
            template = subprocess.run([str(psyz), '--no-audio', '--show-settings', '--settings', str(ROOT / 'port/settings.ini')], cwd=empty,
                                      capture_output=True, text=True, timeout=3)
            self.assertEqual(template.stdout, shown.stdout)
            config.write_text('[experimental]\narcade_video = typo\n')
            run([], 2)
            config.write_text('[experimental]\narcade_video = true\narcade_video = false\n')
            run([], 2)

if __name__ == '__main__':
    unittest.main()
