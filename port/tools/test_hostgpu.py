#!/usr/bin/env python3
"""Controls for the graphics layer: psyzbuild.py, the patch, gpu.c and PsyZ underneath.

    python3 test_hostgpu.py --cc CROSS_CC [--run PREFIX]

Two groups.

The tool and the patch (no program is started; the build of PsyZ is the real
one, cached under port/build/psyz-TRIPLET, so the first run takes a minute):
- the patch applies to the pinned PsyZ file at exactly one place, and the result
  is the file without the two preprocessor lines around the flush;
- the patch is refused, and the file left as it was, when the place is not there,
  when it is there twice, and when the file is missing;
- the whole tool refuses a PsyZ whose source lacks the place (status 2, one line,
  nothing built), and builds the real one (the lines it prints, psyz.json, a
  library that exists, the submodule's file unchanged, the copy patched).

The runtime's gpu.c, linked like the game's C with a made-up game of this file's
own (a C program written below, using the library only by the names of
port_gpu_library[] and port_gpu_overrides[], in Sony's layouts built byte by
byte in the PS1's RAM) and stand-ins for the driver's port_tick, port_halt and
port_callbacks_reset. Every expected value is worked out HERE from Sony's
documented layouts, never read back from the runtime. Cases:
- gpu.c compiles without a warning with PsyZ's headers, and to empty tables
  without them;
- the routines on Sony's layouts (the ordering table setters, AddPrim and its
  kin, MargePrim, the Set* setters, GetTPage, GetClut, SetDef*Env, SetDrawMode,
  SetDrawEnv's words) produce the bytes worked out here;
- the picture: a flat rectangle and a textured sprite from a made-up texture
  through a hand-built ordering table, in a window for about three seconds; the
  picture is read back with StoreImage and compared with the 64 x 48 pixels
  computed here, pixel by pixel;
- packets of kinds PsyZ does not decode in the list are reported once per kind
  by name and skipped, and the rest of the picture is right;
- a list ended wrongly (a cycle, a link outside RAM, a packet running past RAM,
  a list not in RAM) stops with one line and the graphics status;
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
import hashlib
import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import psyzbuild  # noqa: E402

PORT = HERE.parent
SRC = PORT / "src"
BUILD = PORT / "build"
PSYZ = PORT / "external" / "psyz"
LIBGPU = Path("psyz/src/psyz/libgpu.c")
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]
EXE = "hostgpu-trial.exe"
EXIT_GRAPHICS = 7

TRIAL_C = r'''
/* The made-up "game" for the graphics layer's controls (test_hostgpu.py).
 *
 * It is linked like the game's C: the runtime's gpu.c, the port's memory.c (the PS1's RAM at
 * its own addresses) and stand-ins for the driver's port_tick, port_halt and
 * port_callbacks_reset. It reaches the library the way the game's jumps do: by the names in
 * port_gpu_library[] and port_gpu_overrides[], with Sony's layouts built byte by byte in RAM.
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

static void *override(const char *name)
{
    const struct port_override *o;
    for (o = port_gpu_overrides; o->name; o++) if (!strcmp(o->name, name)) return o->host;
    printf("t: no override named %s\n", name);
    fflush(stdout);
    exit(90);
}

typedef int      (*f_ii)(int);
typedef void     (*f_vi)(int);
typedef int      (*f_rv)(void);
typedef int      (*f_rp)(void *, void *);
typedef int      (*f_rpiii)(void *, int, int, int);
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
    ((f_pp)override("func_80158374"))(W(env));
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

int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "";
    setvbuf(stdout, NULL, _IONBF, 0);
    map_or_die();
    if (!strcmp(mode, "semantics")) semantics();
    else if (!strcmp(mode, "picture") && argc > 2) scene(0, argv[2], 3000);
    else if (!strcmp(mode, "unknown") && argc > 2) scene(1, argv[2], 200);
    else if (!strcmp(mode, "cycle") || !strcmp(mode, "outside") || !strcmp(mode, "past") || !strcmp(mode, "notram")) bad_list(mode);
    else if (!strcmp(mode, "closed")) closed();
    else if (!strcmp(mode, "presents")) presents();
    else { printf("t: bad usage\n"); return 93; }
    printf("t: done\n");
    return 0;
}

'''

# ---------------------------------------------------------------- helpers


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def raises(fn, *needles):
    """None when fn raises a psyzbuild.Problem whose text has every needle."""
    try:
        fn()
    except psyzbuild.Problem as err:
        missing = [n for n in needles if n not in str(err)]
        return None if not missing else f"the error {str(err)!r} does not name {missing}"
    return "no error was raised"


def pin_commit() -> str | None:
    proc = subprocess.run(["git", "-C", str(PSYZ), "rev-parse", "HEAD"], capture_output=True, text=True)
    return proc.stdout.strip() if proc.returncode == 0 else None


# ---------------------------------------------------------------- the tool and the patch


def patch_cases(cc: str, work: Path):
    real = PSYZ / LIBGPU
    if not real.is_file():
        yield "the pinned PsyZ source is present", f"{real} is missing (git submodule update --init port/external/psyz)"
        return
    text = (PORT / "psyz.patch").read_text()
    original = real.read_text()

    tree = work / "fix-ok"
    (tree / LIBGPU).parent.mkdir(parents=True)
    (tree / LIBGPU).write_text(original)
    done = psyzbuild.apply_patch(text, tree)
    lines = original.split("\n")
    k = next(i for i, l in enumerate(lines) if l == "#ifdef __EMSCRIPTEN__" and lines[i + 1].strip() == "Draw_FlushBuffer();")
    want = "\n".join(lines[:k] + [lines[k + 1]] + lines[k + 3:])
    yield "patch-applies-at-exactly-one-place", same(len(done), 1)
    yield "patch-result-is-the-file-without-the-two-lines-around-the-flush", same((tree / LIBGPU).read_text(), want)
    yield "patch-names-the-pins-commit", same(psyzbuild.patch_commit(text), pin_commit())

    tree = work / "fix-absent"
    (tree / LIBGPU).parent.mkdir(parents=True)
    changed = original.replace("        return;\n    }\n    while (1) {", "        return ;\n    }\n    while (1) {")
    (tree / LIBGPU).write_text(changed)
    yield "fixture-without-the-place-differs-from-the-real-file", same(changed != original, True)
    yield "patch-refused-when-the-place-is-absent", raises(lambda: psyzbuild.apply_patch(text, tree), "libgpu.c", "no place")
    yield "patch-refused-file-left-as-it-was", same((tree / LIBGPU).read_text(), changed)

    tree = work / "fix-twice"
    (tree / LIBGPU).parent.mkdir(parents=True)
    block_start = original.index("        // fast path for 32-bit systems")
    block_end = original.index("    while (1) {\n        if (queue_len", block_start)
    block = original[block_start:block_end]
    twice = original + "\nstatic void again(void) {\n    if (sizeof(u_long) == 4) {\n" + block + "    while (1) {\n    }\n}\n"
    (tree / LIBGPU).write_text(twice)
    yield "patch-refused-when-the-place-is-there-twice", raises(lambda: psyzbuild.apply_patch(text, tree), "libgpu.c", "2 places")
    yield "patch-refused-twice-file-left-as-it-was", same((tree / LIBGPU).read_text(), twice)

    tree = work / "fix-nofile"
    tree.mkdir()
    yield "patch-refused-when-the-file-is-missing", raises(lambda: psyzbuild.apply_patch(text, tree), "libgpu.c", "no such file")
    yield "patch-without-a-hunk-is-refused", raises(lambda: psyzbuild.parse_patch("only text\n"), "no hunk")

    # the whole tool on a PsyZ that lacks the place
    fake = work / "fake-psyz"
    (fake / LIBGPU).parent.mkdir(parents=True)
    (fake / LIBGPU).write_text(changed)
    (fake / "psyz" / "CMakeLists.txt").write_text("project(psyz)\n")
    (fake / "decomp").mkdir()
    (fake / "decomp" / "x").write_text("x\n")
    (fake / "external" / "SDL").mkdir(parents=True)
    (fake / "external" / "SDL" / "CMakeLists.txt").write_text("project(SDL)\n")
    out = work / "fake-build"
    proc = subprocess.run([sys.executable, str(HERE / "psyzbuild.py"), "--psyz", str(fake), "--cc", cc, "--build", str(out)], capture_output=True, text=True, timeout=300)
    yield "tool-refuses-a-psyz-without-the-place", same(
        (proc.returncode, proc.stdout, len(proc.stderr.strip().splitlines()), "no place" in proc.stderr and "libgpu.c" in proc.stderr),
        (2, "", 1, True))
    yield "tool-refusal-builds-nothing", same((out / "obj").exists(), False)
    proc = subprocess.run([sys.executable, str(HERE / "psyzbuild.py"), "--psyz", str(work / "nothing"), "--cc", cc, "--build", str(out)], capture_output=True, text=True, timeout=300)
    yield "tool-refuses-a-missing-psyz-folder", same((proc.returncode, proc.stdout, len(proc.stderr.strip().splitlines())), (2, "", 1))
    proc = subprocess.run([sys.executable, str(HERE / "psyzbuild.py"), "--cc", str(work / "no-such-gcc"), "--build", str(out)], capture_output=True, text=True, timeout=300)
    yield "tool-refuses-a-missing-compiler", same((proc.returncode, proc.stdout, "no-such-gcc" in proc.stderr), (2, "", True))

    # the real build
    before = hashlib.sha256(real.read_bytes()).hexdigest()
    proc = subprocess.run([sys.executable, str(HERE / "psyzbuild.py"), "--cc", cc], capture_output=True, text=True, timeout=2400)
    lines = proc.stdout.splitlines()
    yield "tool-builds-the-real-psyz", same((proc.returncode, proc.stderr.strip()), (0, ""))
    if proc.returncode != 0:
        return
    info = json.loads((Path(lines[3][len("library: "):]).parents[2] / "psyz.json").read_text()) if len(lines) >= 5 else {}
    yield "tool-prints-commit-patch-library-include-link", same(
        (lines[0], lines[1], [l.split(":")[0] for l in lines[2:]]),
        (f"psyz: {pin_commit()}", "patch: psyz.patch applied at 1 place", ["library", "include", "link"]))
    library = Path(lines[2][len("library: "):])
    yield "tool-library-exists-and-is-the-first-link-item", same((library.is_file(), lines[4].split()[1:2] == [str(library)]), (True, True))
    link_items = lines[4][len("link: "):].split()
    yield "tool-link-line-names-sdl-and-system-libraries", same(
        (any(x.endswith("libSDL3.a") for x in link_items), "-lgdi32" in link_items, "-lws2_32" in link_items), (True, True, True))
    yield "tool-psyz-json-matches-the-output", same((info.get("link"), info.get("define"), info.get("include")), (link_items, ["__psyz"], lines[3][len("include: "):]))
    patched = (Path(lines[3][len("include: "):]).parents[1] / LIBGPU).read_text()
    yield "tool-copy-is-patched", same(("#ifdef __EMSCRIPTEN__\n        Draw_FlushBuffer();" in patched, "        Draw_FlushBuffer();\n        return;" in patched), (False, True))
    yield "tool-leaves-the-submodule-file-alone", same(hashlib.sha256(real.read_bytes()).hexdigest(), before)
    status = subprocess.run(["git", "-C", str(PSYZ), "status", "--porcelain"], capture_output=True, text=True)
    yield "tool-leaves-the-submodule-clean", same(status.stdout.strip(), "")


# ---------------------------------------------------------------- the program


class Rig:
    def __init__(self, cc: str, prefix: list[str], work: Path):
        self.cc, self.prefix, self.work = cc, prefix, work
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
        proc = subprocess.run([sys.executable, str(HERE / "psyzbuild.py"), "--cc", self.cc], capture_output=True, text=True, timeout=2400)
        if proc.returncode != 0:
            return "psyzbuild.py failed: " + proc.stderr.strip()
        lib = [l for l in proc.stdout.splitlines() if l.startswith("library: ")][0][len("library: "):]
        self.info = json.loads((Path(lib).parents[2] / "psyz.json").read_text())
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
    yield "stub-defines-the-tables-and-present-and-needs-nothing", same((defined, undefined), (sorted(["_port_gpu_library", "_port_gpu_overrides", "_port_gpu_present"]), []))

    reason = rig.build()
    yield "trial-program-builds-gpu-c-with-psyz-warning-free", reason
    if reason:
        return

    status, lines, _ = rig.run("semantics")
    want = expected_semantics() + ["t: ticks 0", "t: done"]
    yield "semantics-status-0", same(status, 0)
    yield "semantics-every-line-as-worked-out", same(lines, want)

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
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostgpu-", dir=BUILD))
    failed = 0
    try:
        rig = Rig(args.cc, args.run.split(), work)
        groups = [patch_cases(args.cc, work)]
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
