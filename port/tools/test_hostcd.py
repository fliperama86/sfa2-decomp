#!/usr/bin/env python3
"""Controls for the disc layer, port/src/cd.c, built natively with the host's `cc`.

What is tested: the host routines of the CD library functions that the game
calls, on disc images of invented bytes (every sector says which sector it is
in its first two bytes; the rest is a pattern), with a made-up ready handler,
a made-up port_tick and a made-up RAM in the test's own C (a script on
standard input drives it, one command a line). Cases: Setloc and ReadN
delivering sectors in order, at the console's rate counted by the tick
(2, 3, 2, 3 ... a tick at double speed and 1, 1, 1, 2 ... at single speed,
the totals of 60 ticks, the carry cleared by a read's start and by a change
of the speed bit, a read paused by the handler in the middle of a tick's
delivery) and none outside a tick;
CdGetSector handing a sector out in pieces; Pause and a new Setloc in the
middle of a read, and ReadN continuing after a Pause; reading past the end of
the image (a disc error to the handler, the read stopped); the position
conversions at their borders; the word record; the polling reader (no
handler: one sector waits for CdReady); the implied Setloc of SeekL and
ReadS and its second response; GetlocP; a stream of audio that no one polls (the game's music: with the XA bit set and no CdReady
since the read began the drive goes on over the sectors that are no audio, each replacing the last in the FIFO; after a CdReady, or
with the bit clear, one sector waits; a new read begins the question again);
XA audio sectors: consumed in silence with no sink, and with a sink decoded and queued
(the filter on and off, the file and channel, the volumes of CdMix, the hold when the queue is full and its release, the reset at a
read's start, refused sectors, the lock calls); what CdInit does to the sound chip and to the drive's mix (the library's CD_initvol: the
rig's seam is a made-up file of the chip's registers and a record of the calls, with and without a model of the chip); the
library variables the game's C reads; an unknown command's stop and a
Setmode with the 2,328-byte bit (0x10); the whole-sector mode (bit 0x20) on
an image whose every byte is a pattern (a sector as one piece of 2,340 bytes and
as pieces, zeros past the end, the mode changed between sectors, the audio, the
word record and the hook); the host ready handler (port_cd_host_ready: its calls
and codes, the game's variable not called, the polling rule and the ring guard
not applied, a handler that pauses or starts a read, the depth counter, CdInit);
the buffer CdGetSector is given: its
start or its end outside the PS1's RAM, a count that overflows 32 bits, a
negative count, the last bytes of RAM (accepted), each ending the program
with a line and status 6 before a byte is copied; the guard by which the drive
waits for the game's ring (the fill for all 49 pairs of places, places outside 1
to 7, held vblanks and the carry, a fill that reaches 4 within a row, the 30
held vblanks and the forced row, and the loads and calls that are not guarded).
The seam of the rig for it: the library variable of the ready handler can hold
the game's stream handler (0x8014f918) or another, and the two bytes of the
game's stream object (0x80190948) are in the rig's RAM, which cd.c reads like the
library's variables; the made-up handler steps the writer's place when told.

What is NOT tested here: the table's use by the driver (library.c), the
Windows pointers, and the game itself. Those need the linked program.

The expected values are worked out here from each fixture, never read back
from the runtime. It needs `cc`; without it this file says so and ends with
status 2.
"""

from __future__ import annotations

import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"
sys.path.insert(0, str(HERE))
import test_hostxa as X  # noqa: E402  (the decoder's reference, for the expected frames)

SECTOR = 2352
HANDLER_VALUE = 0x1234

TEST_MAIN = r'''
#include "cd.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *ram;
static int ticks, autotick;
static int h_on;
static unsigned h_dest;
static int h_pieces[16], h_np, h_stop, h_calls, h_count;
static int a_when, a_calls, a_arg;   /* an action of the ready handler: on call number a_when (0: on every call) */
static char a_kind;
static unsigned char rbuf[4096];   /* the runtime's own buffer for port_cd_fifo_read and port_cd_store */
static int g_calls, hh_calls;   /* calls of the game's made-up handler and of the host ready handler */
static int ring_adv;   /* the made-up handler steps the writer's place of the game's ring (byte 0x16 of the stream object) as the game's does */

volatile int port_handler_depth;   /* the driver's counter; cd.c raises it around the ready handler */
/* The seams of the streamed audio: the audio lock (counted: calls, depth, an unlock without a lock), the frame count and the trace
 * (to standard error, so that the lines the cases read on standard output are the same as before). */
static int lock_calls, lock_depth, lock_bad;
void port_audio_lock(void) { lock_calls++; lock_depth++; }
void port_audio_unlock(void) { if (--lock_depth < 0) lock_bad++; }
/* The seam of the sound chip (iopage.c's two routines for CdInit): a made-up file of the chip's 256 registers, all 0 at start, and a
 * record of every call in order ("r180" a read, "w180=3fff" a write). With chip_on 0 it stands for a build with no model of the chip:
 * the call is recorded with a "-" behind it, nothing is read or written, and the return is 0. An offset that is odd or not below
 * 0x200 is recorded with a "!" and refused the same way. */
static unsigned short chip[0x100];
static int chip_on = 1;
static char chip_log[1024];
static void chip_note(const char *fmt, unsigned a, unsigned b)
{
    size_t n = strlen(chip_log);
    if (n + 24 < sizeof chip_log) snprintf(chip_log + n, sizeof chip_log - n, fmt, a, b);
}
int port_iopage_chip_read(unsigned offset, unsigned *value)
{
    if ((offset & 1) || offset >= 0x200) { chip_note(" r%x!", offset, 0); return 0; }
    if (!chip_on) { chip_note(" r%x-", offset, 0); return 0; }
    chip_note(" r%x", offset, 0);
    *value = chip[offset >> 1];
    return 1;
}
int port_iopage_chip_write(unsigned offset, unsigned value)
{
    if ((offset & 1) || offset >= 0x200) { chip_note(" w%x=%x!", offset, value); return 0; }
    if (!chip_on) { chip_note(" w%x=%x-", offset, value); return 0; }
    chip_note(" w%x=%x", offset, value);
    chip[offset >> 1] = (unsigned short)value;
    return 1;
}
static unsigned frames;
unsigned port_frames(void) { return frames; }
void port_trace_line(const char *fmt, ...) { va_list ap; va_start(ap, fmt); fputs("T ", stderr); vfprintf(stderr, fmt, ap); fputc('\n', stderr); va_end(ap); }
/* jumps.c's check of a callback's address; cd.c calls it only when port_cd_call is not set, and here it is, so this
 * stands in for the link. The real check on the ready callback is exercised through the linked program
 * (test_hostlaunch.py). */
void port_target_check(const char *path, const void *target) { (void)path; (void)target; }
void port_tick(void)
{
    ticks++;
    if (autotick) port_cd_tick();
}

static unsigned char *at(unsigned a) { return ram + (a - PORT_RAM_BASE); }

static unsigned sum(const unsigned char *p, unsigned n)
{
    unsigned s = 0, i;
    for (i = 0; i < n; i++) s += p[i];
    return s;
}

static void handler(unsigned h, int intr, unsigned char *res)
{
    int i, done = 0;
    if (h != 0x7777u) g_calls++;   /* 0x7777: the host ready handler below, which calls this body */
    if (h != 0x1234 && h != 0x8014f918u && h != 0x2345 && h != 0x7777u) { printf("H bad handler value %x\n", h); return; }
    printf("H intr=%d res0=%02x\n", intr, res[0]);
    if (intr == 1) h_count++;
    if (intr == 1 && ring_adv) { unsigned char *w = at(0x80190948u) + 0x16; *w = (unsigned char)(*w + 1 == 8 ? 1 : *w + 1); }
    if (intr == 1 && a_kind && (!a_when || ++a_calls == a_when)) {
        unsigned char p[4] = {0, 0, 0, 0}, rs[8];
        if (a_kind == 'N' || a_kind == 'S') { port_CdIntToPos(a_arg, p); port_CdControl(a_kind == 'N' ? 6 : 0x1b, p, rs); }
        else if (a_kind == 'M') { p[0] = (unsigned char)a_arg; port_CdControl(0xe, p, rs); }
        else if (a_kind == 'I') port_CdInit();
        else if (a_kind == 'R') { port_CdReady(0, rs); }
        else if (a_kind == 'P') { port_CdControlB(9, 0, rs); }
    }
    if (intr != 1 || !h_on) return;
    for (i = 0; i < h_np; i++) {
        int r = port_CdGetSector(at(h_dest) + 4 * done, h_pieces[i]);
        printf("  piece %d words %d ret %d\n", i, h_pieces[i], r);
        done += h_pieces[i];
    }
    printf("  sector %u sum %u\n", at(h_dest)[0] | at(h_dest)[1] << 8, sum(at(h_dest), 4u * (unsigned)done));
    if (h_stop && ++h_calls == h_stop) {   /* the game's end of a stream: clear the handler, Pause with a blocking wait */
        unsigned char *p = at(PORT_CD_VAR_READY_CB);
        p[0] = p[1] = p[2] = p[3] = 0;
        printf("  pause from the handler ret %d\n", port_CdControlB(9, 0, 0));
    }
}

/* The runtime's own ready handler (port_cd_host_ready): marks its call, then does what the made-up game handler does. */
static void host_handler(int intr, unsigned char *res)
{
    hh_calls++;
    printf("HH intr=%d res0=%02x depth=%d\n", intr, res[0], port_handler_depth);
    handler(0x7777u, intr, res);
}

static void hook(unsigned a, unsigned b) { printf("K %x %u\n", a, b); }

static void show(const char *tag, int ret, const unsigned char *res)
{
    int i;
    printf("%s ret=%d", tag, ret);
    if (res) { printf(" res="); for (i = 0; i < 8; i++) printf("%02x%s", res[i], i < 7 ? " " : ""); }
    printf("\n");
}

int main(int argc, char **argv)
{
    char line[512], err[PORT_ERR];
    struct port_disc d;
    if (argc < 2) return 64;
    ram = calloc(1, PORT_RAM_SIZE);
    port_cd_ram = ram;
    port_cd_call = handler;
    if (port_disc_open(&d, argv[1], err, sizeof err) != 0) { printf("refused: %s\n", err); return 2; }
    port_cd_init(&d);
    while (fgets(line, sizeof line, stdin)) {
        char cmd[32], arg[32];
        char *rest = line;
        int n = 0;
        if (sscanf(line, "%31s %n", cmd, &n) < 1) continue;
        rest += n;
        if (!strcmp(cmd, "cdinit")) printf("R %d\n", port_CdInit());
        else if (!strcmp(cmd, "autotick")) autotick = atoi(rest);
        else if (!strcmp(cmd, "ticks")) printf("T %d\n", ticks);
        else if (!strcmp(cmd, "tick")) { int k = atoi(rest); while (k-- > 0) { frames++; port_cd_tick(); } }
        else if (!strcmp(cmd, "tickc")) { int k = atoi(rest); while (k-- > 0) { int before = h_count; frames++; port_cd_tick(); printf("V %d\n", h_count - before); } }   /* ticks one at a time, each with the sectors it gave the handler */
        else if (!strcmp(cmd, "act")) { char k = 0; int w = 0, a = 0; if (sscanf(rest, "%d %c %d", &w, &k, &a) < 2) return 64; a_when = w; a_kind = k; a_arg = a; a_calls = 0; }   /* what the ready handler does: N ReadN to a, S ReadS to a, M Setmode a, I CdInit, R CdReady(0) */
        else if (!strcmp(cmd, "cb")) {   /* the ready handler variable: 0 none, 1 the made-up handler, 2 the game's stream handler, 3 another handler */
            static const unsigned values[4] = { 0u, HANDLER_VALUE_C, 0x8014f918u, 0x2345u };
            unsigned v = values[atoi(rest) & 3]; unsigned char *p = at(PORT_CD_VAR_READY_CB);
            p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24);
        }
        else if (!strcmp(cmd, "ring")) { unsigned w, r; if (sscanf(rest, "%u %u", &w, &r) != 2) return 64; at(0x80190948u)[0x16] = (unsigned char)w; at(0x80190948u)[0x17] = (unsigned char)r; }   /* the two places of the game's ring */
        else if (!strcmp(cmd, "fr")) {   /* port_cd_fifo_read into rbuf+off, bytes; rbuf is filled with 0xa5 first on request by "rfill" */
            unsigned off, n, real; int sec = 12345;
            if (sscanf(rest, "%u %u", &off, &n) != 2 || off + n > sizeof rbuf) return 64;
            real = port_cd_fifo_read(rbuf + off, n, &sec);
            printf("FR real %u sector %d\n", real, sec);
        }
        else if (!strcmp(cmd, "frram")) {   /* port_cd_fifo_read with a destination INSIDE the game's RAM (a caller's mistake): it must still touch neither the record nor the hook */
            unsigned a, n; int sec;
            if (sscanf(rest, "%x %u", &a, &n) != 2) return 64;
            printf("FQ real %u\n", port_cd_fifo_read(at(a), n, &sec));
        }
        else if (!strcmp(cmd, "frnull")) { unsigned n = (unsigned)atoi(rest); printf("FN real %u\n", port_cd_fifo_read(rbuf, n, 0)); }
        else if (!strcmp(cmd, "rfill")) { unsigned i, v = (unsigned)atoi(rest); for (i = 0; i < sizeof rbuf; i++) rbuf[i] = (unsigned char)(v + i); }
        else if (!strcmp(cmd, "rdump")) {
            unsigned off, k, i;
            if (sscanf(rest, "%u %u", &off, &k) != 2 || off + k > sizeof rbuf) return 64;
            printf("M"); for (i = 0; i < k; i++) printf(" %02x", rbuf[off + i]); printf("\n");
        }
        else if (!strcmp(cmd, "st")) {   /* port_cd_store(game address, rbuf+off, bytes, sector); the address may be anywhere (raw) */
            unsigned a, off, n; int sec;
            if (sscanf(rest, "%x %u %u %d", &a, &off, &n, &sec) != 4 || off + n > sizeof rbuf) return 64;
            port_cd_store(ram + ((size_t)a - PORT_RAM_BASE), rbuf + off, n, sec);
            printf("ST done\n");
        }
        else if (!strcmp(cmd, "host")) port_cd_host_ready(atoi(rest) ? host_handler : 0);   /* the host ready handler on or off (CdInit removes it) */
        else if (!strcmp(cmd, "calls")) printf("CA game %d host %d\n", g_calls, hh_calls);
        else if (!strcmp(cmd, "depth")) printf("D %d\n", port_handler_depth);
        else if (!strcmp(cmd, "hook")) port_cd_written_hook = atoi(rest) ? hook : 0;
        else if (!strcmp(cmd, "adv")) ring_adv = atoi(rest);
        else if (!strcmp(cmd, "stopafter")) { h_stop = atoi(rest); h_calls = 0; }
        else if (!strcmp(cmd, "handler")) {
            char *q = rest; char *e;
            h_dest = (unsigned)strtoul(q, &e, 16); q = e; h_np = 0; h_on = 1;
            while (h_np < 16) { long w = strtol(q, &e, 10); if (e == q) break; h_pieces[h_np++] = (int)w; q = e; }
        }
        else if (!strcmp(cmd, "ctl")) {
            unsigned char param[8], res[8], *pp = 0;
            char kind; unsigned com; char *q = rest, *e; int np = 0, ret;
            memset(res, 0xee, 8);
            kind = *q++; com = (unsigned)strtoul(q, &e, 16); q = e;
            while (np < 8) { unsigned long b = strtoul(q, &e, 16); if (e == q) break; param[np++] = (unsigned char)b; q = e; }
            if (np) pp = param;
            if (kind == 'F') ret = port_CdControlF((int)com, pp);
            else if (kind == 'B') ret = port_CdControlB((int)com, pp, res);
            else ret = port_CdControl((int)com, pp, res);
            show("C", ret, kind == 'F' ? 0 : res);
        }
        else if (!strcmp(cmd, "setloc")) {
            unsigned char p[4] = {0, 0, 0, 0}, res[8]; int ret;
            port_CdIntToPos(atoi(rest), p);
            ret = port_CdControlB(2, p, res);
            show("L", ret, res);
        }
        else if (!strcmp(cmd, "readn")) {
            unsigned char p[4] = {0, 0, 0, 0}, res[8]; int ret;
            port_CdIntToPos(atoi(rest), p);
            ret = port_CdControlB(2, p, res);
            ret = ret && port_CdControl(6, 0, res);
            show("RN", ret, res);
        }
        else if (!strcmp(cmd, "ready")) { unsigned char res[8]; int ret; memset(res, 0xee, 8); ret = port_CdReady(atoi(rest), res); show("Y", ret, res); }
        else if (!strcmp(cmd, "sync")) { unsigned char res[8]; int ret; memset(res, 0xee, 8); ret = port_CdSync(atoi(rest), res); show("S", ret, res); }
        else if (!strcmp(cmd, "gs")) {
            unsigned a, w; int ret;
            if (sscanf(rest, "%x %u", &a, &w) != 2) return 64;
            ret = port_CdGetSector(at(a), (int)w);
            printf("G ret=%d first=%u sum=%u last=%02x\n", ret, at(a)[0] | at(a)[1] << 8, sum(at(a), w * 4), w ? at(a)[w * 4 - 1] : 0);
        }
        else if (!strcmp(cmd, "gsraw")) {   /* CdGetSector with an address of the game's choosing, any 32-bit value, and a count (maybe huge or negative) */
            unsigned a; long w; int ret;
            if (sscanf(rest, "%x %ld", &a, &w) != 2) return 64;
            ret = port_CdGetSector(ram + ((size_t)a - PORT_RAM_BASE), (int)w);
            printf("G ret=%d\n", ret);
        }
        else if (!strcmp(cmd, "dump")) {
            unsigned a, k, i;
            if (sscanf(rest, "%x %u", &a, &k) != 2) return 64;
            printf("M");
            for (i = 0; i < k; i++) printf(" %02x", at(a)[i]);
            printf("\n");
        }
        else if (!strcmp(cmd, "src")) { unsigned a = (unsigned)strtoul(rest, 0, 16); printf("P %d\n", port_cd_source_at(a)); }
        else if (!strcmp(cmd, "i2p")) { unsigned char p[4] = {9, 9, 9, 9}; unsigned char *r = port_CdIntToPos(atoi(rest), p); printf("I %02x %02x %02x %02x same=%d\n", p[0], p[1], p[2], p[3], r == p); }
        else if (!strcmp(cmd, "p2i")) { unsigned a, b, c; unsigned char p[3]; if (sscanf(rest, "%x %x %x", &a, &b, &c) != 3) return 64; p[0] = (unsigned char)a; p[1] = (unsigned char)b; p[2] = (unsigned char)c; printf("J %d\n", port_CdPosToInt(p)); }
        else if (!strcmp(cmd, "word")) { unsigned a = (unsigned)strtoul(rest, 0, 16); unsigned char *p = at(a); printf("W %08x\n", p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24); }
        else if (!strcmp(cmd, "mix")) { printf("X %d\n", port_CdMix(0)); }
        else if (!strcmp(cmd, "mixv")) { unsigned char v[4]; unsigned a, b, c, d; if (sscanf(rest, "%u %u %u %u", &a, &b, &c, &d) != 4) return 64; v[0] = (unsigned char)a; v[1] = (unsigned char)b; v[2] = (unsigned char)c; v[3] = (unsigned char)d; printf("X %d\n", port_CdMix(v)); }
        else if (!strcmp(cmd, "sink")) port_xa_set_sink(atoi(rest));
        else if (!strcmp(cmd, "chip")) chip_on = atoi(rest);   /* 0: no model of the chip */
        else if (!strcmp(cmd, "chipset")) { unsigned o, v; if (sscanf(rest, "%x %x", &o, &v) != 2 || (o & 1) || o >= 0x200) return 64; chip[o >> 1] = (unsigned short)v; }
        else if (!strcmp(cmd, "chiplog")) { printf("CL%s\n", chip_log); chip_log[0] = 0; }   /* the calls since the last chiplog, in order */
        else if (!strcmp(cmd, "chipregs")) printf("CR main %04x %04x cd %04x %04x control %04x current %04x %04x\n", chip[0x180 >> 1], chip[0x182 >> 1], chip[0x1b0 >> 1], chip[0x1b2 >> 1], chip[0x1aa >> 1], chip[0x1b8 >> 1], chip[0x1ba >> 1]);
        else if (!strcmp(cmd, "chipsum")) { unsigned i, s = 0, nz = 0; for (i = 0; i < 0x100; i++) { s += chip[i]; nz += chip[i] != 0; } printf("CS %u registers not 0, sum %u\n", nz, s); }
        else if (!strcmp(cmd, "xac")) { struct port_xa_counts c; port_xa_counts_get(&c); printf("XC played %u held %u refused %u skipped %u silent %u frames %u pulled %u queued %u\n", c.played, c.held, c.refused, c.skipped, c.silent, c.frames, c.pulled, c.queued); }
        else if (!strcmp(cmd, "locks")) printf("LK calls %d depth %d bad %d\n", lock_calls, lock_depth, lock_bad);
        else if (!strcmp(cmd, "pull")) {   /* what PsyZ does: the pull is called with the lock held, so it takes none itself */
            static short buf[48000 * 2]; size_t n = (size_t)atoi(rest), got, i;
            if (n > 48000) return 64;
            got = port_xa_pull(buf, n);
            printf("PL %u", (unsigned)got);
            for (i = 0; i < got * 2; i++) printf(" %d", buf[i]);
            printf("\n");
        }
        else if (!strcmp(cmd, "drain")) { static short buf[48000 * 2]; size_t got = port_xa_pull(buf, 48000); printf("DR %u\n", (unsigned)got); }
        else { printf("unknown script command %s\n", cmd); return 64; }
        (void)arg;
        fflush(stdout);
    }
    return 0;
}
'''.replace("HANDLER_VALUE_C", "0x1234u")


# ---- the images ------------------------------------------------------------

def sector_data(n: int) -> bytes:
    b = bytearray(((n * 13 + i) & 0xFF) for i in range(2048))
    b[0] = n & 0xFF
    b[1] = (n >> 8) & 0xFF
    return bytes(b)


def make_image(path: Path, count: int, audio: dict[int, tuple[int, int, int]] | None = None) -> None:
    """count raw sectors of 2352 bytes. audio: sector -> (file, channel, submode)."""
    audio = audio or {}
    out = bytearray()
    for n in range(count):
        raw = bytearray(SECTOR)
        raw[0:12] = bytes([0] + [0xFF] * 10 + [0])
        raw[12:16] = bytes([0, 2, n % 75, 2])
        f, c, sm = audio.get(n, (1, 0, 0x08))
        raw[16:24] = bytes([f, c, sm, 0, f, c, sm, 0])
        raw[24:24 + 2048] = sector_data(n)
        out += raw
    path.write_bytes(bytes(out))


def total(n: int) -> int:
    return sum(sector_data(n))


def bcd(v: int) -> int:
    return (v // 10) * 16 + v % 10


def pos_of(lba: int) -> tuple[int, int, int]:
    f = lba + 150
    return bcd(f // 75 // 60), bcd(f // 75 % 60), bcd(f % 75)


# ---- running -----------------------------------------------------------------

def build(root: Path, cc: str) -> Path:
    main = root / "cdmain.c"
    main.write_text(TEST_MAIN)
    exe = root / "cdtest"
    proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-o", str(exe), str(main), str(SRC / "cd.c"), str(SRC / "xa.c"), str(SRC / "disc.c"), str(SRC / "sha256.c")],
                          capture_output=True, text=True, timeout=180)
    if proc.returncode != 0:
        raise RuntimeError("the test program does not build:\n" + proc.stderr[-1500:])
    return exe


def run(exe: Path, image: Path, script: str) -> tuple[int, list[str]]:
    proc = subprocess.run([str(exe), str(image)], input=script, capture_output=True, text=True, timeout=60)
    return proc.returncode, proc.stdout.splitlines()


def run_err(exe: Path, image: Path, script: str) -> tuple[int, list[str], list[str]]:
    proc = subprocess.run([str(exe), str(image)], input=script, capture_output=True, text=True, timeout=60)
    return proc.returncode, proc.stdout.splitlines(), proc.stderr.splitlines()


def check(name: str, ok: bool, detail: str = ""):
    return name, None if ok else detail


def hexs(*b: int) -> str:
    return " ".join(f"{x:02x}" for x in b)


# ---- cases -------------------------------------------------------------------

def read_cases(root: Path, exe: Path):
    img = root / "r.bin"
    make_image(img, 60)
    # cdinit leaves the mode at 0: single speed, so the ticks give 1, 1, 1, 2 sectors (see rate_cases)
    script = ("cdinit\ncb 1\nhandler 80100000 512\nreadn 10\n"
              "ticks\n"                       # nothing delivered by the commands
              "tick 1\ntick 1\n")
    rc, lines = run(exe, img, script)
    got = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    want = [10, 11]   # single speed: one sector at each of the first two ticks
    yield check("Setloc and ReadN deliver the sectors in order, one a tick at single speed", got == want and rc == 0, f"{got} {want} {lines}")
    rc, lines2 = run(exe, img, "cdinit\ncb 1\nhandler 80100000 512\nctl N e 80\nreadn 10\ntick 1\ntick 1\n")
    got2 = [int(l.split()[1]) for l in lines2 if l.startswith("  sector")]
    yield check("at double speed the first two ticks give 2 and 3 sectors, in order", got2 == [10, 11, 12, 13, 14] and rc == 0, f"{got2}")
    sums = [int(l.split()[3]) for l in lines if l.startswith("  sector")]
    yield check("each sector's 2048 bytes reach the handler's buffer", sums == [total(s) for s in want], f"{sums}")
    intr = [l for l in lines if l.startswith("H ")]
    yield check("the handler gets interrupt code 1 and a status result", all(l == "H intr=1 res0=22" for l in intr) and len(intr) == 2, f"{intr}")
    # no delivery outside a tick, however many commands and polls are made
    script = "cdinit\ncb 1\nhandler 80100000 512\nreadn 10\nsync 1\nsync 1\nready 1\nctl N 1\n"
    rc, lines = run(exe, img, script)
    yield check("nothing is delivered outside port_cd_tick", not any(l.startswith("H ") for l in lines), f"{lines}")


def pieces_cases(root: Path, exe: Path):
    img = root / "p.bin"
    make_image(img, 30)
    # the handler takes one sector as 0x180 and 0x80 words, then the game's dump of a remainder
    script = "cdinit\ncb 1\nhandler 80100000 384 128\nreadn 5\ntick 1\n"
    rc, lines = run(exe, img, script)
    ok = ["  sector 5 sum %d" % total(5) in lines, "  piece 0 words 384 ret 1" in lines, "  piece 1 words 128 ret 1" in lines]
    yield check("CdGetSector in two pieces gives the whole sector", all(ok), f"{lines}")
    # three pieces, direct calls: first 0x100 words, then 0x100 more, then past the end
    script = ("cdinit\nreadn 7\nautotick 1\nready 0\nautotick 0\ngs 80100000 256\ngs 80100400 256\ngs 80100800 16\n"
              "dump 80100800 8\n")
    rc, lines = run(exe, img, script)
    g = [l for l in lines if l.startswith("G ")]
    first = sector_data(7)
    s1 = sum(first[:1024])
    s2 = sum(first[1024:])
    yield check("successive CdGetSector calls give successive pieces",
                len(g) == 3 and g[0] == f"G ret=1 first=7 sum={s1} last={first[1023]:02x}" and g[1].split()[3] == f"sum={s2}" and g[1].split()[4] == f"last={first[2047]:02x}", f"{g}")
    yield check("reading past the sector's end gives zeros and still returns 1",
                g[2] == "G ret=1 first=0 sum=0 last=00" and lines[-1] == "M 00 00 00 00 00 00 00 00", f"{lines}")
    # a new sector starts the FIFO over
    script = "cdinit\nreadn 7\nautotick 1\nready 0\nautotick 0\ngs 80100000 512\ntick 1\ngs 80100000 512\n"
    rc, lines = run(exe, img, script)
    g = [l for l in lines if l.startswith("G ")]
    yield check("the next sector starts at its first byte", len(g) == 2 and g[1].startswith("G ret=1 first=8 "), f"{g} {lines}")


def pause_cases(root: Path, exe: Path):
    img = root / "z.bin"
    make_image(img, 80)
    # single speed (cdinit leaves the mode 0): one sector at the tick after each read's start
    script = ("cdinit\ncb 1\nhandler 80100000 512\nreadn 10\ntick 1\n"
              "ctl B 9\n"
              "tick 2\n"
              "readn 40\ntick 1\n"
              "ctl B 9\nctl N 6\ntick 1\n")
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    want = [10, 40, 41]   # the second read and the continuation after Pause each start with a zero carry
    yield check("Pause stops delivery; a new Setloc and ReadN restart elsewhere; ReadN after Pause continues", sec == want, f"{sec} {want}")
    pa = [l for l in lines if l.startswith("C ret=")]
    yield check("CdControlB(Pause) returns 1 and the final status shows the idle drive",
                pa and pa[0] == "C ret=1 res=02 00 00 00 00 00 00 00", f"{pa}")
    # the game's end of a stream: the handler clears itself and Pauses with a blocking wait, in the tick
    rc, lines = run(exe, img, "cdinit\ncb 1\nhandler 80100000 512\nstopafter 2\nreadn 10\nautotick 1\ntick 1\ntick 2\nsync 1\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("Pause with a blocking wait from inside the handler ends the read at once",
                rc == 0 and sec == [10, 11] and "  pause from the handler ret 1" in lines and "S ret=2 res=02 00 00 00 00 00 00 00" in lines, f"{rc} {lines}")
    # nothing is delivered by a tick when no read was ever started
    rc, lines = run(exe, img, "cdinit\ncb 1\nhandler 80100000 512\ntick 3\n")
    yield check("a tick with no read on delivers nothing", not any(l.startswith("H ") for l in lines), f"{lines}")


def end_cases(root: Path, exe: Path):
    img = root / "e.bin"
    make_image(img, 12)
    script = "cdinit\ncb 1\nhandler 80100000 512\nreadn 9\ntick 4\ntick 4\nready 1\nsync 1\n"
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    intr = [l for l in lines if l.startswith("H ")]
    yield check("sectors up to the image's end are delivered", sec == [9, 10, 11], f"{sec}")
    yield check("past the end the handler gets interrupt code 5 once and the read stops",
                intr[-1] == "H intr=5 res0=03" and sum(1 for l in intr if "intr=5" in l) == 1 and len(intr) == 4, f"{intr}")
    yield check("the error flag stays for CdReady, which returns 5 and clears it",
                [l for l in lines if l.startswith("Y ")] == ["Y ret=5 res=03 40 00 00 00 00 00 00"], f"{lines}")
    # a truncated last sector counts as the end
    img2 = root / "e2.bin"
    img2.write_bytes(img.read_bytes()[: 11 * SECTOR + 1000])
    rc, lines = run(exe, img2, "cdinit\ncb 1\nhandler 80100000 512\nreadn 10\ntick 4\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("a truncated last sector is the end", sec == [10] and "H intr=5 res0=03" in lines, f"{lines}")
    # Setloc beyond the image is accepted (the error comes when the read gets there)
    rc, lines = run(exe, img, "cdinit\ncb 1\nreadn 5000\ntick 1\n")
    yield check("Setloc far past the end is accepted; the read ends in an error",
                "RN ret=1 res=22 00 00 00 00 00 00 00" in lines and "H intr=5 res0=03" in lines, f"{lines}")


def conversion_cases(root: Path, exe: Path):
    img = root / "c.bin"
    make_image(img, 4)
    values = [-150, -1, 0, 74, 75, 149, 4349, 4350, 4351, 4499 - 150 + 1, 100000, 449849]
    script = "".join(f"i2p {v}\n" for v in values)
    rc, lines = run(exe, img, script)
    ok = True
    detail = ""
    for v, l in zip(values, lines):
        m, s, f = pos_of(v)
        want = f"I {m:02x} {s:02x} {f:02x} 09 same=1"
        if l != want:
            ok = False
            detail += f" {v}: {l} != {want};"
    yield check("CdIntToPos at its borders (frame, second and minute carries, the 150 offset, 99:59:74)", ok and len(lines) == len(values), detail)
    # the inverse, at borders, and BCD digits that are not decimal-looking are taken digit by digit
    cases = [((0, 2, 0), 0), ((0, 2, 0x74), 74), ((0, 3, 0), 75), ((0, 0, 0), -150), ((0x99, 0x59, 0x74), 449849 if False else (99 * 60 + 59) * 75 + 74 - 150),
             ((1, 0, 0), 60 * 75 - 150), ((0x12, 0x34, 0x56), (12 * 60 + 34) * 75 + 56 - 150)]
    script = "".join(f"p2i {a:x} {b:x} {c:x}\n" for (a, b, c), _ in cases)
    rc, lines = run(exe, img, script)
    want = [f"J {w}" for _, w in cases]
    yield check("CdPosToInt at its borders", lines == want, f"{lines} {want}")
    # round trip over a range
    vals = list(range(-150, 200)) + list(range(4300, 4400)) + [449849]
    script = "".join(f"i2p {v}\n" for v in vals)
    rc, lines = run(exe, img, script)
    back = "".join(f"p2i {l.split()[1]} {l.split()[2]} {l.split()[3]}\n" for l in lines)
    rc, lines2 = run(exe, img, back)
    yield check("CdPosToInt undoes CdIntToPos", [int(x.split()[1]) for x in lines2] == vals, "round trip differs")


def page_cases(root: Path, exe: Path):
    img = root / "g.bin"
    make_image(img, 30)
    base = "cdinit\nreadn 12\nautotick 1\nready 0\nautotick 0\n"
    queries = lambda *a: "".join(f"src {x:x}\n" for x in a)

    def record(script):
        rc, lines = run(exe, img, base + script)
        return rc, [int(l.split()[1]) for l in lines if l.startswith("P ")]

    # a sector of 0x800 bytes written at 0x80101800: the words it covers hold its number, the rest of the page none
    rc, p = record("gs 80101800 512\n" + queries(0x80101000, 0x801017fc, 0x80101800, 0x80101ffc, 0x80101fff, 0x80102000))
    yield check("the word record: the words a copy wrote hold the sector, the words before and after it none, the page's other words too",
                p == [-1, -1, 12, 12, 12, -1], f"{p}")
    # the next sector crosses a page border and overwrites the end of the first
    rc, p = record("gs 80101800 512\ntick 1\ngs 80101f00 512\n" + queries(0x80101efc, 0x80101f00, 0x80101ffc, 0x80102000, 0x801026fc, 0x80102700))
    yield check("the word record: a copy over an older one takes its words, the older ones keep theirs, across a page border",
                p == [12, 13, 13, 13, 13, -1], f"{p}")
    # the borders of the RAM
    rc, p = record("gs 80000000 1\n" + queries(0x80000000, 0x80000003, 0x80000004, 0x7ffffffc, 0x7fffffff, 0x80200000, 0x00101000))
    yield check("the word record: the first word of the RAM, and addresses outside it", p == [12, 12, -1, -1, -1, -1, -1], f"{p}")
    rc, p = record("gs 801ffffc 1\n" + queries(0x801ffff8, 0x801ffffc, 0x801fffff, 0x80200000))
    yield check("the word record: the last word of the RAM", p == [-1, 12, 12, -1], f"{p}")
    # a copy that does not start or end on a word border: 16 bytes at 0x80100002 cover words 0x80100004, 8 and c whole
    rc, p = record("gsraw 80100002 4\n" + queries(0x80100000, 0x80100004, 0x80100008, 0x8010000c, 0x80100010))
    yield check("the word record: a copy that begins and ends in the middle of a word attributes only the words it wrote whole",
                p == [-1, 12, 12, 12, -1], f"{p}")
    # a whole word then a copy over half of it: the word holds bytes of two origins
    rc, p = record("gs 80100000 4\ntick 1\ngsraw 80100002 1\n" + queries(0x80100000, 0x80100004, 0x80100008))
    yield check("the word record: a word that is overwritten in part holds two origins and none is claimed (a 4-byte copy at +2 touches two words)", p == [-1, -1, 12], f"{p}")
    # the zeros of an empty FIFO are not the disc's: the sector is read out, then more words are asked for
    rc, p = record("gs 80100000 512\ngs 80100000 4\n" + queries(0x80100000, 0x8010000c, 0x80100010, 0x801007fc))
    yield check("the word record: the zeros that an empty data FIFO gives are from nowhere", p == [-1, -1, 12, 12], f"{p}")


def poll_cases(root: Path, exe: Path):
    img = root / "q.bin"
    make_image(img, 40)
    # no handler: the game polls CdReady; one sector waits until it is taken
    script = ("cdinit\nreadn 3\n"
              "ready 1\n"                      # nothing yet: no tick has run
              "tick 5\n"                       # one sector only (the FIFO holds one)
              "ready 1\n"                      # the sector 3 is announced, once
              "ready 1\n"
              "gs 80100000 512\n"
              "tick 5\nready 1\ngs 80100000 512\n"
              "ticks\nready 0\nticks\n")
    rc, lines = run(exe, img, script)
    y = [l for l in lines if l.startswith("Y ")]
    g = [l for l in lines if l.startswith("G ")]
    yield check("polling reader: CdReady(1) is 0 before a tick, 1 with a status result after, then 0",
                y[0].startswith("Y ret=0") and y[1] == "Y ret=1 res=22 00 00 00 00 00 00 00" and y[2].startswith("Y ret=0"), f"{y}")
    yield check("polling reader: one sector per take, in order", [x.split()[2] for x in g] == ["first=3", "first=4"], f"{g}")
    rc, lines = run(exe, img, "cdinit\nreadn 3\nticks\nready 0\nticks\nready 1\nticks\n")
    t = [int(l.split()[1]) for l in lines if l.startswith("T ")]
    yield check("a blocking CdReady(0) calls port_tick and CdReady(1) polls it too",
                len(t) == 3 and t[1] > t[0] and t[2] > t[1], f"{t}")
    # CdReady(0) with a read on: returns 1 (delivery inside the call)
    rc, lines = run(exe, img, "cdinit\nreadn 20\nready 0\ngs 80100000 512\n")
    yield check("blocking CdReady(0) returns 1 with the first sector and its status",
                "Y ret=1 res=22 00 00 00 00 00 00 00" in lines and lines[-1].startswith("G ret=1 first=20 "), f"{lines}")
    # CdReady(1) drains a stale flag (the game does this before a read): the flag is one value
    rc, lines = run(exe, img, "cdinit\nreadn 20\ntick 1\nready 1\nready 1\n")
    y = [l for l in lines if l.startswith("Y ")]
    yield check("a stale ready flag is drained by one CdReady(1)", y[0].startswith("Y ret=1") and y[1].startswith("Y ret=0"), f"{y}")
    # a new ReadN clears a stale flag
    rc, lines = run(exe, img, "cdinit\nreadn 20\ntick 1\nreadn 25\nready 1\n")
    y = [l for l in lines if l.startswith("Y ")]
    yield check("ReadN clears a stale ready flag", y[0].startswith("Y ret=0"), f"{y}")


def command_cases(root: Path, exe: Path):
    img = root / "k.bin"
    make_image(img, 100)
    p12 = pos_of(12)
    # implied Setloc: SeekL with a position, then ReadN without one starts there
    script = (f"cdinit\ncb 1\nhandler 80100000 512\nctl N 15 {hexs(*p12, 0)}\nsync 1\ntick 1\nsync 1\nsync 1\n"
              "ctl N 6\ntick 1\n")
    rc, lines = run(exe, img, script)
    s = [l for l in lines if l.startswith("S ")]
    yield check("SeekL takes the position (implied Setloc); its second response comes in the next tick",
                s[0].startswith("S ret=0") and s[1] == "S ret=2 res=02 00 00 00 00 00 00 00" and s[2].startswith("S ret=2"), f"{s}")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("ReadN after SeekL reads from the sought position", sec[:1] == [12], f"{sec} {lines}")
    # ReadS with a position starts there, and the command it leaves is ReadS
    script = (f"cdinit\ncb 1\nhandler 80100000 512\nctl N 1b {hexs(*pos_of(30), 0)}\nword 80181b54\ntick 1\n")
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("ReadS with a position reads from it", sec[:1] == [30] and "C ret=1 res=22 00 00 00 00 00 00 00" in lines, f"{lines}")
    # the library variables the game's C reads
    script = "cdinit\nctl N 1\nword 80181b54\nctl N e 80\nword 80181b54\nword 80181b44\nctl N 9\nword 80181b54\nword 80181b44\n"
    rc, lines = run(exe, img, script)
    w = [l for l in lines if l.startswith("W ")]
    yield check("the last-command and status variables are kept in RAM", w == ["W 00000001", "W 0000000e", "W 00000002", "W 00000009", "W 00000002"], f"{w}")
    # CdInit zeroes the three callback variables and returns 1
    rc, lines = run(exe, img, "cb 1\nword 80181b38\ncdinit\nword 80181b38\nword 80181b34\nword 80181b3c\n")
    yield check("CdInit returns 1 and clears the callback variables",
                lines == ["W 00001234", "R 1", "W 00000000", "W 00000000", "W 00000000"], f"{lines}")
    # parameters: Setmode and Setloc with none fail; Setloc with a bad BCD digit or frame fails
    rc, lines = run(exe, img, "cdinit\nctl N e\nctl N 2\nctl N 2 00 0a 00 00\nctl N 2 00 02 75\nctl N 2 00 01 00\nctl B 2 00 02 10\n")
    c = [l for l in lines if l.startswith("C ")]
    yield check("a command with no or an invalid parameter fails (returns 0)", [x.split()[1] for x in c[:5]] == ["ret=0"] * 5 and c[5].startswith("C ret=1"), f"{c}")
    # GetlocP: track, index, relative, absolute
    script = f"cdinit\nreadn 33\nctl F 11\nsync 1\n"
    rc, lines = run(exe, img, script)
    s = [l for l in lines if l.startswith("S ")]
    a = pos_of(33)
    rel = (bcd(33 // 75 // 60), bcd(33 // 75 % 60), bcd(33 % 75))
    yield check("GetlocP after CdControlF: the result carries the relative and absolute position",
                s and s[0] == "S ret=2 res=" + hexs(1, 1, *rel, *a), f"{s}")
    # CdMix and the pass-through ones
    rc, lines = run(exe, img, "cdinit\nmix\nctl N d 01 02\nctl N e c8\nctl B 1\n")
    yield check("CdMix returns 1; Setfilter and Setmode are accepted", lines[1] == "X 1" and all(l.startswith("C ret=1") for l in lines[2:]), f"{lines}")


def audio_cases(root: Path, exe: Path):
    img = root / "x.bin"
    # sectors 6 and 7 are XA audio (submode 0x64); 8 is data
    make_image(img, 20, audio={6: (1, 3, 0x64), 7: (1, 3, 0x64)})
    # double speed: two ticks consume 2 + 3 = 5 sectors, 4 to 8, audio ones included
    span = range(4, 9)
    script = "cdinit\nctl N e c8\ncb 1\nhandler 80100000 512\nreadn 4\ntick 1\ntick 1\n"
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("with XA on, audio sectors are consumed (they use the tick's budget) and never reach the handler",
                sec == [x for x in span if x not in (6, 7)], f"{sec}")
    script = "cdinit\nctl N e 80\ncb 1\nhandler 80100000 512\nreadn 4\ntick 1\ntick 1\n"
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("with XA off, the same sectors are data", sec == list(span), f"{sec}")


def unpolled_cases(root: Path, exe: Path):
    """A stream of audio that no one polls: the game's music. No ready handler, no CdReady; the game asks only GetlocP."""
    img = root / "u.bin"
    # A made-up audio file as the disc has them: channel 3's track in the sectors 8, 12 and 16 (the last with the end-of-file bit),
    # channel 4's shorter track in 9 and 13, and every other place from 8 on a sector with the submode 0 (no audio bit).
    pad = (1, 0, 0x00)
    audio = {n: pad for n in range(8, 40)}
    audio.update({8: (1, 3, 0x64), 12: (1, 3, 0x64), 16: (1, 3, 0xe4), 9: (1, 4, 0x64), 13: (1, 4, 0xe4)})
    make_image(img, 40, audio=audio)
    p8 = hexs(*pos_of(8), 0)
    # the game's sequence: Setfilter, Setmode, SeekL with the position, ReadS with the position; then only GetlocP and CdSync
    head = f"cdinit\nctl N d 01 03\nctl N e MODE\nctl N 15 {p8}\ntick 1\nsync 1\nctl N 1b {p8}\n"
    tail = "ctl F 11\nsync 1\nxac\nready 1\nready 1\ngs 80100000 512\n"
    # double speed: 4 vblanks bring 2 + 3 + 2 + 3 = 10 sectors, 8 to 17; the position is then 18
    rc, lines = run(exe, img, head.replace("MODE", "c8") + "tick 4\n" + tail)
    s = [l for l in lines if l.startswith("S ")]
    c = counts_of(lines)
    y = [l for l in lines if l.startswith("Y ")]
    g = [l for l in lines if l.startswith("G ")]
    rel = (bcd(18 // 75 // 60), bcd(18 // 75 % 60), bcd(18 % 75))
    yield check("an audio stream that no one polls: the drive goes on over the sectors that are no audio (GetlocP after 4 vblanks is 10 sectors on)",
                rc == 0 and len(s) == 2 and s[1] == "S ret=2 res=" + hexs(1, 1, *rel, *pos_of(18)), f"{rc} {s}")
    yield check("the channel's three audio sectors and the other channel's two are consumed on the way",
                rc == 0 and c and c[0]["silent"] == 3 and c[0]["skipped"] == 2, f"{rc} {c}")
    yield check("each such sector is a data sector: the ready flag is set (taken once), and the FIFO holds the last one, which replaced the ones before",
                rc == 0 and len(y) == 2 and y[0] == "Y ret=1 res=22 00 00 00 00 00 00 00" and y[1].startswith("Y ret=0") and g and g[0].split()[2] == "first=17", f"{rc} {y} {g}")
    # 12 more vblanks bring 30 more sectors: the read runs off the end of the image of 40, so the position passed every sector
    rc, lines = run(exe, img, head.replace("MODE", "c8") + "tick 16\nctl F 11\nsync 1\n")
    s = [l for l in lines if l.startswith("S ")]
    yield check("it goes on to the end of the file's sectors (the position after 16 vblanks is the image's end)",
                rc == 0 and len(s) == 2 and s[1].endswith(hexs(*pos_of(40))), f"{rc} {s}")
    # with a sink the same: the channel's sectors are played, the position is the same
    rc, lines = run(exe, img, "sink 1\n" + head.replace("MODE", "c8") + "tick 4\nctl F 11\nsync 1\nxac\n")
    s = [l for l in lines if l.startswith("S ")]
    c = counts_of(lines)
    yield check("with a sink the same position, the channel's three sectors played and none held",
                rc == 0 and len(s) == 2 and s[1].endswith(hexs(*pos_of(18))) and c and c[0]["played"] == 3 and c[0]["skipped"] == 2 and c[0]["held"] == 0, f"{rc} {s} {c}")
    # a poll during the read shows a reader that polls: from then on one sector waits, with the XA bit set too.
    # The poll comes before any vblank; then 2 sectors (8, 9: audio), then 10 is a data sector and waits: the position is 11.
    rc, lines = run(exe, img, head.replace("MODE", "c8") + "ready 1\ntick 4\n" + tail)
    s = [l for l in lines if l.startswith("S ")]
    y = [l for l in lines if l.startswith("Y ")]
    g = [l for l in lines if l.startswith("G ")]
    yield check("a CdReady during the read makes it a polled one: one sector waits for the poll again (the position after 4 vblanks is 3 sectors on, the FIFO holds the first data sector)",
                rc == 0 and len(s) == 2 and s[1].endswith(hexs(*pos_of(11))) and len(y) == 3 and y[0].startswith("Y ret=0") and y[1].startswith("Y ret=1") and g and g[0].split()[2] == "first=10", f"{rc} {s} {y} {g}")
    # and a new read begins the question again: ReadS once more, no poll, and the drive goes on
    rc, lines = run(exe, img, head.replace("MODE", "c8") + f"ready 1\ntick 4\nctl N 1b {p8}\ntick 4\nctl F 11\nsync 1\n")
    s = [l for l in lines if l.startswith("S ")]
    yield check("a new read begins the question again: after ReadS with no poll the drive goes on (10 sectors in 4 vblanks)",
                rc == 0 and len(s) == 2 and s[1].endswith(hexs(*pos_of(18))), f"{rc} {s}")
    # the control: the same sequence with the XA bit clear is a polling reader from its start, and one sector waits for the poll
    rc, lines = run(exe, img, head.replace("MODE", "80") + "tick 4\n" + tail)
    s = [l for l in lines if l.startswith("S ")]
    g = [l for l in lines if l.startswith("G ")]
    yield check("control: with the XA bit clear the polling rule holds (after 4 vblanks the position is 1 sector on, and the FIFO holds that sector)",
                rc == 0 and len(s) == 2 and s[1].endswith(hexs(*pos_of(9))) and g and g[0].split()[2] == "first=8", f"{rc} {s} {g}")


# ---- the streamed audio with a sink ------------------------------------------------------

def make_xa_image(path: Path, count: int, xa: dict[int, bytes]) -> None:
    """count sectors of the usual pattern, with the sectors in `xa` (raw XA sectors of test_hostxa.make_sector) in place."""
    make_image(path, count)
    raw = bytearray(path.read_bytes())
    for n, sec in xa.items():
        raw[n * SECTOR + 16:(n + 1) * SECTOR] = sec[16:]
    path.write_bytes(bytes(raw))


def xa_sector(seed: int, file: int = 1, channel: int = 3, coding: int = 1, copy=None, submode: int = 0x64) -> bytes:
    import random
    rnd = random.Random(seed)
    return X.make_sector(file=file, channel=channel, submode=submode, coding=coding, headers=lambda g, b: rnd.randrange(256) & 0x3f | 0, data=lambda g, j, k: rnd.randrange(256), copy=copy)


def expected_frames(sectors: list[bytes], rate: int = 37800) -> list[tuple[int, int]]:
    d = X.RefDecoder()
    frames: list[tuple[int, int]] = []
    for raw in sectors:
        code, fr = d.sector(raw)
        assert code == X.OK
        frames += fr
    return X.ref_resample(frames, rate)


def parse_pull(lines: list[str]) -> list[list[tuple[int, int]]]:
    out = []
    for l in lines:
        if l.startswith("PL "):
            v = [int(x) for x in l.split()[1:]]
            out.append(list(zip(v[1::2], v[2::2])) if v[0] else [])
    return out


def counts_of(lines: list[str]) -> list[dict[str, int]]:
    out = []
    for l in lines:
        if l.startswith("XC "):
            w = l.split()
            out.append({w[i]: int(w[i + 1]) for i in range(1, len(w), 2)})
    return out


def volumes(frames: list[tuple[int, int]], v: tuple[int, int, int, int]) -> list[tuple[int, int]]:
    def cl(x):
        return max(-32768, min(32767, x))
    if v[0] in (127, 128) and v[2] in (127, 128) and v[1] == 0 and v[3] == 0:
        return list(frames)
    return [(cl((l * v[0] + r * v[3]) >> 7), cl((l * v[1] + r * v[2]) >> 7)) for l, r in frames]


def stream_cases(root: Path, exe: Path):
    sa, sb, sc = xa_sector(1), xa_sector(2), xa_sector(3)
    img = root / "xs.bin"
    make_xa_image(img, 24, {6: sa, 7: sb})
    start = "cdinit\nsink 1\ncb 1\nhandler 80100000 512\n"
    mode_filter = "ctl N d 01 03\nctl N e c8\n"
    want = expected_frames([sa, sb])
    # the filter on and the file and channel equal: both sectors are played, the handler sees only data sectors (double speed: 2 + 3)
    rc, lines = run(exe, img, start + mode_filter + "readn 4\ntick 2\nxac\npull 48000\nxac\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    c = counts_of(lines)
    pl = parse_pull(lines)
    yield check("filter on, file and channel equal: two audio sectors played, the handler sees the data sectors only",
                rc == 0 and sec == [4, 5, 8] and c and c[0]["played"] == 2 and c[0]["skipped"] == 0 and c[0]["held"] == 0, f"{rc} {sec} {c}")
    yield check("a played sector reaches the queue: the frames counted are the reference's (decode, then resample), every sample equal",
                rc == 0 and c[0]["frames"] == len(want) and c[0]["queued"] == len(want) and pl and pl[0] == want, f"{c} {len(pl[0]) if pl else None} vs {len(want)}")
    yield check("after the pull the queue is empty and the pulled count is the frames played", c[1]["queued"] == 0 and c[1]["pulled"] == len(want), f"{c}")
    # the file differs, then the channel differs: skipped, consumed in silence, nothing queued
    for tag, flt in (("file", "ctl N d 02 03"), ("channel", "ctl N d 01 04")):
        rc, lines = run(exe, img, start + flt + "\nctl N e c8\nreadn 4\ntick 2\nxac\n")
        sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
        c = counts_of(lines)
        yield check(f"filter on, {tag} differs: the two audio sectors are skipped (consumed, nothing queued)",
                    rc == 0 and sec == [4, 5, 8] and c[0]["skipped"] == 2 and c[0]["played"] == 0 and c[0]["queued"] == 0, f"{rc} {sec} {c}")
    # a Setfilter after the read started takes effect for the next sector
    rc, lines = run(exe, img, start + "ctl N d 05 05\nctl N e c8\nreadn 5\ntick 1\nctl N d 01 03\ntick 1\nxac\n")
    c = counts_of(lines)
    yield check("the Setfilter given in the middle of a read is used at once (sector 6 skipped, sector 7 played)",
                rc == 0 and c[0]["skipped"] == 1 and c[0]["played"] == 1, f"{rc} {c}")
    # the filter off: every audio sector is played whatever its channel; setfilter bytes unused
    rc, lines = run(exe, img, start + "ctl N d 09 09\nctl N e c0\nreadn 4\ntick 2\nxac\n")
    c = counts_of(lines)
    yield check("filter bit off: every audio sector is played, whatever the Setfilter says", rc == 0 and c[0]["played"] == 2 and c[0]["skipped"] == 0, f"{rc} {c}")
    # the XA bit off: the sectors are data
    rc, lines = run(exe, img, start + "ctl N e 80\nreadn 4\ntick 2\nxac\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    c = counts_of(lines)
    yield check("XA bit off: the same sectors go to the handler as data and nothing is queued", rc == 0 and len(sec) == 5 and sec[:2] == [4, 5] and sec[4] == 8 and c[0]["played"] == 0 and c[0]["queued"] == 0, f"{rc} {sec} {c}")
    # no sink: consumed in silence, as before
    rc, lines = run(exe, img, "cdinit\ncb 1\nhandler 80100000 512\n" + mode_filter + "readn 4\ntick 2\nxac\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    c = counts_of(lines)
    yield check("no sink: the audio sectors are consumed in silence (counted apart), nothing is queued, the data sectors are delivered as before",
                rc == 0 and sec == [4, 5, 8] and c[0]["silent"] == 2 and c[0]["played"] == 0 and c[0]["queued"] == 0, f"{rc} {sec} {c}")
    # no stream: a pull gives nothing
    rc, lines = run(exe, img, start + "pull 100\n")
    yield check("no stream: a pull returns 0 frames", rc == 0 and "PL 0" in lines, f"{rc} {lines}")

    # mono and 18,900 Hz sectors play too
    imgm = root / "xm.bin"
    m1, m2 = xa_sector(11, coding=0), xa_sector(12, coding=4)
    make_xa_image(imgm, 12, {6: m1})
    rc, lines = run(exe, imgm, start + mode_filter + "readn 6\ntick 1\nxac\npull 48000\n")
    pl = parse_pull(lines)
    yield check("a mono sector at 37,800 Hz is played, both sides equal", rc == 0 and pl and pl[0] == expected_frames([m1]) and all(a == b for a, b in pl[0]), f"{rc} {len(pl[0]) if pl else None}")
    make_xa_image(imgm, 12, {6: m2})
    rc, lines = run(exe, imgm, start + mode_filter + "readn 6\ntick 1\nxac\npull 48000\n")
    pl = parse_pull(lines)
    yield check("a mono sector at 18,900 Hz is played (resampled from 18,900 Hz)", rc == 0 and pl and pl[0] == expected_frames([m2], 18900), f"{rc} {len(pl[0]) if pl else None}")

    # refused: the 8-bit sector and a sector whose two subheader copies differ; consumed and counted, the position moves on
    imgr = root / "xr.bin"
    r8 = xa_sector(21, coding=0x11)
    rbad = xa_sector(22, copy=[1, 3, 0x64, 2])
    make_xa_image(imgr, 12, {6: sa, 7: r8, 8: rbad, 9: sb})
    rc, lines, err = run_err(exe, imgr, start + mode_filter + "readn 6\ntick 2\ntick 1\nxac\npull 48000\n")
    c = counts_of(lines)
    pl = parse_pull(lines)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("an 8-bit sector and a sector with differing subheader copies are refused and counted; the others are played in order",
                rc == 0 and c[0]["refused"] == 2 and c[0]["played"] == 2 and sec == [10, 11] and pl and pl[0] == expected_frames([sa, sb]), f"{rc} {c} {sec}")
    yield check("each refusal is a trace line with the reason", len([e for e in err if "refused:" in e]) == 2 and any("8 bit samples are not decoded" in e for e in err) and any("two copies of the subheader differ" in e for e in err), f"{err}")
    yield check("the stream's start is one trace line and one line on the output",
                len([e for e in err if "stream starts" in e]) == 1 and [l for l in lines if l.startswith("audio:")] == ["audio: a stream starts at sector 6: file 1 channel 3, stereo, 37800 Hz"], f"{err} {lines[:3]}")

    # the volumes of CdMix: unity, 127 (passthrough as PsyZ), half, crossed and clamped
    v_cases = [("unity 128, 0, 128, 0", (128, 0, 128, 0)), ("127, 0, 127, 0 goes through unchanged", (127, 0, 127, 0)), ("half 64, 0, 64, 0", (64, 0, 64, 0)),
               ("mixed 100, 30, 90, 20", (100, 30, 90, 20)), ("swapped sides 0, 128, 0, 128", (0, 128, 0, 128)), ("twice 255, 0, 255, 0 (clamped)", (255, 0, 255, 0)),
               ("126, 0, 126, 0 is a real scaling", (126, 0, 126, 0))]
    base = expected_frames([sa, sb])
    for label, v in v_cases:
        rc, lines = run(exe, img, start + mode_filter + f"mixv {v[0]} {v[1]} {v[2]} {v[3]}\nreadn 4\ntick 2\npull 48000\n")
        pl = parse_pull(lines)
        yield check(f"CdMix {label}: every pulled frame by the sum", rc == 0 and pl and pl[0] == volumes(base, v), f"{rc} {len(pl[0]) if pl else None}")
    yield check("the 255 case reaches the clamp (so that the check means something)", any(abs(x) == 32767 or x == -32768 for fr in volumes(base, (255, 0, 255, 0)) for x in fr), "no sample reached a limit")
    # the volumes are kept until the next CdMix and a read's start does not reset them
    rc, lines = run(exe, img, start + mode_filter + "mixv 64 0 64 0\nreadn 4\ntick 2\nreadn 4\ntick 2\npull 48000\n")
    pl = parse_pull(lines)
    yield check("the volumes stay after a new read", rc == 0 and pl and pl[0] == volumes(base, (64, 0, 64, 0)), f"{rc}")

    # the lock: every access of the game's thread to the queue is between a lock and an unlock; the pull takes none (PsyZ holds it)
    rc, lines = run(exe, img, start + mode_filter + "readn 4\ntick 2\nlocks\npull 100\nlocks\nxac\nlocks\n")
    lk = [l.split() for l in lines if l.startswith("LK ")]
    yield check("the lock calls are balanced (depth 0, none unlocked without a lock), and a pull takes no lock",
                rc == 0 and len(lk) == 3 and lk[0][2] == lk[1][2] and lk[0][4] == "0" and lk[1][4] == "0" and lk[2][4] == "0" and lk[2][6] == "0" and int(lk[0][2]) > 0 and int(lk[2][2]) > int(lk[1][2]), f"{rc} {lk}")

    # the lock is taken for every access to the queue: a tick that plays two sectors takes it six times (to look at whether the sink is gone, at the
    # room and to add, for each), and nothing else takes it
    rc, lines = run(exe, img, start + mode_filter + "readn 4\nlocks\ntick 1\nlocks\ntick 1\nlocks\n")
    lk = [int(l.split()[2]) for l in lines if l.startswith("LK ")]
    yield check("each played sector takes the lock three times (sink gone, room, add); the sectors of data take none", rc == 0 and len(lk) == 3 and lk[1] - lk[0] == 0 and lk[2] - lk[1] == 6, f"{rc} {lk}")

    # the reset at a read's start: the queue is emptied and the decoder begins again (history and resampler)
    imgq = root / "xq.bin"
    make_xa_image(imgq, 24, {6: sa, 7: sb, 14: sc})
    rc, lines = run(exe, imgq, start + mode_filter + "readn 6\ntick 2\nxac\nreadn 14\nxac\ntick 1\nxac\npull 48000\n")
    c = counts_of(lines)
    pl = parse_pull(lines)
    yield check("a read's start empties the queue", rc == 0 and c[0]["queued"] == len(expected_frames([sa, sb])) and c[1]["queued"] == 0, f"{c}")
    yield check("after the new read the first sector decodes as the first of a stream (history and phase fresh): every sample equal",
                rc == 0 and pl and pl[0] == expected_frames([sc]), f"{len(pl[0]) if pl else None}")

    # the hold: a run of audio sectors fills the queue, the sector that does not fit stays on the disc; a pull releases it
    N = 40
    audio = {n: xa_sector(100 + n) for n in range(6, 6 + N)}
    imgh = root / "xh.bin"
    make_xa_image(imgh, 6 + N + 6, audio)
    secs = [audio[n] for n in range(6, 6 + N)]
    d = X.RefDecoder()
    frames: list[tuple[int, int]] = []
    cum = []
    for raw in secs:
        frames += d.sector(raw)[1]
        cum.append(len(X.ref_resample(frames, 37800)))
    bound = 2016 * 44100 // 37800 + 2
    played = 1
    while played < N and 44100 - cum[played - 1] >= bound:
        played += 1
    held_at = 6 + played   # the sector that stays on the disc
    rc, lines = run(exe, imgh, start + mode_filter + "readn 6\ntick 40\nxac\nlocks\nctl B 11\ntick 5\nxac\nlocks\n")
    c = counts_of(lines)
    loc = [l for l in lines if l.startswith("C ") and "res=" in l]
    pos = None
    if loc:
        b = [int(x, 16) for x in loc[-1].split("res=")[1].split()]
        bc = lambda x: (x >> 4) * 10 + (x & 15)
        pos = (bc(b[2]) * 60 + bc(b[3])) * 75 + bc(b[4])
    yield check(f"the queue is full: {played} sectors are played (by the reference's frame counts), the next is held and not consumed (the drive waits)",
                rc == 0 and c[0]["played"] == played and c[0]["held"] >= 1 and c[0]["queued"] == cum[played - 1], f"{rc} {c} wanted played {played}")
    lk = [l.split() for l in lines if l.startswith("LK ")]
    yield check("a held sector leaves the lock released (depth 0, nothing unlocked without a lock), after 40 and after 45 ticks", rc == 0 and len(lk) == 2 and all(x[4] == "0" and x[6] == "0" for x in lk), f"{rc} {lk}")
    yield check("the read position stays at the held sector however many ticks pass", rc == 0 and pos == held_at and c[1]["played"] == played and c[1]["held"] > c[0]["held"], f"{pos} vs {held_at}, {c}")
    # the release: a pull empties the queue; the next tick takes the sector, and no burst (at most 3 sectors in one tick)
    rc, lines = run(exe, imgh, start + mode_filter + "readn 6\ntick 40\nxac\ndrain\ntick 1\nxac\ndrain\ntick 1\nxac\n")
    c = counts_of(lines)
    yield check("after the queue is drained the held sector is taken at the next tick, and the wait is not paid back as a burst (at most 3 sectors)",
                rc == 0 and 1 <= c[1]["played"] - c[0]["played"] <= 3 and 1 <= c[2]["played"] - c[1]["played"] <= 3, f"{rc} {c}")
    # the hold does not reorder: data sectors after the audio run reach the handler only after all audio sectors are consumed
    imgo = root / "xo.bin"
    audio2 = {n: xa_sector(200 + n) for n in range(6, 30)}
    make_xa_image(imgo, 40, audio2)
    rc, lines = run(exe, imgo, start + mode_filter + "readn 4\ntick 30\nxac\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    c = counts_of(lines)
    yield check("the data sectors after a held run are not delivered while the audio sector is held", rc == 0 and sec == [4, 5] and c[0]["held"] >= 1, f"{rc} {sec} {c}")
    rc, lines = run(exe, imgo, start + mode_filter + "readn 4\ntick 30\ndrain\ntick 30\ndrain\ntick 30\ndrain\ntick 30\ndrain\nxac\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("drained in time, the whole run is consumed and the data sectors follow in order", rc == 0 and sec == [4, 5] + list(range(30, 40)), f"{rc} {sec}")


    # the sink is switched off in the middle of a hold (the dump stopped): the queue is emptied, the held sector and the audio behind it are consumed in
    # silence, and the data sectors behind them reach the handler. By hand: the image is audio 6 to 29 and data 30 to 39; 18 sectors are played, sector
    # 24 is held; after "sink 0" the 6 sectors 24 to 29 are silent, and the handler sees sectors 4, 5 and 30 to 39
    rc, lines = run(exe, imgo, start + mode_filter + "readn 4\ntick 30\nxac\nsink 0\nxac\ntick 30\nxac\nlocks\n")
    c = counts_of(lines)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    lk = [l.split() for l in lines if l.startswith("LK ")]
    yield check("the sink switched off while a sector is held: the queue is emptied, nothing is held after it, the data sectors behind it are delivered",
                rc == 0 and c[0]["played"] == 18 and c[0]["held"] >= 1 and c[0]["queued"] > 40000 and c[1]["queued"] == 0 and c[2]["held"] == c[0]["held"]
                and c[2]["silent"] == 6 and c[2]["played"] == 18 and sec == [4, 5] + list(range(30, 40)) and lk[0][4] == "0" and lk[0][6] == "0", f"{rc} {c} {sec} {lk}")

    # a consumer that is gone. The image is audio 6 to 80, then data 81 to 89. No pull: the queue fills (18 played), sector 24 is held at every
    # tick from the first; the 121st attempt is the cut-off. By hand: held 120, played 18, then the held sector and the audio behind it are silent
    imgg = root / "xg.bin"
    audio3 = {n: xa_sector(300 + n) for n in range(6, 81)}
    make_xa_image(imgg, 90, audio3)
    line_gone = "audio: nothing has taken sound for 120 vblanks; the disc's audio is consumed in silence until something does"
    line_back = "audio: something takes sound again; the disc's audio is played from this sector, with the decoder begun again"
    rc, lines = run(exe, imgg, start + mode_filter + "readn 4\ntick 100\nxac\n")
    c0 = counts_of(lines)
    yield check("held with no pull for 100 vblanks: not cut off yet (no line, nothing silent), 18 sectors played and the hold counted",
                rc == 0 and line_gone not in lines and c0[0]["played"] == 18 and c0[0]["silent"] == 0 and 80 < c0[0]["held"] < 100, f"{rc} {c0}")
    rc, lines = run(exe, imgg, start + mode_filter + "readn 4\ntick 200\nxac\n")
    c = [None] + counts_of(lines)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("held with no pull: cut off at the 121st attempt (held 120, played 18), one line, the rest of the audio and the data consumed or delivered",
                rc == 0 and c[1]["held"] == 120 and c[1]["played"] == 18 and c[1]["silent"] == 75 - 18 and lines.count(line_gone) == 1 and sec == [4, 5] + list(range(81, 90)) and c[1]["queued"] == 0, f"{rc} {c} {sec} {lines.count(line_gone)}")
    # the same with pulls going on: one frame each tick keeps the queue full and moves the pull count, so no cut-off after 250 vblanks
    rc, lines = run(exe, imgg, start + mode_filter + "readn 4\n" + "tick 1\npull 1\n" * 250 + "xac\n")
    c = counts_of(lines)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("held with pulls going on: no cut-off after 250 vblanks (nothing silent, no line, no data sector delivered, the hold still counted)",
                rc == 0 and line_gone not in lines and c[0]["silent"] == 0 and sec == [4, 5] and c[0]["held"] > 200 and 18 <= c[0]["played"] <= 19, f"{rc} {c} {sec}")
    # a pull after the cut-off: ticks until the first silent sector, then one pull, then ticks; the audio is played again from the next audio sector
    rc, lines = run(exe, imgg, start + mode_filter + "readn 4\n" + "tick 1\nxac\n" * 130)
    cs = counts_of(lines)
    k = next(i for i, x in enumerate(cs) if x["silent"] > 0) + 1
    silent_at = cs[k - 1]["silent"]
    rc, lines = run(exe, imgg, start + mode_filter + f"readn 4\ntick {k}\nxac\npull 10\nxac\ntick 1\nxac\npull 48000\n")
    c = counts_of(lines)
    pl = parse_pull(lines)
    nxt = 24 + silent_at   # the first audio sector after the silent ones (the audio run is unbroken)
    played_new = c[2]["played"] - c[1]["played"]
    yield check("the cut-off comes at the 121st attempt of the hold (hold 120 before it)", k >= 120 and cs[k - 1]["held"] == 120 and cs[k - 2]["silent"] == 0, f"{k} {cs[k - 2:k]}")
    yield check("after the cut-off one pull puts the sink on again: one line, the audio played from the next audio sector with the decoder begun again, every frame equal",
                rc == 0 and lines.count(line_back) == 1 and 2 <= played_new <= 3 and pl and pl[-1] == expected_frames([audio3[nxt + i] for i in range(played_new)]) and c[1]["queued"] == 0,
                f"{rc} {c} next sector {nxt}, new {played_new}, line {lines.count(line_back)}")

    # a held sector and the sink off: with no sink nothing is ever held
    rc, lines = run(exe, imgh, "cdinit\ncb 1\nhandler 80100000 512\n" + mode_filter + "readn 6\ntick 40\nxac\n")
    c = counts_of(lines)
    yield check("without a sink nothing is held, however many sectors", rc == 0 and c[0]["held"] == 0 and c[0]["silent"] >= 40 and c[0]["played"] == 0, f"{c}")


def initvol_cases(root: Path, exe: Path):
    """CdInit does what the library's CD_initvol does: the chip's main volume if it holds 0 on both sides, the CD volume, the control,
    and the drive's mix. Every expected line is written out by hand from the routine at 0x8015e10c of the resident program."""
    sa, sb = xa_sector(1), xa_sector(2)
    img = root / "iv.bin"
    make_xa_image(img, 24, {6: sa, 7: sb})
    WRITES = " w1b0=3fff w1b2=3fff w1aa=c001"   # the CD volume, left and right, then the control: always, in this order
    MAIN = " w180=3fff w182=3fff"               # the main volume, left and right: only when it holds 0 on both sides

    def go(script):
        rc, lines = run(exe, img, script)
        return rc, [l for l in lines if l.startswith(("R ", "CL", "CR ", "CS "))]

    rc, got = go("chiplog\ncdinit\nchiplog\nchipregs\nchipsum\n")
    yield check("a chip whose registers all hold 0: both main volume registers are read, then the main volume, the CD volume and the control are written",
                rc == 0 and got == ["CL", "R 1", "CL r180 r182" + MAIN + WRITES, "CR main 3fff 3fff cd 3fff 3fff control c001 current 0000 0000", "CS 5 registers not 0, sum 114685"], f"{rc} {got}")
    # 0x3fff * 4 + 0xc001 = 65532 + 49153 = 114685: no other register was written
    rc, got = go("chipset 180 1234\ncdinit\nchiplog\nchipregs\n")
    yield check("the main volume's left side holds a value: the right side is not read, the main volume is left alone, the rest is written",
                rc == 0 and got == ["R 1", "CL r180" + WRITES, "CR main 1234 0000 cd 3fff 3fff control c001 current 0000 0000"], f"{rc} {got}")
    rc, got = go("chipset 182 1\ncdinit\nchiplog\nchipregs\n")
    yield check("only the right side holds a value: both are read and the main volume is left alone",
                rc == 0 and got == ["R 1", "CL r180 r182" + WRITES, "CR main 0000 0001 cd 3fff 3fff control c001 current 0000 0000"], f"{rc} {got}")
    rc, got = go("chipset 180 8000\ncdinit\nchiplog\nchipregs\n")
    yield check("a main volume in a sweep mode with no level bits (0x8000) counts as not 0 and is left alone",
                rc == 0 and got == ["R 1", "CL r180" + WRITES, "CR main 8000 0000 cd 3fff 3fff control c001 current 0000 0000"], f"{rc} {got}")
    rc, got = go("chipset 1b8 7ffe\nchipset 1ba 7ffe\ncdinit\nchiplog\nchipregs\n")
    yield check("the two registers of the current main volume are not what is asked: with a value there and 0 in the main volume, the main volume is written",
                rc == 0 and got == ["R 1", "CL r180 r182" + MAIN + WRITES, "CR main 3fff 3fff cd 3fff 3fff control c001 current 7ffe 7ffe"], f"{rc} {got}")
    rc, got = go("chipset 1b0 111\nchipset 1b2 222\nchipset 1aa ffff\nchipset 180 2000\nchipset 182 2000\ncdinit\nchiplog\nchipregs\n")
    yield check("the CD volume and the control are written whatever they held",
                rc == 0 and got == ["R 1", "CL r180" + WRITES, "CR main 2000 2000 cd 3fff 3fff control c001 current 0000 0000"], f"{rc} {got}")
    rc, got = go("cdinit\nchiplog\nchipset 180 0\nchipset 182 0\ncdinit\nchiplog\ncdinit\nchiplog\n")
    yield check("a second CdInit asks again: after the main volume went back to 0 it is written again, and with 0x3fff there it is not",
                rc == 0 and got == ["R 1", "CL r180 r182" + MAIN + WRITES, "R 1", "CL r180 r182" + MAIN + WRITES, "R 1", "CL r180" + WRITES], f"{rc} {got}")
    # no model of the chip (a build without PsyZ): one read is tried, nothing is written, CdInit returns 1 all the same
    rc, got = go("chipset 1b0 111\nchip 0\ncdinit\nchiplog\nchipregs\n")
    yield check("with no model of the chip CdInit tries one read, writes nothing and returns 1",
                rc == 0 and got == ["R 1", "CL r180-", "CR main 0000 0000 cd 0111 0000 control 0000 current 0000 0000"], f"{rc} {got}")

    # the drive's mix: CdInit sets it to 0x80, 0, 0x80, 0, so the frames of a stream played after it are the reference's own
    start = "sink 1\ncb 1\nhandler 80100000 512\nctl N d 01 03\nctl N e c8\n"
    play = "readn 4\ntick 2\npull 48000\n"
    base = expected_frames([sa, sb])
    for label, pre in (("with the chip's model", ""), ("with no model of the chip", "chip 0\n")):
        rc, lines = run(exe, img, pre + "cdinit\n" + start + "mixv 10 20 30 40\ncdinit\n" + start + play)
        pl = parse_pull(lines)
        yield check(f"CdInit after a CdMix of 10, 20, 30, 40 sets the mix back to 0x80, 0, 0x80, 0 ({label}): every pulled frame is the reference's",
                    rc == 0 and pl and pl[0] == base and base != volumes(base, (10, 20, 30, 40)), f"{rc} {len(pl[0]) if pl else None} of {len(base)}")
    rc, lines = run(exe, img, "cdinit\n" + start + "mixv 10 20 30 40\n" + play)
    pl = parse_pull(lines)
    yield check("the same stream without the second CdInit keeps the mix of 10, 20, 30, 40 (so that the cases above mean something)",
                rc == 0 and pl and pl[0] == volumes(base, (10, 20, 30, 40)), f"{rc} {len(pl[0]) if pl else None}")
    # the lock: the mix that CdInit sets is written under the audio lock, like CdMix's (one lock and one unlock, none left open)
    rc, lines = run(exe, img, "locks\ncdinit\nlocks\n")
    lk = [l for l in lines if l.startswith("LK")]
    yield check("CdInit takes the audio lock once for the mix and leaves it closed", rc == 0 and lk == ["LK calls 0 depth 0 bad 0", "LK calls 1 depth 0 bad 0"], f"{rc} {lk}")


def stop_cases(root: Path, exe: Path):
    img = root / "s.bin"
    make_image(img, 10)
    for com, label in ((0x10, "GetlocL"), (0x03, "Play"), (0x07, "MotorOn"), (0x1a, "GetID")):
        rc, lines = run(exe, img, f"cdinit\nctl N {com:x}\nmix\n")
        yield check(f"an unsent command ({label}) stops with its name and status 6",
                    rc == 6 and lines[-1] == f"stop: disc command 0x{com:x} is not handled" and "X 1" not in lines, f"{rc} {lines}")
    rc, lines = run(exe, img, "cdinit\nctl F 10\n")
    yield check("the stop is the same through CdControlF", rc == 6 and lines[-1] == "stop: disc command 0x10 is not handled", f"{rc} {lines}")
    # the whole-sector bit (0x20) is accepted now (see whole_cases); the 2,328-byte form (0x10) is not used by anything and stops
    for mode in (0x10, 0x30, 0xf8):
        rc, lines = run(exe, img, f"cdinit\nctl N e {mode:x}\nmix\n")
        yield check(f"Setmode 0x{mode:x} (the 2,328-byte form) stops with its line and status 6",
                    rc == 6 and lines[-1] == f"stop: disc mode (2,328-byte form) 0x{mode:x} is not handled" and "X 1" not in lines, f"{rc} {lines}")


def buffer_cases(root: Path, exe: Path):
    img = root / "b.bin"
    make_image(img, 20)
    RAMEND = 0x80200000
    fill = "cdinit\ncb 1\nhandler 80100000 0\nreadn 4\ntick 1\n"   # a sector is under the FIFO; the handler takes no piece
    line = lambda a, w: f"stop: CdGetSector buffer 0x{(a & 0xffffffff):08x} ({w * 4} bytes) is outside the PS1's RAM"
    for tag, a, w in (("starts-below-the-ram", 0x7ffffffc, 4), ("starts-at-the-end-of-the-ram", RAMEND, 1),
                      ("runs-over-the-end-of-the-ram", RAMEND - 8, 3), ("is-the-whole-ram-and-one-word-more", 0x80000000, 0x80001)):
        rc, lines = run(exe, img, fill + f"gsraw {a:x} {w}\ngsraw 80100000 1\n")
        got = lines[-1]
        # the host address printed is the test's own pointer (its RAM plus the offset): compare only the shape
        yield check(f"CdGetSector: a buffer that {tag} ends the program with status 6 and copies nothing",
                    rc == 6 and got.startswith("stop: CdGetSector buffer 0x") and got.endswith(f"({w * 4} bytes) is outside the PS1's RAM") and not any(l.startswith("G ") for l in lines[-1:]), f"{rc} {lines[-3:]}")
    rc, lines = run(exe, img, fill + "gsraw 80100000 1073741824\n")
    yield check("CdGetSector: a count of 2^30 words (4 GB, which wraps to 0 bytes in 32 bits) ends the program with status 6",
                rc == 6 and lines[-1].endswith("(4294967296 bytes) is outside the PS1's RAM"), f"{rc} {lines[-2:]}")
    rc, lines = run(exe, img, fill + "gsraw 80100000 -5\ngsraw 80100000 0\n")
    yield check("CdGetSector: a count of 0 or below copies nothing and returns 1", rc == 0 and lines[-2:] == ["G ret=1", "G ret=1"], f"{rc} {lines[-3:]}")
    rc, lines = run(exe, img, fill + "gsraw 801ffff0 4\n")
    yield check("CdGetSector: a buffer that ends exactly at the end of the RAM is accepted", rc == 0 and lines[-1] == "G ret=1", f"{rc} {lines[-2:]}")
    rc, lines = run(exe, img, "cdinit\ngsraw 80100000 4\n")
    yield check("CdGetSector with no sector under the FIFO gives zeros and returns 1", rc == 0 and lines[-1] == "G ret=1", f"{rc} {lines[-2:]}")
    # the page record: the last page of the RAM
    rc, lines = run(exe, img, fill + "gsraw 801ffff0 4\nsrc 801ff000\nsrc 801fe000\nsrc 80200000\nsrc 7fffffff\nsrc 801ffff0\nsrc 801ffffc\n")
    yield check("the word record: the last 16 bytes of the RAM hold the sector (the one tick gave sector 4, at single speed), the page's other words and the page before none",
                rc == 0 and [l for l in lines if l.startswith("P ")] == ["P -1", "P -1", "P -1", "P -1", "P 4", "P 4"], f"{rc} {lines[-7:]}")


def rate_cases(root: Path, exe: Path):
    img = root / "rate.bin"
    make_image(img, 400)
    head = "cdinit\ncb 1\nhandler 80100000 0\n"      # the handler takes no piece: only its calls are counted
    dbl, sgl = "ctl N e 80\n", "ctl N e 00\n"

    def counts(script):
        rc, lines = run(exe, img, head + script)
        return rc, [int(l.split()[1]) for l in lines if l.startswith("V ")]

    # worked by hand: the carry grows by 5 a tick. Double speed, a sector costs 2:
    # 5 -> 2 sectors, carry 1; 6 -> 3, carry 0; and again. Single speed, a sector costs 4:
    # 5 -> 1, carry 1; 6 -> 1, carry 2; 7 -> 1, carry 3; 8 -> 2, carry 0; and again.
    rc, v = counts(dbl + "readn 0\ntickc 10\n")
    yield check("double speed: 2, 3, 2, 3 ... sectors a tick over ten ticks", rc == 0 and v == [2, 3, 2, 3, 2, 3, 2, 3, 2, 3], f"{v}")
    rc, v = counts(sgl + "readn 0\ntickc 10\n")
    yield check("single speed: 1, 1, 1, 2 ... sectors a tick over ten ticks", rc == 0 and v == [1, 1, 1, 2, 1, 1, 1, 2, 1, 1], f"{v}")
    # 60 ticks are a second: 30 * 5 = 150 at double speed, 15 * 5 = 75 at single speed
    rc, v = counts(dbl + "readn 0\ntickc 60\n")
    yield check("double speed: 150 sectors in 60 ticks", rc == 0 and len(v) == 60 and sum(v) == 150, f"{len(v)} {sum(v)}")
    rc, v = counts(sgl + "readn 0\ntickc 60\n")
    yield check("single speed: 75 sectors in 60 ticks", rc == 0 and len(v) == 60 and sum(v) == 75, f"{len(v)} {sum(v)}")
    # the sectors come in order, in a row, from the read's position
    rc, lines = run(exe, img, "cdinit\ncb 1\nhandler 80100000 512\n" + dbl + "readn 100\ntickc 2\n")
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("the sectors of a tick come in order, one after the other, from the read's position", rc == 0 and sec == [100, 101, 102, 103, 104], f"{sec}")
    # a new read starts from a zero carry. Double speed, one tick (2 sectors, carry 1), stop,
    # new read: 2 then 3. With the carry kept it would be 3 (6 / 2).
    rc, v = counts(dbl + "readn 0\ntickc 1\nctl B 9\nreadn 50\ntickc 2\n")
    yield check("double speed: a new read after a stopped one starts from a zero carry (2, 3)", rc == 0 and v == [2, 2, 3], f"{v}")
    # single speed, two ticks (1, 1, carry 2), a new read: 1, 1, 1, 2. With the carry kept: 1, 2.
    rc, v = counts(sgl + "readn 0\ntickc 2\nreadn 50\ntickc 4\n")
    yield check("single speed: a new read in the middle of a read starts from a zero carry (1, 1, 1, 2)", rc == 0 and v == [1, 1, 1, 1, 1, 2], f"{v}")
    # ReadN after Pause is a start too: double speed, 2 (carry 1), Pause, ReadN (no position): 2, 3
    rc, v = counts(dbl + "readn 0\ntickc 1\nctl B 9\nctl N 6\ntickc 2\n")
    yield check("ReadN after Pause starts from a zero carry", rc == 0 and v == [2, 2, 3], f"{v}")
    # the speed bit changes in the middle of a read: the carry is cleared, the cost is the new one.
    # double: 2 (carry 1); to single: 1, 1, 1, 2 from zero (carry kept: 6 -> 1, 7 -> 1, 8 -> 2).
    rc, v = counts(dbl + "readn 0\ntickc 1\n" + sgl + "tickc 4\n")
    yield check("the speed bit cleared (double to single) in a read: the carry is cleared and a sector costs 4", rc == 0 and v == [2, 1, 1, 1, 2], f"{v}")
    # single: 1, 1, 1 (carry 3); to double: 2, 3 from zero (carry kept: 8 -> 4)
    rc, v = counts(sgl + "readn 0\ntickc 3\n" + dbl + "tickc 2\n")
    yield check("the speed bit set (single to double) in a read: the carry is cleared and a sector costs 2", rc == 0 and v == [1, 1, 1, 2, 3], f"{v}")
    # a Setmode that leaves the speed bit as it is keeps the carry: double 2 (carry 1), mode 0xc8 (still double), 3 (6 / 2)
    rc, v = counts(dbl + "readn 0\ntickc 1\nctl N e c8\ntickc 1\n")
    yield check("a Setmode that does not change the speed bit keeps the carry", rc == 0 and v == [2, 3], f"{v}")
    # the handler pauses the read during a tick's delivery (the end of a stream): double speed, the tick
    # owes 2 sectors and delivers 1; the rest is not delivered, then nothing; and the next read does not get it
    # as extra (the carry of 1 left by the pause, if kept, would give 3 sectors, not 2; the whole 3, 4). The tick: 5, one sector (3 left),
    # the next fails, 3 % 2 = 1; the read start clears it.
    rc, v = counts(dbl + "stopafter 1\nreadn 0\ntickc 3\nstopafter 0\ncb 1\nreadn 50\ntickc 2\n")
    yield check("a read paused by the handler in a tick's delivery: the rest of the tick's sectors are not delivered, and the next read starts from zero",
                rc == 0 and v == [1, 0, 0, 2, 3], f"{v}")
    # a sector that could not be delivered: the row ends and the carry is cut to its remainder (carry modulo the cost).
    # A polling reader (no handler) holds the first sector in the FIFO, so every later sector fails; then a handler comes.
    # Double speed (cost 2): tick 1: 5, one sector, 3, the next fails, 3 % 2 = 1; tick 2: 6, fails, 0; tick 3: 5, fails, 1;
    # tick 4: 6, fails, 0. After 3 blocked ticks the carry is 1: the next ticks give 3 (6), then 2, 3 (carry 0 -> 5, 6).
    # After 4 blocked ticks the carry is 0: 2 (5, carry 1), 3 (6), 2.
    rc, v = counts(dbl + "cb 0\nreadn 0\ntick 3\ncb 1\ntickc 3\n")
    yield check("double speed: after 3 blocked ticks the first free one gives 3, then 2, 3 (no burst)", rc == 0 and v == [3, 2, 3], f"{v}")
    rc, v = counts(dbl + "cb 0\nreadn 0\ntick 4\ncb 1\ntickc 3\n")
    yield check("double speed: after 4 blocked ticks the first free one gives 2, then 3, 2", rc == 0 and v == [2, 3, 2], f"{v}")
    # Single speed (cost 4): tick 1: 5, one sector, 1 (no failure yet); tick 2: 6, fails, 6 % 4 = 2; tick 3: 7, fails, 3;
    # tick 4: 8, fails, 0. After 3 blocked ticks the carry is 3: 8 gives 2, then 1, 1, 1 (carry 0 -> 5, 6, 7).
    # After 4 blocked ticks the carry is 0: 1 (5), 1 (6), 1 (7), 2 (8).
    rc, v = counts(sgl + "cb 0\nreadn 0\ntick 3\ncb 1\ntickc 4\n")
    yield check("single speed: after 3 blocked ticks the first free one gives 2, then 1, 1, 1 (no burst)", rc == 0 and v == [2, 1, 1, 1], f"{v}")
    rc, v = counts(sgl + "cb 0\nreadn 0\ntick 4\ncb 1\ntickc 4\n")
    yield check("single speed: after 4 blocked ticks the first free one gives 1, 1, 1, 2", rc == 0 and v == [1, 1, 1, 2], f"{v}")
    # the bound, stated directly: after 50 blocked ticks no tick brings more than 3 (double) or 2 (single), and the bound is reached
    rc, v = counts(dbl + "cb 0\nreadn 0\ntick 50\ncb 1\ntickc 12\n")
    yield check("double speed: after 50 blocked ticks no tick brings more than 3 sectors", rc == 0 and len(v) == 12 and max(v) == 3, f"{v}")
    rc, v = counts(sgl + "cb 0\nreadn 0\ntick 50\ncb 1\ntickc 12\n")
    yield check("single speed: after 50 blocked ticks no tick brings more than 2 sectors", rc == 0 and len(v) == 12 and max(v) == 2, f"{v}")
    # the ready handler acts on the drive from inside the row (a made-up image of 32 sectors; the handler takes whole
    # sectors, so the sector numbers show). The cost is taken before the handler runs; a read start or a speed change
    # made by the handler clears the carry and ends the row, so it brings no burst. By hand, counts of five ticks:
    img32 = root / "act.bin"
    make_image(img32, 32)

    def acted(script):
        rc, lines = run(exe, img32, "cdinit\ncb 1\nhandler 80100000 512\n" + script)
        return rc, [int(l.split()[1]) for l in lines if l.startswith("V ")], [int(l.split()[1]) for l in lines if l.startswith("  sector")]

    # double speed, the first callback issues ReadN to 20: tick 1: 5, cost 2, sector 0, the reset ends the row (1);
    # then from zero: 5 -> 2 (20, 21), 6 -> 3 (22, 23, 24), 5 -> 2 (25, 26), 6 -> 3 (27, 28, 29)
    for kind, label in (("N", "ReadN"), ("S", "ReadS")):
        rc, v, sec = acted(dbl + f"readn 0\nact 1 {kind} 20\ntickc 5\n")
        yield check(f"the first ready callback issues {label} (double speed): 1, 2, 3, 2, 3 sectors, the new read's from its position",
                    rc == 0 and v == [1, 2, 3, 2, 3] and sec == [0] + list(range(20, 30)), f"{rc} {v} {sec}")
    # double to single: tick 1: sector 0, the reset ends the row (1); from zero at cost 4: 1 (1), 1 (2), 1 (3), 2 (4, 5)
    rc, v, sec = acted(dbl + "readn 0\nact 1 M 0\ntickc 5\n")
    yield check("the first ready callback sets single speed (from double): 1, 1, 1, 1, 2 sectors", rc == 0 and v == [1, 1, 1, 1, 2] and sec == [0, 1, 2, 3, 4, 5], f"{rc} {v} {sec}")
    # single to double: tick 1: 5, cost 4, sector 0, carry 1, the reset ends the row (1); from zero at cost 2: 2, 3, 2, 3
    rc, v, sec = acted(sgl + "readn 0\nact 1 M 128\ntickc 5\n")
    yield check("the first ready callback sets double speed (from single): 1, 2, 3, 2, 3 sectors", rc == 0 and v == [1, 2, 3, 2, 3] and sec == list(range(0, 11)), f"{rc} {v} {sec}")
    # a read started at every sector: each tick brings exactly one (the handler cannot make the drive run ahead)
    rc, v, sec = acted(dbl + "readn 0\nact 0 N 20\ntickc 5\n")
    yield check("a ReadN at every sector: exactly 1 sector a tick (double speed)", rc == 0 and v == [1, 1, 1, 1, 1] and sec == [0, 20, 20, 20, 20], f"{rc} {v} {sec}")
    rc, v, sec = acted(sgl + "readn 0\nact 0 N 20\ntickc 5\n")
    yield check("a ReadN at every sector: exactly 1 sector a tick (single speed)", rc == 0 and v == [1, 1, 1, 1, 1] and sec == [0, 20, 20, 20, 20], f"{rc} {v} {sec}")
    # what the handler does that leaves the row alone: a Setmode with the same speed bit, CdReady(0) taking the flag
    rc, v, sec = acted(dbl + "readn 0\nact 1 M 200\ntickc 5\n")
    yield check("a Setmode that keeps the speed bit, from the handler: 2, 3, 2, 3, 2 sectors", rc == 0 and v == [2, 3, 2, 3, 2] and sec == list(range(0, 12)), f"{rc} {v} {sec}")
    rc, v, sec = acted(dbl + "readn 0\nact 0 R 0\ntickc 5\n")
    yield check("CdReady(0) from the handler (no re-entry of the tick, the carry untouched): 2, 3, 2, 3, 2 sectors", rc == 0 and v == [2, 3, 2, 3, 2] and sec == list(range(0, 12)), f"{rc} {v} {sec}")
    # CdInit from the handler stops the read: the row ends after the sector it was called for, nothing after
    rc, v, sec = acted(dbl + "readn 0\nact 1 I 0\ntickc 5\n")
    yield check("CdInit from the handler stops the read: 1, 0, 0, 0, 0 sectors", rc == 0 and v == [1, 0, 0, 0, 0] and sec == [0], f"{rc} {v} {sec}")
    # (every list above stays within the bound: at most 3 sectors a tick)
    # nothing is delivered, and no carry kept, when no read is on
    rc, v = counts(dbl + "tickc 4\nreadn 0\ntickc 2\n")
    yield check("ticks with no read on deliver nothing and leave no carry for the read that follows (0, 0, 0, 0, then 2, 3)", rc == 0 and v == [0, 0, 0, 0, 2, 3], f"{v}")


# The fill of the game's ring, worked by hand for every pair of places: row w - 1, column r - 1. The writer's place w (byte 0x16)
# is the place the next sector goes to, the reader's place r (byte 0x17) the place the main loop reads next; the fill is how many
# written sectors have not been read, w - r counted round the ring of seven (1 to 7). Written out, not computed.
FILL = (
    (0, 6, 5, 4, 3, 2, 1),
    (1, 0, 6, 5, 4, 3, 2),
    (2, 1, 0, 6, 5, 4, 3),
    (3, 2, 1, 0, 6, 5, 4),
    (4, 3, 2, 1, 0, 6, 5),
    (5, 4, 3, 2, 1, 0, 6),
    (6, 5, 4, 3, 2, 1, 0),
)
FORCED_LINE = "cd: the game's ring looked full for 30 vblanks; delivering"


def guard_cases(root: Path, exe: Path):
    img = root / "guard.bin"
    make_image(img, 400)
    head = "cdinit\ncb 2\nhandler 80100000 0\n"      # the game's stream handler; it takes no piece, only its calls are counted
    dbl, sgl = "ctl N e 80\n", "ctl N e 00\n"

    def go(script, cb_head=head):
        rc, lines, err = run_err(exe, img, cb_head + script)
        return rc, lines, err, [int(l.split()[1]) for l in lines if l.startswith("V ")], [l for l in lines if l == FORCED_LINE]

    # all 49 pairs, double speed (a tick gives 2 sectors from a zero carry): delivered when the fill is 3 or less, held from 4
    script = "".join(f"ring {w} {r}\nreadn 0\ntickc 1\n" for w in range(1, 8) for r in range(1, 8))
    rc, lines, err, v, forced = go(dbl + script)
    want = [2 if FILL[w - 1][r - 1] <= 3 else 0 for w in range(1, 8) for r in range(1, 8)]
    yield check("the fill for all 49 pairs of places (table written out): 2 sectors with a fill of 0 to 3, none from 4 to 6", rc == 0 and v == want and not forced, f"{rc} {v} {want}")
    bad = [(w, r) for w in range(1, 8) for r in range(1, 8) if v[(w - 1) * 7 + r - 1] != want[(w - 1) * 7 + r - 1]]
    yield check("(each pair of the table that differs, if any)", not bad, f"{bad}")
    # a byte that is 0 or above 7: no guard, whatever the other holds
    for w, r in ((0, 1), (5, 0), (0, 0), (8, 2), (5, 8), (255, 1), (5, 255), (1, 200)):
        rc, lines, err, v, forced = go(dbl + f"ring {w} {r}\nreadn 0\ntickc 1\n")
        yield check(f"places {w} and {r} (outside 1 to 7): no guard, 2 sectors", rc == 0 and v == [2], f"{rc} {v}")
    # held at fill 4, 5 and 6, double speed: carry 5 % 2 = 1, then 6 % 2 = 0, then 1; the reader moves (fill 3): 1 + 5 = 6 -> 3 sectors.
    # (A carry that was not cut would give 15 / 2 = 7 sectors.)
    for w, fill in ((5, 4), (6, 5), (7, 6)):
        rc, lines, err, v, forced = go(dbl + f"ring {w} 1\nreadn 0\ntickc 3\nring 4 1\ntickc 2\n")
        yield check(f"double speed, fill {fill}: nothing for 3 vblanks, then fill 3: 3 sectors (the carry cut to 1), then 2", rc == 0 and v == [0, 0, 0, 3, 2], f"{rc} {v}")
    # single speed (a sector costs 4): held 5 % 4 = 1, 6 % 4 = 2, 7 % 4 = 3; resumed 3 + 5 = 8 -> 2 sectors. (Not cut: 20 / 4 = 5.)
    for w, fill in ((5, 4), (6, 5), (7, 6)):
        rc, lines, err, v, forced = go(sgl + f"ring {w} 1\nreadn 0\ntickc 3\nring 4 1\ntickc 2\n")
        yield check(f"single speed, fill {fill}: nothing for 3 vblanks, then fill 3: 2 sectors, then 1", rc == 0 and v == [0, 0, 0, 2, 1], f"{rc} {v}")
    # within a row: the handler steps the writer's place. Fill 3 (w 4, r 1): the first sector comes, w is 5, the fill 4, the second waits.
    # Carry: 5 - 2 = 3, cut to 1; next vblank held: 6 % 2 = 0; with fill 3 (w 5, r 2): 5 -> a sector, w 6, fill 4 -> held.
    rc, lines, err, v, forced = go(dbl + "adv 1\nring 4 1\nreadn 0\ntickc 2\nring 5 2\ntickc 1\n")
    yield check("within a row the fill reaches 4 after the first sector: the second is not delivered (1, 0, 1)", rc == 0 and v == [1, 0, 1], f"{rc} {v}")
    # the writer's place wraps from 7 to 1 as the game's does: w 7, r 5 is fill 2; after one sector w is 1, fill 3; after two w is 2, fill 4
    rc, lines, err, v, forced = go(dbl + "adv 1\nring 7 5\nreadn 0\ntickc 1\n")
    yield check("the writer's place wraps from 7 to 1: w 7, r 5 (fill 2) gives 2 sectors (fills 2, then 3 after the first)", rc == 0 and v == [2], f"{rc} {v}")
    rc, lines, err, v, forced = go(dbl + "adv 1\nring 7 4\nreadn 0\ntickc 1\n")
    yield check("w 7, r 4 (fill 3): the first sector makes w 1, the fill 4: the second waits (1 sector)", rc == 0 and v == [1], f"{rc} {v}")
    # the trace's last line (standard error): the vblank that delivered a sector before the row ended was not a held vblank
    rc, lines, err, v, forced = go(dbl + "adv 1\nring 4 1\nreadn 0\ntickc 2\n")
    yield check("the trace's last line: the row that delivered one and then waited is not counted as held; the next one is (held 1, largest fill 4)",
                rc == 0 and any(l.startswith("T cd: the ring guard looked") and "held 1 vblanks, forced 0 rows, largest fill 4" in l for l in err), f"{err}")
    # 30 held vblanks, then the next row is delivered regardless, with its line; the count starts again
    rc, lines, err, v, forced = go(dbl + "ring 5 1\nreadn 0\ntickc 30\ntickc 1\n")
    yield check("30 held vblanks, then the 31st delivers its row regardless (2 sectors: the carry after 30 cuts is 0) with one line",
                rc == 0 and v == [0] * 30 + [2] and forced == [FORCED_LINE], f"{rc} {v} {forced}")
    rc, lines, err, v, forced = go(dbl + "ring 5 1\nreadn 0\ntickc 31\ntickc 30\ntickc 1\n")
    # after the forced row (2 sectors, carry 1): 30 more held (carry 0, 1, ..., ends at 1), the next forced gives (1 + 5) / 2 = 3
    yield check("the count of held vblanks starts again after a forced row: 30 more held, then forced again (3 sectors), 2 lines",
                rc == 0 and v == [0] * 30 + [2] + [0] * 30 + [3] and len(forced) == 2, f"{rc} {v} {len(forced)}")
    runs = [l for l in err if l.startswith("T cd: the ring guard held ")]
    yield check("the trace gets a line when a run of held vblanks ends: two runs of 30, with the frame (31 and 62) and the largest fill 4",
                len(runs) == 2 and "held 30 vblanks in a row" in runs[0] and "frame 31" in runs[0] and "frame 62" in runs[1] and "largest fill so far 4" in runs[1], f"{runs}")
    yield check("the trace's last line: held 60, forced 2, largest fill 4",
                any(l.startswith("T cd: the ring guard looked") and "held 60 vblanks, forced 2 rows, largest fill 4" in l for l in err), f"{err}")
    # a vblank that is not held ends the run: 29 held, one free (fill 3), 29 held: no forced row
    rc, lines, err, v, forced = go(dbl + "ring 5 1\nreadn 0\ntickc 29\nring 4 1\ntickc 1\nring 5 1\ntickc 29\n")
    yield check("29 held, one vblank free, 29 held: the run began again, no forced row", rc == 0 and len(v) == 59 and sum(v[:29]) == 0 and v[29] > 0 and sum(v[30:]) == 0 and not forced, f"{rc} {v} {forced}")
    rc, lines, err, v, forced = go(dbl + "ring 5 1\nreadn 0\ntickc 29\nring 4 1\ntickc 1\nring 5 1\ntickc 30\ntickc 1\n")
    yield check("... and the 30th held after it still waits, the 31st is forced", rc == 0 and v[30:60] == [0] * 30 and v[60] > 0 and len(forced) == 1, f"{rc} {v[30:]} {forced}")
    # a ring that looks full when no read is on holds nothing and counts nothing
    rc, lines, err, v, forced = go(dbl + "ring 5 1\ntickc 40\nreadn 0\ntickc 1\n")
    yield check("40 vblanks with no read on and a full-looking ring count nothing: the read's first vblank is held, not forced",
                rc == 0 and v == [0] * 40 + [0] and not forced and not any(l.startswith("T cd: the ring guard") and "forced 1" in l for l in err), f"{rc} {v[-3:]} {forced}")
    # not a streamed load of the game: no guard with a ring that looks full
    rc, lines, err, v, forced = go(dbl + "ring 5 1\nreadn 0\ntickc 3\n", head.replace("cb 2", "cb 3"))
    yield check("another handler: no guard, 2, 3, 2 sectors with a ring that looks full", rc == 0 and v == [2, 3, 2], f"{rc} {v}")
    rc, lines, err, v, forced = go(dbl + "ring 5 1\nreadn 0\ntickc 3\n", head.replace("cb 2", "cb 1"))
    yield check("the other made-up handler: no guard, 2, 3, 2 sectors", rc == 0 and v == [2, 3, 2], f"{rc} {v}")
    rc, lines, err, v, forced = go("ring 5 1\n" + dbl + "readn 0\ntickc 1\nready 1\n", "cdinit\ncb 0\n")
    ys = [l for l in lines if l.startswith("Y ")]
    yield check("no handler (the game polls): no guard, the first sector waits for the poll and the poll gets it", rc == 0 and len(ys) == 1 and ys[0].startswith("Y ret=1 "), f"{rc} {ys}")
    rc, lines, err, v, forced = go("ctl N e c0\nring 5 1\nreadn 0\ntickc 3\n")
    yield check("the XA bit of the mode set (0xc0): no guard, 2, 3, 2 sectors with a ring that looks full", rc == 0 and v == [2, 3, 2], f"{rc} {v}")
    # the blocking wait of the game: CdReady(0) with a ring that looks full gets the sector (single speed: 5 -> 1 sector)
    rc, lines, err, v, forced = go(sgl + "ring 5 1\nreadn 0\nready 0\n")
    ys = [l for l in lines if l.startswith("Y ")]
    yield check("CdReady(0) with a ring that looks full: the sector comes (ret 1, the handler called once)", rc == 0 and len(ys) == 1 and ys[0].startswith("Y ret=1 ") and lines.count("H intr=1 res0=22") == 1, f"{rc} {ys} {lines}")
    # ... and it counts as neither held nor free: 3 held, the blocking wait, 27 held (30 held in all), the next is forced
    rc, lines, err, v, forced = go(sgl + "ring 5 1\nreadn 0\ntickc 3\nready 0\ntickc 27\ntickc 1\n")
    yield check("the blocking wait in the middle of a run neither ends nor adds to it: forced at the 31st vblank, with one line",
                rc == 0 and v[:30] == [0] * 30 and v[30] > 0 and len(forced) == 1, f"{rc} {v} {forced}")


def make_pattern_image(path: Path, count: int, audio: tuple[int, ...] = ()) -> bytes:
    """count raw sectors in which every byte of the 2352 is a pattern of the sector and the place (the sync, the header, the subheader
    and the error codes too), so that a piece taken from a wrong place differs. The sectors of `audio` have the audio bit in their
    submode (byte 18); the others do not. Returns the image's bytes."""
    out = bytearray()
    for n in range(count):
        raw = bytearray(((n * 31 + i * 7 + (i >> 8) * 3 + 1) & 0xFF) for i in range(SECTOR))
        raw[18] = raw[22] = 0x64 if n in audio else 0x08
        out += raw
    path.write_bytes(bytes(out))
    return bytes(out)


def dump_of(lines: list[str], k: int = 0) -> bytes:
    """The bytes of the k-th `dump` line of a run."""
    return bytes(int(x, 16) for x in [l for l in lines if l.startswith("M ")][k].split()[1:])


def whole_cases(root: Path, exe: Path):
    """The whole-sector mode (Setmode bit 0x20): the FIFO of a data sector holds the raw bytes 12 to 2351 (2,340)."""
    img = root / "w.bin"
    data = make_pattern_image(img, 40, audio=(14,))
    raw = lambda n: data[n * SECTOR:(n + 1) * SECTOR]
    whole = lambda n: raw(n)[12:]
    plain = lambda n: raw(n)[24:24 + 2048]
    take = "autotick 1\nready 0\nautotick 0\n"   # the polling reader's blocking CdReady(0): the first sector of the read
    A = 0x80100000
    for mode in (0x20, 0xa0, 0xe8):
        rc, lines = run(exe, img, f"cdinit\nctl N e {mode:x}\nmix\n")
        yield check(f"Setmode 0x{mode:x} (whole sectors) is accepted", rc == 0 and "C ret=1 res=02 00 00 00 00 00 00 00" in lines and "X 1" in lines, f"{rc} {lines}")
    rc, lines = run(exe, img, "cdinit\nctl N e 20\nctl N 6\nctl B 9\n")
    yield check("ReadN with the whole-sector mode on is accepted (it stopped the program before)", rc == 0 and lines.count("C ret=1 res=22 00 00 00 00 00 00 00") == 1, f"{rc} {lines}")
    # one sector as one piece of 585 words = 2340 bytes: the raw bytes 12 to 2351, in order
    rc, lines = run(exe, img, f"cdinit\nctl N e 20\nreadn 5\n{take}gs {A:x} 585\ndump {A:x} 2340\ndump {A + 2340:x} 8\n")
    yield check("whole sectors: one sector taken as one piece of 2,340 bytes is the raw bytes 12 to 2351",
                rc == 0 and dump_of(lines, 0) == whole(5) and dump_of(lines, 1) == bytes(8), f"{rc} {dump_of(lines, 0)[:24].hex()} {whole(5)[:24].hex()}")
    yield check("whole sectors: the first four bytes are the sector's header and the next eight its subheader",
                dump_of(lines, 0)[:12] == raw(5)[12:24] and dump_of(lines, 0)[12:16] == raw(5)[24:28], "the start is not the raw byte 12")
    # pieces of 12, 32 and 2,016 bytes and a rest of 280, taken one after the other; the places do not overlap
    pieces = [(0, 3), (12, 8), (44, 504), (2060, 70)]
    script = f"cdinit\nctl N e 20\nreadn 5\n{take}" + "".join(f"gs {A + off:x} {w}\n" for off, w in pieces) + f"dump {A:x} 2340\ndump {A + 2340:x} 8\n"
    rc, lines = run(exe, img, script)
    g = [l for l in lines if l.startswith("G ")]
    yield check("whole sectors: pieces of 12, 32 and 2,016 bytes and the rest of 280 bytes give the raw bytes in order",
                rc == 0 and len(g) == 4 and all(x.startswith("G ret=1 ") for x in g) and dump_of(lines, 0) == whole(5) and dump_of(lines, 1) == bytes(8), f"{rc} {g}")
    # a rest that asks for more than is left: the 280 bytes and then zeros
    script = f"cdinit\nctl N e 20\nreadn 5\n{take}gs {A:x} 3\ngs {A + 12:x} 8\ngs {A + 44:x} 504\ngs {A + 2060:x} 100\ndump {A + 2060:x} 400\n"
    rc, lines = run(exe, img, script)
    yield check("whole sectors: a rest asked for beyond the sector's end gives its 280 bytes and then zeros",
                rc == 0 and dump_of(lines, 0) == whole(5)[2060:] + bytes(120), f"{rc} {dump_of(lines, 0)[:8].hex()}")
    # a piece past the end gives zeros, over what was there before
    script = f"cdinit\nctl N e 20\nreadn 5\n{take}gs {A:x} 585\ndump {A:x} 16\ngs {A:x} 4\ndump {A:x} 16\n"
    rc, lines = run(exe, img, script)
    yield check("whole sectors: a piece past the end gives zeros and returns 1, over the bytes that were there",
                rc == 0 and dump_of(lines, 0) == whole(5)[:16] and dump_of(lines, 1) == bytes(16) and "G ret=1 first=0 sum=0 last=00" in lines, f"{rc} {lines[-4:]}")
    # the bit is looked at when a sector is delivered: set after ReadN and before the first tick
    rc, lines = run(exe, img, f"cdinit\nreadn 5\nctl N e 20\n{take}gs {A:x} 585\ndump {A:x} 2340\n")
    yield check("whole sectors: the bit is looked at when the sector is delivered, so a mode set after ReadN and before the first sector counts",
                rc == 0 and dump_of(lines) == whole(5), f"{rc}")
    # and cleared between two sectors of one read, then set again
    script = (f"cdinit\nctl N e 20\nreadn 5\n{take}gs {A:x} 585\ndump {A:x} 2340\n"                  # sector 5 whole
              f"ctl N e 00\ntick 1\ngs {A + 0x1000:x} 512\ndump {A + 0x1000:x} 2048\ngs {A + 0x1000:x} 4\ndump {A + 0x1000:x} 16\n"   # sector 6, mode cleared
              f"ctl N e 20\nready 1\ntick 1\ngs {A + 0x2000:x} 585\ndump {A + 0x2000:x} 2340\n")      # sector 7, mode set again
    rc, lines = run(exe, img, script)
    yield check("whole sectors: the mode cleared between two sectors of one read changes what the next gives (2,048 bytes from byte 24, then zeros), and set again changes it back",
                rc == 0 and dump_of(lines, 0) == whole(5) and dump_of(lines, 1) == plain(6) and dump_of(lines, 2) == bytes(16) and dump_of(lines, 3) == whole(7),
                f"{rc} {[dump_of(lines, k)[:6].hex() for k in range(4)]}")
    # without the bit: a sector is the 2,048 bytes from byte 24, in both speeds, as before
    for mode, label in ((0x00, "single speed"), (0x80, "double speed")):
        rc, lines = run(exe, img, f"cdinit\nctl N e {mode:x}\nreadn 9\n{take}gs {A:x} 512\ndump {A:x} 2048\ngs {A:x} 4\ndump {A:x} 16\n")
        yield check(f"without the whole-sector bit ({label}) a sector is the 2,048 bytes from byte 24, and then zeros",
                    rc == 0 and dump_of(lines, 0) == plain(9) and dump_of(lines, 0) == raw(9)[24:2072] and dump_of(lines, 1) == bytes(16), f"{rc}")
    # the mode bits other than 0x20 do not change the form of the data
    rc, lines = run(exe, img, f"cdinit\nctl N e c8\nreadn 9\n{take}gs {A:x} 512\ndump {A:x} 2048\n")
    yield check("without the whole-sector bit but with the others (0xc8) a sector is the 2,048 bytes from byte 24", rc == 0 and dump_of(lines) == plain(9), f"{rc}")
    # the disc's compressed audio goes to the audio in either mode and never to the data FIFO or the handler
    for mode, form, words, payload in ((0x60, "whole-sector", 585, whole), (0x40, "2,048-byte", 512, plain)):
        rc, lines = run(exe, img, f"cdinit\ncb 1\nhandler {A:x} {words}\nctl N e {mode:x}\nreadn 13\ntick 3\nxac\n")
        got = [(int(l.split()[1]), int(l.split()[3])) for l in lines if l.startswith("  sector")]
        want = [(payload(n)[0] | payload(n)[1] << 8, sum(payload(n))) for n in (13, 15)]
        xc = [l for l in lines if l.startswith("XC ")]
        yield check(f"{form} mode with the audio on: the audio sector 14 never reaches the handler (13 and 15 do, with their bytes) and is consumed as silence",
                    rc == 0 and got == want and xc and "silent 1" in xc[0] and lines.count("H intr=1 res0=22") == 2, f"{rc} {got} {want} {xc}")
    # the record of where the bytes came from, and the hook, work as for the 2,048-byte form, for what CdGetSector copies
    script = (f"cdinit\nctl N e 20\nreadn 12\n{take}hook 1\ngs {A:x} 100\ngs {A + 400:x} 485\n"
              f"src {A:x}\nsrc {A + 396:x}\nsrc {A + 400:x}\nsrc {A + 2336:x}\nsrc {A + 2340:x}\n")
    rc, lines = run(exe, img, script)
    p = [int(l.split()[1]) for l in lines if l.startswith("P ")]
    k = [l for l in lines if l.startswith("K ")]
    yield check("whole sectors: the word record says sector 12 for the words a piece wrote (the last word of the 2,340 bytes too), none past it; the hook is told each copy",
                rc == 0 and p == [12, 12, 12, 12, -1] and k == [f"K {0x80100000:x} 400", f"K {0x80100190:x} 1940"], f"{rc} {p} {k}")
    script = f"cdinit\nctl N e 20\nreadn 12\n{take}gs {A:x} 585\ngs {A:x} 4\nsrc {A:x}\nsrc {A + 12:x}\nsrc {A + 16:x}\n"
    rc, lines = run(exe, img, script)
    p = [int(l.split()[1]) for l in lines if l.startswith("P ")]
    yield check("whole sectors: the zeros of a dry FIFO are from nowhere (-1) and replace the record of the bytes they overwrote", rc == 0 and p == [-1, -1, 12], f"{rc} {p}")


def host_cases(root: Path, exe: Path):
    """The host ready handler (port_cd_host_ready): called in place of the game's handler variable."""
    img = root / "h.bin"
    make_image(img, 60, audio={13: (1, 0, 0x64)})
    A = 0x80100000
    dbl = "ctl N e 80\n"
    sectors = lambda lines: [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    # called once per data sector, in order, with interrupt code 1 and the status byte the game's handler gets
    host = "cdinit\nhost 1\ncb 3\nhandler 80100000 512\n" + dbl + "readn 10\ntick 1\ntick 1\n"
    game = "cdinit\ncb 1\nhandler 80100000 512\n" + dbl + "readn 10\ntick 1\ntick 1\n"
    rc, lines = run(exe, img, host + "calls\n")
    rc2, lines2 = run(exe, img, game + "calls\n")
    hh = [l for l in lines if l.startswith("HH ")]
    yield check("the host handler is called once per data sector, in order (double speed: 2 then 3 sectors), with interrupt code 1 and the status 0x22",
                rc == 0 and hh == ["HH intr=1 res0=22 depth=1"] * 5 and sectors(lines) == [10, 11, 12, 13, 14], f"{rc} {hh} {sectors(lines)}")
    yield check("... the same calls, status and bytes as the game's handler gets",
                [l for l in lines if l.startswith("H ") or l.startswith("  ")] == [l for l in lines2 if l.startswith("H ") or l.startswith("  ")] and sectors(lines) == sectors(lines2),
                f"{[l for l in lines if l.startswith('H ')][:2]} {[l for l in lines2 if l.startswith('H ')][:2]}")
    yield check("the game's handler variable, set to a made-up handler at the same time, is not called", "CA game 0 host 5" in lines, f"{lines[-1]}")
    rc, lines = run(exe, img, host.replace("cb 3", "cb 1") + "calls\n")
    yield check("... whatever the variable holds (the other made-up handler too)", "CA game 0 host 5" in lines, f"{lines[-1]}")
    rc, lines = run(exe, img, host.replace("cb 3", "cb 2") + "calls\n")
    yield check("... or the game's stream handler's address", "CA game 0 host 5" in lines, f"{lines[-1]}")
    # a handler that takes the sector's bytes inside the call gets that sector's
    rc, lines = run(exe, img, host)
    yield check("a handler that takes the sector's bytes inside the call gets that sector's (the sums of sectors 10 to 14)", [int(l.split()[3]) for l in lines if l.startswith("  sector")] == [total(n) for n in range(10, 15)], f"{[l for l in lines if l.startswith('  sector')]}")
    # removal: the game's handler is called again
    rc, lines = run(exe, img, "cdinit\nhost 1\ncb 1\nhandler 80100000 512\n" + dbl + "readn 10\ntick 1\nhost 0\ntick 1\ncalls\n")
    yield check("after the host handler is removed (a null pointer) the game's handler is called again (2 host calls, then 3 game calls)",
                "CA game 3 host 2" in lines and sectors(lines) == [10, 11, 12, 13, 14], f"{lines[-1]} {sectors(lines)}")
    rc, lines = run(exe, img, "cdinit\nhost 1\nhost 0\ncb 1\nhandler 80100000 512\n" + dbl + "readn 10\ntick 1\ncalls\n")
    yield check("a host handler set and removed before the read is never called", "CA game 2 host 0" in lines, f"{lines[-1]}")
    # CdInit removes it
    rc, lines = run(exe, img, "cdinit\nhost 1\ncdinit\ncb 1\nhandler 80100000 512\n" + dbl + "readn 10\ntick 1\ncalls\n")
    yield check("port_CdInit removes the host handler", "CA game 2 host 0" in lines and not any(l.startswith("HH ") for l in lines), f"{lines[-1]}")
    rc, lines = run(exe, img, "cdinit\nhost 1\ncb 1\nhandler 80100000 512\n" + dbl + "readn 10\ntick 1\nact 1 I 0\ntick 1\ncalls\n")
    yield check("... a CdInit from a handler included: the read stops, and a later read is the game's", "CA game 0 host 3" in lines or "CA game 0 host 2" in lines, f"{lines[-1]}")
    # the compressed audio never reaches it
    rc, lines = run(exe, img, "cdinit\nhost 1\nhandler 80100000 512\nctl N e 40\nreadn 12\ntick 3\ncalls\nxac\n")
    xc = [l for l in lines if l.startswith("XC ")]
    yield check("with the compressed audio on, an audio sector (13) never reaches the host handler: sectors 12 and 14 do",
                sectors(lines) == [12, 14] and "CA game 0 host 2" in lines and "silent 1" in xc[0], f"{sectors(lines)} {lines[-2:]}")
    # a read past the image's end: interrupt code 5 with the error status
    img2 = root / "h2.bin"
    make_image(img2, 12)
    rc, lines = run(exe, img2, "cdinit\nhost 1\nhandler 80100000 512\nreadn 9\ntick 4\ntick 4\nready 1\ncalls\n")
    hh = [l for l in lines if l.startswith("HH ")]
    yield check("a read past the image's end gives the host handler interrupt code 5 with the error result, once, after sectors 9 to 11",
                hh == ["HH intr=1 res0=22 depth=1"] * 3 + ["HH intr=5 res0=03 depth=1"] and sectors(lines) == [9, 10, 11], f"{hh}")
    rc, lines = run(exe, img2, "cdinit\nhost 1\ncb 3\nreadn 11\ntick 4\ncalls\n")
    yield check("... the error does not reach the game's handler", "CA game 0 host 2" in lines, f"{lines[-1]}")
    # a handler that pauses the read stops the row; ReadN continues
    rc, lines = run(exe, img, "cdinit\nhost 1\nhandler 80100000 512\n" + dbl + "act 1 P 0\nreadn 10\ntickc 3\nctl N 6\ntickc 1\n")
    v = [int(l.split()[1]) for l in lines if l.startswith("V ")]
    yield check("a host handler that pauses the read stops the row: 1 sector, then none; ReadN continues from the next with a fresh carry (2 sectors)",
                rc == 0 and v == [1, 0, 0, 2] and sectors(lines) == [10, 11, 12], f"{rc} {v} {sectors(lines)}")
    # a handler that starts a new read gets one sector a vblank
    rc, lines = run(exe, img, "cdinit\nhost 1\nhandler 80100000 512\n" + dbl + "act 0 N 40\nreadn 10\ntickc 5\n")
    v = [int(l.split()[1]) for l in lines if l.startswith("V ")]
    yield check("a host handler that starts a new read at every sector gets one sector a vblank (the header's rule): 10, then 40 each time",
                rc == 0 and v == [1] * 5 and sectors(lines) == [10, 40, 40, 40, 40], f"{rc} {v} {sectors(lines)}")
    rc, lines = run(exe, img, "cdinit\ncb 1\nhandler 80100000 512\n" + dbl + "act 0 N 40\nreadn 10\ntickc 5\n")
    yield check("... the same as the game's handler gets", [int(l.split()[1]) for l in lines if l.startswith("V ")] == [1] * 5, f"{lines}")
    # the handler calls the layer: CdReady inside takes the flag
    rc, lines = run(exe, img, "cdinit\nhost 1\nhandler 80100000 512\n" + dbl + "act 0 R 0\nreadn 10\ntick 1\nready 1\n")
    yield check("a host handler may call CdReady and CdGetSector from inside (the flag is taken: CdReady(1) afterwards finds none)",
                rc == 0 and sectors(lines) == [10, 11] and [l for l in lines if l.startswith("Y ")][0].startswith("Y ret=0"), f"{rc} {lines[-3:]}")
    # port_handler_depth is 1 inside, 0 after
    rc, lines = run(exe, img2, "cdinit\nhost 1\ndepth\n" + dbl + "readn 9\ntick 1\ndepth\ntick 4\ndepth\n")
    d = [int(l.split()[1]) for l in lines if l.startswith("D ")]
    yield check("port_handler_depth is 1 inside the host handler (for data and for the error) and 0 before and after",
                d == [0, 0, 0] and all(l.endswith("depth=1") for l in lines if l.startswith("HH ")) and any("intr=5" in l for l in lines), f"{d} {[l for l in lines if l.startswith('HH ')]}")
    # the ring guard of the game's own stream does not apply: a ring that would hold a sector
    guard = "cdinit\ncb 2\nhandler 80100000 0\n" + dbl + "ring 5 1\nreadn 0\ntickc 2\n"
    rc, lines = run(exe, img, guard)
    v0 = [int(l.split()[1]) for l in lines if l.startswith("V ")]
    rc, lines = run(exe, img, guard.replace("cb 2", "host 1\ncb 2"))
    v1 = [int(l.split()[1]) for l in lines if l.startswith("V ")]
    yield check("with the game's ring variables arranged so that the guard holds a sector (fill 4), it does: no sector without a host handler (control), the host handler gets 2 and 3",
                v0 == [0, 0] and v1 == [2, 3], f"{v0} {v1}")
    # the polling rule does not apply: no handler variable, the flag not taken
    poll = "cdinit\ncb 0\nhandler 80100000 0\n" + dbl + "readn 0\ntick 2\nctl N 11\n"   # GetlocP: bytes 2 to 4 are the position of the next sector to read
    rc, lines = run(exe, img, poll)
    p0 = [l for l in lines if l.startswith("C ret=1 res=01 01")]
    rc, lines = run(exe, img, poll.replace("cb 0", "host 1\ncb 0"))
    p1 = [l for l in lines if l.startswith("C ret=1 res=01 01")]
    yield check("a polling game (no handler variable): without a host handler one sector waits for the poll (control: after 2 ticks the next sector is 1); with one all of them are delivered (2 and 3: the next is 5)",
                p0 == ["C ret=1 res=01 01 00 00 01 00 02 01"] and p1 == ["C ret=1 res=01 01 00 00 05 00 02 05"], f"{p0} {p1}")
    # whole sectors through the host handler: the bytes taken inside the call
    img3 = root / "h3.bin"
    data = make_pattern_image(img3, 20)
    rc, lines = run(exe, img3, f"cdinit\nhost 1\nctl N e 20\nhandler {A:x} 585\nreadn 5\ntick 1\n")
    got = [int(l.split()[3]) for l in lines if l.startswith("  sector")]
    yield check("a host handler in the whole-sector mode takes the 2,340 bytes of its sector inside the call", got == [sum(data[n * SECTOR + 12:(n + 1) * SECTOR]) for n in (5,)], f"{got}")


def seam_cases(root: Path, exe: Path):
    """port_cd_fifo_read and port_cd_store, the two routines for a stream's code in the runtime."""
    img = root / "s2.bin"
    data = make_pattern_image(img, 40)
    raw = lambda n: data[n * SECTOR:(n + 1) * SECTOR]
    whole = lambda n: raw(n)[12:]
    plain = lambda n: raw(n)[24:24 + 2048]
    take = "autotick 1\nready 0\nautotick 0\n"
    A = 0x80100000
    # the FIFO read: one piece, and pieces, in both forms
    for mode, form, body, size in ((0x20, "whole-sector", whole, 2340), (0x00, "2,048-byte", plain, 2048)):
        head = f"cdinit\nctl N e {mode:x}\nreadn 7\n{take}"
        rc, lines = run(exe, img, head + f"fr 0 {size}\nrdump 0 {size}\n")
        yield check(f"fifo read, {form} mode: one piece of {size} bytes is the sector's bytes, all real, with the sector's number",
                    rc == 0 and "FR real %d sector 7" % size in lines and dump_of(lines) == body(7), f"{rc} {lines[:3]}")
        cuts = [12, 32, 2016, size - 2060] if size == 2340 else [12, 32, 2004]
        script = head + "".join(f"fr {sum(cuts[:i])} {c}\n" for i, c in enumerate(cuts)) + f"rdump 0 {size}\n"
        rc, lines = run(exe, img, script)
        frs = [l for l in lines if l.startswith("FR ")]
        yield check(f"fifo read, {form} mode: pieces of {cuts} bytes continue each other and give the bytes in order",
                    rc == 0 and frs == [f"FR real {c} sector 7" for c in cuts] and dump_of(lines) == body(7), f"{rc} {frs}")
        rc, lines = run(exe, img, head + f"rfill 1\nfr 0 {size - 4}\nfr 0 40\nrdump 0 40\n")
        d = dump_of(lines)
        yield check(f"fifo read, {form} mode: past the end the rest is zeros and the count says how many were real (4 of 40)",
                    rc == 0 and "FR real 4 sector 7" in lines and d == body(7)[size - 4:] + bytes(36), f"{rc} {lines[-3:]}")
        rc, lines = run(exe, img, head + f"fr 0 {size}\nrfill 3\nfr 0 16\nrdump 0 16\n")
        yield check(f"fifo read, {form} mode: a read of a sector taken whole gives 0 real bytes, zeros over what was there, still the sector's number",
                    "FR real 0 sector 7" in lines and dump_of(lines) == bytes(16), f"{rc} {lines[-3:]}")
    # alternately with port_CdGetSector
    w = whole(7)
    script = (f"cdinit\nctl N e 20\nreadn 7\n{take}gs {A:x} 3\nfr 0 32\ngs {A + 12:x} 8\nfr 32 2016\ngs {A + 44:x} 70\n"
              f"rdump 0 2048\ndump {A:x} 12\ndump {A + 12:x} 32\ndump {A + 44:x} 280\n")
    rc, lines = run(exe, img, script)
    r = dump_of(lines, 0)
    yield check("fifo read and CdGetSector alternate: bytes 0-11 by gs, 12-43 by fr, 44-75 by gs, 76-2091 by fr, 2092 on by gs (zeros past 2340)",
                rc == 0 and dump_of(lines, 1) == w[:12] and r[:32] == w[12:44] and dump_of(lines, 2) == w[44:76] and r[32:2048] == w[76:2092]
                and dump_of(lines, 3) == w[2092:] + bytes(280 - (2340 - 2092)), f"{rc} {[len(dump_of(lines, k)) for k in range(4)]}")
    # no sector in the FIFO: zeros, 0 real, sector -1; also after CdInit
    rc, lines = run(exe, img, "cdinit\nrfill 9\nfr 0 8\nrdump 0 8\n")
    yield check("fifo read with no sector in the FIFO: 0 real bytes, zeros, sector -1", "FR real 0 sector -1" in lines and dump_of(lines) == bytes(8), f"{rc} {lines}")
    rc, lines = run(exe, img, f"cdinit\nreadn 7\n{take}cdinit\nrfill 9\nfr 0 8\n")
    yield check("fifo read after CdInit: no sector in the FIFO again (sector -1)", "FR real 0 sector -1" in lines, f"{lines}")
    rc, lines = run(exe, img, f"cdinit\nreadn 7\n{take}frnull 4\nfr 0 0\n")
    yield check("fifo read with a null sector pointer works, and with 0 bytes gives 0 and moves nothing", rc == 0 and "FN real 4" in lines and "FR real 0 sector 7" in lines, f"{rc} {lines}")
    # nothing of the game's memory or record changes, and the hook is not called
    script = (f"cdinit\nctl N e 20\nreadn 7\n{take}hook 1\nfr 0 2340\nsrc {A:x}\nsrc {A + 400:x}\ndump {A:x} 8\nword {A:x}\n")
    rc, lines = run(exe, img, script)
    yield check("fifo read changes nothing in the game's RAM, nothing in the record of sources (-1 for the words it would have covered) and does not call the hook",
                rc == 0 and [l for l in lines if l.startswith("P ")] == ["P -1", "P -1"] and dump_of(lines) == bytes(8) and not any(l.startswith("K ") for l in lines), f"{rc} {lines[-5:]}")
    script = f"cdinit\nctl N e 20\nreadn 7\n{take}hook 1\nfrram {A + 0x200:x} 100\nsrc {A + 0x200:x}\nsrc {A + 0x210:x}\n"
    rc, lines = run(exe, img, script)
    yield check("fifo read with a destination inside the game's RAM (a caller's mistake) still records nothing and calls no hook, though the bytes land",
                rc == 0 and "FQ real 100" in lines and [l for l in lines if l.startswith("P ")] == ["P -1", "P -1"] and not any(l.startswith("K ") for l in lines), f"{rc} {lines}")
    script = (f"cdinit\nctl N e 20\nreadn 7\n{take}gs {A:x} 585\nreadn 8\ntick 1\nfr 0 100\nsrc {A:x}\nsrc {A + 400:x}\n")
    rc, lines = run(exe, img, script)
    yield check("fifo read leaves the record of an earlier copy alone (sector 7 still the source)", [l for l in lines if l.startswith("P ")] == ["P 7", "P 7"], f"{lines[-3:]}")
    # store
    script = (f"cdinit\nhook 1\nrfill 17\nst {A + 0x100:x} 0 64 9\nsrc {A + 0xfc:x}\nsrc {A + 0x100:x}\nsrc {A + 0x13c:x}\nsrc {A + 0x140:x}\n"
              f"dump {A + 0xfc:x} 70\n")
    rc, lines = run(exe, img, script)
    d = dump_of(lines)
    yield check("store: the bytes land, the words covered come from the sector given, the neighbours from nowhere, the hook is called once with the address and the length",
                rc == 0 and d == bytes(4) + bytes((17 + i) & 0xFF for i in range(64)) + bytes(2) and [l for l in lines if l.startswith("P ")] == ["P -1", "P 9", "P 9", "P -1"]
                and [l for l in lines if l.startswith("K ")] == [f"K {A + 0x100:x} 64"], f"{rc} {lines}")
    rc, lines = run(exe, img, f"cdinit\nhook 1\nrfill 17\nst {A:x} 0 64 9\nst {A:x} 0 32 -1\nsrc {A:x}\nsrc {A + 32:x}\n")
    yield check("store with the sector -1: the words are from nowhere (and replace the record of an earlier store); a second hook call",
                [l for l in lines if l.startswith("P ")] == ["P -1", "P 9"] and len([l for l in lines if l.startswith("K ")]) == 2, f"{lines}")
    rc, lines = run(exe, img, f"cdinit\nhook 1\nst {A:x} 0 0 5\nst 7ffffff0 0 0 5\nst 80300000 0 0 5\nsrc {A:x}\n")
    yield check("store of 0 bytes does nothing (no hook, no record, no stop, whatever the address)", rc == 0 and "ST done" in lines and not any(l.startswith("K ") for l in lines) and "P -1" in lines, f"{rc} {lines}")
    for a, n, label in ((0x7ffffff0, 16, "below the RAM"), (0x80200000 - 8, 16, "over the end of the RAM"), (0x80200000, 4, "past the RAM")):
        rc, lines = run(exe, img, f"cdinit\nhook 1\nrfill 17\nst {a:x} 0 {n} 3\n")
        yield check(f"store to a destination {label} stops with the routine's name and status 6, nothing copied",
                    rc == 6 and lines and lines[-1].startswith("stop: port_cd_store buffer 0x") and lines[-1].endswith(f"({n} bytes) is outside the PS1's RAM") and "ST done" not in lines and not any(l.startswith("K ") for l in lines), f"{rc} {lines}")
    # the last bytes of the RAM are accepted
    rc, lines = run(exe, img, f"cdinit\nrfill 17\nst 801ffff0 0 16 3\nsrc 801ffffc\n")
    yield check("store into the last 16 bytes of the RAM is accepted and recorded", rc == 0 and "P 3" in lines, f"{rc} {lines}")


def groups(root: Path, exe: Path):
    yield read_cases(root, exe)
    yield rate_cases(root, exe)
    yield guard_cases(root, exe)
    yield pieces_cases(root, exe)
    yield pause_cases(root, exe)
    yield end_cases(root, exe)
    yield conversion_cases(root, exe)
    yield page_cases(root, exe)
    yield poll_cases(root, exe)
    yield command_cases(root, exe)
    yield audio_cases(root, exe)
    yield unpolled_cases(root, exe)
    yield stream_cases(root, exe)
    yield initvol_cases(root, exe)
    yield stop_cases(root, exe)
    yield buffer_cases(root, exe)
    yield whole_cases(root, exe)
    yield host_cases(root, exe)
    yield seam_cases(root, exe)


def main() -> int:
    global SRC
    if len(sys.argv) == 3 and sys.argv[1] == "--src":   # another copy of the runtime's folder (the mutants of the controls run on a scratch copy)
        SRC = Path(sys.argv[2]).resolve()
    elif len(sys.argv) != 1:
        print("usage: test_hostcd.py [--src DIR]")
        return 2
    cc = shutil.which("cc")
    if not cc:
        print("the host's C compiler `cc` is missing; these controls need it")
        return 2
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostcd-test-") as tmp:
        root = Path(tmp)
        exe = build(root, cc)
        for produced in groups(root, exe):
            while True:
                try:
                    name, detail = next(produced)
                except StopIteration:
                    break
                except Exception as err:  # a control must report, not crash
                    print(f"FAIL the control itself raised {type(err).__name__}: {err}")
                    failed += 1
                    break
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
