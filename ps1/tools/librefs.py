#!/usr/bin/env python3
"""Find the names by which reference-derived library units refer to library functions.

A unit whose source lies under `sdk/` is built from a reconstruction of the
Sony library. Where its source calls a function or takes its address, the
unit's object has a relocation against that function's name, and the link
gives the name an address: that of a function some unit declares under
the name, or the value the symbol file assigns it. The build is exact, so
the original has the same address at the same place. This tool reads those
relocations from the unit objects of a finished build and reports, for
every library function of the executable, the names by which such units
refer to its start.

What a name found here shows: at an exact place, the reference's source
refers to the function under that name. It does not show that the name is
the original symbol, and a reference that names the wrong function at its
only place of use would go unnoticed here.

The code of the executable is swept for functions as families.py does, and
a function that starts at or above `--library` is a library function. Each
is one of:

- named by its own unit: a unit under `sdk/` declares it;
- named by reference: no unit under `sdk/` declares it, and one or more
  units under `sdk/` refer to its start under one name;
- named two ways: such units refer to its start under more than one name,
  or under a name other than the one a unit under `sdk/` declares it with.
  This is a contradiction and makes the exit status 1;
- without a name: neither.

A relocation in a section whose name starts with `.text` is a call when it
is of type `R_MIPS_26`, and otherwise the address formed in code; one in
any other section is the address in data. Relocations against a name that
has no address, or whose address is no start of a library function, are
not about library functions and are left out.

usage:
  librefs.py EXECUTABLE --config BUILD_TOML --build BUILD_DIR --symbols SYMBOLS_LD
             --library ADDRESS --end ADDRESS [--start ADDRESS] [--out TSV]

`--build` is the folder that holds `unit-<name>.o` for every unit, as
matchbuild.py leaves it. `--out` gets one line per library function:
address, size, the name its unit declares or `-`, its class, the name by
reference or `-`, and the number of units that call it, form its address
in code, and hold its address in data under that name.
"""

from __future__ import annotations

import argparse
import collections
import struct
import sys
import tomllib
from pathlib import Path

from elftools.common.exceptions import ELFError
from elftools.elf.elffile import ELFFile

import funcscan
import matchbuild
import pac

KINDS = ("call", "code", "data")
CLASSES = ("named by its own unit", "named by reference", "named two ways", "without a name")


def relocations(path: Path) -> set[tuple[str, str]]:
    """The (name, kind) pairs of the relocations of one object: see the module text for the kinds."""
    found = set()
    with open(path, "rb") as handle:
        elf = ELFFile(handle)
        for section in elf.iter_sections():
            if section.header.sh_type not in ("SHT_REL", "SHT_RELA"):
                continue
            in_code = elf.get_section(section.header.sh_info).name.startswith(".text")
            symbols = elf.get_section(section.header.sh_link)
            for relocation in section.iter_relocations():
                name = symbols.get_symbol(relocation["r_info_sym"]).name
                if not name:
                    continue
                kind = "data" if not in_code else "call" if relocation["r_info_type"] == 4 else "code"
                found.add((name, kind))
    return found


def run(args) -> int:
    try:
        image = pac.Image(Path(args.executable).read_bytes())
        declared = pac.resident_functions(args.config)
        errors: list[str] = []
        values: dict[str, int] = {}
        matchbuild.parse_symbols(Path(args.symbols).read_text(), errors, values)
        if errors:
            raise pac.FormatError(errors[0])
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
    library = {address: size for address, size in swept if address >= args.library}

    # A name stands for the function that a unit declares under it, else for what the symbol file assigns.
    address_of = dict(values)
    address_of.update({name: address for address, (name, _) in declared.items() if name})
    own = {address: name for address, (name, source) in declared.items() if source.startswith("sdk/")}
    with open(args.config, "rb") as handle:
        units = [u for u in tomllib.load(handle).get("unit", []) if u.get("image", pac.RESIDENT) == pac.RESIDENT]
    referring = [u for u in units if str(u.get("source", "")).startswith("sdk/")]
    # (address, name) -> kind -> the units that refer so
    uses: dict[tuple[int, str], dict[str, set[str]]] = collections.defaultdict(lambda: collections.defaultdict(set))
    for unit in referring:
        path = Path(args.build) / f"unit-{unit['name']}.o"
        try:
            found = relocations(path)
        except (OSError, ELFError) as exc:
            print(f"{path}: {exc}")
            return 1
        for name, kind in found:
            address = address_of.get(name)
            if address in library:
                uses[address, name][kind].add(unit["name"])

    by_address: dict[int, list[str]] = collections.defaultdict(list)
    for address, name in sorted(uses):
        by_address[address].append(name)
    rows, classes = [], collections.Counter()
    for address in sorted(library):
        names = by_address.get(address, [])
        mine = own.get(address)
        if len(set(names) | ({mine} if mine else set())) > 1:
            kind = CLASSES[2]
        elif mine:
            kind = CLASSES[0]
        elif names:
            kind = CLASSES[1]
        else:
            kind = CLASSES[3]
        classes[kind] += 1
        shown = declared.get(address, ("", ""))[0] or "-"
        for name in names if kind in CLASSES[1:3] else ["-"]:
            counts = [len(uses[address, name][k]) if name != "-" else 0 for k in KINDS]
            rows.append((address, library[address], shown, kind, name, counts))

    print(f"swept {start:#x} to {args.end:#x}: {len(swept)} functions, {len(library)} library")
    print(f"units under sdk/: {len(referring)}")
    for kind in CLASSES:
        print(f"{kind}: {classes[kind]}")
    for wanted in CLASSES[1:3]:
        for address, size, shown, kind, name, counts in rows:
            if kind == wanted:
                print(f"  {address:#x} {shown}: {name}, units that call it {counts[0]}, form its address {counts[1]}, hold it in data {counts[2]}")
    if args.out:
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text(
            "".join(
                f"{address:08x}\t{size}\t{shown}\t{kind}\t{name}\t" + "\t".join(map(str, counts)) + "\n"
                for address, size, shown, kind, name, counts in rows
            )
        )
    return 1 if classes[CLASSES[2]] else 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])

    def address(text: str) -> int:
        return int(text, 0)

    parser.add_argument("executable", help="the resident PS-X executable")
    parser.add_argument("--config", required=True, help="the build configuration")
    parser.add_argument("--build", required=True, help="the folder with the unit objects of a finished build")
    parser.add_argument("--symbols", required=True, help="the symbol file of the build")
    parser.add_argument("--library", type=address, required=True, help="a function that starts at or above this is library code")
    parser.add_argument("--end", type=address, required=True, help="address after the last word of code")
    parser.add_argument("--start", type=address, help="first address to sweep (default: the start of the image)")
    parser.add_argument("--out", help="one line per library function and name")
    return run(parser.parse_args())


if __name__ == "__main__":
    sys.exit(main())
