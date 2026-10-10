#!/usr/bin/env python3
"""Controls for the modules package (port/src/modules.c): the launch path, run for real.

    python3 test_hostmodules.py --cc CROSS_CC [--run PREFIX]

The test builds the runtime (every port/src/*.c the launch controls build, plus
modules.c) with the cross compiler CC and made-up tables of its own, then runs
the program on disc images made here from invented bytes. The images hold three
made-up archives of the real archive layout (u32 count, 32-byte entries of
u16 slot, u16 table, u32 size and a copy of the first word, data from 0x800 in
whole sectors) in a folder, and the made-up game code of this file loads them
with the real CdIntToPos / CdControlB / CdReady / CdGetSector host routines:

- module A (slot 5) and module B (slot 5, another archive) load at one address
  (0x80150000, two pages). The tables give each image one function with C on the
  first page, one without C, and B a second one with C on the second page;
- load A, call its function: the C of A runs and the `module:` line says so;
- load B over it, call the same address: the C of B runs (a new `module:` line);
  B's function on the second page runs without another line; load A again: A's;
- the data of a loaded module is readable before any call (no fault, no line);
- the function without C: the stop line names it, status 3;
- an address in a loaded page that is no function start of the image: refused with
  the line that names the sector, the file and the images at that address, status 5;
- a module no image of the tables fits (another slot): the same kind of line;
- a `like` image (its C is not built yet): the line that says so and names it, status 3;
- a fault of the kind "read" is not swallowed: the program's own crash line, status 10;
- identity: a chunk whose bytes differ from the content the tables pin ends the program with `refused:`
  naming the image, and no jump is written; so does an image the tables pin nothing for; so does a module
  whose bytes in memory were changed by the game after the disc wrote them;
- the archive is the user's file: a header with too many chunks, a chunk length that wraps 32 bits, a later
  chunk that runs past the file, no chunk, a file shorter than a header, a chunk that would end outside the
  RAM, each ends with a line and no jump;
- a fault on a thread that is not the game's is not swallowed; an address that a module function is handed to
  the disc layer as the ready callback runs when the module is placed, and an address inside the module that is
  no function start is refused before the call;
- a second placement (`like` image) with its own table: its C runs, its function without C stops by name.

The expected lines are worked out here from the fixtures. Needs the cross compiler
and a way to start a Windows program, like test_hostlaunch.py; without them this
says so and ends with status 2.
"""

from __future__ import annotations

import argparse
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostlaunch as tl  # noqa: E402
from test_hostrun import CNF, Image  # noqa: E402

SRC, BUILD, RAM = tl.SRC, tl.BUILD, tl.RAM
import hashlib
BASE = RAM + 0x150000      # where A and B load
LIKE_BASE = RAM + 0x160000
OTHER_BASE = RAM + 0x170000
SIZE = 0x2000


def archive(slot: int, size: int, seed: int) -> bytes:
    """A made-up archive of one chunk of table 0: the real container layout, invented content."""
    data = bytearray(((i * seed + 1) & 0xFF) for i in range(size))
    header = bytearray(0x800)
    struct.pack_into("<I", header, 0, 1)
    struct.pack_into("<HHI", header, 0x20, slot, 0, size)
    header[0x28:0x2C] = data[0:4]
    return bytes(header + data)


def content(seed: int, size: int) -> bytes:
    return bytes(((i * seed + 1) & 0xFF) for i in range(size))


KBASE = RAM + 0x180000     # the second placement with a table of its own
YBASE = RAM + 0x1f8000     # an image whose chunk would end outside the RAM
PBASE = RAM + 0x1a0800     # two images that share the page 0x801a0000: mq in its first half, mp in its second
QBASE = RAM + 0x1a0000
PF, QF = PBASE + 0x10, QBASE + 0x10
TBASE = RAM + 0x1b0000     # two pages: entries with C at +0x10, +0x900 and +0x1010, without C at +0x20 and +0xa00
UBASE = RAM + 0x1b8000     # entries without C only, at +0x10 and +0x900
T0, TG, T1, TZ, TW = TBASE + 0x10, TBASE + 0x20, TBASE + 0x900, TBASE + 0xa00, TBASE + 0x1010
U0, U1 = UBASE + 0x10, UBASE + 0x900
MANY = 2000
YSIZE = 0x10000
ATK = RAM + 0x190000       # where the broken archives are loaded


def raw_archive(entries: list[tuple[int, int, int]], data: bytes, count: int | None = None) -> bytes:
    """An archive with a header of the given (slot, table, size) entries (and, if given, another count) and `data`."""
    header = bytearray(0x800)
    struct.pack_into("<I", header, 0, len(entries) if count is None else count)
    for k, (slot, table, size) in enumerate(entries):
        struct.pack_into("<HHI", header, 0x20 + 32 * k, slot, table, size)
    return bytes(header) + data.ljust((len(data) + 2047) // 2048 * 2048, b"\0")


FILES = {"MODA.PAC;1": archive(5, SIZE, 3), "MODB.PAC;1": archive(5, SIZE, 5), "MODL.PAC;1": archive(7, 0x800, 7), "MODX.PAC;1": archive(9, 0x800, 11),
         "MODK.PAC;1": archive(8, 0x800, 13), "MODY.PAC;1": archive(11, YSIZE, 17),
         "MODP.PAC;1": archive(12, 0x800, 19), "MODQ.PAC;1": archive(13, 0x800, 23),
         "MODT.PAC;1": archive(14, 0x2000, 29), "MODU.PAC;1": archive(15, 0x2000, 31),
         "MODH1.PAC;1": raw_archive([(5, 0, 0x800)], bytes(0x800), count=64),        # more chunks than a header holds
         "MODH2.PAC;1": raw_archive([(5, 0, 0xFFFFFFFF)], bytes(0x800)),              # a length that wraps 32 bits
         "MODH3.PAC;1": raw_archive([(5, 0, 0x800), (5, 0, 0x100000)], bytes(0x800)),  # a later chunk past the file
         "MODH4.PAC;1": raw_archive([], bytes(0x800), count=0),                       # no chunk
         "MODH5.PAC;1": b"\x01\x00\x00\x00" + bytes(96)}                           # shorter than a header
ATTACKS = ["MODH1", "MODH2", "MODH3", "MODH4", "MODH5"]


def disc_image(entry_program: bytes, files: dict | None = None) -> Image:
    return Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": entry_program, "PAC": dict(files or FILES)})


PROBE = disc_image(tl.program(tl.ENTRY_C))
FILE_SECTOR = {n.split(";")[0]: PROBE.sector["PAC/" + n] for n in FILES}   # "MODA.PAC" -> first sector

# resident game functions (with C) and the library functions the game code calls (absent, library)
G = {n: RAM + 0x101800 + 0x10 * i for i, n in enumerate(
    ["g_a", "g_ab", "g_noc", "g_nonstart", "g_other", "g_like", "g_read", "g_data", "g_tamper", "g_thread", "g_cb_ok", "g_cb_bad", "g_second", "g_second_noc", "g_big"]
    + [f"g_atk{k}" for k in range(len(ATTACKS))] + ["g_resident", "g_trial", "g_edge", "g_inside", "g_shared", "g_shared_one", "g_shared_data", "g_partial", "g_gap", "g_many",
     "g_pa", "g_pa_cb_unloaded", "g_pa_cb_first", "g_pu", "g_four_t", "g_four_t_cb", "g_four_u", "g_full_t", "g_over_t", "g_rewrite_page", "g_rewrite_entry", "g_direct_unloaded"])}
LIB = {n: RAM + 0x100700 + 0x10 * i for i, n in enumerate(["CdControlB", "CdReady", "CdGetSector", "CdIntToPos", "CdInit", "CdSync", "CdControl", "CdControlF", "CdMix", "CdPosToInt"])}
FA, GA, FB2 = BASE + 0x10, BASE + 0x20, BASE + 0x1010   # C, without C, C on the second page
NONSTART = BASE + 0x1018
LIKE_F, OTHER_F = LIKE_BASE + 0x10, OTHER_BASE + 0x10
KF, KG = KBASE + 0x10, KBASE + 0x20          # the second placement's function with C and one without
YF = YBASE + 0x10

TRIAL_DEST = RAM + 0x101000    # a data sector copied here ends where the resident functions of the game code begin (0x101800)


def attack_functions() -> str:
    out = []
    for k, name in enumerate(ATTACKS):
        sector = FILE_SECTOR[name + ".PAC"] + (0 if name == "MODH5" else 1)
        out.append(f"void g_atk{k}(void) {{ load({sector}, 1, 0x{ATK + 0x1000 * k:08x}u); SAY(\"loaded\\n\"); CALL(0x{ATK + 0x1000 * k + 0x10:08x}u); SAY(\"not reached\\n\"); }}")
    return "\n".join(out)


GAME = tl.PROLOGUE + f"""
extern int ps1_CdControlB(int, unsigned char *, unsigned char *);
extern int ps1_CdReady(int, unsigned char *);
extern int ps1_CdGetSector(void *, int);
extern unsigned char *ps1_CdIntToPos(int, unsigned char *);
static void load(int sector, int sectors, unsigned dest)
{{
    unsigned char pos[4], res[8];
    int i;
    ps1_CdIntToPos(sector, pos);
    ps1_CdControlB(2, pos, res);
    ps1_CdControlB(6, 0, res);
    for (i = 0; i < sectors; i++) {{
        ps1_CdReady(0, res);
        ps1_CdGetSector((void *)(size_t)(dest + 2048u * (unsigned)i), 512);
    }}
    ps1_CdControlB(9, 0, res);
}}
#define LOAD_A load({FILE_SECTOR['MODA.PAC'] + 1}, 4, 0x{BASE:08x}u)
#define LOAD_B load({FILE_SECTOR['MODB.PAC'] + 1}, 4, 0x{BASE:08x}u)
#define CALL(a) ((void (*)(void))(size_t)(a))()
void ma_f(void) {{ SAY("A ran\\n"); }}
void mb_f(void) {{ SAY("B ran\\n"); }}
void mb_g(void) {{ SAY("B second page ran\\n"); }}
void g_a(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u); }}
void g_ab(void)
{{
    LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u);
    LOAD_B; SAY("loaded B\\n"); CALL(0x{FA:08x}u); CALL(0x{FB2:08x}u);
    LOAD_A; SAY("loaded A again\\n"); CALL(0x{FA:08x}u);
}}
void g_noc(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{GA:08x}u); SAY("not reached\\n"); }}
void g_nonstart(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{NONSTART:08x}u); SAY("not reached\\n"); }}
void g_other(void)
{{
    load({FILE_SECTOR['MODX.PAC'] + 1}, 1, 0x{OTHER_BASE:08x}u); SAY("loaded X\\n"); CALL(0x{OTHER_F:08x}u); SAY("not reached\\n");
}}
void g_like(void)
{{
    load({FILE_SECTOR['MODL.PAC'] + 1}, 1, 0x{LIKE_BASE:08x}u); SAY("loaded L\\n"); CALL(0x{LIKE_F:08x}u); SAY("not reached\\n");
}}
void g_read(void)
{{
    volatile unsigned *p = (volatile unsigned *)0x1000;
    LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u);
    SAY("about to read the mirror\\n");
    SAY("read %u\\n", *p);
}}
void g_tamper(void)
{{
    volatile unsigned char *p = (volatile unsigned char *)0x{FA + 8:08x}u;
    LOAD_A; SAY("loaded A\\n");
    *p ^= 1;
    SAY("changed one byte\\n");
    CALL(0x{FA:08x}u); SAY("not reached\\n");
}}
static DWORD WINAPI caller(LPVOID unused) {{ (void)unused; CALL(0x{FA:08x}u); return 0; }}
void g_thread(void)
{{
    LOAD_A; SAY("loaded A\\n");
    WaitForSingleObject(CreateThread(NULL, 0, caller, NULL, 0, NULL), 5000);
    SAY("not reached\\n");
}}
#define READY_CB (*(volatile unsigned *)0x80181b38u)
static void ready_via(unsigned target)
{{
    unsigned char pos[4], res[8];
    READY_CB = target;   /* set before the read starts: the handler is called for each sector as it is delivered */
    ps1_CdIntToPos({FILE_SECTOR['MODA.PAC'] + 1}, pos);
    ps1_CdControlB(2, pos, res);
    ps1_CdControlB(6, 0, res);
    ps1_CdReady(0, res);
}}
void g_cb_ok(void) {{ LOAD_A; SAY("loaded A\\n"); ready_via(0x{FA:08x}u); SAY("after the callback\\n"); }}
void g_cb_bad(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u); ready_via(0x{NONSTART:08x}u); SAY("after the callback\\n"); }}
void g_second(void)
{{
    load({FILE_SECTOR['MODK.PAC'] + 1}, 1, 0x{KBASE:08x}u); SAY("loaded K\\n"); CALL(0x{KF:08x}u); SAY("after K\\n");
}}
void g_second_noc(void)
{{
    load({FILE_SECTOR['MODK.PAC'] + 1}, 1, 0x{KBASE:08x}u); SAY("loaded K\\n"); CALL(0x{KG:08x}u); SAY("not reached\\n");
}}
void g_big(void)
{{
    load({FILE_SECTOR['MODY.PAC'] + 1}, 1, 0x{YBASE:08x}u); SAY("loaded Y\\n"); CALL(0x{YF:08x}u); SAY("not reached\\n");
}}
{attack_functions()}
void g_resident(void) {{ SAY("resident C ran\\n"); }}
void g_trial(void)
{{
    load({FILE_SECTOR['MODA.PAC'] + 1}, 1, 0x{TRIAL_DEST:08x}u); SAY("loaded data\\n");
    CALL(0x{G['g_resident']:08x}u); SAY("after\\n");
}}
void g_edge(void)
{{
    load({FILE_SECTOR['MODA.PAC'] + 1}, 1, 0x{TRIAL_DEST + 1:08x}u); SAY("not reached\\n");
}}
void g_inside(void)
{{
    load({FILE_SECTOR['MODA.PAC'] + 1}, 2, 0x{TRIAL_DEST + 0x400:08x}u); SAY("not reached\\n");
}}
void mp_f(void) {{ SAY("P ran\\n"); }}
void mq_f(void) {{ SAY("Q ran\\n"); }}
void g_shared(void)
{{
    load({FILE_SECTOR['MODP.PAC'] + 1}, 1, 0x{PBASE:08x}u); load({FILE_SECTOR['MODQ.PAC'] + 1}, 1, 0x{QBASE:08x}u);
    SAY("loaded P and Q\\n"); CALL(0x{PF:08x}u); SAY("not reached\\n");
}}
void g_shared_one(void)
{{
    load({FILE_SECTOR['MODP.PAC'] + 1}, 1, 0x{PBASE:08x}u); SAY("loaded P\\n"); CALL(0x{PF:08x}u); SAY("after P\\n");
}}
void g_shared_data(void)
{{
    load({FILE_SECTOR['MODP.PAC'] + 1}, 1, 0x{PBASE:08x}u); load({FILE_SECTOR['MODX.PAC'] + 1}, 1, 0x{QBASE:08x}u);
    SAY("loaded P and data\\n"); CALL(0x{PF:08x}u); SAY("after P\\n");
}}
void g_partial(void)
{{
    load({FILE_SECTOR['MODB.PAC'] + 1}, 1, 0x{BASE:08x}u); SAY("loaded the first sector of B\\n");
    CALL(0x{FA:08x}u); SAY("after B\\n");
}}
static void load_words(int sector, int words, unsigned dest)
{{
    unsigned char pos[4], res[8];
    ps1_CdIntToPos(sector, pos);
    ps1_CdControlB(2, pos, res);
    ps1_CdControlB(6, 0, res);
    ps1_CdReady(0, res);
    ps1_CdGetSector((void *)(size_t)dest, words);
    ps1_CdControlB(9, 0, res);
}}
#define LOAD_T4 load({FILE_SECTOR['MODT.PAC'] + 1}, 4, 0x{TBASE:08x}u)
#define LOAD_T1 load({FILE_SECTOR['MODT.PAC'] + 1}, 1, 0x{TBASE:08x}u)
#define LOAD_U1 load({FILE_SECTOR['MODU.PAC'] + 1}, 1, 0x{UBASE:08x}u)
void mt_f0(void) {{ SAY("T0 ran\\n"); }}
void mt_f1(void) {{ SAY("T1 ran\\n"); }}
void mt_fw(void) {{ SAY("TW ran\\n"); }}
void g_pa(void) {{ LOAD_T1; SAY("loaded the first sector of T\\n"); CALL(0x{T0:08x}u); SAY("not reached\\n"); }}
void g_pa_cb_unloaded(void) {{ LOAD_T1; SAY("loaded the first sector of T\\n"); ready_via(0x{T1:08x}u); SAY("accepted: not reached\\n"); }}
void g_pa_cb_first(void) {{ LOAD_T1; SAY("loaded the first sector of T\\n"); ready_via(0x{T0:08x}u); SAY("accepted: not reached\\n"); }}
void g_pu(void) {{ LOAD_U1; SAY("loaded the first sector of U\\n"); CALL(0x{U0:08x}u); SAY("not reached\\n"); }}
void g_four_t(void) {{ load_words({FILE_SECTOR['MODT.PAC'] + 1}, 1, 0x{T0:08x}u); SAY("copied four bytes\\n"); CALL(0x{T0:08x}u); SAY("not reached\\n"); }}
void g_four_t_cb(void) {{ load_words({FILE_SECTOR['MODT.PAC'] + 1}, 1, 0x{T0:08x}u); SAY("copied four bytes\\n"); ready_via(0x{T0:08x}u); SAY("accepted: not reached\\n"); }}
void g_four_u(void) {{ load_words({FILE_SECTOR['MODU.PAC'] + 1}, 1, 0x{U0:08x}u); SAY("copied four bytes\\n"); CALL(0x{U0:08x}u); SAY("not reached\\n"); }}
void g_full_t(void)
{{
    LOAD_T4; SAY("loaded T\\n"); CALL(0x{T0:08x}u); ready_via(0x{T1:08x}u); ready_via(0x{TW:08x}u); SAY("after the callbacks\\n");
}}
void g_over_t(void)
{{
    LOAD_T4; SAY("loaded T\\n"); CALL(0x{T0:08x}u);
    *(volatile unsigned char *)0x{T1:08x}u ^= 0xff;   /* the game's own write over an installed entry */
    SAY("overwrote the entry\\n"); ready_via(0x{T1:08x}u); SAY("accepted: not reached\\n");
}}
void g_rewrite_page(void)
{{
    LOAD_T4; SAY("loaded T\\n"); CALL(0x{T0:08x}u);
    load({FILE_SECTOR['MODA.PAC'] + 1}, 1, 0x{TBASE + 0x800:08x}u); SAY("the disc wrote over tf1\\n");
    CALL(0x{T0:08x}u); SAY("not reached\\n");
}}
void g_rewrite_entry(void)
{{
    LOAD_T4; SAY("loaded T\\n"); CALL(0x{T0:08x}u);
    load({FILE_SECTOR['MODA.PAC'] + 1}, 1, 0x{TBASE:08x}u); SAY("the disc wrote over tf0\\n");
    CALL(0x{T0:08x}u); SAY("not reached\\n");
}}
void g_direct_unloaded(void) {{ LOAD_T1; SAY("loaded the first sector of T\\n"); CALL(0x{T1:08x}u); SAY("not reached\\n"); }}
void g_gap(void)
{{
    load({FILE_SECTOR['MODA.PAC'] + 1}, 1, 0x{BASE:08x}u); SAY("loaded the first sector of A\\n");
    CALL(0x{BASE + 0x900:08x}u); SAY("not reached\\n");
}}
void g_many(void)
{{
    int k;
    for (k = 0; k < {MANY}; k++) {{ LOAD_A; CALL(0x{FA:08x}u); }}
    SAY("many done\\n");
}}
void mk_f(void) {{ SAY("K ran\\n"); }}
void g_data(void)
{{
    volatile unsigned char *p = (volatile unsigned char *)0x{BASE + 4:08x}u;
    LOAD_A; SAY("loaded A\\n");
    SAY("data %u %u\\n", p[0], p[0x1004]);
}}
"""

DOMAINS = r"""
#include "port.h"
const struct port_domain port_domains[] = { { "cd", port_cd_library } };
const unsigned port_domain_count = 1;
static const struct port_override o[] = { { 0, 0, 0 } };
const struct port_override *const port_override_sets[] = { o };
const unsigned port_override_set_count = 1;
"""


def pinned(seed: int, size: int) -> bytes:
    return hashlib.sha256(content(seed, size)).digest()


def c_bytes(b: bytes) -> str:
    return "{ " + ", ".join(f"0x{x:02x}" for x in b) + " }"


def tables_c(pin: bytes, unpinned: str | None = None) -> str:
    """The made-up tables. `unpinned` names an image whose hash the build did not give."""
    out = ['#include "port_tables.h"']
    syms = ("g_a", "g_ab", "g_noc", "g_nonstart", "g_other", "g_like", "g_read", "g_data", "ma_f", "mb_f", "mb_g", "mk_f", "mp_f", "mq_f", "mt_f0", "mt_f1", "mt_fw", "c_main", "c_big") + tuple(G)[8:]
    for sym in dict.fromkeys(syms):
        out.append(f"extern void {sym}(void);")
    out.append('static const char *const arch_a[] = { "MODA.PAC", "MODA2.PAC", 0 };')
    out.append('static const char *const arch_b[] = { "modb.pac", 0 };')
    out.append('static const char *const arch_l[] = { "MODL.PAC", 0 };')
    out.append('static const char *const arch_k[] = { "MODK.PAC", 0 };')
    out.append('static const char *const arch_y[] = { "MODY.PAC", 0 };')
    out.append('static const char *const arch_p[] = { "MODP.PAC", 0 };')
    out.append('static const char *const arch_q[] = { "MODQ.PAC", 0 };')
    out.append('static const char *const arch_t[] = { "MODT.PAC", 0 };')
    out.append('static const char *const arch_u[] = { "MODU.PAC", 0 };')
    images = [("ma", BASE, "0", 5, "arch_a", pinned(3, SIZE)), ("mb", BASE, "0", 5, "arch_b", pinned(5, SIZE)),
              ("ml", LIKE_BASE, '"ma"', 7, "arch_l", pinned(7, 0x800)), ("mk", KBASE, '"ma"', 8, "arch_k", pinned(13, 0x800)),
              ("my", YBASE, "0", 11, "arch_y", pinned(17, YSIZE)),
              ("mp", PBASE, "0", 12, "arch_p", pinned(19, 0x800)), ("mq", QBASE, "0", 13, "arch_q", pinned(23, 0x800)),
              ("mt", TBASE, "0", 14, "arch_t", pinned(29, 0x2000)), ("mu", UBASE, "0", 15, "arch_u", pinned(31, 0x2000))]
    for n, im in enumerate(images):
        out.append(f"static const unsigned char sha_{n}[32] __attribute__((unused)) = {c_bytes(im[5])};")
    out.append("const struct port_image port_images[] = {")
    for n, (name, base, like, slot, arch, _) in enumerate(images):
        out.append(f'    {{ "{name}", 0x{base:08x}u, {like}, {slot}, {arch}, {"0" if name == unpinned else f"sha_{n}"} }},')
    out.append("};")
    out.append(f"const unsigned port_image_count = {len(images)};")
    fns = [(tl.ENTRY_C, "c_main", "c_main", -1), (tl.BIG_C, "c_big", "c_big", -1)]
    fns += [(a, n, n, -1) for n, a in G.items()]
    fns += [(FA, "ma_f", "ma_f", 0), (FA, "mb_f", "mb_f", 1), (FB2, "mb_g", "mb_g", 1), (KF, "mk_f", "mk_f", 3),
             (PF, "mp_f", "mp_f", 5), (QF, "mq_f", "mq_f", 6),
             (T0, "mt_f0", "mt_f0", 7), (T1, "mt_f1", "mt_f1", 7), (TW, "mt_fw", "mt_fw", 7)]
    fns.sort(key=lambda f: (f[3], f[0]))
    out.append("const struct port_function port_functions[] = {")
    for a, name, sym, image in fns:
        out.append(f'    {{ 0x{a:08x}u, (void *){sym}, "{name}", {image} }},')
    out.append("};")
    out.append(f"const unsigned port_function_count = {len(fns)};")
    abs_ = [(a, n, -1, 1) for n, a in LIB.items()] + [(GA, "func_80150020_ma", 0, 0), (GA, "func_80150020_mb", 1, 0), (KG, "func_80180020_mk", 3, 0),
                                                       (YF, "func_801f8010_my", 4, 0),
                                                       (TG, "func_801b0020_mt", 7, 0), (TZ, "func_801b0a00_mt", 7, 0), (U0, "func_801b8010_mu", 8, 0), (U1, "func_801b8900_mu", 8, 0)]
    abs_.sort(key=lambda f: (f[2], f[0]))
    out.append("const struct port_absent port_absents[] = {")
    for a, name, image, lib in abs_:
        out.append(f'    {{ 0x{a:08x}u, "{name}", {image}, {lib} }},')
    out.append("};")
    out.append(f"const unsigned port_absent_count = {len(abs_)};")
    out.append(f"const unsigned char port_program_sha256[32] = {{ {tl.sha_bytes(pin)} }};")
    return "\n".join(out) + "\n"


class ModRig(tl.Rig):
    built_parts: dict = {}

    def runtime_objects(self):
        if not self.objects:
            self.objects = [self.compile(SRC / f"{n}.c") for n in tl.RUNTIME]
            stubs = self.work / "stubs.c"
            stubs.write_text(tl.STUBS)
            self.objects.append(self.compile(stubs))
        return self.objects

    def program(self, pin: bytes, unpinned: str | None = None) -> Path:
        """The runtime with the made-up tables for the program `pin` (the tables carry its SHA-256)."""
        key = hashlib.sha256(pin).hexdigest()[:8] + (unpinned or "")
        if key not in self.built:
            if "fixed" not in self.built_parts:
                parts = []
                for tag, text in (("domains", DOMAINS), ("mbegin", tl.MARK_BEGIN), ("game", GAME), ("mend", tl.MARK_END)):
                    path = self.work / f"mod-{tag}.c"
                    path.write_text(text)
                    parts.append(self.compile(path, ["-Wno-unused-function"] if tag == "game" else None))
                self.built_parts["fixed"] = parts
            path = self.work / f"mod-tables-{key}.c"
            path.write_text(tables_c(pin, unpinned))
            objs = list(self.runtime_objects()) + self.built_parts["fixed"] + [self.compile(path)]
            names = {n: a for n, a in LIB.items()} | G | {"ma_f": FA, "mb_f": FA, "mb_g": FB2}
            defs = [f"-Wl,--defsym,_ps1_{n}=0x{a:08x}" for n, a in names.items()]
            out = self.work / f"sfa2-mod-{key}.exe"
            proc = subprocess.run([self.cc, "-o", str(out), *map(str, objs), *tl.LINK_FLAGS, *defs], capture_output=True, text=True, timeout=300)
            if proc.returncode != 0:
                raise RuntimeError("the runtime did not link:\n" + proc.stderr.strip())
            self.built[key] = out
        return self.built[key]

    def go(self, tag: str, entry: int, timeout: int = 60, files: dict | None = None, unpinned: str | None = None) -> tuple[int, list[str]]:
        data = tl.program(entry)
        exe = self.program(data, unpinned)
        img = disc_image(data, files)
        path = self.work / f"{tag}.bin"
        img.write(path)
        proc = subprocess.run([*self.prefix, str(exe), self.native(path)], capture_output=True, text=True, timeout=timeout)
        lines = proc.stdout.replace("\r\n", "\n").splitlines()
        if f"start: 0x{entry:08x}" in lines:
            lines = lines[lines.index(f"start: 0x{entry:08x}") + 1:]
        return proc.returncode, lines


def verdict(got, want):
    return None if got == want else f"got {got!r}, wanted {want!r}"


def mline(name: str, base: int, c: int, a: int) -> str:
    return f"module: {name} at 0x{base:08x}, {c} C jumps, {a} without C"


def cases(rig: ModRig):
    yield "fixture-the-two-modules-share-an-address-and-an-slot", verdict((BASE, 5), (BASE, 5))
    yield "fixture-archives-are-sector-aligned-and-hold-their-chunk", verdict(
        (len(FILES["MODA.PAC;1"]) % 2048, len(FILES["MODA.PAC;1"]) - 0x800), (0, SIZE))

    status, lines = rig.go("a", G["g_a"])
    yield "load-a-call-its-function-the-c-of-a-runs", verdict((status, lines), (0, ["loaded A", mline("ma", BASE, 1, 1), "A ran", "stop: main returned"]))

    status, lines = rig.go("ab", G["g_ab"])
    want = ["loaded A", mline("ma", BASE, 1, 1), "A ran",
            "loaded B", mline("mb", BASE, 2, 1), "B ran", "B second page ran",
            "loaded A again", mline("ma", BASE, 1, 1), "A ran", "stop: main returned"]
    yield "load-b-over-a-the-c-of-b-runs-then-a-again-each-with-its-line-and-the-second-page-without-one", verdict((status, lines), (0, want))

    status, lines = rig.go("data", G["g_data"])
    want = ["loaded A", f"data {content(3, SIZE)[4]} {content(3, SIZE)[0x1008]}", "stop: main returned"]
    yield "the-data-of-a-loaded-module-is-readable-without-a-module-line", verdict((status, lines), (0, want))

    status, lines = rig.go("noc", G["g_noc"])
    yield "a-function-without-c-stops-with-its-name", verdict(
        (status, lines), (3, ["loaded A", mline("ma", BASE, 1, 1), f"stop: no C yet for func_80150020_ma (0x{GA:08x})"]))

    sector = FILE_SECTOR["MODA.PAC"] + 1 + (NONSTART - BASE) // 2048   # the sector that wrote the word at that address
    status, lines = rig.go("nonstart", G["g_nonstart"])
    want = ["loaded A", f"stop: call to 0x{NONSTART:08x}, which no module can be placed at: the page was written from sector {sector} of moda.pac "
            f"(image ma is there but 0x{NONSTART:08x} is no function start of it); images the tables have at that address: ma, mb"]
    yield "an-address-in-a-loaded-page-that-is-no-function-start-is-refused-with-sector-file-and-images", verdict((status, lines), (5, want))

    status, lines = rig.go("other", G["g_other"])
    want = ["loaded X", f"stop: call to 0x{OTHER_F:08x}, which no module can be placed at: the page was written from sector {FILE_SECTOR['MODX.PAC'] + 1} of modx.pac "
            "(no image of the tables has that archive, slot and address); images the tables have at that address: none"]
    yield "a-module-no-image-fits-is-refused-with-the-same-kind-of-line", verdict((status, lines), (5, want))

    status, lines = rig.go("like", G["g_like"])
    want = ["loaded L", f"stop: call to 0x{LIKE_F:08x} in image ml, which is a like placement of ma; its C is not built yet"]
    yield "a-like-image-ends-with-a-line-that-names-it-and-says-its-c-is-not-built", verdict((status, lines), (3, want))

    status, lines = rig.go("read", G["g_read"])
    ok = (status == 10 and lines[:4] == ["loaded A", mline("ma", BASE, 1, 1), "A ran", "about to read the mirror"]
          and len(lines) == 5 and lines[4].startswith("stop: crash: the game used the PS1's RAM mirror at 0x00001000 (read)"))
    yield "a-fault-of-the-kind-read-is-not-swallowed", None if ok else f"status {status}, lines {lines!r}"

    # ---- identity before jumps ----
    sec_a = FILE_SECTOR["MODA.PAC"] + 1
    changed = dict(FILES)
    changed["MODA.PAC;1"] = archive(5, SIZE, 4)    # same layout, other bytes: not the content the tables pin
    status, lines = rig.go("wrong-content", G["g_a"], files=changed)
    want = ["loaded A", f"refused: image ma: the chunk at sector {sec_a} of moda.pac is not the pinned content (its SHA-256 differs from the build's); no jump of it was written"]
    yield "a-chunk-that-is-not-the-pinned-content-ends-with-a-line-naming-the-image-and-no-jump-is-written", verdict((status, lines), (2, want))
    status, lines = rig.go("unpinned", G["g_a"], unpinned="ma")
    yield "an-image-the-build-pinned-nothing-for-gets-no-jump", verdict((status, lines), (2, ["loaded A", "refused: image ma has no pinned content in this build; no jump of it was written"]))
    status, lines = rig.go("tamper", G["g_tamper"])
    want = ["loaded A", "changed one byte", f"refused: image ma: the memory at 0x{FA:08x} is not the pinned content any more (the chunk at sector {sec_a} of moda.pac was changed after the disc wrote it); no jump of it was written"]
    yield "a-module-the-game-changed-after-the-disc-wrote-it-gets-no-jump", verdict((status, lines), (2, want))
    status, lines = rig.go("wrong-content-b", G["g_ab"], files=dict(FILES, **{"MODB.PAC;1": archive(5, SIZE, 6)}))
    yield "the-identity-is-checked-for-the-image-that-is-loaded-now-not-the-one-before", None if (status == 2 and "A ran" in lines and any("image mb:" in l for l in lines) and "B ran" not in lines) else f"status {status}, lines {lines!r}"

    # ---- the archive is the user's file ----
    names = {"MODH1": "its header does not look like an archive", "MODH2": "its header gives a chunk that runs past the end of the file",
             "MODH3": "its header gives a chunk that runs past the end of the file", "MODH4": "its header does not look like an archive",
             "MODH5": "its header cannot be read"}
    for k, name in enumerate(ATTACKS):
        status, lines = rig.go(f"atk-{name}", G[f"g_atk{k}"])
        sector = FILE_SECTOR[name + ".PAC"] + (0 if name == "MODH5" else 1)
        want = ["loaded", f"stop: call to 0x{ATK + 0x1000 * k + 0x10:08x}, which no module can be placed at: the page was written from sector {sector} of {name.lower()}.pac "
                f"({names[name]}); images the tables have at that address: none"]
        yield f"archive-{name.lower()}-ends-with-a-line-and-writes-no-jump", verdict((status, lines), (5, want))
    status, lines = rig.go("big", G["g_big"])
    want = ["loaded Y", f"stop: call to 0x{YF:08x}, which no module can be placed at: the page was written from sector {FILE_SECTOR['MODY.PAC'] + 1} of mody.pac "
            f"(the chunk would end outside the PS1's RAM); images the tables have at that address: my"]
    yield "a-chunk-that-would-end-outside-the-ram-ends-with-a-line", verdict((status, lines), (5, want))

    # ---- who may fault, and the addresses the game hands over ----
    status, lines = rig.go("thread", G["g_thread"])
    ok = status == 10 and lines[0] == "loaded A" and len(lines) == 2 and lines[1].startswith(f"stop: crash: access violation (execute 0x{FA:08x})")
    yield "an-execute-fault-on-a-thread-that-is-not-the-games-is-not-placed-it-is-the-crash-line", None if ok else f"status {status}, lines {lines!r}"
    status, lines = rig.go("cb-ok", G["g_cb_ok"])
    ran = [l for i, l in enumerate(lines) if l != "A ran" or lines[i - 1] != "A ran"]   # the handler runs once for each sector delivered
    yield "a-module-function-handed-over-as-the-ready-callback-is-placed-and-runs", verdict(
        (status, ran), (0, ["loaded A", mline("ma", BASE, 1, 1), "A ran", "after the callback", "stop: main returned"]))
    status, lines = rig.go("cb-bad", G["g_cb_bad"])
    yield "an-address-inside-a-module-that-is-no-function-start-is-refused-as-a-callback", verdict(
        (status, lines), (12, ["loaded A", mline("ma", BASE, 1, 1), "A ran", f"refused: ready callback 0x{NONSTART:08x} is not a function this program installed"]))

    # ---- resident code is never a module's ----
    status, lines = rig.go("trial", G["g_trial"])
    yield "a-data-sector-ending-in-the-page-where-resident-code-begins-leaves-the-page-executable-and-the-resident-c-runs", verdict(
        (status, lines), (0, ["loaded data", "resident C ran", "after", "stop: main returned"]))
    status, lines = rig.go("edge", G["g_edge"])
    want = [f"stop: the game overwrote resident code at 0x{G['g_a']:08x} (g_a): a copy of 2048 bytes to 0x{TRIAL_DEST + 1:08x} reaches the jump written there, and the C stays while the bytes would not"]
    yield "a-copy-that-reaches-the-first-byte-of-a-resident-function-ends-with-a-line-that-names-it", verdict((status, lines[-1:]), (6, want))
    status, lines = rig.go("inside", G["g_inside"])
    want = [f"stop: the game overwrote resident code at 0x{G['g_a']:08x} (g_a): a copy of 2048 bytes to 0x{TRIAL_DEST + 0x400:08x} reaches the jump written there, and the C stays while the bytes would not"]
    yield "a-copy-over-several-resident-functions-names-the-lowest", verdict((status, lines[-1:]), (6, want))

    # ---- where each word came from ----
    status, lines = rig.go("shared", G["g_shared"])
    want = ["loaded P and Q", f"refused: images mp and mq share the page 0x{PBASE & ~0xfff:08x}: the function of mq at 0x{QF:08x} lies in it, mq is not placed, and its first call would no longer fault once the page is executable"]
    yield "two-images-sharing-a-page-the-second-not-placed-is-refused-naming-both-and-the-page", verdict((status, lines), (2, want))
    status, lines = rig.go("shared-one", G["g_shared_one"])
    yield "an-image-beginning-in-the-middle-of-a-page-with-nothing-else-of-the-disc-in-it-is-placed", verdict(
        (status, lines), (0, ["loaded P", mline("mp", PBASE, 1, 0), "P ran", "after P", "stop: main returned"]))
    status, lines = rig.go("shared-data", G["g_shared_data"])
    yield "a-page-shared-with-disc-data-that-is-no-image-is-placed-for-the-image-that-is-called", verdict(
        (status, lines), (0, ["loaded P and data", mline("mp", PBASE, 1, 0), "P ran", "after P", "stop: main returned"]))
    status, lines = rig.go("partial", G["g_partial"])
    yield "only-the-functions-whose-bytes-the-chunk-wrote-get-a-jump-the-image-s-second-page-function-is-left-alone", verdict(
        (status, lines), (0, ["loaded the first sector of B", mline("mb", BASE, 1, 1), "B ran", "after B", "stop: main returned"]))
    # ---- installation is per entry ----
    sec_t = FILE_SECTOR["MODT.PAC"] + 1
    def refusal(img, name, addr):
        return (f"refused: image {img}: the entry {name} at 0x{addr:08x} cannot be installed: the bytes of this entry did not all come from the pinned chunk "
                f"(at sector {{sec}} of {{file}}), and the page 0x{addr & ~0xfff:08x} would become executable without a jump there; no jump of the image was written")
    t_line = lambda name, addr: refusal("mt", name, addr).format(sec=sec_t, file="modt.pac")
    u_line = lambda name, addr: refusal("mu", name, addr).format(sec=FILE_SECTOR["MODU.PAC"] + 1, file="modu.pac")
    status, lines = rig.go("pa", G["g_pa"])
    yield "only-the-first-sector-loaded-the-page-is-not-made-executable-for-the-first-function-the-entry-that-was-not-loaded-is-named", verdict(
        (status, lines), (2, ["loaded the first sector of T", t_line("mt_f1", T1)]))
    status, lines = rig.go("pa-cb-first", G["g_pa_cb_first"])
    yield "only-the-first-sector-loaded-the-first-function-handed-over-as-a-callback-is-not-accepted", verdict(
        (status, lines), (2, ["loaded the first sector of T", t_line("mt_f1", T1)]))
    status, lines = rig.go("pa-cb-unloaded", G["g_pa_cb_unloaded"])
    yield "only-the-first-sector-loaded-a-later-entry-on-the-page-handed-over-as-a-callback-is-refused-and-never-reported-accepted", verdict(
        (status, lines), (12, ["loaded the first sector of T", f"refused: ready callback 0x{T1:08x} is not a function this program installed"]))
    status, lines = rig.go("pu", G["g_pu"])
    yield "the-same-for-an-entry-without-c-the-stop-path", verdict((status, lines), (2, ["loaded the first sector of U", u_line("func_801b8900_mu", U1)]))
    status, lines = rig.go("four-t", G["g_four_t"])
    yield "only-four-bytes-of-an-entry-copied-nothing-is-installed-and-the-placement-is-refused", verdict(
        (status, lines), (2, ["copied four bytes", t_line("mt_f0", T0)]))
    status, lines = rig.go("four-t-cb", G["g_four_t_cb"])
    yield "only-four-bytes-of-an-entry-copied-it-handed-over-as-a-callback-is-refused", verdict(
        (status, lines), (2, ["copied four bytes", t_line("mt_f0", T0)]))
    status, lines = rig.go("four-u", G["g_four_u"])
    yield "only-four-bytes-of-an-entry-without-c-copied-the-same", verdict((status, lines), (2, ["copied four bytes", u_line("func_801b8010_mu", U0)]))
    status, lines = rig.go("direct-unloaded", G["g_direct_unloaded"])
    ok = status == 10 and lines[0] == "loaded the first sector of T" and len(lines) == 2 and lines[1].startswith(f"stop: crash: access violation (execute 0x{T1:08x})")
    yield "a-direct-call-to-an-entry-whose-bytes-were-never-loaded-is-no-module-call", None if ok else f"status {status}, lines {lines!r}"
    status, lines = rig.go("full-t", G["g_full_t"])
    ran = [l for i, l in enumerate(lines) if not (l in ("T0 ran", "T1 ran", "TW ran") and lines[i - 1] == l)]
    yield "the-whole-chunk-loaded-every-entry-is-installed-and-accepted-while-its-jump-is-there", verdict(
        (status, [l for l in ran if l in ("loaded T", "after the callbacks") or l.startswith("module:")]), (0, ["loaded T", mline("mt", TBASE, 3, 2), "after the callbacks"]))
    status, lines = rig.go("over-t", G["g_over_t"])
    yield "an-installed-entry-the-game-wrote-over-is-refused-as-a-callback-with-a-line-that-says-its-jump-is-gone", verdict(
        (status, lines[-2:]), (12, ["overwrote the entry", f"refused: ready callback 0x{T1:08x} is an entry of a module whose jump is no longer there"]))
    status, lines = rig.go("rewrite-page", G["g_rewrite_page"])
    yield "a-later-disc-write-to-the-page-takes-the-entries-away-and-the-next-call-is-refused-naming-the-entry-that-lost-its-bytes", verdict(
        (status, lines[-2:]), (2, ["the disc wrote over tf1", t_line("mt_f1", T1)]))
    status, lines = rig.go("rewrite-entry", G["g_rewrite_entry"])
    ok = status == 5 and lines[-2] == "the disc wrote over tf0" and "no image of the tables has that archive, slot and address" in lines[-1]
    yield "a-later-disc-write-over-the-entry-itself-and-the-next-call-is-placed-for-what-is-there-now-and-refused", None if ok else f"status {status}, lines {lines[-2:]!r}"

    status, lines = rig.go("gap", G["g_gap"])
    ok = status == 10 and lines[0] == "loaded the first sector of A" and len(lines) == 2 and lines[1].startswith(f"stop: crash: access violation (execute 0x{BASE + 0x900:08x})")
    yield "a-call-at-an-address-whose-bytes-did-not-come-from-the-disc-is-no-module-call-it-is-the-crash-line", None if ok else f"status {status}, lines {lines!r}"
    status, lines = rig.go("many", G["g_many"], timeout=60)
    yield "many-faults-in-a-row-with-the-timer-running-end-without-a-hang", verdict(
        (status, lines.count("A ran"), [l for l in lines if l not in ("A ran", "loaded A") and not l.startswith("module:")]), (0, MANY, ["many done", "stop: main returned"]))

    # ---- a second placement has its own table ----
    status, lines = rig.go("second", G["g_second"])
    yield "a-like-image-with-a-table-of-its-own-runs-its-c", verdict((status, lines), (0, ["loaded K", mline("mk", KBASE, 1, 1), "K ran", "after K", "stop: main returned"]))
    status, lines = rig.go("second-noc", G["g_second_noc"])
    yield "a-function-of-a-like-image-that-has-no-c-there-stops-by-name", verdict(
        (status, lines), (3, ["loaded K", mline("mk", KBASE, 1, 1), f"stop: no C yet for func_80180020_mk (0x{KG:08x})"]))

    shape = [l for l in lines if l.startswith("module:")]
    import re
    yield "the-module-line-has-its-shape", None if shape and all(re.fullmatch(r"module: \w+ at 0x[0-9a-f]{8}, \d+ C jumps, \d+ without C", l) for l in shape) else f"{shape!r}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostmodules-", dir=BUILD))
    try:
        rig = ModRig(args.cc, args.run.split(), work)
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
