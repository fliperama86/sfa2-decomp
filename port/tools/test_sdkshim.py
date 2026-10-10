#!/usr/bin/env python3
"""Control for the one unit of the sound library whose assembly differs under the shim: libspu's s_sca.c (SpuSetCommonAttr).

    python3 test_sdkshim.py --cc CROSS_CC --psyz-build DIR [--run PREFIX]

DIR is a build folder of psyzbuild.py (it holds psyz.json, which names PsyZ's include folder). --run is a command
prefix that starts a Windows program, as for the other test_*.py files (empty: the program is started directly).

Why. PsyZ declares the two halves of SpuVolume as `short`. The library C was written for `unsigned short`. With the
shim (port/sdkshim) and PsyZ's folder s_sca.c compiles, and 66 of the 67 units of sdk/libsnd and sdk/libspu give the
same assembly as with the headers the matching work uses. s_sca.c gives different assembly: it reads fields of
SpuVolume, and the compiler schedules the comparisons differently for a signed field. By reading the unit, every such
read is stored into a 16-bit variable or register, or cast to s16 before it is compared or switched on, so the
field's own signedness should not matter. This file tests that.

What it does. It builds one small program from s_sca.c compiled twice with the shim and PsyZ's include folder: variant
S as it is, variant U with a temporary copy of PsyZ's libspu.h, put first on the include path, in which exactly the two
field lines of SpuVolume read `unsigned short` (the test makes the copy and refuses if it does not find exactly
those two lines). The two functions get other names (-D), and each works on its own block of plain memory through its
own _spu_RXX. The program fills the two blocks with the same random bytes, calls both with the same attribute record,
and compares the blocks byte for byte after every call. Inputs: mask 0, each single bit of the twelve bits the unit
tests, random masks; volume modes -2 to 9 and random 16-bit values; volumes 0, 1, 0x7f, 0x80, 0x7fff, 0x8000, 0xffff
and random ones for both halves; random CD and external volumes; the four reverb and mix flags 0, 1 and random. It
prints the number of calls and `different N`; `different 0` is required.

Negative control. Variant U built from a scratch copy of s_sca.c in which one `(s16)` cast before a comparison of a
volume is removed must differ from variant S; the case passes only if it does. Without it the test shows nothing.

How a run is read. A run of the test program counts only when the program ended with status 0, printed exactly one
result line, and that line has the number of calls this file asked for. A result line from a program that ended with
another status is not a result, for the test and for the negative control alike. The `result:` cases show this on
made-up outputs, with no compiler and no program.

What it does not show. It tests one function, on the inputs above, with plain memory in place of the chip's registers.
It does not compare with a build that uses the headers the repository does not carry (the repository cannot do that;
the 66-of-67 comparison was made once, privately). It is not a proof for inputs outside those it draws.
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
sys.path.insert(0, str(HERE))
import hostbuild  # noqa: E402

SHIM = REPO / "port/sdkshim"
UNIT = REPO / "ps1/src/sdk/libspu/s_sca.c"
UNIT_DIR = UNIT.parent
CALLS = 150000
LINES = ("    short left;  /**< Left channel value */", "    short right; /**< Right channel value */")


class Refusal(Exception):
    pass


def psyz_include(folder: Path) -> Path:
    """PsyZ's include folder, from the build folder's psyz.json."""
    info = folder / "psyz.json"
    if not info.is_file():
        raise Refusal(f"{info} does not exist; build PsyZ first with port/tools/psyzbuild.py")
    inc = Path(json.loads(info.read_text())["include"])
    if not (inc / "libspu.h").is_file():
        raise Refusal(f"PsyZ's include folder {inc} has no libspu.h")
    return inc


def unsigned_copy(inc: Path, dest: Path) -> Path:
    """A folder with a copy of PsyZ's libspu.h in which the two SpuVolume field lines read unsigned short."""
    text = (inc / "libspu.h").read_text()
    for line in LINES:
        if text.count(line) != 1:
            raise Refusal(f"libspu.h has {text.count(line)} copies of the line {line.strip()!r}, wanted exactly 1")
    for line in LINES:
        text = text.replace(line, line.replace("short", "unsigned short", 1))
    dest.mkdir(parents=True, exist_ok=True)
    (dest / "libspu.h").write_text(text)
    return dest


HARNESS = r"""
#include "common.h"
#include "libspu_internal.h"
#include <stdio.h>
#include <string.h>

void sca_S(SpuCommonAttr *);
void sca_U(SpuCommonAttr *);
union SpuUnion *rxx_S, *rxx_U;
static union SpuUnion blk_S, blk_U;

static unsigned long long state = 0x9e3779b97f4a7c15ULL;
static unsigned rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (unsigned)(state >> 16); }
static const unsigned short edge[] = {0, 1, 0x7f, 0x80, 0x7fff, 0x8000, 0xffff};
static unsigned short vol(void)
{
    unsigned k = rnd() % 10;
    return k < 6 ? edge[rnd() % 7] : (unsigned short)rnd();
}
static unsigned short mode(void)
{
    unsigned k = rnd() % 10;
    return k < 7 ? (unsigned short)(int)(rnd() % 12 - 2) : (unsigned short)rnd();
}
static unsigned long flag(void)
{
    unsigned k = rnd() % 8;
    return k < 3 ? 0 : k < 6 ? 1 : rnd();
}
static const unsigned bits[12] = {0, 1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13};

int main(void)
{
    long calls = %CALLS%, different = 0, i;
    rxx_S = &blk_S;
    rxx_U = &blk_U;
    for (i = 0; i < calls; i++) {
        SpuCommonAttr a;
        unsigned char *p = (unsigned char *)&blk_S;
        unsigned k = rnd() % 10;
        size_t n;
        for (n = 0; n < sizeof blk_S; n++) p[n] = (unsigned char)rnd();
        memcpy(&blk_U, &blk_S, sizeof blk_S);
        memset(&a, 0, sizeof a);
        a.mask = k == 0 ? 0 : k < 5 ? 1ul << bits[rnd() % 12] : (unsigned long)(rnd() & 0x3fff);
        a.mvol.left = vol(); a.mvol.right = vol();
        a.mvolmode.left = mode(); a.mvolmode.right = mode();
        a.mvolx.left = vol(); a.mvolx.right = vol();
        a.cd.volume.left = vol(); a.cd.volume.right = vol();
        a.ext.volume.left = vol(); a.ext.volume.right = vol();
        a.cd.reverb = flag(); a.cd.mix = flag(); a.ext.reverb = flag(); a.ext.mix = flag();
        sca_S(&a);
        sca_U(&a);
        if (memcmp(&blk_S, &blk_U, sizeof blk_S) != 0) different++;
    }
    printf("calls %ld different %ld\n", calls, different);
    return 0;
}
"""


class Rig:
    def __init__(self, cc: str, prefix: list[str], inc: Path, work: Path) -> None:
        self.cc, self.prefix, self.inc, self.work = cc, prefix, inc, work
        self.ucopy = unsigned_copy(inc, work / "u")

    def compile(self, out: Path, source: Path, defs: list[str], first: list[Path]) -> str | None:
        cmd = [self.cc, *hostbuild.COMPILE_FLAGS, "-c", *defs, *sum((["-I", str(d)] for d in first), []),
               "-I", str(UNIT_DIR), "-I", str(SHIM), "-I", str(self.inc), "-o", str(out), str(source)]
        proc = subprocess.run(cmd, capture_output=True, text=True, timeout=300)
        return None if proc.returncode == 0 else proc.stderr.strip().splitlines()[-1] if proc.stderr.strip() else "no message"

    def program(self, tag: str, unit_u: Path) -> tuple[Path | None, str]:
        w = self.work / tag
        w.mkdir()
        objs = {}
        for v, src, first in (("S", UNIT, []), ("U", unit_u, [self.ucopy])):
            obj = w / f"{v}.o"
            problem = self.compile(obj, src, [f"-DSpuSetCommonAttr=sca_{v}", f"-D_spu_RXX=rxx_{v}"], first)
            if problem:
                return None, f"variant {v} does not compile: {problem}"
            objs[v] = obj
        h = w / "harness.c"
        h.write_text(HARNESS.replace("%CALLS%", str(CALLS)))
        hobj = w / "harness.o"
        problem = self.compile(hobj, h, [], [])
        if problem:
            return None, f"harness does not compile: {problem}"
        exe = w / "t.exe"
        proc = subprocess.run([self.cc, "-o", str(exe), str(hobj), str(objs["S"]), str(objs["U"])], capture_output=True, text=True, timeout=300)
        if proc.returncode != 0:
            return None, f"link: {proc.stderr.strip()[-200:]}"
        return exe, ""

    def run(self, exe: Path) -> tuple[int, int] | str:
        try:
            proc = subprocess.run([*self.prefix, str(exe)], capture_output=True, text=True, timeout=120)
        except subprocess.TimeoutExpired:
            return "the test program had not ended after 120 seconds"
        return result_of(proc.returncode, proc.stdout, proc.stderr)


def result_of(status: int, stdout: str, stderr: str) -> tuple[int, int] | str:
    """What a run of the test program showed: (calls, different), or the reason it showed nothing.

    A run counts only when the program ended with status 0, printed exactly one result line and made the number of
    calls this file asked for. A result line from a program that ended otherwise is not a result."""
    if status != 0:
        return f"the test program ended with status {status}, not 0: {stdout[-100:]!r} {stderr[-100:]!r}"
    found = re.findall(r"^calls (\d+) different (\d+)$", stdout, re.M)
    if len(found) != 1:
        return f"{len(found)} result lines, wanted 1: {stdout[-100:]!r} {stderr[-100:]!r}"
    calls, different = int(found[0][0]), int(found[0][1])
    if calls != CALLS:
        return f"the test program made {calls} calls, not {CALLS}"
    return calls, different


def result_cases():
    """The reading of a run, on made-up outputs: no compiler and no program is needed. Expected values written out."""
    line = f"calls {CALLS} different 0\n"
    yield "result: status 0 and one line is a result", same(result_of(0, line, ""), (150000, 0))
    yield "result: differences are counted", same(result_of(0, f"calls {CALLS} different 5186\n", ""), (150000, 5186))
    for name, got in (
        ("result: status 1 with a valid line is no result", result_of(1, line, "")),
        ("result: status 1 with differences is no result", result_of(1, f"calls {CALLS} different 5186\n", "")),
        ("result: a signal's status is no result", result_of(-11, line, "")),
        ("result: no line is no result", result_of(0, "", "")),
        ("result: two lines are no result", result_of(0, line + line, "")),
        ("result: fewer calls than asked is no result", result_of(0, "calls 10 different 0\n", "")),
        ("result: a line inside other text is no result", result_of(0, f"xcalls {CALLS} different 0 y\n", "")),
    ):
        yield name, None if isinstance(got, str) else f"taken as the result {got}"


def same(got, want) -> str | None:
    return None if got == want else f"wanted {want}, got {got}"


def cases(rig: Rig, inc: Path):
    # 0. how a run is read
    yield from result_cases()
    # 1. the unit as it is: both variants give the same blocks
    exe, why = rig.program("plain", UNIT)
    if not exe:
        yield "unit, signed and unsigned SpuVolume", why
    else:
        got = rig.run(exe)
        if isinstance(got, str):
            yield "unit, signed and unsigned SpuVolume", got
        else:
            print(f"     calls {got[0]}, different {got[1]}")
            yield "unit, signed and unsigned SpuVolume", None if got[1] == 0 else f"calls {got[0]}, different {got[1]}"
    # 2. negative control: one (s16) cast of a volume comparison removed
    src = UNIT.read_text()
    old = "if ((s16)attr->mvol.left >= 0x80) {"
    if src.count(old) != 1:
        yield "negative control", f"the line {old!r} is not found exactly once in the unit"
        return
    mut = rig.work / "s_sca_mut.c"
    mut.write_text(src.replace(old, "if (attr->mvol.left >= 0x80) {"))
    exe, why = rig.program("mutant", mut)
    if not exe:
        yield "negative control: one (s16) cast removed", why
    else:
        got = rig.run(exe)
        if isinstance(got, str):
            yield "negative control: one (s16) cast removed", got
        else:
            print(f"     calls {got[0]}, different {got[1]}")
            yield "negative control: one (s16) cast removed", None if got[1] > 0 else "no difference found, so the test would not notice a signedness dependence"
    # 3. refusals
    try:
        psyz_include(rig.work / "missing")
        yield "refusal: PsyZ's folder missing", "no refusal"
    except Refusal as err:
        yield "refusal: PsyZ's folder missing", None if "does not exist" in str(err) else str(err)
    fake = rig.work / "fake"
    fake.mkdir()
    (fake / "libspu.h").write_text((inc / "libspu.h").read_text().replace(LINES[0], "    short left;"))
    try:
        unsigned_copy(fake, rig.work / "fake_u")
        yield "refusal: a SpuVolume line not found", "no refusal"
    except Refusal as err:
        yield "refusal: a SpuVolume line not found", None if "wanted exactly 1" in str(err) else str(err)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--psyz-build", type=Path, required=True, help="a build folder of psyzbuild.py")
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; this control needs it")
        return 2
    try:
        inc = psyz_include(args.psyz_build)
    except Refusal as err:
        print(err)
        return 2
    started = time.time()
    failed = 0
    with tempfile.TemporaryDirectory(prefix="sdkshim-") as tmp:
        rig = Rig(args.cc, args.run.split(), inc, Path(tmp))
        for name, detail in cases(rig, inc):
            if detail is None:
                print(f"ok   {name}")
            else:
                failed += 1
                print(f"FAIL {name}: {detail}")
    print(f"run time {time.time() - started:.1f} s")
    print("all cases behaved as required" if not failed else f"{failed} cases did not behave as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
