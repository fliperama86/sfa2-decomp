#!/usr/bin/env python3
"""Controls for the loop of the setting fast_loads, port/src/loadwait.c (port_loadwait), built natively with the host's `cc`.

What is tested: the loop with made-up hooks in the test's own C (no game, no disc, no host clock): the load is started once with its two
arguments before anything else; the game's step and then the disc's step run once per pass, in that order, until the load reports done;
the check of `done` comes before the first pass (a load that is done at once runs no pass); the real clock step runs on every 256th
pass and not otherwise; `finish` runs once after the last pass, and not when it is null; a long load (3,000 passes) ends. Also the
table of the two overrides (names, notes, the setting each is tied to) and the gate: library.c installs a row only while its setting is not 0
(checked on the source of library.c's loop, which has no test rig of its own that reaches it).

What is NOT tested here: the game's own routines that the two overrides call by address (they run in the game), and a load on a real disc.
Those need the linked program. It needs `cc`; without it this file says so and ends with status 2.
"""

from __future__ import annotations

import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"

DRIVER = r'''
#include "loadwait.h"
#include "port.h"
#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct port_settings port_settings;
void port_cd_tick(void) {}
void port_tick(void) {}
extern const struct port_override port_loadwait_overrides[];

static int passes_needed, passes, ticks_seen, finished;
static char order[64];
static unsigned started_a = 99, started_b = 99;
static int started;

static void note(char c) { size_t n = strlen(order); if (n + 1 < sizeof order) { order[n] = c; order[n + 1] = 0; } }
static void h_start(unsigned a, unsigned b) { started++; started_a = a; started_b = b; note('S'); }
static int  h_done(void) { return passes >= passes_needed; }
static void h_step(void) { note('u'); }
static void h_drive(void) { note('d'); passes++; }
static void h_tick(void) { ticks_seen++; note('T'); }
static void h_finish(void) { finished++; note('F'); }

int main(int argc, char **argv)
{
    struct port_loadwait_hooks h = { h_start, h_done, h_step, h_drive, h_tick, h_finish };
    int use_finish;
    if (argc != 3) return 64;
    passes_needed = atoi(argv[1]);
    use_finish = atoi(argv[2]);
    if (!use_finish) h.finish = 0;
    port_loadwait(&h, 7, 9);
    printf("R started=%d a=%u b=%u passes=%d ticks=%d finished=%d order=%s\n", started, started_a, started_b, passes, ticks_seen, finished, passes_needed > 3 ? "-" : order);
    if (!strcmp(argv[1], "table")) return 0;
    return 0;
}
'''

TABLE = r'''
#include "port.h"
#include "settings.h"
#include <stdio.h>
struct port_settings port_settings;
void port_cd_tick(void) {}
void port_tick(void) {}
extern const struct port_override port_loadwait_overrides[];
int main(void)
{
    unsigned i;
    for (i = 0; port_loadwait_overrides[i].name; i++)
        printf("O %s %d %d %d\n", port_loadwait_overrides[i].name, port_loadwait_overrides[i].host != 0, port_loadwait_overrides[i].note != 0,
               port_loadwait_overrides[i].enabled == &port_settings.fast_loads);
    printf("N %u\n", i);
    return 0;
}
'''


def check(name: str, ok: bool, detail: str = ""):
    return name, None if ok else detail


def build(root: Path, cc: str, main_c: str, name: str) -> Path:
    (root / f"{name}.c").write_text(main_c)
    exe = root / name
    proc = subprocess.run([cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), str(root / f"{name}.c"), str(SRC / "loadwait.c"), "-o", str(exe)],
                          capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        raise RuntimeError("does not build:\n" + proc.stderr[-1500:])
    return exe


def run(exe: Path, *args: str) -> list[str]:
    proc = subprocess.run([str(exe), *args], capture_output=True, text=True, timeout=30)
    if proc.returncode != 0:
        raise RuntimeError(f"exit {proc.returncode}: {proc.stdout}{proc.stderr}")
    return proc.stdout.splitlines()


def cases(root: Path, cc: str):
    loop = build(root, cc, DRIVER, "loop")
    r = run(loop, "3", "1")[0]
    yield check("a load of 3 passes: started once with its two arguments, 3 passes, finish once", "started=1 a=7 b=9 passes=3 ticks=0 finished=1" in r, r)
    r = run(loop, "3", "1")[0]
    yield check("(order) start, then step and drive for each pass, finish last, no real step for 3 passes", "order=SudududF" in r, r)
    r = run(loop, "0", "1")[0]
    yield check("a load that is done at once runs no pass, and still finishes", "started=1 a=7 b=9 passes=0 ticks=0 finished=1 order=SF" in r, r)
    r = run(loop, "3", "0")[0]
    yield check("without a finish hook none is called", "passes=3 ticks=0 finished=0" in r, r)
    for n, want in ((255, 0), (256, 1), (257, 1), (511, 1), (512, 2), (3000, 11)):
        r = run(loop, str(n), "1")[0]
        yield check(f"{n} passes: the real clock step runs {want} time(s) (every 256th pass)", f"passes={n} ticks={want} " in r, r)
    table = build(root, cc, TABLE, "table")
    t = run(table)
    yield check("two overrides, each with a routine, a note and the setting fast_loads", t[-1] == "N 2" and t[:2] == ["O start_cd_track_wait_with_thread_switch 1 1 1", "O start_cd_track_and_wait_updating_control 1 1 1"], f"{t}")
    # the gate in library.c: the install loop skips a row whose setting is 0
    lib = (SRC / "library.c").read_text()
    gate = "if (override_at(k)->enabled && !*override_at(k)->enabled) continue;"
    install = lib.split("for (k = 0; k < override_total(); k++) {", 2)
    yield check("library.c's install loop skips an override whose setting is 0", len(install) == 3 and gate in install[2].split("out->overrides++", 1)[0], "the gate is missing in the install loop")


def main() -> int:
    global SRC
    if len(sys.argv) == 3 and sys.argv[1] == "--src":
        SRC = Path(sys.argv[2]).resolve()
    elif len(sys.argv) != 1:
        print("usage: test_hostloadwait.py [--src DIR]")
        return 2
    cc = shutil.which("cc")
    if not cc:
        print("the host's C compiler `cc` is missing; these controls need it")
        return 2
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostloadwait-test-") as tmp:
        for name, detail in cases(Path(tmp), cc):
            if detail is None:
                print(f"ok   {name}")
            else:
                print(f"FAIL {name}: {detail}")
                failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
