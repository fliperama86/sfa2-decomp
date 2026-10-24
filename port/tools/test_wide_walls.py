"""Widescreen's walls (port/src/wide_walls.c, port/tools/wide_walls_test.c; port/widescreen.md "The walls of the wide view"). No game, disc or window.

A 32-bit program (the game's C holds 32-bit pointers) links the game's own C of init_sprite_position_wide, clamp_pos_x_to_bounds_and_push_other,
push_both_objects_inside_bounds, update_camera_x_from_player_span, move_cam_left, move_cam_right and set_cam_pos_x with the four overrides, over
memory mapped at the game's addresses, and checks three things: at an extra of 0 each override leaves the memory exactly as the game's C does
(seeded random fixtures); at the extra of 64 the walls stand where the 512 view puts them (hand-made fixtures); with any other setting the
override is the game's C even at 64. The structs come from ps1/src/types.fields through structgen; the symbols are placed with the linker's
--defsym from ps1/src/symbols.ld.

Built with gcc -m32 and run in a few seconds. Skipped, with the reason, if this machine cannot build and run 32-bit programs.
"""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
RESIDENT = ROOT / 'ps1/src/resident'
GAME_C = [RESIDENT / 'graphics/init_sprite_position_wide.c', RESIDENT / 'fighters/clamp_pos_x_to_bounds_and_push_other.c',
          RESIDENT / 'graphics/update_camera_x_from_player_span.c', RESIDENT / 'graphics/move_cam_left.c',
          RESIDENT / 'graphics/move_cam_right.c', RESIDENT / 'graphics/set_cam_pos_x.c']
DEFSYMS = ['game_state', 'player_left', 'player_right', 'box_margin', 'main_layer', 'ref_other', 'ref_first', 'ref_second']
WALLS = ROOT / 'port/src/wide_walls.c'


def symbols() -> dict[str, int]:
    out = {}
    for line in (ROOT / 'ps1/src/symbols.ld').read_text().splitlines():
        m = re.match(r'(\w+)\s*=\s*(0x[0-9a-fA-F]+);', line)
        if m:
            out[m.group(1)] = int(m.group(2), 16)
    return out


def can_run_32bit(folder: Path) -> str | None:
    """None if a 32-bit program builds and runs here, else the reason it does not."""
    source = folder / 'probe.c'
    source.write_text('#include <stdio.h>\nint main(void) { return 0; }\n')
    try:
        done = subprocess.run(['gcc', '-m32', str(source), '-o', str(folder / 'probe')], timeout=30, capture_output=True, text=True)
        if done.returncode != 0:
            return 'gcc -m32 cannot build 32-bit programs here: ' + (done.stderr.strip().splitlines() or ['no message'])[-1]
        if subprocess.run([str(folder / 'probe')], timeout=10).returncode != 0:
            return 'a 32-bit program does not run here'
    except (OSError, subprocess.SubprocessError) as err:
        return f'gcc -m32 is not usable here: {err}'
    return None


def build(folder: Path, walls: Path = WALLS) -> Path:
    """Builds the program (the game's C from ps1/src, `walls` as the overrides) in `folder` and returns its path."""
    sym = symbols()
    subprocess.run([sys.executable, '-I', str(ROOT / 'ps1/tools/structgen.py'), 'generate', str(ROOT / 'ps1/src/types.fields'),
                    '-o', str(folder / 'types.gen.h')], check=True, timeout=60, capture_output=True)

    def compile_game(item: tuple[int, Path]) -> str:
        i, source = item
        obj = folder / f'game_{i}.o'
        done = subprocess.run(['gcc', '-m32', '-fno-pie', '-O1', '-w', '-ffunction-sections', '-I', str(folder), '-c', str(source), '-o', str(obj)],
                              capture_output=True, text=True, timeout=120)       # the game's C as it is
        if done.returncode != 0:
            raise RuntimeError(f'the game\'s C {source.name} did not build:\n' + done.stdout + done.stderr)
        return str(obj)

    with ThreadPoolExecutor(max_workers=len(GAME_C)) as pool:
        objects = list(pool.map(compile_game, enumerate(GAME_C)))
    link = ['-Wl,--gc-sections'] + [f'-Wl,--defsym={name}=0x{sym[name]:08x}' for name in DEFSYMS]
    exe = folder / 'wide_walls_test'
    cmd = ['gcc', '-m32', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror', '-fno-pie', '-no-pie', '-D_GNU_SOURCE',
           '-I', str(folder), '-I', str(ROOT / 'port/src'), str(walls), str(ROOT / 'port/tools/wide_walls_test.c'), *objects, *link, '-o', str(exe)]
    done = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    if done.returncode != 0:
        raise RuntimeError('build failed:\n' + done.stdout + done.stderr)
    return exe


class WideWalls(unittest.TestCase):
    def test_walls(self):
        with tempfile.TemporaryDirectory() as tmp:
            folder = Path(tmp)
            why = can_run_32bit(folder)
            if why:
                self.skipTest(why)
            exe = build(folder)
            result = subprocess.run([str(exe)], capture_output=True, text=True, timeout=60)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('wide walls ok', result.stdout)
            print(result.stdout.strip().splitlines()[-1])


def main() -> int:
    """Each test as one case: `ok   NAME` or `FAIL NAME: why`, then the summary line that the review reads."""
    cases = [t for suite in unittest.defaultTestLoader.loadTestsFromModule(sys.modules[__name__]) for t in suite]
    bad = 0
    for case in cases:
        result = unittest.TestResult()
        case.run(result)
        problems = result.failures + result.errors
        name = case.id().rsplit('.', 1)[-1]
        if problems:
            bad += 1
            print(f"FAIL {name}: {problems[0][1].strip()}")
        elif result.skipped:
            print(f"skip {name}: {result.skipped[0][1]}")
        else:
            print(f"ok   {name}")
    if bad:
        print(f"{bad} case(s) behaved wrongly")
        return 1
    print(f"all {len(cases)} cases behaved as required")
    return 0


if __name__ == '__main__':
    sys.exit(main())
