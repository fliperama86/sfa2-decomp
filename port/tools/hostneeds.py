#!/usr/bin/env python3
"""List what the game's compiled C units need and none of them gives.

`hostcheck.py` compiles every game unit into an object. This reads the
symbol tables of those objects with `nm` and sorts every name that some
object needs and no object defines. It is what a linker would ask for,
without linking: nothing is linked, nothing is run and no game file is
read. Python 3.11 or later, standard library only.

Inputs
------

- BUILD, the folder that `hostcheck.py` wrote: `units.tsv` and `obj/`. The
  objects read are those of the units that `units.tsv` calls `passed`,
  `obj/UNIT.o` each. A passed unit without its object is an error: the
  folder is not what one run of `hostcheck.py` left.
- The build configuration: the units, their sources, their images and the
  functions that each declares with a name and an address; the names of
  the images.
- The symbol file: lines `NAME = 0xADDRESS;`.
- The inventory folder: `game.tsv` and `library.tsv`, whose first column is
  the address of a function of the resident executable.

`nm -g` is run on the objects. A line `ADDRESS TYPE NAME` with a type
other than `U` defines NAME; a line `U NAME` needs it. Lines of any other
form are not symbols. A name is needed when some object needs it and no
object defines it.

Classes
-------

The image of a name is the image of the configuration whose name, after a
`_`, ends it; a name that ends with no image's name has none and belongs
to the resident executable. Of several images that fit, the longest.

The address of a name is, the first that it has: the address with which a
unit declares it as a function; the value that the symbol file gives it;
the eight hexadecimal digits in a name of the form `WORD_XXXXXXXX` or
`WORD_XXXXXXXX_REST`, with WORD of letters.

A name that two units declare, or that the symbol file gives twice, has
the address of the first. A needed name has the first class that applies:

    library by name        a unit under `sdk/` declares it
    assembly               a unit whose source is not C declares it
    unit not compiled      a C unit that is not under `sdk/` and did not
                           pass declares it
    unknown                it has no address
    C under another name   a passed unit of the same image declares a
                           function at its address under another name
    library by address     it has no image and its address is a row of
                           `library.tsv`
    game function          it has no image and its address is a row of
                           `game.tsv`
    module function        it has an image and begins with `func_`
    function elsewhere     it has no image and begins with `func_`
    scratchpad data        its address is from 0x1f800000 to 0x1f8003ff
    module data            it has an image
    resident data          anything else

`game function` and `module function` are functions that the C calls and
that no unit has yet. `function elsewhere` is a call to an address where
the sweep of the resident executable has no function: another block that
is loaded at that moment. The three data classes are names that only the
symbol file places: no C defines them.

A name of the class `library by address` is one of two kinds. It is
named when a unit under `sdk/` declares a function at that address, or
when the name itself does not begin with `func_`: then the symbol file
has given the function a name of its own. It is unnamed otherwise. The
named ones are the list that a port needs to call the replacement library
by name.

Output
------

Standard output, in this order, with nothing else:

    objects: N of N units
    defined: N names
    needed: N names
    CLASS: N names, needed by N units
    ...
    library by address: N named, N unnamed
    unknown: NAME NAME ...

One CLASS line for each of the twelve classes, in the order above, also
when its count is 0, with the number of names and the number of units
that need at least one of them. `objects` gives the units that passed and
all the units of `units.tsv`. The last line lists the names of the class
`unknown` in order of name and is left out when there is none.

`--out FILE` gets one line per needed name, in order of name: name, class,
address in eight hexadecimal digits or `-`, image or `-`, the number of
units that need it, and for `library by address` and `C under another
name` the other name, else `-`. Of several other names, the first in
order of name.

Exit status: 0 when the list was made; 2 when an input is missing or
malformed or `nm` cannot be run or fails, with one line on standard error
that names it.

usage:
  hostneeds.py [--build DIR] [--config BUILD_TOML] [--symbols FILE]
               [--inventory DIR] [--out FILE]

The defaults are those of `hostcheck.py` and the files of this
repository: `port/build/hostcheck`, `ps1/src/build.toml`,
`ps1/src/symbols.ld`, `ps1/inventory`, found from the place of this
script.
"""

from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
import tomllib
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]

CLASSES = (
    "library by name",
    "assembly",
    "unit not compiled",
    "unknown",
    "C under another name",
    "library by address",
    "game function",
    "module function",
    "function elsewhere",
    "scratchpad data",
    "module data",
    "resident data",
)
BATCH = 300
NM_TIMEOUT = 120
SCRATCHPAD = (0x1F800000, 0x1F8003FF)
SYMBOL = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*(0[xX][0-9a-fA-F]+)\s*;", re.M)
FORM = re.compile(r"[A-Za-z]+_([0-9a-f]{8})(?:_.*)?", re.S)
DEFINES = re.compile(r"([0-9a-fA-F]+) ([A-Za-z]) (\S+)")
NEEDS = re.compile(r"\s*U (\S+)")


class Problem(Exception):
    pass


@dataclass
class Unit:
    name: str
    kind: str  # "sdk", "assembly" or "c"
    image: str | None
    functions: list[tuple[str, int]]


@dataclass
class World:
    images: list[str]
    declared: dict[str, list[tuple[str, bool]]] = field(default_factory=dict)  # name -> (kind, passed)
    declared_at: dict[str, int] = field(default_factory=dict)
    symbols: dict[str, int] = field(default_factory=dict)
    passed_at: dict[tuple[str | None, int], set[str]] = field(default_factory=dict)
    sdk_at: dict[int, set[str]] = field(default_factory=dict)
    game: set[int] = field(default_factory=set)
    library: set[int] = field(default_factory=set)


# Inputs.


def read_text(path: Path, what: str) -> str:
    try:
        return path.read_text()
    except OSError as err:
        raise Problem(f"cannot read {what} {path}: {err.strerror}")


def read_config(path: Path) -> tuple[list[Unit], list[str]]:
    try:
        with open(path, "rb") as handle:
            config = tomllib.load(handle)
    except OSError as err:
        raise Problem(f"cannot read configuration {path}: {err.strerror}")
    except tomllib.TOMLDecodeError as err:
        raise Problem(f"configuration {path} is not valid TOML: {err}")
    entries = config.get("unit", [])
    if not isinstance(entries, list):
        raise Problem(f"{path}: `unit` is not an array of tables")
    units: list[Unit] = []
    for index, entry in enumerate(entries, 1):
        name = entry.get("name") if isinstance(entry, dict) else None
        source = entry.get("source") if isinstance(entry, dict) else None
        if not isinstance(name, str) or not name:
            raise Problem(f"{path}: unit {index} has no name")
        if not isinstance(source, str) or not source:
            raise Problem(f"{path}: unit {name} has no source")
        kind = "sdk" if source.startswith("sdk/") else "c" if source.endswith(".c") else "assembly"
        image = entry.get("image")
        functions: list[tuple[str, int]] = []
        for item in entry.get("functions", []):
            fname = item.get("name") if isinstance(item, dict) else None
            address = item.get("address") if isinstance(item, dict) else None
            if not isinstance(fname, str) or not isinstance(address, int) or isinstance(address, bool):
                raise Problem(f"{path}: unit {name} has a function without a name and an address")
            functions.append((fname, address))
        units.append(Unit(name, kind, image if isinstance(image, str) else None, functions))
    images = []
    for index, entry in enumerate(config.get("image", []), 1):
        if not isinstance(entry, dict) or not isinstance(entry.get("name"), str):
            raise Problem(f"{path}: image {index} has no name")
        images.append(entry["name"])
    return units, images


def read_status(build: Path) -> dict[str, bool]:
    if not build.is_dir():
        raise Problem(f"build folder {build} does not exist")
    path = build / "units.tsv"
    status: dict[str, bool] = {}
    for number, line in enumerate(read_text(path, "table").splitlines(), 1):
        cells = line.split("\t")
        if len(cells) != 6 or cells[2] not in ("passed", "failed"):
            raise Problem(f"{path} line {number} is not unit, source, passed or failed and three counts")
        status[cells[0]] = cells[2] == "passed"
    return status


def read_symbols(path: Path) -> dict[str, int]:
    text = re.sub(r"/\*.*?\*/", " ", read_text(path, "symbol file"), flags=re.S)
    found: dict[str, int] = {}
    for m in SYMBOL.finditer(text):
        found.setdefault(m.group(1), int(m.group(2), 16))
    return found


def read_rows(path: Path) -> set[int]:
    rows: set[int] = set()
    for number, line in enumerate(read_text(path, "inventory table").splitlines(), 1):
        if not line.strip():
            continue
        cell = line.split("\t")[0].strip()
        if not re.fullmatch(r"(?:0[xX])?[0-9a-fA-F]+", cell):
            raise Problem(f"{path} line {number}: first column is not a hexadecimal address")
        rows.add(int(cell, 16))
    return rows


def build_world(units: list[Unit], images: list[str], status: dict[str, bool], args: argparse.Namespace) -> World:
    world = World(sorted(images, key=len, reverse=True))
    for unit in units:
        passed = unit.kind == "c" and status.get(unit.name, False)
        for name, address in unit.functions:
            world.declared.setdefault(name, []).append((unit.kind, passed))
            world.declared_at.setdefault(name, address)
            if unit.kind == "sdk":
                world.sdk_at.setdefault(address, set()).add(name)
            elif passed:
                world.passed_at.setdefault((unit.image, address), set()).add(name)
    world.symbols = read_symbols(args.symbols)
    world.game = read_rows(args.inventory / "game.tsv")
    world.library = read_rows(args.inventory / "library.tsv")
    return world


# Objects.


def read_objects(build: Path, names: list[str]) -> tuple[dict[str, set[str]], dict[str, set[str]]]:
    """For each object the names it defines and the names it needs."""
    paths: dict[str, str] = {}
    for name in names:
        path = build / "obj" / f"{name}.o"
        if not path.is_file():
            raise Problem(f"object {path} of the passed unit {name} does not exist")
        paths[os.path.abspath(path)] = name
    defined: dict[str, set[str]] = {name: set() for name in names}
    needed: dict[str, set[str]] = {name: set() for name in names}
    listed = list(paths)
    env = dict(os.environ, LC_ALL="C")
    for start in range(0, len(listed), BATCH):
        batch = listed[start : start + BATCH]
        try:
            proc = subprocess.run(
                ["nm", "-g", "-A", *batch], capture_output=True, text=True, errors="replace", env=env, timeout=NM_TIMEOUT
            )
        except FileNotFoundError:
            raise Problem("cannot run nm: not found")
        except OSError as err:
            raise Problem(f"cannot run nm: {err.strerror}")
        except subprocess.TimeoutExpired:
            raise Problem(f"nm did not end in {NM_TIMEOUT} seconds")
        if proc.returncode != 0:
            raise Problem(f"nm ended with status {proc.returncode}")
        known = set(batch)
        for line in proc.stdout.splitlines():
            # The object path may hold a colon: take the first colon after which the path is a known one.
            cut = -1
            while True:
                cut = line.find(":", cut + 1)
                if cut < 0 or line[:cut] in known:
                    break
            if cut < 0:
                continue
            unit, rest = paths[line[:cut]], line[cut + 1 :]
            m = DEFINES.fullmatch(rest.strip()) if rest[:1] != " " else None
            if m:
                if m.group(2) != "U":
                    defined[unit].add(m.group(3))
                continue
            m = NEEDS.fullmatch(rest)
            if m:
                needed[unit].add(m.group(1))
    return defined, needed


# Classes.


def image_of(name: str, images: list[str]) -> str | None:
    for image in images:
        if name.endswith("_" + image):
            return image
    return None


def address_of(name: str, world: World) -> int | None:
    if name in world.declared_at:
        return world.declared_at[name]
    if name in world.symbols:
        return world.symbols[name]
    m = FORM.fullmatch(name)
    return int(m.group(1), 16) if m else None


def classify(name: str, world: World) -> tuple[str, int | None, str | None, str | None]:
    """Class, address, image and the other name of one needed name."""
    kinds = {kind for kind, _ in world.declared.get(name, [])}
    address = address_of(name, world)
    image = image_of(name, world.images)
    if "sdk" in kinds:
        return "library by name", address, image, None
    if "assembly" in kinds:
        return "assembly", address, image, None
    if any(kind == "c" and not passed for kind, passed in world.declared.get(name, [])):
        return "unit not compiled", address, image, None
    if address is None:
        return "unknown", address, image, None
    others = world.passed_at.get((image, address), set()) - {name}
    if others:
        return "C under another name", address, image, min(others)
    if image is None and address in world.library:
        named = world.sdk_at.get(address)
        return "library by address", address, image, min(named) if named else None
    if image is None and address in world.game:
        return "game function", address, image, None
    if name.startswith("func_"):
        return ("module function" if image else "function elsewhere"), address, image, None
    if SCRATCHPAD[0] <= address <= SCRATCHPAD[1]:
        return "scratchpad data", address, image, None
    return ("module data" if image else "resident data"), address, image, None


# Output.


def render(names_total: tuple[int, int], defined: int, rows: list[tuple]) -> str:
    by_class: dict[str, list[tuple]] = defaultdict(list)
    for row in rows:
        by_class[row[1]].append(row)
    lines = [f"objects: {names_total[0]} of {names_total[1]} units", f"defined: {defined} names", f"needed: {len(rows)} names"]
    for cls in CLASSES:
        group = by_class[cls]
        wanted = set()
        for row in group:
            wanted |= row[6]
        lines.append(f"{cls}: {len(group)} names, needed by {len(wanted)} units")
    named = sum(1 for row in by_class["library by address"] if row[5] is not None or not row[0].startswith("func_"))
    lines.append(f"library by address: {named} named, {len(by_class['library by address']) - named} unnamed")
    if by_class["unknown"]:
        lines.append("unknown: " + " ".join(row[0] for row in by_class["unknown"]))
    return "\n".join(lines) + "\n"


def render_rows(rows: list[tuple]) -> str:
    out = []
    for name, cls, address, image, count, other, _ in rows:
        cells = [name, cls, "-" if address is None else f"{address:08x}", image or "-", str(count), other or "-"]
        out.append("\t".join(cells) + "\n")
    return "".join(out)


def main() -> int:
    parser = argparse.ArgumentParser(description="Sort the names that the compiled game units need and none defines.")
    parser.add_argument("--build", type=Path, default=REPO / "port" / "build" / "hostcheck")
    parser.add_argument("--config", type=Path, default=REPO / "ps1" / "src" / "build.toml")
    parser.add_argument("--symbols", type=Path, default=REPO / "ps1" / "src" / "symbols.ld")
    parser.add_argument("--inventory", type=Path, default=REPO / "ps1" / "inventory")
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    try:
        status = read_status(args.build)
        units, images = read_config(args.config)
        world = build_world(units, images, status, args)
        passed = sorted(name for name, ok in status.items() if ok)
        defined, needed = read_objects(args.build, passed)
        every = set().union(*defined.values()) if defined else set()
        users: dict[str, set[str]] = defaultdict(set)
        for unit, names in needed.items():
            for name in names - every:
                users[name].add(unit)
        rows = []
        for name in sorted(users):
            cls, address, image, other = classify(name, world)
            rows.append((name, cls, address, image, len(users[name]), other, users[name]))
        sys.stdout.write(render((len(passed), len(status)), len(every), rows))
        if args.out:
            try:
                args.out.write_text(render_rows(rows))
            except OSError as err:
                raise Problem(f"cannot write {args.out}: {err.strerror}")
    except Problem as err:
        print(f"hostneeds.py: {err}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
