#!/usr/bin/env python3
"""Controls for verify_cc1_golden.py.

Uses a stand-in compiler that copies its input to its output, and synthetic
golden directories in a temporary location. No real compiler or game data.
"""

from __future__ import annotations

import json
import subprocess
import sys
import tempfile
from pathlib import Path

TOOL = Path(__file__).resolve().parent / "verify_cc1_golden.py"

STAND_IN = """#!/bin/sh
# Stand-in compiler: copies the input file to the -o output file.
while [ $# -gt 0 ]; do
  case "$1" in
    -o) out="$2"; shift 2 ;;
    -*) shift ;;
    *) src="$1"; shift ;;
  esac
done
cp "$src" "$out"
"""


def add_case(golden: Path, name: str, *, compiler="263", source="body\n", expected="body\n"):
    """Create one golden directory. Pass None to leave an artifact out."""
    directory = golden / name
    directory.mkdir(parents=True)
    (directory / "result.json").write_text(json.dumps({"compiler": compiler, "flags": ["-quiet"]}))
    if source is not None:
        (directory / "metrics.i").write_text(source)
    if expected is not None:
        (directory / "metrics.s").write_text(expected)


def scenarios():
    """Yield (name, setup, expected exit status, required output substring)."""

    def complete(golden):
        add_case(golden, "a")
        add_case(golden, "b")

    def missing_output(golden):
        add_case(golden, "a")
        add_case(golden, "b", expected=None)

    def missing_input(golden):
        add_case(golden, "a")
        add_case(golden, "b", source=None)

    def different(golden):
        add_case(golden, "a")
        add_case(golden, "b", expected="other\n")

    def none_selected(golden):
        add_case(golden, "a", compiler="other")

    def unselected_incomplete(golden):
        # An incomplete case for another compiler is not selected and must not fail the run.
        add_case(golden, "a")
        add_case(golden, "b", compiler="other", expected=None)

    yield "complete", complete, 0, "2/2 identical"
    yield "missing-output", missing_output, 1, "missing metrics.s"
    yield "missing-input", missing_input, 1, "missing metrics.i"
    yield "different-output", different, 1, "DIFF b"
    yield "none-selected", none_selected, 1, "0/0 identical"
    yield "unselected-incomplete", unselected_incomplete, 0, "1/1 identical"


def main() -> int:
    failed = 0
    for name, setup, want_status, want_text in scenarios():
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            cc1 = root / "cc1"
            cc1.write_text(STAND_IN)
            cc1.chmod(0o755)
            golden = root / "golden"
            golden.mkdir()
            setup(golden)
            proc = subprocess.run(
                [sys.executable, str(TOOL), "--cc1", str(cc1), "--golden", str(golden)],
                capture_output=True,
                text=True,
            )
        output = proc.stdout + proc.stderr
        ok = proc.returncode == want_status and want_text in output
        print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {proc.returncode}, wanted {want_status} with {want_text!r}")
        if not ok:
            print(output)
            failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
