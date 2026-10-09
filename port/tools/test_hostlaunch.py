#!/usr/bin/env python3
"""Controls for the runtime's launch path: the whole program, run for real.

    python3 test_hostlaunch.py --cc CROSS_CC [--run PREFIX]

The test builds the runtime (every port/src/*.c) with the cross compiler CC
and small made-up tables of its own (a handful of functions, some with C that
this file writes, some without, and the SHA-256 of an invented program), then
runs the program on disc images made here from invented bytes. Every case
goes through the real path: map, disc, program, identity, jumps, entry gate,
start. Cases (expected values are worked out here from the fixtures):

- the baseline: the right image, the entry at a registered function without C:
  the five lines, `identity:`, `jumps:`, `start:` and `stop: no C yet`, status 3;
- the entry at a registered function with C: that C runs (it prints a line),
  then `stop: main returned`, status 0;
- an image that differs from the pinned program in one byte, with an x86 `ret`
  at its entry target: refused for its identity, status 2, no `start:`, no
  `stop: main returned`, and no `identity:` line;
- the right image with an entry no table holds, and a `ret` there: refused for
  its entry, status 2, nothing executed;
- an entry inside a registered function but not at its start: refused;
- a library function without a host routine as entry: status 4.

PREFIX is a command prefix to start the Windows program (for example a
compatibility layer); without it the program is started directly (on Windows,
or from a shell under WSL, where a path is converted with wslpath). When the
program cannot be started this says so in one line and ends with status 2, as
it does without the compiler. The run's files live in a folder of its own
under port/build/ which only this run removes.
"""

from __future__ import annotations

import argparse
import hashlib
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
from test_hostrun import CNF, Image  # noqa: E402

SRC = HERE.parent / "src"
BUILD = HERE.parent / "build"
RAM = 0x80000000
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]

T_ADDR = RAM + 0x100000
T_SIZE = 0x2000
PC0 = RAM + 0x100800
B_ADDR = RAM + 0x110000
B_SIZE = 16
ENTRY_ABSENT = RAM + 0x100020     # a registered function without C
ENTRY_LIBRARY = RAM + 0x100040    # a registered library function without a host routine
ENTRY_C = RAM + 0x100080          # a registered function with C
BIG_C = RAM + 0x100100            # another one with C
UNREGISTERED = RAM + 0x101000     # in the program's text, in no table; holds an x86 ret


def jal(target: int) -> int:
    return 0x0C000000 | ((target & 0x0FFFFFFF) >> 2)


def program(entry: int, flip: tuple[int, int] | None = None) -> bytes:
    """The made-up PS-X EXE: entry code at PC0 (nop, jal ENTRY, nop, break), a ret byte
    at the unregistered address and 4 bytes into the absent function (the byte its jump overwrites)."""
    text = bytearray(T_SIZE)
    for k, w in enumerate((0, jal(entry), 0, 0x0000000D)):
        struct.pack_into("<I", text, PC0 - T_ADDR + 4 * k, w)
    text[UNREGISTERED - T_ADDR] = 0xC3
    text[ENTRY_ABSENT - T_ADDR + 4] = 0xC3
    header = bytearray(0x800)
    header[0:8] = b"PS-X EXE"
    struct.pack_into("<IIIII", header, 0x10, PC0, 0, T_ADDR, T_SIZE, 0)
    struct.pack_into("<II", header, 0x28, B_ADDR, B_SIZE)
    data = bytearray(header + text)
    if flip:
        data[flip[0]] ^= flip[1]
    return bytes(data)


TABLES = r"""
#include "port_tables.h"
#include <stdio.h>
static void c_main(void) { puts("C main ran"); fflush(stdout); }
static void c_big(void) { puts("C big ran"); fflush(stdout); }
const struct port_image port_images[] = {{ "mod", 0x80180000u, 0, 1, 0 }};
const unsigned port_image_count = 1;
const struct port_function port_functions[] = {
    { 0x%(c)08xu, (void *)c_main, "c_main", -1 },
    { 0x%(big)08xu, (void *)c_big, "c_big", -1 },
};
const unsigned port_function_count = 2;
const struct port_absent port_absents[] = {
    { 0x%(absent)08xu, "func_%(absent)08x", -1, 0 },
    { 0x%(library)08xu, "memcpy_like", -1, 1 },
};
const unsigned port_absent_count = 2;
const unsigned char port_program_sha256[32] = { %(sha)s };
"""


def sha_bytes(data: bytes) -> str:
    return ", ".join(f"0x{b:02x}" for b in hashlib.sha256(data).digest())


class Rig:
    def __init__(self, cc: str, prefix: list[str], work: Path):
        self.cc, self.prefix, self.work = cc, prefix, work
        self.wsl = not prefix and shutil.which("wslpath") is not None
        self.built: dict[str, Path] = {}

    def native(self, path: Path) -> str:
        if self.wsl:
            return subprocess.run(["wslpath", "-w", str(path)], capture_output=True, text=True, timeout=30).stdout.strip()
        return str(path)

    def start_check(self) -> str | None:
        """None when a Windows program can be started, else the reason."""
        probe = self.work / "probe.c"
        probe.write_text("int main(void) { return 7; }\n")
        out = self.work / "probe.exe"
        proc = subprocess.run([self.cc, "-o", str(out), str(probe)], capture_output=True, text=True, timeout=120)
        if proc.returncode != 0:
            return "the compiler cannot build a program: " + (proc.stderr.strip().splitlines() or ["?"])[0]
        try:
            ran = subprocess.run([*self.prefix, str(out)], capture_output=True, timeout=60)
        except (OSError, subprocess.TimeoutExpired) as err:
            return f"cannot start a Windows program: {err}"
        return None if ran.returncode == 7 else f"cannot start a Windows program (status {ran.returncode})"

    def program_for(self, pin: bytes) -> Path:
        """The runtime built with tables that pin `pin` (built once per pin)."""
        key = hashlib.sha256(pin).hexdigest()
        if key not in self.built:
            tables = self.work / f"port_tables_{key[:8]}.c"
            tables.write_text(TABLES % dict(c=ENTRY_C, big=BIG_C, absent=ENTRY_ABSENT, library=ENTRY_LIBRARY, sha=sha_bytes(pin)))
            out = self.work / f"sfa2-{key[:8]}.exe"
            sources = sorted(str(p) for p in SRC.glob("*.c"))
            proc = subprocess.run([self.cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-o", str(out), str(tables), *sources, *LINK_FLAGS],
                                  capture_output=True, text=True, timeout=300)
            if proc.returncode != 0:
                raise RuntimeError("the runtime did not build:\n" + proc.stderr.strip())
            self.built[key] = out
        return self.built[key]

    def run(self, tag: str, data: bytes, pin: bytes | None = None) -> tuple[int, list[str], Image, str]:
        """Run the program built for `pin` (default: `data` itself) on an image holding `data`."""
        exe = self.program_for(data if pin is None else pin)
        img = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": data})
        path = self.work / f"{tag}.bin"
        img.write(path)
        arg = self.native(path)
        proc = subprocess.run([*self.prefix, str(exe), arg], capture_output=True, text=True, timeout=60)
        return proc.returncode, proc.stdout.replace("\r\n", "\n").splitlines(), img, arg


def head(img: Image, arg: str) -> list[str]:
    s = img.sector["SLPS_004.15;1"]
    return [
        "memory: RAM at 0x80000000 (2 MB), scratchpad at 0x1f800000",
        f"disc: {arg}, 2352-byte sectors",
        f"program: SLPS_004.15 at sector {s}, {T_SIZE} bytes to 0x{T_ADDR:08x}, entry 0x{PC0:08x}",
        "identity: SHA-256 matches the build's baseline",
        "jumps: 2 written for functions with C, 2 for functions without",
    ]


def cases(rig: Rig):
    good = program(ENTRY_ABSENT)
    status, lines, img, arg = rig.run("baseline", good)
    want = head(img, arg) + [f"start: 0x{ENTRY_ABSENT:08x}", f"stop: no C yet for func_{ENTRY_ABSENT:08x} (0x{ENTRY_ABSENT:08x})"]
    yield "baseline-right-image-stops-at-the-function-without-c", None if (status, lines) == (3, want) else f"status {status}, lines {lines!r}, wanted {want!r}"

    status, lines, img, arg = rig.run("withc", program(ENTRY_C))
    want = head(img, arg) + [f"start: 0x{ENTRY_C:08x}", "C main ran", "stop: main returned"]
    yield "entry-at-a-function-with-c-runs-it-then-main-returned", None if (status, lines) == (0, want) else f"status {status}, lines {lines!r}, wanted {want!r}"

    status, lines, img, arg = rig.run("library", program(ENTRY_LIBRARY))
    want = head(img, arg) + [f"start: 0x{ENTRY_LIBRARY:08x}", f"stop: library function memcpy_like (0x{ENTRY_LIBRARY:08x}) has no host routine yet"]
    yield "entry-at-a-library-function-stops-with-status-4", None if (status, lines) == (4, want) else f"status {status}, lines {lines!r}, wanted {want!r}"

    # the program differs from the pinned one in one byte (the jal's
    # entry target becomes 0x80100024, in no table, and an x86 ret lies there)
    off = bytearray(good)
    off[0x800 + (PC0 - T_ADDR) + 4] ^= 1
    off = bytes(off)
    yield "wrong-image-fixture-differs-in-exactly-one-byte", None if len(off) == len(good) and sum(a != b for a, b in zip(off, good)) == 1 else "the fixture differs in more than one byte"
    yield "wrong-image-fixture-has-a-ret-at-its-entry-target", None if off[0x800 + (ENTRY_ABSENT + 4 - T_ADDR)] == 0xC3 else "no ret at the entry target"
    status, lines, img, arg = rig.run("wrong", off, pin=good)
    bad = [l for l in lines if l.startswith(("start:", "stop:", "identity:", "jumps:", "program:"))]
    yield "wrong-image-is-refused-for-its-identity", None if (status, lines[-1], bad, len(lines)) == (2, "refused: the disc's program is not the one this build is for", [], 3) else f"status {status}, lines {lines!r}"

    for tag, target in (("unregistered", UNREGISTERED), ("inside-a-function-without-c", ENTRY_ABSENT + 4), ("inside-a-function-with-c", BIG_C + 4),
                        ("just-before-a-function", ENTRY_ABSENT - 4), ("module-address", 0x80180000)):
        status, lines, img, arg = rig.run(tag, program(target))
        refusal = f"refused: the entry 0x{target:08x} is not a function this build knows"
        bad = [l for l in lines if l.startswith(("start:", "stop:")) or l.endswith("ran")]
        yield f"entry-{tag}-is-refused", None if status == 2 and lines[-1] == refusal and not bad and lines[:5] == head(img, arg) else f"status {status}, lines {lines!r}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostlaunch-", dir=BUILD))
    try:
        rig = Rig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = 0
        try:
            for name, detail in cases(rig):
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
        except Exception as err:  # a control must report, not crash
            print(f"FAIL the control itself raised {type(err).__name__}: {err}")
            failed += 1
        print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
