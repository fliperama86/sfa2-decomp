#!/usr/bin/env python3
"""Controls for the loop of the setting fast_loads, port/src/loadwait.c (port_loadwait), built natively with the host's `cc`.

What is tested: the loop with made-up hooks in the test's own C (no game, no disc, no host clock): the load is started once with its two
arguments before anything else; the game's step and then the disc's step run once per pass, in that order, until the load reports done;
the check of `done` comes before the first pass (a load that is done at once runs no pass); the real clock step runs on every 256th
pass and not otherwise; `finish` runs once after the last pass, and not when it is null; a long load (3,000 passes) ends. Also the
table of the overrides (names, notes, the setting each is tied to). That library.c installs a gated row only while its setting is not 0
is checked in `test_hostrun.py`, on the real library.c.

The loading screen's two collapsed states (port_loadscreen_start, port_loadscreen_fadeout) on made-up fields: what each leaves, from several
starting values, and that only the first calls the palette routine.

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
#include "port_tables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct port_settings port_settings;
const struct port_function port_functions[] = { { 0, 0, "none", 0, 0 } };
const unsigned port_function_count = 0;
void port_halt(int status, const char *fmt, ...) { (void)status; (void)fmt; exit(70); }
void port_clock_hurry(int on) { (void)on; }
void port_clock_blank_start(unsigned max_frames) { (void)max_frames; }
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
static void h_hurry(int on) { note(on ? 'H' : 'h'); }
static void h_finish(void) { finished++; note('F'); }

int main(int argc, char **argv)
{
    struct port_loadwait_hooks h = { h_start, h_done, h_step, h_drive, h_tick, h_hurry, h_finish };
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

SCREEN = r'''
#include "loadwait.h"
#include "port.h"
#include "settings.h"
#include "port_tables.h"
#include <stdio.h>
#include <stdlib.h>
struct port_settings port_settings;
const struct port_function port_functions[] = { { 0, 0, "none", 0, 0 } };
const unsigned port_function_count = 0;
void port_halt(int status, const char *fmt, ...) { (void)status; (void)fmt; exit(70); }
void port_clock_hurry(int on) { (void)on; }
void port_clock_blank_start(unsigned max_frames) { (void)max_frames; }
void port_cd_tick(void) {}
void port_tick(void) {}
static int palette_calls, blank_calls;
static void pal(void) { palette_calls++; }
static void blank(void) { blank_calls++; }
int main(int argc, char **argv)
{
    unsigned short state = (unsigned short)atoi(argv[2]), counter = 0x1234, delay = 77;
    unsigned char wait = 0x55;
    unsigned text = 0x8016da10u;
    if (argc != 3) return 64;
    struct port_loadscreen_fields f = { &state, &counter, &delay, &wait, &text, 0x8016da1eu, pal, blank };
    if (argv[1][0] == 's') port_loadscreen_start(&f); else port_loadscreen_fadeout(&f);
    printf("S state=%u counter=%u delay=%u wait=%u text=%x palette=%d blank=%d\n", state, counter, delay, wait, text, palette_calls, blank_calls);
    return 0;
}
'''

FADE = r'''
#include "loadwait.h"
#include "port.h"
#include "settings.h"
#include "port_tables.h"
#include <stdio.h>
#include <stdlib.h>
struct port_settings port_settings;
const struct port_function port_functions[] = { { 0, 0, "none", 0, 0 } };
const unsigned port_function_count = 0;
void port_halt(int status, const char *fmt, ...) { (void)status; (void)fmt; exit(70); }
void port_clock_hurry(int on) { (void)on; }
void port_clock_blank_start(unsigned max_frames) { (void)max_frames; }
void port_cd_tick(void) {}
void port_tick(void) {}
static unsigned calls, done_at, last_bank, last_mode;
static int last_amount;
static unsigned step(void *ctx, unsigned bank, int amount, unsigned mode)
{
    (void)ctx;
    calls++;
    last_bank = bank; last_amount = amount; last_mode = mode;
    return done_at && calls >= done_at;
}
int main(int argc, char **argv)
{
    unsigned r;
    if (argc != 4) return 64;
    done_at = (unsigned)atoi(argv[3]);
    r = port_fade_to_end(step, 0, 2, -1, (unsigned)atoi(argv[1]));
    printf("F mode=%s result=%u calls=%u bank=%u amount=%d lastmode=%u\n", argv[1], r, calls, last_bank, last_amount, last_mode);
    (void)argv[2];
    return 0;
}
'''

TABLE = r'''
#include "port.h"
#include "settings.h"
#include "port_tables.h"
#include <stdio.h>
#include <stdlib.h>
struct port_settings port_settings;
const struct port_function port_functions[] = { { 0, 0, "none", 0, 0 } };
const unsigned port_function_count = 0;
void port_halt(int status, const char *fmt, ...) { (void)status; (void)fmt; exit(70); }
void port_clock_hurry(int on) { (void)on; }
void port_clock_blank_start(unsigned max_frames) { (void)max_frames; }
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
    yield check("(order) hurry on, start, then step and drive for each pass, hurry off, finish last, no real step for 3 passes", "order=HSudududhF" in r, r)
    r = run(loop, "0", "1")[0]
    yield check("a load that is done at once runs no pass, and still finishes", "started=1 a=7 b=9 passes=0 ticks=0 finished=1 order=HShF" in r, r)
    r = run(loop, "3", "0")[0]
    yield check("without a finish hook none is called", "passes=3 ticks=0 finished=0" in r, r)
    for n, want in ((255, 0), (256, 1), (257, 1), (511, 1), (512, 2), (3000, 11)):
        r = run(loop, str(n), "1")[0]
        yield check(f"{n} passes: the real clock step runs {want} time(s) (every 256th pass)", f"passes={n} ticks={want} " in r, r)
    screen = build(root, cc, SCREEN, "screen")
    for st in (0, 1, 5):
        r = run(screen, "s", str(st))[0]
        yield check(f"state 0 collapsed, state index {st}: index {st + 2}, delay 1, counter 32, wait flag 0x80, one palette call, text on the empty string, the clock not started",
                    r == f"S state={st + 2} counter=32 delay=1 wait=128 text=8016da1e palette=1 blank=0", r)
    for st in (3, 4):
        r = run(screen, "f", str(st))[0]
        yield check(f"state 3 collapsed, state index {st}: index {st + 1}, delay 1, counter 0xffff, wait flag and text untouched, no palette call, the blank fast-forward started once",
                    r == f"S state={st + 1} counter=65535 delay=1 wait=85 text=8016da10 palette=0 blank=1", r)
    fade = build(root, cc, FADE, "fade")
    for mode, done_at, want_calls, want_result in (("0", "32", 32, 1), ("1", "32", 32, 1), ("0", "1", 1, 1), ("0", "0", 256, 0), ("2", "0", 1, 0), ("3", "5", 1, 0), ("2", "1", 1, 1)):
        r = run(fade, mode, "x", done_at)[0]
        yield check(f"fade mode {mode}, the routine says done at call {done_at}: {want_calls} call(s), result {want_result}, arguments passed on",
                    r == f"F mode={mode} result={want_result} calls={want_calls} bank=2 amount=-1 lastmode={mode}", r)
    table = build(root, cc, TABLE, "table")
    t = run(table)
    yield check("five overrides, each with a routine, a note and the setting fast_loads",
                t[-1] == "N 5" and t[:5] == ["O start_cd_track_wait_with_thread_switch 1 1 1", "O start_cd_track_and_wait_updating_control 1 1 1",
                                              "O start_hud_stage_step_sequence 1 1 1", "O update_fade_step_counter_down 1 1 1", "O fade_palette_bank 1 1 1"], f"{t}")



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
