/* Widescreen's side tiles (port/widescreen.md, "The side tiles"): the stages' tile layers drawn past the columns that the game's own C
 * draws. Nine forms of layer function are shared by several stages (the C is the same in every copy of a form; only the pool differs):
 * the plain tile grids draw_tile_layer_0_16px, draw_tile_layer_1_16px, draw_tilemap_layer_32px_9x13 and _7x13,
 * draw_tile_map_layer_16px_half_scroll and draw_tile_map_layer_32px (half speed, and gouki's copy), and the second layer's
 * draw_tile_layer_sprites_with_partial_row and draw_skewed_tile_layer_quads. Each copy gets an override that runs the game's own C and
 * then, with widescreen on, makes the packets of the columns beside the C's that the wide view can show, with the C's arithmetic for
 * each column and row, and queues them for the layer's entry of the ordering table (port_gpu_queue_packet). With widescreen off it only
 * runs the C. The maker (port_wide_columns) is pure and is compared with the C in port/tools/wide_stage_test.c. The per-object grids
 * (draw_object_tile_grid_sprites and its kin, which take an Object) are not here: they draw one object's cells, not a map. */
#include "port.h"
#include "gpu.h"
#include "settings.h"
#include "wide_stage.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* The game's memory that the C reads (ps1/src/symbols.ld, types.fields): game_state + 0x92 is game_state.scroll_y (u16),
 * draw_buffer_index an int. A layer record (struct Chan, 0x90 bytes): +0x12 u16 scroll_x, +0x14 s32 scroll_y, +0x1e u16 field_1e (the
 * blank cell), +0x50 u16 *map_data, +0x58 u32 map_width, +0x5c u32 map_height, +0x8a u8 ot_index. */
#define GAME_STATE_SCROLL_Y 0x8019019au
#define DRAW_BUFFER_INDEX   0x801a27d0u
#define CHAN_SCROLL_X       0x12u
#define CHAN_SCROLL_Y       0x14u
#define CHAN_BLANK          0x1eu
#define CHAN_MAP_DATA       0x50u
#define CHAN_MAP_WIDTH      0x58u
#define CHAN_MAP_HEIGHT     0x5cu
#define CHAN_OT_INDEX       0x8au
/* The second layer (main_layer, background_layers[1]) as the two other forms' C reads it (Slot06Layer, Slot06Draw in types.fields). */
#define SL_BOUND_X_A        0x08u   /* s32 at 0x08 and at 0x10 (bound_x_a, bound_x_b read as words); Slot06Draw: start_x_fixed, end_x_fixed */
#define SL_BOUND_X_B        0x10u
#define SL_BASE_X_FIXED     0x14u   /* Slot06Draw.base_x_fixed, s32 */
#define SL_RANGE_START      0x1cu   /* Slot06Draw.range_start s16; Slot06Layer.field_1c u16 */
#define SL_SIGNED_PARAM     0x3eu   /* Slot06Layer.signed_param s16 */
#define SL_FIELD_4C         0x4cu   /* u16 / s16: the camera's y */
#define SL_RECT_RIGHT       0x4eu   /* Slot06Layer.rect_right u16; Slot06Draw.range_end s16 */

/* What the anchored wide view can show, in the game's screen x: the canvas is 64 pixels wider on each side and the view slides by up to 64. */
#define VIEW_LEFT  (-128)
#define VIEW_RIGHT 512

/* The packet's words after the tag, in the order of the record (Slot06Tile, Slot06TileW): mode[0] (0xe1000000 + the tile's texture
 * page bits), mode[1], the sprite's tag (a NOP command), its colour and code, its x and y, its u, v and clut, and for the wide
 * record its w and h. */
#define WORD_MODE 0
#define WORD_XY   4
#define WORD_UV   5

const struct port_wide_shape port_wide_shape_L0   = { .shift = 4, .rows = 16, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT };
const struct port_wide_shape port_wide_shape_L1   = { .shift = 4, .rows = 16, .cols = 25, .words = 6, .kind = PORT_WIDE_FLAT };
const struct port_wide_shape port_wide_shape_T32  = { .shift = 5, .rows = 9, .cols = 13, .words = 7, .row_inclusive = 1, .kind = PORT_WIDE_FLAT };
const struct port_wide_shape port_wide_shape_SPR  = { .shift = 4, .cols = 25, .words = 6, .kind = PORT_WIDE_SPRITES };    /* the row count comes from the camera */
const struct port_wide_shape port_wide_shape_SKEW = { .shift = 4, .cols = 44, .words = 9, .kind = PORT_WIDE_SKEWED, .k_first = -11 };   /* a POLY_FT4 a cell: 9 words */
/* the same arithmetic with other counts or the half-speed scroll */
const struct port_wide_shape port_wide_shape_T32_7 = { .shift = 5, .rows = 7, .cols = 13, .words = 7, .row_inclusive = 1, .kind = PORT_WIDE_FLAT };   /* draw_tilemap_layer_32px_7x13 */
const struct port_wide_shape port_wide_shape_T16H  = { .shift = 4, .rows = 16, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT, .half_scroll = 1 };   /* draw_tile_map_layer_16px_half_scroll: layer 0's compare */
const struct port_wide_shape port_wide_shape_T32H  = { .shift = 5, .rows = 9, .cols = 13, .words = 7, .row_inclusive = 1, .kind = PORT_WIDE_FLAT, .half_scroll = 1 };   /* draw_tile_map_layer_32px (sakura, vega) */
const struct port_wide_shape port_wide_shape_T32G  = { .shift = 5, .rows = 9, .cols = 13, .words = 7, .row_inclusive = 1, .kind = PORT_WIDE_FLAT };   /* draw_tile_map_layer_32px of gouki: full speed, stops at 13 records */
/* the first layer's 16 px grids of other row counts, layer 0's compare: ken 12 rows, rose 10, and gouki's 13 and nash's 16, which also skip the next layer's blank (their C stops at a record count) */
const struct port_wide_shape port_wide_shape_L0K = { .shift = 4, .rows = 12, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT };
const struct port_wide_shape port_wide_shape_L0R = { .shift = 4, .rows = 10, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT };
const struct port_wide_shape port_wide_shape_L0G = { .shift = 4, .rows = 13, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT, .second_blank = 1 };
const struct port_wide_shape port_wide_shape_L0N = { .shift = 4, .rows = 16, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT, .second_blank = 1 };
/* nash's draw_tile_layer_sprites_saving_pos: layer 1's grid with a row count from the camera */
const struct port_wide_shape port_wide_shape_SAVE = { .shift = 4, .cols = 25, .words = 6, .kind = PORT_WIDE_FLAT, .camera_rows = 1 };
/* nash's draw_tile_map_layer_polys: 24 unskewed quads a row, in entry 14, from the map 0x800 halfwords on */
const struct port_wide_shape port_wide_shape_POLY = { .shift = 4, .cols = 24, .words = 9, .kind = PORT_WIDE_QUADS, .map_offset = 0x800, .fixed_entry = 15 };
/* rolento's draw_perspective_skewed_tile_map is the skewed form (its pool pointer moves for every cell, which the packets do not show);
 * its two grids have their own rows and lines; sodom's draw_tile_map_layer_sprites_interpolated is the partial-row form; the two
 * draw_scrolling_tile_map_layer are the second layer's scrolling rows (birdie's and chunli_alt's, which differ) */
const struct port_wide_shape port_wide_shape_SKEWP = { .shift = 4, .cols = 44, .words = 9, .kind = PORT_WIDE_SKEWED, .k_first = -11 };
const struct port_wide_shape port_wide_shape_R16  = { .shift = 4, .rows = 16, .cols = 25, .words = 6, .unsigned_blank = 1, .kind = PORT_WIDE_FLAT, .variant = PORT_WIDE_V_ROLENTO16 };
const struct port_wide_shape port_wide_shape_R32  = { .shift = 5, .rows = 9, .cols = 13, .words = 7, .kind = PORT_WIDE_FLAT, .variant = PORT_WIDE_V_ROLENTO32 };
const struct port_wide_shape port_wide_shape_SPRI = { .shift = 4, .cols = 25, .words = 6, .kind = PORT_WIDE_SPRITES };
const struct port_wide_shape port_wide_shape_SCRB = { .shift = 4, .cols = 25, .words = 6, .kind = PORT_WIDE_SPRITES, .variant = PORT_WIDE_V_BIRDIE };
const struct port_wide_shape port_wide_shape_SCRC = { .shift = 4, .rows = 16, .cols = 25, .words = 6, .kind = PORT_WIDE_SCROLLC };

/* The words of a POLY_FT4 record after its tag (Poly28): colour and code, then x y / u v clut / x y / u v tpage / x y / u v / x y / u v
 * for the four corners. */
#define PW_XY0 1
#define PW_UV0 2
#define PW_XY1 3
#define PW_UV1 4
#define PW_XY2 5
#define PW_UV2 6
#define PW_XY3 7
#define PW_UV3 8

static uint16_t rd16(const uint8_t *p, unsigned off) { uint16_t v; memcpy(&v, p + off, sizeof v); return v; }
static uint32_t rd32(const uint8_t *p, unsigned off) { uint32_t v; memcpy(&v, p + off, sizeof v); return v; }

/* The sprite form's x position: x12 = (q + start_x_fixed) >> 16 with q = (end - start) * (camera y - range_start) / (range_end - range_start),
 * all in 32-bit ints. 0 in *ok when the range is empty (the C would divide by zero). */
static int sprites_x12(const uint8_t *layer, int *ok)
{
    uint32_t start = rd32(layer, SL_BOUND_X_A), end = rd32(layer, SL_BOUND_X_B);
    int32_t dy = (int16_t)rd16(layer, SL_FIELD_4C) - (int16_t)rd16(layer, SL_RANGE_START);
    int32_t span = (int16_t)rd16(layer, SL_RECT_RIGHT) - (int16_t)rd16(layer, SL_RANGE_START);
    int32_t q;
    *ok = span != 0;
    if (!span) return 0;
    q = (int32_t)((uint32_t)(int32_t)(end - start) * (uint32_t)dy);
    q = (span == -1) ? (int32_t)(0u - (uint32_t)q) : q / span;
    return ((int32_t)((uint32_t)q + start)) >> 16;
}

/* The columns beside the C's whose cell reaches into the view, for a horizontal offset sx (the C's scroll within a cell). */
static void range_for_sx(const struct port_wide_shape *shape, int sx, int *k_left, int *k_right)
{
    int size = 1 << shape->shift;
    int kl = shape->k_first, kr = shape->k_first + shape->cols;
    while ((kl - 1) * size - sx + size > VIEW_LEFT && kl > shape->k_first - 64) kl--;   /* the cell kl - 1 reaches into the view */
    while (kr * size - sx < VIEW_RIGHT && kr < shape->k_first + shape->cols + 64) kr++;
    *k_left = kl;
    *k_right = kr;
}

/* PORT_WIDE_SCROLLC: the horizontal offset of the first eight rows (the global g) and of the last eight (the layer's own scroll_x). */
static int scrollc_sx(const uint8_t *layer, unsigned game, int half)
{
    return half ? (int)(rd16(layer, CHAN_SCROLL_X) & 0xf) : (int)((int16_t)(game >> 16) & 0xf);
}

void port_wide_side_range(const struct port_wide_shape *shape, const uint8_t *layer, int *k_left, int *k_right)
{
    int ok = 1, size = 1 << shape->shift;
    int sx = shape->kind == PORT_WIDE_SPRITES ? (sprites_x12(layer, &ok) & (size - 1)) : (int)(rd16(layer, CHAN_SCROLL_X) & (unsigned)(size - 1));
    range_for_sx(shape, sx, k_left, k_right);
}

/* The map's 16 px lines: blocks of 32 x 16 cells, two halfwords a cell. */
static uint32_t line16(uint32_t width, int row)
{
    return ((uint32_t)((uint16_t)row >> 4)) * ((width >> 9) << 10) + (((unsigned)row & 0xf) << 6);
}

static uint32_t cell16(unsigned cc) { return ((cc >> 5) << 10) + ((cc & 0x1f) << 1); }

/* The cell at halfword index `cell` of the map: 0 when it lies past the map's bytes. */
static int read_cell(const uint8_t *map, uint32_t map_bytes, uint32_t cell, uint16_t *tile, uint16_t *clut)
{
    if ((uint64_t)cell * 2 + 4 > map_bytes) return 0;
    *tile = rd16(map, cell * 2);
    *clut = rd16(map, cell * 2 + 2);
    return 1;
}

#define MAX_ROWS 64   /* a bound on the rows made up from a camera (the C's own count is 15 or fewer) */

/* The plain grids (struct Chan): the C's arithmetic step by step; the three first forms are draw_tile_layer_0_16px, _1_16px and
 * draw_tilemap_layer_32px_9x13, the others differ in counts, the scroll, a second blank, the row count or (rolento) the rows and lines. */
static unsigned flat_columns(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                             unsigned game_scroll_y, const uint32_t *record, int k0, int k1, port_wide_emit emit)
{
    const int size = 1 << shape->shift;
    const uint32_t width = rd32(layer, CHAN_MAP_WIDTH), height = rd32(layer, CHAN_MAP_HEIGHT);
    const unsigned scroll_x = rd16(layer, CHAN_SCROLL_X);
    const int blank = (int16_t)rd16(layer, CHAN_BLANK);
    const int blank2 = shape->second_blank ? (int16_t)rd16(layer, 0x90u + CHAN_BLANK) : 0;   /* the next layer's blank */
    const int entry = layer[CHAN_OT_INDEX];
    const unsigned len = shape->words;
    const int rolento = shape->variant == PORT_WIDE_V_ROLENTO16 || shape->variant == PORT_WIDE_V_ROLENTO32;
    uint32_t x_before, pos_neg;
    int pos, y, sx, sy, rows, nrows, mask, row0, i, k;
    unsigned col;
    uint32_t words[8];
    unsigned made = 0;

    if (!width || (!height && !rolento) || !map || (record[0] >> 24) != len) return 0;
    x_before = rd32(layer, CHAN_SCROLL_Y) - 0x100000u;                 /* x = scroll_y - 0x100000 */
    pos_neg = 0u - x_before;                                           /* pos = -x */
    pos = (int)pos_neg;
    y = (int16_t)(uint16_t)((pos >> 16) - (shape->half_scroll ? ((int16_t)game_scroll_y >> 1) : (int)(game_scroll_y & 0xffffu)));   /* t = (pos >> 16) - game_state.scroll_y (or its half), 16 bits; y = t */
    sx = (int)(scroll_x & (unsigned)(size - 1));
    sy = y & (size - 1);
    rows = (int16_t)(height >> shape->shift);
    mask = (uint16_t)((width >> shape->shift) - 1);
    col = (((uint32_t)(int32_t)(int16_t)scroll_x) & (width - 1)) >> shape->shift;
    row0 = y >= 0 ? y >> shape->shift : (y - (size - 1)) / size;
    col &= 0xffffu;                                                    /* cc is 16 bits */
    nrows = shape->rows;
    if (shape->camera_rows) {                                          /* nr = ((u32)(field_4c - ((pos >> 16) & -16)) >> 4) + 1 */
        nrows = (int16_t)(((uint32_t)((int16_t)rd16(layer, SL_FIELD_4C) - (y & -16)) >> 4) + 1);
        if (nrows > MAX_ROWS) nrows = MAX_ROWS;
    }
    if (rolento) row0 = (y & 0x1f0) / size;                            /* row = (y & 0x1f0) / 16 (or 32), 16 bits, no bounds */

    for (i = 0; i < nrows; i++) {
        uint32_t line;
        if (rolento) {
            unsigned row = (uint16_t)(row0 + i);
            if (shape->variant == PORT_WIDE_V_ROLENTO16) {
                line = ((row >> 15) & 1) + ((row & 0xf) << 6);
            } else {                                                   /* the half of the map by the layer's scroll_y (high half) and bit 3 of the row */
                unsigned f16 = rd16(layer, 0x16), half;
                if (f16 < 0x100) half = (row & 8) ? 1 : 0;
                else if (f16 < 0x600) half = 1;
                else half = (row & 8) ? 0 : 1;
                line = ((row >> 12) & 0xf) + half * 0x200 + ((row & 7) << 5);
            }
        } else {
            int row = (int16_t)(row0 + i);
            if (row < 0 || (shape->row_inclusive ? rows < row : row >= rows)) continue;
            /* the map is blocks of 32 x 16 cells (16 px form) or 16 x 8 cells (32 px form), two halfwords a cell (tile, clut) */
            if (shape->shift == 4)
                line = ((uint32_t)((uint16_t)row >> 4)) * ((width >> 9) << 10) + (((unsigned)row & 0xf) << 6);
            else
                line = ((uint32_t)((uint16_t)row >> 3)) * ((width >> 9) << 8) + (((unsigned)row & 7) << 5);
        }
        for (k = k0; k < k1; k++) {
            unsigned cc = (unsigned)(mask & (int)(col + (unsigned)k)) & 0xffffu;    /* mask & (cc + 1), k times; k < 0 backwards */
            uint32_t cell = line + (shape->shift == 4 ? ((cc >> 5) << 10) + ((cc & 0x1f) << 1) : ((cc >> 4) << 8) + ((cc & 0xf) << 1));
            uint16_t raw;
            int tile, tile_s;
            if ((uint64_t)cell * 2 + 4 > map_bytes) continue;
            raw = rd16(map, cell * 2);
            tile_s = (int16_t)raw;
            tile = shape->unsigned_blank ? (int)raw : tile_s;
            if (tile == blank || (shape->second_blank && tile == blank2)) continue;
            memcpy(words, record + 1, len * sizeof words[0]);
            words[WORD_MODE] = 0xe1000000u + (uint32_t)(tile_s >> 10);
            words[WORD_XY] = (uint32_t)(uint16_t)(int16_t)(k * size - sx) | ((uint32_t)(uint16_t)(int16_t)(i * size - sy) << 16);
            words[WORD_UV] = (uint32_t)(uint8_t)((tile_s * 8) & -8) | ((uint32_t)(uint8_t)((tile_s >> 2) & -8) << 8) |
                             ((uint32_t)rd16(map, cell * 2 + 2) << 16);
            if (emit(entry, words, len)) return made;
            made++;
        }
    }
    return made;
}

/* One row of 16 px sprite packets of the second layer's forms (Slot06Draw): the cells k0..k1 of map row `row` at screen y `yy`. */
struct row16 {
    const struct port_wide_shape *shape;
    const uint8_t *map;
    uint32_t map_bytes, width;
    const uint32_t *record;
    port_wide_emit emit;
    int blank, mask, entry;
    unsigned made;
    int stopped;
};

static void row16_cells(struct row16 *r, int row, int yy, int sx, unsigned col, int k0, int k1)
{
    const uint32_t line = line16(r->width, row);
    const unsigned len = r->shape->words;
    uint32_t words[8];
    int k;
    for (k = k0; k < k1; k++) {
        unsigned cc = (unsigned)(r->mask & (int)(col + (unsigned)k)) & 0xffffu;
        uint16_t raw, clut;
        int tile;
        if (!read_cell(r->map, r->map_bytes, line + cell16(cc), &raw, &clut)) continue;
        tile = (int16_t)raw;
        if (tile == r->blank) continue;
        memcpy(words, r->record + 1, len * sizeof words[0]);
        words[WORD_MODE] = 0xe1000000u + (uint32_t)(tile >> 10);
        words[WORD_XY] = (uint32_t)(uint16_t)(int16_t)(k * 16 - sx) | ((uint32_t)(uint16_t)yy << 16);
        words[WORD_UV] = (uint32_t)(uint8_t)((tile * 8) & -8) | ((uint32_t)(uint8_t)((tile >> 2) & -8) << 8) | ((uint32_t)clut << 16);
        if (r->emit(r->entry, words, len)) { r->stopped = 1; return; }
        r->made++;
    }
}

static void row16_init(struct row16 *r, const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                       const uint32_t *record, port_wide_emit emit)
{
    r->shape = shape;
    r->map = map;
    r->map_bytes = map_bytes;
    r->width = rd32(layer, CHAN_MAP_WIDTH);
    r->record = record;
    r->emit = emit;
    r->blank = (int16_t)rd16(layer, CHAN_BLANK);
    r->mask = (uint16_t)((r->width >> 4) - 1);
    r->entry = layer[CHAN_OT_INDEX];
    r->made = 0;
    r->stopped = 0;
}

/* draw_tile_layer_sprites_with_partial_row (and its relatives): n rows of 25 cells from the camera, then one partial row when the camera's
 * y is not a multiple of 16. The C's arithmetic, in its order; the layer is Slot06Draw. Variant PORT_WIDE_V_BIRDIE is birdie's
 * draw_scrolling_tile_map_layer: the same row loop with the row counter at 4, the loop counter from the position and the position moved
 * by 0x40. */
static unsigned sprites_columns(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                                unsigned game, const uint32_t *record, int k0, int k1, port_wide_emit emit)
{
    struct row16 r;
    const uint32_t width = rd32(layer, CHAN_MAP_WIDTH), height = rd32(layer, CHAN_MAP_HEIGHT);
    const int f4c = (int16_t)rd16(layer, SL_FIELD_4C);
    int ok, x12, sx, sy, y, row, rows, n, i, i0;
    unsigned col;

    x12 = sprites_x12(layer, &ok);
    if (!ok || !width || !height || !map || (record[0] >> 24) != (unsigned)shape->words) return 0;
    if (shape->variant == PORT_WIDE_V_BIRDIE) {
        int32_t x = (int32_t)rd32(layer, SL_BASE_X_FIXED);
        uint16_t t = (uint16_t)((x >> 16) + (int)(game & 0xffffu));                    /* t = (x >> 16) + game_state.scroll_y */
        int32_t xm = (int32_t)(((uint32_t)x & 0xffffu) | ((uint32_t)t << 16));         /* x &= 0xffff; x |= t << 16 */
        int32_t pos = (int32_t)(0x400000u - ((uint32_t)xm - 0x100000u));                /* p2 = x - 0x100000; pos = 0x400000 - p2 */
        y = pos >> 16;
        sy = y & 0xf;
        n = (int16_t)((uint32_t)(f4c - ((y - 0x40) & -16)) >> 4);
        i0 = ((xm >> 16) + 0x3f) / 16;
        row = 4;
    } else {
        int32_t pos = (int32_t)(0u - (rd32(layer, SL_BASE_X_FIXED) - 0x100000u));      /* x = base_x_fixed - 0x100000; pos = -x */
        y = (int16_t)(uint16_t)((pos >> 16) - (int)(game & 0xffffu));                  /* t = (pos >> 16) - game_state.scroll_y; pos = low | t << 16 */
        sy = y & 0xf;
        row = y >= 0 ? y >> 4 : (y - 15) / 16;
        n = (int16_t)((uint32_t)(f4c - (y & -16)) >> 4);                               /* pos >> 16 is y again */
        i0 = 0;
    }
    sx = x12 & 0xf;
    col = ((((uint32_t)x12) & (width - 1)) >> 4) & 0xffffu;
    rows = (int16_t)(height >> 4);
    row16_init(&r, shape, layer, map, map_bytes, record, emit);

    for (i = i0; i < n && i - i0 < MAX_ROWS; i++, row = (int16_t)(row + 1)) {
        if (row < 0 || row > rows) continue;                                           /* the C's rows */
        row16_cells(&r, row, (int16_t)(i * 16 - sy), sx, col, k0, k1);
        if (r.stopped) return r.made;
    }
    if (!(i < n) && (f4c & 0xf) && (row > 0 || row <= rows))                           /* the partial row, at the counter where the loop ended */
        row16_cells(&r, row, (int16_t)(i * 16 - sy), sx, col, k0, k1);
    return r.made;
}

/* draw_tile_map_layer_polys of nash: 24 unskewed quads a row for rows from the camera's y, the first cropped at the top; the layer is
 * Slot06Layer, the map starts 0x800 halfwords into its tile_map_ptr (the caller gives `map` from there), entry 14. */
static unsigned quads_make(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                           const uint32_t *record, int k0, int k1, port_wide_emit emit)
{
    const uint32_t width = rd32(layer, CHAN_MAP_WIDTH);
    const int blank = (int16_t)rd16(layer, CHAN_BLANK), n = (int16_t)rd16(layer, SL_SIGNED_PARAM);
    const int f12 = (int16_t)rd16(layer, CHAN_SCROLL_X), f4c = (int16_t)rd16(layer, SL_FIELD_4C);
    const int y = (uint16_t)(rd16(layer, SL_FIELD_4C) - (n + 5)), sx = f12 & 0xf, sy = y & 0xf;
    const int entry = shape->fixed_entry - 1;
    const unsigned len = shape->words;
    int t, row, off, mask, i, i0, lim, k;
    unsigned col, made = 0;
    uint32_t words[12];

    if (!width || !map || (record[0] >> 24) != len || entry < 0) return 0;
    col = (((uint32_t)f12) & (width - 1)) >> 4;
    col &= 0xffffu;
    t = f4c - 5;
    row = (int16_t)(t / 16);
    off = (int16_t)(t - (row << 4));
    mask = (uint16_t)((width >> 4) - 1);
    i0 = (int16_t)y / 16;
    lim = (int16_t)y / 16 + 15 - (f4c - 5) / 16;

    for (i = i0; i < lim && i - i0 < MAX_ROWS; i++, row = (int16_t)(row + 1)) {
        const int first = i == i0, ya = sy + (i << 4), yu = (uint16_t)ya;
        const int yb = (uint16_t)(first ? ya + 16 - off : ya - off);                   /* yb (first row) or yb2 */
        const uint32_t line = line16(width, row);
        for (k = k0; k < k1; k++) {
            unsigned cc = (unsigned)(mask & (int)(col + (unsigned)k)) & 0xffffu;
            uint16_t raw, clut;
            int tile, x, u, v, y0, y2, v0;
            if (!read_cell(map, map_bytes, line + cell16(cc), &raw, &clut)) continue;
            tile = (int16_t)raw;
            if (tile == blank) continue;
            x = (uint16_t)(k * 16 - sx);
            u = raw << 3;
            v = (tile >> 2) & -16;
            y0 = (int16_t)(first ? yu : yb);
            y2 = (int16_t)(first ? yb : yb + 16);
            v0 = (uint8_t)(first ? off + v : v);
            memcpy(words, record + 1, len * sizeof words[0]);
            words[PW_XY0] = (uint32_t)(uint16_t)x | ((uint32_t)(uint16_t)y0 << 16);
            words[PW_UV0] = (uint32_t)(uint8_t)u | ((uint32_t)(uint8_t)v0 << 8) | ((uint32_t)clut << 16);
            words[PW_XY1] = (uint32_t)(uint16_t)(x + 16) | ((uint32_t)(uint16_t)y0 << 16);
            words[PW_UV1] = (uint32_t)(uint8_t)(u + 15) | ((uint32_t)(uint8_t)v0 << 8) | ((uint32_t)(uint16_t)(tile >> 10) << 16);
            words[PW_XY2] = (uint32_t)(uint16_t)x | ((uint32_t)(uint16_t)y2 << 16);
            words[PW_UV2] = (words[PW_UV2] & 0xffff0000u) | (uint32_t)(uint8_t)u | ((uint32_t)(uint8_t)(v | 15) << 8);
            words[PW_XY3] = (uint32_t)(uint16_t)(x + 16) | ((uint32_t)(uint16_t)y2 << 16);
            words[PW_UV3] = (words[PW_UV3] & 0xffff0000u) | (uint32_t)(uint8_t)(u + 15) | ((uint32_t)(uint8_t)(v | 15) << 8);
            if (emit(entry, words, len)) return made;
            made++;
        }
    }
    return made;
}

/* draw_scrolling_tile_map_layer of chunli_alt (Slot06Draw): 16 rows of 25 cells; rows 0..7 with the horizontal offset from a global (the
 * high half of `game`), rows 8..15 with the layer's own (the high half of end_x_fixed, which is scroll_x at 0x12). `halves`: bit 0
 * the first eight rows, bit 1 the last eight. */
static unsigned scrollc_make(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                             unsigned game, const uint32_t *record, int k0, int k1, int halves, port_wide_emit emit)
{
    struct row16 r;
    const uint32_t width = rd32(layer, CHAN_MAP_WIDTH), height = rd32(layer, CHAN_MAP_HEIGHT);
    int y, sy, row0, rows, i;
    int32_t pos;

    if (!width || !height || !map || (record[0] >> 24) != (unsigned)shape->words) return 0;
    pos = (int32_t)(0u - (rd32(layer, SL_BASE_X_FIXED) - 0x100000u));
    y = (int16_t)(uint16_t)((pos >> 16) - (int)(game & 0xffffu));
    sy = y & 0xf;
    row0 = y >= 0 ? y >> 4 : (y - 15) / 16;
    rows = (int16_t)(height >> 4);
    row16_init(&r, shape, layer, map, map_bytes, record, emit);
    for (i = 0; i < 16; i++) {
        int half = i >= 8, g = half ? (int16_t)rd16(layer, CHAN_SCROLL_X) : (int16_t)(game >> 16), row = (int16_t)(row0 + i);
        unsigned col = (((uint32_t)g) & (width - 1)) >> 4;
        if (!(halves & (1 << half))) continue;
        if (row < 0 || row >= rows) continue;
        row16_cells(&r, row, (int16_t)(i * 16 - sy), g & 0xf, col & 0xffffu, k0, k1);
        if (r.stopped) return r.made;
    }
    return r.made;
}

static int32_t wrap_mul(int32_t a, int32_t b) { return (int32_t)((uint32_t)a * (uint32_t)b); }
static int32_t wrap_div(int32_t a, int32_t b) { return b == -1 ? (int32_t)(0u - (uint32_t)a) : a / b; }

/* draw_skewed_tile_layer_quads: rows of 44 cells from k = -11, each cell (that is not blank) a quad whose four x are moved by an offset that
 * the row sets (s1 for the top edge, s0 for the bottom) and does not depend on the cell. `visible_only`: a cell is made only when its quad
 * reaches into screen x -128..512 (the sides); else every cell of k0..k1. The layer is Slot06Layer. */
static unsigned skew_make(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                          const uint32_t *record, int k0, int k1, int visible_only, port_wide_emit emit)
{
    const uint32_t width = rd32(layer, CHAN_MAP_WIDTH);
    const int blank = (int16_t)rd16(layer, CHAN_BLANK), entry = layer[CHAN_OT_INDEX] - 1;
    const int n = (int16_t)rd16(layer, SL_SIGNED_PARAM), f4c = (int16_t)rd16(layer, SL_FIELD_4C);
    const int f12 = (int16_t)rd16(layer, CHAN_SCROLL_X);
    const int y = (uint16_t)(rd16(layer, SL_FIELD_4C) - n);
    const int d1x = (int16_t)(rd16(layer, SL_RANGE_START) - n);
    const int d2x = (int16_t)(rd16(layer, SL_RECT_RIGHT) - rd16(layer, SL_RANGE_START));
    const int sx = (f12 - 0xb0) & 0xf, sy = y & 0xf;
    const int32_t a = (int32_t)(rd32(layer, SL_BOUND_X_B) - rd32(layer, SL_BOUND_X_A));
    const unsigned len = shape->words;
    int row = (int16_t)(f4c / 16), t, sy2, sy2s, m16, i, i0, lim, mask, k;
    unsigned col, made = 0;
    uint32_t words[12];

    if (!width || !d2x || !map || (record[0] >> 24) != len || entry < 0) return 0;
    col = (((uint32_t)(f12 - 0xb0)) & (width - 1)) >> 4;
    col &= 0xffffu;
    mask = (uint16_t)((width >> 4) - 1);
    t = f4c - (row * 16);
    sy2 = (uint16_t)t;
    sy2s = (int16_t)sy2;
    m16 = sy2s - 16;
    i0 = (int16_t)y / 16;
    lim = (int16_t)y / 16 + 15 - f4c / 16;

    for (i = i0; i < lim && i - i0 < MAX_ROWS; i++, row = (int16_t)(row + 1)) {
        const int first = i == i0, ya = sy + (i << 4), yu = (uint16_t)ya;
        const uint32_t line = line16(width, row);
        /* the top edge's offset: the first row uses the row's own y, the others y less the crop sy2 */
        const int32_t s1 = a - wrap_div(wrap_mul(a, first ? yu - d1x : yu - sy2s - d1x), d2x);
        const int32_t s0 = a - wrap_div(wrap_mul(a, yu - m16 - d1x), d2x);
        for (k = k0; k < k1; k++) {
            unsigned cc = (unsigned)(mask & (int)(col + (unsigned)(k + 11))) & 0xffffu;
            uint16_t raw, clut;
            int tile, u, v, x0, x1, x2, x3, y0, y2, v0;
            if (!read_cell(map, map_bytes, line + cell16(cc), &raw, &clut)) continue;
            tile = (int16_t)raw;
            if (tile == blank) continue;
            u = raw << 3;
            v = (tile >> 2) & -16;
            x0 = (int16_t)(k * 16 - sx + (s1 >> 16));
            x1 = (int16_t)(x0 + 16);
            x2 = (int16_t)(k * 16 - sx + (s0 >> 16));
            x3 = (int16_t)(x2 + 16);
            if (visible_only) {
                int lo = x0 < x2 ? x0 : x2, hi = x1 > x3 ? x1 : x3;
                if (!(hi > VIEW_LEFT && lo < VIEW_RIGHT)) continue;
            }
            if (first) {
                y0 = (int16_t)yu;
                y2 = (int16_t)(ya + 16 - sy2);
                v0 = (uint8_t)(sy2 + v);
            } else {
                y0 = (int16_t)(ya - sy2);
                y2 = (int16_t)(y0 + 16);
                v0 = (uint8_t)v;
            }
            memcpy(words, record + 1, len * sizeof words[0]);
            words[PW_XY0] = (uint32_t)(uint16_t)x0 | ((uint32_t)(uint16_t)y0 << 16);
            words[PW_UV0] = (uint32_t)(uint8_t)u | ((uint32_t)(uint8_t)v0 << 8) | ((uint32_t)clut << 16);
            words[PW_XY1] = (uint32_t)(uint16_t)x1 | ((uint32_t)(uint16_t)y0 << 16);
            words[PW_UV1] = (uint32_t)(uint8_t)(u + 15) | ((uint32_t)(uint8_t)v0 << 8) | ((uint32_t)(uint16_t)(tile >> 10) << 16);
            words[PW_XY2] = (uint32_t)(uint16_t)x2 | ((uint32_t)(uint16_t)y2 << 16);
            words[PW_UV2] = (words[PW_UV2] & 0xffff0000u) | (uint32_t)(uint8_t)u | ((uint32_t)(uint8_t)(v | 15) << 8);
            words[PW_XY3] = (uint32_t)(uint16_t)x3 | ((uint32_t)(uint16_t)y2 << 16);
            words[PW_UV3] = (words[PW_UV3] & 0xffff0000u) | (uint32_t)(uint8_t)(u + 15) | ((uint32_t)(uint8_t)(v | 15) << 8);
            if (emit(entry, words, len)) return made;
            made++;
        }
    }
    return made;
}

unsigned port_wide_columns(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                           unsigned game, const uint32_t *record, int k0, int k1, port_wide_emit emit)
{
    switch (shape->kind) {
    case PORT_WIDE_SPRITES: return sprites_columns(shape, layer, map, map_bytes, game, record, k0, k1, emit);
    case PORT_WIDE_SKEWED:  return skew_make(shape, layer, map, map_bytes, record, k0, k1, 0, emit);
    case PORT_WIDE_QUADS:   return quads_make(shape, layer, map, map_bytes, record, k0, k1, emit);
    case PORT_WIDE_SCROLLC: return scrollc_make(shape, layer, map, map_bytes, game, record, k0, k1, 3, emit);
    default:                return flat_columns(shape, layer, map, map_bytes, game, record, k0, k1, emit);
    }
}

unsigned port_wide_sides(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                         unsigned game, const uint32_t *record, port_wide_emit emit)
{
    int left, right;
    unsigned made;
    if (shape->kind == PORT_WIDE_SKEWED) {
        int first = shape->k_first, end = shape->k_first + shape->cols;
        made = skew_make(shape, layer, map, map_bytes, record, first - PORT_WIDE_SKEW_EXTRA, first, 1, emit);
        return made + skew_make(shape, layer, map, map_bytes, record, end, end + PORT_WIDE_SKEW_EXTRA, 1, emit);
    }
    if (shape->kind == PORT_WIDE_SCROLLC) {          /* each half has its own offset, so its own columns */
        int half;
        made = 0;
        for (half = 0; half < 2; half++) {
            range_for_sx(shape, scrollc_sx(layer, game, half), &left, &right);
            made += scrollc_make(shape, layer, map, map_bytes, game, record, left, 0, 1 << half, emit);
            made += scrollc_make(shape, layer, map, map_bytes, game, record, shape->cols, right, 1 << half, emit);
        }
        return made;
    }
    port_wide_side_range(shape, layer, &left, &right);
    made = port_wide_columns(shape, layer, map, map_bytes, game, record, left, 0, emit);
    return made + port_wide_columns(shape, layer, map, map_bytes, game, record, shape->cols, right, emit);
}

int port_wide_entry(const struct port_wide_shape *shape, const uint8_t *layer)
{
    if (shape->fixed_entry) return shape->fixed_entry - 1;
    return shape->kind == PORT_WIDE_SKEWED ? layer[CHAN_OT_INDEX] - 1 : layer[CHAN_OT_INDEX];
}

/* The cells of a scan: the x range of each packet. */
#define EXTENT_MAX 4096
static struct { int xmin, xmax; } extent_cell[EXTENT_MAX];
static unsigned extent_n;
static int extent_size, extent_quad;

static int extent_emit(int entry, const uint32_t *w, unsigned len)
{
    int xmin, xmax;
    (void)entry; (void)len;
    if (extent_n >= EXTENT_MAX) return -1;
    if (extent_quad) {                                   /* the four corners, each with the row's own offset */
        int x0 = (int16_t)w[PW_XY0], x1 = (int16_t)w[PW_XY1], x2 = (int16_t)w[PW_XY2], x3 = (int16_t)w[PW_XY3];
        xmin = x0 < x1 ? x0 : x1; if (x2 < xmin) xmin = x2; if (x3 < xmin) xmin = x3;
        xmax = x0 > x1 ? x0 : x1; if (x2 > xmax) xmax = x2; if (x3 > xmax) xmax = x3;
        if (xmax <= xmin) xmax = xmin + 1;               /* x + 16 is the cell's right edge */
    } else {
        xmin = (int16_t)w[WORD_XY];
        xmax = xmin + extent_size;
    }
    extent_cell[extent_n].xmin = xmin;
    extent_cell[extent_n].xmax = xmax;
    extent_n++;
    return 0;
}

static int floor_div(int a, int b) { int q = a / b; return (a % b && ((a < 0) != (b < 0))) ? q - 1 : q; }

/* The grid of the scan's columns: for the forms with one horizontal offset (the plain grids, the sprite forms, the quads) the cells' own
 * columns, k = floor((x + sx) / size); for the skewed form (each row has its own offset) and chunli_alt's (two) the columns are slots of the
 * cell's width on the screen grid with 0 on an edge. */
static int extent_offset(const struct port_wide_shape *shape, const uint8_t *layer)
{
    int ok, size = 1 << shape->shift;
    if (shape->kind == PORT_WIDE_SKEWED || shape->kind == PORT_WIDE_SCROLLC) return 0;
    if (shape->kind == PORT_WIDE_SPRITES) return sprites_x12(layer, &ok) & (size - 1);
    return (int)(rd16(layer, CHAN_SCROLL_X) & (unsigned)(size - 1));
}

int port_wide_extent(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                     unsigned game, const uint32_t *record, int *art_lo, int *art_hi)
{
    const int size = 1 << shape->shift, off = extent_offset(shape, layer);
    const int first = shape->k_first - (size == 16 ? 40 : 20), end = shape->k_first + shape->cols + (size == 16 ? 40 : 20);
    unsigned char painted[80];
    const int base = -(256 / size) - 2, count = (640 + 256) / size + 6;     /* the slots from base on */
    const int s_min = floor_div(-256 + off + size - 1, size);                /* the leftmost slot whose left edge is at -256 or more */
    const int s_max = floor_div(640 + off, size) - 1;                        /* the rightmost whose right edge is at 640 or less */
    const int view_left = -1, view_right = floor_div(384 + off + size - 1, size);   /* the slots wholly left of 0 start at this and below; wholly right of 384 start here */
    int s, blank, last_left, last_right, lo = 0, hi = 384;
    unsigned i;

    extent_n = 0;
    extent_size = size;
    extent_quad = shape->words == 9;
    port_wide_columns(shape, layer, map, map_bytes, game, record, first, end, extent_emit);
    if (extent_n >= EXTENT_MAX) return -1;             /* the scan was cut: no view is better than a wrong one */
    memset(painted, 0, sizeof painted);
    for (i = 0; i < extent_n; i++) {
        int a = floor_div(extent_cell[i].xmin + off, size), b = floor_div(extent_cell[i].xmax - 1 + off, size);
        for (s = a; s <= b; s++)
            if (s >= base && s < base + count) painted[s - base] = 1;
    }
    last_left = 0;                                      /* 0: nothing painted left of the view */
    for (s = view_left, blank = 0; s >= s_min; s--) {
        if (painted[s - base]) { last_left = s; blank = 0; }
        else if (++blank > 2) break;
    }
    last_right = view_right - 1;
    for (s = view_right, blank = 0; s <= s_max; s++) {
        if (painted[s - base]) { last_right = s; blank = 0; }
        else if (++blank > 2) break;
    }
    for (i = 0; i < extent_n; i++) {
        int a = floor_div(extent_cell[i].xmin + off, size), b = floor_div(extent_cell[i].xmax - 1 + off, size);
        if (last_left < 0 && b >= last_left && extent_cell[i].xmin < lo) lo = extent_cell[i].xmin;
        if (a <= last_right && extent_cell[i].xmax > hi) hi = extent_cell[i].xmax;   /* a cell that straddles 384 counts */
    }
    *art_lo = lo < -256 ? -256 : lo;
    *art_hi = hi > 640 ? 640 : hi;
    return 0;
}

void port_wide_map_bytes(const struct port_wide_shape *shape, const uint8_t *layer, unsigned game, uint32_t *preferred, uint32_t *minimum)
{
    uint32_t width = rd32(layer, CHAN_MAP_WIDTH), height = rd32(layer, CHAN_MAP_HEIGHT);
    uint32_t block_row = ((width >> 9) << (shape->shift == 4 ? 10 : 8)) * 2u;   /* the bytes of one row of blocks */
    (void)game;
    *preferred = *minimum = 0;
    if (!width) return;
    if (shape->kind == PORT_WIDE_SKEWED || shape->kind == PORT_WIDE_QUADS) {
        /* no height in this layer (Slot06Layer has none): the rows that the C reads, up to the last one's block row */
        const int f4c = (int16_t)rd16(layer, SL_FIELD_4C), cut = shape->kind == PORT_WIDE_QUADS ? 5 : 0;
        int row = (int16_t)((f4c - cut) / 16), i, count = 15 - (f4c - cut) / 16;
        unsigned top = 0;
        for (i = 0; i < count && i < MAX_ROWS; i++, row = (int16_t)(row + 1))
            if (((unsigned)(uint16_t)row >> 4) > top) top = (unsigned)(uint16_t)row >> 4;
        *preferred = *minimum = (top + 1) * block_row;
        return;
    }
    if (shape->variant == PORT_WIDE_V_ROLENTO16) {        /* one row of blocks, whatever the rows */
        *preferred = *minimum = block_row;
        return;
    }
    if (shape->variant == PORT_WIDE_V_ROLENTO32) {        /* the second half of the map is 0x200 halfwords on */
        *preferred = *minimum = block_row + 0x300u * 2u;
        return;
    }
    if (!height) return;
    *minimum = (width >> shape->shift) * (height >> shape->shift) * 4u;
    *preferred = *minimum + block_row;     /* the C of the 32 px form (and the sprite form) also reads the row after the last */
}

/* ---- the stages' copies. Read from ps1/src/overlays/stages/NAME/FUNCTION.c (port/tools/test_wide_stage.py checks every line against
 * the sources, and that no copy is missing): the layer is background_layers[0], [1] or [2] (0x801aa544, 0x90 apart; the last two forms
 * read [1] as main_layer, 0x801aa5d4), the pool the `extern Slot06Tile data_ADDRESS_slot06_XX[2][count]` of the function (Poly28 for the
 * skewed form), and the form follows from the name. ---- */
#define STAGE_COPIES(X) \
    X( 0, draw_tile_layer_0_16px_slot06_00, L0, 0x801aa544, 0x801ec390, 114) \
    X( 1, draw_tile_layer_0_16px_slot06_04, L0, 0x801aa544, 0x801eeb48, 112) \
    X( 2, draw_tile_layer_0_16px_slot06_05, L0, 0x801aa544, 0x801ec3b4, 125) \
    X( 3, draw_tile_layer_0_16px_slot06_06, L0, 0x801aa544, 0x801ee288, 307) \
    X( 4, draw_tile_layer_0_16px_slot06_07, L0, 0x801aa544, 0x801ee22c, 66) \
    X( 5, draw_tile_layer_0_16px_slot06_08, L0, 0x801aa544, 0x801ed4f4, 235) \
    X( 6, draw_tile_layer_0_16px_slot06_0b, L0, 0x801aa544, 0x801ec138, 52) \
    X( 7, draw_tile_layer_0_16px_slot06_0c, L0, 0x801aa544, 0x801ee070, 69) \
    X( 8, draw_tile_layer_0_16px_slot06_0f, L0, 0x801aa544, 0x801ef004, 201) \
    X( 9, draw_tile_layer_0_16px_slot06_10, L0, 0x801aa544, 0x801eec10, 159) \
    X(10, draw_tile_layer_0_16px_slot06_11, L0, 0x801aa544, 0x801ee328, 125) \
    X(11, draw_tile_layer_0_16px_slot06_12, L0, 0x801aa544, 0x801ef340, 140) \
    X(12, draw_tile_layer_0_16px_slot06_13, L0, 0x801aa544, 0x801eb5b8, 250) \
    X(13, draw_tile_layer_1_16px_slot06_00, L1, 0x801aa5d4, 0x801edc80, 270) \
    X(14, draw_tile_layer_1_16px_slot06_05, L1, 0x801aa5d4, 0x801edf0c, 373) \
    X(15, draw_tile_layer_1_16px_slot06_0a, L1, 0x801aa5d4, 0x801efed0, 292) \
    X(16, draw_tile_layer_1_16px_slot06_10, L1, 0x801aa5d4, 0x801f0ed8, 301) \
    X(17, draw_tile_layer_1_16px_slot06_11, L1, 0x801aa5d4, 0x801efe80, 400) \
    X(18, draw_tile_layer_1_16px_slot06_13, L1, 0x801aa5d4, 0x801eec68, 195) \
    X(19, draw_tilemap_layer_32px_9x13_slot06_00, T32, 0x801aa664, 0x801f1790, 98) \
    X(20, draw_tilemap_layer_32px_9x13_slot06_04, T32, 0x801aa664, 0x801f6660, 26) \
    X(21, draw_tilemap_layer_32px_9x13_slot06_05, T32, 0x801aa664, 0x801f30a4, 104) \
    X(22, draw_tilemap_layer_32px_9x13_slot06_06, T32, 0x801aa664, 0x801f6d18, 91) \
    X(23, draw_tilemap_layer_32px_9x13_slot06_07, T32, 0x801aa664, 0x801f4f44, 81) \
    X(24, draw_tilemap_layer_32px_9x13_slot06_08, T32, 0x801aa664, 0x801f7784, 47) \
    X(25, draw_tilemap_layer_32px_9x13_slot06_0b, T32, 0x801aa664, 0x801f3bd0, 91) \
    X(26, draw_tilemap_layer_32px_9x13_slot06_0c, T32, 0x801aa664, 0x801f4500, 91) \
    X(27, draw_tilemap_layer_32px_9x13_slot06_0f, T32, 0x801aa664, 0x801f6fe4, 78) \
    X(28, draw_tilemap_layer_32px_9x13_slot06_10, T32, 0x801aa664, 0x801f50b0, 93) \
    X(29, draw_tilemap_layer_32px_9x13_slot06_11, T32, 0x801aa664, 0x801f5600, 48) \
    X(30, draw_tilemap_layer_32px_9x13_slot06_12, T32, 0x801aa664, 0x801f4cf0, 78) \
    X(31, draw_tilemap_layer_32px_9x13_slot06_13, T32, 0x801aa664, 0x801f1710, 78) \
    X(32, draw_skewed_tile_layer_quads_slot06_01, SKEW, 0x801aa5d4, 0x801f6b7c, 132) \
    X(33, draw_skewed_tile_layer_quads_slot06_02, SKEW, 0x801aa5d4, 0x801f5cf8, 132) \
    X(34, draw_skewed_tile_layer_quads_slot06_04, SKEW, 0x801aa5d4, 0x801f4ae0, 88) \
    X(35, draw_skewed_tile_layer_quads_slot06_06, SKEW, 0x801aa5d4, 0x801f3618, 176) \
    X(36, draw_skewed_tile_layer_quads_slot06_07, SKEW, 0x801aa5d4, 0x801f33c4, 88) \
    X(37, draw_skewed_tile_layer_quads_slot06_09, SKEW, 0x801aa5d4, 0x801f34e4, 88) \
    X(38, draw_skewed_tile_layer_quads_slot06_0b, SKEW, 0x801aa5d4, 0x801f04d0, 176) \
    X(39, draw_skewed_tile_layer_quads_slot06_0c, SKEW, 0x801aa5d4, 0x801f2980, 88) \
    X(40, draw_skewed_tile_layer_quads_slot06_0d, SKEW, 0x801aa5d4, 0x801f365c, 88) \
    X(41, draw_skewed_tile_layer_quads_slot06_0f, SKEW, 0x801aa5d4, 0x801f46a4, 132) \
    X(42, draw_tile_layer_sprites_with_partial_row_slot06_01, SPR, 0x801aa5d4, 0x801f29dc, 300) \
    X(43, draw_tile_layer_sprites_with_partial_row_slot06_02, SPR, 0x801aa5d4, 0x801f1f48, 282) \
    X(44, draw_tile_layer_sprites_with_partial_row_slot06_04, SPR, 0x801aa5d4, 0x801f03c8, 325) \
    X(45, draw_tile_layer_sprites_with_partial_row_slot06_07, SPR, 0x801aa5d4, 0x801ef09c, 307) \
    X(46, draw_tile_layer_sprites_with_partial_row_slot06_09, SPR, 0x801aa5d4, 0x801efaec, 265) \
    X(47, draw_tile_layer_sprites_with_partial_row_slot06_0b, SPR, 0x801aa5d4, 0x801ecc98, 257) \
    X(48, draw_tile_layer_sprites_with_partial_row_slot06_0c, SPR, 0x801aa5d4, 0x801eef88, 265) \
    X(49, draw_tile_layer_sprites_with_partial_row_slot06_0d, SPR, 0x801aa5d4, 0x801eef44, 325) \
    X(50, draw_tile_layer_sprites_with_partial_row_slot06_0e, SPR, 0x801aa5d4, 0x801f22d4, 124) \
    X(51, draw_tile_layer_sprites_with_partial_row_slot06_0f, SPR, 0x801aa5d4, 0x801f1bfc, 195) \
    X(52, draw_tilemap_layer_32px_7x13_slot06_01, T32_7, 0x801aa664, 0x801f94bc, 67) \
    X(53, draw_tilemap_layer_32px_7x13_slot06_03, T32_7, 0x801aa664, 0x801f62c0, 117) \
    X(54, draw_tilemap_layer_32px_7x13_slot06_09, T32_7, 0x801aa664, 0x801f5064, 91) \
    X(55, draw_tile_map_layer_16px_half_scroll_slot06_0a, T16H, 0x801aa544, 0x801ed498, 193) \
    X(56, draw_tile_map_layer_16px_half_scroll_slot06_0d, T16H, 0x801aa544, 0x801ed5e4, 116) \
    X(57, draw_tile_map_layer_32px_slot06_02, T32G, 0x801aa664, 0x801f8638, 13) \
    X(58, draw_tile_map_layer_32px_slot06_0a, T32H, 0x801aa664, 0x801f3eb0, 61) \
    X(59, draw_tile_map_layer_32px_slot06_0d, T32H, 0x801aa664, 0x801f51dc, 90) \
    X(60, draw_scrolling_tile_map_layer_slot06_08, SCRB, 0x801aa5d4, 0x801f085c, 131) \
    X(61, draw_scrolling_tile_map_layer_slot06_12, SCRC, 0x801aa5d4, 0x801f11e0, 270) \
    X(62, draw_tile_map_layer_16px_slot06_02, L0G, 0x801aa544, 0x801ef0e8, 212) \
    X(63, draw_tile_map_layer_16px_slot03_slot06_03, L0N, 0x801aa544, 0x801ee810, 170) \
    X(64, draw_tile_map_layer_16px_slot06_09, L0R, 0x801aa544, 0x801eca24, 223) \
    X(65, draw_tilemap_layer_16px_12x25_slot06_01, L0K, 0x801aa544, 0x801eedb4, 275) \
    X(66, draw_tile_layer_sprites_saving_pos_slot06_03, SAVE, 0x801aa5d4, 0x801f0d40, 288) \
    X(67, draw_tile_map_layer_polys_slot06_03, POLY, 0x801aa5d4, 0x801f4c40, 72) \
    X(68, draw_perspective_skewed_tile_map_slot06_0e, SKEWP, 0x801aa5d4, 0x801f3df4, 132) \
    X(69, draw_tile_map_16px_tiles_slot06_0e, R16, 0x801aa544, 0x801ef554, 208) \
    X(70, draw_tile_map_layer_sprites_32px_slot06_0e, R32, 0x801aa664, 0x801f6734, 117) \
    X(71, draw_tile_map_layer_sprites_interpolated_slot06_06, SPRI, 0x801aa5d4, 0x801f25b0, 75)

#define AS_COPY(n, name, form, layer, pool, count) { #name, &port_wide_shape_##form, layer, pool, count },
const struct port_wide_copy port_wide_copies[] = { STAGE_COPIES(AS_COPY) };
#undef AS_COPY
#define WIDE_COUNT(n, name, form, layer, pool, count) +1
const unsigned port_wide_copy_count = 0 STAGE_COPIES(WIDE_COUNT);
#undef WIDE_COUNT

/* Slot06Tile is 0x1c bytes and Slot06TileW 0x20: the tag and the packet's words. */
static unsigned record_bytes(const struct port_wide_shape *shape) { return 4u + 4u * (unsigned)shape->words; }

static port_fn impl_of[sizeof port_wide_copies / sizeof port_wide_copies[0]];
static unsigned char impl_looked[sizeof port_wide_copies / sizeof port_wide_copies[0]];

static void side_tiles(const struct port_wide_copy *c)
{
    const struct port_wide_shape *shape = c->shape;
    const uint8_t *layer, *map;
    const uint32_t *record;
    const uint16_t *scroll_y;
    const int16_t *global;
    const int *buffer;
    uint32_t preferred, minimum, map_data;
    unsigned game;
    int buf, art_lo, art_hi;

    layer = (const uint8_t *)port_gs((const void *)(uintptr_t)c->layer, shape->second_blank ? 2 * PORT_WIDE_CHAN_SIZE : PORT_WIDE_CHAN_SIZE);
    scroll_y = (const uint16_t *)port_gs((const void *)(uintptr_t)GAME_STATE_SCROLL_Y, sizeof *scroll_y);
    buffer = (const int *)port_gs((const void *)(uintptr_t)DRAW_BUFFER_INDEX, sizeof *buffer);
    if (!layer || !scroll_y || !buffer) return;
    buf = *buffer;
    if (buf < 0 || buf > 1) return;
    game = *scroll_y;
    if (shape->kind == PORT_WIDE_SCROLLC) {            /* the global that the first eight rows scroll by */
        global = (const int16_t *)port_gs((const void *)(uintptr_t)PORT_WIDE_SCROLL_GLOBAL, sizeof *global);
        if (!global) return;
        game |= (unsigned)(uint16_t)*global << 16;
    }
    record = (const uint32_t *)port_gs((const void *)(uintptr_t)(c->pool + (unsigned)buf * c->count * record_bytes(shape)),
                                       record_bytes(shape));
    if (!record) return;
    /* the map: preferred bytes when the game's memory has them (the C of some forms reads one row past the map), else the minimum */
    port_wide_map_bytes(shape, layer, game, &preferred, &minimum);
    if (!minimum) return;
    map_data = rd32(layer, CHAN_MAP_DATA) + 2u * (unsigned)shape->map_offset;
    map = (const uint8_t *)port_gs((const void *)(uintptr_t)map_data, preferred);
    if (!map) {
        preferred = minimum;
        map = (const uint8_t *)port_gs((const void *)(uintptr_t)map_data, minimum);
    }
    if (!map) return;
    port_wide_sides(shape, layer, map, preferred, game, record, port_gpu_queue_packet);
    /* the plane's painting, for the bridge to keep its wide view inside it (the map's own bytes: the row past its end, which the C of some
     * forms reads, is not painting): the pool of both buffers and the entry of its side cells */
    if (port_wide_extent(shape, layer, map, minimum, game, record, &art_lo, &art_hi) == 0)
        port_gpu_layer_view(c->layer, c->pool, c->pool + 2u * c->count * record_bytes(shape), port_wide_entry(shape, layer), art_lo, art_hi);
}

static void run(unsigned index)
{
    const struct port_wide_copy *c = &port_wide_copies[index];
    if (!impl_looked[index]) {
        impl_looked[index] = 1;
        impl_of[index] = port_function_impl(c->name);
        if (impl_of[index] == PORT_FN_NULL) fprintf(stderr, "wide-stage: %s has no C in this build; its layer is not drawn\n", c->name);
    }
    if (impl_of[index] == PORT_FN_NULL) return;
    port_call0(impl_of[index]);
    if (port_settings.widescreen && port_settings.arcade_video) side_tiles(c);
}

#define AS_WRAPPER(n, name, form, layer, pool, count) static void wide_run_##n(void) { run(n); }
STAGE_COPIES(AS_WRAPPER)
#undef AS_WRAPPER

#define AS_OVERRIDE(n, name, form, layer, pool, count) \
    { #name, (void *)wide_run_##n, \
      "runs the game's C, then with widescreen on makes the tile packets of the columns beside the C's, for the wide view", NULL },
const struct port_override port_wide_overrides[] = {
    STAGE_COPIES(AS_OVERRIDE)
    { NULL, NULL, NULL, NULL }
};
#undef AS_OVERRIDE
