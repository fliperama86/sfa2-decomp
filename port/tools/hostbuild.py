#!/usr/bin/env python3
"""Build the game's C as one program for the machine this runs on, with every name at its PS1 address.

Reads the build configuration, the symbol file, the nonmatching functions
and the function inventory, runs the C compiler of the target machine, and
links the runtime of the port (`port/src/*.c`) with the units. Nothing of
the game is read but its published source and tables; no disc image is
touched, and nothing is run. Python 3.11 or later, standard library only,
and this repository's modules `hostcheck.py` (the reading of the
configuration, the units, the shared-struct header and the compiler runs),
`ps1/tools/coveragemap.py` (the reading of the inventory) and
`ps1/tools/structgen.py`, used as modules and not changed.

The mechanism
-------------

The program maps the PS1's memory at its own addresses and loads the
game's data there. Every name of the game, data or function, is linked as
its PS1 address (an absolute symbol). A unit's C is compiled to assembly
and every global symbol that the unit DEFINES is renamed there to
`impl_NAME`, on its definition lines only; its references keep the plain
name and so bind to the absolute symbol. The runtime writes at each PS1
function address a jump to `impl_NAME`.

The units
---------

The C units of the configuration as `hostcheck` selects them (not under
`sdk/`, source ending in `.c`), and every `func_*.c` of every folder
`*_nonmatching` next to the configuration. A nonmatching file defines one
function, named by the file's stem, whose address is the eight-digit hex
number after `func_`; what follows that number, after an underscore, names
the image the function is in and must be an image of the configuration
(no suffix: the resident executable). A function that a unit of the
configuration declares and a nonmatching file defines is an error naming
both, and so is a nonmatching stem that is also the name of a compiled
unit. A unit whose `image` is declared `like` another image is an error; the
second placement of a `like` image is not built in this step, and the
images that are `like` another are counted.

Each unit is compiled alone to assembly,

    CC -std=gnu89 -O1 -fno-inline -fno-strict-aliasing -fwrapv -fno-pic
       -fno-builtin -ffreestanding -fno-stack-protector
       -fno-asynchronous-unwind-tables -fno-ident -S -I BUILD/gen
       -o BUILD/asm/UNIT.s SOURCE

with the shared-struct header written to `BUILD/gen` as `hostcheck` writes
it. A unit that fails is counted, listed in `BUILD/failed.tsv` with the
first line of its first error (or the reason), and the build goes on; the
tool then ends with status 1. A compiler that has not ended after
`--timeout` seconds is stopped (the unit fails; any other run of the
compiler is an error).

The rename
----------

`rename_definitions(text, underscore)` returns the assembly with the
definitions renamed and the set of symbols defined. A symbol is defined when
a `.globl` or `.global` line names it and a label `NAME:` or a
`.comm NAME,` line gives it a place. (A `.comm` symbol is global by nature
and counts as defined with no `.globl` line.) For each defined symbol exactly
these lines change, in the symbol's name only: the `.globl` line, the label,
the `.comm` line, a COFF `.def NAME;` line, an ELF `.type NAME,` line and an
ELF `.size NAME,` line, whose operand is renamed with it, since it names the
symbol it measures. No other operand, no other directive, no local label, no
symbol that is only referenced or only named by a `.globl`, and no symbol
whose name merely begins or ends like a defined one changes. Line ends and
the blanks of every line are kept. With `underscore` true the target puts
`_` before C names (`_f` becomes `_impl_f`); false is for ELF (`f` becomes
`impl_f`). After the rewrite the tool checks that no defined name is left as
a label; a miss is an error. The renamed assembly replaces
`BUILD/asm/UNIT.s` and is assembled to `BUILD/obj/UNIT.o`. Whether the
target uses the underscore is found by compiling a one-line probe.

The namespace
-------------

The game has functions named like those of the host's C library (`memcpy`,
`printf`, `rand`, `strcmp`). So that no game name can meet a host name,
after assembling every object of a unit is run through
`objcopy --redefine-syms=BUILD/gen/redefine.txt`, a file with one line
`NAME ps1_NAME` (`_NAME _ps1_NAME` with the target's underscore) for every
name of the names file and every host data alias, once each. The only plain
game names left in an object are references, so this renames references. The
runtime's objects are not touched.

The names
---------

`BUILD/gen/names.ld`, one line `ps1_NAME = 0xADDRESS;` (with the target's
underscore, `_ps1_NAME`; it defines the `ps1_` name and never the plain
one), in order of name, for every assignment of `symbols.ld` (read
next to the configuration), every function that a selected unit declares
in the configuration (name and address from its `functions`; a unit that
failed to compile included, since its callers need the name) and every
nonmatching function. A name with two different addresses is an error. The
names that only an `[image.symbols]` table gives are not written. A symbol
that a unit defines and that is neither a function of the configuration nor
in `symbols.ld` is data defined in C, which has no PS1 address: it stays
renamed, and the line `ps1_NAME = impl_NAME;` makes the references use the host
copy. These names are counted and listed.

The tables
----------

`BUILD/gen/port_tables.c` defines what `RUNTIME/port_tables.h` declares.
`port_images` holds every image of the configuration in its order.
`port_functions` holds every function with C: a function that a unit which
compiled declares and defines, and a nonmatching function that its file
defines; sorted by image (the resident executable first, as -1) and
address. Two at one address of one image is an error. `port_absents` holds
every function of the inventory (`ps1/inventory/game.tsv`, `library.tsv`
and, for the images of the configuration that are not `like` another,
`modules.tsv`; a row of `modules.tsv` belongs to an image as
`coveragemap.py` assigns it) that no function with C is at: `library` is 1
for a row of `library.tsv`, the name is the one that a unit of the
configuration declares for that address in that image, else
`func_<address>` with `_<image>` for a module image. Sorted as above.

The link
--------

All objects of the units, every `RUNTIME/*.c` compiled with
`CC -O1 -Wall -Wextra -c`, `port_tables.o` and `names.ld` go to one run of
the compiler by a response file, with `-static -Wl,--large-address-aware
-Wl,--disable-dynamicbase`, to `BUILD/NAME`. The link is then verified, and
a miss ends the tool with status 1 naming the symbols: with `nm` on the
linked file, every `ps1_` name of `names.ld` has exactly its address, no plain game name is
at a PS1 address, every
`impl_` function of `port_functions` exists outside the PS1's ranges
(`0x80000000` to `0x801fffff`, `0x1f800000` to `0x1f8003ff`), every host
data alias `ps1_NAME` has the address of its `impl_` copy outside those
ranges. The plain name may exist (the host's own function of that name).

Output
------

Standard output, in this order, with nothing else:

    compiler: FIRST LINE OF `CC --version`
    units: U compiled, N of them nonmatching, F failed
    like images not built: L
    functions with C: C
    functions without C: A, library L2, game and modules G
    names at PS1 addresses: P
    data defined in C, at host addresses: D
    linked: PATH, verified

PATH is relative to the current folder when the linked file lies under it.

U counts the units tried, nonmatching ones included. A is the number of
rows of `port_absents`, L2 those of the library and G the others (the
resident game and the module images), so that A = L2 + G. The last line is
printed only for a link that was verified. With `--list` the names behind
F, D and the game rows of A follow, one per line:

    failed: UNIT: FIRST ERROR LINE
    data: NAME
    absent: NAME 0xADDRESS IMAGE

(IMAGE is `-` for the resident executable). Objects are always rebuilt.

Exit status: 0 when everything built and verified; 1 when a unit failed, the
runtime or the link failed or the verification missed; 2 when an input is
missing or malformed, a name has two addresses, a definition is twice or the
compiler cannot be run, with one line on standard error that names it.

usage:
  hostbuild.py [--config BUILD_TOML] [--cc CC] [--nm NM] [--objcopy OBJCOPY] [--build DIR]
               [--out NAME] [--runtime DIR] [--jobs N] [--timeout SECONDS]
               [--list]

The defaults: `ps1/src/build.toml`, found from the place of this script;
`i686-w64-mingw32-gcc`; the `nm` next to CC with the same prefix (`gcc` at
its end replaced by `nm`, else `nm`); `port/build/host`; `sfa2.exe`;
`port/src`; as many jobs as the machine has processors; 300 seconds.
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / "ps1" / "tools"))
sys.path.insert(0, str(Path(__file__).resolve().parent))

import coveragemap  # noqa: E402
import hostcheck  # noqa: E402
import structgen  # noqa: E402

Problem = hostcheck.Problem

COMPILE_FLAGS = [
    "-std=gnu89", "-O1", "-fno-inline", "-fno-strict-aliasing", "-fwrapv", "-fno-pic", "-fno-builtin",
    "-ffreestanding", "-fno-stack-protector", "-fno-asynchronous-unwind-tables", "-fno-ident",
]
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]
PS1_RANGES = ((0x80000000, 0x801FFFFF), (0x1F800000, 0x1F8003FF))

NONMATCHING = re.compile(r"^func_([0-9a-fA-F]{8})(?:_(.+))?$")
ASSIGNMENT = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0[xX][0-9a-fA-F]+|[0-9]+)\s*;")
COMMENT = re.compile(r"/\*.*?\*/", re.S)

LABEL = re.compile(r"^(\s*)([A-Za-z_.$][\w.$@]*):")
GLOBL = re.compile(r"^(\s*\.glob(?:a)?l\s+)(.*?)(\s*)$")
COMM = re.compile(r"^(\s*\.comm\s+)([^\s,]+)(\s*,.*)$")
DEF = re.compile(r"^(\s*\.def\s+)([^\s;]+)(\s*(?:;.*)?)$")
TYPE = re.compile(r"^(\s*\.type\s+)([^\s,]+)(\s*,.*)$")
SIZE = re.compile(r"^(\s*\.size\s+)([^\s,]+)(\s*,)(.*)$")
SYMBOL = re.compile(r"[A-Za-z_.$][\w.$]*")


# The rename.


def renamed(name: str, underscore: bool) -> str:
    if underscore and name.startswith("_"):
        return "_impl_" + name[1:]
    return "impl_" + name


def rename_definitions(text: str, underscore: bool) -> tuple[str, set[str]]:
    """The assembly with the definitions of its global symbols renamed, and the symbols defined."""
    lines = text.split("\n")
    stripped = [x[:-1] if x.endswith("\r") else x for x in lines]
    named: set[str] = set()
    placed: set[str] = set()
    for line in stripped:
        match = GLOBL.match(line)
        if match:
            named.update(n.strip() for n in match.group(2).split(",") if n.strip())
            continue
        match = COMM.match(line)
        if match:
            placed.add(match.group(2))
            named.add(match.group(2))
            continue
        match = LABEL.match(line)
        if match:
            placed.add(match.group(2))
    defined = named & placed
    new = {name: renamed(name, underscore) for name in defined}
    out = []
    for raw, line in zip(lines, stripped):
        tail = raw[len(line):]
        match = GLOBL.match(line)
        if match:
            parts = match.group(2).split(",")
            fixed = ",".join(_swap(p, new) for p in parts)
            line = match.group(1) + fixed + match.group(3)
        elif COMM.match(line):
            match = COMM.match(line)
            line = match.group(1) + new.get(match.group(2), match.group(2)) + match.group(3)
        elif DEF.match(line):
            match = DEF.match(line)
            line = match.group(1) + new.get(match.group(2), match.group(2)) + match.group(3)
        elif TYPE.match(line):
            match = TYPE.match(line)
            line = match.group(1) + new.get(match.group(2), match.group(2)) + match.group(3)
        elif SIZE.match(line):
            match = SIZE.match(line)
            operand = SYMBOL.sub(lambda m: new.get(m.group(0), m.group(0)), match.group(4))
            line = match.group(1) + new.get(match.group(2), match.group(2)) + match.group(3) + operand
        else:
            match = LABEL.match(line)
            if match and match.group(2) in new:
                line = match.group(1) + new[match.group(2)] + ":" + line[match.end():]
        out.append(line + tail)
    return "\n".join(out), defined


def _swap(part: str, new: dict[str, str]) -> str:
    name = part.strip()
    return part.replace(name, new[name], 1) if name in new else part


def labels_left(text: str, defined: set[str]) -> list[str]:
    """The defined names that are still a label in the text."""
    found = set()
    for line in text.split("\n"):
        match = LABEL.match(line.rstrip("\r"))
        if match and match.group(2) in defined:
            found.add(match.group(2))
    return sorted(found)


def c_name(symbol: str, underscore: bool) -> str:
    return symbol[1:] if underscore and symbol.startswith("_") else symbol


# Units.


@dataclass
class Function:
    name: str
    address: int
    image: str | None  # None: the resident executable
    unit: str


@dataclass
class Job:
    name: str
    source: Path
    nonmatching: bool
    functions: list[Function]


@dataclass
class Selection:
    jobs: list[Job]
    declared: list[Function]  # every function any unit of the configuration declares, nonmatching included
    like_images: int
    images: list[dict]


def read_images(config: dict, path: Path) -> list[dict]:
    images = config.get("image", [])
    if not isinstance(images, list):
        raise Problem(f"{path}: `image` is not an array of tables")
    seen = set()
    for image in images:
        name = image.get("name") if isinstance(image, dict) else None
        if not isinstance(name, str) or not name or name in seen:
            raise Problem(f"{path}: an image has no name or one that is used twice: {name!r}")
        if not isinstance(image.get("address"), int) or not isinstance(image.get("slot"), int):
            raise Problem(f"{path}: image {name} needs integer `address` and `slot`")
        seen.add(name)
    for image in images:
        if "like" in image and image["like"] not in seen:
            raise Problem(f"{path}: image {image['name']} is like {image['like']}, which is not declared")
    return images


def read_functions(entry: dict, path: Path) -> list[Function]:
    out = []
    image = entry.get("image")
    for item in entry.get("functions", []):
        if not isinstance(item, dict) or not isinstance(item.get("name"), str) or not isinstance(item.get("address"), int):
            raise Problem(f"{path}: unit {entry.get('name')} has a function without a name and an address")
        out.append(Function(item["name"], item["address"], image, entry["name"]))
    return out


def select_units(config: dict, path: Path) -> Selection:
    """The jobs to compile, every declared function, and the count of images that are like another."""
    images = read_images(config, path)
    by_name = {i["name"]: i for i in images}
    units, _, _ = hostcheck.read_units(config, path)
    compiled = {u.name for u in units}
    jobs: list[Job] = []
    declared: list[Function] = []
    for entry in config.get("unit", []):
        image = entry.get("image")
        if image is not None and image not in by_name:
            raise Problem(f"{path}: unit {entry['name']} is in image {image}, which is not declared")
        if image is not None and "like" in by_name[image]:
            raise Problem(f"{path}: unit {entry['name']} is in image {image}, which is like {by_name[image]['like']}")
        declared.extend(read_functions(entry, path))
    owner = {}
    for fn in declared:
        owner.setdefault(fn.name, fn.unit)
    for u in units:
        entry = next(e for e in config["unit"] if e["name"] == u.name)
        jobs.append(Job(u.name, u.path, False, read_functions(entry, path)))
    seen_files: dict[str, Path] = {}
    for folder in sorted(path.parent.glob("*_nonmatching")):
        if not folder.is_dir():
            continue
        for file in sorted(folder.glob("func_*.c")):
            stem = file.stem
            match = NONMATCHING.match(stem)
            if not match:
                raise Problem(f"{file}: a nonmatching file is named func_ADDRESS.c or func_ADDRESS_IMAGE.c with an eight-digit hex address")
            image = match.group(2)
            if image is not None and image not in by_name:
                raise Problem(f"{file}: names image {image}, which {path} does not declare")
            if stem in owner:
                raise Problem(f"function {stem} is declared by unit {owner[stem]} of {path} and defined by {file}")
            if stem in compiled:
                raise Problem(f"{file} has the name of the unit {stem} of {path}")
            if stem in seen_files:
                raise Problem(f"function {stem} is defined by {seen_files[stem]} and by {file}")
            seen_files[stem] = file
            fn = Function(stem, int(match.group(1), 16), image, stem)
            declared.append(fn)
            jobs.append(Job(stem, file, True, [fn]))
    return Selection(jobs, declared, sum(1 for i in images if "like" in i), images)


# The names.


def read_symbols(text: str, origin: str = "symbols.ld") -> list[tuple[str, int, str]]:
    """The assignments of a symbol file: name, address, where it came from."""
    body = COMMENT.sub(" ", text)
    out = [(m.group(1), int(m.group(2), 0), origin) for m in ASSIGNMENT.finditer(body)]
    rest = ASSIGNMENT.sub(" ", body).strip()
    if rest:
        raise Problem(f"{origin}: cannot read: {rest.splitlines()[0].strip()[:60]!r}")
    return out


def merge_names(entries: list[tuple[str, int, str]]) -> dict[str, int]:
    names: dict[str, int] = {}
    origin: dict[str, str] = {}
    for name, address, where in entries:
        if name in names and names[name] != address:
            raise Problem(f"{name} has two addresses: {names[name]:#x} ({origin[name]}) and {address:#x} ({where})")
        names.setdefault(name, address)
        origin.setdefault(name, where)
    return names


def render_names(names: dict[str, int], aliases: list[str], underscore: bool) -> str:
    """The names file: it defines `ps1_NAME`, never the plain name."""
    us = "_" if underscore else ""
    lines = [f"{us}ps1_{n} = 0x{names[n]:08x};\n" for n in sorted(names)]
    lines += [f"{us}ps1_{n} = {us}impl_{n};\n" for n in sorted(aliases)]
    return "".join(lines)


def render_redefine(names, aliases: list[str], underscore: bool) -> str:
    """The file for `objcopy --redefine-syms`: `NAME ps1_NAME` once for every name of the names file."""
    us = "_" if underscore else ""
    return "".join(f"{us}{n} {us}ps1_{n}\n" for n in sorted(set(names) | set(aliases)))


# The tables.


@dataclass
class Row:
    address: int
    image: str | None
    library: bool


def read_inventory(directory: Path, config: dict) -> list[Row]:
    """The rows of the inventory that the build is about: game, library and the images that are not like another."""
    try:
        rows = []
        for block in coveragemap.read_resident(directory):
            rows += [Row(f.address, None, block.key != coveragemap.GAME) for f in block.functions]
        modules = coveragemap.read_modules(directory)
        coveragemap.assign_images(modules, config)
    except coveragemap.Problem as err:
        raise Problem(str(err))
    except OSError as err:
        raise Problem(f"cannot read the inventory in {directory}: {err.strerror}")
    for block in modules:
        if block.image and not block.second_link:
            rows += [Row(f.address, block.image, False) for f in block.functions]
    return rows


def default_name(address: int, image: str | None) -> str:
    return f"func_{address:08x}" + (f"_{image}" if image else "")


def order_key(image: str | None, address: int, index: dict[str | None, int]):
    return index[image], address


def build_tables(images: list[dict], with_c: list[tuple[Function, str]], rows: list[Row], declared: list[Function]):
    """(port_functions, port_absents) as lists of tuples, sorted; raises Problem for two at one address."""
    index: dict[str | None, int] = {None: -1, **{i["name"]: n for n, i in enumerate(images)}}
    seen: dict[tuple[str | None, int], str] = {}
    functions = []
    for fn, impl in with_c:
        key = (fn.image, fn.address)
        if key in seen:
            raise Problem(f"{seen[key]} and {fn.name} are both functions with C at {fn.address:#x}" + (f" in image {fn.image}" if fn.image else ""))
        seen[key] = fn.name
        functions.append((index[fn.image], fn.address, impl, fn.name))
    functions.sort(key=lambda f: (f[0], f[1]))
    names: dict[tuple[str | None, int], str] = {}
    for fn in declared:
        names.setdefault((fn.image, fn.address), fn.name)
    absents = []
    taken = set()
    for row in rows:
        key = (row.image, row.address)
        if key in seen or key in taken:
            continue
        taken.add(key)
        absents.append((index[row.image], row.address, names.get(key) or default_name(row.address, row.image), int(row.library)))
    absents.sort(key=lambda a: (a[0], a[1]))
    return functions, absents


def c_string(text: str) -> str:
    return '"' + text.replace("\\", "\\\\").replace('"', '\\"') + '"'


def render_tables(images: list[dict], functions: list[tuple], absents: list[tuple]) -> str:
    out = ['/* Written by hostbuild.py; not to be edited. */\n', '#include "port_tables.h"\n\n']
    for impl in dict.fromkeys(f[2] for f in functions):
        out.append(f"extern void {impl}(void);\n")
    out.append("\n")

    def table(kind: str, name: str, rows: list[str], zero: str):
        out.append(f"const struct {kind} {name}[] = {{\n")
        out.extend(f"    {r},\n" for r in rows)
        if not rows:
            out.append(f"    {zero},\n")
        out.append("};\n")
        out.append(f"const unsigned {kind}_count = {len(rows)};\n\n")

    table("port_image", "port_images", [
        "{ %s, 0x%08xu, %s, 0x%xu }" % (c_string(i["name"]), i["address"], c_string(i["like"]) if "like" in i else "0", i["slot"])
        for i in images
    ], "{ 0, 0, 0, 0 }")
    table("port_function", "port_functions", [
        "{ 0x%08xu, (void *)%s, %s, %d }" % (f[1], f[2], c_string(f[3]), f[0]) for f in functions
    ], "{ 0, 0, 0, 0 }")
    table("port_absent", "port_absents", [
        "{ 0x%08xu, %s, %d, %d }" % (a[1], c_string(a[2]), a[0], a[3]) for a in absents
    ], "{ 0, 0, 0, 0 }")
    return "".join(out)


# The verification.


NM_LINE = re.compile(r"^([0-9a-fA-F]+)\s+(\S)\s+(\S+)$")


def in_ps1(address: int) -> bool:
    return any(low <= address <= high for low, high in PS1_RANGES)


def verify_link(nm_text: str, names: dict[str, int], aliases: list[str], impls: list[str], underscore: bool) -> list[str]:
    """The misses of the linked file, from the text of `nm`; empty when none."""
    us = "_" if underscore else ""
    seen: dict[str, list[int]] = {}
    for line in nm_text.splitlines():
        match = NM_LINE.match(line.strip())
        if match:
            seen.setdefault(match.group(3), []).append(int(match.group(1), 16))
    misses = []
    for name in sorted(names):
        found = seen.get(us + "ps1_" + name)
        if not found:
            misses.append(f"ps1_{name}: not in the linked file")
        elif set(found) != {names[name]}:
            misses.append(f"ps1_{name}: at {', '.join(f'{a:#x}' for a in sorted(set(found)))}, wanted {names[name]:#x}")
        plain = [a for a in seen.get(us + name, []) if in_ps1(a)]
        if plain:
            misses.append(f"{name}: a plain game name at the PS1 address {plain[0]:#x}")
    for name in sorted(impls):
        found = seen.get(us + "impl_" + name)
        if not found:
            misses.append(f"impl_{name}: not in the linked file")
        elif any(in_ps1(a) for a in found):
            misses.append(f"impl_{name}: at a PS1 address {found[0]:#x}")
    for name in sorted(aliases):
        plain, copy = seen.get(us + "ps1_" + name), seen.get(us + "impl_" + name)
        if not plain or not copy:
            misses.append(f"{name}: host data or its copy is not in the linked file")
        elif set(plain) != set(copy) or any(in_ps1(a) for a in plain + copy):
            misses.append(f"{name}: host data at {plain[0]:#x}, its copy at {copy[0]:#x}")
    return misses


# Running the compiler.


def first_error(stderr: str) -> str:
    lines = [x.strip() for x in stderr.splitlines() if x.strip()]
    for line in lines:
        if ": error:" in line or ": fatal error:" in line:
            return line
    return lines[0] if lines else "no diagnostic"


@dataclass
class Outcome:
    job: Job
    ok: bool = False
    reason: str = ""
    defined: set[str] = field(default_factory=set)  # C names


def build_unit(job: Job, cc: str, build: Path, underscore: bool, timeout: int) -> Outcome:
    out = Outcome(job)
    asm, obj = build / "asm" / f"{job.name}.s", build / "obj" / f"{job.name}.o"
    asm.unlink(missing_ok=True)
    obj.unlink(missing_ok=True)
    proc = hostcheck.compile_run([cc, *COMPILE_FLAGS, "-S", "-I", str(build / "gen"), "-o", str(asm), str(job.source)], timeout)
    if proc is None:
        out.reason = f"no end after {timeout} seconds"
        return out
    if proc.returncode != 0:
        out.reason = first_error(proc.stderr)
        return out
    try:
        with open(asm, encoding="utf-8", errors="surrogateescape", newline="") as handle:
            text = handle.read()
        new, defined = rename_definitions(text, underscore)
        left = labels_left(new, defined)
        if left:
            raise Problem(f"the rename of {job.name} left these defined names as labels: {' '.join(left)}")
        with open(asm, "w", encoding="utf-8", errors="surrogateescape", newline="") as handle:
            handle.write(new)
    except OSError as err:
        raise Problem(f"cannot rewrite {asm}: {err.strerror}")
    proc = hostcheck.compile_run([cc, "-c", "-o", str(obj), str(asm)], timeout)
    if proc is None or proc.returncode != 0:
        out.reason = "assembler: " + (first_error(proc.stderr) if proc else f"no end after {timeout} seconds")
        return out
    out.ok = True
    out.defined = {c_name(s, underscore) for s in defined}
    return out


def detect_underscore(cc: str, probe: Path, timeout: int) -> bool:
    source, asm = probe / "underscore.c", probe / "underscore.s"
    source.write_text("int port_probe_symbol = 1;\n")
    proc = hostcheck.compile_run([cc, "-S", "-o", str(asm), str(source)], timeout)
    if proc is None or proc.returncode != 0:
        raise Problem(f"cannot compile a probe with compiler {cc}")
    text = asm.read_text()
    if re.search(r"^\s*\.glob(?:a)?l\s+_port_probe_symbol\s*$", text, re.M):
        return True
    if re.search(r"^\s*\.glob(?:a)?l\s+port_probe_symbol\s*$", text, re.M):
        return False
    raise Problem(f"cannot tell whether compiler {cc} puts an underscore before C names")


def link_errors(stderr: str) -> str:
    """The linker's complaints, each symbol once, at most twenty."""
    found = sorted(set(re.findall(r"(?:undefined reference to|multiple definition of) `[^']*'", stderr)))
    if found:
        return "\n".join(found[:20]) + (f"\n... and {len(found) - 20} more" if len(found) > 20 else "")
    return first_error(stderr)


def default_nm(cc: str) -> str:
    return cc[:-3] + "nm" if cc.endswith("gcc") else "nm"


def default_objcopy(cc: str) -> str:
    return cc[:-3] + "objcopy" if cc.endswith("gcc") else "objcopy"


def redefine(job: str, objcopy: str, build: Path, timeout: int) -> str:
    """Rename the references of one object into the `ps1_` namespace; empty when it worked, else the reason."""
    obj = build / "obj" / f"{job}.o"
    proc = hostcheck.compile_run([objcopy, f"--redefine-syms={build / 'gen' / 'redefine.txt'}", str(obj)], timeout)
    if proc is None or proc.returncode != 0:
        return "objcopy: " + (first_error(proc.stderr) if proc else f"no end after {timeout} seconds")
    return ""


def quote(path: Path) -> str:
    return '"' + str(path).replace("\\", "\\\\").replace('"', '\\"') + '"'


# The run.


def read_text(path: Path) -> str:
    try:
        return path.read_text()
    except OSError as err:
        raise Problem(f"cannot read {path}: {err.strerror}")


def write_text(path: Path, text: str) -> None:
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
    except OSError as err:
        raise Problem(f"cannot write {path}: {err.strerror}")


class Failure(Exception):
    """The build ran and did not produce a verified program."""


def run(args: argparse.Namespace, out: list[str], listing: list[str]) -> int:
    config_path: Path = args.config
    config = hostcheck.read_config(config_path)
    fields_path, header = hostcheck.read_types(config, config_path)
    selection = select_units(config, config_path)
    model = hostcheck.read_model(fields_path)
    symbols = read_symbols(read_text(config_path.parent / "symbols.ld"))
    runtime: Path = args.runtime
    if not (runtime / "port_tables.h").is_file():
        raise Problem(f"{runtime / 'port_tables.h'} does not exist")
    runtime_sources = sorted(runtime.glob("*.c"))
    inventory = read_inventory(config_path.parent.parent / "inventory", config)

    # Names that can be worked out before any compiler runs.
    entries = list(symbols) + [(fn.name, fn.address, f"unit {fn.unit}") for fn in selection.declared]
    function_names = {fn.name for fn in selection.declared}
    names = merge_names(entries)

    version = hostcheck.compiler_version(args.cc, args.timeout)
    out.append(f"compiler: {version}")
    build: Path = args.build
    for sub in ("gen", "asm", "obj", "rt", "probe"):
        (build / sub).mkdir(parents=True, exist_ok=True)
    write_text(build / "gen" / header, structgen.generate_header(model, Path(header).name))
    underscore = detect_underscore(args.cc, build / "probe", args.timeout)

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [pool.submit(build_unit, j, args.cc, build, underscore, args.timeout) for j in selection.jobs]
        outcomes = sorted((f.result() for f in futures), key=lambda o: o.job.name)
    failed = [o for o in outcomes if not o.ok]
    write_text(build / "failed.tsv", "".join(f"{o.job.name}\t{o.reason}\n" for o in failed))
    listing.extend(f"failed: {o.job.name}: {o.reason}" for o in failed)
    out.append(f"units: {len(outcomes)} compiled, {sum(1 for o in outcomes if o.job.nonmatching)} of them nonmatching, {len(failed)} failed")
    out.append(f"like images not built: {selection.like_images}")

    # Functions with C.
    with_c: list[tuple[Function, str]] = []
    defined_all: set[str] = set()
    for o in outcomes:
        if not o.ok:
            continue
        defined_all |= o.defined
        for fn in o.job.functions:
            if fn.name in o.defined:
                with_c.append((fn, "impl_" + fn.name))
    functions, absents = build_tables(selection.images, with_c, inventory, selection.declared)
    out.append(f"functions with C: {len(functions)}")
    library = sum(1 for a in absents if a[3])
    out.append(f"functions without C: {len(absents)}, library {library}, game and modules {len(absents) - library}")
    images_by_index = [i["name"] for i in selection.images]
    listing.extend(
        f"absent: {a[2]} {a[1]:#x} {images_by_index[a[0]] if a[0] >= 0 else '-'}" for a in absents if not a[3]
    )

    symbol_names = {s[0] for s in symbols}
    aliases = sorted(n for n in defined_all if n not in function_names and n not in symbol_names)
    out.append(f"names at PS1 addresses: {len(names)}")
    out.append(f"data defined in C, at host addresses: {len(aliases)}")
    listing.extend(f"data: {n}" for n in aliases)
    names_path = build / "gen" / "names.ld"
    write_text(names_path, render_names(names, aliases, underscore))
    write_text(build / "gen" / "redefine.txt", render_redefine(names, aliases, underscore))
    objcopy = args.objcopy or default_objcopy(args.cc)
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        reasons = list(pool.map(lambda o: redefine(o.job.name, objcopy, build, args.timeout), [o for o in outcomes if o.ok]))
    broken = [(o.job.name, r) for o, r in zip([o for o in outcomes if o.ok], reasons) if r]
    if broken:
        raise Failure("\n".join(f"{n}: {r}" for n, r in broken[:20]))
    tables_path = build / "gen" / "port_tables.c"
    write_text(tables_path, render_tables(selection.images, functions, absents))

    # The runtime, the tables and the link.
    objects = sorted(o.job.name for o in outcomes if o.ok)
    rt_objects: list[Path] = []
    for source in [*runtime_sources, tables_path]:
        obj = build / "rt" / f"{source.stem}.o"
        obj.unlink(missing_ok=True)
        proc = hostcheck.compile_run([args.cc, "-O1", "-Wall", "-Wextra", "-c", "-I", str(runtime), "-o", str(obj), str(source)], args.timeout)
        if proc is None or proc.returncode != 0:
            raise Failure(f"{source}: " + (first_error(proc.stderr) if proc else f"no end after {args.timeout} seconds"))
        rt_objects.append(obj)
    response = build / "link.rsp"
    write_text(response, "".join(quote(p) + "\n" for p in [*(build / "obj" / f"{n}.o" for n in objects), *rt_objects, names_path]))
    exe = build / args.out
    exe.unlink(missing_ok=True)
    proc = hostcheck.compile_run([args.cc, f"@{response}", *LINK_FLAGS, "-o", str(exe)], args.timeout)
    if proc is None or proc.returncode != 0 or not exe.is_file():
        raise Failure("link: " + (link_errors(proc.stderr) if proc else f"no end after {args.timeout} seconds"))
    nm = hostcheck.compile_run([args.nm or default_nm(args.cc), str(exe)], args.timeout)
    if nm is None or nm.returncode != 0:
        raise Failure("nm did not run on the linked file")
    misses = verify_link(nm.stdout, names, aliases, [f[3] for f in functions], underscore)
    if misses:
        raise Failure("the linked file does not verify:\n" + "\n".join(misses))
    out.append(f"linked: {shown(exe)}, verified")
    return 1 if failed else 0


def positive(text: str) -> int:
    value = int(text)
    if value < 1:
        raise argparse.ArgumentTypeError("must be at least 1")
    return value


def shown(path: Path) -> str:
    """The path as one would type it here: relative to the current folder when it lies under it."""
    try:
        return str(path.resolve().relative_to(Path.cwd().resolve()))
    except ValueError:
        return str(path)


def main() -> int:
    parser = argparse.ArgumentParser(description="Build the game's C as one program with every name at its PS1 address.")
    parser.add_argument("--config", type=Path, default=REPO / "ps1/src/build.toml")
    parser.add_argument("--cc", default="i686-w64-mingw32-gcc")
    parser.add_argument("--nm", default=None)
    parser.add_argument("--objcopy", default=None)
    parser.add_argument("--build", type=Path, default=REPO / "port/build/host")
    parser.add_argument("--out", default="sfa2.exe")
    parser.add_argument("--runtime", type=Path, default=REPO / "port/src")
    parser.add_argument("--timeout", type=positive, default=300)
    parser.add_argument("--jobs", type=positive, default=os.cpu_count() or 1)
    parser.add_argument("--list", action="store_true")
    args = parser.parse_args()
    out: list[str] = []
    listing: list[str] = []
    try:
        status = run(args, out, listing)
    except Problem as err:
        print(f"hostbuild.py: {' '.join(str(err).split())}", file=sys.stderr)
        return 2
    except Failure as err:
        sys.stdout.write("".join(x + "\n" for x in out))
        print(f"hostbuild.py: {err}", file=sys.stderr)
        return 1
    sys.stdout.write("".join(x + "\n" for x in out))
    if args.list:
        sys.stdout.write("".join(x + "\n" for x in listing))
    return status


if __name__ == "__main__":
    sys.exit(main())
