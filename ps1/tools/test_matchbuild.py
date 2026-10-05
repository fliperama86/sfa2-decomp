#!/usr/bin/env python3
"""Controls for matchbuild.py.

Each case builds a temporary sibling copy of the private configuration
directory (`<config dir>.selftest-<case>`, tag `selftest-<case>`), applies one
change, and requires the tool to fail for the intended reason or, where stated,
to pass. The copies and their build directories are removed afterwards.
"""

from __future__ import annotations

import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
import tomllib
from pathlib import Path

import matchbuild
import structgen

ROOT = Path(__file__).resolve().parents[2]
TOOL = Path(__file__).resolve().parent / "matchbuild.py"
FNDIFF = Path(__file__).resolve().parent / "fndiff.py"

# Synthetic all-C fixture: one authored function that is the whole payload.
FIXTURE_LOAD = 0x80010000
FIXTURE_SOURCE = "int value(void) { return 7; }\n"
FIXTURE_SIZE = 8
HEADER_SIZE = 2048

# Synthetic read-only data fixture: a dense switch that compiles to a jump table.
RODATA_SOURCE = """\
extern int g;
int pick(int a) {
    switch (a) {
    case 0: return g + 1;
    case 1: return g * 3;
    case 2: return g - 5;
    case 3: return g ^ 9;
    case 4: return g | 2;
    case 5: return g & 6;
    }
    return 0;
}
"""
# Six bytes of read-only data: the object ends two bytes short of a word.
PADDED_SOURCE = """
char *pick(void) {
    return "hello";
}
"""
DIVISION_SOURCE = """
int pick(int a, int b) {
    return a / b;
}
"""
# Initialised data, a short data object and an uninitialised global that a function uses.
DATA_SOURCE = """
int table[4] = { 3, 5, 7, 11 };
int pick(int a) {
    table[1] = a;
    return table[a & 3];
}
"""
DATA_PADDED_SOURCE = """
char buf[6] = { 1, 2, 3, 4, 5, 6 };
char *pick(void) {
    return buf;
}
"""
BSS_SOURCE = """
int counter;
int total;
int pick(int a) {
    counter = counter + a;
    total = total + counter;
    return total;
}
"""
SIBLING_OWNER_SOURCE = "int datum = 7;\nint counter;\nint owner(void) { return 1; }\n"
SIBLING_USER_SOURCE = "extern int datum;\nextern int counter;\nint consumer(int a) { counter = counter + datum + a; return counter; }\n"
ASM_C_SOURCE = "int pick(int a) { return a + 1; }\n"
ASM_SOURCE = """\
.set noreorder
.text
.globl stub
.type stub, @function
stub:
    addiu $v0, $a0, 1
    addu  $v0, $v0, $a1
    jr    $ra
    nop
.size stub, . - stub
"""
ASM_SIZE = 16
ASM_KIND = 'kind = "asm"\n'
BSS_ADDRESS = 0x80300000  # outside the fixture payload
RODATA_GAP = 0x40  # raw filler between the end of the text and the table
RODATA_TAIL = 0x20  # raw filler after the table
FILLER = 0xA5


class Case:
    def __init__(self, name, expect_pass, reason, mutate=None, verify=None, fndiff=None):
        self.name = name
        self.expect_pass = expect_pass
        self.reason = reason  # substring required in the tool output when it must fail
        self.mutate = mutate
        self.verify = verify  # optional check of report.json, returns an error string or None
        self.fndiff = fndiff  # optional (unit, expected exit status, required text) for fndiff.py


def replace_once(text: str, old: str, new: str, what: str) -> str:
    if text.count(old) != 1:
        raise SystemExit(f"test setup: expected exactly one occurrence of {what}, found {text.count(old)}")
    return text.replace(old, new)


def run_tool(
    config: Path, tag: str, cache: Path | None = None, extra: tuple[str, ...] = (), env: dict | None = None
) -> subprocess.CompletedProcess:
    """Run the tool, with an explicit cache directory when one is given."""
    cache_args = [] if cache is None else ["--cache", str(cache)]
    return subprocess.run(
        [sys.executable, str(TOOL), "--config", str(config), "--tag", tag, *cache_args, *extra],
        capture_output=True,
        text=True,
        env=env,
    )


def toml_table(name: str, table: dict) -> str:
    """Serialise a table of strings, booleans, integers, string lists and sub-tables."""
    lines, subtables = [f"[{name}]"], []
    for key, value in table.items():
        if isinstance(value, dict):
            subtables.append((f"{name}.{key}", value))
        elif isinstance(value, bool):
            lines.append(f"{key} = {'true' if value else 'false'}")
        else:
            lines.append(f"{key} = {json.dumps(value)}")
    return "\n".join(lines) + "\n\n" + "".join(toml_table(n, t) for n, t in subtables)


def fixture_toolchain(parsed: dict) -> dict:
    """The configured toolchain without keys that name directories of the real configuration."""
    return {key: value for key, value in parsed["toolchain"].items() if key != "include_dirs"}


def add_include_dir(copy: Path, entry: str) -> None:
    """Add one entry to [toolchain] include_dirs of a copied configuration.

    Works whether the key is absent, an empty list or a populated one. The
    list is rebuilt from its parsed value, so the result is valid TOML.
    """
    path = copy / "build.toml"
    text = path.read_text()
    current = tomllib.loads(text).get("toolchain", {}).get("include_dirs")
    line = "include_dirs = [" + ", ".join(json.dumps(item) for item in [*(current or []), entry]) + "]"
    if current is None:
        text = replace_once(text, "[toolchain]\n", f"[toolchain]\n{line}\n", "[toolchain] header")
    else:
        existing = re.findall(r"^include_dirs\s*=\s*\[[^\]]*\]", text, flags=re.M)
        if len(existing) != 1:
            raise SystemExit(f"test setup: expected one include_dirs list in build.toml, found {len(existing)}")
        text = text.replace(existing[0], line)
    if tomllib.loads(text)["toolchain"]["include_dirs"] != [*(current or []), entry]:
        raise SystemExit("test setup: include_dirs was not rewritten as intended")
    path.write_text(text)


def fixture_executable(payload: bytes) -> bytes:
    header = bytearray(HEADER_SIZE)
    header[:8] = b"PS-X EXE"
    header[0x18:0x1C] = FIXTURE_LOAD.to_bytes(4, "little")
    header[0x1C:0x20] = len(payload).to_bytes(4, "little")
    return bytes(header) + payload


def make_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    unit = parsed["unit"][0]
    selftest = parsed["selftest"]
    target_unit = next(u for u in parsed["unit"] if u["name"] == selftest["unit"])
    first_fn = unit["functions"][0]
    last_fn = unit["functions"][-1]

    def mutate_source(copy: Path):
        path = copy / target_unit["source"]
        path.write_text(replace_once(path.read_text(), selftest["find"], selftest["replace"], "selftest 'find' text"))

    def wrong_size(copy: Path):
        path = copy / "build.toml"
        text = path.read_text()
        pattern = re.compile(r'(name\s*=\s*"%s"[^}]*?size\s*=\s*)(\d+)' % re.escape(last_fn["name"]))
        if len(pattern.findall(text)) != 1:
            raise SystemExit("test setup: cannot locate the last function's size in build.toml")
        path.write_text(pattern.sub(lambda m: f"{m.group(1)}{last_fn['size'] - 4}", text))

    def overlapping(copy: Path):
        path = copy / "build.toml"
        flags = ", ".join(f'"{f}"' for f in unit["flags"])
        path.write_text(
            path.read_text()
            + "\n[[unit]]\n"
            + 'name = "selftest_overlap"\n'
            + f'source = "{unit["source"]}"\n'
            + f"flags = [{flags}]\n"
            + "functions = [\n"
            + f'  {{ name = "selftest_overlap_fn", address = {first_fn["address"] + 4:#x}, size = 8 }},\n'
            + "]\n"
        )

    def remove_symbol(copy: Path):
        path = copy / "symbols.ld"
        lines = path.read_text().splitlines(keepends=True)
        index = next(i for i, line in enumerate(lines) if re.match(r"\s*[A-Za-z_]\w*\s*=\s*\S+;", line))
        del lines[index]
        path.write_text("".join(lines))

    def duplicate_symbol(copy: Path):
        path = copy / "symbols.ld"
        path.write_text(path.read_text() + f"{first_fn['name']} = {first_fn['address']:#x};\n")

    def flip_hex(value: str) -> str:
        return ("0" if value[0] != "0" else "1") + value[1:]

    def replace_in_config(copy: Path, old: str, new: str, what: str):
        path = copy / "build.toml"
        path.write_text(replace_once(path.read_text(), old, new, what))

    def wrong_cc1_hash(copy: Path):
        old = parsed["toolchain"]["cc1"]["sha256"]
        replace_in_config(copy, old, flip_hex(old), "cc1 sha256")

    def wrong_baseline_hash(copy: Path):
        old = parsed["baseline"]["sha256"]
        replace_in_config(copy, old, flip_hex(old), "baseline sha256")

    def unknown_maspsx_commit(copy: Path):
        old = parsed["toolchain"]["maspsx_commit"]
        replace_in_config(copy, old, flip_hex(old), "maspsx commit")

    def dirty_maspsx(copy: Path):
        """Point the build at a clone whose script is edited but not committed.

        The edit makes the script abort, so the build can only pass if the tool
        runs the pinned commit instead of the checkout's working tree.
        """
        configured = parsed["toolchain"]["maspsx"]
        script = Path(configured).expanduser()
        if not script.is_absolute():
            script = cfg_dir / script
        top = Path(
            subprocess.run(
                ["git", "-C", str(script.parent), "rev-parse", "--show-toplevel"],
                capture_output=True, text=True, check=True,
            ).stdout.strip()
        )
        clone = copy / "maspsx-clone"
        subprocess.run(["git", "clone", "-q", "--no-hardlinks", str(top), str(clone)], check=True)
        edited = clone / script.resolve().relative_to(top.resolve())
        edited.write_text("raise SystemExit('selftest: the dirty checkout was executed')\n" + edited.read_text())
        replace_in_config(copy, configured, str(edited), "maspsx path")

    def verify_dirty_reported(report: dict):
        if report.get("tools", {}).get("maspsx", {}).get("checkout_dirty") is not True:
            return "report does not record the dirty maspsx checkout"
        return None

    def add_float(copy: Path):
        path = copy / target_unit["source"]
        path.write_text(path.read_text() + "\ndouble selftest_float(void) { return 1.5; }\n")

    def float_in_literals(copy: Path):
        # Float-looking text inside string and character literals is data.
        path = copy / target_unit["source"]
        path.write_text(
            path.read_text()
            + '\ntypedef char selftest_literal_a[sizeof("$Id: x.c,v 1.71 float 2e3 \\" 3.5")];\n'
            + "typedef char selftest_literal_b['.' + '\\'' + 1];\n"
        )

    def add_inline_asm(copy: Path):
        path = copy / target_unit["source"]
        path.write_text(path.read_text() + '\n__asm__("nop");\n')

    def asm_in_literals(copy: Path):
        # The tokens inside string and character literals are data.
        path = copy / target_unit["source"]
        path.write_text(path.read_text() + '\ntypedef char selftest_asm_text[sizeof("__asm__ asm __asm")];\n')

    include_guard = "#include <selftest_inc.h>\n#ifndef SELFTEST_INC_OK\n#error include directory not used\n#endif\n"

    def use_include(copy: Path, listed: bool, body: str = "#define SELFTEST_INC_OK 1\n"):
        (copy / "selftest_inc").mkdir()
        (copy / "selftest_inc" / "selftest_inc.h").write_text(body)
        if listed:
            add_include_dir(copy, "selftest_inc")
        path = copy / target_unit["source"]
        path.write_text(include_guard + path.read_text())

    def include_shadows_header(copy: Path):
        (copy / "selftest_inc").mkdir()
        (copy / "selftest_inc" / parsed["types"]["header"]).write_text("/* would shadow the generated header */\n")
        add_include_dir(copy, "selftest_inc")

    def all_c_fixture(copy: Path):
        """Replace the copy with a fixture whose whole payload is one C function.

        The baseline is produced by the same toolchain: a seed build against a
        zero payload fails its comparison but leaves the image, which then
        becomes the fixture's baseline payload.
        """
        for child in copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        (copy / "value.c").write_text(FIXTURE_SOURCE)
        (copy / "symbols.ld").write_text("/* The fixture needs no external symbols. */\n")
        flags = ", ".join(json.dumps(f) for f in unit["flags"])

        def write(payload: bytes):
            executable = fixture_executable(payload)
            (copy / "baseline.bin").write_bytes(executable)
            (copy / "build.toml").write_text(
                "[baseline]\n"
                'executable = "baseline.bin"\n'
                f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
                + toml_table("toolchain", fixture_toolchain(parsed))
                + "[[unit]]\n"
                'name = "value"\n'
                'source = "value.c"\n'
                f"flags = [{flags}]\n"
                f'functions = [ {{ name = "value", address = {FIXTURE_LOAD:#x}, size = {FIXTURE_SIZE} }} ]\n'
            )

        seed_tag = "selftest-all-c-payload-seed"
        seed_build = cfg_dir.parent / "build" / seed_tag
        try:
            write(bytes(FIXTURE_SIZE))
            seed = run_tool(copy / "build.toml", seed_tag)
            image = seed_build / "image.bin"
            if not image.is_file() or image.stat().st_size != FIXTURE_SIZE:
                raise SystemExit(f"test setup: the fixture seed build produced no image:\n{seed.stdout}{seed.stderr}")
            write(image.read_bytes())
        finally:
            if seed_build.exists():
                shutil.rmtree(seed_build)

    def verify_all_c(report: dict):
        if report.get("coverage", {}).get("raw_payload_bytes") != 0:
            return "fixture payload is not fully owned by C"
        raw = [c for c in report.get("controls", []) if c["kind"] == "raw"]
        if len(raw) != 1 or raw[0]["applicable"]:
            return "raw control should be recorded as not applicable"
        functions = [c for c in report.get("controls", []) if c["kind"] == "function"]
        if not functions or not all(c["tripped"] for c in functions):
            return "function controls must still trip"
        return None

    def shadow_header(copy: Path):
        (copy / parsed["types"]["header"]).write_text("/* a hand-written header that would shadow the generated one */\n")

    def overlapping_field(copy: Path):
        path = copy / parsed["types"]["fields"]
        model = structgen.parse(path.read_text(), str(path))
        # The appended line joins the last struct in the file.
        first = min(model.structs[-1].fields, key=lambda f: f.offset)
        text = path.read_text()
        path.write_text(text + ("" if text.endswith("\n") else "\n") + f"{first.offset:#x} u8 selftest_overlap\n")

    types_cases = (
        [
            Case("types-shadowing-header", False, "would shadow the generated header", shadow_header),
            Case("include-dir-shadows-header", False, "would shadow the generated header", include_shadows_header),
            Case("types-overlapping-field", False, "overlaps field", overlapping_field),
        ]
        if "types" in parsed
        else []
    )

    # Only meaningful when the default compiler declares the limitation.
    float_cases = (
        [
            Case("float-with-no-float-compiler", False, "floating-point token", add_float),
            Case("float-text-in-literals", True, "", float_in_literals),
        ]
        if parsed["toolchain"]["cc1"].get("no_float")
        else []
    )
    asm_cases = [
        Case("asm-inline-in-c-unit", False, "inline assembly", add_inline_asm),
        Case("asm-word-in-literal", True, "", asm_in_literals),
    ]
    include_cases = [
        Case("include-dir", True, "", lambda c: use_include(c, True)),
        Case("include-dir-not-listed", False, "preprocess", lambda c: use_include(c, False)),
        Case("include-dir-changes-code", False, "text size mismatch", lambda c: use_include(
            c, True, "#define SELFTEST_INC_OK 1\nint selftest_inc_extra(void) { return 1; }\n")),
        Case("include-dir-missing", False, "include directory not found", lambda c: add_include_dir(c, "selftest_absent")),
    ]

    mutation_reason = (
        f"function '{target_unit['functions'][0]['name']}': bytes differ"
        if selftest["unit"] == unit["name"]
        else "bytes differ"
    )
    return float_cases + asm_cases + types_cases + include_cases + make_rodata_cases(cfg_dir, parsed) + make_padded_cases(cfg_dir, parsed) + make_data_cases(cfg_dir, parsed) + make_asm_cases(cfg_dir, parsed) + make_bss_cases(cfg_dir, parsed) + make_symbol_cases(cfg_dir, parsed) + make_sibling_cases(cfg_dir, parsed) + make_division_cases(cfg_dir, parsed) + [
        Case("clean", True, "", fndiff=(unit["name"], 0, "IDENTICAL")),
        Case(
            "source-mutation", False, mutation_reason, mutate_source,
            fndiff=(target_unit["name"], 1, "DIFFERENT"),
        ),
        Case("wrong-size", False, "text size mismatch", wrong_size),
        Case("overlapping-units", False, "overlap", overlapping),
        Case("symbol-removed", False, "undefined reference", remove_symbol),
        Case("symbol-duplicated", False, "symbols.ld defines unit function", duplicate_symbol),
        Case("wrong-cc1-hash", False, "cc1 sha256 mismatch", wrong_cc1_hash),
        Case("wrong-baseline-hash", False, "baseline sha256 mismatch", wrong_baseline_hash),
        Case("unknown-maspsx-commit", False, "maspsx commit", unknown_maspsx_commit),
        Case("dirty-maspsx-checkout", True, "", dirty_maspsx, verify_dirty_reported),
        Case("all-c-payload", True, "", all_c_fixture, verify_all_c),
    ]


class SeededFixture:
    """A fixture whose baseline comes from throwaway seed builds.

    The seed builds use fixed directory names taken from `label`. Several
    cases share one fixture object, and two objects of one class share the
    names, so `prepare` lets one caller at a time seed a label. Cases that use
    different labels still run side by side.
    """

    label = ""
    _locks: dict[str, threading.Lock] = {}
    _locks_guard = threading.Lock()

    def prepare(self) -> None:
        with SeededFixture._locks_guard:
            lock = SeededFixture._locks.setdefault(self.label, threading.Lock())
        with lock:
            self._prepare()


class RodataFixture(SeededFixture):
    """A one-unit configuration whose function has a jump table placed away from the text.

    The baseline comes from the same toolchain. A seed build with the correct
    declarations over a payload of filler bytes fails its comparison but leaves
    the image. That image, text and table with filler between and after them,
    becomes the baseline payload. The table sits RODATA_GAP bytes after the
    text, so it is not adjacent to it.
    """

    source = RODATA_SOURCE
    label = "rodata"  # names the throwaway seed builds
    kind = "rodata"  # the declaration key the fixture exercises: rodata, data or bss

    def __init__(self, cfg_dir: Path, parsed: dict):
        self.cfg_dir = cfg_dir
        self.flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])
        self.toolchain = fixture_toolchain(parsed)
        self.text_size = 0
        self.table = 0  # declared size: the object's read-only data rounded up to four
        self.object_rodata = 0
        self.baseline = b""

    def write(self, copy: Path, payload: bytes, text_size: int, rodata, extra: str = "") -> None:
        (copy / "pick.c").write_text(self.source)
        (copy / "other.c").write_text("int other(void) { return 1; }\n")
        (copy / "symbols.ld").write_text(f"g = {FIXTURE_LOAD + 0x10000:#x};\n")
        executable = fixture_executable(payload)
        (copy / "baseline.bin").write_bytes(executable)
        ro = "" if rodata is None else f"{self.kind} = {{ address = {rodata[0]:#x}, size = {rodata[1]} }}\n"
        (copy / "build.toml").write_text(
            "[baseline]\n"
            'executable = "baseline.bin"\n'
            f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
            + toml_table("toolchain", self.toolchain)
            + "[[unit]]\n"
            'name = "pick"\n'
            'source = "pick.c"\n'
            f"flags = [{self.flags}]\n"
            f'functions = [ {{ name = "pick", address = {FIXTURE_LOAD:#x}, size = {text_size} }} ]\n'
            + ro
            + extra
        )

    def _seed(self, name: str, payload: bytes, text_size: int, rodata) -> Path:
        """Run a throwaway build. Returns its build directory (caller removes it)."""
        copy = self.cfg_dir.with_name(f"{self.cfg_dir.name}.selftest-{name}")
        if copy.exists():
            shutil.rmtree(copy)
        copy.mkdir()
        build = self.cfg_dir.parent / "build" / f"selftest-{name}"
        cache = self.cfg_dir.parent / "build" / f".selftest-cache-{name}"
        for leftover in (build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)
        self.write(copy, payload, text_size, rodata)
        proc = run_tool(copy / "build.toml", f"selftest-{name}", cache)
        shutil.rmtree(copy)
        shutil.rmtree(cache, ignore_errors=True)
        self.last = proc
        return build

    def _prepare(self) -> None:
        if self.baseline:
            return
        from elftools.elf.elffile import ELFFile

        build = self._seed(f"{self.label}-geometry", bytes(64), 4, None)
        try:
            with open(build / "unit-pick.o", "rb") as handle:
                sizes = {sec.name: sec["sh_size"] for sec in ELFFile(handle).iter_sections()}
        finally:
            shutil.rmtree(build, ignore_errors=True)
        sections = matchbuild.BSS_SECTIONS if self.kind == "bss" else matchbuild.LOADED_SECTIONS[self.kind]
        self.object_rodata = sum(sizes.get(name, 0) for name in sections)
        if not sizes.get(".text") or not self.object_rodata:
            raise SystemExit(f"test setup: the fixture object has no {self.kind}: {sizes}")
        self.text_size, self.table = sizes[".text"], (self.object_rodata + 3) // 4 * 4
        build = self._seed(
            f"{self.label}-seed", bytes([FILLER]) * self.payload_size, self.text_size, (self.table_address, self.table)
        )
        try:
            image = build / "image.bin"
            if not image.is_file() or image.stat().st_size != self.payload_size:
                raise SystemExit(f"test setup: the rodata seed build produced no image:\n{self.last.stdout}")
            self.baseline = image.read_bytes()
        finally:
            shutil.rmtree(build, ignore_errors=True)
        if self.baseline[self.text_size : self.text_size + RODATA_GAP] != bytes([FILLER]) * RODATA_GAP:
            raise SystemExit("test setup: no raw filler between the text and the table")

    @property
    def table_address(self) -> int:
        return BSS_ADDRESS if self.kind == "bss" else FIXTURE_LOAD + self.text_size + RODATA_GAP

    @property
    def payload_size(self) -> int:
        # Bss holds no payload bytes: only raw filler follows the text.
        table = 0 if self.kind == "bss" else self.table
        return self.text_size + RODATA_GAP + table + RODATA_TAIL

    def install(self, copy: Path, *, address=None, size=None, declare=True, extra="", source=None, baseline=None):
        """Replace the copy with the fixture, with the given declaration."""
        self.prepare()
        for child in copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        rodata = None
        if declare:
            rodata = (self.table_address if address is None else address, self.table if size is None else size)
        self.write(copy, self.baseline if baseline is None else baseline, self.text_size, rodata, extra)
        if source is not None:
            (copy / "pick.c").write_text(source)


def make_rodata_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = RodataFixture(cfg_dir, parsed)

    def geometry():
        fx.prepare()
        return fx

    def declared(copy: Path):
        fx.install(copy)

    def verify_declared(report: dict):
        cov = report.get("coverage", {})
        if cov.get("rodata_bytes") != fx.table:
            return f"coverage rodata_bytes is {cov.get('rodata_bytes')}, want {fx.table}"
        if cov.get("c_bytes") != fx.text_size or cov.get("raw_payload_bytes") != RODATA_GAP + RODATA_TAIL:
            return f"coverage does not split text, rodata and raw: {cov}"
        ro = report["units"][0].get("rodata")
        if not ro or not ro["exact"] or ro["address"] != fx.table_address or ro["size"] != fx.table or ro["first_diff"] is not None:
            return f"per-unit rodata record is wrong: {ro}"
        kinds = {c["kind"]: c for c in report["controls"]}
        for kind in ("function", "rodata", "raw"):
            if kind not in kinds or not kinds[kind]["applicable"] or not kinds[kind]["tripped"]:
                return f"control {kind!r} missing or not tripped: {report['controls']}"
        return None

    def at(delta: int):
        return lambda copy: fx.install(copy, address=geometry().table_address + delta)

    def sized(delta: int):
        return lambda copy: fx.install(copy, size=geometry().table + delta)

    def own_text(copy: Path):
        fx.install(copy, address=FIXTURE_LOAD + 4)

    def outside(copy: Path):
        fx.install(copy, address=FIXTURE_LOAD + geometry().payload_size)

    def other_text(copy: Path):
        extra = (
            '\n[[unit]]\nname = "other"\nsource = "other.c"\nflags = ["-O2"]\n'
            f'functions = [ {{ name = "other", address = {geometry().table_address:#x}, size = 4 }} ]\n'
        )
        fx.install(copy, extra=extra)

    def other_rodata(copy: Path):
        tail = FIXTURE_LOAD + geometry().payload_size - 4
        extra = (
            '\n[[unit]]\nname = "other"\nsource = "other.c"\nflags = ["-O2"]\n'
            f'functions = [ {{ name = "other", address = {tail:#x}, size = 4 }} ]\n'
            f"rodata = {{ address = {geometry().table_address + 4:#x}, size = 8 }}\n"
        )
        fx.install(copy, extra=extra)

    def no_table(copy: Path):
        fx.install(copy, source="int pick(int a) { return a + 1; }\n")

    def table_byte_differs(copy: Path):
        # Same code and declaration; one byte of the baseline table differs.
        geometry()
        baseline = bytearray(fx.baseline)
        baseline[fx.table_address - FIXTURE_LOAD + 8] ^= 0x10
        fx.install(copy, baseline=bytes(baseline))

    def verify_table_differs(report: dict):
        ro = report["units"][0].get("rodata") if "units" in report else None
        if not ro or ro["exact"] or ro["first_diff"] != 8:
            return f"rodata record should report offset 8: {ro}"
        if not all(f["exact"] for f in report["units"][0]["functions"]):
            return "the code must still be exact"
        return None

    return [
        Case("rodata-declared", True, "", declared, verify_declared, fndiff=("pick", 0, "IDENTICAL")),
        Case("rodata-undeclared", False, "rodata must be declared", lambda c: fx.install(c, declare=False)),
        Case("rodata-size-too-small", False, "rodata size mismatch", sized(-4)),
        Case("rodata-size-too-large", False, "rodata size mismatch", sized(4)),
        Case("rodata-size-misaligned", False, "is not a multiple of four", sized(2)),
        Case("rodata-zero-size", False, "size must be greater than zero", lambda c: fx.install(c, size=0)),
        Case("rodata-address-wrong", False, "rodata: bytes differ from baseline", at(8)),
        Case("rodata-address-misaligned", False, "is not a multiple of four", at(2)),
        Case("rodata-table-byte-differs", False, "rodata: bytes differ from baseline at offset 8", table_byte_differs, verify_table_differs),
        Case("rodata-overlaps-other-text", False, "overlaps the text range of unit 'other'", other_text),
        Case("rodata-overlaps-other-rodata", False, "overlaps the rodata range of unit 'pick'", other_rodata),
        Case("rodata-overlaps-own-text", False, "overlaps its own text range", own_text),
        Case("rodata-outside-payload", False, "outside the payload", outside),
        Case("rodata-declared-but-absent", False, "declares rodata", no_table),
    ]


class PaddedFixture(RodataFixture):
    """Read-only data that ends two bytes short of a word."""

    source = PADDED_SOURCE
    label = "padded"


def make_padded_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = PaddedFixture(cfg_dir, parsed)

    def geometry():
        fx.prepare()
        if fx.object_rodata % 4 == 0:
            raise SystemExit(f"test setup: the padded fixture has {fx.object_rodata} bytes of read-only data, a whole number of words")
        return fx

    def verify_padded(report: dict):
        ro = report["units"][0].get("rodata")
        if not ro or not ro["exact"] or ro["size"] != fx.table or fx.table - fx.object_rodata not in (1, 2, 3):
            return f"the padded range should be owned and exact: {ro}, object {fx.object_rodata}"
        if report.get("coverage", {}).get("rodata_bytes") != fx.table:
            return f"coverage must count the padding as rodata: {report.get('coverage')}"
        return None

    def padding_differs(copy: Path):
        geometry()
        baseline = bytearray(fx.baseline)
        baseline[fx.table_address - FIXTURE_LOAD + fx.table - 1] ^= 0x01
        fx.install(copy, baseline=bytes(baseline))

    def verify_padding_differs(report: dict):
        ro = report["units"][0].get("rodata") if "units" in report else None
        if not ro or ro["exact"] or ro["first_diff"] != fx.table - 1:
            return f"the rodata record should report the last padding byte: {ro}"
        return None

    return [
        Case("rodata-padded", True, "", lambda c: geometry().install(c), verify_padded, fndiff=("pick", 0, "IDENTICAL")),
        Case("rodata-padding-differs", False, "rodata: bytes differ from baseline", padding_differs, verify_padding_differs),
        Case("rodata-padded-one-word-short", False, "rodata size mismatch", lambda c: geometry().install(c, size=fx.table - 4)),
        Case("rodata-padded-one-word-long", False, "rodata size mismatch", lambda c: geometry().install(c, size=fx.table + 4)),
    ]


class DataFixture(RodataFixture):
    """Initialised data, placed away from the text."""

    source = DATA_SOURCE
    label = "data"
    kind = "data"


class DataPaddedFixture(DataFixture):
    """Initialised data that ends two bytes short of a word."""

    source = DATA_PADDED_SOURCE
    label = "datapadded"


class BssFixture(RodataFixture):
    """An uninitialised global that the code refers to, with its bss outside the payload."""

    source = BSS_SOURCE
    label = "bss"
    kind = "bss"


def make_data_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = DataFixture(cfg_dir, parsed)
    pad = DataPaddedFixture(cfg_dir, parsed)

    def geometry():
        fx.prepare()
        return fx

    def verify_declared(report: dict):
        cov = report.get("coverage", {})
        if cov.get("data_bytes") != fx.table or cov.get("rodata_bytes") != 0:
            return f"coverage data_bytes is {cov.get('data_bytes')}, want {fx.table}: {cov}"
        if cov.get("c_bytes") != fx.text_size or cov.get("raw_payload_bytes") != RODATA_GAP + RODATA_TAIL:
            return f"coverage does not split text, data and raw: {cov}"
        unit = report["units"][0]
        rec = unit.get("data")
        if not rec or not rec["exact"] or rec["address"] != fx.table_address or rec["size"] != fx.table or rec["first_diff"] is not None:
            return f"per-unit data record is wrong: {rec}"
        if unit.get("rodata") is not None or unit.get("bss") is not None:
            return f"unit must not report rodata or bss: {unit}"
        kinds = {c["kind"]: c for c in report["controls"]}
        for kind in ("function", "data", "raw"):
            if kind not in kinds or not kinds[kind]["applicable"] or not kinds[kind]["tripped"]:
                return f"control {kind!r} missing or not tripped: {report['controls']}"
        return None

    def at(delta: int):
        return lambda copy: fx.install(copy, address=geometry().table_address + delta)

    def sized(delta: int):
        return lambda copy: fx.install(copy, size=geometry().table + delta)

    def other_unit(address: int, extra: str = "") -> str:
        return (
            '\n[[unit]]\nname = "other"\nsource = "other.c"\nflags = ["-O2"]\n'
            f'functions = [ {{ name = "other", address = {address:#x}, size = 4 }} ]\n' + extra
        )

    def other_text(copy: Path):
        fx.install(copy, extra=other_unit(geometry().table_address))

    def other_data(copy: Path):
        tail = FIXTURE_LOAD + geometry().payload_size - 4
        extra = other_unit(tail, f"data = {{ address = {geometry().table_address + 4:#x}, size = 8 }}\n")
        fx.install(copy, extra=extra)

    def other_rodata(copy: Path):
        tail = FIXTURE_LOAD + geometry().payload_size - 4
        extra = other_unit(tail, f"rodata = {{ address = {geometry().table_address + 4:#x}, size = 8 }}\n")
        fx.install(copy, extra=extra)

    def no_data(copy: Path):
        fx.install(copy, source="int pick(int a) { return a + 1; }\n")

    def byte_differs(copy: Path):
        geometry()
        baseline = bytearray(fx.baseline)
        baseline[fx.table_address - FIXTURE_LOAD + 8] ^= 0x10
        fx.install(copy, baseline=bytes(baseline))

    def verify_differs(report: dict):
        rec = report["units"][0].get("data") if "units" in report else None
        if not rec or rec["exact"] or rec["first_diff"] != 8:
            return f"data record should report offset 8: {rec}"
        if not all(f["exact"] for f in report["units"][0]["functions"]):
            return "the code must still be exact"
        return None

    def padded():
        pad.prepare()
        if pad.object_rodata % 4 == 0:
            raise SystemExit(f"test setup: the padded fixture has {pad.object_rodata} bytes of data, a whole number of words")
        return pad

    def verify_padded(report: dict):
        rec = report["units"][0].get("data")
        if not rec or not rec["exact"] or rec["size"] != pad.table or pad.table - pad.object_rodata not in (1, 2, 3):
            return f"the padded range should be owned and exact: {rec}, object {pad.object_rodata}"
        if report.get("coverage", {}).get("data_bytes") != pad.table:
            return f"coverage must count the padding as data: {report.get('coverage')}"
        return None

    def padding_differs(copy: Path):
        padded()
        baseline = bytearray(pad.baseline)
        baseline[pad.table_address - FIXTURE_LOAD + pad.table - 1] ^= 0x01
        pad.install(copy, baseline=bytes(baseline))

    return [
        Case("data-declared", True, "", lambda c: fx.install(c), verify_declared, fndiff=("pick", 0, "IDENTICAL")),
        Case("data-undeclared", False, "data must be declared", lambda c: fx.install(c, declare=False)),
        Case("data-size-too-small", False, "data size mismatch", sized(-4)),
        Case("data-size-too-large", False, "data size mismatch", sized(4)),
        Case("data-size-misaligned", False, "is not a multiple of four", sized(2)),
        Case("data-zero-size", False, "size must be greater than zero", lambda c: fx.install(c, size=0)),
        Case("data-address-wrong", False, "data: bytes differ from baseline", at(8)),
        Case("data-address-misaligned", False, "is not a multiple of four", at(2)),
        Case("data-byte-differs", False, "data: bytes differ from baseline at offset 8", byte_differs, verify_differs),
        Case("data-overlaps-other-text", False, "overlaps the text range of unit 'other'", other_text),
        Case("data-overlaps-other-data", False, "overlaps the data range of unit 'pick'", other_data),
        Case("data-overlaps-other-rodata", False, "overlaps the data range of unit 'pick'", other_rodata),
        Case("data-overlaps-own-text", False, "overlaps its own text range", lambda c: fx.install(c, address=FIXTURE_LOAD + 4)),
        Case("data-outside-payload", False, "outside the payload", lambda c: fx.install(c, address=FIXTURE_LOAD + geometry().payload_size)),
        Case("data-declared-but-absent", False, "declares data", no_data),
        Case("data-padded", True, "", lambda c: padded() and pad.install(c), verify_padded),
        Case("data-padding-differs", False, "data: bytes differ from baseline", padding_differs),
        Case("data-padded-one-word-short", False, "data size mismatch", lambda c: padded() and pad.install(c, size=pad.table - 4)),
    ]


def make_bss_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = BssFixture(cfg_dir, parsed)

    def geometry():
        fx.prepare()
        return fx

    def verify_declared(report: dict):
        cov = report.get("coverage", {})
        if report.get("bss_bytes") != fx.table or "bss_bytes" in cov:
            return f"bss_bytes must sit next to coverage, not in it: {report.get('bss_bytes')}, {cov}"
        if cov.get("c_bytes") != fx.text_size or cov.get("raw_payload_bytes") != RODATA_GAP + RODATA_TAIL:
            return f"bss must add nothing to coverage: {cov}"
        unit = report["units"][0]
        if unit.get("bss") != {"address": BSS_ADDRESS, "size": fx.table}:
            return f"per-unit bss record is wrong: {unit.get('bss')}"
        if any(c["kind"] == "bss" for c in report["controls"]):
            return "bss has no control"
        return None

    def at(delta: int):
        return lambda copy: fx.install(copy, address=BSS_ADDRESS + delta)

    def sized(delta: int):
        return lambda copy: fx.install(copy, size=geometry().table + delta)

    def other_bss(copy: Path):
        tail = FIXTURE_LOAD + geometry().payload_size - 4
        extra = (
            '\n[[unit]]\nname = "other"\nsource = "other.c"\nflags = ["-O2"]\n'
            f'functions = [ {{ name = "other", address = {tail:#x}, size = 4 }} ]\n'
            f"bss = {{ address = {BSS_ADDRESS + 4:#x}, size = 8 }}\n"
        )
        fx.install(copy, extra=extra)

    def no_bss(copy: Path):
        fx.install(copy, source="int pick(int a) { return a + 1; }\n")

    return [
        Case("bss-declared", True, "", lambda c: fx.install(c), verify_declared, fndiff=("pick", 0, "IDENTICAL")),
        Case("bss-undeclared", False, "bss must be declared", lambda c: fx.install(c, declare=False)),
        Case("bss-size-too-small", False, "bss size mismatch", sized(-4)),
        Case("bss-size-too-large", False, "bss size mismatch", sized(4)),
        Case("bss-size-misaligned", False, "is not a multiple of four", sized(2)),
        Case("bss-zero-size", False, "size must be greater than zero", lambda c: fx.install(c, size=0)),
        Case("bss-address-wrong", False, "function 'pick': bytes differ from baseline", at(8)),
        Case("bss-address-misaligned", False, "is not a multiple of four", at(2)),
        Case("bss-touches-payload", False, "touches the payload", lambda c: fx.install(c, address=FIXTURE_LOAD + 8)),
        Case("bss-overlaps-other-bss", False, "overlaps the bss range", other_bss),
        Case("bss-declared-but-absent", False, "declares bss", no_bss),
    ]


class DivisionFixture(SeededFixture):
    """One function that divides, followed by raw filler.

    The baseline is the image of a seed build made with `expand_div = true`.
    """

    label = "division"
    TAIL = 0x20

    def __init__(self, cfg_dir: Path, parsed: dict):
        self.cfg_dir = cfg_dir
        self.flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])
        self.toolchain = fixture_toolchain(parsed)
        self.text_size = 0
        self.baseline = b""

    def write(self, copy: Path, payload: bytes, text_size: int, expand_div: bool) -> None:
        (copy / "pick.c").write_text(DIVISION_SOURCE)
        (copy / "symbols.ld").write_text("/* The fixture needs no external symbols. */\n")
        executable = fixture_executable(payload)
        (copy / "baseline.bin").write_bytes(executable)
        (copy / "build.toml").write_text(
            "[baseline]\n"
            'executable = "baseline.bin"\n'
            f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
            + toml_table("toolchain", {**self.toolchain, "expand_div": expand_div})
            + "[[unit]]\n"
            'name = "pick"\n'
            'source = "pick.c"\n'
            f"flags = [{self.flags}]\n"
            f'functions = [ {{ name = "pick", address = {FIXTURE_LOAD:#x}, size = {text_size} }} ]\n'
        )

    def _seed(self, name: str, payload: bytes, text_size: int) -> Path:
        copy = self.cfg_dir.with_name(f"{self.cfg_dir.name}.selftest-{name}")
        build = self.cfg_dir.parent / "build" / f"selftest-{name}"
        cache = self.cfg_dir.parent / "build" / f".selftest-cache-{name}"
        for leftover in (copy, build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)
        copy.mkdir()
        self.write(copy, payload, text_size, True)
        self.last = run_tool(copy / "build.toml", f"selftest-{name}", cache)
        shutil.rmtree(copy)
        shutil.rmtree(cache, ignore_errors=True)
        return build

    def _prepare(self) -> None:
        if self.baseline:
            return
        from elftools.elf.elffile import ELFFile

        build = self._seed(f"{self.label}-geometry", bytes(64), 4)
        try:
            with open(build / "unit-pick.o", "rb") as handle:
                self.text_size = ELFFile(handle).get_section_by_name(".text")["sh_size"]
        finally:
            shutil.rmtree(build, ignore_errors=True)
        build = self._seed(f"{self.label}-seed", bytes([FILLER]) * (self.text_size + self.TAIL), self.text_size)
        try:
            image = build / "image.bin"
            if not image.is_file() or image.stat().st_size != self.text_size + self.TAIL:
                raise SystemExit(f"test setup: the division seed build produced no image:\n{self.last.stdout}")
            self.baseline = image.read_bytes()
        finally:
            shutil.rmtree(build, ignore_errors=True)

    def install(self, copy: Path, expand_div: bool) -> None:
        self.prepare()
        for child in copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        self.write(copy, self.baseline, self.text_size, expand_div)


class AsmFixture(SeededFixture):
    """A C unit followed by an assembly unit, then raw filler.

    The baseline is the image of a seed build over a payload of filler bytes.
    """

    label = "asm"
    TAIL = 0x20

    def __init__(self, cfg_dir: Path, parsed: dict):
        self.cfg_dir = cfg_dir
        self.flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])
        self.toolchain = fixture_toolchain(parsed)
        self.text_size = 0
        self.baseline = b""

    @property
    def payload_size(self) -> int:
        return self.text_size + ASM_SIZE + self.TAIL

    def write(self, copy: Path, payload: bytes, text_size: int, asm_source: str, extra: str = "", asm_extra: str = ASM_KIND) -> None:
        (copy / "pick.c").write_text(ASM_C_SOURCE)
        (copy / "stub.s").write_text(asm_source)
        (copy / "symbols.ld").write_text("/* The fixture needs no external symbols. */\n")
        executable = fixture_executable(payload)
        (copy / "baseline.bin").write_bytes(executable)
        (copy / "build.toml").write_text(
            "[baseline]\n"
            'executable = "baseline.bin"\n'
            f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
            + toml_table("toolchain", self.toolchain)
            + "[[unit]]\n"
            'name = "pick"\n'
            'source = "pick.c"\n'
            f"flags = [{self.flags}]\n"
            f'functions = [ {{ name = "pick", address = {FIXTURE_LOAD:#x}, size = {text_size} }} ]\n\n'
            "[[unit]]\n"
            'name = "stub"\n'
            'source = "stub.s"\n'
            f'functions = [ {{ name = "stub", address = {FIXTURE_LOAD + text_size:#x}, size = {ASM_SIZE} }} ]\n'
            + asm_extra
            + extra
        )

    def _seed(self, name: str, payload: bytes, text_size: int) -> Path:
        copy = self.cfg_dir.with_name(f"{self.cfg_dir.name}.selftest-{name}")
        build = self.cfg_dir.parent / "build" / f"selftest-{name}"
        cache = self.cfg_dir.parent / "build" / f".selftest-cache-{name}"
        for leftover in (copy, build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)
        copy.mkdir()
        self.write(copy, payload, text_size, ASM_SOURCE)
        self.last = run_tool(copy / "build.toml", f"selftest-{name}", cache)
        shutil.rmtree(copy)
        shutil.rmtree(cache, ignore_errors=True)
        return build

    def _prepare(self) -> None:
        if self.baseline:
            return
        from elftools.elf.elffile import ELFFile

        build = self._seed(f"{self.label}-geometry", bytes(64), 4)
        try:
            with open(build / "unit-pick.o", "rb") as handle:
                self.text_size = ELFFile(handle).get_section_by_name(".text")["sh_size"]
        finally:
            shutil.rmtree(build, ignore_errors=True)
        build = self._seed(f"{self.label}-seed", bytes([FILLER]) * self.payload_size, self.text_size)
        try:
            image = build / "image.bin"
            if not image.is_file() or image.stat().st_size != self.payload_size:
                raise SystemExit(f"test setup: the assembly seed build produced no image:\n{self.last.stdout}")
            self.baseline = image.read_bytes()
        finally:
            shutil.rmtree(build, ignore_errors=True)

    def install(self, copy: Path, *, asm_source: str = ASM_SOURCE, extra: str = "", asm_extra: str = ASM_KIND) -> None:
        self.prepare()
        for child in copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        self.write(copy, self.baseline, self.text_size, asm_source, extra, asm_extra)


def make_asm_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = AsmFixture(cfg_dir, parsed)

    def verify_unit(report: dict):
        kinds = {u["name"]: u.get("kind") for u in report["units"]}
        if kinds != {"pick": "c", "stub": "asm"}:
            return f"unit kinds are wrong: {kinds}"
        cov = report["coverage"]
        if cov.get("asm_functions") != 1 or cov.get("asm_bytes") != ASM_SIZE or cov.get("c_functions") != 1:
            return f"assembly accounting is wrong: {cov}"
        if cov["c_bytes"] != fx.text_size:
            return f"c_bytes must exclude the assembly unit: {cov}"
        owned = cov["c_bytes"] + cov["asm_bytes"] + cov["raw_payload_bytes"] + cov["rodata_bytes"] + cov["data_bytes"]
        if owned != fx.payload_size:
            return f"coverage does not add up to the payload: {owned} != {fx.payload_size}"
        if report["cache"]["units"]["stub"] != {"cache": "off", "key": None}:
            return f"an assembly unit is not cached: {report['cache']['units']['stub']}"
        if "stub" in report["inputs"]["preprocessed"]:
            return "an assembly unit is not preprocessed"
        control = [c for c in report["controls"] if c["kind"] == "function" and c["target"] == "stub"]
        if len(control) != 1 or not control[0]["tripped"]:
            return f"function control for the assembly function missing or not tripped: {report['controls']}"
        return None

    def unknown_kind(copy: Path):
        fx.install(copy, asm_extra='kind = "pascal"\n')

    return [
        Case("asm-unit", True, "", lambda c: fx.install(c), verify_unit, fndiff=("stub", 0, "IDENTICAL")),
        Case("asm-instruction-differs", False, "function 'stub': bytes differ from baseline", lambda c: fx.install(c, asm_source=ASM_SOURCE.replace("$a0, 1", "$a0, 2"))),
        Case("asm-unit-with-flags", False, "an assembly unit takes no flags", lambda c: fx.install(c, asm_extra=ASM_KIND + 'flags = ["-O2"]\n')),
        Case("kind-unknown", False, "kind must be 'c' or 'asm'", unknown_kind),
    ]


class SiblingFixture(SeededFixture):
    """Two C units: one owns a data and a bss variable, the other's function uses both.

    The baseline is the image of a seed build over filler bytes. Layout: the
    owner text, the consumer text, RODATA_GAP filler, the owner data, RODATA_TAIL
    filler. The owner bss sits at BSS_ADDRESS, outside the payload.
    """

    label = "sibling"

    def __init__(self, cfg_dir: Path, parsed: dict):
        self.cfg_dir = cfg_dir
        self.flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])
        self.toolchain = fixture_toolchain(parsed)
        self.owner_text = self.consumer_text = self.data = self.bss = 0
        self.baseline = b""

    @property
    def data_address(self) -> int:
        return FIXTURE_LOAD + self.owner_text + self.consumer_text + RODATA_GAP

    @property
    def payload_size(self) -> int:
        return self.owner_text + self.consumer_text + RODATA_GAP + self.data + RODATA_TAIL

    def write(self, copy: Path, payload: bytes, sizes: tuple[int, int], data, bss, consumer_extra: int = 0) -> None:
        (copy / "owner.c").write_text(SIBLING_OWNER_SOURCE)
        (copy / "consumer.c").write_text(SIBLING_USER_SOURCE)
        (copy / "symbols.ld").write_text("/* The fixture needs no external symbols. */\n")
        executable = fixture_executable(payload)
        (copy / "baseline.bin").write_bytes(executable)
        owner_text, consumer_text = sizes
        owned = "".join(
            f"{kind} = {{ address = {decl[0]:#x}, size = {decl[1]} }}\n" for kind, decl in (("data", data), ("bss", bss)) if decl
        )
        (copy / "build.toml").write_text(
            "[baseline]\n"
            'executable = "baseline.bin"\n'
            f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
            + toml_table("toolchain", self.toolchain)
            + "[[unit]]\n"
            'name = "owner"\n'
            'source = "owner.c"\n'
            f"flags = [{self.flags}]\n"
            f'functions = [ {{ name = "owner", address = {FIXTURE_LOAD:#x}, size = {owner_text} }} ]\n'
            + owned
            + "\n[[unit]]\n"
            'name = "consumer"\n'
            'source = "consumer.c"\n'
            f"flags = [{self.flags}]\n"
            f'functions = [ {{ name = "consumer", address = {FIXTURE_LOAD + owner_text:#x}, size = {consumer_text + consumer_extra} }} ]\n'
        )

    def _seed(self, name: str, payload: bytes, sizes: tuple[int, int], data, bss) -> Path:
        copy = self.cfg_dir.with_name(f"{self.cfg_dir.name}.selftest-{name}")
        build = self.cfg_dir.parent / "build" / f"selftest-{name}"
        cache = self.cfg_dir.parent / "build" / f".selftest-cache-{name}"
        for leftover in (copy, build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)
        copy.mkdir()
        self.write(copy, payload, sizes, data, bss)
        self.last = run_tool(copy / "build.toml", f"selftest-{name}", cache)
        shutil.rmtree(copy)
        shutil.rmtree(cache, ignore_errors=True)
        return build

    def _prepare(self) -> None:
        if self.baseline:
            return
        from elftools.elf.elffile import ELFFile

        build = self._seed(f"{self.label}-geometry", bytes(64), (4, 4), None, None)
        try:
            found = {}
            for unit in ("owner", "consumer"):
                with open(build / f"unit-{unit}.o", "rb") as handle:
                    found[unit] = {sec.name: sec["sh_size"] for sec in ELFFile(handle).iter_sections()}
        finally:
            shutil.rmtree(build, ignore_errors=True)
        self.owner_text, self.consumer_text = found["owner"][".text"], found["consumer"][".text"]
        self.data = (found["owner"].get(".data", 0) + 3) // 4 * 4
        self.bss = sum(found["owner"].get(name, 0) for name in matchbuild.BSS_SECTIONS)
        self.bss = (self.bss + 3) // 4 * 4
        if not (self.owner_text and self.consumer_text and self.data and self.bss):
            raise SystemExit(f"test setup: the sibling objects lack text, data or bss: {found}")
        build = self._seed(
            f"{self.label}-seed",
            bytes([FILLER]) * self.payload_size,
            (self.owner_text, self.consumer_text),
            (self.data_address, self.data),
            (BSS_ADDRESS, self.bss),
        )
        try:
            image = build / "image.bin"
            if not image.is_file() or image.stat().st_size != self.payload_size:
                raise SystemExit(f"test setup: the sibling seed build produced no image:\n{self.last.stdout}")
            self.baseline = image.read_bytes()
        finally:
            shutil.rmtree(build, ignore_errors=True)

    def install(self, copy: Path, *, consumer_extra: int = 0) -> None:
        self.prepare()
        for child in copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        self.write(
            copy,
            self.baseline,
            (self.owner_text, self.consumer_text),
            (self.data_address, self.data),
            (BSS_ADDRESS, self.bss),
            consumer_extra,
        )


def make_sibling_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = SiblingFixture(cfg_dir, parsed)
    return [
        Case("sibling-data-bss", True, "", lambda c: fx.install(c), fndiff=("consumer", 0, "IDENTICAL")),
        # The size is wrong, so the build stops before the link; fndiff still links the unit.
        Case(
            "sibling-data-bss-before-link",
            False,
            "text size mismatch",
            lambda c: fx.install(c, consumer_extra=4),
            fndiff=("consumer", 1, "DIFFERENT"),
        ),
    ]


def make_division_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    fx = DivisionFixture(cfg_dir, parsed)

    def verify_on(report: dict):
        if report.get("inputs", {}).get("maspsx_flags") != ["--expand-div"]:
            return f"the report must record the maspsx flag: {report.get('inputs', {}).get('maspsx_flags')}"
        return None

    def not_boolean(copy: Path):
        fx.install(copy, True)
        path = copy / "build.toml"
        path.write_text(replace_once(path.read_text(), "expand_div = true", 'expand_div = "yes"', "expand_div"))

    return [
        Case("expand-div-on", True, "", lambda c: fx.install(c, True), verify_on, fndiff=("pick", 0, "IDENTICAL")),
        # The same source without the option: the assembler expands the division its own way.
        Case("expand-div-off", False, "text size mismatch", lambda c: fx.install(c, False)),
        Case("expand-div-not-boolean", False, "'expand_div' must be a boolean", not_boolean),
    ]


def make_symbol_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    """Names that `symbols.ld` assigns although a unit object defines them."""
    bss = BssFixture(cfg_dir, parsed)
    data = DataFixture(cfg_dir, parsed)

    def assigned(fx, text: str, **install):
        def mutate(copy: Path):
            fx.install(copy, **install)
            with open(copy / "symbols.ld", "a") as handle:
                handle.write(text)

        return mutate

    def table_assignment(copy: Path):
        assigned(data, f"table = {data.table_address:#x};\n")(copy)

    return [
        # The declared bss is at the wrong place, but the assignments give the unit's own
        # variables their old addresses: the link bound them as absolute and the build passed.
        Case(
            "symbol-bss-overridden",
            False,
            "which unit 'pick' defines",
            assigned(bss, f"counter = {BSS_ADDRESS:#x}; total = {BSS_ADDRESS + 4:#x};\n", address=BSS_ADDRESS + 0x100),
        ),
        Case("symbol-data-overridden", False, "which unit 'pick' defines", table_assignment),
        Case("symbol-alias-allowed", True, "", assigned(bss, f"counter_alias = {BSS_ADDRESS:#x};\n")),
    ]


def run_case(case: Case, cfg_dir: Path) -> tuple[bool, str]:
    copy = cfg_dir.with_name(f"{cfg_dir.name}.selftest-{case.name}")
    tag = f"selftest-{case.name}"
    build = cfg_dir.parent / "build" / tag
    cache = cfg_dir.parent / "build" / f".selftest-cache-{case.name}"
    try:
        for leftover in (copy, build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)
        shutil.copytree(cfg_dir, copy)
        if case.mutate:
            case.mutate(copy)
        proc = run_tool(copy / "build.toml", tag, cache)
        output = proc.stdout + proc.stderr
        if case.expect_pass:
            if proc.returncode != 0 or "RESULT: PASS" not in output:
                return False, f"expected success, exit {proc.returncode}:\n{output}"
            message = "passes as required"
        else:
            if proc.returncode == 0:
                return False, "expected failure but the tool exited 0"
            if case.reason not in output:
                return False, f"failed for the wrong reason (wanted {case.reason!r}), exit {proc.returncode}:\n{output}"
            message = f"fails as required ({case.reason})"
        if case.verify:
            problem = case.verify(json.loads((build / "report.json").read_text()))
            if problem:
                return False, problem
        if case.fndiff:
            unit_name, want_status, want_text = case.fndiff
            diff = subprocess.run(
                [sys.executable, str(FNDIFF), "--config", str(copy / "build.toml"), "--tag", tag, unit_name],
                capture_output=True,
                text=True,
            )
            if diff.returncode != want_status or want_text not in diff.stdout:
                return False, f"fndiff exit {diff.returncode}, wanted {want_status} with {want_text!r}:\n{diff.stdout[-600:]}{diff.stderr[-600:]}"
            message += f"; fndiff reports {want_text}"
        return True, message
    finally:
        for leftover in (copy, build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)


# ---------------------------------------------------------------------------
# Object cache cases: several builds of one copy against one cache directory.


class CacheCase:
    """A scenario driven by `body(ctx)`; it returns an error string or None."""

    def __init__(self, name, body):
        self.name = name
        self.body = body


class CacheContext:
    def __init__(self, name: str, cfg_dir: Path, parsed: dict):
        self.copy = cfg_dir.with_name(f"{cfg_dir.name}.selftest-{name}")
        self.tag = f"selftest-{name}"
        self.build = cfg_dir.parent / "build" / self.tag
        self.cache = cfg_dir.parent / "build" / f".selftest-cache-{name}"
        self.wrappers = cfg_dir.with_name(f"{cfg_dir.name}.selftest-{name}-bin")
        self.config = self.copy / "build.toml"
        self.target = next(u for u in parsed["unit"] if u["name"] == parsed["selftest"]["unit"])

    def cleanup(self):
        for leftover in (self.copy, self.build, self.cache, self.wrappers):
            if leftover.exists():
                shutil.rmtree(leftover)

    def run(self, extra: tuple[str, ...] = (), env: dict | None = None):
        """Build once. Returns (process, report, per-unit cache records)."""
        proc = run_tool(self.config, self.tag, self.cache, extra, env)
        try:
            report = json.loads((self.build / "report.json").read_text())
        except OSError:
            report = {}  # the build aborted before writing a report
        # An assembly unit is never cached and has no key. The cases speak about the units that are.
        records = report.get("cache", {}).get("units", {})
        return proc, report, {name: record for name, record in records.items() if record.get("key") is not None}

    def entry(self, key: str) -> Path:
        return self.cache / key


def states(units: dict) -> dict:
    return {name: record["cache"] for name, record in units.items()}


def passed(proc) -> bool:
    return proc.returncode == 0 and "RESULT: PASS" in proc.stdout


def describe(proc) -> str:
    return f"exit {proc.returncode}:\n{(proc.stdout + proc.stderr)[-800:]}"


def make_cache_cases(parsed: dict) -> list[CacheCase]:
    selftest = parsed["selftest"]
    first = parsed["unit"][0]["name"]

    def edit_config(ctx: CacheContext, pattern: str, repl, what: str):
        text = ctx.config.read_text()
        new, count = re.subn(pattern, repl, text, count=1)
        if count != 1:
            raise SystemExit(f"test setup: cannot locate {what} in build.toml")
        ctx.config.write_text(new)

    def reuse(ctx):
        p1, r1, u1 = ctx.run()
        if not passed(p1) or any(v != "miss" for v in states(u1).values()):
            return f"cold build: expected PASS with all misses, got {states(u1)}; {describe(p1)}"
        p2, r2, u2 = ctx.run()
        if not passed(p2):
            return f"warm build did not pass; {describe(p2)}"
        if any(v != "hit" for v in states(u2).values()):
            return f"warm build: expected all hits, got {states(u2)}"
        if f"cache: {len(u2)} hits, 0 misses" not in p2.stdout:
            return "summary line lacks the hit totals"
        if {n: v["key"] for n, v in u1.items()} != {n: v["key"] for n, v in u2.items()}:
            return "keys changed between identical builds"
        for field in ("image_sha256", "executable_sha256"):
            if r1[field] != r2[field]:
                return f"{field} differs between cold and warm builds"
        return None

    def source_mutation(ctx):
        p1, _, u1 = ctx.run()
        if not passed(p1):
            return f"clean cached build did not pass; {describe(p1)}"
        path = ctx.copy / ctx.target["source"]
        text = path.read_text()
        if text.count(selftest["find"]) != 1:
            raise SystemExit("test setup: selftest 'find' text not found exactly once")
        path.write_text(text.replace(selftest["find"], selftest["replace"]))
        p2, _, u2 = ctx.run()
        if p2.returncode == 0 or "bytes differ" not in p2.stdout:
            return f"mutated build must fail on the byte comparison; {describe(p2)}"
        name = ctx.target["name"]
        if u2[name]["cache"] != "miss" or u2[name]["key"] == u1[name]["key"]:
            return "the mutated unit must miss under a new key"
        return None

    def key_inputs(ctx):
        p1, _, u1 = ctx.run()
        if not passed(p1):
            return f"clean build did not pass; {describe(p1)}"
        # A repeated flag is valid for the compiler and changes the flag list.
        edit_config(
            ctx,
            r"flags\s*=\s*\[([^\]]*?)\s*,?\s*\]",
            lambda m: f"flags = [{m.group(1)}, {m.group(1).split(',')[-1].strip()}]",
            "first unit flags",
        )
        _, _, u2 = ctx.run()
        if u2[first]["key"] == u1[first]["key"] or u2[first]["cache"] != "miss":
            return "changed flags did not change the key"
        if any(u2[n]["cache"] != "hit" for n in u2 if n != first):
            return f"other units should still hit: {states(u2)}"
        old = parsed["toolchain"]["aspsx_version"]
        edit_config(ctx, r'(aspsx_version\s*=\s*)"[^"]*"', lambda m: f'{m.group(1)}"{old}.1"', "aspsx_version")
        _, _, u3 = ctx.run()
        if any(u3[n]["key"] == u1[n]["key"] or u3[n]["cache"] != "miss" for n in u3):
            return "a changed aspsx version must change every key and miss"
        if "expand_div" in parsed["toolchain"]:
            edit_config(
                ctx, r"(expand_div\s*=\s*)(true|false)",
                lambda m: m.group(1) + ("false" if m.group(2) == "true" else "true"), "expand_div",
            )
        else:
            edit_config(ctx, r'(aspsx_version\s*=\s*"[^"]*"\n)', lambda m: m.group(1) + "expand_div = true\n", "aspsx_version line")
        _, _, u4 = ctx.run()
        if any(u4[n]["key"] == u3[n]["key"] or u4[n]["cache"] != "miss" for n in u4):
            return "a changed expand_div must change every key and miss"
        return None

    def pinned_cc1(ctx):
        p1, _, _ = ctx.run()
        if not passed(p1):
            return f"clean build did not pass; {describe(p1)}"
        old = parsed["toolchain"]["cc1"]["sha256"]
        flipped = ("0" if old[0] != "0" else "1") + old[1:]
        edit_config(ctx, re.escape(old), lambda m: flipped, "cc1 sha256")
        p2, _, u2 = ctx.run()
        if p2.returncode == 0 or "cc1 sha256 mismatch" not in p2.stdout:
            return f"a wrong pin must fail the pin check even with a warm cache; {describe(p2)}"
        if u2:
            return "no unit may be looked up after a failed pin check"
        return None

    def key_function(ctx):
        """Every key input is covered: changing any one changes the key."""
        base = {
            "format": matchbuild.CACHE_FORMAT,
            "preprocessed_sha256": "a" * 64,
            "unit": "u",
            "cc1_flags": ["-O2"],
            "cc1_sha256": "b" * 64,
            "cc1_table": "cc1",
            "maspsx_commit": "c" * 40,
            "maspsx_script": "m.py",
            "aspsx_version": "2.21",
            "as_flags": list(matchbuild.AS_FLAGS),
            "as_version": "GNU assembler 1",
            "as_sha256": "d" * 64,
            "python_version": "3.12.0 (main)",
            "python_sha256": "e" * 64,
        }
        key = matchbuild.cache_key(base)
        for field, value in base.items():
            changed = {**base, field: [*value, "x"] if isinstance(value, list) else value + "x"}
            if matchbuild.cache_key(changed) == key:
                return f"key ignores {field}"
        return None

    def assembler_identity(ctx):
        """An assembler that keeps its banner but changes its output must not hit."""
        prefix = parsed["toolchain"]["binutils_prefix"]
        real = shutil.which(prefix + "as")
        if real is None:
            raise SystemExit(f"test setup: {prefix}as not on PATH")
        ctx.wrappers.mkdir()
        wrapper = ctx.wrappers / (prefix + "as")

        def install(mutate: bool):
            wrapper.write_text(
                f"#!{sys.executable}\n"
                "import subprocess, sys\n"
                "from elftools.elf.elffile import ELFFile\n"
                f"result = subprocess.run([{real!r}, *sys.argv[1:]])\n"
                f"if result.returncode == 0 and {mutate!r} and '-o' in sys.argv:\n"
                "    out = sys.argv[sys.argv.index('-o') + 1]\n"
                "    data = bytearray(open(out, 'rb').read())\n"
                "    data[ELFFile(open(out, 'rb')).get_section_by_name('.text')['sh_offset']] ^= 0xFF\n"
                "    open(out, 'wb').write(bytes(data))\n"
                "sys.exit(result.returncode)\n"
            )
            wrapper.chmod(0o755)

        env = {**os.environ, "PATH": f"{ctx.wrappers}{os.pathsep}{os.environ['PATH']}"}
        install(False)
        p1, r1, u1 = ctx.run(env=env)
        if not passed(p1) or any(v != "miss" for v in states(u1).values()):
            return f"seed build through the faithful wrapper: {states(u1)}; {describe(p1)}"
        install(True)  # same banner, same path, different bytes
        p2, r2, u2 = ctx.run(env=env)
        if r2["tools"]["as"]["version"] != r1["tools"]["as"]["version"]:
            return "test setup: the banner changed"
        if p2.returncode == 0 or "bytes differ" not in p2.stdout:
            return f"the changed assembler must fail the byte comparison, not pass from the cache: {sorted(set(states(u2).values()))};{describe(p2)}"
        if any(v != "miss" for v in states(u2).values()) or any(u2[n]["key"] == u1[n]["key"] for n in u2):
            return f"a changed assembler executable must change every key and miss: {sorted(set(states(u2).values()))}"
        if r2["tools"]["as"].get("sha256") == r1["tools"]["as"].get("sha256"):
            return "the report does not identify the assembler by content"
        return None

    def corrupt(ctx):
        p1, _, u1 = ctx.run()
        if not passed(p1):
            return f"clean build did not pass; {describe(p1)}"
        entry = ctx.entry(u1[first]["key"])
        obj = entry / "unit.o"
        data = bytearray(obj.read_bytes())
        data[len(data) // 2] ^= 0xFF
        obj.write_bytes(bytes(data))
        p2, _, u2 = ctx.run()
        if not passed(p2) or u2[first]["cache"] != "miss":
            return f"a corrupted entry must miss and rebuild to PASS: {states(u2)}; {describe(p2)}"
        recorded = json.loads((entry / "entry.json").read_text())["object_sha256"]
        if matchbuild.file_sha(obj) != recorded or obj.read_bytes() == bytes(data):
            return "the corrupted entry was not replaced"
        _, _, u3 = ctx.run()
        if states(u3)[first] != "hit":
            return "the replaced entry does not hit"
        return None

    def missing_file(ctx):
        p1, _, u1 = ctx.run()
        if not passed(p1):
            return f"clean build did not pass; {describe(p1)}"
        entry = ctx.entry(u1[first]["key"])
        (entry / "unit.gnu.s").unlink()
        p2, _, u2 = ctx.run()
        if not passed(p2) or u2[first]["cache"] != "miss":
            return f"an entry with a missing file must miss: {states(u2)}; {describe(p2)}"
        if not (entry / "unit.gnu.s").is_file():
            return "the incomplete entry was not replaced"
        return None

    def malformed_files_field(ctx):
        """Only the files field of a valid entry becomes a list: the next build must rebuild and PASS."""
        p1, _, u1 = ctx.run()
        if not passed(p1):
            return f"clean build did not pass; {describe(p1)}"
        entry = ctx.entry(u1[first]["key"])
        record = json.loads((entry / "entry.json").read_text())
        record["files"] = list(record["files"])
        (entry / "entry.json").write_text(json.dumps(record))
        p2, _, u2 = ctx.run()
        if not passed(p2) or u2[first]["cache"] != "miss":
            return f"an entry whose files field is a list must miss and rebuild to PASS: {states(u2)}; {describe(p2)}"
        if "Traceback" in p2.stdout + p2.stderr:
            return "the build printed a traceback"
        fresh = json.loads((entry / "entry.json").read_text())
        if not isinstance(fresh["files"], dict) or not matchbuild.cache_valid(entry, u1[first]["key"]):
            return "the malformed entry was not replaced by a valid one"
        _, _, u3 = ctx.run()
        if states(u3)[first] != "hit":
            return "the replaced entry does not hit"
        return None

    def listing(cache: Path):
        return {str(p.relative_to(cache)): p.stat().st_mtime_ns for p in cache.rglob("*")}

    def no_cache(ctx):
        p1, _, _ = ctx.run()
        if not passed(p1):
            return f"seed build did not pass; {describe(p1)}"
        before = listing(ctx.cache)
        p2, _, u2 = ctx.run(("--no-cache",))
        if not passed(p2) or any(v != "off" for v in states(u2).values()):
            return f"--no-cache must report off for every unit: {states(u2)}; {describe(p2)}"
        if "cache: off" not in p2.stdout:
            return "summary line does not say the cache is off"
        if listing(ctx.cache) != before:
            return "--no-cache touched the cache directory"
        shutil.rmtree(ctx.cache)
        p3, _, _ = ctx.run(("--no-cache",))
        if not passed(p3) or ctx.cache.exists():
            return "--no-cache created the cache directory"
        return None

    def header_change(ctx):
        p1, r1, _ = ctx.run()
        if not passed(p1):
            return f"clean build did not pass; {describe(p1)}"
        fields = ctx.copy / parsed["types"]["fields"]
        text = fields.read_text()
        fields.write_text(
            text + ("" if text.endswith("\n") else "\n") + "\nstruct SelftestExtra size=0x4\n0x000 u32 selftest_extra\n"
        )
        _, r2, u2 = ctx.run()
        changed = [n for n in u2 if r2["inputs"]["preprocessed"][n] != r1["inputs"]["preprocessed"][n]]
        if not changed:
            return "the header change did not alter any preprocessed text"
        for name in u2:
            want = "miss" if name in changed else "hit"
            if u2[name]["cache"] != want:
                return f"unit {name}: expected {want}, got {u2[name]['cache']}"
        return None

    cases = [
        CacheCase("cache-reuse", reuse),
        CacheCase("cache-source-mutation", source_mutation),
        CacheCase("cache-key-inputs", key_inputs),
        CacheCase("cache-pinned-cc1", pinned_cc1),
        CacheCase("cache-key-function", key_function),
        CacheCase("cache-assembler-identity", assembler_identity),
        CacheCase("cache-corrupt-entry", corrupt),
        CacheCase("cache-missing-file", missing_file),
        CacheCase("cache-malformed-files-field", malformed_files_field),
        CacheCase("cache-off", no_cache),
    ]
    if "types" in parsed:
        cases.append(CacheCase("cache-header-change", header_change))
    return cases


# Object cache publication and lookup, driven directly in a controlled order.


def make_cache_unit_cases() -> list[CacheCase]:
    key = "k" * 64

    def sources(root: Path, tag: str) -> tuple[Path, Path, Path]:
        paths = tuple(root / f"src-{tag}{ext}" for ext in (".o", ".s", ".gnu.s"))
        for path in paths:
            path.write_bytes(f"{tag}{path.suffix}".encode() * 8)
        return paths

    def dests(root: Path) -> dict[str, Path]:
        return {name: root / f"out-{name}" for name in matchbuild.CACHE_PAYLOAD}

    def store(cache: Path, root: Path, tag: str, **kw):
        matchbuild.cache_store(cache, key, {"writer": tag}, *sources(root, tag), **kw)

    def writer_of(cache: Path) -> str:
        return json.loads((cache / key / "entry.json").read_text())["inputs"]["writer"]

    def leftovers(cache: Path) -> list[str]:
        return [p.name for p in cache.iterdir() if p.name.startswith(".tmp-")]

    def fetch_ok(root: Path):
        cache = root / "cache"
        store(cache, root, "a")
        out = dests(root)
        if not matchbuild.cache_fetch(cache / key, key, out):
            return "a valid entry must hit"
        if out["unit.o"].read_bytes() != b"a.o" * 8:
            return "the copied object differs from the stored one"
        return None

    def publish_keeps_valid(root: Path):
        """Another writer publishes a valid entry after this one missed."""
        cache = root / "cache"
        store(cache, root, "second", before_publish=lambda: store(cache, root, "first"))
        if writer_of(cache) != "first":
            return f"a valid entry published by another writer was replaced by {writer_of(cache)!r}"
        if not matchbuild.cache_valid(cache / key, key) or leftovers(cache):
            return "entry invalid or temporary directories left behind"
        return None

    def publish_replaces_invalid(root: Path):
        cache = root / "cache"
        store(cache, root, "first")
        (cache / key / "unit.o").write_bytes(b"broken")
        store(cache, root, "second")
        if writer_of(cache) != "second" or not matchbuild.cache_valid(cache / key, key):
            return "an invalid entry must be replaced by a valid one"
        if leftovers(cache) or any(p.name.endswith(".old") for p in cache.iterdir()):
            return "temporary directories left behind"
        return None

    def publish_winner_after_broken(root: Path):
        """The broken entry is replaced by another writer's valid one before publication."""
        cache = root / "cache"
        store(cache, root, "first")
        (cache / key / "unit.o").write_bytes(b"broken")

        def hook():
            shutil.rmtree(cache / key)
            store(cache, root, "winner")

        store(cache, root, "second", before_publish=hook)
        if writer_of(cache) != "winner":
            return f"the winner was replaced by {writer_of(cache)!r}"
        return None

    def read_entry_removed(root: Path):
        cache = root / "cache"
        store(cache, root, "a")
        out = dests(root)
        if matchbuild.cache_fetch(cache / key, key, out, before_copy=lambda: shutil.rmtree(cache / key)):
            return "an entry removed during the read must be a miss"
        if any(p.exists() for p in out.values()):
            return "partial files left in the build directory"
        return None

    def read_entry_swapped(root: Path):
        """The entry is replaced by another valid one after the metadata was read."""
        cache = root / "cache"
        store(cache, root, "a")
        out = dests(root)

        def swap():
            shutil.rmtree(cache / key)
            store(cache, root, "b")

        if matchbuild.cache_fetch(cache / key, key, out, before_copy=swap):
            return "a copy that does not match the metadata read earlier must be a miss"
        if any(p.exists() for p in out.values()):
            return "mismatching files left in the build directory"
        return None

    def read_tampered(root: Path):
        cache = root / "cache"
        store(cache, root, "a")
        for name in matchbuild.CACHE_PAYLOAD:
            path = cache / key / name
            good = path.read_bytes()
            path.write_bytes(good[:-1] + bytes([good[-1] ^ 1]))
            hit = matchbuild.cache_fetch(cache / key, key, dests(root))
            path.write_bytes(good)
            if hit:
                return f"a modified {name} must be a miss"
        return None

    def read_wrong_key(root: Path):
        cache = root / "cache"
        store(cache, root, "a")
        other = "z" * 64
        shutil.copytree(cache / key, cache / other)
        if matchbuild.cache_fetch(cache / other, other, dests(root)):
            return "an entry that records a different key must be a miss"
        return None

    def stored_hashes(root: Path):
        cache = root / "cache"
        store(cache, root, "a")
        record = json.loads((cache / key / "entry.json").read_text())
        for name in matchbuild.CACHE_PAYLOAD:
            if record["files"][name] != matchbuild.file_sha(cache / key / name):
                return f"recorded hash of {name} does not match the stored file"
        return None

    def good_record(cache: Path) -> dict:
        return json.loads((cache / key / "entry.json").read_text())

    def malformed_shapes(record: dict) -> list[tuple[str, object]]:
        """Metadata variants that must all be rejected. A bytes value is written as is."""
        names = list(matchbuild.CACHE_PAYLOAD)
        hashes = record["files"]
        return [
            ("files-list", {**record, "files": names}),
            ("files-string", {**record, "files": "unit.o"}),
            ("files-number", {**record, "files": 7}),
            ("files-null", {**record, "files": None}),
            ("files-missing", {k: v for k, v in record.items() if k != "files"}),
            ("files-empty", {**record, "files": {}}),
            ("files-extra-name", {**record, "files": {**hashes, "extra": "0" * 64}}),
            ("files-missing-name", {**record, "files": {n: hashes[n] for n in names[:-1]}}),
            ("hash-number", {**record, "files": {**hashes, names[0]: 5}}),
            ("hash-null", {**record, "files": {**hashes, names[0]: None}}),
            ("hash-list", {**record, "files": {**hashes, names[0]: [hashes[names[0]]]}}),
            ("hash-nested", {**record, "files": {**hashes, names[0]: {"a": {"b": [1]}}}}),
            ("hash-short", {**record, "files": {**hashes, names[0]: "ab"}}),
            ("hash-upper", {**record, "files": {**hashes, names[0]: hashes[names[0]].upper()}}),
            ("key-missing", {k: v for k, v in record.items() if k != "key"}),
            ("key-number", {**record, "key": 5}),
            ("key-list", {**record, "key": [key]}),
            ("key-null", {**record, "key": None}),
            ("top-list", [record]),
            ("top-string", "entry"),
            ("top-number", 3),
            ("top-null", None),
            ("top-true", True),
            ("empty-object", {}),
            ("invalid-json", b"{not json"),
            ("empty-file", b""),
            ("truncated", json.dumps(record).encode()[:40]),
            ("invalid-utf8", b"\xff\xfe\x80{"),
            ("deep-nesting", b"[" * 200000 + b"]" * 200000),
            ("deep-nesting-object", b'{"files":' * 100000),
        ]

    def write_entry_json(cache: Path, value) -> None:
        path = cache / key / "entry.json"
        if path.is_dir():
            shutil.rmtree(path)
        path.write_bytes(value if isinstance(value, bytes) else json.dumps(value).encode())

    def check_rejected(cache: Path, root: Path, label: str):
        """Both readers must miss without raising and leave no partial files."""
        out = dests(root)
        try:
            if matchbuild.cache_valid(cache / key, key):
                return f"{label}: validation accepted malformed metadata"
            if matchbuild.cache_fetch(cache / key, key, out):
                return f"{label}: fetch accepted malformed metadata"
        except Exception as exc:
            return f"{label}: a reader raised {type(exc).__name__}: {exc}"
        if any(p.exists() for p in out.values()):
            return f"{label}: partial files left in the build directory"
        return None

    def check_replaced(cache: Path, root: Path, label: str):
        try:
            store(cache, root, "fresh")
        except Exception as exc:
            return f"{label}: publication raised {type(exc).__name__}: {exc}"
        if not matchbuild.cache_valid(cache / key, key) or writer_of(cache) != "fresh":
            return f"{label}: the malformed entry was not replaced by a valid one"
        if not matchbuild.cache_fetch(cache / key, key, dests(root)):
            return f"{label}: the replacement entry does not hit"
        if leftovers(cache) or any(p.name.endswith(".old") for p in cache.iterdir()):
            return f"{label}: temporary directories left behind"
        return None

    def malformed_files_container(root: Path):
        """files is a list holding the expected names: the set check passes, .values() does not exist."""
        cache = root / "cache"
        store(cache, root, "a")
        write_entry_json(cache, {**good_record(cache), "files": list(matchbuild.CACHE_PAYLOAD)})
        return check_rejected(cache, root, "files as a list") or check_replaced(cache, root, "files as a list")

    def malformed_table(root: Path):
        cache = root / "cache"
        store(cache, root, "a")
        record = good_record(cache)
        for label, value in malformed_shapes(record):
            write_entry_json(cache, value)
            problem = check_rejected(cache, root, label) or check_replaced(cache, root, label)
            if problem:
                return problem
        return None

    def malformed_directory(root: Path):
        """entry.json is a directory, and the entry itself is a plain file."""
        cache = root / "cache"
        store(cache, root, "a")
        (cache / key / "entry.json").unlink()
        (cache / key / "entry.json").mkdir()
        problem = check_rejected(cache, root, "entry.json as a directory") or check_replaced(
            cache, root, "entry.json as a directory"
        )
        if problem:
            return problem
        shutil.rmtree(cache / key)
        (cache / key).write_bytes(b"not a directory")
        return check_rejected(cache, root, "entry as a file") or check_replaced(cache, root, "entry as a file")

    def malformed_during_fetch(root: Path):
        """The metadata is malformed when fetch reads it, with the copy hook installed."""
        cache = root / "cache"
        store(cache, root, "a")
        write_entry_json(cache, {**good_record(cache), "files": list(matchbuild.CACHE_PAYLOAD)})
        out = dests(root)
        try:
            hit = matchbuild.cache_fetch(cache / key, key, out, before_copy=lambda: None)
        except Exception as exc:
            return f"fetch raised {type(exc).__name__}: {exc}"
        return "fetch accepted a list as files" if hit else None

    table = (
        ("cache-unit-fetch", fetch_ok),
        ("cache-unit-publish-keeps-valid", publish_keeps_valid),
        ("cache-unit-publish-replaces-invalid", publish_replaces_invalid),
        ("cache-unit-publish-winner-after-broken", publish_winner_after_broken),
        ("cache-unit-read-entry-removed", read_entry_removed),
        ("cache-unit-read-entry-swapped", read_entry_swapped),
        ("cache-unit-read-tampered", read_tampered),
        ("cache-unit-read-wrong-key", read_wrong_key),
        ("cache-unit-stored-hashes", stored_hashes),
        ("cache-unit-malformed-files-container", malformed_files_container),
        ("cache-unit-malformed-table", malformed_table),
        ("cache-unit-malformed-directory", malformed_directory),
        ("cache-unit-malformed-during-fetch", malformed_during_fetch),
    )
    return [CacheCase(name, body) for name, body in table]


def make_rodata_unit_cases(parsed: dict) -> list[CacheCase]:
    """Read-only data ownership without a build: ranges, controls and object checks."""
    UnitDecl, FunctionDecl, RodataDecl = matchbuild.UnitDecl, matchbuild.FunctionDecl, matchbuild.RodataDecl
    load = 0x1000

    def unit(name: str, text: int, size: int, rodata=None):
        return UnitDecl(name, f"{name}.c", (), (FunctionDecl(f"{name}_fn", text, size),), RodataDecl(*rodata) if rodata else None)

    units = [
        unit("a", load + 0x10, 0x20, (load + 0x200, 0x18)),
        unit("b", load + 0x40, 0x10),
        unit("c", load + 0x60, 0x10, (load + 0x100, 0x8)),
    ]

    def raw(root: Path):
        got = matchbuild.raw_ranges(load, 0x300, units)
        want = [(load, 0x10), (load + 0x30, 0x10), (load + 0x50, 0x10), (load + 0x70, 0x90), (load + 0x108, 0xF8), (load + 0x218, 0xE8)]
        return None if got == want else f"raw ranges {got} != {want}"

    def controls(root: Path):
        payload = bytes((i * 7 + 3) & 0xFF for i in range(0x300))
        image = payload
        records = matchbuild.run_controls(image, payload, load, units)
        kinds = [c["kind"] for c in records]
        if kinds.count("rodata") != 2 or kinds.count("function") != 3 or kinds.count("raw") != 1:
            return f"unexpected controls: {kinds}"
        if not all(c["tripped"] for c in records if c["applicable"]):
            return f"a control did not trip: {records}"
        # One flipped byte anywhere in a rodata range is flagged for exactly that unit.
        for u in units:
            if u.rodata is None:
                continue
            for off in (0, u.rodata.size - 1):
                result = matchbuild.compare_image(
                    matchbuild.flip_byte(image, u.rodata.address - load + off), payload, load, units
                )
                failed = [r.unit for r in result.rodata if not r.exact]
                if failed != [u.name] or not result.all_functions_exact or result.image_exact:
                    return f"flip at {off} in {u.name} rodata flagged {failed}"
                bad = next(r for r in result.rodata if r.unit == u.name)
                if bad.first_diff != off:
                    return f"first_diff {bad.first_diff}, want {off}"
        # A byte just outside the range is raw, not rodata.
        result = matchbuild.compare_image(matchbuild.flip_byte(image, 0x200 + 0x18), payload, load, units)
        if not result.all_rodata_exact or result.image_exact:
            return "a raw byte next to a rodata range was attributed to the rodata"
        return None

    def object_checks(root: Path):
        prefix = parsed["toolchain"]["binutils_prefix"]
        assembler = shutil.which(prefix + "as")
        if assembler is None:
            raise SystemExit(f"test setup: {prefix}as not on PATH")

        def build(name: str, body: str) -> Path:
            source, obj = root / f"{name}.s", root / f"{name}.o"
            source.write_text(body)
            subprocess.run([assembler, *matchbuild.AS_FLAGS, "-o", str(obj), str(source)], check=True)
            return obj

        text = ".text\n.word 0\n.word 0\n"
        only_text = build("t", text)
        both = build("both", text + '.section .rodata,"a"\n.word 1\n.word 2\n' + '.section .rdata,"a"\n.word 3\n')
        rdata = build("rdata", text + '.section .rdata,"a"\n.word 3\n.word 4\n')
        data = build("data", text + '.section .rodata,"a"\n.word 1\n.sdata\n.word 9\n')
        plain = unit("u", load, 8)
        with_ro = lambda n: unit("u", load, 8, (load + 0x100, n))
        checks = [
            ("no table, none declared", only_text, plain, []),
            ("no table, one declared", only_text, with_ro(8), ["declares rodata"]),
            ("table, none declared", both, plain, ["rodata must be declared"]),
            ("rodata and rdata add up", both, with_ro(12), []),
            ("rodata and rdata wrong total", both, with_ro(8), ["rodata size mismatch"]),
            ("rdata alone", rdata, with_ro(8), []),
            ("sdata is still rejected", data, with_ro(4), ["only .text, read-only data, data and bss"]),
        ]
        for label, obj, decl, wanted in checks:
            errors = matchbuild.check_unit_object(obj, decl)
            text_errors = " | ".join(errors)
            if len(errors) != len(wanted) or any(w not in text_errors for w in wanted):
                return f"{label}: got {errors}, wanted {wanted}"
        return None

    table = (
        ("rodata-unit-raw-ranges", raw),
        ("rodata-unit-controls", controls),
        ("rodata-unit-object-checks", object_checks),
    )
    return [CacheCase(name, body) for name, body in table]


def make_symbol_unit_cases(parsed: dict) -> list[CacheCase]:
    """The check of the linked ELF for the symbols a unit defines, on an authored object."""
    UnitDecl, FunctionDecl, RodataDecl = matchbuild.UnitDecl, matchbuild.FunctionDecl, matchbuild.RodataDecl
    text, bss, rodata, data = 0x1000, 0x2000, 0x3000, 0x4000
    # Each kind holds one word before its symbol, and the second section of a kind follows the first.
    expected = {"pick": text, "counter": bss + 4, "small": bss + 12, "ro_second": rodata + 4, "rd_second": rodata + 12, "data_second": data + 4}

    def binding_check(root: Path):
        prefix = parsed["toolchain"]["binutils_prefix"]
        assembler, linker = shutil.which(prefix + "as"), shutil.which(prefix + "ld")
        if assembler is None or linker is None:
            raise SystemExit(f"test setup: {prefix}as and {prefix}ld must be on PATH")
        (root / "pick.s").write_text(
            ".set noreorder\n.text\n.globl pick\n.type pick, @function\npick:\n    jr $ra\n    nop\n.size pick, . - pick\n"
            ".section .rodata\n.word 1\n.globl ro_second\nro_second:\n.word 2\n"
            '.section .rdata,"a",@progbits\n.word 3\n.globl rd_second\nrd_second:\n.word 4\n'
            ".data\n.word 5\n.globl data_second\ndata_second:\n.word 6\n"
            ".bss\n.space 4\n.globl counter\ncounter:\n.space 4\n"
            '.section .sbss,"aw",@nobits\n.space 4\n.globl small\nsmall:\n.space 4\n'
        )
        subprocess.run([assembler, *matchbuild.AS_FLAGS, "-o", str(root / "unit-pick.o"), str(root / "pick.s")], check=True)
        unit = UnitDecl(
            "pick", "pick.s", (), (FunctionDecl("pick", text, 8),),
            rodata=RodataDecl(rodata, 16), data=RodataDecl(data, 8), bss=RodataDecl(bss, 16), kind="asm",
        )
        defined = {"pick": matchbuild.defined_symbols(root / "unit-pick.o")}
        placed = {sym.name: matchbuild.symbol_address(unit, sym) for sym in defined["pick"]}
        if placed != expected:
            return f"symbols are placed at {placed}, want {expected}"
        links = {}
        for label, assignment in (("clean", ""), ("overridden", f"counter = {bss + 4:#x};\n")):
            (root / f"{label}.ld").write_text(
                "OUTPUT_ARCH(mips)\n"
                + assignment
                + "SECTIONS {\n"
                f" .text.pick {text:#x} : SUBALIGN(1) {{ unit-pick.o(.text) }}\n"
                f" .bss.pick {bss:#x} (NOLOAD) : SUBALIGN(1) {{ unit-pick.o(.bss) unit-pick.o(.sbss) }}\n"
                f" .rodata.pick {rodata:#x} : SUBALIGN(1) {{ unit-pick.o(.rodata) unit-pick.o(.rdata) }}\n"
                f" .data.pick {data:#x} : SUBALIGN(1) {{ unit-pick.o(.data) }}\n"
                " /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n"
                "}\n"
            )
            proc = subprocess.run(
                [linker, "-EL", "-T", f"{label}.ld", "-e", f"{text:#x}", "-o", f"{label}.elf", "unit-pick.o"],
                cwd=root, capture_output=True, text=True,
            )
            if proc.returncode != 0:
                return f"{label}: the authored link failed:\n{proc.stderr}"
            links[label] = matchbuild.elf_symbol_checks(root / f"{label}.elf", [unit], defined)
        if links["clean"]:
            return f"a correctly placed symbol was rejected: {links['clean']}"
        if len(links["overridden"]) != 1 or "'counter' is bound to" not in links["overridden"][0]:
            return f"an overridden symbol must be rejected once, naming it: {links['overridden']}"
        return None

    return [CacheCase("symbol-binding-check", binding_check)]


def run_cache_unit_case(case: CacheCase) -> tuple[bool, str]:
    with tempfile.TemporaryDirectory(prefix="mb-cache-unit-") as tmp:
        try:
            problem = case.body(Path(tmp))
        except Exception as exc:  # an unfixed or broken cache must fail the case, not the suite
            problem = f"raised {type(exc).__name__}: {exc}"
    return (False, problem) if problem else (True, "behaves as required")


def run_cache_case(case: CacheCase, cfg_dir: Path, parsed: dict) -> tuple[bool, str]:
    ctx = CacheContext(case.name, cfg_dir, parsed)
    try:
        ctx.cleanup()
        shutil.copytree(cfg_dir, ctx.copy)
        problem = case.body(ctx)
        return (False, problem) if problem else (True, "behaves as required")
    finally:
        ctx.cleanup()


def run_ordered(jobs: int, tasks: list):
    """Run `tasks` (callables returning (ok, message)) on up to `jobs` threads.

    Yields the results in the order of `tasks`, each as soon as it and all
    earlier ones are done, whatever order they finish in. A task that raises
    is a failed case, not a broken run: each case has its own copy, build tag
    and cache directory, and one of them must not take the others down. That
    includes `SystemExit`, which the setup helpers raise ("test setup: ...")
    and which `except Exception` does not catch. An interrupt is not a result:
    `KeyboardInterrupt` still ends the run. What cases do share, a fixture's
    seed builds, is serialized by `SeededFixture`.
    """

    def guarded(task):
        try:
            return task()
        except SystemExit as exc:
            return False, f"stopped with SystemExit: {exc.code}"
        except Exception as exc:
            return False, f"raised {type(exc).__name__}: {exc}"

    if jobs <= 1:
        for task in tasks:
            yield guarded(task)
        return
    with ThreadPoolExecutor(max_workers=jobs) as pool:
        yield from pool.map(guarded, tasks)


def select_cases(cases: list, only: str) -> list:
    """The cases whose name contains `only`. An empty `only` selects all."""
    return [case for case in cases if only in case.name]


def make_runner_unit_cases(config_path: Path) -> list[CacheCase]:
    """The test runner's own helpers: case selection and the include list edit."""

    class Named:
        def __init__(self, name):
            self.name = name

    def selection(root: Path):
        cases = [Named("alpha-one"), Named("alpha-two"), Named("beta")]
        picks = {only: [c.name for c in select_cases(cases, only)] for only in ("", "alpha", "beta", "gamma")}
        want = {"": ["alpha-one", "alpha-two", "beta"], "alpha": ["alpha-one", "alpha-two"], "beta": ["beta"], "gamma": []}
        return None if picks == want else f"selection is wrong: {picks}"

    def run_self(only: str) -> subprocess.CompletedProcess:
        return subprocess.run(
            [sys.executable, str(Path(__file__).resolve()), "--config", str(config_path), "--only", only],
            capture_output=True, text=True,
        )

    def empty_selection(root: Path):
        proc = run_self("no-case-has-this-name")
        if proc.returncode == 0:
            return "an --only text that selects nothing must not exit 0"
        if "no case matches" not in proc.stdout + proc.stderr or "all cases behaved as required" in proc.stdout:
            return f"an empty selection must say so and must not report success:\n{proc.stdout}{proc.stderr}"
        return None

    def nonempty_selection(root: Path):
        proc = run_self("runner-selection-filter")
        ran = [line for line in proc.stdout.splitlines() if line.startswith(("ok  ", "FAIL"))]
        if proc.returncode != 0 or ran != ["ok   runner-selection-filter: behaves as required"]:
            return f"a matching --only text must run exactly the matching case:\n{proc.stdout}{proc.stderr}"
        if "not the full control set" not in proc.stdout:
            return "a filtered run must say that it is not the full control set"
        return None

    def include_list(root: Path):
        head = '[baseline]\nexecutable = "x"\n\n[toolchain]\n'
        tail = 'cpp = "clang"\n\n[toolchain.cc1]\nkind = "local"\n'
        variants = {
            "absent": ("", []),
            "empty": ("include_dirs = []\n", []),
            "empty-spaced": ("include_dirs = [ ]\n", []),
            "one": ('include_dirs = ["a"]\n', ["a"]),
            "two-trailing-comma": ('include_dirs = ["a", "b/c",]\n', ["a", "b/c"]),
            "multi-line": ('include_dirs = [\n  "a",\n  "b",\n]\n', ["a", "b"]),
        }
        for label, (line, before) in variants.items():
            copy = root / label
            copy.mkdir()
            (copy / "build.toml").write_text(head + line + tail)
            add_include_dir(copy, "selftest_inc")
            try:
                parsed = tomllib.loads((copy / "build.toml").read_text())
            except tomllib.TOMLDecodeError as exc:
                return f"{label}: the edited file is not valid TOML: {exc}"
            if parsed["toolchain"].get("include_dirs") != [*before, "selftest_inc"]:
                return f"{label}: include_dirs is {parsed['toolchain'].get('include_dirs')}"
            if parsed["toolchain"].get("cpp") != "clang" or parsed["toolchain"]["cc1"] != {"kind": "local"}:
                return f"{label}: the rest of [toolchain] changed"
        return None

    def ordered(root: Path):
        gate = threading.Event()

        def slow_first():
            gate.wait(5)
            return True, "first"

        def fast_second():
            gate.set()
            return True, "second"

        def raising():
            raise RuntimeError("boom")

        for jobs in (1, 4):
            gate.clear()
            if jobs == 1:
                gate.set()
            started = time.monotonic()
            got = list(run_ordered(jobs, [slow_first, fast_second, raising]))
            if [g[0] for g in got] != [True, True, False] or [got[0][1], got[1][1]] != ["first", "second"]:
                return f"jobs={jobs}: results are not in task order or lost a result: {got}"
            if "RuntimeError: boom" not in got[2][1]:
                return f"jobs={jobs}: a raising task must become a failed case that names the error: {got[2]}"
            if time.monotonic() - started > 4:
                return f"jobs={jobs}: the second task did not run while the first was waiting"
        return None

    def setup_exit(root: Path):
        def setup_fails():
            # A real setup helper: it stops with SystemExit when its text is not there once.
            replace_once("no such text", "absent", "x", "the probe text")
            return True, "not reached"

        def exits_with_status():
            raise SystemExit(3)

        def interrupted():
            raise KeyboardInterrupt

        def succeeds():
            return True, "second"

        for jobs in (1, 4):
            for failing, wanted in ((setup_fails, "test setup: expected exactly one occurrence of the probe text"), (exits_with_status, "3")):
                try:
                    got = list(run_ordered(jobs, [failing, succeeds]))
                except SystemExit as exc:
                    return f"jobs={jobs}, {failing.__name__}: SystemExit left the runner ({exc.code}) and the next task has no result"
                if len(got) != 2 or got[0][0] is not False or wanted not in got[0][1]:
                    return f"jobs={jobs}, {failing.__name__}: the exit must be a failed case that names its reason: {got}"
                if got[1] != (True, "second"):
                    return f"jobs={jobs}, {failing.__name__}: the task after the exit must still run and report: {got}"
            try:
                got = list(run_ordered(jobs, [interrupted, succeeds]))
            except KeyboardInterrupt:
                continue
            return f"jobs={jobs}: an interrupt must end the run, not become a case result: {got}"
        return None

    def seeds_one_at_a_time(root: Path):
        def peak_of(cls, enter) -> int:
            state = {"inside": 0, "peak": 0}
            count = threading.Lock()

            class Probe(cls):
                def _prepare(self):
                    with count:
                        state["inside"] += 1
                        state["peak"] = max(state["peak"], state["inside"])
                    time.sleep(0.05)
                    with count:
                        state["inside"] -= 1

            # Separate objects, as the case lists create them: they share only the label.
            probes = [Probe.__new__(Probe) for _ in range(4)]
            with ThreadPoolExecutor(max_workers=4) as pool:
                list(pool.map(enter, probes))
            return state["peak"]

        def below(cls) -> list:
            return [sub for child in cls.__subclasses__() for sub in (child, *below(child))]

        # Found, not listed: a fixture class added later is checked too. The
        # probe classes of an earlier call are local classes and do not count.
        families = [cls for cls in below(SeededFixture) if "<locals>" not in cls.__qualname__]
        seeders = [v for v in globals().values() if isinstance(v, type) and "_seed" in vars(v)]
        unlocked = [cls.__name__ for cls in seeders if not issubclass(cls, SeededFixture)]
        if unlocked or not seeders:
            return f"classes that run seed builds must derive from SeededFixture: {unlocked or 'none found'}"
        labels = [cls.label for cls in families]
        if "" in labels or len(set(labels)) != len(labels):
            return f"every fixture class needs its own seed label: {labels}"
        for cls in families:
            if "prepare" in vars(cls):
                return f"{cls.__name__} overrides prepare and so seeds without the label's lock"
            peak = peak_of(cls, lambda probe: probe.prepare())
            if peak != 1:
                return f"{cls.__name__}: {peak} callers seeded the label at the same time"
        # The probe must be able to see an overlap: without the lock all four are inside together.
        if peak_of(RodataFixture, lambda probe: probe._prepare()) < 2:
            return "the probe saw no overlap where nothing prevents one"
        return None

    return [
        CacheCase("runner-parallel-order", ordered),
        CacheCase("runner-setup-exit-is-failed-case", setup_exit),
        CacheCase("runner-fixture-seeds-serialized", seeds_one_at_a_time),
        CacheCase("runner-selection-filter", selection),
        CacheCase("runner-selection-empty-fails", empty_selection),
        CacheCase("runner-selection-nonempty-runs", nonempty_selection),
        CacheCase("runner-include-list-edit", include_list),
    ]


def main() -> int:
    parser = argparse.ArgumentParser(description="Controls for matchbuild.py")
    parser.add_argument("--config", type=Path, default=ROOT / "ps1/src/build.toml")
    parser.add_argument("--only", default="", help="run only the cases whose name contains this text")
    parser.add_argument(
        "--jobs", type=int, default=min(8, os.cpu_count() or 1),
        help="cases to run at the same time (default: up to 8); 1 runs them one after another",
    )
    args = parser.parse_args()
    if args.jobs < 1:
        print("--jobs must be at least 1", file=sys.stderr)
        return 2
    config_path = Path(os.path.abspath(args.config))
    cfg_dir = config_path.parent
    if config_path.name != "build.toml":
        print("the configuration file must be named build.toml", file=sys.stderr)
        return 2
    parsed = tomllib.loads(config_path.read_text())
    if "selftest" not in parsed:
        print("configuration has no [selftest] section", file=sys.stderr)
        return 2

    builds = select_cases(make_cases(cfg_dir, parsed), args.only)
    caches = select_cases(make_cache_cases(parsed), args.only)
    units = select_cases(
        make_cache_unit_cases() + make_rodata_unit_cases(parsed) + make_symbol_unit_cases(parsed) + make_runner_unit_cases(config_path), args.only
    )
    if not builds and not caches and not units:
        print(f"no case matches --only {args.only!r}: nothing ran", file=sys.stderr)
        return 2

    # Every case works on its own copy, build tag and cache directory, named
    # after the case, so the cases can run side by side. The seed builds that
    # the cases of one fixture share are serialized (`SeededFixture`). The
    # cases are reported in their fixed order.
    ordered_cases = [*builds, *caches, *units]
    tasks = (
        [lambda case=case: run_case(case, cfg_dir) for case in builds]
        + [lambda case=case: run_cache_case(case, cfg_dir, parsed) for case in caches]
        + [lambda case=case: run_cache_unit_case(case) for case in units]
    )
    failed = 0
    for case, (ok, message) in zip(ordered_cases, run_ordered(args.jobs, tasks)):
        print(f"{'ok  ' if ok else 'FAIL'} {case.name}: {message}", flush=True)
        failed += not ok
    ran = len(ordered_cases)
    if args.only:
        print(f"note: only the {ran} case(s) matching {args.only!r} ran; this is not the full control set")
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
