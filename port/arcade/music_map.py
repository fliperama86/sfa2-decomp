#!/usr/bin/env python3
"""Analysis tool: which arcade song plays where the PS1 plays a recorded music track, found by STRUCTURE (the two games' tables and states), not by audio.

What it does. The PS1 starts a recorded track with a code (XA file * 256 + channel); the arcade starts a song number. This tool pairs them state by
state from the PS1's request sites and the arcade's, and prints one row per PS1 code with the two sites, a confidence, whether the audio match of
music.md section 11 agrees, then the codes and songs left without a partner. It reads the arcade program (decrypted transiently, as
port/intro/import_full.py does, or given with --decrypted) and the disc, and the function addresses of ps1/src/build.toml. It writes nothing, and prints
addresses and numbers, never game bytes. It is an analysis and reference tool; the runtime table (`songs[]` in port/src/arcade.c) is made from it with
--c-table. A check that fails stops the tool with a message.

The arcade side (music.md section 10). One function, 0x15494, requests music with d0 = 2 * selector: selector 0 plays the fighter's theme from the
20-word table at 0x154ee, selector 1 the ending theme from the 24-word table at 0x1552c (indexed by the ending index, game variable 0x130), selector 2
song 62, selectors 3 to 23 the 21-word table at 0x15576. The tool finds every jump or call to it (25 sites) and reads d0 at each (a `moveq` in the
straight run before the call, or a byte table read `move.b X(pc,d0.w),d0`); SITES says what each asks, as read by hand, and the tool stops if the scan
disagrees. The ending code is 18 blocks: the 24 ending indexes go through the block table of the dispatcher at 0x1a44c, six indexes repeat a block, and
a request inside a block belongs to those endings.

The PS1 side. The fighters' themes are table_8016e6f8 of the executable. The ending overlay (slot 0x28) is one program for the 21 endings: its substep
table holds, from entry 8, one scene handler for each ending k = 0..20 (three repeat a scene); a handler indexes the scene's pass table, and the scene's
requests (`set_menu_command_one_and_wait_pending(code)` and `set_menu_command_by_code(1, code)`) are found in the machine code by their `jal` and the
constant in the argument registers, and lie in the functions of that pass table. The staff roll of ending k plays a code from a 21-entry table of the
overlay; the bonus stages (slot 0x2a) play one of two codes by the stage number.

The pairs.
  fighters  fighter k: PS1 table_8016e6f8[k] <-> arcade selector-0 table[k]. Confidence "table".
  endings   ending k: the scene's first request <-> selector-1 table[k] ("table"); the other request of the scene <-> the other song requested in the
            ending's block ("position"). Checked: the PS1 repeats a scene for exactly the ending pairs where the arcade repeats a block (4/18, 2/20, 17/19);
            when reading the code the scene objects' kind numbers agreed too (ending 2 spawns kind 0x76 in both).
  staff     ending k: the code of the PS1's staff table <-> the song the arcade picks for k (byte table 0x95c10 by selector; endings 2 and 20 play their
            ending theme). One ending (18) differs; the rest agree. "table".
  bonus     the two codes of the bonus module <-> the two songs of the byte table 0x9142, which differs only at stage 11 (the PS1 tests stage 0xb). "table".
  declared  the other states, from the pairs in DECLARED: each is verified (the PS1 function loads the code, the arcade site gives the song) and carries
            the evidence that identified the state (a screen's scroll values, the stores before the request, the kind numbers spawned, the left/right
            counters). "position". The last two (match start, the prompt) are only inferred, by elimination and the length class of the tracks, and are
            marked "inferred".

    python3 port/arcade/music_map.py --disc BIN (--arcade ZIP | --decrypted FILE) [--evidence]
    python3 port/arcade/music_map.py --disc BIN --arcade ZIP --c-table [--all]

--c-table prints the rows of songs[] (confidence table and position; --all adds the inferred ones). The disc is the raw 2352-byte-sector image. The
arcade ZIP and the decrypted program are pinned by hash (port/intro/import_scene.py). The decryption is a short C++ compile in a temporary folder.
Under a second on the supplied files.
"""
import argparse
import hashlib
import struct
import sys
import tempfile
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'ps1/tools'))
sys.path.insert(0, str(ROOT / 'port/intro'))

# ---------------------------------------------------------------------------------------------------------- the arcade side

REQUEST = 0x15494                  # the music request function
SELECTOR_JUMPS = 0x154aa           # its word table of 24 entries (selector 0 .. 23)
ENDING_DISPATCH = 0x1a44c          # moveq #0,d0 / move.b $130(a5),d0 / add.w d0,d0 / add.w d0,d0 / movea.l X(pc,d0.w),a0 / jmp (a0)
ENDING_INDEXES = 24
ENDING_CODE = (0x86000, 0x8c600)   # the ending blocks lie here (the last block's end is not marked)
STAFF_FUNCTION = 0x95be8           # staff roll: endings 2 and 20 play the ending theme, the others a song by the byte table
BONUS_SITE = 0x913c                # bonus stages: a byte table by the stage count (byte 0x147)


def be16(b, a):
    return b[a] << 8 | b[a + 1]


def be32(b, a):
    return b[a] << 24 | b[a + 1] << 16 | b[a + 2] << 8 | b[a + 3]


def pc_table(prog, a):
    """The base of the table that the `move.w/move.b/movea.l X(pc,d0.w),..` at `a` reads: the address of the extension word plus its 8-bit displacement."""
    ext = be16(prog, a + 2)
    disp = ext & 0xff
    return a + 2 + (disp - 256 if disp > 127 else disp)


def request_tables(prog):
    """The song tables of the request function, read through its code: ({0: [fighter themes], 1: [ending themes], 3: [selectors 3 to 23]}, song of selector 2)."""
    if be16(prog, REQUEST) != 0x4a2d or be16(prog, REQUEST + 2) != 0x00a6:
        raise ValueError('the request function does not start with tst.b $a6(a5)')
    jumps = [SELECTOR_JUMPS + be16(prog, SELECTOR_JUMPS + 2 * i) for i in range(24)]
    if len({jumps[0], jumps[1], jumps[2]}) != 3 or len(set(jumps[3:])) != 1:
        raise ValueError('the selector jump table is not "three different, then one for all"')
    starts, tables = sorted(set(jumps)), {}
    for sel in (0, 1, 3):
        body = jumps[sel]
        a = body
        while be16(prog, a) != 0x323b:               # move.w X(pc,d0.w),d1
            a += 2
            if a > body + 0x20:
                raise ValueError('selector %d: no table read' % sel)
        base = pc_table(prog, a)
        n = (min(x for x in starts if x > body) - base) // 2      # the table ends where the next selector's code starts
        tables[sel] = [be16(prog, base + 2 * i) for i in range(n)]
    if be16(prog, jumps[2]) != 0x723e:                # moveq #$3e,d1: song 62
        raise ValueError('selector 2 does not send song 62')
    return tables, 0x3e


def song_of(tables, sel, index=0):
    """Song of selector `sel` (with the table index for selectors 0 and 1)."""
    t, s2 = tables
    if sel == 2:
        return s2
    if sel in (0, 1):
        return t[sel][index]
    return t[3][sel - 3]


def request_sites(prog):
    """Addresses of every `jsr/jmp REQUEST.l`."""
    out = []
    for a in range(0, len(prog) - 5, 2):
        if be16(prog, a) in (0x4eb9, 0x4ef9) and be32(prog, a + 2) == REQUEST:
            out.append(a)
    return out


def is_break(w):
    """A word that ends a straight run when scanning backwards: an absolute jump or call, return, branch always or to subroutine."""
    return w in (0x4eb9, 0x4ef9, 0x4e75, 0x6000, 0x6100) or (w >> 8) in (0x60, 0x61) and (w & 0xff) not in (0, 0xff)


def d0_values(prog, site, window=0x18):
    """What d0 can hold at the request: ('imm', sorted values of 2 * selector) or ('table', address of a byte table indexed by d0)."""
    if be16(prog, site - 4) == 0x103b and be16(prog, site - 2) < 0x100:
        return 'table', pc_table(prog, site - 4)
    vals = set()
    p = site - 2
    while p >= site - window:
        w = be16(prog, p)
        if is_break(w):
            break
        if w >> 8 == 0x70:                            # moveq #n,d0
            vals.add(w & 0xff)
        p -= 2
    return 'imm', sorted(vals)


def byte_table(prog, base, n):
    return list(prog[base:base + n])


def site_songs(prog, tables, site, ending=None):
    """(kind, d0 values or table address, songs) requested at `site`. A selector-1 request (the ending theme) gives the song of ending index `ending`, or
    every ending theme when it is None; selector 0 (the fighter's theme) always gives every fighter theme."""
    kind, v = d0_values(prog, site)
    songs = set()
    if kind == 'imm':
        d0s = v
    else:
        d0s = [d for d in set(byte_table(prog, v, 24)) if 6 <= d <= 0x2e and d % 2 == 0]
    for d0 in d0s:
        sel = d0 // 2
        if sel == 0:
            songs.update(tables[0][0])
        elif sel == 1:
            songs.update(tables[0][1] if ending is None else [tables[0][1][ending]])
        else:
            songs.add(song_of(tables, sel))
    return kind, v, sorted(songs)


def ending_blocks(prog):
    """The 24 block addresses of the ending dispatcher, after checking its code."""
    a = ENDING_DISPATCH
    code = [be16(prog, a + 2 * i) for i in range(7)]
    if code[:3] != [0x7000, 0x102d, 0x0130] or code[3:5] != [0xd040, 0xd040] or code[5] != 0x207b or code[6] != 0x0004 or be16(prog, a + 14) != 0x4ed0:
        raise ValueError('the ending dispatcher at 0x%x is not the expected code' % a)
    base = pc_table(prog, a + 10)
    return [be32(prog, base + 4 * i) for i in range(ENDING_INDEXES)]


def block_range(blocks, k):
    """[start, end) of the block of ending k: from its address to the next distinct block address (the last to the end of the ending code)."""
    starts = sorted(set(blocks) | {ENDING_CODE[1]})
    s = blocks[k]
    return s, min(x for x in starts if x > s)


def staff_songs(prog, tables):
    """Song of the staff roll for ending indexes 0..23 (endings 2 and 20 play their ending theme)."""
    a = STAFF_FUNCTION
    if [be16(prog, a), be16(prog, a + 2)] != [0x0c2d, 0x0002] or be16(prog, a + 8) != 0x0c2d or be16(prog, a + 10) != 0x0014:
        raise ValueError('the staff-roll function at 0x%x is not the expected code' % a)
    kind, base = d0_values(prog, a + 0x1a)
    if kind != 'table':
        raise ValueError('the staff roll does not read a byte table')
    out = []
    for k in range(ENDING_INDEXES):
        if k in (2, 20):
            out.append(tables[0][1][k])
        else:
            out.append(song_of(tables, prog[base + k] // 2))
    return out


def bonus_songs(prog, tables):
    """Songs of the bonus stages by the stage count (0..21): from the byte table read at BONUS_SITE."""
    kind, base = d0_values(prog, BONUS_SITE)
    if kind != 'table':
        raise ValueError('the bonus request does not read a byte table')
    return [song_of(tables, prog[base + i] // 2) for i in range(22)]


# ---------------------------------------------------------------------------------------------------------- the PS1 side

def mips_words(data, base, lo, hi):
    """The instruction words of [lo, hi) of an image loaded at `base`."""
    return list(struct.unpack_from('<%dI' % ((hi - lo) // 4), data, lo - base))


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def writes_register(w, reg):
    """Does the instruction word write general register `reg` (approximately: the forms a compiler emits)?"""
    op = w >> 26
    if op == 0:
        return (w >> 11) & 31 == reg and (w & 0x3f) not in (0x08, 0x18, 0x19, 0x1a, 0x1b)
    if 8 <= op <= 0xf or 0x20 <= op <= 0x26:
        return (w >> 16) & 31 == reg
    return False


def immediate_of(w, reg):
    """The value `addiu reg,zero,imm` / `ori reg,zero,imm` loads, or None."""
    if w >> 26 in (9, 0xd) and (w >> 21) & 31 == 0 and (w >> 16) & 31 == reg:
        return w & 0xffff
    return None


def is_control_flow(w):
    """A branch, jump or call (the code before it is another run of instructions)."""
    op = w >> 26
    return 1 <= op <= 7 or (op == 0 and w & 0x3f in (8, 9))


def argument(words, i, reg, reach=48):
    """The immediate in argument register `reg` at the call (the `jal` at words[i], its delay slot at i + 1): the closest write in the delay slot or in the straight run
    of instructions before the call (the compiler may load the argument early and fill the delay slot with something else)."""
    for j in (i + 1, *range(i - 1, max(i - reach - 1, -1), -1)):
        if j >= len(words):
            continue
        if j < i and is_control_flow(words[j]):
            return None
        if writes_register(words[j], reg):
            return immediate_of(words[j], reg)
    return None


def music_calls(words, base, lo, by_code, pending):
    """[(address, code)] of the calls of set_menu_command_by_code(1, code) and set_menu_command_one_and_wait_pending(code) that pass a constant."""
    out = []
    for i, w in enumerate(words):
        if w >> 26 != 3:
            continue
        target = (w & 0x3ffffff) << 2 | 0x80000000
        addr = lo + 4 * i
        if target == pending:
            code = argument(words, i, 4)
            if code is not None:
                out.append((addr, code))
        elif target == by_code and argument(words, i, 4) == 1:
            code = argument(words, i, 5)
            if code is not None:
                out.append((addr, code))
    return out


def loads_immediate(words, value):
    return any(w >> 26 in (9, 0xd) and (w >> 21) & 31 == 0 and w & 0xffff == value for w in words)


def handler_table(words):
    """The table a scene handler indexes: the address built by `lui r,hi` followed by `addiu r,r,lo` in its first instructions, or None."""
    for i in range(min(len(words) - 1, 14)):
        a, b = words[i], words[i + 1]
        if a >> 26 == 0xf and b >> 26 == 9 and (b >> 21) & 31 == (a >> 16) & 31 == (b >> 16) & 31:
            return ((a & 0xffff) << 16) + sext16(b & 0xffff) & 0xffffffff
    return None


def pointer_run(read_word, start, in_code, limit=64):
    """The consecutive words from `start` that are code addresses."""
    out = []
    while len(out) < limit and in_code(read_word(start + 4 * len(out))):
        out.append(read_word(start + 4 * len(out)))
    return out


class Overlay:
    """A chunk of a PAC archive loaded at `base`."""

    def __init__(self, data, base):
        self.data, self.base = data, base

    def word(self, a):
        return struct.unpack_from('<I', self.data, a - self.base)[0]

    def words(self, lo, hi):
        return mips_words(self.data, self.base, lo, hi)


# ---------------------------------------------------------------------------------------------------------- pairing

def pair_ending(ps1_codes, arcade_first, arcade_mids):
    """Pairs for one ending: (code, song, kind) for the first request ("table") and, when the counts agree, the others ("position").
    ps1_codes: the PS1 scene's requests in order; arcade_first: the ending theme of this index; arcade_mids: the other songs requested in its block."""
    first = ps1_codes[0]
    out = [(first, arcade_first, 'table')]
    others = []
    for c in ps1_codes[1:]:
        if c != first and c not in others:
            others.append(c)
    mids = sorted(set(arcade_mids) - {arcade_first})
    if len(others) != len(mids):
        raise ValueError('ending scene with first request 0x%03x: %d other codes against %d other songs' % (first, len(others), len(mids)))
    if len(others) == 1:
        out.append((others[0], mids[0], 'position'))
    elif others:
        raise ValueError('ending scene with first request 0x%03x: several other requests, no order to pair them by' % first)
    return out


def merge_rows(rows):
    """{code: (song, confidence, family)} from (code, song, confidence, family, ...) rows; the same code with two songs is an error."""
    out = {}
    for r in rows:
        code, song, conf, family = r[:4]
        if code in out and out[code][0] != song:
            raise ValueError('code 0x%03x paired with songs %d (%s) and %d (%s)' % (code, out[code][0], out[code][2], song, family))
        if code not in out or CONFIDENCE.index(conf) < CONFIDENCE.index(out[code][1]):
            out[code] = (song, conf, family)
    return out


CONFIDENCE = ('table', 'position', 'inferred')


def c_table(merged, include_inferred, columns=8):
    """The rows of songs[] in port/src/arcade.c: {code, song}, in order of code, a few to a line."""
    items = [(c, s) for c, (s, conf, _) in sorted(merged.items()) if include_inferred or conf != 'inferred']
    lines, cur = [], []
    for c, s in items:
        cur.append('{0x%03x, %d}' % (c, s))
        if len(cur) == columns:
            lines.append('    ' + ', '.join(cur) + ',')
            cur = []
    if cur:
        lines.append('    ' + ', '.join(cur) + ',')
    return '\n'.join(lines)


# ---------------------------------------------------------------------------------------------------------- inputs

def iso_files(f):
    """{path: (lba, size)} of the disc's ISO 9660 files (raw 2352-byte sectors, data at +24), and the sector reader."""
    def sec(lba, n=1):
        out = b''
        for i in range(n):
            f.seek((lba + i) * 2352 + 24)
            out += f.read(2048)
        return out
    files = {}

    def walk(lba, size, path):
        d, i = sec(lba, (size + 2047) // 2048), 0
        while i < len(d):
            ln = d[i]
            if ln == 0:
                i = (i // 2048 + 1) * 2048
                continue
            e = d[i:i + ln]
            i += ln
            el, sz = struct.unpack('<I', e[2:6])[0], struct.unpack('<I', e[10:14])[0]
            nm = e[33:33 + e[32]]
            if nm in (b'\0', b'\1'):
                continue
            p = path + '/' + nm.decode('latin1').split(';')[0]
            if e[25] & 2:
                walk(el, sz, p)
            else:
                files[p] = (el, sz)
    root = sec(16)[156:156 + 34]
    walk(struct.unpack('<I', root[2:6])[0], struct.unpack('<I', root[10:14])[0], '')
    return files, sec


def decrypted_program(zip_path, workdir):
    """The arcade program in the 68000's view, decrypted (port/intro/import_full.py's method, built in `workdir`)."""
    import ctypes
    import subprocess
    import import_scene as IS
    with zipfile.ZipFile(zip_path) as z:
        if hashlib.sha256(Path(zip_path).read_bytes()).hexdigest() != IS.ARCHIVE_SHA:
            raise ValueError('unsupported arcade archive (not the pinned set)')
        raw = b''.join(z.read('sz2u.%02d' % i) for i in range(3, 9))
        key = z.read('sfa2u.key')
    data = bytearray(len(raw))
    data[::2], data[1::2] = raw[1::2], raw[::2]
    folder = ROOT / 'port/intro/tools/cps2crypt'
    so = Path(workdir) / 'decrypt.so'
    subprocess.run(['c++', '-std=c++17', '-O2', '-shared', '-fPIC', '-Wno-sign-compare', str(folder / 'cps2crypt.cpp'), str(folder / 'decrypt.cpp'), '-o', str(so)], check=True, timeout=120)
    dll = ctypes.CDLL(str(so))
    dll.intro_import_decrypt.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p]
    src, k, out = ctypes.create_string_buffer(bytes(data)), ctypes.create_string_buffer(key), ctypes.create_string_buffer(len(data))
    if dll.intro_import_decrypt(src, len(data), k, len(key), out) != 0:
        raise ValueError('decryption failed')
    return check_program(out.raw)


def check_program(prog):
    import import_scene as IS
    if len(prog) != 0x300000 or hashlib.sha256(prog).hexdigest() != IS.OPCODE_SHA:
        raise ValueError('the decrypted program is not the pinned one')
    return prog


# ---------------------------------------------------------------------------------------------------------- the declared pairs

# code, song, confidence, PS1 function (its image from build.toml; the function must load the code), arcade request sites (must give the song), why.
DECLARED = [
    (0x307, 20, 'position', 'load_slot27_screen_graphics_and_camera_slot27', [0x19b8e, 0x19d12],
     'select screen: the PS1 sets the three layers to scroll (0x248,0x100) (0x148,0) (0x48,0x700) and requests; the arcade state before the request sets '
     '(0x240,0x100) (0x140,0) (0x40,0x700), the same plus 8 (arcade song 21 ends the screen)'),
    (0x406, 23, 'position', 'init_substep_screen_and_spawn_objects_slot12', [0x6c58],
     'continue countdown: both request, then spawn object kind 0x7b (the arcade also kind 0xd first)'),
    (0x607, 25, 'position', 'init_step_screen_and_spawn_blocks_slot12', [0x6fb2],
     'next state after the continue (the same two images as the continue screen, kind 0x26 spawned after the request, kinds 0x7c and 0x7e nearby)'),
    (0x603, 27, 'position', 'advance_left_rank_and_reload_screen_slot12', [0xd6d2],
     'left ranking board (counter left_rank_index, stops at 5); arcade d306 variant 0 counts -0x584c down to 5 and picks song 27 (selector 11)'),
    (0x606, 26, 'position', 'advance_right_rank_and_reload_screen_slot12', [0xd6d2],
     'right ranking board; arcade d31e variant 1 counts -0x584a and picks song 26 (selector 10). The PS1 attract screens use 0x606 for both boards'),
    (0x605, 22, 'position', 'init_slot01_screen_and_spawn_objects_slot01', [0x609a],
     'character demo: both set a 0x30 frame timer, clear the two players\' first byte, then store 1, 0xff, 1 in that order, spawn kind 0x1b with the focus object\'s team, then request'),
    (0x604, 21, 'inferred', 'init_match_state_and_players', [0x19f1c, 0x64ec],
     'match start: by elimination (the song is a jingle of 6.2 s at the end of the select and of the character demo; the PS1 track is in the 14 s class like the other jingle tracks)'),
    (0x700, 24, 'inferred', 'blink_item_loop_until_button_with_palette_dim', [0x5516, 0x9b76],
     'prompt after the countdown: by elimination and class (jingle of 2.1 s, the PS1 track in the 10 s class); the sound test lists 0x700 twice, so it may stand for two songs'),
]

# The audio matches of music.md section 11 (song -> (code, "high" or "probable")), for the cross-check column.
AUDIO = {28: (0x600, 'high'), 29: (0x601, 'high'), 30: (0x300, 'high'), 31: (0x304, 'high'), 32: (0x404, 'high'), 37: (0x405, 'high'), 39: (0x403, 'high'),
         40: (0x206, 'high'), 41: (0x301, 'high'), 42: (0x302, 'high'), 43: (0x401, 'high'), 45: (0x506, 'high'), 46: (0x507, 'high'), 48: (0x303, 'high'),
         54: (0x502, 'high'), 55: (0x402, 'high'), 38: (0x501, 'probable'), 47: (0x306, 'probable'), 50: (0x203, 'probable'), 57: (0x205, 'probable')}

FIGHTER_TABLE = 0x8016e6f8
STAFF_TABLE = 0x8005175c           # ending overlay: the staff-roll code by ending (read by call_menu_code_from_table_by_state_index_slot28)
SOUND_TEST_LIST = 0x800e9074       # attract overlay (DEMO.PAC slot 0xf): the sound test's 59 codes
ENDING_FIRST_SUBSTEP = 8
ENDING_COUNT = 21


def load_build(path):
    """({function name: (address, size, image name or None)}, {image name: (archive file name, slot, address)}) from ps1/src/build.toml."""
    import tomllib
    t = tomllib.loads(Path(path).read_text())
    funcs = {}
    for u in t['unit']:
        for f in u['functions']:
            funcs[f['name']] = (f['address'], f['size'], u.get('image'))
    images = {i['name']: (Path(i['archive']).name, i['slot'], i['address']) for i in t['image']}
    return funcs, images


class Disc:
    def __init__(self, f):
        self.files, self.sector = iso_files(f)
        self.cache = {}

    def file(self, name):
        if name not in self.cache:
            lba, size = self.files[name]
            self.cache[name] = self.sector(lba, (size + 2047) // 2048)[:size]
        return self.cache[name]


def pac_chunk(data, table, slot):
    import pac
    for c in pac.chunk_table(data):
        if c['table'] == table and c['slot'] == slot:
            return data[c['offset']:c['offset'] + c['size']]
    raise ValueError('no chunk (%d, 0x%x)' % (table, slot))


def build_ps1(disc, funcs, images):
    """The PS1 images used here: the executable and the overlays (slot 0x28 ending, and those of DECLARED) as Overlay objects keyed by image name."""
    exe = disc.file('/SLPS_004.15')
    base = struct.unpack_from('<I', exe, 0x18)[0]
    ps1 = {None: Overlay(exe[0x800:], base)}
    for name, (archive, slot, addr) in images.items():
        if archive in ('END00.PAC', 'CONT00.PAC', 'DEMO.PAC', 'SELECTA.PAC', 'BOSS00.PAC', 'CDEMO00.PAC'):
            ps1[name] = Overlay(pac_chunk(disc.file('/PAC/' + archive), 0, slot), addr)
    return ps1


def function_words(ps1, funcs, name):
    addr, size, image = funcs[name]
    ov = ps1[image]
    return addr, size, image, ov.words(addr, addr + size)


# ---------------------------------------------------------------------------------------------------------- the analysis

# What each of the 25 request sites asks for, read by hand from the code and checked here against what the scan derives:
# a song number, a set of them (a byte table or two branches), "fighter" (selector 0: the fighter's theme) or "ending" (selector 1: the ending index's theme).
SITES = {0x1bde: '62', 0x27fc: '19', 0x5516: '24', 0x5818: 'fighter', 0x609a: '22', 0x64ec: '21', 0x6c58: '23', 0x6fb2: '25', 0x913c: '52,58', 0x9b76: '24',
         0xd6d2: '26,27', 0x19b8e: '20', 0x19d12: '20', 0x19f1c: '21', 0x1a3d0: 'ending', 0x86fcc: '29', 0x87866: '55', 0x880c2: '54', 0x88bda: '35',
         0x8b064: '49', 0x8bc54: '56', 0x8be10: 'ending', 0x8c1dc: '46', 0x95c02: '50,51,57', 0x95c0a: 'ending'}


def describe_site(prog, tables, site):
    """'fighter', 'ending' or the sorted song numbers requested at the site (selector 0 and 1 are named, not expanded)."""
    kind, v = d0_values(prog, site)
    d0s = v if kind == 'imm' else sorted({d for d in byte_table(prog, v, 24) if 6 <= d <= 0x2e and d % 2 == 0})
    names = {d // 2 for d in d0s if d // 2 < 2}
    songs = sorted(song_of(tables, d // 2) for d in d0s if d // 2 >= 2)
    if names and songs:
        raise ValueError('site 0x%x mixes theme requests and songs' % site)
    if names:
        return 'fighter' if names == {0} else 'ending' if names == {1} else 'fighter,ending'
    return ','.join(str(x) for x in songs)


def analyse(prog, disc, build_path, say):
    """(rows, sites): rows are (code, song, confidence, family, PS1 site text, arcade site text, why) for every PS1 code that has a partner;
    sites is a list of (address, what it requests)."""
    funcs, images = load_build(build_path)
    ps1 = build_ps1(disc, funcs, images)
    exe = ps1[None]
    tables = request_tables(prog)
    sites = request_sites(prog)
    described = {s: describe_site(prog, tables, s) for s in sites}
    if described != SITES:
        bad = sorted(set(described.items()) ^ set(SITES.items()))
        raise ValueError('the request sites are not the ones this tool was written for: %s' % ', '.join('0x%x=%s' % x for x in bad[:4]))
    say('arcade: %d request sites as read by hand; tables: %d fighter themes, %d ending themes, %d for selectors 3 to 23' % (
        len(sites), len(tables[0][0]), len(tables[0][1]), len(tables[0][3])))
    site_songs_of = lambda s, k=None: site_songs(prog, tables, s, k)[2]
    rows = []

    # fighters: the same table, indexed alike
    codes = [struct.unpack_from('<h', exe.data, FIGHTER_TABLE - exe.base + 2 * k)[0] for k in range(20)]
    for c in sorted(set(codes)):
        ks = [k for k in range(20) if codes[k] == c]
        songs = {tables[0][0][k] for k in ks}
        if len(songs) != 1:
            raise ValueError('fighters %s share code 0x%03x but the arcade gives them songs %s' % (ks, c, sorted(songs)))
        rows.append((c, songs.pop(), 'table', 'fighter', 'table_8016e6f8[%s]' % ','.join(map(str, ks)),
                     'selector 0 table 0x154ee[%s], request 0x5818' % ','.join(map(str, ks)), 'the same fighter index in both tables'))
    say('fighters: 20 entries, %d distinct codes, each with one song' % len(set(codes)))

    # endings
    by_code, pending = funcs['set_menu_command_by_code'][0], funcs['set_menu_command_one_and_wait_pending'][0]
    end = ps1['slot28']
    lo = min(a for a, s_, i in funcs.values() if i == 'slot28')
    hi = max(a + s_ for a, s_, i in funcs.values() if i == 'slot28')
    calls = music_calls(end.words(lo, hi), end.base, lo, by_code, pending)
    substep_fn, substep_size, _ = funcs['call_handler_by_substep_index_slot28']
    substep = handler_table(end.words(substep_fn, substep_fn + substep_size))
    scene_of = []
    for k in range(ENDING_COUNT + 1):
        h = end.word(substep + 4 * (ENDING_FIRST_SUBSTEP + k))
        t = handler_table(end.words(h, h + 48)) if lo <= h < hi else None
        scene_of.append(t)
    if None in scene_of[:ENDING_COUNT] or scene_of[ENDING_COUNT] is not None:
        raise ValueError('the substep table does not hold exactly %d scene handlers from entry %d' % (ENDING_COUNT, ENDING_FIRST_SUBSTEP))
    scene_of = scene_of[:ENDING_COUNT]
    in_code = lambda w: lo <= w < hi and w % 4 == 0
    entries = {t: pointer_run(end.word, t, in_code) for t in set(scene_of)}
    owner = {}                                    # function start -> (pass table, position)
    for t, run in entries.items():
        for i, p in enumerate(run):
            owner.setdefault(p, (t, i))
    starts = sorted(owner)
    scene_calls, stray = {t: [] for t in entries}, []
    for addr, code in calls:
        f = max((x for x in starts if x <= addr), default=None)
        if f is None:
            stray.append((addr, code))
        else:
            t, i = owner[f]
            scene_calls[t].append((i, addr, code))
    for t in scene_calls:
        scene_calls[t].sort()
    say('ending overlay: %d constant requests, %d scene pass tables for %d endings; requests outside them: %s' % (
        len(calls), len(entries), ENDING_COUNT, ', '.join('0x%08x=0x%03x' % x for x in stray) or 'none'))
    blocks = ending_blocks(prog)
    for a in range(ENDING_COUNT):
        for b in range(a + 1, ENDING_COUNT):
            if (scene_of[a] == scene_of[b]) != (blocks[a] == blocks[b]):
                raise ValueError('endings %d and %d: the PS1 and the arcade disagree on whether they share a scene' % (a, b))
    say('endings: the PS1 repeats scenes exactly where the arcade repeats blocks (%d distinct of %d)' % (len(entries), ENDING_COUNT))
    ending_sites = [s for s in sites if ENDING_CODE[0] <= s < ENDING_CODE[1]]
    groups = {}
    for k in range(ENDING_COUNT):
        groups.setdefault(scene_of[k], []).append(k)
    for t, ks in groups.items():
        k = ks[0]
        if len({tables[0][1][x] for x in ks}) != 1:
            raise ValueError('endings %s share a scene but not an ending theme' % ks)
        bs, be = block_range(blocks, k)
        in_block = [s for s in ending_sites if bs <= s < be]
        info = {s: site_songs_of(s, k) for s in in_block}
        seq = [c for i, a, c in scene_calls[t]]
        label = ','.join(map(str, ks))
        for code, song, kind in pair_ending(seq, tables[0][1][k], [x for s in in_block for x in info[s]]):
            if kind == 'table':
                rows.append((code, song, kind, 'ending', 'ending %s: first request of the scene at 0x%08x' % (label, t),
                             'selector 1 table 0x1552c[%s], request 0x1a3d0' % label, 'the same ending index in both tables'))
            else:
                at = [s for s in in_block if song in info[s]]
                rows.append((code, song, kind, 'ending', 'ending %s: later request of the scene' % label,
                             'block 0x%x of ending %s, request %s' % (bs, label, ','.join('0x%x' % s for s in at)),
                             'the only other song requested in the ending\'s block'))

    # staff roll
    staff_codes = [struct.unpack_from('<h', end.data, STAFF_TABLE - end.base + 2 * k)[0] for k in range(ENDING_COUNT)]
    staff = staff_songs(prog, tables)
    votes = {}
    for k in range(ENDING_COUNT):
        votes.setdefault(staff_codes[k], {}).setdefault(staff[k], []).append(k)
    for code, d in sorted(votes.items()):
        song = max(d, key=lambda x: len(d[x]))
        total = sum(len(v) for v in d.values())
        odd = [k for x, ks in d.items() if x != song for k in ks]
        rows.append((code, song, 'table', 'staff', 'staff code table 0x%08x: %d of %d endings%s' % (STAFF_TABLE, len(d[song]), total, ', exception ending %s' % ','.join(map(str, odd)) if odd else ''),
                     'request 0x95c02/0x95c0a, byte table 0x95c10', 'the song the arcade picks for the same ending index'))
    # bonus stages
    bonus = bonus_songs(prog, tables)
    other = {x for i, x in enumerate(bonus) if i != 11}
    if len(other) != 1 or bonus[11] in other:
        raise ValueError('the bonus byte table does not differ only at stage 11')
    bfn, bsize, bimg, bwords = function_words(ps1, funcs, 'init_palette_and_blocks_for_stage_slot2a')
    for v in (0x407, 0x503, 0xb):
        if not loads_immediate(bwords, v):
            raise ValueError('init_palette_and_blocks_for_stage_slot2a does not load 0x%x' % v)
    rows.append((0x407, other.pop(), 'table', 'bonus', 'bonus module 0x%08x, every stage but 0xb' % bfn, 'byte table 0x9142, request 0x913c', 'one song for every stage but number 11'))
    rows.append((0x503, bonus[11], 'table', 'bonus', 'bonus module 0x%08x, stage 0xb' % bfn, 'byte table 0x9142 entry 11, request 0x913c', 'the PS1 tests stage 0xb, the arcade differs at 11'))

    # declared pairs, each verified
    for code, song, conf, fn, arc_sites, why in DECLARED:
        addr, size, image, words = function_words(ps1, funcs, fn)
        if not loads_immediate(words, code):
            raise ValueError('%s does not load 0x%03x' % (fn, code))
        for s in arc_sites:
            if song not in site_songs_of(s):
                raise ValueError('arcade site 0x%x gives %s, not %d' % (s, described[s], song))
        rows.append((code, song, conf, 'declared', '%s 0x%08x' % (fn, addr), 'request %s' % ','.join('0x%x' % s for s in arc_sites), why))
    for fn in ('start_phase_screen_variant_a_slot0f', 'setup_rank_screen_images_and_blocks_slot0f'):
        if not loads_immediate(function_words(ps1, funcs, fn)[3], 0x606):
            raise ValueError('%s does not load 0x606' % fn)
    demo = ps1['slot0f']
    listed = [struct.unpack_from('<H', demo.data, SOUND_TEST_LIST - demo.base + 2 * i)[0] for i in range(59)]
    return rows, sorted(described.items()), listed, tables


def report(rows, sites, listed, tables, evidence, out=print):
    merged = merge_rows([(r[0], r[1], r[2], r[3]) for r in rows])
    for code, song, conf, family, p, a, why in sorted(rows, key=lambda r: (r[0], r[1], r[3])):
        au = AUDIO.get(song)
        audio = '-' if not au else 'agrees (%s)' % au[1] if au[0] == code else 'DISAGREES'
        out('0x%03x -> song %2d  %-8s %-8s audio %-15s | PS1 %s | arcade %s' % (code, song, conf, family, audio, p, a))
        if evidence:
            out('        why: %s' % why)
    out('--')
    for s, what in sites:
        out('arcade request 0x%05x: %s' % (s, what))
    out('--')
    counts = {c: sum(1 for v in merged.values() if v[1] == c) for c in CONFIDENCE}
    out('%d PS1 codes paired: %s' % (len(merged), ', '.join('%d %s' % (n, c) for c, n in counts.items())))
    unpaired = sorted(set(listed) - set(merged))
    out('sound-test codes with no arcade song: %s' % (' '.join('0x%03x' % c for c in unpaired) or 'none'))
    songs = {x for t in (tables[0][0], tables[0][1], tables[0][3]) for x in t} | {tables[1]}
    out('arcade songs with no PS1 code: %s' % (' '.join(str(x) for x in sorted(songs - {v[0] for v in merged.values()})) or 'none'))
    out('the sound-test list has %d entries and lists 0x700 %d times' % (len(listed), listed.count(0x700)))
    return merged


def run(args):
    work = tempfile.TemporaryDirectory()
    quiet = args.c_table
    say = (lambda s: None) if quiet else print
    prog = check_program(Path(args.decrypted).read_bytes()) if args.decrypted else decrypted_program(args.arcade, work.name)
    with open(args.disc, 'rb') as f:
        rows, sites, listed, tables = analyse(prog, Disc(f), ROOT / 'ps1/src/build.toml', say)
    if args.c_table:
        merged = merge_rows([(r[0], r[1], r[2], r[3]) for r in rows])
        print('/* GENERATED by `python3 port/arcade/music_map.py --disc DISC --arcade SET --c-table%s`; do not edit. PS1 music code, arcade song. */' % (' --all' if args.all else ''))
        print(c_table(merged, args.all))
        return 0
    report(rows, sites, listed, tables, args.evidence)
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disc', required=True, help='PS1 disc image (raw 2352-byte sectors)')
    ap.add_argument('--arcade', help='the arcade set ZIP (sfa2u)')
    ap.add_argument('--decrypted', help='the decrypted program view instead of --arcade (port/intro/import_full.py --opcodes)')
    ap.add_argument('--c-table', action='store_true', help='print the rows of songs[] for port/src/arcade.c')
    ap.add_argument('--all', action='store_true', help='with --c-table: include the inferred rows')
    ap.add_argument('--evidence', action='store_true', help='with the report: a line of evidence under every row')
    args = ap.parse_args()
    if not args.arcade and not args.decrypted:
        ap.error('--arcade or --decrypted is required')
    try:
        return run(args)
    except (ValueError, OSError, KeyError, zipfile.BadZipFile) as e:
        print('music_map: %s' % e, file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
