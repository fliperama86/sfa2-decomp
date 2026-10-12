#!/usr/bin/env python3
"""Compare what the Windows build compiles from the runtime (`port/src/*.c`) before and after a change.

    python3 objcheck.py record [--name NAME] [--only FILE ...]   # keep the code of each file as it is now
    python3 objcheck.py check  [--name NAME] [--only FILE ...]   # compile again, say which files differ (and show the first lines)

A change that makes the runtime serve a second system (the macOS build, see `port/mac/README.md`) must not change the Windows
program. This tool is the check for that: every file is compiled as hostbuild.py compiles it (`i686-w64-mingw32-gcc -O1 -Wall
-Wextra -c -I port/src`, and with PsyZ's headers for the files that use PsyZ), with no debug information, and the sections' bytes
and instructions (`objdump -d -r -s`) are kept in `port/build/objcheck/NAME/`. `check` compares them with a fresh compile. The
hash of each file is in `index.json`, the text in `FILE.txt`.

Reads only `port/src`, the PsyZ header folder (`--psyz`, default the macOS build's copy, `port/build/psyz-mac/src/psyz/include`; the
Windows build's own can be given) and runs the cross compiler. It needs no disc and runs nothing. Cost: about 10 seconds for all
files (eleven at a time); `--only` names the files that a change touched.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
SRC = PORT / "src"
PSYZ_FILES = ("gpu.c", "input.c", "iopage.c", "press.c")   # the files hostbuild.py gives -DPORT_HAVE_PSYZ and PsyZ's headers
DEFAULT_PSYZ = PORT / "build" / "psyz-mac" / "src" / "psyz" / "include"


def compile_one(cc: str, objdump: str, source: Path, psyz: Path, out: Path) -> str:
    extra = ["-DPORT_HAVE_PSYZ", "-D__psyz", "-isystem", str(psyz)] if source.name in PSYZ_FILES else []
    if source.name != "port_tables.c":
        extra.append("-DPORT_SOUND_LIBRARY_C")
    obj = out / f"{source.stem}.o"
    proc = subprocess.run([cc, "-O1", "-g0", "-fno-ident", "-Wall", "-Wextra", "-c", *extra, "-I", str(SRC), "-o", str(obj), str(source)],
                          capture_output=True, text=True)
    if proc.returncode != 0:
        raise SystemExit(f"objcheck.py: {source.name} does not compile:\n{proc.stderr[:1500]}")
    dump = subprocess.run([objdump, "-d", "-r", "-s", "-j", ".text", "-j", ".data", "-j", ".rdata", "-j", ".bss", str(obj)],
                          capture_output=True, text=True, check=True).stdout
    obj.unlink()
    return "\n".join(line for line in dump.splitlines() if not line.startswith(str(obj)) and "file format" not in line) + "\n"


def run(args: argparse.Namespace) -> int:
    base = PORT / "build" / "objcheck" / args.name
    sources = sorted(SRC / f for f in args.only) if args.only else sorted(SRC.glob("*.c"))
    for s in sources:
        if not s.is_file():
            raise SystemExit(f"objcheck.py: {s} does not exist")
    base.mkdir(parents=True, exist_ok=True)
    work = base / "work"
    work.mkdir(exist_ok=True)
    objdump = args.objdump or args.cc[:-3] + "objdump"
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        texts = list(pool.map(lambda s: compile_one(args.cc, objdump, s, args.psyz, work), sources))
    work.rmdir()
    index_path = base / "index.json"
    index = json.loads(index_path.read_text()) if index_path.exists() else {}
    if args.mode == "record":
        for s, text in zip(sources, texts):
            (base / f"{s.stem}.txt").write_text(text)
            index[s.name] = hashlib.sha256(text.encode()).hexdigest()
        index_path.write_text(json.dumps(index, indent=1, sort_keys=True) + "\n")
        print(f"recorded {len(sources)} files in {base}")
        return 0
    if not index:
        raise SystemExit(f"objcheck.py: nothing is recorded under {base}; run `record` on the unchanged tree first")
    differ = []
    for s, text in zip(sources, texts):
        old = index.get(s.name)
        if old is None:
            differ.append((s.name, "no record of this file"))
        elif old != hashlib.sha256(text.encode()).hexdigest():
            before = (base / f"{s.stem}.txt").read_text().splitlines()
            diff = list(difflib.unified_diff(before, text.splitlines(), "before", "after", lineterm="", n=1))
            differ.append((s.name, "\n".join(diff[:30])))
    for name, why in differ:
        print(f"DIFFERS {name}\n{why}")
    print(f"{len(sources) - len(differ)} of {len(sources)} files are the same as recorded" if differ else f"all {len(sources)} files are the same as recorded")
    return 1 if differ else 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("mode", choices=("record", "check"))
    parser.add_argument("--name", default="windows")
    parser.add_argument("--only", nargs="*", default=[])
    parser.add_argument("--cc", default="i686-w64-mingw32-gcc")
    parser.add_argument("--objdump", default=None)
    parser.add_argument("--psyz", type=Path, default=DEFAULT_PSYZ)
    parser.add_argument("--jobs", type=int, default=min(11, os.cpu_count() or 1))
    return run(parser.parse_args())


if __name__ == "__main__":
    sys.exit(main())
