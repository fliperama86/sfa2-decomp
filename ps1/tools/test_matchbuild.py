#!/usr/bin/env python3
"""Negative controls for matchbuild.py.

Each case builds a temporary sibling copy of the private configuration
directory (`<config dir>.selftest-<case>`, tag `selftest-<case>`), applies one
defect, and requires the tool to fail for the intended reason. The copies and
their build directories are removed afterwards.
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TOOL = Path(__file__).resolve().parent / "matchbuild.py"


class Case:
    def __init__(self, name, expect_pass, reason, mutate=None):
        self.name = name
        self.expect_pass = expect_pass
        self.reason = reason  # substring required in the tool output when it must fail
        self.mutate = mutate


def read_text(path: Path) -> str:
    return path.read_text()


def replace_once(text: str, old: str, new: str, what: str) -> str:
    if text.count(old) != 1:
        raise SystemExit(f"test setup: expected exactly one occurrence of {what}, found {text.count(old)}")
    return text.replace(old, new)


def make_cases(cfg_dir: Path, parsed: dict) -> list[Case]:
    toml_path = cfg_dir / "build.toml"
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

    def wrong_cc1_hash(copy: Path):
        path = copy / "build.toml"
        old = parsed["toolchain"]["cc1"]["sha256"]
        path.write_text(replace_once(path.read_text(), old, flip_hex(old), "cc1 sha256"))

    def wrong_baseline_hash(copy: Path):
        path = copy / "build.toml"
        old = parsed["baseline"]["sha256"]
        path.write_text(replace_once(path.read_text(), old, flip_hex(old), "baseline sha256"))

    def add_float(copy: Path):
        path = copy / target_unit["source"]
        path.write_text(path.read_text() + "\ndouble selftest_float(void) { return 1.5; }\n")

    # Only meaningful when the default compiler declares the limitation.
    float_cases = (
        [Case("float-with-no-float-compiler", False, "floating-point token", add_float)]
        if parsed["toolchain"]["cc1"].get("no_float")
        else []
    )

    return float_cases + [
        Case("clean", True, ""),
        Case("source-mutation", False, f"function '{target_unit['functions'][0]['name']}': bytes differ", mutate_source)
        if selftest["unit"] == unit["name"]
        else Case("source-mutation", False, "bytes differ", mutate_source),
        Case("wrong-size", False, "text size mismatch", wrong_size),
        Case("overlapping-units", False, "overlap", overlapping),
        Case("symbol-removed", False, "undefined reference", remove_symbol),
        Case("symbol-duplicated", False, "symbols.ld defines unit function", duplicate_symbol),
        Case("wrong-cc1-hash", False, "cc1 sha256 mismatch", wrong_cc1_hash),
        Case("wrong-baseline-hash", False, "baseline sha256 mismatch", wrong_baseline_hash),
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
        proc = subprocess.run(
            [sys.executable, str(TOOL), "--config", str(copy / "build.toml"), "--tag", tag],
            capture_output=True,
            text=True,
        )
        output = proc.stdout + proc.stderr
        if case.expect_pass:
            if proc.returncode == 0 and "RESULT: PASS" in output:
                return True, "passes as required"
            return False, f"expected success, exit {proc.returncode}:\n{output}"
        if proc.returncode == 0:
            return False, "expected failure but the tool exited 0"
        if case.reason not in output:
            return False, f"failed for the wrong reason (wanted {case.reason!r}), exit {proc.returncode}:\n{output}"
        return True, f"fails as required ({case.reason})"
    finally:
        for leftover in (copy, build):
            if leftover.exists():
                shutil.rmtree(leftover)


def main() -> int:
    parser = argparse.ArgumentParser(description="Negative controls for matchbuild.py")
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
