#!/usr/bin/env python3
"""Build the macOS program of the port: the runtime of `port/src` and `port/mac`, PsyZ and SDL, and the tables of the guest image.

    python3 macbuild.py [--guest DIR] [--psyz DIR] [--out DIR] [--jobs N]

Needs, built before: the guest image (`python3 port/tools/hostbuild.py --guest`, default folder `port/build/guest`: `guest.elf` and
`gen/guest_tables.c`) and PsyZ for this Mac (`python3 port/tools/psyzbuild.py --cc clang --build port/build/psyz-mac`, whose `psyz.json` this
reads). Compiles every runtime file with the system's clang for this Mac (`-DPORT_GUEST_VM`, see `port/mac/README.md`), links one program
`sfa2-mac`, signs it ad hoc with the hypervisor entitlement and copies `guest.elf` beside it. Reads no game file, runs nothing. Cost: about 20
seconds, most of it the compiler. The program is started as `sfa2-mac DISC` (a .cue or the .bin itself) from its own folder, or with `--guest FILE`.

The runtime files that are Windows' only or x86's only are left out: `mirrorcore.c` (an x86 instruction decoder). Every other file of
`port/src` is compiled; where it has Windows-only code it is inside `#ifdef _WIN32`, and the macOS build's own code is in `port/mac`.
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
SRC = PORT / "src"
MAC = PORT / "mac"
LEFT_OUT = {"mirrorcore.c"}
PSYZ_FILES = {"gpu.c", "input.c", "iopage.c", "press.c", "settings.c"}   # settings.c: its graphics defaults are on with PsyZ
CFLAGS = ["-O1", "-g", "-Wall", "-Wextra", "-DPORT_GUEST_VM", "-I", str(SRC)]


def runtime_sources() -> list[Path]:
    """The files that make the program, in a fixed order: the runtime's (less what is left out), the platform layer, the machine."""
    files = [p for p in sorted(SRC.glob("*.c")) if p.name not in LEFT_OUT]
    return files + sorted((PORT / "intro").glob("*.c")) + [MAC / "platform.c", MAC / "guest.c", MAC / "vm.c", MAC / "elf.c"]


def compile_args(source: Path, obj: Path, psyz: dict | None, sound: bool = False) -> list[str]:
    extra: list[str] = ["-DPORT_SOUND_LIBRARY_C"] if sound and source.name != "port_tables.c" and not source.name.startswith("guest_tables") else []
    if psyz and source.name in PSYZ_FILES:
        extra += ["-DPORT_HAVE_PSYZ", *(f"-D{d}" for d in psyz["define"]), "-isystem", psyz["include"]]
    return ["clang", *CFLAGS, *extra, "-c", "-o", str(obj), str(source)]


def build_objects(jobs: list[tuple[Path, Path]], psyz: dict | None, sound: bool, pool_size: int, runner=subprocess.run) -> tuple[list[tuple[str, int, str]], list[tuple[str, str]]]:
    """Compile every job, each object made new: an object left by an earlier run is deleted first, so that none can stand in for one that did not
    build. Returns (failures, warnings): a failure is a job whose compiler did not end with status 0 (whatever it printed, or nothing: a compiler
    killed by a signal prints nothing), as (name, status, diagnostics); a warning is a job that ended with 0 and printed something."""
    def compile_one(job: tuple[Path, Path]) -> tuple[int, str]:
        s, obj = job
        obj.unlink(missing_ok=True)
        proc = runner(compile_args(s, obj, psyz, sound), capture_output=True, text=True)
        return proc.returncode, proc.stderr or ""

    with ThreadPoolExecutor(max_workers=pool_size) as pool:
        results = list(pool.map(compile_one, jobs))
    failures = [(j[0].name, rc, text) for j, (rc, text) in zip(jobs, results) if rc != 0]
    warnings = [(j[0].name, text) for j, (rc, text) in zip(jobs, results) if rc == 0 and text.strip()]
    return failures, warnings


def run(args: argparse.Namespace, runner=subprocess.run) -> int:
    guest: Path = args.guest
    tables, elf = guest / "gen" / "guest_tables.c", guest / "guest.elf"
    for need in (tables, elf):
        if not need.is_file():
            print(f"macbuild.py: {need} does not exist; run `python3 port/tools/hostbuild.py --guest` first", file=sys.stderr)
            return 2
    try:
        psyz = json.loads((args.psyz / "psyz.json").read_text())
    except (OSError, ValueError) as err:
        print(f"macbuild.py: cannot read {args.psyz / 'psyz.json'}: {err}; run `python3 port/tools/psyzbuild.py --cc clang --build {args.psyz}` first", file=sys.stderr)
        return 2
    out: Path = args.out
    sound = (guest / "gen" / "sound_library").is_file()   # hostbuild.py --guest --psyz made the guest with the game's sound library
    (out / "obj").mkdir(parents=True, exist_ok=True)
    exe = out / "sfa2-mac"
    exe.unlink(missing_ok=True)   # nothing of an earlier run stays to look like this one's result
    (out / "guest.elf").unlink(missing_ok=True)
    jobs = [(s, out / "obj" / (("tables_" if s == tables else "") + s.stem + ".o")) for s in runtime_sources() + [tables]]
    failures, warnings = build_objects(jobs, psyz, sound, args.jobs, runner)
    for name, text in warnings:
        print(f"== {name}\n{text.strip()[:1500]}", file=sys.stderr)
    if failures:
        for name, status, text in failures:
            print(f"== {name}: the compiler ended with status {status}\n{text.strip()[:1500]}", file=sys.stderr)
        print(f"macbuild.py: {len(failures)} of {len(jobs)} files did not compile; nothing was linked", file=sys.stderr)
        return 1
    link = ["clang", "-o", str(exe), *(str(j[1]) for j in jobs), *psyz["link"], "-framework", "Hypervisor", "-lpthread"]
    proc = runner(link, capture_output=True, text=True)
    if proc.returncode != 0:
        print(proc.stderr.strip()[:3000] or f"macbuild.py: the link ended with status {proc.returncode}", file=sys.stderr)
        exe.unlink(missing_ok=True)
        return 1
    sign = runner(["codesign", "-s", "-", "--entitlements", str(MAC / "vm.entitlements"), "--force", str(exe)], capture_output=True, text=True)
    if sign.returncode != 0:
        print(sign.stderr.strip() or f"macbuild.py: codesign ended with status {sign.returncode}", file=sys.stderr)
        exe.unlink(missing_ok=True)
        return 1
    shutil.copyfile(elf, out / "guest.elf")
    print(f"built: {exe} ({len(jobs)} files{', with the game\'s sound library' if sound else ''}), guest image {out / 'guest.elf'}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--guest", type=Path, default=PORT / "build" / "guest")
    parser.add_argument("--psyz", type=Path, default=PORT / "build" / "psyz-mac")
    parser.add_argument("--out", type=Path, default=PORT / "build" / "mac")
    parser.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 1))
    return run(parser.parse_args())


if __name__ == "__main__":
    sys.exit(main())
