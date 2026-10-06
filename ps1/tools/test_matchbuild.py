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
import struct
import subprocess
import sys
import tempfile
import threading
import time
import tomllib
from pathlib import Path

import fndiff
import matchbuild
import structgen
from test_disc_tools import make_archive, make_executable

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
    def __init__(self, name, expect_pass, reason, mutate=None, verify=None, fndiff=None, status=None, extra=()):
        self.name = name
        self.expect_pass = expect_pass
        self.reason = reason  # substring required in the tool output when it must fail
        self.mutate = mutate
        self.verify = verify  # optional check of report.json, returns an error string or None
        self.fndiff = fndiff  # optional (unit, expected exit status, required text) for fndiff.py
        self.status = status  # optional exit status that a failing run must have
        self.extra = extra  # extra command line arguments for the tool


def strip_images(text: str) -> str:
    """A configuration text without `[overlays]`, without `[[image]]` tables and without the units of module images.

    The text is cut into tables at the lines that open one. The comment lines
    directly above a table belong to it.
    """
    blocks, current = [], []
    for line in text.splitlines(keepends=True):
        if line.startswith("["):
            lead = []
            while current and current[-1].startswith("#"):
                lead.insert(0, current.pop())
            blocks.append(current)
            current = lead
        current.append(line)
    blocks.append(current)

    def kept(block: list[str]) -> bool:
        header = next((line.strip() for line in block if line.startswith("[")), "")
        if header in ("[overlays]", "[[image]]"):
            return False
        return not (header == "[[unit]]" and any(re.match(r"image\s*=", line) for line in block))

    return "".join(line for block in blocks if kept(block) for line in block)


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


def fixture_executable(payload: bytes, load: int = FIXTURE_LOAD) -> bytes:
    header = bytearray(HEADER_SIZE)
    header[:8] = b"PS-X EXE"
    header[0x18:0x1C] = load.to_bytes(4, "little")
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

    def without_images(copy: Path):
        """The configuration as it is, less its module images and their units."""
        path = copy / "build.toml"
        path.write_text(strip_images(path.read_text()))

    def verify_no_images(report: dict):
        keys = [k for k in ("images", "selected_image") if k in report] + (["inputs.images"] if "images" in report["inputs"] else [])
        if keys:
            return f"a configuration without images reports {keys}"
        out = cfg_dir.parent / "build" / report["tag"]
        if (out / "others.ld").exists() or "others" in (out / "link.ld").read_text():
            return "a configuration without images must have no others.ld and a linker script that does not mention it"
        return None

    mutation_reason = (
        f"function '{target_unit['functions'][0]['name']}': bytes differ"
        if selftest["unit"] == unit["name"]
        else "bytes differ"
    )
    return float_cases + asm_cases + types_cases + include_cases + make_rodata_cases(cfg_dir, parsed) + make_padded_cases(cfg_dir, parsed) + make_data_cases(cfg_dir, parsed) + make_asm_cases(cfg_dir, parsed) + make_bss_cases(cfg_dir, parsed) + make_symbol_cases(cfg_dir, parsed) + make_image_cases(cfg_dir, parsed) + make_second_cases(cfg_dir, parsed) + make_second_build_cases(cfg_dir, parsed) + make_moved_symbol_cases(cfg_dir, parsed) + make_sibling_cases(cfg_dir, parsed) + make_division_cases(cfg_dir, parsed) + [
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
        Case("images-absent-unchanged", True, "", without_images, verify_no_images),
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


# Synthetic module images: table 0 of the loader holds four addresses, the block of table
# addresses follows a gap in the resident payload, and an archive holds a few chunks.
IMAGE_POINTERS = FIXTURE_LOAD + 0x100
IMAGE_TABLE0 = [0x80700000, FIXTURE_LOAD, 0x80800000, 0x80900000]
IMAGE_CHUNK = bytes(range(1, 17))
IMAGE_SIZE = len(IMAGE_CHUNK)
IMAGE_BODY = "int {name}_fn(void) {{ return {value}; }}\n"
SECOND_SLOT, SECOND_ADDRESS = 2, 0x80800000  # the second link: its slot and its entry in table 0
MOVED_READER_SIZE = 24  # a function that reads two variables of symbols.ld
MOVED_TOTAL = 0x60  # the payload of the images of the moved-names fixture: the reader, then raw bytes
MOVED_OFFSET = 0x40  # the variable inside the payload that no unit owns
MOVED_OUTSIDE = 0x80400000  # a name of symbols.ld outside both payloads
MOVED_ELSEWHERE = 0x80500000  # the address that the table of the second link gives the inside name
TWO_CALLER_SIZE = 48  # the caller of the second-link fixture: it calls a function and reads a variable
CALLER_SIZE = 32  # a function that calls another: frame, jal and its delay slot, restore, return


class ModuleSeeds(SeededFixture):
    """Chunks that the toolchain itself produces, so that a module image can be exact.

    Each chunk is the image of a seed build over a payload of zeros: a
    function that returns a number, at an offset in a payload of a given size.
    "A" is the function alone, "B" the same with another number, and "C" the
    function with four raw bytes before it and three after it.
    """

    label = "module-image"
    SEEDS = {"A": (7, 0, 8), "B": (9, 0, 8), "C": (7, 4, 15)}
    # "D" is two units, the second of which calls the function of the first.
    CALLS = [
        ("callee", IMAGE_BODY.format(name="callee", value=7), 0, 8),
        ("caller", "int callee_fn(void);\nint caller_fn(void) { return callee_fn(); }\n", 8, CALLER_SIZE),
    ]
    CALLS_SIZE = 8 + CALLER_SIZE
    # "R" and "M" call each other across images: the resident image has a function
    # of 8 bytes and then "res", which calls the module's "mod"; the module image is
    # "mod", which calls "res". In a seed build the other image's function is an assigned address.
    CROSS_RES = [
        ("pad", IMAGE_BODY.format(name="pad", value=7), 0, 8),
        ("res", "int mod_fn(void);\nint res_fn(void) { return mod_fn(); }\n", 8, CALLER_SIZE),
    ]
    CROSS_RES_SIZE = 8 + CALLER_SIZE
    CROSS_MOD = [("mod", "int res_fn(void);\nint mod_fn(void) { return res_fn(); }\n", 0, CALLER_SIZE)]
    # "S1" and "S2" are one function that calls `ext_fn`, a name of symbols.ld, in two seed builds
    # in which that name has the addresses `SYM_ADDRESSES`.
    SYM = [("sym", "int ext_fn(void);\nint sym_fn(void) { return ext_fn(); }\n", 0, CALLER_SIZE)]
    SYM_ADDRESSES = (0x80310000, 0x80320000)
    # "F" is the first image of a second link: a callee, the owner of a variable in declared data, a
    # caller that calls the callee and reads the variable, and a unit that nobody uses. "G" is the
    # same sources built at the second address.
    TWO_SIDES = [
        ("callee", IMAGE_BODY.format(name="callee", value=7), 0, 8),
        ("owner", "int shared_var = 5;\nint owner_fn(void) { return 1; }\n", 8, 8,
         lambda load: f"data = {{ address = {load + 16:#x}, size = 4 }}\n"),
        ("caller", "int callee_fn(void);\nextern int shared_var;\nint caller_fn(void) { return callee_fn() + shared_var; }\n",
         20, TWO_CALLER_SIZE),
        ("lone", IMAGE_BODY.format(name="lone", value=3), 20 + TWO_CALLER_SIZE, 8),
    ]
    TWO_SIZE = 28 + TWO_CALLER_SIZE
    # The unit "reader" reads `inside_var`, which symbols.ld places inside the payload of the first image where no
    # unit owns the bytes, and `outside_var`, which it places outside. "H" is the first image, "I" the second link
    # at SECOND_ADDRESS with the inside name moved; "J" reads it at the first address, "K" at MOVED_ELSEWHERE.
    READER = [("reader", "extern int inside_var;\nextern int outside_var;\n"
               "int reader_fn(void) { return inside_var + outside_var; }\n", 0, MOVED_READER_SIZE)]

    @staticmethod
    def reader_symbols(inside: int) -> str:
        return f"inside_var = {inside:#x};\noutside_var = {MOVED_OUTSIDE:#x};\n"

    def __init__(self, cfg_dir: Path, parsed: dict):
        self.cfg_dir = cfg_dir
        self.flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])
        self.toolchain = fixture_toolchain(parsed)
        self.chunks: dict[str, bytes] = {}

    def _seed(self, key: str, value: int, offset: int, total: int) -> bytes:
        return self._seed_units(key, [("value", IMAGE_BODY.format(name="value", value=value), offset, 8)], total)

    def _seed_units(
        self, key: str, units: list[tuple], total: int, symbols: str = "", load: int = FIXTURE_LOAD
    ) -> bytes:
        """The image of a seed build of units (name, source, offset, size), each with its function `<name>_fn`.

        `symbols` is the text of the seed's symbols.ld, which assigns the addresses of functions of other images.
        A unit may have a fifth item, more keys of its table, with `{load}` for the load address.
        `load` is where the seed image sits: a second link is seeded at its own address.
        """
        name = f"{self.label}-{key}"
        copy = self.cfg_dir.with_name(f"{self.cfg_dir.name}.selftest-{name}")
        build = self.cfg_dir.parent / "build" / f"selftest-{name}"
        cache = self.cfg_dir.parent / "build" / f".selftest-cache-{name}"
        for leftover in (copy, build, cache):
            if leftover.exists():
                shutil.rmtree(leftover)
        copy.mkdir()
        try:
            text = ""
            for unit_name, source, offset, size, *more in units:
                (copy / f"{unit_name}.c").write_text(source)
                text += (
                    "[[unit]]\n"
                    f'name = "{unit_name}"\n'
                    f'source = "{unit_name}.c"\n'
                    f"flags = [{self.flags}]\n"
                    f'functions = [ {{ name = "{unit_name}_fn", address = {load + offset:#x}, size = {size} }} ]\n'
                    + "".join(extra(load) for extra in more)
                    + "\n"
                )
            (copy / "symbols.ld").write_text(symbols or "/* The fixture needs no external symbols. */\n")
            executable = fixture_executable(bytes(total), load)
            (copy / "baseline.bin").write_bytes(executable)
            (copy / "build.toml").write_text(
                "[baseline]\n"
                'executable = "baseline.bin"\n'
                f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
                + toml_table("toolchain", self.toolchain)
                + text
            )
            proc = run_tool(copy / "build.toml", f"selftest-{name}", cache)
            image = build / "image.bin"
            if not image.is_file() or image.stat().st_size != total:
                raise SystemExit(f"test setup: the module seed build produced no image:\n{proc.stdout}{proc.stderr}")
            return image.read_bytes()
        finally:
            for leftover in (copy, build, cache):
                if leftover.exists():
                    shutil.rmtree(leftover, ignore_errors=True)

    def _prepare(self) -> None:
        if self.chunks:
            return
        self.chunks = {key: self._seed(key, *seed) for key, seed in self.SEEDS.items()}
        self.chunks["D"] = self._seed_units("D", self.CALLS, self.CALLS_SIZE)
        self.chunks["R"] = self._seed_units(
            "R", self.CROSS_RES, self.CROSS_RES_SIZE, f"mod_fn = {FIXTURE_LOAD:#x};\n"
        )
        self.chunks["M"] = self._seed_units(
            "M", self.CROSS_MOD, CALLER_SIZE, f"res_fn = {FIXTURE_LOAD + 8:#x};\n"
        )
        self.chunks["F"] = self._seed_units("F", self.TWO_SIDES, self.TWO_SIZE)
        self.chunks["G"] = self._seed_units("G", self.TWO_SIDES, self.TWO_SIZE, load=SECOND_ADDRESS)
        for key, load, inside in (
            ("H", FIXTURE_LOAD, FIXTURE_LOAD + MOVED_OFFSET),
            ("I", SECOND_ADDRESS, SECOND_ADDRESS + MOVED_OFFSET),
            ("J", SECOND_ADDRESS, FIXTURE_LOAD + MOVED_OFFSET),
            ("K", SECOND_ADDRESS, MOVED_ELSEWHERE),
        ):
            self.chunks[key] = self._seed_units(key, self.READER, MOVED_TOTAL, self.reader_symbols(inside), load)
        for key, address in zip(("S1", "S2"), self.SYM_ADDRESSES):
            self.chunks[key] = self._seed_units(key, self.SYM, CALLER_SIZE, f"ext_fn = {address:#x};\n")


class ImageFixture:
    """A resident executable with the loader's tables, archives and module units.

    The resident unit and the module unit cover the same addresses. The
    controls that stop at the configuration compile nothing; the others use the
    chunks of `ModuleSeeds` and the resident bytes `code` at the load address.
    """

    def __init__(self, parsed: dict):
        self.toolchain = fixture_toolchain(parsed)
        self.flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])

    @staticmethod
    def unit(name: str, address: int, size: int = IMAGE_SIZE, *, image: str | None = "example", extra: str = "") -> str:
        where = "" if image is None else f'image = "{image}"\n'
        return (
            "[[unit]]\n"
            f'name = "{name}"\n'
            f"{where}"
            f'source = "{name}.c"\n'
            f'functions = [ {{ name = "{name}_fn", address = {address:#x}, size = {size} }} ]\n'
            f"{extra}\n"
        )

    @staticmethod
    def image(chunk: bytes = IMAGE_CHUNK, **overrides) -> dict:
        return {"name": "example", "archive": "EXAMPLE.PAC", "slot": 1, "sha256": hashlib.sha256(chunk).hexdigest(),
                "address": FIXTURE_LOAD, **overrides}

    @staticmethod
    def executable(code: bytes) -> bytes:
        """The resident executable: `code` at the load address, then the tables and the block of their addresses."""
        data = bytearray(make_executable(FIXTURE_LOAD + len(code), IMAGE_POINTERS, [IMAGE_TABLE0, [0x80A00000, 0]]))
        struct.pack_into("<II", data, 0x18, FIXTURE_LOAD, len(data) - 0x800 + len(code))
        return bytes(data[:0x800]) + code + bytes(data[0x800:])

    def install(self, copy: Path, *, overlays: bool = True, pointers: int = IMAGE_POINTERS, images=None,
                archive=None, archives=None, chunk: bytes = IMAGE_CHUNK, units=None, code: bytes = b"",
                sources=None, head: str = "", symbols: str = "") -> None:
        """Replace the copy with the fixture.

        `archive` None writes the default archive, False writes none.
        `archives` maps further file names to their bytes, `sources` maps unit
        names to the text of their source, and `code` is the resident prefix.
        `head` is text for the start of the configuration, before any table.
        `symbols` is the text of symbols.ld. A table in an image's mapping, such as
        `symbols`, is written as `[image.<key>]` after the image's own keys.
        """
        for child in copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        executable = self.executable(code)
        (copy / "baseline.bin").write_bytes(executable)
        (copy / "symbols.ld").write_text(symbols or "/* The fixture needs no external symbols. */\n")
        if archive is None:
            archive = bytes(make_archive([(1, chunk), ((1 << 16) | 2, bytes(8))]))
        if archive is not False:
            (copy / "EXAMPLE.PAC").write_bytes(archive)
        for name, data in (archives or {}).items():
            (copy / name).write_bytes(data)
        text = (
            head
            + "[baseline]\n"
            'executable = "baseline.bin"\n'
            f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
            + toml_table("toolchain", self.toolchain)
        )
        if overlays:
            text += f"[overlays]\ntable_pointers = {pointers:#x}\n\n"
        for image in [self.image(chunk)] if images is None else images:
            scalars = {k: v for k, v in image.items() if not isinstance(v, dict)}
            text += "[[image]]\n" + "".join(
                f"{k} = {json.dumps(v)}\n" if isinstance(v, (str, list)) else f"{k} = {v:#x}\n" for k, v in scalars.items()
            ) + "\n"
            for key, table in ((k, v) for k, v in image.items() if isinstance(v, dict)):
                text += f"[image.{key}]\n" + "".join(
                    f"{k} = {json.dumps(v)}\n" if isinstance(v, str) else f"{k} = {v:#x}\n" for k, v in table.items()
                ) + "\n"
        units = [self.unit("res", FIXTURE_LOAD, image=None), self.unit("mod", FIXTURE_LOAD)] if units is None else units
        for unit in units:
            name = re.search(r'name = "(\w+)"', unit).group(1)
            body = (sources or {}).get(name, IMAGE_BODY.format(name=name, value=7))
            (copy / f"{name}.c").write_text(body)
            # An assembly unit takes no flags.
            text += unit if 'kind = "asm"' in unit else unit.replace('source = "', f'flags = [{self.flags}]\nsource = "', 1)
        (copy / "build.toml").write_text(text)


def make_image_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    """Module images: the declaration, then the build and its report."""
    fx = ImageFixture(parsed)
    seeds = ModuleSeeds(cfg_dir, parsed)
    unit = fx.unit
    other = "other"
    build_dir = lambda report: cfg_dir.parent / "build" / report["tag"]
    resident_keys = ("units", "coverage", "bss_bytes", "image_sha256", "executable_sha256", "baseline_executable_sha256", "controls")

    def install(**options):
        return lambda copy: fx.install(copy, **options)

    def flipped_first_word(copy: Path):
        data = bytearray(make_archive([(1, IMAGE_CHUNK)]))
        data[0x28] ^= 0xFF  # the first-word copy in the first entry
        fx.install(copy, archive=bytes(data))

    def refuses(name, reason, **options):
        return Case(f"image-{name}", False, reason, install(**options), status=2)

    def wrong_chunk(copy: Path):
        fx.install(copy, images=[fx.image(sha256="0" * 64)])

    # Builds. The resident unit returns 7 and the resident code is the chunk "A".
    res = lambda: unit("res", FIXTURE_LOAD, 8, image=None)
    mod = lambda **kw: unit("mod", FIXTURE_LOAD, 8, **kw)

    def built(chunk="A", **options):
        """A mutate function: the fixture over one of the seeded chunks, with the resident code."""
        def mutate(copy: Path):
            seeds.prepare()
            fx.install(copy, chunk=seeds.chunks[chunk], code=seeds.chunks["A"], **options)

        return mutate

    def two_images(copy: Path, **options):
        seeds.prepare()
        fx.install(
            copy,
            chunk=seeds.chunks["A"],
            code=seeds.chunks["A"],
            images=[fx.image(seeds.chunks["A"]), fx.image(seeds.chunks["B"], name=other, archive="OTHER.PAC")],
            archives={"OTHER.PAC": bytes(make_archive([(1, seeds.chunks["B"])]))},
            units=[res(), mod(), unit("mod2", FIXTURE_LOAD, 8, image=other)],
            sources={"mod2": IMAGE_BODY.format(name="mod2", value=9)},
            **options,
        )

    def differences(actual: dict, want: dict) -> list[str]:
        return [f"{key} is {actual.get(key)!r}, wanted {value!r}" for key, value in want.items() if actual.get(key) != value]

    def record(report: dict, name: str = "example"):
        return next((r for r in report.get("images", []) if r["name"] == name), None)

    def exact_resident(report: dict) -> list[str]:
        problems = []
        if report.get("executable_sha256") != report.get("baseline_executable_sha256"):
            problems.append("the resident executable is not exact")
        if not all(f["exact"] for u in report.get("units", []) for f in u["functions"]):
            problems.append("a resident function is not exact")
        return problems

    def verify_whole_c(report: dict):
        sha = hashlib.sha256(seeds.chunks["A"]).hexdigest()
        rec = record(report)
        if rec is None or len(report["images"]) != 1:
            return f"expected one record named 'example': {report.get('images')}"
        problems = differences(rec, {
            "slot": 1, "address": FIXTURE_LOAD, "size": 8, "baseline_sha256": sha, "image_sha256": sha,
            "exact": True, "carriers": 1, "bss_bytes": 0,
        })
        problems += differences(rec["coverage"], {
            "c_bytes": 8, "c_functions": 1, "asm_bytes": 0, "raw_payload_bytes": 0, "raw_ranges": 0,
        })
        if "raw_header_bytes" in rec["coverage"]:
            problems.append("a module image has no header: raw_header_bytes must be absent")
        if [u["name"] for u in rec["units"]] != ["mod"]:
            problems.append(f"units of the record: {[u['name'] for u in rec['units']]}")
        kinds = [(c["kind"], c["applicable"], c["tripped"]) for c in rec["controls"]]
        if kinds != [("function", True, True), ("raw", False, False)]:
            problems.append(f"controls: {kinds}")
        if report["units"] or report["coverage"]["raw_header_bytes"] != HEADER_SIZE:
            problems.append("the top level must describe the resident image alone, which has no unit here")
        if report["inputs"]["images"].get("example", {}).get("chunk") != sha:
            problems.append(f"inputs.images: {report['inputs'].get('images')}")
        if "selected_image" in report:
            problems.append("selected_image without --image")
        problems += exact_resident(report)
        out = build_dir(report)
        missing = [n for n in ("payload.bin", "raw.s", "link.ld", "image.elf", "image.bin") if not (out / "image-example" / n).is_file()]
        if missing or not (out / "unit-mod.o").is_file() or (out / "image-example" / "unit-mod.o").exists():
            problems.append(f"files of the module image: missing {missing}, objects in the wrong place")
        return "; ".join(problems) or None

    def verify_raw_around(report: dict):
        rec = record(report)
        if rec is None:
            return "no record"
        problems = differences(rec, {"size": 15, "exact": True})
        problems += differences(rec["coverage"], {"c_bytes": 8, "raw_payload_bytes": 7, "raw_ranges": 2})
        kinds = [(c["kind"], c["applicable"], c["tripped"]) for c in rec["controls"]]
        if kinds != [("function", True, True), ("raw", True, True)]:
            problems.append(f"controls: {kinds}")
        return "; ".join(problems) or None

    def verify_two_images(report: dict):
        names = [r["name"] for r in report.get("images", [])]
        if names != ["example", other]:
            return f"records in declaration order: {names}"
        a, b = record(report), record(report, other)
        problems = [] if a["exact"] and b["exact"] else ["both images must be exact"]
        if a["address"] != b["address"] or a["image_sha256"] == b["image_sha256"]:
            problems.append("the images must share an address and differ in content")
        problems += exact_resident(report)
        if sorted(report["inputs"]["sources"]) != ["mod", "mod2", "res"]:
            problems.append(f"sources: {sorted(report['inputs']['sources'])}")
        return "; ".join(problems) or None

    def verify_same_addresses(report: dict):
        rec = record(report)
        if rec is None or not rec["exact"] or [u["name"] for u in report["units"]] != ["res"]:
            return f"record {rec and rec['exact']}, resident units {[u['name'] for u in report['units']]}"
        if rec["units"][0]["range"] != report["units"][0]["range"]:
            return "the two units must cover the same addresses"
        return "; ".join(exact_resident(report)) or None

    def verify_selected_module(report: dict):
        problems = []
        if report.get("selected_image") != "example":
            problems.append(f"selected_image is {report.get('selected_image')!r}")
        present = [k for k in resident_keys if k in report]
        if present:
            problems.append(f"resident keys present: {present}")
        if [r["name"] for r in report.get("images", [])] != ["example"] or not record(report)["exact"]:
            problems.append(f"images: {[r['name'] for r in report.get('images', [])]}")
        if sorted(report["inputs"]["sources"]) != ["mod"] or sorted(report["inputs"]["images"]) != ["example", other]:
            problems.append(f"inputs: {sorted(report['inputs']['sources'])}, {sorted(report['inputs']['images'])}")
        out = build_dir(report)
        if (out / "unit-res.o").exists() or (out / "unit-mod2.o").exists() or (out / "image.bin").exists():
            problems.append("only the selected image may be compiled and linked")
        return "; ".join(problems) or None

    def verify_selected_resident(report: dict):
        problems = []
        if report.get("selected_image") != "resident":
            problems.append(f"selected_image is {report.get('selected_image')!r}")
        if "images" in report:
            problems.append("images present with the resident image selected")
        absent = [k for k in resident_keys if k not in report]
        if absent:
            problems.append(f"resident keys absent: {absent}")
        if sorted(report["inputs"]["sources"]) != ["res"] or sorted(report["inputs"]["images"]) != ["example", other]:
            problems.append(f"inputs: {sorted(report['inputs']['sources'])}, {sorted(report['inputs']['images'])}")
        out = build_dir(report)
        if (out / "unit-mod.o").exists() or (out / "image-example").exists():
            problems.append("module units must not be compiled or linked")
        return "; ".join(problems) or None

    def verify_changed(report: dict):
        problems = exact_resident(report)
        rec = record(report)
        if rec is None or rec["exact"]:
            problems.append("the module record must say that the image is not exact")
        if not report["failures"] or not all(f.startswith("image 'example': ") for f in report["failures"]):
            problems.append(f"failures must all name the image: {report['failures']}")
        return "; ".join(problems) or None

    def carriers(copy: Path):
        seeds.prepare()
        a, b = seeds.chunks["A"], seeds.chunks["B"]
        fx.install(
            copy, chunk=a, code=a, units=[res(), mod()],
            archives={
                "SECOND.PAC": bytes(make_archive([(1, a)])),  # the same bytes: counted
                "THIRD.PAC": bytes(make_archive([(1, b), ((1 << 16) | 1, a), (3, a)])),  # others in the slot, table 1, slot 3
                "BAD.PAC": bytes(10),  # rejected by the archive reader: ignored
            },
        )

    def verify_carriers(report: dict):
        rec = record(report)
        return None if rec and rec["carriers"] == 2 else f"carriers: {rec and rec['carriers']}, wanted 2"

    def changed_instruction(copy: Path):
        built("A", units=[res(), mod()], sources={"mod": IMAGE_BODY.format(name="mod", value=8)})(copy)

    def step_fails(unit_name: str, source: str, **unit_options):
        """A build in which one step of one unit's pipeline fails: the resident unit's or the module unit's."""
        return built("A", units=[res(), mod(**unit_options)], sources={unit_name: source})

    def verify_step_names_image(report: dict):
        failures = report.get("failures", [])
        if len(failures) != 1 or not failures[0].startswith("image 'example': "):
            return f"the failed step of a module unit must be the one failure and name its image: {failures}"
        return None

    def verify_step_resident(report: dict):
        failures = report.get("failures", [])
        if len(failures) != 1 or not failures[0].startswith("preprocess res failed"):
            return f"the failed step of a resident unit must be reported as before, without an image: {failures}"
        return None

    absent_header = '#include "absent.h"\n'

    # Names across images. The resident image is "pad" and "res", which calls the
    # module's "mod"; the module image is "mod", which calls "res".
    pad = lambda: unit("pad", FIXTURE_LOAD, 8, image=None)
    res_call = lambda: unit("res", FIXTURE_LOAD + 8, CALLER_SIZE, image=None)
    mod_call = lambda **kw: unit("mod", FIXTURE_LOAD, CALLER_SIZE, **kw)
    calls = {name: source for name, source, _, _ in seeds.CROSS_RES + seeds.CROSS_MOD}
    variable_range = f"bss = {{ address = {BSS_ADDRESS:#x}, size = 4 }}\n"

    def crossing(units=None, sources=None, **options):
        def mutate(copy: Path):
            seeds.prepare()
            fx.install(
                copy, chunk=seeds.chunks["M"], code=seeds.chunks["R"],
                units=[pad(), res_call(), mod_call()] if units is None else units,
                sources={**calls, **(sources or {})}, **options,
            )

        return mutate

    def crossing_two_images(copy: Path):
        seeds.prepare()
        fx.install(
            copy, chunk=seeds.chunks["M"], code=seeds.chunks["R"],
            images=[fx.image(seeds.chunks["M"]), fx.image(seeds.chunks["M"], name=other, archive="OTHER.PAC")],
            archives={"OTHER.PAC": bytes(make_archive([(1, seeds.chunks["M"])]))},
            units=[pad(), res_call(), mod_call(), unit("mod2", FIXTURE_LOAD, CALLER_SIZE, image=other)],
            sources={**calls, "mod2": "int res_fn(void);\nint mod2_fn(void) { return res_fn(); }\n"},
        )

    def assigned_lines(path: Path) -> list[str]:
        return path.read_text().splitlines() if path.is_file() else ["<missing>"]

    def verify_names(report: dict):
        problems = exact_resident(report)
        rec = record(report)
        if rec is None or not rec["exact"]:
            problems.append("the module image must be exact")
        out = build_dir(report)
        want = {
            out / "others.ld": [f"mod_fn = {FIXTURE_LOAD:#x};"],
            out / "image-example" / "others.ld": [f"pad_fn = {FIXTURE_LOAD:#x};", f"res_fn = {FIXTURE_LOAD + 8:#x};"],
        }
        for path, lines in want.items():
            if assigned_lines(path) != lines:
                problems.append(f"{path.relative_to(out)} is {assigned_lines(path)}, wanted {lines}")
            if "others.ld" not in (path.parent / "link.ld").read_text():
                problems.append(f"{path.parent.name} link.ld does not include others.ld")
        return "; ".join(problems) or None

    def verify_names_resident(report: dict):
        problems = exact_resident(report)
        if "images" in report or (build_dir(report) / "image-example").exists():
            problems.append("only the resident image may be built")
        return "; ".join(problems) or None

    def verify_names_module(report: dict):
        rec = record(report)
        problems = [] if rec and rec["exact"] else ["the module image must be exact"]
        if any(k in report for k in resident_keys) or (build_dir(report) / "unit-res.o").exists():
            problems.append("only the module image may be built")
        return "; ".join(problems) or None

    def verify_names_two(report: dict):
        problems = exact_resident(report)
        records = [record(report), record(report, other)]
        if any(r is None or not r["exact"] for r in records):
            problems.append("both module images must be exact")
        for name in ("example", other):
            lines = assigned_lines(build_dir(report) / f"image-{name}" / "others.ld")
            twin = f"mod{'2' if name == 'example' else ''}_fn = {FIXTURE_LOAD:#x};"  # the function of the other module
            if lines != [f"pad_fn = {FIXTURE_LOAD:#x};", f"res_fn = {FIXTURE_LOAD + 8:#x};", twin]:
                problems.append(f"others.ld of image {name}: {lines}")
        return "; ".join(problems) or None

    def verify_before_link(report: dict, image: str, declaring: str):
        failures = report.get("failures", [])
        out = build_dir(report)
        linked = (out / "image.elf").exists() or (out / "image-example" / "image.elf").exists()
        want = (f"image '{image}': " if image != "resident" else "") + "others.ld defines"
        if len(failures) != 1 or not failures[0].startswith(want) or f"image {declaring!r} declares" not in failures[0]:
            return f"the one failure must start with {want!r} and name the declaring image {declaring!r}: {failures}"
        return "a link ran although the check failed" if linked else None

    def verify_link_names_image(report: dict):
        failures = report.get("failures", [])
        if len(failures) != 1 or not failures[0].startswith("image 'example': "):
            return f"the failed link of a module image must name it: {failures}"
        return None

    # Addresses of the symbol file per image: one function that calls `ext_fn`, in two images, whose
    # baselines have the call going to two addresses.
    sym_first, sym_second = seeds.SYM_ADDRESSES
    sym_slot, sym_address = 2, 0x80800000  # the second image: its slot and its entry in table 0
    ext_text = f"ext_fn = {sym_first:#x};\n"

    def symbols_pair(table=True):
        def mutate(copy: Path):
            seeds.prepare()
            fx.install(
                copy,
                chunk=seeds.chunks["S1"],
                images=[
                    fx.image(seeds.chunks["S1"]),
                    fx.image(
                        seeds.chunks["S2"], name=other, archive="OTHER.PAC", slot=sym_slot, address=sym_address,
                        **({"symbols": {"ext_fn": sym_second}} if table else {}),
                    ),
                ],
                archives={"OTHER.PAC": bytes(make_archive([(sym_slot, seeds.chunks["S2"])]))},
                units=[unit("sym", FIXTURE_LOAD, CALLER_SIZE), unit("sym2", sym_address, CALLER_SIZE, image=other)],
                sources={"sym": seeds.SYM[0][1], "sym2": seeds.SYM[0][1].replace("sym_fn", "sym2_fn")},
                symbols=ext_text,
            )

        return mutate

    def verify_symbols_table(report: dict):
        problems = []
        first, second = record(report), record(report, other)
        if first is None or second is None or not (first["exact"] and second["exact"]):
            return "both module images must be exact"
        if first.get("symbols") != {} or second.get("symbols") != {"ext_fn": sym_second}:
            problems.append(f"symbols of the records: {first.get('symbols')!r}, {second.get('symbols')!r}")
        out = build_dir(report)
        if (out / "image-example" / "local.ld").exists() or "local.ld" in (out / "image-example" / "link.ld").read_text():
            problems.append("an image without the table must have no local.ld and no mention of it")
        local = out / f"image-{other}" / "local.ld"
        if assigned_lines(local) != [f"ext_fn = {sym_second:#x};"]:
            problems.append(f"local.ld of image {other}: {assigned_lines(local)}")
        link = (out / f"image-{other}" / "link.ld").read_text()
        order = [link.find(f"{n}.ld") for n in ("symbols", "others", "local")]
        if -1 in order or order != sorted(order):
            problems.append("link.ld must include symbols.ld, others.ld and local.ld in this order")
        return "; ".join(problems) or None

    def verify_symbols_missing(report: dict):
        first, second = record(report), record(report, other)
        failures = report.get("failures", [])
        problems = []
        if first is None or not first["exact"]:
            problems.append("the first image must be exact")
        if second is None or second["exact"]:
            problems.append("the second image must not be exact")
        if not failures or not all(f.startswith(f"image '{other}': ") for f in failures):
            problems.append(f"failures must all name the second image: {failures}")
        return "; ".join(problems) or None

    def symbols_refused(table):
        def mutate(copy: Path):
            fx.install(
                copy, symbols=ext_text,
                images=[fx.image(), fx.image(name=other, archive="OTHER.PAC", slot=sym_slot, address=sym_address, symbols=table)],
                archives={"OTHER.PAC": bytes(make_archive([(sym_slot, IMAGE_CHUNK)]))},
            )

        return mutate

    return [
        Case("symbols-both-images-exact-with-table", True, "", symbols_pair(), verify_symbols_table),
        Case("symbols-second-image-fails-without-table", False, f"image '{other}': function 'sym2_fn'",
             symbols_pair(table=False), verify_symbols_missing, status=1),
        Case("symbols-key-not-in-symbols-file", False, f"image '{other}': symbols names 'absent_fn', which symbols.ld does not assign",
             symbols_refused({"absent_fn": sym_second}), status=2),
        Case("symbols-value-is-a-string", False, f"image '{other}': symbols 'ext_fn' must be an integer address",
             symbols_refused({"ext_fn": "0x80320000"}), status=2),
        refuses("without-overlays", "needs an [overlays] section", overlays=False),
        refuses("pointers-outside-payload", "not a word address inside the resident payload", pointers=FIXTURE_LOAD + 0x10000),
        refuses("pointers-without-block", "no block of table addresses", pointers=FIXTURE_LOAD + 0x4),
        refuses("named-resident", "reserved for the resident image", images=[fx.image(name="resident")]),
        refuses("name-repeated", "duplicate image name", images=[fx.image(), fx.image()]),
        # A value of the wrong shape under the key is a configuration error, not a crash.
        refuses("value-is-a-number", "[[image]] must be an array of tables", images=[], head="image = 4\n"),
        refuses("value-is-one-table", "[[image]] must be an array of tables", images=[], head='image = { name = "example" }\n'),
        refuses("entry-is-not-a-table", "[[image]] #1: must be a table", images=[], head="image = [4]\n"),
        refuses("name-is-not-a-string", "'name' must be a non-empty string", images=[fx.image(name=5)]),
        refuses("archive-missing", "cannot read archive", archive=False),
        Case("image-archive-first-word-copy", False, "first word does not match", flipped_first_word, status=2),
        refuses("no-chunk-with-slot", "has 0 chunks with table number 0 and slot 0x1",
                archive=bytes(make_archive([((1 << 16) | 1, IMAGE_CHUNK)]))),
        refuses("two-chunks-with-slot", "has 2 chunks with table number 0 and slot 0x1",
                archive=bytes(make_archive([(1, IMAGE_CHUNK), (1, IMAGE_CHUNK)]))),
        refuses("chunk-without-bytes", "has no bytes", archive=bytes(make_archive([(1, b"")])),
                images=[fx.image(b"")]),
        Case("image-wrong-chunk-hash", False, "chunk sha256 mismatch", wrong_chunk, status=2),
        refuses("address-differs-from-table", "differs from entry 0x1 of table 0", images=[fx.image(address=FIXTURE_LOAD + 4)]),
        refuses("address-not-word", "is not a multiple of four", images=[fx.image(address=FIXTURE_LOAD + 2)]),
        refuses("slot-beyond-table", "is beyond table 0", images=[fx.image(slot=9)]),
        refuses("unit-unknown-image", "image 'absent' is not declared",
                units=[unit("res", FIXTURE_LOAD, image=None), unit("mod", FIXTURE_LOAD, image="absent")]),
        refuses("unit-outside-image", "unit 'mod' range",
                units=[unit("res", FIXTURE_LOAD, image=None), unit("mod", FIXTURE_LOAD + 0x10)]),
        refuses("units-overlap-in-image", "image 'example': units 'mod' and 'mod2' overlap",
                units=[unit("mod", FIXTURE_LOAD, 8), unit("mod2", FIXTURE_LOAD + 4, 8)]),
        refuses("bss-touches-payload", "unit 'mod' bss range", units=[unit(
            "mod", FIXTURE_LOAD, extra=f"bss = {{ address = {FIXTURE_LOAD + 8:#x}, size = 8 }}\n")]),
        refuses("function-name-in-two-images", "duplicate function name 'res_fn'",
                units=[unit("res", FIXTURE_LOAD, image=None),
                       unit("mod", FIXTURE_LOAD).replace('name = "mod_fn"', 'name = "res_fn"')]),
        Case("image-selected-unknown", False, "names no declared image", install(), status=2, extra=("--image", "absent")),
        Case("image-selected-unknown-without-images", False, "names no declared image",
             lambda copy: (copy / "build.toml").write_text(strip_images((copy / "build.toml").read_text())),
             status=2, extra=("--image", "absent")),
        # Failures of a module image name it and leave the resident image alone.
        Case("image-build-changed-instruction", False, "image 'example': function 'mod_fn': bytes differ",
             changed_instruction, verify_changed, status=1),
        Case("image-build-wrong-unit-size", False, "image 'example': unit 'mod': text size mismatch",
             built("A", units=[res(), unit("mod", FIXTURE_LOAD, 4)]), status=1),
        # A failed step of a module unit's pipeline names the image too, and keeps the step's own text.
        Case("image-build-preprocess-fails", False, "image 'example': preprocess mod failed",
             step_fails("mod", absent_header), verify_step_names_image, status=1),
        Case("image-build-compile-fails", False, "image 'example': compile mod failed",
             step_fails("mod", "int mod_fn(void) { return }\n"), verify_step_names_image, status=1),
        Case("image-build-assemble-fails", False, "image 'example': assemble mod failed",
             step_fails("mod", "not an instruction\n", extra='kind = "asm"\n'), verify_step_names_image, status=1),
        Case("image-build-resident-step-fails", False, "preprocess res failed",
             step_fails("res", absent_header), verify_step_resident, status=1),
        # Builds that must be exact.
        Case("image-build-whole-c-function", True, "", built("A", units=[mod()]), verify_whole_c),
        Case("image-build-raw-bytes-around", True, "",
             built("C", units=[res(), unit("mod", FIXTURE_LOAD + 4, 8)]), verify_raw_around),
        Case("image-build-same-addresses-as-resident", True, "", built("A", units=[res(), mod()]), verify_same_addresses),
        Case("image-build-two-images-same-address", True, "", two_images, verify_two_images),
        Case("image-build-selected-module", True, "", two_images, verify_selected_module, extra=("--image", "example")),
        Case("image-build-selected-resident", True, "", two_images, verify_selected_resident, extra=("--image", "resident")),
        Case("image-build-carriers", True, "", carriers, verify_carriers),
        # Names across images.
        Case("names-calls-between-images", True, "", crossing(), verify_names),
        Case("names-built-alone-resident", True, "", crossing(), verify_names_resident, extra=("--image", "resident")),
        Case("names-built-alone-module", True, "", crossing(), verify_names_module, extra=("--image", "example")),
        Case("names-two-modules-call-resident", True, "", crossing_two_images, verify_names_two),
        Case("names-module-variable-takes-resident-name", False,
             "image 'example': others.ld defines 'res_fn', which unit 'mod' defines in",
             crossing(units=[pad(), res_call(), unit("mod", FIXTURE_LOAD, 16, extra=variable_range)],
                      sources={"mod": "int res_fn;\nint mod_fn(void) { return res_fn; }\n"}),
             lambda report: verify_before_link(report, "example", "resident"), status=1),
        Case("names-resident-variable-takes-module-name", False,
             "others.ld defines 'mod_fn', which unit 'res' defines in",
             crossing(units=[pad(), unit("res", FIXTURE_LOAD + 8, 16, image=None, extra=variable_range), mod_call()],
                      sources={"res": "int mod_fn;\nint res_fn(void) { return mod_fn; }\n"}),
             lambda report: verify_before_link(report, "resident", "example"), status=1),
        Case("names-call-of-undeclared-function", False, "image 'example': link failed",
             crossing(sources={"mod": "int absent_fn(void);\nint mod_fn(void) { return absent_fn(); }\n"}),
             verify_link_names_image, status=1),
    ]


THIRD_SLOT, THIRD_ADDRESS = 3, 0x80900000


def second_fixture(parsed: dict):
    """The fixture of the controls on second links: `install` of a configuration that declares one.

    The first image is "example" with the unit "mod" (8 bytes of text); the second link is "second",
    like it, with a chunk of `size` bytes. `first` and `unit_extra` change the first image's unit,
    `second` the keys of the second link, and `more` adds images (name, slot, address, keys).
    """
    fx = ImageFixture(parsed)

    def install(copy: Path, *, size: int = 0x200, unit_extra: str = "", second: dict | None = None,
                more=(), units=None, first: dict | None = None):
        chunk = bytes(size)
        images = [
            fx.image(IMAGE_CHUNK, **(first or {})),
            fx.image(chunk, name="second", archive="SECOND.PAC", slot=SECOND_SLOT, address=SECOND_ADDRESS,
                     **({"like": "example"} if second is None else second)),
        ]
        archives = {"SECOND.PAC": bytes(make_archive([(SECOND_SLOT, chunk)]))}
        for name, slot, address, keys in more:
            images.append(fx.image(chunk, name=name, archive=f"{name.upper()}.PAC", slot=slot, address=address, **keys))
            archives[f"{name.upper()}.PAC"] = bytes(make_archive([(slot, chunk)]))
        fx.install(
            copy, images=images, archives=archives,
            units=[fx.unit("mod", FIXTURE_LOAD, 8, extra=unit_extra)] if units is None else units,
        )

    return fx, install


def make_second_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    """Second links: the declaration and its validation. Every case stops before the compiler."""
    fx, install = second_fixture(parsed)

    def refuses(name, reason, **options):
        return Case(f"second-{name}", False, reason, lambda copy: install(copy, **options), status=2)

    # A moved range lies outside a payload of 8 bytes: the first image's rodata or data sits at +8.
    beyond = lambda kind: f"{kind} = {{ address = {FIXTURE_LOAD + 8:#x}, size = 8 }}\n"
    return [
        refuses("like-names-no-image", "image 'second': 'like' names 'absent', which is not a declared image",
                second={"like": "absent"}),
        refuses("like-names-itself", "image 'second': 'like' names the image itself", second={"like": "second"}),
        refuses("like-names-a-second-link", "image 'third': 'like' names 'second', which is a second link itself",
                more=[("third", THIRD_SLOT, THIRD_ADDRESS, {"like": "second"})]),
        refuses("unit-in-a-second-link", "unit 'extra': image 'second' is a second link and has no units of its own",
                units=[fx.unit("mod", FIXTURE_LOAD, 8), fx.unit("extra", SECOND_ADDRESS, 8, image="second")]),
        refuses("leave-out-without-like", "image 'second': 'leave_out' needs 'like'", second={"leave_out": ["mod"]}),
        refuses("leave-out-without-like-empty", "image 'second': 'leave_out' needs 'like'", second={"leave_out": []}),
        refuses("leave-out-not-a-unit", "image 'second': 'leave_out' names 'absent', which is not a unit of image 'example'",
                second={"like": "example", "leave_out": ["absent"]}),
        refuses("leave-out-twice", "image 'second': 'leave_out' names 'mod' twice",
                second={"like": "example", "leave_out": ["mod", "mod"]}),
        refuses("moved-text-outside", "image 'second': moved unit 'mod' range", size=4),
        refuses("moved-rodata-outside", "image 'second': moved unit 'mod' rodata range", size=8,
                units=[fx.unit("mod", FIXTURE_LOAD, 8, extra=beyond("rodata"))]),
        refuses("moved-data-outside", "image 'second': moved unit 'mod' data range", size=8,
                units=[fx.unit("mod", FIXTURE_LOAD, 8, extra=beyond("data"))]),
        # The bss lies outside the first payload and moves into the second.
        refuses("moved-bss-touches-payload", "image 'second': moved unit 'mod' bss range",
                units=[fx.unit("mod", FIXTURE_LOAD, 8, extra=f"bss = {{ address = {FIXTURE_LOAD + 0x100:#x}, size = 4 }}\n")]),
    ]


def install_two_sides(fx, seeds, copy: Path, body: bytes | None = None, leave_out=(), strays: bool = False) -> None:
    """The fixture of the second-link builds: four units in the first image, and a second link of the same sources.

    `body` is the second link's chunk, the seeded one by default. `strays` adds an ordinary image "other"
    with one unit "stray", which the second link is not like.
    """
    seeds.prepare()
    body = seeds.chunks["G"] if body is None else body
    keys = {"like": "example", "leave_out": list(leave_out)} if leave_out else {"like": "example"}
    images = [
        fx.image(seeds.chunks["F"]),
        fx.image(body, name="second", archive="SECOND.PAC", slot=SECOND_SLOT, address=SECOND_ADDRESS, **keys),
    ]
    archives = {"SECOND.PAC": bytes(make_archive([(SECOND_SLOT, body)]))}
    units = [
        fx.unit("callee", FIXTURE_LOAD, 8),
        fx.unit("owner", FIXTURE_LOAD + 8, 8, extra=f"data = {{ address = {FIXTURE_LOAD + 16:#x}, size = 4 }}\n"),
        fx.unit("caller", FIXTURE_LOAD + 20, TWO_CALLER_SIZE),
        fx.unit("lone", FIXTURE_LOAD + 20 + TWO_CALLER_SIZE, 8),
    ]
    if strays:
        images.append(fx.image(bytes(8), name="other", archive="OTHER.PAC", slot=THIRD_SLOT, address=THIRD_ADDRESS))
        archives["OTHER.PAC"] = bytes(make_archive([(THIRD_SLOT, bytes(8))]))
        units.append(fx.unit("stray", THIRD_ADDRESS, 8, image="other"))
    fx.install(
        copy, chunk=seeds.chunks["F"], code=bytes(0), images=images, archives=archives, units=units,
        sources={name: source for name, source, *_ in seeds.TWO_SIDES},
    )


def make_second_build_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    """Second links built: a first image of four units and a second link whose chunk is the same sources at its address."""
    fx = ImageFixture(parsed)
    seeds = ModuleSeeds(cfg_dir, parsed)
    shift = SECOND_ADDRESS - FIXTURE_LOAD
    first_side = {"callee": 0, "owner": 8, "caller": 20, "lone": 20 + TWO_CALLER_SIZE}
    build_dir = lambda report: cfg_dir.parent / "build" / report["tag"]
    record = lambda report, name: next((r for r in report.get("images", []) if r["name"] == name), None)
    resident_keys = ("units", "coverage", "bss_bytes", "image_sha256", "executable_sha256", "controls")

    def second_chunk(patch=None) -> bytes:
        chunk = bytearray(seeds.chunks["G"])
        if patch:
            patch(chunk)
        return bytes(chunk)

    def install(chunk=None, leave_out=()):
        return lambda copy: install_two_sides(fx, seeds, copy, None if chunk is None else chunk(seeds), leave_out)

    def words_of(chunk, start, size):
        return [(i, int.from_bytes(chunk[i : i + 4], "little")) for i in range(start, start + size, 4)]

    def patched_call(chunk):
        """The second chunk whose call goes to the callee's first address."""
        def patch(seeds_):
            data = bytearray(seeds_.chunks["G"])
            at = next(i for i, w in words_of(data, first_side["caller"], TWO_CALLER_SIZE) if w >> 26 == 3)
            data[at : at + 4] = (0x0C000000 | ((FIXTURE_LOAD >> 2) & 0x03FFFFFF)).to_bytes(4, "little")
            return bytes(data)
        return patch

    def patched_variable(seeds_):
        """The second chunk whose read of the variable uses the first address."""
        data = bytearray(seeds_.chunks["G"])
        at = next(i for i, w in words_of(data, first_side["caller"], TWO_CALLER_SIZE) if w >> 26 == 0x0F)
        data[at : at + 2] = ((FIXTURE_LOAD + 16) >> 16).to_bytes(2, "little")
        return bytes(data)

    def changed_word(seeds_):
        data = bytearray(seeds_.chunks["G"])
        data[0] ^= 0xFF
        return bytes(data)

    def first_exact(report):
        rec = record(report, "example")
        problems = []
        if rec is None or not rec["exact"]:
            problems.append("the first image must be exact")
        elif rec["coverage"].get("c_functions") != 4 or "like" in rec:
            problems.append(f"the first image's record must be unchanged, with its 4 functions counted once: {rec['coverage']}")
        return problems

    def assigned(path: Path):
        return path.read_text().splitlines() if path.is_file() else ["<missing>"]

    def verify_exact(report: dict, left_out=(), again_functions=4, again_bytes=72):
        rec = record(report, "second")
        problems = first_exact(report)
        if rec is None or not rec["exact"]:
            return "; ".join(problems + ["the second link must be exact"])
        problems += [
            f"{key} is {rec.get(key)!r}, wanted {want!r}"
            for key, want in {"like": "example", "shift": shift, "left_out": list(left_out)}.items()
            if rec.get(key) != want
        ]
        cov = rec["coverage"]
        if cov.get("linked_again_functions") != again_functions or cov.get("linked_again_bytes") != again_bytes:
            problems.append(f"coverage: {cov}")
        if any(k in cov for k in ("c_bytes", "c_functions", "asm_bytes", "asm_functions")):
            problems.append(f"a second link counts no function from C or assembly: {cov}")
        want_units = [n for n in first_side if n not in left_out]
        if [u["name"] for u in rec["units"]] != want_units:
            problems.append(f"units: {[u['name'] for u in rec['units']]}")
        elif [u["range"][0] for u in rec["units"]] != [SECOND_ADDRESS + first_side[n] for n in want_units]:
            problems.append("the ranges of the units must be the moved ranges")
        if not rec["controls"] or not all(c["tripped"] for c in rec["controls"] if c["applicable"]):
            problems.append(f"controls: {rec['controls']}")
        return "; ".join(problems) or None

    def verify_all(report: dict):
        problems = verify_exact(report)
        out = build_dir(report)
        first_names = [f"{n}_fn = {FIXTURE_LOAD + a:#x};" for n, a in first_side.items()]
        if assigned(out / "others.ld") != first_names:
            problems = (problems or "") + f"; resident others.ld is {assigned(out / 'others.ld')}"
        if assigned(out / "image-second" / "others.ld") != []:
            problems = (problems or "") + f"; others.ld of the second link: {assigned(out / 'image-second' / 'others.ld')}"
        if assigned(out / "image-example" / "others.ld") != []:
            problems = (problems or "") + f"; others.ld of the first image: {assigned(out / 'image-example' / 'others.ld')}"
        return problems

    def verify_lone_out(report: dict):
        problems = verify_exact(report, ("lone",), 3, 64)
        cov = record(report, "second")["coverage"]
        if cov["raw_payload_bytes"] != 8:
            problems = (problems or "") + f"; the unit left out must count as raw: {cov}"
        lines = assigned(build_dir(report) / "image-second" / "others.ld")
        if lines != [f"lone_fn = {SECOND_ADDRESS + first_side['lone']:#x};"]:
            problems = (problems or "") + f"; others.ld of the second link: {lines}"
        return problems

    def verify_callee_out(report: dict):
        problems = verify_exact(report, ("callee",), 3, 64)
        out = build_dir(report) / "image-second"
        lines = assigned(out / "others.ld")
        if lines != [f"callee_fn = {SECOND_ADDRESS:#x};"]:
            problems = (problems or "") + f"; others.ld of the second link: {lines}"
        image = (out / "image.bin").read_bytes()
        word = next(w for _, w in words_of(image, first_side["caller"], TWO_CALLER_SIZE) if w >> 26 == 3)
        if word & 0x03FFFFFF != (SECOND_ADDRESS >> 2) & 0x03FFFFFF:
            problems = (problems or "") + f"; the call word is {word:#010x}, wanted the callee's second address"
        return problems

    def verify_owner_out(report: dict):
        problems = verify_exact(report, ("owner",), 3, 64)
        lines = assigned(build_dir(report) / "image-second" / "others.ld")
        want = [f"owner_fn = {SECOND_ADDRESS + 8:#x};", f"shared_var = {SECOND_ADDRESS + 16:#x};"]
        if lines != want:
            problems = (problems or "") + f"; others.ld of the second link: {lines}, wanted {want}"
        return problems

    def verify_selected(report: dict):
        problems = []
        if report.get("selected_image") != "second":
            problems.append(f"selected_image is {report.get('selected_image')!r}")
        if [r["name"] for r in report.get("images", [])] != ["second"] or not record(report, "second")["exact"]:
            problems.append("only the second link must be built, and exact")
        if any(k in report for k in resident_keys):
            problems.append("resident keys present")
        out = build_dir(report)
        if sorted(report["inputs"]["sources"]) != sorted(first_side):
            problems.append(f"every unit of the first image is compiled: {sorted(report['inputs']['sources'])}")
        if (out / "image-example").exists() or (out / "image.bin").exists() or not (out / "unit-lone.o").is_file():
            problems.append("only the second link may be linked, with the objects of the first image's units")
        return "; ".join(problems) or None

    def verify_fails(report: dict):
        problems = first_exact(report)
        rec = record(report, "second")
        if rec is None or rec["exact"]:
            problems.append("the second link's record must say that it is not exact")
        failures = report.get("failures", [])
        if not failures or not all(f.startswith("image 'second': ") for f in failures):
            problems.append(f"failures must all name the second link: {failures}")
        return "; ".join(problems) or None

    differs = "image 'second': function '{}': bytes differ"
    return [
        Case("second-link-exact", True, "", install(), verify_all),
        Case("second-link-unit-left-out", True, "", install(leave_out=["lone"]), verify_lone_out),
        Case("second-link-callee-left-out", True, "", install(leave_out=["callee"]), verify_callee_out),
        Case("second-link-variable-owner-left-out", True, "", install(leave_out=["owner"]), verify_owner_out),
        Case("second-link-built-alone", True, "", install(), verify_selected, extra=("--image", "second")),
        Case("second-link-changed-word", False, differs.format("callee_fn"), install(chunk=changed_word), verify_fails, status=1),
        Case("second-link-call-to-first-address", False, differs.format("caller_fn"),
             install(chunk=patched_call(None)), verify_fails, status=1),
        Case("second-link-call-to-first-address-callee-left-out", False, differs.format("caller_fn"),
             install(chunk=patched_call(None), leave_out=["callee"]), verify_fails, status=1),
        Case("second-link-variable-at-first-address", False, differs.format("caller_fn"),
             install(chunk=patched_variable), verify_fails, status=1),
    ]


def install_moved_names(fx, seeds, copy: Path, body: str = "I", table: dict | None = None) -> None:
    """The fixture of the moved names: one unit that reads a name inside the first image and one outside it."""
    seeds.prepare()
    chunk = seeds.chunks[body]
    second = {"like": "example", **({"symbols": table} if table else {})}
    fx.install(
        copy, chunk=seeds.chunks["H"], code=bytes(0), symbols=seeds.reader_symbols(FIXTURE_LOAD + MOVED_OFFSET),
        images=[
            fx.image(seeds.chunks["H"]),
            fx.image(chunk, name="second", archive="SECOND.PAC", slot=SECOND_SLOT, address=SECOND_ADDRESS, **second),
        ],
        archives={"SECOND.PAC": bytes(make_archive([(SECOND_SLOT, chunk)]))},
        units=[fx.unit("reader", FIXTURE_LOAD, MOVED_READER_SIZE)],
        sources={"reader": seeds.READER[0][1]},
    )


def make_moved_symbol_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    """Names of symbols.ld inside the first image move with a second link."""
    fx = ImageFixture(parsed)
    seeds = ModuleSeeds(cfg_dir, parsed)
    moved = SECOND_ADDRESS + MOVED_OFFSET
    record = lambda report, name: next((r for r in report.get("images", []) if r["name"] == name), None)

    def install(body="I", table=None):
        return lambda copy: install_moved_names(fx, seeds, copy, body, table)

    def exact_both(report):
        first, second = record(report, "example"), record(report, "second")
        if first is None or second is None or not (first["exact"] and second["exact"]):
            return None, "both images must be exact"
        if "moved_symbols" in first:
            return None, "an image that is not a second link has no moved_symbols"
        return second, None

    def verify_moved(report: dict):
        second, problem = exact_both(report)
        if problem:
            return problem
        if second.get("moved_symbols") != {"inside_var": moved}:
            return f"moved_symbols is {second.get('moved_symbols')!r}"
        return None if second.get("symbols") == {} else f"symbols is {second.get('symbols')!r}"

    def verify_outside(report: dict):
        second, problem = exact_both(report)
        if problem:
            return problem
        if "outside_var" in second.get("moved_symbols", {}):
            return "a name outside the first image's payload is not moved"
        return None

    def verify_table(report: dict):
        second, problem = exact_both(report)
        if problem:
            return problem
        if second.get("moved_symbols") != {} or second.get("symbols") != {"inside_var": MOVED_ELSEWHERE}:
            return f"moved_symbols {second.get('moved_symbols')!r}, symbols {second.get('symbols')!r}"
        return None

    def verify_fails(report: dict):
        first, second = record(report, "example"), record(report, "second")
        failures = report.get("failures", [])
        if first is None or not first["exact"] or second is None or second["exact"]:
            return "the first image must be exact and the second link not"
        if not failures or not all(f.startswith("image 'second': ") for f in failures):
            return f"failures must all name the second link: {failures}"
        return None

    return [
        Case("second-moved-symbol-exact", True, "", install(), verify_moved),
        Case("second-moved-symbol-outside-is-not-moved", True, "", install(), verify_outside),
        Case("second-moved-symbol-read-at-first-address", False, "image 'second': function 'reader_fn': bytes differ",
             install("J"), verify_fails, status=1),
        Case("second-moved-symbol-table-wins", True, "", install("K", {"inside_var": MOVED_ELSEWHERE}), verify_table),
    ]


def make_moved_symbol_fndiff_cases(cfg_dir: Path, parsed: dict) -> list[CacheCase]:
    fx = ImageFixture(parsed)
    seeds = ModuleSeeds(cfg_dir, parsed)

    def identical(ctx):
        install_moved_names(fx, seeds, ctx.copy)
        proc, _, _ = ctx.run()
        if not passed(proc):
            return f"test setup: the whole build must pass: {describe(proc)}"
        proc = subprocess.run(
            [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, "--image", "second", "reader"],
            capture_output=True, text=True,
        )
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"the reader as the second link has it must be IDENTICAL: exit {proc.returncode}\n{(proc.stdout + proc.stderr)[-600:]}"
        return None

    return [CacheCase("second-moved-symbol-fndiff-identical", identical)]


def make_second_fndiff_cases(cfg_dir: Path, parsed: dict) -> list[CacheCase]:
    """`fndiff.py --image`: a unit compared as a second link has it."""
    fx = ImageFixture(parsed)
    seeds = ModuleSeeds(cfg_dir, parsed)

    def diff(ctx, *args: str, rebuild: bool = False) -> subprocess.CompletedProcess:
        command = [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag]
        if rebuild:
            command += ["--rebuild", "--cache", str(ctx.cache)]
        return subprocess.run([*command, *args], capture_output=True, text=True)

    def say(proc) -> str:
        return f"exit {proc.returncode}:\n{(proc.stdout + proc.stderr)[-700:]}"

    def expect(proc, status: int, text: str, what: str):
        output = proc.stdout + proc.stderr
        return None if proc.returncode == status and text in output else f"{what}: wanted exit {status} with {text!r}, got {say(proc)}"

    def built(ctx, body=None, leave_out=(), passes=True):
        install_two_sides(fx, seeds, ctx.copy, body, leave_out)
        proc, _, _ = ctx.run()
        if passes != passed(proc):
            return f"test setup: the whole build {'must pass' if passes else 'must fail'}: {describe(proc)}"
        return None

    def identical(ctx):
        return (
            built(ctx)
            or expect(diff(ctx, "--image", "second", "caller"), 0, "IDENTICAL", "the caller as the second link has it")
            or expect(diff(ctx, "caller"), 0, "IDENTICAL", "the caller in its own image")
        )

    def changed_chunk(ctx):
        seeds.prepare()
        data = bytearray(seeds.chunks["G"])
        data[0] ^= 0xFF  # inside callee_fn
        return (
            built(ctx, bytes(data), passes=False)
            or expect(diff(ctx, "--image", "second", "callee"), 1, "DIFFERENT", "the callee as the second link has it")
            or expect(diff(ctx, "callee"), 0, "IDENTICAL", "the callee in its own image")
        )

    def callee_left_out(ctx):
        return (
            built(ctx, leave_out=["callee"])
            or expect(diff(ctx, "--image", "second", "caller"), 0, "IDENTICAL", "the caller with the callee left out")
        )

    def rebuilt(ctx):
        problem = built(ctx)
        if problem:
            return problem
        source = (ctx.copy / "caller.c").read_text()
        (ctx.copy / "caller.c").write_text(source.replace("shared_var; }", "shared_var + 1; }"))
        problem = expect(diff(ctx, "--image", "second", "caller", rebuild=True), 1, "DIFFERENT", "after the change")
        (ctx.copy / "caller.c").write_text(source)
        problem = problem or expect(diff(ctx, "--image", "second", "caller", rebuild=True), 0, "IDENTICAL", "after restoring")
        if problem:
            return problem
        # A failed step of the unit's pipeline names the unit's own image, as a whole build does: the
        # pipeline makes the unit's one object, whichever link the diff is for.
        (ctx.copy / "caller.c").write_text("int caller_fn(void) { return }\n")
        proc = diff(ctx, "--image", "second", "caller", rebuild=True)
        (ctx.copy / "caller.c").write_text(source)
        output = proc.stdout + proc.stderr
        if proc.returncode != 1 or "FAIL: image 'example': compile caller" not in output or "image 'second': compile" in output:
            return f"a failed step under --rebuild --image must name the unit's own image: {say(proc)}"
        return None

    def refusals(ctx):
        install_two_sides(fx, seeds, ctx.copy, strays=True)
        for args, text in (
            (("--image", "absent", "caller"), "'absent' names no declared image"),
            (("--image", "resident", "caller"), "--image is for a second link: 'resident'"),
            (("--image", "example", "caller"), "--image is for a second link: 'example'"),
            (("--image", "other", "caller"), "--image is for a second link: image 'other' is not one"),
            (("--image", "second", "stray"), "image 'second': --image: this second link is like image 'example', not like 'other'"),
        ):
            problem = expect(diff(ctx, *args), 2, text, " ".join(args))
            if problem:
                return problem
        install_two_sides(fx, seeds, ctx.copy, leave_out=["callee"])
        return expect(
            diff(ctx, "--image", "second", "callee"), 2, "image 'second': --image: unit 'callee' is left out", "a unit left out"
        )

    def overrides(ctx):
        """The owned-name rule holds in this link, with the second link's names and its prefix."""
        for leave_out, name, text in (
            ((), "lone_fn", "image 'second': unit 'caller' defines 'lone_fn' in .data, which unit 'lone' of its image defines too"),
            (("callee",), "callee_fn", "image 'second': others.ld defines 'callee_fn', which unit 'caller' defines in .data"),
        ):
            install_two_sides(fx, seeds, ctx.copy, None, leave_out)
            (ctx.copy / "caller.c").write_text(f"int {name} = 1;\nint caller_fn(void) {{ return {name}; }}\n")
            proc, _, _ = ctx.run()
            if passed(proc):
                return "test setup: the whole build must reject this fixture"
            for rebuild in (False, True):
                got = diff(ctx, "--image", "second", "caller", rebuild=rebuild)
                problem = expect(got, 1, text, f"{name} {'with' if rebuild else 'without'} --rebuild")
                if problem or (ctx.build / "unit-caller.second.fndiff.elf").exists():
                    return problem or "the unit was linked although a name given to the link overrides its symbol"
        return None

    return [
        CacheCase("second-fndiff-overrides", overrides),
        CacheCase("second-fndiff-identical", identical),
        CacheCase("second-fndiff-changed-chunk", changed_chunk),
        CacheCase("second-fndiff-callee-left-out", callee_left_out),
        CacheCase("second-fndiff-rebuild", rebuilt),
        CacheCase("second-fndiff-refusals", refusals),
    ]


def make_second_map_cases(cfg_dir: Path, parsed: dict) -> list[CacheCase]:
    """The placement map of a second link, from the configuration alone."""
    fx, install = second_fixture(parsed)

    def placement(ctx):
        install(
            ctx.copy, second={"like": "example", "leave_out": ["mod"]},
            units=[
                fx.unit("mod", FIXTURE_LOAD, 4, extra=f"rodata = {{ address = {FIXTURE_LOAD + 4:#x}, size = 4 }}\n"
                        f"bss = {{ address = {FIXTURE_LOAD + 0x300:#x}, size = 4 }}\n"),
                fx.unit("other", FIXTURE_LOAD + 8, 8),
            ],
        )
        cfg = matchbuild.load_config(ctx.config)
        shift = SECOND_ADDRESS - FIXTURE_LOAD
        units, left_out = cfg.placed_units("second")
        want = {
            "mod": (FIXTURE_LOAD + shift, (FIXTURE_LOAD + 4 + shift, 4), (FIXTURE_LOAD + 0x300 + shift, 4)),
            "other": (FIXTURE_LOAD + 8 + shift, None, None),
        }
        got = {u.name: (u.functions[0].address, u.rodata and (u.rodata.address, u.rodata.size), u.bss and (u.bss.address, u.bss.size))
               for u in units}
        if got != want or left_out != ("mod",):
            return f"placement map is {got} with {left_out} left out, wanted {want} with ('mod',)"
        if [u.image for u in units] != ["second", "second"]:
            return "the moved units belong to the second link"
        if cfg.units_of("second") or [u.name for u in cfg.units_of("example")] != ["mod", "other"]:
            return "the configuration's own units must stay as declared"
        own, none = cfg.placed_units("example")
        if own != cfg.units_of("example") or none != ():
            return "an image that is not a second link takes its own units unchanged"
        # A second link gives no names to another link: it has no unit of its own.
        names = [name for name, _, _ in cfg.functions_of_others("resident")]
        if names != ["mod_fn", "other_fn"] or [i for _, _, i in cfg.functions_of_others("resident")] != ["example"] * 2:
            return f"the resident link is given {names}"
        return None

    def integers(ctx):
        """The integers of symbols.ld are read as the linker reads them: a leading 0 is octal."""
        got = {text: matchbuild.linker_integer(text) for text in ("0", "10", "010", "0x10", "0X1f", "0777", "089", "08")}
        want = {"0": 0, "10": 10, "010": 8, "0x10": 16, "0X1f": 31, "0777": 511, "089": None, "08": None}
        if got != want:
            return f"read as {got}, wanted {want}"
        errors: list[str] = []
        values: dict[str, int] = {}
        names = matchbuild.parse_symbols("eight = 010;\nbad = 089;\nsixteen = 0x10;\n", errors, values)
        if names != ["eight", "bad", "sixteen"] or values != {"eight": 8, "sixteen": 16}:
            return f"parsed {names} with {values}"
        if len(errors) != 1 or "'bad' is assigned '089'" not in errors[0]:
            return f"an integer the linker does not read must be one configuration error: {errors}"
        return None

    def octal_moves(ctx):
        """An address written in octal lies inside the first image and moves. Read as decimal it would lie outside."""
        install(ctx.copy, second={"like": "example"})
        inside = FIXTURE_LOAD + 8
        symbols = ctx.copy / "symbols.ld"
        symbols.write_text(symbols.read_text() + f"inside_octal = 0{inside:o};\n")
        if int(f"0{inside:o}".lstrip("0")) in range(FIXTURE_LOAD, FIXTURE_LOAD + 0x10000):
            return "test setup: the decimal reading must lie outside the first image"
        cfg = matchbuild.load_config(ctx.config)
        if cfg.symbol_values.get("inside_octal") != inside:
            return f"inside_octal is read as {cfg.symbol_values.get('inside_octal')}, wanted {inside}"
        moved = cfg.local_symbols("second")[1]
        if moved.get("inside_octal") != inside + SECOND_ADDRESS - FIXTURE_LOAD:
            return f"the second link must be given the name at its moved address: {moved}"
        return None

    def not_an_integer(ctx):
        """Through the tool: a configuration error with exit status 2, no traceback."""
        install(ctx.copy, second={"like": "example"})
        symbols = ctx.copy / "symbols.ld"
        symbols.write_text(symbols.read_text() + "bad_octal = 089;\n")
        proc, _, _ = ctx.run()
        output = proc.stdout + proc.stderr
        if proc.returncode != 2 or "CONFIG ERROR: symbols.ld: 'bad_octal' is assigned '089'" not in output or "Traceback" in output:
            return f"wanted a configuration error with exit status 2: exit {proc.returncode}\n{output[-600:]}"
        return None

    return [
        CacheCase("second-placement-map", placement),
        CacheCase("symbols-file-integers", integers),
        CacheCase("second-moved-symbol-octal-address", octal_moves),
        CacheCase("symbols-file-not-an-integer", not_an_integer),
    ]


def make_comparison_unit_cases() -> list[CacheCase]:
    """What a comparison reports as failures, for an image made here: 8 bytes of one function, 8 retained."""
    load = 0x80300000
    payload = bytes(range(16))
    unit = matchbuild.UnitDecl("u", "u.c", (), (matchbuild.FunctionDecl("f", load, 8),))

    def reasons(image: bytes) -> list[str]:
        return matchbuild.comparison_failures(matchbuild.compare_image(image, payload, load, [unit]))

    def equal(root: Path):
        got = reasons(payload)
        return f"an equal image must report nothing: {got}" if got else None

    def retained_byte(root: Path):
        """No range of a unit differs: only the line for the image can report it."""
        got = reasons(matchbuild.flip_byte(payload, 12))
        if len(got) != 1 or not got[0].startswith("image differs from baseline payload"):
            return f"a changed retained byte must be reported by the image line alone: {got}"
        return None

    def function_byte(root: Path):
        got = reasons(matchbuild.flip_byte(payload, 2))
        if len(got) != 2 or not got[0].startswith("function 'f': bytes differ") or not got[1].startswith("image differs"):
            return f"a changed function byte must be reported for the function and the image: {got}"
        return None

    def shorter(root: Path):
        got = reasons(payload[:12])
        if not any(r.startswith("image differs from baseline payload (size 12 vs 16") for r in got):
            return f"a shorter image must be reported with both sizes: {got}"
        return None

    table = [
        ("comparison-equal-image", equal),
        ("comparison-retained-byte", retained_byte),
        ("comparison-function-byte", function_byte),
        ("comparison-shorter-image", shorter),
    ]
    return [CacheCase(name, body) for name, body in table]


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
        proc = run_tool(copy / "build.toml", tag, cache, case.extra)
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
            if case.status is not None and proc.returncode != case.status:
                return False, f"wanted exit status {case.status}, got {proc.returncode}:\n{output}"
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


# ---------------------------------------------------------------------------
# fndiff.py --rebuild: the loop for one unit, on a two-unit fixture.

TWO_ALPHA = "int alpha(void) { return 7; }\n"
TWO_BETA = "int beta(void) { return 9; }\n"
TWO_BROKEN = "int alpha(void) { return 7 }\n"


def make_publish_unit_cases() -> list[CacheCase]:
    """`fndiff.publish`: the report of the last whole build goes before any file of the unit is replaced."""

    def setup(root: Path) -> tuple[Path, Path]:
        build, scratch = root / "build", root / "scratch"
        for folder, word in ((build, b"old"), (scratch, b"new")):
            folder.mkdir()
            for ext in (".s", ".o"):
                (folder / f"unit-u{ext}").write_bytes(word + ext.encode())
        for name in fndiff.BUILD_RESULTS:
            (build / name).write_text("exact")
        return build, scratch

    def state(build: Path) -> dict:
        found = {name: (build / name).exists() for name in fndiff.BUILD_RESULTS}
        found.update({ext: (build / f"unit-u{ext}").read_bytes()[:3].decode() for ext in (".s", ".o")})
        return found

    def failing(after: int, error: BaseException):
        """A replacement that works `after` times and then raises."""
        done = []

        def replace(source, dest):
            if len(done) == after:
                raise error
            done.append(dest)
            os.replace(source, dest)

        return replace

    def whole(root: Path):
        build, scratch = setup(root)
        fndiff.publish(scratch, build, "u")
        want = {"report.json": False, "summary.txt": False, ".s": "new", ".o": "new"}
        return None if state(build) == want else f"after a publication the directory holds {state(build)}"

    def interrupted(root: Path):
        """An interruption after the first file is in place: the report must already be gone."""
        build, scratch = setup(root)
        try:
            fndiff.publish(scratch, build, "u", replace=failing(1, KeyboardInterrupt()))
        except KeyboardInterrupt:
            pass
        else:
            return "the interruption did not reach the caller"
        found = state(build)
        if found["report.json"] or found["summary.txt"]:
            return f"an interrupted publication left the report of the earlier build: {found}"
        if sorted([found[".s"], found[".o"]]) != ["new", "old"]:
            return f"the fixture did not stop between the two files: {found}"
        return None

    def replace_error(root: Path):
        build, scratch = setup(root)
        try:
            fndiff.publish(scratch, build, "u", replace=failing(0, OSError("no space")))
        except OSError:
            pass
        else:
            return "the error did not reach the caller"
        want = {"report.json": False, "summary.txt": False, ".s": "old", ".o": "old"}
        return None if state(build) == want else f"after a failed first replacement the directory holds {state(build)}"

    def report_stays(root: Path):
        """The report cannot be removed (it is a directory here): no file of the unit may be replaced."""
        build, scratch = setup(root)
        (build / "report.json").unlink()
        (build / "report.json").mkdir()
        try:
            fndiff.publish(scratch, build, "u")
        except OSError:
            pass
        else:
            return "a report that cannot be removed must stop the publication"
        found = state(build)
        if found[".s"] != "old" or found[".o"] != "old" or not (scratch / "unit-u.o").exists():
            return f"a file was replaced although the report could not be removed: {found}"
        return None

    table = [
        ("publish-removes-report-and-replaces", whole),
        ("publish-interrupted-between-files", interrupted),
        ("publish-replacement-fails", replace_error),
        ("publish-report-cannot-be-removed", report_stays),
    ]
    return [CacheCase(name, body) for name, body in table]


def make_fndiff_cases(cfg_dir: Path, parsed: dict) -> list[CacheCase]:
    """`fndiff.py --rebuild`: one unit through the pipeline of a whole build, then the object checks and the diff."""
    flags = ", ".join(json.dumps(f) for f in parsed["unit"][0]["flags"])
    toolchain = fixture_toolchain(parsed)

    def write(ctx: CacheContext, payload: bytes, beta_size: int = 8) -> None:
        executable = fixture_executable(payload)
        (ctx.copy / "baseline.bin").write_bytes(executable)
        units = ""
        for name, offset, size in (("alpha", 0, 8), ("beta", 8, beta_size)):
            units += (
                f'[[unit]]\nname = "{name}"\nsource = "{name}.c"\nflags = [{flags}]\n'
                f'functions = [ {{ name = "{name}", address = {FIXTURE_LOAD + offset:#x}, size = {size} }} ]\n\n'
            )
        (ctx.copy / "build.toml").write_text(
            "[baseline]\n"
            'executable = "baseline.bin"\n'
            f'sha256 = "{hashlib.sha256(executable).hexdigest()}"\n\n'
            + toml_table("toolchain", toolchain)
            + units
        )

    def fixture(ctx: CacheContext, build: bool = True):
        """Replace the copy with two units, and with `build` run a whole build that passes."""
        for child in ctx.copy.iterdir():
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        (ctx.copy / "alpha.c").write_text(TWO_ALPHA)
        (ctx.copy / "beta.c").write_text(TWO_BETA)
        (ctx.copy / "symbols.ld").write_text("/* The fixture needs no external symbols. */\n")
        write(ctx, bytes(16))
        if not build:
            return None
        proc, _, _ = ctx.run()  # fails its comparison, but leaves the image: that is the baseline
        image = ctx.build / "image.bin"
        if not image.is_file() or image.stat().st_size != 16:
            return f"test setup: the seed build produced no image:\n{describe(proc)}"
        write(ctx, image.read_bytes())
        proc, _, _ = ctx.run()
        return None if passed(proc) else f"test setup: the whole build does not pass: {describe(proc)}"

    def rebuild(ctx: CacheContext, *args: str, cache: bool = True) -> subprocess.CompletedProcess:
        command = [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, "--rebuild"]
        if cache:
            command += ["--cache", str(ctx.cache)]
        return subprocess.run([*command, *args], capture_output=True, text=True)

    def snapshot(ctx: CacheContext) -> dict:
        return {p.name: p.read_bytes() for p in sorted(ctx.build.glob("unit-alpha.*"))}

    def say(proc) -> str:
        return f"exit {proc.returncode}:\n{(proc.stdout + proc.stderr)[-700:]}"

    def leftovers(ctx: CacheContext):
        return [p.name for p in ctx.build.glob(".rebuild-*")]

    def different_then_identical(ctx):
        problem = fixture(ctx)
        if problem:
            return problem
        (ctx.copy / "alpha.c").write_text(TWO_ALPHA.replace("7", "8"))
        proc = rebuild(ctx, "alpha")
        if proc.returncode != 1 or "DIFFERENT" not in proc.stdout:
            return f"a changed source must be DIFFERENT with exit 1 and no whole build: {say(proc)}"
        (ctx.copy / "alpha.c").write_text(TWO_ALPHA)
        proc = rebuild(ctx, "alpha")
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"the restored source must be IDENTICAL with exit 0: {say(proc)}"
        return None

    def no_build_directory(ctx):
        problem = fixture(ctx, build=False)
        if problem:
            return problem
        proc = rebuild(ctx, "alpha")
        if proc.returncode != 2 or "run matchbuild.py" not in proc.stdout:
            return f"without a build directory: wanted exit 2 and the advice to run matchbuild.py: {say(proc)}"
        if ctx.build.exists():
            return "a missing build directory must not be created"
        return None

    def removes_report(ctx):
        problem = fixture(ctx)
        if problem:
            return problem
        if not (ctx.build / "report.json").is_file() or not (ctx.build / "summary.txt").is_file():
            return "test setup: the whole build left no report"
        proc = rebuild(ctx, "alpha")
        if proc.returncode != 0:
            return say(proc)
        left = [n for n in ("report.json", "summary.txt") if (ctx.build / n).exists()]
        return f"after a successful --rebuild these are still there: {left}" if left else None

    def failed_keeps(ctx, damage, label: str, reason: str):
        problem = fixture(ctx)
        if problem:
            return problem
        before = snapshot(ctx)
        damage(ctx)
        proc = rebuild(ctx, "alpha")
        if proc.returncode != 1 or "RESULT: FAIL" not in proc.stdout or reason not in proc.stdout:
            return f"{label}: wanted exit 1 and {reason!r}: {say(proc)}"
        if snapshot(ctx) != before:
            return f"{label}: the previous object and listings of the unit changed"
        if not (ctx.build / "report.json").is_file() or not (ctx.build / "summary.txt").is_file():
            return f"{label}: the report was removed"
        if leftovers(ctx):
            return f"{label}: the scratch directory is left: {leftovers(ctx)}"
        return None

    def wrong_pin(ctx):
        def damage(ctx):
            text = ctx.config.read_text()
            sha = parsed["toolchain"]["cc1"]["sha256"]
            start = text.index("[toolchain.cc1]")
            at = text.index(sha, start)
            flipped = ("0" if sha[0] != "0" else "1") + sha[1:]
            ctx.config.write_text(text[:at] + flipped + text[at + len(sha):])

        return failed_keeps(ctx, damage, "a wrong cc1 pin", "cc1 sha256 mismatch")

    def syntax_error(ctx):
        return failed_keeps(ctx, lambda c: (c.copy / "alpha.c").write_text(TWO_BROKEN), "a syntax error", "compile alpha")

    def wrong_size(ctx):
        problem = fixture(ctx)
        if problem:
            return problem
        write(ctx, (ctx.build / "payload.bin").read_bytes(), beta_size=4)
        proc = rebuild(ctx, "beta")
        out = proc.stdout
        if "text size mismatch" not in out or "differing instruction slots" not in out:
            return f"the failed object check and the diff must both be printed: {say(proc)}"
        return None

    def other_unit_broken(ctx):
        problem = fixture(ctx)
        if problem:
            return problem
        (ctx.copy / "beta.c").write_text(TWO_BROKEN.replace("alpha", "beta"))
        proc = rebuild(ctx, "alpha")
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"a fault in another unit must not stop the rebuild: {say(proc)}"
        return None

    def cache_line(ctx):
        problem = fixture(ctx)
        if problem:
            return problem
        for args, cache, want in (((), True, "cache hit"), (("--no-cache",), False, "cache off")):
            proc = rebuild(ctx, *args, "alpha", cache=cache)
            if proc.returncode != 0 or f"object of unit 'alpha': {want}" not in proc.stdout:
                return f"wanted '{want}': {say(proc)}"
        (ctx.copy / "alpha.c").write_text(TWO_ALPHA.replace("7", "8"))
        proc = rebuild(ctx, "alpha")
        if "object of unit 'alpha': cache miss" not in proc.stdout:
            return f"a changed source must be a cache miss: {say(proc)}"
        return None

    def unknown_unit(ctx):
        problem = fixture(ctx, build=False)
        if problem:
            return problem
        proc = rebuild(ctx, "absent")
        return None if proc.returncode == 2 and "no unit named" in proc.stdout else say(proc)

    # Module units. The resident unit "res" covers the same addresses as "mod" and has other bytes.
    seeds = ModuleSeeds(cfg_dir, parsed)
    images = ImageFixture(parsed)

    def module_fixture(ctx: CacheContext, sources: dict | None = None):
        """A module image of one unit beside a resident unit at the same addresses, and a whole build that passes."""
        seeds.prepare()
        images.install(
            ctx.copy,
            chunk=seeds.chunks["A"],
            code=seeds.chunks["B"],
            units=[images.unit("res", FIXTURE_LOAD, 8, image=None), images.unit("mod", FIXTURE_LOAD, 8)],
            sources={"res": IMAGE_BODY.format(name="res", value=9), **(sources or {})},
        )
        proc, _, _ = ctx.run()
        return None if passed(proc) else f"test setup: the whole build does not pass: {describe(proc)}"

    def module_baseline(ctx):
        problem = module_fixture(ctx)
        if problem:
            return problem
        for unit_name in ("mod", "res"):
            proc = subprocess.run(
                [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, unit_name],
                capture_output=True, text=True,
            )
            if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
                return f"unit {unit_name!r}, after a whole build, must be IDENTICAL with exit 0: {say(proc)}"
        return None

    def module_rebuild(ctx):
        problem = module_fixture(ctx)
        if problem:
            return problem
        (ctx.copy / "mod.c").write_text(IMAGE_BODY.format(name="mod", value=8))
        proc = rebuild(ctx, "mod")
        if proc.returncode != 1 or "DIFFERENT" not in proc.stdout:
            return f"a changed module source must be DIFFERENT with exit 1: {say(proc)}"
        (ctx.copy / "mod.c").write_text(IMAGE_BODY.format(name="mod", value=7))
        proc = rebuild(ctx, "mod")
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"the restored module source must be IDENTICAL with exit 0: {say(proc)}"
        return None

    def module_syntax_error(ctx):
        problem = module_fixture(ctx)
        if problem:
            return problem
        before = {p.name: p.read_bytes() for p in sorted(ctx.build.glob("unit-mod.*"))}
        (ctx.copy / "mod.c").write_text("int mod_fn(void) { return }\n")
        proc = rebuild(ctx, "mod")
        if proc.returncode != 1 or "RESULT: FAIL" not in proc.stdout or "FAIL: image 'example': compile mod" not in proc.stdout:
            return f"a failed step of a module unit must exit 1 and name the image: {say(proc)}"
        if {p.name: p.read_bytes() for p in sorted(ctx.build.glob("unit-mod.*"))} != before:
            return "the previous object and listings of the module unit changed"
        if leftovers(ctx):
            return f"the scratch directory is left: {leftovers(ctx)}"
        return None

    def module_wrong_size(ctx):
        """A failed object check of a module unit names the image too, and the diff is still printed."""
        problem = module_fixture(ctx)
        if problem:
            return problem
        declared = f'name = "mod_fn", address = {FIXTURE_LOAD:#x}, size = '
        ctx.config.write_text(replace_once(ctx.config.read_text(), declared + "8", declared + "4", "the size of the module function"))
        proc = rebuild(ctx, "mod")
        out = proc.stdout
        if "FAIL: image 'example': unit 'mod': text size mismatch" not in out or "differing instruction slots" not in out:
            return f"the failed object check must name the image and the diff must still be printed: {say(proc)}"
        return None

    unresolved = "int absent_fn(void);\nint mod_fn(void) { return absent_fn(); }\n"

    def plain(ctx, unit_name: str) -> subprocess.CompletedProcess:
        """`fndiff.py` without `--rebuild`."""
        return subprocess.run(
            [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, unit_name],
            capture_output=True, text=True,
        )

    def names_image(proc, text: str, what: str):
        """The failure of a module unit: not exit 0, and `text` right after the image's name."""
        output = proc.stdout + proc.stderr
        if proc.returncode == 0 or f"image 'example': {text}" not in output:
            return f"{what} must fail and name the image before {text!r}: exit {proc.returncode}\n{output}"
        return None

    def module_link_fails(ctx):
        """The unit of a module image cannot be linked alone, without and with `--rebuild`."""
        seeds.prepare()
        images.install(
            ctx.copy, chunk=seeds.chunks["A"], code=seeds.chunks["B"],
            units=[images.unit("res", FIXTURE_LOAD, 8, image=None), images.unit("mod", FIXTURE_LOAD, 8)],
            sources={"res": IMAGE_BODY.format(name="res", value=9), "mod": unresolved},
        )
        ctx.run()  # it fails, after every unit has its object
        if not (ctx.build / "unit-mod.o").is_file():
            return "test setup: the whole build left no object of the module unit"
        problem = names_image(plain(ctx, "mod"), "cannot link unit 'mod' alone", "a link of the module unit alone that fails")
        if problem:
            return problem
        proc = plain(ctx, "res")
        if proc.returncode != 0 or "image '" in proc.stdout + proc.stderr:
            return f"the resident unit beside it must be compared as before: {say(proc)}"
        problem = module_fixture(ctx)
        if problem:
            return problem
        (ctx.copy / "mod.c").write_text(unresolved)
        return names_image(rebuild(ctx, "mod"), "cannot link unit 'mod' alone", "a link after a rebuild that fails")

    def module_object_missing(ctx):
        problem = module_fixture(ctx)
        if problem:
            return problem
        proc = subprocess.run(
            [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, "mod", "absent_fn"],
            capture_output=True, text=True,
        )
        problem = names_image(proc, "function 'absent_fn' is not declared", "a function that the module unit does not have")
        if problem:
            return problem
        (ctx.build / "unit-mod.o").unlink()
        return names_image(plain(ctx, "mod"), "no object ", "a module unit without an object")

    def module_publication_fails(ctx):
        """The report cannot be removed, so nothing is replaced: the failure of a module unit names the image."""
        problem = module_fixture(ctx)
        if problem:
            return problem
        (ctx.build / "report.json").unlink()
        (ctx.build / "report.json").mkdir()
        before = (ctx.build / "unit-mod.o").read_bytes()
        (ctx.copy / "mod.c").write_text(IMAGE_BODY.format(name="mod", value=8))
        problem = names_image(rebuild(ctx, "mod"), "cannot put the files of unit 'mod' into", "a publication that fails")
        if problem:
            return problem
        if (ctx.build / "unit-mod.o").read_bytes() != before:
            return "the object of the module unit was replaced although the report could not be removed"
        (ctx.copy / "res.c").write_text(IMAGE_BODY.format(name="res", value=8))
        proc = rebuild(ctx, "res")
        output = proc.stdout + proc.stderr
        if proc.returncode != 1 or "FAIL: cannot put the files of unit 'res' into" not in output or "image '" in output:
            return f"the same failure of the resident unit must read as before, without an image: {say(proc)}"
        return None

    def unbuilt(ctx, units=None, chunk="A", symbols: str | None = None, **sources):
        """The module fixture with sources that a whole build rejects. The build runs and leaves the objects."""
        seeds.prepare()
        images.install(
            ctx.copy, chunk=seeds.chunks[chunk], code=seeds.chunks["B"],
            units=units or [images.unit("res", FIXTURE_LOAD, 8, image=None), images.unit("mod", FIXTURE_LOAD, 8)],
            sources={"res": IMAGE_BODY.format(name="res", value=9), **sources},
        )
        if symbols is not None:
            (ctx.copy / "symbols.ld").write_text(symbols)
        proc, _, _ = ctx.run()
        return "test setup: the whole build must reject this fixture" if passed(proc) else None

    def refused_both_ways(ctx, unit_name: str, start: str):
        """Without and with `--rebuild`: not exit 0, and a line that starts with `start`."""
        for mode, proc in (("without --rebuild", plain(ctx, unit_name)), ("with --rebuild", rebuild(ctx, unit_name))):
            lines = (proc.stdout + proc.stderr).splitlines()
            if proc.returncode == 0 or not any(line.startswith(start) for line in lines):
                return f"{mode}: unit {unit_name!r} must be refused with a line that starts {start!r}: {say(proc)}"
            if (ctx.build / f"unit-{unit_name}.fndiff.elf").exists():
                return f"{mode}: the unit was linked although a name given to the link overrides its symbol"
        return None

    def override_cross_module(ctx):
        """A module unit defines a variable under the name of a function that the resident image declares."""
        problem = unbuilt(ctx, mod="int res_fn = 1;\nint mod_fn(void) { return res_fn; }\n")
        return problem or refused_both_ways(
            ctx, "mod", "image 'example': others.ld defines 'res_fn', which unit 'mod' defines in .data and image 'resident' declares"
        )

    def override_cross_resident(ctx):
        """The other direction: the resident unit defines a variable under the module function's name."""
        problem = unbuilt(ctx, res="int mod_fn = 1;\nint res_fn(void) { return mod_fn; }\n")
        return problem or refused_both_ways(
            ctx, "res", "others.ld defines 'mod_fn', which unit 'res' defines in .data and image 'example' declares"
        )

    def override_weak(ctx):
        """A weak definition counts like a global one. The module unit is assembly here."""
        source = (
            ".text\n.globl mod_fn\n.type mod_fn, @function\nmod_fn:\n jr $31\n nop\n.size mod_fn, .-mod_fn\n"
            ".weak res_fn\nres_fn:\n .word 0\n"
        )
        units = [images.unit("res", FIXTURE_LOAD, 8, image=None), images.unit("mod", FIXTURE_LOAD, 8, extra='kind = "asm"\n')]
        problem = unbuilt(ctx, units=units, mod=source)
        return problem or refused_both_ways(
            ctx, "mod", "image 'example': others.ld defines 'res_fn', which unit 'mod' defines in .text and image 'resident' declares"
        )

    def override_symbols(ctx):
        """The rule of `symbols.ld` holds for the link of one unit too."""
        problem = unbuilt(ctx, symbols="taken = 0x80700000;\n", mod="int taken = 1;\nint mod_fn(void) { return taken; }\n")
        return problem or refused_both_ways(ctx, "mod", "image 'example': symbols.ld defines 'taken', which unit 'mod' defines in .data")

    def override_sibling(ctx):
        """A unit defines a variable under the name of a function of another unit of its image."""
        units = [images.unit("callee", FIXTURE_LOAD, 8), images.unit("caller", FIXTURE_LOAD + 8, CALLER_SIZE)]
        problem = unbuilt(
            ctx, units=units, chunk="D", callee=IMAGE_BODY.format(name="callee", value=7),
            caller="int callee_fn = 1;\nint caller_fn(void) { return callee_fn; }\n",
        )
        return problem or refused_both_ways(
            ctx, "caller", "image 'example': unit 'caller' defines 'callee_fn' in .data, which unit 'callee' of its image defines too"
        )

    def module_calls_sibling(ctx):
        seeds.prepare()
        images.install(
            ctx.copy,
            chunk=seeds.chunks["D"],
            units=[images.unit("callee", FIXTURE_LOAD, 8), images.unit("caller", FIXTURE_LOAD + 8, CALLER_SIZE)],
            sources={name: source for name, source, _, _ in seeds.CALLS},
        )
        proc, _, _ = ctx.run()
        if not passed(proc):
            return f"test setup: the whole build does not pass: {describe(proc)}"
        proc = subprocess.run(
            [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, "caller"],
            capture_output=True, text=True,
        )
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"a module unit that calls a unit of its image must link and be IDENTICAL: {say(proc)}"
        return None

    def names_module_calls_resident(ctx):
        """The module unit that calls the resident function links alone, also after a rebuild."""
        seeds.prepare()
        calls = {name: source for name, source, _, _ in seeds.CROSS_RES + seeds.CROSS_MOD}
        images.install(
            ctx.copy,
            chunk=seeds.chunks["M"],
            code=seeds.chunks["R"],
            units=[
                images.unit("pad", FIXTURE_LOAD, 8, image=None),
                images.unit("res", FIXTURE_LOAD + 8, CALLER_SIZE, image=None),
                images.unit("mod", FIXTURE_LOAD, CALLER_SIZE),
            ],
            sources=calls,
        )
        proc, _, _ = ctx.run()
        if not passed(proc):
            return f"test setup: the whole build does not pass: {describe(proc)}"
        for unit_name in ("mod", "res"):
            proc = plain(ctx, unit_name)
            if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
                return f"unit {unit_name!r} calls a function of the other image and must be IDENTICAL: {say(proc)}"
        proc = rebuild(ctx, "mod")
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"the module unit must be IDENTICAL after --rebuild: {say(proc)}"
        return None

    def symbols_per_image(ctx):
        """The unit of each of two images that give one name of symbols.ld two addresses links alone as built."""
        seeds.prepare()
        first, second = seeds.SYM_ADDRESSES
        slot, address = 2, 0x80800000
        source = seeds.SYM[0][1]
        images.install(
            ctx.copy,
            chunk=seeds.chunks["S1"],
            images=[
                images.image(seeds.chunks["S1"]),
                images.image(
                    seeds.chunks["S2"], name="other", archive="OTHER.PAC", slot=slot, address=address,
                    symbols={"ext_fn": second},
                ),
            ],
            archives={"OTHER.PAC": bytes(make_archive([(slot, seeds.chunks["S2"])]))},
            units=[images.unit("sym", FIXTURE_LOAD, CALLER_SIZE), images.unit("sym2", address, CALLER_SIZE, image="other")],
            sources={"sym": source, "sym2": source.replace("sym_fn", "sym2_fn")},
            symbols=f"ext_fn = {first:#x};\n",
        )
        proc, _, _ = ctx.run()
        if not passed(proc):
            return f"test setup: the whole build does not pass: {describe(proc)}"
        for unit_name in ("sym", "sym2"):
            proc = plain(ctx, unit_name)
            if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
                return f"unit {unit_name!r} must be IDENTICAL with the addresses of its image: {say(proc)}"
        proc = rebuild(ctx, "sym2")
        if proc.returncode != 0 or "IDENTICAL" not in proc.stdout:
            return f"the unit of the second image must be IDENTICAL after --rebuild: {say(proc)}"
        return None

    def options_need_rebuild(ctx):
        fixture(ctx, build=False)
        proc = subprocess.run(
            [sys.executable, str(FNDIFF), "--config", str(ctx.config), "--tag", ctx.tag, "--no-cache", "alpha"],
            capture_output=True, text=True,
        )
        return None if proc.returncode == 2 and "need --rebuild" in proc.stderr else say(proc)

    return [
        CacheCase("fndiff-rebuild-different-then-identical", different_then_identical),
        CacheCase("fndiff-rebuild-no-build-directory", no_build_directory),
        CacheCase("fndiff-rebuild-removes-report", removes_report),
        CacheCase("fndiff-rebuild-wrong-cc1-pin", wrong_pin),
        CacheCase("fndiff-rebuild-syntax-error", syntax_error),
        CacheCase("fndiff-rebuild-wrong-size-still-diffs", wrong_size),
        CacheCase("fndiff-rebuild-other-unit-broken", other_unit_broken),
        CacheCase("fndiff-rebuild-cache-line", cache_line),
        CacheCase("fndiff-rebuild-unknown-unit", unknown_unit),
        CacheCase("fndiff-module-unit-baseline", module_baseline),
        CacheCase("fndiff-rebuild-module-unit", module_rebuild),
        CacheCase("fndiff-rebuild-module-syntax-error", module_syntax_error),
        CacheCase("fndiff-rebuild-module-wrong-size", module_wrong_size),
        CacheCase("fndiff-module-link-fails", module_link_fails),
        CacheCase("names-fndiff-override-in-module-unit", override_cross_module),
        CacheCase("names-fndiff-override-in-resident-unit", override_cross_resident),
        CacheCase("names-fndiff-override-by-weak-symbol", override_weak),
        CacheCase("names-fndiff-override-of-symbols-file-name", override_symbols),
        CacheCase("names-fndiff-override-of-sibling-name", override_sibling),
        CacheCase("fndiff-module-object-missing", module_object_missing),
        CacheCase("fndiff-rebuild-module-publication-fails", module_publication_fails),
        CacheCase("fndiff-module-unit-calls-sibling", module_calls_sibling),
        CacheCase("fndiff-options-need-rebuild", options_need_rebuild),
        CacheCase("symbols-fndiff-unit-in-each-image", symbols_per_image),
        CacheCase("names-fndiff-unit-calls-other-image", names_module_calls_resident),
    ]


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
    caches = select_cases(make_cache_cases(parsed) + make_fndiff_cases(cfg_dir, parsed) + make_second_map_cases(cfg_dir, parsed) + make_second_fndiff_cases(cfg_dir, parsed) + make_moved_symbol_fndiff_cases(cfg_dir, parsed), args.only)
    units = select_cases(
        make_cache_unit_cases() + make_rodata_unit_cases(parsed) + make_symbol_unit_cases(parsed)
        + make_comparison_unit_cases() + make_publish_unit_cases() + make_runner_unit_cases(config_path), args.only
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
