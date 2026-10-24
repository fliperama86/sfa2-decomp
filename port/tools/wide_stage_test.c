/* Native fixtures of widescreen's side tiles (port/src/wide_stage.c, port/widescreen.md "The side tiles"). No game, disc or window.
 *
 * The game's own C of the stage "ryu" for the three forms (draw_tile_layer_0_16px_slot06_00, draw_tile_layer_1_16px_slot06_00,
 * draw_tilemap_layer_32px_9x13_slot06_00) is compiled as it is and linked with its globals at the game's addresses (the linker's
 * --defsym, from ps1/src/symbols.ld), over a block of memory mapped at those addresses. It runs on made-up layers and maps; the
 * packets it writes into its pool are compared with the maker's for the same columns. The side columns are compared with an
 * expectation computed here from a logical map, by other formulas. The overrides run end to end with a stand-in for
 * port_function_impl that returns the real C.
 * Needs a 32-bit program (the game's C holds 32-bit pointers); test_wide_stage.py builds it. Prints `ok   NAME` or `FAIL NAME: why`.
 * With argument `table` it prints the table of stage copies instead (name, form, layer, pool, count), for the check against ps1/src. */
#include "port.h"
#include "gpu.h"
#include "settings.h"
#include "wide_stage.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* ---- the game's memory and symbols at the game's addresses ---- */
#define REGION      0x80100000u
#define REGION_SIZE 0x00100000u
#define ADDR_GAME_STATE_SCROLL_Y 0x8019019au
#define ADDR_DRAW_BUFFER_INDEX   0x801a27d0u
#define ADDR_CURRENT_OT          0x801987c8u
#define ADDR_LAYER0              0x801aa544u
#define ADDR_OT                  0x80120000u
#define ADDR_MAP                 0x80130000u

#define POOL_L0  0x801ec390u   /* ryu: data_801ec390_slot06_00[2][0x72] */
#define POOL_L1  0x801edc80u   /* data_801edc80_slot06_00[2][270] */
#define POOL_T32 0x801f1790u   /* data_801f1790_slot06_00[2][98] */

void draw_tile_layer_0_16px_slot06_00(void);
void draw_tile_layer_1_16px_slot06_00(void);
void draw_tilemap_layer_32px_9x13_slot06_00(void);

struct port_settings port_settings;
struct port_settings port_settings_file;

/* The stand-in for library.c's lookup: the game's C for the three ryu functions, nothing for any other name (a build without that C). */
port_fn port_function_impl(const char *name)
{
    if (!strcmp(name, "draw_tile_layer_0_16px_slot06_00")) return draw_tile_layer_0_16px_slot06_00;
    if (!strcmp(name, "draw_tile_layer_1_16px_slot06_00")) return draw_tile_layer_1_16px_slot06_00;
    if (!strcmp(name, "draw_tilemap_layer_32px_9x13_slot06_00")) return draw_tilemap_layer_32px_9x13_slot06_00;
    return PORT_FN_NULL;
}

#define MAX_QUEUED 8192
static struct queued { int entry; unsigned len; uint32_t w[8]; } queued[MAX_QUEUED];
static unsigned queued_count;
static int queue_refuse_after = -1;   /* refuse every packet after this many (-1: never) */

int port_gpu_queue_packet(int entry, const uint32_t *words, unsigned len)
{
    if (entry < 0 || entry >= 30 || len > 8) return -1;
    if (queue_refuse_after >= 0 && queued_count >= (unsigned)queue_refuse_after) return -1;
    if (queued_count >= MAX_QUEUED) return -1;
    queued[queued_count].entry = entry;
    queued[queued_count].len = len;
    memcpy(queued[queued_count].w, words, len * sizeof words[0]);
    queued_count++;
    return 0;
}

/* ---- reporting ---- */
static int failures, cases;
static void report(const char *name, const char *why)
{
    cases++;
    if (why) { failures++; printf("FAIL %s: %s\n", name, why); }
    else printf("ok   %s\n", name);
}
static char why[256];
#define BAD(...) (snprintf(why, sizeof why, __VA_ARGS__), 0)

/* ---- the memory ---- */
static void wr16(uint32_t addr, unsigned v) { uint16_t x = (uint16_t)v; memcpy((void *)(uintptr_t)addr, &x, 2); }
static void wr32(uint32_t addr, uint32_t v) { memcpy((void *)(uintptr_t)addr, &v, 4); }
static uint32_t rd32a(uint32_t addr) { uint32_t v; memcpy(&v, (void *)(uintptr_t)addr, 4); return v; }

static int map_region(void)
{
    void *p = mmap((void *)(uintptr_t)REGION, REGION_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    return p == (void *)(uintptr_t)REGION;
}

/* ---- the three forms, as the test sees them ---- */
struct form {
    const char *tag;
    const struct port_wide_shape *shape;
    unsigned layer, pool, count;
    void (*game_c)(void);
    uint32_t width, height;      /* the made-up map */
    unsigned code;               /* the sprite's code byte that the init gives (0x7c Sprt16, 0x64 Sprt) */
};
static const struct form forms[3] = {
    { "L0",  &port_wide_shape_L0,  ADDR_LAYER0,        POOL_L0,  0x72, draw_tile_layer_0_16px_slot06_00,           0x400, 0x200, 0x7c },
    { "L1",  &port_wide_shape_L1,  ADDR_LAYER0 + 0x90, POOL_L1,  270,  draw_tile_layer_1_16px_slot06_00,           0x400, 0x200, 0x7c },
    { "T32", &port_wide_shape_T32, ADDR_LAYER0 + 0x120, POOL_T32, 98,  draw_tilemap_layer_32px_9x13_slot06_00,     0x400, 0x100, 0x64 },
};

static unsigned record_words(const struct form *f) { return (unsigned)f->shape->words + 1; }   /* the tag and the packet */

/* The pool as the stage's init leaves it (init_first_layer ... init_draw_mode_prim_3/4 and MargePrim: ps1/src/resident/graphics/call_handler_by_kind_index_a.c,
 * port/src/gpu.c): tag length 2 + 3 (or 4) + 1, drawing mode 0xe1000000, window 0, the sprite's tag, grey colour and its code. */
static void init_pool(const struct form *f, uint32_t colour_word)
{
    /* The made-up maps can draw more cells than the stage's real pool holds (the 16 px layer 0 with a blank that matches nothing draws
     * all 400), and the C then writes on past the pool: the records beyond it are set up the same way, so that the comparison is
     * about the arithmetic and not about what lies after a pool. */
    unsigned i, n = record_words(f), total = 2 * f->count + 512;
    for (i = 0; i < total; i++) {
        uint32_t a = f->pool + i * n * 4;
        unsigned j;
        for (j = 0; j < n; j++) wr32(a + j * 4, 0);
        wr32(a, (uint32_t)f->shape->words << 24);
        wr32(a + 4, 0xe1000000u);
        wr32(a + 12, f->shape->words == 6 ? 0x03000000u : 0x04000000u);
        wr32(a + 16, colour_word);
        if (f->shape->words == 7) wr32(a + 28, 0x00200020u);
    }
}

/* ---- the logical map: tile(col, row) and clut(col, row), written into the game's block layout by other formulas than the C's ---- */
static uint32_t mix(uint32_t a, uint32_t b) { uint32_t h = a * 2654435761u ^ (b + 0x9e3779b9u) * 40503u; h ^= h >> 15; h *= 2246822519u; h ^= h >> 13; return h; }
static unsigned cells_across, cells_down, blank_value, density;   /* one cell in `density` is not the blank cell: the stage's pool holds fewer records than it has cells */

static unsigned logical_tile(unsigned col, unsigned row)
{
    uint32_t h = mix(col, row);
    if (h % density != 0) return blank_value;
    return (h >> 8) & 0xffffu;
}
static unsigned logical_clut(unsigned col, unsigned row) { return mix(row + 77, col * 3 + 1) & 0xffffu; }

static void build_map(const struct form *f)
{
    unsigned bc = f->shape->shift == 4 ? 32 : 16, br = f->shape->shift == 4 ? 16 : 8, col, row;
    unsigned blocks_across;
    density = f->shape->shift == 4 ? (f->shape == &port_wide_shape_L0 ? 5 : 2) : 2;   /* L0 pool: 114 records for 400 cells; L1 270; T32 98 for 117 */
    cells_across = f->width >> f->shape->shift;
    cells_down = f->height >> f->shape->shift;
    blocks_across = cells_across / bc;
    memset((void *)(uintptr_t)ADDR_MAP, 0, 0x10000);     /* the padding after the map is zero */
    for (row = 0; row < cells_down; row++)
        for (col = 0; col < cells_across; col++) {
            unsigned block = (row / br) * blocks_across + col / bc;
            unsigned index = block * bc * br + (row % br) * bc + col % bc;
            wr16(ADDR_MAP + index * 4, logical_tile(col, row));
            wr16(ADDR_MAP + index * 4 + 2, logical_clut(col, row));
        }
}

struct fixture { unsigned scroll_x; int32_t scroll_y; unsigned game_scroll_y; unsigned blank; unsigned ot_index; unsigned buffer; };

static void set_layer(const struct form *f, const struct fixture *x)
{
    uint32_t a = f->layer;
    memset((void *)(uintptr_t)a, 0, 0x90);
    wr16(a + 0x12, x->scroll_x);
    wr32(a + 0x14, (uint32_t)x->scroll_y);
    wr16(a + 0x1e, x->blank);
    wr32(a + 0x50, ADDR_MAP);
    wr32(a + 0x58, f->width);
    wr32(a + 0x5c, f->height);
    ((uint8_t *)(uintptr_t)a)[0x8a] = (uint8_t)x->ot_index;
    wr16(ADDR_GAME_STATE_SCROLL_Y, x->game_scroll_y);
    wr32(ADDR_DRAW_BUFFER_INDEX, x->buffer);
    wr32(ADDR_CURRENT_OT, ADDR_OT);
    memset((void *)(uintptr_t)ADDR_OT, 0, 30 * 4);
    blank_value = x->blank;
    build_map(f);
}

/* ---- the packets, as arrays ---- */
struct packet { int entry; uint32_t w[8]; };
#define MAX_PACKETS 4096
struct packets { struct packet p[MAX_PACKETS]; unsigned n; };
static struct packets from_c, from_maker, expected;

static struct packets *sink;
static int sink_emit(int entry, const uint32_t *words, unsigned len)
{
    if (sink->n >= MAX_PACKETS) return -1;
    sink->p[sink->n].entry = entry;
    memset(sink->p[sink->n].w, 0, sizeof sink->p[sink->n].w);
    memcpy(sink->p[sink->n].w, words, len * sizeof words[0]);
    sink->n++;
    return 0;
}

/* The records that the game's C linked into entry `entry`, in the order it drew them (it links each at the head, so the chain is reversed). */
static void harvest(const struct form *f, unsigned entry, struct packets *out)
{
    uint32_t a = rd32a(ADDR_OT + entry * 4) & 0xffffffu;
    unsigned i, n = 0, len = (unsigned)f->shape->words;
    uint32_t chain[MAX_PACKETS];
    while (a && n < MAX_PACKETS) {
        chain[n++] = 0x80000000u | a;
        a = rd32a(0x80000000u | a) & 0xffffffu;
    }
    out->n = 0;
    for (i = n; i-- > 0;) {
        unsigned j;
        memset(&out->p[out->n], 0, sizeof out->p[out->n]);
        out->p[out->n].entry = (int)entry;
        for (j = 0; j < len; j++) out->p[out->n].w[j] = rd32a(chain[i] + 4 + 4 * j);
        out->n++;
    }
}

static const uint32_t *first_record(const struct form *f, unsigned buffer)
{
    return (const uint32_t *)(uintptr_t)(f->pool + buffer * f->count * record_words(f) * 4);
}

static const uint8_t *layer_of(const struct form *f) { return (const uint8_t *)(uintptr_t)f->layer; }
static const uint8_t *map_of(void) { return (const uint8_t *)(uintptr_t)ADDR_MAP; }
#define MAP_BYTES 0x10000u

static unsigned run_maker(const struct form *f, const struct fixture *x, int k0, int k1, struct packets *out)
{
    unsigned n;
    sink = out;
    out->n = 0;
    n = port_wide_columns(f->shape, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y, first_record(f, x->buffer), k0, k1, sink_emit);
    return n;
}

/* 0 when the two lists are the same packets in the same order; else the index of the first difference + 1 */
static unsigned compare(const struct packets *a, const struct packets *b, unsigned words)
{
    unsigned i, n = a->n < b->n ? a->n : b->n;
    for (i = 0; i < n; i++)
        if (a->p[i].entry != b->p[i].entry || memcmp(a->p[i].w, b->p[i].w, words * sizeof(uint32_t))) return i + 1;
    return a->n == b->n ? 0 : n + 1;
}

/* ---- fixtures ---- */
static uint32_t lcg_state = 12345;
static uint32_t lcg(void) { lcg_state = lcg_state * 1664525u + 1013904223u; return lcg_state >> 8; }

#define NFIX 64
static struct fixture fixtures[NFIX];
static unsigned fixture_count;

static void make_fixtures(void)
{
    unsigned i;
    /* by hand: the edges of the arithmetic. scroll_y 0x100000 and game scroll_y 0 put the layer's row 0 at the top. */
    static const struct fixture edge[] = {
        { 0x0000, 0x00100000, 0,    0x0000, 3, 0 },   /* origin, blank 0 */
        { 0x0003, 0x00100000, 0,    0x0001, 5, 1 },   /* a few pixels in */
        { 0x03f5, 0x00100000, 0,    0x0002, 7, 0 },   /* the map wraps to the left inside the 25 columns */
        { 0xfff5, 0x000f8000, 0x10, 0x0003, 9, 1 },   /* negative scroll_x as a s16; y above the map */
        { 0x01ff, 0x00080000, 0x00, 0x8123, 11, 0 },  /* a blank with the high bit: the 16 px layer 0 compares the u16 cell with the s16 blank */
        { 0x0080, 0x00300000, 0x08, 0x0004, 13, 1 },  /* rows near the bottom of the map */
        { 0x0100, 0x000c0000, 0x40, 0x0005, 15, 0 },  /* rows partly above, partly in the map (y negative) */
        { 0x0213, 0x7fff0000, 0x00, 0x0006, 17, 1 },  /* a big scroll_y */
        { 0x000f, 0xfff00000u, 0xff, 0xffff, 19, 0 }, /* negative scroll_y; blank 0xffff */
        { 0x03ff, 0x00104000, 0x01, 0x0007, 21, 1 },
    };
    fixture_count = 0;
    for (i = 0; i < sizeof edge / sizeof edge[0]; i++) fixtures[fixture_count++] = edge[i];
    while (fixture_count < NFIX) {
        struct fixture x;
        x.scroll_x = lcg() & 0xffffu;
        x.scroll_y = (int32_t)(0x100000 - (int32_t)(lcg() % 0x300) * 0x10000 + (int32_t)(lcg() & 0xffff));
        x.game_scroll_y = lcg() % 0x30;
        x.blank = (lcg() % 3) ? 1 + lcg() % 9 : (lcg() & 0xffffu);
        x.ot_index = lcg() % 30;
        x.buffer = lcg() & 1;
        fixtures[fixture_count++] = x;
    }
}

/* ---- case: the maker reproduces the game's C ---- */
static int case_matches_c(const struct form *f, unsigned *packets)
{
    unsigned i, total = 0;
    for (i = 0; i < fixture_count; i++) {
        const struct fixture *x = &fixtures[i];
        unsigned d;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        f->game_c();
        harvest(f, x->ot_index, &from_c);
        run_maker(f, x, 0, f->shape->cols, &from_maker);
        d = compare(&from_c, &from_maker, (unsigned)f->shape->words);
        if (d) return BAD("fixture %u: packet %u differs (C made %u, the maker %u)", i, d - 1, from_c.n, from_maker.n);
        total += from_c.n;
    }
    *packets = total;
    return total != 0 ? 1 : BAD("the C drew nothing in any fixture");
}

/* ---- case: the side columns by an independent expectation ---- */
static int floordiv(int a, int b) { int q = a / b; return (a % b != 0 && ((a < 0) != (b < 0))) ? q - 1 : q; }
static int floormod(int a, int b) { return a - floordiv(a, b) * b; }

static void expect_columns(const struct form *f, const struct fixture *x, int k0, int k1, struct packets *out)
{
    const int size = 1 << f->shape->shift, rows_c = f->shape->rows;
    const int b = (int16_t)x->blank;
    int64_t s = (int64_t)(int32_t)x->scroll_y;
    int64_t above = ((int64_t)0x100000 - s);                           /* -(scroll_y - 0x100000) */
    int y = (int16_t)(uint16_t)(floordiv((int)(int32_t)above, 65536) - (int)x->game_scroll_y);
    int sx = x->scroll_x & (size - 1), sy = floormod(y, size), row0 = floordiv(y, size);
    int col0 = floordiv((int)(int16_t)x->scroll_x, size);
    int i, k;
    const uint32_t *rec = first_record(f, x->buffer);
    out->n = 0;
    for (i = 0; i < rows_c; i++) {
        int row = row0 + i;
        if (row < 0 || row > (int)cells_down || (row == (int)cells_down && !f->shape->row_inclusive)) continue;   /* only the 32 px C also takes the row after the last */
        for (k = k0; k < k1; k++) {
            unsigned col = (unsigned)floormod(col0 + k, (int)cells_across);
            unsigned tile = row < (int)cells_down ? logical_tile(col, (unsigned)row) : 0;   /* past the map: the zero padding */
            unsigned clut = row < (int)cells_down ? logical_clut(col, (unsigned)row) : 0;
            int tile_s = (int16_t)tile, skip;
            struct packet *p;
            if (f->shape == &port_wide_shape_L0) skip = (b >= 0 && (int)tile == b);   /* u16 against s16 as ints: a blank with the high bit matches nothing */
            else skip = (tile_s == b);
            if (skip) continue;
            p = &out->p[out->n++];
            memset(p, 0, sizeof *p);
            p->entry = (int)x->ot_index;
            memcpy(p->w, rec + 1, (unsigned)f->shape->words * 4);
            p->w[0] = 0xe1000000u + (uint32_t)floordiv(tile_s, 1024);
            p->w[4] = (uint32_t)(uint16_t)(k * size - sx) | ((uint32_t)(uint16_t)(i * size - sy) << 16);
            p->w[5] = (uint32_t)((tile & 31) * 8) | (uint32_t)(((tile >> 5) & 31) * 8) << 8 | (uint32_t)clut << 16;
        }
    }
}

static int case_sides(const struct form *f, unsigned *packets)
{
    unsigned i, total = 0;
    for (i = 0; i < fixture_count; i++) {
        const struct fixture *x = &fixtures[i];
        int kl, kr, side;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        f->game_c();
        port_wide_side_range(f->shape, layer_of(f), &kl, &kr);
        if (kl >= 0 || kr <= f->shape->cols) return BAD("fixture %u: side range %d..%d", i, kl, kr);
        for (side = 0; side < 2; side++) {
            int k0 = side ? f->shape->cols : kl, k1 = side ? kr : 0;
            unsigned d;
            run_maker(f, x, k0, k1, &from_maker);
            expect_columns(f, x, k0, k1, &expected);
            d = compare(&from_maker, &expected, (unsigned)f->shape->words);
            if (d) return BAD("fixture %u, k %d..%d: packet %u differs (maker %u, expected %u)", i, k0, k1 - 1, d - 1, from_maker.n, expected.n);
            total += from_maker.n;
        }
    }
    *packets = total;
    return total != 0 ? 1 : BAD("no side tiles in any fixture");
}

/* ---- case: the expectation also reproduces the game's C in its own range (so the independent formulas are the C's) ---- */
static int case_expect_in_c_range(const struct form *f)
{
    unsigned i;
    for (i = 0; i < fixture_count; i++) {
        const struct fixture *x = &fixtures[i];
        unsigned d;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        f->game_c();
        harvest(f, x->ot_index, &from_c);
        expect_columns(f, x, 0, f->shape->cols, &expected);
        d = compare(&from_c, &expected, (unsigned)f->shape->words);
        if (d) return BAD("fixture %u: packet %u differs (C %u, expected %u)", i, d - 1, from_c.n, expected.n);
    }
    return 1;
}

/* ---- case: the visible range ---- */
static int case_range(void)
{
    unsigned fi;
    int sx;
    for (fi = 0; fi < 3; fi++) {
        const struct form *f = &forms[fi];
        int size = 1 << f->shape->shift;
        for (sx = 0; sx < size; sx++) {
            struct fixture x = { (unsigned)sx + 0x40u, 0x00100000, 0, 1, 3, 0 };
            int kl, kr;
            set_layer(f, &x);
            port_wide_side_range(f->shape, layer_of(f), &kl, &kr);
            /* the first column kept reaches into the view, the one beyond does not */
            if (!((kl) * size - sx + size > -128)) return BAD("%s sx %d: left column %d is out of the view", f->tag, sx, kl);
            if (((kl - 1) * size - sx + size > -128)) return BAD("%s sx %d: left column %d is visible but dropped", f->tag, sx, kl - 1);
            if (!((kr - 1) * size - sx < 512)) return BAD("%s sx %d: right column %d is out of the view", f->tag, sx, kr - 1);
            if ((kr * size - sx < 512)) return BAD("%s sx %d: right column %d is visible but dropped", f->tag, sx, kr);
        }
    }
    return 1;
}

/* ---- case: the static words come from the pool's first record ---- */
static int case_static_words(void)
{
    unsigned fi;
    for (fi = 0; fi < 3; fi++) {
        const struct form *f = &forms[fi];
        struct fixture x = { 0x0123, 0x00100000, 0, 1, 12, 1 };
        uint32_t a = f->pool + 1 * f->count * record_words(f) * 4;   /* pool[1][0] */
        unsigned i;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, &x);
        wr32(a + 8, 0xe2000abcu);                                     /* the texture window word */
        wr32(a + 16, (f->code << 24) | 0x00112233u);                  /* colour and code */
        if (f->shape->words == 7) wr32(a + 28, 0x00400040u);          /* the wide record's size */
        run_maker(f, &x, -3, 0, &from_maker);
        if (!from_maker.n) return BAD("%s: nothing made", f->tag);
        for (i = 0; i < from_maker.n; i++) {
            const uint32_t *w = from_maker.p[i].w;
            if (w[1] != 0xe2000abcu || w[2] != (f->shape->words == 6 ? 0x03000000u : 0x04000000u) || w[3] != ((f->code << 24) | 0x00112233u))
                return BAD("%s: packet %u does not carry the first record's words (%08x %08x %08x)", f->tag, i, w[1], w[2], w[3]);
            if (f->shape->words == 7 && w[6] != 0x00400040u) return BAD("%s: packet %u lost the sprite's size", f->tag, i);
            if (from_maker.p[i].entry != 12) return BAD("%s: packet %u for entry %d", f->tag, i, from_maker.p[i].entry);
        }
        /* the buffer 0 record is not the template when the draw buffer is 1 */
        x.buffer = 0;
        run_maker(f, &x, -3, 0, &from_maker);
        if (from_maker.n && from_maker.p[0].w[1] == 0xe2000abcu) return BAD("%s: took the other buffer's record", f->tag);
        /* a pool the init has not filled (tag length 0) gives nothing */
        wr32(f->pool, 0);
        x.buffer = 0;
        if (run_maker(f, &x, -3, 0, &from_maker) != 0) return BAD("%s: made packets from an empty pool", f->tag);
    }
    return 1;
}

/* ---- case: a cell past the map's bytes is blank; a refused packet stops the maker ---- */
static int case_limits(void)
{
    const struct form *f = &forms[1];
    struct fixture x = { 0x0100, 0x00100000, 0, 1, 4, 0 };
    unsigned all, some;
    init_pool(f, 0x00808080u | (f->code << 24));
    set_layer(f, &x);
    all = run_maker(f, &x, 25, 33, &from_maker);
    some = port_wide_columns(f->shape, layer_of(f), map_of(), 0x100, x.game_scroll_y, first_record(f, 0), 25, 33, sink_emit);
    sink = &from_maker;
    if (!all || some >= all) return BAD("a short map gave %u packets against %u", some, all);
    if (port_wide_columns(f->shape, layer_of(f), NULL, 0, 0, first_record(f, 0), 25, 33, sink_emit) != 0) return BAD("no map gave packets");
    return 1;
}

static int refuse_third(int entry, const uint32_t *words, unsigned len)
{
    static unsigned n;
    (void)entry; (void)words; (void)len;
    return ++n >= 3 ? -1 : 0;
}

static int case_emit_refused(void)
{
    const struct form *f = &forms[1];
    struct fixture x = { 0x0100, 0x00100000, 0, 1, 4, 0 };
    unsigned made;
    init_pool(f, 0x00808080u | (f->code << 24));
    set_layer(f, &x);
    made = port_wide_columns(f->shape, layer_of(f), map_of(), MAP_BYTES, 0, first_record(f, 0), 25, 33, refuse_third);
    return made == 2 ? 1 : BAD("expected 2 packets before the refusal, got %u", made);
}

/* ---- negative controls: the comparison must fail for a wrong column and for a wrong word ---- */
static int case_negative_control(const struct form *f)
{
    struct fixture x = { 0x0123, 0x00100000, 0x03, 5, 4, 0 };
    unsigned d, k;
    init_pool(f, 0x00808080u | (f->code << 24));
    set_layer(f, &x);
    f->game_c();
    harvest(f, x.ot_index, &from_c);
    if (from_c.n < 20) return BAD("%s: too little drawn to test (%u)", f->tag, from_c.n);
    /* a column off by one */
    run_maker(f, &x, 1, f->shape->cols + 1, &from_maker);
    if (!compare(&from_c, &from_maker, (unsigned)f->shape->words)) return BAD("%s: the shifted columns were accepted", f->tag);
    /* the C's own columns but one packet with a wrong word, each word that varies */
    for (k = 0; k < 3; k++) {
        static const unsigned word[3] = { 0, 4, 5 };
        run_maker(f, &x, 0, f->shape->cols, &from_maker);
        from_maker.p[from_maker.n / 2].w[word[k]] ^= 0x10u;
        d = compare(&from_c, &from_maker, (unsigned)f->shape->words);
        if (d != from_maker.n / 2 + 1) return BAD("%s: a changed word %u was not found at its packet (%u)", f->tag, word[k], d);
    }
    /* the other layers' record geometry: a wrong shape (the 32 px maker on the 16 px layer) differs */
    if (f->shape != &port_wide_shape_T32) {
        struct port_wide_shape wrong = *f->shape;
        wrong.shift = 5;
        sink = &from_maker;
        from_maker.n = 0;
        port_wide_columns(&wrong, layer_of(f), map_of(), MAP_BYTES, x.game_scroll_y, first_record(f, 0), 0, f->shape->cols, sink_emit);
        if (!compare(&from_c, &from_maker, (unsigned)f->shape->words)) return BAD("%s: a wrong shape was accepted", f->tag);
    }
    return 1;
}

/* the blank with the high bit: layer 0 draws every cell, the other two skip the blank cells */
static int case_blank_forms(void)
{
    struct fixture x = { 0x0000, 0x00100000, 0, 0x8123, 4, 0 };
    unsigned counts[3], fi;
    for (fi = 0; fi < 3; fi++) {
        const struct form *f = &forms[fi];
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, &x);
        f->game_c();
        harvest(f, 4, &from_c);
        counts[fi] = from_c.n;
    }
    if (counts[0] != 16u * 25u) return BAD("layer 0 drew %u cells, expected all %u", counts[0], 16u * 25u);
    if (counts[1] >= 16u * 25u || counts[1] < 16u * 25u / 2) return BAD("layer 1 drew %u of %u cells", counts[1], 16u * 25u);
    return 1;
}

/* ---- the overrides end to end ---- */
static void (*override_of(const char *name))(void)
{
    const struct port_override *o;
    for (o = port_wide_overrides; o->name; o++)
        if (!strcmp(o->name, name)) return (void (*)(void))o->host;
    return NULL;
}

static int case_override_wiring(void)
{
    /* widescreen, arcade_video, whether packets are expected: the side tiles need both settings */
    static const int settings[4][3] = { { 0, 0, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 1, 1, 1 } };
    unsigned fi, s;
    for (fi = 0; fi < 3; fi++) {
        const struct form *f = &forms[fi];
        const char *name = fi == 0 ? "draw_tile_layer_0_16px_slot06_00" : fi == 1 ? "draw_tile_layer_1_16px_slot06_00" : "draw_tilemap_layer_32px_9x13_slot06_00";
        void (*run)(void) = override_of(name);
        struct fixture x = { 0x0153, 0x00108000, 0x02, 3, 6, 1 };
        if (!run) return BAD("%s: no override", name);
        for (s = 0; s < 4; s++) {
            int kl, kr;
            unsigned n;
            port_settings.widescreen = settings[s][0];
            port_settings.arcade_video = settings[s][1];
            init_pool(f, 0x00808080u | (f->code << 24));
            set_layer(f, &x);
            queued_count = 0;
            run();
            harvest(f, x.ot_index, &from_c);
            if (from_c.n == 0) return BAD("%s: the game's C was not run (settings %u)", name, s);
            if (!settings[s][2]) {
                if (queued_count) return BAD("%s: %u packets queued with widescreen %d, arcade_video %d", name, queued_count, settings[s][0], settings[s][1]);
                continue;
            }
            port_wide_side_range(f->shape, layer_of(f), &kl, &kr);
            run_maker(f, &x, kl, 0, &from_maker);
            n = from_maker.n;
            run_maker(f, &x, f->shape->cols, kr, &from_maker);
            if (n + from_maker.n == 0) return BAD("%s: the fixture has no side tiles", name);
            if (queued_count != n + from_maker.n) return BAD("%s: %u packets queued, expected %u", name, queued_count, n + from_maker.n);
            if (queued[0].entry != 6 || queued[0].len != (unsigned)f->shape->words) return BAD("%s: first packet for entry %d, %u words", name, queued[0].entry, queued[0].len);
        }
    }
    return 1;
}

static int case_override_without_c(void)
{
    /* a copy of the table whose function has no C here (the stand-in gives only the ryu ones) runs nothing and queues nothing */
    void (*run)(void) = override_of("draw_tile_layer_0_16px_slot06_04");
    if (!run) return BAD("no override for a copy of another stage");
    port_settings.widescreen = port_settings.arcade_video = 1;
    queued_count = 0;
    run();
    run();
    return queued_count == 0 ? 1 : BAD("%u packets queued without the game's C", queued_count);
}

static void print_table(void)
{
    unsigned i;
    for (i = 0; i < port_wide_copy_count; i++) {
        const struct port_wide_copy *c = &port_wide_copies[i];
        const char *form = c->shape == &port_wide_shape_L0 ? "L0" : c->shape == &port_wide_shape_L1 ? "L1" : "T32";
        printf("%s %s 0x%08x 0x%08x %u\n", c->name, form, c->layer, c->pool, c->count);
    }
}

int main(int argc, char **argv)
{
    unsigned fi, packets;
    char name[96];
    if (argc > 1 && !strcmp(argv[1], "table")) { print_table(); return 0; }
    if (!map_region()) { printf("FAIL setup: could not map the game's address range\n"); return 2; }
    make_fixtures();
    for (fi = 0; fi < 3; fi++) {
        snprintf(name, sizeof name, "maker_reproduces_c_%s", forms[fi].tag);
        packets = 0;
        report(name, case_matches_c(&forms[fi], &packets) ? NULL : why);
        if (!failures) printf("     (%u fixtures, %u packets of the game's C compared)\n", fixture_count, packets);
    }
    for (fi = 0; fi < 3; fi++) {
        snprintf(name, sizeof name, "expectation_reproduces_c_%s", forms[fi].tag);
        report(name, case_expect_in_c_range(&forms[fi]) ? NULL : why);
    }
    for (fi = 0; fi < 3; fi++) {
        snprintf(name, sizeof name, "side_columns_%s", forms[fi].tag);
        packets = 0;
        report(name, case_sides(&forms[fi], &packets) ? NULL : why);
        if (!failures) printf("     (%u side packets compared)\n", packets);
    }
    report("side_range_is_the_visible_columns", case_range() ? NULL : why);
    report("static_words_from_first_record", case_static_words() ? NULL : why);
    report("blank_compare_per_form", case_blank_forms() ? NULL : why);
    report("map_limits", case_limits() ? NULL : why);
    report("emit_refused_stops_the_maker", case_emit_refused() ? NULL : why);
    for (fi = 0; fi < 3; fi++) {
        snprintf(name, sizeof name, "negative_control_%s", forms[fi].tag);
        report(name, case_negative_control(&forms[fi]) ? NULL : why);
    }
    report("override_wiring", case_override_wiring() ? NULL : why);
    report("override_without_c", case_override_without_c() ? NULL : why);
    printf("%d cases, %d failed\n", cases, failures);
    return failures ? 1 : 0;
}
