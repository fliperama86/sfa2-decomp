#!/usr/bin/env python3
"""Control for hostcheck.py on any system: a compiler that does not end is stopped.

`test_hostcheck.py` has this case with a stand-in that is a script with a
`#!` line, which Windows cannot start. This one builds the stand-in for
the system it runs on, a `.cmd` file in front of the script on Windows,
so that the way `hostcheck.py` ends a compiler and what it started is run
there too. The stand-in gives its version at once and then sleeps for 60
seconds on every other call; with `--timeout 2` the tool must give up on
the pointer size, with status 2 and one line that names the compiler,
long before the stand-in would have ended.
"""

from __future__ import annotations

import os
import subprocess
import sys
import tempfile
import time
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
SCRIPT = TOOLS / "hostcheck.py"
SLEEP = 60
LIMIT = 45

SLOW = """import sys, time
if sys.argv[1:] == ["--version"]:
    print("slowcc 1.0")
    sys.exit(0)
time.sleep({sleep})
"""


def stand_in(root: Path) -> Path:
    body = root / "slowcc.py"
    body.write_text(SLOW.format(sleep=SLEEP))
    if os.name == "nt":
        front = root / "slowcc.cmd"
        front.write_text(f'@"{sys.executable}" "{body}" %*\n')
        return front
    front = root / "slowcc"
    front.write_text(f"#!/bin/sh\nexec '{sys.executable}' '{body}' \"$@\"\n")
    front.chmod(0o755)
    return front


def cases(root: Path):
    (root / "t.fields").write_text("struct One size=0x4\n0x000 u32 a\n")
    (root / "a.c").write_text("int a(void) { return 1; }\n")
    config = root / "build.toml"
    config.write_text('[types]\nfields = "t.fields"\nheader = "t.h"\n\n[[unit]]\nname = "a"\nsource = "a.c"\n')
    cc = stand_in(root)
    argv = [sys.executable, str(SCRIPT), "--config", str(config), "--build", str(root / "build"), "--cc", str(cc), "--timeout", "2"]
    started = time.monotonic()
    try:
        proc = subprocess.run(argv, capture_output=True, text=True, timeout=SLEEP * 4)
    except subprocess.TimeoutExpired:
        yield "stop-ends-the-run", f"the tool had not ended after {SLEEP * 4} seconds"
        return
    took = time.monotonic() - started
    err = proc.stderr.strip()
    yield "stop-ends-the-run", None if took < LIMIT else f"the run took {took:.0f} seconds, the limit is {LIMIT}"
    yield "stop-status-2", None if proc.returncode == 2 else f"exit {proc.returncode}, stderr {err!r}"
    yield "stop-one-line-naming-the-compiler", None if len(err.splitlines()) == 1 and cc.name in err else f"stderr {err!r}"
    yield "stop-nothing-on-standard-output", None if proc.stdout == "" else f"stdout {proc.stdout!r}"


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for name, detail in cases(Path(tmp)):
            print(f"{'ok  ' if detail is None else 'FAIL'} {name}" + ("" if detail is None else f": {detail}"))
            failed += detail is not None
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
