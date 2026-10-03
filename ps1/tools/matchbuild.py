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

import structgen

SHF_ALLOC = 0x2
HEADER_SIZE = 2048
EXE_MAGIC = b"PS-X EXE"
# Sections that are not loaded on target and are dropped by the linker script.
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
CACHE_FORMAT = "matchbuild-objcache-1"
CACHE_FILES = ("unit.o", "unit.s", "unit.gnu.s", "entry.json")
FLOAT_RE = re.compile(r"\b(?:float|double)\b|(?<![\w.])(?:\d+\.\d*|\.\d+|\d+(?=[eE]))(?:[eE][+-]?\d+)?")


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
class UnitDecl:
    name: str
    source: str
    flags: tuple[str, ...]
    functions: tuple[FunctionDecl, ...]  # sorted by address

    @property
    def start(self) -> int:
        return self.functions[0].address

    @property
    def end(self) -> int:
        last = self.functions[-1]
        return last.address + last.size


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
class ImageComparison:
    image_size: int
    baseline_size: int
    image_sha256: str
    baseline_sha256: str
    image_exact: bool
    functions: tuple[FunctionResult, ...]

    @property
    def all_functions_exact(self) -> bool:
        return all(f.exact for f in self.functions)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def raw_ranges(load: int, size: int, units: list[UnitDecl] | tuple[UnitDecl, ...]) -> list[tuple[int, int]]:
    """Return (address, size) of every payload byte range not owned by a unit."""
    ranges = []
    cursor = load
    for unit in sorted(units, key=lambda u: u.start):
        if unit.start > cursor:
            ranges.append((cursor, unit.start - cursor))
        cursor = unit.end
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
    for unit in units:
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
            tripped = failed == [fn.name] and not result.image_exact
            controls.append(
                {
                    "kind": "function",
                    "target": fn.name,
                    "applicable": True,
                    "tripped": tripped,
                    "detail": f"functions failing: {failed}, image exact: {result.image_exact}",
                }
            )
    ranges = raw_ranges(load, len(payload), units)
    if ranges:
        address, size = ranges[0]
        result = compare_image(flip_byte(image, address - load + size // 2), payload, load, units)
        tripped = (not result.image_exact) and result.all_functions_exact
        controls.append(
            {
                "kind": "raw",
                "target": f"raw byte at {address + size // 2:#010x}",
                "applicable": True,
                "tripped": tripped,
                "detail": f"image exact: {result.image_exact}, functions exact: {result.all_functions_exact}",
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
    units: list[UnitDecl]
    symbols_path: Path
    symbol_names: list[str]
    types_fields: Path | None = None  # [types] fields file, None when absent
    types_header: str = ""
    # Filled in during validation of the baseline.
    baseline: bytes = b""
    load: int = 0
    payload_size: int = 0

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
        flags = table.get("flags", [])
        if not isinstance(flags, list) or not all(isinstance(f, str) and FLAG_RE.match(f) for f in flags):
            errors.append(f"{where}: 'flags' must be a list of plain option strings")
            flags = []
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
        if not name or not functions or len(functions) != len(fn_tables):
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
            units.append(UnitDecl(name, source, tuple(flags), tuple(functions)))

    # Unit overlap.
    ordered = sorted(units, key=lambda u: u.start)
    for a, b in zip(ordered, ordered[1:]):
        if a.end > b.start:
            errors.append(
                f"units {a.name!r} and {b.name!r} overlap "
                f"({a.start:#x}-{a.end:#x} and {b.start:#x}-{b.end:#x})"
            )

    # symbols.ld
    symbols_path = directory / "symbols.ld"
    symbol_names: list[str] = []
    try:
        symbol_names = parse_symbols(symbols_path.read_text(), errors)
    except OSError as exc:
        errors.append(f"cannot read {symbols_path}: {exc}")
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
                source_dirs = {directory} | {_expand(u.source, directory).parent for u in units}
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
                for unit in units:
                    if unit.start < config.load or unit.end > config.load + config.payload_size:
                        errors.append(
                            f"unit {unit.name!r} range {unit.start:#x}-{unit.end:#x} is outside the payload "
                            f"{config.load:#x}-{config.load + config.payload_size:#x}"
                        )
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
    """Check one unit object: only .text, size equals the declared range, no common symbols."""
    errors = []
    declared = unit.end - unit.start
    with open(obj_path, "rb") as handle:
        elf = ELFFile(handle)
        text_size = 0
        for section in elf.iter_sections():
            name = section.name
            if not name or not (section["sh_flags"] & SHF_ALLOC):
                continue
            if name == ".text":
                text_size = section["sh_size"]
            elif name in DISCARDED_SECTIONS:
                continue
            elif section["sh_size"] > 0:
                errors.append(
                    f"unit {unit.name!r}: non-empty allocated section {name} ({section['sh_size']} bytes); "
                    f"data ownership is not implemented"
                )
        symtab = elf.get_section_by_name(".symtab")
        if symtab is not None:
            for sym in symtab.iter_symbols():
                if sym["st_shndx"] == "SHN_COMMON":
                    errors.append(f"unit {unit.name!r}: common symbol {sym.name!r} (data ownership is not implemented)")
        if text_size != declared:
            errors.append(
                f"unit {unit.name!r}: text size mismatch: .text is {text_size} bytes, declared range is {declared}"
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
    return errors


def generate_raw_and_linker(cfg: Config, build: Path, ranges: list[tuple[int, int]]) -> None:
    raw_lines = []
    for index, (address, size) in enumerate(ranges):
        raw_lines.append(f'.section .raw{index},"a",@progbits')
        raw_lines.append(f'.incbin "payload.bin", {address - cfg.load}, {size}')
    (build / "raw.s").write_text("\n".join(raw_lines) + "\n")

    entries = [(address, f".raw{index}", f"*(.raw{index})") for index, (address, _) in enumerate(ranges)]
    for unit in cfg.units:
        entries.append((unit.start, f".text.{unit.name}", f"unit-{unit.name}.o(.text)"))
    entries.sort()
    lines = ["OUTPUT_ARCH(mips)", f'INCLUDE "{cfg.symbols_path}"', "SECTIONS {"]
    for address, name, pattern in entries:
        lines.append(f" {name} {address:#x} : SUBALIGN(1) {{ {pattern} }}")
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


def collect_versions(cfg: Config, tools: dict) -> None:
    tools["python"] = sys.version.split()[0]
    tools["cpp"] = {"path": cfg.cpp, "version": first_line(run([cfg.cpp, "--version"], step="cpp version").stdout)}
    for tool in ("as", "ld", "objcopy"):
        name = cfg.binutils_prefix + tool
        tools[tool] = {"name": name, "version": first_line(run([name, "--version"], step=f"{tool} version").stdout)}
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


def cache_key_inputs(
    cfg: Config, tools: dict, unit: UnitDecl, preprocessed_sha256: str, maspsx_script: Path
) -> dict:
    """Everything that can change the unit object, in readable form.

    The preprocessed text enters only as its hash. The unit name is included
    because the compiler may record the input file name, which derives from it.
    The maspsx script path is included because the pinned commit holds more
    than one script.
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
        "as_flags": list(AS_FLAGS),
        "as_version": tools["as"]["version"],
    }


def cache_key(inputs: dict) -> str:
    canonical = json.dumps(inputs, sort_keys=True, separators=(",", ":"), ensure_ascii=True)
    return sha256(canonical.encode("ascii"))


def cache_lookup(entry: Path) -> bool:
    """True when the entry is complete and its object matches the recorded hash."""
    try:
        if not all((entry / name).is_file() for name in CACHE_FILES):
            return False
        recorded = json.loads((entry / "entry.json").read_text())["object_sha256"]
        return file_sha(entry / "unit.o") == recorded
    except (OSError, ValueError, KeyError, TypeError):
        return False


def cache_store(cache_dir: Path, key: str, inputs: dict, obj: Path, asm: Path, gnu: Path) -> None:
    """Publish an entry atomically. Losing a race to a concurrent run is fine."""
    cache_dir.mkdir(parents=True, exist_ok=True)
    entry = cache_dir / key
    temp = Path(tempfile.mkdtemp(prefix=".tmp-", dir=cache_dir))
    try:
        shutil.copyfile(obj, temp / "unit.o")
        shutil.copyfile(asm, temp / "unit.s")
        shutil.copyfile(gnu, temp / "unit.gnu.s")
        record = {"object_sha256": file_sha(obj), "key": key, "inputs": inputs}
        (temp / "entry.json").write_text(json.dumps(record, indent=2, sort_keys=True) + "\n")
        if entry.exists():
            # A broken entry is being replaced. Move it aside first: rename
            # cannot replace a non-empty directory.
            try:
                os.rename(entry, temp.with_name(temp.name + ".old"))
            except OSError:
                pass
            shutil.rmtree(temp.with_name(temp.name + ".old"), ignore_errors=True)
        try:
            os.rename(temp, entry)
        except OSError:
            pass  # a concurrent run published first; use or ignore its entry
    finally:
        shutil.rmtree(temp, ignore_errors=True)


def build_all(
    cfg: Config, tag: str, build: Path, cache_dir: Path | None = None, report: dict | None = None
) -> tuple[dict, list[str]]:
    """Run the pipeline. Returns the report and the list of failure reasons.

    `cache_dir` is the object cache, None to disable it. A caller may pass the
    report so that a step failure still leaves what was recorded so far.
    """
    failures: list[str] = []
    if report is None:
        report = {"tag": tag}
    report["tag"] = tag
    report["cache"] = {"mode": "off" if cache_dir is None else "on", "dir": None if cache_dir is None else str(cache_dir), "units": {}}
    report["inputs"] = {
        "configuration": file_sha(cfg.path),
        "symbols": file_sha(cfg.symbols_path),
        "baseline": sha256(cfg.baseline),
        "sources": {u.name: file_sha(_expand(u.source, cfg.directory)) for u in cfg.units},
    }

    include_args: list = []
    if cfg.types_fields is not None:
        report["inputs"]["types_fields"] = file_sha(cfg.types_fields)
        try:
            gen = generate_types(cfg, build)
        except structgen.FieldsError as exc:
            return report, [f"shared types: {error}" for error in exc.errors]
        include_args = ["-I", gen]

    compiler = Compiler(cfg.cc1, tag)
    compiler.check_master()
    pin_errors, tools, maspsx_script = check_pins(cfg, compiler, build)
    report["tools"] = tools
    if pin_errors:
        return report, pin_errors
    collect_versions(cfg, tools)
    # The exported copy carries no bytecode; keep it that way.
    maspsx_env = {**os.environ, "PYTHONDONTWRITEBYTECODE": "1"}

    payload = cfg.payload
    (build / "payload.bin").write_bytes(payload)

    prefix = cfg.binutils_prefix
    report["inputs"]["preprocessed"] = {}
    for unit in cfg.units:
        source = _expand(unit.source, cfg.directory)
        pre, asm, gnu, obj = (build / f"unit-{unit.name}{ext}" for ext in (".i", ".s", ".gnu.s", ".o"))
        run(
            [cfg.cpp, "-E", "-P", "-x", "c", "-target", "mipsel-none-elf", "-nostdinc", *include_args, source, "-o", pre],
            step=f"preprocess {unit.name}",
        )
        # The preprocessed text covers the source and every header it includes.
        report["inputs"]["preprocessed"][unit.name] = file_sha(pre)
        if cfg.cc1.no_float:
            match = FLOAT_RE.search(pre.read_text(errors="replace"))
            if match:
                failures.append(
                    f"unit {unit.name!r}: floating-point token {match.group(0)!r} is not supported by "
                    f"compiler {cfg.cc1.name!r} (no_float); build with the reference compiler"
                )
                continue
        inputs = cache_key_inputs(cfg, tools, unit, report["inputs"]["preprocessed"][unit.name], maspsx_script)
        key = cache_key(inputs)
        entry = cache_dir / key if cache_dir is not None else None
        status = "off"
        if entry is not None:
            status = "hit" if cache_lookup(entry) else "miss"
        report["cache"]["units"][unit.name] = {"cache": status, "key": key}
        if status == "hit":
            shutil.copyfile(entry / "unit.o", obj)
            shutil.copyfile(entry / "unit.s", asm)
            shutil.copyfile(entry / "unit.gnu.s", gnu)
        else:
            compiler.compile(unit.name, unit.flags, pre, asm, build / f"unit-{unit.name}.compiler.log")
            converted = run(
                [sys.executable, maspsx_script, f"--aspsx-version={cfg.aspsx_version}"],
                input=asm.read_bytes(),
                env=maspsx_env,
                step=f"maspsx {unit.name}",
            )
            gnu.write_bytes(converted.stdout)
            run(
                [prefix + "as", *AS_FLAGS, "-o", obj, gnu],
                step=f"assemble {unit.name}",
            )
            if entry is not None:
                try:
                    cache_store(cache_dir, key, inputs, obj, asm, gnu)
                except OSError:
                    pass  # an unwritable cache must not fail the build
        failures += check_unit_object(obj, unit)
    if failures:
        return report, failures

    ranges = raw_ranges(cfg.load, cfg.payload_size, cfg.units)
    generate_raw_and_linker(cfg, build, ranges)
    run([prefix + "as", *AS_FLAGS, "-o", "raw.o", "raw.s"], cwd=build, step="assemble raw")
    objects = [f"unit-{u.name}.o" for u in cfg.units] + ["raw.o"]
    run(
        [prefix + "ld", "-EL", "-T", "link.ld", "-e", f"{cfg.load:#x}", "-o", "image.elf", *objects],
        cwd=build,
        step="link",
    )
    run([prefix + "objcopy", "-O", "binary", "image.elf", "image.bin"], cwd=build, step="objcopy")
    image = (build / "image.bin").read_bytes()
    executable = cfg.header + image
    (build / "rebuilt.exe").write_bytes(executable)

    failures += elf_function_checks(build / "image.elf", cfg.units)
    comparison = compare_image(image, payload, cfg.load, cfg.units)
    for fn in comparison.functions:
        if not fn.exact:
            failures.append(
                f"function {fn.name!r}: bytes differ from baseline at offset {fn.first_diff} "
                f"({fn.equal_words}/{fn.total_words} words equal)"
            )
    if not comparison.image_exact:
        failures.append(
            f"image differs from baseline payload (size {comparison.image_size} vs {comparison.baseline_size}, "
            f"sha256 {comparison.image_sha256} vs {comparison.baseline_sha256})"
        )
    exe_sha = sha256(executable)
    if exe_sha != cfg.baseline_sha256:
        failures.append(f"executable sha256 mismatch: rebuilt {exe_sha}, baseline {cfg.baseline_sha256}")

    # Controls are only meaningful against an image that matches.
    if comparison.image_exact:
        controls = run_controls(image, payload, cfg.load, cfg.units)
        for control in controls:
            if control["applicable"] and not control["tripped"]:
                failures.append(f"comparator control did not trip: {control['kind']} {control['target']}: {control['detail']}")
    else:
        controls = []

    c_bytes = sum(u.end - u.start for u in cfg.units)
    report.update(
        {
            "units": [
                {
                    "name": u.name,
                    "cache": report["cache"]["units"][u.name]["cache"],
                    "cache_key": report["cache"]["units"][u.name]["key"],
                    "range": [u.start, u.end],
                    "functions": [
                        dataclasses.asdict(f) for f in comparison.functions if f.unit == u.name
                    ],
                }
                for u in cfg.units
            ],
            "coverage": {
                "c_bytes": c_bytes,
                "c_functions": sum(len(u.functions) for u in cfg.units),
                "raw_payload_bytes": cfg.payload_size - c_bytes,
                "raw_header_bytes": HEADER_SIZE,
                "raw_ranges": len(ranges),
            },
            "image_sha256": comparison.image_sha256,
            "executable_sha256": exe_sha,
            "baseline_executable_sha256": cfg.baseline_sha256,
            "controls": controls,
        }
    )
    return report, failures


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
        cov = report["coverage"]
        lines.append(f"functions exact: {exact}/{len(functions)}")
        states = [u.get("cache", "off") for u in report["units"]]
        if states and all(state == "off" for state in states):
            lines.append("cache: off")
        else:
            lines.append(f"cache: {states.count('hit')} hits, {states.count('miss')} misses")
        lines.append(
            f"coverage: C {cov['c_bytes']:,} bytes ({cov['c_functions']} functions), "
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
    for reason in failures:
        lines.append(f"FAIL: {reason}")
    lines.append("RESULT: " + ("FAIL" if failures else "PASS"))
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description="PS1 matching build")
    parser.add_argument("--config", type=Path, default=root / "ps1/local/src/build.toml")
    parser.add_argument("--tag", default="default")
    parser.add_argument(
        "--reference",
        action="store_true",
        help="compile with [toolchain.cc1_reference] instead of [toolchain.cc1]",
    )
    parser.add_argument("--cache", type=Path, help="object cache directory (default: <config dir>/../build/.objcache)")
    parser.add_argument("--no-cache", action="store_true", help="do not read or write the object cache")
    args = parser.parse_args(argv)

    if not TAG_RE.match(args.tag):
        print(f"FAIL: invalid tag {args.tag!r}", file=sys.stderr)
        return 2
    config_path = Path(os.path.abspath(args.config))
    build = config_path.parent.parent / "build" / args.tag
    cache_dir = None
    if not args.no_cache:
        cache_dir = Path(os.path.abspath(args.cache)) if args.cache else config_path.parent.parent / "build" / ".objcache"
    if build.exists():
        shutil.rmtree(build)

    try:
        cfg = load_config(config_path, use_reference=args.reference)
    except ConfigError as exc:
        for error in exc.errors:
            print(f"CONFIG ERROR: {error}")
        print("RESULT: FAIL")
        return 2

    build.mkdir(parents=True)
    report: dict = {"tag": args.tag}
    try:
        report, failures = build_all(cfg, args.tag, build, cache_dir, report)
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
