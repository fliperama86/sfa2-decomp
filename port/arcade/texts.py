#!/usr/bin/env python3
"""Analysis tool: the text strings of the screen modules (the endings above all). Derives the table that texts.c reads, from the disc and
the arcade set, and checks the C against the disc.

What it does. A PS1 screen module (the ending files ENDnn.PAC, SELECT.PAC, DEMO.PAC and others) holds text strings: characters (0x20..0x7e),
then "@@", then zero bytes, a string starting at a multiple of 4 after a zero byte (or at the chunk's start) and being round4(chars + 3)
bytes long (at least one zero). The arcade program holds the same text in one of two forms (texts.h): CODES, a big-endian 16-bit word a
character (0x0000 a space; else 0x00 or 0x80 in the high byte and the ASCII code with bit 7 set in the low byte), and PLAIN, the ASCII bytes.
For every table-0 chunk of every .PAC file of the disc except PL* and STAGE* files, this tool finds those strings and, for each, the first
place of its characters in the arcade program: CODES (an even address) first, else PLAIN. A string it does not find is not in the table (the
PS1 has menus and messages of its own). The 21 ending files carry one chunk, so one shared strings array in the table.

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade; MAME's sfa2u). It writes nothing but what
--c-table names and a temporary folder (--compare-c); it prints numbers, never game bytes (--missing prints the text of the unfound strings).

    python3 port/arcade/texts.py --disc BIN --arcade ZIP [--c-table FILE | --compare-c] [--missing]

--c-table FILE writes port/arcade/texts_table.c (offsets, addresses, counts; no game bytes). --compare-c compiles texts.c, the checked-in
texts_table.c and tests/texts_test.c into a temporary folder, builds every string of every row of the table with arcade_text_build from the
arcade program, compares each with the disc's bytes at its place, and runs arcade_text_module_valid on each row; it prints a line per file
kind and ends with exit status 0 only when every string built equals the disc's, every row is valid and the checked-in table equals what
--c-table would write now. Without either option the summary per file kind is printed (strings found / strings, bytes).
"""
import argparse
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import fighters as F        # noqa: E402  read only: the disc reader, the arcade program view
import pac                  # noqa: E402  read only: the chunk table

CODES, PLAIN = 0, 1
FORM_NAME = {CODES: 'ARCADE_TEXT_CODES', PLAIN: 'ARCADE_TEXT_PLAIN'}
SKIP_PREFIXES = ('PL', 'STAGE')
MIN_STRINGS = 3   # a chunk with fewer strings found is not a text chunk: 1 or 2 characters met by chance in tiles (END02's slot 0x29)
DISC_HELP = 'disc image, raw 2352-byte sectors'


class Fail(Exception):
    pass


def kind_of(file):
    """'END00.PAC' -> 'END', 'CONT0AX.PAC' -> 'CONT': the file number is two hex digits with a decimal one among them, then maybe an X."""
    return re.sub(r'(\d[0-9A-F]|[0-9A-F]\d)X?$', '', file.upper().removesuffix('.PAC'))


def round4(n):
    return (n + 3) & ~3


# ---------------------------------------------------------------- the PS1 strings

RUN = re.compile(rb'[\x20-\x7e]+')


def scan_strings(chunk):
    """[(offset, chars, size)] of the strings of a chunk. A string is a maximal run of 0x20..0x7e bytes that ends in "@@" and has at least
    one character before them, followed by a zero byte. It starts at the first multiple of 4 at or after the run's start (up to 3 leading
    bytes are left over from what the buffer held before, and are not part of it). Its size is chars + 2 + the zero bytes after the "@@",
    at most up to round4(chars + 3) (the next string's place): the PS1's padding is not always zeros, and a string's bytes end where
    its zeros do. Strings with no zero after the "@@" and empty ones are not recognised; text split by a byte outside 0x20..0x7e gives only its last piece."""
    out, n = [], len(chunk)
    for m in RUN.finditer(chunk):
        start, end = round4(m.start()), m.end()
        chars = end - start - 2
        if chars <= 0 or chunk[end - 2:end] != b'@@' or end >= n or chunk[end] != 0:
            continue
        limit = min(n, start + round4(chars + 3))
        stop = end + 1
        while stop < limit and chunk[stop] == 0:
            stop += 1
        out.append((start, chunk[start:start + chars], stop - start))
    return out


# ---------------------------------------------------------------- the arcade program

def codes_view(program):
    """One byte per even-address word of the program: the ASCII code of the character the word is in the CODES form, else 0."""
    hi, lo = program[0::2], program[1::2]
    return bytes(0x20 if h == 0 and l == 0 else (l & 0x7f if h in (0, 0x80) and 0xa1 <= l <= 0xfe else 0) for h, l in zip(hi, lo))


class Finder:
    """The first place of a string's characters in the program: (form, address) or None. CODES (an even address) is tried before PLAIN."""

    def __init__(self, program):
        self.program, self.view, self.cache = program, codes_view(program), {}

    def find(self, chars):
        if chars not in self.cache:
            k = self.view.find(chars)
            if k >= 0:
                self.cache[chars] = (CODES, 2 * k)
            else:
                k = self.program.find(chars)
                self.cache[chars] = (PLAIN, k) if k >= 0 else None
        return self.cache[chars]


# ---------------------------------------------------------------- the derivation

def derive(chunks, program):
    """chunks: [(file, slot, bytes)]. Returns (rows, stats): rows = [dict(file, slot, size, strings=[dict(offset, source, chars, size, form)])]
    for the chunks with at least MIN_STRINGS strings found, sorted by (file, slot); stats = [dict(file, slot, total, found, bytes, found_bytes, missing=[chars])]."""
    finder = Finder(program)
    rows, stats, seen = [], [], set()
    for file, slot, data in sorted(chunks, key=lambda c: (c[0], c[1])):
        if (file, slot) in seen:
            raise Fail(f'{file} has two table-0 chunks in slot {slot:#x}')
        seen.add((file, slot))
        strings, missing, tb, fb = [], [], 0, 0
        found = list(scan_strings(data))
        for offset, chars, size in found:
            tb += size
            hit = finder.find(chars)
            if hit is None:
                missing.append(chars)
                continue
            fb += size
            strings.append(dict(offset=offset, source=hit[1], chars=len(chars), size=size, form=hit[0]))
        if found:
            stats.append(dict(file=file, slot=slot, total=len(found), found=len(strings), bytes=tb, found_bytes=fb, missing=missing))
        if len(strings) >= MIN_STRINGS:
            rows.append(dict(file=file, slot=slot, size=len(data), strings=strings))
    return rows, stats


def summary_lines(stats):
    """One line per file kind: strings found / strings, bytes found / bytes."""
    kinds = {}
    for s in stats:
        k = kinds.setdefault(kind_of(s['file']), dict(files=set(), total=0, found=0, bytes=0, found_bytes=0))
        k['files'].add(s['file'])
        for f in ('total', 'found', 'bytes', 'found_bytes'):
            k[f] += s[f]
    return [f"{name}: {len(k['files'])} files, {k['found']} of {k['total']} strings found, {k['found_bytes']} of {k['bytes']} bytes"
            for name, k in sorted(kinds.items())]


# ---------------------------------------------------------------- the C table

def c_table(rows):
    """The text of port/arcade/texts_table.c. Rows with equal strings arrays share one array (the 21 ending files)."""
    lines = ['/* GENERATED by `python3 port/arcade/texts.py --disc DISC --arcade SET --c-table FILE` (FILE = port/arcade/texts_table.c); do not edit.',
             ' * For each module chunk of a screen file with text strings in the arcade program (the endings above all): the places and sizes of',
             ' * the strings in the chunk, and the address and form of each one\'s characters in the arcade program. Only offsets, addresses and',
             ' * counts: no byte of either game. */',
             '#include "texts.h"', '']
    names, order = {}, []
    for r in rows:
        key = (r['size'], tuple(tuple(s[f] for f in ('offset', 'source', 'chars', 'size', 'form')) for s in r['strings']))
        if key not in names:
            names[key] = f"strings_{r['file'].removesuffix('.PAC').lower()}_{r['slot']:x}"
            order.append((key, r))
    for key, r in order:
        lines.append(f'static const struct arcade_text_string {names[key]}[] = {{')
        lines += [f"    {{{s['offset']:#x}u, {s['source']:#x}u, {s['chars']}u, {s['size']}u, {FORM_NAME[s['form']]}}}," for s in r['strings']]
        lines += ['};', '']
    lines.append('const struct arcade_text_module arcade_text_modules[] = {')
    for r in rows:
        key = (r['size'], tuple(tuple(s[f] for f in ('offset', 'source', 'chars', 'size', 'form')) for s in r['strings']))
        lines.append(f"    {{\"{r['file']}\", {r['slot']}u, {r['size']}u, {names[key]}, {len(r['strings'])}u}},")
    lines += ['};', 'const unsigned arcade_text_module_count = sizeof arcade_text_modules / sizeof arcade_text_modules[0];']
    return '\n'.join(lines) + '\n'


def parse_dump(text):
    """The driver's `table` output -> rows (as derive makes them)."""
    rows, lines, k = [], text.split('\n'), 0
    while k < len(lines) and lines[k]:
        file, slot, size, count = lines[k].split()
        strings = []
        for line in lines[k + 1:k + 1 + int(count)]:
            o, a, c, s, f = map(int, line.split())
            strings.append(dict(offset=o, source=a, chars=c, size=s, form=f))
        rows.append(dict(file=file, slot=int(slot), size=int(size), strings=strings))
        k += 1 + int(count)
    return rows


# ---------------------------------------------------------------- the disc

def read_chunks(disc):
    """[(file, slot, bytes)]: the table-0 chunks of the disc's .PAC files other than PL* and STAGE* ones. Also the names of files not read."""
    out, unread = [], []
    with open(disc, 'rb') as f:
        index, sec = F.iso_files(f)
        for path, (lba, size) in sorted(index.items()):
            name = path.rsplit('/', 1)[-1].upper()
            if not name.endswith('.PAC') or name.startswith(SKIP_PREFIXES):
                continue
            data = sec(lba, (size + 2047) // 2048)[:size]
            try:
                chunks = pac.chunk_table(data)
            except pac.FormatError:
                unread.append(name)
                continue
            out += [(name, c['slot'], data[c['offset']:c['offset'] + c['size']]) for c in chunks if c['table'] == 0 and c['slot'] != 0xffff]
    return out, unread


# ---------------------------------------------------------------- the C

def build_driver(tmp):
    exe = str(Path(tmp) / 'texts_test')
    subprocess.run(['gcc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-o', exe, str(HERE / 'texts.c'), str(HERE / 'texts_table.c'),
                    str(HERE / 'tests/texts_test.c')], check=True, timeout=120)
    return exe


def compare_c(rows, chunks, program, text, tmp):
    """Build every string of the checked-in table in C and compare with the disc's chunks. Prints a line per file kind; returns the status."""
    status = 0
    table_file = HERE / 'texts_table.c'
    same = table_file.exists() and table_file.read_text() == text
    print('checked-in texts_table.c ' + ('equals' if same else 'DIFFERS FROM') + ' the table this tool derives')
    if not same:
        status = 1
    exe = build_driver(tmp)
    c_rows = parse_dump(subprocess.run([exe, 'table'], check=True, capture_output=True, text=True, timeout=60).stdout)
    if [(r['file'], r['slot'], r['size'], r['strings']) for r in c_rows] != [(r['file'], r['slot'], r['size'], r['strings']) for r in rows]:
        print("the C table's rows differ from the derived rows")
        status = 1
    by_key = {(f, s): d for f, s, d in chunks}
    prog = Path(tmp) / 'program'
    prog.write_bytes(program)
    out = Path(tmp) / 'out'
    kinds = {}
    for idx, r in enumerate(c_rows):
        k = kinds.setdefault(kind_of(r['file']), dict(rows=0, valid=0, strings=0, equal=0, bytes=0))
        k['rows'] += 1
        k['strings'] += len(r['strings'])
        chunk = by_key.get((r['file'], r['slot']))
        if chunk is None or len(chunk) != r['size']:
            print(f"{r['file']} slot {r['slot']:#x}: not on the disc with size {r['size']}")
            status = 1
            continue
        run = subprocess.run([exe, 'row', str(idx), str(prog), str(out)], capture_output=True, text=True, timeout=60)
        if run.returncode:
            print(f"{r['file']} slot {r['slot']:#x}: the module is not valid: {run.stdout.strip()} {run.stderr.strip()}")
            status = 1
            continue
        k['valid'] += 1
        built, marks = out.read_bytes(), run.stdout.strip()
        at = 0
        for s, mark in zip(r['strings'], marks):
            piece = built[at:at + s['size']]
            at += s['size']
            if mark == '.' and piece == chunk[s['offset']:s['offset'] + s['size']]:
                k['equal'] += 1
                k['bytes'] += s['size']
            else:
                print(f"{r['file']} slot {r['slot']:#x} offset {s['offset']:#x}: " + ('refused by the C' if mark != '.' else 'built bytes differ from the disc'))
        if len(marks) != len(r['strings']):
            print(f"{r['file']}: the driver reported {len(marks)} strings of {len(r['strings'])}")
            status = 1
    for name, k in sorted(kinds.items()):
        ok = k['valid'] == k['rows'] and k['equal'] == k['strings']
        print(f"{name}: {k['valid']} of {k['rows']} rows valid, {k['equal']} of {k['strings']} strings equal to the disc's, {k['bytes']} bytes "
              + ('EQUAL' if ok else 'PROBLEMS'))
        if not ok:
            status = 1
    return status


# ---------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help=DISC_HELP)
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--c-table', metavar='FILE', help='write port/arcade/texts_table.c to FILE')
    ap.add_argument('--compare-c', action='store_true', help='build the checked-in table in C and compare with the disc')
    ap.add_argument('--missing', action='store_true', help='print the text of the strings not found in the arcade program (game text)')
    args = ap.parse_args(argv)
    started = time.monotonic()
    try:
        chunks, unread = read_chunks(args.disc)
        program = F.arcade_views(args.arcade)[0]
        rows, stats = derive(chunks, program)
    except Fail as e:
        print(f'FAILED: {e}')
        return 1
    print(f'{len(chunks)} table-0 chunks of {len({c[0] for c in chunks})} files'
          + (f'; {len(unread)} files not readable as archives: {" ".join(unread)}' if unread else ''))
    for line in summary_lines(stats):
        print(line)
    print(f'{len(rows)} rows, {sum(len(r["strings"]) for r in rows)} strings in the table')
    if args.missing:
        shown = set()
        for s in stats:
            for chars in s['missing']:
                if (kind_of(s['file']), chars) not in shown:
                    shown.add((kind_of(s['file']), chars))
                    print(f"  missing {s['file']} slot {s['slot']:#x}: {chars.decode('ascii')!r}")
    text = c_table(rows)
    status = 0
    if args.c_table:
        Path(args.c_table).write_text(text)
        print(f'wrote {args.c_table}')
    if args.compare_c:
        tmp = tempfile.mkdtemp(prefix='texts-')
        try:
            status = compare_c(rows, chunks, program, text, tmp)
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    print(f'done in {time.monotonic() - started:.1f} s')
    return status


if __name__ == '__main__':
    sys.exit(main())
