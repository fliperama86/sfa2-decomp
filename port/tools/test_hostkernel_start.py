#!/usr/bin/env python3
"""One control for port_callbacks_program_start (port/src/kernel.c): the effect of the routine on the interrupt library.

    python3 test_hostkernel_start.py --cc CROSS_CC [--src DIR]

The driver includes the real kernel.c, so it can read the file's own state (callbacks_active, interrupts_enabled, vcount), and
stands in for the routines that the file takes from the rest of the runtime. The case: with the callbacks started (ResetCallback
called once) and interrupts off (EnterCriticalSection, as the game has them when it calls Exec), a second ResetCallback leaves
interrupts off and the count of vblanks where it was; after port_callbacks_program_start the same call turns interrupts on and
the count of vblanks starts at 0, and the routine alone changes neither interrupts nor the count. NOTHING IS COMPARED WITH THE
CONSOLE: the contract is the text of the routine's comment in kernel.c.

The last line is `all 1 cases behaved as required` or the number that did not.
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
    return 0;
}
'''

WANT = [
    "started: active 1, interrupts 1, vcount 0",
    "interrupts off: active 1, interrupts 0, vcount 57",
    "reset, running: active 1, interrupts 0, vcount 57",
    "program start: active 0, interrupts 0, vcount 57",
    "reset, fresh: active 1, interrupts 1, vcount 0",
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
    print(("ok" if ok else f"FAIL: status {r.returncode}, lines {lines}, wanted {WANT}") + " callbacks_fresh_at_program_start")
    print("all 1 cases behaved as required" if ok else "1 case(s) behaved wrongly")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
