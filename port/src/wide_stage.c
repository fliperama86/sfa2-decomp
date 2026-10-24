/* Widescreen's side tiles (port/widescreen.md, "The side tiles"): the stages' tile layers drawn past the columns that the game's own C
 * draws. Three forms of layer function are shared by many stages (the C is the same in every copy; only the pool differs):
 * draw_tile_layer_0_16px (13 stages), draw_tile_layer_1_16px (6) and draw_tilemap_layer_32px_9x13 (13). Each copy gets an override
 * that runs the game's own C and then, with widescreen on, makes the packets of the columns left of the C's first and right of its
 * last that the wide view can show, with the C's arithmetic for each column and row, and queues them for the layer's entry of the
 * ordering table (port_gpu_queue_packet). With widescreen off it only runs the C. The maker (port_wide_columns) is pure and is
 * compared with the C in port/tools/wide_stage_test.c. */
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

/* What the anchored wide view can show, in the game's screen x: the canvas is 64 pixels wider on each side and the view slides by up to 64. */
#define VIEW_LEFT  (-128)
#define VIEW_RIGHT 512

/* The packet's words after the tag, in the order of the record (Slot06Tile, Slot06TileW): mode[0] (0xe1000000 + the tile's texture
 * page bits), mode[1], the sprite's tag (a NOP command), its colour and code, its x and y, its u, v and clut, and for the wide
 * record its w and h. */
#define WORD_MODE 0
#define WORD_XY   4
#define WORD_UV   5

const struct port_wide_shape port_wide_shape_L0  = { 4, 16, 25, 6, 1, 0 };
const struct port_wide_shape port_wide_shape_L1  = { 4, 16, 25, 6, 0, 0 };
const struct port_wide_shape port_wide_shape_T32 = { 5, 9, 13, 7, 0, 1 };

static uint16_t rd16(const uint8_t *p, unsigned off) { uint16_t v; memcpy(&v, p + off, sizeof v); return v; }
static uint32_t rd32(const uint8_t *p, unsigned off) { uint32_t v; memcpy(&v, p + off, sizeof v); return v; }

void port_wide_side_range(const struct port_wide_shape *shape, const uint8_t *layer, int *k_left, int *k_right)
{
    int size = 1 << shape->shift, sx = (int)(rd16(layer, CHAN_SCROLL_X) & (unsigned)(size - 1));
    int kl = 0, kr = shape->cols;
    while ((kl - 1) * size - sx + size > VIEW_LEFT && kl > -64) kl--;   /* the cell kl - 1 reaches into the view */
    while (kr * size - sx < VIEW_RIGHT && kr < 64) kr++;
    *k_left = kl;
    *k_right = kr;
}

unsigned port_wide_columns(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                           unsigned game_scroll_y, const uint32_t *record, int k0, int k1, port_wide_emit emit)
{
    /* The C's own arithmetic (the three functions under ps1/src/overlays/stages, any copy), step by step. */
    const int size = 1 << shape->shift;
    const uint32_t width = rd32(layer, CHAN_MAP_WIDTH), height = rd32(layer, CHAN_MAP_HEIGHT);
    const unsigned scroll_x = rd16(layer, CHAN_SCROLL_X);
    const int blank = (int16_t)rd16(layer, CHAN_BLANK);
    const int entry = layer[CHAN_OT_INDEX];
    const unsigned len = shape->words;
    uint32_t x_before, pos_neg;
    int pos, y, sx, sy, rows, mask, row0, i, k;
    unsigned col;
    uint32_t words[8];
    unsigned made = 0;

    if (!width || !height || !map || (record[0] >> 24) != len) return 0;
    x_before = rd32(layer, CHAN_SCROLL_Y) - 0x100000u;                 /* x = scroll_y - 0x100000 */
    pos_neg = 0u - x_before;                                           /* pos = -x */
    pos = (int)pos_neg;
    y = (int16_t)(uint16_t)((pos >> 16) - (int)(game_scroll_y & 0xffffu));   /* t = (pos >> 16) - game_state.scroll_y, 16 bits; y = t */
    sx = (int)(scroll_x & (unsigned)(size - 1));
    sy = y & (size - 1);
    rows = (int16_t)(height >> shape->shift);
    mask = (uint16_t)((width >> shape->shift) - 1);
    col = (((uint32_t)(int32_t)(int16_t)scroll_x) & (width - 1)) >> shape->shift;
    row0 = y >= 0 ? y >> shape->shift : (y - (size - 1)) / size;
    col &= 0xffffu;                                                    /* cc is 16 bits */

    for (i = 0; i < shape->rows; i++) {
        int row = (int16_t)(row0 + i);
        uint32_t line;
        if (row < 0 || (shape->row_inclusive ? rows < row : row >= rows)) continue;
        /* the map is blocks of 32 x 16 cells (16 px form) or 16 x 8 cells (32 px form), two halfwords a cell (tile, clut) */
        if (shape->shift == 4)
            line = ((uint32_t)((uint16_t)row >> 4)) * ((width >> 9) << 10) + (((unsigned)row & 0xf) << 6);
        else
            line = ((uint32_t)((uint16_t)row >> 3)) * ((width >> 9) << 8) + (((unsigned)row & 7) << 5);
        for (k = k0; k < k1; k++) {
            unsigned cc = (unsigned)(mask & (int)(col + (unsigned)k)) & 0xffffu;    /* mask & (cc + 1), k times; k < 0 backwards */
            uint32_t cell = line + (shape->shift == 4 ? ((cc >> 5) << 10) + ((cc & 0x1f) << 1) : ((cc >> 4) << 8) + ((cc & 0xf) << 1));
            uint16_t raw;
            int tile, tile_s;
            if ((uint64_t)cell * 2 + 4 > map_bytes) continue;
            raw = rd16(map, cell * 2);
            tile_s = (int16_t)raw;
            tile = shape->unsigned_blank ? (int)raw : tile_s;
            if (tile == blank) continue;
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

/* ---- the stages' copies. Read from ps1/src/overlays/stages/NAME/FUNCTION.c (port/tools/test_wide_stage.py checks every line against
 * the sources, and that no copy is missing): the layer is background_layers[0], [1] or [2] (0x801aa544, 0x90 apart), the pool the
 * `extern Slot06Tile data_ADDRESS_slot06_XX[2][count]` of the function, and the form follows from the name. ---- */
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
    X(31, draw_tilemap_layer_32px_9x13_slot06_13, T32, 0x801aa664, 0x801f1710, 78)

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
    const int *buffer;
    uint32_t width, height, exact, block_row;
    int left, right, buf;

    layer = (const uint8_t *)port_gs((const void *)(uintptr_t)c->layer, PORT_WIDE_CHAN_SIZE);
    scroll_y = (const uint16_t *)port_gs((const void *)(uintptr_t)GAME_STATE_SCROLL_Y, sizeof *scroll_y);
    buffer = (const int *)port_gs((const void *)(uintptr_t)DRAW_BUFFER_INDEX, sizeof *buffer);
    if (!layer || !scroll_y || !buffer) return;
    buf = *buffer;
    if (buf < 0 || buf > 1) return;
    record = (const uint32_t *)port_gs((const void *)(uintptr_t)(c->pool + (unsigned)buf * c->count * record_bytes(shape)),
                                       record_bytes(shape));
    width = rd32(layer, CHAN_MAP_WIDTH);
    height = rd32(layer, CHAN_MAP_HEIGHT);
    if (!record || !width || !height) return;
    /* the map: (width / cell) x (height / cell) cells of two halfwords; the 32 px form's C also reads the row after the last (`rows < row`),
     * so one more row of blocks is asked for when the memory has it */
    exact = (width >> shape->shift) * (height >> shape->shift) * 4u;
    block_row = ((width >> 9) << (shape->shift == 4 ? 10 : 8)) * 2u;
    map = (const uint8_t *)port_gs((const void *)(uintptr_t)rd32(layer, CHAN_MAP_DATA), exact + block_row);
    if (map) exact += block_row;
    else map = (const uint8_t *)port_gs((const void *)(uintptr_t)rd32(layer, CHAN_MAP_DATA), exact);
    if (!map) return;
    port_wide_side_range(shape, layer, &left, &right);
    port_wide_columns(shape, layer, map, exact, *scroll_y, record, left, 0, port_gpu_queue_packet);
    port_wide_columns(shape, layer, map, exact, *scroll_y, record, shape->cols, right, port_gpu_queue_packet);
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
