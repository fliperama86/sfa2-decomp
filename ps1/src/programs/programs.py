#!/usr/bin/env python3
"""Reader of `programs.toml`, the declaration of the disc's stand-alone programs.

    python programs.py [programs.toml]       prints each program with its functions and symbols

A stand-alone program is an executable of the disc that the game starts with `Exec` (the logo
is the first). It is not the resident executable and not a module of an archive, so
`ps1/src/build.toml` does not declare it. This module reads the declaration and, when a tool
needs the program's code, the user's own copy of the program (`read_body`). Other tools use it:
the differential test tool (`ps1/src/slot06_nonmatching/difftest.py`) and the workflow tool.

The file holds one `[[program]]` table per program:

    name       lower case letters and digits; the image part of a function's name
    file       the user's copy of the disc's file, a path relative to the toml (private, never committed)
    disc       the file's path on the disc
    sha256     the file's hash, 64 lower case hex digits
    address    where the body loads; the file's header must say the same
    functions  array of { name, address, size } for the functions that have C in the tree; the name
               is func_ADDRESS_PROGRAM with the address in eight lower case hex digits; required
               (an empty array when none has C yet)
    symbols    table NAME = ADDRESS of names inside this program (its data, its own copy of a library
               function); they hold for the links of this program's functions only; optional

`load` reads the toml only, never the program's file; it collects every refusal and raises one
`ProgramError` that names each. `read_body` reads the file and refuses a wrong hash, a header
that is not a PS-X EXE header, a load address or a size that disagrees with the declaration,
and a function that lies outside the body. Cost: a run reads one small text file (and, for
`read_body`, the program's file of about 130 kilobytes), milliseconds.
"""

from __future__ import annotations

import hashlib
import re
import struct
import sys
import tomllib
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).resolve().parent
DECLARATION = HERE / "programs.toml"
HEADER_SIZE = 2048
MAGIC = b"PS-X EXE"
RAM_BASE = 0x80000000
RAM_END = 0x80200000
PROGRAM_KEYS = {"name", "file", "disc", "sha256", "address", "functions", "symbols"}
REQUIRED_KEYS = ("name", "file", "disc", "sha256", "address", "functions")
FUNCTION_KEYS = {"name", "address", "size"}


class ProgramError(Exception):
    """A refusal. `errors` holds every finding, `str` joins them."""

    def __init__(self, errors: list[str] | str):
        self.errors = [errors] if isinstance(errors, str) else list(errors)
        super().__init__("; ".join(self.errors))


@dataclass(frozen=True)
class ProgramFunction:
    name: str
    address: int
    size: int


@dataclass(frozen=True)
class Program:
    name: str
    file: Path  # resolved against the folder of the toml
    disc: str
    sha256: str
    address: int
    functions: tuple[ProgramFunction, ...]
    symbols: dict[str, int]


def is_int(value) -> bool:
    return isinstance(value, int) and not isinstance(value, bool)


def in_ram(value) -> bool:
    return is_int(value) and RAM_BASE <= value < RAM_END


def check_function(program: str, address: int, entry, errors: list[str]) -> ProgramFunction | None:
    if not isinstance(entry, dict):
        errors.append(f"program {program}: a function is not a table")
        return None
    label = f"program {program}: function {entry.get('name')!r}"
    unknown = sorted(set(entry) - FUNCTION_KEYS)
    missing = sorted(FUNCTION_KEYS - set(entry))
    if unknown:
        errors.append(f"{label}: unknown key {', '.join(unknown)}")
    if missing:
        errors.append(f"{label}: missing key {', '.join(missing)}")
    if unknown or missing:
        return None
    name, at, size = entry["name"], entry["address"], entry["size"]
    found = re.fullmatch(r"func_([0-9a-f]{8})_([a-z0-9]+)", name) if isinstance(name, str) else None
    if found is None or found.group(2) != program:
        errors.append(f"{label}: the name is not func_<8 hex digits>_{program}")
        return None
    if not is_int(at) or not is_int(size):
        errors.append(f"{label}: address and size are integers")
        return None
    if int(found.group(1), 16) != at:
        errors.append(f"{label}: the address {at:#x} is not the one in the name")
        return None
    if at % 4 or size <= 0 or size % 4:
        errors.append(f"{label}: address {at:#x} and size {size} must be word aligned, the size above 0")
        return None
    if at < address or at + size > RAM_END:
        errors.append(f"{label}: [{at:#x}, {at + size:#x}) lies outside the body (from {address:#x}, below {RAM_END:#x})")
        return None
    return ProgramFunction(name, at, size)


def check_program(entry, base: Path, errors: list[str]) -> Program | None:
    if not isinstance(entry, dict):
        errors.append("a program is not a table")
        return None
    name = entry.get("name")
    label = f"program {name!r}"
    before = len(errors)
    unknown = sorted(set(entry) - PROGRAM_KEYS)
    if unknown:
        errors.append(f"{label}: unknown key {', '.join(unknown)}")
    missing = [k for k in REQUIRED_KEYS if k not in entry]
    if missing:
        errors.append(f"{label}: missing key {', '.join(missing)}")
        return None
    if not isinstance(name, str) or re.fullmatch(r"[a-z0-9]+", name) is None:
        errors.append(f"{label}: the name is lower case letters and digits")
        return None
    file, disc, digest, address = entry["file"], entry["disc"], entry["sha256"], entry["address"]
    if not isinstance(file, str) or not file or Path(file).is_absolute():
        errors.append(f"{label}: file is a path relative to the toml")
    if not isinstance(disc, str) or re.fullmatch(r"[A-Za-z0-9_.-]+(/[A-Za-z0-9_.-]+)*", disc) is None or ".." in disc.split("/"):
        errors.append(f"{label}: disc is a path of the disc with no leading slash")
    if not isinstance(digest, str) or re.fullmatch(r"[0-9a-f]{64}", digest) is None:
        errors.append(f"{label}: sha256 is 64 lower case hex digits")
    if not in_ram(address) or address % 4:
        errors.append(f"{label}: address is a word aligned address in RAM")
        return None
    functions = []
    declared = entry["functions"]
    if not isinstance(declared, list):
        errors.append(f"{label}: functions is an array of tables")
        declared = []
    for item in declared:
        function = check_function(name, address, item, errors)
        if function is not None:
            functions.append(function)
    names = [f.name for f in functions]
    for repeated in sorted({n for n in names if names.count(n) > 1}):
        errors.append(f"{label}: function {repeated} is declared twice")
    ordered = sorted(functions, key=lambda f: f.address)
    for first, second in zip(ordered, ordered[1:]):
        if second.address < first.address + first.size:
            errors.append(f"{label}: {first.name} and {second.name} overlap")
    symbols = entry.get("symbols", {})
    if not isinstance(symbols, dict):
        errors.append(f"{label}: symbols is a table")
        symbols = {}
    for symbol, value in symbols.items():
        if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol) is None:
            errors.append(f"{label}: symbol {symbol!r} is not a C name")
        elif not in_ram(value):
            errors.append(f"{label}: symbol {symbol} is not an address in RAM")
        elif symbol in names:
            errors.append(f"{label}: symbol {symbol} is also a function of the program")
    if len(errors) > before or not isinstance(file, str) or not isinstance(digest, str) or not isinstance(disc, str):
        return None
    return Program(name, (base / file).resolve(), disc, digest, address, tuple(functions),
                   {k: v for k, v in symbols.items()})


def load(path: Path = DECLARATION) -> tuple[Program, ...]:
    """The programs of the declaration. Refuses, naming every finding, what it does not understand."""
    path = Path(path)
    try:
        document = tomllib.loads(path.read_text())
    except OSError as exc:
        raise ProgramError(f"{path.name} cannot be read: {exc.strerror or exc}") from exc
    except (tomllib.TOMLDecodeError, UnicodeDecodeError) as exc:
        raise ProgramError(f"{path.name} is not valid TOML: {exc}") from exc
    errors: list[str] = []
    unknown = sorted(set(document) - {"program"})
    if unknown:
        errors.append(f"unknown top-level key {', '.join(unknown)}")
    entries = document.get("program", [])
    if not isinstance(entries, list):
        errors.append("program is an array of tables")
        entries = []
    programs = []
    for entry in entries:
        program = check_program(entry, path.resolve().parent, errors)
        if program is not None:
            programs.append(program)
    seen = [p.name for p in programs]
    for repeated in sorted({n for n in seen if seen.count(n) > 1}):
        errors.append(f"program {repeated} is declared twice")
    functions = [f.name for p in programs for f in p.functions]
    for repeated in sorted({n for n in functions if functions.count(n) > 1}):
        errors.append(f"function {repeated} is declared twice")
    if errors:
        raise ProgramError(errors)
    return tuple(programs)


def program_of(programs: tuple[Program, ...], name: str) -> Program | None:
    """The program that the function `name` (func_ADDRESS_PROGRAM) belongs to, None when there is none."""
    found = re.fullmatch(r"func_[0-9a-f]{8}_([a-z0-9]+)", name)
    if found is None:
        return None
    return next((p for p in programs if p.name == found.group(1)), None)


def read_body(program: Program) -> bytes:
    """The body of the program's file (the file after the 2,048-byte header), checked against the declaration."""
    try:
        data = program.file.read_bytes()
    except OSError as exc:
        raise ProgramError(f"program {program.name}: the file {program.file.name} cannot be read ({exc.strerror or exc}); "
                           f"take {program.disc} out of your disc image with discfile.py") from exc
    digest = hashlib.sha256(data).hexdigest()
    if digest != program.sha256:
        raise ProgramError(f"program {program.name}: the file's sha256 is {digest}, the declaration says {program.sha256}")
    if len(data) <= HEADER_SIZE or data[: len(MAGIC)] != MAGIC:
        raise ProgramError(f"program {program.name}: the file does not begin with a PS-X EXE header")
    load_address, size = struct.unpack_from("<II", data, 0x18)
    if load_address != program.address:
        raise ProgramError(f"program {program.name}: the header loads the body at {load_address:#x}, the declaration says {program.address:#x}")
    body = data[HEADER_SIZE:]
    if size != len(body):
        raise ProgramError(f"program {program.name}: the header says a body of {size} bytes, the file holds {len(body)}")
    for function in program.functions:
        if function.address + function.size > program.address + len(body):
            raise ProgramError(f"program {program.name}: {function.name} ends at {function.address + function.size:#x}, "
                               f"past the body's end {program.address + len(body):#x}")
    return body


def main(argv: list[str] | None = None) -> int:
    argv = sys.argv[1:] if argv is None else argv
    if len(argv) > 1:
        print("usage: programs.py [programs.toml]", file=sys.stderr)
        return 2
    try:
        programs = load(Path(argv[0]) if argv else DECLARATION)
    except ProgramError as exc:
        for error in exc.errors:
            print(f"INPUT ERROR: {error}", file=sys.stderr)
        return 2
    for program in programs:
        print(f"{program.name}: {program.disc} at {program.address:#x}, {len(program.functions)} functions, "
              f"{len(program.symbols)} symbols")
    return 0


if __name__ == "__main__":
    sys.exit(main())
