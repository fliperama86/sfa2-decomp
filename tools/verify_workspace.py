#!/usr/bin/env python3
"""Verify the migrated private pilots without depending on the old repository.

Requires the user-owned inputs and ignored workspaces; writes new verification
reports without overwriting the original measured results. No game bytes are
embedded in this script and no game/emulator UI or remote service is launched.
"""
from pathlib import Path
import hashlib
import json
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "local/migration/verification"


def run(label, args):
    result = subprocess.run(args, cwd=ROOT, text=True, capture_output=True)
    (OUT / f"{label}.log").write_text(result.stdout + result.stderr)
    if result.returncode:
        raise RuntimeError(f"{label} failed; see {OUT / (label + '.log')}")
    return result.stdout


def main():
    if not __debug__:
        raise RuntimeError("Run without -O: verification assertions must remain enabled")
    OUT.mkdir(parents=True, exist_ok=True)
    py = sys.executable
    ps1 = ROOT / "ps1/local/matching-pilot"
    windows = ROOT / "windows/local/gameplay-pilot"
    comparison = json.loads(run("ps1-byte-comparison", [
        py, str(ps1 / "scripts/compare.py"),
        str(ps1 / "build/final/metrics.elf"),
        "--out", str(OUT / "ps1-byte-comparison.json"),
    ]))
    assert len(comparison) == 3
    assert all(item["exact_at_original_address"] for item in comparison.values())

    # Independently compare the entire contiguous family, not only symbol slices.
    rebuilt = OUT / "ps1-rebuilt.text"
    run("ps1-objcopy", [
        "mipsel-linux-gnu-objcopy", "-O", "binary", "--only-section=.text",
        str(ps1 / "build/final/metrics.elf"), str(rebuilt),
    ])
    baseline = (ROOT / "ps1/local/audit/files/SLPS_004.15").read_bytes()
    offset = 2048 + 0x80142128 - 0x80118900
    assert rebuilt.read_bytes() == baseline[offset:offset + 592]
    assert len(rebuilt.read_bytes()) == 592

    run("ps1-execution", [
        py, str(ps1 / "scripts/test_metrics.py"),
        "--out", str(OUT / "ps1-execution.json"),
    ])
    ps1_execution = json.loads((OUT / "ps1-execution.json").read_text())
    assert ps1_execution["all_passed"] and ps1_execution["cases"] == 5120
    assert not ps1_execution["unexecuted_instructions"]
    assert not ps1_execution["uncovered_conditional_outcomes"]

    # Existing optimization-level builds are historical test subjects, not new
    # byte-matching claims. Full native compiler rebuilding is a separate check.
    windows_cases = {}
    for level in (0, 2):
        output = OUT / f"windows-O{level}.json"
        run(f"windows-O{level}", [
            py, str(windows / "test_metrics.py"),
            "--elf", str(windows / f"build/metrics-O{level}.elf"),
            "--cases", "1024", "--helpers", "2048", "--out", str(output),
        ])
        result = json.loads(output.read_text())
        assert result["all_passed"] and result["cases"] == 5120
        windows_cases[f"O{level}"] = result["cases"]

    report = {
        "ps1_exact_functions": len(comparison),
        "ps1_exact_bytes": 592,
        "ps1_text_sha256": hashlib.sha256(rebuilt.read_bytes()).hexdigest(),
        "ps1_execution_cases": ps1_execution["cases"],
        "windows_execution_cases_per_build": windows_cases,
        "old_repository_runtime_dependency": False,
        "whole_game_build_or_boot": False,
    }
    (OUT / "summary.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
