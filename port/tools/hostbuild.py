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
units of the image that a `like` image is like are placed a second time (see
"The second placements").

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

The second placements
----------------------

An `[[image]]` with `like = "Y"` is image Y linked a second time at its own
address X (the second side of a character). The matching build links every
unit of Y again, each of its ranges moved by the shift, X's address minus
Y's, except the units that X's `leave_out` names; it gives that link every
other name at the address it has, and these names at a moved address: the
functions that the units of Y declare (the left-out units' too), and every
name of `symbols.ld` that lies inside Y's payload, unless X's
`[image.symbols]` table gives it. That table's names win over a moved
address, and it adds names. Everything else (the resident executable, the
other images, names outside the payload) keeps its address.

So here, for each unit of Y that is built (the nonmatching ones too) and
each such X: the second object is made from the same assembly as the first
(no second compile). Its definitions are `impl_NAME__X` (a data name that the
unit defines too), and its references are renamed by `redefine__X.txt` to
`ps1_NAME__X` for the names that move and to `ps1_NAME` for all others. The
names file gives `ps1_NAME__X` its moved address (or
`ps1_NAME__X = impl_NAME__X;` for a data copy). `port_functions` and
`port_absents` get X's functions as `NAME__X` at the moved addresses (the
inventory rows of X are read like the others'). A left-out unit has no second
object; its functions are absent in X and its names are still given; a
left-out unit that defines data is an error. A unit named like
`<other unit>__X` is an error. A second image must lie above its first.

The matching build bounds Y's payload by the length of the image's chunk,
which only the game's archive gives. The bound used here is the shift: a
second link must not overlap the first, so the payload is shorter than the
shift. A name of `symbols.ld` between the end of the payload and the shift
gets a moved address here and none in the matching build; that differs for
a program only if a unit of a `like` image refers to such a name. For the
tree of 2026-10-09 none did: that was one comparison made with the chunk
lengths of the game's archives, which this repository does not hold, and no
command here repeats it.

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
`port_program_sha256` holds the SHA-256 that `[baseline] sha256` of the
configuration pins for the game's executable (32 bytes; no game file is read
for it; a configuration without 64 hex digits there is an error).
`port_images` holds every image of the configuration in its order, with
the names of all the archives that carry its content (`contents.tsv`; the
configuration's own archive when that table is absent), which the runtime's
placing of modules (`modules.c`) matches against the disc's files.
`port_functions` holds every function with C: a function that a unit which
compiled declares and defines, and a nonmatching function that its file
defines; sorted by image (the resident executable first, as -1) and
address. Two at one address of one image is an error. `port_absents` holds
every function of the inventory (`ps1/inventory/game.tsv`, `library.tsv`
and, for every image of the configuration (the second placements too),
`modules.tsv`; a row of `modules.tsv` belongs to an image as
`coveragemap.py` assigns it) that no function with C is at: `library` is 1
for a row of `library.tsv`, the name is the one that a unit of the
configuration declares for that address in that image, else
`func_<address>` with `_<image>` for a module image. Sorted as above.

Some rows of the inventory are not functions: the static sweep took data in
front of a function for code, or split a function in two. The runtime would
write its stop call at such a row's address, into the data or the middle of a
function. Two things leave such a row out of `port_absents`, and nothing else
does: a row that is not covered by either keeps its stop, whatever lies near it
(a missing function directly before a later function with C, bytes that no
declared function owns in front of one).

The reviewed table, for data in front of a function with C. A row is left out
only when `port/sweep_rows.toml` (option `--sweep-rows`) lists it, and each entry
is verified against the tree first. The inventory must have a row at the entry's
image and address; the function it names must be a function with C of that image
(compiled in this build, at the stated address; for a second placement, the name
and moved address the tool gives it there; a `like` image's row is listed on its
own); the function's address minus the row's address is `data_bytes`, above zero,
the function lies strictly inside the row and no other function begins between;
when the entry has a `data_symbol`, that name of `symbols.ld` is at exactly the
row's address (the configuration then names the data's owner; an entry without
one rests on its written evidence alone). Any miss ends the build with status 1
and names the entry; a malformed table (a field missing or unknown, an entry
twice) is status 2; a missing table drops nothing.

Rule 2, for the tail of a split function. A row that begins inside the address
range of a unit that is built here (compiled; for a second placement the moved
range, unless the unit is left out for that image) and is not the address of a
function the unit declares is part of that unit and gets no stop. The range of a
unit is function coverage because the matching build's validator requires the
functions of a unit to be contiguous ("gap or overlap between functions" in
`parse_units` of `ps1/tools/matchbuild.py`). This tool relies on it, so it checks
it where it uses it: a unit whose declared functions are not contiguous gets no
rule 2; the tool says so on standard error and with `--list`.

The rows left out are counted (`sweep rows that are not functions`) and named
with `--list`: the table's rows with their evidence, rule 2's with the unit.

The link
--------

The link places two marker symbols, `port_game_text_begin` before and
`port_game_text_end` after the text of all the game's objects (the units'
objects, second placements and nonmatching ones included; not the runtime's,
`port_tables.o`, PsyZ or any library): two one-line assembly objects of the
tool, first and last among the game objects of the response file. The reason:
the port delivers the vertical-blank interrupt by interrupting the game's
thread, and may do so only inside the game's own code, which the runtime
tells by whether an instruction pointer lies between the markers. The check
below covers them.

All objects of the units, every `RUNTIME/*.c` compiled with
`CC -O1 -Wall -Wextra -c`, `port_tables.o`, the filler object (below) and
`names.ld` go to one run of the compiler by a response file, with `-static
-Wl,--large-address-aware -Wl,--disable-dynamicbase` and the settings of the
image (below), to `BUILD/NAME`. The link is then verified, and a miss ends
the tool with status 1 naming the symbols: the header of the linked file
says what the settings say (below); both markers exist and begin lies below end, every `impl_` function of the tables lies between them and no text symbol of the runtime's objects (`nm` on each) does; with `nm` on the
linked file, every `ps1_` name of `names.ld` has exactly its address, no plain game name is
at a PS1 address, every
`impl_` function of `port_functions` exists outside the PS1's ranges
(`0x80000000` to `0x801fffff`, `0x1f800000` to `0x1f8003ff`), every host
data alias `ps1_NAME` has the address of its `impl_` copy outside those
ranges. The plain name may exist (the host's own function of that name).

The image over the console's copy of RAM
-----------------------------------------

The console shows its RAM a second time at addresses 0 to 0x1fffff, and the
game's code reaches that view in ordinary play (the runtime's `mirror.c`
serves every access there as a fault). Windows gives a process nothing below
0x10000 and puts memory of its own into the rest of the range, where an access
would not fault. So the program's own image takes the range first. The link
settings: image base 0x10000 (`--image-base`), no relocations
(`--disable-reloc-section`, with `--disable-dynamicbase`), the first real section at 0x200000 (`--section-start=.text`).
Windows refuses an image whose sections leave a gap between them or after the
header, so the range between the header page and 0x200000 is filled by a
section of its own, `.hole` (`--section-start=.hole=0x11000`), uninitialized,
from an assembly object of the tool (`BUILD/gen/hole.s`, `BUILD/rt/hole.o`,
first in the response file). The system maps it readable and writable; the
runtime closes it, with the header page, at start (`mirror.c`).
The check reads the linked file's PE header: image base 0x10000, the flag
for stripped relocations and no relocation table, no dynamic base, section
alignment 0x1000, the first section `.hole` at 0x11000 with no bytes in the
file, the sections one after the other with no gap or overlap, the filler
reaching 0x200000 and every other section at 0x200000 or above, the headers
inside the first page. A miss ends the build with status 1.

The graphics library
--------------------

With `--psyz DIR` (DIR is a build folder of `psyzbuild.py`, which holds
`psyz.json`) the runtime's `gpu.c` is compiled with `-DPORT_HAVE_PSYZ`,
PsyZ's defines and `-isystem` its include folder, and the link line gets
PsyZ's libraries (its static library, SDL's and the system libraries, in the
order the file lists them) after the objects. No other runtime file sees
PsyZ's headers. Without the option `gpu.c` compiles to empty tables and the
program links nothing of PsyZ. A DIR without `psyz.json`, or one whose file
is not valid or names a file that is missing, is an error (status 2). With the
option the line `psyz: COMMIT` follows the `compiler:` line.

Output
------

Standard output, in this order, with nothing else:

    compiler: FIRST LINE OF `CC --version`
    units: U compiled, N of them nonmatching, F failed
    like images built: L
    functions with C: C
    functions without C: A, library L2, game and modules G
    sweep rows that are not functions: N
    names at PS1 addresses: P
    data defined in C, at host addresses: D
    linked: PATH, verified

With `--psyz` the line `psyz: COMMIT` follows `compiler:`. PATH is relative to the current folder when the linked file lies under it.

U counts the units tried, nonmatching ones included. A is the number of
rows of `port_absents`, L2 those of the library and G the others (the
resident game and the module images, without the rows that begin with data), so that A = L2 + G. L is the number of
`like` images placed a second time. The last line is
printed only for a link that was verified. With `--list` the names behind
F, D, the `like` images and the game rows of A follow, one per line:

    failed: UNIT: FIRST ERROR LINE
    data: NAME
    data-row: NAME at 0xADDRESS, IMAGE, table, function NAME2 at 0xADDRESS2: EVIDENCE
    data-row: NAME at 0xADDRESS, IMAGE, rule 2: inside the unit UNIT (0xSTART-0xEND)
    not-contiguous: unit UNIT: its functions are not contiguous; rows inside its range keep their stop
    like: IMAGE of FIRST, shift +0xSHIFT, N names move, units left out: UNIT ...
    absent: NAME 0xADDRESS IMAGE

(IMAGE is `-` for the resident executable). Objects are always rebuilt.

Exit status: 0 when everything built and verified; 1 when a unit failed, the
runtime or the link failed or the verification missed; 2 when an input is
missing or malformed, a name has two addresses, a definition is twice or the
compiler cannot be run, with one line on standard error that names it.

usage:
  hostbuild.py [--config BUILD_TOML] [--cc CC] [--nm NM] [--objcopy OBJCOPY] [--build DIR]
               [--out NAME] [--runtime DIR] [--sweep-rows FILE] [--psyz DIR] [--jobs N]
               [--timeout SECONDS] [--list]

The defaults: `ps1/src/build.toml`, found from the place of this script;
`i686-w64-mingw32-gcc`; the `nm` next to CC with the same prefix (`gcc` at
its end replaced by `nm`, else `nm`); `port/build/host`; `sfa2.exe`;
`port/src`; as many jobs as the machine has processors; 300 seconds.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
import tomllib
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
# The image holds the console's copy of RAM at address 0 (see "The link"): base 0x10000, no relocations, a filler
# section `.hole` from 0x11000 and the first real section at 0x200000.
IMAGE_BASE = 0x10000
HOLE_BEGIN = 0x11000
IMAGE_TOP = 0x200000
IMAGE_FLAGS = [
    f"-Wl,--image-base={IMAGE_BASE:#x}", "-Wl,--disable-reloc-section",
    f"-Wl,--section-start=.hole={HOLE_BEGIN:#x}", f"-Wl,--section-start=.text={IMAGE_TOP:#x}",
]
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase", *IMAGE_FLAGS]
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


def renamed(name: str, underscore: bool, suffix: str = "") -> str:
    if underscore and name.startswith("_"):
        return "_impl_" + name[1:] + suffix
    return "impl_" + name + suffix


def rename_definitions(text: str, underscore: bool, suffix: str = "") -> tuple[str, set[str]]:
    """The assembly with the definitions of its global symbols renamed, and the symbols defined.

    `suffix` is added to the new names (`impl_NAME` + suffix), for the second placement of a unit."""
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
    new = {name: renamed(name, underscore, suffix) for name in defined}
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
    size: int = 0


@dataclass
class Job:
    name: str
    source: Path
    nonmatching: bool
    functions: list[Function]
    image: str | None = None  # the image the unit belongs to; None: the resident executable


@dataclass
class Selection:
    jobs: list[Job]
    declared: list[Function]  # every function any unit of the configuration declares, nonmatching included
    like_images: int
    images: list[dict]
    unit_names: dict[str | None, list[str]] = field(default_factory=dict)  # every unit of the configuration, by image


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
        out.append(Function(item["name"], item["address"], image, entry["name"], item.get("size", 0) if isinstance(item.get("size", 0), int) else 0))
    return out


def select_units(config: dict, path: Path) -> Selection:
    """The jobs to compile, every declared function, and the count of images that are like another."""
    images = read_images(config, path)
    by_name = {i["name"]: i for i in images}
    units, _, _ = hostcheck.read_units(config, path)
    compiled = {u.name for u in units}
    jobs: list[Job] = []
    declared: list[Function] = []
    unit_names: dict[str | None, list[str]] = {}
    for entry in config.get("unit", []):
        image = entry.get("image")
        unit_names.setdefault(image, []).append(entry["name"])
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
        jobs.append(Job(u.name, u.path, False, read_functions(entry, path), entry.get("image")))
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
            jobs.append(Job(stem, file, True, [fn], image))
    return Selection(jobs, declared, sum(1 for i in images if "like" in i), images, unit_names)


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


# The second placements of `like` images.


@dataclass
class Placement:
    """The second link of the units of image `first` at the address of image `image`."""

    image: str
    first: str
    shift: int
    left_out: set[str]
    moved: dict[str, int]  # the names that move with the image (base names) and their addresses in it
    data: set[str] = field(default_factory=set)  # data that the units of `first` define: moves too, as a host copy

    @property
    def suffix(self) -> str:
        return "__" + self.image


def plan_placements(images: list[dict], declared: list[Function], symbols: list[tuple[str, int, str]],
                    unit_names: dict[str | None, list[str]]) -> list[Placement]:
    """One Placement for every image that is `like` another, in the order of the configuration.

    The names that move are the ones the matching build moves for a second link: every function that a unit of
    the first image declares (at its address plus the shift, the units that are left out among them), and every
    name of the symbol file whose address lies in the first image's payload (at its address plus the shift); the
    image's own `symbols` table then adds names and wins over a moved address. Every other name keeps its address.
    The matching build bounds the payload by the length of the image's chunk, which only the game's archive gives;
    here the bound is the shift (the second image's address minus the first's), which is at least as large as the
    payload, since a second link must not overlap the first. The two bounds give the same moved names for every
    name that the units of the 22 `like` images of the configuration refer to; see the controls and the report."""
    by_name = {i["name"]: i for i in images}
    out = []
    for image in images:
        if "like" not in image:
            continue
        first = by_name[image["like"]]
        if first.get("like") is not None or image["like"] == image["name"]:
            raise Problem(f"image {image['name']} is like {image['like']}, which is itself a second link or the image itself")
        shift = image["address"] - first["address"]
        if shift <= 0:
            raise Problem(f"image {image['name']} is like {first['name']} but lies at or below it; only a second link above the first is supported")
        own = image.get("symbols", {})
        if not isinstance(own, dict) or not all(isinstance(k, str) and isinstance(v, int) for k, v in own.items()):
            raise Problem(f"image {image['name']}: [image.symbols] needs names and integer addresses")
        left = image.get("leave_out", [])
        known = set(unit_names.get(first["name"], []))
        for name in left:
            if name not in known:
                raise Problem(f"image {image['name']}: leave_out names {name}, which is not a unit of image {first['name']}")
        moved: dict[str, int] = {}
        for fn in declared:
            if fn.image == first["name"]:
                moved[fn.name] = fn.address + shift
        for name, address, _ in symbols:
            if first["address"] <= address < first["address"] + shift:
                moved[name] = address + shift
        moved.update(own)
        out.append(Placement(image["name"], first["name"], shift, set(left), moved))
    return out


def render_names(names: dict[str, int], aliases: list[str], underscore: bool) -> str:
    """The names file: it defines `ps1_NAME`, never the plain name."""
    us = "_" if underscore else ""
    lines = [f"{us}ps1_{n} = 0x{names[n]:08x};\n" for n in sorted(names)]
    lines += [f"{us}ps1_{n} = {us}impl_{n};\n" for n in sorted(aliases)]
    return "".join(lines)


def render_redefine(names, aliases: list[str], underscore: bool, moved=(), suffix: str = "") -> str:
    """The file for `objcopy --redefine-syms`: `NAME ps1_NAME` once for every name of the names file.

    For a second placement `moved` holds the names that move with the image: their line is `NAME ps1_NAME` + suffix."""
    us = "_" if underscore else ""
    moved = set(moved)
    return "".join(f"{us}{n} {us}ps1_{n}{suffix if n in moved else ''}\n" for n in sorted(set(names) | set(aliases)))


# The tables.


@dataclass
class Row:
    address: int
    image: str | None
    library: bool
    size: int = 0


def read_inventory(directory: Path, config: dict) -> list[Row]:
    """The rows of the inventory that the build is about: game, library and every image, the second placements too."""
    try:
        rows = []
        for block in coveragemap.read_resident(directory):
            rows += [Row(f.address, None, block.key != coveragemap.GAME, f.size) for f in block.functions]
        modules = coveragemap.read_modules(directory)
        coveragemap.assign_images(modules, config)
    except coveragemap.Problem as err:
        raise Problem(str(err))
    except OSError as err:
        raise Problem(f"cannot read the inventory in {directory}: {err.strerror}")
    for block in modules:
        if block.image:
            rows += [Row(f.address, block.image, False, f.size) for f in block.functions]
    return rows


def read_image_archives(directory: Path, config: dict) -> dict[str, list[str]]:
    """Per image of the configuration, the names of all the archives that carry its content (`contents.tsv`;
    without that table, the archive the configuration names), for the runtime's placing of modules."""
    try:
        modules = coveragemap.read_modules(directory)
        coveragemap.assign_images(modules, config)
        carriers = coveragemap.read_contents(directory)
    except coveragemap.Problem as err:
        raise Problem(str(err))
    except OSError as err:
        raise Problem(f"cannot read the inventory in {directory}: {err.strerror}")
    out: dict[str, list[str]] = {}
    for block in modules:
        if not block.image:
            continue
        slot_text, first = block.key.split("/", 1)
        out[block.image] = list(carriers[(int(slot_text, 16), first)]) if carriers is not None else [first]
    return out


def default_name(address: int, image: str | None) -> str:
    return f"func_{address:08x}" + (f"_{image}" if image else "")


def order_key(image: str | None, address: int, index: dict[str | None, int]):
    return index[image], address


@dataclass
class SweepEntry:
    """One entry of the reviewed table of inventory rows that are not functions (`port/sweep_rows.toml`)."""

    image: str | None  # None: the resident executable
    address: int
    function: str
    function_address: int
    data_bytes: int
    evidence: str
    data_symbol: str | None = None  # a name of symbols.ld at exactly the row's address, when the configuration names the data


@dataclass
class Span:
    """The text range of a unit that is built, in the image it is placed in (moved for a second placement)."""

    unit: str
    image: str | None
    start: int
    end: int
    declared: set[int]  # the addresses of the functions the unit declares


def contiguous(functions: list[Function]) -> bool:
    """Whether the functions of a unit, by address, follow each other with no gap and no overlap, as the matching
    build's validator (`matchbuild.py`, "gap or overlap between functions") requires of every unit."""
    ordered = sorted(functions, key=lambda f: f.address)
    return all(a.address + a.size == b.address for a, b in zip(ordered, ordered[1:]))


def split_span_rows(rows: list[Row], spans: list[Span]) -> tuple[list[Row], list[tuple[Row, Span]]]:
    """(the rows to keep, the rows that begin inside a unit's range without being one of its functions, with that unit).

    The range of a unit is function coverage because the functions of a unit are contiguous (see `contiguous`)."""
    inside: dict[str | None, list[Span]] = {}
    for span in spans:
        inside.setdefault(span.image, []).append(span)
    kept, dropped = [], []
    for row in rows:
        owner = next((sp for sp in inside.get(row.image, []) if sp.start <= row.address < sp.end and row.address not in sp.declared), None)
        if owner is None:
            kept.append(row)
        else:
            dropped.append((row, owner))
    return kept, dropped


def read_sweep_rows(path: Path | None) -> list[SweepEntry]:
    """The entries of the reviewed table; none when the file does not exist. A malformed table is an error."""
    if path is None or not path.is_file():
        return []
    try:
        with open(path, "rb") as handle:
            table = tomllib.load(handle)
    except (OSError, tomllib.TOMLDecodeError) as err:
        raise Problem(f"cannot read {path}: {err}")
    rows = table.get("row", [])
    if not isinstance(rows, list):
        raise Problem(f"{path}: `row` is not an array of tables")
    out: list[SweepEntry] = []
    seen: set[tuple[str | None, int]] = set()
    for number, raw in enumerate(rows, 1):
        where = f"{path}: entry {number}"
        if not isinstance(raw, dict):
            raise Problem(f"{where} is not a table")
        for key in ("image", "address", "function", "function_address", "data_bytes", "evidence"):
            if key not in raw:
                raise Problem(f"{where} has no `{key}`")
        unknown = sorted(set(raw) - {"image", "address", "function", "function_address", "data_bytes", "evidence", "data_symbol"})
        if unknown:
            raise Problem(f"{where} has an unknown field `{unknown[0]}`")
        types_ok = (isinstance(raw["image"], str) and isinstance(raw["address"], int) and isinstance(raw["function"], str)
                    and isinstance(raw["function_address"], int) and isinstance(raw["evidence"], str) and raw["evidence"].strip()
                    and isinstance(raw["data_bytes"], int) and isinstance(raw.get("data_symbol", ""), str))
        if not types_ok:
            raise Problem(f"{where} has a field of the wrong type or an empty evidence")
        image = None if raw["image"] == "resident" else raw["image"]
        key = (image, raw["address"])
        if key in seen:
            raise Problem(f"{where}: {raw['image']} {raw['address']:#x} is listed twice")
        seen.add(key)
        out.append(SweepEntry(image, raw["address"], raw["function"], raw["function_address"], raw["data_bytes"], raw["evidence"].strip(), raw.get("data_symbol")))
    return out


def verify_sweep_rows(entries: list[SweepEntry], rows: list[Row], with_c: list[Function], declared: list[Function],
                      symbols: dict[str, int] | None = None) -> tuple[list[Row], list[str]]:
    """(the rows to keep, the misses). A row is dropped only when the table lists it and the entry is verified against
    the tree; an entry that cannot be verified is a miss and drops nothing. `symbols` are the names of the symbol file."""
    by_key = {(r.image, r.address): r for r in rows}
    misses: list[str] = []
    dropped: set[tuple[str | None, int]] = set()
    for e in entries:
        name = f"{e.image or 'resident'} {e.address:#x}"
        row = by_key.get((e.image, e.address))
        if row is None:
            misses.append(f"{name}: the inventory has no row at this address in this image")
            continue
        fns = [f for f in with_c if f.image == e.image and f.name == e.function]
        if not fns:
            misses.append(f"{name}: {e.function} is not a function with C in this image")
            continue
        fn = fns[0]
        if fn.address != e.function_address:
            misses.append(f"{name}: {e.function} is at {fn.address:#x}, not at {e.function_address:#x}")
            continue
        if e.data_bytes <= 0 or e.function_address - e.address != e.data_bytes:
            misses.append(f"{name}: data_bytes {e.data_bytes} is not the distance {e.function_address - e.address} to the function")
            continue
        if not e.address < e.function_address < e.address + row.size:
            misses.append(f"{name}: {e.function} at {e.function_address:#x} does not lie strictly inside the row")
            continue
        between = [a for a in (f.address for f in declared if f.image == e.image and f.address != fn.address) if e.address <= a < e.function_address]
        if between:
            misses.append(f"{name}: another function begins at {between[0]:#x}, between the row and {e.function}")
            continue
        if e.data_symbol is not None and (symbols or {}).get(e.data_symbol) != e.address:
            found = (symbols or {}).get(e.data_symbol)
            misses.append(f"{name}: data_symbol {e.data_symbol} is " + ("not in the symbol file" if found is None else f"at {found:#x}, not at the row"))
            continue
        dropped.add((e.image, e.address))
    return [r for r in rows if (r.image, r.address) not in dropped], misses


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


def read_baseline_hash(config: dict, path: Path) -> str:
    """The SHA-256 that the configuration pins for the game's executable, as 64 lower-case hex digits."""
    baseline = config.get("baseline")
    value = baseline.get("sha256") if isinstance(baseline, dict) else None
    if not isinstance(value, str) or not re.fullmatch(r"[0-9a-fA-F]{64}", value):
        raise Problem(f"{path}: [baseline] sha256 is missing or is not 64 hex digits")
    return value.lower()


def render_tables(images: list[dict], functions: list[tuple], absents: list[tuple], sha256: str, archives: dict[str, list[str]] | None = None) -> str:
    """`archives` (image name to archive names) adds the archive list of each image that has one."""
    archives = archives or {}
    out = ['/* Written by hostbuild.py; not to be edited. */\n', '#include "port_tables.h"\n\n']
    for impl in dict.fromkeys(f[2] for f in functions):
        out.append(f"extern void {impl}(void);\n")
    out.append("\n")
    for n, i in enumerate(images):
        if i["name"] in archives:
            out.append(f"static const char *const port_archives_{n}[] = {{ " + "".join(c_string(a) + ", " for a in archives[i["name"]]) + "0 };\n")
    if archives:
        out.append("\n")

    def table(kind: str, name: str, rows: list[str], zero: str):
        out.append(f"const struct {kind} {name}[] = {{\n")
        out.extend(f"    {r},\n" for r in rows)
        if not rows:
            out.append(f"    {zero},\n")
        out.append("};\n")
        out.append(f"const unsigned {kind}_count = {len(rows)};\n\n")

    table("port_image", "port_images", [
        "{ %s, 0x%08xu, %s, 0x%xu%s }" % (c_string(i["name"]), i["address"], c_string(i["like"]) if "like" in i else "0", i["slot"],
                                          f", port_archives_{n}" if i["name"] in archives else "")
        for n, i in enumerate(images)
    ], "{ 0, 0, 0, 0 }")
    table("port_function", "port_functions", [
        "{ 0x%08xu, (void *)%s, %s, %d }" % (f[1], f[2], c_string(f[3]), f[0]) for f in functions
    ], "{ 0, 0, 0, 0 }")
    table("port_absent", "port_absents", [
        "{ 0x%08xu, %s, %d, %d }" % (a[1], c_string(a[2]), a[0], a[3]) for a in absents
    ], "{ 0, 0, 0, 0 }")
    digest = bytes.fromhex(sha256)
    out.append("const unsigned char port_program_sha256[32] = {\n")
    for i in range(0, 32, 8):
        out.append("    " + ", ".join("0x%02x" % b for b in digest[i:i + 8]) + ",\n")
    out.append("};\n")
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


MARKERS = ("port_game_text_begin", "port_game_text_end")


def marker_source(name: str, underscore: bool) -> str:
    """The assembly of one marker: a global label in the text section."""
    sym = ("_" if underscore else "") + name
    return f"\t.text\n\t.globl\t{sym}\n{sym}:\n"


def hole_source() -> str:
    """The assembly of the filler section: uninitialized, from HOLE_BEGIN to IMAGE_TOP (its address is the link flag's)."""
    return f'\t.section\t.hole,"b"\n\t.space\t{IMAGE_TOP - HOLE_BEGIN:#x}\n'


def verify_image(data: bytes) -> list[str]:
    """The misses of the linked file's PE header against the link settings; empty when none.

    Image base 0x10000, no relocations (the flag, no table), not dynamic base; the first section is `.hole`
    at 0x11000, uninitialized (no bytes in the file); the sections follow one another with no gap (Windows
    refuses an image with one); the first real section lies at 0x200000 or above; the headers fit the page
    that is the image's first, so that nothing of the range 0x10000..0x1fffff is outside the image."""
    def u16(o: int) -> int:
        return int.from_bytes(data[o:o + 2], "little")

    def u32(o: int) -> int:
        return int.from_bytes(data[o:o + 4], "little")

    if len(data) < 0x40 or data[:2] != b"MZ":
        return ["the file has no DOS header"]
    pe = u32(0x3C)
    if pe + 24 + 96 > len(data) or data[pe:pe + 4] != b"PE\0\0":
        return ["the file has no PE header"]
    count, optsize, chars = u16(pe + 6), u16(pe + 20), u16(pe + 22)
    opt = pe + 24
    if u16(opt) != 0x10B or optsize < 96 + 8 * 16:
        return ["the file is not a 32-bit PE image"]
    misses = []
    base, align, headers, dll = u32(opt + 28), u32(opt + 32), u32(opt + 60), u16(opt + 70)
    if base != IMAGE_BASE:
        misses.append(f"image base {base:#x}, wanted {IMAGE_BASE:#x}")
    if not chars & 1:
        misses.append("relocations are not stripped (flag)")
    if u32(opt + 96 + 8 * 5) or u32(opt + 96 + 8 * 5 + 4):
        misses.append("the image has a relocation table")
    if dll & 0x40:
        misses.append("the image is relocatable (dynamic base)")
    if align != 0x1000:
        misses.append(f"section alignment {align:#x}, wanted 0x1000")
    if headers > HOLE_BEGIN - IMAGE_BASE:
        misses.append(f"the headers ({headers:#x} bytes) do not fit the first page")
    table = opt + optsize
    if count < 2 or table + 40 * count > len(data):
        return misses + [f"{count} sections: the filler and the program's own are needed"]
    rows = []
    for i in range(count):
        o = table + 40 * i
        name = data[o:o + 8].rstrip(b"\0").decode("ascii", "replace")
        rows.append((name, base + u32(o + 12), u32(o + 8), u32(o + 16), u32(o + 36)))
    name, begin, size, raw, flags = rows[0]
    if name != ".hole":
        misses.append(f"the first section is {name}, wanted .hole")
    if begin != HOLE_BEGIN:
        misses.append(f"the first section begins at {begin:#x}, wanted {HOLE_BEGIN:#x}")
    if raw or not flags & 0x80:
        misses.append("the filler section is not uninitialized")
    for (n1, b1, s1, _, _), (n2, b2, _, _, _) in zip(rows, rows[1:]):
        if b2 != b1 + (s1 + align - 1) // align * align:
            misses.append(f"a gap or an overlap between {n1} ({b1:#x}, {s1:#x} bytes) and {n2} ({b2:#x})")
    if begin + (size + align - 1) // align * align < IMAGE_TOP:
        misses.append(f"the filler ends at {begin + (size + align - 1) // align * align:#x}, below {IMAGE_TOP:#x}")
    for n, b, _, _, _ in rows[1:]:
        if b < IMAGE_TOP:
            misses.append(f"section {n} begins at {b:#x}, below {IMAGE_TOP:#x}")
    return misses


def text_symbols(nm_text: str) -> list[str]:
    """The names of the text symbols (type T or t) in the text of `nm` for one object; the section symbols (`.text`) are no function."""
    out = []
    for line in nm_text.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in ("T", "t") and not parts[2].startswith("."):
            out.append(parts[2])
    return out


def verify_markers(nm_text: str, impls: list[str], runtime_symbols: list[str], underscore: bool) -> list[str]:
    """The misses of the game-code markers in the linked file; empty when none.

    Both markers exist, begin lies below end, every `impl_` function lies between them, and no text symbol
    of the runtime's own objects does."""
    us = "_" if underscore else ""
    seen: dict[str, list[int]] = {}
    for line in nm_text.splitlines():
        match = NM_LINE.match(line.strip())
        if match:
            seen.setdefault(match.group(3), []).append(int(match.group(1), 16))
    begin, end = (seen.get(us + m) for m in MARKERS)
    misses = []
    for marker, found in zip(MARKERS, (begin, end)):
        if not found:
            misses.append(f"{marker}: not in the linked file")
    if misses:
        return misses
    low, high = begin[0], end[0]
    if len(begin) > 1 or len(end) > 1:
        misses.append("a marker is defined twice")
    if not low < high:
        return misses + [f"port_game_text_begin ({low:#x}) is not below port_game_text_end ({high:#x})"]
    for name in sorted(impls):
        for a in seen.get(us + "impl_" + name, []):
            if not low <= a < high:
                misses.append(f"impl_{name}: at {a:#x}, outside the game's code {low:#x}-{high:#x}")
    for name in sorted(set(runtime_symbols)):
        for a in seen.get(name, []):
            if low <= a < high:
                misses.append(f"{name}: a runtime symbol at {a:#x}, inside the game's code {low:#x}-{high:#x}")
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
    seconds: list[Placement] = field(default_factory=list)  # the second placements that were assembled, as `<unit>__<image>.o`


def build_unit(job: Job, cc: str, build: Path, underscore: bool, timeout: int, placements: list[Placement] = ()) -> Outcome:
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
    # The second placements are made from the same assembly: its definitions get another suffix.
    for place in placements:
        if place.first != job.image or job.name in place.left_out:
            continue
        second, again = rename_definitions(text, underscore, place.suffix)
        left = labels_left(second, again)
        if left:
            raise Problem(f"the rename of {job.name} for {place.image} left these defined names as labels: {' '.join(left)}")
        name = f"{job.name}{place.suffix}"
        asm2, obj2 = build / "asm" / f"{name}.s", build / "obj" / f"{name}.o"
        obj2.unlink(missing_ok=True)
        try:
            with open(asm2, "w", encoding="utf-8", errors="surrogateescape", newline="") as handle:
                handle.write(second)
        except OSError as err:
            raise Problem(f"cannot write {asm2}: {err.strerror}")
        proc = hostcheck.compile_run([cc, "-c", "-o", str(obj2), str(asm2)], timeout)
        if proc is None or proc.returncode != 0:
            out.ok = False
            out.reason = f"assembler for {place.image}: " + (first_error(proc.stderr) if proc else f"no end after {timeout} seconds")
            return out
        out.seconds.append(place)
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


def redefine(job: str, objcopy: str, build: Path, timeout: int, table: str = "redefine.txt") -> str:
    """Rename the references of one object into the `ps1_` namespace; empty when it worked, else the reason."""
    obj = build / "obj" / f"{job}.o"
    proc = hostcheck.compile_run([objcopy, f"--redefine-syms={build / 'gen' / table}", str(obj)], timeout)
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


def read_psyz(folder: Path) -> tuple[dict, str]:
    """The description of a psyzbuild.py build folder: (the parsed psyz.json, its commit)."""
    path = folder / "psyz.json"
    if not path.is_file():
        raise Problem(f"{path} does not exist; build PsyZ first with port/tools/psyzbuild.py")
    try:
        info = json.loads(path.read_text())
        include, defines, link = info["include"], info["define"], info["link"]
        ok = isinstance(include, str) and isinstance(defines, list) and isinstance(link, list)
    except (OSError, ValueError, KeyError, TypeError):
        ok = False
    if not ok:
        raise Problem(f"{path} is not a psyz.json (it needs include, define and link)")
    if not Path(include).is_dir():
        raise Problem(f"{path} names the include folder {include}, which does not exist")
    for item in link:
        if not str(item).startswith("-") and not Path(item).is_file():
            raise Problem(f"{path} names the library {item}, which does not exist")
    return info, str(info.get("commit", "unknown"))


def run(args: argparse.Namespace, out: list[str], listing: list[str]) -> int:
    config_path: Path = args.config
    config = hostcheck.read_config(config_path)
    fields_path, header = hostcheck.read_types(config, config_path)
    baseline_hash = read_baseline_hash(config, config_path)
    selection = select_units(config, config_path)
    model = hostcheck.read_model(fields_path)
    symbols = read_symbols(read_text(config_path.parent / "symbols.ld"))
    runtime: Path = args.runtime
    if not (runtime / "port_tables.h").is_file():
        raise Problem(f"{runtime / 'port_tables.h'} does not exist")
    runtime_sources = sorted(runtime.glob("*.c"))
    sweep_entries = read_sweep_rows(args.sweep_rows)
    inventory = read_inventory(config_path.parent.parent / "inventory", config)
    image_archives = read_image_archives(config_path.parent.parent / "inventory", config)

    # Names that can be worked out before any compiler runs.
    entries = list(symbols) + [(fn.name, fn.address, f"unit {fn.unit}") for fn in selection.declared]
    function_names = {fn.name for fn in selection.declared}
    plain = merge_names(entries)
    placements = plan_placements(selection.images, selection.declared, symbols, selection.unit_names)
    for job in selection.jobs:
        for place in placements:
            if job.name.endswith(place.suffix):
                raise Problem(f"unit {job.name} has a name that ends like the second placement {place.suffix}")
    # The functions of the second placements: the names of the first image's functions, at their moved addresses.
    moved_functions = [
        Function(fn.name + pl.suffix, pl.moved[fn.name], pl.image, fn.unit, fn.size)
        for pl in placements for fn in selection.declared if fn.image == pl.first
    ]
    names = merge_names(entries + [(f.name, f.address, f"unit {f.unit} placed in {f.image}") for f in moved_functions]
                        + [(n + pl.suffix, a, f"{pl.image} placed") for pl in placements for n, a in pl.moved.items()])

    psyz = read_psyz(args.psyz) if args.psyz else None
    version = hostcheck.compiler_version(args.cc, args.timeout)
    out.append(f"compiler: {version}")
    if psyz:
        out.append(f"psyz: {psyz[1]}")
    build: Path = args.build
    for sub in ("gen", "asm", "obj", "rt", "probe"):
        (build / sub).mkdir(parents=True, exist_ok=True)
    write_text(build / "gen" / header, structgen.generate_header(model, Path(header).name))
    underscore = detect_underscore(args.cc, build / "probe", args.timeout)

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [pool.submit(build_unit, j, args.cc, build, underscore, args.timeout, placements) for j in selection.jobs]
        outcomes = sorted((f.result() for f in futures), key=lambda o: o.job.name)
    failed = [o for o in outcomes if not o.ok]
    write_text(build / "failed.tsv", "".join(f"{o.job.name}\t{o.reason}\n" for o in failed))
    listing.extend(f"failed: {o.job.name}: {o.reason}" for o in failed)
    out.append(f"units: {len(outcomes)} compiled, {sum(1 for o in outcomes if o.job.nonmatching)} of them nonmatching, {len(failed)} failed")
    out.append(f"like images built: {len(placements)}")
    listing.extend(
        f"like: {pl.image} of {pl.first}, shift {pl.shift:+#x}, {len(pl.moved)} names move, units left out: {' '.join(sorted(pl.left_out)) or '-'}"
        for pl in placements
    )

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
                for pl in o.seconds:
                    with_c.append((Function(fn.name + pl.suffix, pl.moved[fn.name], pl.image, fn.unit, fn.size), "impl_" + fn.name + pl.suffix))
    kept_rows, misses = verify_sweep_rows(sweep_entries, inventory, [fn for fn, _ in with_c], selection.declared + moved_functions, {n: a for n, a, _ in symbols})
    if misses:
        raise Failure("the table of sweep rows does not verify:\n" + "\n".join(misses))
    spans: list[Span] = []
    for o in outcomes:
        if not o.ok or o.job.nonmatching or not o.job.functions:
            continue
        if not contiguous(o.job.functions):
            note = f"unit {o.job.name}: its functions are not contiguous; rows inside its range keep their stop"
            print(f"hostbuild.py: {note}", file=sys.stderr)
            listing.append(f"not-contiguous: {note}")
            continue
        fns = o.job.functions
        start, end = min(f.address for f in fns), max(f.address + f.size for f in fns)
        spans.append(Span(o.job.name, o.job.image, start, end, {f.address for f in fns}))
        for pl in o.seconds:
            spans.append(Span(o.job.name, pl.image, start + pl.shift, end + pl.shift, {pl.moved[f.name] for f in fns}))
    kept_rows, span_rows = split_span_rows(kept_rows, spans)
    functions, absents = build_tables(selection.images, with_c, kept_rows, selection.declared + moved_functions)
    out.append(f"functions with C: {len(functions)}")
    library = sum(1 for a in absents if a[3])
    out.append(f"functions without C: {len(absents)}, library {library}, game and modules {len(absents) - library}")
    out.append(f"sweep rows that are not functions: {len(sweep_entries) + len(span_rows)}")
    images_by_index = [i["name"] for i in selection.images]
    declared_names = {(f.image, f.address): f.name for f in reversed(selection.declared + moved_functions)}
    listing.extend(
        f"absent: {a[2]} {a[1]:#x} {images_by_index[a[0]] if a[0] >= 0 else '-'}" for a in absents if not a[3]
    )
    listing.extend(
        f"data-row: {declared_names.get((e.image, e.address)) or default_name(e.address, e.image)} at {e.address:#x}, {e.image or '-'}, "
        f"table, function {e.function} at {e.function_address:#x}: {e.evidence}"
        for e in sweep_entries
    )
    listing.extend(
        f"data-row: {declared_names.get((r.image, r.address)) or default_name(r.address, r.image)} at {r.address:#x}, {r.image or '-'}, "
        f"rule 2: inside the unit {sp.unit} ({sp.start:#x}-{sp.end:#x})"
        for r, sp in span_rows
    )

    symbol_names = {s[0] for s in symbols}
    aliases = sorted(n for n in defined_all if n not in function_names and n not in symbol_names)
    second_aliases: list[str] = []
    for o in outcomes:
        data = {n for n in o.defined if n not in function_names and n not in symbol_names}
        for pl in placements:
            if pl.first != o.job.image:
                continue
            if o.job.name in pl.left_out:
                if data:
                    raise Problem(f"unit {o.job.name} defines data ({' '.join(sorted(data))}) and is left out of image {pl.image}; not supported")
            elif o.ok:
                pl.data |= data
                second_aliases.extend(n + pl.suffix for n in sorted(data))
    out.append(f"names at PS1 addresses: {len(names)}")
    out.append(f"data defined in C, at host addresses: {len(aliases)}")
    listing.extend(f"data: {n}" for n in aliases)
    names_path = build / "gen" / "names.ld"
    all_aliases = aliases + second_aliases
    write_text(names_path, render_names(names, all_aliases, underscore))
    write_text(build / "gen" / "redefine.txt", render_redefine(plain, aliases, underscore))
    for pl in placements:
        write_text(build / "gen" / f"redefine{pl.suffix}.txt",
                   render_redefine(plain, aliases, underscore, moved=set(pl.moved) | pl.data, suffix=pl.suffix))
    objcopy = args.objcopy or default_objcopy(args.cc)
    todo = [(o.job.name, "redefine.txt") for o in outcomes if o.ok]
    todo += [(f"{o.job.name}{pl.suffix}", f"redefine{pl.suffix}.txt") for o in outcomes if o.ok for pl in o.seconds]
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        reasons = list(pool.map(lambda t: redefine(t[0], objcopy, build, args.timeout, t[1]), todo))
    broken = [(t[0], r) for t, r in zip(todo, reasons) if r]
    if broken:
        raise Failure("\n".join(f"{n}: {r}" for n, r in broken[:20]))
    tables_path = build / "gen" / "port_tables.c"
    write_text(tables_path, render_tables(selection.images, functions, absents, baseline_hash, image_archives))

    # The runtime, the tables and the link.
    objects = sorted([o.job.name for o in outcomes if o.ok] + [f"{o.job.name}{pl.suffix}" for o in outcomes if o.ok for pl in o.seconds])
    rt_objects: list[Path] = []
    for source in [*runtime_sources, tables_path]:
        obj = build / "rt" / f"{source.stem}.o"
        obj.unlink(missing_ok=True)
        extra: list[str] = []
        if psyz and source.name == "gpu.c":
            extra = ["-DPORT_HAVE_PSYZ", *(f"-D{d}" for d in psyz[0]["define"]), "-isystem", psyz[0]["include"]]
        proc = hostcheck.compile_run([args.cc, "-O1", "-Wall", "-Wextra", "-c", *extra, "-I", str(runtime), "-o", str(obj), str(source)], args.timeout)
        if proc is None or proc.returncode != 0:
            raise Failure(f"{source}: " + (first_error(proc.stderr) if proc else f"no end after {args.timeout} seconds"))
        rt_objects.append(obj)
    marks = []
    for marker in MARKERS:
        asm, obj = build / "gen" / f"{marker}.s", build / "rt" / f"{marker}.o"
        write_text(asm, marker_source(marker, underscore))
        obj.unlink(missing_ok=True)
        proc = hostcheck.compile_run([args.cc, "-c", "-o", str(obj), str(asm)], args.timeout)
        if proc is None or proc.returncode != 0:
            raise Failure(f"{asm}: " + (first_error(proc.stderr) if proc else f"no end after {args.timeout} seconds"))
        marks.append(obj)
    hole_asm, hole_obj = build / "gen" / "hole.s", build / "rt" / "hole.o"
    write_text(hole_asm, hole_source())
    hole_obj.unlink(missing_ok=True)
    proc = hostcheck.compile_run([args.cc, "-c", "-o", str(hole_obj), str(hole_asm)], args.timeout)
    if proc is None or proc.returncode != 0:
        raise Failure(f"{hole_asm}: " + (first_error(proc.stderr) if proc else f"no end after {args.timeout} seconds"))
    response = build / "link.rsp"
    write_text(response, "".join(quote(p) + "\n" for p in [hole_obj, marks[0], *(build / "obj" / f"{n}.o" for n in objects), marks[1], *rt_objects, names_path]))
    exe = build / args.out
    exe.unlink(missing_ok=True)
    libraries = [str(x) for x in psyz[0]["link"]] if psyz else []
    proc = hostcheck.compile_run([args.cc, f"@{response}", *LINK_FLAGS, *libraries, "-o", str(exe)], args.timeout)
    if proc is None or proc.returncode != 0 or not exe.is_file():
        raise Failure("link: " + (link_errors(proc.stderr) if proc else f"no end after {args.timeout} seconds"))
    nm = hostcheck.compile_run([args.nm or default_nm(args.cc), str(exe)], args.timeout)
    if nm is None or nm.returncode != 0:
        raise Failure("nm did not run on the linked file")
    runtime_symbols: list[str] = []
    for obj in rt_objects[:-1]:  # the runtime's objects, not port_tables.o (the last)
        listing_nm = hostcheck.compile_run([args.nm or default_nm(args.cc), str(obj)], args.timeout)
        if listing_nm is None or listing_nm.returncode != 0:
            raise Failure(f"nm did not run on {obj}")
        runtime_symbols += text_symbols(listing_nm.stdout)
    misses = verify_image(exe.read_bytes())
    misses += verify_link(nm.stdout, names, all_aliases, [f[3] for f in functions], underscore)
    misses += verify_markers(nm.stdout, [f[3] for f in functions], runtime_symbols, underscore)
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
    parser.add_argument("--sweep-rows", type=Path, default=REPO / "port/sweep_rows.toml")
    parser.add_argument("--psyz", type=Path, default=None)
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
