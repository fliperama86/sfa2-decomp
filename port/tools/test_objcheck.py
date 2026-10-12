#!/usr/bin/env python3
"""Controls for objcheck.py (the comparison of the Windows objects of the runtime before and after a change), on a made-up compiler.

    python3 test_objcheck.py

No real compiler, no PsyZ, a fraction of a second. The made-up `cc` writes the text of its source file into the object and the made-up `objdump`
prints that text back, so the controls show what the tool does with what the compiler produced: `record` keeps it, `check` says that a file is the
same, says that it differs (and shows lines of the difference), refuses when nothing was recorded, and notices a file that was not recorded.
"""

from __future__ import annotations

import contextlib
import io
import os
import stat
import sys
import tempfile
from pathlib import Path
from types import SimpleNamespace

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import objcheck as O  # noqa: E402

CC = """#!/bin/sh
# made-up cc: -o OBJ SOURCE: the object is a copy of the source
while [ $# -gt 0 ]; do case "$1" in -o) out="$2"; shift 2;; -*) shift;; *) src="$1"; shift;; esac; done
cp "$src" "$out"
"""
OBJDUMP = """#!/bin/sh
# made-up objdump: prints the file it is given (the last argument)
for last; do :; done
cat "$last"
"""


def script(path: Path, text: str) -> str:
    path.write_text(text)
    path.chmod(path.stat().st_mode | stat.S_IEXEC)
    return str(path)


def main() -> int:
    failed = total = 0
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        src = root / "src"
        src.mkdir()
        (src / "a.c").write_text("int a;\n")
        (src / "b.c").write_text("int b;\n")
        O.SRC, O.PORT = src, root
        cc, objdump = script(root / "cc", CC), script(root / "objdump", OBJDUMP)

        def run(mode: str, only=()) -> tuple[int, str]:
            args = SimpleNamespace(mode=mode, name="t", only=list(only), cc=cc, objdump=objdump, psyz=root, jobs=2)
            out = io.StringIO()
            with contextlib.redirect_stdout(out):
                try:
                    status = O.run(args)
                except SystemExit as err:
                    status = 2
                    print(err)
            return status, out.getvalue()

        def case(name: str, ok: bool, why: str = "") -> None:
            nonlocal failed, total
            total += 1
            if ok:
                print(f"ok   {name}")
            else:
                failed += 1
                print(f"FAIL {name}: {why}")

        status, text = run("check")
        case("nothing-recorded-is-refused", status == 2 and "nothing is recorded" in text, text)
        status, text = run("record")
        case("record-keeps-every-file", status == 0 and "recorded 2 files" in text, text)
        status, text = run("check")
        case("an-unchanged-tree-is-the-same", status == 0 and "all 2 files are the same as recorded" in text, text)
        (src / "a.c").write_text("int a2;\n")
        status, text = run("check")
        case("a-changed-file-differs-and-shows-it", status == 1 and "DIFFERS a.c" in text and "+int a2;" in text and "1 of 2 files are the same" in text, text)
        status, text = run("check", ["b.c"])
        case("only-narrows-the-check", status == 0 and "all 1 files are the same as recorded" in text, text)
        (src / "c.c").write_text("int c;\n")
        status, text = run("check", ["c.c"])
        case("an-unrecorded-file-is-said", status == 1 and "no record of this file" in text, text)
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {total} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
