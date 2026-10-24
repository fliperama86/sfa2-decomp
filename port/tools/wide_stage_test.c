/* Native fixtures of widescreen's side tiles (port/src/wide_stage.c, port/widescreen.md "The side tiles"). No game, disc or window.
 *
 * The game's own C of the stage "ryu" for the three plain forms (draw_tile_layer_0_16px_slot06_00, draw_tile_layer_1_16px_slot06_00,
 * draw_tilemap_layer_32px_9x13_slot06_00) and of the stage "chunli" for the sprite and skewed forms
 * (draw_tile_layer_sprites_with_partial_row_slot06_04, draw_skewed_tile_layer_quads_slot06_04) is compiled as it is and linked with its globals at the game's addresses (the linker's
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
#define REGION_SIZE 0x00180000u
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
void draw_tile_layer_sprites_with_partial_row_slot06_04(void);
void draw_skewed_tile_layer_quads_slot06_04(void);
void draw_tilemap_layer_32px_7x13_slot06_01(void);
void draw_tile_map_layer_16px_half_scroll_slot06_0d(void);
void draw_tile_map_layer_32px_slot06_0d(void);
void draw_tile_map_layer_32px_slot06_02(void);
void draw_scrolling_tile_map_layer_slot06_08(void);
void draw_scrolling_tile_map_layer_slot06_12(void);
void draw_tile_map_layer_16px_slot06_02(void);
void draw_tile_map_layer_16px_slot03_slot06_03(void);
void draw_tile_map_layer_16px_slot06_09(void);
void draw_tilemap_layer_16px_12x25_slot06_01(void);
void draw_tile_layer_sprites_saving_pos_slot06_03(void);
void draw_tile_map_layer_polys_slot06_03(void);
void draw_perspective_skewed_tile_map_slot06_0e(void);
void draw_tile_map_16px_tiles_slot06_0e(void);
void draw_tile_map_layer_sprites_32px_slot06_0e(void);
void draw_tile_map_layer_sprites_interpolated_slot06_06(void);

#define POOL_SPR  0x801f03c8u   /* chunli: data_801f03c8_slot06_04[2][0x145] */
#define POOL_SKEW 0x801f4ae0u   /* data_801f4ae0_slot06_04[2][0x58] */
#define POOL_T32_7 0x801f94bcu  /* ken: data_801f94bc_slot06_01[2][0x43] */
#define POOL_T16H 0x801ed5e4u   /* sakura: data_801ed5e4_slot06_0d[2][0x74] */
#define POOL_T32H 0x801f51dcu   /* sakura: data_801f51dc_slot06_0d[2][0x5a] */
#define POOL_T32G 0x801f8638u   /* gouki: data_801f8638_slot06_02[2][13] */

struct port_settings port_settings;
struct port_settings port_settings_file;

/* The stand-in for library.c's lookup: the game's C for the three ryu functions, nothing for any other name (a build without that C). */
port_fn port_function_impl(const char *name)
{
    if (!strcmp(name, "draw_tile_layer_0_16px_slot06_00")) return draw_tile_layer_0_16px_slot06_00;
    if (!strcmp(name, "draw_tile_layer_1_16px_slot06_00")) return draw_tile_layer_1_16px_slot06_00;
    if (!strcmp(name, "draw_tilemap_layer_32px_9x13_slot06_00")) return draw_tilemap_layer_32px_9x13_slot06_00;
    if (!strcmp(name, "draw_tile_layer_sprites_with_partial_row_slot06_04")) return draw_tile_layer_sprites_with_partial_row_slot06_04;
    if (!strcmp(name, "draw_skewed_tile_layer_quads_slot06_04")) return draw_skewed_tile_layer_quads_slot06_04;
    if (!strcmp(name, "draw_tilemap_layer_32px_7x13_slot06_01")) return draw_tilemap_layer_32px_7x13_slot06_01;
    if (!strcmp(name, "draw_tile_map_layer_16px_half_scroll_slot06_0d")) return draw_tile_map_layer_16px_half_scroll_slot06_0d;
    if (!strcmp(name, "draw_tile_map_layer_32px_slot06_0d")) return draw_tile_map_layer_32px_slot06_0d;
    if (!strcmp(name, "draw_tile_map_layer_32px_slot06_02")) return draw_tile_map_layer_32px_slot06_02;
    if (!strcmp(name, "draw_scrolling_tile_map_layer_slot06_08")) return draw_scrolling_tile_map_layer_slot06_08;
    if (!strcmp(name, "draw_scrolling_tile_map_layer_slot06_12")) return draw_scrolling_tile_map_layer_slot06_12;
    if (!strcmp(name, "draw_tile_map_layer_16px_slot06_02")) return draw_tile_map_layer_16px_slot06_02;
    if (!strcmp(name, "draw_tile_map_layer_16px_slot03_slot06_03")) return draw_tile_map_layer_16px_slot03_slot06_03;
    if (!strcmp(name, "draw_tile_map_layer_16px_slot06_09")) return draw_tile_map_layer_16px_slot06_09;
    if (!strcmp(name, "draw_tilemap_layer_16px_12x25_slot06_01")) return draw_tilemap_layer_16px_12x25_slot06_01;
    if (!strcmp(name, "draw_tile_layer_sprites_saving_pos_slot06_03")) return draw_tile_layer_sprites_saving_pos_slot06_03;
    if (!strcmp(name, "draw_tile_map_layer_polys_slot06_03")) return draw_tile_map_layer_polys_slot06_03;
    if (!strcmp(name, "draw_perspective_skewed_tile_map_slot06_0e")) return draw_perspective_skewed_tile_map_slot06_0e;
    if (!strcmp(name, "draw_tile_map_16px_tiles_slot06_0e")) return draw_tile_map_16px_tiles_slot06_0e;
    if (!strcmp(name, "draw_tile_map_layer_sprites_32px_slot06_0e")) return draw_tile_map_layer_sprites_32px_slot06_0e;
    if (!strcmp(name, "draw_tile_map_layer_sprites_interpolated_slot06_06")) return draw_tile_map_layer_sprites_interpolated_slot06_06;
    return PORT_FN_NULL;
}

#define MAX_QUEUED 8192
static struct queued { int entry; unsigned len; uint32_t w[12]; } queued[MAX_QUEUED];
static unsigned queued_count;
static int queue_refuse_after = -1;   /* refuse every packet after this many (-1: never) */

static struct view_call { uint32_t layer, lo, hi; int entry, art_lo, art_hi; } view_calls[8];
static unsigned view_count;

void port_gpu_layer_view(uint32_t layer, uint32_t pool_lo, uint32_t pool_hi, int entry, int art_lo, int art_hi)
{
    if (view_count < 8) {
        view_calls[view_count].layer = layer;
        view_calls[view_count].lo = pool_lo;
        view_calls[view_count].hi = pool_hi;
        view_calls[view_count].entry = entry;
        view_calls[view_count].art_lo = art_lo;
        view_calls[view_count].art_hi = art_hi;
    }
    view_count++;
}

int port_gpu_queue_packet(int entry, const uint32_t *words, unsigned len)
{
    if (entry < 0 || entry >= 30 || len > 12) return -1;
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
    unsigned code;               /* the sprite's code byte that the init gives (0x7c Sprt16, 0x64 Sprt, 0x2c POLY_FT4) */
    unsigned density;            /* one cell in `density` is not blank, when the default for the form does not fit the pool */
    unsigned cap;                /* the C returns after this many records (gouki's and nash's grids), else 0 */
};
#define NFORMS 21
static const struct form forms[NFORMS] = {
    { "L0",  &port_wide_shape_L0,  ADDR_LAYER0,        POOL_L0,  0x72, draw_tile_layer_0_16px_slot06_00,           0x400, 0x200, 0x7c, 0, 0 },
    { "L1",  &port_wide_shape_L1,  ADDR_LAYER0 + 0x90, POOL_L1,  270,  draw_tile_layer_1_16px_slot06_00,           0x400, 0x200, 0x7c, 0, 0 },
    { "T32", &port_wide_shape_T32, ADDR_LAYER0 + 0x120, POOL_T32, 98,  draw_tilemap_layer_32px_9x13_slot06_00,     0x400, 0x100, 0x64, 0, 0 },
    { "SPR", &port_wide_shape_SPR, ADDR_LAYER0 + 0x90, POOL_SPR, 0x145, draw_tile_layer_sprites_with_partial_row_slot06_04, 0x400, 0x200, 0x7c, 0, 0 },
    { "SKEW", &port_wide_shape_SKEW, ADDR_LAYER0 + 0x90, POOL_SKEW, 0x58, draw_skewed_tile_layer_quads_slot06_04,  0x400, 0x200, 0x2c, 0, 0 },
    { "T32_7", &port_wide_shape_T32_7, ADDR_LAYER0 + 0x120, POOL_T32_7, 0x43, draw_tilemap_layer_32px_7x13_slot06_01, 0x400, 0x100, 0x64, 0, 0 },
    { "T16H", &port_wide_shape_T16H, ADDR_LAYER0,        POOL_T16H, 0x74, draw_tile_map_layer_16px_half_scroll_slot06_0d, 0x400, 0x200, 0x7c, 0, 0 },
    { "T32H", &port_wide_shape_T32H, ADDR_LAYER0 + 0x120, POOL_T32H, 0x5a, draw_tile_map_layer_32px_slot06_0d,       0x400, 0x100, 0x64, 0, 0 },
    /* gouki's C stops at 13 records: a sparse map keeps it under that */
    { "T32G", &port_wide_shape_T32G, ADDR_LAYER0 + 0x120, POOL_T32G, 13,   draw_tile_map_layer_32px_slot06_02,       0x400, 0x100, 0x64, 60, 13 },
    { "SCRB", &port_wide_shape_SCRB, ADDR_LAYER0 + 0x90, 0x801f085cu, 0x83, draw_scrolling_tile_map_layer_slot06_08, 0x400, 0x200, 0x7c, 0, 0x0 },
    { "SCRC", &port_wide_shape_SCRC, ADDR_LAYER0 + 0x90, 0x801f11e0u, 0x10e, draw_scrolling_tile_map_layer_slot06_12, 0x400, 0x200, 0x7c, 0, 0x0 },
    { "L0G", &port_wide_shape_L0G, ADDR_LAYER0, 0x801ef0e8u, 0xd4, draw_tile_map_layer_16px_slot06_02, 0x400, 0x200, 0x7c, 0, 0xd4 },
    { "L0N", &port_wide_shape_L0N, ADDR_LAYER0, 0x801ee810u, 0xaa, draw_tile_map_layer_16px_slot03_slot06_03, 0x400, 0x200, 0x7c, 2, 0xaa },
    { "L0R", &port_wide_shape_L0R, ADDR_LAYER0, 0x801eca24u, 0xdf, draw_tile_map_layer_16px_slot06_09, 0x400, 0x200, 0x7c, 0, 0x0 },
    { "L0K", &port_wide_shape_L0K, ADDR_LAYER0, 0x801eedb4u, 0x113, draw_tilemap_layer_16px_12x25_slot06_01, 0x400, 0x200, 0x7c, 0, 0x0 },
    { "SAVE", &port_wide_shape_SAVE, ADDR_LAYER0 + 0x90, 0x801f0d40u, 0x120, draw_tile_layer_sprites_saving_pos_slot06_03, 0x400, 0x200, 0x7c, 0, 0x0 },
    { "POLY", &port_wide_shape_POLY, ADDR_LAYER0 + 0x90, 0x801f4c40u, 0x48, draw_tile_map_layer_polys_slot06_03, 0x400, 0x200, 0x2c, 0, 0x0 },
    { "SKEWP", &port_wide_shape_SKEWP, ADDR_LAYER0 + 0x90, 0x801f3df4u, 0x84, draw_perspective_skewed_tile_map_slot06_0e, 0x400, 0x200, 0x2c, 0, 0x0 },
    { "R16", &port_wide_shape_R16, ADDR_LAYER0, 0x801ef554u, 0xd0, draw_tile_map_16px_tiles_slot06_0e, 0x400, 0x200, 0x7c, 0, 0x0 },
    { "R32", &port_wide_shape_R32, ADDR_LAYER0 + 0x120, 0x801f6734u, 0x75, draw_tile_map_layer_sprites_32px_slot06_0e, 0x400, 0x200, 0x64, 0, 0x0 },
    { "SPRI", &port_wide_shape_SPRI, ADDR_LAYER0 + 0x90, 0x801f25b0u, 0x4b, draw_tile_map_layer_sprites_interpolated_slot06_06, 0x400, 0x200, 0x7c, 0, 0x0 },
};
#define KIND(f) ((f)->shape->kind)
/* the columns the game's C draws, and the ordering table entry its packets go to */
#define C_LO(f) ((f)->shape->k_first)
#define C_HI(f) ((f)->shape->k_first + (f)->shape->cols)

static unsigned record_words(const struct form *f) { return (unsigned)f->shape->words + 1; }   /* the tag and the packet */

/* The pool as the stage's init leaves it (init_first_layer ... init_draw_mode_prim_3/4 and MargePrim: ps1/src/resident/graphics/call_handler_by_kind_index_a.c,
 * port/src/gpu.c): tag length 2 + 3 (or 4) + 1, drawing mode 0xe1000000, window 0, the sprite's tag, grey colour and its code. */
static void init_pool(const struct form *f, uint32_t colour_word)
{
    /* The made-up maps can draw more cells than the stage's real pool holds (the 16 px layer 0 with a blank that matches nothing draws
     * all 400), and the C then writes on past the pool: the records beyond it are set up the same way, so that the comparison is
     * about the arithmetic and not about what lies after a pool. */
    unsigned i, n = record_words(f), total = 2 * f->count + 1024;
    for (i = 0; i < total; i++) {
        uint32_t a = f->pool + i * n * 4;
        unsigned j;
        for (j = 0; j < n; j++) wr32(a + j * 4, 0);
        wr32(a, (uint32_t)f->shape->words << 24);
        if (f->shape->words == 9) {               /* SetPolyFT4: tag length 9, colour and code 0x2c; the init sets the grey */
            wr32(a + 4, colour_word);
            continue;
        }
        wr32(a + 4, 0xe1000000u);
        wr32(a + 12, f->shape->words == 6 ? 0x03000000u : 0x04000000u);
        wr32(a + 16, colour_word);
        if (f->shape->words == 7) wr32(a + 28, 0x00200020u);
    }
}

/* ---- the logical map: tile(col, row) and clut(col, row), written into the game's block layout by other formulas than the C's ---- */
static uint32_t mix(uint32_t a, uint32_t b) { uint32_t h = a * 2654435761u ^ (b + 0x9e3779b9u) * 40503u; h ^= h >> 15; h *= 2246822519u; h ^= h >> 13; return h; }
static int blank2_value = -1;      /* the second blank of the forms that skip one (-1: none) */
static unsigned cells_across, cells_down, blank_value, density;   /* one cell in `density` is not the blank cell: the stage's pool holds fewer records than it has cells */

/* The extent test's maps: whole columns blank outside a window of the map (wrapping) and in fixed gaps of two and three columns inside it. */
static int ext_mode;
static int ext_scan;         /* the expectation is for the extent: cells past the map's own rows are blank, not the zero padding */
static unsigned ext_start, ext_len;

static unsigned logical_tile(unsigned col, unsigned row)
{
    uint32_t h = mix(col, row);
    if (ext_mode && (((col + cells_across - ext_start) % cells_across) >= ext_len || col % 17 == 5 || col % 17 == 6 ||
                     (col % 29 >= 10 && col % 29 <= 12)))
        return blank_value;
    if (h % density != 0) return blank_value;
    if (blank2_value >= 0 && (h >> 4) % 5 == 0) return (unsigned)blank2_value;   /* some of the drawn cells are the other blank */
    return (h >> 8) & 0xffffu;
}
static unsigned logical_clut(unsigned col, unsigned row) { return mix(row + 77, col * 3 + 1) & 0xffffu; }

static void build_map(const struct form *f)
{
    unsigned bc = f->shape->shift == 4 ? 32 : 16, br = f->shape->shift == 4 ? 16 : 8, col, row;
    unsigned blocks_across;
    density = f->density ? f->density : f->shape->words == 9 ? (KIND(f) == PORT_WIDE_QUADS ? 8 : 12) : f->shape->shift == 4 ? (f->shape->unsigned_blank ? 5 : 2) : 2;   /* L0 pool: 114 records for 400 cells; L1 270; T32 98 for 117; SKEW 88 for about 500 */
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

/* v[] is for the sprite and skewed forms. SPR: v[0] start_x_fixed, v[1] end_x_fixed, v[3] range_start, v[4] range_end, v[5] field_4c
 * (scroll_y is its base_x_fixed, the same word). SKEW: v[0] bound_x_a, v[1] the low half of bound_x_b (scroll_x is its high half),
 * v[2] signed_param, v[3] field_1c, v[4] rect_right, v[5] field_4c. */
struct fixture { unsigned scroll_x; int32_t scroll_y; unsigned game_scroll_y; unsigned blank; unsigned ot_index; unsigned buffer; int32_t v[6]; };

/* the ordering table entry that the game's C links its packets into */
static unsigned entry_of(const struct form *f, const struct fixture *x)
{
    if (f->shape->fixed_entry) return (unsigned)f->shape->fixed_entry - 1;
    return KIND(f) == PORT_WIDE_SKEWED ? x->ot_index - 1 : x->ot_index;
}

static void set_layer(const struct form *f, const struct fixture *x)
{
    uint32_t a = f->layer;
    memset((void *)(uintptr_t)a, 0, f->shape->second_blank ? 0x120 : 0x90);
    wr16(a + 0x1e, x->blank);
    wr32(a + 0x50, ADDR_MAP - 2u * (unsigned)f->shape->map_offset);   /* the C adds map_offset halfwords */
    wr32(a + 0x58, f->width);
    switch (KIND(f)) {
    case PORT_WIDE_SPRITES:
        wr32(a + 0x14, (uint32_t)x->scroll_y);
        wr32(a + 0x08, (uint32_t)x->v[0]);
        wr32(a + 0x10, (uint32_t)x->v[1]);
        wr16(a + 0x1c, (unsigned)x->v[3]);
        wr16(a + 0x4e, (unsigned)x->v[4]);
        wr16(a + 0x4c, (unsigned)x->v[5]);
        wr32(a + 0x5c, f->height);
        break;
    case PORT_WIDE_SKEWED:
        wr32(a + 0x08, (uint32_t)x->v[0]);
        wr32(a + 0x10, ((uint32_t)(x->scroll_x & 0xffffu) << 16) | ((uint32_t)x->v[1] & 0xffffu));
        wr16(a + 0x3e, (unsigned)x->v[2]);
        wr16(a + 0x1c, (unsigned)x->v[3]);
        wr16(a + 0x4e, (unsigned)x->v[4]);
        wr16(a + 0x4c, (unsigned)x->v[5]);
        wr32(a + 0x5c, 0x100);                     /* Slot06Layer.scale: the layer has no height */
        break;
    case PORT_WIDE_QUADS:
        wr16(a + 0x12, x->scroll_x);
        wr16(a + 0x3e, (unsigned)x->v[2]);
        wr16(a + 0x4c, (unsigned)x->v[5]);
        wr32(a + 0x5c, 0x100);
        break;
    default:
        wr16(a + 0x12, x->scroll_x);
        wr32(a + 0x14, (uint32_t)x->scroll_y);
        wr32(a + 0x5c, f->height);
        if (f->shape->camera_rows) wr16(a + 0x4c, (unsigned)x->v[5]);
        if (f->shape->second_blank) wr16(a + 0x90 + 0x1e, (unsigned)x->v[0]);
    }
    wr16(0x8019054eu, x->game_scroll_y >> 16);       /* the global that chunli_alt's first eight rows scroll by */
    ((uint8_t *)(uintptr_t)a)[0x8a] = (uint8_t)x->ot_index;
    wr16(ADDR_GAME_STATE_SCROLL_Y, x->game_scroll_y);
    wr32(ADDR_DRAW_BUFFER_INDEX, x->buffer);
    wr32(ADDR_CURRENT_OT, ADDR_OT);
    memset((void *)(uintptr_t)ADDR_OT, 0, 30 * 4);
    blank_value = x->blank;
    blank2_value = f->shape->second_blank ? (int)(x->v[0] & 0xffff) : -1;
    build_map(f);
}

/* ---- the packets, as arrays ---- */
struct packet { int entry; uint32_t w[12]; };
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
static struct fixture fixtures[NFORMS][NFIX];
static unsigned fixture_count;
static const struct fixture *fix(const struct form *f, unsigned i) { return &fixtures[f - forms][i]; }

static int floordiv(int a, int b);
static int floormod(int a, int b);
static void make_scrb_fixture(struct fixture *x, unsigned variant);
static void make_poly_fixture(struct fixture *x, unsigned variant);
static void make_r32_fixture(struct fixture *x, unsigned variant);

/* the sprite form: a camera and a range made so that the layer's y is y0 and the C draws a few rows */
static void make_sprites_fixture(struct fixture *x, unsigned variant)
{
    int gs = (int)(lcg() % 0x30), y0 = variant % 7 == 3 ? -(int)(lcg() % 20) : (int)(lcg() % 400);
    int rest = variant % 5 == 2 ? 16 * (int)(lcg() % 14) : (int)(lcg() % 224);   /* the camera above the first row: no partial row when a multiple of 16 */
    int32_t start = (int32_t)(((lcg() % 0x300) << 16) | (lcg() & 0xffff)), diff = (int32_t)(((lcg() % 0x30) << 16) | (lcg() & 0xffff));
    int f4c;
    memset(x, 0, sizeof *x);
    if (y0 < 0 && rest < 32) rest += 32;                                            /* keep the partial row inside the map's memory */
    f4c = floordiv(y0, 16) * 16 + rest;
    x->game_scroll_y = (unsigned)gs;
    x->scroll_y = (int32_t)(0x100000 - ((y0 + gs) << 16) - (int)(lcg() & 0xffff));  /* pos >> 16 is y0 + gs */
    x->blank = variant % 4 == 1 ? (lcg() & 0xffffu) : lcg() % 6;
    x->ot_index = 1 + lcg() % 28;
    x->buffer = lcg() & 1;
    x->v[0] = start;
    x->v[1] = variant % 6 == 4 ? start - diff : start + diff;
    x->v[3] = f4c - (int)(lcg() % 100);
    x->v[4] = x->v[3] + 1 + (int)(lcg() % 300);
    x->v[5] = f4c;
}

/* the skewed form: the camera's y in 0..99 (the C draws 15 less its sixteenth rows, from the map's first rows), offsets of up to
 * a few hundred pixels from the row's perspective */
static void make_skewed_fixture(struct fixture *x, unsigned variant)
{
    uint32_t bound_b;
    int delta;
    memset(x, 0, sizeof *x);
    x->scroll_x = lcg() & 0xffffu;
    bound_b = (x->scroll_x << 16) | (lcg() & 0xffffu);
    delta = variant % 2 ? (int)(((lcg() % 0xa0) - 0x50) << 16) + (int)(lcg() & 0xffff)      /* offsets of hundreds of pixels: quads reach past the 44 cells */
                         : (int)(((lcg() % 0x30) - 0x10) << 16) + (int)(lcg() & 0xffff);
    x->v[1] = (int32_t)(bound_b & 0xffffu);
    x->v[0] = (int32_t)(bound_b - (uint32_t)delta);
    x->v[2] = (int)(lcg() % 13) - 6;
    x->v[3] = (int)(lcg() % 100);
    x->v[4] = x->v[3] + (variant % 5 == 3 ? -(60 + (int)(lcg() % 200)) : 60 + (int)(lcg() % 200));
    x->v[5] = (int)(lcg() % 100);
    x->blank = variant % 4 == 1 ? (lcg() & 0xffffu) : lcg() % 6;
    x->ot_index = 1 + lcg() % 28;
    x->buffer = lcg() & 1;
}

static void make_fixtures(void)
{
    unsigned i, fi;
    /* by hand: the edges of the arithmetic. scroll_y 0x100000 and game scroll_y 0 put the layer's row 0 at the top. */
    static const struct fixture edge[] = {
        { 0x0000, 0x00100000, 0,    0x0000, 3, 0, { 0 } },   /* origin, blank 0 */
        { 0x0003, 0x00100000, 0,    0x0001, 5, 1, { 0 } },   /* a few pixels in */
        { 0x03f5, 0x00100000, 0,    0x0002, 7, 0, { 0 } },   /* the map wraps to the left inside the 25 columns */
        { 0xfff5, 0x000f8000, 0x10, 0x0003, 9, 1, { 0 } },   /* negative scroll_x as a s16; y above the map */
        { 0x01ff, 0x00080000, 0x00, 0x8123, 11, 0, { 0 } },  /* a blank with the high bit: the 16 px layer 0 compares the u16 cell with the s16 blank */
        { 0x0080, 0x00300000, 0x08, 0x0004, 13, 1, { 0 } },  /* rows near the bottom of the map */
        { 0x0100, 0x000c0000, 0x40, 0x0005, 15, 0, { 0 } },  /* rows partly above, partly in the map (y negative) */
        { 0x0213, 0x7fff0000, 0x00, 0x0006, 17, 1, { 0 } },  /* a big scroll_y */
        { 0x000f, 0xfff00000u, 0xff, 0xffff, 19, 0, { 0 } }, /* negative scroll_y; blank 0xffff */
        { 0x03ff, 0x00104000, 0x01, 0x0007, 21, 1, { 0 } },
    };
    fixture_count = 0;
    for (i = 0; i < sizeof edge / sizeof edge[0]; i++) fixtures[0][fixture_count++] = edge[i];
    while (fixture_count < NFIX) {
        struct fixture x;
        memset(&x, 0, sizeof x);
        x.scroll_x = lcg() & 0xffffu;
        x.scroll_y = (int32_t)(0x100000 - (int32_t)(lcg() % 0x300) * 0x10000 + (int32_t)(lcg() & 0xffff));
        x.game_scroll_y = lcg() % 0x30;
        x.blank = (lcg() % 3) ? 1 + lcg() % 9 : (lcg() & 0xffffu);
        x.ot_index = lcg() % 30;
        x.buffer = lcg() & 1;
        fixtures[0][fixture_count++] = x;
    }
    memcpy(fixtures[1], fixtures[0], sizeof fixtures[0]);
    memcpy(fixtures[2], fixtures[0], sizeof fixtures[0]);
    for (i = 5; i < NFORMS; i++) memcpy(fixtures[i], fixtures[0], sizeof fixtures[0]);   /* the other plain forms: the same layers */
    for (i = 0; i < NFIX; i++) {
        make_sprites_fixture(&fixtures[3][i], i);
        make_skewed_fixture(&fixtures[4][i], i);
    }
    /* the forms of the fourth increment: their own fixtures where the layer's fields differ */
    for (fi = 5; fi < NFORMS; fi++) {
        const char *tag = forms[fi].tag;
        for (i = 0; i < NFIX; i++) {
            struct fixture *x = &fixtures[fi][i];
            if (!strcmp(tag, "SCRB")) make_scrb_fixture(x, i);
            else if (!strcmp(tag, "SPRI")) make_sprites_fixture(x, i);
            else if (!strcmp(tag, "SKEWP")) make_skewed_fixture(x, i);
            else if (!strcmp(tag, "SAVE")) { make_sprites_fixture(x, i); x->scroll_x = lcg() & 0xffffu; }
            else if (!strcmp(tag, "POLY")) make_poly_fixture(x, i);
            else if (!strcmp(tag, "R32")) make_r32_fixture(x, i);
            else if (!strcmp(tag, "SCRC")) x->game_scroll_y |= (lcg() & 0xffffu) << 16;           /* the global at 0x8019054e */
            else if (!strcmp(tag, "L0G") || !strcmp(tag, "L0N")) {                              /* the next layer's blank */
                x->v[0] = i % 4 == 3 ? (int32_t)(0x8000u | (lcg() & 0xff)) : (int32_t)((x->blank + 1 + lcg() % 3) & 0xffffu);
            }
        }
    }
}

/* birdie's draw_scrolling_tile_map_layer: t = (base_x_fixed >> 16) + scroll_y is the layer's top, the camera row count follows from f4c */
static void make_scrb_fixture(struct fixture *x, unsigned variant)
{
    int gs = (int)(lcg() % 0x30), tt = (int)(lcg() % 300), f4c = 80 + (variant % 5 == 2 ? 16 * (int)(lcg() % 12) : (int)(lcg() % 200));
    int32_t start = (int32_t)(((lcg() % 0x300) << 16) | (lcg() & 0xffff)), diff = (int32_t)(((lcg() % 0x30) << 16) | (lcg() & 0xffff));
    memset(x, 0, sizeof *x);
    x->game_scroll_y = (unsigned)gs;
    x->scroll_y = (int32_t)(((uint32_t)(tt - gs) << 16) | (lcg() & 0xffff));       /* (base >> 16) + gs = tt */
    x->blank = variant % 4 == 1 ? (lcg() & 0xffffu) : lcg() % 6;
    x->ot_index = 1 + lcg() % 28;
    x->buffer = lcg() & 1;
    x->v[0] = start;
    x->v[1] = variant % 6 == 4 ? start - diff : start + diff;
    x->v[3] = f4c - (int)(lcg() % 100);
    x->v[4] = x->v[3] + 1 + (int)(lcg() % 300);
    x->v[5] = f4c;
}

/* nash's draw_tile_map_layer_polys: unskewed quads from the camera's y (8..100) */
static void make_poly_fixture(struct fixture *x, unsigned variant)
{
    memset(x, 0, sizeof *x);
    x->scroll_x = lcg() & 0xffffu;
    x->v[2] = (int)(lcg() % 13) - 6;
    x->v[5] = 8 + (int)(lcg() % 93);
    x->blank = variant % 4 == 1 ? (lcg() & 0xffffu) : lcg() % 6;
    x->ot_index = 1 + lcg() % 28;
    x->buffer = lcg() & 1;
}

/* rolento's 32 px grid: the half of the map comes from the layer's scroll_y (high half) in three ranges, and from bit 3 of the row */
static void make_r32_fixture(struct fixture *x, unsigned variant)
{
    unsigned hi = variant % 3 == 0 ? lcg() % 0x100 : variant % 3 == 1 ? 0x100 + lcg() % 0x500 : 0x600 + lcg() % 0xa00;
    memset(x, 0, sizeof *x);
    x->scroll_x = lcg() & 0xffffu;
    x->scroll_y = (int32_t)((hi << 16) | (lcg() & 0xffff));
    x->game_scroll_y = lcg() % 0x30;
    x->blank = variant % 4 == 1 ? (lcg() & 0xffffu) : lcg() % 6;
    x->ot_index = 1 + lcg() % 28;
    x->buffer = lcg() & 1;
}

/* ---- case: the maker reproduces the game's C ---- */
static int case_matches_c(const struct form *f, unsigned *packets)
{
    unsigned i, total = 0;
    for (i = 0; i < fixture_count; i++) {
        const struct fixture *x = fix(f, i);
        unsigned d;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        f->game_c();
        harvest(f, entry_of(f, x), &from_c);
        run_maker(f, x, C_LO(f), C_HI(f), &from_maker);
        if (f->cap && from_c.n >= f->cap && from_maker.n > f->cap) from_maker.n = f->cap;   /* the C returns at its record limit (gouki, nash): the first records are the same */
        d = compare(&from_c, &from_maker, (unsigned)f->shape->words);
        if (d) {
            unsigned w = 0;
            if (d <= from_c.n && d <= from_maker.n)
                while (w + 1 < (unsigned)f->shape->words && from_c.p[d - 1].w[w] == from_maker.p[d - 1].w[w]) w++;
            return BAD("fixture %u: packet %u differs (C made %u, the maker %u; word %u: C %08x, maker %08x)", i, d - 1, from_c.n, from_maker.n, w,
                       d <= from_c.n ? from_c.p[d - 1].w[w] : 0, d <= from_maker.n ? from_maker.p[d - 1].w[w] : 0);
        }
        total += from_c.n;
    }
    *packets = total;
    return total != 0 ? 1 : BAD("the C drew nothing in any fixture");
}

/* ---- case: the side columns by an independent expectation ---- */
static int floordiv(int a, int b) { int q = a / b; return (a % b != 0 && ((a < 0) != (b < 0))) ? q - 1 : q; }
static int floormod(int a, int b) { return a - floordiv(a, b) * b; }

/* One cell of the logical map as the packet words: the packet starts as the pool's first record. */
static uint32_t pack_xy(int x, int y) { return (uint32_t)(uint16_t)x | ((uint32_t)(uint16_t)y << 16); }

/* The plain forms' cells: rows of the C's own count from the layer's scroll (nash's saving_pos: from the camera; rolento's: the map's own lines). */
static void expect_flat(const struct form *f, const struct fixture *x, int k0, int k1, struct packets *out)
{
    const int size = 1 << f->shape->shift, b = (int16_t)x->blank, b2 = (int16_t)x->v[0];
    const int rolento = f->shape->variant == PORT_WIDE_V_ROLENTO16 || f->shape->variant == PORT_WIDE_V_ROLENTO32;
    int64_t s = (int64_t)(int32_t)x->scroll_y;
    int64_t above = ((int64_t)0x100000 - s);                           /* -(scroll_y - 0x100000) */
    int gs = f->shape->half_scroll ? floordiv((int16_t)x->game_scroll_y, 2) : (int)(x->game_scroll_y & 0xffff);   /* the half-speed layers subtract half of it */
    int y = (int16_t)(uint16_t)(floordiv((int)(int32_t)above, 65536) - gs);
    int sx = x->scroll_x & (size - 1), sy = floormod(y, size), row0 = floordiv(y, size);
    int col0 = floordiv((int)(int16_t)x->scroll_x, size);
    int nrows = f->shape->rows, i, k;
    const uint32_t *rec = first_record(f, x->buffer);
    if (f->shape->camera_rows) nrows = (int16_t)(((uint32_t)(x->v[5] - 16 * floordiv(y, 16)) >> 4) + 1);
    out->n = 0;
    for (i = 0; i < nrows; i++) {
        int row = row0 + i, lrow;
        if (rolento) {
            /* the map has one row of blocks: 16 rows in the 16 px grid; in the 32 px grid two halves of 8 rows, chosen by scroll_y and row bit 3 */
            unsigned ru = (uint16_t)(((y & 0x1f0) / size) + i);
            if (f->shape->variant == PORT_WIDE_V_ROLENTO16) lrow = (int)(ru & 15);
            else {
                unsigned f16 = (uint16_t)(x->scroll_y >> 16);
                int half = f16 < 0x100 ? ((ru & 8) != 0) : f16 < 0x600 ? 1 : ((ru & 8) == 0);
                lrow = (int)(ru & 7) + 8 * half;
            }
        } else {
            lrow = row;
            if (row < 0 || row > (int)cells_down || (row == (int)cells_down && !f->shape->row_inclusive)) continue;   /* only the 32 px C also takes the row after the last */
        }
        if (ext_scan && lrow >= (int)cells_down) continue;
        for (k = k0; k < k1; k++) {
            unsigned col = (unsigned)floormod(col0 + k, (int)cells_across);
            unsigned tile = lrow < (int)cells_down ? logical_tile(col, (unsigned)lrow) : 0;   /* past the map: the zero padding */
            unsigned clut = lrow < (int)cells_down ? logical_clut(col, (unsigned)lrow) : 0;
            int tile_s = (int16_t)tile, skip;
            struct packet *p;
            if (f->shape->unsigned_blank)   /* u16 against s16 as ints: a blank with the high bit matches nothing */
                skip = (b >= 0 && (int)tile == b) || (f->shape->second_blank && b2 >= 0 && (int)tile == b2);
            else skip = (tile_s == b);
            if (skip) continue;
            p = &out->p[out->n++];
            memset(p, 0, sizeof *p);
            p->entry = (int)entry_of(f, x);
            memcpy(p->w, rec + 1, (unsigned)f->shape->words * 4);
            p->w[0] = 0xe1000000u + (uint32_t)floordiv(tile_s, 1024);
            p->w[4] = pack_xy(k * size - sx, i * size - sy);
            p->w[5] = (uint32_t)((tile & 31) * 8) | (uint32_t)(((tile >> 5) & 31) * 8) << 8 | (uint32_t)clut << 16;
        }
    }
}

/* The sprite form: the x position and the row count come from the camera (a position between two ends), then one partial row.
 * Birdie's draw_scrolling_tile_map_layer: the same with the row counter at 4 and the loop counter and the top from the position. */
static void expect_sprites(const struct form *f, const struct fixture *x, int k0, int k1, struct packets *out)
{
    const int b = (int16_t)x->blank, f4c = x->v[5];
    int64_t diff = (int32_t)((uint32_t)x->v[1] - (uint32_t)x->v[0]);
    int64_t q = diff * (int64_t)(f4c - x->v[3]) / (int64_t)(x->v[4] - x->v[3]);
    int x12 = (int)floordiv((int)(q + (int64_t)x->v[0]), 65536);
    int y, i0, row_start, n, i, k, i_end, sx, sy, row0, col0;
    const uint32_t *rec = first_record(f, x->buffer);
    if (f->shape->variant == PORT_WIDE_V_BIRDIE) {
        int hi = (int16_t)(uint16_t)(floordiv(x->scroll_y, 65536) + (int)(x->game_scroll_y & 0xffff));     /* the top t, as the high half of the word */
        int64_t xm = (int64_t)hi * 65536 + (x->scroll_y & 0xffff);
        y = floordiv((int)(0x500000 - xm), 65536);                                                          /* 0x400000 - (xm - 0x100000) */
        i0 = (hi + 0x3f) / 16;
        row_start = 4;
        n = (int16_t)((uint32_t)(f4c - 16 * floordiv(y - 0x40, 16)) >> 4);
    } else {
        int32_t pos = (int32_t)(0x100000u - (uint32_t)x->scroll_y);
        y = (int16_t)(floordiv(pos, 65536) - (int)(x->game_scroll_y & 0xffff));
        i0 = 0;
        row_start = floordiv(y, 16);
        n = (int16_t)((uint32_t)(f4c - 16 * floordiv(y, 16)) >> 4);
    }
    sx = floormod(x12, 16);
    sy = floormod(y, 16);
    col0 = floordiv(x12, 16);
    row0 = row_start;
    i_end = i0 < n ? n : i0;
    out->n = 0;
    for (i = i0; i <= i_end; i++) {
        int row = row0 + (i - i0), partial = i == i_end;
        if (partial ? !(f4c & 15) : (row < 0 || row > (int)cells_down)) continue;       /* the C's rows; the partial row has no bounds */
        if (ext_scan && row >= (int)cells_down) continue;
        for (k = k0; k < k1; k++) {
            unsigned col = (unsigned)floormod(col0 + k, (int)cells_across);
            unsigned tile = row < (int)cells_down ? logical_tile(col, (unsigned)row) : 0;
            unsigned clut = row < (int)cells_down ? logical_clut(col, (unsigned)row) : 0;
            int tile_s = (int16_t)tile;
            struct packet *p;
            if (tile_s == b) continue;
            p = &out->p[out->n++];
            memset(p, 0, sizeof *p);
            p->entry = (int)entry_of(f, x);
            memcpy(p->w, rec + 1, (unsigned)f->shape->words * 4);
            p->w[0] = 0xe1000000u + (uint32_t)floordiv(tile_s, 1024);
            p->w[4] = pack_xy(k * 16 - sx, i * 16 - sy);
            p->w[5] = (uint32_t)((tile & 31) * 8) | (uint32_t)(((tile >> 5) & 31) * 8) << 8 | (uint32_t)clut << 16;
        }
    }
}

/* nash's draw_tile_map_layer_polys: 24 unskewed quads a row, fixed entry. */
static void expect_quads(const struct form *f, const struct fixture *x, int k0, int k1, struct packets *out)
{
    const int b = (int16_t)x->blank, n = x->v[2], f4c = x->v[5], f12 = (int16_t)x->scroll_x;
    const int y = (uint16_t)(f4c - (n + 5)), sy = y & 15, sx = floormod(f12, 16);
    const int col0 = floordiv(f12, 16), t = f4c - 5, row0 = t / 16, off = (int16_t)(t - row0 * 16);
    const int i0 = (int16_t)y / 16, lim = i0 + 15 - t / 16;
    const uint32_t *rec = first_record(f, x->buffer);
    int i, k;
    out->n = 0;
    for (i = i0; i < lim; i++) {
        const int first = i == i0, ya = sy + 16 * i, yu = (uint16_t)ya, row = row0 + (i - i0);
        const int yb = (uint16_t)(first ? ya + 16 - off : ya - off);
        for (k = k0; k < k1; k++) {
            unsigned col = (unsigned)floormod(col0 + k, (int)cells_across);
            unsigned tile = row < (int)cells_down ? logical_tile(col, (unsigned)row) : 0;
            unsigned clut = row < (int)cells_down ? logical_clut(col, (unsigned)row) : 0;
            int tile_s = (int16_t)tile, u = (int)tile * 8, v = (tile_s >> 2) & -16;
            int x0 = (int16_t)(16 * k - sx), x1 = (int16_t)(16 * k - sx + 16);
            int y0 = (int16_t)(first ? yu : yb), y2 = (int16_t)(first ? yb : yb + 16);
            int v0 = (first ? off + v : v) & 0xff;
            struct packet *p;
            if (tile_s == b) continue;
            p = &out->p[out->n++];
            memset(p, 0, sizeof *p);
            p->entry = (int)entry_of(f, x);
            memcpy(p->w, rec + 1, (unsigned)f->shape->words * 4);
            p->w[1] = pack_xy(x0, y0);
            p->w[2] = (uint32_t)(u & 0xff) | (uint32_t)v0 << 8 | (uint32_t)clut << 16;
            p->w[3] = pack_xy(x1, y0);
            p->w[4] = (uint32_t)((u + 15) & 0xff) | (uint32_t)v0 << 8 | (uint32_t)(uint16_t)floordiv(tile_s, 1024) << 16;
            p->w[5] = pack_xy(x0, y2);
            p->w[6] = (rec[7] & 0xffff0000u) | (uint32_t)(u & 0xff) | (uint32_t)((v | 15) & 0xff) << 8;
            p->w[7] = pack_xy(x1, y2);
            p->w[8] = (rec[9] & 0xffff0000u) | (uint32_t)((u + 15) & 0xff) | (uint32_t)((v | 15) & 0xff) << 8;
        }
    }
}

/* chunli_alt's draw_scrolling_tile_map_layer: rows 0..7 scroll by the global (high half of the game word), rows 8..15 by the layer's own
 * scroll_x; `halves` bit 0 / bit 1 choose which. */
static void expect_scrollc(const struct form *f, const struct fixture *x, int k0, int k1, int halves, struct packets *out)
{
    const int b = (int16_t)x->blank;
    int32_t pos = (int32_t)(0x100000u - (uint32_t)x->scroll_y);
    int y = (int16_t)(floordiv(pos, 65536) - (int)(x->game_scroll_y & 0xffff));
    int sy = floormod(y, 16), row0 = floordiv(y, 16), i, k;
    const uint32_t *rec = first_record(f, x->buffer);
    out->n = 0;
    for (i = 0; i < 16; i++) {
        int half = i >= 8, g = half ? (int16_t)x->scroll_x : (int16_t)(x->game_scroll_y >> 16), row = row0 + i;
        int sx = floormod(g, 16), col0 = floordiv(g, 16);
        if (!(halves & (1 << half)) || row < 0 || row >= (int)cells_down) continue;
        for (k = k0; k < k1; k++) {
            unsigned col = (unsigned)floormod(col0 + k, (int)cells_across);
            unsigned tile = logical_tile(col, (unsigned)row), clut = logical_clut(col, (unsigned)row);
            int tile_s = (int16_t)tile;
            struct packet *p;
            if (tile_s == b) continue;
            p = &out->p[out->n++];
            memset(p, 0, sizeof *p);
            p->entry = (int)entry_of(f, x);
            memcpy(p->w, rec + 1, (unsigned)f->shape->words * 4);
            p->w[0] = 0xe1000000u + (uint32_t)floordiv(tile_s, 1024);
            p->w[4] = pack_xy(k * 16 - sx, i * 16 - sy);
            p->w[5] = (uint32_t)((tile & 31) * 8) | (uint32_t)(((tile >> 5) & 31) * 8) << 8 | (uint32_t)clut << 16;
        }
    }
}

/* The skewed form: 44 cells a row from k = -11, each a quad with the row's x offsets; `visible`: only the quads that reach into x -128..512. */
static void expect_skewed(const struct form *f, const struct fixture *x, int k0, int k1, int visible, struct packets *out)
{
    const int b = (int16_t)x->blank, n = x->v[2], f4c = x->v[5];
    const int y = (uint16_t)(f4c - n), d1 = (int16_t)(x->v[3] - n), d2 = (int16_t)(x->v[4] - x->v[3]);
    const int f12 = (int16_t)x->scroll_x, sx = floormod(f12 - 0xb0, 16), sy = y & 15;
    const uint32_t bound_b = ((uint32_t)(x->scroll_x & 0xffffu) << 16) | ((uint32_t)x->v[1] & 0xffffu);
    const int64_t a = (int32_t)(bound_b - (uint32_t)x->v[0]);
    const int col0 = floormod(floordiv(f12 - 0xb0, 16), (int)cells_across);
    const int row0 = f4c / 16, sy2 = (uint16_t)(f4c - row0 * 16), i0 = (int16_t)y / 16, lim = i0 + 15 - f4c / 16;
    const uint32_t *rec = first_record(f, x->buffer);
    int i, k;
    out->n = 0;
    for (i = i0; i < lim; i++) {
        const int first = i == i0, ya = sy + 16 * i, yu = (uint16_t)ya, row = row0 + (i - i0);
        const int32_t m1 = (int32_t)(a * (first ? yu - d1 : yu - (int16_t)sy2 - d1));    /* wrapped to 32 bits */
        const int32_t m0 = (int32_t)(a * (yu - ((int16_t)sy2 - 16) - d1));
        const int s1 = (int32_t)(a - m1 / d2), s0 = (int32_t)(a - m0 / d2);
        for (k = k0; k < k1; k++) {
            unsigned col = (unsigned)floormod(col0 + (k + 11), (int)cells_across);
            unsigned tile = row < (int)cells_down ? logical_tile(col, (unsigned)row) : 0;
            unsigned clut = row < (int)cells_down ? logical_clut(col, (unsigned)row) : 0;
            int tile_s = (int16_t)tile, u = (int)tile * 8, v = (tile_s >> 2) & -16;
            int x0 = (int16_t)(k * 16 - sx + floordiv(s1, 65536)), x2 = (int16_t)(k * 16 - sx + floordiv(s0, 65536));
            int x1 = (int16_t)(x0 + 16), x3 = (int16_t)(x2 + 16), y0, y2, v0;
            struct packet *p;
            if (tile_s == b) continue;
            if (visible && !((x1 > x3 ? x1 : x3) > -128 && (x0 < x2 ? x0 : x2) < 512)) continue;
            if (first) { y0 = (int16_t)yu; y2 = (int16_t)(ya + 16 - sy2); v0 = (sy2 + v) & 0xff; }
            else { y0 = (int16_t)(ya - sy2); y2 = (int16_t)(y0 + 16); v0 = v & 0xff; }
            p = &out->p[out->n++];
            memset(p, 0, sizeof *p);
            p->entry = (int)entry_of(f, x);
            memcpy(p->w, rec + 1, (unsigned)f->shape->words * 4);
            p->w[1] = pack_xy(x0, y0);
            p->w[2] = (uint32_t)(u & 0xff) | (uint32_t)v0 << 8 | (uint32_t)clut << 16;
            p->w[3] = pack_xy(x1, y0);
            p->w[4] = (uint32_t)((u + 15) & 0xff) | (uint32_t)v0 << 8 | (uint32_t)(uint16_t)floordiv(tile_s, 1024) << 16;
            p->w[5] = pack_xy(x2, y2);
            p->w[6] = (rec[7] & 0xffff0000u) | (uint32_t)(u & 0xff) | (uint32_t)((v | 15) & 0xff) << 8;
            p->w[7] = pack_xy(x3, y2);
            p->w[8] = (rec[9] & 0xffff0000u) | (uint32_t)((u + 15) & 0xff) | (uint32_t)((v | 15) & 0xff) << 8;
        }
    }
}

static void expect_columns(const struct form *f, const struct fixture *x, int k0, int k1, int visible, struct packets *out)
{
    switch (KIND(f)) {
    case PORT_WIDE_SPRITES: expect_sprites(f, x, k0, k1, out); break;
    case PORT_WIDE_SKEWED: expect_skewed(f, x, k0, k1, visible, out); break;
    case PORT_WIDE_QUADS: expect_quads(f, x, k0, k1, out); break;
    case PORT_WIDE_SCROLLC: expect_scrollc(f, x, k0, k1, visible ? visible : 3, out); break;
    default: expect_flat(f, x, k0, k1, out);
    }
}

static void append(struct packets *dst, const struct packets *src)
{
    unsigned i;
    for (i = 0; i < src->n && dst->n < MAX_PACKETS; i++) dst->p[dst->n++] = src->p[i];
}

static struct packets whole_maker, whole_expected, side_a;

static int case_sides(const struct form *f, unsigned *packets)
{
    unsigned i, total = 0;
    const int skewed = KIND(f) == PORT_WIDE_SKEWED;
    for (i = 0; i < fixture_count; i++) {
        const struct fixture *x = fix(f, i);
        int kl, kr, side;
        unsigned d;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        f->game_c();
        if (skewed) {
            kl = C_LO(f) - PORT_WIDE_SKEW_EXTRA;
            kr = C_HI(f) + PORT_WIDE_SKEW_EXTRA;
            /* the extra columns reach far enough: no quad of the next eight columns is in the view */
            expect_columns(f, x, kl - 8, kl, 1, &expected);
            if (expected.n) return BAD("fixture %u: %u visible quads left of the extra columns", i, expected.n);
            expect_columns(f, x, kr, kr + 8, 1, &expected);
            if (expected.n) return BAD("fixture %u: %u visible quads right of the extra columns", i, expected.n);
        } else if (KIND(f) == PORT_WIDE_SCROLLC) {
            kl = kr = 0;                                              /* per half, below */
        } else {
            port_wide_side_range(f->shape, layer_of(f), &kl, &kr);
            if (kl >= C_LO(f) || kr <= C_HI(f)) return BAD("fixture %u: side range %d..%d", i, kl, kr);
            for (side = 0; side < 2; side++) {
                int k0 = side ? C_HI(f) : kl, k1 = side ? kr : C_LO(f);
                run_maker(f, x, k0, k1, &from_maker);
                expect_columns(f, x, k0, k1, 0, &expected);
                d = compare(&from_maker, &expected, (unsigned)f->shape->words);
                if (d) return BAD("fixture %u, k %d..%d: packet %u differs (maker %u, expected %u)", i, k0, k1 - 1, d - 1, from_maker.n, expected.n);
            }
        }
        /* what the override queues, port_wide_sides: the left columns, then the right ones (per half for chunli_alt's two scrolls) */
        if (KIND(f) == PORT_WIDE_SCROLLC) {
            int half;
            whole_expected.n = 0;
            for (half = 0; half < 2; half++) {
                int g = half ? (int16_t)x->scroll_x : (int16_t)(x->game_scroll_y >> 16), sx = floormod(g, 16), l = 0, r = f->shape->cols;
                while ((l - 1) * 16 - sx + 16 > -128) l--;
                while (r * 16 - sx < 512) r++;
                expect_columns(f, x, l, 0, 1 << half, &side_a);
                append(&whole_expected, &side_a);
                expect_columns(f, x, f->shape->cols, r, 1 << half, &side_a);
                append(&whole_expected, &side_a);
            }
        } else {
            expect_columns(f, x, kl, C_LO(f), skewed, &whole_expected);
            expect_columns(f, x, C_HI(f), kr, skewed, &side_a);
            append(&whole_expected, &side_a);
        }
        sink = &whole_maker;
        whole_maker.n = 0;
        port_wide_sides(f->shape, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y, first_record(f, x->buffer), sink_emit);
        d = compare(&whole_maker, &whole_expected, (unsigned)f->shape->words);
        if (d) return BAD("fixture %u: port_wide_sides packet %u differs (maker %u, expected %u)", i, d - 1, whole_maker.n, whole_expected.n);
        total += whole_maker.n;
    }
    *packets = total;
    return total != 0 ? 1 : BAD("no side tiles in any fixture");
}

/* ---- case: the expectation also reproduces the game's C in its own range (so the independent formulas are the C's) ---- */
static int case_expect_in_c_range(const struct form *f)
{
    unsigned i;
    for (i = 0; i < fixture_count; i++) {
        const struct fixture *x = fix(f, i);
        unsigned d;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        f->game_c();
        harvest(f, entry_of(f, x), &from_c);
        expect_columns(f, x, C_LO(f), C_HI(f), 0, &expected);
        if (f->cap && from_c.n >= f->cap && expected.n > f->cap) expected.n = f->cap;   /* the C stopped at its record limit */
        d = compare(&from_c, &expected, (unsigned)f->shape->words);
        if (d) return BAD("fixture %u: packet %u differs (C %u, expected %u)", i, d - 1, from_c.n, expected.n);
    }
    return 1;
}

/* ---- the painted extent (step 5) ---- */
static int exp_x12(const struct fixture *x)
{
    int64_t diff = (int32_t)((uint32_t)x->v[1] - (uint32_t)x->v[0]);
    int64_t q = diff * (int64_t)(x->v[5] - x->v[3]) / (int64_t)(x->v[4] - x->v[3]);
    return (int)floordiv((int)(q + (int64_t)x->v[0]), 65536);
}

static struct packets ext_tmp;

/* The extent by the packets of the expectation's own cells over a wide range of columns: a slot is painted when a cell overlaps it;
 * from the view's edges outward, slot by slot, `gap` blank slots in a row end it. The grid is the cells' own columns for the forms with one
 * offset, the screen's slots (0 on an edge) for the skewed form and chunli_alt's. */
static void expected_extent(const struct form *f, const struct fixture *x, int gap, int *lo, int *hi)
{
    const int size = 1 << f->shape->shift, wide = size == 16 ? 40 : 20;
    int off = 0, i, s, blank, left_stop, right_stop;
    if (KIND(f) == PORT_WIDE_SPRITES) off = floormod(exp_x12(x), size);
    else if (KIND(f) != PORT_WIDE_SKEWED && KIND(f) != PORT_WIDE_SCROLLC) off = x->scroll_x & (size - 1);
    ext_scan = 1;
    expect_columns(f, x, C_LO(f) - wide, C_HI(f) + wide, 0, &ext_tmp);
    ext_scan = 0;
    #define SLOT(px) floordiv((px) + off, size)
    #define XMIN(p) (f->shape->words == 9 ? ((int16_t)(p)->w[1] < (int16_t)(p)->w[3] ? ((int16_t)(p)->w[1] < (int16_t)(p)->w[5] ? ((int16_t)(p)->w[1] < (int16_t)(p)->w[7] ? (int16_t)(p)->w[1] : (int16_t)(p)->w[7]) : ((int16_t)(p)->w[5] < (int16_t)(p)->w[7] ? (int16_t)(p)->w[5] : (int16_t)(p)->w[7])) : ((int16_t)(p)->w[3] < (int16_t)(p)->w[5] ? ((int16_t)(p)->w[3] < (int16_t)(p)->w[7] ? (int16_t)(p)->w[3] : (int16_t)(p)->w[7]) : ((int16_t)(p)->w[5] < (int16_t)(p)->w[7] ? (int16_t)(p)->w[5] : (int16_t)(p)->w[7]))) : (int16_t)(p)->w[4])
    #define XMAX(p) (f->shape->words == 9 ? ((int16_t)(p)->w[1] > (int16_t)(p)->w[3] ? ((int16_t)(p)->w[1] > (int16_t)(p)->w[5] ? ((int16_t)(p)->w[1] > (int16_t)(p)->w[7] ? (int16_t)(p)->w[1] : (int16_t)(p)->w[7]) : ((int16_t)(p)->w[5] > (int16_t)(p)->w[7] ? (int16_t)(p)->w[5] : (int16_t)(p)->w[7])) : ((int16_t)(p)->w[3] > (int16_t)(p)->w[5] ? ((int16_t)(p)->w[3] > (int16_t)(p)->w[7] ? (int16_t)(p)->w[3] : (int16_t)(p)->w[7]) : ((int16_t)(p)->w[5] > (int16_t)(p)->w[7] ? (int16_t)(p)->w[5] : (int16_t)(p)->w[7]))) : (int16_t)(p)->w[4] + size)
    /* slots wholly left of 0 are s <= -1; those wholly right of 384 start at view_right; the limits keep the cells' edges inside -256 and 640 */
    left_stop = 0;
    for (s = -1, blank = 0; s >= floordiv(-256 + off + size - 1, size); s--) {
        int hit = 0;
        for (i = 0; i < (int)ext_tmp.n && !hit; i++) hit = SLOT(XMIN(&ext_tmp.p[i])) <= s && SLOT(XMAX(&ext_tmp.p[i]) - 1) >= s;
        if (hit) { left_stop = s; blank = 0; } else if (++blank > gap) break;
    }
    right_stop = floordiv(384 + off + size - 1, size) - 1;
    for (s = right_stop + 1, blank = 0; s <= floordiv(640 + off, size) - 1; s++) {
        int hit = 0;
        for (i = 0; i < (int)ext_tmp.n && !hit; i++) hit = SLOT(XMIN(&ext_tmp.p[i])) <= s && SLOT(XMAX(&ext_tmp.p[i]) - 1) >= s;
        if (hit) { right_stop = s; blank = 0; } else if (++blank > gap) break;
    }
    *lo = 0;
    *hi = 384;
    for (i = 0; i < (int)ext_tmp.n; i++) {
        const struct packet *p = &ext_tmp.p[i];
        if (left_stop < 0 && SLOT(XMAX(p) - 1) >= left_stop && XMIN(p) < *lo) *lo = XMIN(p);
        if (SLOT(XMIN(p)) <= right_stop && XMAX(p) > *hi) *hi = XMAX(p);
    }
    if (*lo < -256) *lo = -256;
    if (*hi > 640) *hi = 640;
    #undef XMIN
    #undef XMAX
    #undef SLOT
}

static int extent_of(const struct form *f, const struct fixture *x, int *lo, int *hi)
{
    uint32_t preferred, minimum;
    port_wide_map_bytes(f->shape, layer_of(f), x->game_scroll_y, &preferred, &minimum);
    return port_wide_extent(f->shape, layer_of(f), map_of(), minimum, x->game_scroll_y, first_record(f, x->buffer), lo, hi);
}

/* every form, every fixture: the maker's extent is the expectation's; and the fixtures reach into the sides, so that it is not always 0 and 384 */
static unsigned differs_gap1, differs_gap3;     /* fixtures on which another gap rule gives another extent (the negative control) */

static int case_extent_forms(void)
{
    unsigned fi, i, wide = 0, narrow = 0, total = 0;
    ext_mode = 1;
    for (fi = 0; fi < NFORMS; fi++) {
        const struct form *f = &forms[fi];
        for (i = 0; i < fixture_count; i++) {
            const struct fixture *x = fix(f, i);
            int lo, hi, elo, ehi, glo, ghi;
            ext_start = (i * 13 + fi * 7) % 64;                          /* the painted window: where it starts and how long it is */
            ext_len = 20 + (i * 5 + fi) % 40;
            init_pool(f, 0x00808080u | (f->code << 24));
            set_layer(f, x);
            if (extent_of(f, x, &lo, &hi)) { ext_mode = 0; return BAD("%s fixture %u: no extent", f->tag, i); }
            expected_extent(f, x, 2, &elo, &ehi);
            if (lo != elo || hi != ehi) { ext_mode = 0; return BAD("%s fixture %u: extent %d..%d, expected %d..%d", f->tag, i, lo, hi, elo, ehi); }
            if (lo > 0 || hi < 384 || lo < -256 || hi > 640) { ext_mode = 0; return BAD("%s fixture %u: extent %d..%d out of its bounds", f->tag, i, lo, hi); }
            expected_extent(f, x, 1, &glo, &ghi);
            if (glo != lo || ghi != hi) differs_gap1++;
            expected_extent(f, x, 3, &glo, &ghi);
            if (glo != lo || ghi != hi) differs_gap3++;
            total++;
            if (lo < 0 || hi > 384) wide++;
            if (lo > -256 || hi < 640) narrow++;
        }
    }
    ext_mode = 0;
    if (wide * 4 < total) return BAD("only %u of %u extents go past the view", wide, total);
    return narrow * 4 >= total ? 1 : BAD("only %u of %u extents end before the scan limits", narrow, total);
}

/* a map painted by whole columns: tile 5 where painted[col], the blank cell elsewhere, in every row */
static void paint_columns(const struct form *f, const unsigned char *painted)
{
    unsigned bc = f->shape->shift == 4 ? 32 : 16, br = f->shape->shift == 4 ? 16 : 8, col, row, blocks_across = cells_across / bc;
    for (row = 0; row < cells_down; row++)
        for (col = 0; col < cells_across; col++) {
            unsigned index = ((row / br) * blocks_across + col / bc) * bc * br + (row % br) * bc + col % bc;
            wr16(ADDR_MAP + index * 4, painted[col] ? 5u : blank_value);
            wr16(ADDR_MAP + index * 4 + 2, 0);
        }
}

static void columns(unsigned char *painted, int a, int b)
{
    int c;
    for (c = a; c <= b; c++) painted[c] = 1;
}

/* the scan on made-up maps: layer 1's 64 x 32 cells of 16 px with the view at column 32, and the 32 px layer's 32 columns with it at 16 */
static int check_extent(const struct form *f, unsigned scroll_x, const unsigned char *painted, int elo, int ehi, const char *what)
{
    struct fixture x = { scroll_x, 0x00100000, 0, 1, 3, 0, { 0 } };
    int lo, hi;
    init_pool(f, 0x00808080u | (f->code << 24));
    set_layer(f, &x);
    paint_columns(f, painted);
    if (extent_of(f, &x, &lo, &hi)) return BAD("%s: no extent", what);
    return lo == elo && hi == ehi ? 1 : BAD("%s: extent %d..%d, expected %d..%d", what, lo, hi, elo, ehi);
}

static int case_extent_hand(void)
{
    const struct form *f = &forms[1], *g = &forms[2];     /* layer 1 (16 px), the 32 px layer */
    unsigned char m[64];
    int sx;
    for (sx = 0; sx <= 5; sx += 5) {
        char what[96];
        unsigned scroll = 0x200u + (unsigned)sx;
        memset(m, 0, sizeof m); columns(m, 20, 58);
        snprintf(what, sizeof what, "sx %d painted span", sx);
        if (!check_extent(f, scroll, m, -192 - sx, 432 - sx, what)) return 0;
        memset(m, 0, sizeof m); columns(m, 20, 26); columns(m, 29, 58);                   /* a gap of two columns: it goes on */
        snprintf(what, sizeof what, "sx %d left gap of two", sx);
        if (!check_extent(f, scroll, m, -192 - sx, 432 - sx, what)) return 0;
        memset(m, 0, sizeof m); columns(m, 20, 27); columns(m, 31, 58);                   /* a gap of three: it ends there */
        snprintf(what, sizeof what, "sx %d left gap of three", sx);
        if (!check_extent(f, scroll, m, -16 - sx, 432 - sx, what)) return 0;
        memset(m, 0, sizeof m); columns(m, 20, 58); columns(m, 61, 63);                   /* right: two blank columns, then more */
        snprintf(what, sizeof what, "sx %d right gap of two", sx);
        if (!check_extent(f, scroll, m, -192 - sx, 512 - sx, what)) return 0;
        memset(m, 0, sizeof m); columns(m, 20, 58); columns(m, 62, 63);                   /* three: the painting ends at column 58 */
        snprintf(what, sizeof what, "sx %d right gap of three", sx);
        if (!check_extent(f, scroll, m, -192 - sx, 432 - sx, what)) return 0;
        memset(m, 1, sizeof m);                                                           /* everything painted: the scan limits (a cell's edges stay inside) */
        snprintf(what, sizeof what, "sx %d everything painted", sx);
        if (!check_extent(f, scroll, m, sx ? -245 : -256, sx ? 635 : 640, what)) return 0;
        memset(m, 0, sizeof m); columns(m, 32, 55);                                       /* only the view */
        snprintf(what, sizeof what, "sx %d only the view", sx);
        if (!check_extent(f, scroll, m, 0, 384, what)) return 0;                  /* the view's own edges */
        memset(m, 0, sizeof m); columns(m, 32, 56);                                       /* the C's last column is cut by the view's edge */
        snprintf(what, sizeof what, "sx %d the straddling column", sx);
        if (!check_extent(f, scroll, m, 0, 400 - sx, what)) return 0;
    }
    memset(m, 0, sizeof m); columns(m, 10, 30);                                             /* the 32 px layer: view at column 16, 13 columns */
    if (!check_extent(g, 0x200, m, -192, 480, "32 px span")) return 0;
    memset(m, 0, sizeof m); columns(m, 8, 8); columns(m, 11, 30);                           /* two blank columns, then the column at -256 */
    if (!check_extent(g, 0x200, m, -256, 480, "32 px gap of two")) return 0;
    memset(m, 0, sizeof m); columns(m, 8, 8); columns(m, 12, 30);
    if (!check_extent(g, 0x200, m, -128, 480, "32 px gap of three")) return 0;
    memset(m, 1, sizeof m);
    return check_extent(g, 0x200, m, -256, 640, "32 px everything painted");
}

/* a negative control: another gap rule (one blank column, or three) gives other extents on the maps of case_extent_forms, so that comparison
 * can tell the rules apart */
static int case_extent_negative(void)
{
    if (!differs_gap1) return BAD("a gap rule of one gave the same extents everywhere");
    if (!differs_gap3) return BAD("a gap rule of three gave the same extents everywhere");
    return 1;
}

/* ---- case: the visible range ---- */
static int case_range(void)
{
    unsigned fi;
    int sx;
    for (fi = 0; fi < NFORMS; fi++) {
        const struct form *f = &forms[fi];
        int size = 1 << f->shape->shift;
        if (KIND(f) == PORT_WIDE_SKEWED || KIND(f) == PORT_WIDE_SCROLLC) continue;   /* their sides are per row or per half (case_sides) */
        for (sx = 0; sx < size; sx++) {
            struct fixture x = { (unsigned)sx + 0x40u, 0x00100000, 0, 1, 3, 0, { 0 } };
            int kl, kr;
            if (KIND(f) == PORT_WIDE_SPRITES) {              /* x12 = start >> 16 when the camera is at the range's start */
                memset(&x, 0, sizeof x);
                x.scroll_y = 0x00100000; x.blank = 1; x.ot_index = 3;
                x.v[0] = x.v[1] = (int32_t)((sx + 0x40) << 16);
                x.v[4] = 1;
            }
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
    for (fi = 0; fi < NFORMS; fi++) {
        const struct form *f = &forms[fi];
        struct fixture x = *fix(f, 0);
        uint32_t a = f->pool + 1 * f->count * record_words(f) * 4;   /* pool[1][0] */
        /* the packet words the C never writes: whole words, or the half of a word that the C leaves */
        int skewed = f->shape->words == 9;                           /* the quad records: whole word 0, halves of words 6 and 8 */
        unsigned words[4] = { 1, 2, 3, 6 }, count = f->shape->words == 7 ? 4 : 3, i, j;
        uint32_t masks[4] = { 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu };
        int k0, k1;
        if (skewed) { words[0] = 0; words[1] = 6; words[2] = 8; masks[1] = masks[2] = 0xffff0000u; count = 3; }
        x.buffer = 1;
        x.ot_index = 12;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, &x);
        for (j = 0; j < (unsigned)f->shape->words; j++) wr32(a + 4 + 4 * j, 0x5a000000u + (j + 1) * 0x010101u);   /* packet word j of pool[1][0] */
        k0 = skewed ? C_LO(f) : f->density ? C_LO(f) - 13 : C_LO(f) - 3;      /* a sparse map needs more columns to have a cell */
        k1 = skewed ? C_LO(f) + 8 : f->density ? C_HI(f) + 13 : C_LO(f);
        run_maker(f, &x, k0, k1, &from_maker);
        if (!from_maker.n) return BAD("%s: nothing made", f->tag);
        for (i = 0; i < from_maker.n; i++) {
            for (j = 0; j < count; j++) {
                uint32_t want = rd32a(a + 4 + 4 * words[j]);
                if ((from_maker.p[i].w[words[j]] & masks[j]) != (want & masks[j]))
                    return BAD("%s: packet %u word %u is %08x, the first record has %08x", f->tag, i, words[j], from_maker.p[i].w[words[j]], want);
            }
            if (from_maker.p[i].entry != (int)entry_of(f, &x)) return BAD("%s: packet %u for entry %d", f->tag, i, from_maker.p[i].entry);
        }
        /* the buffer 0 record is not the template when the draw buffer is 1 */
        x.buffer = 0;
        run_maker(f, &x, k0, k1, &from_maker);
        if (from_maker.n && from_maker.p[0].w[words[0]] == rd32a(a + 4 + 4 * words[0]))
            return BAD("%s: took the other buffer's record", f->tag);
        /* a pool the init has not filled (tag length 0) gives nothing */
        wr32(f->pool, 0);
        x.buffer = 0;
        if (run_maker(f, &x, k0, k1, &from_maker) != 0) return BAD("%s: made packets from an empty pool", f->tag);
    }
    return 1;
}

/* ---- case: a cell past the map's bytes is blank; a refused packet stops the maker ---- */
static int case_limits(void)
{
    const struct form *f = &forms[1];
    struct fixture x = { 0x0100, 0x00100000, 0, 1, 4, 0, { 0 } };
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

static unsigned refused_seen;
static int refuse_third(int entry, const uint32_t *words, unsigned len)
{
    (void)entry; (void)words; (void)len;
    return ++refused_seen >= 3 ? -1 : 0;
}

static int case_emit_refused(void)
{
    unsigned fi;
    for (fi = 1; fi < NFORMS; fi += 3) {       /* the 16 px layer 1 and the skewed form */
        const struct form *f = &forms[fi];
        const struct fixture *x = fix(f, 20);
        unsigned made, i;
        for (i = 20; i < fixture_count; i++) {                 /* a fixture with at least 3 cells in the C's columns */
            init_pool(f, 0x00808080u | (f->code << 24));
            set_layer(f, fix(f, i));
            if (run_maker(f, fix(f, i), C_LO(f), C_HI(f), &from_maker) >= 3) { x = fix(f, i); break; }
        }
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, x);
        refused_seen = 0;
        made = port_wide_columns(f->shape, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y, first_record(f, x->buffer), C_LO(f), C_HI(f), refuse_third);
        if (made != 2) return BAD("%s: expected 2 packets before the refusal, got %u", f->tag, made);
    }
    return 1;
}

/* ---- negative controls: the comparison must fail for a wrong column and for a wrong word ---- */
static int case_negative_control(const struct form *f)
{
    const struct fixture *x = NULL;
    unsigned d, k, i;
    static const unsigned flat_words[3] = { 0, 4, 5 }, skew_words[4] = { 1, 2, 4, 5 };
    const unsigned *words = f->shape->words == 9 ? skew_words : flat_words;
    const unsigned nwords = f->shape->words == 9 ? 4 : 3;
    for (i = 10; i < fixture_count && !x; i++) {          /* the first fixture in which the C draws something worth comparing */
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, fix(f, i));
        f->game_c();
        harvest(f, entry_of(f, fix(f, i)), &from_c);
        if (from_c.n >= (f->cap && f->cap < 20 ? 4u : 20u)) x = fix(f, i);   /* gouki's pool holds 13 */
    }
    if (!x) return BAD("%s: the C never drew enough to test", f->tag);
    /* a column off by one */
    run_maker(f, x, C_LO(f) + 1, C_HI(f) + 1, &from_maker);
    if (!compare(&from_c, &from_maker, (unsigned)f->shape->words)) return BAD("%s: the shifted columns were accepted", f->tag);
    /* the C's own columns but one packet with a wrong word, each word that varies */
    for (k = 0; k < nwords; k++) {
        run_maker(f, x, C_LO(f), C_HI(f), &from_maker);
        from_maker.p[from_maker.n / 2].w[words[k]] ^= 0x10u;
        d = compare(&from_c, &from_maker, (unsigned)f->shape->words);
        if (d != from_maker.n / 2 + 1) return BAD("%s: a changed word %u was not found at its packet (%u)", f->tag, words[k], d);
    }
    /* a wrong shape: the 32 px maker on a 16 px layer, the plain maker on the sprite and skewed layers */
    if (KIND(f) != PORT_WIDE_FLAT || f->shape->shift == 4) {
        struct port_wide_shape wrong = *f->shape;
        if (KIND(f) == PORT_WIDE_FLAT) wrong.shift = 5; else wrong.kind = PORT_WIDE_FLAT;
        sink = &from_maker;
        from_maker.n = 0;
        port_wide_columns(&wrong, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y, first_record(f, x->buffer), C_LO(f), C_HI(f), sink_emit);
        if (!compare(&from_c, &from_maker, (unsigned)f->shape->words)) return BAD("%s: a wrong shape was accepted", f->tag);
    }
    /* a wrong game scroll_y moves the rows */
    if (KIND(f) != PORT_WIDE_SKEWED && KIND(f) != PORT_WIDE_QUADS) {      /* these two never read game_state.scroll_y */
        sink = &from_maker;
        from_maker.n = 0;
        port_wide_columns(f->shape, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y + 16, first_record(f, x->buffer), C_LO(f), C_HI(f), sink_emit);
        if (!compare(&from_c, &from_maker, (unsigned)f->shape->words)) return BAD("%s: a wrong scroll_y was accepted", f->tag);
    }
    return 1;
}

/* the blank with the high bit: layer 0 draws every cell, the other two skip the blank cells */
static int case_blank_forms(void)
{
    struct fixture x = { 0x0000, 0x00100000, 0, 0x8123, 4, 0, { 0 } };
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
    /* the other plain forms: the half-speed 16 px layer compares as layer 0 does, the 32 px ones skip the blank cells */
    for (fi = 5; fi < 9; fi++) {
        const struct form *f = &forms[fi];
        unsigned total = (unsigned)(f->shape->rows * f->shape->cols), n;
        init_pool(f, 0x00808080u | (f->code << 24));
        set_layer(f, &x);
        f->game_c();
        harvest(f, 4, &from_c);
        n = from_c.n;
        if (f->shape->unsigned_blank ? n != total : (n >= total || n == 0))
            return BAD("%s drew %u of %u cells", f->tag, n, total);
    }
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
    static const char *const names[NFORMS] = { "draw_tile_layer_0_16px_slot06_00", "draw_tile_layer_1_16px_slot06_00", "draw_tilemap_layer_32px_9x13_slot06_00",
                                               "draw_tile_layer_sprites_with_partial_row_slot06_04", "draw_skewed_tile_layer_quads_slot06_04",
                                               "draw_tilemap_layer_32px_7x13_slot06_01", "draw_tile_map_layer_16px_half_scroll_slot06_0d",
                                               "draw_tile_map_layer_32px_slot06_0d", "draw_tile_map_layer_32px_slot06_02",
                                               "draw_scrolling_tile_map_layer_slot06_08", "draw_scrolling_tile_map_layer_slot06_12",
                                               "draw_tile_map_layer_16px_slot06_02", "draw_tile_map_layer_16px_slot03_slot06_03", "draw_tile_map_layer_16px_slot06_09",
                                               "draw_tilemap_layer_16px_12x25_slot06_01", "draw_tile_layer_sprites_saving_pos_slot06_03",
                                               "draw_tile_map_layer_polys_slot06_03", "draw_perspective_skewed_tile_map_slot06_0e",
                                               "draw_tile_map_16px_tiles_slot06_0e", "draw_tile_map_layer_sprites_32px_slot06_0e",
                                               "draw_tile_map_layer_sprites_interpolated_slot06_06" };
    unsigned fi, s, j;
    for (fi = 0; fi < NFORMS; fi++) {
        const struct form *f = &forms[fi];
        const char *name = names[fi];
        void (*run)(void) = override_of(name);
        const struct fixture *x = fix(f, 12);
        unsigned pick;
        if (!run) return BAD("%s: no override", name);
        for (pick = 12; pick < fixture_count; pick++) {        /* the first fixture that has side tiles */
            x = fix(f, pick);
            init_pool(f, 0x00808080u | (f->code << 24));
            set_layer(f, x);
            sink = &from_maker;
            from_maker.n = 0;
            port_wide_sides(f->shape, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y, first_record(f, x->buffer), sink_emit);
            if (from_maker.n) break;
        }
        for (s = 0; s < 4; s++) {
            unsigned d;
            port_settings.widescreen = settings[s][0];
            port_settings.arcade_video = settings[s][1];
            init_pool(f, 0x00808080u | (f->code << 24));
            set_layer(f, x);
            queued_count = 0;
            view_count = 0;
            run();
            harvest(f, entry_of(f, x), &from_c);
            if (from_c.n == 0) return BAD("%s: the game's C was not run (settings %u)", name, s);
            if (!settings[s][2]) {
                if (queued_count) return BAD("%s: %u packets queued with widescreen %d, arcade_video %d", name, queued_count, settings[s][0], settings[s][1]);
                if (view_count) return BAD("%s: a plane view with widescreen %d, arcade_video %d", name, settings[s][0], settings[s][1]);
                continue;
            }
            {   /* the plane's view: its pool of both buffers, the entry of its side cells and the extent that the maker finds */
                int lo, hi, elo, ehi;
                if (view_count != 1) return BAD("%s: %u plane views, expected 1", name, view_count);
                extent_of(f, x, &lo, &hi);
                expected_extent(f, x, 2, &elo, &ehi);
                if (view_calls[0].layer != f->layer) return BAD("%s: plane view for the record %08x, expected %08x", name, view_calls[0].layer, f->layer);
                if (view_calls[0].lo != f->pool || view_calls[0].hi != f->pool + 2 * f->count * record_words(f) * 4 ||
                    view_calls[0].entry != (int)entry_of(f, x) || view_calls[0].art_lo != lo || view_calls[0].art_hi != hi || lo != elo || hi != ehi)
                    return BAD("%s: plane view %08x..%08x entry %d art %d..%d, expected %08x..%08x entry %u art %d..%d", name, view_calls[0].lo, view_calls[0].hi,
                               view_calls[0].entry, view_calls[0].art_lo, view_calls[0].art_hi, f->pool, f->pool + 2 * f->count * record_words(f) * 4,
                               entry_of(f, x), elo, ehi);
            }
            sink = &from_maker;
            from_maker.n = 0;
            port_wide_sides(f->shape, layer_of(f), map_of(), MAP_BYTES, x->game_scroll_y, first_record(f, x->buffer), sink_emit);
            if (from_maker.n == 0) return BAD("%s: the fixture has no side tiles", name);
            if (queued_count != from_maker.n) return BAD("%s: %u packets queued, expected %u", name, queued_count, from_maker.n);
            for (j = 0; j < queued_count; j++) {
                if (queued[j].entry != from_maker.p[j].entry || queued[j].len != (unsigned)f->shape->words ||
                    memcmp(queued[j].w, from_maker.p[j].w, queued[j].len * sizeof(uint32_t)))
                    return BAD("%s: queued packet %u differs from the maker's", name, j);
            }
            (void)d;
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
        const char *form = "?";
        unsigned j;
        for (j = 0; j < NFORMS; j++) if (forms[j].shape == c->shape) form = forms[j].tag;
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
    for (fi = 0; fi < NFORMS; fi++) {
        snprintf(name, sizeof name, "maker_reproduces_c_%s", forms[fi].tag);
        packets = 0;
        report(name, case_matches_c(&forms[fi], &packets) ? NULL : why);
        if (!failures) printf("     (%u fixtures, %u packets of the game's C compared)\n", fixture_count, packets);
    }
    for (fi = 0; fi < NFORMS; fi++) {
        snprintf(name, sizeof name, "expectation_reproduces_c_%s", forms[fi].tag);
        report(name, case_expect_in_c_range(&forms[fi]) ? NULL : why);
    }
    for (fi = 0; fi < NFORMS; fi++) {
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
    for (fi = 0; fi < NFORMS; fi++) {
        snprintf(name, sizeof name, "negative_control_%s", forms[fi].tag);
        report(name, case_negative_control(&forms[fi]) ? NULL : why);
    }
    report("extent_hand_made_maps", case_extent_hand() ? NULL : why);
    report("extent_every_form", case_extent_forms() ? NULL : why);
    report("extent_negative_control", case_extent_negative() ? NULL : why);
    report("override_wiring", case_override_wiring() ? NULL : why);
    report("override_without_c", case_override_without_c() ? NULL : why);
    printf("%d cases, %d failed\n", cases, failures);
    return failures ? 1 : 0;
}
