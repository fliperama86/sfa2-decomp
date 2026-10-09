#!/usr/bin/env python3
"""Controls for the graphics layer: gpu.c, linked like the game's C, with PsyZ underneath.

    python3 test_hostgpu.py --cc CROSS_CC --psyz-build DIR [--run PREFIX]

DIR is a build folder of psyzbuild.py (it holds psyz.json, the library and the
headers), for example
`python3 port/tools/psyzbuild.py --psyz port/external/psyz --build SCRATCH/psyz`.
The patch and the tool are tested by test_psyzbuild.py; this file starts from a
built PsyZ.

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
- packets of kinds PsyZ does not decode in the list are reported once per kind
  by name and skipped, and the rest of the picture is right;
- the game's data read as an attacker would: lists ended wrongly (a cycle, a
  link outside RAM, a packet running past RAM, a list not in RAM), each with the
  last valid value and the first invalid one of the start, a link, a packet's
  end and the count; image routines with rectangles at and past the frame
  buffer's edges (skipped with one line, never an overflow of a size), buffers
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

PREFIX is a command prefix to start the Windows program; without it the program
is started directly (on Windows, or from a shell under WSL, where paths are
converted with wslpath). When no Windows program can be started the program
cases are not run and this says so and ends with status 2. A program that does
not end is stopped by name with taskkill.exe. The run's files live in a folder
of its own under port/build/, removed at the end.
"""

from __future__ import annotations

import argparse
import json
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
EXE = "hostgpu-trial.exe"
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
 *   script OP...  operations given as arguments (see script())
 */
#include "port.h"
#include "gpu.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifdef TRIAL_SDL
#include <SDL3/SDL_events.h>
#endif

/* ---- stand-ins for the driver ---- */

static unsigned ticks;
static unsigned resets;
static int presenting = 1;

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
    int i = 2;
    if (argc > 2 && !strcmp(argv[2], "noreset")) i++;    /* the first graphics call is then the script's own */
    else ((f_ii)lib("ResetGraph"))(0);
    for (; i < argc; i++) {
        char buf[200];
        char *a[10];
        int n = 0;
        char *tok;
        unsigned long long k;
        strncpy(buf, argv[i], sizeof buf - 1);
        buf[sizeof buf - 1] = 0;
        for (tok = strtok(buf, ":"); tok && n < 10; tok = strtok(NULL, ":")) a[n++] = tok;
        if (!strcmp(a[0], "w")) W((uint32_t)num(a[1]))[0] = (uint32_t)num(a[2]);
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
    map_or_die();
    if (!strcmp(mode, "semantics")) semantics();
    else if (!strcmp(mode, "picture") && argc > 2) scene(0, argv[2], 3000);
    else if (!strcmp(mode, "unknown") && argc > 2) scene(1, argv[2], 200);
    else if (!strcmp(mode, "cycle") || !strcmp(mode, "outside") || !strcmp(mode, "past") || !strcmp(mode, "notram")) bad_list(mode);
    else if (!strcmp(mode, "closed")) closed();
    else if (!strcmp(mode, "presents")) presents();
    else if (!strcmp(mode, "names")) names();
    else if (!strcmp(mode, "nodisplay")) nodisplay();
    else if (!strcmp(mode, "script")) script(argc, argv);
    else { printf("t: bad usage\n"); return 93; }
    printf("t: done\n");
    return 0;
}

'''

# ---------------------------------------------------------------- helpers


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"



class Rig:
    def __init__(self, cc: str, prefix: list[str], work: Path, psyz_build: Path):
        self.cc, self.prefix, self.work, self.psyz_build = cc, prefix, work, psyz_build
        self.wsl = not prefix and shutil.which("wslpath") is not None
        self.info: dict = {}
        self.exe: Path | None = None

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
            "gpu.o": [*flags, "-Werror", "-DPORT_HAVE_PSYZ", *(f"-D{d}" for d in self.info["define"]), "-isystem", str(include), str(SRC / "gpu.c")],
            "memory.o": [*flags, "-Werror", str(SRC / "memory.c")],
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
        argv = [*self.prefix, str(self.exe), mode, *more]
        started = time.time()
        try:
            proc = subprocess.run(argv, capture_output=True, text=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            subprocess.run(["taskkill.exe", "/IM", EXE, "/F"], capture_output=True, timeout=60)
            return -999, ["(timeout)"], time.time() - started
        lines = (proc.stdout + proc.stderr).replace("\r\n", "\n").splitlines()
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
    stub = rig.compile(["-O1", "-Wall", "-Wextra", "-Werror", "-c", "-I", str(SRC), str(SRC / "gpu.c"), "-o", str(work / "stub.o")])
    yield "gpu-c-compiles-without-psyz-warning-free", same((stub.returncode, stub.stderr.strip()), (0, ""))
    nm = rig.cc[:-3] + "nm" if rig.cc.endswith("gcc") else "nm"
    syms = subprocess.run([nm, "-g", str(work / "stub.o")], capture_output=True, text=True)
    defined = sorted(l.split()[-1] for l in syms.stdout.splitlines() if " T " in l or " D " in l or " R " in l)
    undefined = sorted(l.split()[-1] for l in syms.stdout.splitlines() if l.strip().startswith("U "))
    yield "stub-defines-the-tables-and-present-and-needs-nothing", same((defined, undefined), (sorted(["_port_gpu_library", "_port_gpu_present", "_port_gpu_read_frame"]), []))

    reason = rig.build()
    yield "trial-program-builds-gpu-c-with-psyz-warning-free", reason
    if reason:
        return

    status, lines, _ = rig.run("semantics")
    want = expected_semantics() + ["t: ticks 0", "t: done"]
    yield "semantics-status-0", same(status, 0)
    yield "semantics-every-line-as-worked-out", same(lines, want)


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
    gpu_lines = [l for l in lines if l.startswith("gpu:")]
    yield "unknown-kinds-are-reported-once-each-by-name", same(gpu_lines, [
        "gpu: packet kind 0xA0 (copy rectangle CPU to VRAM) is not handled by PsyZ; skipped (once per kind)",
        "gpu: packet kind 0xC0 (copy rectangle VRAM to CPU) is not handled by PsyZ; skipped (once per kind)"])
    yield "unknown-kinds-are-skipped-the-rest-is-drawn", same((status, diff_picture(read_picture(shot2), colors)), (0, None))

    for mode, text in (
        ("cycle", "stop: gpu: the list from 0x80100000 has no end within 524288 packets (at 0x80100000); a cycle, or a list never terminated"),
        ("outside", "stop: gpu: ordering table link 0xa00000 (from the packet at 0x80100000) is outside RAM"),
        ("past", "stop: gpu: the packet at 0x801ffff0 is 20 words long and runs past the end of RAM"),
        ("notram", "stop: gpu: DrawOTag(0x00001000): the list does not start in RAM"),
    ):
        status, lines, seconds = rig.run(mode)
        yield f"list-{mode}-stops-with-one-line-and-the-graphics-status", same((status, lines, seconds < 30), (EXIT_GRAPHICS, [text], True))


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
    yield "load-image-pixels-one-word-later-stop", stops(["rect:0x80100000:0:0:16:16", "load:0x80100000:0x801ffe04"], "gpu: LoadImage: 512 bytes at 0x801ffe04 are not inside the PS1's RAM")
    yield "load-image-odd-size-rounds-up-to-a-word-last-valid", same(rig.run("script", "rect:0x80100000:0:0:3:1", "load:0x80100000:0x801ffff8")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "load-image-odd-size-first-invalid", stops(["rect:0x80100000:0:0:3:1", "load:0x80100000:0x801ffffc"], "gpu: LoadImage: 8 bytes at 0x801ffffc are not inside the PS1's RAM")
    yield "store-image-pixels-first-invalid", stops(["rect:0x80100000:0:0:16:16", "store:0x80100000:0x801ffe04"], "gpu: StoreImage: 512 bytes at 0x801ffe04 are not inside the PS1's RAM")
    yield "store-image-into-the-scratchpad-stops", stops(["rect:0x80100000:0:0:16:16", "store:0x80100000:0x1f800000"], "gpu: StoreImage: 512 bytes at 0x1f800000 are not inside the PS1's RAM")
    yield "load-image-from-an-unmapped-segment-stops", stops(["rect:0x80100000:0:0:16:16", "load:0x80100000:0xc0000000"], "gpu: LoadImage: 512 bytes at 0xc0000000 are not inside the PS1's RAM")
    yield "load-image-whole-frame-buffer-last-valid-place-ends-at-the-end-of-ram", same(rig.run("script", "rect:0x80100000:0:0:1024:512", "load:0x80100000:0x80100bfc")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "load-image-whole-frame-buffer-one-word-later-stops", stops(["rect:0x80100000:0:0:1024:512", "load:0x80100000:0x80100c00"], "gpu: LoadImage: 1045508 bytes at 0x80100c00 are not inside the PS1's RAM")
    yield "rectangle-pointer-at-the-last-valid-place", same(rig.run("script", "rect:0x801ffff8:0:0:1:1", "load:0x801ffff8:0x80000800")[1][-2:], ["t: LoadImage 0", "t: done"])
    yield "rectangle-pointer-first-invalid-stops", stops(["load:0x801ffffc:0x80000800"], "gpu: LoadImage: 8 bytes at 0x801ffffc are not inside the PS1's RAM")
    yield "move-image-rectangle-pointer-outside-ram-stops", stops(["move:0x1f800000:0:0"], "gpu: MoveImage: 8 bytes at 0x1f800000 are not inside the PS1's RAM")
    yield "clear-image-rectangle-pointer-outside-ram-stops", stops(["clear:0x80200000"], "gpu: ClearImage: 8 bytes at 0x80200000 are not inside the PS1's RAM")

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

    # the walker: start, link, packet end and count, last valid and first invalid
    yield "list-start-at-the-last-word-of-ram-is-valid", same(rig.run("script", "w:0x801ffffc:0x00ffffff", "draw:0x801ffffc")[1][-1:], ["t: done"])
    yield "list-start-in-the-uncached-mirror-is-valid", same(rig.run("script", "w:0x801ffffc:0x00ffffff", "draw:0xa01ffffc")[1][-1:], ["t: done"])
    yield "list-start-one-past-ram-stops", stops(["draw:0x80200000"], "gpu: DrawOTag(0x80200000): the list does not start in RAM")
    yield "list-start-one-past-the-uncached-mirror-stops", stops(["draw:0xa0200000"], "gpu: DrawOTag(0xa0200000): the list does not start in RAM")
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

    # each primitive type the game uses and a few more: the packet that is as long as the kind needs, one of 255 words
    # (the rest are no-operation words), and one word too short
    for name, code, need in (("POLY_FT4", 0x2C, 9), ("POLY_GT4", 0x3C, 12), ("POLY_F3", 0x20, 4), ("POLY_FT3", 0x24, 7), ("POLY_G3", 0x30, 6), ("POLY_GT3", 0x34, 9),
                              ("POLY_F4", 0x28, 5), ("POLY_G4", 0x38, 8), ("TILE", 0x60, 3), ("TILE_16", 0x78, 2), ("SPRT", 0x64, 4), ("SPRT_16", 0x7C, 3),
                              ("SPRT_8", 0x74, 3), ("LINE_F2", 0x40, 3), ("LINE_G2", 0x50, 4), ("fill", 0x02, 3), ("copy", 0x80, 4)):
        for length in (need, 255):
            status, lines, _ = rig.run("script", "env", f"prim:0x80100000:0x{code:02x}:{length}", "draw:0x80100000", "pixel:0:0")
            yield f"{name}-packet-of-{length}-words-is-converted", same((status, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, []))
        yield f"{name}-packet-one-word-too-short-stops", stops(["env", f"prim:0x80100000:0x{code:02x}:{need - 1}", "draw:0x80100000"] if need > 1 else ["env"],
                                                            f"gpu: the packet at 0x80100000 is kind 0x{code:02X} with {need - 1} words; that kind needs {need}")
    for name, code in (("GP0 drawing mode", 0xE1), ("GP0 texture window", 0xE2), ("GP0 area start", 0xE3), ("GP0 area end", 0xE4), ("GP0 offset", 0xE5), ("GP0 mask", 0xE6)):
        for length in (1, 2, 255):
            status, lines, _ = rig.run("script", "env", f"prim:0x80100000:0x{code:02x}:{length}", "draw:0x80100000")
            yield f"{name}-packet-of-{length}-words-is-converted", same((status, [l for l in lines if l.startswith(("stop:", "gpu:"))]), (0, []))

    status, lines, _ = rig.run("closed")
    yield "closed-window-ends-the-program-with-status-0-and-the-line", same((status, lines), (0, ["t: before", "stop: window closed"]))

    status, lines, _ = rig.run("presents")
    milliseconds = [int(l.split()[2]) for l in lines if l.startswith("t: presents-ms ")]
    # 300 presents. PsyZ's frame limiter is off (its log line says so); the swapchain of a window that cannot tear
    # still holds each present to the display's refresh (about 16.7 ms here), which is all the time one may take.
    yield "present-takes-at-most-one-refresh-300-presents-under-7.5-s", same((status, bool(milliseconds) and milliseconds[0] < 7500), (0, True))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--psyz-build", required=True, type=Path, help="a build folder of psyzbuild.py")
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostgpu-", dir=BUILD))
    failed = 0
    try:
        rig = Rig(args.cc, args.run.split(), work, args.psyz_build.resolve())
        groups = []
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        groups.append(program_cases(rig, work))
        for group in groups:
            try:
                for name, detail in group:
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
