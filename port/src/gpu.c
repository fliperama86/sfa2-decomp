/* The graphics layer: host routines for the game's graphics
 * library functions, drawn by PsyZ.
 *
 * The game builds its drawing lists exactly as it did on the PS1, in Sony's
 * layouts. PsyZ has its own, a little different. Everything below is written
 * so that the conversion between the two happens in one place (the seam
 * routines of the "seam" section: chunk_add and send_list) and every other
 * routine works on one side only.
 *
 * Sony's layouts that the game uses (all little endian, natural alignment;
 * sizes checked against the game's own buffers: the game keeps one buffer per
 * display page of 0xe8 bytes, a 30-entry ordering table (0x78), a DISPENV
 * (0x14) and a DRAWENV (0x5c)), and what PsyZ expects instead
 * (psyz/include/libgpu.h of the pinned commit unless another file is named):
 *
 *   tag word       one u32: bits 0..23 the physical address of the next packet
 *                  (0xffffff ends the list), bits 24..31 the number of u32
 *                  words that follow the tag. A packet is the tag and those
 *                  words. The ordering table is an array of tags with length 0.
 *                  PsyZ: two u_long words, {next, len}, 8 bytes, next a full
 *                  host pointer (lines 207-229 for P_TAG and O_TAG; libgpu.c
 *                  lines 148-165 for the walker, which reads next at offset 0,
 *                  len at offset 4 and the payload from offset 8; the end is
 *                  next == 0xffffff, line 138).
 *   POLY_FT4       0x28 bytes, 9 words after the tag: r0 g0 b0 code, x0 y0,
 *                  u0 v0 clut, x1 y1, u1 v1 tpage, x2 y2, u2 v2 pad, x3 y3,
 *                  u3 v3 pad. PsyZ: 0x2c (line 285), the same after offset 8.
 *   SPRT           0x14 bytes, 4 words: r0 g0 b0 code, x0 y0, u0 v0 clut, w h.
 *                  PsyZ 0x18 (line 481). SPRT_16: 0x10 bytes, 3 words (no w,h;
 *                  PsyZ 0x14, line 520). TILE: 0x10 bytes, 3 words: r0 g0 b0
 *                  code, x0 y0, w h. PsyZ 0x14 (line 459).
 *   DR_MODE        0x0c bytes, 2 words: the GP0 words E1 (drawing mode) and E2
 *                  (texture window). PsyZ 0x10 (line 528).
 *   DR_ENV         0x40 bytes, tag and 15 GP0 words. PsyZ 0x44 (line 237).
 *   DRAWENV        0x5c bytes: clip RECT at 0x00, ofs[2] at 0x08, tw RECT at
 *                  0x0c, tpage u16 at 0x14, dtd 0x16, dfe 0x17, isbg 0x18,
 *                  r0 g0 b0 at 0x19..0x1b, DR_ENV at 0x1c. PsyZ: the same
 *                  fields up to 0x1c, then its 0x44-byte DR_ENV: 0x60 bytes
 *                  (line 582, whose comment says 0x58: the comment is wrong
 *                  for the host build). Not used by this file: see below.
 *   DISPENV        0x14 bytes: disp RECT 0x00, screen RECT 0x08, isinter
 *                  0x10, isrgb24 0x11, two pad bytes. PsyZ: identical (line
 *                  591). The pad byte at 0x12 is written by PutDispEnv in both.
 *   RECT           four s16: x y w h, 8 bytes. PsyZ: identical (line 194).
 *
 * So RECT and DISPENV pass to PsyZ as they are (the static asserts below
 * keep that true); a list is converted packet by packet: the tag is dropped and
 * the words after it are copied behind a PsyZ tag in a buffer of the port's own,
 * a few hundred packets at a time, each buffer handed to PsyZ's DrawOTag at once
 * (PsyZ draws a list while it is enqueued, so the buffer is free again when
 * DrawOTag returns). A DRAWENV is not handed to PsyZ at all: its GP0 words are
 * made here on Sony's side (SetDrawEnv, after the library's code), in the game's own
 * DR_ENV, and that packet goes to PsyZ through the same walker. The GP0 words
 * themselves (codes 0x20..0x7f, 0xe1..0xe6, 0x02, 0x80) mean the same on both
 * sides; PsyZ decodes them in DispatchPackets (libgpu.c 53-122).
 *
 * What the host routines keep to their own: the game's copies of the library's
 * state (the data the library's C keeps at fixed addresses) are not read by
 * any host routine; PsyZ keeps its own. The pointer to a list is a PS1
 * address, which is a valid host pointer in this program.
 *
 * Deviations from PSY-Q, each stated where it happens:
 *  - ClearOTag and ClearOTagR end the table with the link 0xffffff itself. PSY-Q
 *    links the last entry to a four-word packet of no-operations in the library's
 *    data; a host address of PsyZ's would not fit in 24 bits, and such a packet
 *    draws nothing.
 *  - A list may point anywhere in the first 8 MB (RAM is mirrored four times
 *    on the PS1, DMA addresses are 24 bits); the mirror is folded to 2 MB.
 *  - GetTPage is the form for graphics type 0 (the retail GPU, which is what
 *    PsyZ reports).
 *  - ResetGraph: this game's library (code at 0x801578fc) does the full
 *    initialisation only for mode & 7 == 0 and for any other mode just
 *    cancels the drawing; PsyZ's own ResetGraph initialises fully for 3 and 5
 *    too, so the mode is mapped to 0 or 1 before PsyZ sees it.
 *
 * What each routine does was read from the library's code at the address it names ("library code").
 * The player's window: PsyZ opens it on its first use, never full screen. */

#include "gpu.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PORT_HAVE_PSYZ

const struct port_library  port_gpu_library[]   = { { 0, 0, 0 } };
void port_gpu_present(void) {}
int  port_gpu_read_frame(unsigned short *pixels) { (void)pixels; return -1; }

#else

#include <psyz.h>
#include <libgpu.h>

_Static_assert(sizeof(unsigned long) == 4, "gpu.c is written for a host whose unsigned long is 4 bytes (PsyZ's tag is then two words)");
_Static_assert(sizeof(RECT) == 8, "RECT");
_Static_assert(sizeof(DISPENV) == 0x14, "DISPENV");
_Static_assert(offsetof(DR_ENV, code) == 8 && sizeof(DR_ENV) == 0x44, "PsyZ's DR_ENV is Sony's plus the length word");

/* ---- Sony's layouts (the game's side) ---- */

struct sony_dr_env { uint32_t tag; uint32_t code[15]; };
struct sony_drawenv {
    RECT clip;
    int16_t ofs[2];
    RECT tw;
    uint16_t tpage;
    uint8_t dtd, dfe, isbg, r0, g0, b0;
    struct sony_dr_env dr_env;
};
_Static_assert(sizeof(struct sony_dr_env) == 0x40, "Sony DR_ENV");
_Static_assert(sizeof(struct sony_drawenv) == 0x5c && offsetof(struct sony_drawenv, dr_env) == 0x1c, "Sony DRAWENV");

#define LINK_END 0x00ffffffu

static void stop(const char *fmt, ...)
{
    char line[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(line, sizeof line, fmt, ap);
    va_end(ap);
    port_halt(PORT_EXIT_GRAPHICS, "%s", line);
}

/* ---- what the game hands over is checked before it is used ----
 * The game's pointers are PS1 addresses. A pointer is accepted when its segment is one of the three that
 * reach RAM (the low mirror 0, cached 4, uncached 5) and the whole span lies in the 2 MB; the host pointer
 * returned is the one in the mapping at PORT_RAM_BASE. Anything else ends the program with a line. */

static void *ram_span(const char *what, const void *p, unsigned long long length)
{
    uint32_t a = (uint32_t)(uintptr_t)p;
    uint32_t segment = a >> 29, physical = a & 0x1fffffffu;
    if ((segment != 0 && segment != 4 && segment != 5) || physical >= PORT_RAM_SIZE || length > PORT_RAM_SIZE - physical)
        stop("gpu: %s: %llu bytes at 0x%08x are not inside the PS1's RAM", what, length, a);
    return (void *)(uintptr_t)(PORT_RAM_BASE + physical);
}

/* A rectangle of the frame buffer (1024 x 512 halfwords) the image routines accept: no negative field and
 * nothing past the edges (sums in int, the fields are 16 bits). Anything else is not drawn or copied: the
 * routine says so once and returns -1. */
#define FRAME_W 1024
#define FRAME_H 512

static int rect_fits(int x, int y, int w, int h)
{
    return x >= 0 && y >= 0 && w >= 0 && h >= 0 && x + w <= FRAME_W && y + h <= FRAME_H;
}

static int rect_refused(unsigned routine, const char *what, const RECT *r)
{
    static unsigned reported;
    if (!(reported & (1u << routine))) {
        reported |= 1u << routine;
        printf("gpu: %s: the rectangle (%d,%d %dx%d) is not inside the %dx%d frame buffer; not done (once per routine)\n", what, r->x, r->y, r->w, r->h, FRAME_W, FRAME_H);
        fflush(stdout);
    }
    return -1;
}

/* ---- the first use: PsyZ's window rules ---- */

static int started;
static int closed;

static void window_closed(void)
{
    if (Psyz_QuitRequested()) {
        closed = 1;
        printf("stop: window closed\n");
        fflush(stdout);
    }
}

/* Every routine that reaches PsyZ calls this first. PsyZ opens its window and
 * GPU lazily, on its first drawing call; the pacing is the port's (port_tick
 * does one vblank every 1/60 s), so PsyZ's own frame limiter is switched off
 * before it can start. PsyZ ends the program with exit(0) from inside the
 * event pump when the window is closed; the exit handler says why. The window
 * opens at the game's first image routine or list (never full screen). */
static void begin(void)
{
    if (started) return;
    started = 1;
    Psyz_VideoSetVsyncMode(PSYZ_VSYNC_LIMITLESS);
    atexit(window_closed);
}

/* PsyZ's primitive path does not open its window or set up its vertex buffer by itself: the window opens in
 * its image routines, the buffer on the first sync, and a present before the display is switched on never
 * returns. A game may draw a list before any image routine ran, so before the first list the layer clears
 * one pixel of the frame buffer's last corner to black (a fill opens the window) and syncs. It needs
 * PsyZ's ResetGraph to have run, as the game's own first call is. */
static int warmed;

static void warm_up(void)
{
    RECT corner = { FRAME_W - 1, FRAME_H - 1, 1, 1 };
    if (warmed) return;
    warmed = 1;
    ClearImage(&corner, 0, 0, 0);
    DrawSync(0);
}

void port_gpu_present(void)
{
    if (!started) return;
    /* PsyZ's VSync(0) presents, pumps the events, and waits for the next frame
     * (with the limiter off, not at all). Its VSync also runs PsyZ's kernel
     * callbacks; this is the video part alone, which is what we want. */
    Psyz_VideoVSync(0);
}

/* The video memory as the game sees it, 1024 x 512 halfwords, into the caller's own buffer (a host buffer,
 * not the game's: used by the picture dump). -1 and an untouched buffer when the graphics were not used yet
 * or the window is gone. */
int port_gpu_read_frame(unsigned short *pixels)
{
    RECT r = { 0, 0, FRAME_W, FRAME_H };
    if (!started || closed) return -1;
    memset(pixels, 0, (size_t)FRAME_W * FRAME_H * 2);
    StoreImage(&r, (u_long *)pixels);
    return 0;
}

/* ---- seam: the list walker ---- */

/* The GP0 command kinds (the top byte of the first word after a tag) that
 * PsyZ's DispatchPackets decodes: no-operation 0x00, clear cache 0x01, fill
 * rectangle 0x02, the primitives 0x20..0x7f, copy VRAM to VRAM 0x80, and the
 * settings 0xe1..0xe6. Any other kind it reports on every packet and then
 * reads on with the next word, which turns a copy command's rectangle into
 * garbage commands, so the walker does not hand such a packet over: it says
 * once per kind what it is and skips the packet. (The GPU ignores kinds it does
 * not know; skipping is the same. A game that depended on a copy command in a
 * list would show the missing picture, with the line saying why.) */
static int kind_known(unsigned code)
{
    return code <= 0x02 || (code >= 0x20 && code <= 0x80) || (code >= 0xe1 && code <= 0xe6);
}

static const char *kind_name(unsigned code)
{
    if (code >= 0x03 && code <= 0x1e) return "reserved";
    if (code == 0x1f) return "interrupt request";
    if (code >= 0x81 && code <= 0x9f) return "copy rectangle VRAM to VRAM, mirror";
    if (code >= 0xa0 && code <= 0xbf) return "copy rectangle CPU to VRAM";
    if (code >= 0xc0 && code <= 0xdf) return "copy rectangle VRAM to CPU";
    return "no operation";
}

#define CHUNK_WORDS 8192u
/* PsyZ decodes a primitive by its code and reads as many words as that primitive has (at most 12), without
 * looking at the length of a packet that is shorter; the slack after the buffer keeps those reads inside it. */
static unsigned long chunk[CHUNK_WORDS + 32];
static unsigned chunk_used;
static unsigned long *chunk_last;

static void chunk_flush(void)
{
    if (chunk_last) {
        chunk_last[0] = LINK_END;
        DrawOTag((OT_TYPE *)chunk);
    }
    chunk_used = 0;
    chunk_last = NULL;
}

static void chunk_add(const uint32_t *words, unsigned n)
{
    unsigned long *node;
    unsigned i;
    if (chunk_used + 2 + n > CHUNK_WORDS) chunk_flush();
    node = &chunk[chunk_used];
    node[0] = LINK_END;
    node[1] = n;
    for (i = 0; i < n; i++) node[2 + i] = words[i];
    if (chunk_last) chunk_last[0] = (unsigned long)node;
    chunk_last = node;
    chunk_used += 2 + n;
}

static uint32_t *ram_at(uint32_t physical, uint32_t from)
{
    if (physical >= 0x800000u) stop("gpu: ordering table link 0x%06x (from the packet at 0x%08x) is outside RAM", physical, from);
    return (uint32_t *)(uintptr_t)(PORT_RAM_BASE + (physical & (PORT_RAM_SIZE - 1) & ~3u));
}

static void send_list(uint32_t *first)
{
    warm_up();
    static unsigned char seen[256];
    uint32_t *node;
    uint32_t start = (uint32_t)(uintptr_t)first;
    unsigned count = 0;
    if (!((start >= PORT_RAM_BASE && start < PORT_RAM_BASE + PORT_RAM_SIZE) || (start >= 0xa0000000u && start < 0xa0000000u + PORT_RAM_SIZE)))
        stop("gpu: DrawOTag(0x%08x): the list does not start in RAM", start);
    node = ram_at(start & 0xffffffu, start);
    for (;;) {
        uint32_t head = node[0];
        unsigned len = head >> 24;
        uint32_t here = (uint32_t)(uintptr_t)node;
        if (++count > PORT_GPU_MAX_PACKETS)
            stop("gpu: the list from 0x%08x has no end within %u packets (at 0x%08x); a cycle, or a list never terminated", start, PORT_GPU_MAX_PACKETS, here);
        if (here + 4u + 4u * len > PORT_RAM_BASE + PORT_RAM_SIZE)
            stop("gpu: the packet at 0x%08x is %u words long and runs past the end of RAM", here, len);
        if (len) {
            unsigned code = node[1] >> 24;
            if (kind_known(code)) {
                chunk_add(node + 1, len);
            } else if (!seen[code]) {
                seen[code] = 1;
                printf("gpu: packet kind 0x%02X (%s) is not handled by PsyZ; skipped (once per kind)\n", code, kind_name(code));
                fflush(stdout);
            }
        }
        if ((head & LINK_END) == LINK_END) break;
        node = ram_at(head & LINK_END, here);
    }
    chunk_flush();
}

/* ---- routines that go to PsyZ ---- */

/* library code at 0x801578fc. Mode & 7 == 0: print, clear the library's state,
 * ResetCallback, set up the GPU, return the graphics type; anything else: ask
 * the GPU driver to cancel the drawing and return what it returns. PsyZ's
 * ResetGraph does the same for 0, and the same as the second for 1 (it is the
 * full path for 3 and 5 as well, which this library does not do, so those map
 * to 1). The callbacks of PSY-Q's ResetCallback are kernel.c's (port_callbacks_reset). */
static int host_ResetGraph(int mode)
{
    begin();
    if ((mode & 7) == 0) {
        port_callbacks_reset();
        return ResetGraph(0);
    }
    return ResetGraph(1);
}

/* library code at 0x80157c94 returns the debug level byte. */
static int host_GetGraphDebug(void)
{
    begin();
    return GetGraphDebug();
}

/* library code at 0x80157d00. Mode 0 forgets the display environment and sends GP1
 * 0x03000001; non-zero sends 0x03000000; PsyZ's SetDispMask is the same. */
static void host_SetDispMask(int mask)
{
    begin();
    SetDispMask(mask);
}

/* library code at 0x80157d9c returns what the GPU driver's sync returns: 0 when the
 * drawing is done, and with mode 1 the number of waiting packets. Drawing
 * through PsyZ is complete when each call returns, so the answer is 0; the
 * call is one in which the game waits, so the clock gets a step first. */
static int host_DrawSync(int mode)
{
    begin();
    port_tick();
    return DrawSync(mode);
}

/* library code at 0x80157f30 (checks the RECT, queues the fill, returns the queue
 * result); PsyZ fills synchronously. The RECT's w and h are clamped in place,
 * in both. The RECT must lie in RAM, and inside the frame buffer (rect_fits). */
static int host_ClearImage(RECT *rect, int r, int g, int b)
{
    begin();
    rect = ram_span("ClearImage", rect, sizeof *rect);
    if (!rect_fits(rect->x, rect->y, rect->w, rect->h)) return rect_refused(0, "ClearImage", rect);
    return ClearImage(rect, (unsigned char)r, (unsigned char)g, (unsigned char)b);
}

/* library code at 0x80157fc4 and 0x80158028: queue the transfer, return the queue
 * result. The pixels are 16-bit words packed in u32s at the PS1 address; the
 * whole transfer (width x height halfwords, rounded up to a word) must lie in RAM.
 * The size is computed in 64 bits from fields the rectangle check has bounded. */
static unsigned long long transfer_bytes(const RECT *r)
{
    return ((unsigned long long)r->w * (unsigned long long)r->h * 2ull + 3ull) & ~3ull;
}

static int host_LoadImage(RECT *rect, void *pixels)
{
    begin();
    rect = ram_span("LoadImage", rect, sizeof *rect);
    if (!rect_fits(rect->x, rect->y, rect->w, rect->h)) return rect_refused(1, "LoadImage", rect);
    pixels = ram_span("LoadImage", pixels, transfer_bytes(rect));
    return LoadImage(rect, (u_long *)pixels);
}

static int host_StoreImage(RECT *rect, void *pixels)
{
    begin();
    rect = ram_span("StoreImage", rect, sizeof *rect);
    if (!rect_fits(rect->x, rect->y, rect->w, rect->h)) return rect_refused(2, "StoreImage", rect);
    pixels = ram_span("StoreImage", pixels, transfer_bytes(rect));
    return StoreImage(rect, (u_long *)pixels);
}

/* library code at 0x8015808c returns -1 for an empty rectangle, else the queue result. Both the source
 * rectangle and its destination (x, y) with the same width and height must lie inside the frame buffer. */
static int host_MoveImage(RECT *rect, int x, int y)
{
    begin();
    rect = ram_span("MoveImage", rect, sizeof *rect);
    if (!rect_fits(rect->x, rect->y, rect->w, rect->h) || !rect_fits(x, y, rect->w, rect->h)) return rect_refused(3, "MoveImage", rect);
    return MoveImage(rect, x, y);
}

/* DISPENV is the same on both sides (see the top), so the game's own struct is
 * handed over. Library code at 0x80158470 returns its argument. */
static DISPENV *host_PutDispEnv(DISPENV *env)
{
    begin();
    return PutDispEnv(env);
}

/* library code at 0x80158300. The game's list is followed and handed to PsyZ in
 * PsyZ's own tags (send_list). */
static void host_DrawOTag(uint32_t *list)
{
    begin();
    send_list(list);
}

/* ---- routines the port writes on Sony's layouts ---- */

static uint32_t phys(const void *p) { return (uint32_t)(uintptr_t)p & LINK_END; }
static void set_link(uint32_t *tag, uint32_t physical) { *tag = (*tag & 0xff000000u) | (physical & LINK_END); }
static void set_len_code(void *p, unsigned len, unsigned code)
{
    ((uint8_t *)p)[3] = (uint8_t)len;
    ((uint8_t *)p)[7] = (uint8_t)code;
}

/* library code at 0x80158150. Entries 0..n-2 link to the next entry with length 0,
 * the last is the end of the list; returns the last entry. PSY-Q's last link is
 * its terminator packet (deviation, top of file). n < 1 is refused (the
 * library's loop would run on past the table), and so is a table that does
 * not lie in RAM with all its n words. Returns the last entry in the segment
 * the caller used. */
static uint32_t *host_ClearOTag(uint32_t *ot, int n)
{
    uint32_t *table;
    int i;
    if (n < 1) stop("gpu: ClearOTag(0x%08x, %d): the table has no entry", (unsigned)(uintptr_t)ot, n);
    table = ram_span("ClearOTag", ot, 4ull * (unsigned)n);
    for (i = 0; i < n - 1; i++) table[i] = phys(&table[i + 1]);
    table[n - 1] = LINK_END;
    return ot + (n - 1);
}

/* library code at 0x80158208. The ordering-table DMA writes the table backwards: entry
 * i links to entry i-1 and entry 0 is the end; then the library links entry 0
 * to its terminator (deviation as above). Refused as ClearOTag is. Returns ot. */
static uint32_t *host_ClearOTagR(uint32_t *ot, int n)
{
    uint32_t *table;
    int i;
    if (n < 1) stop("gpu: ClearOTagR(0x%08x, %d): the table has no entry", (unsigned)(uintptr_t)ot, n);
    table = ram_span("ClearOTagR", ot, 4ull * (unsigned)n);
    for (i = n - 1; i > 0; i--) table[i] = phys(&table[i - 1]);
    table[0] = LINK_END;
    return ot;
}

/* library code at 0x8015bf34: p takes ot's link, ot takes p's address; the length
 * bytes stay. */
static void host_AddPrim(uint32_t *ot, uint32_t *p)
{
    set_link(p, *ot);
    set_link(ot, phys(p));
}

/* library code at 0x8015bf70: p1 takes ot's link, ot takes p0's address. */
static void host_AddPrims(uint32_t *ot, uint32_t *p0, uint32_t *p1)
{
    set_link(p1, *ot);
    set_link(ot, phys(p0));
}

/* library code at 0x8015c23c: the length byte of p0 becomes len0 + len1 + 1, unless
 * that is 0x21 or more (-1, nothing changed); returns 0. PSY-Q 4.0 leaves p1's
 * tag alone, as this does. */
static int host_MargePrim(uint8_t *p0, uint8_t *p1)
{
    unsigned len = (unsigned)p0[3] + p1[3] + 1;
    if (len >= 0x21) return -1;
    p0[3] = (uint8_t)len;
    return 0;
}

/* library code at 0x8015bfe8: bit 1 of the code byte (offset 7) set or cleared. */
static void host_SetSemiTrans(uint8_t *p, int abe)
{
    p[7] = abe ? (uint8_t)(p[7] | 0x02) : (uint8_t)(p[7] & 0xfd);
}

/* library code at 0x8015c09c, 0x8015c0ec, 0x8015c100, 0x8015c150: length byte and
 * code byte, nothing else. */
static void host_SetPolyFT4(void *p) { set_len_code(p, 9, 0x2c); }
static void host_SetSprt16(void *p) { set_len_code(p, 3, 0x7c); }
static void host_SetSprt(void *p) { set_len_code(p, 4, 0x64); }
static void host_SetTile(void *p) { set_len_code(p, 3, 0x60); }

/* library code at 0x8015bd0c, graphics type 0 branch (the type is PsyZ's: 0). */
static unsigned host_GetTPage(int tp, int abr, int x, int y)
{
    return (unsigned)(((tp & 3) << 7) | ((abr & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3ff) >> 6) | ((y & 0x200) << 2)) & 0xffffu;
}

/* library code at 0x8015bdd4: (y << 6) | ((x >> 4) & 0x3f), 16 bits. */
static unsigned host_GetClut(int x, int y)
{
    return (unsigned)((y << 6) | ((x >> 4) & 0x3f)) & 0xffffu;
}

/* library code at 0x80158a2c: length byte 2, word 1 the drawing mode, word 2 the
 * texture window. The mode (library code at 0x80158d28, type 0): 0xe1000000, bit 9 if
 * dtd, bit 10 if dfe, and tpage & 0x9ff. The window (library code at 0x80158f64): 0 for
 * a null RECT, else 0xe2000000 with the fields of x, y, w and h below. */
static uint32_t drawing_mode(int dfe, int dtd, unsigned tpage)
{
    return (dtd ? 0xe1000200u : 0xe1000000u) | (dfe ? 0x400u : 0u) | (tpage & 0x9ffu);
}

static uint32_t texture_window(const RECT *r)
{
    uint32_t x, y, w, h;
    if (!r) return 0;
    x = ((uint32_t)r->x & 0xff) >> 3;
    y = ((uint32_t)r->y & 0xff) >> 3;
    w = ((uint32_t)(-(int)r->w) & 0xff) >> 3;
    h = ((uint32_t)(-(int)r->h) & 0xff) >> 3;
    return 0xe2000000u | (y << 15) | (x << 10) | (h << 5) | w;
}

static void host_SetDrawMode(uint32_t *p, int dfe, int dtd, int tpage, const RECT *tw)
{
    ((uint8_t *)p)[3] = 2;
    p[1] = drawing_mode(dfe, dtd, (unsigned)tpage & 0xffffu);
    p[2] = texture_window(tw);
}

/* library code at 0x8015c5f0 (env, x, y, w, h): clip = (x, y, w, h), tw all 0,
 * r0 g0 b0 = 0, dtd = 1, dfe = 0, ofs = (x, y), tpage = 0x0a (the type 0 form
 * of page (0,0) at x = 640), isbg = 0. Returns env. */
static struct sony_drawenv *host_SetDefDrawEnv(struct sony_drawenv *env, int x, int y, int w, int h)
{
    env->clip.x = (int16_t)x;
    env->clip.y = (int16_t)y;
    env->clip.w = (int16_t)w;
    env->clip.h = (int16_t)h;
    env->tw.x = env->tw.y = env->tw.w = env->tw.h = 0;
    env->r0 = env->g0 = env->b0 = 0;
    env->dtd = 1;
    env->dfe = 0;
    env->ofs[0] = (int16_t)x;
    env->ofs[1] = (int16_t)y;
    env->tpage = 0x0a;
    env->isbg = 0;
    return env;
}

/* library code at 0x8015c6b0 (env, x, y, w, h): disp = (x, y, w, h), screen all 0,
 * the four bytes at 0x10..0x13 0. Returns env. */
static DISPENV *host_SetDefDispEnv(DISPENV *env, int x, int y, int w, int h)
{
    env->disp.x = (int16_t)x;
    env->disp.y = (int16_t)y;
    env->disp.w = (int16_t)w;
    env->disp.h = (int16_t)h;
    env->screen.x = env->screen.y = env->screen.w = env->screen.h = 0;
    env->isinter = env->isrgb24 = env->pad0 = env->pad1 = 0;
    return env;
}

/* library code at 0x80158d84, 0x80158e50, 0x80158f1c (type 0): the three drawing-area
 * and offset commands. The area is clamped to the 1024 x 512 VRAM of the retail
 * GPU (the library's width and height words for type 0). */
static int clamp_to(int v, int high)
{
    return v < 0 ? 0 : (v > high ? high : v);
}

static uint32_t area_word(uint32_t base, int x, int y)
{
    return base | (((uint32_t)clamp_to((int16_t)y, 511) & 0x3ff) << 10) | ((uint32_t)clamp_to((int16_t)x, 1023) & 0x3ff);
}

static uint32_t pack(int a, int b) { return ((uint32_t)(uint16_t)b << 16) | (uint16_t)a; }

/* library code at 0x80158a84 (dr_env, env). Writes the GP0 words of the environment
 * into dr_env and the word count into the top byte of its tag (the link bits
 * stay): E3 drawing area start, E4 end, E5 offset, E1 mode, E2 texture window
 * (the RECT is the environment's own, so never null here), E6 mask; then, if
 * isbg, a clear of the clip: width and height at least 1 and at most one less
 * than the VRAM's; when x and the width are both multiples of 64 the fill
 * command 0x02 with the rectangle as it is, otherwise a monochrome rectangle
 * 0x60 moved by the negative offset (it is drawn through the offset). */
static void host_SetDrawEnv(struct sony_dr_env *dr_env, struct sony_drawenv *env)
{
    unsigned n = 0;
    uint32_t *code = dr_env->code;
    code[n++] = area_word(0xe3000000u, env->clip.x, env->clip.y);
    code[n++] = area_word(0xe4000000u, (int16_t)(env->clip.w + env->clip.x - 1), (int16_t)(env->clip.h + env->clip.y - 1));
    code[n++] = 0xe5000000u | (((uint32_t)env->ofs[1] & 0x7ff) << 11) | ((uint32_t)env->ofs[0] & 0x7ff);
    code[n++] = drawing_mode(env->dfe, env->dtd, env->tpage);
    code[n++] = texture_window(&env->tw);
    code[n++] = 0xe6000000u;
    if (env->isbg) {
        int x = env->clip.x, y = env->clip.y;
        int w = env->clip.w > 0 ? (env->clip.w > 1023 ? 1023 : env->clip.w) : 1;
        int h = env->clip.h > 0 ? (env->clip.h > 511 ? 511 : env->clip.h) : 1;
        uint32_t color = ((uint32_t)env->b0 << 16) | ((uint32_t)env->g0 << 8) | env->r0;
        if ((x & 0x3f) || (w & 0x3f)) {
            code[n++] = 0x60000000u | color;
            code[n++] = pack(x - env->ofs[0], y - env->ofs[1]);
        } else {
            code[n++] = 0x02000000u | color;
            code[n++] = pack(x, y);
        }
        code[n++] = pack(w, h);
    }
    dr_env->tag = (dr_env->tag & LINK_END) | ((uint32_t)n << 24);
}

/* Library code at 0x80158374 (PutDrawEnv): SetDrawEnv into the environment's packet, set the
 * link to the end, send the packet to the GPU, keep a copy of the environment
 * as the library's current one (nothing reads that copy; GetDrawEnv has no
 * caller), return the environment. The packet goes to PsyZ through the walker. */
static struct sony_drawenv *host_PutDrawEnv(struct sony_drawenv *env)
{
    begin();
    host_SetDrawEnv(&env->dr_env, env);
    env->dr_env.tag |= LINK_END;
    send_list((uint32_t *)&env->dr_env);
    return env;
}

/* ---- the tables ---- */

/* The table: every entry does its job (the note field, which marks a routine that does nothing on purpose, is empty).
 * The deviations from PSY-Q are stated at the routines. */
const struct port_library port_gpu_library[] = {
    { "ResetGraph",       (void *)host_ResetGraph,      0 },
    { "GetGraphDebug",    (void *)host_GetGraphDebug,   0 },
    { "SetDispMask",      (void *)host_SetDispMask,     0 },
    { "DrawSync",         (void *)host_DrawSync,        0 },
    { "ClearImage",       (void *)host_ClearImage,      0 },
    { "LoadImage",        (void *)host_LoadImage,       0 },
    { "StoreImage",       (void *)host_StoreImage,      0 },
    { "MoveImage",        (void *)host_MoveImage,       0 },
    { "ClearOTag",        (void *)host_ClearOTag,       0 },
    { "ClearOTagR",       (void *)host_ClearOTagR,      0 },
    { "DrawOTag",         (void *)host_DrawOTag,        0 },
    { "func_80158470",    (void *)host_PutDispEnv,      0 },
    { "PutDrawEnv",       (void *)host_PutDrawEnv,      0 },
    { "func_80158a84",    (void *)host_SetDrawEnv,      0 },
    { "SetDrawMode",      (void *)host_SetDrawMode,     0 },
    { "GetTPage",         (void *)host_GetTPage,        0 },
    { "GetClut",          (void *)host_GetClut,         0 },
    { "AddPrim",          (void *)host_AddPrim,         0 },
    { "AddPrims",         (void *)host_AddPrims,        0 },
    { "MargePrim",        (void *)host_MargePrim,       0 },
    { "SetSemiTrans",     (void *)host_SetSemiTrans,    0 },
    { "SetPolyFT4",       (void *)host_SetPolyFT4,      0 },
    { "SetSprt16",        (void *)host_SetSprt16,       0 },
    { "SetSprt",          (void *)host_SetSprt,         0 },
    { "SetTile",          (void *)host_SetTile,         0 },
    { "SetDefDrawEnv",    (void *)host_SetDefDrawEnv,   0 },
    { "SetDefDispEnv",    (void *)host_SetDefDispEnv,   0 },
    { 0, 0, 0 },
};

#endif
