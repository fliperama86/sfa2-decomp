#!/usr/bin/env python3
"""Label the game functions of the resident executable by the families of Sony library functions they call.

The code of the executable is swept for functions with funcscan.py, from
`--start` (default: the start of the image) to `--end`. A function that
starts below `--library` is a game function, any other a library function.

A library function has one family, the first of these that applies:

1. its name: the family that `[names]` of the family table gives the name
   under which a unit of the resident image declares it in the build
   configuration;
2. its BIOS call: if it starts with a stub (`li t2,0xa0`, `0xb0` or `0xc0`,
   `jr t2`, `li t1,N`), the family that `[bios]` gives the key made of the
   table and the number in hexadecimal, the number in at least two digits,
   as in `b0:12`;
3. its folder: if the source of the unit that declares it is
   `sdk/FOLDER/...`, the family that `[folders]` gives the folder;
4. `unidentified`.

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

This is a static estimate over the boundaries of a sweep. Nothing here was
observed in a running game.

usage:
  families.py EXECUTABLE --config BUILD_TOML --families TABLE_TOML --library ADDRESS --end ADDRESS
              [--start ADDRESS] [--out TSV] [--library-out TSV] [--show N]

`--out` gets one line per game function: address, size, name or `-`, direct
families, reached families (each a list joined by `,`, or `-`), its calls
through a register, its calls elsewhere, and `open` or `closed`.
`--library-out` gets one line per library function: address, size, name or
`-`, family, and the number of game functions that call it.
"""

from __future__ import annotations

import argparse
import collections
import struct
import sys
import tomllib
from pathlib import Path

import funcscan
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


def family_of(address: int, declared: dict[int, tuple[str, str]], table: dict, words: list[int], base: int) -> str:
    """The family of the library function at `address`: by name, by BIOS call, by folder, or UNIDENTIFIED."""
    name, source = declared.get(address, ("", ""))
    if name in table["names"]:
        return table["names"][name]
    call = bios_call(words, (address - base) // 4)
    if call in table["bios"]:
        return table["bios"][call]
    path = source.split("/")
    if len(path) >= 3 and path[0] == "sdk" and path[1] in table["folders"]:
        return table["folders"][path[1]]
    return UNIDENTIFIED


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
    family = {address: family_of(address, declared, table, words, image.start) for address in library}
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
            "".join(f"{a:08x}\t{sizes[a]}\t{named(a)}\t{family[a]}\t{callers[a]}\n" for a in library)
        )
    return 0


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
    return run(parser.parse_args())


if __name__ == "__main__":
    sys.exit(main())
