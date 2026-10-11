#!/usr/bin/env python3
"""Controls for the logo program's small library routines (port/src/progcalls.c, progcalls.h): the plain read of sectors
(CdRead, CdReadSync), Sony's simple pad calls (PadInit, PadRead, PadStop) and SetVideoMode.

    python3 test_hostprogcalls.py --cc CROSS_CC [--src DIR] [--only SUBSTRING]

The test compiles progcalls.c with the disc layer (cd.c, xa.c, disc.c, sha256.c) and the pads' file (input.c, with its read of the
devices renamed so that the driver can supply a frame of its own or call it) and a driver of its own, which maps the game's RAM at
its own address and stands in for what these files take from the runtime: port_tick (a clock the test sets: it takes a vblank, which
is the frame count and the disc's tick, at every K-th call or never; the disc can be set to move at every P-th vblank or never), port_halt (prints `stop: ...` and ends the driver with the status it was
given; each case that stops runs in a driver of its own), the frame count, the audio lock and the trace. The calls the disc layer
gets from progcalls.c are logged by wrappers at the link (`CMD B 9`, `CMD B e 80`, `CMD N 6`, `HOST set`, `HOST clear`), and the
test can make one of them fail. The disc is a made-up image whose every sector says which sector it is in its first two bytes
(test_hostcd.py's); the buffers are compared with that image's bytes by a hash computed here, in Python, from the image's own
formula. NOTHING IS COMPARED WITH THE CONSOLE'S LIBRARY: the contract is the text of progcalls.h, and the cases check that the
calls do what it says. The disc layer and the pads have their own tests.

Groups: the read (the bytes for one sector and for several, the commands in order, the handler removed and the drive paused after
the last, the sync in both modes before, during and after, the result bytes, a read past the image's end, a second read, a read
while one is on, a refused step, no progress and slow progress), the stops of the read, the pads (each button bit, both halves,
frames that are not a digital pad's, the input script, PadInit and PadStop), SetVideoMode.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required` or the number that did not.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostcd as CD  # noqa: E402  (the made-up disc image)

SRC = HERE.parent / "src"
LIMIT = 60
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase",
              "-Wl,--wrap=port_CdControl", "-Wl,--wrap=port_CdControlB", "-Wl,--wrap=port_cd_host_ready"]
RAM, RAM_END = 0x80000000, 0x80200000
STATUS = 9   # PORT_EXIT_OTHER, the status of port_halt in a stop of progcalls.c
BUF = 0x80100000
VAR_STATUS = 0x80181b44

DRIVER_C = r'''
#include "port.h"
#include "cd.h"
#include "progcalls.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

volatile int port_handler_depth;
static unsigned frames;
static int ticks, divider = 1, disc_period = 1, disc_count, log_on, fail_what; /* fail_what: 1 Pause, 2 Setmode, 3 ReadN: that step returns 0 */
static int pad_mode;                              /* 0 the runtime's own read (the idle pad), 1 the made-up frame below */
static unsigned char pad_frame[34];

unsigned port_frames(void) { return frames; }
void port_audio_lock(void) {}
void port_audio_unlock(void) {}
void port_trace_line(const char *fmt, ...) { (void)fmt; }
void port_target_check(const char *path, const void *target) { (void)path; (void)target; }
void port_tick(void)
{
    ticks++;
    if (divider > 0 && ticks % divider == 0) {   /* a vblank: only at every divider-th call */
        frames++;
        if (disc_period > 0 && ++disc_count % disc_period == 0) port_cd_tick();   /* the disc moves at every disc_period-th vblank */
    }
}
void port_halt(int status, const char *fmt, ...)
{
    va_list ap;
    printf("stop: ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    exit(status);
}

/* The runtime's own read of the pad (input.c, renamed at the compile) or the made-up frame. */
void real_pad_host_read(unsigned char *dst, int len);
void port_pad_host_read(unsigned char *dst, int len)
{
    if (pad_mode == 0) { real_pad_host_read(dst, len); return; }
    memset(dst, 0, (size_t)len);
    memcpy(dst, pad_frame, (size_t)(len < 34 ? len : 34));
}

/* The wrappers (the linker's --wrap): what progcalls.c asks of the disc layer, logged and, on request, refused. */
int __real_port_CdControl(int com, unsigned char *param, unsigned char *result);
int __real_port_CdControlB(int com, unsigned char *param, unsigned char *result);
void __real_port_cd_host_ready(void (*handler)(int intr, unsigned char *result));
int __wrap_port_CdControl(int com, unsigned char *param, unsigned char *result)
{
    if (log_on) { printf("CMD N %x\n", com); if (fail_what == 3 && com == 6) return 0; }
    return __real_port_CdControl(com, param, result);
}
int __wrap_port_CdControlB(int com, unsigned char *param, unsigned char *result)
{
    if (log_on) {
        if (com == 0xe) printf("CMD B e %02x\n", param ? param[0] : 0xee);
        else printf("CMD B %x\n", com);
        if ((fail_what == 1 && com == 9) || (fail_what == 2 && com == 0xe)) return 0;
    }
    return __real_port_CdControlB(com, param, result);
}
void __wrap_port_cd_host_ready(void (*handler)(int intr, unsigned char *result))
{
    if (log_on) printf(handler ? "HOST set\n" : "HOST clear\n");
    __real_port_cd_host_ready(handler);
}

static unsigned long long fnv(const unsigned char *p, unsigned n)
{
    unsigned long long h = 1469598103934665603ull;
    while (n--) { h ^= *p++; h *= 1099511628211ull; }
    return h;
}

int main(int argc, char **argv)
{
    char line[512], err[PORT_ERR];
    struct port_disc d;
    void *ram = VirtualAlloc((void *)(size_t)PORT_RAM_BASE, PORT_RAM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (ram != (void *)(size_t)PORT_RAM_BASE) { printf("no RAM\n"); return 3; }
    if (argc < 2) return 64;
    if (strcmp(argv[1], "-") != 0) {
        if (port_disc_open(&d, argv[1], err, sizeof err) != 0) { printf("refused: %s\n", err); return 2; }
        port_cd_init(&d);
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    while (fgets(line, sizeof line, stdin)) {
        char cmd[32];
        char *rest = line;
        int n = 0;
        if (sscanf(line, "%31s %n", cmd, &n) < 1) continue;
        rest += n;
        if (!strcmp(cmd, "cdinit")) printf("R %d\n", port_CdInit());
        else if (!strcmp(cmd, "log")) log_on = atoi(rest);
        else if (!strcmp(cmd, "fail")) fail_what = atoi(rest);
        else if (!strcmp(cmd, "div")) divider = atoi(rest);   /* port_tick takes a vblank (the frame count) at every K-th call; 0 never */
        else if (!strcmp(cmd, "disc")) { disc_period = atoi(rest); disc_count = 0; }   /* the disc delivers at every P-th vblank; 0 never */
        else if (!strcmp(cmd, "fr")) printf("F %u\n", frames);
        else if (!strcmp(cmd, "ticks")) printf("T %d\n", ticks);
        else if (!strcmp(cmd, "tick")) { int k = atoi(rest); while (k-- > 0) { frames++; port_cd_tick(); } }
        else if (!strcmp(cmd, "seek")) {   /* Setloc and SeekL with a position, as the program sends it (CdControl 0x15) */
            unsigned char p[4] = {0, 0, 0, 0}, res[8];
            port_CdIntToPos(atoi(rest), p);
            printf("K %d\n", port_CdControlB(0x15, p, res));
        }
        else if (!strcmp(cmd, "read")) {   /* sectors, address, mode (hex) */
            int sectors, mode; unsigned a;
            if (sscanf(rest, "%d %x %x", &sectors, &a, &mode) != 3) return 64;
            printf("RD %d\n", port_CdRead(sectors, (void *)(size_t)a, mode));
        }
        else if (!strcmp(cmd, "sync")) {   /* mode */
            unsigned char res[8];
            int i, r;
            memset(res, 0xee, 8);
            r = port_CdReadSync(atoi(rest), res);
            printf("SY %d res", r);
            for (i = 0; i < 8; i++) printf(" %02x", res[i]);
            printf("\n");
        }
        else if (!strcmp(cmd, "syncnull")) printf("SN %d\n", port_CdReadSync(atoi(rest), 0));
        else if (!strcmp(cmd, "hash")) {   /* the hash of n bytes at an address */
            unsigned a, k;
            if (sscanf(rest, "%x %u", &a, &k) != 2) return 64;
            printf("H %016llx\n", fnv((const unsigned char *)(size_t)a, k));
        }
        else if (!strcmp(cmd, "poke")) { unsigned a, v; if (sscanf(rest, "%x %x", &a, &v) != 2) return 64; *(unsigned char *)(size_t)a = (unsigned char)v; }
        else if (!strcmp(cmd, "word")) { unsigned a = (unsigned)strtoul(rest, 0, 16); printf("W %08x\n", *(unsigned *)(size_t)a); }
        else if (!strcmp(cmd, "padmode")) pad_mode = atoi(rest);
        else if (!strcmp(cmd, "frame")) {   /* the made-up frame: status, kind, byte 2, byte 3 (hex) */
            unsigned a, b, c, e;
            if (sscanf(rest, "%x %x %x %x", &a, &b, &c, &e) != 4) return 64;
            memset(pad_frame, 0, sizeof pad_frame);
            pad_frame[0] = (unsigned char)a; pad_frame[1] = (unsigned char)b; pad_frame[2] = (unsigned char)c; pad_frame[3] = (unsigned char)e;
        }
        else if (!strcmp(cmd, "frames")) frames = (unsigned)atoi(rest);
        else if (!strcmp(cmd, "input")) {
            char e[PORT_INPUT_ERR];
            char *p = rest + strlen(rest);
            while (p > rest && (p[-1] == '\n' || p[-1] == '\r')) *--p = 0;
            printf("IN %d\n", port_input_load(rest, e, sizeof e));
        }
        else if (!strcmp(cmd, "padinit")) printf("PI %d\n", port_PadInit(atoi(rest)));
        else if (!strcmp(cmd, "padstop")) { port_PadStop(); printf("PS\n"); }
        else if (!strcmp(cmd, "padread")) printf("PR %08x\n", port_PadRead(atoi(rest)));
        else if (!strcmp(cmd, "vmode")) printf("VM %ld\n", port_SetVideoMode(atol(rest)));
        else { printf("unknown script command %s\n", cmd); return 64; }
    }
    return 0;
}
'''


class Ctx:
    exe: Path
    tmp: Path
    image: Path
    short: Path   # an image of 20 sectors


def run(script: str, image: Path | None = None, extra=()) -> tuple[int, list[str]]:
    """Runs the driver on a script (a list of lines). Returns (status, the lines it printed)."""
    p = subprocess.run([str(Ctx.exe), str(image or Ctx.image), *extra], input=(script.rstrip("\n") + "\n").encode(), capture_output=True, timeout=LIMIT)
    return p.returncode, p.stdout.decode(errors="replace").splitlines()


def fnv(data: bytes) -> str:
    h = 1469598103934665603
    for b in data:
        h = ((h ^ b) * 1099511628211) & 0xFFFFFFFFFFFFFFFF
    return f"{h:016x}"


def sectors_hash(first: int, count: int) -> str:
    return "H " + fnv(b"".join(CD.sector_data(n) for n in range(first, first + count)))


def zeros_hash(n: int) -> str:
    return "H " + fnv(bytes(n))


def after(lines: list[str], prefix: str) -> list[str]:
    return [l for l in lines if l.startswith(prefix)]


SETUP = "cdinit\nseek 10\n"   # the program's own order: a position with a seek, then the read


# ---- the read ---------------------------------------------------------------

def read_one_sector():
    rc, l = run(SETUP + "read 1 80100000 80\nsync 0\nhash 80100000 2048\nhash 80100800 2048\n")
    if rc != 0:
        return f"status {rc} {l}"
    if after(l, "RD ") != ["RD 1"]:
        return f"CdRead returned {after(l, 'RD ')}"
    if after(l, "SY ")[0].split()[1] != "0":
        return f"CdReadSync returned {after(l, 'SY ')}"
    h = after(l, "H ")
    if h != [sectors_hash(10, 1), zeros_hash(2048)]:
        return f"the buffer holds other bytes than sector 10, or the next 2048 bytes were written: {h}"


def read_three_sectors():
    rc, l = run(SETUP + "read 3 80100000 80\nsync 0\nhash 80100000 6144\nhash 80101800 16\n")
    if after(l, "RD ") != ["RD 1"] or after(l, "SY ")[0].split()[1] != "0":
        return f"{l}"
    if after(l, "H ") != [sectors_hash(10, 3), zeros_hash(16)]:
        return f"the buffer differs from sectors 10 to 12: {after(l, 'H ')}"


def read_many_sectors():
    # 74 sectors, as the logo's program reads its code (0x4a), at double speed: the sync ticks about 30 times
    rc, l = run(SETUP + "read 74 80100000 80\nsync 0\nhash 80100000 %d\nhash %x 16\nticks\n" % (74 * 2048, 0x80100000 + 74 * 2048))
    if after(l, "RD ") != ["RD 1"] or after(l, "SY ")[0].split()[1] != "0":
        return f"{l}"
    if after(l, "H ") != [sectors_hash(10, 74), zeros_hash(16)]:
        return f"the buffer differs from sectors 10 to 83: {after(l, 'H ')}"
    t = int(after(l, "T ")[0].split()[1])
    if not 25 <= t <= 40:
        return f"the wait ticked {t} times; the drive's rate (2, 3, 2, 3 sectors a tick) gives about 30"


def commands_in_order():
    rc, l = run(SETUP + "log 1\nread 2 80100000 80\ntick 2\nsync 0\n")
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    want = ["HOST clear", "CMD B e 80", "HOST set", "CMD N 6", "HOST clear", "CMD B 9"]
    if got != want:
        return f"{got}, wanted {want}"


def mode_low_byte_is_sent():
    rc, l = run(SETUP + "log 1\nread 1 80100000 1c80\nsync 0\n")
    got = [x for x in l if x.startswith("CMD B e")]
    if got != ["CMD B e 80"]:
        return f"{got}"
    rc, l = run(SETUP + "log 1\nread 1 80100000 0\nsync 0\n")
    got = [x for x in l if x.startswith("CMD B e")]
    if got != ["CMD B e 00"]:
        return f"{got}"


def handler_removed_and_drive_paused_after_last():
    # 2 sectors at double speed come at the first tick; two more ticks would give 3 and 2 more if the drive still read
    rc, l = run(SETUP + "read 2 80100000 80\ntick 1\nword %x\ntick 3\nhash 80101000 4096\nword %x\n" % (VAR_STATUS, VAR_STATUS))
    w = after(l, "W ")
    h = after(l, "H ")
    if w != ["W 00000002", "W 00000002"]:
        return f"the drive's status after the last sector (idle is 2): {w}"
    if h != [zeros_hash(4096)]:
        return f"sectors went on arriving in the buffer after the last one: {h}"
    if after(l, "RD ") != ["RD 1"]:
        return f"{l}"


def sync_before_during_after():
    # 12 sectors at double speed: ticks give 2, 3, 2, 3, ... so 2 are in after the first tick and 5 after the second
    rc, l = run(SETUP + "read 12 80100000 80\nsync 1\ntick 1\nsync 1\ntick 1\nsync 1\ntick 3\nsync 1\nsync 0\nsync 1\n")
    s = [int(x.split()[1]) for x in after(l, "SY ")]
    if s != [12, 10, 7, 0, 0, 0]:
        return f"CdReadSync(1) gave {s}, wanted [12, 10, 7, 0, 0, 0]"
    rc, l = run(SETUP + "read 12 80100000 80\nsync 0\nsync 1\nhash 80100000 %d\n" % (12 * 2048))
    s = [int(x.split()[1]) for x in after(l, "SY ")]
    if s != [0, 0] or after(l, "H ") != [sectors_hash(10, 12)]:
        return f"{s} {after(l, 'H ')}"


def sync_waits_by_ticking():
    rc, l = run(SETUP + "read 5 80100000 80\nticks\nsync 0\nticks\n")
    t = [int(x.split()[1]) for x in after(l, "T ")]
    if t[1] <= t[0]:
        return f"ticks before and after the wait: {t}"
    rc, l = run(SETUP + "read 5 80100000 80\nticks\nsync 1\nticks\n")
    t = [int(x.split()[1]) for x in after(l, "T ")]
    if t[1] != t[0]:
        return "the polling mode ticked"


def result_bytes():
    # the result of the last sector's interrupt: the status 0x22 (reading) and zeros
    rc, l = run(SETUP + "read 2 80100000 80\nsync 0\nsync 1\nsyncnull 1\nsyncnull 0\n")
    s = after(l, "SY ")
    want = "SY 0 res 22 00 00 00 00 00 00 00"
    if s != [want, want] or after(l, "SN ") != ["SN 0", "SN 0"]:
        return f"{l}"
    rc, l = run(SETUP + "read 6 80100000 80\ntick 1\nsync 1\n")
    if after(l, "SY ") != ["SY 4 res 22 00 00 00 00 00 00 00"]:
        return f"the result during a read: {after(l, 'SY ')}"
    rc, l = run(SETUP + "sync 0\nsync 1\n")
    if after(l, "SY ") != ["SY 0 res 00 00 00 00 00 00 00 00"] * 2:
        return f"with no read ever started: {after(l, 'SY ')}"


def read_past_the_end_fails():
    # the image has 20 sectors; sectors 18 and 19 come, then the disc error
    rc, l = run("cdinit\nseek 18\nlog 1\nread 5 80100000 80\nsync 0\nsync 1\nsync 0\nhash 80100000 4096\nhash 80101000 2048\n", Ctx.short)
    s = after(l, "SY ")
    err = "res 03 40 00 00 00 00 00 00"
    if [x.split()[1] for x in s] != ["-1", "-1", "-1"] or any(err not in x for x in s):
        return f"CdReadSync: {s}"
    if after(l, "H ") != [sectors_hash(18, 2), zeros_hash(2048)]:
        return f"the buffer: {after(l, 'H ')}"
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    if got != ["HOST clear", "CMD B e 80", "HOST set", "CMD N 6", "HOST clear"]:
        return f"the handler was not removed, or the drive was paused after a failure: {got}"
    # a read right after the failure works
    rc, l = run("cdinit\nseek 18\nread 5 80100000 80\nsync 0\nseek 4\nread 2 80102000 80\nsync 0\nhash 80102000 4096\n", Ctx.short)
    s = after(l, "SY ")
    if [x.split()[1] for x in s] != ["-1", "0"] or after(l, "H ") != [sectors_hash(4, 2)]:
        return f"{s} {after(l, 'H ')}"


def read_ending_exactly_at_the_images_end():
    rc, l = run("cdinit\nseek 18\nread 2 80100000 80\nsync 0\nhash 80100000 4096\n", Ctx.short)
    if after(l, "SY ")[0].split()[1] != "0" or after(l, "H ") != [sectors_hash(18, 2)]:
        return f"{l}"


def second_read_after_a_finished_one():
    script = SETUP + ("log 1\nread 2 80100000 80\nsync 0\nlog 0\nseek 30\nlog 1\nread 3 80104000 80\nsync 0\nlog 0\n"
                      "hash 80100000 4096\nhash 80104000 6144\nhash 80101000 4096\n")
    rc, l = run(script)
    s = after(l, "SY ")
    if [x.split()[1] for x in s] != ["0", "0"]:
        return f"{s}"
    if after(l, "H ") != [sectors_hash(10, 2), sectors_hash(30, 3), zeros_hash(4096)]:
        return f"{after(l, 'H ')}"
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    want = ["HOST clear", "CMD B e 80", "HOST set", "CMD N 6", "HOST clear", "CMD B 9"] * 2
    if got != want:
        return f"{got}"


def second_read_without_a_seek_goes_on_from_the_drive():
    # documented in progcalls.h: the read starts where the drive is, after the sectors the last read took
    rc, l = run(SETUP + "read 2 80100000 80\nsync 0\nread 2 80104000 80\nsync 0\nhash 80104000 4096\n")
    if after(l, "H ") != [sectors_hash(12, 2)]:
        return f"{after(l, 'H ')}"


def read_while_one_is_on_pauses_first():
    rc, l = run(SETUP + "read 50 80100000 80\ntick 1\nlog 1\nread 3 80104000 80\nsync 0\nhash 80100000 4096\nhash 80104000 6144\nhash 80101000 4096\n")
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    want = ["HOST clear", "CMD B 9", "CMD B e 80", "HOST set", "CMD N 6", "HOST clear", "CMD B 9"]
    if got != want:
        return f"{got}, wanted {want}"
    # the first read got its two sectors (10, 11); the second starts where the drive stopped (12)
    if after(l, "H ") != [sectors_hash(10, 2), sectors_hash(12, 3), zeros_hash(4096)]:
        return f"{after(l, 'H ')}"
    if after(l, "SY ")[0].split()[1] != "0":
        return f"{l}"


def a_refused_step_returns_zero_and_leaves_nothing():
    # the Pause before the read (the drive is reading)
    rc, l = run(SETUP + "read 50 80100000 80\ntick 1\nlog 1\nfail 1\nread 3 80104000 80\nsync 0\nsync 1\n")
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    if after(l, "RD ") != ["RD 1", "RD 0"] or got != ["HOST clear", "CMD B 9"]:
        return f"pause refused: {l}"
    if [x.split()[1] for x in after(l, "SY ")] != ["0", "0"]:
        return f"after a refused pause a sync returned {after(l, 'SY ')}"
    # the Setmode
    rc, l = run(SETUP + "log 1\nfail 2\nread 3 80104000 80\nsync 0\nhash 80104000 6144\n")
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    if after(l, "RD ") != ["RD 0"] or got != ["HOST clear", "CMD B e 80"] or after(l, "H ") != [zeros_hash(6144)]:
        return f"setmode refused: {l}"
    # the ReadN: the handler set for it is removed again
    rc, l = run(SETUP + "log 1\nfail 3\nread 3 80104000 80\nsync 0\nsync 1\ntick 4\nhash 80104000 6144\n")
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    if after(l, "RD ") != ["RD 0"] or got != ["HOST clear", "CMD B e 80", "HOST set", "CMD N 6", "HOST clear"]:
        return f"readn refused: {l}"
    if [x.split()[1] for x in after(l, "SY ")] != ["0", "0"] or after(l, "H ") != [zeros_hash(6144)]:
        return f"after a refused ReadN: {l}"


def the_drive_is_reading_by_the_status_bits_0xe0():
    # Sony's routine looks at the bits 0xe0 of the status byte; the first Pause is logged only when one of them is set
    for status, pauses in ((0x00, 0), (0x02, 0), (0x1f, 0), (0x20, 1), (0x22, 1), (0x40, 1), (0x80, 1), (0xe0, 1), (0xc2, 1)):
        rc, l = run(SETUP + "poke %x %x\nlog 1\nread 1 80100000 80\nsync 0\n" % (VAR_STATUS, status))
        got = [x for x in l if x.startswith("CMD B 9")]
        want = ["CMD B 9"] * (pauses + 1)   # the handler's own Pause after the last sector is one more
        if got != want:
            return f"status 0x{status:02x}: {got}"


def result_of_a_refused_read_is_cleared():
    rc, l = run(SETUP + "read 2 80100000 80\nsync 0\nfail 2\nlog 1\nread 2 80102000 80\nsync 1\nsync 0\n")
    s = after(l, "SY ")
    zero = "res 00 00 00 00 00 00 00 00"
    if after(l, "RD ") != ["RD 1", "RD 0"] or [x.split()[1] for x in s] != ["0", "0", "0"] or s[1:] != ["SY 0 " + zero] * 2:
        return f"{l}"


def no_sectors_arrive_the_wait_runs_out():
    # the limit is 600 vblanks (port_frames), not 600 calls of the tick: for one call per vblank and for seven
    for k in (1, 7):
        rc, l = run(SETUP + "read 3 80100000 80\ndiv %d\ndisc 0\nfr\nticks\nsync 0\nfr\nticks\nsync 1\n" % k)
        s = after(l, "SY ")
        t = [int(x.split()[1]) for x in after(l, "T ")]
        f = [int(x.split()[1]) for x in after(l, "F ")]
        if [x.split()[1] for x in s] != ["-1", "3"] or l.count("cd: CdReadSync timed out") != 1:
            return f"divisor {k}: {l}"
        if f[1] - f[0] != 600:
            return f"divisor {k}: the wait ended after {f[1] - f[0]} vblanks, not 600"
        if t[1] % k != 0 or not 599 * k < t[1] - t[0] <= 600 * k:
            return f"divisor {k}: the wait took {t[1] - t[0]} calls; it must end at the call that took the 600th vblank"


def a_far_divisor_does_not_cut_a_read_off():
    # 74 sectors need some 30 vblanks; with a vblank only at every 5,000th call the old count of calls (600) gave up at once
    rc, l = run(SETUP + "div 5000\nread 74 80100000 80\nsync 0\nhash 80100000 %d\nfr\n" % (74 * 2048))
    if after(l, "SY ")[0].split()[1] != "0" or "cd: CdReadSync timed out" in l:
        return f"{l}"
    if after(l, "H ") != [sectors_hash(10, 74)]:
        return f"the buffer differs from sectors 10 to 83: {after(l, 'H ')}"


def a_sector_at_the_limit_comes_in_a_sector_after_it_does_not():
    # (set after the read, which ticks too) one sector, the disc delivering at every P-th vblank: the 600th vblank's tick is the last one the wait makes
    for p, want in ((599, "0"), (600, "0"), (601, "-1"), (1200, "-1")):
        rc, l = run(SETUP + "read 1 80100000 80\ndisc %d\nsync 0\nhash 80100000 2048\n" % p)
        timed = "cd: CdReadSync timed out" in l
        if after(l, "SY ")[0].split()[1] != want or timed != (want == "-1"):
            return f"a sector at vblank {p}: {l}"
        if want == "0" and after(l, "H ") != [sectors_hash(10, 1)]:
            return f"{after(l, 'H ')}"


def a_late_sector_starts_the_count_again():
    # sectors at vblank 400 (two) and 800 (the third): 800 vblanks in all, but never 600 without one
    rc, l = run(SETUP + "read 3 80100000 80\ndisc 400\nfr\nsync 0\nfr\nhash 80100000 6144\n")
    f = [int(x.split()[1]) for x in after(l, "F ")]
    if after(l, "SY ")[0].split()[1] != "0" or "cd: CdReadSync timed out" in l or after(l, "H ") != [sectors_hash(10, 3)]:
        return f"{l}"
    if f[1] - f[0] != 800:
        return f"the wait took {f[1] - f[0]} vblanks; the case needs 800 (past the limit, with sectors inside it)"
    # six sectors, two each at vblanks 400, 800 and 1200: more than 600 vblanks pass twice over, and the read still ends with 0
    # at 1200, because each pair of sectors starts the count again
    rc, l = run(SETUP + "read 6 80100000 80\ndisc 400\nfr\nsync 0\nfr\n")
    f = [int(x.split()[1]) for x in after(l, "F ")]
    if after(l, "SY ")[0].split()[1] != "0" or f[1] - f[0] != 1200:
        return f"{l}"


def slow_progress_does_not_run_out():
    # a sector every 400 calls: 3 sectors need over 1,000 calls, more than the limit of 600, but each arrival starts the count again
    rc, l = run(SETUP + "div 400\nread 3 80100000 00\nsync 0\nticks\nhash 80100000 6144\n")
    if after(l, "SY ")[0].split()[1] != "0" or "cd: CdReadSync timed out" in l or after(l, "H ") != [sectors_hash(10, 3)]:
        return f"{l}"
    t = int(after(l, "T ")[0].split()[1])
    if t <= 600:
        return f"only {t} calls; the case does not reach past the limit"


def fewer_than_one_sector_returns_zero():
    rc, l = run(SETUP + "log 1\nread 0 80100000 80\nread -1 80100000 80\nread -2147483648 80100000 80\nsync 0\n")
    got = [x for x in l if x.startswith(("CMD", "HOST"))]
    if after(l, "RD ") != ["RD 0"] * 3 or got:
        return f"{l}"


def last_sectors_of_ram_are_accepted():
    rc, l = run(SETUP + "read 2 801ff000 80\nsync 0\nhash 801ff000 4096\n")
    if after(l, "RD ") != ["RD 1"] or after(l, "H ") != [sectors_hash(10, 2)]:
        return f"{l}"


STOPS = {
    "mode_0x10": ("read 1 80100000 90", "CdRead: mode 0x90 asks for a sector form other than 2,048 bytes"),
    "mode_0x20": ("read 1 80100000 a0", "CdRead: mode 0xa0 asks for a sector form other than 2,048 bytes"),
    "mode_0x30": ("read 1 80100000 b0", "CdRead: mode 0xb0 asks for a sector form other than 2,048 bytes"),
    "mode_checked_before_the_count": ("read 0 80100000 20", "CdRead: mode 0x20 asks for a sector form other than 2,048 bytes"),
    "buffer_null": ("read 1 0 80", "CdRead: the buffer at 0x00000000, 2048 bytes, is not inside the game's RAM"),
    "buffer_below_ram": ("read 1 7ffff800 80", "CdRead: the buffer at 0x7ffff800, 2048 bytes, is not inside the game's RAM"),
    "buffer_ends_one_byte_past_ram": ("read 2 801ff7ff 80", "CdRead: the buffer at 0x801ff7ff, 4096 bytes, is not inside the game's RAM"),
    "second_sector_past_ram": ("read 2 801ff800 80", "CdRead: the buffer at 0x801ff800, 4096 bytes, is not inside the game's RAM"),
    "buffer_after_ram": ("read 1 80200000 80", "CdRead: the buffer at 0x80200000, 2048 bytes, is not inside the game's RAM"),
    "count_of_two_million": ("read 2097151 80100000 80", "CdRead: the buffer at 0x80100000, 4294965248 bytes, is not inside the game's RAM"),
    "count_of_two_billion": ("read 2147483647 80100000 80", "CdRead: the buffer at 0x80100000, 4398046509056 bytes, is not inside the game's RAM"),
}


def stop_function(key):
    script, line = STOPS[key]

    def case():
        rc, l = run(SETUP + "log 1\n" + script)
        if rc != STATUS:
            return f"status {rc}, wanted {STATUS}: {l}"
        if l[-1:] != ["stop: " + line] or any(x.startswith(("CMD", "HOST")) for x in l):
            return f"{l}"
    case.__name__ = "stop_" + key
    return case


# ---- the pads ---------------------------------------------------------------

# the word's bits: bit 0 L2 to bit 15 left (input.c's header)
BITS = {"l2": 0, "r2": 1, "l1": 2, "r1": 3, "triangle": 4, "circle": 5, "cross": 6, "square": 7,
        "select": 8, "l3": 9, "r3": 10, "start": 11, "up": 12, "right": 13, "down": 14, "left": 15}


def frame_for(pressed: int) -> str:
    """A digital pad's frame (status 0, kind 0x41) with the bits of `pressed` held (a 0 bit in the frame)."""
    word = ~pressed & 0xFFFF
    return "frame 00 41 %02x %02x" % (word >> 8, word & 0xFF)


def pad_each_button_in_the_low_half():
    lines = "padmode 1\n"
    for b in BITS.values():
        lines += frame_for(1 << b) + "\npadread 1\n"
    rc, l = run(lines)
    got = after(l, "PR ")
    want = ["PR %08x" % (1 << b) for b in BITS.values()]
    if got != want:
        return f"{got}"


def pad_start_and_circle():
    rc, l = run("padmode 1\n" + frame_for(1 << 11) + "\npadread 1\n" + frame_for(1 << 5) + "\npadread 1\n" + frame_for((1 << 11) | (1 << 5)) + "\npadread 1\n")
    if after(l, "PR ") != ["PR 00000800", "PR 00000020", "PR 00000820"]:
        return f"{after(l, 'PR ')}"


def pad_nothing_pressed_all_pressed():
    rc, l = run("padmode 1\n" + frame_for(0) + "\npadread 1\n" + frame_for(0xFFFF) + "\npadread 1\n")
    if after(l, "PR ") != ["PR 00000000", "PR 0000ffff"]:
        return f"{after(l, 'PR ')}"


def pad_high_half_is_a_port_with_no_controller():
    # whatever port 1 shows, the high half (port 2) has no bit pressed
    rc, l = run("padmode 1\n" + frame_for(0xFFFF) + "\npadread 1\n" + frame_for(0x5555) + "\npadread 0\n" + "padmode 0\npadread 1\n")
    for x in after(l, "PR "):
        if int(x.split()[1], 16) >> 16:
            return f"{after(l, 'PR ')}"


def pad_frames_that_are_not_a_digital_pad():
    cases = ["frame 00 ff 00 00", "frame 00 73 00 00", "frame 01 41 00 00", "frame ff 41 00 00", "frame 00 00 00 00", "frame 00 40 00 00"]
    rc, l = run("padmode 1\n" + "".join(c + "\npadread 1\n" for c in cases))
    if after(l, "PR ") != ["PR 00000000"] * len(cases):
        return f"{after(l, 'PR ')}"


def pad_id_is_not_used():
    rc, l = run("padmode 1\n" + frame_for(1 << 11) + "\n" + "".join("padread %d\n" % i for i in (0, 1, 2, -1, 100000)))
    if after(l, "PR ") != ["PR 00000800"] * 5:
        return f"{after(l, 'PR ')}"


def pad_runtime_read_with_no_button_gives_zero():
    rc, l = run("padmode 0\npadread 1\n")   # input.c's own read without the library: a digital pad with no button pressed
    if after(l, "PR ") != ["PR 00000000"]:
        return f"{l}"


def pad_input_script_is_seen():
    script = Ctx.tmp / "in1.txt"
    script.write_text("0.5 start down\n1.0 start up\n1.5 circle down\n2.0 circle up\n")
    # the frame of the clock is round(seconds * 60): 30, 60, 90, 120
    def at(f):
        return "frames %d\npadread 1\n" % f
    rc, l = run("padmode 0\ninput %s\n" % script + at(29) + at(30) + at(59) + at(60) + at(89) + at(90) + at(119) + at(120))
    if after(l, "IN ") != ["IN 0"]:
        return f"{l}"
    want = ["00000000", "00000800", "00000800", "00000000", "00000000", "00000020", "00000020", "00000000"]
    got = [x.split()[1] for x in after(l, "PR ")]
    if got != want:
        return f"{got}, wanted {want}"


def pad_input_script_over_a_made_up_frame():
    script = Ctx.tmp / "in2.txt"
    script.write_text("0.0 start down\n")
    rc, l = run("padmode 1\n" + frame_for(1 << 6) + "\ninput %s\nframes 5\npadread 1\n" % script)
    if after(l, "PR ") != ["PR 00000840"]:
        return f"{l}"


def pad_input_script_not_applied_to_no_controller():
    script = Ctx.tmp / "in3.txt"
    script.write_text("0.0 start down\n")
    rc, l = run("padmode 1\nframe 00 ff ff ff\ninput %s\nframes 5\npadread 1\n" % script)
    if after(l, "PR ") != ["PR 00000000"]:
        return f"{l}"


def pad_init_and_stop_change_nothing():
    rc, l = run("padmode 1\n" + frame_for((1 << 11) | 3) + "\npadread 1\npadinit 0\npadread 1\npadstop\npadread 1\npadinit 1\npadinit 0\npadread 1\npadstop\npadstop\npadread 1\n")
    if after(l, "PR ") != ["PR 0000080" + "3"] * 5:
        return f"{after(l, 'PR ')}"
    if after(l, "PI ") != ["PI 1"] * 3 or len(after(l, "PS")) != 3:
        return f"{l}"


def pad_read_before_init_works():
    rc, l = run("padmode 1\n" + frame_for(1 << 4) + "\npadread 1\n")
    if after(l, "PR ") != ["PR 00000010"]:
        return f"{l}"


# ---- SetVideoMode -----------------------------------------------------------

def video_mode_returns_the_one_before():
    rc, l = run("vmode 0\nvmode 1\nvmode 1\nvmode 0\nvmode -1\nvmode 2147483647\nvmode 0\n", None)
    want = ["VM 0", "VM 0", "VM 1", "VM 1", "VM 0", "VM -1", "VM 2147483647"]
    if after(l, "VM ") != want:
        return f"{after(l, 'VM ')}, wanted {want}"


NAMES = [read_one_sector, read_three_sectors, read_many_sectors, commands_in_order, mode_low_byte_is_sent,
         handler_removed_and_drive_paused_after_last, sync_before_during_after, sync_waits_by_ticking, result_bytes,
         read_past_the_end_fails, read_ending_exactly_at_the_images_end, second_read_after_a_finished_one,
         second_read_without_a_seek_goes_on_from_the_drive, read_while_one_is_on_pauses_first,
         a_refused_step_returns_zero_and_leaves_nothing, the_drive_is_reading_by_the_status_bits_0xe0, result_of_a_refused_read_is_cleared, no_sectors_arrive_the_wait_runs_out, a_far_divisor_does_not_cut_a_read_off,
         a_sector_at_the_limit_comes_in_a_sector_after_it_does_not, a_late_sector_starts_the_count_again, slow_progress_does_not_run_out,
         fewer_than_one_sector_returns_zero, last_sectors_of_ram_are_accepted, *[stop_function(k) for k in STOPS],
         pad_each_button_in_the_low_half, pad_start_and_circle, pad_nothing_pressed_all_pressed,
         pad_high_half_is_a_port_with_no_controller, pad_frames_that_are_not_a_digital_pad, pad_id_is_not_used,
         pad_runtime_read_with_no_button_gives_zero, pad_input_script_is_seen, pad_input_script_over_a_made_up_frame,
         pad_input_script_not_applied_to_no_controller, pad_init_and_stop_change_nothing, pad_read_before_init_works,
         video_mode_returns_the_one_before]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cc", default=None)
    ap.add_argument("--src", type=Path, default=SRC, help="another copy of the runtime's folder (the mutants run on a scratch copy)")
    ap.add_argument("--only", default="")
    args = ap.parse_args()
    cc = args.cc or shutil.which("cc")
    if not cc:
        print("a C compiler for the host (--cc) is needed; these controls need it")
        return 2
    t0 = time.time()
    failed = count = 0
    with tempfile.TemporaryDirectory(prefix="hostprogcalls-test-") as tmp:
        root = Path(tmp)
        Ctx.tmp = root
        (root / "progmain.c").write_text(DRIVER_C)
        exe = root / "progtest.exe"
        s = args.src
        # input.c without the library: its read of the devices is renamed so that the driver's can call it or supply a frame
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-Dport_pad_host_read=real_pad_host_read", "-I", str(s), "-c",
                               "-o", str(root / "input.o"), str(s / "input.c")], capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL input.c does not build:\n" + proc.stderr[-2000:])
            return 1
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(s), "-o", str(exe), str(root / "progmain.c"), str(s / "progcalls.c"),
                               str(root / "input.o"), str(s / "cd.c"), str(s / "xa.c"), str(s / "disc.c"), str(s / "sha256.c")] + LINK_FLAGS,
                              capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the test program does not build and link without the library:\n" + proc.stderr[-2000:])
            return 1
        Ctx.exe = exe
        Ctx.image = root / "image.bin"
        CD.make_image(Ctx.image, 100)
        Ctx.short = root / "short.bin"
        CD.make_image(Ctx.short, 20)
        for fn in NAMES:
            name = fn.__name__
            if args.only not in name:
                continue
            count += 1
            try:
                bad = fn()
            except subprocess.TimeoutExpired:
                bad = "time limit: the driver did not finish"
            except Exception as e:  # the driver ended, or answered short
                bad = f"{type(e).__name__}: {e}"
            if bad is None:
                print(f"ok   {name}")
            else:
                print(f"FAIL {name}: {bad}")
                failed += 1
    print(f"{count} cases, {time.time() - t0:.1f} seconds")
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
