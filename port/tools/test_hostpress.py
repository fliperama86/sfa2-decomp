#!/usr/bin/env python3
"""Controls for Sony's movie decoder calls (port/src/press.c, press.h) on top of PsyZ's decoder chip core.

    python3 test_hostpress.py --cc CROSS_CC --psyz-build DIR [--src DIR] [--only SUBSTRING]

The test compiles press.c, the port's mdec.c (the bitstream reader and the reset tables) and PsyZ's chip core (src/psyz/mdec.c of the
source copy inside the library build DIR; it is compiled here, nothing of the library is changed) with a driver of its own, which
maps the game's RAM at its own address and stands in for what press.c takes from the runtime: a clock the test sets,
port_handler_depth, port_interrupt_allowed (set by the test), ResetCallback (counted), the table of DMA routines, and port_halt
(prints `stop: ...` and ends the driver with the status it was given; each case that stops runs in a driver of its own). Frames come
from the Python encoder of test_hostmdec.py. The expected codes come from the driver's own direct call of port_mdec_vlc, and the
expected pixels from its own direct calls of the chip (Psyz_MdecCommand, Psyz_MdecRead after the same reset and tables); the chip
and the reader have their own tests, and this file is about the calls. NOTHING IS COMPARED WITH THE CONSOLE'S LIBRARY: the contract
is the text of press.h, and the cases check that the calls do what it says.

Groups: DecDCTvlc (layout, versions, null, broken frames, the RAM's end), DecDCTin (the four modes on two words, the count of
words, each stop), DecDCTout (strips against the chip's pixels, 24 and 15 bits, the mask bit, odd word counts, too much, guards),
the completion (due times, the poll, the timer's claim and delivery, once only, the routine that starts the next transfer, no
routine, an output with a completion pending), DecDCTReset, DecDCToutCallback.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required` or the number that did not.
"""

from __future__ import annotations

import argparse
import random
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostmdec as M  # noqa: E402

SRC = HERE.parent / "src"
LIMIT = 60
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]
BASE, END = 0x80000000, 0x80200000
STATUS = 9   # PORT_EXIT_OTHER, the status of port_halt in a stop of press.c
BS, BUFV, IN, OUT = 0x80010000, 0x80020000, 0x80040000, 0x80100000
US = 55

DRIVER_C = r'''
#include "port.h"
#include "press.h"
#include "mdec.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static unsigned long long clock_us;
static int allowed = 1, resets;
volatile int port_handler_depth;
static void (*dma[7])(void);

/* PsyZ's chip core, called directly for the expected pixels and for the tables the tests set. */
void Psyz_MdecReset(void);
int Psyz_MdecCommand(unsigned int command, const void *data, size_t words);
int Psyz_MdecRead(void *data, size_t words);

unsigned long long port_now_us(void) { return clock_us; }
int port_interrupt_allowed(void) { return allowed; }
void *port_h_ResetCallback(void) { resets++; return 0; }
void *port_h_DMACallback(int d, void (*f)(void))
{
    void (*p)(void);
    if (d < 0 || d > 6) return 0;
    p = dma[d];
    dma[d] = f;
    return (void *)p;
}
void (*port_iopage_dma_callback(int d))(void) { return d < 0 || d > 6 ? 0 : dma[d]; }
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

static unsigned next_addr, next_words;
static void cb_a(void) { printf("cb A t=%llu depth=%d\n", clock_us, port_handler_depth); }
static void cb_b(void)
{
    printf("cb B t=%llu depth=%d\n", clock_us, port_handler_depth);
    port_DecDCTout((unsigned long *)next_addr, (int)next_words);
    printf("cb B started the next\n");
}
static char kind(void (*f)(void)) { return f == 0 ? '0' : f == cb_a ? 'A' : f == cb_b ? 'B' : '?'; }
/* The runtime's check of an address before it is called: here it prints what it was asked about, and the order shows. */
void port_target_check(const char *path, const void *target) { printf("target %s %c\n", path, kind((void (*)(void))target)); }

static int unhex(const char *s, unsigned char *out)
{
    int n = 0;
    if (s[0] == '-') return 0;
    while (s[0] && s[1] && s[0] != '\n' && s[0] != '\r') {
        char t[3] = { s[0], s[1], 0 };
        out[n++] = (unsigned char)strtoul(t, NULL, 16);
        s += 2;
    }
    return n;
}
static void hex(const unsigned char *p, unsigned n)
{
    unsigned i;
    if (!n) printf("-");
    for (i = 0; i < n; i++) printf("%02x", p[i]);
}
/* The chip's reset and the two tables, sent by this driver itself (not by press.c), from port_mdec_reset's values. */
static void ref_reset(void)
{
    struct port_mdec m;
    unsigned char q[128], sc[128];
    int i;
    port_mdec_reset(&m);
    memcpy(q, m.iq_y, 64);
    memcpy(q + 64, m.iq_uv, 64);
    for (i = 0; i < 64; i++) { sc[2 * i] = (unsigned char)((unsigned short)m.scale[i] & 0xFF); sc[2 * i + 1] = (unsigned char)((unsigned short)m.scale[i] >> 8); }
    Psyz_MdecReset();
    if (Psyz_MdecCommand(0x40000001u, q, 32) || Psyz_MdecCommand(0x60000000u, sc, 32)) { printf("ref_reset failed\n"); exit(3); }
}

int main(void)
{
    char *line = malloc(1 << 22);
    unsigned char *tmp = malloc(1 << 21);
    void *ram = VirtualAlloc((void *)(size_t)PORT_RAM_BASE, PORT_RAM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (ram != (void *)(size_t)PORT_RAM_BASE) { printf("no RAM\n"); return 3; }
    ref_reset();
    while (fgets(line, 1 << 22, stdin)) {
        char cmd[16], *h = malloc(1 << 22);
        unsigned a = 0, b = 0, c = 0;
        int at = 0;
        if (sscanf(line, "%15s%n", cmd, &at) != 1) { free(h); continue; }
        if (!strcmp(cmd, "put")) {
            sscanf(line + at, "%x %s", &a, h);
            memcpy((void *)(size_t)a, tmp, (size_t)unhex(h, tmp));
        } else if (!strcmp(cmd, "fill")) {
            sscanf(line + at, "%x %x %x", &a, &b, &c);
            memset((void *)(size_t)a, (int)c, b);
        } else if (!strcmp(cmd, "get")) {
            sscanf(line + at, "%x %x", &a, &b);
            printf("get "); hex((const unsigned char *)(size_t)a, b); printf("\n");
        } else if (!strcmp(cmd, "time")) {
            sscanf(line + at, "%u", &a);
            clock_us = a;
        } else if (!strcmp(cmd, "allowed")) {
            sscanf(line + at, "%u", &a);
            allowed = (int)a;
        } else if (!strcmp(cmd, "cb")) {
            char k;
            void (*f)(void), (*prev)(void);
            sscanf(line + at, " %c", &k);
            f = k == 'A' ? cb_a : k == 'B' ? cb_b : 0;
            prev = port_DecDCToutCallback(f);
            printf("prev %c\n", kind(prev));
        } else if (!strcmp(cmd, "cbat")) {
            int i;
            printf("cbat ");
            for (i = 0; i < 7; i++) printf("%c", kind(dma[i]));
            printf("\n");
        } else if (!strcmp(cmd, "dmacb")) {   /* a routine on another channel, directly */
            sscanf(line + at, "%u", &a);
            port_h_DMACallback((int)a, cb_a);
        } else if (!strcmp(cmd, "next")) {
            sscanf(line + at, "%x %u", &next_addr, &next_words);
        } else if (!strcmp(cmd, "reset")) {
            int m;
            sscanf(line + at, "%d", &m);
            port_DecDCTReset(m);
            printf("reset resets=%d\n", resets);
        } else if (!strcmp(cmd, "tmod")) {   /* other tables, sent to the chip directly: q = quantisation, s = scale */
            char k;
            unsigned char q[128], sc[128];
            sscanf(line + at, " %c", &k);
            memset(q, 1, sizeof q);
            memset(sc, 0, sizeof sc);
            sc[0] = 0x00; sc[1] = 0x20;
            if (k == 'q') Psyz_MdecCommand(0x40000001u, q, 32);
            else Psyz_MdecCommand(0x60000000u, sc, 32);
        } else if (!strcmp(cmd, "refreset")) {
            ref_reset();
        } else if (!strcmp(cmd, "refvlc")) {   /* the reader's codes of a frame: rc, count, codes */
            unsigned used = 0;
            unsigned short *codes = malloc(1 << 20);
            int n, rc;
            sscanf(line + at, "%s", h);
            n = unhex(h, tmp);
            rc = port_mdec_vlc(tmp, (unsigned)n, codes, 1 << 19, &used);
            printf("refvlc %d %u ", rc, used); hex((const unsigned char *)codes, used * 2); printf("\n");
            free(codes);
        } else if (!strcmp(cmd, "mkin")) {   /* at A: the command word, the core's codes, 64 end codes, padding of FE00 to whole blocks */
            unsigned used = 0, total, blocks, i;
            unsigned short *codes = malloc(1 << 20), *w;
            int n, rc;
            sscanf(line + at, "%x %s", &a, h);
            n = unhex(h, tmp);
            rc = port_mdec_vlc(tmp, (unsigned)n, codes, 1 << 19, &used);
            total = used + 64;
            blocks = (total + 63) / 64;
            w = (unsigned short *)(size_t)(a + 4);
            memcpy(w, codes, used * 2);
            for (i = used; i < blocks * 64; i++) w[i] = 0xFE00;
            *(unsigned *)(size_t)a = 0x38000000u | (blocks * 32);
            printf("mkin %d %u %u\n", rc, used, blocks);
            free(codes);
        } else if (!strcmp(cmd, "refpix")) {   /* the chip's pixels of the codes after the word at A, by direct calls (count from the word's low 16 bits) */
            unsigned w, mbw, blocks = 0, written = 0;
            int depth, mask, sgn, fresh;
            unsigned char *px = malloc(1 << 24);
            sscanf(line + at, "%x %d %d %d %d", &a, &depth, &mask, &sgn, &fresh);
            w = 0x20000000u | ((depth == 24 ? 2u : 3u) << 27) | ((unsigned)(mask != 0) << 25) | ((unsigned)(sgn != 0) << 26) | (*(unsigned *)(size_t)a & 0xFFFFu);
            mbw = depth == 24 ? 192 : 128;
            if (fresh) ref_reset();
            if (Psyz_MdecCommand(w, (const void *)(size_t)(a + 4), w & 0xFFFFu)) { printf("refpix command refused\n"); exit(3); }
            while (written + mbw * 4 <= (1u << 24) && Psyz_MdecRead(px + written, mbw) == 0) { written += mbw * 4; blocks++; }
            printf("refpix 0 %u %u ", blocks, written); hex(px, written); printf("\n");
            free(px);
        } else if (!strcmp(cmd, "vlc")) {
            sscanf(line + at, "%x %x", &a, &b);
            printf("vlc %d\n", port_DecDCTvlc((unsigned long *)(size_t)a, (unsigned long *)(size_t)b));
        } else if (!strcmp(cmd, "in")) {
            int m;
            sscanf(line + at, "%x %d", &a, &m);
            port_DecDCTin((unsigned long *)(size_t)a, m);
            printf("in %08x\n", *(unsigned *)(size_t)a);
        } else if (!strcmp(cmd, "out")) {
            int w;
            sscanf(line + at, "%x %d", &a, &w);
            port_DecDCTout((unsigned long *)(size_t)a, w);
            printf("out\n");
        } else if (!strcmp(cmd, "poll")) {
            port_press_poll();
        } else if (!strcmp(cmd, "claim")) {
            printf("claim %d\n", port_press_claim_due());
        } else if (!strcmp(cmd, "deliver")) {
            port_press_deliver_claimed();
        } else {
            printf("unknown command %s\n", cmd);
        }
        printf(".\n");
        fflush(stdout);
        free(h);
    }
    return 0;
}
'''

# press.c as a program without the picture library has it: linked with no chip. The three calls that need one must stop.
NOCHIP_C = r'''
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "press.h"
volatile int port_handler_depth;
static int resets;
unsigned long long port_now_us(void) { return 0; }
int port_interrupt_allowed(void) { return 1; }
void *port_h_ResetCallback(void) { resets++; return 0; }
void *port_h_DMACallback(int d, void (*f)(void)) { (void)d; (void)f; return 0; }
void (*port_iopage_dma_callback(int d))(void) { (void)d; return 0; }
void port_target_check(const char *path, const void *target) { (void)path; (void)target; }
void port_halt(int status, const char *fmt, ...)
{
    va_list ap;
    printf("resets %d\nstop: ", resets);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    exit(status);
}
int main(int argc, char **argv)
{
    static unsigned long word[40];
    if (argc < 2) return 2;
    if (!strcmp(argv[1], "reset0")) port_DecDCTReset(0);
    if (!strcmp(argv[1], "reset1")) port_DecDCTReset(1);
    if (!strcmp(argv[1], "reset7")) port_DecDCTReset(7);
    if (!strcmp(argv[1], "in")) port_DecDCTin(word, 1);
    if (!strcmp(argv[1], "out")) port_DecDCTout(word, 32);
    if (!strcmp(argv[1], "callback")) printf("callback %p\n", (void *)port_DecDCToutCallback(0));
    printf("returned\n");
    return 0;
}
'''

NAMES: list = []


def case(fn):
    NAMES.append(fn)
    return fn


class Ctx:
    exe: Path
    nochip: Path


def run(cmds):
    """Runs the driver on the commands. Returns (status, groups, rest): the lines each command printed, and the lines after the last
    finished command (the line of a stop)."""
    p = subprocess.run([str(Ctx.exe)], input=("\n".join(cmds) + "\n").encode(), capture_output=True, timeout=LIMIT)
    groups, cur = [], []
    for l in p.stdout.decode().splitlines():
        if l == ".":
            groups.append(cur)
            cur = []
        else:
            cur.append(l)
    return p.returncode, groups, cur


def ok_run(cmds):
    st, groups, rest = run(cmds)
    if st != 0 or len(groups) != len(cmds):
        raise RuntimeError(f"the driver ended with status {st} after {len(groups)} of {len(cmds)} commands: {rest}")
    return groups


def stops(cmds, call, *words):
    """None when the commands end in a stop whose line begins `stop: CALL` and holds every word; else why not."""
    st, groups, rest = run(cmds)
    line = rest[0] if rest else ""
    if st != STATUS:
        return f"status {st}, expected {STATUS} (line {line!r}; {len(groups)} commands finished)"
    if not line.startswith(f"stop: {call}:"):
        return f"stop line {line!r}, expected it to begin `stop: {call}:`"
    for w in words:
        if w not in line:
            return f"stop line {line!r} lacks {w!r}"
    return None


def eq(got, want):
    return None if got == want else f"got {got!r}, expected {want!r}"


# ---- frames ---------------------------------------------------------------------------------------------------------------

def frame(version=2, nmb=3, seed=1, quant=2, size_field=0x123, ident=0x3800):
    rnd = random.Random(seed * 100 + version)
    mbs = []
    for _ in range(nmb):
        mb = []
        for _b in range(6):
            dc = rnd.randrange(-60, 60) * (4 if version == 3 else 1)
            pairs = [(rnd.randrange(0, 4), rnd.choice([-1, 1]) * rnd.randrange(1, 5)) for _ in range(rnd.randrange(0, 4))]
            mb.append((dc, pairs))
        mbs.append(mb)
    bits, _codes = M.encode_frame(version, quant, mbs)
    return M.frame_bytes(version, quant, bits, size_field=size_field, ident=ident)


def refvlc(fb):
    t = ok_run([f"refvlc {fb.hex()}"])[0][0].split()
    return int(t[1]), int(t[2]), bytes.fromhex(t[3]) if len(t) > 3 and t[3] != "-" else b""


def word(bits_set, size):
    return (bits_set & ~0xFFFF) | size


def px_of(line):
    t = line.split()
    return int(t[1]), int(t[2]), int(t[3]), bytes.fromhex(t[4]) if len(t) > 4 else b""


def getbytes(line):
    t = line.split()
    return bytes.fromhex(t[1]) if t[1] != "-" else b""


def mk_and_in(version=2, nmb=3, mode=1, seed=1, t0=1000):
    """Commands that put a frame's codes at IN, then DecDCTin it at time t0. Returns (commands, the frame)."""
    fb = frame(version, nmb, seed)
    return [f"mkin {IN:x} {fb.hex()}", f"time {t0}", f"in {IN:x} {mode}"], fb


# ---- DecDCTvlc ------------------------------------------------------------------------------------------------------------

def vlc_layout(version):
    fb = frame(version, 4, 3)
    rc, used, codes = refvlc(fb)
    g = ok_run([f"fill {BUFV:x} 2000 a5", f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {BUFV:x}", f"get {BUFV:x} 2000", f"get {BS:x} {len(fb):x}"])
    want = fb[:4] + codes + b"\x00\xfe" * 64 + b"\xa5" * (0x2000 - 4 - len(codes) - 128)
    if rc != 0 or used < 30:
        return f"the reference failed ({rc}, {used} codes)"
    return eq((g[2][0], getbytes(g[3][0]) == want, getbytes(g[4][0]) == fb), ("vlc 0", True, True))


@case
def vlc_layout_version_2():
    return vlc_layout(2)


@case
def vlc_layout_version_3():
    return vlc_layout(3)


@case
def vlc_layout_version_1():
    return vlc_layout(1)


@case
def vlc_unknown_version_writes_the_header_only():
    for v in (0, 4, 7):
        fb = frame(2, 2, 1)
        fb = fb[:6] + v.to_bytes(2, "little") + fb[8:]
        g = ok_run([f"fill {BUFV:x} 400 a5", f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {BUFV:x}", f"get {BUFV:x} 400"])
        got = getbytes(g[3][0])
        bad = eq((g[2][0], got), ("vlc -1", fb[:4] + b"\xa5" * (0x400 - 4)))
        if bad:
            return f"version {v}: {bad}"
    return None


@case
def vlc_null_bitstream_stops():
    return stops([f"vlc 0 {BUFV:x}"], "DecDCTvlc", "null")


@case
def vlc_broken_frame_stops_with_the_cores_result():
    fb = frame(2, 2, 1, ident=0x3801)
    bad = stops([f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {BUFV:x}"], "DecDCTvlc", "-1")
    if bad:
        return "header: " + bad
    fb = frame(2, 3, 1)
    bad = stops([f"put {BS:x} {fb[:len(fb) // 2].hex()}", f"vlc {BS:x} {BUFV:x}"], "DecDCTvlc")
    if bad:
        return "truncated: " + bad
    return None


@case
def vlc_bitstream_header_at_the_rams_end():
    fb = frame(2, 2, 1)
    # the 8-byte header only partly inside the RAM: the first halfword at END - 4
    return stops([f"vlc {END - 4:x} {BUFV:x}"], "DecDCTvlc", "not inside")


@case
def vlc_frame_ending_at_the_rams_end_is_read():
    fb = frame(2, 2, 1)
    at = END - len(fb)
    rc, used, codes = refvlc(fb)
    g = ok_run([f"put {at:x} {fb.hex()}", f"vlc {at:x} {BUFV:x}", f"get {BUFV:x} {4 + len(codes):x}"])
    return eq((g[1][0], getbytes(g[2][0])), ("vlc 0", fb[:4] + codes))


@case
def vlc_buf_so_near_the_rams_end_that_the_codes_do_not_fit():
    fb = frame(2, 3, 1)
    rc, used, codes = refvlc(fb)
    fits = END - 4 - 128 - used * 2
    g = ok_run([f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {fits:x}", f"get {fits:x} {END - fits:x}"])
    want = fb[:4] + codes + b"\x00\xfe" * 64
    bad = eq((g[1][0], getbytes(g[2][0])), ("vlc 0", want))
    if bad:
        return "exact fit: " + bad
    bad = stops([f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {fits + 2:x}"], "DecDCTvlc", "-5")
    if bad:
        return "one code short: " + bad
    bad = stops([f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {END - 100:x}"], "DecDCTvlc", "not inside")
    if bad:
        return "no room for the end codes: " + bad
    return None


@case
def vlc_buf_outside_the_ram_stops():
    fb = frame(2, 2, 1)
    return stops([f"put {BS:x} {fb.hex()}", f"vlc {BS:x} {BASE - 0x100:x}"], "DecDCTvlc", "not inside")


# ---- DecDCTin -------------------------------------------------------------------------------------------------------------

def after_mode(w, mode):
    w &= ~((1 << 27) | (1 << 25))
    return w | ((0 if mode & 1 else 1) << 27) | ((1 << 25) if mode & 2 else 0)


@case
def in_the_word_after_each_mode():
    fb = frame(2, 2, 2)
    for name, base in (("others set", 0x3FFF0000 & ~((1 << 27) | (1 << 25)) | (1 << 26)), ("others clear", 0x30000000)):
        for mode in range(4):
          for start25 in (0, 1):
            for start27 in (0, 1):
                w0 = base | (start27 << 27) | (start25 << 25)
                g = ok_run([f"mkin {IN:x} {fb.hex()}", f"time 1", f"get {IN:x} 4"])
                size = int.from_bytes(getbytes(g[2][0]), "little") & 0xFFFF
                w0 = word(w0, size)
                g = ok_run([f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {w0.to_bytes(4, 'little').hex()}", f"in {IN:x} {mode}", f"get {IN:x} 4"])
                want = after_mode(w0, mode)
                got_in = int(g[2][0].split()[1], 16)
                got_mem = int.from_bytes(getbytes(g[3][0]), "little")
                if (got_in, got_mem) != (want, want):
                    return f"{name}, mode {mode}, bit 27 at start {start27}, bit 25 at start {start25}: word {got_in:08x} / memory {got_mem:08x}, expected {want:08x}"
    return None


@case
def in_decodes_the_depth_and_flags_of_the_word_after_the_mode():
    fb = frame(2, 2, 2)
    # mode 1 on the library's word 0x3800 : 24 bits, unsigned, no mask; mode 0: 15 bits; mode 2: 15 bits with the mask; mode 3: 24 bits with the mask bit (ignored)
    out = {}
    for mode, depth, mask in ((1, 24, 0), (0, 15, 0), (2, 15, 1), (3, 24, 1)):
        g = ok_run([f"mkin {IN:x} {fb.hex()}", "time 5", f"in {IN:x} {mode}", f"out {OUT:x} 0", f"refpix {IN:x} {depth} {mask} 0 0"])
        rc, blocks, written, px = px_of(g[4][0])
        g2 = ok_run([f"mkin {IN:x} {fb.hex()}", "time 5", f"in {IN:x} {mode}", f"out {OUT:x} {(written // 128) * 32}", f"get {OUT:x} {written:x}"])
        if rc != 0 or getbytes(g2[4][0]) != px:
            return f"mode {mode}: pixels differ from the core's {depth} bit, mask {mask}"
    return None


@case
def in_signed_bit_gives_signed_pixels():
    fb = frame(2, 2, 2)
    g = ok_run([f"mkin {IN:x} {fb.hex()}", f"get {IN:x} 4", f"refpix {IN:x} 24 0 1 0", f"refpix {IN:x} 24 0 0 0"])
    size = int.from_bytes(getbytes(g[1][0]), "little") & 0xFFFF
    rc, blocks, written, px_signed = px_of(g[2][0])
    px_plain = px_of(g[3][0])[3]
    if px_signed == px_plain:
        return "the reference does not tell signed from unsigned"
    w = 0x38000000 | (1 << 26) | size
    g = ok_run([f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {w.to_bytes(4, 'little').hex()}", "time 1", f"in {IN:x} 1", f"out {OUT:x} {written // 4}", f"get {OUT:x} {written:x}"])
    return eq(getbytes(g[5][0]) == px_signed, True)


@case
def in_hands_the_chip_the_count_of_words_the_low_16_bits_say():
    fb = frame(2, 3, 1)
    g = ok_run([f"mkin {IN:x} {fb.hex()}", f"get {IN:x} 4"])
    blocks = int(g[0][0].split()[3])
    full = int.from_bytes(getbytes(g[1][0]), "little")
    if blocks < 2:
        return "the frame is too small for this case"
    # the last unit of 32 words is only padding (FE00): the count one unit short, with garbage in that unit, decodes the same frame;
    # a count that reaches into the codes does not (the chip then runs out of codes: DecDCTout stops)
    short = (full & ~0xFFFF) | (32 * (blocks - 1))
    garbage = (b"\xff\xff" * 64).hex()
    cmds = [f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {short.to_bytes(4, 'little').hex()}", f"put {IN + 4 + 128 * (blocks - 1):x} {garbage}",
            "time 1", f"in {IN:x} 1", f"refpix {IN:x} 24 0 0 0"]
    g = ok_run(cmds)
    rc, nb, written, px = px_of(g[5][0])
    g = ok_run(cmds[:5] + [f"out {OUT:x} {(written // 128) * 32}", f"get {OUT:x} {written:x}"])
    bad = eq((rc, getbytes(g[6][0]) == px, written > 0), (0, True, True))
    if bad:
        return "one unit short: " + bad
    cut = (full & ~0xFFFF) | 32
    return stops([f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {cut.to_bytes(4, 'little').hex()}", "time 1", f"in {IN:x} 1", f"out {OUT:x} {3 * 192}"], "DecDCTout")


@case
def in_stop_count_not_a_multiple_of_32_words():
    fb = frame(2, 2, 2)
    g = ok_run([f"mkin {IN:x} {fb.hex()}", f"get {IN:x} 4"])
    full = int.from_bytes(getbytes(g[1][0]), "little")
    for count in (1, 16, 17, 31, 33, 48, (full & 0xFFFF) + 1):
        w = (full & ~0xFFFF) | count
        bad = stops([f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {w.to_bytes(4, 'little').hex()}", f"in {IN:x} 1"], "DecDCTin", "multiple of 32", str(count))
        if bad:
            return f"count {count}: {bad}"
    return None


@case
def in_stop_not_a_decode():
    fb = frame(2, 2, 2)
    for top in (0x00000000, 0x40000000, 0x60000000, 0xE0000000, 0x80000000):
        g = ok_run([f"mkin {IN:x} {fb.hex()}", f"get {IN:x} 4"])
        size = int.from_bytes(getbytes(g[1][0]), "little") & 0xFFFF
        w = top | (3 << 27) | size
        bad = stops([f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {w.to_bytes(4, 'little').hex()}", f"in {IN:x} 1"], "DecDCTin", "not a decode")
        if bad:
            return f"top bits {top >> 29:03b}: {bad}"
    return None


@case
def in_stop_monochrome_depths():
    fb = frame(2, 2, 2)
    g = ok_run([f"mkin {IN:x} {fb.hex()}", f"get {IN:x} 4"])
    size = int.from_bytes(getbytes(g[1][0]), "little") & 0xFFFF
    w = 0x20000000 | size   # bit 28 clear: depth field 0 (mode 1) or 1 (mode 0)
    for mode in (1, 0):
        bad = stops([f"mkin {IN:x} {fb.hex()}", f"put {IN:x} {w.to_bytes(4, 'little').hex()}", f"in {IN:x} {mode}"], "DecDCTin", "monochrome")
        if bad:
            return f"mode {mode}: {bad}"
    return None


@case
def out_stop_codes_that_end_inside_a_macroblock():
    # five blocks of a macroblock, then padding: the chip runs out of codes in the sixth block, and the read fails
    codes = b"".join(((2 << 10) | 5).to_bytes(2, "little") + b"\x00\xfe" for _ in range(5)) + b"\x00\xfe" * (64 - 10)
    w = 0x38000000 | 32
    return stops([f"put {IN:x} {(w.to_bytes(4, 'little') + codes).hex()}", "time 1", f"in {IN:x} 1", f"out {OUT:x} 32"], "DecDCTout", "chip")


@case
def in_stop_an_output_transfer_is_pending():
    cmds, fb = mk_and_in(nmb=1)
    return stops(cmds + [f"out {OUT:x} {768 // 4}", f"mkin {IN:x} {fb.hex()}", f"in {IN:x} 1"], "DecDCTin", "pending")


@case
def in_next_frame_after_everything_was_handed_out_and_taken():
    cmds, fb = mk_and_in(nmb=1)
    g = ok_run(cmds + [f"out {OUT:x} {768 // 4}", "time 100000", "poll", f"mkin {IN:x} {fb.hex()}", f"in {IN:x} 1"])
    return None if g[-1][0].startswith("in ") else str(g[-1])


@case
def in_codes_or_word_outside_the_ram_stop():
    fb = frame(2, 2, 2)
    # the word at the very end of RAM with a count of codes that runs past it
    w = 0x38000000 | 32
    bad = stops([f"put {END - 4:x} {w.to_bytes(4, 'little').hex()}", f"in {END - 4:x} 1"], "DecDCTin", "not inside")
    if bad:
        return "codes past the end: " + bad
    # the codes start 100 bytes before the end: the 128 bytes of 32 words do not fit, 64 (half) would
    bad = stops([f"put {END - 104:x} {w.to_bytes(4, 'little').hex()}", f"in {END - 104:x} 1"], "DecDCTin", "not inside")
    if bad:
        return "codes half past the end: " + bad
    bad = stops([f"in {BASE - 4:x} 1"], "DecDCTin", "not inside")
    if bad:
        return "word below the RAM: " + bad
    return stops(["in 0 1"], "DecDCTin", "not inside")


# ---- DecDCTout ------------------------------------------------------------------------------------------------------------

def strips_case(depth, mode, mask, words_seq, nmb=5):
    fb = frame(2, nmb, 4)
    gap = max(128 * (w >> 5) for w in words_seq) + 0x100
    area = 0x200 + gap * len(words_seq) + 0x200
    cmds = [f"mkin {IN:x} {fb.hex()}", f"refpix {IN:x} {depth} {mask} 0 0", "time 7", f"fill {OUT:x} {area:x} a5", f"in {IN:x} {mode}"]
    at = 0x200
    for w in words_seq:
        cmds += [f"out {OUT + at:x} {w}", f"get {OUT:x} {area:x}"]
        at += gap
    g = ok_run(cmds)
    rc, blocks, written, px = px_of(g[1][0])
    if rc != 0 or blocks != nmb:
        return f"the reference failed: {rc}, {blocks}"
    pos, at = 0, 0x200
    mem = bytearray(b"\xa5" * area)
    for k, w in enumerate(words_seq):
        n = 128 * (w >> 5)
        mem[at:at + n] = px[pos:pos + n]
        pos += n
        got = getbytes(g[5 + 2 * k + 1][0])
        if got != bytes(mem):
            first = next((i for i in range(min(len(got), area)) if got[i] != mem[i]), -1)
            return f"strip {k} ({w} words): memory differs from the core's pixels cut at the same place, first at offset {first:#x}"
        at += gap
    return None


@case
def out_strips_equal_the_cores_pixels_24_bit():
    return strips_case(24, 1, 0, [192, 32, 160, 96, 64, 64, 256, 32, 64])


@case
def out_strips_equal_the_cores_pixels_15_bit_without_mask():
    return strips_case(15, 0, 0, [128, 32, 96, 160, 64, 32, 128])


@case
def out_strips_equal_the_cores_pixels_15_bit_with_mask():
    return strips_case(15, 2, 1, [128, 32, 96, 160, 64, 32, 128])


@case
def out_whole_frame_in_one_strip_and_in_strips_of_one_block():
    bad = strips_case(24, 1, 0, [3 * 192], nmb=3)
    if bad:
        return "one strip: " + bad
    return strips_case(24, 1, 0, [32] * 18, nmb=3)


@case
def out_words_not_a_multiple_of_32_hand_out_whole_blocks_only():
    bad = strips_case(24, 1, 0, [50, 33, 63, 31, 64, 32])
    if bad:
        return bad
    # 31 words hand out nothing and move nothing: the next 32 words are the first block
    fb = frame(2, 2, 4)
    g = ok_run([f"mkin {IN:x} {fb.hex()}", f"refpix {IN:x} 24 0 0 0", "time 1", f"in {IN:x} 1", f"fill {OUT:x} 400 a5", f"out {OUT:x} 31", f"get {OUT:x} 400",
                f"out {OUT:x} 32", f"get {OUT:x} 80"])
    px = px_of(g[1][0])[3]
    return eq((getbytes(g[6][0]) == b"\xa5" * 0x400, getbytes(g[8][0]) == px[:128]), (True, True))


@case
def out_stop_more_than_is_left():
    cmds, fb = mk_and_in(nmb=1)
    bad = stops(cmds + [f"out {OUT:x} {768 // 4 + 32}"], "DecDCTout", "chip")
    if bad:
        return "at once: " + bad
    bad = stops(cmds + [f"out {OUT:x} 96", f"out {OUT:x} 128"], "DecDCTout", "chip")
    if bad:
        return "after a strip: " + bad
    g = ok_run(cmds + [f"out {OUT:x} {768 // 4}"])
    return None if g[-1] == ["out"] else f"exactly what is left was refused: {g[-1]}"


@case
def out_stop_before_any_frame_and_negative_words():
    bad = stops([f"out {OUT:x} 32"], "DecDCTout", "chip")
    if bad:
        return "no frame: " + bad
    cmds, fb = mk_and_in(nmb=1)
    return stops(cmds + [f"out {OUT:x} -32"], "DecDCTout", "-32 words")


@case
def out_stop_strip_outside_the_ram():
    cmds, fb = mk_and_in(nmb=1)
    bad = stops(cmds + [f"out {END - 64:x} 32"], "DecDCTout", "not inside")
    if bad:
        return "past the end: " + bad
    g = ok_run(cmds + [f"out {END - 128:x} 32", f"get {END - 128:x} 80"])
    if g[-2] != ["out"]:
        return "a strip ending exactly at the end was refused"
    return stops(cmds + [f"out {BASE - 64:x} 32"], "DecDCTout", "not inside")


# ---- the completion -------------------------------------------------------------------------------------------------------

def three(mode_cb="A", t0=1000, nmb=3):
    cmds, fb = mk_and_in(nmb=nmb, t0=t0)
    return ([f"cb {mode_cb}"] if mode_cb else []) + cmds


STRIP = 192   # words of one macroblock in 24 bits


def due(k, t0=1000):
    return t0 + k * US


def cbs(g):
    return [l for grp in g for l in grp if l.startswith("cb ") and " t=" in l]


def claims(g):
    return [l for grp in g for l in grp if l.startswith("claim ")]


def started(g):
    return [l for grp in g for l in grp if l == "cb B started the next"]


@case
def completion_not_taken_before_its_due_time():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1) - 1}", "poll", "claim", "deliver", "poll"])
    return eq((cbs(g), claims(g)), ([], ["claim 0"]))


@case
def without_the_library_the_calls_that_need_the_chip_stop_by_name():
    # press.c built as a program without the picture library has it (no define, no chip in the link). The reset, the input
    # and the output stop with a line that names the call, before anything else is done (the reset before ResetCallback);
    # a wrong mode is still the first thing said; the callback's registration needs no chip.
    def run_one(what):
        p = subprocess.run([str(Ctx.nochip), what], capture_output=True, timeout=LIMIT)
        return p.returncode, p.stdout.decode(errors="replace").splitlines()
    line = "this program was built without the picture library, which has the decoder's chip"
    want = {"reset0": (STATUS, ["resets 0", "stop: DecDCTReset: " + line]),
            "reset1": (STATUS, ["resets 0", "stop: DecDCTReset: " + line]),
            "reset7": (STATUS, ["resets 0", "stop: DecDCTReset: mode 7 is not 0 or 1"]),
            "in": (STATUS, ["resets 0", "stop: DecDCTin: " + line]),
            "out": (STATUS, ["resets 0", "stop: DecDCTout: " + line])}
    got = {what: run_one(what) for what in want}
    callback = run_one("callback")
    if callback[0] != 0 or callback[1][-1:] != ["returned"]:
        return f"the callback's registration: {callback}"
    return eq(got, want)


@case
def completion_checks_the_routines_address_before_calling_it():
    # The runtime's check of a call target is asked about the registered routine, once, right before the call, by the
    # poll and by the timer's way alike; with no routine registered it is not asked at all.
    c = three()
    by_poll = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll"])
    by_timer = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "claim", "deliver"])
    want = ["target decoder output callback A", f"cb A t={due(1)} depth=1"]
    seen = lambda g: [l for grp in g for l in grp if l.startswith(("target ", "cb "))]
    none = ok_run(three(mode_cb="") + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll"])
    return eq((seen(by_poll), seen(by_timer), seen(none)), (want, want, []))


@case
def completion_taken_once_at_its_due_time_by_the_poll():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll", "poll", "claim", "deliver", f"time {due(1) + 5000}", "poll"])
    return eq((cbs(g), claims(g)), ([f"cb A t={due(1)} depth=1"], ["claim 0"]))


@case
def completion_due_time_follows_the_macroblocks_handed_out_rounded_up():
    c = three(nmb=3)
    # a strip of 32 words is a sixth of a macroblock: one macroblock rounded up; 2 more strips... cumulative 1/6, then 7/6 -> 2
    g = ok_run(c + [f"out {OUT:x} 32", f"time {due(1) - 1}", "poll", f"time {due(1)}", "poll", f"out {OUT:x} {STRIP}",
                    f"time {due(2) - 1}", "poll", f"time {due(2)}", "poll"])
    return eq(cbs(g), [f"cb A t={due(1)} depth=1", f"cb A t={due(2)} depth=1"])


@case
def completion_in_15_bit_uses_the_macroblocks_of_that_depth():
    cmds, fb = mk_and_in(nmb=3, mode=0, t0=1000)
    g = ok_run(["cb A"] + cmds + [f"out {OUT:x} 128", f"time {due(1) - 1}", "poll", f"time {due(1)}", "poll", f"out {OUT:x} 129",
                                  f"time {due(2) - 1}", "poll", f"time {due(2)}", "poll", f"out {OUT:x} 128", f"time {due(3) - 1}", "poll"])
    return eq(cbs(g), [f"cb A t={due(1)} depth=1", f"cb A t={due(2)} depth=1"])


@case
def completion_taken_once_by_claim_and_delivery():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "claim", "claim", "poll", "deliver", "deliver", "poll", "claim"])
    return eq((claims(g), cbs(g)), (["claim 1", "claim 0", "claim 0"], [f"cb A t={due(1)} depth=1"]))


@case
def completion_claim_not_before_the_due_time():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1) - 1}", "claim", "deliver", f"time {due(1)}", "claim", "deliver"])
    return eq((claims(g), cbs(g)), (["claim 0", "claim 1"], [f"cb A t={due(1)} depth=1"]))


@case
def completion_poll_then_claim_takes_it_once():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll", "claim", "deliver"])
    return eq((claims(g), cbs(g)), (["claim 0"], [f"cb A t={due(1)} depth=1"]))


@case
def completion_claim_then_poll_takes_it_once():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "claim", "poll", "deliver", "poll"])
    return eq((claims(g), cbs(g)), (["claim 1"], [f"cb A t={due(1)} depth=1"]))


@case
def completion_handler_depth_is_raised_around_the_routine_and_lowered_after():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll", f"out {OUT:x} {STRIP}", f"time {due(2)}", "claim", "deliver"])
    return eq(cbs(g), [f"cb A t={due(1)} depth=1", f"cb A t={due(2)} depth=1"])


@case
def completion_third_strip_due_time_does_not_depend_on_when_the_first_two_were_taken():
    base = three()
    # taken on time
    g = ok_run(base + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll", f"out {OUT:x} {STRIP}", f"time {due(2)}", "poll", f"out {OUT:x} {STRIP}",
                       f"time {due(3) - 1}", "poll", f"time {due(3)}", "poll"])
    bad = eq(cbs(g)[2:], [f"cb A t={due(3)} depth=1"])
    if bad:
        return "on time: " + bad
    # the first two taken late: the third is due at once when the clock is already past it
    g = ok_run(base + [f"out {OUT:x} {STRIP}", f"time {due(1) + 300}", "poll", f"out {OUT:x} {STRIP}", f"time {due(2) + 300}", "poll",
                       f"out {OUT:x} {STRIP}", "poll"])
    bad = eq(len(cbs(g)), 3)
    if bad:
        return "late (its due time was already past, so the third comes at the next poll): " + bad
    return None


@case
def completion_due_time_does_not_hang_on_the_time_of_the_out_call():
    c = three()
    # DecDCTin at 1000, the first out only at 5000 : still due at 1000 + 55
    g = ok_run(c + ["time 5000", f"out {OUT:x} {STRIP}", "poll"])
    return eq(cbs(g), ["cb A t=5000 depth=1"])


@case
def completion_routine_starting_the_next_transfer_leaves_it_pending():
    c = three("B")
    g = ok_run(c + [f"next {OUT + 0x1000:x} {STRIP}", f"out {OUT:x} {STRIP}", f"time {due(2) + 9}", "poll", "poll", f"time {due(2) + 9}", "poll"])
    # the first poll takes completion 1 (due long past); B starts transfer 2 from inside; the second poll takes 2 (also past) and B
    # starts 3, whose due time is still ahead; the third poll finds nothing due
    return eq((len(cbs(g)), len(started(g))), (2, 2))


@case
def completion_routine_starting_the_next_is_not_taken_in_the_same_call():
    c = three("B")
    g = ok_run(c + [f"next {OUT + 0x1000:x} {STRIP}", f"out {OUT:x} {STRIP}", f"time {due(5)}", "poll"])
    # the clock is past the due times of every strip, and one poll still runs the routine once (and the routine started the next)
    return eq((len(cbs(g)), len(started(g))), (1, 1))


@case
def completion_routine_that_starts_the_next_by_the_timers_delivery():
    c = three("B", nmb=4)
    g = ok_run(c + [f"next {OUT + 0x1000:x} {STRIP}", f"out {OUT:x} {STRIP}", f"time {due(5)}", "claim", "deliver", "poll", "claim", "deliver"])
    return eq((claims(g), len(cbs(g)), len(started(g))), (["claim 1", "claim 1"], 3, 3))


@case
def completion_no_routine_registered_drops_it_and_clears_the_state():
    c = three(None)
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll", "claim", f"out {OUT:x} {STRIP}", f"time {due(2)}", "claim", "deliver"])
    # the first completion is dropped by the poll (state cleared: no claim, and the next out does not stop); the second by the timer's delivery
    return eq((cbs(g), claims(g), [x for grp in g for x in grp if x == "out"]), ([], ["claim 0", "claim 1"], ["out", "out"]))


@case
def completion_routine_removed_between_out_and_due_drops_it():
    c = three("A")
    g = ok_run(c + [f"out {OUT:x} {STRIP}", "cb 0", f"time {due(1)}", "poll", "poll"])
    return eq(cbs(g), [])


@case
def completion_second_frame_counts_from_its_own_input():
    c = three(nmb=3)
    fb = frame(2, 3, 2)
    g = ok_run(c + [f"out {OUT:x} {3 * STRIP}", f"time {due(3)}", "poll", "time 9000", f"mkin {IN:x} {fb.hex()}", f"in {IN:x} 1",
                    f"out {OUT:x} 32", "time 9054", "poll", "time 9055", "poll"])
    return eq(cbs(g), [f"cb A t={due(3)} depth=1", "cb A t=9055 depth=1"])


@case
def completion_routine_is_the_one_of_channel_1_only():
    c = three(None)
    g = ok_run(c + ["dmacb 4", "dmacb 0", f"out {OUT:x} {STRIP}", f"time {due(1)}", "poll"])
    return eq(cbs(g), [])


@case
def completion_out_with_one_pending_and_interrupts_allowed_takes_it_first():
    c = three()
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1) - 30}", "allowed 1", f"out {OUT:x} {STRIP}", "poll", f"time {due(2)}", "poll", f"time {due(2)}", "poll"])
    # the first completion is taken inside the second out (before its due time), at the clock of that call, with the depth raised
    return eq(cbs(g), [f"cb A t={due(1) - 30} depth=1", f"cb A t={due(2)} depth=1"])


@case
def completion_out_with_one_pending_and_interrupts_not_allowed_stops():
    c = three()
    return stops(c + [f"out {OUT:x} {STRIP}", "allowed 0", f"out {OUT:x} {STRIP}"], "DecDCTout", "no interruption is allowed")


@case
def completion_out_inside_the_routine_is_a_new_transfer_not_a_stop():
    c = three("B")
    g = ok_run(c + [f"next {OUT + 0x1000:x} {STRIP}", f"out {OUT:x} {STRIP}", "allowed 0", f"time {due(1)}", "poll"])
    # the routine's own DecDCTout finds no completion pending (the state was cleared first), so it is a new transfer and not a stop,
    # although interruptions are not allowed there
    return eq((cbs(g), len(started(g))), ([f"cb B t={due(1)} depth=1"], 1))


# ---- DecDCTReset ----------------------------------------------------------------------------------------------------------

def decode_after(prefix, fb=None, nmb=3):
    """The pixels of a frame that DecDCTin and DecDCTout give after the commands in `prefix`."""
    fb = fb or frame(2, nmb, 5)
    g = ok_run(prefix + [f"mkin {IN:x} {fb.hex()}", "time 1", f"in {IN:x} 1", f"out {OUT:x} {nmb * STRIP}", f"get {OUT:x} {nmb * 768:x}"])
    return getbytes(g[-1][0])


def direct_pixels(prefix, fb=None, nmb=3):
    """The pixels of the same frame from the driver's own direct calls of the chip, after the commands in `prefix`."""
    fb = fb or frame(2, nmb, 5)
    g = ok_run(prefix + [f"mkin {IN:x} {fb.hex()}", f"refpix {IN:x} 24 0 0 0"])
    rc, blocks, written, px = px_of(g[-1][0])
    return px if (rc, blocks) == (0, nmb) else None


@case
def reset_0_calls_resetcallback_once_and_puts_the_tables_back():
    want = direct_pixels(["refreset"])
    if not want:
        return "the reference failed"
    for what in ("q", "s"):
        changed = direct_pixels([f"tmod {what}"])
        if changed == want:
            return f"the test cannot see a change of the {what} table"
        g = ok_run([f"tmod {what}", "reset 0"])
        if g[1] != ["reset resets=1"]:
            return f"table {what}: {g[1]}"
        got = decode_after([f"tmod {what}", "reset 0"])
        if got != want:
            return f"table {what}: a decode after reset 0 differs from a decode after the driver's own reset and tables"
    return None


@case
def reset_0_after_both_tables_changed():
    want = direct_pixels(["refreset"])
    return eq(decode_after(["tmod q", "tmod s", "reset 0"]) == want, True)


@case
def reset_1_does_not_call_resetcallback_and_leaves_the_tables():
    want = direct_pixels(["refreset"])
    for what in ("q", "s"):
        changed = direct_pixels([f"tmod {what}"])
        g = ok_run([f"tmod {what}", "reset 1"])
        if g[1] != ["reset resets=0"]:
            return f"table {what}: {g[1]}"
        got = decode_after([f"tmod {what}", "reset 1"])
        if got != changed or got == want:
            return f"table {what}: the decode after reset 1 is not the decode with the changed table (equal to reset-tables pixels: {got == want})"
    return None


@case
def reset_both_modes_drop_a_pending_completion():
    for mode in (0, 1):
        c = three("A")
        fb = frame(2, 3, 1)
        g = ok_run(c + [f"out {OUT:x} {STRIP}", f"reset {mode}", f"time {due(9)}", "poll", "claim", "deliver", f"mkin {IN:x} {fb.hex()}", f"in {IN:x} 1", f"out {OUT:x} {STRIP}"])
        bad = eq((cbs(g), g[-1], claims(g)), ([], ["out"], ["claim 0"]))
        if bad:
            return f"mode {mode}: {bad}"
    return None


@case
def reset_drops_a_completion_the_timer_has_claimed():
    c = three("A")
    g = ok_run(c + [f"out {OUT:x} {STRIP}", f"time {due(1)}", "claim", "reset 1", "deliver", "poll"])
    return eq((cbs(g), claims(g)), ([], ["claim 1"]))


@case
def reset_the_chip_forgets_the_frame_so_out_stops_until_a_new_frame():
    cmds, fb = mk_and_in()
    return stops(cmds + ["reset 1", f"out {OUT:x} 32"], "DecDCTout", "chip")


@case
def reset_another_mode_stops():
    for m in (2, -1, 3, 100):
        bad = stops([f"reset {m}"], "DecDCTReset", str(m))
        if bad:
            return f"mode {m}: {bad}"
    return None


@case
def reset_does_not_remove_the_registered_routine():
    c = three("A")
    g = ok_run(c + [f"out {OUT:x} {STRIP}", "reset 0", "cbat"])
    return eq(g[-1][0], "cbat 0A00000")


# ---- DecDCToutCallback ----------------------------------------------------------------------------------------------------

@case
def callback_registers_on_channel_1_and_returns_the_one_before():
    g = ok_run(["cbat", "cb A", "cbat", "cb B", "cbat", "cb 0", "cbat", "dmacb 4", "cb A", "cbat"])
    # channel 4 holds a routine of its own throughout: the calls change channel 1 only
    return eq([g[0][0], g[1][0], g[2][0], g[3][0], g[4][0], g[5][0], g[6][0], g[8][0], g[9][0]],
              ["cbat 0000000", "prev 0", "cbat 0A00000", "prev A", "cbat 0B00000", "prev B", "cbat 0000000", "prev 0", "cbat 0A00A00"])


# ---- the run --------------------------------------------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cc", default=None)
    ap.add_argument("--src", type=Path, default=SRC, help="another copy of the runtime's folder (the mutants run on a scratch copy)")
    ap.add_argument("--only", default="")
    ap.add_argument("--psyz-build", type=Path, required=True, help="a build folder of psyzbuild.py; the chip core is compiled from the source copy inside it")
    args = ap.parse_args()
    cc = args.cc or shutil.which("cc")
    if not cc:
        print("a C compiler for the host (--cc) is needed; these controls need it")
        return 2
    t0 = time.time()
    failed = count = 0
    with tempfile.TemporaryDirectory(prefix="hostpress-test-") as tmp:
        root = Path(tmp)
        (root / "pressmain.c").write_text(DRIVER_C)
        exe = root / "presstest.exe"
        chip = args.psyz_build / "src" / "psyz" / "src" / "psyz" / "mdec.c"
        if not chip.is_file():
            print(f"{chip} is missing; give --psyz-build a build folder of psyzbuild.py")
            return 2
        chip_obj = root / "chip.o"   # the library's file, as it is: its own warnings are not this test's
        proc = subprocess.run([cc, "-O1", "-c", "-o", str(chip_obj), str(chip)], capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the chip core does not build:\n" + proc.stderr[-2000:])
            return 1
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-DPORT_HAVE_PSYZ", "-I", str(args.src), "-o", str(exe), str(root / "pressmain.c"),
                               str(args.src / "press.c"), str(args.src / "mdec.c"), str(chip_obj)] + LINK_FLAGS, capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the test program does not build:\n" + proc.stderr[-2000:])
            return 1
        Ctx.exe = exe
        # The same file as a program without the library is built: no define, and nothing of the chip in the link.
        (root / "nochip.c").write_text(NOCHIP_C)
        nochip = root / "nochip.exe"
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(args.src), "-o", str(nochip), str(root / "nochip.c"),
                               str(args.src / "press.c"), str(args.src / "mdec.c")] + LINK_FLAGS, capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL press.c does not build and link without the library:\n" + proc.stderr[-2000:])
            return 1
        Ctx.nochip = nochip
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
