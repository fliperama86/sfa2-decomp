#!/usr/bin/env python3
"""Controls for the controller pads and the input script: input.c and the pads part of kernel.c, run for real.

    python3 test_hostpads.py --cc CROSS_CC [--psyz-build DIR] [--run PREFIX] [--src DIR] [--only SUBSTRING]

The runtime is built twice with the cross compiler, as test_hostlaunch.py builds it, with made-up game code that
calls the pad routines linked like the game's C: once as a build without PsyZ ("plain") and once with input.c
compiled for PsyZ (-DPORT_HAVE_PSYZ) and linked with a stand-in of this file for PsyZ's three routines (PadInit,
Psyz_PadsPoll, Psyz_PadsGet), which count their calls and give a frame that depends on the number of polls, and,
like PsyZ's, make Psyz_PadsGet poll once by itself when no poll came before and hand out the last frame otherwise.
Nothing starts PsyZ's window or reads a device. The game code records the buffer at every vertical blank in an event
handler (so each record is whole) and prints what it saw; this file works out what must have been seen.

- InitPAD and StartPAD: nothing is written before StartPAD; from it on port 1's buffer is written at every vertical
  blank and port 2's with 0xff; PsyZ's PadInit runs at InitPAD; with the stand-in the poll comes once per vertical
  blank before the frame is fetched (so every frame arrives whole and fresh), the fetch is for port 1 only; without
  PsyZ port 1 is a digital pad with no button pressed;
- lengths 0, 1, 2, 3, 4, 5, 8, 33, 34, 35 and 40 for both buffers, in both builds: the first min(length, 34) bytes
  are the frame (port 2: 0xff), the bytes after them are not touched, a length of 0 polls nothing;
- the buffer the game gives is checked when it is given: a buffer outside the game's memory (the low mirror, past
  the RAM), one that ends past the RAM or the scratchpad, a negative length (also the smallest int), each for port 1
  and port 2, end the run with a line (status 9) before any write; a buffer that ends exactly at the end, a length of
  0 with a wild address, a null buffer with a length are served;
- --trace: a line `pad frame N: 0x.... -> 0x....` when the word the game builds from port 1's buffer changes,
  also to 0 when the frame says "no controller"; no such line without a change;
- --input FILE: the script's buttons are pressed at their exact frames (and released at theirs), on top of the
  stand-in's buttons and in the build without PsyZ, with a repeat line and its end, with the lines on one frame in
  order; on a frame that says "no controller" nothing is pressed and one line says so; every malformed line
  (unknown button, neither down nor up, too few or too many fields, a time that is not a number, negative, too large, a
  time that goes backwards after rounding to frames, a repeat with a bad interval, too many steps, a line that is too
  long, a NUL byte), an empty file, a file with nothing but comments, a file that is too large and one that cannot
  be opened refuse the start (status 2, one line, nothing else printed) with the file and the line number;
  the options that go with it (twice, no value, with --list-library) give the usage;
- the line that names the keys (`pad: keys: ...`) is printed only in the build with PsyZ, and with --psyz-build its
  text is checked against PsyZ's own table (keyb_p1 in src/platform/sdl3_common.h) and PsyZ's button definitions
  (libetc.h);
- the pad cases of the games run with the timer on (default), off (--no-interrupt) and with --timer-burst, which
  must print none of its lines.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all cases behaved as required` or the number of cases
that did not. --src names another copy of the runtime's folder (the mutants of the controls run on a scratch copy).
"""

from __future__ import annotations

import argparse
import re
import shutil
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostlaunch as L  # noqa: E402
from test_hostlaunch import ABS_BASE, FUN_BASE, PROLOGUE, RAM, Variant, program, head, verdict  # noqa: E402

BUF1, BUF2 = 0x80150000, 0x80151000
PAD_NAMES = ["InitPAD", "StartPAD", "ResetCallback", "OpenEvent", "EnableEvent", "StartRCnt", "VSync", "EnterCriticalSection", "ExitCriticalSection"]
PAD_ADDR = {n: RAM + 0x102000 + 0x10 * i for i, n in enumerate(PAD_NAMES)}
G_NAMES = ["g_basic", "g_len", "g_words", "g_script", "g_script_none", "g_init_ok"]
G = {n: RAM + 0x102200 + 0x10 * i for i, n in enumerate(G_NAMES + ["g_stop"])}
ABS_PADS = ABS_BASE + [(n, a, 1) for n, a in PAD_ADDR.items()]
HOST_PADS = {n: L.HOSTS[n] for n in PAD_NAMES if n in L.HOSTS} | {"InitPAD": "port_h_InitPAD", "StartPAD": "port_h_StartPAD"}
KEYS_PREFIX = "pad: keys: "

STANDIN_C = r"""
#include <string.h>
int standin_polls, standin_gets, standin_bad_port, standin_pad_inits, standin_mode;
static unsigned char frame[34];
static int sampled;
int standin_expect(int n, int i) { return i == 0 ? 0 : i == 1 ? 0x41 : (n * 7 + i * 13 + 1) & 0xff; }
static unsigned word_mode2(int n)
{
    unsigned w = 0;
    if (n >= 5 && n <= 8) w |= 0x0110;
    if (n >= 13 && n <= 17) w |= 0x0040;
    if (n >= 22 && n <= 23) w |= 0x0001;
    return w;
}
static void build(int n)
{
    int i;
    unsigned w;
    memset(frame, 0, sizeof frame);
    switch (standin_mode) {
    case 0:
        for (i = 0; i < 34; i++) frame[i] = (unsigned char)standin_expect(n, i);
        break;
    case 1:
        if (n >= 5 && n <= 6) memset(frame, 0xff, sizeof frame);
        else {
            frame[1] = 0x41;
            frame[2] = frame[3] = 0xff;
            if (n >= 3 && n <= 4) { frame[2] = 0xf7; frame[3] = 0xbf; }
            else if (n >= 7) frame[3] = 0x7f;
        }
        break;
    case 2:
        w = word_mode2(n);
        frame[1] = 0x41;
        frame[2] = (unsigned char)~(w >> 8);
        frame[3] = (unsigned char)~w;
        break;
    default:
        memset(frame, 0xff, sizeof frame);
    }
}
void PadInit(int mode) { (void)mode; standin_pad_inits++; }
void Psyz_PadsPoll(void)
{
    standin_polls++;
    build(standin_polls);
    sampled = 1;
}
void Psyz_PadsGet(int port, char *dst, int len)
{
    standin_gets++;
    if (port != 0) standin_bad_port++;
    if (len <= 0) return;
    if (!sampled) Psyz_PadsPoll();
    if (len > 34) len = 34;
    memcpy(dst, frame, (size_t)len);
}
"""

PADS_PRELUDE = PROLOGUE + r"""
#include <string.h>
extern unsigned port_frames(void);
extern int standin_polls, standin_gets, standin_bad_port, standin_pad_inits, standin_mode;
extern int standin_expect(int polls, int i);
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_EnableEvent(unsigned), ps1_StartRCnt(unsigned), ps1_VSync(int);
extern int ps1_InitPAD(void *, int, void *, int), ps1_StartPAD(void), ps1_EnterCriticalSection(void), ps1_ExitCriticalSection(void);
extern void *ps1_ResetCallback(void);
#define LINKED @LINKED@
#define BUF1 ((unsigned char *)0x80150000u)
#define BUF2 ((unsigned char *)0x80151000u)
#define EXP(n, i) (LINKED ? standin_expect((n), (i)) : ((i) == 1 ? 0x41 : ((i) == 2 || (i) == 3) ? 0xff : 0))
"""

PADS_GAME = r"""
struct snap { unsigned frame; int polls, gets, phase; unsigned char b[8]; };
static struct snap snaps[600];
static volatile int nsnap, phase;
static void on_vblank(void)
{
    if (nsnap < 600) {
        struct snap *s = &snaps[nsnap];
        s->frame = port_frames();
        s->polls = standin_polls;
        s->gets = standin_gets;
        s->phase = phase;
        memcpy(s->b, BUF1, 8);
        nsnap++;
    }
}
static void setup(void)
{
    unsigned ev;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, on_vblank);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
}
static void wait_snaps(int k) { while (nsnap < k) ps1_VSync(0); }
static int touched(const struct snap *s)
{
    int j;
    for (j = 0; j < 8; j++) if (s->b[j] != 0xA5) return 1;
    return 0;
}

void g_basic(void)
{
    int i, j, pre = 0, untouched = 1, cons = 1, gets_ok = 1, bytes_ok = 1, seen = 0, no_gap = 1, n_post = 0, mark, p2 = 1, guard = 1;
    memset(BUF1, 0xA5, 64);
    memset(BUF2, 0xA5, 64);
    setup();
    ps1_InitPAD(BUF1, 34, BUF2, 34);
    SAY("pad inits %d\n", standin_pad_inits);
    wait_snaps(3);
    phase = 1;
    ps1_StartPAD();
    mark = nsnap;
    wait_snaps(mark + 12);
    ps1_EnterCriticalSection();
    for (i = 0; i < nsnap; i++) {
        struct snap *s = &snaps[i];
        if (!touched(s)) {
            if (seen) no_gap = 0;
            if (s->phase == 0) {
                pre++;
                if (s->polls || s->gets) untouched = 0;
            }
            continue;
        }
        if (s->phase == 0) untouched = 0;
        seen = 1;
        n_post++;
        if (LINKED) {
            if (s->polls != n_post) cons = 0;
            if (s->gets != s->polls) gets_ok = 0;
        } else if (s->polls || s->gets) gets_ok = 0;
        for (j = 0; j < 8; j++) if (s->b[j] != EXP(s->polls, j)) bytes_ok = 0;
    }
    for (i = 0; i < 34; i++) if (BUF2[i] != 0xff) p2 = 0;
    for (i = 34; i < 64; i++) if (BUF2[i] != 0xA5) guard = 0;
    SAY("before StartPAD: %d vblanks seen, buffers untouched and nothing polled %d\n", pre >= 2, untouched);
    SAY("after StartPAD: %d vblanks, polls consecutive %d, fetches equal polls %d, frame as fetched %d, no gap %d\n", n_post >= 10, cons, gets_ok, bytes_ok, no_gap);
    SAY("port 2: filled with 0xff %d, bytes after 34 untouched %d, no fetch for it %d\n", p2, guard, standin_bad_port == 0);
    ps1_ExitCriticalSection();
}

void g_len(void)
{
    static const int lens[] = { 0, 1, 2, 3, 4, 5, 8, 33, 34, 35, 40 };
    int k;
    setup();
    for (k = 0; k < (int)(sizeof lens / sizeof lens[0]); k++) {
        int len = lens[k], m = len < 34 ? len : 34, i, p0, n, whole = 1, beyond = 1, port2 = 1, mark;
        ps1_InitPAD(0, 0, 0, 0);
        memset(BUF1, 0xA5, 64);
        memset(BUF2, 0xA5, 64);
        ps1_StartPAD();
        p0 = standin_polls;
        mark = nsnap;
        ps1_InitPAD(BUF1, len, BUF2, len);
        wait_snaps(mark + 3);
        ps1_EnterCriticalSection();
        ps1_InitPAD(0, 0, 0, 0);
        n = standin_polls;
        for (i = 0; i < m; i++) { if (BUF1[i] != EXP(n, i)) whole = 0; if (BUF2[i] != 0xff) port2 = 0; }
        for (i = m; i < 64; i++) { if (BUF1[i] != 0xA5) beyond = 0; if (BUF2[i] != 0xA5) port2 = 0; }
        SAY("length %d: frame whole %d, bytes after it untouched %d, port 2 %d, polled %d\n", len, whole, beyond, port2, n > p0);
        ps1_ExitCriticalSection();
    }
}

void g_words(void)
{
    int i;
    unsigned last = 0;
    standin_mode = 1;
    memset(BUF1, 0xA5, 64);
    setup();
    ps1_InitPAD(BUF1, 34, 0, 0);
    ps1_StartPAD();
    wait_snaps(nsnap + 16);
    ps1_EnterCriticalSection();
    for (i = 0; i < nsnap; i++) {
        struct snap *s = &snaps[i];
        unsigned word;
        if (!s->polls) continue;
        word = (~(s->b[3] | (s->b[2] << 8))) & 0xffffu;
        if (word != last) SAY("expect pad frame %u: 0x%04x -> 0x%04x\n", s->frame, last, word);
        last = word;
    }
    SAY("words done\n");
    ps1_ExitCriticalSection();
}

static void script_run(int mode, unsigned until)
{
    int i;
    standin_mode = mode;
    memset(BUF1, 0xA5, 64);
    setup();
    ps1_InitPAD(BUF1, 34, 0, 0);
    ps1_StartPAD();
    while (!(nsnap && snaps[nsnap - 1].frame >= until)) ps1_VSync(0);
    ps1_EnterCriticalSection();
    for (i = 0; i < nsnap; i++) {
        struct snap *s = &snaps[i];
        if (touched(s)) SAY("snap %u %d %02x %02x %02x %02x\n", s->frame, s->polls, s->b[0], s->b[1], s->b[2], s->b[3]);
    }
    SAY("script done\n");
    ps1_ExitCriticalSection();
}
void g_script(void) { script_run(2, 80); }
void g_script_none(void) { script_run(3, 30); }

/* buffers that are served: nothing is written for them, or the bytes are inside the memory */
static void say_buffer(const char *what, unsigned char *p, int len, int n)
{
    int i, ok = 1;
    for (i = 0; i < len; i++) if (p[i] != EXP(n, i)) ok = 0;
    SAY("%s: frame as fetched %d\n", what, ok);
}
void g_init_ok(void)
{
    volatile unsigned char *end = (volatile unsigned char *)0x801ffff8u, *scratch = (volatile unsigned char *)0x1f8003f0u;
    int i, n, mark;
    setup();
    SAY("wild address, length 0: returned %d\n", ps1_InitPAD((void *)0x1000u, 0, (void *)0x1000u, 0));
    ps1_StartPAD();
    mark = nsnap;
    wait_snaps(mark + 3);
    SAY("after vblanks\n");
    SAY("null buffers with lengths: returned %d\n", ps1_InitPAD(0, 34, 0, 34));
    mark = nsnap;
    wait_snaps(mark + 3);
    SAY("after vblanks\n");
    for (i = 0; i < 8; i++) end[i] = 0xA5;
    for (i = 0; i < 16; i++) scratch[i] = 0xA5;
    SAY("ends at the last byte: returned %d\n", ps1_InitPAD((void *)0x801ffffcu, 3, (void *)0x801fffffu, 1));
    mark = nsnap;
    wait_snaps(mark + 3);
    ps1_EnterCriticalSection();
    n = standin_polls;
    say_buffer("RAM port 1, 3 bytes", (unsigned char *)0x801ffffcu, 3, n);
    SAY("RAM port 2 last byte 0xff %d, the byte before the buffers untouched %d\n", end[7] == 0xff, end[3] == 0xA5);
    ps1_ExitCriticalSection();
    SAY("scratchpad end: returned %d\n", ps1_InitPAD((void *)0x1f8003fcu, 4, 0, 0));
    mark = nsnap;
    wait_snaps(mark + 3);
    ps1_EnterCriticalSection();
    n = standin_polls;
    say_buffer("scratchpad port 1, 4 bytes", (unsigned char *)0x1f8003fcu, 4, n);
    SAY("scratchpad before the buffer untouched %d\n", scratch[11] == 0xA5);
    ps1_ExitCriticalSection();
    {
        unsigned char local[40];
        int kept = 1;
        memset(local, 0xA5, sizeof local);
        SAY("stack buffer: returned %d\n", ps1_InitPAD(local, 34, 0, 0));
        mark = nsnap;
        wait_snaps(mark + 3);
        ps1_EnterCriticalSection();
        ps1_InitPAD(0, 0, 0, 0);
        n = standin_polls;
        say_buffer("stack buffer", local, 34, n);
        for (i = 34; i < 40; i++) if (local[i] != 0xA5) kept = 0;
        SAY("stack buffer: bytes after the frame untouched %d\n", kept);
        ps1_ExitCriticalSection();
    }
}
"""


def game_stop(b1: int, l1: str, b2: int, l2: str, linked: int = 0) -> str:
    return PADS_PRELUDE.replace("@LINKED@", str(linked)) + f"""
void g_stop(void)
{{
    SAY("before\\n");
    SAY("returned %d\\n", ps1_InitPAD((void *)0x{b1:08x}u, {l1}, (void *)0x{b2:08x}u, {l2}));
    ps1_StartPAD();
    ps1_VSync(0);
    ps1_VSync(0);
    SAY("after\\n");
}}
"""


def pads_domains() -> str:
    decls = "".join(f"extern void {h}();\n" for h in HOST_PADS.values())
    rows = "".join(f'{{ "{n}", (void *){h}, 0 }}, ' for n, h in HOST_PADS.items())
    return f"""
#include "port.h"
{decls}
static const struct port_library t[] = {{ {rows} {{ 0, 0, 0 }} }};
const struct port_domain port_domains[] = {{ {{ "kernel", t }} }};
const unsigned port_domain_count = 1;
static const struct port_override o[] = {{ {{ 0, 0, 0 }} }};
const struct port_override *const port_override_sets[] = {{ o }};
const unsigned port_override_set_count = 1;
"""


FUN_PADS = FUN_BASE + [(n, G[n], n) for n in G_NAMES]
for _flavor, _linked in (("plain", 0), ("linked", 1)):
    L.VARIANTS[f"pads-{_flavor}"] = Variant(f"pads-{_flavor}", PADS_PRELUDE.replace("@LINKED@", str(_linked)) + PADS_GAME, FUN_PADS, ABS_PADS, pads_domains())

INT_MIN = "(-2147483647 - 1)"
STOPS = {   # name: (buffer 1, length 1, buffer 2, length 2, the line)
    "port1-in-the-low-mirror": (0x00001000, "34", BUF2, "34", "InitPAD: the buffer of port 1 (0x00001000, 34 bytes) is not inside the game's memory"),
    "port1-above-the-ram": (0x80200000, "4", BUF2, "34", "InitPAD: the buffer of port 1 (0x80200000, 4 bytes) is not inside the game's memory"),
    "port1-ends-past-the-ram": (0x801ffffc, "8", BUF2, "34", "InitPAD: the buffer of port 1 (0x801ffffc, 8 bytes) is not inside the game's memory"),
    "port1-ends-past-the-scratchpad": (0x1f8003fc, "8", BUF2, "34", "InitPAD: the buffer of port 1 (0x1f8003fc, 8 bytes) is not inside the game's memory"),
    "port1-negative-length": (BUF1, "-1", BUF2, "34", "InitPAD: the length -1 of the buffer of port 1 is negative"),
    "port1-smallest-length": (BUF1, INT_MIN, BUF2, "34", "InitPAD: the length -2147483648 of the buffer of port 1 is negative"),
    "port2-in-the-low-mirror": (BUF1, "34", 0x00001000, "34", "InitPAD: the buffer of port 2 (0x00001000, 34 bytes) is not inside the game's memory"),
    "port2-ends-past-the-ram": (BUF1, "34", 0x801ffffd, "4", "InitPAD: the buffer of port 2 (0x801ffffd, 4 bytes) is not inside the game's memory"),
    "port2-negative-length": (BUF1, "34", BUF2, "-5", "InitPAD: the length -5 of the buffer of port 2 is negative"),
    "port1-checked-before-port2": (0x80200000, "4", 0x00001000, "34", "InitPAD: the buffer of port 1 (0x80200000, 4 bytes) is not inside the game's memory"),
    "length-checked-before-address": (0x80200000, "-3", BUF2, "34", "InitPAD: the length -3 of the buffer of port 1 is negative"),
}
for _name, (_b1, _l1, _b2, _l2, _line) in STOPS.items():
    L.VARIANTS[f"pads-stop-{_name}"] = Variant(f"pads-stop-{_name}", game_stop(_b1, _l1, _b2, _l2), FUN_BASE + [("g_stop", G["g_stop"], "g_stop")], ABS_PADS, pads_domains())

ACTIVE: list = [None]   # the rig that ran last, for the report of a failing case


class PadRig(L.Rig):
    """The launch rig, with input.c built for PsyZ (and the stand-in linked) when `psyz` is set."""

    def __init__(self, cc, prefix, work, psyz: bool):
        super().__init__(cc, prefix, work)
        self.psyz = psyz

    def runtime_objects(self):
        if not self.objects:
            self.objects = [self.compile(L.SRC / f"{n}.c", ["-DPORT_HAVE_PSYZ"] if (n == "input" and self.psyz) else None) for n in L.RUNTIME]
            for name, text in (("stubs", L.STUBS), ("standin", STANDIN_C)):
                path = self.work / f"{name}.c"
                path.write_text(text)
                self.objects.append(self.compile(path))
        return self.objects


def go(rig: PadRig, tag: str, entry: int, variant: str, args=None, timeout: int = 60):
    ACTIVE[0] = rig
    return rig.run(tag, program(entry), variant=variant, args=args, timeout=timeout)


def with_keys(want: list[str], lines: list[str], rig: PadRig) -> list[str]:
    """The lines of a run in which the keys line (the line after `overrides:`) is taken out when it is not known exactly (no --psyz-build): it must still start as the line does."""
    if rig.psyz and want[7] == "?":
        if len(lines) > 7 and lines[7].startswith(KEYS_PREFIX):
            return lines[:7] + ["?"] + lines[8:]
    return lines


def pads_head(img, arg, rig: PadRig, keys: str | None, functions: int = len(FUN_PADS)) -> list[str]:
    lines = head(img, arg, host=len(HOST_PADS), stops=1, overrides=0, with_c=functions, without_c=len(ABS_PADS))
    if rig.psyz:
        lines.append(keys or "?")
    return lines


# ---- what PsyZ's own table says ----

def psyz_keys_line(build: Path) -> str:
    """The keys line that PsyZ's table keyb_p1 and its button definitions give."""
    common = (build / "src/psyz/src/platform/sdl3_common.h").read_text()
    table = re.search(r"keyb_p1\[\]\s*=\s*\{(.*?)\};", common, re.S)
    if not table:
        raise RuntimeError("keyb_p1 is not in PsyZ's sdl3_common.h")
    keys = re.findall(r"SDL_SCANCODE_(\w+),", table.group(1))
    libetc = (build / "src/psyz/include/libetc.h").read_text()
    by_bit: dict[int, str] = {}
    for name, bit, comment in re.findall(r"#define (PAD\w+)\s+\(1<<\s*(\d+)\)\s*// (\S+)", libetc):
        if name.startswith("PADL") and name[4:] in ("up", "down", "left", "right"):
            by_bit[int(bit)] = "d-pad " + comment.lower()
        else:
            by_bit[int(bit)] = comment if re.fullmatch(r"[LR]\d", comment) else comment.lower()
    if sorted(by_bit) != list(range(16)) or len(keys) != 16:
        raise RuntimeError(f"PsyZ's table has {len(keys)} keys and {len(by_bit)} buttons, wanted 16 each")

    def key_name(k: str) -> str:
        if k in ("UP", "DOWN", "LEFT", "RIGHT"):
            return k.capitalize() + " arrow"
        return k if len(k) == 1 else k.capitalize()
    return KEYS_PREFIX + ", ".join(f"{key_name(k)} = {by_bit[i]}" for i, k in enumerate(keys)) + "; a game controller if one is plugged in; port 2 has none"


# ---- the script, as this file reads it ----

BUTTON_BIT = {n: i for i, n in enumerate(["l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square", "select", "l3", "r3", "start", "up", "right", "down", "left"])}


def script_held(text: str, frame: int) -> int:
    """The word of the buttons the script holds at `frame` (a 1 bit is pressed), worked out from its text alone."""
    parsed = []
    for raw in text.splitlines():
        t = raw.split()
        if not t or t[0].startswith("#"):
            continue
        if t[0] == "repeat":
            parsed.append(("repeat", int(float(t[1]) * 60 + 0.5), int(float(t[2]) * 60 + 0.5), BUTTON_BIT[t[3]]))
        else:
            parsed.append(("step", int(float(t[0]) * 60 + 0.5), t[2] == "down", BUTTON_BIT[t[1]]))
    # Two things press a button: a down line that no up line has ended yet, and a press of a repeat that is running
    # (4 frames from each of its times, never past the repeat's end, which is the next line that is not a repeat).
    held = 0
    for p in parsed:
        if p[0] == "step" and p[1] <= frame:
            held = held | (1 << p[3]) if p[2] else held & ~(1 << p[3])
    for i, p in enumerate(parsed):
        if p[0] == "repeat":
            end = next((q[1] for q in parsed[i + 1:] if q[0] == "step"), p[1] + 3600 * 60)
            if any(t <= frame < min(t + 4, end) for t in range(p[1], end, p[2])):
                held |= 1 << p[3]
    return held


def base_word(mode_linked: bool, polls: int) -> int:
    """The word the stand-in's mode 2 holds after `polls` polls (as STANDIN_C builds it); the build without PsyZ holds none."""
    if not mode_linked:
        return 0
    w = 0
    if 5 <= polls <= 8:
        w |= 0x0110
    if 13 <= polls <= 17:
        w |= 0x0040
    if 22 <= polls <= 23:
        w |= 0x0001
    return w


SCRIPT_1 = """# the buttons pressed at their frames
0.2 cross down
0.2 start down
0.25 cross up
0.3 cross down
0.3 cross up
0.35 l2 down
0.45 l2 up
repeat 0.5 0.2 up
0.9 select down
"""
SCRIPT_2 = "repeat 0.5 0.2 square\n"
# Scripts whose words are written out by hand, frame by frame (not worked out by script_held): each is
# (name, text, {frame: word}, last frame). cross is 0x0040, start 0x0800.
EXPLICIT = [
    # a repeat ends at a down line for its own button: the hold lasts until its up line (frame 60)
    ("repeat-then-hold-of-the-same-button", "repeat 0 0.1 cross\n0.15 cross down\n1 cross up\n",
     {1: 0x0040, 3: 0x0040, 4: 0, 5: 0, 6: 0x0040, 8: 0x0040, 9: 0x0040, 10: 0x0040, 11: 0x0040, 30: 0x0040, 59: 0x0040, 60: 0, 61: 0}, 62),
    # a repeat ends at a line for another button: its running press ends at that frame, not 4 frames after it began
    ("repeat-ends-at-a-line-of-another-button", "repeat 0 0.1 cross\n0.15 start down\n0.5 start up\n",
     {7: 0x0040, 8: 0x0040, 9: 0x0800, 10: 0x0800, 29: 0x0800, 30: 0, 31: 0}, 32),
    # a hold that began before a repeat on the same button is not released by the repeat's presses
    ("hold-then-repeat-of-the-same-button", "0.05 cross down\nrepeat 0.1 0.1 cross\n0.5 cross up\n",
     {2: 0, 3: 0x0040, 5: 0x0040, 10: 0x0040, 11: 0x0040, 16: 0x0040, 17: 0x0040, 29: 0x0040, 30: 0, 31: 0}, 32),
    # two repeats on one button whose presses overlap: the end of one press does not cut the other short
    ("two-repeats-on-one-button", "repeat 0 0.1 cross\nrepeat 0.05 0.1 cross\n0.5 start down\n",
     {1: 0x0040, 4: 0x0040, 5: 0x0040, 6: 0x0040, 7: 0x0040, 16: 0x0040, 29: 0x0040, 30: 0x0800, 31: 0x0800}, 32),
]
SCRIPT_3 = "\t0.1   triangle\tdown  \r\n\r\n   # a comment\r\n0.2 triangle up"   # tabs and spaces, CRLF, no newline at the end


def snap_lines(lines: list[str]) -> list[tuple[int, int, int, int, int, int]]:
    out = []
    for line in lines:
        m = re.fullmatch(r"snap (\d+) (\d+) ([0-9a-f]{2}) ([0-9a-f]{2}) ([0-9a-f]{2}) ([0-9a-f]{2})", line)
        if m:
            out.append((int(m[1]), int(m[2]), *(int(x, 16) for x in m.groups()[2:])))
    return out


def game_lines(lines: list[str], entry: int) -> list[str]:
    """The lines between `start:` and the last line."""
    start = f"start: 0x{entry:08x}"
    return lines[lines.index(start) + 1:-1] if start in lines else []


def script_cases(rig: PadRig, linked: bool, text: str, name: str, until: int, extra=None, mode=2):
    """Run g_script with the script `text`; each snapshot must hold the base word with the script's word pressed on it."""
    flavor = "linked" if linked else "plain"
    path = rig.work / f"{name}-{flavor}.txt"
    path.write_bytes(text.encode())
    for label, flags in (("timer", []), ("no-interrupt", ["--no-interrupt"]), ("timer-burst", ["--timer-burst"])) if extra is None else extra:
        status, lines, img, arg = go(rig, f"{name}-{flavor}-{label}", G["g_script"], f"pads-{flavor}", ["--input", rig.native(path), *flags])
        snaps = snap_lines(lines)
        bad = []
        for frame, polls, b0, b1, b2, b3 in snaps:
            want = (base_word(linked, polls) | script_held(text, frame)) & 0xffff
            got = ~(b3 | (b2 << 8)) & 0xffff
            if (b0, b1) != (0, 0x41) or got != want:
                bad.append((frame, polls, f"{got:04x}", f"{want:04x}"))
        frames = [s[0] for s in snaps]
        shape = status == 0 and bool(snaps) and snaps[-1][0] >= until and frames == list(range(frames[0], frames[0] + len(frames))) and lines[-2:] == ["script done", "stop: main returned"]
        pressed = sorted({f for f, _, _, _, b2, b3 in snaps if not (b2 == 0xff and b3 == 0xff)})
        scripted = any(script_held(text, f) for f in frames)
        yield (f"script-{name}-{flavor}-{label}-presses-and-releases-at-the-exact-frames-on-top-of-the-pad",
               None if shape and not bad and scripted and pressed else f"status {status}, {len(snaps)} snapshots, frames {frames[:3]}.., first mismatches (frame, polls, got, wanted) {bad[:6]}")


def explicit_cases(rig: PadRig, linked: bool, name: str, text: str, want: dict[int, int], until: int):
    """Run g_script with `text`; at each listed frame the pad word is the word written in EXPLICIT (with the stand-in's
    own buttons pressed on it in the linked build). Nothing of the expectation comes from script_held."""
    flavor = "linked" if linked else "plain"
    path = rig.work / f"explicit-{name}-{flavor}.txt"
    path.write_bytes(text.encode())
    for label, flags in (("timer", []), ("no-interrupt", ["--no-interrupt"])):
        status, lines, img, arg = go(rig, f"explicit-{name}-{flavor}-{label}", G["g_script"], f"pads-{flavor}", ["--input", rig.native(path), *flags])
        snaps = {frame: (polls, b0, b1, b2, b3) for frame, polls, b0, b1, b2, b3 in snap_lines(lines)}
        bad = []
        for frame, word in sorted(want.items()):
            if frame not in snaps:
                bad.append((frame, "no snapshot", f"{word:04x}"))
                continue
            polls, b0, b1, b2, b3 = snaps[frame]
            got = ~(b3 | (b2 << 8)) & 0xffff
            expected = (word | base_word(linked, polls)) & 0xffff
            if (b0, b1) != (0, 0x41) or got != expected:
                bad.append((frame, f"{got:04x}", f"{expected:04x}"))
        shape = status == 0 and max(snaps, default=0) >= until and lines[-2:] == ["script done", "stop: main returned"]
        yield (f"script-{name}-{flavor}-{label}-holds-the-words-written-out-frame-by-frame",
               None if shape and not bad else f"status {status}, {len(snaps)} snapshots, (frame, got, wanted) {bad[:8]}")


def cases(plain: PadRig, linked: PadRig, psyz_build: Path | None):
    keys_line = psyz_keys_line(psyz_build) if psyz_build else None
    kl = keys_line if keys_line else None
    rigs = ((plain, "plain", False), (linked, "linked", True))

    # ---- the buffer: nothing before StartPAD, every vertical blank after it ----
    for rig, flavor, is_linked in rigs:
        for label, flags in (("timer", []), ("no-interrupt", ["--no-interrupt"]), ("timer-burst", ["--timer-burst"])):
            status, lines, img, arg = go(rig, f"basic-{flavor}-{label}", G["g_basic"], f"pads-{flavor}", flags)
            want = pads_head(img, arg, rig, kl) + [f"start: 0x{G['g_basic']:08x}", f"pad inits {1 if is_linked else 0}",
                "before StartPAD: 1 vblanks seen, buffers untouched and nothing polled 1",
                "after StartPAD: 1 vblanks, polls consecutive 1, fetches equal polls 1, frame as fetched 1, no gap 1",
                "port 2: filled with 0xff 1, bytes after 34 untouched 1, no fetch for it 1", "stop: main returned"]
            yield f"pads-{flavor}-{label}-the-buffer-is-written-at-each-vblank-after-startpad-and-not-before", verdict((status, with_keys(want, lines, rig)), (0, want))

    # ---- lengths ----
    for rig, flavor, is_linked in rigs:
        status, lines, img, arg = go(rig, f"len-{flavor}", G["g_len"], f"pads-{flavor}")
        want = [f"length {n}: frame whole 1, bytes after it untouched 1, port 2 1, polled {1 if n and is_linked else 0}" for n in (0, 1, 2, 3, 4, 5, 8, 33, 34, 35, 40)]
        yield f"pads-{flavor}-every-length-from-0-to-beyond-the-frame-arrives-whole-and-the-rest-is-untouched", verdict((status, game_lines(lines, G["g_len"])), (0, want))

    # ---- the buffer is checked when it is given ----
    for name, (_, _, _, _, line) in STOPS.items():
        status, lines, img, arg = go(plain, f"stop-{name}", G["g_stop"], f"pads-stop-{name}")
        want = pads_head(img, arg, plain, None, functions=len(FUN_BASE) + 1) + [f"start: 0x{G['g_stop']:08x}", "before", "stop: " + line]
        yield f"initpad-{name}-stops-before-any-write", verdict((status, lines), (9, want))
    status, lines, img, arg = go(plain, "init-ok", G["g_init_ok"], "pads-plain")
    want = ["wild address, length 0: returned 1", "after vblanks", "null buffers with lengths: returned 1", "after vblanks",
            "ends at the last byte: returned 1", "RAM port 1, 3 bytes: frame as fetched 1", "RAM port 2 last byte 0xff 1, the byte before the buffers untouched 1",
            "scratchpad end: returned 1", "scratchpad port 1, 4 bytes: frame as fetched 1", "scratchpad before the buffer untouched 1",
            "stack buffer: returned 1", "stack buffer: frame as fetched 1", "stack buffer: bytes after the frame untouched 1"]
    yield "initpad-served-buffers-length-0-null-ends-at-the-last-byte-and-on-the-stack", verdict((status, game_lines(lines, G["g_init_ok"])), (0, want))
    status, lines, img, arg = go(linked, "init-ok-linked", G["g_init_ok"], "pads-linked")
    yield "initpad-served-buffers-with-the-stand-in-linked", verdict((status, game_lines(lines, G["g_init_ok"])), (0, want))

    # ---- the trace line ----
    trace = linked.work / "pad-trace.txt"
    for label, flags in (("timer", []), ("no-interrupt", ["--no-interrupt"]), ("timer-burst", ["--timer-burst"])):
        status, lines, img, arg = go(linked, f"words-{label}", G["g_words"], "pads-linked", ["--trace", "--trace-file", linked.native(trace), *flags])
        expect = [x[len("expect "):] for x in game_lines(lines, G["g_words"]) if x.startswith("expect ")]
        got = [x for x in trace.read_text().splitlines() if x.startswith("pad frame")] if trace.exists() else []
        parsed = [re.fullmatch(r"pad frame (\d+): 0x([0-9a-f]{4}) -> 0x([0-9a-f]{4})", x) for x in got]
        steps = [(int(m[1]), int(m[2], 16), int(m[3], 16)) for m in parsed if m]
        shape = len(steps) == 3 and [s[1:] for s in steps] == [(0, 0x0840), (0x0840, 0), (0, 0x0080)] and steps[1][0] - steps[0][0] == 2 and steps[2][0] - steps[1][0] == 2
        yield (f"pads-trace-{label}-a-line-when-the-word-changes-also-to-0-for-no-controller",
               None if status == 0 and shape and got == expect and lines[-2:] == ["words done", "stop: main returned"] and not any(x.startswith("pad frame") for x in lines)
               else f"status {status}, trace {got!r}, the game saw {expect!r}")
    status, lines, img, arg = go(linked, "words-notrace", G["g_words"], "pads-linked")
    yield "pads-without-trace-no-pad-frame-line-anywhere", None if status == 0 and not any("pad frame" in x and not x.startswith("expect ") for x in lines) else f"status {status}, lines {lines!r}"

    # ---- the script ----
    for rig, flavor, is_linked in rigs:
        yield from script_cases(rig, is_linked, SCRIPT_1, "one", 80)
        yield from script_cases(rig, is_linked, SCRIPT_2, "repeat", 70, extra=[("timer", [])])
        for name, text, want, until in EXPLICIT:
            yield from explicit_cases(rig, is_linked, name, text, want, until)
        yield from script_cases(rig, is_linked, SCRIPT_3, "layout", 20, extra=[("timer", [])])
    path = linked.work / "none.txt"
    path.write_text(SCRIPT_1)
    status, lines, img, arg = go(linked, "script-none", G["g_script_none"], "pads-linked", ["--input", linked.native(path), "--trace", "--trace-file", linked.native(trace)])
    snaps = snap_lines(lines)
    said = [x for x in lines if x.startswith("pad: port 1")]
    yield ("script-on-a-frame-that-says-no-controller-presses-nothing-and-says-so-once",
           None if status == 0 and snaps and all(s[2:] == (0xff, 0xff, 0xff, 0xff) for s in snaps) and said == ["pad: port 1 reports no controller, so the buttons of the input script are not applied"]
           and not [x for x in trace.read_text().splitlines() if x.startswith("pad frame")] else f"status {status}, said {said!r}, snapshots {snaps[:3]!r}")

    # ---- malformed scripts ----
    entry = G["g_basic"]
    for rig, flavor, is_linked in (rigs[0],):
        def refuse(name: str, data: bytes | None, tail: str | None, line: int | None = None, path_text: str | None = None):
            if data is not None:
                p = rig.work / f"bad-{name}.txt"
                p.write_bytes(data)
                native = rig.native(p)
            else:
                native = path_text or ""
            status, lines, img, arg = go(rig, f"bad-{name}", entry, "pads-plain", ["--input", native])
            want = f"refused: input: {native} line {line}: {tail}" if line else f"refused: input: {native} {tail}"
            return f"script-{name}-is-refused-with-the-file-and-the-line", None if (status, lines) == (2, [want]) else f"status {status}, lines {lines!r}, wanted {want!r}"

        buttons = "start select up down left right cross circle square triangle l1 r1 l2 r2 l3 r3"
        yield refuse("unknown-button", b"# c\n\n0.5 jump down\n", f"`jump` is not a button ({buttons})", 3)
        yield refuse("button-in-capitals", b"0.5 Cross down\n", f"`Cross` is not a button ({buttons})", 1)
        yield refuse("neither-down-nor-up", b"0.5 cross hold\n", "`hold` is neither down nor up", 1)
        yield refuse("too-few-fields", b"0.5 cross\n", "not in the form `SECONDS BUTTON down|up`", 1)
        yield refuse("too-many-fields", b"0.5 cross down now\n", "not in the form `SECONDS BUTTON down|up`", 1)
        yield refuse("comment-after-a-line", b"0.5 cross down # now\n", "not in the form `SECONDS BUTTON down|up`", 1)
        for tag, tok, why in (("letters", "abc", "is not a number (digits and one point)"), ("trailing-letter", "1x", "is not a number (digits and one point)"),
                              ("nan", "nan", "is not a number (digits and one point)"), ("inf", "inf", "is not a number (digits and one point)"),
                              ("hex", "0x10", "is not a number (digits and one point)"), ("exponent", "1e3", "is not a number (digits and one point)"),
                              ("comma", "1,5", "is not a number (digits and one point)"), ("two-points", "1.2.3", "is not a number (digits and one point)"),
                              ("only-a-point", ".", "is not a number (digits and one point)"), ("plus-sign", "+1", "is not a number (digits and one point)"),
                              ("negative", "-1", "is negative"), ("too-large", "1000001", "is more than 1000000 seconds"),
                              ("too-long", "1" * 40, "is too long")):
            yield refuse(f"time-{tag}", f"0.1 l1 down\n{tok} cross down\n".encode(), f"the time `{tok}` {why}", 2)
        yield refuse("time-backwards", b"1.0 cross down\n0.5 cross up\n", "the time `0.5` (frame 30) is earlier than the line before (frame 60)", 2)
        yield refuse("time-backwards-after-rounding", b"0.30 cross down\n0.29 cross up\n", "the time `0.29` (frame 17) is earlier than the line before (frame 18)", 2)
        yield refuse("repeat-too-few-fields", b"repeat 0.5 cross\n", "not in the form `repeat SECONDS EVERY BUTTON`", 1)
        yield refuse("repeat-too-many-fields", b"repeat 0.5 0.2 cross up\n", "not in the form `repeat SECONDS EVERY BUTTON`", 1)
        yield refuse("repeat-interval-too-small", b"repeat 0.5 0.09 cross\n", "the interval `0.09` is less than 0.1 seconds", 1)
        yield refuse("repeat-interval-not-a-number", b"repeat 0.5 often cross\n", "the interval `often` is not a number (digits and one point)", 1)
        yield refuse("repeat-bad-button", b"repeat 0.5 0.2 jump\n", f"`jump` is not a button ({buttons})", 1)
        yield refuse("repeat-time-backwards", b"2 cross down\nrepeat 1 0.2 cross\n", "the time `1` (frame 60) is earlier than the line before (frame 120)", 2)
        yield refuse("too-many-steps", b"repeat 0 0.1 cross\n1000000 cross up\n", "the script would hold more than 1000000 steps", 1)
        yield refuse("line-too-long", b"0.1 cross down\n" + b"#" * 201 + b"\n", "longer than 200 characters", 2)
        yield refuse("nul-byte", b"0.1 cross down\n0.2 cross\x00 up\n", "holds a NUL byte", 2)
        yield refuse("empty-file", b"", "holds no press or release")
        yield refuse("only-comments-and-blanks", b"# nothing\n\n   \n\t\n", "holds no press or release")
        yield refuse("file-too-large", b"#" * 100 + b"\n" + (b"# padding padding padding padding padding padding\n" * 25000), "is longer than 1048576 bytes")
        missing = rig.work / "no-such-script.txt"
        yield refuse("missing-file", None, "cannot be opened", None, rig.native(missing))
        yield refuse("a-folder", None, "cannot be opened", None, rig.native(rig.work))

        # a path so long that the whole message is longer than 256 characters keeps its line number and its reason
        folder = rig.work / ("d" * max(1, 225 - len(rig.native(rig.work)) - 9))
        folder.mkdir()
        (folder / "bad.txt").write_bytes(b"0.1 cross down\n1x cross down\n")
        long_path = rig.native(folder / "bad.txt")
        want_long = f"refused: input: {long_path} line 2: the time `1x` is not a number (digits and one point)"
        status, lines, img, arg = go(rig, "bad-long-path", entry, "pads-plain", ["--input", long_path])
        yield "script-with-a-long-path-keeps-its-line-and-reason-in-the-refusal", None if len(want_long) > 256 and (status, lines) == (2, [want_long]) else f"status {status}, lines {lines!r}, wanted {want_long!r}"

        # accepted: rounding to one frame is not backwards; the layout forms; the largest time
        for name, data in (("same-frame-after-rounding", b"0.3 cross down\n0.2999 cross up\n"), ("largest-time", b"1000000 cross down\n"), ("layout", SCRIPT_3.encode())):
            p = rig.work / f"ok-{name}.txt"
            p.write_bytes(data)
            status, lines, img, arg = go(rig, f"ok-{name}", G["g_basic"], "pads-plain", ["--input", rig.native(p)])
            yield f"script-{name}-is-accepted", None if status == 0 and lines[-1] == "stop: main returned" and not any(x.startswith("refused") for x in lines) else f"status {status}, lines {lines!r}"

        # the options that go with it
        p = rig.work / "ok-layout.txt"
        for name, args in (("twice", ["--input", rig.native(p), "--input", rig.native(p)]), ("no-value", ["--input"]), ("with-list-library", ["--input", rig.native(p), "--list-library"])):
            exe = rig.program_for("pads-plain", program(entry))
            status, out = rig.start(exe, [*args] if name != "twice" else [*args, "x.bin"], 60)
            text = out.replace("\r\n", "\n").splitlines()
            yield f"option-input-{name}-gives-the-usage", None if status == 2 and text and text[0].startswith("usage: ") and "[--input FILE]" in text[0] else f"status {status}, lines {text!r}"

    # ---- the keys line ----
    for rig, flavor, is_linked in rigs:
        status, lines, img, arg = go(rig, f"keys-{flavor}", G["g_basic"], f"pads-{flavor}")
        shown = [x for x in lines if x.startswith("pad:")]
        if not is_linked:
            yield "keys-line-is-not-printed-without-psyz", None if status == 0 and shown == [] else f"status {status}, lines {shown!r}"
        elif keys_line is None:
            print("skip pads-keys-line-names-the-keys-of-psyz-s-table: no --psyz-build")
        else:
            yield "keys-line-names-the-keys-of-psyz-s-table-and-follows-the-overrides-in-c-line", None if status == 0 and shown == [keys_line] and lines[lines.index("overrides in C: 0") + 1] == keys_line else f"status {status}, lines {shown!r}, wanted {keys_line!r}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    parser.add_argument("--src", default=None, help="another copy of the runtime's folder")
    parser.add_argument("--psyz-build", default=None, help="a build folder of psyzbuild.py: its table of keys is read for the keys line")
    parser.add_argument("--only", default="", help="run the cases whose name contains this")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    if args.src:
        L.SRC = Path(args.src).resolve()
    L.BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostpads-", dir=L.BUILD))
    try:
        (work / "plain").mkdir()
        (work / "linked").mkdir()
        plain = PadRig(args.cc, args.run.split(), work / "plain", False)
        linked = PadRig(args.cc, args.run.split(), work / "linked", True)
        problem = plain.start_check()
        if problem:
            print(problem)
            return 2
        failed = 0
        try:
            for name, detail in cases(plain, linked, Path(args.psyz_build) if args.psyz_build else None):
                if args.only and args.only not in name:
                    continue
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    print("\n".join((ACTIVE[0] or plain).report()))
                    failed += 1
        except Exception as err:  # a control must report, not crash
            print(f"FAIL the control itself raised {type(err).__name__}: {err}")
            print("\n".join((ACTIVE[0] or plain).report()))
            failed += 1
        print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
