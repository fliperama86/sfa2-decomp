#!/usr/bin/env python3
"""Analysis tool: the object grids and animation steps of the stage modules, rebuilt from the arcade (the table that stage_objects.c reads).

What it does. A stage file's module (table 0, slot 6) holds, among its code and tables, the grids its objects draw (draw_object_tile_grid_sprites_slot06_NN)
and the 12-byte animation steps that point at them. They are the PS1 tool's conversion of the arcade's art records (the records fighters.py converts to
cell lists) and of the arcade's sequence records:

  A grid is: byte 0 columns, byte 1 zero, byte 2 rows, byte 3 the palette row (the records' cells' attr & 31: one row a record), u16 ox, u16 oy (minus the
  smallest x and y of the record's cells, which are multiples of 16 apart), then columns * rows u16 cells row by row: 0 for no sprite (also a cell whose
  arcade tile is transparent), else the number of an object tile. The numbers are those of the stage's tile list (the object tile chunk, then the
  shared effect chunk): tile n is an arcade tile with a flip (tile_runs, as stages.py), and a cell is the number n of the (code + 0x10000, flip) its arcade
  cell names. The PS1's tool numbered the tiles of an object (a run of grids) once each, and numbered the next object's tiles again, so the number of a
  (code, flip) is the one that lies in the grid's group: each grid carries its lowest and highest number (lo, hi), and the tile of the cell is the one number of
  (code, flip) in that range. (Layout facts, as the page slots of stages.py: the tool finds them by merging the grids' ranges and checks that the range names
  each tile once.)

This tool finds the grids in each stage module (the header's shape, cells that name tiles), finds the arcade art record each one is (the cells' positions and
tile codes), builds the grid from the record by the rule above and keeps those that equal the disc's bytes: only those are in the table. Nothing is
invented: a grid that does not equal is left to the disc. It prints counts and addresses, never game bytes.

    python3 port/arcade/stage_objects.py --disc BIN --arcade ZIP [--only 0E] [--c-table FILE]

--c-table writes port/arcade/stage_objects_table.c: for each stage file the module offset of each grid, the address of its art record in the arcade
program, and its range of tile numbers. `stages.py --compare-c` builds the C with the checked-in table and checks that its module bytes equal the model's
(which builds the grids with build_grid) and the disc's at the known bytes.
"""
import argparse
import collections
import ctypes
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
sys.path.insert(0, str(ROOT / 'port/intro'))
import fighters as F       # noqa: E402  read only: the arcade art records
import stages as S         # noqa: E402  read only: the stage files, their tile runs, the arcade views

BANK = 0x10000             # added to a record's tile codes: the graphics' tile of an object


# ---------------------------------------------------------------- pure functions (tested without game data)

def tile_keys(object_runs, shared_runs):
    """[(arcade tile index, flip) or None] for the tile numbers 0, 1, ...: the object tile chunk's tiles, then the shared effect chunk's. A transparent tile or a
    tile with no arcade source is None. object_runs, shared_runs: (first, count, flip) as stages.py makes them."""
    out = []
    for runs in (object_runs, shared_runs):
        for first, count, flip in runs:
            for k in range(count):
                out.append(None if first in (S.RUN_ZERO, S.RUN_DISC, S.RUN_BLANK) else (first + k, flip & 3))
    return out


def grid_cells(cells, blank):
    """The cells (code + BANK, x, y, attr) of an art record that show something (their arcade tile is not transparent)."""
    return [(code + BANK, x, y, fl) for code, x, y, fl, _g, _gp in cells if code + BANK not in blank]


def build_grid(cells, blank, keys, lo, hi):
    """The bytes of the grid of an art record's cells (see the module text), or None when the record cannot be a grid: no visible cell, cells off the 16 pixel
    grid, two palette rows, two cells at one place, a (code, flip) that the numbers lo..hi do not hold once."""
    vis = grid_cells(cells, blank)
    if not vis:
        return None
    minx, miny = min(c[1] for c in vis), min(c[2] for c in vis)
    cols, rows = (max(c[1] for c in vis) - minx) // 16 + 1, (max(c[2] for c in vis) - miny) // 16 + 1
    if cols > 255 or rows > 255 or any((c[1] - minx) % 16 or (c[2] - miny) % 16 for c in vis) or len({c[3] & 31 for c in vis}) != 1:
        return None
    where = collections.defaultdict(list)
    for n in range(lo, min(hi + 1, len(keys))):
        if keys[n] is not None:
            where[keys[n]].append(n)
    grid = [0] * (cols * rows)
    for code, x, y, fl in vis:
        found = where.get((code, (fl >> 5 & 1) | (fl >> 6 & 1) << 1), [])
        at = (y - miny) // 16 * cols + (x - minx) // 16
        if len(found) != 1 or grid[at]:
            return None
        grid[at] = found[0]
    return struct.pack('<BBBBHH', cols, 0, rows, vis[0][3] & 31, -minx & 0xffff, -miny & 0xffff) + struct.pack('<%dH' % len(grid), *grid)


# ---------------------------------------------------------------- finding the grids on the disc

def find_grids(mod, ntiles):
    """[(offset, columns, rows, cells)] of the places in a module that look like a grid (and do not overlap an earlier one)."""
    out, end, i = [], 0, 0
    while i + 8 <= len(mod):
        cols, b1, rows, b3 = mod[i], mod[i + 1], mod[i + 2], mod[i + 3]
        ox, oy = struct.unpack_from('<HH', mod, i + 4)
        if i >= end and 1 <= cols <= 24 and 1 <= rows <= 24 and b1 == 0 and b3 < 16 and i + 8 + 2 * cols * rows <= len(mod):
            cells = struct.unpack_from('<%dH' % (cols * rows), mod, i + 8)
            if all(c <= ntiles + 2 for c in cells) and any(cells) and ox <= cols * 16 and oy <= rows * 16 + 64:
                out.append((i, cols, rows, cells))
                end = i + 8 + 2 * cols * rows
        i += 2
    return out


def grid_rows(mod, runs, shared_runs, index, blank, rec_key):
    """Rows (grid module offset, art record address, lo, hi) for one stage module: the grids that the rule builds equal to the disc's, with the counts.
    runs: the stage's object tile runs (stages.py's object_runs, or tile_runs of the chunk)."""
    keys = tile_keys(runs, shared_runs)
    found = find_grids(mod, len(keys))
    rows, counts = [], collections.Counter()
    for at, cols, nrows, cells in found:
        nums = sorted({c & 0x3fff for c in cells if c & 0x3fff})
        lo, hi = nums[0], nums[-1]
        pts = []
        okk = True
        ox, oy = struct.unpack_from('<HH', mod, at + 4)
        for k, c in enumerate(cells):
            n = c & 0x3fff
            if not n:
                continue
            if n >= len(keys):
                okk = False
                break
            if keys[n] is None or keys[n][0] in blank:
                continue
            pts.append((k % cols * 16 - ox, k // cols * 16 - oy, keys[n][0] - BANK))
        if not okk or not pts:
            counts['no arcade tiles'] += 1
            continue
        pts.sort(key=lambda c: (c[1], c[0]))
        c0 = pts[0][2]
        hits = rec_key.get(tuple((x, y, c - c0) for x, y, c in pts))
        if not hits:
            counts['no art record'] += 1
            continue
        disc = bytes(mod[at:at + 8 + 2 * cols * nrows])
        made = None
        for addr, _c in sorted(hits):
            made = build_grid(index.record(addr)[1], blank, keys, lo, hi)
            if made == disc:
                rows.append((at, addr, lo, hi))
                counts['equal'] += 1
                counts['equal bytes'] += len(disc)
                break
        else:
            counts['built differently' if made else 'not built'] += 1
    return rows, counts


def analyse(name, parts, helper, shared_runs, index, blank, rec_key):
    """Rows and counts for one stage file (a dict of its chunks from stages.split_stage)."""
    runs = S.tile_runs(parts['object tiles'], helper.tiles(parts['object tiles'])[2]) if 'object tiles' in parts else []
    return grid_rows(parts['module'], runs, shared_runs, index, blank, rec_key)


def context(prog, gfx):
    """(view, index, blank, rec_key) of the arcade: the art records by their cells (see grid_rows), the transparent graphics tiles."""
    view = F.View(prog)
    index = F.Index(view)
    blank = {t for t in range(len(gfx) // 128) if gfx[t * 128:(t + 1) * 128] == b'\xff' * 128}
    rec_key = {}
    for n, addrs in index.by_cells.items():
        for a in addrs:
            r = index.record(a)
            if not r:
                continue
            cells = sorted(((x, y, code) for code, x, y, fl, g, gp in r[1] if code + BANK not in blank), key=lambda c: (c[1], c[0]))
            if cells:
                c0 = cells[0][2]
                rec_key.setdefault(tuple((x, y, c - c0) for x, y, c in cells), []).append((a, c0))
    return view, index, blank, rec_key


def load_arcade(arcade_zip):
    prog, gfx = S.arcade_views(arcade_zip)
    return (prog, gfx) + context(prog, gfx)


# ---------------------------------------------------------------- the table

def c_table(rows_by_file):
    out = ['/* GENERATED by `python3 port/arcade/stage_objects.py --disc DISC --arcade SET --c-table FILE`; do not edit.',
           ' * For each stage file: the grids of its module that the arcade builds equal to the disc\'s: the module offset of each, the address of its art record in the',
           ' * arcade program and the range of tile numbers it uses. Only offsets, addresses and numbers: no byte of either game. */',
           '#include "stage_objects.h"', '']
    for file, rows in rows_by_file:
        if rows:
            out.append(f'static const struct arcade_stage_grid grids_{file[5:7].lower()}[] = {{')
            out += [f'    {{{at}u, {addr:#x}u, {lo}u, {hi}u}},' for at, addr, lo, hi in rows]
            out += ['};', '']
    out.append('const struct arcade_stage_objects arcade_stage_objects[] = {')
    for file, rows in rows_by_file:
        out.append(f'    {{"{file}", {"grids_" + file[5:7].lower() if rows else "0"}, {len(rows)}u}},')
    out += ['};', 'const unsigned arcade_stage_objects_count = sizeof arcade_stage_objects / sizeof arcade_stage_objects[0];', '']
    return '\n'.join(out)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True)
    ap.add_argument('--arcade', required=True)
    ap.add_argument('--only')
    ap.add_argument('--c-table', metavar='FILE')
    args = ap.parse_args(argv)
    started = time.monotonic()
    prog, gfx, view, index, blank, rec_key = load_arcade(args.arcade)
    tmp = tempfile.mkdtemp(prefix='stage-objects-')
    try:
        helper = S.Helper(tmp, gfx, prog)
        with open(args.disc, 'rb') as f:
            files, sec = S.iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/STAGE') and p.endswith('.PAC'))
            if args.only:
                names = [p for p in names if p.endswith('STAGE' + args.only.upper() + '.PAC')]
            datas = {}
            for p in names:
                lba, size = files[p]
                datas[p.split('/')[-1]] = sec(lba, (size + 2047) // 2048)[:size]
        first = S.split_stage(next(iter(datas.values())))
        shared_runs = S.tile_runs(first['effect tiles'], helper.tiles(first['effect tiles'])[2])
        total, table = collections.Counter(), []
        for name, data in datas.items():
            parts = S.split_stage(data)
            rows, counts = analyse(name, parts, helper, shared_runs, index, blank, rec_key)
            table.append((name, rows))
            total.update(counts)
            print(f'{name}: {dict(counts)}')
        print(f'{dict(total)}')
        if args.c_table:
            Path(args.c_table).write_text(c_table(table))
            print(f'wrote {args.c_table}')
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    print(f'done in {time.monotonic() - started:.1f} s')
    return 0


if __name__ == '__main__':
    sys.exit(main())
