#!/usr/bin/env python3
"""One control for port_callbacks_program_start (port/src/kernel.c): the effect of the routine on the interrupt library.

    python3 test_hostkernel_start.py --cc CROSS_CC [--src DIR]

The driver includes the real kernel.c, so it can read the file's own state (callbacks_active, interrupts_enabled, vcount), and
stands in for the routines that the file takes from the rest of the runtime. The case: with the callbacks started (ResetCallback
called once) and interrupts off (EnterCriticalSection, as the game has them when it calls Exec), a second ResetCallback leaves
interrupts off and the count of vblanks where it was; after port_callbacks_program_start the same call turns interrupts on and
the count of vblanks starts at 0, and the routine alone changes neither interrupts nor the count, with interrupts off or on. NOTHING IS COMPARED WITH THE
CONSOLE: the contract is the text of the routine's comment in kernel.c.

The fast clock of the setting fast_loads (port_clock_hurry, port_clock_blank_start, port_clock_frame_drawn), on the same driver's stand-ins and
with the real clock: with hurry on, 50 port_tick calls take under 30 ms and with it off over 40 ms (each sleeps for the next vblank, up to 2 ms);
after port_clock_blank_start(k) each port_tick takes a vblank at once, for k frames and no more; a frame drawn with 149 packets does not end it and one with
150 does; hurry nests (two on, one off: still on); and with nothing set the clock is as it was.

The last line is `all N cases behaved as required` or the number that did not.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"

DRIVER_C = r'''
#include "kernel.c"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void port_cd_tick(void) {}
void port_debug_tick(unsigned f) { (void)f; }
int port_game_span(const void *p, size_t n) { (void)p; (void)n; return 1; }
void port_gpu_present(void) {}
void port_menu_vblank(void) {}
void port_halt(int s, const char *fmt, ...) { (void)fmt; printf("stop %d\n", s); exit(s); }
void *port_h_DMACallback(int c, void (*f)(void)) { (void)c; (void)f; return 0; }
void port_input_apply(unsigned char *f, unsigned n) { (void)f; (void)n; }
int port_input_loaded(void) { return 0; }
int port_iopage_claim_due(void) { return 0; }
void port_iopage_deliver(void) {}
void port_iopage_deliver_claimed(void) {}
void port_iopage_poll(void) {}
void port_iopage_vblank(void) {}
void port_pad_host_init(void) {}
void port_pad_host_read(unsigned char *d, int n) { (void)d; (void)n; }
int port_press_claim_due(void) { return 0; }
void port_press_deliver_claimed(void) {}
void port_press_poll(void) {}
void port_target_check(const char *p, const void *t) { (void)p; (void)t; }
void port_trace_line(const char *fmt, ...) { (void)fmt; }

static void say(const char *what) { printf("%s: active %d, interrupts %d, vcount %u\n", what, callbacks_active, interrupts_enabled, vcount); }

int main(void)
{
    port_h_ResetCallback();
    say("started");
    vcount = 57;
    port_h_EnterCriticalSection();
    say("interrupts off");
    port_h_ResetCallback();
    say("reset, running");
    port_callbacks_program_start();
    say("program start");
    port_h_ResetCallback();
    say("reset, fresh");
    /* with interrupts on and the callbacks running: the routine marks the layer not started and changes nothing else */
    vcount = 31;
    port_h_ExitCriticalSection();
    say("interrupts on");
    port_callbacks_program_start();
    say("program start, interrupts on");
    return 0;
}
'''

CLOCK_DRIVER_C = r'''
#include "kernel.c"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void port_cd_tick(void) {}
void port_debug_tick(unsigned f) { (void)f; }
int port_game_span(const void *p, size_t n) { (void)p; (void)n; return 1; }
void port_gpu_present(void) {}
void port_menu_vblank(void) {}
void port_halt(int s, const char *fmt, ...) { (void)fmt; printf("stop %d\n", s); exit(s); }
void *port_h_DMACallback(int c, void (*f)(void)) { (void)c; (void)f; return 0; }
void port_input_apply(unsigned char *f, unsigned n) { (void)f; (void)n; }
int port_input_loaded(void) { return 0; }
int port_iopage_claim_due(void) { return 0; }
void port_iopage_deliver(void) {}
void port_iopage_deliver_claimed(void) {}
void port_iopage_poll(void) {}
void port_iopage_vblank(void) {}
void port_pad_host_init(void) {}
void port_pad_host_read(unsigned char *d, int n) { (void)d; (void)n; }
int port_press_claim_due(void) { return 0; }
void port_press_deliver_claimed(void) {}
void port_press_poll(void) {}
void port_target_check(const char *p, const void *t) { (void)p; (void)t; }
void port_trace_line(const char *fmt, ...) { (void)fmt; }


static unsigned long long ms_since(unsigned long long t0) { return (now_us() - t0) / 1000ull; }

int main(int argc, char **argv)
{
    const char *what = argc > 1 ? argv[1] : "";
    unsigned long long t0;
    int i;
    port_clock_start();
    t0 = now_us();
    if (!strcmp(what, "plain")) {            /* nothing set: 50 ticks sleep for the vblank */
        for (i = 0; i < 50; i++) port_tick();
        printf("elapsed_over_40=%d frames_under_12=%d\n", ms_since(t0) > 40, total_frames < 12);
    } else if (!strcmp(what, "hurry")) {     /* hurry on: no sleeping */
        port_clock_hurry(1);
        for (i = 0; i < 50; i++) port_tick();
        printf("elapsed_under_30=%d frames_under_3=%d\n", ms_since(t0) < 30, total_frames < 3);
    } else if (!strcmp(what, "nest")) {      /* two on, one off: still on */
        port_clock_hurry(1);
        port_clock_hurry(1);
        port_clock_hurry(0);
        for (i = 0; i < 50; i++) port_tick();
        printf("elapsed_under_30=%d\n", ms_since(t0) < 30);
    } else if (!strcmp(what, "off_too_far")) {   /* more offs than ons does not go below zero: one on after is on */
        port_clock_hurry(0);
        port_clock_hurry(0);
        port_clock_hurry(1);
        for (i = 0; i < 50; i++) port_tick();
        printf("elapsed_under_30=%d\n", ms_since(t0) < 30);
    } else if (!strcmp(what, "blank")) {     /* 8 frames, then the clock is real again */
        port_clock_blank_start(8);
        for (i = 0; i < 8; i++) port_tick();
        printf("frames_after_8_ticks=%u\n", total_frames);
        for (i = 0; i < 12; i++) port_tick();
        printf("frames_after_12_more_under_14=%d\n", total_frames < 14);
    } else if (!strcmp(what, "rich")) {      /* 149 packets do not end the run, 150 do */
        port_clock_blank_start(100);
        for (i = 0; i < 5; i++) port_tick();
        port_clock_frame_drawn(PORT_CLOCK_RICH_PACKETS - 1);
        for (i = 0; i < 5; i++) port_tick();
        printf("frames_after_10_ticks=%u\n", total_frames);
        port_clock_frame_drawn(PORT_CLOCK_RICH_PACKETS);
        for (i = 0; i < 12; i++) port_tick();
        printf("frames_after_12_more_under_14=%d\n", total_frames < 14);
    }
    return 0;
}
'''

CLOCK_CASES = [
    ("plain", "elapsed_over_40=1 frames_under_12=1", "with nothing set, 50 ticks sleep for the vblank (over 40 ms, about 3 frames)"),
    ("hurry", "elapsed_under_30=1 frames_under_3=1", "hurry on: 50 ticks take under 30 ms and no sleep"),
    ("nest", "elapsed_under_30=1", "hurry nests: two on, one off is still on"),
    ("off_too_far", "elapsed_under_30=1", "more offs than ons does not go below zero"),
    ("blank", "frames_after_8_ticks=8\nframes_after_12_more_under_14=1", "blank fast-forward: each tick takes a vblank for 8 frames, then the clock is real again"),
    ("rich", "frames_after_10_ticks=10\nframes_after_12_more_under_14=1", "a frame of 149 packets does not end the blank run, one of 150 does"),
]

WANT = [
    "started: active 1, interrupts 1, vcount 0",
    "interrupts off: active 1, interrupts 0, vcount 57",
    "reset, running: active 1, interrupts 0, vcount 57",
    "program start: active 0, interrupts 0, vcount 57",
    "reset, fresh: active 1, interrupts 1, vcount 0",
    "interrupts on: active 1, interrupts 1, vcount 31",
    "program start, interrupts on: active 0, interrupts 1, vcount 31",
]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cc", default=None)
    ap.add_argument("--src", type=Path, default=SRC)
    args = ap.parse_args()
    cc = args.cc or shutil.which("cc")
    if not cc:
        print("a C compiler for the host (--cc) is needed; this control needs it")
        return 2
    with tempfile.TemporaryDirectory(prefix="hostkernel-start-") as tmp:
        root = Path(tmp)
        (root / "driver.c").write_text(DRIVER_C)
        exe = root / "driver.exe"
        p = subprocess.run([cc, "-O1", "-Wall", "-I", str(args.src), "-o", str(exe), str(root / "driver.c")], capture_output=True, text=True)
        if p.returncode != 0:
            print(p.stdout + p.stderr)
            print("the driver does not build")
            return 2
        r = subprocess.run([str(exe)], capture_output=True, timeout=60)
        lines = r.stdout.decode(errors="replace").splitlines()
        ok = r.returncode == 0 and lines == WANT
        results = [("callbacks_fresh_at_program_start", ok, f"status {r.returncode}, lines {lines}, wanted {WANT}")]
        (root / "clock.c").write_text(CLOCK_DRIVER_C)
        exe2 = root / "clock.exe"
        p = subprocess.run([cc, "-O1", "-Wall", "-I", str(args.src), "-o", str(exe2), str(root / "clock.c")], capture_output=True, text=True)
        if p.returncode != 0:
            print(p.stdout + p.stderr)
            print("the clock driver does not build")
            return 2
        for arg, want, name in CLOCK_CASES:
            r2 = subprocess.run([str(exe2), arg], capture_output=True, timeout=60)
            got = r2.stdout.decode(errors="replace").strip()
            results.append((name, r2.returncode == 0 and got == want, f"status {r2.returncode}, got {got!r}, wanted {want!r}"))
    failed = 0
    for name, good, detail in results:
        print(f"ok   {name}" if good else f"FAIL {name}: {detail}")
        failed += 0 if good else 1
    print(f"all {len(results)} cases behaved as required" if not failed else f"{failed} case(s) behaved wrongly")
    return 0 if not failed else 1


if __name__ == "__main__":
    sys.exit(main())
