#!/usr/bin/env python3
"""Analysis tool: the arcade tile and flip of every tile of every fighter file's sprite chunk (the table that sprite_tiles.c reads).

What it does. The PS1's fighter tiles are arcade graphics tiles (the colour index one higher, modulo 16: the arcade's transparent 15 is
the PS1's 0), each stored with a flip. fighter.c numbers the tiles its cell lists use; a chunk also holds tile 0 and tiles after the
numbered ones, and some files (PL02, PL08, PL0B, PL0E, PL14, PL10, PL11, PL13 and their X files) have none numbered. This tool reads
every PL*.PAC file of the disc (the sprite chunk: table 0, slot 1 or 2), decodes each tile (tilepack.py), and finds the arcade tile
and flip that make it: an index of the arcade graphics' 16 x 16 tiles in the PS1's colours, looked up under the 4 flips (a flip is its
own inverse, so the disc's tile flipped is the arcade's tile). A blank tile (all zero) is a BLANK run; a tile with no arcade tile is
an OWN run. A tile found under several (code, flip) takes the one that makes the longest run: at the start of a run each candidate is
tried with step 1, 0 and -1 and the longest wins (ties: the previous run's flip, then step 1, 0, -1, then the smaller code and flip), so
a tile continues the previous tile's run whenever it can. Runs are (code, count, step, flip), at most 65535 tiles.

The table has a row for every STAGEnn.PAC file's effect sprite chunk too (table 0 slot 3, the same 2,336 tiles in all 20 files): 1,758 of
its tiles are arcade 16 x 16 tiles, 555 more are quarters of the arcade's 32 x 32 pixel scroll 3 tiles (SCROLL3_FLIP: the full-screen
picture of the effects is that layer's art), one is blank, and 23 have no arcade source: by the owner's rule ("rebuild arcade-style") those
are blank (BLANK runs marked REDRAWN, differing from the disc by design).

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade, MAME's sfa2u), both read only. It writes
nothing but what --c-table names and a temporary folder (--compare-c); it prints numbers and names, never game bytes.

    python3 port/arcade/sprite_tiles.py --disc BIN --arcade ZIP [--only NAME] [--c-table FILE] [--compare-c]

Per file it prints: the tiles, the runs, the tiles with no arcade tile (OWN), and, where fighter.c numbers the file, how the numbered
tiles compare with its numbering (the same arcade tile and flip, another one that makes the same pixels, or a different tile).
--c-table writes port/arcade/sprite_tiles_table.c (one row per file and chunk: numbers only; files whose runs are identical share one
array). --compare-c compiles sprite_tiles.c, the checked-in sprite_tiles_table.c and tests/sprite_tiles_test.c into a temporary folder,
builds every row's tiles from the arcade graphics and checks that each equals the disc's decoded tile, and that the tiles pack with
tilepack.c within the row's size (it prints the largest packed size against its room; a row that does not pack is a finding, not a
failure), and that the checked-in table is the text --c-table writes now. Exit 0 only when every tile equals the disc's.
"""
import argparse
import collections
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import fighters as F        # noqa: E402  read only: the disc reader and the numbering of fighter.c
import pac                  # noqa: E402  read only: the archive reader
import tilepack as T        # noqa: E402  read only: the chunk decoder and the arcade tile conversion

BLANK = 0xfffffffe          # ARCADE_SPRITE_RUN_BLANK
OWN = 0xffffffff            # ARCADE_SPRITE_RUN_OWN
MAX_COUNT = 0xffff
SWAP = bytes((b >> 4 | b << 4) & 0xff for b in range(256))   # swaps the nibbles of a byte (the pixels of a pair)
BLANK_TILE = bytes(128)

# Gen's PL11, PL11X, PL13 and PL13X (the four files are one chunk twice) hold 62 tiles of the PS1's own art, in four poses the arcade draws
# its own way. By the owner's rule ("rebuild arcade-style") each such tile number is given the arcade tile the arcade draws in the same
# cell, so the chunk builds whole from the arcade graphics and the cell lists stay the PS1's, byte for byte. Evidence (measured on
# 2026-10-24 against the disc and the arcade set): the poses are the cell lists 186, 284, 285 and 287
# of PL11; the arcade's lists of the same poses are lists 260, 515, 516 and 520 of PL17 (the arcade's mirror module, whose lists are the
# converter's output for the arcade's records at 0x1eec78, 0x202440, 0x202458 and 0x202500). In each, every cell of the arcade's
# list lies at a cell position of the PS1's list, with the same palette (list 186: the arcade's 28 cells are 28 of the PS1's 32; the other
# four PS1 cells, tile numbers 3180 to 3182 and 3204, are blank). Each entry is (PS1 tile number, arcade tile code, flip as the PS1 stores
# it, bit 0 across, bit 1 down; code None: blank); the flip is PL17's tile flip combined with the flip flags of the arcade's cell and of the
# PS1's, so that the cell shows the arcade's pixels (the codes are those PL17's own rows of this table give for the arcade's cells).
REDRAWN = (
    (3180, None, 0), (3181, None, 0), (3182, None, 0),
    (3183, 153068, 0), (3184, 153069, 0), (3185, 153070, 0),
    (3186, 153071, 0), (3187, 153035, 0), (3188, 153037, 0),
    (3189, 153038, 0), (3190, 153084, 0), (3191, 153085, 0),
    (3192, 153086, 0), (3193, 153087, 0), (3194, 153053, 0),
    (3195, 153054, 0), (3196, 153100, 0), (3197, 153101, 0),
    (3198, 153102, 0), (3199, 153103, 0), (3200, 152880, 0),
    (3201, 152881, 0), (3202, 152882, 0), (3203, 152879, 0),
    (3204, None, 0), (3205, 152896, 0), (3206, 152897, 0),
    (3207, 152898, 0), (3208, 152895, 0), (3209, 152912, 0),
    (3210, 152913, 0), (3211, 152914, 0), (5374, 156089, 0),
    (5379, 156105, 0), (5380, 156106, 0), (5381, 156107, 0),
    (5386, 156122, 0), (5387, 156123, 0), (5390, 156080, 0),
    (5392, 156082, 0), (5393, 156083, 0), (5394, 156084, 0),
    (5396, 156096, 0), (5399, 156099, 0), (5400, 156100, 0),
    (5401, 156101, 0), (5406, 156113, 0), (5407, 156114, 0),
    (5408, 156115, 0), (5409, 156116, 0), (5410, 156117, 0),
    (5415, 156092, 0), (5416, 156093, 0), (5422, 156153, 0),
    (5423, 156154, 0), (5424, 156155, 0), (5425, 156142, 0),
    (5426, 156144, 0), (5427, 156145, 0), (5429, 156169, 0),
    (5431, 156171, 0), (5432, 156158, 0),
)
REDRAWN_FILES = {'PL11.PAC', 'PL11X.PAC', 'PL13.PAC', 'PL13X.PAC'}
REDRAWN_FLIP = 4   # bit 2 of a run's flip: the run's tiles are REDRAWN ones (sprite_tiles.h ARCADE_SPRITE_REDRAWN)
SCROLL3_FLIP = 8   # bit 3: the run's codes are 32 pixel scroll 3 tiles, a quarter each (bits 4 and 5: which); sprite_tiles.h ARCADE_SPRITE_SCROLL3
BASE32 = 1 << 40   # candidate keys of a quarter of a 32 pixel tile: BASE32 + (tile * 4 + quarter) * 4 + flip; the others are code * 4 + flip


# ---------------------------------------------------------------- pure functions (tested without game data)

def flip_bytes(tile, flip):
    """The 128 bytes of a PS1 tile flipped across (flip & 1: each row reversed, pixels of a pair swapped) and/or down (flip & 2)."""
    rows = [tile[i:i + 8] for i in range(0, 128, 8)]
    if flip & 1:
        rows = [r[::-1].translate(SWAP) for r in rows]
    if flip & 2:
        rows.reverse()
    return b''.join(rows)


def tile_index(gfx):
    """{PS1 tile bytes: [codes]} of the arcade graphics' tiles (128 bytes a tile), the blank tile left out."""
    index = {}
    for code in range(len(gfx) // 128):
        index.setdefault(T.arcade_tile(gfx, code), []).append(code)
    index.pop(BLANK_TILE, None)
    return index


def quarter_raw(gfx, t32, q):
    """The 128 bytes, in the arcade graphics' layout of a 16 x 16 tile, of quarter q (0 top left, 1 top right, 2 bottom left, 3 bottom
    right) of the 32 x 32 pixel arcade tile t32 (512 bytes: rows of 16 bytes, four groups of four planes)."""
    base = t32 * 512
    return b''.join(gfx[base + (y + 16 * (q >> 1)) * 16 + 8 * (q & 1):base + (y + 16 * (q >> 1)) * 16 + 8 * (q & 1) + 8] for y in range(16))


def tile_index32(gfx):
    """{PS1 tile bytes: [(32 pixel tile, quarter)]} of the quarters of the arcade graphics' 32 x 32 pixel tiles (512 bytes a tile), the
    blank one left out (a quarter is the unflipped tile; candidates looks it up under the 4 flips as for the 16 pixel tiles)."""
    index = {}
    for t in range(len(gfx) // 512):
        for q in range(4):
            index.setdefault(T.arcade_tile(quarter_raw(gfx, t, q), 0), []).append((t, q))
    index.pop(BLANK_TILE, None)
    return index


def candidates(tile, index, cache=None, index32=None):
    """Sorted keys code * 4 + flip of the arcade tiles that make `tile` stored with a flip; [] when none (BLANK is not asked for).
    With index32 (tile_index32) also the keys BASE32 + (tile * 4 + quarter) * 4 + flip of 32 pixel tiles' quarters; a `cache` then
    belongs to this pair of indexes."""
    if cache is not None and tile in cache:
        return cache[tile]
    keys = set()
    for flip in range(4):
        for code in index.get(flip_bytes(tile, flip), ()):
            keys.add(code * 4 + flip)
        for t, q in (index32 or {}).get(flip_bytes(tile, flip), ()):
            keys.add(BASE32 + (t * 4 + q) * 4 + flip)
    out = sorted(keys)
    if cache is not None:
        cache[tile] = out
    return out


def key_stride(key):
    """What one step of a run adds to a candidate key: the next tile of its kind."""
    return 16 if key >= BASE32 else 4


def run_of(key, step, length):
    """The run (code, count, step, flip) of a candidate key: a 16 pixel tile's code and flip, or a 32 pixel tile's code with the quarter in the flip."""
    if key >= BASE32:
        k = key - BASE32
        return (k >> 4, length, step if length > 1 else 0, (k & 3) | SCROLL3_FLIP | (k >> 2 & 3) << 4)
    return (key >> 2, length, step if length > 1 else 0, key & 3)


def make_runs(tiles, index, cache=None, index32=None):
    """[(code, count, step, flip)] of the decoded tiles (each 128 bytes): BLANK runs, OWN runs (step 0, flip 0) and arcade runs. With
    index32 (tile_index32) a tile may also be a quarter of a 32 pixel scroll 3 tile (SCROLL3_FLIP; a 16 pixel tile is preferred at a tie)."""
    cands = []
    for t in tiles:
        cands.append(None if t == BLANK_TILE else frozenset(candidates(t, index, cache, index32)))
    runs = []
    n, i, prev_flip = len(tiles), 0, 0
    while i < n:
        c = cands[i]
        if not c:      # BLANK (None) or no arcade tile (empty): a run of the same kind
            j = i
            while j < n and j - i < MAX_COUNT and (cands[j] is None) == (c is None) and not cands[j]:
                j += 1
            runs.append((BLANK if c is None else OWN, j - i, 0, 0))
            i = j
            continue
        best = None
        for key in sorted(c):
            for rank, step in enumerate((1, 0, -1)):
                length = 1
                while i + length < n and length < MAX_COUNT and cands[i + length] and key + key_stride(key) * step * length in cands[i + length]:
                    length += 1
                score = (length, key & 3 == prev_flip, -rank, -key)
                if best is None or score > best[0]:
                    best = (score, key, step, length)
        _score, key, step, length = best
        runs.append(run_of(key, step, length))
        prev_flip = key & 3
        i += length
    return runs


def expand_runs(runs, gfx):
    """The tiles (128 bytes each) the runs make from the graphics, as sprite_tiles.c builds them; ValueError as that refuses."""
    out = []
    for code, count, step, flip in runs:
        if code == OWN:
            raise ValueError('a run of tiles the arcade does not have')
        for k in range(count):
            if code == BLANK:
                out.append(BLANK_TILE)
                continue
            c = code + step * k
            if flip & SCROLL3_FLIP:
                if c < 0 or c * 512 + 512 > len(gfx):
                    raise ValueError('a code lies outside the graphics')
                out.append(flip_bytes(T.arcade_tile(quarter_raw(gfx, c, flip >> 4 & 3), 0), flip & 3))
                continue
            if c < 0 or c * 128 + 128 > len(gfx):
                raise ValueError('a code lies outside the graphics')
            out.append(flip_bytes(T.arcade_tile(gfx, c), flip))
    return out


def own_tiles(runs):
    return sum(count for code, count, _s, _f in runs if code == OWN)


def redraw(tiles, own, gfx):
    """(the chunk's tiles with each REDRAWN tile number made from the arcade's tile, the set of numbers changed); ValueError when the numbers
    are not exactly the tiles the arcade does not have (`own`: their numbers) or a code lies outside the graphics."""
    if {t for t, _c, _f in REDRAWN} != set(own):
        raise ValueError('the redrawn tile numbers are not the chunk\'s own tiles')
    out = list(tiles)
    for t, code, flip in REDRAWN:
        if code is None:
            out[t] = BLANK_TILE
        elif code * 128 + 128 > len(gfx):
            raise ValueError('a code lies outside the graphics')
        else:
            out[t] = flip_bytes(T.arcade_tile(gfx, code), flip)
    return out, {t for t, _c, _f in REDRAWN}


def blank_own(runs, data):
    """(the runs with each OWN run a BLANK run marked REDRAWN, the tiles with those tiles blank, how many): by the owner's rule "rebuild
    arcade-style" a tile the arcade has no source for is blank where the arcade does not show it; it differs from the disc's by design."""
    numbers = own_numbers(runs)
    out = [(BLANK, count, 0, REDRAWN_FLIP) if code == OWN else (code, count, step, flip) for code, count, step, flip in runs]
    return out, [BLANK_TILE if k in numbers else t for k, t in enumerate(data)], len(numbers)


def mark(runs, numbers):
    """The runs split so that the tiles numbered in `numbers` are runs of their own with REDRAWN_FLIP in their flip."""
    out, at = [], 0
    for code, count, step, flip in runs:
        k = 0
        while k < count:
            red = at + k in numbers
            j = k
            while j < count and (at + j in numbers) == red:
                j += 1
            n = j - k
            out.append((code if code in (BLANK, OWN) else code + step * k, n, step if n > 1 else 0, flip | (REDRAWN_FLIP if red else 0)))
            k = j
        at += count
    return out


def own_numbers(runs):
    """The numbers of the tiles the runs mark OWN."""
    out, at = set(), 0
    for code, count, _s, _f in runs:
        if code == OWN:
            out |= set(range(at, at + count))
        at += count
    return out


def tiles_of(runs):
    return sum(r[1] for r in runs)


# ---------------------------------------------------------------- the table

def code_text(code):
    return {BLANK: 'ARCADE_SPRITE_RUN_BLANK', OWN: 'ARCADE_SPRITE_RUN_OWN'}.get(code, f'{code}u')


def c_table(rows):
    """The text of sprite_tiles_table.c for rows [dict(file, slot, size, tiles, runs)] (one per file and chunk); identical runs share an array."""
    out = ['/* GENERATED by `python3 port/arcade/sprite_tiles.py --disc DISC --arcade SET --c-table FILE` (FILE = port/arcade/sprite_tiles_table.c);',
           ' * do not edit. For each fighter file\'s sprite chunk: the table 0 slot, the chunk\'s size on the disc, its tile count and the runs of',
           ' * arcade tiles that make its tiles (code, count, step, flip). Only codes, flips and counts: no byte of either game. */',
           '#include "sprite_tiles.h"', '']
    names, order = {}, []
    for row in rows:
        key = tuple(row['runs'])
        if key not in names:
            names[key] = 'runs_' + row['file'].removesuffix('.PAC').lower()
            order.append(key)
    for key in order:
        out.append(f'static const struct arcade_sprite_run {names[key]}[] = {{')
        for code, count, step, flip in key:
            out.append(f'    {{{code_text(code)}, {count}u, {step}, {flip}u}},')
        out.append('};')
        out.append('')
    out.append('const struct arcade_sprite_chunk arcade_sprite_chunks[] = {')
    for row in rows:
        key = tuple(row['runs'])
        out.append(f'    {{"{row["file"]}", {row["slot"]}u, {row["size"]}u, {row["tiles"]}u, {names[key]}, {len(key)}u}},')
    out += ['};', 'const unsigned arcade_sprite_chunk_count = sizeof arcade_sprite_chunks / sizeof arcade_sprite_chunks[0];', '']
    return '\n'.join(out)


# ---------------------------------------------------------------- the disc

STAGE_SLOT = 3   # table 0 slot of a stage file's effect sprite chunk (the same chunk, tilepack form, in all 20 files)


def read_chunks(disc_path, only=None):
    """[dict(file, slot, size, data (the decoded tiles), tiles (their count), stage (True for a STAGEnn.PAC file's effect sprites))] of every
    PL*.PAC file's sprite chunk and then of every STAGEnn.PAC file's, each sorted by name; (rows, messages)."""
    rows, notes = [], []
    with open(disc_path, 'rb') as f:
        files, sec = F.iso_files(f)
        names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
        names += sorted(p for p in files if p.startswith('/PAC/STAGE') and p.endswith('.PAC'))
        if only:
            want = only.upper().removesuffix('.PAC') + '.PAC'
            names = [p for p in names if p.endswith('/' + want)]
        for p in names:
            lba, size = files[p]
            data = sec(lba, (size + 2047) // 2048)[:size]
            name = p.split('/')[-1]
            try:
                stage = name.startswith('STAGE')
                chunks = [c for c in pac.chunk_table(data) if c['table'] == 0 and c['slot'] in ((STAGE_SLOT,) if stage else F.TILE_SLOTS)]
            except (pac.FormatError, struct.error) as exc:
                notes.append(f'{name}: {exc}')
                continue
            for c in chunks:
                chunk = data[c['offset']:c['offset'] + c['size']]
                tiles = [struct.pack('<64H', *w) for _form, w in T.decode_chunk(chunk)]
                rows.append(dict(file=name, slot=c['slot'], size=len(chunk), data=tiles, tiles=len(tiles), stage=stage))
            if len(chunks) != 1:
                notes.append(f'{name}: {len(chunks)} sprite chunks')
    return rows, notes


def compare_numbering(tiles, runs, numbers, flips, gfx):
    """(numbered tiles, same (code, flip) as the runs, same pixels but another (code, flip), a different tile) for fighter.c's numbers."""
    ours = {}
    at = 0
    for code, count, step, flip in runs:
        for k in range(count):
            ours[at + k] = None if code in (BLANK, OWN) else (code + step * k, flip & 3)
        at += count
    same = alike = wrong = 0
    for n, code in numbers.items():
        if n >= len(tiles):
            wrong += 1
            continue
        theirs = flips[n]
        if ours.get(n) == (code, theirs):
            same += 1
        elif flip_bytes(T.arcade_tile(gfx, code), theirs) == tiles[n]:
            alike += 1
        else:
            wrong += 1
    return len(numbers), same, alike, wrong


# ---------------------------------------------------------------- the C check

def build_driver(tmp, table):
    exe = str(Path(tmp) / 'sprite_tiles_test')
    subprocess.run(['gcc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-I', str(HERE), '-o', exe, str(HERE / 'sprite_tiles.c'), str(table),
                    str(HERE / 'tilepack.c'), str(HERE / 'tests/sprite_tiles_test.c')], check=True, capture_output=True, text=True, timeout=120)
    return exe


def compare_c(rows, gfx, text):
    table = HERE / 'sprite_tiles_table.c'
    same = table.exists() and table.read_text() == text
    print('checked-in sprite_tiles_table.c ' + ('equals' if same else 'DIFFERS FROM') + ' the table this tool writes')
    if not same:
        return 1
    tmp = tempfile.mkdtemp(prefix='sprite_tiles-')
    try:
        exe = build_driver(tmp, table)
        gpath, dpath = Path(tmp) / 'gfx', Path(tmp) / 'disc'
        gpath.write_bytes(gfx)
        status, bad_rows, worst, unpack, owned = 0, 0, None, [], []
        total_time = 0.0
        for row in rows:
            dpath.write_bytes(b''.join(row['data']))
            t0 = time.perf_counter()
            run = subprocess.run([exe, 'check', row['file'], str(gpath), str(dpath)], capture_output=True, text=True, timeout=300)
            total_time += time.perf_counter() - t0
            fields = run.stdout.split()
            if run.returncode or len(fields) < 5 or fields[0] != 'built':
                print(f'{row["file"]}: the C refused or failed ({run.returncode}): {run.stderr.strip() or run.stdout.strip()}')
                status = 1
                bad_rows += 1
                continue
            own, differ, packed = int(fields[1]), int(fields[2]), fields[4]
            if own:
                owned.append((row['file'], own))
            if differ:
                print(f'{row["file"]}: {differ} of {row["tiles"]} tiles differ from the disc (first {fields[3]})')
                status = 1
                bad_rows += 1
            if packed == 'unpacked':
                unpack.append(row['file'])
                print(f'{row["file"]}: finding: the tiles do not pack in the room ({row["size"]} B): {" ".join(fields[5:])}')
            else:
                size = int(packed)
                if worst is None or size - row['size'] > worst[0] - worst[1]:
                    worst = (size, row['size'], row['file'])
        redrawn = [(r['file'], r['by_design']) for r in rows if r['by_design']]
        if redrawn:
            print('tiles that differ from the disc by design (REDRAWN: the PS1\'s own poses drawn as the arcade draws them; the C is checked against '
                  'them as redrawn): ' + ', '.join(f'{n} {c}' for n, c in redrawn))
        print(f'{len(rows) - bad_rows} of {len(rows)} rows: every tile built from the arcade equals the disc\'s decoded tile (REDRAWN tiles: the redrawn one)'
              + (f' (OWN tiles, which the C refuses and which are checked as the disc\'s, in {len(owned)} rows: ' + ', '.join(f'{n} {c}' for n, c in owned) + ')' if owned else ''))
        if worst:
            print(f'largest packed size against its room: {worst[2]} {worst[0]} B of {worst[1]} B (margin {worst[1] - worst[0]})')
        print(f'{len(unpack)} rows do not pack within the room' + (': ' + ' '.join(unpack) if unpack else ''))
        print(f'C time {total_time:.1f} s')
        return status
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


# ---------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00.PAC or PL00)')
    ap.add_argument('--c-table', metavar='FILE', help='write port/arcade/sprite_tiles_table.c to FILE')
    ap.add_argument('--compare-c', action='store_true', help='build the checked-in table in C and check it against the disc\'s tiles')
    args = ap.parse_args(argv)
    started = time.monotonic()
    prog, gfx = F.arcade_views(args.arcade)
    rows, notes = read_chunks(args.disc, args.only)
    for note in notes:
        print(note)
    t0 = time.monotonic()
    index = tile_index(gfx)
    print(f'index of {len(gfx) // 128} arcade tiles ({len(index)} distinct in the PS1 colours) in {time.monotonic() - t0:.1f} s')
    cache, cache32 = {}, {}
    index32 = tile_index32(gfx) if any(r['stage'] for r in rows) else None
    tmp = tempfile.mkdtemp(prefix='sprite_tiles-')
    totals = collections.Counter()
    try:
        clib = F.CLib(tmp)
        for row in rows:
            row['by_design'] = 0
            if row['stage']:   # the effect sprites: arcade tiles or quarters of the arcade's 32 pixel tiles; the rest blank
                row['runs'], row['data'], row['by_design'] = blank_own(make_runs(row['data'], index, cache32, index32), row['data'])
            else:
                row['runs'] = make_runs(row['data'], index, cache)
            if row['file'] in REDRAWN_FILES:   # the PS1's own poses drawn as the arcade draws them (REDRAWN); the tiles then differ from the disc's by design
                row['data'], changed = redraw(row['data'], own_numbers(row['runs']), gfx)
                row['by_design'] = len(changed)
                row['runs'] = mark(make_runs(row['data'], index, cache), changed)
            assert tiles_of(row['runs']) == row['tiles']
            own = own_tiles(row['runs'])
            blank = sum(c for code, c, _s, _f in row['runs'] if code == BLANK)
            s = f'{row["file"]} slot {row["slot"]}: {row["tiles"]} tiles, {len(row["runs"])} runs, {blank} blank, OWN {own}'
            if row['by_design']:
                s += f', {row["by_design"]} {"blank, no arcade source" if row["stage"] else "redrawn from the arcade"} (differ from the disc by design)'
            built = None if row['stage'] else clib.build(prog, row['file'])
            if row['stage']:
                s += f'; {sum(c for code, c, _s, f in row["runs"] if f & SCROLL3_FLIP)} tiles are quarters of the arcade\'s 32 pixel tiles'
            if isinstance(built, str):
                s += f'; fighter.c refused: {built}'
            elif built is None:
                s += '; fighter.c does not cover the file'
            else:
                numbered, same, alike, wrong = compare_numbering(row['data'], row['runs'], built[2], built[3], gfx)
                s += f'; fighter.c numbers {numbered}: same arcade tile and flip {same}, same pixels another way {alike}, different {wrong}'
                totals['numbered'] += numbered
                totals['same'] += same
                totals['alike'] += alike
                totals['wrong'] += wrong
            print(s)
            totals['tiles'] += row['tiles']
            totals['runs'] += len(row['runs'])
            totals['own'] += own
            totals['blank'] += blank
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    print(f'{len(rows)} chunks: {totals["tiles"]} tiles, {totals["runs"]} runs, {totals["blank"]} blank tiles, {totals["own"]} OWN tiles; '
          f'fighter.c numbered {totals["numbered"]}: same {totals["same"]}, same pixels {totals["alike"]}, different {totals["wrong"]}')
    text = c_table(rows)
    status = 0
    if args.c_table:
        Path(args.c_table).write_text(text)
        print(f'wrote {args.c_table} ({len(text)} bytes, {len(set(tuple(r["runs"]) for r in rows))} distinct run lists)')
    if args.compare_c:
        try:
            status = compare_c(rows, gfx, text)
        except subprocess.CalledProcessError as e:
            print('FAILED: %s' % (e.stderr or e))
            status = 1
    print('done in %.1f s' % (time.monotonic() - started))
    return status


if __name__ == '__main__':
    sys.exit(main())
