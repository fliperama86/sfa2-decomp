/* Widescreen's side tiles, the maker (port/widescreen.md, "The side tiles"): the packets of the tile columns that the game's own
 * layer functions do not draw. Pure: it reads a layer record and a map through host pointers and hands each packet to a callback,
 * so port/tools/wide_stage_test.c can compare it with the game's C. wide_stage.c has the overrides that use it. */
#ifndef PORT_WIDE_STAGE_H
#define PORT_WIDE_STAGE_H

#include <stdint.h>

/* One of the three forms of the stages' layer functions (the C is the same in every copy of a form; the copies differ in the pool). */
struct port_wide_shape {
    int shift;           /* the cell is 1 << shift pixels: 4 or 5 */
    int rows, cols;      /* the cells the C draws: 16 x 25, or 9 x 13 */
    int words;           /* the packet's words after the tag (the record's tag length): 6, or 7 with the sprite's width and height */
    int unsigned_blank;  /* the C compares the u16 cell with the s16 blank as ints (draw_tile_layer_0_16px); else both as s16 */
    int row_inclusive;   /* the C skips a row when `rows < row` (the 32 px form), not `row >= rows` */
};
extern const struct port_wide_shape port_wide_shape_L0, port_wide_shape_L1, port_wide_shape_T32;

/* The layer record (struct Chan, 0x90 bytes) in the game's order; the maker reads these fields of it. */
#define PORT_WIDE_CHAN_SIZE 0x90u

/* A packet: `len` words (the GP0 commands) for entry `entry` of the ordering table; 0, or not 0 when it was not taken. This is the
 * signature of port_gpu_queue_packet. */
typedef int (*port_wide_emit)(int entry, const uint32_t *words, unsigned len);

/* The packets of the columns k0 <= k < k1 (k = 0 is the C's first column, k < 0 left of it, k >= shape->cols right of the last), in the
 * C's order (row by row, then column), for the layer record `layer` (host pointer, PORT_WIDE_CHAN_SIZE bytes), its map `map` (the
 * bytes the layer's map_data points to, `map_bytes` of them; a cell past them is taken as blank), the game's scroll_y
 * (game_state.scroll_y) and the pool's first record `record` (shape->words + 1 words: the tag, whose length byte must equal
 * shape->words, and the static words). Returns the number of packets made; stops at the first that `emit` refuses. */
unsigned port_wide_columns(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                           unsigned game_scroll_y, const uint32_t *record, int k0, int k1, port_wide_emit emit);

/* The columns of the sides: k_left (<= -1) is the first column left of the C's that is visible in the wide view (screen x from -128 to
 * 512 when the packet's cell reaches into it), k_right the first column right of the C's last that is not. */
void port_wide_side_range(const struct port_wide_shape *shape, const uint8_t *layer, int *k_left, int *k_right);

/* A stage's copy of one of the forms (the table in wide_stage.c, which the unit test checks against ps1/src). */
struct port_wide_copy {
    const char *name;    /* the game's function */
    const struct port_wide_shape *shape;
    unsigned layer;      /* the guest address of its layer record (background_layers[0], [1] or [2]) */
    unsigned pool;       /* the guest address of its pool, Slot06Tile or Slot06TileW [2][count] */
    unsigned count;
};
extern const struct port_wide_copy port_wide_copies[];
extern const unsigned port_wide_copy_count;

#endif
