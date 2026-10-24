/* Widescreen's side tiles, the maker (port/widescreen.md, "The side tiles"): the packets of the tile columns that the game's own
 * layer functions do not draw. Pure: it reads a layer record and a map through host pointers and hands each packet to a callback,
 * so port/tools/wide_stage_test.c can compare it with the game's C. wide_stage.c has the overrides that use it. */
#ifndef PORT_WIDE_STAGE_H
#define PORT_WIDE_STAGE_H

#include <stdint.h>

/* The forms of the stages' layer functions (the C is the same in every copy of a form; the copies differ in the pool). Kinds:
 *  PORT_WIDE_FLAT     plain tile grids from a layer record (struct Chan): draw_tile_layer_0_16px, _1_16px, draw_tilemap_layer_32px_9x13 and
 *                     the forms that are the same arithmetic with other counts or a flag (shape fields below);
 *  PORT_WIDE_SPRITES  draw_tile_layer_sprites_with_partial_row (the second layer, a row count from the camera, a last partial row) and
 *                     its relatives (variant PORT_WIDE_V_BIRDIE: draw_scrolling_tile_map_layer of birdie);
 *  PORT_WIDE_SKEWED   draw_skewed_tile_layer_quads (the second layer in perspective: 44 cells a row from k = -11, each a quad whose x is
 *                     moved by an offset that depends on the row only);
 *  PORT_WIDE_QUADS    draw_tile_map_layer_polys of nash: 24 unskewed quads a row, in a fixed entry of the ordering table;
 *  PORT_WIDE_SCROLLC  draw_scrolling_tile_map_layer of chunli_alt: 16 rows of 25 cells, the first eight with the horizontal scroll from a
 *                     global and the last eight with the layer's own. */
enum { PORT_WIDE_FLAT = 0, PORT_WIDE_SPRITES, PORT_WIDE_SKEWED, PORT_WIDE_QUADS, PORT_WIDE_SCROLLC };
/* variants of the kinds above */
enum { PORT_WIDE_V_NONE = 0, PORT_WIDE_V_ROLENTO16, PORT_WIDE_V_ROLENTO32, PORT_WIDE_V_BIRDIE };
struct port_wide_shape {
    int shift;           /* the cell is 1 << shift pixels: 4 or 5 */
    int rows, cols;      /* the cells the C draws: 16 x 25, or 9 x 13 */
    int words;           /* the packet's words after the tag (the record's tag length): 6, or 7 with the sprite's width and height, 9 for a quad */
    int unsigned_blank;  /* the C compares the u16 cell with the s16 blank as ints (draw_tile_layer_0_16px); else both as s16 */
    int row_inclusive;   /* the C skips a row when `rows < row` (the 32 px form), not `row >= rows` */
    int kind;            /* PORT_WIDE_FLAT ... PORT_WIDE_SCROLLC */
    int k_first;         /* the C's first column: 0, or -11 for the skewed form (its cols = 44 columns run from there) */
    int half_scroll;     /* the C subtracts (s16)game_state.scroll_y >> 1 (a layer that scrolls at half speed), not scroll_y */
    int variant;         /* PORT_WIDE_V_*: the rows and the map's lines are not the plain ones (rolento's two grids, birdie's rows) */
    int second_blank;    /* the C also skips the cell that equals the next layer's blank (the layer record is then 0x120 bytes: both layers) */
    int camera_rows;     /* the row count comes from the camera (field_4c) as nash's draw_tile_layer_sprites_saving_pos does, plus one */
    int map_offset;      /* halfwords added to the layer's map pointer before the C reads the map (0x800 in nash's quads) */
    int fixed_entry;     /* 1 + the entry of the ordering table when it is not the layer's ot_index (nash's quads: 14 + 1), else 0 */
};
extern const struct port_wide_shape
    port_wide_shape_L0, port_wide_shape_L1, port_wide_shape_T32, port_wide_shape_SPR, port_wide_shape_SKEW,
    port_wide_shape_T32_7, port_wide_shape_T16H, port_wide_shape_T32H, port_wide_shape_T32G,
    port_wide_shape_L0K, port_wide_shape_L0R, port_wide_shape_L0G, port_wide_shape_L0N, port_wide_shape_SAVE, port_wide_shape_POLY,
    port_wide_shape_SKEWP, port_wide_shape_R16, port_wide_shape_R32, port_wide_shape_SPRI, port_wide_shape_SCRB, port_wide_shape_SCRC;

/* The layer record (struct Chan, 0x90 bytes) in the game's order; the maker reads these fields of it. With shape->second_blank the
 * record of the next layer follows it (0x120 bytes). */
#define PORT_WIDE_CHAN_SIZE 0x90u

/* A packet: `len` words (the GP0 commands) for entry `entry` of the ordering table; 0, or not 0 when it was not taken. This is the
 * signature of port_gpu_queue_packet. */
typedef int (*port_wide_emit)(int entry, const uint32_t *words, unsigned len);

/* The packets of the columns k0 <= k < k1 (k = shape->k_first is the C's first column, smaller k left of it, k >= k_first + cols right of the
 * last; for the skewed form every column asked for is made, visible or not), in the
 * C's order (row by row, then column), for the layer record `layer` (host pointer, PORT_WIDE_CHAN_SIZE bytes), its map `map` (the
 * bytes the layer's map_data points to, `map_bytes` of them; a cell past them is taken as blank), the game's scroll_y
 * (game_state.scroll_y in the low 16 bits; for PORT_WIDE_SCROLLC the s16 global at 0x8019054e in the high 16) and the pool's first record `record` (shape->words + 1 words: the tag, whose length byte must equal
 * shape->words, and the static words). Returns the number of packets made; stops at the first that `emit` refuses. */
unsigned port_wide_columns(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                           unsigned game, const uint32_t *record, int k0, int k1, port_wide_emit emit);

/* The packets of the columns beside the C's that the wide view can show (what the override queues): for the plain and sprite forms the
 * columns of port_wide_side_range; for the skewed form, per row, the cells outside k = -11..32 (at most PORT_WIDE_SKEW_EXTRA more each
 * side) whose quad reaches into screen x -128..512. Returns the number of packets made. */
#define PORT_WIDE_SKEW_EXTRA 24
unsigned port_wide_sides(const struct port_wide_shape *shape, const uint8_t *layer, const uint8_t *map, uint32_t map_bytes,
                         unsigned game, const uint32_t *record, port_wide_emit emit);

/* How much of the map the maker reads for this layer: *preferred bytes (the whole map and, for the forms whose C reads one row past it, a
 * block row more) and *minimum (what is needed); 0 in *minimum when the layer cannot be drawn. The caller maps that much of the game's
 * memory and gives it as map_bytes. */
void port_wide_map_bytes(const struct port_wide_shape *shape, const uint8_t *layer, unsigned game, uint32_t *preferred, uint32_t *minimum);
/* The game's global that PORT_WIDE_SCROLLC reads (an s16), packed into the high half of the `game` word. */
#define PORT_WIDE_SCROLL_GLOBAL 0x8019054eu

/* The columns of the sides (plain and sprite forms): k_left (<= -1) is the first column left of the C's that is visible in the wide view (screen x from -128 to
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
