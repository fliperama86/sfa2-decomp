#!/usr/bin/env python3
"""Controls for the runtime's launch path: the whole program, run for real.

    python3 test_hostlaunch.py --cc CROSS_CC [--run PREFIX]

The test builds the runtime (every port/src/*.c) with the cross compiler CC
and small made-up tables of its own (a handful of functions, some with C that
this file writes, some without, and the SHA-256 of an invented program), then
runs the program on disc images made here from invented bytes. Every case
goes through the real path: map, disc, program, identity, jumps, entry gate,
start. Cases (expected values are worked out here from the fixtures):

- the baseline: the right image, the entry at a registered function without C:
  the five lines, `identity:`, `jumps:`, `start:` and `stop: no C yet`, status 3;
- the entry at a registered function with C: that C runs (it prints a line),
  then `stop: main returned`, status 0;
- an image that differs from the pinned program in one byte, with an x86 `ret`
  at its entry target: refused for its identity, status 2, no `start:`, no
  `stop: main returned`, and no `identity:` line;
- the right image with an entry no table holds, and a `ret` there: refused for
  its entry, status 2, nothing executed;
- an entry inside a registered function but not at its start: refused;
- a library function without a host routine as entry: status 4;
- the library layer on made-up tables of the test's own (its domains.c stands in
  for the runtime's): a library function with a host routine is reached, one
  without still stops; a name listed twice (also once by name and once as
  `@0xADDRESS`) and a listed name that no library function has are refused; an
  override replaces a game function's C, and one for a function that has no C is
  refused; `--trace` writes one line per library call including the one that
  stops; `--list-library` needs no disc;
- the kernel and the threads on game code of the test's own, linked like the
  game's (its calls go to `ps1_NAME` symbols at the PS1 addresses): a handler
  opened with OpenEvent and enabled runs once per vblank while the code waits
  in VSync, interrupts disabled hold it back, TestEvent sees a delivered event;
  three tasks that hand control round with ChangeTh run in the expected order;
  a task that closes itself is gone after the switch and its slot is reused;
  a task whose function returns ends the program with status 8; GetGp returns
  what the entry code loads.

- the disc layer on the linked program: the ready callback that cd.c stores (an
  address in no table is refused before the call, a function without C ends with
  its named stop, a handler in the game's own code runs and no vertical blank is
  delivered inside it); Exec of a program of the disc ends the run with its name
  and sector, or with --skip-programs prints a line and returns; CdGetSector into
  an address outside the RAM ends the run before a byte is copied;

PREFIX is a command prefix to start the Windows program (for example a
compatibility layer); without it the program is started directly (on Windows,
or from a shell under WSL, where a path is converted with wslpath). When the
program cannot be started this says so in one line and ends with status 2, as
it does without the compiler. The run's files live in a folder of its own
under port/build/ which only this run removes.
"""

from __future__ import annotations

import argparse
import hashlib
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
from test_hostrun import CNF, Image  # noqa: E402

SRC = HERE.parent / "src"
BUILD = HERE.parent / "build"
RAM = 0x80000000
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]
# the runtime's files that a test builds; domains.c is the test's own
RUNTIME = ["main", "memory", "disc", "jumps", "sha256", "library", "kernel", "threads", "overrides", "clib", "sound", "card", "cd", "modules", "debug", "interrupt"]

T_ADDR = RAM + 0x100000
T_SIZE = 0x2000
PC0 = RAM + 0x100800
B_ADDR = RAM + 0x110000
B_SIZE = 16
GP = RAM + 0x103904
ENTRY_ABSENT = RAM + 0x100020     # a registered function without C
ENTRY_LIBRARY = RAM + 0x100040    # a registered library function without a host routine
ENTRY_C = RAM + 0x100080          # a registered function with C
BIG_C = RAM + 0x100100            # another one with C
UNREGISTERED = RAM + 0x101000     # in the program's text, in no table; holds an x86 ret

LIB_A, LIB_B, LIB_C = RAM + 0x100200, RAM + 0x100210, RAM + 0x100220
G_OVER, G_MECH = RAM + 0x100230, RAM + 0x100240
K = {name: RAM + 0x100300 + 0x10 * i for i, name in enumerate(
    ["OpenEvent", "EnableEvent", "TestEvent", "VSync", "ResetCallback", "StartRCnt", "EnterCriticalSection",
     "ExitCriticalSection", "OpenTh", "ChangeTh", "CloseTh", "GetGp", "CloseEvent", "InterruptCallback", "VSyncCallback"])}
G_CRASH = RAM + 0x100520
G_VBLANK, G_THREADS, G_THREAD_RET, T1, T2, T3 = (RAM + 0x100400 + 0x10 * i for i in range(6))


def jal(target: int) -> int:
    return 0x0C000000 | ((target & 0x0FFFFFFF) >> 2)


def program(entry: int, flip: tuple[int, int] | None = None) -> bytes:
    """The made-up PS-X EXE: entry code at PC0 (lui gp, addiu gp, nop, jal ENTRY, nop, break), a ret byte
    at the unregistered address and 4 bytes into the absent function (the byte its jump overwrites)."""
    text = bytearray(T_SIZE)
    for k, w in enumerate((0x3C1C0000 | (GP >> 16), 0x279C0000 | (GP & 0xFFFF), 0, jal(entry), 0, 0x0000000D)):
        struct.pack_into("<I", text, PC0 - T_ADDR + 4 * k, w)
    text[UNREGISTERED - T_ADDR] = 0xC3
    text[ENTRY_ABSENT - T_ADDR + 4] = 0xC3
    header = bytearray(0x800)
    header[0:8] = b"PS-X EXE"
    struct.pack_into("<IIIII", header, 0x10, PC0, 0, T_ADDR, T_SIZE, 0)
    struct.pack_into("<II", header, 0x28, B_ADDR, B_SIZE)
    data = bytearray(header + text)
    if flip:
        data[flip[0]] ^= flip[1]
    return bytes(data)


def sha_bytes(data: bytes) -> str:
    return ", ".join(f"0x{b:02x}" for b in hashlib.sha256(data).digest())


# ---- the test's own game code, library names and tables ----

PROLOGUE = r"""
#include <stdio.h>
#include <windows.h>
#define SAY(...) do { printf(__VA_ARGS__); fflush(stdout); } while (0)
"""

GAME_BASE = PROLOGUE

GAME_MECH = PROLOGUE + r"""
extern int ps1_lib_a(int);
extern int ps1_lib_b(int);
extern int ps1_lib_c(int);
extern void ps1_game_over(void);
int host_a(int x) { SAY("host a ran %d\n", x); return x + 1; }
int host_c(int x) { SAY("host c ran %d\n", x); return x + 2; }
void game_over(void) { SAY("original C ran\n"); }
void over_host(void) { SAY("override ran\n"); }
void game_mech(void)
{
    SAY("a returned %d\n", ps1_lib_a(5));
    SAY("c returned %d\n", ps1_lib_c(7));
    ps1_game_over();
    ps1_lib_b(1);
    SAY("not reached\n");
}
"""

GAME_VBLANK = PROLOGUE + r"""
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_EnableEvent(unsigned), ps1_TestEvent(unsigned), ps1_VSync(int), ps1_StartRCnt(unsigned);
extern int ps1_EnterCriticalSection(void), ps1_ExitCriticalSection(void), ps1_CloseEvent(unsigned);
extern void *ps1_ResetCallback(void);
static volatile int count;
static void handler(void) { count++; }
void game_vblank(void)
{
    unsigned ev, ev2;
    int i, v, c, was;
    DWORD t0;
    SAY("reset %s\n", ps1_ResetCallback() ? "first" : "again");
    SAY("reset %s\n", ps1_ResetCallback() ? "first" : "again");
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, handler);
    SAY("event %08x enable %d\n", ev, ps1_EnableEvent(ev));
    ps1_StartRCnt(3);
    t0 = GetTickCount();
    for (i = 0; i < 5; i++) ps1_VSync(0);
    t0 = GetTickCount() - t0;
    v = ps1_VSync(-1);
    c = count;
    SAY("five waits: vblanks %d handler %d slow %d fast %d\n", v, c, t0 < 50, t0 > 1000);
    was = ps1_EnterCriticalSection();
    c = count;
    for (i = 0; i < 3; i++) ps1_VSync(0);
    SAY("enter returned %d; in the critical section the handler ran %d times\n", was, count - c);
    SAY("exit; enter again returned %d\n", ps1_EnterCriticalSection());
    ps1_ExitCriticalSection();
    c = count;
    ps1_VSync(0);
    SAY("after exit the handler ran %d times in one wait\n", count - c);
    ev2 = ps1_OpenEvent(0xf2000003u, 2, 0x2000, 0);
    ps1_EnableEvent(ev2);
    for (i = 0; i < 100000 && !ps1_TestEvent(ev2); i++) {
    }
    SAY("polled event ready after %d polls: %d\n", i, i < 100000);
    i = ps1_CloseEvent(ev);
    SAY("close %d %d\n", i, ps1_CloseEvent(ev));
    c = count;
    ps1_VSync(0);
    SAY("closed handler ran %d times\n", count - c);
}
"""

GAME_THREADS = PROLOGUE + r"""
extern unsigned ps1_OpenTh(unsigned, unsigned, unsigned), ps1_GetGp(void);
extern int ps1_ChangeTh(unsigned), ps1_CloseTh(unsigned);
static unsigned hm = 0xff000000u, h1, h2, h3;
void t1(void)
{
    SAY("t1-a\n"); ps1_ChangeTh(h2);
    SAY("t1-b\n"); ps1_ChangeTh(hm);
    SAY("t1-c\n"); ps1_ChangeTh(hm);
    SAY("t1 never\n");
}
void t2(void)
{
    SAY("t2-a\n"); ps1_ChangeTh(h3);
    SAY("t2-b\n"); ps1_ChangeTh(h1);
    SAY("t2 never\n");
}
void t3(void)
{
    SAY("t3-a\n"); ps1_ChangeTh(h1);
    SAY("t3-b\n");
    SAY("t3 closes itself: %d\n", ps1_CloseTh(h3));
    ps1_ChangeTh(hm);
    SAY("t3-after (must not appear)\n");
}
void game_threads(void)
{
    unsigned again;
    int i;
    SAY("gp %08x\n", ps1_GetGp());
    h1 = ps1_OpenTh((unsigned)(size_t)t1, 0x801fec00u, 0);
    h2 = ps1_OpenTh((unsigned)(size_t)t2, 0x801ff400u, 0);
    h3 = ps1_OpenTh((unsigned)(size_t)t3, 0x801ffc00u, 0);
    again = ps1_OpenTh((unsigned)(size_t)t1, 0, 0);
    SAY("handles %08x %08x %08x fourth %08x\n", h1, h2, h3, again);
    ps1_ChangeTh(h1);
    SAY("main-1\n");
    ps1_ChangeTh(h2);
    SAY("main-2\n");
    i = ps1_CloseTh(h1);
    SAY("close t1 from outside %d; change to it %d\n", i, ps1_ChangeTh(h1));
    ps1_ChangeTh(h3);
    SAY("main-3\n");
    again = ps1_OpenTh((unsigned)(size_t)t1, 0, 0);
    i = ps1_ChangeTh(h3);
    SAY("reopened %08x; change to the closed t3 %d; close the main thread %d\n", again, i, ps1_CloseTh(hm));
}
void thread_returns(void) { SAY("thread function ran and returns\n"); }
void game_thread_ret(void)
{
    unsigned h = ps1_OpenTh((unsigned)(size_t)thread_returns, 0, 0);
    ps1_ChangeTh(h);
    SAY("not reached\n");
}
"""

GAME_CRASH = PROLOGUE + r"""
void game_crash(void)
{
    volatile unsigned *p = (volatile unsigned *)0x1000;
    SAY("about to read the mirror\n");
    SAY("read %u\n", *p);
}
"""

# The addresses that the game hands to the runtime to call: an unregistered one in the program's text (an x86
# ret lies there) and a registered function without C. One game function for each path that calls an address.
G_TARGET = {name: RAM + 0x101500 + 0x10 * i for i, name in enumerate(["thread", "event", "irq", "vsync", "valid", "wipe"])}


def game_targets(target: int) -> str:
    return PROLOGUE + f"""
extern unsigned ps1_OpenTh(unsigned, unsigned, unsigned), ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_ChangeTh(unsigned), ps1_EnableEvent(unsigned), ps1_VSync(int), ps1_StartRCnt(unsigned);
extern void *ps1_ResetCallback(void), *ps1_InterruptCallback(int, void (*)(void));
extern void ps1_VSyncCallback(void (*)(void));
static volatile int count;
static void handler(void) {{ count++; }}
void g_thread(void)
{{
    unsigned h = ps1_OpenTh(0x{target:08x}u, 0, 0);
    SAY("thread opened\\n");
    ps1_ChangeTh(h);
    SAY("after the switch\\n");
}}
void g_event(void)
{{
    unsigned ev;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, (void (*)(void))0x{target:08x}u);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
    SAY("event armed\\n");
    ps1_VSync(0);
    SAY("after the vblank\\n");
}}
void g_irq(void)
{{
    ps1_ResetCallback();
    ps1_InterruptCallback(0, (void (*)(void))0x{target:08x}u);
    SAY("interrupt callback set\\n");
    ps1_VSync(0);
    SAY("after the vblank\\n");
}}
void g_vsync(void)
{{
    ps1_ResetCallback();
    ps1_VSyncCallback((void (*)(void))0x{target:08x}u);
    SAY("vsync callback set\\n");
    ps1_VSync(0);
    SAY("after the vblank\\n");
}}
void g_wipe(void)
{{
    unsigned h;
    *(volatile unsigned *)0x{ENTRY_C:08x}u = 0x90909090u;   /* the game's own write over a resident entry's jump */
    h = ps1_OpenTh(0x{ENTRY_C:08x}u, 0, 0);
    SAY("overwrote the jump\\n");
    ps1_ChangeTh(h);
    SAY("not reached\\n");
}}
void g_valid(void)
{{
    unsigned ev;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, handler);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
    ps1_VSync(0);
    SAY("event handler ran %d\\n", count > 0);
    count = 0;
    ps1_InterruptCallback(0, handler);
    ps1_VSync(0);
    SAY("interrupt callback ran %d\\n", count > 0);
    ps1_InterruptCallback(0, 0);
    count = 0;
    ps1_VSyncCallback(handler);
    ps1_VSync(0);
    SAY("vsync callback ran %d\\n", count > 0);
}}
"""


# The disc layer on the linked program: the ready callback that cd.c stores and calls, Exec, and the buffer of CdGetSector.
CD_NAMES = ["CdInit", "CdSync", "CdReady", "CdControl", "CdControlF", "CdControlB", "CdMix", "CdGetSector", "CdIntToPos", "CdPosToInt", "Exec", "FlushCache"]
CD_ADDR = {n: RAM + 0x101600 + 0x10 * i for i, n in enumerate(CD_NAMES)}
G_CD = {n: RAM + 0x101700 + 0x10 * i for i, n in enumerate(["ready", "exec", "exec_unknown", "getsector"])}
EXEC_SECTOR = 23     # SLPS_004.15 in every image the rig writes: the directory at 20, SYSTEM.CNF at 22


def game_cd(target: str) -> str:
    return PROLOGUE + f"""
extern int ps1_CdInit(void), ps1_CdControl(int, unsigned char *, unsigned char *), ps1_CdControlB(int, unsigned char *, unsigned char *);
extern int ps1_CdReady(int, unsigned char *), ps1_CdGetSector(void *, int), ps1_Exec(void *, int, char **);
extern unsigned char *ps1_CdIntToPos(int, unsigned char *);
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_EnableEvent(unsigned), ps1_VSync(int), ps1_StartRCnt(unsigned);
extern void *ps1_ResetCallback(void);
#define READY_CB (*(volatile unsigned *)0x80181b38u)
static volatile int in_ready, seen_inside, vblanks, ready_calls;
static void ready_handler(int intr, unsigned char *res)
{{
    DWORD t0;
    (void)res;
    ready_calls++;
    in_ready = 1;
    t0 = GetTickCount();
    while (GetTickCount() - t0 < 60) {{ }}
    if (intr == 1) ps1_CdGetSector((void *)0x80140000u, 0x200);
    in_ready = 0;
}}
static void on_vblank(void) {{ vblanks++; if (in_ready) seen_inside++; }}
static void start_read(int sector)
{{
    unsigned char pos[4], res[8];
    ps1_CdInit();
    ps1_CdIntToPos(sector, pos);
    ps1_CdControlB(2, pos, res);
    ps1_CdControl(6, 0, res);
}}
void g_ready(void)
{{
    unsigned char res[8];
    unsigned ev;
    int i;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, on_vblank);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
    start_read(16);
    READY_CB = (unsigned)(size_t)({target});
    SAY("handler set\\n");
    ps1_CdReady(0, res);
    for (i = 0; i < 6; i++) ps1_VSync(0);
    SAY("after the vblanks\\n");
    SAY("ready handler ran %d, vblank handlers inside it %d, vblanks %d\\n", ready_calls > 0, seen_inside, vblanks > 3);
}}
void g_exec(void)
{{
    unsigned char res[8];
    start_read({EXEC_SECTOR});
    ps1_CdReady(0, res);
    ps1_CdGetSector((void *)0x801e0000u, 0x200);
    SAY("sector read\\n");
    ps1_Exec((void *)0x801e0000u, 0, 0);
    SAY("after exec\\n");
}}
void g_exec_unknown(void)
{{
    ps1_Exec((void *)0x80150000u, 0, 0);
    SAY("after exec\\n");
}}
void g_getsector(void)
{{
    unsigned char res[8];
    start_read(16);
    ps1_CdReady(0, res);
    SAY("before\\n");
    ps1_CdGetSector((void *)0x1000u, 0x200);
    SAY("after\\n");
}}
"""


# name, address, library, C symbol (None: no C)
ABS_BASE = [("func_%08x" % ENTRY_ABSENT, ENTRY_ABSENT, 0), ("memcpy_like", ENTRY_LIBRARY, 1)]
FUN_BASE = [("c_main", ENTRY_C, "c_main"), ("c_big", BIG_C, "c_big")]
ABS_MECH = ABS_BASE + [("lib_a", LIB_A, 1), ("lib_b", LIB_B, 1), ("lib_c", LIB_C, 1)]
FUN_MECH = FUN_BASE + [("game_over", G_OVER, "game_over"), ("game_mech", G_MECH, "game_mech")]
ABS_KERN = ABS_BASE + [(n, a, 1) for n, a in K.items()]
FUN_KERN = FUN_BASE + [("game_vblank", G_VBLANK, "game_vblank"), ("game_threads", G_THREADS, "game_threads"),
                       ("game_thread_ret", G_THREAD_RET, "game_thread_ret"),
                       ("t1", T1, "t1"), ("t2", T2, "t2"), ("t3", T3, "t3"), ("thread_returns", RAM + 0x100460, "thread_returns")]

FUN_CRASH = FUN_BASE + [("game_crash", G_CRASH, "game_crash")]
ABS_CD = ABS_KERN + [(n, a, 1) for n, a in CD_ADDR.items()]
FUN_CD = FUN_BASE + [(f"g_{n}", a, f"g_{n}") for n, a in G_CD.items()]
FUN_TARGET = FUN_BASE + [(f"g_{n}", a, f"g_{n}") for n, a in G_TARGET.items()]

CARD_NAMES = ["InitCARD", "StartCARD", "_bu_init", "_card_info", "_card_load", "@card_clear", "open", "read", "write", "close", "firstfile", "nextfile", "format"]
CARD_ADDR = {n: RAM + 0x100600 + 0x10 * i for i, n in enumerate(CARD_NAMES + ["OpenEvent", "EnableEvent", "TestEvent"])}
G_CARD = RAM + 0x100700
CARD_ADDR["@card_clear"] = 0x8016D8A0
ABS_CARD = ABS_BASE + [(("func_8016d8a0" if n == "@card_clear" else n), a, 1) for n, a in CARD_ADDR.items()]
FUN_CARD = FUN_BASE + [("game_card", G_CARD, "game_card")]
GAME_CARD = PROLOGUE + r"""
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void *);
extern int ps1_EnableEvent(unsigned), ps1_TestEvent(unsigned);
extern long ps1__card_info(long), ps1__card_load(long), ps1_func_8016d8a0(long);
extern int ps1_open(const char *, int), ps1_read(int, void *, int), ps1_firstfile(const char *, void *), ps1_format(const char *);
void game_card(void)
{
    unsigned p[4];
    static const unsigned spec[4] = { 4, 0x8000, 0x100, 0x2000 };
    int i, k, got = -1;
    for (i = 0; i < 4; i++) { p[i] = ps1_OpenEvent(0xf4000001u, spec[i], 0x2000, 0); ps1_EnableEvent(p[i]); }
    SAY("polling before the request: %d\n", ps1_TestEvent(p[2]));
    SAY("info %ld\n", ps1__card_info(0));
    for (k = 0; k < 4; k++) if (ps1_TestEvent(p[k])) { got = k; break; }
    SAY("first ready event index %d\n", got);
    SAY("again %d\n", ps1_TestEvent(p[2]));
    SAY("load %ld clear %ld\n", ps1__card_load(1), ps1_func_8016d8a0(0));
    SAY("ready after load: %d\n", ps1_TestEvent(p[2]));
    SAY("open %d read %d firstfile %d format %d\n", ps1_open("bu00:x", 1), ps1_read(0, 0, 0), ps1_firstfile("bu00:*", 0), ps1_format("bu00:"));
}
"""

IRQ_NAMES = ["OpenEvent", "EnableEvent", "StartRCnt", "ResetCallback", "busy"]
IRQ_ADDR = {n: RAM + 0x101200 + 0x10 * i for i, n in enumerate(IRQ_NAMES)}
G_SPIN, G_HOST, G_NEST, G_REGS, G_FPU = (RAM + 0x101300 + 0x10 * i for i in range(5))
ABS_IRQ = ABS_BASE + [(n, a, 1) for n, a in IRQ_ADDR.items()]
FUN_IRQ = FUN_BASE + [("g_spin", G_SPIN, "g_spin"), ("g_host", G_HOST, "g_host"), ("g_nest", G_NEST, "g_nest"), ("g_regs", G_REGS, "g_regs"), ("g_fpu", G_FPU, "g_fpu")]
GAME_IRQ = PROLOGUE + r"""
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_EnableEvent(unsigned), ps1_StartRCnt(unsigned);
extern void *ps1_ResetCallback(void);
extern void ps1_busy(void);
static volatile int count, depth, maxdepth, slow;
static volatile DWORD handler_thread;
static void handler(void)
{
    DWORD t;
    handler_thread = GetCurrentThreadId();
    depth++;
    if (depth > maxdepth) maxdepth = depth;
    if (slow) { t = GetTickCount(); while (GetTickCount() - t < 45) { } }
    { volatile double d = 3.25 * (double)count; (void)d; }   /* x87 work inside the handler */
    depth--;
    count++;
}
static void setup_with(void (*h)(void))
{
    unsigned ev;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, h);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
}
static void setup(void) { setup_with(handler); }
void g_spin(void)
{
    DWORD me = GetCurrentThreadId();
    setup();
    while (count < 5) { }
    SAY("spin ended; the handler ran on the game's thread: %d\n", handler_thread == me);
}
void g_host(void)
{
    int before, during;
    setup();
    while (count < 2) { }
    before = count;
    ps1_busy();
    during = count - before;
    SAY("count moved %d during the host routine's 200 ms spin\n", during);
    while (count < before + 2) { }
    SAY("and moved after it: %d\n", count > before);
}
void g_nest(void)
{
    setup();
    slow = 1;
    while (count < 4) { }
    SAY("handler runs %d, deepest nesting %d\n", count >= 4, maxdepth);
}
static volatile int iterations_left, failed;
void g_regs(void)
{
    int round, start;
    double a = 0, b = 0;
    setup();
    start = count;
    for (round = 0; round < 12; round++) {
        iterations_left = 3000000;
        __asm__ volatile(
            "fld1\n\tfldpi\n\t"
            "movl $0x11111111, %%eax\n\tmovl $0x22222222, %%ebx\n\tmovl $0x33333333, %%ecx\n"
            "\tmovl $0x44444444, %%edx\n\tmovl $0x55555555, %%esi\n\tmovl $0x66666666, %%edi\n"
            "1:\n\tstc\n\tnop\n\tnop\n\tnop\n\tjnc 9f\n"
            "\tcmpl $0x11111111, %%eax\n\tjne 9f\n\tcmpl $0x22222222, %%ebx\n\tjne 9f\n\tcmpl $0x33333333, %%ecx\n\tjne 9f\n"
            "\tcmpl $0x44444444, %%edx\n\tjne 9f\n\tcmpl $0x55555555, %%esi\n\tjne 9f\n\tcmpl $0x66666666, %%edi\n\tjne 9f\n"
            "\tstd\n\tnop\n\tnop\n\tcld\n"
            "\tdecl %0\n\tjnz 1b\n\tjmp 8f\n"
            "9:\n\tmovl $1, %1\n"
            "8:\n\tfstpl %2\n\tfstpl %3\n"
            : "+m"(iterations_left), "=m"(failed), "=m"(a), "=m"(b)
            :
            : "eax", "ebx", "ecx", "edx", "esi", "edi", "cc", "memory");
        if (a != 3.14159265358979323846 || b != 1.0) failed = 1;
        if (failed) break;
    }
    SAY("registers, flags and x87 survived: %d; handler ran %d times meanwhile: %d\n", !failed, count - start, count - start >= 10);
}

#include <emmintrin.h>
/* ---- the floating-point environment of the handler, and of the interrupted code ---- */
static volatile int fcount, f_cw_bad, f_tag_bad, f_mx_bad, f_df_bad, f_x87_bad, f_sse_bad;
__attribute__((target("sse2"))) static double sse_mul(double a, double b) { return _mm_cvtsd_f64(_mm_mul_sd(_mm_set_sd(a), _mm_set_sd(b))); }
static void fhandler(void)
{
    unsigned char env[28];
    unsigned short cw, tag;
    unsigned mx, fl;
    int n = fcount;
    double d, s;
    __asm__ volatile("fnstenv %0" : "=m"(env));
    __asm__ volatile("stmxcsr %0" : "=m"(mx));
    __asm__ volatile("pushfl\n\tpopl %0" : "=r"(fl));
    cw = *(unsigned short *)env;
    tag = *(unsigned short *)(env + 8);
    if (cw != 0x037f) f_cw_bad++;
    if (tag != 0xffff) f_tag_bad++;
    if (mx != 0x1f80) f_mx_bad++;
    if (fl & 0x400) f_df_bad++;
    d = 3.25 * (double)n;
    s = sse_mul(3.25, (double)n);
    if ((int)(d * 4.0) != 13 * n) f_x87_bad++;
    if ((int)(s * 4.0) != 13 * n) f_sse_bad++;
    fcount++;
}
static const double fvals[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static double fout[8];
static volatile unsigned f_out_cw, f_out_mx, f_out_df;
void g_fpu(void)
{
    static const unsigned short cw_set = 0x0c7f, cw_def = 0x037f;
    static const unsigned mx_set = 0x7f80, mx_def = 0x1f80;
    int round, i, values_ok = 1, cw_ok = 1, mx_ok = 1, df_ok = 1, start;
    unsigned n;
    setup_with(fhandler);
    start = fcount;
    for (round = 0; round < 8; round++) {
        n = 600000000u;
        __asm__ volatile(
            "fldcw %[cws]\n\tldmxcsr %[mxs]\n\t"
            "fldl 0(%[v])\n\tfldl 8(%[v])\n\tfldl 16(%[v])\n\tfldl 24(%[v])\n\tfldl 32(%[v])\n\tfldl 40(%[v])\n\tfldl 48(%[v])\n\tfldl 56(%[v])\n\t"
            "std\n"
            "1:\n\tnop\n\tnop\n\tdecl %[n]\n\tjnz 1b\n\t"
            "pushfl\n\tpopl %%eax\n\tshrl $10, %%eax\n\tandl $1, %%eax\n\tmovl %%eax, %[df]\n\t"
            "cld\n\t"
            "stmxcsr %[omx]\n\tfnstcw %[ocw]\n\t"
            "fstpl 0(%[o])\n\tfstpl 8(%[o])\n\tfstpl 16(%[o])\n\tfstpl 24(%[o])\n\tfstpl 32(%[o])\n\tfstpl 40(%[o])\n\tfstpl 48(%[o])\n\tfstpl 56(%[o])\n\t"
            "fldcw %[cwd]\n\tldmxcsr %[mxd]\n"
            : [n] "+c"(n), [df] "=m"(f_out_df), [omx] "=m"(f_out_mx), [ocw] "=m"(f_out_cw)
            : [v] "r"(fvals), [o] "r"(fout), [cws] "m"(cw_set), [mxs] "m"(mx_set), [cwd] "m"(cw_def), [mxd] "m"(mx_def)
            : "eax", "cc", "memory", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
        for (i = 0; i < 8; i++)
            if (fout[i] != fvals[7 - i]) values_ok = 0;
        if (f_out_cw != 0x0c7f) cw_ok = 0;
        if (f_out_mx != 0x7f80) mx_ok = 0;
        if (f_out_df != 1) df_ok = 0;
    }
    SAY("interrupted state back: values %d control word %d mxcsr %d direction flag %d\n", values_ok, cw_ok, mx_ok, df_ok);
    SAY("handler saw: default control word %d empty x87 stack %d default mxcsr %d direction flag clear %d\n", !f_cw_bad, !f_tag_bad, !f_mx_bad, !f_df_bad);
    SAY("handler arithmetic: x87 %d sse %d; handler ran %d times: %d\n", !f_x87_bad, !f_sse_bad, fcount - start, fcount - start >= 10);
}
"""

IRQ_DOMAINS = r"""
#include "port.h"
#include <windows.h>
extern void port_h_OpenEvent(), port_h_EnableEvent(), port_h_StartRCnt(), port_h_ResetCallback();
void host_busy(void) { DWORD t = GetTickCount(); while (GetTickCount() - t < 200) { } }
static const struct port_library t[] = {
    { "OpenEvent", (void *)port_h_OpenEvent, 0 }, { "EnableEvent", (void *)port_h_EnableEvent, 0 },
    { "StartRCnt", (void *)port_h_StartRCnt, 0 }, { "ResetCallback", (void *)port_h_ResetCallback, 0 },
    { "busy", (void *)host_busy, 0 }, { 0, 0, 0 } };
const struct port_domain port_domains[] = { { "irq", t } };
const unsigned port_domain_count = 1;
static const struct port_override o[] = { { 0, 0, 0 } };
const struct port_override *const port_override_sets[] = { o };
const unsigned port_override_set_count = 1;
"""

DOMAINS_NONE = r"""
#include "port.h"
static const struct port_library none[] = { { 0, 0, 0 } };
const struct port_domain port_domains[] = { { "none", none } };
const unsigned port_domain_count = 0;
static const struct port_override no_overrides[] = { { 0, 0, 0 } };
const struct port_override *const port_override_sets[] = { no_overrides };
const unsigned port_override_set_count = 1;
"""


def domains(table: str, overrides: str = "") -> str:
    return f"""
#include "port.h"
extern int host_a(int), host_c(int);
extern void over_host(void);
static const struct port_library t[] = {{ {table} {{ 0, 0, 0 }} }};
const struct port_domain port_domains[] = {{ {{ "test", t }} }};
const unsigned port_domain_count = 1;
static const struct port_override o[] = {{ {overrides} {{ 0, 0, 0 }} }};
const struct port_override *const port_override_sets[] = {{ o }};
const unsigned port_override_set_count = 1;
"""


KERN_ROUTINES = {
    "OpenEvent": "unsigned port_h_OpenEvent(); ", "EnableEvent": "", "TestEvent": "", "VSync": "", "ResetCallback": "",
    "StartRCnt": "", "EnterCriticalSection": "", "ExitCriticalSection": "", "OpenTh": "", "ChangeTh": "", "CloseTh": "", "GetGp": "", "CloseEvent": "",
}
# the kernel's real routines under the names the made-up absents have; StartRCnt is func_80157720 in the real build
HOSTS = {"OpenEvent": "port_h_OpenEvent", "EnableEvent": "port_h_EnableEvent", "TestEvent": "port_h_TestEvent",
         "VSync": "port_h_VSync", "ResetCallback": "port_h_ResetCallback", "StartRCnt": "port_h_StartRCnt",
         "EnterCriticalSection": "port_h_EnterCriticalSection", "ExitCriticalSection": "port_h_ExitCriticalSection",
         "OpenTh": "port_h_OpenTh", "ChangeTh": "port_h_ChangeTh", "CloseTh": "port_h_CloseTh", "GetGp": "port_h_GetGp",
         "CloseEvent": "port_h_CloseEvent", "InterruptCallback": "port_h_InterruptCallback", "VSyncCallback": "port_h_VSyncCallback"}


def kernel_domains() -> str:
    decls = "".join(f"extern void {h}();\n" for h in HOSTS.values())
    rows = "".join(f'{{ "{n}", (void *){h}, 0 }}, ' for n, h in HOSTS.items())
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


def cd_domains() -> str:
    decls = "".join(f"extern void {h}();\n" for h in HOSTS.values())
    rows = "".join(f'{{ "{n}", (void *){h}, 0 }}, ' for n, h in HOSTS.items())
    return f"""
#include "port.h"
{decls}
static const struct port_library t[] = {{ {rows} {{ 0, 0, 0 }} }};
const struct port_domain port_domains[] = {{ {{ "kernel", t }}, {{ "cd", port_cd_library }}, {{ "system", port_system_library }} }};
const unsigned port_domain_count = 3;
static const struct port_override o[] = {{ {{ 0, 0, 0 }} }};
const struct port_override *const port_override_sets[] = {{ o }};
const unsigned port_override_set_count = 1;
"""


def tables_c(functions, absents, pin: bytes) -> str:
    out = ['#include "port_tables.h"']
    for _, _, sym in functions:
        out.append(f"extern void {sym}(void);")
    out.append('const struct port_image port_images[] = {{ "mod", 0x80180000u, 0, 1, 0, 0 }};')
    out.append("const unsigned port_image_count = 1;")
    out.append("const struct port_function port_functions[] = {")
    for name, addr, sym in functions:
        out.append(f'    {{ 0x{addr:08x}u, (void *){sym}, "{name}", -1 }},')
    out.append("};")
    out.append(f"const unsigned port_function_count = {len(functions)};")
    out.append("const struct port_absent port_absents[] = {")
    for name, addr, lib in absents:
        out.append(f'    {{ 0x{addr:08x}u, "{name}", -1, {lib} }},')
    out.append("};")
    out.append(f"const unsigned port_absent_count = {len(absents)};")
    out.append(f"const unsigned char port_program_sha256[32] = {{ {sha_bytes(pin)} }};")
    return "\n".join(out) + "\n"


# the build's two marker symbols, here around the made-up game objects
MARK_BEGIN = '__asm__(".text\\n.globl _port_game_text_begin\\n_port_game_text_begin:\\n\\tnop\\n");\n'
MARK_END = '__asm__(".text\\n\\tnop\\n.globl _port_game_text_end\\n_port_game_text_end:\\n\\tnop\\n");\n'


class Variant:
    def __init__(self, name, game, functions, absents, domains_c):
        self.name, self.game, self.functions, self.absents, self.domains_c = name, game, functions, absents, domains_c


VARIANTS = {
    "base": Variant("base", GAME_BASE, FUN_BASE, ABS_BASE, DOMAINS_NONE),
    "mech": Variant("mech", GAME_MECH, FUN_MECH, ABS_MECH,
                    domains('{ "lib_a", (void *)host_a, 0 }, { "@0x%08x", (void *)host_c, 0 },' % LIB_C, '{ "game_over", (void *)over_host, "the test replaces it" },')),
    "mech-noover": Variant("mech-noover", GAME_MECH, FUN_MECH, ABS_MECH, domains('{ "lib_a", (void *)host_a, 0 }, { "@0x%08x", (void *)host_c, 0 },' % LIB_C)),
    "dup": Variant("dup", GAME_MECH, FUN_MECH, ABS_MECH, domains('{ "lib_a", (void *)host_a, 0 }, { "lib_a", (void *)host_c, 0 },')),
    "dup-address": Variant("dup-address", GAME_MECH, FUN_MECH, ABS_MECH,
                           domains('{ "lib_a", (void *)host_a, 0 }, { "@0x%08x", (void *)host_c, 0 },' % LIB_A)),
    "unknown": Variant("unknown", GAME_MECH, FUN_MECH, ABS_MECH, domains('{ "lib_a", (void *)host_a, 0 }, { "no_such_function", (void *)host_c, 0 },')),
    "unknown-address": Variant("unknown-address", GAME_MECH, FUN_MECH, ABS_MECH, domains('{ "@0x80100999", (void *)host_c, 0 },')),
    "over-unknown": Variant("over-unknown", GAME_MECH, FUN_MECH, ABS_MECH, domains("", '{ "lib_a", (void *)over_host, "a library function has no C to replace" },')),
    "crash": Variant("crash", GAME_CRASH, FUN_CRASH, ABS_BASE, DOMAINS_NONE),
    "tgt-unreg": Variant("tgt-unreg", game_targets(UNREGISTERED), FUN_TARGET, ABS_KERN, kernel_domains()),
    "tgt-absent": Variant("tgt-absent", game_targets(ENTRY_ABSENT), FUN_TARGET, ABS_KERN, kernel_domains()),
    "card": Variant("card", GAME_CARD, FUN_CARD, ABS_CARD,
                    "#include \"port.h\"\nextern void port_h_OpenEvent(), port_h_EnableEvent(), port_h_TestEvent();\n"
                    "static const struct port_library k[] = { { \"OpenEvent\", (void *)port_h_OpenEvent, 0 }, { \"EnableEvent\", (void *)port_h_EnableEvent, 0 }, { \"TestEvent\", (void *)port_h_TestEvent, 0 }, { 0, 0, 0 } };\n"
                    "const struct port_domain port_domains[] = { { \"kernel\", k }, { \"card\", port_card_library } };\nconst unsigned port_domain_count = 2;\n"
                    "static const struct port_override o[] = { { 0, 0, 0 } };\n"
                    "const struct port_override *const port_override_sets[] = { o };\nconst unsigned port_override_set_count = 1;\n"),
    "irq": Variant("irq", GAME_IRQ, FUN_IRQ, ABS_IRQ, IRQ_DOMAINS),
    "cd-unreg": Variant("cd-unreg", game_cd(f"0x{UNREGISTERED:08x}u"), FUN_CD, ABS_CD, cd_domains()),
    "cd-absent": Variant("cd-absent", game_cd(f"0x{ENTRY_ABSENT:08x}u"), FUN_CD, ABS_CD, cd_domains()),
    "cd-valid": Variant("cd-valid", game_cd("ready_handler"), FUN_CD, ABS_CD, cd_domains()),
    "kern": Variant("kern", GAME_VBLANK + GAME_THREADS.replace(PROLOGUE, ""), FUN_KERN, ABS_KERN, kernel_domains()),
}

STUBS = r"""
#include <stdio.h>
void c_main(void) { puts("C main ran"); fflush(stdout); }
void c_big(void) { puts("C big ran"); fflush(stdout); }
"""


class Rig:
    def __init__(self, cc: str, prefix: list[str], work: Path):
        self.cc, self.prefix, self.work = cc, prefix, work
        self.wsl = not prefix and shutil.which("wslpath") is not None
        self.built: dict[str, Path] = {}
        self.objects: list[Path] = []

    def native(self, path: Path) -> str:
        if self.wsl:
            return subprocess.run(["wslpath", "-w", str(path)], capture_output=True, text=True, timeout=30).stdout.strip()
        return str(path)

    def compile(self, source: Path, extra: list[str] | None = None) -> Path:
        obj = self.work / (source.stem + ("-rt" if source.parent == SRC else "") + ".o")
        proc = subprocess.run([self.cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-c", str(source), "-o", str(obj), *(extra or [])],
                              capture_output=True, text=True, timeout=300)
        if proc.returncode != 0:
            raise RuntimeError(f"{source.name} did not build:\n" + proc.stderr.strip())
        return obj

    def start_check(self) -> str | None:
        """None when a Windows program can be started, else the reason."""
        probe = self.work / "probe.c"
        probe.write_text("int main(void) { return 7; }\n")
        out = self.work / "probe.exe"
        proc = subprocess.run([self.cc, "-o", str(out), str(probe)], capture_output=True, text=True, timeout=120)
        if proc.returncode != 0:
            return "the compiler cannot build a program: " + (proc.stderr.strip().splitlines() or ["?"])[0]
        try:
            ran = subprocess.run([*self.prefix, str(out)], capture_output=True, timeout=60)
        except (OSError, subprocess.TimeoutExpired) as err:
            return f"cannot start a Windows program: {err}"
        return None if ran.returncode == 7 else f"cannot start a Windows program (status {ran.returncode})"

    def runtime_objects(self) -> list[Path]:
        if not self.objects:
            self.objects = [self.compile(SRC / f"{n}.c") for n in RUNTIME]
            stubs = self.work / "stubs.c"
            stubs.write_text(STUBS)
            self.objects.append(self.compile(stubs))
        return self.objects

    def program_for(self, variant: str, pin: bytes) -> Path:
        """The runtime built with the variant's tables, domains and game code, for the program `pin`."""
        v = VARIANTS[variant]
        key = f"{variant}-{hashlib.sha256(pin).hexdigest()[:8]}"
        if key not in self.built:
            objs = list(self.runtime_objects())
            for tag, text in (("tables", tables_c(v.functions, v.absents, pin)), ("domains", v.domains_c), ("mbegin", MARK_BEGIN), ("game", v.game), ("mend", MARK_END)):
                path = self.work / f"{key}-{tag}.c"
                path.write_text(text)
                objs.append(self.compile(path, ["-Wno-unused-function"] if tag == "game" else None))
            names = {n: a for n, a, _ in v.absents} | {n: a for n, a, _ in v.functions}
            defs = [f"-Wl,--defsym,_ps1_{n}=0x{a:08x}" for n, a in names.items()]
            out = self.work / f"sfa2-{key}.exe"
            proc = subprocess.run([self.cc, "-o", str(out), *map(str, objs), *LINK_FLAGS, *defs], capture_output=True, text=True, timeout=300)
            if proc.returncode != 0:
                raise RuntimeError("the runtime did not link:\n" + proc.stderr.strip())
            self.built[key] = out
        return self.built[key]

    def run(self, tag: str, data: bytes, pin: bytes | None = None, variant: str = "base", args: list[str] | None = None,
            timeout: int = 60) -> tuple[int, list[str], Image, str]:
        """Run the program built for `pin` (default: `data` itself) on an image holding `data`."""
        exe = self.program_for(variant, data if pin is None else pin)
        img = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": data})
        path = self.work / f"{tag}.bin"
        img.write(path)
        arg = self.native(path)
        for attempt in range(3):   # the program prints its first line before anything can fail: no output at all, or a run that outlasts a loaded machine's patience, is tried again
            try:
                proc = subprocess.run([*self.prefix, str(exe), *(args or []), arg], capture_output=True, text=True, timeout=timeout)
            except subprocess.TimeoutExpired:
                if attempt == 2:
                    raise
                continue
            if proc.stdout:
                break
        return proc.returncode, proc.stdout.replace("\r\n", "\n").splitlines(), img, arg


def head(img: Image, arg: str, host: int = 0, stops: int = 1, overrides: int = 0, with_c: int = 2, without_c: int = 2) -> list[str]:
    s = img.sector["SLPS_004.15;1"]
    return [
        "memory: RAM at 0x80000000 (2 MB), scratchpad at 0x1f800000",
        f"disc: {arg}, 2352-byte sectors",
        f"program: SLPS_004.15 at sector {s}, {T_SIZE} bytes to 0x{T_ADDR:08x}, entry 0x{PC0:08x}",
        "identity: SHA-256 matches the build's baseline",
        f"jumps: {with_c} written for functions with C, {without_c} for functions without",
        f"library: {host} host routines, {stops} left that stop",
        f"overrides: {overrides}",
    ]


def verdict(got, want):
    return None if got == want else f"got {got!r}, wanted {want!r}"


def cases(rig: Rig):
    good = program(ENTRY_ABSENT)
    status, lines, img, arg = rig.run("baseline", good)
    want = head(img, arg) + [f"start: 0x{ENTRY_ABSENT:08x}", f"stop: no C yet for func_{ENTRY_ABSENT:08x} (0x{ENTRY_ABSENT:08x})"]
    yield "baseline-right-image-stops-at-the-function-without-c", verdict((status, lines), (3, want))

    status, lines, img, arg = rig.run("withc", program(ENTRY_C))
    want = head(img, arg) + [f"start: 0x{ENTRY_C:08x}", "C main ran", "stop: main returned"]
    yield "entry-at-a-function-with-c-runs-it-then-main-returned", verdict((status, lines), (0, want))

    status, lines, img, arg = rig.run("library", program(ENTRY_LIBRARY))
    want = head(img, arg) + [f"start: 0x{ENTRY_LIBRARY:08x}", f"stop: library function memcpy_like (0x{ENTRY_LIBRARY:08x}) has no host routine yet"]
    yield "entry-at-a-library-function-stops-with-status-4", verdict((status, lines), (4, want))

    # the program differs from the pinned one in one byte (the jal's
    # entry target becomes 0x80100024, in no table, and an x86 ret lies there)
    off = bytearray(good)
    off[0x800 + (PC0 - T_ADDR) + 12] ^= 1
    off = bytes(off)
    yield "wrong-image-fixture-differs-in-exactly-one-byte", None if len(off) == len(good) and sum(a != b for a, b in zip(off, good)) == 1 else "the fixture differs in more than one byte"
    yield "wrong-image-fixture-has-a-ret-at-its-entry-target", None if off[0x800 + (ENTRY_ABSENT + 4 - T_ADDR)] == 0xC3 else "no ret at the entry target"
    status, lines, img, arg = rig.run("wrong", off, pin=good)
    bad = [l for l in lines if l.startswith(("start:", "stop:", "identity:", "jumps:", "program:"))]
    yield "wrong-image-is-refused-for-its-identity", None if (status, lines[-1], bad, len(lines)) == (2, "refused: the disc's program is not the one this build is for", [], 3) else f"status {status}, lines {lines!r}"

    for tag, target in (("unregistered", UNREGISTERED), ("inside-a-function-without-c", ENTRY_ABSENT + 4), ("inside-a-function-with-c", BIG_C + 4),
                        ("just-before-a-function", ENTRY_ABSENT - 4), ("module-address", 0x80180000)):
        status, lines, img, arg = rig.run(tag, program(target))
        refusal = f"refused: the entry 0x{target:08x} is not a function this build knows"
        bad = [l for l in lines if l.startswith(("start:", "stop:")) or l.endswith("ran")]
        yield f"entry-{tag}-is-refused", None if status == 2 and lines[-1] == refusal and not bad and lines[:len(head(img, arg))] == head(img, arg) else f"status {status}, lines {lines!r}"

    # ---- the library layer on made-up tables ----
    status, lines, img, arg = rig.run("mech", program(G_MECH), variant="mech")
    want = head(img, arg, host=2, stops=2, overrides=1, with_c=4, without_c=5) + [
        f"start: 0x{G_MECH:08x}", "host a ran 5", "a returned 6", "host c ran 7", "c returned 9", "override ran",
        f"stop: library function lib_b (0x{LIB_B:08x}) has no host routine yet"]
    yield "a-library-function-with-a-host-routine-is-reached-one-without-still-stops-an-override-replaces-the-c", verdict((status, lines), (4, want))

    status, lines, img, arg = rig.run("mech-noover", program(G_MECH), variant="mech-noover")
    yield "without-the-override-the-games-own-c-runs", verdict((status, "original C ran" in lines, "override ran" in lines), (4, True, False))

    for tag, text in (("dup", "library: lib_a is listed twice (test and test, as lib_a)"),
                      ("dup-address", f"library: lib_a is listed twice (test and test, as @0x{LIB_A:08x})"),
                      ("unknown", "library: no_such_function (test) is listed but this build has no library function of that name without C"),
                      ("unknown-address", "library: @0x80100999 (test) is listed but this build has no library function of that name without C"),
                      ("over-unknown", "overrides: lib_a is listed but this build has no function of that name with C")):
        status, lines, img, arg = rig.run(tag, program(G_MECH), variant=tag)
        bad = [l for l in lines if l.startswith(("library:", "start:", "stop:")) or l.endswith("ran")]
        yield f"table-{tag}-is-refused", None if status == 2 and lines[-1] == "refused: " + text and not bad else f"status {status}, lines {lines!r}"

    trace = rig.work / "trace.txt"
    status, lines, img, arg = rig.run("traced", program(G_MECH), variant="mech", args=["--trace", "--trace-file", rig.native(trace)])
    got = trace.read_text().splitlines() if trace.exists() else None
    want_trace = ["lib_a 0x5 0x0 0x0 0x0", "lib_c 0x7 0x0 0x0 0x0", "lib_b 0x1 0x0 0x0 0x0"]
    got_cmp = [" ".join(l.split()[:2] + ["0x0"] * 3) for l in got] if got else got
    yield "trace-logs-each-library-call-including-the-one-that-stops", verdict(
        (status, got_cmp, "host a ran 5" in lines, "c returned 9" in lines, lines[-1]),
        (4, want_trace, True, True, f"stop: library function lib_b (0x{LIB_B:08x}) has no host routine yet"))
    status, lines, img, arg = rig.run("trace-half", program(G_MECH), variant="mech", args=["--trace"])
    yield "trace-without-a-file-is-refused", verdict((status, lines), (2, ["refused: --trace and --trace-file FILE go together"]))

    exe = rig.program_for("mech", program(G_MECH))
    proc = subprocess.run([*rig.prefix, str(exe), "--list-library"], capture_output=True, text=True, timeout=60)
    out = proc.stdout.replace("\r\n", "\n").splitlines()
    want = ["host routines (2):", f"  lib_a 0x{LIB_A:08x} (test)", f"  @0x{LIB_C:08x} 0x{LIB_C:08x} (test)", "routines that do nothing on purpose (0):",
            "left that stop (2):", f"  memcpy_like 0x{ENTRY_LIBRARY:08x}", f"  lib_b 0x{LIB_B:08x}", "overrides of game functions (1):",
            "  game_over: the test replaces it", "library: 2 host routines, 2 left that stop"]
    yield "list-library-needs-no-disc-and-prints-the-groups-with-addresses", verdict((proc.returncode, out), (0, want))

    status, lines, img, arg = rig.run("crash", program(G_CRASH), variant="crash")
    ok = status == 10 and lines[-2] == "about to read the mirror" and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x00001000 \(read\) in game_crash \(at 0x[0-9a-f]{8}\)", lines[-1])
    yield "a-read-of-the-ram-mirror-ends-with-a-line-that-says-so-and-names-the-function", None if ok else f"status {status}, lines {lines[-3:]!r}"

    # ---- addresses that the game hands to the runtime to call ----
    for path, name, after in (("thread entry", "thread", "after the switch"), ("event handler", "event", "after the vblank"),
                              ("interrupt callback", "irq", "after the vblank"), ("vsync callback", "vsync", "after the vblank")):
        entry = G_TARGET[name]
        status, lines, img, arg = rig.run(f"tgt-{name}", program(entry), variant="tgt-unreg", timeout=60)
        ran = [l for l in lines if l.startswith(("after", "returned")) or "returned" in l]
        refusal = f"refused: {path} 0x{UNREGISTERED:08x} is not a function this program installed"
        yield f"{name}-target-in-unregistered-ram-is-refused-and-never-run", None if (status, lines[-1]) == (12, refusal) and not ran and not any(l.startswith("stop:") for l in lines) else f"status {status}, lines {lines[-3:]!r}"
        status, lines, img, arg = rig.run(f"tgt-{name}-absent", program(entry), variant="tgt-absent", timeout=60)
        yield f"{name}-target-that-is-a-function-without-c-ends-with-its-named-stop", verdict(
            (status, lines[-1]), (3, f"stop: no C yet for func_{ENTRY_ABSENT:08x} (0x{ENTRY_ABSENT:08x})"))
    status, lines, img, arg = rig.run("tgt-wipe", program(G_TARGET["wipe"]), variant="tgt-unreg", timeout=60)
    yield "a-resident-entry-whose-jump-the-game-wrote-over-is-refused-as-a-thread-entry-with-a-line-that-says-so", verdict(
        (status, lines[-2:]), (12, ["overwrote the jump", f"refused: thread entry 0x{ENTRY_C:08x} is a resident entry whose jump is no longer there"]))
    status, lines, img, arg = rig.run("tgt-valid", program(G_TARGET["valid"]), variant="tgt-unreg", timeout=60)
    yield "handlers-and-callbacks-inside-the-games-own-code-are-called", verdict(
        (status, lines[-4:]), (0, ["event handler ran 1", "interrupt callback ran 1", "vsync callback ran 1", "stop: main returned"]))

    # ---- the disc layer on the linked program ----
    refusal = f"refused: ready callback 0x{UNREGISTERED:08x} is not a function this program installed"
    status, lines, img, arg = rig.run("cd-unreg", program(G_CD["ready"]), variant="cd-unreg", timeout=60)
    yield "ready-callback-in-unregistered-ram-is-refused-and-never-run", None if (status, lines[-1]) == (12, refusal) and "handler set" in lines and not any(l.startswith(("after", "ready")) for l in lines) else f"status {status}, lines {lines[-3:]!r}"
    status, lines, img, arg = rig.run("cd-absent", program(G_CD["ready"]), variant="cd-absent", timeout=60)
    yield "ready-callback-that-is-a-function-without-c-ends-with-its-named-stop", verdict((status, lines[-1]), (3, f"stop: no C yet for func_{ENTRY_ABSENT:08x} (0x{ENTRY_ABSENT:08x})"))
    status, lines, img, arg = rig.run("cd-valid", program(G_CD["ready"]), variant="cd-valid", timeout=60)
    yield "ready-callback-inside-the-games-own-code-is-called-and-no-vblank-is-delivered-inside-it", verdict(
        (status, lines[-3:]), (0, ["after the vblanks", "ready handler ran 1, vblank handlers inside it 0, vblanks 1", "stop: main returned"]))

    status, lines, img, arg = rig.run("cd-exec", program(G_CD["exec"]), variant="cd-valid", timeout=60)
    yield "exec-of-a-program-of-the-disc-ends-the-run-by-default-and-names-the-file-and-sector", verdict(
        (status, lines[-2:]), (13, ["sector read", f"stop: no C yet for the program slps_004.15 (disc sector {EXEC_SECTOR})"]))
    status, lines, img, arg = rig.run("cd-exec-skip", program(G_CD["exec"]), variant="cd-valid", args=["--skip-programs"], timeout=60)
    yield "exec-with-skip-programs-prints-a-line-and-returns-at-once", verdict(
        (status, lines[-4:]), (0, ["sector read", f"skipped: Exec of slps_004.15 (disc sector {EXEC_SECTOR}): a program of its own for which no C exists yet; it returns at once", "after exec", "stop: main returned"]))
    status, lines, img, arg = rig.run("cd-exec-unknown", program(G_CD["exec_unknown"]), variant="cd-valid", timeout=60)
    yield "exec-of-a-header-that-did-not-come-from-the-disc-stops-with-an-unknown-name", verdict(
        (status, lines[-1]), (13, "stop: no C yet for the program ? (disc sector -1)"))
    status, lines, img, arg = rig.run("cd-getsector", program(G_CD["getsector"]), variant="cd-valid", timeout=60)
    yield "cdgetsector-into-an-address-outside-the-ram-ends-the-run-before-a-byte-is-copied", verdict(
        (status, lines[-2:]), (6, ["before", "stop: CdGetSector buffer 0x00001000 (2048 bytes) is outside the PS1's RAM"]))

    # ---- the memory card model: empty slots ----
    status, lines, img, arg = rig.run("card", program(G_CARD), variant="card")
    body = lines[lines.index(f"start: 0x{G_CARD:08x}") + 1:] if f"start: 0x{G_CARD:08x}" in lines else lines
    want = ["polling before the request: 0", "info 1", "first ready event index 2", "again 0", "load 1 clear 1", "ready after load: 1",
            "open -1 read -1 firstfile 0 format 0", "stop: main returned"]
    yield "card-requests-are-accepted-and-answered-with-the-time-out-event-and-the-file-calls-fail", verdict((status, body), (0, want))
    exe = rig.program_for("card", program(G_CARD))
    proc = subprocess.run([*rig.prefix, str(exe), "--list-library"], capture_output=True, text=True, timeout=60)
    out = proc.stdout.replace("\r\n", "\n")
    ok = proc.returncode == 0 and "routines that do nothing on purpose (13):" in out and out.count("no memory card in this port yet") == 13
    yield "card-routines-are-listed-as-doing-nothing-on-purpose-with-the-note", None if ok else out[:400]

    # ---- the vblank as an interrupt of the game's thread ----
    status, lines, img, arg = rig.run("irq-spin", program(G_SPIN), variant="irq", timeout=60)
    yield "a-spin-on-a-counter-only-the-handler-raises-ends-by-itself-and-the-handler-ran-on-the-games-thread", verdict(
        (status, lines[-2:]), (0, ["spin ended; the handler ran on the game's thread: 1", "stop: main returned"]))
    status, lines, img, arg = rig.run("irq-off", program(G_SPIN), variant="irq", args=["--no-interrupt", "--watchdog", "3"], timeout=60)
    yield "the-same-spin-with-no-interrupt-is-ended-by-the-watchdog", None if status == 11 and lines[-1].startswith("stop: hang: no vblank for 3 s; the program is at") else f"status {status}, lines {lines[-2:]!r}"
    status, lines, img, arg = rig.run("irq-host", program(G_HOST), variant="irq", timeout=60)
    yield "a-spin-inside-a-host-routine-is-not-interrupted", verdict((status, lines[-3:]), (0, ["count moved 0 during the host routine's 200 ms spin", "and moved after it: 1", "stop: main returned"]))
    status, lines, img, arg = rig.run("irq-nest", program(G_NEST), variant="irq", timeout=60)
    yield "a-handler-is-not-interrupted-by-a-second-vblank", verdict((status, lines[-2:]), (0, ["handler runs 1, deepest nesting 1", "stop: main returned"]))
    status, lines, img, arg = rig.run("irq-regs", program(G_REGS), variant="irq", timeout=120)
    yield "registers-flags-and-x87-survive-many-interruptions", verdict((status, lines[-2:]), (0, ["registers, flags and x87 survived: 1; handler ran %s times meanwhile: 1" % (lines[-2].split("ran ")[1].split(" ")[0] if "ran " in lines[-2] else "?"), "stop: main returned"]))

    status, lines, img, arg = rig.run("irq-fpu", program(G_FPU), variant="irq", timeout=180)
    body = [l for l in lines if l.startswith(("interrupted state", "handler saw", "handler arithmetic"))]
    yield "the-interrupted-x87-values-control-word-mxcsr-and-direction-flag-come-back", verdict(
        (status, body[:1]), (0, ["interrupted state back: values 1 control word 1 mxcsr 1 direction flag 1"]))
    yield "the-handler-sees-an-empty-x87-stack-the-default-control-word-the-default-mxcsr-and-a-clear-direction-flag", verdict(
        body[1:2], ["handler saw: default control word 1 empty x87 stack 1 default mxcsr 1 direction flag clear 1"])
    yield "the-handler-computes-right-with-all-eight-x87-slots-occupied-by-the-interrupted-code", verdict(
        body[2:3], ["handler arithmetic: x87 1 sse 1; handler ran %s times: 1" % (body[2].split("ran ")[1].split(" ")[0] if len(body) > 2 and "ran " in body[2] else "?")])

    # ---- the kernel ----
    status, lines, img, arg = rig.run("vblank", program(G_VBLANK), variant="kern", timeout=60)
    body = lines[len(head(img, arg)) + 1:]
    want = ["reset first", "reset again", "event f1000000 enable 1",
            "five waits: vblanks 5 handler 5 slow 0 fast 0",
            "enter returned 1; in the critical section the handler ran 0 times",
            "exit; enter again returned 0",
            "after exit the handler ran 2 times in one wait",  # the held-back vblank (merged into one) and the one waited for
            ]
    got_main = [l for l in body if not l.startswith(("polled", "close", "closed", "stop"))]
    yield "a-handler-opened-with-openevent-runs-once-per-vblank-held-back-in-a-critical-section-and-delivered-once-after", verdict((status, got_main), (0, want))
    polled = [l for l in body if l.startswith("polled")]
    yield "testevent-sees-a-delivered-event", None if polled and polled[0].endswith(": 1") else f"lines {body!r}"
    tail = [l for l in body if l.startswith(("close", "closed"))]
    yield "closed-event-stops-calling-its-handler", verdict(tail, ["close 1 0", "closed handler ran 0 times"])

    status, lines, img, arg = rig.run("threads", program(G_THREADS), variant="kern", timeout=60)
    body = lines[len(head(img, arg)) + 1:]
    want = [f"gp {GP:08x}", "handles ff000001 ff000002 ff000003 fourth ffffffff",
            "t1-a", "t2-a", "t3-a", "t1-b", "main-1", "t2-b", "t1-c", "main-2",
            "close t1 from outside 1; change to it 0",
            "t3-b", "t3 closes itself: 1", "main-3",
            "reopened ff000001; change to the closed t3 0; close the main thread 0", "stop: main returned"]
    yield "three-tasks-run-in-order-and-a-task-that-closed-itself-is-gone", verdict((status, body), (0, want))

    status, lines, img, arg = rig.run("threadret", program(G_THREAD_RET), variant="kern", timeout=60)
    body = lines[len(head(img, arg)) + 1:]
    ok = status == 8 and body[0] == "thread function ran and returns" and body[1].startswith("stop: the function of a thread (0x") and "not reached" not in " ".join(body)
    yield "a-thread-function-that-returns-ends-the-program-with-status-8", None if ok else f"status {status}, lines {body!r}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostlaunch-", dir=BUILD))
    try:
        rig = Rig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = 0
        try:
            for name, detail in cases(rig):
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
        except Exception as err:  # a control must report, not crash
            print(f"FAIL the control itself raised {type(err).__name__}: {err}")
            failed += 1
        print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
