"""Native build/fixture helpers for the isolated intro sequence checks.

Reads only sequence.c/.h and synthetic fixtures. A native compile takes <1 s.
No game data or CPU emulator is needed by this module or the unit controls.
"""
from contextlib import contextmanager
import ctypes
import json
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
INTRO = ROOT / 'port/intro'


class State(ctypes.Structure):
    _fields_ = [('record', ctypes.c_uint32), ('remaining', ctypes.c_uint16),
                ('flags', ctypes.c_uint16)]


@contextmanager
def native(control=False):
    cc = shutil.which('cc')
    if not cc:
        raise RuntimeError('intro sequence controls require native cc')
    with tempfile.TemporaryDirectory(prefix='intro-sequence-') as tmp:
        source = INTRO / 'sequence.c'
        if control:
            text = source.read_text()
            old = 'next = state->record + 16u;'
            if text.count(old) != 1:
                raise RuntimeError('control mutation is no longer unique')
            source = Path(tmp) / 'mutant.c'
            source.write_text(text.replace(old, 'next = state->record + 8u;'))
        library = Path(tmp) / 'sequence.so'
        subprocess.run([cc, '-std=c99', '-O2', '-shared', '-fPIC', '-Wall',
                        '-Wextra', '-Werror', '-I', str(INTRO), str(source),
                        '-o', str(library)], check=True, timeout=20)
        dll = ctypes.CDLL(str(library))
        args = [ctypes.c_void_p, ctypes.c_size_t]
        dll.intro_sequence_select.argtypes = args + [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(State)]
        dll.intro_sequence_select.restype = ctypes.c_int
        dll.intro_sequence_tick.argtypes = args + [ctypes.POINTER(State)]
        dll.intro_sequence_tick.restype = ctypes.c_int
        yield dll


def invoke(dll, op, rom, before, table=0, index=0):
    # Guard the host-side state too. ROM is immutable by API, verified below.
    guarded = ctypes.create_string_buffer(b'\xa5' * 24, 24)
    state = State.from_buffer(guarded, 8)
    state.record, state.remaining, state.flags = before
    data = ctypes.create_string_buffer(bytes(rom), len(rom)) if rom is not None else None
    size = len(rom) if rom is not None else 0
    if op == 'select':
        result = dll.intro_sequence_select(data, size, table, index, ctypes.byref(state))
    elif op == 'tick':
        result = dll.intro_sequence_tick(data, size, ctypes.byref(state))
    else:
        raise ValueError('unknown operation')
    if guarded.raw[:8] != b'\xa5' * 8 or guarded.raw[16:] != b'\xa5' * 8:
        raise AssertionError('native C wrote outside its state')
    if data is not None and data.raw != bytes(rom):
        raise AssertionError('native C wrote to ROM')
    return result, [state.record, state.remaining, state.flags]


def fixtures():
    return json.loads((INTRO / 'sequence-fixtures.json').read_text())


def fixture_rom(case):
    rom = bytearray(case['size'])
    for at, text in case['data']:
        payload = bytes.fromhex(text)
        if at < 0 or at + len(payload) > len(rom):
            raise ValueError('fixture edit outside ROM')
        rom[at:at+len(payload)] = payload
    return rom
