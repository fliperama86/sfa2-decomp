#!/usr/bin/env python3
"""Controls for the graphics layer: gpu.c, linked like the game's C, with PsyZ underneath.

    python3 test_hostgpu.py --cc CROSS_CC --psyz-build DIR [--run PREFIX] [--gpu-source FILE] [--memory-source FILE] [--group NAMES]

DIR is a build folder of psyzbuild.py (it holds psyz.json, the library and the
headers), for example
`python3 port/tools/psyzbuild.py --psyz port/external/psyz --build SCRATCH/psyz`.
The patch and the tool are tested by test_psyzbuild.py; this file starts from a
built PsyZ. --group NAMES runs only those sections of the cases (basic, images, tables,
walker, warmup, prims, stream, display, memory; the build cases always run), for trying a changed
copy of the layer against the cases that should notice it. --gpu-source FILE and --memory-source FILE test another copy of gpu.c, memory.c in place of the real ones
(for trying a changed copy against the cases; the default is the real file).

The runtime's gpu.c, linked like the game's C with a made-up game of this file's
own (a C program written below, using the library only by the names of
port_gpu_library[], in Sony's layouts built byte by byte in the PS1's RAM) and
stand-ins for the frame clock's port_tick, port_halt and port_callbacks_reset.
Every expected value is worked out HERE from Sony's documented layouts, never
read back from the runtime. Cases:
- gpu.c compiles without a warning with PsyZ's headers, and to empty tables
  without them;
- the table lists the graphics routines the game calls and no routine that takes
  an address to call later (no draw-sync callback is stored);
- the routines on Sony's layouts (the ordering table setters, AddPrim and its
  kin, MargePrim, the Set* setters, GetTPage, GetClut, SetDef*Env, SetDrawMode,
  SetDrawEnv's words, and that SetDrawEnv writes nothing past its packet)
  produce the bytes worked out here;
- the picture: a flat rectangle and a textured sprite from a made-up texture
  through a hand-built ordering table, in a window for about three seconds; the
  picture is read back with StoreImage and compared with the 64 x 48 pixels
  computed here, pixel by pixel;
- a command kind the walker does not decode (polylines, the copy commands with
  data, the interrupt request) in any position of a packet ends the program
  with a line naming the kind, the packet and the word, and nothing of the
  packet is drawn; the console's one-word no-operation kinds are accepted;
- every command of a packet is checked before PsyZ sees the packet (incomplete
  commands in every position, the two probes of the review, a stale conversion
  buffer, several commands of different kinds in one packet, 255 words);
- the game's data read as an attacker would: lists ended wrongly (a cycle, a
  link outside RAM, a packet running past RAM, a list not in RAM), each with the
  last valid value and the first invalid one of the start, a link, a packet's
  end and the count; image routines with rectangles at and past the frame
  buffer's edges (a stop, never an overflow of a size), buffers
  and rectangles at and past the end of RAM (a stop), ordering tables at and past
  the end of RAM; each stop is one line and the graphics status;
- both sides of the conversion: a list that fills PsyZ's buffer to the last
  packet that fits and the first that does not, packets of the largest length
  (255 words), every primitive type the game uses at extreme values, an
  ordering table of the game's largest size (30 entries);
- a closed window ends the program with status 0 after the line
  `stop: window closed`, and nothing after the present runs;
- presenting takes at most one display refresh (PsyZ's frame limiter is off; a window that cannot tear is
  paced by its swapchain, which the port does not control).

The trial program runs with SDL's offscreen video driver (it sets SDL_VIDEODRIVER itself before its first
graphics call): PsyZ and its GPU device run as usual but no window is created, so nothing appears on the desktop
and nothing takes the keyboard focus. The published layer has no option for it; the real program shows its
window as before. (The pictures read back are the frame buffer's, which is what the cases compare.)

PREFIX is a command prefix to start the Windows program; without it the program
is started directly (on Windows, or from a shell under WSL, where paths are
converted with wslpath). When no Windows program can be started the program
cases are not run and this says so and ends with status 2. A program that
outlasts its time ends with the process that started it (a case shows that it
is gone); nothing is stopped by name. This file used to stop its program with
taskkill.exe by image name, and two runs on one machine (two checkouts) then
stopped each other's programs: the stopped one printed nothing and its case
failed. The program's name also carries a random token of the run, so that an
older copy of this file running beside it cannot stop this run's program. The
run's files live in a folder of its own under port/build/, removed at the end.
"""

from __future__ import annotations

import argparse
import json
import secrets
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
PORT = HERE.parent
SRC = PORT / "src"
BUILD = PORT / "build"
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]


def image_name(token: str) -> str:
    """The trial program's file name for one run: not shared with any other run on the machine (another
    checkout, an older copy of this file that stops its program by name)."""
    return f"hostgpu-trial-{token}.exe"


EXE = image_name(secrets.token_hex(4))
SECTIONS = ["build", "basic", "images", "tables", "walker", "warmup", "prims", "stream", "display", "memory"]
EXIT_GRAPHICS = 7

TRIAL_C = r'''
/* The made-up "game" for the graphics layer's controls (test_hostgpu.py).
 *
 * It is linked like the game's C: the runtime's gpu.c, the port's memory.c (the PS1's RAM at
 * its own addresses) and stand-ins for the driver's port_tick, port_halt and
 * port_callbacks_reset. It reaches the library the way the game's jumps do: by the names in
 * port_gpu_library[], with Sony's layouts built byte by byte in RAM.
 * Nothing here knows PsyZ's layouts. The test program (Python) works out what every output
 * must be from Sony's documented layouts and compares.
 *
 * usage: hostgpu_trial MODE [FILE]
 *   semantics     the pure routines on Sony's layouts; prints `NAME: hex words` lines
 *   picture FILE  draw a flat rectangle and a textured sprite through an ordering table,
 *                 read the picture back with StoreImage into FILE (64x48 16-bit pixels), show
 *                 the window for about three seconds
 *   unknown FILE  as picture, with packets of kinds PsyZ does not decode in the list
 *   cycle | outside | past | notram   lists that are ended wrongly (each stops)
 *   closed        the window is told to close during a present
 *   presents      300 presents in a row, timed
 *   names         print the names in the table
 *   nodisplay     PsyZ cannot start: the first call ends the program with a line
 *   stack         port_game_span on every kind of pointer, from the main stack and from a fiber's
 *   roundtrip-main | roundtrip-fiber   LoadImage and StoreImage through locals of the game code, on the main stack and in a fiber
 *   heapload      LoadImage from a heap buffer: a stop
 *   stacknode     DrawOTag with a first node on the stack: a stop
 *   script OP...  operations given as arguments (see script())
 */
#include "port.h"
#include "gpu.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <windows.h>

#ifdef TRIAL_SDL
#include <SDL3/SDL_events.h>
#endif

/* ---- stand-ins for the driver ---- */

static unsigned ticks;
static unsigned resets;
static int presenting = 1;

/* After the script operation `recover`, a stop does not end the trial: it returns to the script, which goes on with
 * the next operation (so that a case can read the picture after a refused list). */
static jmp_buf recover_to;
static int recoverable;

void port_halt(int status, const char *fmt, ...)
{
    va_list ap;
    printf("stop: ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    if (recoverable) longjmp(recover_to, status);
    exit(status);
}

void port_tick(void)
{
    ticks++;
    if (presenting) port_gpu_present();
    Sleep(16);
}

void port_callbacks_reset(void) { resets++; }

/* ---- the library by name ---- */

static void *find(const struct port_library *table, const char *name)
{
    for (; table->name; table++) if (!strcmp(table->name, name)) return table->host;
    printf("t: no host routine named %s\n", name);
    fflush(stdout);
    exit(90);
}

static void *lib(const char *name) { return find(port_gpu_library, name); }

typedef int      (*f_ii)(int);
typedef void     (*f_vi)(int);
typedef int      (*f_rv)(void);
typedef int      (*f_rp)(void *, void *);
typedef int      (*f_rpiii)(void *, int, int, int);
typedef int      (*f_rpii)(void *, int, int);
typedef void    *(*f_pp)(void *);
typedef void    *(*f_ppi)(void *, int);
typedef void     (*f_vpp)(void *, void *);
typedef void     (*f_vppp)(void *, void *, void *);
typedef void     (*f_vp)(void *);
typedef void     (*f_vpi)(void *, int);
typedef void     (*f_vpiiip)(void *, int, int, int, void *);
typedef unsigned (*f_uiiii)(int, int, int, int);
typedef unsigned (*f_uii)(int, int);
typedef void    *(*f_ppiiii)(void *, int, int, int, int);

#define W(a) ((uint32_t *)(uintptr_t)(a))
#define B(a) ((uint8_t *)(uintptr_t)(a))
#define H(a) ((int16_t *)(uintptr_t)(a))

static void dump(const char *name, uint32_t address, unsigned words)
{
    unsigned i;
    printf("t: %s:", name);
    for (i = 0; i < words; i++) printf(" %08x", W(address)[i]);
    printf("\n");
}

static void dumpval(const char *name, unsigned long value) { printf("t: %s: %08lx\n", name, value); }

static void map_or_die(void)
{
    char err[PORT_ERR];
    if (port_map(err, sizeof err) != 0) {
        printf("t: %s\n", err);
        fflush(stdout);
        exit(91);
    }
}

/* ---- semantics ---- */

static void semantics(void)
{
    unsigned base = 0x80100000u;
    uint32_t *r;

    /* ClearOTag(4 entries) then ClearOTagR(4 entries), each filled with 0xdeadbeef first. */
    {
        unsigned i;
        for (i = 0; i < 8; i++) W(base)[i] = 0xdeadbeefu;
        r = ((f_ppi)lib("ClearOTag"))(W(base), 4);
        dump("ClearOTag", base, 5);
        dumpval("ClearOTag-returns-index", (unsigned long)(r - W(base)));
        for (i = 0; i < 8; i++) W(base + 0x100)[i] = 0xdeadbeefu;
        r = ((f_ppi)lib("ClearOTagR"))(W(base + 0x100), 4);
        dump("ClearOTagR", base + 0x100, 5);
        dumpval("ClearOTagR-returns-index", (unsigned long)(r - W(base + 0x100)));
    }

    /* AddPrim keeps the length bytes; AddPrims likewise. */
    W(0x80101000)[0] = 0x07000000u | 0x00123456u;  /* ot: len 7, link 0x123456 */
    W(0x80101100)[0] = 0x05000000u | 0x00abcdefu;  /* p0 */
    ((f_vpp)lib("AddPrim"))(W(0x80101000), W(0x80101100));
    dump("AddPrim-ot", 0x80101000, 1);
    dump("AddPrim-p", 0x80101100, 1);
    W(0x80101200)[0] = 0x07000000u | 0x00123456u;
    W(0x80101300)[0] = 0x03000000u | 0x00111111u;  /* p0 */
    W(0x80101400)[0] = 0x04000000u | 0x00222222u;  /* p1 */
    ((f_vppp)lib("AddPrims"))(W(0x80101200), W(0x80101300), W(0x80101400));
    dump("AddPrims-ot", 0x80101200, 1);
    dump("AddPrims-p0", 0x80101300, 1);
    dump("AddPrims-p1", 0x80101400, 1);

    /* MargePrim: lengths 3 and 4 -> 8; 16 and 16 -> refused (0x21); 16 and 15 -> 0x20. */
    {
        static const unsigned lens[3][2] = { { 3, 4 }, { 0x10, 0x10 }, { 0x10, 0x0f } };
        unsigned i;
        for (i = 0; i < 3; i++) {
            int result;
            W(0x80102000)[0] = (lens[i][0] << 24) | 0x00aaaaaau;
            W(0x80102100)[0] = (lens[i][1] << 24) | 0x00bbbbbbu;
            result = ((int (*)(void *, void *))lib("MargePrim"))(W(0x80102000), W(0x80102100));
            printf("t: MargePrim%u: result %d tag0 %08x tag1 %08x\n", i, result, W(0x80102000)[0], W(0x80102100)[0]);
        }
    }

    /* SetSemiTrans on, then off. */
    W(0x80103000)[1] = 0x64332211u;
    ((f_vpi)lib("SetSemiTrans"))(W(0x80103000), 1);
    dump("SetSemiTrans-on", 0x80103000, 2);
    ((f_vpi)lib("SetSemiTrans"))(W(0x80103000), 0);
    dump("SetSemiTrans-off", 0x80103000, 2);

    /* The setters: tag link and colour kept. */
    {
        static const char *names[] = { "SetPolyFT4", "SetSprt16", "SetSprt", "SetTile" };
        unsigned i;
        for (i = 0; i < 4; i++) {
            W(0x80104000 + 0x100 * i)[0] = 0x00abcdefu;
            W(0x80104000 + 0x100 * i)[1] = 0x00332211u;
            ((f_vp)lib(names[i]))(W(0x80104000 + 0x100 * i));
            printf("t: %s:", names[i]);
            printf(" %08x %08x\n", W(0x80104000 + 0x100 * i)[0], W(0x80104000 + 0x100 * i)[1]);
        }
    }

    /* GetTPage, GetClut. */
    {
        static const int tp[][4] = { { 0, 0, 0, 0 }, { 2, 1, 512, 0 }, { 1, 2, 640, 256 }, { 3, 3, 1023, 511 }, { 2, 0, 320, 256 } };
        unsigned i;
        for (i = 0; i < 5; i++)
            printf("t: GetTPage %d %d %d %d: %04x\n", tp[i][0], tp[i][1], tp[i][2], tp[i][3], ((f_uiiii)lib("GetTPage"))(tp[i][0], tp[i][1], tp[i][2], tp[i][3]));
        for (i = 0; i < 4; i++) {
            static const int xy[][2] = { { 0, 0 }, { 16, 1 }, { 640, 480 }, { 1008, 511 } };
            printf("t: GetClut %d %d: %04x\n", xy[i][0], xy[i][1], ((f_uii)lib("GetClut"))(xy[i][0], xy[i][1]));
        }
    }

    /* SetDefDrawEnv, SetDefDispEnv over memory filled with 0xaa. */
    {
        void *e;
        memset(B(0x80105000), 0xaa, 0x5c);
        e = ((f_ppiiii)lib("SetDefDrawEnv"))(W(0x80105000), 8, 16, 320, 240);
        printf("t: SetDefDrawEnv-returns-env: %d\n", e == (void *)W(0x80105000));
        dump("SetDefDrawEnv", 0x80105000, 0x5c / 4);
        memset(B(0x80105100), 0xaa, 0x14);
        e = ((f_ppiiii)lib("SetDefDispEnv"))(W(0x80105100), 0, 240, 320, 240);
        printf("t: SetDefDispEnv-returns-env: %d\n", e == (void *)W(0x80105100));
        dump("SetDefDispEnv", 0x80105100, 5);
    }

    /* SetDrawMode with and without a texture window. */
    {
        int16_t *tw = H(0x80106100);
        memset(B(0x80106000), 0xaa, 12);
        tw[0] = 8; tw[1] = 16; tw[2] = 32; tw[3] = 64;
        ((f_vpiiip)lib("SetDrawMode"))(W(0x80106000), 1, 1, 0x10a, tw);
        dump("SetDrawMode-window", 0x80106000, 3);
        memset(B(0x80106200), 0xaa, 12);
        ((f_vpiiip)lib("SetDrawMode"))(W(0x80106200), 0, 0, 0xffff, NULL);
        dump("SetDrawMode-null", 0x80106200, 3);
    }

    /* SetDrawEnv (library function without C): the words of an environment. PsyZ needs ResetGraph first. */
    ((f_ii)lib("ResetGraph"))(0);
    dumpval("callbacks-reset-by-ResetGraph(0)", resets);
    ((f_ii)lib("ResetGraph"))(1);
    ((f_ii)lib("ResetGraph"))(3);
    dumpval("callbacks-reset-after-ResetGraph(1)-and-(3)", resets);
    {
        uint8_t *env = B(0x80107000);
        memset(env, 0, 0x5c);
        ((f_ppiiii)lib("SetDefDrawEnv"))(env, 0, 0, 320, 240);
        W(0x80107000 + 0x1c)[0] = 0x00123456u;    /* the packet's link, to be kept */
        ((f_vpp)lib("func_80158a84"))(env + 0x1c, env);
        dump("SetDrawEnv-packet", 0x80107000 + 0x1c, 8);
        /* with a background colour */
        ((f_ppiiii)lib("SetDefDrawEnv"))(env, 0, 240, 320, 240);
        env[0x18] = 1; env[0x19] = 16; env[0x1a] = 40; env[0x1b] = 88;
        W(0x80107000 + 0x1c)[0] = 0;
        ((f_vpp)lib("func_80158a84"))(env + 0x1c, env);
        dump("SetDrawEnv-packet-isbg", 0x80107000 + 0x1c, 10);
    }
    printf("t: ticks %u\n", ticks);
}

/* ---- the picture ---- */

#define TILE_X 40
#define TILE_Y 20
#define SPR_X  32
#define SPR_Y  16

static uint16_t texel(unsigned u, unsigned v) { return (uint16_t)((1 + (u * 2) % 31) | ((1 + (v * 3) % 31) << 5) | (((u ^ v) & 31) << 10)); }

static void scene(int unknown, const char *file, int hold_ms)
{
    uint32_t env = 0x80120000u, disp = 0x80120100u, ot = 0x80130000u;
    uint32_t tile = 0x80131000u, mode = 0x80131100u, sprt = 0x80131200u, bg = 0x80131300u;
    uint32_t tex = 0x80110000u, shot = 0x80140000u, rect = 0x80111000u;
    unsigned u, v, t;
    FILE *f;

    ((f_ii)lib("ResetGraph"))(0);

    for (v = 0; v < 16; v++) for (u = 0; u < 16; u++) ((uint16_t *)(uintptr_t)tex)[v * 16 + u] = texel(u, v);
    H(rect)[0] = 512; H(rect)[1] = 0; H(rect)[2] = 16; H(rect)[3] = 16;
    ((f_rp)lib("LoadImage"))(W(rect), W(tex));

    /* draw page (0,0) 320x240 with a coloured background, display page (0,240) first */
    ((f_ppiiii)lib("SetDefDrawEnv"))(W(env), 0, 0, 320, 240);
    B(env)[0x18] = 1; B(env)[0x19] = 16; B(env)[0x1a] = 40; B(env)[0x1b] = 88;
    ((f_pp)lib("PutDrawEnv"))(W(env));
    ((f_ppiiii)lib("SetDefDispEnv"))(W(disp), 0, 240, 320, 240);
    ((f_pp)lib("func_80158470"))(W(disp));
    ((f_vi)lib("SetDispMask"))(1);

    ((f_ppi)lib("ClearOTagR"))(W(ot), 8);
    /* the flat rectangle, last in the list */
    ((f_vp)lib("SetTile"))(W(tile));
    B(tile)[4] = 255; B(tile)[5] = 0; B(tile)[6] = 255;
    H(tile + 8)[0] = TILE_X; H(tile + 8)[1] = TILE_Y; H(tile + 12)[0] = 16; H(tile + 12)[1] = 8;
    ((f_vpp)lib("AddPrim"))(W(ot + 4 * 2), W(tile));
    /* the drawing mode and the textured sprite, raw texture (code bit 0) */
    ((f_vpiiip)lib("SetDrawMode"))(W(mode), 0, 0, (int)((f_uiiii)lib("GetTPage"))(2, 0, 512, 0), NULL);
    ((f_vpp)lib("AddPrim"))(W(ot + 4 * 5), W(mode));
    ((f_vp)lib("SetSprt"))(W(sprt));
    B(sprt)[4] = 128; B(sprt)[5] = 128; B(sprt)[6] = 128; B(sprt)[7] |= 1;
    H(sprt + 8)[0] = SPR_X; H(sprt + 8)[1] = SPR_Y;
    B(sprt)[12] = 0; B(sprt)[13] = 0; *(uint16_t *)(uintptr_t)(sprt + 14) = 0;
    H(sprt + 16)[0] = 16; H(sprt + 16)[1] = 16;
    ((f_vpp)lib("AddPrim"))(W(ot + 4 * 4), W(sprt));
    if (unknown) {
        /* kinds PsyZ does not decode: copy CPU to VRAM (twice), copy VRAM to CPU */
        uint32_t k;
        for (k = 0; k < 3; k++) {
            uint32_t a = bg + 0x40 * k;
            W(a)[0] = 3u << 24;
            W(a)[1] = (k == 2 ? 0xc0000000u : 0xa0000000u);
            W(a)[2] = 0x00100010u;
            W(a)[3] = 0x00010001u;
            ((f_vpp)lib("AddPrim"))(W(ot + 4 * (k == 2 ? 3 : 6)), W(a));
        }
    }

    ((f_vp)lib("DrawOTag"))(W(ot + 4 * 7));
    ((f_ii)lib("DrawSync"))(0);

    H(rect)[0] = 0; H(rect)[1] = 0; H(rect)[2] = 64; H(rect)[3] = 48;
    ((f_rp)lib("StoreImage"))(W(rect), W(shot));
    ((f_ii)lib("DrawSync"))(0);
    f = fopen(file, "wb");
    if (!f) { printf("t: cannot write %s\n", file); fflush(stdout); exit(92); }
    fwrite((void *)(uintptr_t)shot, 2, 64 * 48, f);
    fclose(f);

    /* show the drawn page */
    ((f_ppiiii)lib("SetDefDispEnv"))(W(disp), 0, 0, 320, 240);
    ((f_pp)lib("func_80158470"))(W(disp));

    for (t = 0; t * 16 < (unsigned)hold_ms; t++) ((f_ii)lib("DrawSync"))(1);
    printf("t: ticks %u\n", ticks);
}

/* ---- lists ended wrongly ---- */

static void bad_list(const char *mode)
{
    ((f_ii)lib("ResetGraph"))(0);
    presenting = 0;
    if (!strcmp(mode, "cycle")) {
        W(0x80100000)[0] = 0x00100000u;           /* len 0, links to itself */
    } else if (!strcmp(mode, "outside")) {
        W(0x80100000)[0] = 0x00a00000u;           /* links beyond the 8 MB of the address space */
    } else if (!strcmp(mode, "past")) {
        W(0x801ffff0)[0] = (20u << 24) | 0x00ffffffu;   /* 20 words from 0x801ffff4 run past the end of RAM */
        ((f_vp)lib("DrawOTag"))(W(0x801ffff0));
        return;
    } else {                                     /* notram */
        ((f_vp)lib("DrawOTag"))(W(0x00001000));
        return;
    }
    ((f_vp)lib("DrawOTag"))(W(0x80100000));
}

/* ---- the window closed ---- */

static void closed(void)
{
    ((f_ii)lib("ResetGraph"))(0);
    ((f_ppiiii)lib("SetDefDispEnv"))(W(0x80120100), 0, 0, 320, 240);
    ((f_pp)lib("func_80158470"))(W(0x80120100));
    ((f_vi)lib("SetDispMask"))(1);
    port_gpu_present();
    port_gpu_present();
#ifdef TRIAL_SDL
    {
        SDL_Event e;
        memset(&e, 0, sizeof e);
        e.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&e);
    }
#endif
    printf("t: before\n");
    fflush(stdout);
    port_gpu_present();
    printf("t: after\n");
    fflush(stdout);
}


/* ---- a script of operations given on the command line ----
 * w:ADDR:VALUE word; h:ADDR:VALUE halfword; rect:ADDR:X:Y:W:H; fillw:ADDR:COUNT:VALUE; peek:ADDR;
 * tile:ADDR:R:G:B:X:Y:W:H a TILE packet (library SetTile) with its tag link left alone;
 * load:RECT:PIXELS  store:RECT:PIXELS  move:RECT:X:Y  clear:RECT   (print the return value)
 * cotag:ADDR:N  cotagr:ADDR:N  draw:ADDR  env  (a draw environment 0,0,320,240 sent with PutDrawEnv)
 * chain:ADDR:COUNT:NEXT   COUNT nodes of one word (length 0), each linking the next, the last linking NEXT
 * nops:ADDR:COUNT:NEXT    COUNT nodes of 255 words of kind 0 (no operation), chained likewise
 * tiles:ADDR:COUNT        COUNT TILE packets, white 1x1, tile k at (k%64, k/64), chained, the last ending the list
 * extremes:ADDR           every primitive type the game uses at extreme values, then a white 64x48 TILE, chained
 * senv:ADDR:ISBG          SetDrawEnv on a default environment at ADDR with a canary word after it
 * setmask:N reset reset1 dsync   SetDispMask(N), ResetGraph(0), ResetGraph(1), DrawSync(0) (printing its value)
 * display                 print the display enable the layer last sent
 * list0:ADDR              a packet of drawing-area commands and a white 4x4 TILE at (5,5), chained
 * prim:ADDR:CODE:LEN      one packet of LEN words, the first with the command code CODE in its top byte, the rest zero
 * frame                   read the whole frame buffer with port_gpu_read_frame, print how many halfwords are not 0
 * pixel:X:Y               DrawSync(0), then print the pixel (StoreImage of 1x1 into RAM)
 * A first argument `noreset` leaves out the ResetGraph that starts every script. */
static long long num(const char *s) { return strtoll(s, NULL, 0); }

static void set_tile(uint32_t at, unsigned r, unsigned g, unsigned b, int x, int y, int w, int h)
{
    ((f_vp)lib("SetTile"))(W(at));
    B(at)[4] = (uint8_t)r; B(at)[5] = (uint8_t)g; B(at)[6] = (uint8_t)b;
    H(at + 8)[0] = (int16_t)x; H(at + 8)[1] = (int16_t)y;
    H(at + 12)[0] = (int16_t)w; H(at + 12)[1] = (int16_t)h;
}

static void link_to(uint32_t at, uint32_t next) { W(at)[0] = (W(at)[0] & 0xff000000u) | (next & 0xffffffu); }

static void script(int argc, char **argv)
{
    volatile int i = 2;
    if (argc > 2 && !strcmp(argv[2], "noreset")) i++;    /* the first graphics call is then the script's own */
    else ((f_ii)lib("ResetGraph"))(0);
    for (; i < argc; i++) {
        char buf[1500];
        char *a[10];
        int n = 0, st;
        char *tok;
        unsigned long long k;
        if (recoverable && (st = setjmp(recover_to)) != 0) {
            printf("t: stopped %d\n", st);
            fflush(stdout);
            continue;
        }
        strncpy(buf, argv[i], sizeof buf - 1);
        buf[sizeof buf - 1] = 0;
        for (tok = strtok(buf, ":"); tok && n < 10; tok = strtok(NULL, ":")) a[n++] = tok;
        if (!strcmp(a[0], "recover")) recoverable = 1;
        else if (!strcmp(a[0], "setmask")) ((f_vi)lib("SetDispMask"))((int)num(a[1]));
        else if (!strcmp(a[0], "reset")) ((f_ii)lib("ResetGraph"))(0);
        else if (!strcmp(a[0], "reset1")) ((f_ii)lib("ResetGraph"))(1);
        else if (!strcmp(a[0], "dsync")) printf("t: DrawSync %d\n", ((f_ii)lib("DrawSync"))(0));
        else if (!strcmp(a[0], "display")) printf("t: display %d\n", port_gpu_display_enabled());
        else if (!strcmp(a[0], "pkt")) {
            /* pkt:ADDR:NEXT:W0,W1,... one packet with these words (hex), linking NEXT */
            uint32_t at = (uint32_t)num(a[1]);
            unsigned count = 0;
            char *word;
            for (word = strtok(a[3], ","); word; word = strtok(NULL, ",")) W(at)[1 + count++] = (uint32_t)strtoul(word, NULL, 16);
            W(at)[0] = (count << 24) | ((uint32_t)num(a[2]) & 0xffffffu);
        } else if (!strcmp(a[0], "tilepkt")) {
            /* tilepkt:ADDR:COUNT one packet of COUNT white 1x1 tiles at (k, 40) */
            uint32_t at = (uint32_t)num(a[1]);
            unsigned count = (unsigned)num(a[2]), j;
            for (j = 0; j < count; j++) {
                W(at)[1 + 3 * j] = 0x60ffffffu;
                H(at + 8 + 12 * j)[0] = (int16_t)j; H(at + 8 + 12 * j)[1] = 40;
                W(at)[3 + 3 * j] = 0x00010001u;
            }
            W(at)[0] = ((3u * count) << 24) | 0xffffffu;
        } else if (!strcmp(a[0], "marge")) {
            /* marge:ADDR two TILE packets one after the other, merged with MargePrim; prints its result and the length */
            uint32_t at = (uint32_t)num(a[1]);
            set_tile(at, 255, 255, 255, 30, 10, 1, 1);
            set_tile(at + 16, 255, 255, 255, 32, 10, 1, 1);
            link_to(at, 0xffffffu);
            link_to(at + 16, 0xffffffu);
            { int rc = ((f_rp)lib("MargePrim"))(W(at), W(at + 16)); printf("t: MargePrim %d length %u\n", rc, W(at)[0] >> 24); }
        } else if (!strcmp(a[0], "penv")) {
            /* penv:ADDR:ISBG:X:W a draw environment with a background fill, sent with PutDrawEnv */
            uint32_t at = (uint32_t)num(a[1]);
            ((f_ppiiii)lib("SetDefDrawEnv"))(W(at), (int)num(a[3]), 0, (int)num(a[4]), 240);
            B(at)[0x18] = (uint8_t)num(a[2]); B(at)[0x19] = 9;
            ((f_pp)lib("PutDrawEnv"))(W(at));
        } else if (!strcmp(a[0], "w")) W((uint32_t)num(a[1]))[0] = (uint32_t)num(a[2]);
        else if (!strcmp(a[0], "h")) H((uint32_t)num(a[1]))[0] = (int16_t)num(a[2]);
        else if (!strcmp(a[0], "rect")) { int j; for (j = 0; j < 4; j++) H((uint32_t)num(a[1]))[j] = (int16_t)num(a[2 + j]); }
        else if (!strcmp(a[0], "fillw")) for (k = 0; k < (unsigned long long)num(a[2]); k++) W((uint32_t)num(a[1]))[k] = (uint32_t)num(a[3]);
        else if (!strcmp(a[0], "peek")) printf("t: peek %08x: %08x\n", (unsigned)num(a[1]), W((uint32_t)num(a[1]))[0]);
        else if (!strcmp(a[0], "tile")) set_tile((uint32_t)num(a[1]), (unsigned)num(a[2]), (unsigned)num(a[3]), (unsigned)num(a[4]), (int)num(a[5]), (int)num(a[6]), (int)num(a[7]), (int)num(a[8]));
        else if (!strcmp(a[0], "load")) printf("t: LoadImage %d\n", ((f_rp)lib("LoadImage"))((void *)(uintptr_t)(uint32_t)num(a[1]), (void *)(uintptr_t)(uint32_t)num(a[2])));
        else if (!strcmp(a[0], "store")) printf("t: StoreImage %d\n", ((f_rp)lib("StoreImage"))((void *)(uintptr_t)(uint32_t)num(a[1]), (void *)(uintptr_t)(uint32_t)num(a[2])));
        else if (!strcmp(a[0], "move")) printf("t: MoveImage %d\n", ((f_rpii)lib("MoveImage"))((void *)(uintptr_t)(uint32_t)num(a[1]), (int)num(a[2]), (int)num(a[3])));
        else if (!strcmp(a[0], "clear")) printf("t: ClearImage %d\n", ((f_rpiii)lib("ClearImage"))((void *)(uintptr_t)(uint32_t)num(a[1]), 0, 0, 0));
        else if (!strcmp(a[0], "cotag")) printf("t: ClearOTag returns %08x\n", (unsigned)(uintptr_t)((f_ppi)lib("ClearOTag"))((void *)(uintptr_t)(uint32_t)num(a[1]), (int)num(a[2])));
        else if (!strcmp(a[0], "cotagr")) printf("t: ClearOTagR returns %08x\n", (unsigned)(uintptr_t)((f_ppi)lib("ClearOTagR"))((void *)(uintptr_t)(uint32_t)num(a[1]), (int)num(a[2])));
        else if (!strcmp(a[0], "draw")) ((f_vp)lib("DrawOTag"))((void *)(uintptr_t)(uint32_t)num(a[1]));
        else if (!strcmp(a[0], "env")) {
            ((f_ppiiii)lib("SetDefDrawEnv"))(W(0x80120000), 0, 0, 320, 240);
            ((f_pp)lib("PutDrawEnv"))(W(0x80120000));
        } else if (!strcmp(a[0], "chain")) {
            uint32_t at = (uint32_t)num(a[1]);
            for (k = 0; k < (unsigned long long)num(a[2]); k++) W(at + 4 * (uint32_t)k)[0] = (k + 1 < (unsigned long long)num(a[2])) ? ((at + 4 * (uint32_t)(k + 1)) & 0xffffffu) : ((uint32_t)num(a[3]) & 0xffffffu);
        } else if (!strcmp(a[0], "nops")) {
            uint32_t at = (uint32_t)num(a[1]);
            unsigned long long count = (unsigned long long)num(a[2]);
            for (k = 0; k < count; k++) {
                uint32_t node = at + 1024u * (uint32_t)k;
                memset(B(node + 4), 0, 255 * 4);
                W(node)[0] = (255u << 24) | (k + 1 < count ? ((node + 1024u) & 0xffffffu) : ((uint32_t)num(a[3]) & 0xffffffu));
            }
        } else if (!strcmp(a[0], "tiles")) {
            uint32_t at = (uint32_t)num(a[1]);
            unsigned long long count = (unsigned long long)num(a[2]);
            for (k = 0; k < count; k++) {
                uint32_t node = at + 16u * (uint32_t)k;
                set_tile(node, 255, 255, 255, (int)(k % 64), (int)(k / 64), 1, 1);
                link_to(node, k + 1 < count ? node + 16u : 0xffffffu);
            }
        } else if (!strcmp(a[0], "extremes")) {
            uint32_t at = (uint32_t)num(a[1]), node[7];
            int j;
            for (j = 0; j < 7; j++) node[j] = at + 0x100u * (uint32_t)j;
            ((f_vpiiip)lib("SetDrawMode"))(W(node[0]), 1, 1, 0xffff, NULL);
            ((f_vp)lib("SetPolyFT4"))(W(node[1]));
            B(node[1])[4] = 255; B(node[1])[5] = 255; B(node[1])[6] = 255;
            { static const int16_t xy[8] = { -32768, -32768, 32767, -32768, -32768, 32767, 32767, 32767 };
              for (j = 0; j < 4; j++) { H(node[1] + 8 + 8 * j)[0] = xy[2 * j]; H(node[1] + 8 + 8 * j)[1] = xy[2 * j + 1]; B(node[1] + 12 + 8 * j)[0] = 255; B(node[1] + 12 + 8 * j)[1] = 255; } }
            *(uint16_t *)(uintptr_t)(node[1] + 14) = 0xffff; *(uint16_t *)(uintptr_t)(node[1] + 22) = 0xffff;
            ((f_vp)lib("SetSprt"))(W(node[2]));
            B(node[2])[4] = 255; B(node[2])[5] = 0; B(node[2])[6] = 0;
            H(node[2] + 8)[0] = -32768; H(node[2] + 8)[1] = -32768; H(node[2] + 16)[0] = 32767; H(node[2] + 16)[1] = 32767;
            ((f_vp)lib("SetSprt16"))(W(node[3]));
            H(node[3] + 8)[0] = 32767; H(node[3] + 8)[1] = 32767;
            set_tile(node[4], 0, 255, 0, -32768, -32768, 32767, 32767);
            set_tile(node[5], 0, 0, 255, 0, 0, 1023, 511);
            set_tile(node[6], 255, 255, 255, 0, 0, 64, 48);
            for (j = 0; j < 7; j++) link_to(node[j], j + 1 < 7 ? node[j + 1] : 0xffffffu);
        } else if (!strcmp(a[0], "list0")) {
            uint32_t at = (uint32_t)num(a[1]);
            W(at)[1] = 0xe3000000u; W(at)[2] = 0xe4000000u | (239u << 10) | 319u; W(at)[3] = 0xe5000000u; W(at)[4] = 0xe6000000u;
            W(at)[0] = (4u << 24) | ((at + 0x40u) & 0xffffffu);
            set_tile(at + 0x40u, 255, 255, 255, 5, 5, 4, 4);
            link_to(at + 0x40u, 0xffffffu);
        } else if (!strcmp(a[0], "prim")) {
            uint32_t at = (uint32_t)num(a[1]);
            unsigned len = (unsigned)num(a[3]);
            memset(B(at + 4), 0, 4u * len);
            W(at)[0] = (len << 24) | 0xffffffu;
            if (len) W(at)[1] = (unsigned)num(a[2]) << 24;
        } else if (!strcmp(a[0], "frame")) {
            static unsigned short frame[1024 * 512];
            unsigned long nonzero = 0, j;
            int rc = port_gpu_read_frame(frame);
            for (j = 0; j < 1024ul * 512ul; j++) if (frame[j]) nonzero++;
            printf("t: frame rc %d nonzero %lu\n", rc, nonzero);
        } else if (!strcmp(a[0], "senv")) {
            uint32_t at = (uint32_t)num(a[1]);
            ((f_ppiiii)lib("SetDefDrawEnv"))(W(at), 0, 240, 320, 240);
            B(at)[0x18] = (uint8_t)num(a[2]); B(at)[0x19] = 1; B(at)[0x1a] = 2; B(at)[0x1b] = 3;
            W(at + 0x5c)[0] = 0xcafecafeu;
            ((f_vpp)lib("func_80158a84"))(W(at + 0x1c), W(at));
            printf("t: senv words %u canary %08x\n", W(at + 0x1c)[0] >> 24, W(at + 0x5c)[0]);
        } else if (!strcmp(a[0], "pixel")) {
            ((f_ii)lib("DrawSync"))(0);
            H(0x80190000)[0] = (int16_t)num(a[1]); H(0x80190000)[1] = (int16_t)num(a[2]); H(0x80190000)[2] = 1; H(0x80190000)[3] = 1;
            ((f_rp)lib("StoreImage"))(W(0x80190000), W(0x80190010));
            printf("t: pixel %s %s %04x\n", a[1], a[2], *(uint16_t *)(uintptr_t)0x80190010);
        } else {
            printf("t: unknown operation %s\n", a[0]);
        }
        fflush(stdout);
    }
}

/* ---- the game's memory: RAM, scratchpad, the live stack of the thread or fiber that is running ---- */

static char global_bytes[64];
static char *fiber_local;             /* the address of a local of the fiber, set by the fiber */
static char *main_local_ptr;          /* the address of a local of the main stack */
static LPVOID main_fiber, other_fiber;
static volatile int fiber_mode;

/* The address of a local of a function that has returned, kept in a variable (the compiler turns a returned address
 * of a local into a null pointer). The array is 4 KB so that its start lies well below the frames of the routines that
 * make the check, which are live and count as the caller's side of the bound; the probe is a pointer into the dead stack. */
static char *volatile dead_saved;

static void dead_local(void)
{
    volatile char x[4096];
    x[0] = 1;
    dead_saved = (char *)x;
}

static void span(const char *name, const void *p, size_t n)
{
    printf("t: span %s %d\n", name, port_game_span(p, n));
}

/* Every kind of pointer, from the stack that is running now (the main one or a fiber's). */
static void probes(const char *tag, const char *foreign)
{
    char local[16];
    char *dead;
    char *base = (char *)((NT_TIB *)NtCurrentTeb())->StackBase;
    char *heap = malloc(64);
    char name[80];
#define PROBE(label, p, n) do { snprintf(name, sizeof name, "%s %s", tag, label); span(name, (p), (n)); } while (0)
    local[0] = 0;
    dead_local();
    dead = dead_saved;
    PROBE("local", local, sizeof local);
    PROBE("local-zero-length", local, 0);
    PROBE("dead-stack-below-the-frame", dead, 8);
    PROBE("last-bytes-below-the-base", base - 4, 4);
    PROBE("at-the-base", base, 1);
    PROBE("straddling-the-base", base - 4, 8);
    PROBE("zero-length-at-the-base", base, 0);
    PROBE("heap", heap, 8);
    PROBE("program-data", global_bytes, 8);
    PROBE("program-code", (const void *)probes, 4);
    if (foreign) PROBE("other-stack", foreign, 4);
    free(heap);
}

static VOID CALLBACK other_main(LPVOID unused)
{
    char mine[16];
    (void)unused;
    mine[0] = 0;
    fiber_local = mine;
    if (fiber_mode == 1) {
        probes("fiber", main_local_ptr);
        SwitchToFiber(main_fiber);     /* this fiber stays suspended, with its stack alive */
    }
    if (fiber_mode == 2) {
        /* the same checks as the game would meet: a local rectangle and a local buffer, through the library */
        uint16_t pix[16], back[16];
        int16_t rect[4] = { 10, 10, 4, 4 }, r2[4] = { 10, 10, 4, 4 };
        unsigned k, same = 1;
        for (k = 0; k < 16; k++) pix[k] = (uint16_t)(0x0101 * (k + 1)), back[k] = 0;
        ((f_rp)lib("LoadImage"))(rect, pix);
        ((f_rp)lib("StoreImage"))(r2, back);
        for (k = 0; k < 16; k++) if (back[k] != pix[k]) same = 0;
        printf("t: fiber roundtrip through locals %d\n", same);
    }
    SwitchToFiber(main_fiber);
}

static void ranges(void)
{
    static const struct { const char *name; uint32_t a; size_t n; } r[] = {
        { "ram first byte", 0x80000000u, 1 }, { "ram last byte", 0x801fffffu, 1 }, { "ram whole", 0x80000000u, 0x200000u },
        { "ram one past the end, 1 byte", 0x80200000u, 1 }, { "ram last byte, 2 bytes", 0x801fffffu, 2 }, { "ram one byte before", 0x7fffffffu, 1 },
        { "ram zero length inside", 0x80100000u, 0 }, { "ram zero length at the end", 0x80200000u, 0 },
        { "ram a size that wraps", 0x80000000u, (size_t)0xffffffffu }, { "ram the largest size", 0x80100000u, (size_t)-1 },
        { "scratch whole", 0x1f800000u, 0x400 }, { "scratch one byte more", 0x1f800000u, 0x401 }, { "scratch last byte", 0x1f8003ffu, 1 },
        { "scratch one past", 0x1f800400u, 1 }, { "scratch one before", 0x1f7fffffu, 1 }, { "null", 0, 1 },
    };
    unsigned i;
    for (i = 0; i < sizeof r / sizeof r[0]; i++) span(r[i].name, (const void *)(uintptr_t)r[i].a, r[i].n);
}

static void stackcheck(void)
{
    char here[16];
    main_fiber = ConvertThreadToFiber(NULL);
    here[0] = 0;
    main_local_ptr = here;
    ranges();
    probes("main", NULL);
    fiber_mode = 1;
    other_fiber = CreateFiber(0, other_main, NULL);
    SwitchToFiber(other_fiber);
    probes("main with a suspended fiber", fiber_local);
    DeleteFiber(other_fiber);
}

static void stackroundtrip(int in_fiber)
{
    ((f_ii)lib("ResetGraph"))(0);
    main_fiber = ConvertThreadToFiber(NULL);
    {
        uint16_t pix[16], back[16];
        int16_t rect[4] = { 10, 10, 4, 4 }, r2[4] = { 10, 10, 4, 4 };
        unsigned k, same = 1;
        for (k = 0; k < 16; k++) pix[k] = (uint16_t)(0x0101 * (k + 1)), back[k] = 0;
        if (!in_fiber) {
            ((f_rp)lib("LoadImage"))(rect, pix);
            ((f_rp)lib("StoreImage"))(r2, back);
            for (k = 0; k < 16; k++) if (back[k] != pix[k]) same = 0;
            printf("t: main roundtrip through locals %d\n", same);
        } else {
            fiber_mode = 2;
            other_fiber = CreateFiber(0, other_main, NULL);
            SwitchToFiber(other_fiber);
            DeleteFiber(other_fiber);
        }
    }
}

static void heapload(void)
{
    uint16_t *heap = malloc(64);
    int16_t rect[4] = { 10, 10, 4, 4 };
    ((f_ii)lib("ResetGraph"))(0);
    ((f_rp)lib("LoadImage"))(rect, heap);
    printf("t: not reached\n");
}

static void firstnode_on_stack(void)
{
    uint32_t node[2];
    node[0] = 0xffffffu;
    node[1] = 0;
    ((f_ii)lib("ResetGraph"))(0);
    ((f_vp)lib("DrawOTag"))(node);
    printf("t: not reached\n");
}

static void nodisplay(void)
{
    SetEnvironmentVariableA("SDL_VIDEODRIVER", "no-such-driver");
    _putenv("SDL_VIDEODRIVER=no-such-driver");
    ((f_rv)lib("GetGraphDebug"))();
    printf("t: not reached\n");
}

static void names(void)
{
    const struct port_library *e;
    for (e = port_gpu_library; e->name; e++) printf("t: name %s\n", e->name);
}

/* ---- presenting does not wait ---- */

static void presents(void)
{
    unsigned i;
    DWORD start;
    ((f_ii)lib("ResetGraph"))(0);
    ((f_ppiiii)lib("SetDefDispEnv"))(W(0x80120100), 0, 0, 320, 240);
    ((f_pp)lib("func_80158470"))(W(0x80120100));
    ((f_vi)lib("SetDispMask"))(1);
    port_gpu_present();
    start = GetTickCount();
    for (i = 0; i < 300; i++) port_gpu_present();
    printf("t: presents-ms %lu\n", (unsigned long)(GetTickCount() - start));
}

/* A fault ends the trial with one line that says where, so that a failing case is not a silent status. */
static LONG WINAPI crashed(EXCEPTION_POINTERS *p)
{
    printf("t: crash: exception 0x%08x at 0x%08x, accessing 0x%08x\n", (unsigned)p->ExceptionRecord->ExceptionCode, (unsigned)(uintptr_t)p->ExceptionRecord->ExceptionAddress,
           p->ExceptionRecord->NumberParameters > 1 ? (unsigned)p->ExceptionRecord->ExceptionInformation[1] : 0u);
    fflush(stdout);
    ExitProcess(10);
    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "";
    SetUnhandledExceptionFilter(crashed);
    setvbuf(stdout, NULL, _IONBF, 0);
    /* No window on the desktop: PsyZ's GPU path runs on SDL's offscreen video driver (the cases that need the video
     * system to fail set their own value later). */
    SetEnvironmentVariableA("SDL_VIDEODRIVER", "offscreen");
    _putenv("SDL_VIDEODRIVER=offscreen");
    map_or_die();
    if (!strcmp(mode, "semantics")) semantics();
    else if (!strcmp(mode, "picture") && argc > 2) scene(0, argv[2], 3000);
    else if (!strcmp(mode, "unknown") && argc > 2) scene(1, argv[2], 200);
    else if (!strcmp(mode, "cycle") || !strcmp(mode, "outside") || !strcmp(mode, "past") || !strcmp(mode, "notram")) bad_list(mode);
    else if (!strcmp(mode, "closed")) closed();
    else if (!strcmp(mode, "presents")) presents();
    else if (!strcmp(mode, "names")) names();
    else if (!strcmp(mode, "nodisplay")) nodisplay();
    else if (!strcmp(mode, "stack")) stackcheck();
    else if (!strcmp(mode, "roundtrip-main")) stackroundtrip(0);
    else if (!strcmp(mode, "roundtrip-fiber")) stackroundtrip(1);
    else if (!strcmp(mode, "heapload")) heapload();
    else if (!strcmp(mode, "stacknode")) firstnode_on_stack();
    else if (!strcmp(mode, "script")) script(argc, argv);
    else if (!strcmp(mode, "linger") && argc > 2) { printf("t: lingering\n"); Sleep((DWORD)atoi(argv[2])); }
    else { printf("t: bad usage\n"); return 93; }
    printf("t: done\n");
    return 0;
}

'''

# ---------------------------------------------------------------- helpers


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"



# ---- both sides of the interface between the walker and PsyZ, kind by kind ----
# The console's length of a command and the number of words PsyZ's decoder takes for it, worked out HERE for all 256
# kinds (the console's from the GP0 command list of psx-spx, PsyZ's by reading its decoder in the pinned copy:
# src/psyz/libgpu.c DispatchPackets and src/platform/sdl3_gpu.c Draw_PushPrim, line numbers in the comment of gpu.c's
# walker). The sweep below uses them as the expected outcome of each kind; they are never read back from gpu.c.

def console_length(code: int) -> int | None:
    """Words of the command whose first word has this top byte; None where the length is not fixed by the kind or the
    kind is not decoded by the port (polylines, the copies that carry data, the interrupt request)."""
    if code in (0x00, 0x01, 0xE0) or 0x03 <= code <= 0x1E or 0xE1 <= code <= 0xE6 or code >= 0xE7:
        return 1                                                    # no-operation, cache clear, the setting words
    if code == 0x02:
        return 3
    if code == 0x80:
        return 4
    if 0x20 <= code <= 0x3F:                                        # polygons: bit 3 quad, bit 2 textured, bit 4 gouraud
        vertices = 4 if code & 0x08 else 3
        return 1 + vertices + (vertices if code & 0x04 else 0) + (vertices - 1 if code & 0x10 else 0)
    if 0x40 <= code <= 0x5F:                                        # lines: bit 3 makes a polyline; bit 2 means nothing here
        return None if code & 0x08 else (4 if code & 0x10 else 3)
    if 0x60 <= code <= 0x7F:                                        # rectangles: the size is in a word when bits 4..3 are clear
        return 2 + (1 if code & 0x04 else 0) + (1 if code & 0x18 == 0 else 0)
    return None                                                     # 0x1F, 0x81..0x9F, 0xA0..0xDF


def psyz_length(code: int) -> tuple[int, bool]:
    """(words PsyZ takes, whether it reports the kind as unsupported) for a command whose first word has this top byte,
    given enough words after it."""
    if code in (0x00, 0x01, 0xA0, 0xC0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6):
        return 1, False                                             # libgpu.c:63-115 (0xA0 and 0xC0 step one word)
    if code == 0x02:
        return 3, False                                             # libgpu.c:69-78
    if code == 0x80:
        return 4, False                                             # libgpu.c:79-88
    if 0x20 <= code <= 0x3F:                                        # sdl3_gpu.c:1105-1136: a writePacket per vertex, a colour word read ahead after the last
        vertices = 4 if code & 0x08 else 3
        textured, gouraud = 1 if code & 0x04 else 0, 1 if code & 0x10 else 0
        return 1 + vertices * (1 + textured + gouraud) - gouraud, False
    if 0x40 <= code <= 0x5F:                                        # sdl3_gpu.c:1159-1199
        points = ((code >> 2) & 3) + 1
        padding = points != 1
        points = 2 if points == 1 else points
        gouraud = 1 if code & 0x10 else 0
        return 1 + points + (points - 1) * gouraud + (1 if padding else 0), False
    if 0x60 <= code <= 0x7F:                                        # sdl3_gpu.c:1270-1343
        return 2 + (1 if code & 0x04 else 0) + (1 if (code & ~3) in (0x60, 0x64) else 0), False
    return 1, True                                                  # libgpu.c:132, "unsupported command"


def kind_is_accepted(code: int) -> bool:
    """What the walker must do with a kind: the no-operation kinds are accepted (and kept from PsyZ); any other kind only
    where the console's length and PsyZ's are the same."""
    console = console_length(code)
    if console is None:
        return False
    if psyz_length(code)[1]:
        return True                                                 # a no-operation: left out of what PsyZ gets
    return console == psyz_length(code)[0]


def kind_refusal_name(code: int) -> str:
    if code == 0x1F:
        return "interrupt request"
    if 0x40 <= code <= 0x5F and code & 0x08:
        return "polyline, its length depends on a terminator word"
    if 0x40 <= code <= 0x5F:
        return "line with a flag bit that the library the port draws with reads as a longer command"
    if 0x81 <= code <= 0x9F:
        return "copy rectangle VRAM to VRAM, mirror"
    if 0xA0 <= code <= 0xBF:
        return "copy rectangle CPU to VRAM"
    return "copy rectangle VRAM to CPU"                             # 0xC0..0xDF



class Rig:
    def __init__(self, cc: str, prefix: list[str], work: Path, psyz_build: Path, gpu_source: Path):
        self.cc, self.prefix, self.work, self.psyz_build, self.gpu_source = cc, prefix, work, psyz_build, gpu_source
        self.memory_source = SRC / "memory.c"
        self.section = "build"
        self.groups: set[str] | None = None   # --group: only these sections of cases run
        self.wsl = not prefix and shutil.which("wslpath") is not None
        self.info: dict = {}
        self.exe: Path | None = None
        self.raw: list[str] = []

    def native(self, path: Path) -> str:
        if self.wsl:
            return subprocess.run(["wslpath", "-w", str(path)], capture_output=True, text=True, timeout=30).stdout.strip()
        return str(path)

    def start_check(self) -> str | None:
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

    def compile(self, args: list[str]) -> subprocess.CompletedProcess:
        return subprocess.run([self.cc, *args], capture_output=True, text=True, timeout=300)

    def build(self) -> str | None:
        """Compile gpu.c with and without PsyZ, then link the trial program. None, or the reason."""
        try:
            self.info = json.loads((self.psyz_build / "psyz.json").read_text())
        except (OSError, ValueError) as err:
            return f"{self.psyz_build / 'psyz.json'} cannot be read ({err}); build PsyZ with psyzbuild.py first"
        include = Path(self.info["include"])
        (self.work / "trial.c").write_text(TRIAL_C)
        flags = ["-O1", "-Wall", "-Wextra", "-c", "-I", str(SRC)]
        steps = {
            "gpu.o": [*flags, "-Werror", "-DPORT_HAVE_PSYZ", *(f"-D{d}" for d in self.info["define"]), "-isystem", str(include), str(self.gpu_source)],
            "memory.o": [*flags, "-Werror", str(self.memory_source)],
            "trial.o": [*flags, "-DTRIAL_SDL", "-I", str(include.parents[1] / "external" / "SDL" / "include"), str(self.work / "trial.c")],
        }
        for obj, argv in steps.items():
            done = self.compile([*argv, "-o", str(self.work / obj)])
            if done.returncode != 0:
                return f"{obj} did not compile:\n" + done.stderr.strip()
        self.exe = self.work / EXE
        done = self.compile([str(self.work / "trial.o"), str(self.work / "gpu.o"), str(self.work / "memory.o"), *LINK_FLAGS, *self.info["link"], "-o", str(self.exe)])
        if done.returncode != 0:
            return "the trial program did not link:\n" + done.stderr.strip()
        return None

    def run(self, mode: str, *more: str, timeout: int = 90) -> tuple[int, list[str], float]:
        """Run the trial program; (status, the lines that are the layer's or the program's, seconds)."""
        import time
        if self.groups is not None and self.section not in self.groups:
            return 0, ["t: done"], 0.0          # a section that was not asked for: no program is started
        argv = [*self.prefix, str(self.exe), mode, *more]
        started = time.time()
        try:
            proc = subprocess.run(argv, capture_output=True, text=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            return -999, ["(timeout)"], time.time() - started   # subprocess.run has ended the program it started
        lines = (proc.stdout + proc.stderr).replace("\r\n", "\n").splitlines()
        self.raw = lines                         # every line, PsyZ's own log lines included (the return value keeps only the layer's and the program's)
        return proc.returncode, [l for l in lines if l.startswith(("t: ", "stop: ", "gpu: "))], time.time() - started


# Sony's formulas, written here from PSY-Q's documented behaviour (and the listing of the game's library).


def rgb555(r: int, g: int, b: int) -> int:
    return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10)


def texel(u: int, v: int) -> int:
    return (1 + (u * 2) % 31) | ((1 + (v * 3) % 31) << 5) | (((u ^ v) & 31) << 10)


def get_tpage(tp: int, abr: int, x: int, y: int) -> int:
    return ((tp & 3) << 7) | ((abr & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2)


def get_clut(x: int, y: int) -> int:
    return ((y << 6) | ((x >> 4) & 0x3F)) & 0xFFFF


def tw_word(x: int, y: int, w: int, h: int) -> int:
    return 0xE2000000 | (((y & 0xFF) >> 3) << 15) | (((x & 0xFF) >> 3) << 10) | (((-h & 0xFF) >> 3) << 5) | ((-w & 0xFF) >> 3)


def hexwords(*words: int) -> str:
    return " ".join(f"{w & 0xFFFFFFFF:08x}" for w in words)


def expected_semantics() -> list[str]:
    ram = 0x80100000
    lines = []
    lines.append("t: ClearOTag: " + hexwords(0x100004, 0x100008, 0x10000C, 0xFFFFFF, 0xDEADBEEF))
    lines.append("t: ClearOTag-returns-index: 00000003")
    lines.append("t: ClearOTagR: " + hexwords(0xFFFFFF, 0x100100, 0x100104, 0x100108, 0xDEADBEEF))
    lines.append("t: ClearOTagR-returns-index: 00000000")
    lines.append("t: AddPrim-ot: " + hexwords(0x07000000 | 0x101100))
    lines.append("t: AddPrim-p: " + hexwords(0x05000000 | 0x123456))
    lines.append("t: AddPrims-ot: " + hexwords(0x07000000 | 0x101300))
    lines.append("t: AddPrims-p0: " + hexwords(0x03111111))
    lines.append("t: AddPrims-p1: " + hexwords(0x04000000 | 0x123456))
    lines.append(f"t: MargePrim0: result 0 tag0 {0x08AAAAAA:08x} tag1 {0x04BBBBBB:08x}")
    lines.append(f"t: MargePrim1: result -1 tag0 {0x10AAAAAA:08x} tag1 {0x10BBBBBB:08x}")
    lines.append(f"t: MargePrim2: result 0 tag0 {0x20AAAAAA:08x} tag1 {0x0FBBBBBB:08x}")
    lines.append("t: SetSemiTrans-on: " + hexwords(0, 0x66332211))
    lines.append("t: SetSemiTrans-off: " + hexwords(0, 0x64332211))
    for name, length, code in (("SetPolyFT4", 9, 0x2C), ("SetSprt16", 3, 0x7C), ("SetSprt", 4, 0x64), ("SetTile", 3, 0x60)):
        lines.append(f"t: {name}: {(length << 24) | 0xABCDEF:08x} {(code << 24) | 0x332211:08x}")
    for tp in ((0, 0, 0, 0), (2, 1, 512, 0), (1, 2, 640, 256), (3, 3, 1023, 511), (2, 0, 320, 256)):
        lines.append(f"t: GetTPage {tp[0]} {tp[1]} {tp[2]} {tp[3]}: {get_tpage(*tp):04x}")
    for xy in ((0, 0), (16, 1), (640, 480), (1008, 511)):
        lines.append(f"t: GetClut {xy[0]} {xy[1]}: {get_clut(*xy):04x}")
    lines.append("t: SetDefDrawEnv-returns-env: 1")
    lines.append("t: SetDefDrawEnv: " + hexwords((16 << 16) | 8, (240 << 16) | 320, (16 << 16) | 8, 0, 0, (1 << 16) | 0x0A, 0) + (" aaaaaaaa" * 16))
    lines.append("t: SetDefDispEnv-returns-env: 1")
    lines.append("t: SetDefDispEnv: " + hexwords(240 << 16, (240 << 16) | 320, 0, 0, 0))
    lines.append("t: SetDrawMode-window: " + hexwords(0x02AAAAAA, 0xE1000200 | 0x400 | (0x10A & 0x9FF), tw_word(8, 16, 32, 64)))
    lines.append("t: SetDrawMode-null: " + hexwords(0x02AAAAAA, 0xE1000000 | (0xFFFF & 0x9FF), 0))
    lines.append("t: callbacks-reset-by-ResetGraph(0): 00000001")
    lines.append("t: callbacks-reset-after-ResetGraph(1)-and-(3): 00000001")
    ce_x, ce_y = 319, 239
    lines.append("t: SetDrawEnv-packet: " + hexwords(
        (6 << 24) | 0x123456, 0xE3000000, 0xE4000000 | (ce_y << 10) | ce_x, 0xE5000000, 0xE1000200 | 0x0A, 0xE2000000, 0xE6000000, 0))
    # the clip (0, 240, 320, 240): the area ends at (319, 479); the offset is (0, 240); aligned, so the fill command
    lines.append("t: SetDrawEnv-packet-isbg: " + hexwords(
        9 << 24, 0xE3000000 | (240 << 10), 0xE4000000 | (479 << 10) | 319, 0xE5000000 | (240 << 11), 0xE1000200 | 0x0A, 0xE2000000, 0xE6000000,
        0x02000000 | (88 << 16) | (40 << 8) | 16, (240 << 16) | 0, (240 << 16) | 320))
    return lines


def expected_picture() -> tuple[list[int], list[int]]:
    """The 64 x 48 colours (15 bits) of the picture and, per pixel, 1 where a primitive drew it."""
    w, h = 64, 48
    px = [rgb555(16, 40, 88)] * (w * h)
    drawn = [0] * (w * h)
    for v in range(16):
        for u in range(16):
            px[(16 + v) * w + 32 + u] = texel(u, v)
            drawn[(16 + v) * w + 32 + u] = 1
    for y in range(20, 28):
        for x in range(40, 56):
            px[y * w + x] = rgb555(255, 0, 255)
            drawn[y * w + x] = 1
    return px, drawn


def read_picture(path: Path) -> list[int] | None:
    try:
        data = path.read_bytes()
    except OSError:
        return None
    return [int.from_bytes(data[i:i + 2], "little") for i in range(0, len(data), 2)]


def diff_picture(got: list[int] | None, want: list[int], bit15: list[int] | None = None) -> str | None:
    """None when every pixel's 15 colour bits are `want`'s; with `bit15`, when bit 15 of every pixel is that list's instead."""
    if got is None:
        return "the program wrote no picture"
    if len(got) != len(want):
        return f"the picture has {len(got)} pixels, wanted {len(want)}"
    if bit15 is None:
        bad = [i for i, (a, b) in enumerate(zip(got, want)) if a & 0x7FFF != b]
    else:
        bad = [i for i, (a, b) in enumerate(zip(got, bit15)) if a >> 15 != b]
    if not bad:
        return None
    i = bad[0]
    return f"{len(bad)} of {len(want)} pixels differ; first at ({i % 64},{i // 64}): wanted 0x{want[i]:04x}{'' if bit15 is None else ' with bit 15 ' + str(bit15[i])}, got 0x{got[i]:04x}"


def program_cases(rig: Rig, work: Path):
    # gpu.c compiles to empty tables without PsyZ, and the stub has nothing to link.
    stub = rig.compile(["-O1", "-Wall", "-Wextra", "-Werror", "-c", "-I", str(SRC), str(rig.gpu_source), "-o", str(work / "stub.o")])
    yield "gpu-c-compiles-without-psyz-warning-free", same((stub.returncode, stub.stderr.strip()), (0, ""))
    nm = rig.cc[:-3] + "nm" if rig.cc.endswith("gcc") else "nm"
    syms = subprocess.run([nm, "-g", str(work / "stub.o")], capture_output=True, text=True)
    defined = sorted(l.split()[-1] for l in syms.stdout.splitlines() if " T " in l or " D " in l or " R " in l)
    undefined = sorted(l.split()[-1] for l in syms.stdout.splitlines() if l.strip().startswith("U "))
    yield "stub-defines-the-tables-and-present-and-needs-nothing", same((defined, undefined), (sorted(["_port_gpu_display_enabled", "_port_gpu_library", "_port_gpu_present", "_port_gpu_read_frame"]), []))

    reason = rig.build()
    yield "trial-program-builds-gpu-c-with-psyz-warning-free", reason
    if reason:
        return

    status, lines, _ = rig.run("semantics")
    want = expected_semantics() + ["t: ticks 0", "t: done"]
    yield "semantics-status-0", same(status, 0)
    yield "semantics-every-line-as-worked-out", same(lines, want)


    rig.section = "basic"
    # the table: what is served, and no routine that stores an address to call later
    status, lines, _ = rig.run("names")
    listed = [l.split()[2] for l in lines if l.startswith("t: name ")]
    served = ["ResetGraph", "GetGraphDebug", "SetDispMask", "DrawSync", "ClearImage", "LoadImage", "StoreImage", "MoveImage", "ClearOTag", "ClearOTagR",
              "DrawOTag", "func_80158470", "PutDrawEnv", "func_80158a84", "SetDrawMode", "GetTPage", "GetClut", "AddPrim", "AddPrims", "MargePrim",
              "SetSemiTrans", "SetPolyFT4", "SetSprt16", "SetSprt", "SetTile", "SetDefDrawEnv", "SetDefDispEnv"]
    yield "the-table-lists-the-served-graphics-routines", same((status, listed), (0, served))
    yield "no-routine-of-the-table-takes-a-callback-so-nothing-is-stored-to-call-later", same([n for n in listed if "Callback" in n], [])

    shot = work / "picture.bin"
    status, lines, seconds = rig.run("picture", rig.native(shot))
    ticks = [int(l.split()[2]) for l in lines if l.startswith("t: ticks ")]
    yield "picture-run-ends-by-itself-with-status-0", same((status, lines[-1] if lines else None, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, "t: done", []))
    colors, drawn = expected_picture()
    yield "picture-matches-every-pixel-computed-here", diff_picture(read_picture(shot), colors)
    # PsyZ keeps the opacity of a drawn pixel in bit 15 of what StoreImage reads; the PS1 would show the texel's own bit 15 (0 here)
    yield "picture-bit-15-reads-back-as-psyz-opacity-1-on-drawn-pixels-0-elsewhere", diff_picture(read_picture(shot), colors, drawn)
    yield "picture-window-was-held-for-a-few-seconds", same((2.5 <= seconds <= 30, bool(ticks) and ticks[0] >= 100), (True, True))

    shot2 = work / "unknown.bin"
    status, lines, _ = rig.run("unknown", rig.native(shot2))
    yield "a-copy-command-with-data-in-a-list-ends-the-program-with-the-line-and-the-graphics-status", same((status, lines[-1:], shot2.exists()), (EXIT_GRAPHICS, [
        "stop: gpu: the packet at 0x80131340 holds a command of kind 0xA0 (copy rectangle CPU to VRAM) at word 0; the port does not decode this kind yet"], False))

    for mode, text in (
        ("cycle", "stop: gpu: the list from 0x80100000 has no end within 524288 packets (at 0x80100000); a cycle, or a list never terminated"),
        ("outside", "stop: gpu: ordering table link 0xa00000 (from the packet at 0x80100000) is outside RAM"),
        ("past", "stop: gpu: the packet at 0x801ffff0 is 20 words long and runs past the end of RAM"),
        ("notram", "stop: gpu: DrawOTag(0x00001000): the list does not start in RAM (its links are RAM addresses, so a first node on the caller's stack or in the scratchpad is refused)"),
    ):
        status, lines, seconds = rig.run(mode)
        yield f"list-{mode}-stops-with-one-line-and-the-graphics-status", same((status, lines, seconds < 30), (EXIT_GRAPHICS, [text], True))


    rig.section = "images"
    # ---- the game's data read as an attacker would ----
    RAM_END = 0x80200000
    # image routines: the library fits the width and height (zero or less -> 1, above the buffer -> 1023 and 511), then the
    # rectangle must be wholly inside the frame buffer
    def stops(ops_, line):
        st, ls, secs = rig.run("script", *ops_)
        return same((st, ls[-1] if ls else None, secs < 60), (EXIT_GRAPHICS, "stop: " + line, True))

    def outside(name, x, y, w, h):
        return f"gpu: {name}: the rectangle ({x},{y} {w}x{h}) is not inside the 1024x512 frame buffer"

    fitted = rig.run("script", "env", "rect:0x80100000:0:0:1024:512", "rect:0x80100010:0:0:0:0", "rect:0x80100020:0:0:-5:-5", "rect:0x80100030:0:0:32767:32767",
                     "load:0x80100000:0x80000800", "load:0x80100010:0x80000800", "load:0x80100020:0x80000800", "load:0x80100030:0x80000800",
                     "peek:0x80100004", "peek:0x80100014", "peek:0x80100024", "peek:0x80100034")
    yield "load-image-fits-width-and-height-in-place-as-the-library-does", same([l for l in fitted[1] if l != "t: done"][-8:], [
        "t: LoadImage 0", "t: LoadImage 0", "t: LoadImage 0", "t: LoadImage 0",
        "t: peek 80100004: 01ff03ff", "t: peek 80100014: 00010001", "t: peek 80100024: 00010001", "t: peek 80100034: 01ff03ff"])
    for routine, op in (("LoadImage", "load:0x80100000:0x80000800"), ("StoreImage", "store:0x80100000:0x80000800"), ("ClearImage", "clear:0x80100000")):
        edge = rig.run("script", "env", "rect:0x80100000:1:0:1023:511", op, "rect:0x80100000:0:1:1023:511", op, "rect:0x80100000:1023:511:1:1", op, "rect:0x80100000:0:0:1:1", op)
        yield f"{routine.lower()}-accepts-rectangles-ending-exactly-at-the-right-and-bottom-edge", same([l for l in edge[1] if l.startswith("t: " + routine)], [f"t: {routine} 0"] * 4)
        for tag, rect in (("one-pixel-past-the-right-edge", (2, 0, 1023, 511)), ("one-pixel-past-the-bottom-edge", (0, 2, 1023, 511)), ("x-1024", (1024, 0, 1, 1)), ("y-512", (0, 512, 1, 1)),
                          ("x-negative", (-1, 0, 4, 4)), ("y-negative", (0, -1, 4, 4)), ("x-lowest-halfword", (-32768, 0, 4, 4)), ("x-highest-halfword", (32767, 0, 4, 4)),
                          ("width-and-position-both-highest", (32767, 32767, 32767, 32767))):
            yield f"{routine.lower()}-rectangle-{tag}-stops", stops(["rect:0x80100000:%d:%d:%d:%d" % rect, op], outside(routine, rect[0], rect[1], rect[2], rect[3]))
    move = rig.run("script", "env", "rect:0x80100000:0:0:512:512", "move:0x80100000:512:0", "rect:0x80100010:0:0:0:5", "move:0x80100010:0:0", "rect:0x80100020:0:0:5:0", "move:0x80100020:3000:3000",
                   "rect:0x80100030:1023:511:1:1", "move:0x80100030:1023:511")
    yield "move-image-edges-and-the-empty-rectangle-the-library-returns-minus-one-for", same([l for l in move[1] if l != "t: done"][-4:], ["t: MoveImage 0", "t: MoveImage -1", "t: MoveImage -1", "t: MoveImage 0"])
    for tag, ops_, line in (
        ("destination-one-pixel-past-the-right-edge", ["rect:0x80100000:0:0:512:512", "move:0x80100000:513:0"], "gpu: MoveImage: the rectangle (0,0 512x512) moved to (513,0) is not inside the 1024x512 frame buffer"),
        ("destination-one-pixel-past-the-bottom-edge", ["rect:0x80100000:0:0:512:512", "move:0x80100000:0:1"], "gpu: MoveImage: the rectangle (0,0 512x512) moved to (0,1) is not inside the 1024x512 frame buffer"),
        ("destination-negative", ["rect:0x80100000:0:0:4:4", "move:0x80100000:-1:0"], "gpu: MoveImage: the rectangle (0,0 4x4) moved to (-1,0) is not inside the 1024x512 frame buffer"),
        ("destination-the-highest-int", ["rect:0x80100000:0:0:4:4", "move:0x80100000:2147483647:0"], "gpu: MoveImage: the rectangle (0,0 4x4) moved to (2147483647,0) is not inside the 1024x512 frame buffer"),
        ("destination-the-lowest-int", ["rect:0x80100000:0:0:4:4", "move:0x80100000:0:-2147483648"], "gpu: MoveImage: the rectangle (0,0 4x4) moved to (0,-2147483648) is not inside the 1024x512 frame buffer"),
        ("source-past-the-edge", ["rect:0x80100000:1:0:1024:4", "move:0x80100000:0:0"], "gpu: MoveImage: the rectangle (1,0 1024x4) moved to (0,0) is not inside the 1024x512 frame buffer"),
        ("negative-width", ["rect:0x80100000:0:0:-1:4", "move:0x80100000:0:0"], "gpu: MoveImage: the rectangle (0,0 -1x4) moved to (0,0) is not inside the 1024x512 frame buffer"),
        ("highest-halfwords", ["rect:0x80100000:32767:32767:32767:32767", "move:0x80100000:32767:32767"], "gpu: MoveImage: the rectangle (32767,32767 32767x32767) moved to (32767,32767) is not inside the 1024x512 frame buffer"),
    ):
        yield f"move-image-{tag}-stops", stops(ops_, line)

    # buffers and rectangles against the end of RAM: a stop with the graphics status
    yield "load-image-pixels-ending-exactly-at-the-end-of-ram-are-accepted", same(rig.run("script", "rect:0x80100000:0:0:16:16", "load:0x80100000:0x801ffe00")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "load-image-pixels-one-word-later-stop", stops(["rect:0x80100000:0:0:16:16", "load:0x80100000:0x801ffe04"], "gpu: LoadImage: 512 bytes at 0x801ffe04 are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "load-image-odd-size-rounds-up-to-a-word-last-valid", same(rig.run("script", "rect:0x80100000:0:0:3:1", "load:0x80100000:0x801ffff8")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "load-image-odd-size-first-invalid", stops(["rect:0x80100000:0:0:3:1", "load:0x80100000:0x801ffffc"], "gpu: LoadImage: 8 bytes at 0x801ffffc are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "store-image-pixels-first-invalid", stops(["rect:0x80100000:0:0:16:16", "store:0x80100000:0x801ffe04"], "gpu: StoreImage: 512 bytes at 0x801ffe04 are not inside the PS1's RAM, its scratchpad or the caller's stack")
    for tag, at, ok in (("first-byte", "0x1f800000", True), ("last-valid-place", "0x1f800200", True), ("one-word-later", "0x1f800204", False), ("past-the-kilobyte", "0x1f800400", False)):
        if ok:
            yield f"store-image-into-the-scratchpad-{tag}-is-accepted", same(rig.run("script", "rect:0x80100000:0:0:16:16", f"store:0x80100000:{at}")[1][-2:], ["t: StoreImage 0", "t: done"])
        else:
            yield f"store-image-into-the-scratchpad-{tag}-stops", stops(["rect:0x80100000:0:0:16:16", f"store:0x80100000:{at}"], f"gpu: StoreImage: 512 bytes at {at} are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "load-image-from-an-unmapped-segment-stops", stops(["rect:0x80100000:0:0:16:16", "load:0x80100000:0xc0000000"], "gpu: LoadImage: 512 bytes at 0xc0000000 are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "load-image-whole-frame-buffer-last-valid-place-ends-at-the-end-of-ram", same(rig.run("script", "rect:0x80100000:0:0:1024:512", "load:0x80100000:0x80100bfc")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "load-image-whole-frame-buffer-one-word-later-stops", stops(["rect:0x80100000:0:0:1024:512", "load:0x80100000:0x80100c00"], "gpu: LoadImage: 1045508 bytes at 0x80100c00 are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "rectangle-pointer-at-the-last-valid-place", same(rig.run("script", "rect:0x801ffff8:0:0:1:1", "load:0x801ffff8:0x80000800")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "rectangle-pointer-first-invalid-stops", stops(["load:0x801ffffc:0x80000800"], "gpu: LoadImage: 8 bytes at 0x801ffffc are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "move-image-rectangle-in-the-scratchpad-last-valid-place", same(rig.run("script", "rect:0x1f8003f8:0:0:4:4", "move:0x1f8003f8:8:8")[1][-2:], ["t: MoveImage 0", "t: done"])
    yield "move-image-rectangle-pointer-past-the-scratchpad-stops", stops(["move:0x1f800400:0:0"], "gpu: MoveImage: 8 bytes at 0x1f800400 are not inside the PS1's RAM, its scratchpad or the caller's stack")
    yield "clear-image-rectangle-pointer-outside-ram-stops", stops(["clear:0x80200000"], "gpu: ClearImage: 8 bytes at 0x80200000 are not inside the PS1's RAM, its scratchpad or the caller's stack")

    rig.section = "tables"
    # ordering tables against the end of RAM
    yield "clearotagr-30-entries-the-largest-table-the-game-declares", same(rig.run("script", "cotagr:0x80130000:30", "peek:0x80130000", "peek:0x80130074")[1][-4:], [
        "t: ClearOTagR returns 80130000", "t: peek 80130000: 00ffffff", "t: peek 80130074: 00130070", "t: done"])
    yield "clearotag-30-entries-returns-the-last-entry", same(rig.run("script", "cotag:0x80130000:30", "peek:0x80130000", "peek:0x80130074")[1][-4:], [
        "t: ClearOTag returns 80130074", "t: peek 80130000: 00130004", "t: peek 80130074: 00ffffff", "t: done"])
    yield "clearotagr-a-table-of-all-of-ram-is-accepted", same(rig.run("script", "cotagr:0x80000000:0x80000", "peek:0x801ffffc")[1][-3:], ["t: ClearOTagR returns 80000000", "t: peek 801ffffc: 001ffff8", "t: done"])
    yield "clearotagr-one-entry-more-than-ram-has-stops", stops(["cotagr:0x80000000:0x80001"], "gpu: ClearOTagR: 2097156 bytes at 0x80000000 are not inside the PS1's RAM")
    yield "clearotag-last-valid-place-near-the-end", same(rig.run("script", "cotag:0x801fffc0:16", "peek:0x801fffc0")[1][-3:], ["t: ClearOTag returns 801ffffc", "t: peek 801fffc0: 001fffc4", "t: done"])
    yield "clearotag-first-invalid-place-near-the-end", stops(["cotag:0x801fffc0:17"], "gpu: ClearOTag: 68 bytes at 0x801fffc0 are not inside the PS1's RAM")
    yield "clearotag-zero-entries-stops", stops(["cotag:0x80130000:0"], "gpu: ClearOTag(0x80130000, 0): the table has no entry")
    yield "clearotagr-negative-count-stops", stops(["cotagr:0x80130000:-1"], "gpu: ClearOTagR(0x80130000, -1): the table has no entry")
    yield "clearotag-the-largest-count-stops-without-overflowing-the-size", stops(["cotag:0x80130000:0x7fffffff"], "gpu: ClearOTag: 8589934588 bytes at 0x80130000 are not inside the PS1's RAM")

    rig.section = "walker"
    # the walker: start, link, packet end and count, last valid and first invalid
    yield "list-start-at-the-last-word-of-ram-is-valid", same(rig.run("script", "w:0x801ffffc:0x00ffffff", "draw:0x801ffffc")[1][-1:], ["t: done"])
    yield "list-start-in-the-uncached-mirror-is-valid", same(rig.run("script", "w:0x801ffffc:0x00ffffff", "draw:0xa01ffffc")[1][-1:], ["t: done"])
    yield "list-start-one-past-ram-stops", stops(["draw:0x80200000"], "gpu: DrawOTag(0x80200000): the list does not start in RAM (its links are RAM addresses, so a first node on the caller's stack or in the scratchpad is refused)")
    yield "list-start-one-past-the-uncached-mirror-stops", stops(["draw:0xa0200000"], "gpu: DrawOTag(0xa0200000): the list does not start in RAM (its links are RAM addresses, so a first node on the caller's stack or in the scratchpad is refused)")
    yield "list-link-to-the-last-word-of-the-mirrored-8-mb-is-folded-into-ram", same(rig.run("script", "w:0x801ffffc:0x00ffffff", "w:0x80100000:0x007ffffc", "draw:0x80100000")[1][-1:], ["t: done"])
    yield "list-link-to-the-first-address-past-8-mb-stops", stops(["w:0x80100000:0x00800000", "draw:0x80100000"], "gpu: ordering table link 0x800000 (from the packet at 0x80100000) is outside RAM")
    yield "list-link-to-just-below-the-end-marker-stops", stops(["w:0x80100000:0x00fffffe", "draw:0x80100000"], "gpu: ordering table link 0xfffffe (from the packet at 0x80100000) is outside RAM")
    yield "packet-ending-exactly-at-the-end-of-ram-is-valid", same(rig.run("script", "fillw:0x801fffe4:7:0", "w:0x801fffe0:0x07ffffff", "draw:0x801fffe0")[1][-1:], ["t: done"])
    yield "packet-one-word-longer-stops", stops(["w:0x801fffe0:0x08ffffff", "draw:0x801fffe0"], "gpu: the packet at 0x801fffe0 is 8 words long and runs past the end of RAM")
    yield "packet-of-255-words-at-the-start-of-ram-is-valid", same(rig.run("script", "nops:0x80100000:1:0xffffff", "draw:0x80100000")[1][-1:], ["t: done"])
    yield "a-list-of-all-of-ram-in-one-chain-ends-within-the-count", same(rig.run("script", "chain:0x80000000:0x80000:0xffffff", "draw:0x80000000")[1][-1:], ["t: done"])
    yield "a-chain-that-loops-back-after-two-nodes-stops-at-the-count", same(rig.run("script", "w:0x80100000:0x00100004", "w:0x80100004:0x00100000", "draw:0x80100000")[1][-1:], [
        "stop: gpu: the list from 0x80100000 has no end within 524288 packets (at 0x80100000); a cycle, or a list never terminated"])

    # both sides of the conversion: PsyZ's buffer filled to the last packet that fits and the first that does not
    for count in (1638, 1639, 1640, 3000):
        status, lines, _ = rig.run("script", "env", f"tiles:0x80100000:{count}", "draw:0x80100000", "pixel:0:0", f"pixel:{(count - 1) % 64}:{(count - 1) // 64}", f"pixel:{count % 64}:{count // 64}")
        px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
        yield f"{count}-tiles-all-reach-the-picture-across-the-buffer-boundary", same((status, px), (0, [0x7FFF, 0x7FFF, 0]))
    status, lines, _ = rig.run("script", "env", "nops:0x80100000:40:0xffffff", "draw:0x80100000")
    yield "forty-packets-of-255-words-fill-the-buffer-several-times-and-end", same((status, lines[-1]), (0, "t: done"))
    # the 255-word nops followed by tiles in one list: link the last nop node to the tiles
    status, lines, _ = rig.run("script", "env", "tiles:0x80140000:3", "nops:0x80100000:40:0x140000", "draw:0x80100000", "pixel:0:0", "pixel:2:0", "pixel:3:0")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    yield "the-tiles-after-forty-packets-of-255-words-are-drawn", same((status, px), (0, [0x7FFF, 0x7FFF, 0]))
    status, lines, _ = rig.run("script", "env", "extremes:0x80100000", "draw:0x80100000", "pixel:0:0", "pixel:63:47")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    yield "every-primitive-type-at-extreme-values-then-a-white-tile-covers-the-corner", same((status, px, lines[-1]), (0, [0x7FFF, 0x7FFF], "t: done"))
    status, lines, _ = rig.run("script", "senv:0x80100000:1", "senv:0x80101000:0")
    yield "setdrawenv-writes-nine-words-at-most-and-nothing-past-its-packet", same((status, lines[-3:]), (0, ["t: senv words 9 canary cafecafe", "t: senv words 6 canary cafecafe", "t: done"]))


    rig.section = "warmup"
    # the warm-up: at the layer's start, changes no pixel, and a list drawn first works
    frame = rig.run("script", "frame")
    yield "warm-up-leaves-the-whole-frame-buffer-as-it-was-all-zero", same([l for l in frame[1] if l != "t: done"][-1:], ["t: frame rc 0 nonzero 0"])
    kept = rig.run("script", "rect:0x80100000:1023:511:1:1", "w:0x80100100:0x12345678", "load:0x80100000:0x80100100", "frame")
    yield "a-pixel-loaded-right-after-resetgraph-is-the-only-one-set", same([l for l in kept[1] if l != "t: done"][-2:], ["t: LoadImage 0", "t: frame rc 0 nonzero 1"])
    for routine, ops_ in (("LoadImage", ["rect:0x80100000:0:0:4:4", "load:0x80100000:0x80100100"]), ("DrawOTag", ["list0:0x80100000", "draw:0x80100000"]),
                          ("StoreImage", ["rect:0x80100000:0:0:4:4", "store:0x80100000:0x80100100"]), ("ClearImage", ["rect:0x80100000:0:0:4:4", "clear:0x80100000"]),
                          ("MoveImage", ["rect:0x80100000:0:0:4:4", "move:0x80100000:8:8"]), ("PutDrawEnv", ["env"])):
        yield f"{routine}-before-resetgraph-stops-with-a-line", stops(["noreset", *ops_], f"gpu: {routine} was called before ResetGraph(0), which sets up the GPU's routines")
    status, lines, _ = rig.run("script", "list0:0x80100000", "draw:0x80100000", "pixel:5:5", "pixel:8:8", "pixel:9:9", "pixel:4:5")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    yield "a-list-drawn-as-the-first-graphics-call-after-resetgraph-gives-the-right-picture", same((status, px), (0, [0x7FFF, 0x7FFF, 0, 0]))
    status, lines, secs = rig.run("nodisplay")
    yield "psyz-that-cannot-start-ends-with-a-line-and-the-graphics-status", same((status, lines[-1:], secs < 60), (EXIT_GRAPHICS, [
        "stop: gpu: PsyZ could not open its window or its GPU device (no display or no usable GPU; its own lines above say why)"], True))

    rig.section = "prims"
    # each primitive type the game uses and a few more: the packet that is as long as the kind needs, one of 255 words
    # (the rest are no-operation words), and one word too short
    for name, code, need in (("POLY_FT4", 0x2C, 9), ("POLY_GT4", 0x3C, 12), ("POLY_F3", 0x20, 4), ("POLY_FT3", 0x24, 7), ("POLY_G3", 0x30, 6), ("POLY_GT3", 0x34, 9),
                              ("POLY_F4", 0x28, 5), ("POLY_G4", 0x38, 8), ("TILE", 0x60, 3), ("TILE_16", 0x78, 2), ("SPRT", 0x64, 4), ("SPRT_16", 0x7C, 3),
                              ("SPRT_8", 0x74, 3), ("LINE_F2", 0x40, 3), ("LINE_G2", 0x50, 4), ("fill", 0x02, 3), ("copy", 0x80, 4)):
        for length in (need, 255):
            status, lines, _ = rig.run("script", "env", f"prim:0x80100000:0x{code:02x}:{length}", "draw:0x80100000", "pixel:0:0")
            yield f"{name}-packet-of-{length}-words-is-converted", same((status, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, []))
        yield f"{name}-packet-one-word-too-short-stops", stops(["env", f"prim:0x80100000:0x{code:02x}:{need - 1}", "draw:0x80100000"] if need > 1 else ["env"],
                                                            f"gpu: the packet at 0x80100000 holds a command of kind 0x{code:02X} at word 0 that needs {need} words; {need - 1} are left")
    for name, code in (("GP0 drawing mode", 0xE1), ("GP0 texture window", 0xE2), ("GP0 area start", 0xE3), ("GP0 area end", 0xE4), ("GP0 offset", 0xE5), ("GP0 mask", 0xE6)):
        for length in (1, 2, 255):
            status, lines, _ = rig.run("script", "env", f"prim:0x80100000:0x{code:02x}:{length}", "draw:0x80100000")
            yield f"{name}-packet-of-{length}-words-is-converted", same((status, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, []))


    rig.section = "stream"
    # ---- every command of a packet is checked before PsyZ sees the packet ----
    def packet(words, nxt="0xffffff", addr="0x80100000"):
        return f"pkt:{addr}:{nxt}:" + ",".join(f"{w:08x}" for w in words)

    def short(addr, code, at, need, left):
        return f"gpu: the packet at {addr} holds a command of kind 0x{code:02X} at word {at} that needs {need} words; {left} are left"

    tile = lambda x, y, w, h, rgb=0xFFFFFF: [0x60000000 | rgb, (y << 16) | x, (h << 16) | w]
    blue_fill = [0x02FF0000, 0x00050005, 0x00040004]
    # the two probes of the review
    yield "an-incomplete-fill-after-a-nop-is-the-named-stop", stops(["env", packet([0x00000000, 0x020000FF]), "draw:0x80100000"], short("0x80100000", 0x02, 1, 3, 1))
    status, lines, _ = rig.run("script", "env", packet([0x00000000, *blue_fill]), "draw:0x80100000", "pixel:5:5", "recover", packet([0x00000000, 0x020000FF], addr="0x80100100"), "draw:0x80100100", "pixel:5:5", "pixel:8:8")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    stopped = [l for l in lines if l.startswith(("stop:", "t: stopped"))]
    yield "after-a-valid-blue-fill-the-incomplete-payload-is-the-named-stop-and-the-picture-is-unchanged", same(
        (status, px, stopped), (0, [0x7C00, 0x7C00, 0x7C00], ["stop: " + short("0x80100100", 0x02, 1, 3, 1), "t: stopped 7"]))
    # an incomplete command as the second, the third and the last of a packet, after a no-op, after each setting word, after a primitive
    incomplete = {
        "second": ([0xE3000000, 0x2C000000], 0x2C, 1, 9, 1),
        "third": ([0x00000000, 0xE1000000, 0x64000000, 0x00000000], 0x64, 2, 4, 2),
        "last-after-a-complete-sprite-16": ([0x7C000000, 0, 0, 0x7C000000], 0x7C, 3, 3, 1),
        "after-a-complete-tile": ([*tile(1, 1, 1, 1), 0x02FF0000], 0x02, 3, 3, 1),
        "textured-quad-one-word-short": ([0x2C000000] + [0] * 7, 0x2C, 0, 9, 8),
        "gouraud-quad-one-word-short": ([0x3C000000] + [0] * 10, 0x3C, 0, 12, 11),
        "the-fill-with-its-position-only": ([0x02000000, 0x00050005], 0x02, 0, 3, 2),
        "the-copy-with-three-words": ([0x80000000, 0, 0], 0x80, 0, 4, 3),
        "a-line-with-one-vertex": ([0x40000000, 0], 0x40, 0, 3, 2),
        "a-gouraud-line-with-one-vertex": ([0x50000000, 0, 0], 0x50, 0, 4, 3),
    }
    for name, code in (("E1", 0xE1), ("E2", 0xE2), ("E3", 0xE3), ("E4", 0xE4), ("E5", 0xE5), ("E6", 0xE6)):
        incomplete[f"after-setting-word-{name}"] = ([code << 24, 0x02000000], 0x02, 1, 3, 1)
    for name, (words, code, at, need, left) in incomplete.items():
        yield f"an-incomplete-command-{name}-is-the-named-stop", stops(["env", packet(words), "draw:0x80100000"], short("0x80100000", code, at, need, left))
    yield "a-stream-that-leaves-one-word-over-is-the-named-stop", stops(["env", packet([*tile(1, 1, 1, 1), 0x00000000, 0x02000000]), "draw:0x80100000"], short("0x80100000", 0x02, 4, 3, 1))
    # several complete commands of different kinds in one packet, the last ending exactly at the packet's end
    words = [0xE3000000, 0xE4000000 | (47 << 10) | 63, 0xE5000000, 0x00000000, *tile(2, 2, 4, 4), 0x03000000, 0xE0000000, 0xE7000000,
             *tile(20, 20, 2, 2), 0x01000000, 0x02FF0000, 0x001E001E, 0x00040004]
    status, lines, _ = rig.run("script", "env", packet(words), "draw:0x80100000", "pixel:3:3", "pixel:20:20", "pixel:31:31", "pixel:10:10", "pixel:34:34")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    yield "a-packet-of-several-complete-commands-draws-all-of-them", same((status, px, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, [0x7FFF, 0x7FFF, 0x7C00, 0, 0], []))
    # the largest packet, 255 words, of 85 commands
    status, lines, _ = rig.run("script", "env", "tilepkt:0x80100000:85", "draw:0x80100000", "pixel:0:40", "pixel:84:40", "pixel:85:40")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    yield "a-packet-of-255-words-holding-85-tiles-draws-them-all", same((status, px), (0, [0x7FFF, 0x7FFF, 0]))

    rig.section = "display"
    # the game's start-up calls SetDispMask(0) and then ResetGraph(0): SetDispMask needs no set-up, and the reset turns the display off
    status, lines, _ = rig.run("script", "noreset", "setmask:0", "reset", "display", "env", "list0:0x80100000", "draw:0x80100000", "pixel:5:5")
    yield "setmask-0-then-resetgraph-then-a-list-the-start-up-order-does-not-stop-and-draws", same((status, [l for l in lines if l.startswith(("stop:", "gpu:", "t: display", "t: pixel"))]), (0, ["t: display 0", "t: pixel 5 5 ffff"]))
    status, lines, _ = rig.run("script", "noreset", "setmask:1", "display", "reset", "display", "env", "list0:0x80100000", "draw:0x80100000", "pixel:5:5")
    yield "setmask-1-before-resetgraph-is-on-until-the-reset-turns-the-display-off", same((status, [l for l in lines if l.startswith(("stop:", "gpu:", "t: display", "t: pixel"))]), (0, ["t: display 1", "t: display 0", "t: pixel 5 5 ffff"]))
    status, lines, _ = rig.run("script", "noreset", "setmask:0", "setmask:1", "reset", "setmask:1", "display", "setmask:0", "display")
    yield "setmask-after-resetgraph-sets-the-display-as-asked", same((status, [l for l in lines if l.startswith(("stop:", "gpu:", "t: display"))]), (0, ["t: display 1", "t: display 0"]))
    status, lines, _ = rig.run("script", "noreset", "dsync", "reset", "dsync")
    yield "drawsync-before-and-after-resetgraph-returns-0", same((status, [l for l in lines if l.startswith(("stop:", "gpu:", "t: DrawSync"))]), (0, ["t: DrawSync 0", "t: DrawSync 0"]))
    yield "resetgraph-with-another-mode-before-resetgraph-0-stops", stops(["noreset", "reset1"], "gpu: ResetGraph with a mode other than 0 was called before ResetGraph(0), which sets up the GPU's routines")
    status, lines, _ = rig.run("script", "env", "display", "reset1", "display")
    yield "resetgraph-1-leaves-the-display-as-it-is", same((status, [l for l in lines if l.startswith("t: display")]), (0, ["t: display 0", "t: display 0"]))
    status, lines, _ = rig.run("script", "setmask:1", "reset1", "display")
    yield "resetgraph-1-after-setmask-1-keeps-the-display-on", same((status, [l for l in lines if l.startswith("t: display")]), (0, ["t: display 1"]))


    rig.section = "memory"
    # the game's memory on this machine: RAM, scratchpad, the live part of the running stack (the game's locals), nothing else
    status, lines, _ = rig.run("stack")
    got = {l[len("t: span "):].rsplit(" ", 1)[0]: int(l.rsplit(" ", 1)[1]) for l in lines if l.startswith("t: span ")}
    want = {
        "ram first byte": 1, "ram last byte": 1, "ram whole": 1, "ram one past the end, 1 byte": 0, "ram last byte, 2 bytes": 0, "ram one byte before": 0,
        "ram zero length inside": 1, "ram zero length at the end": 0, "ram a size that wraps": 0, "ram the largest size": 0,
        "scratch whole": 1, "scratch one byte more": 0, "scratch last byte": 1, "scratch one past": 0, "scratch one before": 0, "null": 0,
    }
    for tag, foreign in (("main", False), ("fiber", True), ("main with a suspended fiber", True)):
        want.update({f"{tag} local": 1, f"{tag} local-zero-length": 1, f"{tag} dead-stack-below-the-frame": 0, f"{tag} last-bytes-below-the-base": 1,
                     f"{tag} at-the-base": 0, f"{tag} straddling-the-base": 0, f"{tag} zero-length-at-the-base": 0, f"{tag} heap": 0,
                     f"{tag} program-data": 0, f"{tag} program-code": 0})
        if foreign:
            want[f"{tag} other-stack"] = 0
    yield "stack-run-ends-by-itself", same((status, lines[-1:]), (0, ["t: done"]))
    for name in want:
        yield f"game-span-{name.replace(' ', '-').replace(',', '')}-is-{want[name]}", same(got.get(name), want[name])
    yield "game-span-answers-nothing-else", same(sorted(set(got) - set(want)), [])
    for mode, text in (("main", "main"), ("fiber", "fiber")):
        status, lines, _ = rig.run("roundtrip-" + mode)
        yield f"a-rectangle-and-a-buffer-in-locals-of-the-game-code-work-{'on-the-main-stack' if mode == 'main' else 'in-a-fiber'}", same((status, [l for l in lines if "roundtrip" in l]), (0, [f"t: {text} roundtrip through locals 1"]))
    status, lines, _ = rig.run("heapload")
    yield "a-buffer-on-the-heap-is-refused-with-the-named-stop", same((status, lines[-1].startswith("stop: gpu: LoadImage: 32 bytes at 0x") and lines[-1].endswith("are not inside the PS1's RAM, its scratchpad or the caller's stack")), (EXIT_GRAPHICS, True))
    status, lines, _ = rig.run("stacknode")
    yield "a-first-list-node-on-the-stack-is-refused-and-the-line-says-why", same((status, lines[-1].startswith("stop: gpu: DrawOTag(0x") and lines[-1].endswith("so a first node on the caller's stack or in the scratchpad is refused)")), (EXIT_GRAPHICS, True))

    rig.section = "stream"
    # a kind the walker does not decode ends the program, in any position, and nothing of its packet is drawn
    def not_decoded(addr, code, name, at):
        return f"gpu: the packet at {addr} holds a command of kind 0x{code:02X} ({name}) at word {at}; the port does not decode this kind yet"

    poly = "polyline, its length depends on a terminator word"
    for tag, words, code, name, at in (
        ("a-polyline-first-in-a-packet", [0x48000000, 0, 0, 0, 0x55555555], 0x48, poly, 0),
        ("a-gouraud-polyline-first-in-a-packet", [0x5C000000] + [0] * 8, 0x5C, poly, 0),
        ("a-polyline-after-a-complete-primitive", [*tile(2, 2, 4, 4), 0x58000000, 0, 0, 0, 0, 0, 0x55555555], 0x58, poly, 3),
        ("a-copy-to-vram-after-a-nop", [0x00000000, 0xA0000000, 0, 0], 0xA0, "copy rectangle CPU to VRAM", 1),
        ("a-copy-from-vram-first-in-a-packet", [0xC0000000, 0, 0], 0xC0, "copy rectangle VRAM to CPU", 0),
        ("a-mirror-of-the-vram-copy", [0x81000000, 0, 0, 0], 0x81, "copy rectangle VRAM to VRAM, mirror", 0),
        ("the-interrupt-request-after-a-tile", [*tile(2, 2, 4, 4), 0x1F000000], 0x1F, "interrupt request", 3),
    ):
        status, lines, _ = rig.run("script", "env", "recover", packet(words), "draw:0x80100000", "pixel:3:3")
        yield f"{tag}-is-the-named-stop-and-nothing-of-the-packet-is-drawn", same(
            ([l for l in lines if l.startswith(("stop:", "gpu:"))], lines[-2:], status), (["stop: " + not_decoded("0x80100000", code, name, at)], ["t: pixel 3 3 0000", "t: done"], 0))
    # no-operation kinds are checked as one-word commands and kept from PsyZ; a packet of nothing else draws nothing and ends well
    status, lines, _ = rig.run("script", "env", packet([0x03000000, 0x1E000000, 0xE0000000, 0xE7000000, 0xFF000000]), "draw:0x80100000", "pixel:0:0")
    yield "a-packet-of-no-operation-kinds-is-accepted", same((status, lines[-2:]), (0, ["t: pixel 0 0 0000", "t: done"]))
    # stale buffer: the words of an earlier packet in the conversion buffer are never read by a later one
    status, lines, _ = rig.run("script", "env", "tilepkt:0x80100000:85", "draw:0x80100000", packet([0x00000000, 0x02000000, 0x00040004, 0x00040004], addr="0x80100800"), "draw:0x80100800", "pixel:4:4")
    yield "a-short-packet-after-a-long-one-uses-only-its-own-words", same((status, lines[-2:]), (0, ["t: pixel 4 4 0000", "t: done"]))
    # PutDrawEnv's packet is a stream too: the background fill, aligned (fill command) and not aligned (rectangle command), goes through the walker whole
    for tag, x, w in (("aligned", 0, 320), ("unaligned-x", 3, 320), ("unaligned-width", 0, 300)):
        status, lines, _ = rig.run("script", f"penv:0x80100000:1:{x}:{w}", "pixel:4:4")
        yield f"putdrawenv-with-a-background-{tag}-passes-the-walker", same((status, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, []))
    # MargePrim joins the second packet's tag word into the first packet's stream (the library leaves it there): the tag's top
    # byte, the second packet's length, is then a command of its own, a no-operation kind for 3; the merged packet is a stream
    # like any other
    status, lines, _ = rig.run("script", "env", "marge:0x80100000", "draw:0x80100000", "pixel:30:10", "pixel:32:10", "pixel:31:10")
    px = [int(l.split()[4], 16) & 0x7FFF for l in lines if l.startswith("t: pixel ")]
    yield "two-tiles-merged-with-margeprim-are-both-drawn-the-tag-word-between-them-is-a-no-operation", same(
        (status, [l for l in lines if l.startswith(("t: Marge", "stop:", "gpu:"))], px), (0, ["t: MargePrim 0 length 7"], [0x7FFF, 0x7FFF, 0]))
    # a list whose packets have length 0 draws nothing and ends
    status, lines, _ = rig.run("script", "env", "chain:0x80100000:100:0xffffff", "draw:0x80100000")
    yield "a-list-of-packets-of-length-0-draws-nothing", same((status, lines[-1]), (0, "t: done"))

    # ---- the sweep: all 256 kinds, each followed in its own packet by a complete blue fill ----
    # An accepted kind is given the words the console takes, whose top bytes are 0x03 where the kind allows (PsyZ reports a
    # command of that kind as unsupported, so a word of the kind that PsyZ took for a command would show), and then the fill:
    # status 0 and the probe pixel (5,5) blue, and PsyZ has reported nothing as unsupported. A refused kind ends with the stop
    # line that names it and the pixel stays 0. The expected outcome is worked out from the tables above, not from gpu.c.
    disagree = [k for k in range(256) if console_length(k) is not None and not psyz_length(k)[1] and console_length(k) != psyz_length(k)[0]]
    yield "the-kinds-where-the-console-and-psyz-take-another-number-of-words-are-the-lines-with-bit-2", same(
        disagree, [0x44, 0x45, 0x46, 0x47, 0x54, 0x55, 0x56, 0x57])
    # does a command that PsyZ reports as unsupported reach this rig? A copy of the layer that forwards the no-operation kind
    # 0x03 to PsyZ (the real one leaves it out) is run with it: PsyZ's own log line must be among the lines the rig keeps.
    forwarding = work / "forward" / "gpu.c"
    forwarding.parent.mkdir(exist_ok=True)
    text = rig.gpu_source.read_text()
    nop_old, len_old = "    return (code >= 0x03 && code <= 0x1e) || code == 0xe0 || code >= 0xe7;", "    if (code <= 0x01) return 1;"
    if nop_old in text and len_old in text:
        forwarding.write_text(text.replace(nop_old, "    return code > 0x1000;").replace(len_old, "    if (code <= 0x01 || code == 0x03) return 1;"))
        other = Rig(rig.cc, rig.prefix, forwarding.parent, rig.psyz_build, forwarding)
        other.memory_source, other.section, other.groups = rig.memory_source, rig.section, rig.groups
        problem = other.build() if rig.groups is None or "stream" in rig.groups else None
        if problem:
            yield "a-copy-of-the-layer-that-forwards-a-no-operation-builds", problem
        else:
            status, lines, _ = other.run("script", "env", packet([0x03000000, *blue_fill]), "draw:0x80100000", "pixel:5:5")
            reports = [l for l in other.raw if "unsupported command 03" in l]
            yield "psyz-reports-an-unsupported-command-and-the-rig-sees-the-line", same((status, len(reports), lines[-2:]), (0, 1, ["t: pixel 5 5 7c00", "t: done"]))
    else:
        yield "psyz-reports-an-unsupported-command-and-the-rig-sees-the-line", "gpu.c no longer has the text this control replaces; adjust the control"
    for kind in range(256):
        console = console_length(kind)
        if kind_is_accepted(kind):
            count = 1 if psyz_length(kind)[1] else console
            low = 0x7FFFF if kind == 0xE4 else 0
            args = [0x03000000 | (i + 1) for i in range(count - 1)]
            if kind == 0x02:                                # the kind's own drawing stays in the frame buffer, far from the probe pixel
                args = [0x00C8012C, 0x00040004]
            if kind == 0x80:
                args = [0x00C8012C, 0x00C80190, 0x00040004]
            # the kind comes twice: before the fill (a walker or PsyZ that reads it longer loses the fill) and as the last
            # command of the packet (a walker that counts it longer finds the packet too short: nothing else notices a count
            # that is one too long, since the fill's words are forwarded as they are)
            words = [(kind << 24) | low, *args, *blue_fill, (kind << 24) | low, *args]
            status, lines, _ = rig.run("script", "env", "recover", packet(words), "draw:0x80100000", "pixel:5:5")
            reports = [l for l in rig.raw if "unsupported command" in l]
            yield f"sweep-kind-0x{kind:02X}-accepted-then-the-blue-fill-draws", same(
                (status, [l for l in lines if l.startswith(("stop:", "gpu:"))], lines[-2:], reports), (0, [], ["t: pixel 5 5 7c00", "t: done"], []))
        else:
            words = [kind << 24, 0x03000001, 0x03000002, *blue_fill]
            status, lines, _ = rig.run("script", "env", "recover", packet(words), "draw:0x80100000", "pixel:5:5")
            yield f"sweep-kind-0x{kind:02X}-refused-with-the-line-and-the-pixel-unchanged", same(
                ([l for l in lines if l.startswith(("stop:", "gpu:"))], lines[-2:], status),
                (["stop: " + not_decoded("0x80100000", kind, kind_refusal_name(kind), 0)], ["t: pixel 5 5 0000", "t: done"], 0))
    # the owner's probes of 2026-10-10, each beside its flag-free twin
    for code, twin, rest in ((0x44, 0x40, [0, 1]), (0x54, 0x50, [0, 1, 2])):
        for kind in (code, twin):
            words = [(kind << 24) | 0xFF, *rest, *blue_fill]
            status, lines, _ = rig.run("script", "env", "recover", packet(words), "draw:0x80100000", "pixel:5:5")
            if kind == code:
                want = (["stop: " + not_decoded("0x80100000", kind, kind_refusal_name(kind), 0)], ["t: pixel 5 5 0000", "t: done"])
            else:
                want = ([], ["t: pixel 5 5 7c00", "t: done"])
            yield f"the-probe-{kind:02x}-then-a-complete-blue-fill-is-{'refused-and-the-pixel-stays-0' if kind == code else 'drawn-blue'}", same(
                ([l for l in lines if l.startswith(("stop:", "gpu:"))], lines[-2:]), want)

    rig.section = "basic"
    status, lines, _ = rig.run("closed")
    yield "closed-window-ends-the-program-with-status-0-and-the-line", same((status, lines), (0, ["t: before", "stop: window closed"]))

    status, lines, _ = rig.run("presents")
    milliseconds = [int(l.split()[2]) for l in lines if l.startswith("t: presents-ms ")]
    # 300 presents. PsyZ's frame limiter is off (its log line says so); the swapchain of a window that cannot tear
    # still holds each present to the display's refresh (about 16.7 ms here), which is all the time one may take.
    yield "present-takes-at-most-one-refresh-300-presents-under-7.5-s", same((status, bool(milliseconds) and milliseconds[0] < 7500), (0, True))

    # A program that outlasts its time is gone when run() returns, and nothing else is: the same program under the
    # SAME file name, started from another folder as another run would start it, lives on and ends by itself.
    if rig.groups is None or rig.section in rig.groups:
        (rig.work / "other-run").mkdir()
        other = rig.work / "other-run" / rig.exe.name
        shutil.copy(rig.exe, other)
        decoy = subprocess.Popen([*rig.prefix, str(other), "linger", "8000"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        status, lines, _ = rig.run("linger", "20000", timeout=2)
        listed = subprocess.run(["tasklist.exe", "/FI", f"IMAGENAME eq {rig.exe.name}", "/FO", "CSV", "/NH"], capture_output=True, text=True, timeout=60).stdout
        try:
            out, _ = decoy.communicate(timeout=60)
        except subprocess.TimeoutExpired:
            decoy.kill()
            out = "(the other run's program did not end)"
        theirs = [l for l in out.replace("\r\n", "\n").splitlines() if l.startswith("t: ")]
        got = (status, lines, listed.count(rig.exe.name), decoy.returncode, theirs)
    else:
        got = None
    # one program of that name is still listed right after the stop: the other run's
    yield "a-program-that-outlasts-its-time-is-gone-and-another-runs-program-of-the-same-name-lives-on", same(
        got, (-999, ["(timeout)"], 1, 0, ["t: lingering", "t: done"]))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--psyz-build", required=True, type=Path, help="a build folder of psyzbuild.py")
    parser.add_argument("--group", default="", help="run only these sections of cases (comma separated; %s)" % ", ".join(SECTIONS))
    parser.add_argument("--memory-source", type=Path, default=SRC / "memory.c", help="another copy of memory.c to test")
    parser.add_argument("--gpu-source", type=Path, default=SRC / "gpu.c", help="another copy of gpu.c to test")
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostgpu-", dir=BUILD))
    failed = 0
    try:
        rig = Rig(args.cc, args.run.split(), work, args.psyz_build.resolve(), args.gpu_source.resolve())
        rig.memory_source = args.memory_source.resolve()
        rig.groups = (set(args.group.split(",")) | {"build"}) if args.group else None
        groups = []
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        groups.append(program_cases(rig, work))
        for group in groups:
            try:
                for name, detail in group:
                    if rig.groups is not None and rig.section not in rig.groups:
                        continue
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
