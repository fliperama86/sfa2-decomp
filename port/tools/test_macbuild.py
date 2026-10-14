#!/usr/bin/env python3
"""Controls for macbuild.py (the macOS program's build) and the Mac files of the port, on made-up compiler runs.

    python3 test_macbuild.py

No real compile, no disc, a fraction of a second. A stand-in for the compiler and the linker (the `runner` that macbuild.py takes) says what each
command did; the controls show that a build never goes on from a compile that did not succeed: an object left by an earlier run is not taken for
this one's, a compiler that ends with a bad status and prints nothing (killed by a signal) fails the build before the link, warnings are shown
and do not fail it, a failed link or signature leaves no program behind, and the files are the ones that should make the program.
On macOS with Apple Silicon one more case compiles every Mac file for syntax with the system's clang; elsewhere it is not run and the last line
says how many cases ran. Every case prints `ok NAME` or `FAIL NAME: why`.
"""

from __future__ import annotations

import argparse
import contextlib
import io
import json
import platform
import subprocess
import sys
import tempfile
from pathlib import Path
from types import SimpleNamespace

TOOLS = Path(__file__).resolve().parent
PORT = TOOLS.parent
sys.path.insert(0, str(TOOLS))

import macbuild as M  # noqa: E402


class Runner:
    """Stands in for subprocess.run: `plan(argv)` gives (status, stderr); a compile makes its object file when the status is 0."""

    def __init__(self, plan):
        self.plan, self.calls = plan, []

    def __call__(self, argv, capture_output=True, text=True):
        self.calls.append(list(argv))
        status, stderr = self.plan(argv)
        if status == 0 and "-c" in argv:
            Path(argv[argv.index("-o") + 1]).write_bytes(b"new")
        if status == 0 and argv[0] == "clang" and "-c" not in argv:
            Path(argv[argv.index("-o") + 1]).write_bytes(b"program")
        return SimpleNamespace(returncode=status, stderr=stderr, stdout="")


def rig(root: Path, sound: bool = False) -> SimpleNamespace:
    guest, psyz, out = root / "guest", root / "psyz", root / "out"
    (guest / "gen").mkdir(parents=True)
    (guest / "gen" / "guest_tables.c").write_text("")
    (guest / "guest.elf").write_bytes(b"elf")
    if sound:
        (guest / "gen" / "sound_library").write_text("")
    psyz.mkdir()
    (psyz / "psyz.json").write_text(json.dumps({"include": str(psyz), "define": ["__psyz"], "link": ["-lm"]}))
    return SimpleNamespace(guest=guest, psyz=psyz, out=out, jobs=2)


def case_stale_object_with_a_silent_bad_status():
    with tempfile.TemporaryDirectory() as tmp:
        args = rig(Path(tmp))
        stale = args.out / "obj" / "kernel.o"
        stale.parent.mkdir(parents=True)
        stale.write_bytes(b"stale")
        (args.out / "sfa2-mac").write_bytes(b"old program")
        runner = Runner(lambda argv: (-9, "") if argv[-1].endswith("kernel.c") else (0, ""))
        status = M.run(args, runner)
        assert status == 1, f"status {status}"
        assert not stale.exists(), "the stale object is still there"
        assert not (args.out / "sfa2-mac").exists(), "the old program is still there"
        assert not [c for c in runner.calls if "-framework" in c or c[0] == "codesign"], "linked or signed after a failed compile"


def case_a_failed_compile_with_text_is_a_failure():
    with tempfile.TemporaryDirectory() as tmp:
        args = rig(Path(tmp))
        runner = Runner(lambda argv: (1, "error: nope") if argv[-1].endswith("gpu.c") else (0, ""))
        assert M.run(args, runner) == 1
        assert not [c for c in runner.calls if "-framework" in c]


def case_warnings_do_not_fail_the_build():
    with tempfile.TemporaryDirectory() as tmp:
        args = rig(Path(tmp))
        runner = Runner(lambda argv: (0, "warning: unused") if argv[-1].endswith("cd.c") else (0, ""))
        assert M.run(args, runner) == 0
        assert (args.out / "sfa2-mac").is_file() and (args.out / "guest.elf").read_bytes() == b"elf"
        assert any(c[0] == "codesign" for c in runner.calls)


def case_a_failed_link_or_signature_leaves_no_program():
    for bad in ("link", "sign"):
        with tempfile.TemporaryDirectory() as tmp:
            args = rig(Path(tmp))
            def plan(argv, bad=bad):
                if bad == "link" and "-framework" in argv:
                    return 1, ""
                if bad == "sign" and argv[0] == "codesign":
                    return 1, ""
                return 0, ""
            assert M.run(args, Runner(plan)) == 1, bad
            assert not (args.out / "sfa2-mac").exists(), bad
            assert not (args.out / "guest.elf").exists(), bad


def case_the_files_and_flags():
    names = [p.name for p in M.runtime_sources()]
    assert "mirrorcore.c" not in names and "kernel.c" in names and names[-4:] == ["platform.c", "guest.c", "vm.c", "elf.c"], names
    assert all(name in names for name in ("intro.c", "scene.c", "sequence.c")), names
    psyz = {"include": "/inc", "define": ["__psyz"]}
    gpu = M.compile_args(PORT / "src" / "gpu.c", Path("g.o"), psyz, True)
    assert "-DPORT_HAVE_PSYZ" in gpu and "-D__psyz" in gpu and "-DPORT_SOUND_LIBRARY_C" in gpu and "-DPORT_GUEST_VM" in gpu, gpu
    kernel = M.compile_args(PORT / "src" / "kernel.c", Path("k.o"), psyz, False)
    assert "-DPORT_HAVE_PSYZ" not in kernel and "-DPORT_SOUND_LIBRARY_C" not in kernel, kernel
    tables = M.compile_args(Path("guest_tables.c"), Path("t.o"), psyz, True)
    assert "-DPORT_SOUND_LIBRARY_C" not in tables, tables


def case_the_mac_files_compile_for_syntax():
    for name in ("platform.c", "guest.c", "vm.c", "elf.c"):
        proc = subprocess.run(["clang", "-target", "arm64-apple-macos", "-fsyntax-only", "-Wall", "-Wextra", "-Werror", "-DPORT_GUEST_VM", "-I", str(PORT / "src"),
                               str(PORT / "mac" / name)], capture_output=True, text=True)
        assert proc.returncode == 0, f"{name}: {proc.stderr.strip()[:600]}"


def main() -> int:
    argparse.ArgumentParser().parse_args()
    cases = [case_stale_object_with_a_silent_bad_status, case_a_failed_compile_with_text_is_a_failure, case_warnings_do_not_fail_the_build,
             case_a_failed_link_or_signature_leaves_no_program, case_the_files_and_flags]
    if platform.system() == "Darwin" and platform.machine() == "arm64":
        cases.append(case_the_mac_files_compile_for_syntax)
    else:
        print("the syntax check of the Mac files needs macOS with Apple Silicon: not run")
    failed = 0
    for fn in cases:
        name = fn.__name__.replace("case_", "").replace("_", "-")
        try:
            with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):   # what macbuild.py says about its made-up failures
                fn()
        except Exception as err:
            failed += 1
            print(f"FAIL {name}: {type(err).__name__}: {err}")
        else:
            print(f"ok   {name}")
    ran = len(cases)
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {ran} cases that ran behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
