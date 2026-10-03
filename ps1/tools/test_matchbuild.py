#!/usr/bin/env python3
"""Controls for matchbuild.py.

Each case builds a temporary sibling copy of the private configuration
directory (`<config dir>.selftest-<case>`, tag `selftest-<case>`), applies one
change, and requires the tool to fail for the intended reason or, where stated,
to pass. The copies and their build directories are removed afterwards.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
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
                + toml_table("toolchain", parsed["toolchain"])
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
            Case("types-overlapping-field", False, "overlaps field", overlapping_field),
        ]
        if "types" in parsed
        else []
    )

    # Only meaningful when the default compiler declares the limitation.
    float_cases = (
        [Case("float-with-no-float-compiler", False, "floating-point token", add_float)]
        if parsed["toolchain"]["cc1"].get("no_float")
        else []
    )

    mutation_reason = (
        f"function '{target_unit['functions'][0]['name']}': bytes differ"
        if selftest["unit"] == unit["name"]
        else "bytes differ"
    )
    return float_cases + types_cases + [
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
                return False, f"fndiff exit {diff.returncode}, wanted {want_status} with {want_text!r}:\n{diff.stdout[-600:]}"
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
        return proc, report, report.get("cache", {}).get("units", {})

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


def main() -> int:
    parser = argparse.ArgumentParser(description="Controls for matchbuild.py")
    parser.add_argument("--config", type=Path, default=ROOT / "ps1/local/src/build.toml")
    args = parser.parse_args()
    config_path = Path(os.path.abspath(args.config))
    cfg_dir = config_path.parent
    if config_path.name != "build.toml":
        print("the configuration file must be named build.toml", file=sys.stderr)
        return 2
    parsed = tomllib.loads(config_path.read_text())
    if "selftest" not in parsed:
        print("configuration has no [selftest] section", file=sys.stderr)
        return 2

    failed = 0
    for case in make_cases(cfg_dir, parsed):
        ok, message = run_case(case, cfg_dir)
        print(f"{'ok  ' if ok else 'FAIL'} {case.name}: {message}")
        failed += not ok
    for case in make_cache_cases(parsed):
        ok, message = run_cache_case(case, cfg_dir, parsed)
        print(f"{'ok  ' if ok else 'FAIL'} {case.name}: {message}")
        failed += not ok
    for case in make_cache_unit_cases():
        ok, message = run_cache_unit_case(case)
        print(f"{'ok  ' if ok else 'FAIL'} {case.name}: {message}")
        failed += not ok
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
