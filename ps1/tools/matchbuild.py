#!/usr/bin/env python3
"""PS1 matching build: rebuild the resident executable from declared owners.

Contract: ps1/docs/matching-build.md. All game-specific data (addresses, names,
hosts, hashes) comes from the private configuration, never from this file.

Exit status: 0 when every check passes, 1 for a failed check or build step,
2 for configuration errors, 3 for an unusable environment.
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import io
import json
import os
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile
import tomllib
from pathlib import Path

from elftools.elf.elffile import ELFFile

import pac
import structgen

SHF_ALLOC = 0x2
# The name of the baseline executable's image, on the command line and in a unit's `image` key.
RESIDENT = "resident"
HEADER_SIZE = 2048
EXE_MAGIC = b"PS-X EXE"
# Sections that are not loaded on target and are dropped by the linker script.
# Read-only data sections a unit object may hold. All of them go to one output section.
RODATA_SECTIONS = (".rodata", ".rdata")
# Sections of each loaded range kind a unit may declare, and the bss sections.
LOADED_SECTIONS = {"rodata": RODATA_SECTIONS, "data": (".data",)}
BSS_SECTIONS = (".bss", ".sbss")
# The object sections a unit owns, per kind, in the order the link places them.
OWNED_SECTIONS = {"text": (".text",), "rodata": RODATA_SECTIONS, "data": LOADED_SECTIONS["data"], "bss": BSS_SECTIONS}
DISCARDED_SECTIONS = (".reginfo", ".MIPS.abiflags", ".pdr", ".comment", ".gnu.attributes")
IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
TAG_RE = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]*\Z")
FLAG_RE = re.compile(r"-[A-Za-z0-9_=.,+-]+\Z")
HOST_RE = re.compile(r"[A-Za-z0-9_.@-]+\Z")
REMOTE_PATH_RE = re.compile(r"[A-Za-z0-9_./-]+\Z")
SYMBOL_STMT_RE = re.compile(r"([A-Za-z_.$][A-Za-z0-9_.$]*)\s*=\s*(0[xX][0-9a-fA-F]+|[0-9]+)\Z")
# Without -no-pad-sections the assembler pads each section to 16 bytes, which
# would spill a unit past its declared range.
AS_FLAGS = ("-EL", "-G0", "-march=r3000", "-mabi=32", "-no-pad-sections")
# Floating-point types and literals in preprocessed C, for compilers marked no_float.
# Bump when anything about how cache entries are keyed or laid out changes.
CACHE_FORMAT = "matchbuild-objcache-3"
CACHE_PAYLOAD = ("unit.o", "unit.s", "unit.gnu.s")
CACHE_FILES = (*CACHE_PAYLOAD, "entry.json")
FLOAT_RE = re.compile(r"\b(?:float|double)\b|(?<![\w.])(?:\d+\.\d*|\.\d+|\d+(?=[eE]))(?:[eE][+-]?\d+)?")
# String and character literals. Their text is data, not code: a version
# string such as "1.71" is not a floating-point constant.
LITERAL_RE = re.compile(r'"(?:[^"\\\n]|\\.)*"' + "|" + r"'(?:[^'\\\n]|\\.)*'")


def find_float(preprocessed: str) -> str | None:
    """The first floating-point type or literal in preprocessed C, outside string and character literals."""
    match = FLOAT_RE.search(LITERAL_RE.sub('""', preprocessed))
    return match.group(0) if match else None


ASM_RE = re.compile(r"\b(?:asm|__asm|__asm__)\b")


def find_inline_asm(preprocessed: str) -> str | None:
    """The first `asm`, `__asm` or `__asm__` token in preprocessed C, outside string and character literals."""
    match = ASM_RE.search(LITERAL_RE.sub('""', preprocessed))
    return match.group(0) if match else None


class ConfigError(Exception):
    """One or more configuration problems, all reported together."""

    def __init__(self, errors: list[str]):
        super().__init__("; ".join(errors))
        self.errors = errors


class StepError(Exception):
    """A build step failed (subprocess exit status or similar)."""


class EnvironmentFailure(Exception):
    """The environment cannot run the build (for example no SSH master)."""


# ---------------------------------------------------------------------------
# Declarations and the pure comparison


@dataclasses.dataclass(frozen=True)
class FunctionDecl:
    name: str
    address: int
    size: int


@dataclasses.dataclass(frozen=True)
class RodataDecl:
    address: int
    size: int

    @property
    def end(self) -> int:
        return self.address + self.size


@dataclasses.dataclass(frozen=True)
class ImageDecl:
    """A module image: one chunk of an archive, loaded by the resident loader to a fixed address."""

    name: str
    archive: Path
    slot: int
    sha256: str
    address: int
    payload: bytes = b""  # the chunk's bytes, filled in once the archive has been read
    symbols: dict = dataclasses.field(default_factory=dict)  # [image.symbols]: addresses of names of symbols.ld in this image

    @property
    def size(self) -> int:
        return len(self.payload)


@dataclasses.dataclass(frozen=True)
class UnitDecl:
    name: str
    source: str
    flags: tuple[str, ...]
    functions: tuple[FunctionDecl, ...]  # sorted by address
    rodata: RodataDecl | None = None  # the one read-only data range, if declared
    data: RodataDecl | None = None  # the one initialised data range, if declared
    bss: RodataDecl | None = None  # where the uninitialised data lives, if declared
    kind: str = "c"  # "c", or "asm" for a unit written in assembly
    image: str = RESIDENT  # the image the unit belongs to: RESIDENT or a declared module image

    def loaded(self) -> list[tuple[str, RodataDecl]]:
        """The declared ranges that hold payload bytes: (kind, decl) for rodata and data."""
        return [(k, d) for k, d in (("rodata", self.rodata), ("data", self.data)) if d is not None]

    @property
    def start(self) -> int:
        return self.functions[0].address

    @property
    def end(self) -> int:
        last = self.functions[-1]
        return last.address + last.size


@dataclasses.dataclass(frozen=True)
class DefinedSymbol:
    """A global or weak symbol that a unit object defines."""

    name: str
    kind: str  # text, rodata, data or bss; "other" for an absolute symbol or any other section
    section: str  # the object section, "*ABS*" for an absolute symbol
    offset: int  # within the unit's range of that kind; st_value for "other"


@dataclasses.dataclass(frozen=True)
class FunctionResult:
    unit: str
    name: str
    address: int
    size: int
    exact: bool
    first_diff: int | None  # offset inside the function, None when equal
    equal_words: int
    total_words: int
    image_sha256: str
    baseline_sha256: str


@dataclasses.dataclass(frozen=True)
class RodataResult:
    """Comparison of one rodata or data range (`kind`)."""

    unit: str
    address: int
    size: int
    exact: bool
    first_diff: int | None  # offset inside the range, None when equal
    image_sha256: str
    baseline_sha256: str
    kind: str = "rodata"


@dataclasses.dataclass(frozen=True)
class ImageComparison:
    image_size: int
    baseline_size: int
    image_sha256: str
    baseline_sha256: str
    image_exact: bool
    functions: tuple[FunctionResult, ...]
    rodata: tuple[RodataResult, ...] = ()
    data: tuple[RodataResult, ...] = ()

    @property
    def all_functions_exact(self) -> bool:
        return all(f.exact for f in self.functions)

    @property
    def all_rodata_exact(self) -> bool:
        return all(r.exact for r in self.rodata)

    @property
    def all_data_exact(self) -> bool:
        return all(r.exact for r in self.data)

    @property
    def all_ranges_exact(self) -> bool:
        return self.all_rodata_exact and self.all_data_exact


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def owned_ranges(units: list[UnitDecl] | tuple[UnitDecl, ...]) -> list[tuple[int, int]]:
    """(start, end) of every unit text, rodata and data range, sorted."""
    owned = [(u.start, u.end) for u in units]
    owned += [(d.address, d.end) for u in units for _, d in u.loaded()]
    return sorted(owned)


def raw_ranges(load: int, size: int, units: list[UnitDecl] | tuple[UnitDecl, ...]) -> list[tuple[int, int]]:
    """Return (address, size) of every payload byte range not owned by a unit.

    Owned means a unit's text range, its read-only data range or its data range.
    """
    ranges = []
    cursor = load
    for start, end in owned_ranges(units):
        if start > cursor:
            ranges.append((cursor, start - cursor))
        cursor = end
    if cursor < load + size:
        ranges.append((cursor, load + size - cursor))
    return ranges


def compare_image(
    image: bytes,
    payload: bytes,
    load: int,
    units: list[UnitDecl] | tuple[UnitDecl, ...],
) -> ImageComparison:
    """Compare a rebuilt image with the baseline payload.

    Pure function: no I/O. Functions are compared from the image at their
    declared addresses, so a mutated copy of the image exercises the same code.
    """
    results = []
    range_results: dict[str, list[RodataResult]] = {kind: [] for kind in LOADED_SECTIONS}
    for unit in units:
        for kind, decl in unit.loaded():
            off = decl.address - load
            got = image[off : off + decl.size]
            want = payload[off : off + decl.size]
            first_diff = None
            if len(got) != decl.size or len(want) != decl.size:
                first_diff = min(len(got), len(want))
            else:
                first_diff = next((i for i in range(decl.size) if got[i] != want[i]), None)
            range_results[kind].append(
                RodataResult(
                    unit=unit.name,
                    address=decl.address,
                    size=decl.size,
                    exact=first_diff is None,
                    first_diff=first_diff,
                    image_sha256=sha256(got),
                    baseline_sha256=sha256(want),
                    kind=kind,
                )
            )
        for fn in unit.functions:
            off = fn.address - load
            got = image[off : off + fn.size]
            want = payload[off : off + fn.size]
            first_diff = None
            if len(got) != fn.size or len(want) != fn.size:
                first_diff = min(len(got), len(want))
            else:
                for i in range(fn.size):
                    if got[i] != want[i]:
                        first_diff = i
                        break
            equal_words = sum(
                1
                for i in range(0, fn.size, 4)
                if len(got) >= i + 4 and got[i : i + 4] == want[i : i + 4]
            )
            results.append(
                FunctionResult(
                    unit=unit.name,
                    name=fn.name,
                    address=fn.address,
                    size=fn.size,
                    exact=first_diff is None,
                    first_diff=first_diff,
                    equal_words=equal_words,
                    total_words=fn.size // 4,
                    image_sha256=sha256(got),
                    baseline_sha256=sha256(want),
                )
            )
    return ImageComparison(
        image_size=len(image),
        baseline_size=len(payload),
        image_sha256=sha256(image),
        baseline_sha256=sha256(payload),
        image_exact=(image == payload),
        functions=tuple(results),
        rodata=tuple(range_results["rodata"]),
        data=tuple(range_results["data"]),
    )


def flip_byte(data: bytes, offset: int) -> bytes:
    mutated = bytearray(data)
    mutated[offset] ^= 0x01
    return bytes(mutated)


def run_controls(
    image: bytes,
    payload: bytes,
    load: int,
    units: list[UnitDecl] | tuple[UnitDecl, ...],
) -> list[dict]:
    """Mutate the image and require the comparator to notice. Returns control records."""
    controls = []
    for unit in units:
        for fn in unit.functions:
            result = compare_image(flip_byte(image, fn.address - load), payload, load, units)
            failed = [f.name for f in result.functions if not f.exact]
            tripped = failed == [fn.name] and result.all_ranges_exact and not result.image_exact
            controls.append(
                {
                    "kind": "function",
                    "target": fn.name,
                    "applicable": True,
                    "tripped": tripped,
                    "detail": f"functions failing: {failed}, image exact: {result.image_exact}",
                }
            )
        for kind, decl in unit.loaded():
            result = compare_image(flip_byte(image, decl.address - load), payload, load, units)
            failed = [r.unit for r in getattr(result, kind) if not r.exact]
            other = result.all_data_exact if kind == "rodata" else result.all_rodata_exact
            tripped = failed == [unit.name] and result.all_functions_exact and other and not result.image_exact
            controls.append(
                {
                    "kind": kind,
                    "target": f"{unit.name} {kind}",
                    "applicable": True,
                    "tripped": tripped,
                    "detail": (
                        f"{kind} failing: {failed}, functions exact: {result.all_functions_exact}, "
                        f"image exact: {result.image_exact}"
                    ),
                }
            )
    ranges = raw_ranges(load, len(payload), units)
    if ranges:
        address, size = ranges[0]
        result = compare_image(flip_byte(image, address - load + size // 2), payload, load, units)
        tripped = (not result.image_exact) and result.all_functions_exact and result.all_ranges_exact
        controls.append(
            {
                "kind": "raw",
                "target": f"raw byte at {address + size // 2:#010x}",
                "applicable": True,
                "tripped": tripped,
                "detail": (
                    f"image exact: {result.image_exact}, functions exact: {result.all_functions_exact}, "
                    f"rodata exact: {result.all_rodata_exact}, data exact: {result.all_data_exact}"
                ),
            }
        )
    else:
        # A payload fully owned by units has no raw byte to mutate. The
        # function controls above already cover every byte of it.
        controls.append(
            {
                "kind": "raw",
                "target": "none",
                "applicable": False,
                "tripped": False,
                "detail": "no raw range: the payload is fully owned by units",
            }
        )
    return controls


# ---------------------------------------------------------------------------
# Configuration


@dataclasses.dataclass
class Cc1Config:
    kind: str
    sha256: str
    path: Path | None = None
    host: str = ""
    socket: Path | None = None
    scp_dir: str = ""
    exec_dir: str = ""
    exec_prefix: str = ""
    binary: str = ""
    name: str = "cc1"  # configuration key: "cc1" or "cc1_reference"
    no_float: bool = False  # this binary miscompiles floating-point constants


@dataclasses.dataclass
class Config:
    path: Path
    directory: Path
    baseline_path: Path
    baseline_sha256: str
    cpp: str
    maspsx: Path
    maspsx_commit: str
    aspsx_version: str
    binutils_prefix: str
    cc1: Cc1Config
    units: list[UnitDecl]  # the units of every image
    symbols_path: Path
    symbol_names: list[str]
    types_fields: Path | None = None  # [types] fields file, None when absent
    types_header: str = ""
    expand_div: bool = False  # run maspsx with --expand-div
    include_dirs: tuple[Path, ...] = ()  # extra -I directories for the preprocessor
    images: list[ImageDecl] = dataclasses.field(default_factory=list)  # module images, in declaration order
    # Filled in during validation of the baseline.
    baseline: bytes = b""
    load: int = 0
    payload_size: int = 0

    def units_of(self, image: str) -> list[UnitDecl]:
        """The units that belong to one image: RESIDENT or the name of a module image."""
        return [u for u in self.units if u.image == image]

    def functions_of_others(self, image: str) -> list[tuple[str, int, str]]:
        """The declared functions of the units of every image but `image`: (name, address, declaring image)."""
        return [(fn.name, fn.address, u.image) for u in self.units if u.image != image for fn in u.functions]

    @property
    def payload(self) -> bytes:
        return self.baseline[HEADER_SIZE : HEADER_SIZE + self.payload_size]

    @property
    def header(self) -> bytes:
        return self.baseline[:HEADER_SIZE]


def _expand(value: str, directory: Path) -> Path:
    path = Path(value).expanduser()
    if not path.is_absolute():
        path = directory / path
    return Path(os.path.normpath(path))


def parse_symbols(text: str, errors: list[str]) -> list[str]:
    """Return names assigned in a symbols.ld file (simple `name = value;` lines)."""
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    names: list[str] = []
    for stmt in (s.strip() for s in text.split(";")):
        if not stmt:
            continue
        match = SYMBOL_STMT_RE.match(stmt)
        if not match:
            errors.append(f"symbols.ld: unsupported statement {stmt!r} (expected 'name = value;')")
            continue
        if match.group(1) in names:
            errors.append(f"symbols.ld: duplicate assignment of {match.group(1)!r}")
        names.append(match.group(1))
    return names


def _get_str(table: dict, key: str, where: str, errors: list[str], pattern=None) -> str:
    value = table.get(key)
    if not isinstance(value, str) or not value:
        errors.append(f"{where}: '{key}' must be a non-empty string")
        return ""
    if pattern is not None and not pattern.match(value):
        errors.append(f"{where}: '{key}' has unsupported characters: {value!r}")
        return ""
    return value


def _get_hex(table: dict, key: str, where: str, errors: list[str]) -> str:
    value = _get_str(table, key, where, errors)
    if value and not re.fullmatch(r"[0-9a-f]{64}", value):
        errors.append(f"{where}: '{key}' must be 64 lowercase hex digits")
        return ""
    return value


def parse_rodata(value, where: str, errors: list[str], key: str = "rodata") -> tuple[RodataDecl | None, bool]:
    """Parse the optional `rodata`, `data` or `bss` key `{ address = .., size = .. }`. Returns (decl, ok)."""
    if value is None:
        return None, True
    if not isinstance(value, dict) or set(value) != {"address", "size"}:
        errors.append(f"{where}: '{key}' must be a table with exactly 'address' and 'size'")
        return None, False
    ok = True
    for label in ("address", "size"):
        item = value[label]
        if not isinstance(item, int) or isinstance(item, bool):
            errors.append(f"{where}: {key} {label} must be an integer")
            ok = False
        elif item % 4:
            errors.append(f"{where}: {key} {label} {item:#x} is not a multiple of four")
            ok = False
    if ok and value["size"] <= 0:
        errors.append(f"{where}: {key} size must be greater than zero")
        ok = False
    return (RodataDecl(value["address"], value["size"]) if ok else None), ok


def rodata_overlaps(units: list[UnitDecl]) -> list[str]:
    """Each rodata or data range against every text range and every other rodata or data range."""
    errors = []
    for unit in units:
        for kind, r in unit.loaded():
            for other in units:
                if r.address < other.end and other.start < r.end:
                    whose = "its own text range" if other is unit else f"the text range of unit {other.name!r}"
                    errors.append(
                        f"unit {unit.name!r}: {kind} range {r.address:#x}-{r.end:#x} overlaps {whose} "
                        f"({other.start:#x}-{other.end:#x})"
                    )
                for other_kind, o in other.loaded():
                    if (other.name, other_kind) <= (unit.name, kind):
                        continue
                    if r.address < o.end and o.address < r.end:
                        errors.append(
                            f"unit {unit.name!r}: {kind} range {r.address:#x}-{r.end:#x} overlaps the {other_kind} "
                            f"range of unit {other.name!r} ({o.address:#x}-{o.end:#x})"
                        )
    return errors


def bss_overlaps(units: list[UnitDecl]) -> list[str]:
    """Two bss ranges must not overlap."""
    errors = []
    for unit in units:
        for other in units:
            if unit.bss is None or other.bss is None or not unit.name < other.name:
                continue
            if unit.bss.address < other.bss.end and other.bss.address < unit.bss.end:
                errors.append(
                    f"unit {unit.name!r}: bss range {unit.bss.address:#x}-{unit.bss.end:#x} overlaps the bss range "
                    f"of unit {other.name!r} ({other.bss.address:#x}-{other.bss.end:#x})"
                )
    return errors


def geometry_errors(units: list[UnitDecl]) -> list[str]:
    """The range rules that need no payload, for the units of one image."""
    errors = []
    ordered = sorted(units, key=lambda u: u.start)
    for a, b in zip(ordered, ordered[1:]):
        if a.end > b.start:
            errors.append(
                f"units {a.name!r} and {b.name!r} overlap "
                f"({a.start:#x}-{a.end:#x} and {b.start:#x}-{b.end:#x})"
            )
    return errors + rodata_overlaps(units) + bss_overlaps(units)


def payload_errors(units: list[UnitDecl], load: int, size: int) -> list[str]:
    """The range rules against the payload `[load, load + size)` of the image the units belong to."""
    errors = []
    for unit in units:
        if unit.start < load or unit.end > load + size:
            errors.append(
                f"unit {unit.name!r} range {unit.start:#x}-{unit.end:#x} is outside the payload "
                f"{load:#x}-{load + size:#x}"
            )
        for kind, decl in unit.loaded():
            if decl.address < load or decl.end > load + size:
                errors.append(
                    f"unit {unit.name!r} {kind} range {decl.address:#x}-{decl.end:#x} "
                    f"is outside the payload {load:#x}-{load + size:#x}"
                )
        if unit.bss is not None and (unit.bss.address < load + size and load < unit.bss.end):
            errors.append(
                f"unit {unit.name!r} bss range {unit.bss.address:#x}-{unit.bss.end:#x} "
                f"touches the payload {load:#x}-{load + size:#x}"
            )
    return errors


def parse_images(raw: dict, directory: Path, errors: list[str]) -> tuple[list[ImageDecl], set[str]]:
    """The structure of the [[image]] tables: the images that are well formed, and every name a table gives.

    The names include those of tables with other faults, so that a unit of
    such an image is not reported a second time. The archives are read later.
    """
    image_tables = raw.get("image", [])
    if not isinstance(image_tables, list):
        errors.append("[[image]] must be an array of tables")
        return [], set()
    images: list[ImageDecl] = []
    seen: set[str] = set()
    declared: set[str] = set()
    for index, table in enumerate(image_tables):
        where = f"[[image]] #{index + 1}"
        if not isinstance(table, dict):
            errors.append(f"{where}: must be a table")
            continue
        if isinstance(table.get("name"), str):
            declared.add(table["name"])
        name = _get_str(table, "name", where, errors, TAG_RE)
        if name:
            where = f"image {name!r}"
            if name == RESIDENT:
                errors.append(f"{where}: the name is reserved for the resident image")
                name = ""
            elif name in seen:
                errors.append(f"{where}: duplicate image name")
                name = ""
            seen.add(name or RESIDENT)
        archive = _get_str(table, "archive", where, errors)
        sha = _get_hex(table, "sha256", where, errors)
        numbers = {}
        for key in ("slot", "address"):
            value = table.get(key)
            if not isinstance(value, int) or isinstance(value, bool):
                errors.append(f"{where}: '{key}' must be an integer")
            else:
                numbers[key] = value
        local: dict[str, int] = {}
        table_symbols = table.get("symbols", {})
        if not isinstance(table_symbols, dict):
            errors.append(f"{where}: 'symbols' must be a table")
        else:
            for key, value in table_symbols.items():
                if not isinstance(value, int) or isinstance(value, bool):
                    errors.append(f"{where}: symbols '{key}' must be an integer address, got {value!r}")
                else:
                    local[key] = value
        if name and archive and sha and len(numbers) == 2:
            images.append(
                ImageDecl(name, _expand(archive, directory), numbers["slot"], sha, numbers["address"], symbols=local)
            )
    return images, declared - {RESIDENT}


def read_images(images: list[ImageDecl], overlays, baseline: bytes, errors: list[str]) -> list[ImageDecl]:
    """Check each image against the loader's tables and its archive; return it with its payload.

    `baseline` is a whole executable that has passed its own checks.
    """
    resident = pac.Image(baseline)
    pointers = overlays.get("table_pointers") if isinstance(overlays, dict) else None
    if not isinstance(pointers, int) or isinstance(pointers, bool):
        errors.append("[overlays]: 'table_pointers' must be an integer address, and images need it")
        return []
    if pointers % 4 or not resident.start <= pointers <= resident.end - 4:
        errors.append(
            f"[overlays]: table_pointers {pointers:#x} is not a word address inside the resident payload "
            f"{resident.start:#x}-{resident.end:#x}"
        )
        return []
    try:
        table = pac.destination_tables(resident, pointers)[0]
    except pac.FormatError as exc:
        errors.append(f"[overlays]: {exc}")
        return []
    read: list[ImageDecl] = []
    for image in images:
        where = f"image {image.name!r}"
        ok = True
        if image.address % 4:
            errors.append(f"{where}: address {image.address:#x} is not a multiple of four")
            ok = False
        if not 0 <= image.slot < len(table):
            errors.append(f"{where}: slot {image.slot:#x} is beyond table 0 ({len(table)} entries)")
            ok = False
        elif ok and table[image.slot] != image.address:
            errors.append(
                f"{where}: address {image.address:#x} differs from entry {image.slot:#x} of table 0 "
                f"({table[image.slot]:#x})"
            )
            ok = False
        try:
            data = image.archive.read_bytes()
        except OSError as exc:
            errors.append(f"{where}: cannot read archive {image.archive}: {exc}")
            continue
        try:
            chunks = pac.chunk_table(data)
        except pac.FormatError as exc:
            errors.append(f"{where}: archive {image.archive.name} is rejected by the archive reader: {exc}")
            continue
        found = [c for c in chunks if c["table"] == 0 and c["slot"] == image.slot]
        if len(found) != 1:
            errors.append(
                f"{where}: archive {image.archive.name} has {len(found)} chunks with table number 0 and "
                f"slot {image.slot:#x}, exactly one is required"
            )
            continue
        payload = data[found[0]["offset"] : found[0]["offset"] + found[0]["size"]]
        if not payload:
            errors.append(f"{where}: the chunk in {image.archive.name} has no bytes")
            continue
        if sha256(payload) != image.sha256:
            errors.append(f"{where}: chunk sha256 mismatch: chunk is {sha256(payload)}, configuration pins {image.sha256}")
            continue
        if ok:
            read.append(dataclasses.replace(image, payload=payload))
    return read


def load_config(config_path: Path, use_reference: bool = False) -> Config:
    """Read and validate the configuration. Raises ConfigError with every problem found.

    With `use_reference`, the compiler comes from [toolchain.cc1_reference]
    instead of [toolchain.cc1].
    """
    errors: list[str] = []
    directory = config_path.parent
    try:
        raw = tomllib.loads(config_path.read_text())
    except (OSError, tomllib.TOMLDecodeError) as exc:
        raise ConfigError([f"cannot read configuration {config_path}: {exc}"])

    baseline_t = raw.get("baseline", {})
    tool_t = raw.get("toolchain", {})
    cc1_key = "cc1_reference" if use_reference else "cc1"
    cc1_where = f"[toolchain.{cc1_key}]"
    cc1_t = tool_t.get(cc1_key, {}) if isinstance(tool_t, dict) else {}
    for name, table in (("baseline", baseline_t), ("toolchain", tool_t), (f"toolchain.{cc1_key}", cc1_t)):
        if not isinstance(table, dict) or not table:
            errors.append(f"[{name}] section is missing")
            if name == "baseline":
                baseline_t = {}
            elif name == "toolchain":
                tool_t = {}
            else:
                cc1_t = {}

    baseline_rel = _get_str(baseline_t, "executable", "[baseline]", errors)
    baseline_sha = _get_hex(baseline_t, "sha256", "[baseline]", errors)
    cpp = _get_str(tool_t, "cpp", "[toolchain]", errors)
    maspsx = _get_str(tool_t, "maspsx", "[toolchain]", errors)
    maspsx_commit = _get_str(tool_t, "maspsx_commit", "[toolchain]", errors, re.compile(r"[0-9a-f]{7,40}\Z"))
    aspsx_version = _get_str(tool_t, "aspsx_version", "[toolchain]", errors, re.compile(r"[0-9.]+\Z"))
    prefix = _get_str(tool_t, "binutils_prefix", "[toolchain]", errors, re.compile(r"[A-Za-z0-9_.-]+\Z"))
    expand_div = tool_t.get("expand_div", False)
    if not isinstance(expand_div, bool):
        errors.append("[toolchain]: 'expand_div' must be a boolean")
        expand_div = False
    include_dirs: list[Path] = []
    include_t = tool_t.get("include_dirs", [])
    if not isinstance(include_t, list) or not all(isinstance(item, str) and item for item in include_t):
        errors.append("[toolchain]: 'include_dirs' must be a list of non-empty strings")
    else:
        for item in include_t:
            path = _expand(item, directory)
            if not path.is_dir():
                errors.append(f"[toolchain]: include directory not found: {item}")
            elif path in include_dirs:
                errors.append(f"[toolchain]: include directory listed twice: {item}")
            else:
                include_dirs.append(path)

    kind = _get_str(cc1_t, "kind", cc1_where, errors)
    cc1 = Cc1Config(kind=kind, sha256=_get_hex(cc1_t, "sha256", cc1_where, errors), name=cc1_key)
    no_float = cc1_t.get("no_float", False)
    if not isinstance(no_float, bool):
        errors.append(f"{cc1_where}: 'no_float' must be a boolean")
    else:
        cc1.no_float = no_float
    if kind == "remote":
        cc1.host = _get_str(cc1_t, "host", cc1_where, errors, HOST_RE)
        cc1.socket = _expand(_get_str(cc1_t, "socket", cc1_where, errors) or ".", directory)
        cc1.scp_dir = _get_str(cc1_t, "scp_dir", cc1_where, errors, REMOTE_PATH_RE)
        cc1.exec_dir = _get_str(cc1_t, "exec_dir", cc1_where, errors, REMOTE_PATH_RE)
        cc1.exec_prefix = _get_str(cc1_t, "exec_prefix", cc1_where, errors)
        cc1.binary = _get_str(cc1_t, "binary", cc1_where, errors, re.compile(r"[A-Za-z0-9_.-]+\Z"))
    elif kind == "local":
        cc1.path = _expand(_get_str(cc1_t, "path", cc1_where, errors) or ".", directory)
    elif kind:
        errors.append(f"{cc1_where}: kind must be 'remote' or 'local', got {kind!r}")

    # Module images: structure only. The archives are read once the baseline is known.
    overlays = raw.get("overlays")
    if overlays is not None and not isinstance(overlays, dict):
        errors.append("[overlays] must be a table")
        overlays = None
    images, declared = parse_images(raw, directory, errors)
    if images and overlays is None:
        errors.append("[[image]] needs an [overlays] section with 'table_pointers'")

    # Units: structure and geometry that needs no baseline.
    units: list[UnitDecl] = []
    unit_tables = raw.get("unit", [])
    if not isinstance(unit_tables, list) or not unit_tables:
        errors.append("at least one [[unit]] is required")
        unit_tables = []
    seen_units: set[str] = set()
    seen_functions: dict[str, str] = {}
    for index, table in enumerate(unit_tables):
        where = f"[[unit]] #{index + 1}"
        if not isinstance(table, dict):
            errors.append(f"{where}: must be a table")
            continue
        name = _get_str(table, "name", where, errors, IDENT_RE)
        if name:
            where = f"unit {name!r}"
            if name in seen_units:
                errors.append(f"{where}: duplicate unit name")
            seen_units.add(name)
        source = _get_str(table, "source", where, errors)
        if source and not _expand(source, directory).is_file():
            errors.append(f"{where}: source file not found: {source}")
        kind = table.get("kind", "c")
        if kind not in ("c", "asm"):
            errors.append(f"{where}: kind must be 'c' or 'asm'")
            kind = "c"
        image_name = table.get("image", RESIDENT)
        if not isinstance(image_name, str) or not image_name:
            errors.append(f"{where}: 'image' must be a non-empty string")
            image_name = RESIDENT
        elif image_name != RESIDENT and image_name not in declared:
            errors.append(f"{where}: image {image_name!r} is not declared")
        flags = table.get("flags", [])
        if not isinstance(flags, list) or not all(isinstance(f, str) and FLAG_RE.match(f) for f in flags):
            errors.append(f"{where}: 'flags' must be a list of plain option strings")
            flags = []
        if kind == "asm" and flags:
            errors.append(f"{where}: an assembly unit takes no flags")
        functions = []
        fn_tables = table.get("functions", [])
        if not isinstance(fn_tables, list) or not fn_tables:
            errors.append(f"{where}: 'functions' must be a non-empty list")
            fn_tables = []
        for fn_table in fn_tables:
            if not isinstance(fn_table, dict):
                errors.append(f"{where}: each function must be a table")
                continue
            fn_name = _get_str(fn_table, "name", where, errors, IDENT_RE)
            address, size = fn_table.get("address"), fn_table.get("size")
            ok = True
            for label, value in (("address", address), ("size", size)):
                if not isinstance(value, int) or isinstance(value, bool):
                    errors.append(f"{where} function {fn_name!r}: '{label}' must be an integer")
                    ok = False
                elif value % 4:
                    errors.append(f"{where} function {fn_name!r}: {label} {value:#x} is not a multiple of four")
                    ok = False
            if ok and size <= 0:
                errors.append(f"{where} function {fn_name!r}: size must be greater than zero")
                ok = False
            if fn_name:
                if fn_name in seen_functions:
                    errors.append(
                        f"duplicate function name {fn_name!r} (units {seen_functions[fn_name]!r} and {name!r})"
                    )
                seen_functions[fn_name] = name
            if fn_name and ok:
                functions.append(FunctionDecl(fn_name, address, size))
        rodata, rodata_ok = parse_rodata(table.get("rodata"), where, errors)
        data, data_ok = parse_rodata(table.get("data"), where, errors, "data")
        bss, bss_ok = parse_rodata(table.get("bss"), where, errors, "bss")
        if not name or not functions or len(functions) != len(fn_tables) or not (rodata_ok and data_ok and bss_ok):
            continue
        functions.sort(key=lambda f: f.address)
        contiguous = True
        for a, b in zip(functions, functions[1:]):
            if a.address + a.size != b.address:
                errors.append(
                    f"{where}: gap or overlap between functions {a.name!r} and {b.name!r} "
                    f"(function ends at {a.address + a.size:#x}, next begins at {b.address:#x})"
                )
                contiguous = False
        if contiguous:
            units.append(UnitDecl(name, source, tuple(flags), tuple(functions), rodata, data, bss, kind, image_name))

    # Overlap is judged among the units of one image.
    errors += geometry_errors([u for u in units if u.image == RESIDENT])
    for name in dict.fromkeys(u.image for u in units if u.image in declared):
        errors += [f"image {name!r}: {e}" for e in geometry_errors([u for u in units if u.image == name])]

    # symbols.ld
    symbols_path = directory / "symbols.ld"
    symbol_names: list[str] = []
    try:
        symbol_names = parse_symbols(symbols_path.read_text(), errors)
    except OSError as exc:
        errors.append(f"cannot read {symbols_path}: {exc}")
    for image in images:
        errors += [
            f"image {image.name!r}: symbols names {key!r}, which symbols.ld does not assign"
            for key in image.symbols
            if key not in symbol_names
        ]
    for fn_name, unit_name in seen_functions.items():
        if fn_name in symbol_names:
            errors.append(
                f"symbols.ld defines unit function {fn_name!r} (unit {unit_name!r}); "
                f"the assignment would override the compiled symbol"
            )

    # Shared types: the generated header must not be shadowed by a source-side file.
    types_fields: Path | None = None
    types_header = ""
    types_t = raw.get("types")
    if types_t is not None:
        if not isinstance(types_t, dict):
            errors.append("[types] must be a table")
        else:
            fields_rel = _get_str(types_t, "fields", "[types]", errors)
            types_header = _get_str(types_t, "header", "[types]", errors, re.compile(r"[A-Za-z0-9_.-]+\Z"))
            if fields_rel:
                types_fields = _expand(fields_rel, directory)
                if not types_fields.is_file():
                    errors.append(f"[types]: fields file not found: {fields_rel}")
            if types_header:
                source_dirs = {directory} | {_expand(u.source, directory).parent for u in units} | set(include_dirs)
                for source_dir in sorted(source_dirs):
                    if (source_dir / types_header).exists():
                        errors.append(
                            f"{source_dir / types_header} would shadow the generated header {types_header!r}: "
                            f"quoted includes resolve next to the including file first"
                        )

    config = Config(
        path=config_path,
        directory=directory,
        baseline_path=_expand(baseline_rel, directory) if baseline_rel else directory,
        baseline_sha256=baseline_sha,
        cpp=cpp,
        maspsx=_expand(maspsx, directory) if maspsx else directory,
        maspsx_commit=maspsx_commit,
        aspsx_version=aspsx_version,
        binutils_prefix=prefix,
        cc1=cc1,
        units=units,
        symbols_path=symbols_path,
        symbol_names=symbol_names,
        types_fields=types_fields,
        types_header=types_header,
        expand_div=expand_div,
        include_dirs=tuple(include_dirs),
    )

    # Baseline executable.
    if baseline_rel:
        try:
            config.baseline = config.baseline_path.read_bytes()
        except OSError as exc:
            errors.append(f"cannot read baseline executable {config.baseline_path}: {exc}")
    if config.baseline:
        if baseline_sha and sha256(config.baseline) != baseline_sha:
            errors.append(
                f"baseline sha256 mismatch: file is {sha256(config.baseline)}, configuration pins {baseline_sha}"
            )
        if config.baseline[:8] != EXE_MAGIC:
            errors.append("baseline is not a PS-X EXE (bad magic)")
        elif len(config.baseline) < HEADER_SIZE:
            errors.append("baseline is shorter than the PS-X EXE header")
        else:
            config.load = int.from_bytes(config.baseline[0x18:0x1C], "little")
            config.payload_size = int.from_bytes(config.baseline[0x1C:0x20], "little")
            if len(config.baseline) != HEADER_SIZE + config.payload_size:
                errors.append(
                    f"baseline size {len(config.baseline)} != header {HEADER_SIZE} + payload {config.payload_size}"
                )
            else:
                errors += payload_errors(config.units_of(RESIDENT), config.load, config.payload_size)
                if images and overlays is not None:
                    config.images = read_images(images, overlays, config.baseline, errors)
                    for image in config.images:
                        errors += [
                            f"image {image.name!r}: {e}"
                            for e in payload_errors(config.units_of(image.name), image.address, image.size)
                        ]
    if errors:
        raise ConfigError(errors)
    return config


# ---------------------------------------------------------------------------
# Subprocess helpers


def run(
    args,
    *,
    cwd: Path | None = None,
    input: bytes | None = None,
    env: dict | None = None,
    step: str,
) -> subprocess.CompletedProcess:
    """Run a command and enforce its exit status."""
    argv = [str(a) for a in args]
    try:
        result = subprocess.run(argv, cwd=cwd, input=input, env=env, capture_output=True)
    except OSError as exc:
        raise StepError(f"{step}: cannot execute {argv[0]}: {exc}")
    if result.returncode != 0:
        output = (result.stderr + result.stdout).decode(errors="replace").strip()
        raise StepError(f"{step} failed (exit {result.returncode}): {output}")
    return result


def first_line(text: bytes) -> str:
    lines = text.decode(errors="replace").strip().splitlines()
    return lines[0] if lines else ""


class Compiler:
    """The cc1 backend, remote over an SSH control master or local."""

    def __init__(self, cfg: Cc1Config, tag: str):
        self.cfg = cfg
        self.tag = tag

    # remote helpers
    def _ssh(self, command: str, step: str) -> subprocess.CompletedProcess:
        c = self.cfg
        return run(["ssh", "-S", c.socket, "-o", "BatchMode=yes", c.host, command], step=step)

    def _scp(self, source: str, dest: str, step: str) -> None:
        c = self.cfg
        run(["scp", "-q", "-o", f"ControlPath={c.socket}", "-o", "BatchMode=yes", source, dest], step=step)

    def check_master(self) -> None:
        c = self.cfg
        if c.kind != "remote":
            return
        result = subprocess.run(
            ["ssh", "-S", str(c.socket), "-O", "check", c.host], capture_output=True
        )
        if result.returncode != 0:
            raise EnvironmentFailure(
                f"no SSH control master on {c.socket} for {c.host}: "
                f"{(result.stderr + result.stdout).decode(errors='replace').strip()}"
            )

    def actual_hash(self) -> str:
        c = self.cfg
        if c.kind == "local":
            if not c.path.is_file():
                raise StepError(f"cc1 not found: {c.path}")
            return sha256(c.path.read_bytes())
        out = self._ssh(f"{c.exec_prefix} sha256sum {c.exec_dir}/{c.binary}", "remote cc1 hash")
        word = out.stdout.decode(errors="replace").split()
        if not word or not re.fullmatch(r"[0-9a-f]{64}", word[0]):
            raise StepError(f"remote cc1 hash: unexpected output {out.stdout!r}")
        return word[0]

    def compile(self, unit: str, flags: tuple[str, ...], source_i: Path, out_s: Path, log: Path) -> None:
        c = self.cfg
        if c.kind == "local":
            result = run([c.path, "-quiet", *flags, source_i, "-o", out_s], step=f"compile {unit}")
            log.write_bytes(result.stdout + result.stderr)
            return
        stem = f"mb-{self.tag}-{unit}"
        remote_i, remote_s = f"{c.exec_dir}/{stem}.i", f"{c.exec_dir}/{stem}.s"
        self._remote_cleanup(stem)
        try:
            self._scp(str(source_i), f"{c.host}:{c.scp_dir}/{stem}.i", f"upload {unit}")
            command = (
                f"{c.exec_prefix} {c.exec_dir}/{c.binary} -quiet {' '.join(flags)} {remote_i} -o {remote_s}"
            )
            result = self._ssh(command, f"compile {unit}")
            log.write_bytes(result.stdout + result.stderr)
            self._scp(f"{c.host}:{c.scp_dir}/{stem}.s", str(out_s), f"download {unit}")
        finally:
            self._remote_cleanup(stem)

    def _remote_cleanup(self, stem: str) -> None:
        c = self.cfg
        self._ssh(f"{c.exec_prefix} rm -f {c.exec_dir}/{stem}.i {c.exec_dir}/{stem}.s", "remote cleanup")


# ---------------------------------------------------------------------------
# Build


def check_unit_object(obj_path: Path, unit: UnitDecl) -> list[str]:
    """Check one unit object.

    Allocated sections: `.text` must equal the declared range, read-only data
    (`.rodata`, `.rdata`), `.data` and bss (`.bss`, `.sbss`), each rounded up to
    a multiple of four, must equal the declared range of its kind, anything
    else non-empty is rejected, and so are common symbols.
    """
    errors = []
    declared = unit.end - unit.start
    with open(obj_path, "rb") as handle:
        elf = ELFFile(handle)
        text_size = 0
        sizes = {"rodata": 0, "data": 0, "bss": 0}
        for section in elf.iter_sections():
            name = section.name
            if not name or not (section["sh_flags"] & SHF_ALLOC):
                continue
            if name == ".text":
                text_size = section["sh_size"]
            elif name in DISCARDED_SECTIONS:
                continue
            elif name in RODATA_SECTIONS:
                sizes["rodata"] += section["sh_size"]
            elif name in LOADED_SECTIONS["data"]:
                sizes["data"] += section["sh_size"]
            elif name in BSS_SECTIONS:
                sizes["bss"] += section["sh_size"]
            elif section["sh_size"] > 0:
                errors.append(
                    f"unit {unit.name!r}: non-empty allocated section {name} ({section['sh_size']} bytes); "
                    f"only .text, read-only data, data and bss are owned by a unit"
                )
        symtab = elf.get_section_by_name(".symtab")
        if symtab is not None:
            for sym in symtab.iter_symbols():
                if sym["st_shndx"] == "SHN_COMMON":
                    errors.append(f"unit {unit.name!r}: common symbol {sym.name!r} (only .text, read-only data, data and bss are owned by a unit)")
        what = {"rodata": "read-only data", "data": "data", "bss": "bss"}
        for kind, decl in (("rodata", unit.rodata), ("data", unit.data), ("bss", unit.bss)):
            size = sizes[kind]
            if decl is None and size:
                errors.append(
                    f"unit {unit.name!r}: the object has {size} bytes of {what[kind]}, "
                    f"so {kind} must be declared ({kind} = {{ address = .., size = {size} }})"
                )
            elif decl is not None and size == 0:
                errors.append(f"unit {unit.name!r}: declares {kind} ({decl.size} bytes) but the object has none")
            elif decl is not None and (size + 3) // 4 * 4 != decl.size:
                errors.append(
                    f"unit {unit.name!r}: {kind} size mismatch: the object has {size} bytes of "
                    f"{what[kind]}, declared {decl.size}"
                )
        if text_size != declared:
            errors.append(
                f"unit {unit.name!r}: text size mismatch: .text is {text_size} bytes, declared range is {declared}"
            )
    return errors


def defined_symbols(obj_path: Path) -> list[DefinedSymbol]:
    """The global and weak symbols a unit object defines, with their offset in the unit's range of that kind."""
    found = []
    with open(obj_path, "rb") as handle:
        elf = ELFFile(handle)
        symtab = elf.get_section_by_name(".symtab")
        if symtab is None:
            return found
        sizes = {section.name: section["sh_size"] for section in elf.iter_sections()}
        for sym in symtab.iter_symbols():
            index = sym["st_shndx"]
            if sym["st_info"]["bind"] not in ("STB_GLOBAL", "STB_WEAK") or index in ("SHN_UNDEF", "SHN_COMMON") or not sym.name:
                continue
            section = "*ABS*" if index == "SHN_ABS" else (elf.get_section(index).name if isinstance(index, int) else None)
            kind = next((k for k, names in OWNED_SECTIONS.items() if section in names), "other")
            if kind == "other":
                found.append(DefinedSymbol(sym.name, kind, section or str(index), sym["st_value"]))
                continue
            names = OWNED_SECTIONS[kind]
            before = sum(sizes.get(name, 0) for name in names[: names.index(section)])
            found.append(DefinedSymbol(sym.name, kind, section, sym["st_value"] + before))
    return found


def symbol_address(unit: UnitDecl, sym: DefinedSymbol) -> int | None:
    """Where the link puts a symbol of the unit, None when its kind is not declared or not owned."""
    if sym.kind == "text":
        return unit.start + sym.offset
    decl = {"rodata": unit.rodata, "data": unit.data, "bss": unit.bss}.get(sym.kind)
    return None if decl is None else decl.address + sym.offset


def symbol_override_errors(units: list[UnitDecl], defined: dict[str, list[DefinedSymbol]], symbol_names: list[str]) -> list[str]:
    """Names that `symbols.ld` assigns although a unit object defines them."""
    errors = []
    assigned = set(symbol_names)
    for unit in units:
        for sym in defined.get(unit.name, []):
            if sym.name in assigned:
                errors.append(
                    f"symbols.ld defines {sym.name!r}, which unit {unit.name!r} defines in {sym.section}; "
                    f"the assignment would override the symbol"
                )
    return errors


def other_image_override_errors(
    units: list[UnitDecl], defined: dict[str, list[DefinedSymbol]], others: list[tuple[str, int, str]]
) -> list[str]:
    """Names that `others.ld` assigns although a unit object of the same link defines them."""
    errors = []
    declared_by = {name: image for name, _, image in others}
    for unit in units:
        for sym in defined.get(unit.name, []):
            if sym.name in declared_by:
                errors.append(
                    f"others.ld defines {sym.name!r}, which unit {unit.name!r} defines in {sym.section} "
                    f"and image {declared_by[sym.name]!r} declares as a function; "
                    f"the assignment would override the symbol"
                )
    return errors


def elf_symbol_checks(elf_path: Path, units: list[UnitDecl], defined: dict[str, list[DefinedSymbol]]) -> list[str]:
    """Each symbol a unit defines is bound in its output section at the declared address plus its offset."""
    errors = []
    with open(elf_path, "rb") as handle:
        elf = ELFFile(handle)
        symtab = elf.get_section_by_name(".symtab")
        # Locals of other units may share a name; only a global or weak symbol takes part in the link.
        linked = {}
        if symtab is not None:
            for sym in symtab.iter_symbols():
                if sym["st_info"]["bind"] in ("STB_GLOBAL", "STB_WEAK"):
                    linked[sym.name] = sym
        for unit in units:
            for sym in defined.get(unit.name, []):
                want = symbol_address(unit, sym)
                if want is None:
                    continue
                section = f".{sym.kind}.{unit.name}"
                got = linked.get(sym.name)
                if got is None:
                    where = "missing"
                elif got["st_shndx"] == "SHN_ABS":
                    where = f"an absolute address {got['st_value']:#x}"
                elif not isinstance(got["st_shndx"], int) or elf.get_section(got["st_shndx"]).name != section:
                    other = elf.get_section(got["st_shndx"]).name if isinstance(got["st_shndx"], int) else got["st_shndx"]
                    where = f"{other} at {got['st_value']:#x}"
                elif got["st_value"] != want:
                    where = f"{section} at {got['st_value']:#x}"
                else:
                    continue
                errors.append(
                    f"unit {unit.name!r}: symbol {sym.name!r} is bound to {where} in the linked ELF, "
                    f"expected {section} at {want:#x}"
                )
    return errors


def elf_function_checks(elf_path: Path, units: list[UnitDecl]) -> list[str]:
    """Each declared function exists in the linked ELF with the declared address and size."""
    errors = []
    with open(elf_path, "rb") as handle:
        elf = ELFFile(handle)
        symtab = elf.get_section_by_name(".symtab")
        symbols = {s.name: s for s in symtab.iter_symbols()} if symtab is not None else {}
        for unit in units:
            for fn in unit.functions:
                sym = symbols.get(fn.name)
                if sym is None:
                    errors.append(f"function {fn.name!r}: symbol missing from the linked ELF")
                    continue
                if sym["st_value"] != fn.address:
                    errors.append(
                        f"function {fn.name!r}: address mismatch: ELF {sym['st_value']:#x}, declared {fn.address:#x}"
                    )
                if sym["st_size"] != fn.size:
                    errors.append(
                        f"function {fn.name!r}: size mismatch: ELF symbol size {sym['st_size']}, declared {fn.size}"
                    )
            if unit.bss is not None:
                section = elf.get_section_by_name(f".bss.{unit.name}")
                if section is None or section["sh_addr"] != unit.bss.address:
                    got = "missing" if section is None else f"{section['sh_addr']:#x}"
                    errors.append(
                        f"unit {unit.name!r}: bss section is {got} in the linked ELF, declared {unit.bss.address:#x}"
                    )
    return errors


def generate_raw_and_linker(
    cfg: Config,
    build: Path,
    load: int,
    units: list[UnitDecl],
    ranges: list[tuple[int, int]],
    others: list[tuple[str, int, str]] = (),
    local: dict[str, int] | None = None,
) -> None:
    """Write raw.s and link.ld of one image into `build`. The unit objects are named relative to its parent for a module image.

    With module images declared, `others` (the functions of the other images) goes to others.ld
    next to the linker script, which includes it after symbols.ld. The addresses of a module
    image's own [image.symbols] go to local.ld, included after others.ld; without any, no file.
    """
    raw_lines = []
    for index, (address, size) in enumerate(ranges):
        raw_lines.append(f'.section .raw{index},"a",@progbits')
        raw_lines.append(f'.incbin "payload.bin", {address - load}, {size}')
    (build / "raw.s").write_text("\n".join(raw_lines) + "\n")

    entries = [(address, f".raw{index}", f"*(.raw{index})", "") for index, (address, _) in enumerate(ranges)]
    for unit in units:
        patterns = " ".join(f"unit-{unit.name}.o({s})" for s in OWNED_SECTIONS["text"])
        entries.append((unit.start, f".text.{unit.name}", patterns, ""))
        for kind, decl in unit.loaded():
            patterns = " ".join(f"unit-{unit.name}.o({s})" for s in OWNED_SECTIONS[kind])
            # The object may end up to three bytes short of the declared range.
            # Fill to the declared size so that the unit owns the padding.
            entries.append((decl.address, f".{kind}.{unit.name}", f"{patterns} . = {decl.size:#x};", ""))
        if unit.bss is not None:
            # NOLOAD: the section takes no bytes of the flat image.
            patterns = " ".join(f"unit-{unit.name}.o({s})" for s in OWNED_SECTIONS["bss"])
            entries.append((unit.bss.address, f".bss.{unit.name}", patterns, " (NOLOAD)"))
    entries.sort()
    lines = ["OUTPUT_ARCH(mips)", f'INCLUDE "{cfg.symbols_path}"']
    if cfg.images:
        (build / "others.ld").write_text("".join(f"{name} = {address:#x};\n" for name, address, _ in others))
        lines.append(f'INCLUDE "{(build / "others.ld").resolve()}"')
    (build / "local.ld").unlink(missing_ok=True)  # no stale file from an earlier build in this directory
    if local:
        (build / "local.ld").write_text("".join(f"{name} = {address:#x};\n" for name, address in local.items()))
        lines.append(f'INCLUDE "{(build / "local.ld").resolve()}"')
    lines.append("SECTIONS {")
    for address, name, pattern, attrs in entries:
        lines.append(f" {name} {address:#x}{attrs} : SUBALIGN(1) {{ {pattern} }}")
    lines.append(" /DISCARD/ : { " + " ".join(f"*({s})" for s in DISCARDED_SECTIONS) + " *(.note*) }")
    lines.append("}")
    (build / "link.ld").write_text("\n".join(lines) + "\n")


def export_maspsx(cfg: Config, build: Path) -> tuple[list[str], dict, Path | None]:
    """Export the pinned maspsx commit into the build directory.

    The build runs this exported copy, never the checkout's working tree, so
    local edits, a different HEAD or stale bytecode in the checkout cannot
    change the code that runs. Returns (errors, tool record, script path).
    """
    git = ["git", "-C", cfg.maspsx.parent]
    try:
        top = Path(run([*git, "rev-parse", "--show-toplevel"], step="maspsx repository").stdout.decode().strip())
        commit = run(
            [*git, "rev-parse", "--verify", "--quiet", f"{cfg.maspsx_commit}^{{commit}}"],
            step="maspsx pinned commit",
        ).stdout.decode().strip()
    except StepError as exc:
        return [f"maspsx commit {cfg.maspsx_commit} is not available in {cfg.maspsx.parent}: {exc}"], {}, None
    head = run([*git, "rev-parse", "HEAD"], step="maspsx head").stdout.decode().strip()
    dirty = bool(run([*git, "status", "--porcelain"], step="maspsx status").stdout.strip())
    script_rel = Path(os.path.realpath(cfg.maspsx)).relative_to(os.path.realpath(top))
    record = {
        "repository": str(top),
        "script": str(script_rel),
        "pinned_commit": commit,
        "executed": "export of the pinned commit",
        "checkout_head": head,
        "checkout_dirty": dirty,
    }
    # Run from the top level: git archive only covers the current directory's subtree.
    archive = run(["git", "-C", top, "archive", "--format=tar", commit], step="maspsx export").stdout
    export_dir = build / "maspsx"
    export_dir.mkdir()
    with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
        tar.extractall(export_dir, filter="data")
    script = export_dir / script_rel
    if not script.is_file():
        return [f"maspsx script {script_rel} does not exist in pinned commit {commit}"], record, None
    return [], record, script


def check_pins(cfg: Config, compiler: Compiler, build: Path) -> tuple[list[str], dict, Path | None]:
    tools: dict = {}
    errors, tools["maspsx"], maspsx_script = export_maspsx(cfg, build)
    # cc1 hash
    actual = compiler.actual_hash()
    tools["cc1"] = {
        "name": cfg.cc1.name,
        "kind": cfg.cc1.kind,
        "sha256": actual,
        "pinned_sha256": cfg.cc1.sha256,
        "no_float": cfg.cc1.no_float,
    }
    if actual != cfg.cc1.sha256:
        errors.append(f"cc1 sha256 mismatch: binary is {actual}, configuration pins {cfg.cc1.sha256}")
    return errors, tools, maspsx_script


def resolve_executable(name: str, step: str) -> Path:
    """The file a bare tool name runs, with symlinks resolved."""
    found = shutil.which(name)
    if found is None:
        raise StepError(f"{step}: cannot find {name} on PATH")
    return Path(os.path.realpath(found))


def collect_versions(cfg: Config, tools: dict) -> None:
    tools["python"] = sys.version.split()[0]
    python = Path(os.path.realpath(sys.executable))
    tools["python_executable"] = {"path": str(python), "version": sys.version, "sha256": file_sha(python)}
    tools["cpp"] = {"path": cfg.cpp, "version": first_line(run([cfg.cpp, "--version"], step="cpp version").stdout)}
    for tool in ("as", "ld", "objcopy"):
        name = cfg.binutils_prefix + tool
        # Identify the executable by content. The banner is self-reported and
        # two different builds can print the same line. Later steps run this
        # resolved file, not the bare name.
        path = resolve_executable(name, f"{tool} version")
        tools[tool] = {
            "name": name,
            "path": str(path),
            "sha256": file_sha(path),
            "version": first_line(run([path, "--version"], step=f"{tool} version").stdout),
        }
    tools["aspsx_version"] = cfg.aspsx_version


def file_sha(path: Path) -> str:
    return sha256(path.read_bytes())


def generate_types(cfg: Config, build: Path) -> Path:
    """Generate and layout-check the shared header into <build>/gen. Raises FieldsError."""
    gen = build / "gen"
    gen.mkdir()
    model = structgen.parse(cfg.types_fields.read_text(), str(cfg.types_fields))
    (gen / cfg.types_header).write_text(structgen.generate_header(model, cfg.types_header))
    structgen.check_layout(model, cfg.cpp, cfg.types_header, gen)
    return gen


def maspsx_flags(cfg: Config) -> list[str]:
    """Options passed to maspsx besides the assembler version."""
    return ["--expand-div"] if cfg.expand_div else []


def cache_key_inputs(
    cfg: Config, tools: dict, unit: UnitDecl, preprocessed_sha256: str, maspsx_script: Path
) -> dict:
    """Everything that can change the unit object, in readable form.

    The preprocessed text enters only as its hash. The unit name is included
    because the compiler may record the input file name, which derives from it.
    The maspsx script path is included because the pinned commit holds more
    than one script. The assembler and the Python interpreter that runs maspsx
    enter by the content hash of the resolved executable plus their version.
    """
    return {
        "format": CACHE_FORMAT,
        "preprocessed_sha256": preprocessed_sha256,
        "unit": unit.name,
        "cc1_flags": list(unit.flags),
        "cc1_sha256": tools["cc1"]["sha256"],
        "cc1_table": cfg.cc1.name,
        "maspsx_commit": tools["maspsx"]["pinned_commit"],
        "maspsx_script": tools["maspsx"]["script"],
        "aspsx_version": cfg.aspsx_version,
        "maspsx_flags": maspsx_flags(cfg),
        "as_flags": list(AS_FLAGS),
        "as_version": tools["as"]["version"],
        "as_sha256": tools["as"]["sha256"],
        "python_version": tools["python_executable"]["version"],
        "python_sha256": tools["python_executable"]["sha256"],
    }


def cache_key(inputs: dict) -> str:
    canonical = json.dumps(inputs, sort_keys=True, separators=(",", ":"), ensure_ascii=True)
    return sha256(canonical.encode("ascii"))


def _read_entry(entry: Path, key: str) -> dict:
    """Parse and validate entry.json. Any malformed content raises one of CACHE_READ_ERRORS."""
    record = json.loads((entry / "entry.json").read_bytes())
    if not isinstance(record, dict):
        raise ValueError("entry metadata is not a mapping")
    if not isinstance(record.get("key"), str) or record["key"] != key:
        raise ValueError("entry records a different key")
    files = record.get("files")
    if not isinstance(files, dict):
        raise ValueError("entry file list is not a mapping")
    if set(files) != set(CACHE_PAYLOAD) or not all(
        isinstance(v, str) and re.fullmatch(r"[0-9a-f]{64}", v) for v in files.values()
    ):
        raise ValueError("entry file hashes are malformed")
    return files


# RecursionError: deeply nested JSON. UnicodeDecodeError and JSONDecodeError are ValueErrors.
CACHE_READ_ERRORS = (OSError, ValueError, KeyError, TypeError, AttributeError, RecursionError, MemoryError)


def cache_valid(entry: Path, key: str) -> bool:
    """True when the entry is complete and every file matches its recorded hash."""
    try:
        files = _read_entry(entry, key)
        return all(file_sha(entry / name) == files[name] for name in CACHE_PAYLOAD)
    except CACHE_READ_ERRORS:
        return False


def cache_fetch(entry: Path, key: str, dests: dict[str, Path], before_copy=None) -> bool:
    """Copy an entry into the build directory. True only for a verified hit.

    The entry may be replaced or removed by another run at any point. So the
    metadata is read once, the files are copied, and each copy is hashed
    against that metadata. Any error or mismatch is a miss and leaves no
    partial files. `before_copy` is a test hook between the read and the copy.
    """
    try:
        files = _read_entry(entry, key)
        if before_copy is not None:
            before_copy()
        for name in CACHE_PAYLOAD:
            shutil.copyfile(entry / name, dests[name])
            if file_sha(dests[name]) != files[name]:
                raise ValueError(f"{name} does not match the entry metadata")
        return True
    except CACHE_READ_ERRORS:
        for dest in dests.values():
            dest.unlink(missing_ok=True)
        return False


def cache_store(
    cache_dir: Path, key: str, inputs: dict, obj: Path, asm: Path, gnu: Path, before_publish=None
) -> None:
    """Publish an entry atomically. Never replaces a valid entry.

    A valid entry published by another run is kept. Only an invalid one is
    moved aside. `before_publish` is a test hook run just before publication.
    """
    cache_dir.mkdir(parents=True, exist_ok=True)
    entry = cache_dir / key
    temp = Path(tempfile.mkdtemp(prefix=".tmp-", dir=cache_dir))
    aside = temp.with_name(temp.name + ".old")
    try:
        for name, source in zip(CACHE_PAYLOAD, (obj, asm, gnu)):
            shutil.copyfile(source, temp / name)
        # Hash the stored copies, not the sources, so the record matches the files.
        files = {name: file_sha(temp / name) for name in CACHE_PAYLOAD}
        record = {"object_sha256": files["unit.o"], "files": files, "key": key, "inputs": inputs}
        (temp / "entry.json").write_text(json.dumps(record, indent=2, sort_keys=True) + "\n")
        if before_publish is not None:
            before_publish()
        if cache_valid(entry, key):
            return
        if entry.exists():
            # Rename cannot replace a non-empty directory. Move the broken
            # entry aside, then look at what was actually moved: a valid entry
            # may have been published between the check and the rename.
            try:
                os.rename(entry, aside)
            except OSError:
                pass
            else:
                if cache_valid(aside, key):
                    try:
                        os.rename(aside, entry)
                        return
                    except OSError:
                        pass
        try:
            os.rename(temp, entry)
        except OSError:
            pass  # a concurrent run published first; use or ignore its entry
    finally:
        shutil.rmtree(temp, ignore_errors=True)
        if aside.is_dir() and not aside.is_symlink():
            shutil.rmtree(aside, ignore_errors=True)
        else:
            try:
                aside.unlink(missing_ok=True)  # a broken entry that was a plain file
            except OSError:
                pass


def count_carriers(image: ImageDecl) -> int:
    """The chunks with table number 0, the image's slot and the image's bytes, in the files of the archive's directory.

    The files are the `*.PAC` files below the directory of the declared
    archive, as `pac.py` finds them, and the declared archive itself. A file
    that the archive reader rejects takes no part.
    """
    files = {p.resolve() for p in image.archive.parent.rglob("*.PAC")} | {image.archive.resolve()}
    count = 0
    for path in sorted(files):
        try:
            data = path.read_bytes()
            chunks = pac.chunk_table(data)
        except (OSError, pac.FormatError):
            continue
        count += sum(
            1
            for c in chunks
            if c["table"] == 0 and c["slot"] == image.slot and c["size"] == image.size
            and data[c["offset"] : c["offset"] + c["size"]] == image.payload
        )
    return count


def comparison_failures(comparison: ImageComparison) -> list[str]:
    """The failure reasons of one comparison: every range that differs, then the image as a whole.

    A unit's ranges cover only what units own. The line for the image is the
    one that reports a difference in a retained byte.
    """
    failures = []
    for fn in comparison.functions:
        if not fn.exact:
            failures.append(
                f"function {fn.name!r}: bytes differ from baseline at offset {fn.first_diff} "
                f"({fn.equal_words}/{fn.total_words} words equal)"
            )
    for ro in comparison.rodata + comparison.data:
        if not ro.exact:
            failures.append(
                f"unit {ro.unit!r} {ro.kind}: bytes differ from baseline at offset {ro.first_diff} "
                f"(range {ro.address:#x}, {ro.size} bytes)"
            )
    if not comparison.image_exact:
        failures.append(
            f"image differs from baseline payload (size {comparison.image_size} vs {comparison.baseline_size}, "
            f"sha256 {comparison.image_sha256} vs {comparison.baseline_sha256})"
        )
    return failures


def link_image(
    cfg: Config,
    build: Path,
    outdir: Path,
    load: int,
    payload: bytes,
    units: list[UnitDecl],
    defined: dict[str, list[DefinedSymbol]],
    cache_units: dict,
    assembler: str,
    header: bytes | None = None,
    image: str = RESIDENT,
    local: dict[str, int] | None = None,
) -> tuple[dict, list[str]]:
    """Link one image alone from its unit objects in `build` and check it against its payload.

    The files of the link go to `outdir`, which is `build` for the resident
    image. With a `header` the executable is rebuilt and compared too. Returns
    the fields shared by the reports of every image and the failures, which
    do not name the image.
    """
    failures: list[str] = []
    outdir.mkdir(exist_ok=True)
    (outdir / "payload.bin").write_bytes(payload)
    prefix = cfg.binutils_prefix
    where = "" if outdir == build else f"{outdir.name}/"  # the link runs in `build`
    ranges = raw_ranges(load, len(payload), units)
    generate_raw_and_linker(cfg, outdir, load, units, ranges, cfg.functions_of_others(image), local)
    run([assembler, *AS_FLAGS, "-o", "raw.o", "raw.s"], cwd=outdir, step="assemble raw")
    objects = [f"unit-{u.name}.o" for u in units] + [f"{where}raw.o"]
    run(
        [prefix + "ld", "-EL", "-T", f"{where}link.ld", "-e", f"{load:#x}", "-o", f"{where}image.elf", *objects],
        cwd=build,
        step="link",
    )
    run([prefix + "objcopy", "-O", "binary", "image.elf", "image.bin"], cwd=outdir, step="objcopy")
    image = (outdir / "image.bin").read_bytes()
    executable = b""
    if header is not None:
        executable = header + image
        (outdir / "rebuilt.exe").write_bytes(executable)

    failures += elf_function_checks(outdir / "image.elf", units)
    failures += elf_symbol_checks(outdir / "image.elf", units, defined)
    comparison = compare_image(image, payload, load, units)
    failures += comparison_failures(comparison)
    exe_sha = sha256(executable)
    if header is not None and exe_sha != cfg.baseline_sha256:
        failures.append(f"executable sha256 mismatch: rebuilt {exe_sha}, baseline {cfg.baseline_sha256}")

    # Controls are only meaningful against an image that matches.
    if comparison.image_exact:
        controls = run_controls(image, payload, load, units)
        for control in controls:
            if control["applicable"] and not control["tripped"]:
                failures.append(f"comparator control did not trip: {control['kind']} {control['target']}: {control['detail']}")
    else:
        controls = []

    c_units = [u for u in units if u.kind == "c"]
    asm_units = [u for u in units if u.kind == "asm"]
    c_bytes = sum(u.end - u.start for u in c_units)
    asm_bytes = sum(u.end - u.start for u in asm_units)
    rodata_bytes = sum(u.rodata.size for u in units if u.rodata is not None)
    data_bytes = sum(u.data.size for u in units if u.data is not None)
    coverage = {
        "c_bytes": c_bytes,
        "c_functions": sum(len(u.functions) for u in c_units),
        "asm_bytes": asm_bytes,
        "asm_functions": sum(len(u.functions) for u in asm_units),
        "rodata_bytes": rodata_bytes,
        "data_bytes": data_bytes,
        "raw_payload_bytes": len(payload) - c_bytes - asm_bytes - rodata_bytes - data_bytes,
    }
    if header is not None:
        coverage["raw_header_bytes"] = HEADER_SIZE
    coverage["raw_ranges"] = len(ranges)
    fields = {
        "units": [
            {
                "name": u.name,
                "kind": u.kind,
                "cache": cache_units[u.name]["cache"],
                "cache_key": cache_units[u.name]["key"],
                "range": [u.start, u.end],
                "functions": [dataclasses.asdict(f) for f in comparison.functions if f.unit == u.name],
                "rodata": next((dataclasses.asdict(r) for r in comparison.rodata if r.unit == u.name), None),
                "data": next((dataclasses.asdict(r) for r in comparison.data if r.unit == u.name), None),
                "bss": dataclasses.asdict(u.bss) if u.bss is not None else None,
            }
            for u in units
        ],
        "coverage": coverage,
        "bss_bytes": sum(u.bss.size for u in units if u.bss is not None),
        "image_sha256": comparison.image_sha256,
        "baseline_sha256": comparison.baseline_sha256,
        "executable_sha256": exe_sha,
        "controls": controls,
    }
    return fields, failures


@dataclasses.dataclass
class Pipeline:
    """What the pipeline of a unit needs, prepared once: the compiler, the pins checked, the tools and the include arguments."""

    cfg: Config
    cache_dir: Path | None
    include_args: list
    compiler: Compiler
    tools: dict
    maspsx_script: Path
    maspsx_env: dict
    assembler: str  # the file that was hashed for the key


def prepare_pipeline(
    cfg: Config, tag: str, build: Path, cache_dir: Path | None, report: dict
) -> tuple[Pipeline | None, list[str]]:
    """Generate the shared types, find the include arguments, check the toolchain pins and record the tools.

    Writes the generated header and the maspsx export into `build`. Returns
    the pipeline, or None and the reasons when the types or a pin fail.
    """
    include_args: list = []
    if cfg.types_fields is not None:
        report["inputs"]["types_fields"] = file_sha(cfg.types_fields)
        try:
            gen = generate_types(cfg, build)
        except structgen.FieldsError as exc:
            return None, [f"shared types: {error}" for error in exc.errors]
        include_args = ["-I", gen]
    for include_dir in cfg.include_dirs:
        include_args += ["-I", include_dir]
    report["inputs"]["include_dirs"] = [str(d) for d in cfg.include_dirs]
    report["inputs"]["maspsx_flags"] = maspsx_flags(cfg)

    compiler = Compiler(cfg.cc1, tag)
    compiler.check_master()
    pin_errors, tools, maspsx_script = check_pins(cfg, compiler, build)
    report["tools"] = tools
    if pin_errors:
        return None, pin_errors
    collect_versions(cfg, tools)
    # The exported copy carries no bytecode; keep it that way.
    maspsx_env = {**os.environ, "PYTHONDONTWRITEBYTECODE": "1"}
    return Pipeline(cfg, cache_dir, include_args, compiler, tools, maspsx_script, maspsx_env, tools["as"]["path"]), []


def unit_object(pipeline: Pipeline, unit: UnitDecl, build: Path, report: dict) -> tuple[list[str], bool]:
    """The pipeline of one unit, up to its object and the checks of that object.

    The files of the unit go to `build`. Returns the failures of the checks
    and whether the object exists: a unit that is refused before the compiler
    (a floating-point token, inline assembly) has none.
    """
    cfg, assembler, cache_dir = pipeline.cfg, pipeline.assembler, pipeline.cache_dir
    source = _expand(unit.source, cfg.directory)
    pre, asm, gnu, obj = (build / f"unit-{unit.name}{ext}" for ext in (".i", ".s", ".gnu.s", ".o"))
    if unit.kind == "asm":
        # Assembled as written: no preprocessing, compiler, maspsx or cache.
        run([assembler, *AS_FLAGS, "-o", obj, source], step=f"assemble {unit.name}")
        report["cache"]["units"][unit.name] = {"cache": "off", "key": None}
        return check_unit_object(obj, unit), True
    run(
        [cfg.cpp, "-E", "-P", "-x", "c", "-target", "mipsel-none-elf", "-nostdinc", *pipeline.include_args, source, "-o", pre],
        step=f"preprocess {unit.name}",
    )
    # The preprocessed text covers the source and every header it includes.
    report["inputs"]["preprocessed"][unit.name] = file_sha(pre)
    if cfg.cc1.no_float:
        token = find_float(pre.read_text(errors="replace"))
        if token:
            return [
                f"unit {unit.name!r}: floating-point token {token!r} is not supported by "
                f"compiler {cfg.cc1.name!r} (no_float); build with the reference compiler"
            ], False
    token = find_inline_asm(pre.read_text(errors="replace"))
    if token:
        return [
            f"unit {unit.name!r}: inline assembly ({token!r}) in a C unit; "
            f"code that was assembly goes into an assembly unit (kind = \"asm\")"
        ], False
    inputs = cache_key_inputs(cfg, pipeline.tools, unit, report["inputs"]["preprocessed"][unit.name], pipeline.maspsx_script)
    key = cache_key(inputs)
    entry = cache_dir / key if cache_dir is not None else None
    status = "off"
    if entry is not None:
        hit = cache_fetch(entry, key, {"unit.o": obj, "unit.s": asm, "unit.gnu.s": gnu})
        status = "hit" if hit else "miss"
    report["cache"]["units"][unit.name] = {"cache": status, "key": key}
    if status != "hit":
        pipeline.compiler.compile(unit.name, unit.flags, pre, asm, build / f"unit-{unit.name}.compiler.log")
        converted = run(
            [sys.executable, pipeline.maspsx_script, *maspsx_flags(cfg), f"--aspsx-version={cfg.aspsx_version}"],
            input=asm.read_bytes(),
            env=pipeline.maspsx_env,
            step=f"maspsx {unit.name}",
        )
        gnu.write_bytes(converted.stdout)
        run(
            [assembler, *AS_FLAGS, "-o", obj, gnu],
            step=f"assemble {unit.name}",
        )
        if entry is not None:
            try:
                cache_store(cache_dir, key, inputs, obj, asm, gnu)
            except OSError:
                pass  # an unwritable cache must not fail the build
    return check_unit_object(obj, unit), True


def build_all(
    cfg: Config,
    tag: str,
    build: Path,
    cache_dir: Path | None = None,
    report: dict | None = None,
    selected: str | None = None,
) -> tuple[dict, list[str]]:
    """Run the pipeline. Returns the report and the list of failure reasons.

    `cache_dir` is the object cache, None to disable it. A caller may pass the
    report so that a step failure still leaves what was recorded so far.
    `selected` builds one image only: RESIDENT or a declared module image.
    """
    failures: list[str] = []
    if report is None:
        report = {"tag": tag}
    report["tag"] = tag
    if selected is not None and cfg.images:
        report["selected_image"] = selected
    build_resident = selected in (None, RESIDENT)
    built_units = cfg.units if selected is None else cfg.units_of(selected)
    report["cache"] = {"mode": "off" if cache_dir is None else "on", "dir": None if cache_dir is None else str(cache_dir), "units": {}}
    report["inputs"] = {
        "configuration": file_sha(cfg.path),
        "symbols": file_sha(cfg.symbols_path),
        "baseline": sha256(cfg.baseline),
        "sources": {u.name: file_sha(_expand(u.source, cfg.directory)) for u in built_units},
    }
    if cfg.images:
        report["inputs"]["images"] = {
            i.name: {"archive": file_sha(i.archive), "chunk": sha256(i.payload)} for i in cfg.images
        }

    pipeline, failures = prepare_pipeline(cfg, tag, build, cache_dir, report)
    if failures:
        return report, failures
    assembler = pipeline.assembler

    payload = cfg.payload
    if build_resident:
        (build / "payload.bin").write_bytes(payload)  # also written by the link; an early failure still leaves it

    report["inputs"]["preprocessed"] = {}
    failures_by_unit: dict[str, list[str]] = {}
    for unit in built_units:
        try:
            failures_by_unit[unit.name], _ = unit_object(pipeline, unit, build, report)
        except StepError as exc:
            # A failed step of a module unit names its image, like every other failure of that image.
            raise StepError(named(unit.image, str(exc))) from exc
    failures = [
        named(unit.image, reason) for unit in built_units for reason in failures_by_unit.get(unit.name, [])
    ]
    if failures:
        return report, failures
    defined = {unit.name: defined_symbols(build / f"unit-{unit.name}.o") for unit in built_units}
    for image in dict.fromkeys(unit.image for unit in built_units):
        failures += [
            named(image, reason)
            for reason in symbol_override_errors(
                [u for u in built_units if u.image == image], defined, cfg.symbol_names
            )
            + other_image_override_errors(
                [u for u in built_units if u.image == image], defined, cfg.functions_of_others(image)
            )
        ]
    if failures:
        return report, failures

    cache_units = report["cache"]["units"]
    if build_resident:
        fields, found = link_image(
            cfg, build, build, cfg.load, payload, cfg.units_of(RESIDENT), defined, cache_units, assembler, cfg.header, RESIDENT
        )
        failures += found
        report.update(
            {
                "units": fields["units"],
                "coverage": fields["coverage"],
                "bss_bytes": fields["bss_bytes"],
                "image_sha256": fields["image_sha256"],
                "executable_sha256": fields["executable_sha256"],
                "baseline_executable_sha256": cfg.baseline_sha256,
                "controls": fields["controls"],
            }
        )
    records = []
    for image in cfg.images:
        if selected not in (None, image.name):
            continue
        try:
            fields, found = link_image(
                cfg, build, build / f"image-{image.name}", image.address, image.payload,
                cfg.units_of(image.name), defined, cache_units, assembler, None, image.name, image.symbols,
            )
        except StepError as exc:
            raise StepError(named(image.name, str(exc))) from exc
        found = [named(image.name, reason) for reason in found]
        failures += found
        fields["coverage"].pop("raw_header_bytes", None)
        records.append(
            {
                "name": image.name,
                "archive": str(image.archive),
                "slot": image.slot,
                "address": image.address,
                "size": image.size,
                "baseline_sha256": fields["baseline_sha256"],
                "image_sha256": fields["image_sha256"],
                "exact": not found,
                "carriers": count_carriers(image),
                "units": fields["units"],
                "coverage": fields["coverage"],
                "bss_bytes": fields["bss_bytes"],
                "controls": fields["controls"],
                "symbols": dict(image.symbols),
            }
        )
    if cfg.images and selected != RESIDENT:
        report["images"] = records
    return report, failures


def named(image: str, reason: str) -> str:
    """A failure reason of a module image starts with the image's name."""
    return reason if image == RESIDENT else f"image {image!r}: {reason}"


def module_summary(record: dict) -> list[str]:
    """The summary lines of one module image."""
    lines = []
    functions = [f for u in record["units"] for f in u["functions"]]
    for f in functions:
        state = "exact" if f["exact"] else f"DIFFERENT at offset {f['first_diff']}"
        lines.append(f"  {f['unit']}.{f['name']}: {f['size']} bytes, {state}")
    for u in record["units"]:
        for kind in ("rodata", "data"):
            ro = u.get(kind)
            if ro:
                state = "exact" if ro["exact"] else f"DIFFERENT at offset {ro['first_diff']}"
                lines.append(f"  {u['name']} {kind}: {ro['size']} bytes at {ro['address']:#x}, {state}")
    cov, name = record["coverage"], f"image {record['name']}"
    lines.append(f"{name} functions exact: {sum(1 for f in functions if f['exact'])}/{len(functions)}")
    lines.append(
        f"{name} coverage: C {cov['c_bytes']:,} bytes ({cov['c_functions']} functions), "
        f"assembly {cov['asm_bytes']:,} bytes ({cov['asm_functions']} functions), "
        f"rodata {cov['rodata_bytes']:,} bytes, data {cov['data_bytes']:,} bytes, "
        f"raw payload {cov['raw_payload_bytes']:,} bytes"
    )
    lines.append(f"{name} sha256:      {record['image_sha256']}")
    lines.append(f"{name} baseline sha256: {record['baseline_sha256']}")
    lines.append(f"{name} carriers: {record['carriers']}")
    controls = record["controls"]
    applicable = [c for c in controls if c["applicable"]]
    if controls:
        note = "" if len(applicable) == len(controls) else " (raw control not applicable: no raw range)"
        lines.append(f"{name} comparator controls: {sum(1 for c in applicable if c['tripped'])}/{len(applicable)} tripped{note}")
    else:
        lines.append(f"{name} comparator controls: not run")
    return lines


def summary_text(report: dict, failures: list[str]) -> str:
    lines = [f"matchbuild tag={report['tag']}"]
    cc1 = report.get("tools", {}).get("cc1")
    if cc1:
        lines.append(f"compiler: {cc1['name']} ({cc1['kind']})")
    if "units" in report:
        functions = [f for u in report["units"] for f in u["functions"]]
        exact = sum(1 for f in functions if f["exact"])
        for f in functions:
            state = "exact" if f["exact"] else f"DIFFERENT at offset {f['first_diff']}"
            lines.append(f"  {f['unit']}.{f['name']}: {f['size']} bytes, {state}")
        for u in report["units"]:
            for kind in ("rodata", "data"):
                ro = u.get(kind)
                if ro:
                    state = "exact" if ro["exact"] else f"DIFFERENT at offset {ro['first_diff']}"
                    lines.append(f"  {u['name']} {kind}: {ro['size']} bytes at {ro['address']:#x}, {state}")
        cov = report["coverage"]
        lines.append(f"functions exact: {exact}/{len(functions)}")
        # Every unit that was compiled, of every image.
        states = [entry["cache"] for entry in report["cache"]["units"].values()]
        if states and all(state == "off" for state in states):
            lines.append("cache: off")
        else:
            lines.append(f"cache: {states.count('hit')} hits, {states.count('miss')} misses")
        lines.append(
            f"coverage: C {cov['c_bytes']:,} bytes ({cov['c_functions']} functions), "
            f"assembly {cov['asm_bytes']:,} bytes ({cov['asm_functions']} functions), "
            f"rodata {cov['rodata_bytes']:,} bytes, "
            f"data {cov['data_bytes']:,} bytes, "
            f"raw payload {cov['raw_payload_bytes']:,} bytes, header {cov['raw_header_bytes']:,} bytes raw"
        )
        lines.append(f"image sha256:      {report['image_sha256']}")
        lines.append(f"executable sha256: {report['executable_sha256']}")
        lines.append(f"baseline sha256:   {report['baseline_executable_sha256']}")
        controls = report["controls"]
        applicable = [c for c in controls if c["applicable"]]
        tripped = sum(1 for c in applicable if c["tripped"])
        if controls:
            note = "" if len(applicable) == len(controls) else " (raw control not applicable: no raw range)"
            lines.append(f"comparator controls: {tripped}/{len(applicable)} tripped{note}")
        else:
            lines.append("comparator controls: not run")
        maspsx = report.get("tools", {}).get("maspsx", {})
        if maspsx.get("checkout_dirty"):
            lines.append("note: the maspsx checkout has local changes; the pinned commit was exported and run instead")
    for record in report.get("images", []):
        lines += module_summary(record)
    for reason in failures:
        lines.append(f"FAIL: {reason}")
    lines.append("RESULT: " + ("FAIL" if failures else "PASS"))
    return "\n".join(lines)


def cache_directory(option: Path | None, no_cache: bool, config_path: Path) -> Path | None:
    """The object cache of a run: None without one, else the given directory or the default next to the build directories."""
    if no_cache:
        return None
    return Path(os.path.abspath(option)) if option else config_path.parent.parent / "build" / ".objcache"


def main(argv: list[str] | None = None) -> int:
    root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description="PS1 matching build")
    parser.add_argument("--config", type=Path, default=root / "ps1/src/build.toml")
    parser.add_argument("--tag", default="default")
    parser.add_argument(
        "--reference",
        action="store_true",
        help="compile with [toolchain.cc1_reference] instead of [toolchain.cc1]",
    )
    parser.add_argument("--cache", type=Path, help="object cache directory (default: <config dir>/../build/.objcache)")
    parser.add_argument("--no-cache", action="store_true", help="do not read or write the object cache")
    parser.add_argument("--image", help=f"build one image: '{RESIDENT}' or a declared module image")
    args = parser.parse_args(argv)

    if not TAG_RE.match(args.tag):
        print(f"FAIL: invalid tag {args.tag!r}", file=sys.stderr)
        return 2
    config_path = Path(os.path.abspath(args.config))
    build = config_path.parent.parent / "build" / args.tag
    cache_dir = cache_directory(args.cache, args.no_cache, config_path)
    if build.exists():
        shutil.rmtree(build)

    try:
        cfg = load_config(config_path, use_reference=args.reference)
    except ConfigError as exc:
        for error in exc.errors:
            print(f"CONFIG ERROR: {error}")
        print("RESULT: FAIL")
        return 2

    if args.image is not None and args.image != RESIDENT and args.image not in [i.name for i in cfg.images]:
        print(f"CONFIG ERROR: --image {args.image!r} names no declared image")
        print("RESULT: FAIL")
        return 2

    build.mkdir(parents=True)
    report: dict = {"tag": args.tag}
    try:
        report, failures = build_all(cfg, args.tag, build, cache_dir, report, args.image)
    except StepError as exc:
        failures = [str(exc)]
    except EnvironmentFailure as exc:
        print(f"ENVIRONMENT ERROR: {exc}")
        print("RESULT: FAIL")
        return 3
    report["exact"] = not failures
    report["failures"] = failures
    (build / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    text = summary_text(report, failures)
    (build / "summary.txt").write_text(text + "\n")
    print(text)
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
