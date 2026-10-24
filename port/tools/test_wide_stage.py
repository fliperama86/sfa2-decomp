"""Fast fixtures of widescreen's side tiles (port/src/wide_stage.c, port/tools/wide_stage_test.c; port/widescreen.md "The side tiles").
No game, disc or window. Two parts:

 * a 32-bit program (the game's C holds 32-bit pointers) that links the game's own C of the stage ryu for the three plain forms and of chunli for the other two with the
   maker and compares their packets (built here with gcc -m32; the structs come from ps1/src/types.fields through structgen);
 * the table of stage copies in wide_stage.c against ps1/src: every copy exists with the declared pool, count and layer, no copy of
   the forms is missing, and the C of the copies of a form is the same up to names and addresses.

Prints `ok   NAME` or `FAIL NAME: why` per case, then the summary line that the review reads.
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
STAGES = ROOT / 'ps1/src/overlays/stages'
FORMS = {  # form tag -> (function prefix, layer expression in the C, record type, layer index)
    'L0': ('draw_tile_layer_0_16px', 'background_layers', 'Slot06Tile', 0),
    'L1': ('draw_tile_layer_1_16px', '&background_layers[1]', 'Slot06Tile', 1),
    'T32': ('draw_tilemap_layer_32px_9x13', '&background_layers[2]', 'Slot06TileW', 2),
    'SPR': ('draw_tile_layer_sprites_with_partial_row', '(Slot06Draw *)main_layer', 'Slot06Tile', 1),     # main_layer is background_layers[1]
    'SKEW': ('draw_skewed_tile_layer_quads', '(Slot06Layer *)main_layer', 'Poly28', 1),
    'T32_7': ('draw_tilemap_layer_32px_7x13', '&background_layers[2]', 'Slot06TileW', 2),
    'T16H': ('draw_tile_map_layer_16px_half_scroll', 'background_layers', 'Slot06Tile', 0),
    # draw_tile_map_layer_32px has two bodies: the half-speed scroll of sakura and vega, and gouki's (full speed, 13 records at most)
    'T32H': ('draw_tile_map_layer_32px', '&background_layers[2]', 'Slot06TileW', 2),
    'T32G': ('draw_tile_map_layer_32px', '&background_layers[2]', 'Slot06TileW', 2),
    # the layer functions of one stage each (or two stages that differ): the second layer's scrolling rows (birdie's and chunli_alt's are two
    # bodies), the first layer's 16 px grids (rose; gouki, which also skips the next layer's blank; nash's likewise; ken's), nash's rows from
    # the camera and quads, rolento's perspective, 16 px and 32 px, and sodom's interpolated rows
    'SCRB': ('draw_scrolling_tile_map_layer', '(Slot06Draw *)main_layer', 'Slot06Tile', 1),
    'SCRC': ('draw_scrolling_tile_map_layer', '(Slot06Draw *)main_layer', 'Slot06Tile', 1),
    'L0G': ('draw_tile_map_layer_16px', 'background_layers', 'Slot06Tile', 0),
    'L0R': ('draw_tile_map_layer_16px', 'background_layers', 'Slot06Tile', 0),
    'L0N': ('draw_tile_map_layer_16px_slot03', 'background_layers', 'Slot06Tile', 0),
    'L0K': ('draw_tilemap_layer_16px_12x25', 'background_layers', 'Slot06Tile', 0),
    'SAVE': ('draw_tile_layer_sprites_saving_pos', '&background_layers[1]', 'Slot06Tile', 1),
    'POLY': ('draw_tile_map_layer_polys', None, 'Poly28', 1),            # reads main_layer without a local
    'SKEWP': ('draw_perspective_skewed_tile_map', '(Slot06Layer *)main_layer', 'Poly28', 1),
    'R16': ('draw_tile_map_16px_tiles', 'background_layers', 'Slot06Tile', 0),
    'R32': ('draw_tile_map_layer_sprites_32px', '&background_layers[2]', 'Slot06TileW', 2),
    'SPRI': ('draw_tile_map_layer_sprites_interpolated', '(Slot06Draw *)main_layer', 'Slot06Tile', 1),
}
HALF_SCROLL = 'scroll_y >> 1'     # the text that tells the half-speed body from the other
# forms that share a function name with another form tell their bodies apart by a text of the C
BODY_IS = {
    'T32H': lambda text: HALF_SCROLL in text, 'T32G': lambda text: HALF_SCROLL not in text,
    'SCRC': lambda text: 'data_8019054e' in text, 'SCRB': lambda text: 'data_8019054e' not in text,
    'L0G': lambda text: 'blank2' in text, 'L0R': lambda text: 'blank2' not in text,
}
# the stage whose C is compiled for the comparison: ryu for the plain forms, chunli for the other two
GAME_C = [('ryu', 'draw_tile_layer_0_16px_slot06_00'), ('ryu', 'draw_tile_layer_1_16px_slot06_00'),
          ('ryu', 'draw_tilemap_layer_32px_9x13_slot06_00'), ('chunli', 'draw_tile_layer_sprites_with_partial_row_slot06_04'),
          ('chunli', 'draw_skewed_tile_layer_quads_slot06_04'), ('ken', 'draw_tilemap_layer_32px_7x13_slot06_01'),
          ('sakura', 'draw_tile_map_layer_16px_half_scroll_slot06_0d'), ('sakura', 'draw_tile_map_layer_32px_slot06_0d'),
          ('gouki', 'draw_tile_map_layer_32px_slot06_02'), ('birdie', 'draw_scrolling_tile_map_layer_slot06_08'),
          ('chunli_alt', 'draw_scrolling_tile_map_layer_slot06_12'), ('gouki', 'draw_tile_map_layer_16px_slot06_02'),
          ('nash', 'draw_tile_map_layer_16px_slot03_slot06_03'), ('rose', 'draw_tile_map_layer_16px_slot06_09'),
          ('ken', 'draw_tilemap_layer_16px_12x25_slot06_01'), ('nash', 'draw_tile_layer_sprites_saving_pos_slot06_03'),
          ('nash', 'draw_tile_map_layer_polys_slot06_03'), ('rolento', 'draw_perspective_skewed_tile_map_slot06_0e'),
          ('rolento', 'draw_tile_map_16px_tiles_slot06_0e'), ('rolento', 'draw_tile_map_layer_sprites_32px_slot06_0e'),
          ('sodom', 'draw_tile_map_layer_sprites_interpolated_slot06_06')]
COPIES = 13 + 6 + 13 + 10 + 10 + 3 + 2 + 3 + 12
LAYERS = 0x801aa544
CHAN_SIZE = 0x90
CFLAGS = ['-m32', '-fno-pie', '-D_GNU_SOURCE', '-O1', '-Wall', '-Wextra', '-Werror']


def symbols() -> dict[str, int]:
    out = {}
    for line in (ROOT / 'ps1/src/symbols.ld').read_text().splitlines():
        m = re.match(r'(\w+)\s*=\s*(0x[0-9a-fA-F]+);', line)
        if m:
            out[m.group(1)] = int(m.group(2), 16)
    return out


def can_run_32bit(folder: Path) -> bool:
    source = folder / 'probe.c'
    source.write_text('int main(void) { return 0; }\n')
    try:
        subprocess.run(['gcc', '-m32', str(source), '-o', str(folder / 'probe')], check=True, timeout=30, capture_output=True)
        return subprocess.run([str(folder / 'probe')], timeout=10).returncode == 0
    except (subprocess.CalledProcessError, OSError):
        return False


def build_and_run(folder: Path) -> tuple[list[str], str]:
    """Builds the native program and returns its output lines and the table it prints."""
    sym = symbols()
    header = folder / 'types.gen.h'
    subprocess.run([sys.executable, '-I', str(ROOT / 'ps1/tools/structgen.py'), 'generate', str(ROOT / 'ps1/src/types.fields'),
                    '-o', str(header)], check=True, timeout=60, capture_output=True)
    game_c = [str(STAGES / stage / f'{name}.c') for stage, name in GAME_C]
    defsyms = ['game_state', 'background_layers', 'main_layer', 'current_ordering_table', 'draw_buffer_index',
               'data_801ec390_slot06_00', 'data_801edc80_slot06_00', 'data_801f1790_slot06_00',
               'data_801f03c8_slot06_04', 'data_801f4ae0_slot06_04', 'data_801f94bc_slot06_01', 'data_801ed5e4_slot06_0d',
               'data_801f51dc_slot06_0d', 'data_801f8638_slot06_02', 'data_801f8978_slot06_02', 'data_8019054e',
               'data_801f085c_slot06_08', 'data_801f11e0_slot06_12', 'data_801ef0e8_slot06_02', 'data_801ee810_slot06_03',
               'data_801f8000_slot06_03', 'data_801eca24_slot06_09', 'data_801eedb4_slot06_01', 'data_801f0d40_slot06_03',
               'data_801f4c40_slot06_03', 'data_801f3df4_slot06_0e', 'data_801ef554_slot06_0e', 'data_801f6734_slot06_0e',
               'data_801f25b0_slot06_06']
    link = [f'-Wl,--defsym={name}=0x{sym[name]:08x}' for name in defsyms]
    exe = folder / 'wide_stage_test'
    objects = []
    for i, source in enumerate(game_c):                  # the game's C as it is (its own warnings are not ours to fix)
        obj = folder / f'game_{i}.o'
        done = subprocess.run(['gcc', '-m32', '-fno-pie', '-O1', '-w', '-I', str(folder), '-c', source, '-o', str(obj)],
                              capture_output=True, text=True, timeout=120)
        if done.returncode != 0:
            raise RuntimeError('the game\'s C did not build:\n' + done.stdout + done.stderr)
        objects.append(str(obj))
    cmd = ['gcc', *CFLAGS, '-no-pie', '-I', str(folder), '-I', str(ROOT / 'port/src'),
           str(ROOT / 'port/src/wide_stage.c'), str(ROOT / 'port/tools/wide_stage_test.c'), *objects, *link, '-o', str(exe)]
    done = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    if done.returncode != 0:
        raise RuntimeError('build failed:\n' + done.stdout + done.stderr)
    run = subprocess.run([str(exe)], capture_output=True, text=True, timeout=60)
    table = subprocess.run([str(exe), 'table'], capture_output=True, text=True, timeout=30)
    if table.returncode != 0:
        raise RuntimeError('the table could not be printed: ' + table.stderr)
    return (run.stdout.splitlines() + ([f'FAIL exit: the program ended with status {run.returncode}'] if run.returncode else [])), table.stdout


def strip_c(text: str) -> str:
    return re.sub(r'/\*.*?\*/', '', text, flags=re.S)


def normalised(path: Path, stage: str) -> str:
    """The function's C without comments, names and addresses of its copy: what must be the same in every copy of a form."""
    text = strip_c(path.read_text())
    text = re.sub(r'data_[0-9a-f]{8}_slot06_' + stage, 'POOL', text)
    text = re.sub(r'slot06_' + stage, 'slot06_XX', text)
    text = re.sub(r'extern [^\n]*POOL[^\n]*\n', '', text)       # the pool's size is the stage's own
    text = re.sub(r'void [A-Za-z0-9_]+\(void\);', '', text)         # a prototype of the function or of its model
    return re.sub(r'\s+', ' ', text).strip()


def source_copies() -> dict[str, dict]:
    """Every copy of the forms under ps1/src, from the C: name -> form, layer, pool, count, path."""
    found = {}
    for form, (prefix, chan, record, index) in FORMS.items():
        for path in sorted(STAGES.glob(f'*/{prefix}_slot06_??.c')):
            text = strip_c(path.read_text())
            if form in BODY_IS and not BODY_IS[form](text):
                continue                                           # the other body of the same function name
            pool = re.search(r'extern ' + record + r' data_([0-9a-f]{8})_slot06_([0-9a-f]{2})\[2\]\[(\w+)\];', text)
            layer = re.search(r'\bc = ([^;]+);', text)
            found[path.stem] = {'form': form, 'path': path, 'pool': int(pool.group(1), 16) if pool else None,
                                'stage': pool.group(2) if pool else None, 'count': int(pool.group(3), 0) if pool else None,
                                'chan': layer.group(1) if layer else None}
    return found


def parse_table(text: str) -> dict[str, tuple]:
    out = {}
    for line in text.splitlines():
        name, form, layer, pool, count = line.split()
        out[name] = (form, int(layer, 16), int(pool, 16), int(count))
    return out


def check_table(table: dict[str, tuple]) -> str | None:
    found = source_copies()
    problems = []
    for name, (form, layer, pool, count) in table.items():
        s = found.get(name)
        if s is None:
            problems.append(f'{name} is not in ps1/src')
            continue
        if s['form'] != form:
            problems.append(f'{name}: the form is {s["form"]}, the table says {form}')
        if s['pool'] != pool:
            problems.append(f'{name}: the pool is {s["pool"] and hex(s["pool"])}, the table says {hex(pool)}')
        if s['count'] != count:
            problems.append(f'{name}: the pool holds {s["count"]} records a buffer, the table says {count}')
        if s['chan'] != FORMS[form][1]:
            problems.append(f'{name}: the layer is {s["chan"]}, the form {form} has {FORMS[form][1]}')
        if layer != LAYERS + FORMS[form][3] * CHAN_SIZE:
            problems.append(f'{name}: the table has layer {hex(layer)}, not background_layers[{FORMS[form][3]}]')
    for name in found:
        if name not in table:
            problems.append(f'{name} is in ps1/src and not in the table')
    return '; '.join(problems) or None


def check_identical() -> str | None:
    problems = []
    for form, (prefix, _, _, _) in FORMS.items():
        bodies = {}
        for name, s in source_copies().items():
            if s['form'] == form:
                bodies.setdefault(normalised(s['path'], s['stage']), []).append(name)
        if len(bodies) != 1:
            problems.append(f'{form}: {len(bodies)} different bodies: ' + ' | '.join(','.join(v) for v in bodies.values()))
    return '; '.join(problems) or None


def main() -> int:
    results: list[tuple[str, str | None]] = []
    with tempfile.TemporaryDirectory() as tmp:
        folder = Path(tmp)
        if not can_run_32bit(folder):
            print('FAIL setup: this machine cannot build and run 32-bit programs (gcc -m32), which the game\'s C needs')
            return 1
        try:
            lines, table_text = build_and_run(folder)
        except (RuntimeError, subprocess.SubprocessError, OSError) as err:
            print(f'FAIL build: {err}')
            return 1
    for line in lines:
        m = re.match(r'(ok  |FAIL) (\S+)(?:: (.*))?$', line)
        if m:
            results.append((m.group(2), None if m.group(1).startswith('ok') else (m.group(3) or 'failed')))
        else:
            print(line)
    table = parse_table(table_text)
    results.append(('table_matches_ps1_src', check_table(table)))
    results.append(('table_has_every_copy', None if len(table) == COPIES else f'{len(table)} copies in the table, {COPIES} expected'))
    results.append(('copies_of_a_form_are_the_same_c', check_identical()))
    bad = 0
    for name, why in results:
        if why is None:
            print(f'ok   {name}')
        else:
            bad += 1
            print(f'FAIL {name}: {why}')
    if not any(n.startswith('maker_reproduces_c') for n, _ in results):
        print('FAIL no comparison with the game\'s C ran')
        return 1
    if bad:
        print(f'{bad} case(s) behaved wrongly')
        return 1
    print(f'all {len(results)} cases behaved as required')
    return 0


if __name__ == '__main__':
    sys.exit(main())
