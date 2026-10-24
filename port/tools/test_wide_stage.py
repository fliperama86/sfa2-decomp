"""Fast fixtures of widescreen's side tiles (port/src/wide_stage.c, port/tools/wide_stage_test.c; port/widescreen.md "The side tiles").
No game, disc or window. Two parts:

 * a 32-bit program (the game's C holds 32-bit pointers) that links the game's own C of the stage ryu for the three forms with the
   maker and compares their packets (built here with gcc -m32; the structs come from ps1/src/types.fields through structgen);
 * the table of stage copies in wide_stage.c against ps1/src: every copy exists with the declared pool, count and layer, no copy of
   the three forms is missing, and the C of the copies of a form is the same up to names and addresses.

Prints `ok   NAME` or `FAIL NAME: why` per case, then the summary line that the review reads.
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
STAGES = ROOT / 'ps1/src/overlays/stages'
FORMS = {  # form tag -> (function prefix, Chan expression in the C, record type, layer index)
    'L0': ('draw_tile_layer_0_16px', 'background_layers', 'Slot06Tile', 0),
    'L1': ('draw_tile_layer_1_16px', '&background_layers[1]', 'Slot06Tile', 1),
    'T32': ('draw_tilemap_layer_32px_9x13', '&background_layers[2]', 'Slot06TileW', 2),
}
RYU = ['draw_tile_layer_0_16px_slot06_00', 'draw_tile_layer_1_16px_slot06_00', 'draw_tilemap_layer_32px_9x13_slot06_00']
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
    game_c = [str(STAGES / 'ryu' / f'{name}.c') for name in RYU]
    defsyms = ['game_state', 'background_layers', 'current_ordering_table', 'draw_buffer_index',
               'data_801ec390_slot06_00', 'data_801edc80_slot06_00', 'data_801f1790_slot06_00']
    link = [f'-Wl,--defsym={name}=0x{sym[name]:08x}' for name in defsyms]
    exe = folder / 'wide_stage_test'
    cmd = ['gcc', *CFLAGS, '-no-pie', '-I', str(folder), '-I', str(ROOT / 'port/src'),
           str(ROOT / 'port/src/wide_stage.c'), str(ROOT / 'port/tools/wide_stage_test.c'), *game_c, *link, '-o', str(exe)]
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
    """Every copy of the three forms under ps1/src, from the C: name -> form, layer, pool, count, path."""
    found = {}
    for form, (prefix, chan, record, index) in FORMS.items():
        for path in sorted(STAGES.glob(f'*/{prefix}_slot06_??.c')):
            text = strip_c(path.read_text())
            pool = re.search(r'extern ' + record + r' data_([0-9a-f]{8})_slot06_([0-9a-f]{2})\[2\]\[(\w+)\];', text)
            layer = re.search(r'Chan \*c = ([^;]+);', text)
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
    results.append(('table_has_every_copy', None if len(table) == 32 else f'{len(table)} copies in the table, 32 expected'))
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
