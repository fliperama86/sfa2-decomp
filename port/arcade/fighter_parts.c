#include "fighter_parts.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The fighter files' palettes and pages from the arcade set. The rules, and how they were found, are in fighter_parts.py
 * (its docstring, and palette_chunk, portrait_page and shared_page, which this follows step by step). In short:
 *  - palette n of the 64: an arcade palette (4 bits a channel, each doubled; the brightness nibble dropped; black written
 *    0x8000 because 0 is transparent; the arcade's color 15 is color 0 and colors 0 to 14 are 1 to 15), at an address the
 *    fighter, the layout and the split give; or a filler ramp of the PS1's own; or the disc's (left zero);
 *  - a portrait page is the arcade's portrait sprite (an art record of mode 4) laid out by position, a backdrop, some shadow
 *    cells of the shared page, and 18 palettes in rows 160 to 177;
 *  - the shared page is four art records of mode 2 at fixed cell offsets. */

#define A_BASE 0x26d0e0u             /* 30 palettes of 32 bytes a fighter, for the even palettes */
#define A_STRIDE 0x3c0u
#define B_BASE 0x272ae0u             /* 16 palettes a fighter, for the odd palettes below the split */
#define B_STRIDE 0x200u
#define RAMP_60 0x28b140u            /* palettes 60 and 62 */
#define RAMP_62 0x28b160u
#define PORTRAIT_BASE 0x29c7c0u      /* the 18 palettes of the portrait page */
#define PORTRAIT_BUTTON 0x720u
#define PORTRAIT_FIGHTER 0x60u
#define PORTRAIT_FIRST_ROW 160u
#define STEPS 0x5e6c0u               /* the portrait's art record pointer of fighter k at STEPS + STEP_STRIDE * k */
#define STEP_STRIDE 0x14u
#define ORIGIN_X 6                   /* cell columns and rows of the record's (x, y) = (-96, -80) */
#define ORIGIN_Y 5
#define SCROLL_ADD 0xa400u
#define TILE_BANK 0x10000u
#define BACKDROP_TILE 0x1a49eu
#define BACKDROP_X 12u
#define BACKDROP_Y 0u
#define BACKDROP_W 4u
#define BACKDROP_H 10u
#define VS_FIRST 0x235658u
#define VS_RECORDS 4u
#define SHADOW_COLUMN 8
#define SHADOW_ROW 14
#define COORD_TABLE 0x9f260u
#define MAX_CELLS 1024u              /* cells of one record; the largest of the arcade's is 107 */
#define MAX_SHARED 128u

struct cell {
    int32_t x, y;
    uint32_t code;
};

struct cells {
    struct cell v[MAX_CELLS];
    unsigned n;
};

struct placed {
    int32_t cx, cy;
    uint32_t code;
};

static void fail(char *err, size_t size, const char *fmt, ...)
{
    va_list ap;
    if (!err || !size) return;
    va_start(ap, fmt);
    vsnprintf(err, size, fmt, ap);
    va_end(ap);
}

static int in_view(size_t size, uint32_t a, uint32_t n)
{
    return (size_t)a <= size && (size_t)n <= size - (size_t)a;
}

static unsigned be16(const uint8_t *p)
{
    return (unsigned)p[0] << 8 | p[1];
}

static uint32_t be32(const uint8_t *p)
{
    return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3];
}

static void put16(uint8_t *p, unsigned v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

/* An arcade palette word as a PS1 color: each 4-bit channel doubled (not scaled to 31); black is 0x8000, never the transparent 0. */
static unsigned ps1_color(unsigned w)
{
    unsigned c = ((w >> 8) & 15u) << 1 | ((w >> 4) & 15u) << 6 | (w & 15u) << 11;
    return c ? c : 0x8000u;
}

/* ---------------------------------------------------------------- palettes */

/* Where the arcade keeps palette n of the file (-1: it names none). */
static int palette_address(const struct arcade_parts_file *f, unsigned n, uint32_t *a)
{
    unsigned p = n >> 1;
    if (n & 1) {
        if (p < f->split) {
            *a = B_BASE + B_STRIDE * f->fighter + 32u * p;
        } else {
            unsigned q = p - f->split;
            if (f->layout == ARCADE_PARTS_FLAT) {
                *a = f->base + 32u * q;
            } else {
                if (q >= 12) return -1;
                *a = f->base + 32u * (5u * (q / 2) + q % 2);
            }
        }
        return 0;
    }
    if (p < 30) {
        *a = A_BASE + A_STRIDE * f->fighter + 32u * p;
        return 0;
    }
    if (n == 60) {
        *a = RAMP_60;
        return 0;
    }
    if (n == 62) {
        *a = RAMP_62;
        return 0;
    }
    return -1;
}

/* The slot of palette n in the file's list of filler ramp starts (-1: none). */
static int ramp_index(unsigned n)
{
    if ((n & 1) && n >= 33) return (int)(n >> 1) - 16;
    if (n == 60) return 16;
    if (n == 62) return 17;
    return -1;
}

int arcade_parts_palettes(const uint8_t *program, size_t program_size, const struct arcade_parts_file *f,
                          uint8_t *out, uint64_t *kept, char *err, size_t errsize)
{
    unsigned n, k;
    if (!program || !f || !out) {
        fail(err, errsize, "fighter parts: bad argument");
        return -1;
    }
    memset(out, 0, ARCADE_PARTS_PALETTE_BYTES);
    if (kept) *kept = f->keep;
    for (n = 0; n < 64; n++) {
        uint8_t *dst = out + 32u * n;
        int ramp;
        uint16_t start = ARCADE_PARTS_NO_RAMP;
        if ((f->keep >> n) & 1u) continue;
        ramp = ramp_index(n);
        if (ramp >= 0 && f->ramps) start = f->ramps[ramp];
        if (start != ARCADE_PARTS_NO_RAMP) {
            for (k = 0; k < 15; k++) put16(dst + 2u * (k + 1), ps1_color(start + (k + 1) / 2));
        } else {
            uint32_t a;
            if (palette_address(f, n, &a) || !in_view(program_size, a, 32)) {
                fail(err, errsize, "%s: palette %u has no arcade address in the program", f->file, n);
                return -1;
            }
            for (k = 0; k < 15; k++) put16(dst + 2u * (k + 1), ps1_color(be16(program + a + 2u * k)));
        }
    }
    return 0;
}

/* ---------------------------------------------------------------- pages */

/* One block of an art record as cells: width and height in tiles from the attribute, no flips, on the 16 pixel grid. */
static int block_cells(struct cells *c, int32_t x, int32_t y, unsigned attr, unsigned code, unsigned add, char *err, size_t errsize)
{
    unsigned w = ((attr >> 8) & 15u) + 1u, h = ((attr >> 12) & 15u) + 1u, i, j;
    if ((attr & 0x60u) || (x & 15) || (y & 15)) {
        fail(err, errsize, "fighter parts: art block with a flip or off the cell grid");
        return -1;
    }
    if (c->n + w * h > MAX_CELLS) {
        fail(err, errsize, "fighter parts: art record has too many cells");
        return -1;
    }
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++) {
            struct cell *d = &c->v[c->n++];
            d->x = x + 16 * (int32_t)i;
            d->y = y + 16 * (int32_t)j;
            d->code = TILE_BANK + ((code + i + 16u * j + add) & 0xffffu);
        }
    return 0;
}

/* The art record of mode 4 at a (header: mode, cells, blocks - 1; blocks: code, attribute, x, y). */
static int record4(const uint8_t *prog, size_t size, uint32_t a, struct cells *c, char *err, size_t errsize)
{
    unsigned nb, k;
    if ((a & 1u) || !in_view(size, a, 6) || be16(prog + a) != 4) {
        fail(err, errsize, "fighter parts: no mode 4 art record at %#x", (unsigned)a);
        return -1;
    }
    nb = be16(prog + a + 4) + 1u;
    if (!in_view(size, a + 6, 8u * nb)) {
        fail(err, errsize, "fighter parts: art record at %#x runs past the program", (unsigned)a);
        return -1;
    }
    c->n = 0;
    for (k = 0; k < nb; k++) {
        const uint8_t *b = prog + a + 6 + 8u * k;
        if (block_cells(c, (int16_t)be16(b + 4), (int16_t)be16(b + 6), be16(b + 2), be16(b), SCROLL_ADD, err, errsize)) return -1;
    }
    return 0;
}

/* The art record of mode 2 at a (header: mode, cells, blocks - 1, coordinate index; blocks: code, attribute; coordinates
 * through the pointer table). *end is the address after the blocks. */
static int record2(const uint8_t *prog, size_t size, uint32_t a, struct cells *c, uint32_t *end, char *err, size_t errsize)
{
    unsigned nb, k;
    int32_t at;
    uint32_t ptr;
    if ((a & 1u) || !in_view(size, a, 8) || be16(prog + a) != 2) {
        fail(err, errsize, "fighter parts: no mode 2 art record at %#x", (unsigned)a);
        return -1;
    }
    nb = be16(prog + a + 4) + 1u;
    at = (int32_t)COORD_TABLE + (int16_t)be16(prog + a + 6);
    if (!in_view(size, a + 8, 4u * nb) || at < 0 || !in_view(size, (uint32_t)at, 4)) {
        fail(err, errsize, "fighter parts: art record at %#x runs past the program", (unsigned)a);
        return -1;
    }
    ptr = be32(prog + at);
    if ((ptr & 1u) || !in_view(size, ptr, 4u * nb)) {
        fail(err, errsize, "fighter parts: coordinates of the art record at %#x lie past the program", (unsigned)a);
        return -1;
    }
    c->n = 0;
    for (k = 0; k < nb; k++) {
        const uint8_t *b = prog + a + 8 + 4u * k, *xy = prog + ptr + 4u * k;
        if (block_cells(c, (int16_t)be16(xy), (int16_t)be16(xy + 2), be16(b + 2), be16(b), 0, err, errsize)) return -1;
    }
    *end = a + 8 + 4u * nb;
    return 0;
}

/* Cell (cx, cy) of the page: the pixels of arcade tile `code` (the index one higher, 15 becoming 0). */
static int put_cell(uint8_t *page, const uint8_t *tiles, size_t tiles_size, int32_t cx, int32_t cy, uint32_t code, char *err, size_t errsize)
{
    const uint8_t *t;
    unsigned y, x;
    if (cx < 0 || cx > 15 || cy < 0 || cy > 15) {
        fail(err, errsize, "fighter parts: cell (%d, %d) is outside the page", (int)cx, (int)cy);
        return -1;
    }
    if (tiles_size < 128u || (size_t)code > tiles_size / 128u - 1u) {
        fail(err, errsize, "fighter parts: tile %#x is outside the graphics", (unsigned)code);
        return -1;
    }
    t = tiles + (size_t)code * 128u;
    for (y = 0; y < 16; y++) {
        uint8_t *row = page + (size_t)(cy * 16 + (int32_t)y) * 128u + (size_t)cx * 8u;
        for (x = 0; x < 16; x += 2) {
            unsigned px[2], e, p;
            for (e = 0; e < 2; e++) {
                const uint8_t *b = t + y * 8u + ((x + e) >> 3) * 4u;
                unsigned bit = 7u - ((x + e) & 7u), v = 0;
                for (p = 0; p < 4; p++) v |= (unsigned)((b[p] >> bit) & 1u) << p;
                px[e] = (v + 1u) & 15u;
            }
            row[x / 2] = (uint8_t)(px[0] | px[1] << 4);
        }
    }
    return 0;
}

/* The shared page's cells (all four records placed) and, from the last record, the cells with y >= 0 in record order, which are
 * the portrait page's shadow cells. */
static int shared_cells(const uint8_t *prog, size_t size, struct placed *all, unsigned *nall, struct placed *shadow, unsigned *nshadow,
                        char *err, size_t errsize)
{
    static const int8_t place[VS_RECORDS][2] = {{12, 2}, {8, 6}, {4, 2}, {4, 6}};
    struct cells *c;
    uint32_t a = VS_FIRST;
    unsigned r, i;
    c = (struct cells *)malloc(sizeof *c);
    if (!c) {
        fail(err, errsize, "fighter parts: out of memory");
        return -1;
    }
    *nall = *nshadow = 0;
    for (r = 0; r < VS_RECORDS; r++) {
        if (record2(prog, size, a, c, &a, err, errsize)) goto bad;
        for (i = 0; i < c->n; i++) {
            const struct cell *e = &c->v[i];
            int32_t cx = e->x / 16 + place[r][0], cy = e->y / 16 + place[r][1];
            if (cx < 0 || cx > 15 || cy < 0 || cy > 15 || *nall >= MAX_SHARED) {
                fail(err, errsize, "fighter parts: shared page cell outside the page");
                goto bad;
            }
            all[*nall].cx = cx, all[*nall].cy = cy, all[*nall].code = e->code, ++*nall;
            if (r == VS_RECORDS - 1 && e->y >= 0) {
                int32_t sx = e->x / 16 + SHADOW_COLUMN, sy = e->y / 16 + SHADOW_ROW;
                if (sx < 0 || sx > 15 || sy < 0 || sy > 15 || *nshadow >= MAX_SHARED) {
                    fail(err, errsize, "fighter parts: shadow cell outside the page");
                    goto bad;
                }
                shadow[*nshadow].cx = sx, shadow[*nshadow].cy = sy, shadow[*nshadow].code = e->code, ++*nshadow;
            }
        }
    }
    free(c);
    return 0;
bad:
    free(c);
    return -1;
}

int arcade_parts_shared(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                        uint8_t *out, char *err, size_t errsize)
{
    struct placed all[MAX_SHARED], shadow[MAX_SHARED];
    unsigned nall, nshadow, i;
    if (!program || !tiles || !out) {
        fail(err, errsize, "fighter parts: bad argument");
        return -1;
    }
    memset(out, 0, ARCADE_PARTS_PAGE_BYTES);
    if (shared_cells(program, program_size, all, &nall, shadow, &nshadow, err, errsize)) return -1;
    for (i = 0; i < nall; i++)
        if (put_cell(out, tiles, tiles_size, all[i].cx, all[i].cy, all[i].code, err, errsize)) return -1;
    return 0;
}

int arcade_parts_portrait(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                          const struct arcade_parts_file *f, uint8_t *out, char *err, size_t errsize)
{
    struct placed all[MAX_SHARED], shadow[MAX_SHARED];
    struct cells *c;
    unsigned nall, nshadow, i, x, y;
    uint32_t step = STEPS + STEP_STRIDE * f->fighter, a;
    if (!program || !tiles || !f || !out) {
        fail(err, errsize, "fighter parts: bad argument");
        return -1;
    }
    if (f->flags & ARCADE_PARTS_PAGE_DISC) return 1;
    memset(out, 0, ARCADE_PARTS_PAGE_BYTES);
    if (!in_view(program_size, step, 4)) {
        fail(err, errsize, "%s: fighter %u has no portrait step", f->file, f->fighter);
        return -1;
    }
    a = be32(program + step);
    c = (struct cells *)malloc(sizeof *c);
    if (!c) {
        fail(err, errsize, "fighter parts: out of memory");
        return -1;
    }
    if (record4(program, program_size, a, c, err, errsize)) goto bad;
    for (i = 0; i < c->n; i++) {
        int32_t cx = c->v[i].x / 16 + ORIGIN_X, cy = c->v[i].y / 16 + ORIGIN_Y;
        if (cx == f->drop_x && cy == f->drop_y) continue;
        if (put_cell(out, tiles, tiles_size, cx, cy, c->v[i].code, err, errsize)) goto bad;
    }
    free(c);
    c = NULL;
    for (y = BACKDROP_Y; y < BACKDROP_Y + BACKDROP_H; y++)
        for (x = BACKDROP_X; x < BACKDROP_X + BACKDROP_W; x++)
            if (put_cell(out, tiles, tiles_size, (int32_t)x, (int32_t)y, BACKDROP_TILE, err, errsize)) return -1;
    if (f->shadow) {
        if (shared_cells(program, program_size, all, &nall, shadow, &nshadow, err, errsize)) return -1;
        for (i = 0; i < nshadow && i < 16; i++)
            if (((f->shadow >> i) & 1u) && put_cell(out, tiles, tiles_size, shadow[i].cx, shadow[i].cy, shadow[i].code, err, errsize)) return -1;
        if (nshadow < 16 && (f->shadow >> nshadow)) {
            fail(err, errsize, "%s: shadow cell %u does not exist", f->file, nshadow);
            return -1;
        }
    }
    for (i = 0; i < 18; i++) {
        uint32_t pa = PORTRAIT_BASE + PORTRAIT_BUTTON * (i / 3) + PORTRAIT_FIGHTER * f->fighter + 32u * (i % 3);
        uint8_t *row = out + (size_t)(PORTRAIT_FIRST_ROW + i) * 128u;
        unsigned k;
        if (!in_view(program_size, pa, 32)) {
            fail(err, errsize, "%s: portrait palette %u lies past the program", f->file, i);
            return -1;
        }
        put16(row, 0);
        for (k = 0; k < 15; k++) put16(row + 2u * (k + 1), ps1_color(be16(program + pa + 2u * k)) | 0x8000u);
    }
    return 0;
bad:
    free(c);
    return -1;
}
