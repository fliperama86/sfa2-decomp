#!/usr/bin/env python3
"""Controls for the disc layer, port/src/cd.c, built natively with the host's `cc`.

What is tested: the host routines of the CD library functions that the game
calls, on disc images of invented bytes (every sector says which sector it is
in its first two bytes; the rest is a pattern), with a made-up ready handler,
a made-up port_tick and a made-up RAM in the test's own C (a script on
standard input drives it, one command a line). Cases: Setloc and ReadN
delivering sectors in order, a fixed number a tick and none outside a tick;
CdGetSector handing a sector out in pieces; Pause and a new Setloc in the
middle of a read, and ReadN continuing after a Pause; reading past the end of
the image (a disc error to the handler, the read stopped); the position
conversions at their borders; the page-source record; the polling reader (no
handler: one sector waits for CdReady); the implied Setloc of SeekL and
ReadS and its second response; GetlocP; XA audio sectors dropped; the
library variables the game's C reads; an unknown command's stop and a
Setmode with the whole-sector bit; the buffer CdGetSector is given: its
start or its end outside the PS1's RAM, a count that overflows 32 bits, a
negative count, the last bytes of RAM (accepted), each ending the program
with a line and status 6 before a byte is copied.

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

SECTOR = 2352
HANDLER_VALUE = 0x1234

TEST_MAIN = r'''
#include "cd.h"
#include <stdlib.h>
#include <string.h>

static unsigned char *ram;
static int ticks, autotick;
static int h_on;
static unsigned h_dest;
static int h_pieces[16], h_np, h_stop, h_calls;

volatile int port_handler_depth;   /* the driver's counter; cd.c raises it around the ready handler */
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
    if (h != 0x1234) { printf("H bad handler value %x\n", h); return; }
    printf("H intr=%d res0=%02x\n", intr, res[0]);
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
        if (!strcmp(cmd, "n")) printf("N %d\n", PORT_CD_SECTORS_PER_TICK);
        else if (!strcmp(cmd, "cdinit")) printf("R %d\n", port_CdInit());
        else if (!strcmp(cmd, "autotick")) autotick = atoi(rest);
        else if (!strcmp(cmd, "ticks")) printf("T %d\n", ticks);
        else if (!strcmp(cmd, "tick")) { int k = atoi(rest); while (k-- > 0) port_cd_tick(); }
        else if (!strcmp(cmd, "cb")) { unsigned v = atoi(rest) ? HANDLER_VALUE_C : 0; unsigned char *p = at(PORT_CD_VAR_READY_CB); p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = p[3] = 0; }
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
        else if (!strcmp(cmd, "page")) { unsigned a = (unsigned)strtoul(rest, 0, 16); printf("P %d\n", port_cd_page_source(a)); }
        else if (!strcmp(cmd, "i2p")) { unsigned char p[4] = {9, 9, 9, 9}; unsigned char *r = port_CdIntToPos(atoi(rest), p); printf("I %02x %02x %02x %02x same=%d\n", p[0], p[1], p[2], p[3], r == p); }
        else if (!strcmp(cmd, "p2i")) { unsigned a, b, c; unsigned char p[3]; if (sscanf(rest, "%x %x %x", &a, &b, &c) != 3) return 64; p[0] = (unsigned char)a; p[1] = (unsigned char)b; p[2] = (unsigned char)c; printf("J %d\n", port_CdPosToInt(p)); }
        else if (!strcmp(cmd, "word")) { unsigned a = (unsigned)strtoul(rest, 0, 16); unsigned char *p = at(a); printf("W %08x\n", p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24); }
        else if (!strcmp(cmd, "mix")) { printf("X %d\n", port_CdMix(0)); }
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
    proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-o", str(exe), str(main), str(SRC / "cd.c"), str(SRC / "disc.c"), str(SRC / "sha256.c")],
                          capture_output=True, text=True, timeout=180)
    if proc.returncode != 0:
        raise RuntimeError("the test program does not build:\n" + proc.stderr[-1500:])
    return exe


def run(exe: Path, image: Path, script: str) -> tuple[int, list[str]]:
    proc = subprocess.run([str(exe), str(image)], input=script, capture_output=True, text=True, timeout=60)
    return proc.returncode, proc.stdout.splitlines()


def check(name: str, ok: bool, detail: str = ""):
    return name, None if ok else detail


def hexs(*b: int) -> str:
    return " ".join(f"{x:02x}" for x in b)


# ---- cases -------------------------------------------------------------------

def read_cases(root: Path, exe: Path):
    img = root / "r.bin"
    make_image(img, 60)
    # N from the runtime is only a quantity to count with; the case's meaning is "the same
    # number every tick, in order, none without a tick"
    rc, lines = run(exe, img, "n\n")
    n = int(lines[0].split()[1])
    yield check("a tick delivers a fixed number of sectors", n >= 1, f"N {n}")
    script = ("cdinit\ncb 1\nhandler 80100000 512\nreadn 10\n"
              "ticks\n"                       # nothing delivered by the commands
              "tick 1\ntick 1\n")
    rc, lines = run(exe, img, script)
    got = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    want = list(range(10, 10 + 2 * n))
    yield check("Setloc and ReadN deliver the sectors in order, N a tick", got == want and rc == 0, f"{got} {want} {lines}")
    sums = [int(l.split()[3]) for l in lines if l.startswith("  sector")]
    yield check("each sector's 2048 bytes reach the handler's buffer", sums == [total(s) for s in want], f"{sums}")
    intr = [l for l in lines if l.startswith("H ")]
    yield check("the handler gets interrupt code 1 and a status result", all(l == "H intr=1 res0=22" for l in intr) and len(intr) == 2 * n, f"{intr}")
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
    rc, lines = run(exe, img, "n\n")
    n = int(lines[0].split()[1])
    script = ("cdinit\ncb 1\nhandler 80100000 512\nreadn 10\ntick 1\n"
              "ctl B 9\n"
              "tick 2\n"
              "readn 40\ntick 1\n"
              "ctl B 9\nctl N 6\ntick 1\n")
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    want = list(range(10, 10 + n)) + list(range(40, 40 + n)) + list(range(40 + n, 40 + 2 * n))
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
    script = ("cdinit\nreadn 12\nautotick 1\nready 0\nautotick 0\n"
              "page 80100000\n"
              "gs 80101800 512\n"          # 0x800 bytes from 0x80101800: ends at 0x80101fff, one page
              "page 80101000\npage 80101fff\npage 80102000\n"
              "tick 1\ngs 80101f00 512\n"      # crosses the page border into 0x80102000
              "page 80101000\npage 80102000\npage 80103000\n"
              "page 7fffffff\npage 80200000\npage 00101000\n")
    rc, lines = run(exe, img, script)
    p = [l for l in lines if l.startswith("P ")]
    want = ["P -1", "P 12", "P 12", "P -1", "P 13", "P 13", "P -1", "P -1", "P -1", "P -1"]
    yield check("the page-source record: pages written by CdGetSector hold the sector, others none", p == want, f"{p} {want}")


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
    rc, lines = run(exe, img, "n\n")
    n = int(lines[0].split()[1])
    span = range(4, 4 + 2 * n)   # two ticks consume 2n sectors, audio ones included
    script = "cdinit\nctl N e c8\ncb 1\nhandler 80100000 512\nreadn 4\ntick 1\ntick 1\n"
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("with XA on, audio sectors are consumed (they use the tick's budget) and never reach the handler",
                sec == [x for x in span if x not in (6, 7)], f"{sec}")
    script = "cdinit\ncb 1\nhandler 80100000 512\nreadn 4\ntick 1\ntick 1\n"
    rc, lines = run(exe, img, script)
    sec = [int(l.split()[1]) for l in lines if l.startswith("  sector")]
    yield check("with XA off, the same sectors are data", sec == list(span), f"{sec}")


def stop_cases(root: Path, exe: Path):
    img = root / "s.bin"
    make_image(img, 10)
    for com, label in ((0x10, "GetlocL"), (0x03, "Play"), (0x07, "MotorOn"), (0x1a, "GetID")):
        rc, lines = run(exe, img, f"cdinit\nctl N {com:x}\nmix\n")
        yield check(f"an unsent command ({label}) stops with its name and status 6",
                    rc == 6 and lines[-1] == f"stop: disc command 0x{com:x} is not handled" and "X 1" not in lines, f"{rc} {lines}")
    rc, lines = run(exe, img, "cdinit\nctl F 10\n")
    yield check("the stop is the same through CdControlF", rc == 6 and lines[-1] == "stop: disc command 0x10 is not handled", f"{rc} {lines}")
    rc, lines = run(exe, img, "cdinit\nctl N e 20\n")
    yield check("Setmode with the whole-sector bit stops", rc == 6 and lines[-1] == "stop: disc mode (whole sector) 0x20 is not handled", f"{rc} {lines}")
    rc, lines = run(exe, img, "cdinit\nctl N e a0\n")
    yield check("the same bit inside other bits", rc == 6 and lines[-1] == "stop: disc mode (whole sector) 0xa0 is not handled", f"{rc} {lines}")


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
    rc, lines = run(exe, img, fill + "gsraw 801ffff0 4\npage 801ff000\npage 801fe000\npage 80200000\npage 7fffffff\n")
    yield check("the page-source record: the last page holds the sector, the page before none, addresses outside the RAM give -1",
                rc == 0 and [l for l in lines if l.startswith("P ")] == ["P 6", "P -1", "P -1", "P -1"], f"{rc} {lines[-5:]}")


def groups(root: Path, exe: Path):
    yield read_cases(root, exe)
    yield pieces_cases(root, exe)
    yield pause_cases(root, exe)
    yield end_cases(root, exe)
    yield conversion_cases(root, exe)
    yield page_cases(root, exe)
    yield poll_cases(root, exe)
    yield command_cases(root, exe)
    yield audio_cases(root, exe)
    yield stop_cases(root, exe)
    yield buffer_cases(root, exe)


def main() -> int:
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
