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
RUNTIME = ["main", "memory", "disc", "jumps", "sha256", "library", "kernel", "threads", "overrides", "clib", "sound", "card", "debug"]

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
     "ExitCriticalSection", "OpenTh", "ChangeTh", "CloseTh", "GetGp", "CloseEvent"])}
G_COMPACT, F_COMPACT, G_CRASH = RAM + 0x100500, RAM + 0x100510, RAM + 0x100520
TABLE = RAM + 0x104000
LINKS = [0,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,1,1,0,0,1,0,0,0,1,1,0,1,1,1,1]
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

def game_compact() -> str:
    links = ", ".join(map(str, LINKS))
    return PROLOGUE + f"""
extern void ps1_func_80119694(void *);
void func_80119694(void *a) {{ (void)a; SAY("original C ran\\n"); }}
void game_compact(void)
{{
    static const int links[40] = {{ {links} }};
    unsigned *t = (unsigned *)0x{TABLE:08x}u;
    int i;
    for (i = 0; i < 40; i++) t[i] = links[i] ? ((0x{TABLE:08x}u + 4u * (unsigned)(i - 1)) & 0xffffffu) : 0x12345678u;
    ps1_func_80119694(t + 39);
    for (i = 0; i < 40; i++) SAY("w%d %08x\\n", i, t[i]);
}}
void game_crash(void)
{{
    volatile unsigned *p = (volatile unsigned *)0x1000;
    SAY("about to read the mirror\\n");
    SAY("read %u\\n", *p);
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

FUN_COMPACT = FUN_BASE + [("game_compact", G_COMPACT, "game_compact"), ("func_80119694", F_COMPACT, "func_80119694"), ("game_crash", G_CRASH, "game_crash")]

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
         "CloseEvent": "port_h_CloseEvent"}


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


def tables_c(functions, absents, pin: bytes) -> str:
    out = ['#include "port_tables.h"']
    for _, _, sym in functions:
        out.append(f"extern void {sym}(void);")
    out.append('const struct port_image port_images[] = {{ "mod", 0x80180000u, 0, 1, 0 }};')
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
    "over-unknown": Variant("over-unknown", GAME_MECH, FUN_MECH, ABS_MECH, domains("", '{ "no_such_function", (void *)over_host, "no function of that name" },')),
    "over-absent": Variant("over-absent", GAME_MECH, FUN_MECH, ABS_MECH,
                           domains('{ "lib_a", (void *)host_a, 0 }, { "@0x%08x", (void *)host_c, 0 },' % LIB_C, '{ "lib_b", (void *)over_host, "the test replaces a library function that has no host routine" },')),
    "compact": Variant("compact", game_compact(), FUN_COMPACT, ABS_BASE,
                       "#include \"port.h\"\nextern void port_o_func_80119694(void *);\nstatic const struct port_library t[] = { { 0, 0, 0 } };\n"
                       "const struct port_domain port_domains[] = { { \"none\", t } };\nconst unsigned port_domain_count = 1;\n"
                       "static const struct port_override o[] = { { \"func_80119694\", (void *)port_o_func_80119694, \"test\" }, { 0, 0, 0 } };\n"
                       "const struct port_override *const port_override_sets[] = { o };\nconst unsigned port_override_set_count = 1;\n"),
    "card": Variant("card", GAME_CARD, FUN_CARD, ABS_CARD,
                    "#include \"port.h\"\nextern void port_h_OpenEvent(), port_h_EnableEvent(), port_h_TestEvent();\n"
                    "static const struct port_library k[] = { { \"OpenEvent\", (void *)port_h_OpenEvent, 0 }, { \"EnableEvent\", (void *)port_h_EnableEvent, 0 }, { \"TestEvent\", (void *)port_h_TestEvent, 0 }, { 0, 0, 0 } };\n"
                    "const struct port_domain port_domains[] = { { \"kernel\", k }, { \"card\", port_card_library } };\nconst unsigned port_domain_count = 2;\n"
                    "static const struct port_override o[] = { { 0, 0, 0 } };\n"
                    "const struct port_override *const port_override_sets[] = { o };\nconst unsigned port_override_set_count = 1;\n"),
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
        proc = subprocess.run([*self.prefix, str(exe), *(args or []), arg], capture_output=True, text=True, timeout=timeout)
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

    status, lines, img, arg = rig.run("over-absent", program(G_MECH), variant="over-absent")
    want = head(img, arg, host=2, stops=2, overrides=1, with_c=4, without_c=5) + [
        f"start: 0x{G_MECH:08x}", "host a ran 5", "a returned 6", "host c ran 7", "c returned 9", "original C ran", "override ran", "not reached", "stop: main returned"]
    yield "an-override-of-a-function-without-c-is-installed-in-place-of-its-stop-and-runs", verdict((status, lines), (0, want))

    for tag, text in (("dup", "library: lib_a is listed twice (test and test, as lib_a)"),
                      ("dup-address", f"library: lib_a is listed twice (test and test, as @0x{LIB_A:08x})"),
                      ("unknown", "library: no_such_function (test) is listed but this build has no library function of that name without C"),
                      ("unknown-address", "library: @0x80100999 (test) is listed but this build has no library function of that name without C"),
                      ("over-unknown", "overrides: no_such_function is listed but this build knows no function of that name")):
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

    # the override of the ordering-table compaction, on a table of the test's own
    def oracle(links):
        t = [((TABLE + 4 * (i - 1)) & 0xFFFFFF) if links[i] else 0x12345678 for i in range(40)]
        def link(i):
            return t[i] == ((TABLE + 4 * (i - 1)) & 0xFFFFFF)
        budget, i = 29, 39
        while True:
            if not link(i):
                budget -= 1
                if budget == 0: return t
                i -= 1
                continue
            budget -= 1
            if budget == 0: return t
            last = i
            i -= 1
            while link(i):
                budget -= 1
                if budget == 0: return t
                i -= 1
            t[last] = (TABLE + 4 * i) & 0xFFFFFF
            budget -= 1
            if budget == 0: return t
            i -= 1
    status, lines, img, arg = rig.run("compact", program(G_COMPACT), variant="compact")
    want_words = [f"w{i} {w:08x}" for i, w in enumerate(oracle(LINKS))]
    start_at = lines.index(f"start: 0x{G_COMPACT:08x}") if f"start: 0x{G_COMPACT:08x}" in lines else -1
    body = lines[start_at + 1:]
    yield "override-of-the-ordering-table-compaction-matches-the-test-s-own-walk", verdict((status, body), (0, want_words + ["stop: main returned"]))
    yield "compaction-fixture-has-links-that-change-something", None if oracle(LINKS) != [((TABLE + 4 * (i - 1)) & 0xFFFFFF) if LINKS[i] else 0x12345678 for i in range(40)] else "the walk changes nothing"

    status, lines, img, arg = rig.run("crash", program(G_CRASH), variant="compact")
    ok = status == 10 and lines[-2] == "about to read the mirror" and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x00001000 \(read\) in game_crash \(at 0x[0-9a-f]{8}\)", lines[-1])
    yield "a-read-of-the-ram-mirror-ends-with-a-line-that-says-so-and-names-the-function", None if ok else f"status {status}, lines {lines[-3:]!r}"

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
