#!/usr/bin/env python3
"""Controls for Sony's movie stream calls (port/src/stream.c, stream.h) on top of the disc layer (cd.c).

    python3 test_hoststream.py --cc CROSS_CC [--src DIR] [--only SUBSTRING]

The test compiles stream.c, cd.c (with xa.c, disc.c and sha256.c) and a driver of its own, which maps the game's RAM at its own
address (as test_hostpress.py does) and stands in for what they take from the runtime: port_tick (counted, does nothing), the frame
count, port_halt (prints `stop: ...` and ends the driver with the status it was given; each case that stops runs in a driver of its
own), the check of a call target (prints what it was asked about), the audio lock, the trace, and the decoder's side of the busy rule:
port_press_out_busy returns what the test sets (`busy N`), and with `f1busy 1` the frame callback sets it, as a callback that starts
an output transfer would. The disc is an image the test makes: sectors of a made-up stream (the 32-byte
header with 0x160 at +0, the type halfword at +2, the chunk number at +4, the chunks of the frame at +6, the frame number at +8 and
bytes the test chooses in the rest; the 2,016 data bytes from a pattern that differs for every sector), sectors of the disc's
compressed audio between them (which carry a valid stream header, so that one that reached the ring would be seen), and sectors that
break each rule. The expected values are worked out here from the image: the header the ring must hold is the sector's own 32
header bytes with bytes 28 to 31 replaced by the sector's own header bytes (raw bytes 12 to 15), the block its 2,016 data bytes.

How sectors reach the calls. Most cases STEP: the driver puts its own host ready handler in place of the stream's after CdRead2
(`hook 1`); it calls port_StCdInterrupt and prints the slots' states, then Pauses the drive, and the `step` command restarts the read
until the next data sector has been handed over, so that the state after each sector can be read. The path of the stream's own host
handler, as CdRead2 sets it, is the one that the cases with `tick` use (rows of sectors a vblank, 2, 3, 2, 3 at double speed) and the
ones that compare a step with a row. Which commands the stream calls sent to the disc layer is read from a link-time wrapper of
port_CdControl (`cdctl` lines). NOTHING IS COMPARED WITH THE CONSOLE'S LIBRARY: the contract is the text of stream.h, and the cases
check that the calls do what it says on this image.

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
SRC = HERE.parent / "src"
LIMIT = 60
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase", "-Wl,--wrap,port_CdControl"]
BASE, END = 0x80000000, 0x80200000
STATUS = 9    # PORT_EXIT_OTHER, the status of port_halt
RING, FLAG = 0x80100000, 0x80090000
SECTOR = 2352
N = 15

DRIVER_C = r'''
#include "cd.h"
#include "stream.h"
#include "press.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

volatile int port_handler_depth;
static unsigned frames;
static int ticks, pbusy, ring_n, hk, hooked, reenter, f1busy, failcom = -1;
static unsigned ring_a;

void port_audio_lock(void) {}
void port_audio_unlock(void) {}
/* the sound chip's registers for CdInit: these rigs have no model of the chip, as a build without PsyZ has none */
int port_iopage_chip_read(unsigned offset, unsigned *value) { (void)offset; (void)value; return 0; }
int port_iopage_chip_write(unsigned offset, unsigned value) { (void)offset; (void)value; return 0; }
unsigned port_frames(void) { return frames; }
void port_trace_line(const char *fmt, ...) { (void)fmt; }
void port_tick(void) { ticks++; }
int port_press_out_busy(void) { return pbusy; }
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

static void cb1(void);
static void cb2(void);
static char kind(const void *f) { return f == (const void *)cb1 ? 'A' : f == (const void *)cb2 ? 'B' : '?'; }
void port_target_check(const char *path, const void *target) { printf("target %s %c\n", path, kind(target)); }

int __real_port_CdControl(int com, unsigned char *param, unsigned char *result);
int __wrap_port_CdControl(int com, unsigned char *param, unsigned char *result)
{
    int r;
    if (com == failcom) { printf("cdctl com=%02x failed by the test\n", com); return 0; }
    if (param && com == 0xe) printf("cdctl com=%02x param=%02x\n", com, param[0]);
    else printf("cdctl com=%02x param=-\n", com);
    r = __real_port_CdControl(com, param, result);
    printf("cdctl ret=%d\n", r);
    return r;
}

static unsigned g16(unsigned a) { const unsigned char *p = (const unsigned char *)(size_t)a; return (unsigned)p[0] | (unsigned)p[1] << 8; }
static void states(const char *tag)
{
    int i;
    printf("%s :", tag);
    for (i = 0; i < ring_n; i++) printf(" %u", g16(ring_a + 32u * (unsigned)i));
    printf("\n");
}
static void cb1(void)
{
    printf("f1 depth=%d ", port_handler_depth);
    states("");
    if (reenter & 1) { port_StCdInterrupt(); printf("f1 re-entered returned\n"); }
    if (f1busy) pbusy = 1;   /* as a frame callback that starts an output transfer */
}
static void cb2(void)
{
    printf("f2 depth=%d ", port_handler_depth);
    states("");
    if (reenter & 2) { port_StCdInterrupt(); printf("f2 re-entered returned\n"); }
}

static void hook_ready(int intr, unsigned char *res)
{
    (void)res;
    hk++;
    printf("hook intr=%d\n", intr);
    port_StCdInterrupt();
    states("S");
    port_CdControlB(9, 0, 0);
}

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

int main(int argc, char **argv)
{
    char *line = malloc(1 << 20);
    unsigned char *tmp = malloc(1 << 20);
    void *ram = VirtualAlloc((void *)(size_t)PORT_RAM_BASE, PORT_RAM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    struct port_disc d;
    char err[PORT_ERR];
    if (ram != (void *)(size_t)PORT_RAM_BASE) { printf("no RAM\n"); return 3; }
    if (argc < 2 || port_disc_open(&d, argv[1], err, sizeof err) != 0) { printf("no disc\n"); return 3; }
    port_cd_init(&d);
    while (fgets(line, 1 << 20, stdin)) {
        char cmd[16], *h = malloc(1 << 20);
        unsigned a = 0, b = 0, c = 0, e = 0, f = 0;
        int at = 0, v = 0;
        if (sscanf(line, "%15s%n", cmd, &at) != 1) { free(h); continue; }
        if (!strcmp(cmd, "cdinit")) {
            printf("cdinit %d\n", port_CdInit());
        } else if (!strcmp(cmd, "setloc")) {
            unsigned char p[4] = {0, 0, 0, 0}, res[8];
            sscanf(line + at, "%d", &v);
            port_CdIntToPos(v, p);
            printf("setloc %d\n", port_CdControlB(2, p, res));
        } else if (!strcmp(cmd, "ring")) {
            sscanf(line + at, "%x %d", &a, &v);
            ring_a = a;
            ring_n = v;
            port_StSetRing((unsigned long *)(size_t)a, v);
        } else if (!strcmp(cmd, "stream")) {
            sscanf(line + at, "%u %u %u %u %u", &a, &b, &c, &e, &f);
            port_StSetStream(a, b, c, e ? cb1 : 0, f ? cb2 : 0);
        } else if (!strcmp(cmd, "bind")) {
            sscanf(line + at, "%x", &a);
            port_stream_bind(a);
        } else if (!strcmp(cmd, "read2")) {
            sscanf(line + at, "%x", &a);
            printf("read2 %d\n", port_CdRead2((long)a));
        } else if (!strcmp(cmd, "unset")) {
            port_StUnSetRing();
        } else if (!strcmp(cmd, "hook")) {
            sscanf(line + at, "%d", &v);
            hooked = v;
            port_cd_host_ready(v ? hook_ready : 0);
        } else if (!strcmp(cmd, "reenter")) {
            sscanf(line + at, "%d", &reenter);
        } else if (!strcmp(cmd, "tick")) {
            sscanf(line + at, "%d", &v);
            while (v-- > 0) { frames++; port_cd_tick(); }
        } else if (!strcmp(cmd, "step")) {
            int tries;
            sscanf(line + at, "%d", &v);
            while (v-- > 0) {
                int before = hk;
                for (tries = 0; tries < 12 && hk == before; tries++) { port_CdControlF(0x1b, 0); frames++; port_cd_tick(); }
                if (hk == before) printf("step: no sector came\n");
            }
        } else if (!strcmp(cmd, "intr")) {
            port_StCdInterrupt();
            printf("intr returned\n");
        } else if (!strcmp(cmd, "busy")) {
            sscanf(line + at, "%d", &pbusy);
        } else if (!strcmp(cmd, "f1busy")) {
            sscanf(line + at, "%d", &f1busy);
        } else if (!strcmp(cmd, "next")) {
            /* the program's two locals are in the game's memory, as on the console: at 0x80080000 and 0x80080004 */
            unsigned long **x = (unsigned long **)(size_t)0x80080000u, **y = (unsigned long **)(size_t)0x80080004u;
            int r;
            *x = 0;
            *y = 0;
            r = port_StGetNext(x, y);
            printf("next %d %x %x\n", r, (unsigned)(size_t)*x, (unsigned)(size_t)*y);
        } else if (!strcmp(cmd, "nexth") || !strcmp(cmd, "nextn")) {
            /* the caller's own variables, on the driver's stack (not in the game's RAM), as the program's C has them; nextn gives a null
             * pointer for the address (first number 1) and/or the header (second number 1) */
            unsigned long *x = 0, *y = 0;
            int na = 0, nh = 0, r, t0 = ticks;
            if (!strcmp(cmd, "nextn")) sscanf(line + at, "%d %d", &na, &nh);
            r = port_StGetNext(na ? 0 : &x, nh ? 0 : &y);
            printf("nexth %d %x %x %d\n", r, (unsigned)(size_t)x, (unsigned)(size_t)y, ticks - t0);
        } else if (!strcmp(cmd, "nextp")) {
            sscanf(line + at, "%x %x", &a, &b);
            printf("nextp %d\n", port_StGetNext((unsigned long **)(size_t)a, (unsigned long **)(size_t)b));
        } else if (!strcmp(cmd, "free")) {
            sscanf(line + at, "%x", &a);
            printf("free %d\n", port_StFreeRing((unsigned long *)(size_t)a));
        } else if (!strcmp(cmd, "ringdump")) {
            unsigned i, n = (unsigned)ring_n * 2048u;
            printf("ring ");
            for (i = 0; i < n; i++) printf("%02x", ((unsigned char *)(size_t)ring_a)[i]);
            printf("\n");
        } else if (!strcmp(cmd, "put")) {
            sscanf(line + at, "%x %s", &a, h);
            memcpy((void *)(size_t)a, tmp, (size_t)unhex(h, tmp));
        } else if (!strcmp(cmd, "fill")) {
            sscanf(line + at, "%x %x %x", &a, &b, &c);
            memset((void *)(size_t)a, (int)c, b);
        } else if (!strcmp(cmd, "get")) {
            unsigned i;
            sscanf(line + at, "%x %x", &a, &b);
            printf("get ");
            for (i = 0; i < b; i++) printf("%02x", ((unsigned char *)(size_t)a)[i]);
            printf("\n");
        } else if (!strcmp(cmd, "src")) {
            sscanf(line + at, "%x", &a);
            printf("src %d\n", port_cd_source_at(a));
        } else if (!strcmp(cmd, "fifo")) {   /* one word more from the sector's FIFO, into the game's memory at 0x80080010 */
            unsigned char *q = (unsigned char *)(size_t)0x80080010u;
            port_CdGetSector(q, 1);
            printf("fifo %02x%02x%02x%02x\n", q[0], q[1], q[2], q[3]);
        } else if (!strcmp(cmd, "fail")) {
            sscanf(line + at, "%x", &a);
            failcom = (int)a;
        } else if (!strcmp(cmd, "ready")) {
            unsigned char res[8];
            printf("ready %d\n", port_CdReady(1, res));
        } else if (!strcmp(cmd, "hooks")) {
            printf("hooks %d\n", hk);
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

NAMES: list = []


def case(fn):
    NAMES.append(fn)
    return fn


class Ctx:
    exe: Path
    root: Path
    serial = 0


# ---- the image -------------------------------------------------------------------------------------------------------------

def pat(n: int, i: int) -> int:
    return (n * 37 + i * 5 + (i >> 8) * 11 + 3) & 0xFF


class Img:
    """Raw sectors of 2,352 bytes. Data sectors: the whole-sector FIFO is raw bytes 12 to 2351: 4 header bytes, 8 subheader bytes
    (submode without the audio bit), the 32-byte stream header (raw 24 to 55), 2,016 data bytes (raw 56 to 2071), 280 bytes of fill."""

    def __init__(self):
        self.raw: list[bytearray] = []
        self.hdr: dict[int, bytes] = {}

    def _new(self) -> tuple[int, bytearray]:
        n = len(self.raw)
        r = bytearray([0xEE]) * SECTOR
        r[0:12] = bytes([0] + [0xFF] * 10 + [0])
        r[12:16] = bytes([0x10 + n % 7, 0x20 + n % 11, 0x30 + n % 13, 0x40 + n % 5])
        r[16:24] = bytes([1, 0, 0x08, 0, 1, 0, 0x08, 0])
        self.raw.append(r)
        return n, r

    def stream(self, chunk=0, count=1, frame=1, ident=0x160, typ=0, typ_low=0) -> int:
        """A data sector of the stream. `typ` goes to bits 10 to 14 of the halfword at +2, `typ_low` is or-ed into it as it is."""
        n, r = self._new()
        h = bytearray(32)
        h[0:2] = ident.to_bytes(2, "little")
        h[2:4] = (((typ & 0x1F) << 10) | typ_low).to_bytes(2, "little")
        h[4:6] = chunk.to_bytes(2, "little")
        h[6:8] = count.to_bytes(2, "little")
        h[8:10] = frame.to_bytes(2, "little")
        for i in range(10, 28):
            h[i] = (0x90 + n + i) & 0xFF
        h[28:32] = bytes([0xCC, 0xCD, 0xCE, 0xCF])
        r[24:56] = h
        for i in range(2016):
            r[56 + i] = pat(n, i)
        self.hdr[n] = bytes(h)
        return n

    def audio(self, chunk=1, count=3, frame=9) -> int:
        """A sector of the disc's compressed audio (submode bit 2). It carries a valid stream header, so that one that reached the
        ring would be seen."""
        n = self.stream(chunk, count, frame)
        self.raw[n][18] = self.raw[n][22] = 0x64
        return n

    def pad(self, k=8) -> None:
        for _ in range(k):
            self.audio()

    def expect_hdr(self, n: int) -> bytes:
        """The header the ring holds for sector n: its own 32 bytes with bytes 28 to 31 replaced by its raw bytes 12 to 15."""
        return self.hdr[n][:28] + bytes(self.raw[n][12:16])

    def expect_data(self, n: int) -> bytes:
        return bytes(self.raw[n][56:56 + 2016])

    def data(self) -> bytes:
        return b"".join(bytes(r) for r in self.raw)


def u16(b: bytes, o: int = 0) -> int:
    return b[o] | b[o + 1] << 8


class Ring:
    def __init__(self, hexs: str, n: int):
        self.b = bytes.fromhex(hexs)
        self.n = n

    def state(self, i):
        return u16(self.b, 32 * i)

    def states(self):
        return [self.state(i) for i in range(self.n)]

    def hdr(self, i):
        return self.b[32 * i:32 * i + 32]

    def blk(self, i):
        o = 32 * self.n + 2016 * i
        return self.b[o:o + 2016]


A5_HDR = b"\x00\x00" + b"\xa5" * 30   # the ring's clearing wrote the state; the rest is the fill
A5_BLK = b"\xa5" * 2016


def with_state(h: bytes, st: int) -> bytes:
    return st.to_bytes(2, "little") + h[2:]


# ---- running -----------------------------------------------------------------------------------------------------------------

def run(img: Img, cmds, expect_end=True):
    """Runs the driver on the image and commands. Returns (status, groups, rest): the lines each command printed, and the lines
    after the last finished command (the line of a stop)."""
    Ctx.serial += 1
    path = Ctx.root / f"img{Ctx.serial}.bin"
    path.write_bytes(img.data())
    p = subprocess.run([str(Ctx.exe), str(path)], input=("\n".join(cmds) + "\n").encode(), capture_output=True, timeout=LIMIT)
    groups, cur = [], []
    for l in p.stdout.decode().splitlines():
        if l == ".":
            groups.append(cur)
            cur = []
        else:
            cur.append(l)
    return p.returncode, groups, cur


def ok_run(img, cmds):
    st, groups, rest = run(img, cmds)
    if st != 0 or len(groups) != len(cmds):
        raise RuntimeError(f"the driver ended with status {st} after {len(groups)} of {len(cmds)} commands: {rest}")
    return groups


def stops(img, cmds, call, *words):
    """None when the commands end in a stop whose line begins `stop: CALL` and holds every word; else why not."""
    st, groups, rest = run(img, cmds)
    line = next((l for l in rest if l.startswith("stop:")), "")
    if st != STATUS:
        return f"status {st}, expected {STATUS} (line {line!r}; {len(groups)} commands finished; rest {rest})"
    if not line.startswith(f"stop: {call}:"):
        return f"stop line {line!r}, expected it to begin `stop: {call}:`"
    for w in words:
        if w not in line:
            return f"stop line {line!r} lacks {w!r}"
    return None


def eq(got, want):
    return None if got == want else f"got {got!r}, expected {want!r}"


def lines_of(g, prefix):
    return [l for grp in g for l in grp if l.startswith(prefix)]


def rings(g, n=N):
    return [Ring(l.split()[1], n) for l in lines_of(g, "ring ")]


def snaps(g):
    """The states printed after each stepped sector."""
    return [[int(x) for x in l.split(":")[1].split()] for l in lines_of(g, "S ")]


def cb_states(g, which):
    return [[int(x) for x in l.split(":")[1].split()] for l in lines_of(g, which + " ")]


def setup(first=0, n=N, mode=1, start=0, end=0, f1=1, f2=1, bind=True, hook=True, ring=RING, fill=True, rd=0x1e8):
    c = ["cdinit"]
    if fill:
        c.append(f"fill {ring:x} {n * 2048:x} a5")
    c.append(f"ring {ring:x} {n}")
    if bind:
        c.append(f"bind {FLAG:x}")
    c += [f"stream {mode} {start} {end} {f1} {f2}", f"setloc {first}", f"read2 {rd:x}"]
    if hook:
        c.append("hook 1")
    return c


def flag(g):
    return [int.from_bytes(bytes.fromhex(l.split()[1]), "little") for l in lines_of(g, "get ")]


def check_slot(ring: Ring, img: Img, slot: int, sector: int, state: int):
    """None when the ring's slot holds the sector's header (with the given state) and data."""
    if ring.hdr(slot) != with_state(img.expect_hdr(sector), state):
        return f"slot {slot}'s header is {ring.hdr(slot).hex()}, expected sector {sector}'s with state {state}: {with_state(img.expect_hdr(sector), state).hex()}"
    if ring.blk(slot) != img.expect_data(sector):
        return f"slot {slot}'s data is not sector {sector}'s"
    return None


def check_dropped(ring: Ring, img: Img, slot: int, sector: int):
    """The slot of a sector that was dropped after its header was read: the header with state 0, the block untouched."""
    if ring.hdr(slot) != with_state(img.expect_hdr(sector), 0):
        return f"slot {slot}'s header is {ring.hdr(slot).hex()}, expected sector {sector}'s with state 0"
    if ring.blk(slot) != A5_BLK:
        return f"slot {slot}'s data block was written"
    return None


def first_bad(*checks):
    for c in checks:
        if c:
            return c
    return None


def ticks_for(k: int) -> int:
    """Vblanks of the disc layer at double speed after which at least k sectors have been handed over (2, 3, 2, 3, ...)."""
    t = 1
    while (5 * t) // 2 < k:
        t += 1
    return t


# ---- the variables ---------------------------------------------------------------------------------------------------------

@case
def start_zero_takes_the_first_frame_whatever_its_number():
    img = Img()
    a, b = img.stream(0, 2, 9), img.stream(1, 2, 9)
    img.pad()
    g = ok_run(img, setup() + ["step 2", "ringdump"])
    r = rings(g)[0]
    return first_bad(eq(snaps(g), [[3] + [0] * 14, [2, 3] + [0] * 13]), check_slot(r, img, 0, a, 2), check_slot(r, img, 1, b, 3))


@case
def start_frame_takes_frames_from_it_and_then_any():
    img = Img()
    f3 = img.stream(0, 1, 3)
    f4 = img.stream(0, 1, 4)
    a, b = img.stream(0, 2, 5), img.stream(1, 2, 5)
    f9 = img.stream(0, 1, 9)
    img.pad()
    g = ok_run(img, setup(start=5) + ["step 1", "ringdump", "step 1", "ringdump", "step 3", "ringdump"])
    r1, r2, r3 = rings(g)
    return first_bad(eq(snaps(g), [[0] * 15, [0] * 15, [3] + [0] * 14, [2, 3] + [0] * 13, [2, 3, 2] + [0] * 12]),
                     check_dropped(r1, img, 0, f3), check_dropped(r2, img, 0, f4), check_slot(r3, img, 0, a, 2), check_slot(r3, img, 1, b, 3),
                     check_slot(r3, img, 2, f9, 2), eq(len(cb_states(g, "f1")), 2))


@case
def a_second_StSetStream_keeps_slots_and_places_and_sets_the_mask_again():
    img = Img()
    f1_ = img.stream(0, 1, 1)
    f2_ = img.stream(0, 1, 2)
    f7 = img.stream(0, 1, 7)
    f8 = img.stream(0, 1, 8)
    img.pad()
    g = ok_run(img, setup() + ["step 1", "stream 1 7 0 1 1", "ringdump", "step 3", "ringdump", "next"])
    ra, rb = rings(g)
    nx = lines_of(g, "next ")[0].split()
    return first_bad(eq(snaps(g), [[2] + [0] * 14, [2, 0] + [0] * 13, [2, 2] + [0] * 13, [2, 2, 2] + [0] * 12]), check_slot(ra, img, 0, f1_, 2),
                     check_slot(rb, img, 1, f7, 2), check_slot(rb, img, 2, f8, 2),
                     eq(nx[0:2], ["next", "0"]), eq(int(nx[3], 16), RING),   # r is still 0: the first frame, not frame 7
                     eq(len(cb_states(g, "f1")), 3))


@case
def StSetRing_clears_states_places_and_flag_and_the_queue():
    img = Img()
    ss = [img.stream(0, 1, k) for k in (1, 2, 3, 4)]
    img.pad()
    cmds = setup(hook=False) + ["busy 1", "tick 1", f"get {FLAG:x} 4", "busy 0", "intr", "ringdump", f"ring {RING:x} {N}", "ringdump", f"get {FLAG:x} 4",
                                "busy 0", "intr", "ringdump", "stream 1 0 0 1 1", "hook 1", "step 1", "ringdump", "next"]
    g = ok_run(img, cmds)
    # tick 1 at double speed: 2 sectors, queued while busy (flag 1); the call with the decoder free works them off; the ring is set again
    r1, r2, r3, r4 = rings(g)
    nx = lines_of(g, "next ")[0].split()
    return first_bad(eq(flag(g), [1, 0]), eq(r1.states()[:3], [2, 2, 0]), eq(r2.states(), [0] * 15), eq(r3.states(), [0] * 15),
                     check_slot(r4, img, 0, ss[2], 2), eq(nx[0:2], ["next", "0"]), eq(int(nx[3], 16), RING),
                     eq(r2.hdr(0), with_state(img.expect_hdr(ss[0]), 0)))   # the ring's clearing writes the states only


@case
def StSetRing_clears_a_partial_frame():
    img = Img()
    c0 = img.stream(0, 2, 5)
    c1 = img.stream(1, 2, 5)
    img.pad()
    g = ok_run(img, setup() + ["step 1", f"ring {RING:x} {N}", "step 1", "ringdump"])
    r = rings(g)[0]
    # the ring is cleared, w is 0: chunk 1 goes to slot 0 over chunk 0's header, finds nextchunk 0 and is dropped (no frame, no f1); the
    # block still holds chunk 0's data
    return first_bad(eq(snaps(g), [[3] + [0] * 14, [0] * 15]), eq(r.hdr(0), with_state(img.expect_hdr(c1), 0)), eq(r.blk(0), img.expect_data(c0)),
                     eq(cb_states(g, "f1"), []))


@case
def StSetStream_clears_a_partial_frame_without_touching_the_slots():
    img = Img()
    c0 = img.stream(0, 2, 5)
    c1 = img.stream(1, 2, 5)
    img.pad()
    g = ok_run(img, setup() + ["step 1", "stream 1 0 0 1 1", "step 1", "ringdump"])
    r = rings(g)[0]
    # w is 1: chunk 1 is read into slot 1, finds nextchunk 0 and drops the partial frame: slots s to w (0 and 1) get state 0
    return first_bad(eq(snaps(g), [[3] + [0] * 14, [0] * 15]), eq(cb_states(g, "f1"), []), check_dropped(r, img, 1, c1),
                     eq(r.hdr(0), with_state(img.expect_hdr(c0), 0)), eq(r.blk(0), img.expect_data(c0)))


# ---- a frame of several sectors; the callbacks -------------------------------------------------------------------------------

@case
def a_frame_of_three_sectors_slot_states_after_each_sector_headers_data_and_f1():
    img = Img()
    s0, s1, s2 = img.stream(0, 3, 7), img.stream(1, 3, 7), img.stream(2, 3, 7)
    s3 = img.stream(0, 1, 8)
    img.pad()
    g = ok_run(img, setup() + ["step 4", "ringdump"])
    r = rings(g)[0]
    z = [0] * 15
    want = [[3] + z[1:], [3, 3] + z[2:], [2, 3, 3] + z[3:], [2, 3, 3, 2] + z[4:]]
    own = [bytes(img.raw[s][12:16]) for s in (s0, s1, s2, s3)]
    return first_bad(eq(snaps(g), want), check_slot(r, img, 0, s0, 2), check_slot(r, img, 1, s1, 3), check_slot(r, img, 2, s2, 3),
                     check_slot(r, img, 3, s3, 2), eq([r.hdr(i)[28:32] for i in range(4)], own),
                     eq([img.hdr[s][28:32] != bytes(img.raw[s][12:16]) for s in (s0, s1, s2, s3)], [True] * 4),
                     eq(cb_states(g, "f1"), [[2, 3, 3] + z[3:], [2, 3, 3, 2] + z[4:]]))


@case
def callbacks_are_checked_before_they_are_called_and_run_with_the_depth_raised():
    img = Img()
    img.stream(0, 1, 1)
    img.stream(0, 1, 5)   # the end frame
    img.pad()
    g = ok_run(img, setup(end=5) + ["step 2"])
    seen = [l.split(" depth")[0] if l.startswith(("f1", "f2")) else l for grp in g for l in grp if l.startswith(("target ", "f1", "f2"))]
    depths = [l.split()[1] for l in lines_of(g, "f1") + lines_of(g, "f2")]
    return first_bad(eq(seen, ["target stream frame callback A", "f1", "target stream end-frame callback B", "f2"]), eq(depths, ["depth=1", "depth=1"]))


@case
def null_callbacks_are_not_called_and_not_checked():
    img = Img()
    img.stream(0, 1, 1)
    img.stream(0, 1, 5)
    img.stream(0, 1, 2)
    img.pad()
    g = ok_run(img, setup(end=5, f1=0, f2=0) + ["step 3", "ringdump"])
    r = rings(g)[0]
    return first_bad(eq(lines_of(g, "target "), []), eq(snaps(g)[-1][:3], [2, 2, 0]))


@case
def the_ring_at_the_end_of_the_ram_works_and_is_checked_by_its_size():
    img = Img()
    a = img.stream(0, 1, 3)
    img.pad()
    top = END - 3 * 2048
    g = ok_run(img, setup(n=3, ring=top) + ["step 1", "ringdump"])
    return first_bad(eq(snaps(g), [[2, 0, 0]]), check_slot(rings(g, 3)[0], img, 0, a, 2))


# ---- StGetNext, StFreeRing -----------------------------------------------------------------------------------------------------

@case
def StGetNext_and_StFreeRing_follow_the_frames():
    img = Img()
    img.stream(0, 2, 1)
    img.stream(1, 2, 1)
    f2_ = img.stream(0, 1, 2)
    img.pad()
    data0, data2 = RING + 32 * N, RING + 32 * N + 2016 * 2
    g = ok_run(img, setup() + ["next", "step 1", "next", "step 1", "next", "next", f"free {data0:x}", "ringdump", "next", "step 1", "next", "ringdump"])
    nx = [l.split()[1:] for l in lines_of(g, "next ")]
    fr = lines_of(g, "free ")
    ra, rb = rings(g)
    return first_bad(eq(nx, [["1", "0", "0"], ["1", "0", "0"], ["0", f"{data0:x}", f"{RING:x}"], ["1", "0", "0"], ["1", "0", "0"],
                            ["0", f"{data2:x}", f"{RING + 64:x}"]]),
                     eq(fr, ["free 0"]), eq(ra.states(), [0, 0] + [0] * 13), eq(rb.states(), [0, 0, 4] + [0] * 12))


@case
def StFreeRing_gives_1_for_a_slot_not_in_state_4_and_frees_a_frames_slots_and_moves_r():
    img = Img()
    img.stream(0, 3, 1)
    img.stream(1, 3, 1)
    img.stream(2, 3, 1)
    img.stream(0, 1, 2)
    img.pad()
    d = lambda i: RING + 32 * N + 2016 * i
    g = ok_run(img, setup() + ["step 3", f"free {d(0):x}", "ringdump", "next", f"free {d(0):x}", "ringdump", "step 1", "next", "ringdump"])
    ra, rb, rc = rings(g)
    nx = [l.split()[1:] for l in lines_of(g, "next ")]
    return first_bad(eq(lines_of(g, "free "), ["free 1", "free 0"]), eq(ra.states()[:4], [2, 3, 3, 0]), eq(rb.states()[:4], [0, 0, 0, 0]),
                     eq(nx, [["0", f"{d(0):x}", f"{RING:x}"], ["0", f"{d(3):x}", f"{RING + 96:x}"]]), eq(rc.states()[:5], [0, 0, 0, 4, 0]))


@case
def StFreeRing_gives_1_for_a_free_slot():
    img = Img()
    img.stream(0, 1, 1)
    img.pad()
    g = ok_run(img, setup() + ["step 1", f"free {RING + 32 * N + 2016 * 4:x}", "ringdump"])
    return first_bad(eq(lines_of(g, "free "), ["free 1"]), eq(rings(g)[0].states()[:2], [2, 0]))


# ---- each failing test --------------------------------------------------------------------------------------------------------

@case
def a_slot_that_is_not_free_means_the_sector_is_not_taken():
    img = Img()
    a, b, c, d = img.stream(0, 1, 1), img.stream(0, 1, 2), img.stream(0, 1, 3), img.stream(0, 1, 4)
    img.pad()
    g = ok_run(img, setup(n=3) + ["step 4", "ringdump"])
    r = rings(g, 3)[0]
    # A and B in slots 0 and 1; C finds no room (the last slot is never used) and stalls: state 1 in slot 2 with C's header; D finds the
    # slot not free and is not taken: slot 2 is as C left it and its block is untouched
    return first_bad(eq(snaps(g), [[2, 0, 0], [2, 2, 0], [2, 2, 1], [2, 2, 1]]), check_slot(r, img, 0, a, 2), check_slot(r, img, 1, b, 2),
                     eq(r.hdr(2), with_state(img.expect_hdr(c), 1)), eq(r.blk(2), A5_BLK),
                     eq(len(cb_states(g, "f1")), 2), eq(len(cb_states(g, "f2")), 1))


@case
def the_mask_before_the_start_frame_frees_the_slot_and_leaves_the_block():
    img = Img()
    f3 = img.stream(0, 1, 3)
    img.stream(0, 1, 5)
    img.pad()
    g = ok_run(img, setup(start=5) + ["step 1", "ringdump"])
    return first_bad(eq(snaps(g), [[0] * 15]), check_dropped(rings(g)[0], img, 0, f3), eq(cb_states(g, "f1"), []))


@case
def a_wrong_id_frees_the_slot_and_the_next_frame_takes_it():
    img = Img()
    bad = img.stream(0, 1, 1, ident=0x161)
    good = img.stream(0, 1, 2)
    img.pad()
    g = ok_run(img, setup() + ["step 1", "ringdump", "step 1", "ringdump"])
    r1, r2 = rings(g)
    return first_bad(eq(snaps(g), [[0] * 15, [2] + [0] * 14]), check_dropped(r1, img, 0, bad), check_slot(r2, img, 0, good, 2))


@case
def a_wrong_type_frees_the_slot_and_only_bits_10_to_14_are_the_type():
    img = Img()
    bads = [img.stream(0, 1, 1, typ=t_) for t_ in (1, 16, 31, 8)]   # bit 10, bit 14, all five bits, bit 13
    good = img.stream(0, 1, 2, typ=0, typ_low=0x8155)   # bit 15 and the low bits are not the type
    img.pad()
    g = ok_run(img, setup() + ["step 1", "ringdump"] * 4 + ["step 1", "ringdump"])
    rs = rings(g)
    return first_bad(eq(snaps(g), [[0] * 15] * 4 + [[2] + [0] * 14]), *[check_dropped(rs[i], img, 0, bads[i]) for i in range(4)], check_slot(rs[4], img, 0, good, 2))


@case
def a_chunk_out_of_order_drops_the_partial_frame_and_the_sector_and_the_next_first_chunk_is_taken():
    img = Img()
    c0 = img.stream(0, 4, 4)
    c2 = img.stream(2, 4, 4)
    c3 = img.stream(3, 4, 4)
    nf = img.stream(0, 2, 5)
    nf1 = img.stream(1, 2, 5)
    img.pad()
    g = ok_run(img, setup() + ["step 2", "ringdump", "step 1", "ringdump", "step 2", "ringdump"])
    r1, r2, r3 = rings(g)
    z = [0] * 15
    return first_bad(eq(snaps(g), [[3] + z[1:], z, z, [3] + z[1:], [2, 3] + z[2:]]),
                     check_dropped(r1, img, 1, c2), eq(r1.hdr(0), with_state(img.expect_hdr(c0), 0)), eq(r2.states(), z),
                     check_slot(r3, img, 0, nf, 2), check_slot(r3, img, 1, nf1, 3), eq(len(cb_states(g, "f1")), 1))


@case
def a_chunk_other_than_zero_with_nothing_before_it_is_dropped():
    img = Img()
    c1 = img.stream(1, 2, 6)
    nf = img.stream(0, 1, 7)
    img.pad()
    g = ok_run(img, setup() + ["step 1", "ringdump", "step 1", "ringdump"])
    r1, r2 = rings(g)
    return first_bad(eq(snaps(g), [[0] * 15, [2] + [0] * 14]), check_dropped(r1, img, 0, c1), check_slot(r2, img, 0, nf, 2))


@case
def a_frame_number_that_changes_inside_a_frame_drops_the_partial_frame_and_the_sector():
    img = Img()
    img.stream(0, 3, 4)
    c1 = img.stream(1, 3, 5)
    c2 = img.stream(2, 3, 5)
    nf = img.stream(0, 1, 6)
    img.pad()
    g = ok_run(img, setup() + ["step 2", "ringdump", "step 1", "ringdump", "step 1", "ringdump"])
    r1, r2, r3 = rings(g)
    z = [0] * 15
    return first_bad(eq(snaps(g), [[3] + z[1:], z, z, [2] + z[1:]]), check_dropped(r1, img, 1, c1), eq(r2.states(), z), check_slot(r3, img, 0, nf, 2),
                     eq(len(cb_states(g, "f1")), 1))


# ---- the end frame ------------------------------------------------------------------------------------------------------------

@case
def the_end_frame_calls_f2_sets_the_mask_and_drops_the_sector():
    img = Img()
    f4, f5 = img.stream(0, 1, 4), img.stream(0, 1, 5)
    f6 = img.stream(0, 1, 6)
    f7 = img.stream(0, 1, 7)
    img.pad()
    g = ok_run(img, setup(end=6) + ["step 4", "ringdump"])
    r = rings(g)[0]
    z = [0] * 15
    return first_bad(eq(snaps(g), [[2] + z[1:], [2, 2] + z[2:], [2, 2] + z[2:], [2, 2] + z[2:]]), check_slot(r, img, 0, f4, 2), check_slot(r, img, 1, f5, 2),
                     eq(r.hdr(2), with_state(img.expect_hdr(f7), 0)), eq(r.blk(2), A5_BLK),
                     eq(len(cb_states(g, "f2")), 2), eq(len(cb_states(g, "f1")), 2))


@case
def the_end_frame_sets_the_mask_so_the_start_frame_is_waited_for_again():
    img = Img()
    f4, f5 = img.stream(0, 1, 4), img.stream(0, 1, 5)
    img.stream(0, 1, 6)   # the end frame: f2, mask 1
    f7 = img.stream(0, 1, 7)   # mask 1 and start 4: dropped at step 6, before the end frame test
    f4b = img.stream(0, 1, 4)
    img.pad()
    g = ok_run(img, setup(start=4, end=6) + ["step 5", "ringdump"])
    r = rings(g)[0]
    z = [0] * 15
    return first_bad(eq(snaps(g)[-1], [2, 2, 2] + z[3:]), eq(len(cb_states(g, "f2")), 1), check_slot(r, img, 2, f4b, 2), check_slot(r, img, 0, f4, 2),
                     check_slot(r, img, 1, f5, 2))


@case
def end_zero_means_no_end_frame_and_a_large_end_is_never_reached():
    img = Img()
    ss = [img.stream(0, 1, k) for k in (100, 40000, 65535)]
    img.pad()
    g1 = ok_run(img, setup(end=0) + ["step 3", "ringdump"])
    g2 = ok_run(img, setup(end=0x0fffffff) + ["step 3", "ringdump"])
    return first_bad(eq(snaps(g1)[-1][:4], [2, 2, 2, 0]), eq(snaps(g2)[-1][:4], [2, 2, 2, 0]), eq(cb_states(g1, "f2"), []), eq(cb_states(g2, "f2"), []),
                     check_slot(rings(g1)[0], img, 2, ss[2], 2))


# ---- no room -------------------------------------------------------------------------------------------------------------------

@case
def no_room_with_end_zero_stalls_and_the_last_slot_is_never_a_frames_start():
    img = Img()
    a, b = img.stream(0, 2, 1), img.stream(1, 2, 1)   # slots 0, 1
    c = img.stream(0, 1, 2)      # slot 2
    d = img.stream(0, 2, 3)      # w = 3, N = 5: room for 1 slot only: stall
    img.stream(1, 2, 3)
    img.pad()
    g = ok_run(img, setup(n=5, start=0) + ["step 4", "ringdump"])
    r = rings(g, 5)[0]
    return first_bad(eq(snaps(g), [[3, 0, 0, 0, 0], [2, 3, 0, 0, 0], [2, 3, 2, 0, 0], [2, 3, 2, 1, 0]]), check_slot(r, img, 0, a, 2), check_slot(r, img, 1, b, 3),
                     check_slot(r, img, 2, c, 2), eq(r.hdr(3), with_state(img.expect_hdr(d), 1)), eq(r.blk(3), A5_BLK), eq(r.hdr(4), A5_HDR),
                     eq(len(cb_states(g, "f2")), 1), eq(len(cb_states(g, "f1")), 2))


@case
def a_frame_that_just_fits_is_taken_and_the_last_slot_stays_unused():
    img = Img()
    a, b = img.stream(0, 2, 1), img.stream(1, 2, 1)
    img.stream(0, 3, 2)   # w = 2, N = 5: N - w - 1 = 2 < 3: no room
    img.pad()
    g = ok_run(img, setup(n=5) + ["step 3", "ringdump"])
    r = rings(g, 5)[0]
    return first_bad(eq(snaps(g)[-1], [2, 3, 1, 0, 0]), check_slot(r, img, 0, a, 2), check_slot(r, img, 1, b, 3), eq(r.hdr(4), A5_HDR), eq(r.blk(4), A5_BLK))


@case
def wrap_to_slot_0_when_it_is_free_and_getnext_follows_the_marker_and_clears_it():
    img = Img()
    a, b, c, d, e = [img.stream(0, 1, k) for k in (1, 2, 3, 4, 5)]
    img.pad()
    dd = lambda i: RING + 32 * 5 + 2016 * i
    g = ok_run(img, setup(n=5, end=0x0fffffff) + ["step 3", "next", f"free {dd(0):x}", "step 2", "ringdump", "next", f"free {dd(1):x}", "next", f"free {dd(2):x}", "next",
                                                  f"free {dd(3):x}", "next", "ringdump"])
    r1, r2 = rings(g, 5)
    nx = [l.split()[1:] for l in lines_of(g, "next ")]
    # D in slot 3 (w = 3), E finds no room (5 - 4 - 1 = 0 < 1) with slot 0 free: the old slot 4 gets state 1, E's header goes to slot 0.
    # The reader takes B, C, D; then its slot is 4, the marker: GetNext clears it (end is set), goes to slot 0 and gives E
    return first_bad(eq(snaps(g)[-1], [2, 2, 2, 2, 1]), check_slot(r1, img, 0, e, 2), check_slot(r1, img, 1, b, 2), check_slot(r1, img, 2, c, 2),
                     check_slot(r1, img, 3, d, 2), eq(r1.hdr(4), with_state(img.expect_hdr(e), 1)), eq(r1.blk(4), A5_BLK),
                     eq(nx, [["0", f"{dd(0):x}", f"{RING:x}"], ["0", f"{dd(1):x}", f"{RING + 32:x}"], ["0", f"{dd(2):x}", f"{RING + 64:x}"],
                             ["0", f"{dd(3):x}", f"{RING + 96:x}"], ["0", f"{dd(0):x}", f"{RING:x}"]]),
                     eq(r2.states(), [4, 0, 0, 0, 0]))


@case
def a_marker_is_cleared_by_getnext_only_when_end_is_set():
    img = Img()
    ss = [img.stream(0, 1, k) for k in (1, 2, 3, 4, 5)]
    img.pad()
    dd = lambda i: RING + 32 * 5 + 2016 * i
    cmds = setup(n=5, end=0) + ["step 5"] + [x for i in range(4) for x in ("next", f"free {dd(i):x}")] + ["next", "ringdump"]
    g = ok_run(img, cmds)
    r = rings(g, 5)[0]
    nx = [l.split()[1] for l in lines_of(g, "next ")]
    # the fifth frame stalls in slot 4 (state 1, end 0). All four frames are read and freed; GetNext meets the marker, goes to slot 0
    # (free: result 1) and leaves the marker
    return first_bad(eq(snaps(g)[-1], [2, 2, 2, 2, 1]), eq(nx, ["0", "0", "0", "0", "1"]), eq(r.states(), [0, 0, 0, 0, 1]))


@case
def wrap_is_dropped_when_slot_0_is_busy_and_the_rest_of_that_frame_with_it():
    img = Img()
    a, b, c = [img.stream(0, 1, k) for k in (1, 2, 3)]
    e0, e1 = img.stream(0, 2, 5), img.stream(1, 2, 5)   # w = 3: no room for 2 slots; slot 0 is busy: dropped; chunk 1 dropped by the chunk test
    f = img.stream(0, 1, 6)
    img.pad()
    g = ok_run(img, setup(n=5, end=0x0fffffff) + ["step 4", "ringdump", "step 1", "ringdump", "step 1", "ringdump"])
    r1, r2, r3 = rings(g, 5)
    return first_bad(eq(snaps(g), [[2, 0, 0, 0, 0], [2, 2, 0, 0, 0], [2, 2, 2, 0, 0], [2, 2, 2, 0, 0], [2, 2, 2, 0, 0], [2, 2, 2, 2, 0]]),
                     eq(r1.hdr(3), with_state(img.expect_hdr(e0), 0)), eq(r2.hdr(3), with_state(img.expect_hdr(e1), 0)), check_slot(r3, img, 3, f, 2),
                     eq(r3.hdr(4), A5_HDR), eq(len(cb_states(g, "f2")), 0), eq(len(cb_states(g, "f1")), 4))


# ---- the compressed audio ------------------------------------------------------------------------------------------------------

@case
def the_compressed_audios_sectors_never_reach_the_ring_whether_stepped_or_in_rows():
    img = Img()
    s0 = img.stream(0, 3, 7)
    img.audio(1, 3, 7)
    s1 = img.stream(1, 3, 7)
    img.audio(2, 3, 7)
    img.audio(0, 3, 8)
    s2 = img.stream(2, 3, 7)
    img.pad()
    g = ok_run(img, setup() + ["step 3", "ringdump"])
    r = rings(g)[0]
    g2 = ok_run(img, setup(hook=False) + ["tick 5", "ringdump"])
    r2 = rings(g2)[0]
    return first_bad(eq(snaps(g)[-1], [2, 3, 3] + [0] * 12), check_slot(r, img, 0, s0, 2), check_slot(r, img, 1, s1, 3), check_slot(r, img, 2, s2, 3),
                     eq(r.hdr(3), A5_HDR), eq(r.blk(3), A5_BLK), eq(r2.states()[:4], [2, 3, 3, 0]), eq(r2.b == r.b, True))


# ---- the busy rule -------------------------------------------------------------------------------------------------------------

def frames_img(k, pad=True):
    img = Img()
    ss = [img.stream(0, 1, 10 + i) for i in range(k)]
    if pad:
        img.pad(10)
    return img, ss


@case
def busy_queues_sectors_in_order_and_writes_the_flag_then_a_call_with_the_decoder_free_works_them_off():
    img, ss = frames_img(6)
    g = ok_run(img, setup(hook=False) + [f"put {FLAG:x} ffffffff", "busy 1", "tick 1", "tick 1", "ringdump", f"get {FLAG:x} 4", "busy 0", "intr", "ringdump", f"get {FLAG:x} 4"])
    r1, r2 = rings(g)
    # 2 + 3 sectors were handed over; none worked on; the flag 1
    return first_bad(eq(r1.states(), [0] * 15), eq([r1.hdr(i) for i in range(5)], [A5_HDR] * 5),
                     eq(flag(g), [1, 1]), eq(r2.states()[:6], [2, 2, 2, 2, 2, 0]),
                     first_bad(*[check_slot(r2, img, i, ss[i], 2) for i in range(5)]),
                     eq(len(cb_states(g, "f1")), 5), eq([s.index(2) for s in [x for x in cb_states(g, "f1")]], [0, 0, 0, 0, 0]))


@case
def a_later_call_with_the_decoder_free_works_the_queue_off_before_the_sector_at_hand():
    img, ss = frames_img(6)
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "busy 0", "tick 1", "ringdump"])
    r = rings(g)[0]
    # tick 1: F0 and F1 queued. tick 2 hands over F2, F3, F4: the first call works off F0 and F1, then F2; then F3 and F4
    return first_bad(eq(r.states()[:6], [2, 2, 2, 2, 2, 0]), first_bad(*[check_slot(r, img, i, ss[i], 2) for i in range(5)]),
                     eq([s.count(2) for s in cb_states(g, "f1")], [1, 2, 3, 4, 5]))


@case
def the_programs_call_with_nothing_queued_and_no_sector_does_nothing():
    img, ss = frames_img(3)
    g = ok_run(img, setup(hook=False) + ["ringdump", "busy 0", "intr", "busy 1", "intr", "ringdump", f"get {FLAG:x} 4", "busy 0", "intr", "ringdump"])
    r1, r2, r3 = rings(g)
    return first_bad(eq(r1.b, r2.b), eq(r1.b, r3.b), eq(flag(g), [0]), eq(cb_states(g, "f1"), []))


@case
def a_busy_call_with_no_sector_at_hand_queues_nothing():
    img, ss = frames_img(3)
    g = ok_run(img, setup(hook=False) + ["busy 1", "intr", f"get {FLAG:x} 4", "busy 0", "intr", "ringdump"])
    return first_bad(eq(flag(g), [0]), eq(rings(g)[0].states(), [0] * 15))


def f1_counts(g):
    """How many frames were complete at each call of f1, in the order of the calls."""
    return [s.count(2) for s in cb_states(g, "f1")]


@case
def a_frame_callback_that_makes_the_decoder_busy_ends_the_queues_work_and_the_rest_comes_in_order_by_the_programs_calls():
    img, ss = frames_img(6)
    free_call = ["busy 0", f"put {FLAG:x} 00000000", "intr", "ringdump", f"get {FLAG:x} 4"]
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "f1busy 1"] + free_call + free_call + free_call)
    r1, r2, r3 = rings(g)
    # tick 1: F0 and F1 queued. Each call finds the decoder free and works on ONE frame: its callback makes the decoder busy. After
    # the first the flag is 1 again (F1 stays queued); after the second the queue is empty and the flag stays as the program left it;
    # the third has nothing to do.
    return first_bad(eq(r1.states()[:3], [2, 0, 0]), eq(r1.hdr(1), A5_HDR), check_slot(r1, img, 0, ss[0], 2),
                     eq(r2.states()[:3], [2, 2, 0]), check_slot(r2, img, 0, ss[0], 2), check_slot(r2, img, 1, ss[1], 2),
                     eq(r3.b, r2.b), eq(flag(g), [1, 0, 0]), eq(f1_counts(g), [1, 2]),
                     eq([len(lines_of([grp], "f1 ")) for grp in g if "intr returned" in grp], [1, 1, 0]))


@case
def a_frame_callback_that_makes_the_decoder_busy_ends_the_queues_work_when_the_disc_layer_calls_and_its_sectors_go_behind():
    img, ss = frames_img(6)
    free_call = ["busy 0", f"put {FLAG:x} 00000000", "intr", "ringdump", f"get {FLAG:x} 4"]
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "f1busy 1", "busy 0", f"put {FLAG:x} 00000000", "tick 1", "ringdump", f"get {FLAG:x} 4"]
               + free_call * 4 + ["busy 0", "intr", "ringdump"])
    rs = rings(g)
    # tick 1: F0 and F1 queued. tick 2 hands over F2, F3 and F4 with the decoder free at first: the first call works on F0, whose
    # callback makes the decoder busy: F1 stays, F2 goes behind it, and F3 and F4 behind that. One callback in that whole vblank.
    # Then four calls of the program, each with the decoder free again: F1, F2, F3, F4, one a call, in order; the flag is 1 while
    # sectors stay queued.
    per_command = [len(lines_of([grp], "f1 ")) for grp in g]
    tick2 = per_command[len(setup(hook=False)) + 5]
    return first_bad(eq(tick2, 1), eq(rs[0].states()[:6], [2, 0, 0, 0, 0, 0]), check_slot(rs[0], img, 0, ss[0], 2), eq(rs[0].hdr(1), A5_HDR),
                     eq([r.states()[:6] for r in rs[1:5]], [[2] * k + [0] * (6 - k) for k in (2, 3, 4, 5)]),
                     first_bad(*[check_slot(rs[4], img, i, ss[i], 2) for i in range(5)]),
                     eq(rs[5].b, rs[4].b), eq(flag(g), [1, 1, 1, 1, 0]), eq(f1_counts(g), [1, 2, 3, 4, 5]),
                     eq([len(lines_of([grp], "f1 ")) for grp in g if "intr returned" in grp], [1, 1, 1, 1, 0]))


@case
def the_sector_at_hand_is_put_aside_when_the_last_queued_frames_callback_made_the_decoder_busy():
    img, ss = frames_img(4)
    g = ok_run(img, setup() + ["busy 1", "step 1", f"get {FLAG:x} 4", "f1busy 1", "busy 0", f"put {FLAG:x} 00000000", "step 1", "ringdump", f"get {FLAG:x} 4",
                               "busy 0", f"put {FLAG:x} 00000000", "hook 0", "intr", "ringdump", f"get {FLAG:x} 4"])
    r1, r2 = rings(g)
    # step 1: F0 queued (one sector, so the queue's work ends with nothing queued). step 2 brings F1 with the decoder free: F0 is
    # worked on, its callback makes the decoder busy, and F1 is put aside (the flag 1), not worked on. The program's call brings it.
    return first_bad(eq(r1.states()[:3], [2, 0, 0]), check_slot(r1, img, 0, ss[0], 2), eq(r1.hdr(1), A5_HDR),
                     eq(r2.states()[:3], [2, 2, 0]), check_slot(r2, img, 1, ss[1], 2), eq(flag(g), [1, 1, 0]), eq(f1_counts(g), [1, 2]))


@case
def a_call_with_the_decoder_busy_and_sectors_queued_writes_the_flag_again_and_with_no_address_bound_stops():
    img, ss = frames_img(3)
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", f"put {FLAG:x} 00000000", "intr", f"get {FLAG:x} 4", "ringdump"])
    return first_bad(eq(flag(g), [1]), eq(rings(g)[0].states(), [0] * 15),
                     stops(img, setup(hook=False) + ["busy 1", "tick 1", "bind 0", "intr"], "StCdInterrupt", "2 sectors are put aside", "no deferred flag is bound"),
                     # with the decoder free the queue is worked off and nothing stays: no flag is needed
                     eq(rings(ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "bind 0", "busy 0", "intr", "ringdump"]))[0].states()[:3], [2, 2, 0]))


@case
def the_queue_holds_sixteen_sectors_in_order():
    img, ss = frames_img(16)
    g = ok_run(img, setup(n=20, hook=False) + ["busy 1", "tick 7", "busy 0", "intr", "ringdump"])
    # 7 vblanks hand over 17 sectors: 16 data sectors and one of audio; all 16 are queued
    r = rings(g, 20)[0]
    return first_bad(eq(r.states()[:17], [2] * 16 + [0]), first_bad(*[check_slot(r, img, i, ss[i], 2) for i in range(16)]))


@case
def a_seventeenth_sector_put_aside_stops():
    img, ss = frames_img(17)
    return first_bad(stops(img, setup(n=20, hook=False) + ["busy 1", "tick 8"], "StCdInterrupt", "queue", "16"))


@case
def with_no_address_bound_a_sector_that_would_be_put_aside_stops():
    img, ss = frames_img(3)
    return first_bad(stops(img, setup(bind=False, hook=False) + ["busy 1", "tick 1"], "StCdInterrupt", "no deferred flag is bound"),
                     stops(img, setup(hook=False) + ["bind 0", "busy 1", "tick 1"], "StCdInterrupt", "no deferred flag is bound"))


@case
def with_mode_bit_0_clear_nothing_is_ever_queued_even_with_no_address_and_the_decoder_busy():
    img, ss = frames_img(4)
    out = []
    for mode in (0, 2):
        g = ok_run(img, setup(mode=mode, bind=False, hook=False) + ["busy 1", "tick 2", "ringdump"])
        out.append(rings(g)[0].states()[:5])
    g = ok_run(img, setup(mode=3, hook=False) + ["busy 1", "tick 1", "ringdump", f"get {FLAG:x} 4"])
    return first_bad(eq(out, [[2, 2, 2, 2, 2][:4] + [0]] * 2), eq(rings(g)[0].states()[:3], [0, 0, 0]), eq(flag(g), [1]))


@case
def StUnSetRing_empties_the_queue_and_leaves_the_flag_and_the_ring():
    img, ss = frames_img(6)
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "unset", f"get {FLAG:x} 4", "busy 0", "intr", "ringdump", "tick 3", "ringdump"])
    r1, r2 = rings(g)
    return first_bad(eq(flag(g), [1]), eq(r1.states(), [0] * 15), eq(r2.b, r1.b))


@case
def CdRead2_does_not_empty_the_queue():
    img, ss = frames_img(6)
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "read2 1e8", "busy 0", "intr", "ringdump"])
    r = rings(g)[0]
    return first_bad(eq(r.states()[:3], [2, 2, 0]), check_slot(r, img, 0, ss[0], 2), check_slot(r, img, 1, ss[1], 2))


@case
def StSetRing_empties_the_queue():
    img, ss = frames_img(6)
    g = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", f"ring {RING:x} {N}", "busy 0", "intr", "ringdump", f"get {FLAG:x} 4"])
    return first_bad(eq(rings(g)[0].states(), [0] * 15), eq(flag(g), [0]))


def queue_vs_hand(name, img, **kw):
    """The same image once with the decoder free (each sector at hand) and once with it busy and the queue worked off: the ring's every
    byte and the callbacks' lines are the same. Returns None or why not."""
    cmds_hand = setup(hook=False, **kw) + ["busy 0", f"tick {ticks_for(len(img.raw))}", "ringdump"]
    cmds_queue = setup(hook=False, **kw) + ["busy 1", f"tick {ticks_for(len(img.raw))}", "busy 0", "intr", "ringdump"]
    a, b = ok_run(img, cmds_hand), ok_run(img, cmds_queue)
    ra, rb = rings(a, kw.get("n", N))[0], rings(b, kw.get("n", N))[0]
    if ra.b != rb.b:
        return f"{name}: the rings differ: {ra.states()} against {rb.states()}"
    strip = lambda ls: [l.split(" depth=")[0] + l.split(" : ")[1] for l in ls]   # the depth differs: the program's call is not inside a handler
    la = strip([l for l in lines_of(a, "f") if l.startswith(("f1", "f2"))])
    lb = strip([l for l in lines_of(b, "f") if l.startswith(("f1", "f2"))])
    if la != lb:
        return f"{name}: the callbacks differ: {la} against {lb}"
    if not ra.states()[0] and not la and name not in ("slot-not-free",):
        pass
    return None


@case
def queued_sectors_end_like_sectors_at_hand_for_every_failing_test():
    out = []
    # mask before the start frame
    i = Img(); i.stream(0, 1, 3); i.stream(0, 1, 5); i.stream(0, 1, 6); i.pad()
    out.append(queue_vs_hand("start", i, start=5))
    # id, type
    i = Img(); i.stream(0, 1, 1, ident=0x161); i.stream(0, 1, 2, typ=2); i.stream(0, 1, 3); i.pad()
    out.append(queue_vs_hand("id-type", i))
    # chunk order, frame number change
    i = Img(); i.stream(0, 3, 4); i.stream(2, 3, 4); i.stream(1, 3, 4); i.stream(0, 2, 5); i.stream(1, 2, 6); i.stream(0, 1, 7); i.pad()
    out.append(queue_vs_hand("chunks", i))
    # end frame
    i = Img(); i.stream(0, 1, 4); i.stream(0, 1, 6); i.stream(0, 1, 7); i.pad()
    out.append(queue_vs_hand("end", i, end=6))
    # stall with end 0, then a sector that finds the slot not free
    i = Img(); [i.stream(0, 1, k) for k in (1, 2, 3, 4, 5)]; i.pad()
    out.append(queue_vs_hand("slot-not-free", i, n=3))
    # wrap and drop
    i = Img(); [i.stream(0, 1, k) for k in (1, 2, 3)]; i.stream(0, 2, 5); i.stream(1, 2, 5); i.stream(0, 1, 6); i.pad()
    out.append(queue_vs_hand("wrap-drop", i, n=5, end=0x0fffffff))
    return next((o for o in out if o), None)


def sources(g):
    return [int(l.split()[1]) for l in lines_of(g, "src ")]


@case
def StSetRing_resets_r_s_and_the_partial_frame_and_writes_all_four_flag_bytes():
    img = Img()
    fs = [img.stream(0, 1, k) for k in (1, 2, 3)]
    stray = img.stream(1, 2, 7)
    img.stream(0, 2, 5)      # a partial frame
    f6 = img.stream(0, 1, 6)
    img.pad()
    d0 = RING + 32 * N
    g = ok_run(img, setup() + ["step 3", "next", f"free {d0:x}", f"put {FLAG:x} ffffffff", f"ring {RING:x} {N}", f"get {FLAG:x} 4", "step 1", "ringdump", "step 1",
                               f"ring {RING:x} {N}", "step 1", "next", "ringdump"])
    r1, r2 = rings(g)
    nx = [l.split()[1:] for l in lines_of(g, "next ")]
    # r (1 after the free) and s (3) are cleared: the stray chunk goes to slot 0 and frees it (s to w is slot 0); the partial frame's
    # state (curframe 5) is cleared: the next frame is taken from slot 0 and handed out from slot 0
    return first_bad(eq(flag(g), [0]), eq(r1.states(), [0] * 15), eq(r1.hdr(0), with_state(img.expect_hdr(stray), 0)),
                     eq(snaps(g)[-1][:2], [2, 0]), check_slot(r2, img, 0, f6, 4), eq(nx[1], ["0", f"{d0:x}", f"{RING:x}"]))


@case
def StSetStream_leaves_r_and_the_slots_and_clears_the_partial_frames_state():
    img = Img()
    f1_, f2_ = img.stream(0, 1, 1), img.stream(0, 1, 2)
    img.stream(0, 2, 5)   # partial
    f6 = img.stream(0, 1, 6)
    img.pad()
    d0 = RING + 32 * N
    g = ok_run(img, setup() + ["step 2", "next", f"free {d0:x}", "stream 1 0 0 1 1", "next", "step 1", "stream 1 0 0 1 1", "step 1", "ringdump"])
    r = rings(g)[0]
    nx = [l.split()[1:] for l in lines_of(g, "next ")]
    # r is 1 after the free and stays; the partial frame is in slot 2 (state 3) and stays; frame 6 is taken though curframe was 5
    return first_bad(eq(nx[1], ["0", f"{d0 + 2016:x}", f"{RING + 32:x}"]), eq(r.states()[:4], [0, 4, 3, 2]), check_slot(r, img, 3, f6, 2))


@case
def StGetNext_never_moves_r_and_gives_one_frame_at_a_time():
    img = Img()
    a, b = img.stream(0, 1, 1), img.stream(0, 1, 2)
    img.pad()
    d0 = RING + 32 * N
    g = ok_run(img, setup() + ["step 2", "next", "next", "ringdump", "next"])
    nx = [l.split()[1:] for l in lines_of(g, "next ")]
    return first_bad(eq(nx, [["0", f"{d0:x}", f"{RING:x}"], ["1", "0", "0"], ["1", "0", "0"]]), eq(rings(g)[0].states()[:3], [4, 2, 0]))


@case
def StFreeRing_may_end_at_the_rings_end_and_the_next_look_stops_there():
    img = Img()
    img.pad()
    d0 = RING + 32 * 4
    st, groups, rest = run(img, ["cdinit", f"ring {RING:x} 4", f"put {RING:x} 0400000000000400", f"free {d0:x}", "ringdump", "next"])
    r = Ring(groups[4][0].split()[1], 4) if len(groups) > 4 else None
    return first_bad(eq(st, STATUS), eq(groups[3] if len(groups) > 3 else None, ["free 0"]), eq(r.states() if r else None, [0, 0, 0, 0]),
                     eq(rest[0].startswith("stop: StGetNext: slot 4 is not in the ring of 4 slots") if rest else None, True))


@case
def a_slot_in_any_nonzero_state_is_not_taken():
    img = Img()
    s0 = img.stream(0, 1, 1)
    img.pad()
    out = []
    for st in (1, 2, 3, 4, 0x160):
        g = ok_run(img, setup(n=3) + [f"put {RING:x} {st & 0xff:02x}{st >> 8:02x}", "step 1", "ringdump"])
        r = rings(g, 3)[0]
        out.append((snaps(g), r.hdr(0), r.blk(0)))
    return first_bad(eq(out, [([[st, 0, 0]], st.to_bytes(2, "little") + b"\xa5" * 30, A5_BLK) for st in (1, 2, 3, 4, 0x160)]), eq(s0, 0))


@case
def wrap_is_dropped_when_slot_0_is_held_by_the_reader():
    img = Img()
    a, b, c, d, e = [img.stream(0, 1, k) for k in (1, 2, 3, 4, 5)]
    img.pad()
    g = ok_run(img, setup(n=5, end=0x0fffffff) + ["step 3", "next", "step 1", "step 1", "ringdump"])
    r = rings(g, 5)[0]
    # slot 0 is handed to the reader (state 4) and not freed; E finds no room and slot 0 is not free: E is dropped, nothing moves
    return first_bad(eq(r.states(), [4, 2, 2, 2, 0]), eq(r.hdr(4), with_state(img.expect_hdr(e), 0)), eq(r.blk(4), A5_BLK), check_slot(r, img, 0, a, 4),
                     check_slot(r, img, 3, d, 2), eq(len(cb_states(g, "f2")), 0))


@case
def the_rings_words_come_from_the_sectors_whether_at_hand_or_queued():
    img, ss = frames_img(3)
    d0 = RING + 32 * N
    qs = [RING + 4, RING + 28, d0, d0 + 2012, RING + 32 + 28, d0 + 2016]
    qcmds = [f"src {a:x}" for a in qs]
    hand = ok_run(img, setup(hook=False) + ["busy 0", "tick 1"] + qcmds)
    que = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "busy 0", "intr"] + qcmds)
    want = [ss[0], ss[0], ss[0], ss[0], ss[1], ss[1]]
    return first_bad(eq(sources(hand), want), eq(sources(que), want))


@case
def the_queue_is_used_round_and_round_in_order():
    img, ss = frames_img(20)
    cmds = setup(n=24, hook=False)
    for _ in range(8):
        cmds += ["busy 1", "tick 1", "busy 0", "intr"]
    g = ok_run(img, cmds + ["ringdump"])
    r = rings(g, 24)[0]
    # 8 rounds of one vblank each hand over 2, 3, 2, 3, ... sectors: 20 in all, more than the queue's 16 across the rounds
    return first_bad(eq(r.states()[:21], [2] * 20 + [0]), first_bad(*[check_slot(r, img, i, ss[i], 2) for i in range(20)]))


@case
def the_queued_sector_is_taken_whole_from_the_fifo_and_the_one_at_hand_leaves_its_tail():
    img, ss = frames_img(4)
    q = ok_run(img, setup(hook=False) + ["busy 1", "tick 1", "fifo"])
    h = ok_run(img, setup(hook=False) + ["busy 0", "tick 1", "fifo"])
    # a sector at hand: 12 + 32 + 2,016 bytes are taken, the 280 bytes after them are still in the FIFO (fill bytes 0xee); a queued one: all 2,340
    return first_bad(eq(lines_of(q, "fifo "), ["fifo 00000000"]), eq(lines_of(h, "fifo "), ["fifo eeeeeeee"]))


# ---- CdRead2 and StUnSetRing --------------------------------------------------------------------------------------------------

@case
def CdRead2_sends_Setmode_with_the_low_byte_then_ReadS_and_sets_the_handler():
    img, ss = frames_img(4)
    g = ok_run(img, ["cdinit", f"ring {RING:x} {N}", f"bind {FLAG:x}", "stream 1 0 0 1 1", "setloc 0", "read2 1e8", "tick 1", "ringdump"])
    cl = [l for grp in g for l in grp if l.startswith(("cdctl", "read2"))]
    return first_bad(eq(cl, ["cdctl com=0e param=e8", "cdctl ret=1", "cdctl com=1b param=-", "cdctl ret=1", "read2 1"]),
                     eq(rings(g)[0].states()[:3], [2, 2, 0]))


@case
def CdRead2_without_bit_0x100_sets_no_handler_and_uses_the_low_byte_only():
    img, ss = frames_img(4)
    out = []
    for rd in (0xe8, 0x2e8, 0xfe8 & ~0x100):
        g = ok_run(img, ["cdinit", f"fill {RING:x} {N * 2048:x} a5", f"ring {RING:x} {N}", f"bind {FLAG:x}", "stream 1 0 0 1 1", "setloc 0", f"read2 {rd:x}", "tick 3", "ringdump"])
        cl = [l for grp in g for l in grp if l.startswith(("cdctl", "read2"))]
        out.append((cl[0], cl[2], rings(g)[0].states()[:4]))
    want = [(f"cdctl com=0e param={rd & 0xff:02x}", "cdctl com=1b param=-", [0, 0, 0, 0]) for rd in (0xe8, 0x2e8, 0xfe8 & ~0x100)]
    return first_bad(eq(out, want))


@case
def CdRead2_with_bit_0x100_and_without_0x20_stops_after_the_Setmode_before_ReadS():
    img, ss = frames_img(2)
    st, groups, rest = run(img, ["cdinit", f"ring {RING:x} {N}", "stream 1 0 0 1 1", "setloc 0", "read2 1c8"])
    return first_bad(eq(st, STATUS), eq(rest[0:2], ["cdctl com=0e param=c8", "cdctl ret=1"]), eq(rest[2].startswith("stop: CdRead2:") and "old type" in rest[2], True),
                     eq(len(rest), 3))


@case
def StUnSetRing_removes_the_handler_leaves_the_ring_and_sends_no_command():
    img, ss = frames_img(8)
    g = ok_run(img, setup(hook=False) + ["tick 1", "ringdump", "unset", "ringdump", "tick 4", "ringdump"])
    r1, r2, r3 = rings(g)
    cl = [l for grp in g for l in grp if l.startswith("cdctl")]
    return first_bad(eq(r1.states()[:4], [2, 2, 0, 0]), eq(r2.b, r1.b), eq(r3.b, r1.b), eq(cl, ["cdctl com=0e param=e8", "cdctl ret=1", "cdctl com=1b param=-", "cdctl ret=1"]),
                     eq(len(ss), 8))


@case
def CdRead2_returns_ReadSs_result_and_ignores_Setmodes():
    img, ss = frames_img(2)
    base = ["cdinit", f"ring {RING:x} {N}", "stream 1 0 0 1 1", "setloc 0"]
    a = ok_run(img, base + ["fail 1b", "read2 1e8"])
    b = ok_run(img, base + ["fail 0e", "read2 1e8"])
    la = [l for grp in a for l in grp if l.startswith(("cdctl", "read2"))]
    lb = [l for grp in b for l in grp if l.startswith(("cdctl", "read2"))]
    return first_bad(eq(la, ["cdctl com=0e param=e8", "cdctl ret=1", "cdctl com=1b failed by the test", "read2 0"]),
                     eq(lb, ["cdctl com=0e failed by the test", "cdctl com=1b param=-", "cdctl ret=1", "read2 1"]))


# ---- the stops -----------------------------------------------------------------------------------------------------------------

@case
def StSetRing_stops_for_fewer_than_two_slots_and_a_ring_outside_the_ram():
    img = Img()
    img.pad()
    r = []
    for n in (1, 0, -3):
        r.append(stops(img, ["cdinit", f"ring {RING:x} {n}"], "StSetRing", "slots"))
    r.append(stops(img, ["cdinit", f"ring {BASE - 0x1000:x} 4"], "StSetRing", "not inside"))
    r.append(stops(img, ["cdinit", f"ring {END - 3 * 2048 + 1:x} 3"], "StSetRing", "not inside"))
    r.append(stops(img, ["cdinit", f"ring {RING:x} 2147483647"], "StSetRing", "not inside"))
    r.append(stops(img, ["cdinit", f"ring {RING:x} 2097152"], "StSetRing", "not inside"))   # 2,097,152 slots of 2,048 bytes: 4 GB, 0 in 32 bits
    ok = ok_run(img, ["cdinit", f"ring {END - 2 * 2048:x} 2", "ringdump"])
    return first_bad(*r, eq(len(ok), 3))


@case
def StGetNext_takes_pointers_outside_the_ram_and_stops_for_a_null_one_and_with_no_ring():
    # the two pointers are the caller's own variables: addresses outside the game's RAM (or inside it) are no fault; null is
    img = Img()
    img.pad()
    r = [stops(img, ["cdinit", f"ring {RING:x} 4", "nextn 1 0"], "StGetNext", "address pointer", "null"),
         stops(img, ["cdinit", f"ring {RING:x} 4", "nextn 0 1"], "StGetNext", "header pointer", "null"),
         stops(img, ["cdinit", f"ring {RING:x} 4", "nextn 1 1"], "StGetNext", "null"),
         stops(img, ["cdinit", f"ring {RING:x} 4", f"nextp 0 {RING:x}"], "StGetNext", "address pointer", "null"),
         stops(img, ["cdinit", f"ring {RING:x} 4", f"nextp {RING:x} 0"], "StGetNext", "header pointer", "null"),
         stops(img, ["cdinit", "nexth"], "StGetNext", "no ring"),
         stops(img, ["cdinit", "nextn 1 0"], "StGetNext", "address pointer", "null"),   # the null is looked at before the ring
         stops(img, ["cdinit", "nextn 0 1"], "StGetNext", "header pointer", "null"),
         stops(img, ["cdinit", f"nextp {RING:x} {RING + 8:x}"], "StGetNext", "no ring")]
    ok = ok_run(img, ["cdinit", f"ring {RING:x} 4", f"nextp {BASE - 8:x} {RING:x}", f"nextp {RING:x} {END - 2:x}", f"nextp {END - 2:x} {RING:x}", "nexth"])
    return first_bad(*r, eq(len(ok), 6), eq([l for g in ok[2:5] for l in g if l.startswith("nextp")], ["nextp 1"] * 3))


@case
def StGetNext_with_the_callers_own_variables_gives_what_it_gives_with_the_games():
    img = Img()
    img.stream(0, 2, 1)
    img.stream(1, 2, 1)
    img.stream(0, 1, 2)
    img.pad()
    data0, data2 = RING + 32 * N, RING + 32 * N + 2016 * 2
    g = ok_run(img, setup() + ["nexth", "step 1", "nexth", "step 1", "nexth", "nexth", f"free {data0:x}", "nexth", "step 1", "nexth", "ringdump"])
    nx = [l.split()[1:4] for l in lines_of(g, "nexth ")]
    ra = rings(g)[0]
    return first_bad(eq(nx, [["1", "0", "0"], ["1", "0", "0"], ["0", f"{data0:x}", f"{RING:x}"], ["1", "0", "0"], ["1", "0", "0"],
                            ["0", f"{data2:x}", f"{RING + 64:x}"]]), eq(ra.states(), [0, 0, 4, 0] + [0] * 11))


@case
def StGetNext_ticks_once_when_it_has_no_frame_and_not_when_it_gives_one():
    img = Img()
    img.stream(0, 1, 1)
    img.pad()
    d0 = RING + 32 * N
    g = ok_run(img, setup() + ["nexth", "nexth", "step 1", "nexth", "nexth", "nexth"])
    nx = [l.split()[1:] for l in lines_of(g, "nexth ")]
    # no frame: one tick each time; the frame: none; then the frame is in state 4 (not 2): one tick again
    return eq(nx, [["1", "0", "0", "1"], ["1", "0", "0", "1"], ["0", f"{d0:x}", f"{RING:x}", "0"], ["1", "0", "0", "1"], ["1", "0", "0", "1"]])


@case
def StGetNext_ticks_once_behind_a_wrap_marker_with_nothing_at_slot_0_and_not_when_a_frame_is_there():
    # as a_marker_is_cleared_by_getnext_only_when_end_is_set: end 0, the marker stays, slot 0 is free: one tick
    img = Img()
    ss = [img.stream(0, 1, k) for k in (1, 2, 3, 4, 5)]
    img.pad()
    dd = lambda i: RING + 32 * 5 + 2016 * i
    g = ok_run(img, setup(n=5, end=0) + ["step 5"] + [x for i in range(4) for x in ("nexth", f"free {dd(i):x}")] + ["nexth"])
    nx = [l.split()[1:] for l in lines_of(g, "nexth ")]
    first = eq([x[0] for x in nx], ["0", "0", "0", "0", "1"])
    t1 = eq([x[3] for x in nx], ["0", "0", "0", "0", "1"])
    # and as wrap_to_slot_0_when_it_is_free: end set, the marker is followed to a frame at slot 0: no tick
    img = Img()
    for k in (1, 2, 3, 4, 5):
        img.stream(0, 1, k)
    img.pad()
    g = ok_run(img, setup(n=5, end=0x0fffffff) + ["step 3", "nexth", f"free {dd(0):x}", "step 2", "nexth", f"free {dd(1):x}", "nexth", f"free {dd(2):x}", "nexth",
                                                  f"free {dd(3):x}", "nexth"])
    nx = [l.split()[1:] for l in lines_of(g, "nexth ")]
    return first_bad(first, t1, eq([x[0] for x in nx], ["0"] * 5), eq([x[3] for x in nx], ["0"] * 5), eq(nx[4][1:3], [f"{dd(0):x}", f"{RING:x}"]))


@case
def StFreeRing_stops_for_pointers_outside_the_ram_the_ring_and_a_frame_past_the_ring():
    img = Img()
    img.pad()
    d = RING + 32 * 4
    r = [stops(img, ["cdinit", f"ring {RING:x} 4", f"free {BASE - 8:x}"], "StFreeRing", "not inside"),
         stops(img, ["cdinit", f"ring {RING:x} 4", f"free {END:x}"], "StFreeRing", "not inside"),
         stops(img, ["cdinit", f"ring {RING:x} 4", f"free {RING + 0x40:x}"], "StFreeRing", "data blocks"),
         stops(img, ["cdinit", f"ring {RING:x} 4", f"free {d + 4 * 2016:x}"], "StFreeRing", "data blocks"),
         stops(img, ["cdinit", f"ring {RING:x} 4", f"put {RING + 64:x} 0400", f"put {RING + 70:x} 0300", f"free {d + 2 * 2016:x}"], "StFreeRing", "reaches past"),
         stops(img, ["cdinit", f"free {d:x}"], "StFreeRing", "no ring")]
    ok = ok_run(img, ["cdinit", f"ring {RING:x} 4", f"free {d + 3 * 2016:x}"])
    return first_bad(*r, eq(ok[-1], ["free 1"]) if ok else "no output")


@case
def the_handler_stops_with_no_ring_and_a_bind_outside_the_ram_stops():
    img = Img()
    img.pad()
    return first_bad(stops(img, ["cdinit", "intr"], "StCdInterrupt", "no ring"), stops(img, ["cdinit", f"bind {BASE - 4:x}"], "port_stream_bind", "not inside"),
                     stops(img, ["cdinit", f"bind {END - 2:x}"], "port_stream_bind", "not inside"), eq(len(ok_run(img, ["cdinit", f"bind {END - 4:x}"])), 2))


@case
def a_call_of_the_handler_from_f1_returns_at_step_1_and_from_f2_stops():
    img = Img()
    img.stream(0, 1, 1)
    img.stream(0, 1, 2)
    img.pad()
    g = ok_run(img, setup() + ["reenter 1", "step 2", "ringdump"])
    r = rings(g)[0]
    # f1 calls the handler while the frame's last transfer runs (dmabusy 1): it returns at once; the ring is as without the call
    stop = stops(img, setup(end=2) + ["reenter 2", "step 2"], "StCdInterrupt", "inside itself")
    return first_bad(eq(lines_of(g, "f1 re-entered"), ["f1 re-entered returned"] * 2), eq(r.states()[:3], [2, 2, 0]), stop)


@case
def the_stream_handler_called_by_the_disc_layer_runs_with_the_depth_raised_and_takes_the_error_code_5():
    img = Img()
    img.stream(0, 1, 1)   # then the image ends: a disc error (5) goes to the handler, which returns at CdReady
    g = ok_run(img, setup(hook=False) + ["tick 3", "ringdump", "ready"])
    return first_bad(eq(lines_of(g, "ready "), ["ready 0"]), eq(rings(g)[0].states()[:2], [2, 0]), eq(cb_states(g, "f1")[0][0], 2), eq([l.split()[1] for l in lines_of(g, "f1")], ["depth=1"]))


# ---- the run ------------------------------------------------------------------------------------------------------------------

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
    with tempfile.TemporaryDirectory(prefix="hoststream-test-") as tmp:
        root = Path(tmp)
        (root / "streammain.c").write_text(DRIVER_C)
        exe = root / "streamtest.exe"
        s = args.src
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(s), "-o", str(exe), str(root / "streammain.c"), str(s / "stream.c"), str(s / "cd.c"),
                               str(s / "xa.c"), str(s / "disc.c"), str(s / "sha256.c")] + LINK_FLAGS, capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the test program does not build:\n" + proc.stderr[-2500:])
            return 1
        Ctx.exe = exe
        Ctx.root = root
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
