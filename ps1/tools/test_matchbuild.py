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
import tomllib
from pathlib import Path

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


def run_tool(config: Path, tag: str) -> subprocess.CompletedProcess:
    return subprocess.run(
        [sys.executable, str(TOOL), "--config", str(config), "--tag", tag],
        capture_output=True,
        text=True,
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
    try:
        for leftover in (copy, build):
            if leftover.exists():
                shutil.rmtree(leftover)
        shutil.copytree(cfg_dir, copy)
        if case.mutate:
            case.mutate(copy)
        proc = run_tool(copy / "build.toml", tag)
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
        for leftover in (copy, build):
            if leftover.exists():
                shutil.rmtree(leftover)


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
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
