#!/usr/bin/env python3
"""Compile the game's C units for the machine this runs on and count what does not carry over.

Reads the build configuration, the field table of the shared structs and
the sources of the units, and runs the C compiler of the host. Nothing is
linked, nothing is run and no game file is read. Python 3.11 or later,
standard library only, and the struct generator of this repository,
`ps1/tools/structgen.py`, used as a module and not changed.

The units
---------

Every `[[unit]]` of the build configuration whose `source` ends in `.c` and
does not begin with `sdk/`. Units under `sdk/` are Sony's library, which a
port swaps; they and the units whose source is not C are left out and
counted, a unit under `sdk/` as that whatever its source ends in. The
`flags` of a unit are for the PS1 compiler and are not used. Two units
that are compiled and have one name are an error: their objects would be
one file.

The header of the shared structs is written to `BUILD/gen/NAME`, with NAME
from `[types] header` of the configuration and the text that
`structgen.generate_header` gives for the field table.

Each unit is compiled alone:

    CC -std=STD -c -I BUILD/gen -o BUILD/obj/UNIT.o SOURCE

with UNIT the unit's `name`. A unit passes when the compiler ends with
status 0. A compiler that has not ended after `--timeout` seconds is
stopped and the unit fails, without diagnostics. The same limit holds
for every other run of the compiler: a struct whose check is stopped
loses its layout, and a pointer size that cannot be found is an error.

A diagnostic is a line of the compiler's standard error of the form
`WHERE: warning: TEXT` or `WHERE: error: TEXT`. Its kind is the level and
the option that the compiler names in square brackets at the end of the
text, as in `[-Wint-conversion]`, or `(no option)`. Lines of any other
form, notes among them, are not counted.

The structs
-----------

For every struct of the field table one source is compiled with
`-fsyntax-only` that includes the header and is valid only when the size of
the struct and the offset of each of its fields are, on the host, what the
table says. The struct keeps its layout when the compiler ends with status
0 and loses it otherwise. A field that is an array is checked at its first
element.

Apart from the compiler, the table says which structs have a pointer: a
field whose type ends in `*`, or a field whose type is a struct that has
one. On a host whose pointers are not four bytes these are the structs
that are expected to lose their layout, and the two counts are printed
side by side so that a struct that loses it for another reason shows.

Fixed addresses
---------------

In the source of each unit, after comments and string and character
literals are taken out, every integer literal, in any base and with any
suffix, whose value lies in one of these ranges is counted:

    main memory            0x80000000 to 0x801fffff
    main memory, uncached  0xa0000000 to 0xa01fffff
    scratchpad             0x1f800000 to 0x1f8003ff
    ports                  0x1f801000 to 0x1f802fff
    BIOS                   0xbfc00000 to 0xbfc7ffff

A literal in a range is counted whether the code uses it as an address or
not: `0x80000000` is also the sign bit of a word.

Output
------

Standard output, in this order, with nothing else:

    compiler: FIRST LINE OF `CC --version`
    language level: STD
    pointer size: N bytes
    units: N compiled, N under sdk/ and N not C left out
    passed: N
    failed: N
    LEVEL OPTION: N in N units
    ...
    structs: N of N keep their layout
    structs with a pointer: N, of which N lose their layout
    structs without a pointer: N, of which N lose their layout
    fixed addresses: N literals in N units
    RANGE: N literals in N units
    ...
    failed: UNIT UNIT ...

The pointer size is found by compiling: no program is run for it. One
`LEVEL OPTION` line per kind of diagnostic, with the number of
diagnostics and of units that have one, in order of the number of
diagnostics, largest first, then of the text of the whole line. One RANGE line
for each of the five ranges, in the order above, with its name as above.
The last line names the units that failed, in order of name, and is left
out when none did.

Two tables are written to BUILD, one line per item, in order of name:

- `units.tsv`: unit, source, `passed` or `failed`, number of warnings,
  number of errors, number of literals at fixed addresses.
- `structs.tsv`: struct, size by the table, `kept` or `lost`, `pointer` or
  `plain`.

Exit status: 0 when the run was made, whatever it found; 2 when an input
is missing or malformed or the compiler cannot be run, with one line on
standard error that names it.

usage:
  hostcheck.py [--config BUILD_TOML] [--cc CC] [--std STD] [--build DIR] [--jobs N]
               [--timeout SECONDS]

The defaults: `ps1/src/build.toml`, found from the place of this script;
`cc`; `gnu89`; `port/build/hostcheck`; as many jobs as the machine has
processors; 120 seconds. `-std=STD` is given to every run of the
compiler but the one for its version. The field table is the one that `[types] fields` of the
configuration names, relative to the configuration. Sources are relative
to the configuration too. The numbers do not depend on the number of jobs.
"""

from __future__ import annotations

import argparse
import os
import re
import signal
import subprocess
import sys
import tomllib
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / "ps1" / "tools"))
sys.path.insert(0, str(Path(__file__).resolve().parent))

import structgen  # noqa: E402
from libgap import blank_literals  # noqa: E402

VERSION_TIMEOUT = 30
RANGES = (
    ("main memory", 0x80000000, 0x801FFFFF),
    ("main memory, uncached", 0xA0000000, 0xA01FFFFF),
    ("scratchpad", 0x1F800000, 0x1F8003FF),
    ("ports", 0x1F801000, 0x1F802FFF),
    ("BIOS", 0xBFC00000, 0xBFC7FFFF),
)
DIAGNOSTIC = re.compile(r"^(?P<where>.+?): (?P<level>warning|error): (?P<text>.*)$")
OPTION = re.compile(r"\[(-[^\[\]]+)\]\s*$")
NUMBER = re.compile(r"(?<![\w.])\.?[0-9](?:[eEpP][+-]|[\w.])*")
SUFFIX = r"(?:[uU](?:ll|LL|l|L)?|(?:ll|LL|l|L)[uU]?)?"
INTEGER = re.compile(
    r"(?:(0[xX][0-9a-fA-F]+)|(0[bB][01]+)|(0[0-7]*)|([1-9][0-9]*))" + SUFFIX + r"\Z"
)


class Problem(Exception):
    pass


@dataclass
class Unit:
    name: str
    source: str
    path: Path


@dataclass
class UnitResult:
    unit: Unit
    passed: bool = False
    diagnostics: list[tuple[str, str]] = field(default_factory=list)  # level, option
    literals: Counter = field(default_factory=Counter)  # range name -> count


@dataclass
class StructResult:
    name: str
    size: int
    kept: bool
    pointer: bool


# Inputs.


def read_config(path: Path) -> dict:
    try:
        with open(path, "rb") as handle:
            return tomllib.load(handle)
    except OSError as err:
        raise Problem(f"cannot read {path}: {err.strerror}")
    except tomllib.TOMLDecodeError as err:
        raise Problem(f"{path} is not valid TOML: {err}")


def read_types(config: dict, path: Path) -> tuple[Path, str]:
    types = config.get("types")
    if not isinstance(types, dict):
        raise Problem(f"{path} has no [types] table")
    fields, header = types.get("fields"), types.get("header")
    if not isinstance(fields, str) or not isinstance(header, str) or not fields or not header:
        raise Problem(f"{path}: [types] needs `fields` and `header`, both strings")
    return path.parent / fields, header


def read_units(config: dict, path: Path) -> tuple[list[Unit], int, int]:
    """The units to compile, the count under sdk/ and the count that are not C."""
    units: list[Unit] = []
    sdk = other = 0
    seen: set[str] = set()
    entries = config.get("unit", [])
    if not isinstance(entries, list):
        raise Problem(f"{path}: `unit` is not an array of tables")
    for index, entry in enumerate(entries, 1):
        name = entry.get("name") if isinstance(entry, dict) else None
        if not isinstance(name, str) or not name:
            raise Problem(f"{path}: unit {index} has no name")
        source = entry.get("source")
        if not isinstance(source, str) or not source:
            raise Problem(f"{path}: unit {name} has no source")
        if source.startswith("sdk/"):
            sdk += 1
            continue
        if not source.endswith(".c"):
            other += 1
            continue
        if name in seen:
            raise Problem(f"{path}: unit name {name} is used twice")
        seen.add(name)
        unit_path = path.parent / source
        if not unit_path.is_file():
            raise Problem(f"source of unit {name} does not exist: {unit_path}")
        units.append(Unit(name, source, unit_path))
    return units, sdk, other


def read_model(path: Path) -> structgen.Model:
    try:
        return structgen.load(path)
    except structgen.FieldsError as err:
        raise Problem(f"field table rejected: {'; '.join(err.errors)}".replace("\n", " "))


# Running the compiler.


def compile_run(argv: list[str], timeout: int) -> subprocess.CompletedProcess | None:
    """None when the compiler ran out of time."""
    env = dict(os.environ, LC_ALL="C")
    try:
        proc = subprocess.Popen(
            argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, errors="replace", env=env, start_new_session=True
        )
    except OSError as err:
        raise Problem(f"cannot run compiler {argv[0]}: {err.strerror}")
    try:
        out, err = proc.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        # The whole process group goes, so that a compiler driver's children do not hold the pipes.
        try:
            os.killpg(proc.pid, signal.SIGKILL)
        except OSError:
            pass
        proc.communicate()
        return None
    return subprocess.CompletedProcess(argv, proc.returncode, out, err)


def compiler_version(cc: str) -> str:
    try:
        proc = subprocess.run([cc, "--version"], capture_output=True, text=True, errors="replace", timeout=VERSION_TIMEOUT)
    except subprocess.TimeoutExpired:
        raise Problem(f"compiler {cc} did not answer --version in time")
    except OSError as err:
        raise Problem(f"cannot run compiler {cc}: {err.strerror}")
    if proc.returncode != 0:
        raise Problem(f"compiler {cc} --version ended with status {proc.returncode}")
    lines = (proc.stdout or proc.stderr).splitlines()
    if not lines:
        raise Problem(f"compiler {cc} --version printed nothing")
    return lines[0]


def pointer_size(cc: str, std: str, probe_dir: Path, timeout: int) -> int:
    for size in (2, 4, 8):
        source = probe_dir / f"pointer{size}.c"
        source.write_text(f"typedef char probe[(sizeof(void *) == {size}) ? 1 : -1];\n")
        proc = compile_run([cc, f"-std={std}", "-fsyntax-only", str(source)], timeout)
        if proc is not None and proc.returncode == 0:
            return size
    raise Problem(f"cannot find the pointer size with compiler {cc} and -std={std}")


# Units.


def parse_diagnostics(text: str) -> list[tuple[str, str]]:
    found = []
    for line in text.splitlines():
        match = DIAGNOSTIC.match(line)
        if not match:
            continue
        option = OPTION.search(match["text"])
        found.append((match["level"], option.group(1) if option else "(no option)"))
    return found


def scan_literals(text: str) -> Counter:
    counts: Counter = Counter()
    for match in NUMBER.finditer(blank_literals(text)):
        found = INTEGER.match(match.group(0))
        if not found:
            continue
        hexa, binary, octal, decimal = found.groups()
        if hexa:
            value = int(hexa, 16)
        elif binary:
            value = int(binary, 2)
        elif octal:
            value = int(octal, 8)
        else:
            value = int(decimal)
        for name, low, high in RANGES:
            if low <= value <= high:
                counts[name] += 1
    return counts


def check_unit(unit: Unit, cc: str, std: str, build: Path, timeout: int) -> UnitResult:
    result = UnitResult(unit)
    try:
        text = unit.path.read_text(errors="replace")
    except OSError as err:
        raise Problem(f"cannot read {unit.path}: {err.strerror}")
    result.literals = scan_literals(text)
    obj = build / "obj" / f"{unit.name}.o"
    obj.parent.mkdir(parents=True, exist_ok=True)
    obj.unlink(missing_ok=True)
    argv = [cc, f"-std={std}", "-c", "-I", str(build / "gen"), "-o", str(obj), str(unit.path)]
    proc = compile_run(argv, timeout)
    if proc is not None:
        result.passed = proc.returncode == 0
        result.diagnostics = parse_diagnostics(proc.stderr)
    return result


# Structs.


def has_pointer(model: structgen.Model) -> dict[str, bool]:
    by_name = {s.name: s for s in model.structs}
    memo: dict[str, bool] = {}

    def visit(name: str, active: tuple[str, ...]) -> bool:
        if name in memo:
            return memo[name]
        if name in active:
            return False
        found = False
        for f in by_name[name].fields:
            base, stars = structgen.TYPE_RE.match(f.type).groups()
            if stars or (base in by_name and visit(base, active + (name,))):
                found = True
                break
        memo[name] = found
        return found

    return {s.name: visit(s.name, ()) for s in model.structs}


def layout_source(model: structgen.Model, struct: structgen.StructDecl, header: str) -> str:
    lines = ["#include <stddef.h>"]
    for decl in model.types.values():
        lines.append(f"typedef struct {{ unsigned char b[{decl.size}]; }} __attribute__((aligned({decl.align}))) {decl.name};")
    lines.append(f'#include "{header}"')
    tests = [f"sizeof({struct.name}) == {struct.size}"]
    for f in struct.fields:
        member = f"{f.name}[0]" if f.count is not None else f.name
        tests.append(f"offsetof({struct.name}, {member}) == {f.offset}")
    lines.append(f"typedef char hostcheck_layout[({' && '.join(tests)}) ? 1 : -1];")
    return "\n".join(lines) + "\n"


def check_struct(model: structgen.Model, struct: structgen.StructDecl, pointer: bool, header: str, cc: str, std: str, build: Path, timeout: int) -> StructResult:
    source = build / "probe" / f"layout_{struct.name}.c"
    source.write_text(layout_source(model, struct, header))
    proc = compile_run([cc, f"-std={std}", "-fsyntax-only", "-I", str(build / "gen"), str(source)], timeout)
    return StructResult(struct.name, struct.size, proc is not None and proc.returncode == 0, pointer)


# Output.


def render(version: str, std: str, psize: int, counts: tuple[int, int, int], units: list[UnitResult], structs: list[StructResult]) -> tuple[str, str, str]:
    compiled, sdk, other = counts
    failed = [u.unit.name for u in units if not u.passed]
    kinds: dict[str, list[str]] = {}
    for u in units:
        for level, option in u.diagnostics:
            kinds.setdefault(f"{level} {option}", []).append(u.unit.name)
    kind_lines = sorted(
        (f"{kind}: {len(names)} in {len(set(names))} units" for kind, names in kinds.items()),
        key=lambda line: (-int(re.search(r": (\d+) in ", line).group(1)), line),
    )
    out = [
        f"compiler: {version}",
        f"language level: {std}",
        f"pointer size: {psize} bytes",
        f"units: {compiled} compiled, {sdk} under sdk/ and {other} not C left out",
        f"passed: {compiled - len(failed)}",
        f"failed: {len(failed)}",
        *kind_lines,
    ]
    kept = sum(s.kept for s in structs)
    out.append(f"structs: {kept} of {len(structs)} keep their layout")
    for label, want in (("with", True), ("without", False)):
        group = [s for s in structs if s.pointer == want]
        out.append(f"structs {label} a pointer: {len(group)}, of which {sum(not s.kept for s in group)} lose their layout")
    total = sum(sum(u.literals.values()) for u in units)
    with_literals = sum(1 for u in units if u.literals)
    out.append(f"fixed addresses: {total} literals in {with_literals} units")
    for name, _, _ in RANGES:
        n = sum(u.literals[name] for u in units)
        k = sum(1 for u in units if u.literals[name])
        out.append(f"{name}: {n} literals in {k} units")
    if failed:
        out.append("failed: " + " ".join(failed))
    unit_rows = []
    for u in units:
        warnings = sum(1 for level, _ in u.diagnostics if level == "warning")
        errors = sum(1 for level, _ in u.diagnostics if level == "error")
        unit_rows.append(
            f"{u.unit.name}\t{u.unit.source}\t{'passed' if u.passed else 'failed'}\t{warnings}\t{errors}\t{sum(u.literals.values())}\n"
        )
    struct_rows = [f"{s.name}\t{s.size}\t{'kept' if s.kept else 'lost'}\t{'pointer' if s.pointer else 'plain'}\n" for s in structs]
    return "\n".join(out) + "\n", "".join(unit_rows), "".join(struct_rows)


def run(args: argparse.Namespace) -> str:
    config_path: Path = args.config
    config = read_config(config_path)
    fields_path, header = read_types(config, config_path)
    units, sdk, other = read_units(config, config_path)
    model = read_model(fields_path)
    version = compiler_version(args.cc)

    build: Path = args.build
    for sub in ("gen", "obj", "probe"):
        (build / sub).mkdir(parents=True, exist_ok=True)
    try:
        header_path = build / "gen" / header
        header_path.parent.mkdir(parents=True, exist_ok=True)
        header_path.write_text(structgen.generate_header(model, Path(header).name))
    except OSError as err:
        raise Problem(f"cannot write {build / 'gen' / header}: {err.strerror}")

    psize = pointer_size(args.cc, args.std, build / "probe", args.timeout)
    pointers = has_pointer(model)
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        unit_jobs = [pool.submit(check_unit, u, args.cc, args.std, build, args.timeout) for u in units]
        struct_jobs = [
            pool.submit(check_struct, model, s, pointers[s.name], header, args.cc, args.std, build, args.timeout) for s in model.structs
        ]
        unit_results = sorted((j.result() for j in unit_jobs), key=lambda r: r.unit.name)
        struct_results = sorted((j.result() for j in struct_jobs), key=lambda r: r.name)

    text, unit_table, struct_table = render(version, args.std, psize, (len(units), sdk, other), unit_results, struct_results)
    try:
        (build / "units.tsv").write_text(unit_table)
        (build / "structs.tsv").write_text(struct_table)
    except OSError as err:
        raise Problem(f"cannot write the tables in {build}: {err.strerror}")
    return text


def positive(text: str) -> int:
    value = int(text)
    if value < 1:
        raise argparse.ArgumentTypeError("must be at least 1")
    return value


def main() -> int:
    parser = argparse.ArgumentParser(description="Compile the game's C units for this machine and count what does not carry over.")
    parser.add_argument("--config", type=Path, default=REPO / "ps1/src/build.toml")
    parser.add_argument("--cc", default="cc")
    parser.add_argument("--std", default="gnu89")
    parser.add_argument("--build", type=Path, default=REPO / "port/build/hostcheck")
    parser.add_argument("--timeout", type=positive, default=120)
    parser.add_argument("--jobs", type=positive, default=os.cpu_count() or 1)
    args = parser.parse_args()
    try:
        sys.stdout.write(run(args))
    except Problem as err:
        print(f"hostcheck.py: {' '.join(str(err).split())}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
