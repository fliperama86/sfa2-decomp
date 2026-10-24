#!/usr/bin/env python3
"""Analysis tool: the data of the PS1 program (SLPS_004.15) that nothing in the port reads, so that a pack need not keep it.

What it does. The program's data block (90,088 bytes from 0x8016d91c) is the game's tables, strings and the Sony library's initial data. The
port runs the game's own C, and every use of a table in that C is a name that the compiler leaves undefined in the unit's object
(`ps1_NAME`, an absolute symbol at the PS1 address). A byte of the block that lies in no symbol range that any object, or any constant of
the runtime, refers to, and that no pointer word of a read range points into, is read by nothing the build runs: the matching library
functions that have no C in this build stop with their name when called (they do not read their data), and the jump tables of the game's
switch statements are the compiler's own in the port. So:

  1. read ranges   every symbol range (symbols.ld; a range runs to the next symbol) whose name an object of the build (obj/ and rt/ of the build
                   folder: `nm -u`) refers to, or whose address is written as a constant in port/src/*.c;
  2. closure       the target of a pointer word (port/arcade/program_relocs_table.c) that lies in a read range is read too (until nothing changes);
  3. unread        every other symbol range of the block, merged into byte ranges.

The ranges go into code_pins.py (RESIDENT_UNREAD): the pin of the resident program zeroes them, as it zeroes the functions, so a pack keeps none of
their bytes and its file is checked by the data pin of the file with those bytes zeroed. The scan is evidence from the references, not a proof (a
function that indexes past the end of its table into the next symbol is not seen); a run that zeroes the ranges (a pack made with them) is the
second evidence, and `--check` is the guard: it fails when a range now in code_pins.py contains a symbol that the build refers to.

    python3 port/arcade/program_unread.py [--build port/build/linux] [--check] [--disc BIN]

Prints the counts and the ranges as (address, size) pairs; with --disc also how many of the bytes are not zero on the disc. Writes nothing. Prints
numbers and names, never game bytes.
"""
import argparse
import glob
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import program_map as PM    # noqa: E402

DATA_START, DATA_END = PM.DATA_START, PM.DATA_END


def undefined_names(nm_output):
    """The PS1 names (without the ps1_ prefix) that the lines of `nm -u` leave undefined."""
    return {m.group(1) for m in (re.match(r'\s+U ps1_(\S+)', ln) for ln in nm_output.splitlines()) if m}


def absolute_addresses(nm_output):
    """{name: address} of the `A ps1_NAME` lines of `nm` on the linked program."""
    out = {}
    for ln in nm_output.splitlines():
        m = re.match(r'([0-9a-f]+) A ps1_(\S+)', ln)
        if m:
            out[m.group(2)] = int(m.group(1), 16)
    return out


def constant_addresses(text, start=DATA_START, end=DATA_END):
    """The addresses of the data block that a C source writes as hexadecimal constants."""
    return {a for a in (int(m.group(1), 16) for m in re.finditer(r'0x(80[01][0-9a-f]{5})u?', text)) if start <= a < end}


def pointer_words(table_text):
    """[(location, target)] of port/arcade/program_relocs_table.c."""
    return [(int(m.group(1), 16), int(m.group(2), 16)) for m in re.finditer(r'\{0x([0-9a-f]{8})u?, 0x([0-9a-f]{8})u?\}', table_text)]


def unread_ranges(ranges, referenced, constants, words, start=DATA_START, end=DATA_END):
    """(unread symbol ranges, merged byte ranges as (address, size)). `ranges` is [(start, end, name)] of the block, `referenced` the addresses of the
    symbols that objects refer to, `constants` the addresses the runtime writes, `words` the pointer words."""
    read = bytearray(end - start)

    def mark(a, e):
        for x in range(max(a, start), min(e, end)):
            read[x - start] = 1

    for a, e, _ in ranges:
        if a in referenced or any(a <= c < e for c in constants):
            mark(a, e)
    changed = True
    while changed:
        changed = False
        for loc, tgt in words:
            if start <= loc < end and read[loc - start] and start <= tgt < end and not read[tgt - start]:
                for a, e, _ in ranges:
                    if a <= tgt < e:
                        mark(a, e)
                        changed = True
                        break
    unread = [(a, e, n) for a, e, n in ranges if not read[a - start]]
    merged = []
    for a, e, _ in sorted(unread):
        if merged and merged[-1][0] + merged[-1][1] == a:
            merged[-1] = (merged[-1][0], merged[-1][1] + e - a)
        else:
            merged.append((a, e - a))
    return unread, merged


def scan(build):
    objs = glob.glob(str(build / 'obj/*.o')) + glob.glob(str(build / 'rt/*.o'))
    if not objs:
        raise SystemExit(f'no objects in {build}: build the program first (./build.sh linux)')
    names = set()
    for i in range(0, len(objs), 400):
        names |= undefined_names(subprocess.run(['nm', '-u'] + objs[i:i + 400], capture_output=True, text=True, check=True).stdout)
    address = absolute_addresses(subprocess.run(['nm', str(build / 'sfa2')], capture_output=True, text=True, check=True).stdout)
    referenced = {address[n] for n in names if n in address and DATA_START <= address[n] < DATA_END}
    constants = set()
    for f in sorted(glob.glob(str(ROOT / 'port/src/*.c'))):
        constants |= constant_addresses(Path(f).read_text(errors='replace'))
    symbols = PM.parse_symbols((ROOT / 'ps1/src/symbols.ld').read_text())
    ranges = PM.symbol_ranges(symbols, DATA_START, DATA_END)
    words = pointer_words((HERE / 'program_relocs_table.c').read_text())
    return ranges, referenced, constants, words


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--build', default=str(ROOT / 'port/build/linux'))
    ap.add_argument('--check', action='store_true', help='fail when a range of code_pins.RESIDENT_UNREAD holds a symbol that the build refers to')
    ap.add_argument('--disc')
    a = ap.parse_args(argv)
    ranges, referenced, constants, words = scan(Path(a.build))
    unread, merged = unread_ranges(ranges, referenced, constants, words)
    print(f'{len(ranges)} symbol ranges in the data block; {len(referenced)} referenced by objects, {len(constants)} constants in the runtime, '
          f'{len(words)} pointer words; unread: {len(unread)} ranges, {sum(e - s for s, e, _ in unread)} bytes in {len(merged)} runs')
    if a.disc:
        load, body = PM.load_exe_from_disc(a.disc)
        q = body[DATA_START - load:DATA_END - load]
        print(f'  of them not zero on the disc: {sum(1 for s, n in merged for b in q[s - DATA_START:s - DATA_START + n] if b)} bytes')
    for s, n in merged:
        print(f'  0x{s:08x} {n}')
    if a.check:
        import code_pins
        bad = []
        for s, n in code_pins.RESIDENT_UNREAD:
            for r0, r1, name in ranges:
                if r0 < s + n and r1 > s and r0 in referenced:
                    bad.append((name, hex(r0)))
        for name, at in bad:
            print(f'FAIL: {name} at {at} is in a range of RESIDENT_UNREAD but the build refers to it')
        missing = [m for m in code_pins.RESIDENT_UNREAD if m not in merged]
        print(f'RESIDENT_UNREAD: {len(code_pins.RESIDENT_UNREAD)} runs; {len(missing)} of them are not among the unread runs of this scan')
        return 1 if bad else 0
    return 0


if __name__ == '__main__':
    sys.exit(main())
