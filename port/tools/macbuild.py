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
PSYZ_FILES = {"gpu.c", "input.c", "iopage.c", "press.c"}
CFLAGS = ["-O1", "-g", "-Wall", "-Wextra", "-DPORT_GUEST_VM", "-I", str(SRC)]


def runtime_sources() -> list[Path]:
    """The files that make the program, in a fixed order: the runtime's (less what is left out), the platform layer, the machine."""
    files = [p for p in sorted(SRC.glob("*.c")) if p.name not in LEFT_OUT]
    return files + [MAC / "platform.c", MAC / "guest.c", MAC / "vm.c", MAC / "elf.c"]


def compile_args(source: Path, obj: Path, psyz: dict | None) -> list[str]:
    extra: list[str] = []
    if psyz and source.name in PSYZ_FILES:
        extra = ["-DPORT_HAVE_PSYZ", *(f"-D{d}" for d in psyz["define"]), "-isystem", psyz["include"]]
    return ["clang", *CFLAGS, *extra, "-c", "-o", str(obj), str(source)]


def run(args: argparse.Namespace) -> int:
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
    (out / "obj").mkdir(parents=True, exist_ok=True)
    sources = runtime_sources() + [tables]
    jobs = []
    for s in sources:
        obj = out / "obj" / (("tables_" if s == tables else "") + s.stem + ".o")
        jobs.append((s, obj))

    def compile_one(job: tuple[Path, Path]) -> str:
        s, obj = job
        proc = subprocess.run(compile_args(s, obj, psyz), capture_output=True, text=True)
        return proc.stderr if proc.returncode != 0 or proc.stderr.strip() else ""

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        results = list(pool.map(compile_one, jobs))
    bad = [(j[0].name, r) for j, r in zip(jobs, results) if r]
    for name, text in bad:
        print(f"== {name}\n{text.strip()[:1500]}", file=sys.stderr)
    if any(subprocess.run(["test", "-f", str(j[1])]).returncode for j in jobs):
        print("macbuild.py: not every file compiled", file=sys.stderr)
        return 1
    exe = out / "sfa2-mac"
    link = ["clang", "-o", str(exe), *(str(j[1]) for j in jobs), *psyz["link"], "-framework", "Hypervisor", "-lpthread"]
    proc = subprocess.run(link, capture_output=True, text=True)
    if proc.returncode != 0:
        print(proc.stderr.strip()[:3000], file=sys.stderr)
        return 1
    sign = subprocess.run(["codesign", "-s", "-", "--entitlements", str(MAC / "vm.entitlements"), "--force", str(exe)], capture_output=True, text=True)
    if sign.returncode != 0:
        print(sign.stderr.strip(), file=sys.stderr)
        return 1
    shutil.copyfile(elf, out / "guest.elf")
    print(f"built: {exe} ({len(jobs)} files), guest image {out / 'guest.elf'}")
    return 1 if bad else 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--guest", type=Path, default=PORT / "build" / "guest")
    parser.add_argument("--psyz", type=Path, default=PORT / "build" / "psyz-mac")
    parser.add_argument("--out", type=Path, default=PORT / "build" / "mac")
    parser.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 1))
    return run(parser.parse_args())


if __name__ == "__main__":
    sys.exit(main())
