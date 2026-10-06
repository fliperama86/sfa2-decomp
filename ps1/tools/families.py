#!/usr/bin/env python3
"""Label the game functions of the resident executable by the families of Sony library functions they call.

The code of the executable is swept for functions with funcscan.py, from
`--start` (default: the start of the image) to `--end`. A function that
starts below `--library` is a game function, any other a library function.

A library function has one family, the first of these that applies:

1. its name: the family that `[names]` of the family table gives the name
   under which a unit of the resident image declares it in the build
   configuration, or, with `--symbols`, another name that the symbol file
   assigns its start (the first in order of name that the table knows);
2. its BIOS call: if it starts with a stub (`li t2,0xa0`, `0xb0` or `0xc0`,
   `jr t2`, `li t1,N`), the family that `[bios]` gives the key made of the
   table and the number in hexadecimal, the number in at least two digits,
   as in `b0:12`;
3. its folder: if the source of the unit that declares it is
   `sdk/FOLDER/...`, the family that `[folders]` gives the folder;
4. its place: if the nearest function below it and the nearest above it
   that units declare from a source under `sdk/` come from the same
   reference file, the family that `[folders]` gives that file's folder.
   The parts `NAME_pN.c` of a file count as the file `NAME.c`. This rests
   on an assumption: that what the reference has in one file is one object
   file of this game, whose code is one piece of the image;
5. `unidentified`.

A game function gets two sets of families:

- direct: the families of the library functions it calls;
- reached: its direct families and those of every game function that it
  reaches through calls, itself included.

A call is a `jal` or a `j` whose target is the start of another function of
the sweep. A `jal` or `j` to an address inside the function itself is not a
call. One to any other address is counted as a call elsewhere: into a
module, or into the middle of a function. A `jalr` is counted as a call
through a register. Neither kind is followed, so the reached set of a
function is a lower bound whenever it, or a game function it reaches, has
one of them: such a function is `open`, any other `closed`. The library is
not entered: what a library function calls does not count for its caller.

With `--modules`, the functions of the overlay modules are labelled too.
Every distinct content of a code-bearing chunk of table 0 is swept as
`pac.py functions` sweeps it, and each is taken alone. A function of a
module calls:

- a function of its own content: a `jal` or `j` to a start that the sweep
  finds there;
- a function of the executable: a `jal` or `j` to a start of the
  executable's sweep, when the address lies outside the chunk.

Any other target outside the function is a call elsewhere: into another
module, whose content at that moment is not known, or to no start. The
direct families of a module function are those of the library functions
it calls. Its reached families add those of the functions of its content
that it reaches and what the game functions of the executable that those
call reach. It is open when it, a function of its content that it reaches,
or a game function of the executable that one of them calls is open. The
totals are taken over all contents: code that several contents share is
counted once in each.

This is a static estimate over the boundaries of a sweep. Nothing here was
observed in a running game.

usage:
  families.py EXECUTABLE --config BUILD_TOML --families TABLE_TOML --library ADDRESS --end ADDRESS
              [--start ADDRESS] [--symbols FILE] [--out TSV] [--library-out TSV] [--show N]
              [--modules PAC_DIRECTORY --pointers ADDRESS [--modules-out TSV]]

`--symbols` is the symbol file of the build. It gives library functions
their other names, and the sweep of the modules its entries, as for
`pac.py functions`.

`--out` gets one line per game function: address, size, name or `-`, direct
families, reached families (each a list joined by `,`, or `-`), its calls
through a register, its calls elsewhere, and `open` or `closed`.
`--library-out` gets one line per library function: address, size, name or
`-`, family, the number of game functions that call it, and the rule that
gave the family: `name`, `bios`, `folder`, `place` or `-`.
`--modules-out` gets one line per function of a module: the first archive
with that content, slot, address, size, and the last five columns of
`--out`.
"""

from __future__ import annotations

import argparse
import collections
import re
import struct
import sys
import tomllib
from pathlib import Path

import funcscan
import matchbuild
import pac

UNIDENTIFIED = "unidentified"
PARTS = ("names", "bios", "folders")
LOAD_NUMBER = (0x2409, 0x3409)  # `li t1,N` as `addiu t1,zero,N` or `ori t1,zero,N`
JALR = 0x09


def read_table(path: str) -> dict[str, dict[str, str]]:
    """The family table: `[names]`, `[bios]` and `[folders]`, each from a key to the name of a family.

    A part may be absent. Any other key, a part that is not a table, and a
    family that is not a non-empty string raise FormatError.
    """
    with open(path, "rb") as handle:
        table = tomllib.load(handle)
    unknown = sorted(set(table) - set(PARTS))
    if unknown:
        raise pac.FormatError(f"{path}: unknown key `{unknown[0]}`")
    for part in PARTS:
        entries = table.setdefault(part, {})
        if not isinstance(entries, dict) or not all(isinstance(family, str) and family for family in entries.values()):
            raise pac.FormatError(f"{path}: `{part}` is not a table of family names")
    return table


def bios_call(words: list[int], index: int) -> str | None:
    """The key of the BIOS call that a stub at words[index] makes, as in `b0:12`, or None if no stub starts there."""
    if index + 2 >= len(words) or words[index] not in funcscan.STUB_LOADS or words[index + 1] != funcscan.STUB_JUMP:
        return None
    if words[index + 2] >> 16 not in LOAD_NUMBER:
        return None
    return f"{words[index] & 0xFF:02x}:{words[index + 2] & 0xFFFF:02x}"


ASSIGNMENT = re.compile(r"(?<![A-Za-z_0-9])([A-Za-z_][A-Za-z_0-9]*)\s*=\s*([^;\s]+)\s*;")


def other_names(path: str | None) -> dict[int, list[str]]:
    """The names that the `name = value;` statements of a symbol file assign each address, in order of name.

    Comments are skipped and anything else in the file carries no name: a
    name starts with a letter or an underscore, so `9x = 1;` assigns
    nothing here. A value is read as the linker reads an integer; one that
    it would not read raises FormatError.
    """
    if not path:
        return {}
    text = re.sub(r"/\*.*?\*/", "", Path(path).read_text(), flags=re.DOTALL)
    found: dict[int, list[str]] = collections.defaultdict(list)
    for name, value in sorted(ASSIGNMENT.findall(text)):
        try:
            address = matchbuild.linker_integer(value)
        except ValueError:
            address = None
        if address is None:
            raise pac.FormatError(f"{path}: `{name}` is assigned `{value}`, which is no integer as the linker reads it")
        found[address].append(name)
    return found


PART = re.compile(r"_p\d+[a-z]?$")


def family_of(
    address: int, declared: dict[int, tuple[str, str]], table: dict, words: list[int], base: int, others: dict[int, list[str]]
) -> tuple[str, str]:
    """The family of the library function at `address` by its name, its BIOS call or its folder, and which of them gave it.

    (UNIDENTIFIED, "-") when none of the three applies.
    """
    name, source = declared.get(address, ("", ""))
    for known in [name, *others.get(address, [])]:
        if known in table["names"]:
            return table["names"][known], "name"
    call = bios_call(words, (address - base) // 4)
    if call in table["bios"]:
        return table["bios"][call], "bios"
    path = source.split("/")
    if len(path) >= 3 and path[0] == "sdk" and path[1] in table["folders"]:
        return table["folders"][path[1]], "folder"
    return UNIDENTIFIED, "-"


def reference_file(source: str) -> str | None:
    """The reference file of a source under `sdk/`: its path below `sdk/` without the ending and without the `_pN` of a part.

    None for a source that is not under `sdk/`. A file directly in `sdk/`
    is a reference file too; it has no folder.
    """
    path = source.split("/")
    if len(path) < 2 or path[0] != "sdk":
        return None
    return "/".join([*path[1:-1], PART.sub("", path[-1].rsplit(".", 1)[0])])


def by_place(library: list[int], declared: dict[int, tuple[str, str]]) -> dict[int, str]:
    """The reference file of each library function that lies between two declared functions of one reference file.

    `library` is in order of address. A function that a unit declares from
    a source under `sdk/` is an anchor and is not in the result; any other
    function is, when the nearest anchor below it and the nearest above it
    have the same reference file.
    """
    found: dict[int, str] = {}
    below: str | None = None
    between: list[int] = []
    for address in library:
        file = reference_file(declared.get(address, ("", ""))[1])
        if file is None:
            between.append(address)
            continue
        if file == below:
            found.update(dict.fromkeys(between, file))
        below, between = file, []
    return found


def calls_of(words: list[int], base: int, address: int, size: int, starts: set[int]) -> tuple[set[int], int, int]:
    """What the function at `address` calls: the starts of other functions, calls through a register, calls elsewhere."""
    callees: set[int] = set()
    registers = elsewhere = 0
    for index in range((address - base) // 4, (address - base + size) // 4):
        word = words[index]
        op = word >> 26
        if op in (0x02, 0x03):
            target = funcscan.jump_target(word, base + 4 * index)
            if address <= target < address + size:
                continue
            if target in starts:
                callees.add(target)
            else:
                elsewhere += 1
        elif op == 0 and word & 0x3F == JALR:
            registers += 1
    return callees, registers, elsewhere


def reach(game: list[int], callees: dict[int, set[int]], direct: dict[int, set[str]], loose: set[int]):
    """The reached families of every game function, and the set of those that are open.

    `loose` holds the game functions that themselves call through a
    register or elsewhere. Both results are the least sets closed under
    "what a game function I call has, I have".
    """
    reached = {address: set(direct[address]) for address in game}
    opened = set(loose)
    inside = set(game)
    changed = True
    while changed:
        changed = False
        for address in game:
            for callee in callees[address] & inside:
                if not reached[callee] <= reached[address]:
                    reached[address] |= reached[callee]
                    changed = True
                if callee in opened and address not in opened:
                    opened.add(address)
                    changed = True
    return reached, opened


def run(args) -> int:
    try:
        image = pac.Image(Path(args.executable).read_bytes())
        declared = pac.resident_functions(args.config)
        table = read_table(args.families)
        others = other_names(args.symbols)
    except (OSError, pac.FormatError, tomllib.TOMLDecodeError) as exc:
        print(exc)
        return 1
    words = list(struct.unpack_from(f"<{len(image.payload) // 4}I", image.payload))
    start = image.start if args.start is None else args.start
    if not image.start <= start < args.end <= image.start + 4 * len(words) or start % 4 or args.end % 4:
        print(f"the range to sweep, {start:#x} to {args.end:#x}, is not a range of words inside the image")
        return 1
    swept = funcscan.scan(
        words, image.start, (start - image.start) // 4, (args.end - image.start) // 4, funcscan.reader(words, image.start)
    )
    sizes = dict(swept)
    game = [address for address, _ in swept if address < args.library]
    library = [address for address, _ in swept if address >= args.library]
    family, rule = {}, {}
    placed = by_place(library, declared)
    for address in library:
        family[address], rule[address] = family_of(address, declared, table, words, image.start, others)
        folder, slash, _ = placed.get(address, "").partition("/")
        # Only where no rule above applied: a table may name the family `unidentified` itself.
        if rule[address] == "-" and slash and folder in table["folders"]:
            family[address], rule[address] = table["folders"][folder], "place"
    callees: dict[int, set[int]] = {}
    registers: dict[int, int] = {}
    elsewhere: dict[int, int] = {}
    for address in game:
        callees[address], registers[address], elsewhere[address] = calls_of(words, image.start, address, sizes[address], set(sizes))
    direct = {address: {family[callee] for callee in callees[address] if callee in family} for address in game}
    reached, opened = reach(game, callees, direct, {a for a in game if registers[a] or elsewhere[a]})
    callers = collections.Counter(callee for address in game for callee in callees[address] if callee in family)
    outside = sorted(address for address in declared if address not in sizes)

    print(f"swept {start:#x} to {args.end:#x}: {len(swept)} functions, {len(game)} game and {len(library)} library")
    print(f"functions of the build that are no start of the sweep: {len(outside)}")
    for address in outside[: args.show]:
        print(f"  {address:#x}")
    members = collections.Counter(family.values())
    calling = collections.Counter(name for address in game for name in direct[address])
    reaching = collections.Counter(name for address in game for name in reached[address])
    print(" family            library functions  called directly by  reached by")
    for name in sorted(members, key=lambda n: (n == UNIDENTIFIED, n)):
        print(f" {name:17} {members[name]:17} {calling[name]:19} {reaching[name]:11}")
    rules = collections.Counter(rule.values())
    print(
        f"library functions with a family by name: {rules['name']}, by BIOS call: {rules['bios']},"
        f" by folder: {rules['folder']}, by place: {rules['place']}"
    )
    none = [address for address in game if not reached[address]]
    print(f"game functions that call a library function directly: {sum(1 for a in game if direct[a])}")
    print(
        f"game functions that reach no library function: {len(none)};"
        f" closed: {sum(1 for a in none if a not in opened)}, open: {sum(1 for a in none if a in opened)}"
    )
    print(
        f"game functions with a call through a register: {sum(1 for a in game if registers[a])};"
        f" with a call elsewhere: {sum(1 for a in game if elsewhere[a])}"
    )

    def joined(names: set[str]) -> str:
        return ",".join(sorted(names)) or "-"

    def named(address: int) -> str:
        return declared.get(address, ("", ""))[0] or "-"

    if args.out:
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text(
            "".join(
                f"{a:08x}\t{sizes[a]}\t{named(a)}\t{joined(direct[a])}\t{joined(reached[a])}"
                f"\t{registers[a]}\t{elsewhere[a]}\t{'open' if a in opened else 'closed'}\n"
                for a in game
            )
        )
    if args.library_out:
        Path(args.library_out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.library_out).write_text(
            "".join(f"{a:08x}\t{sizes[a]}\t{named(a)}\t{family[a]}\t{callers[a]}\t{rule[a]}\n" for a in library)
        )
    if not args.modules:
        return 0

    try:
        slots = pac.destination_tables(image, args.pointers)[0]
        archives, bad = pac.read_archives(args.modules)
        symbols = sorted({a for a in funcscan.read_entries(args.symbols) if a % 4 == 0})
        chunks = list(pac.swept_chunks(archives, slots, symbols))
    except (OSError, pac.FormatError) as exc:
        print(exc)
        return 1
    if not chunks:
        print("no code-bearing chunk found")
        return 1
    totals: collections.Counter = collections.Counter()
    calling = collections.Counter()
    reaching = collections.Counter()
    rows = []
    for archive, slot, base, length, body, functions, _, _ in chunks:
        own = dict(functions)
        # An address inside the chunk is the module's: the executable's function there, if any, is not meant.
        starts = set(own) | {address for address in sizes if not base <= address < base + length}
        inner: dict[int, set[int]] = {}
        first: dict[int, set[str]] = {}
        through: dict[int, int] = {}
        away: dict[int, int] = {}
        loose: set[int] = set()
        seed: dict[int, set[str]] = {}
        for address, size in functions:
            inner[address], through[address], away[address] = calls_of(body, base, address, size, starts)
            outer = inner[address] - set(own)  # what it calls in the executable
            first[address] = {family[callee] for callee in outer if callee in family}
            resident = [callee for callee in outer if callee in reached]
            seed[address] = first[address].union(*(reached[callee] for callee in resident))
            if through[address] or away[address] or any(callee in opened for callee in resident):
                loose.add(address)
            totals["resident"] += bool(resident)
        got, unsure = reach(list(own), inner, seed, loose)
        for address, size in functions:
            totals["functions"] += 1
            totals["direct"] += bool(first[address])
            totals["none"] += not got[address]
            totals["none open"] += not got[address] and address in unsure
            totals["register"] += bool(through[address])
            totals["elsewhere"] += bool(away[address])
            calling.update(first[address])
            reaching.update(got[address])
            rows.append(
                f"{archive}\t{slot:#x}\t{address:08x}\t{size}\t{joined(first[address])}\t{joined(got[address])}"
                f"\t{through[address]}\t{away[address]}\t{'open' if address in unsure else 'closed'}\n"
            )
    print(
        f"modules: {len(archives)} archives parsed, {bad} rejected; code-bearing chunks with distinct contents: {len(chunks)};"
        f" functions: {totals['functions']}"
    )
    print(" family            called directly by  reached by")
    for name in sorted(members, key=lambda n: (n == UNIDENTIFIED, n)):
        print(f" {name:17} {calling[name]:18} {reaching[name]:11}")
    print(f"module functions that call a library function directly: {totals['direct']}")
    print(f"module functions that call a game function of the executable: {totals['resident']}")
    print(
        f"module functions that reach no library function: {totals['none']};"
        f" closed: {totals['none'] - totals['none open']}, open: {totals['none open']}"
    )
    print(f"module functions with a call through a register: {totals['register']}; with a call elsewhere: {totals['elsewhere']}")
    if args.modules_out:
        Path(args.modules_out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.modules_out).write_text("".join(rows))
    return 1 if bad else 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])

    def address(text: str) -> int:
        return int(text, 0)

    parser.add_argument("executable", help="the resident PS-X executable")
    parser.add_argument("--config", required=True, help="the build configuration: names and sources of the declared functions")
    parser.add_argument("--families", required=True, help="the family table")
    parser.add_argument("--library", type=address, required=True, help="a function that starts at or above this is library code")
    parser.add_argument("--end", type=address, required=True, help="address after the last word of code")
    parser.add_argument("--start", type=address, help="first address to sweep (default: the start of the image)")
    parser.add_argument("--out", help="one line per game function")
    parser.add_argument("--library-out", help="one line per library function")
    parser.add_argument("--show", type=int, default=10, help="addresses to print per finding")
    parser.add_argument("--modules", help="directory holding the archives: label the functions of the modules too")
    parser.add_argument("--pointers", type=address, help="address of the block of table addresses, needed with --modules")
    parser.add_argument("--symbols", help="the symbol file of the build: other names of library functions, and entries of the modules")
    parser.add_argument("--modules-out", help="one line per function of a module")
    args = parser.parse_args()
    if args.modules and args.pointers is None:
        parser.error("--modules needs --pointers")
    return run(args)


if __name__ == "__main__":
    sys.exit(main())
