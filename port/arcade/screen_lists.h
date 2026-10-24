/* The cell lists and position runs of the screen modules (select, character demo, continue, ending, attract), built from the arcade
 * program's art records and graphics. The rules are those of the model in screen_lists.py (its docstring says them, model_chain is the
 * specification; tools/test_arcade_screen_lists.py and `screen_lists.py --compare-c` check this code against it and against the disc).
 * Only addresses, counts and rules are here, no byte of either game.
 *
 * A module holds chains: the position runs, then the cell lists, one block each. A chain is a list of items in order: runs of lists made
 * from consecutive art records (REC), empty lists (EMPTY), and lists with no record (OWN: text lines, pictures cut from scroll data, the PS1's
 * own poses), of which only the byte lengths are known. The numbers in a list are indexes of tiles in a tile segment, found by looking the
 * cell's tile up in the segment: the segment is the screens recipe's (screens.h), so the numbers follow the tiles the loader builds.
 * `known` says which bytes were built; the rest of a block (the own lists and their position runs) is for the disc to give. */
#ifndef ARCADE_SCREEN_LISTS_H
#define ARCADE_SCREEN_LISTS_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_SCREEN_LISTS_PAL_SPLIT 1u   /* a change of palette between blocks of a record also starts a part (SELECTA) */

#define ARCADE_SCREEN_LISTS_REC 0u
#define ARCADE_SCREEN_LISTS_EMPTY 1u
#define ARCADE_SCREEN_LISTS_OWN 2u
#define ARCADE_SCREEN_LISTS_WIN 3u
#define ARCADE_SCREEN_LISTS_GRID 4u

/* REC: `count` lists from the records one after another from `address` (art records of mode 0, 2, 4 or 10; the cells of the grids of mode 10 and
 * of the portraits of mode 4 that the PS1 leaves out of its lists are left out here too), tile codes plus `bank`, palettes through map `map`
 * (index + 1 of arcade_screen_lists_maps, 0 for none). WIN: one list from window number `address` of arcade_screen_lists_windows (bank
 * 0x10000). EMPTY: `count` empty lists (one cell, tile 0; the position offset is the runs' size so far). OWN: one list of list_bytes bytes
 * that adds stream_bytes to the position runs. */
struct arcade_screen_lists_item {
    uint8_t kind;
    uint16_t count;
    uint32_t address, bank;
    uint16_t map;
    uint32_t list_bytes, stream_bytes;
};

/* GRID: one list cut from a frame of the arcade: the art record at `address` with `bank` added drawn (compose.h), moved by (ox, oy), cut into the 16 x 16
 * windows of the grid x = qx + 16 k, y = qy + 16 j that have a pixel, in row order; each window is found in the segment by its pixels (one number a picture
 * up to a flip), and the cells all take `palette`. The PS1 drew its poses and pictures this way: the tiles are not the arcade's but every pixel is (the
 * screens table's WINDOW items build them). `address` of the item indexes arcade_screen_lists_grids. */
struct arcade_screen_lists_grid { uint32_t address, bank; int16_t ox, oy; uint8_t qx, qy, palette; };

/* A cell of a REC or WIN item's list that differs from what the rules give: the list's index in the chain, the cell's index in the PS1's order
 * (before the cells that are dropped) and the value: a palette (0 to 31) that the cell keeps against the map; or DROP (the cell is left out); or
 * OWN_TILE with the palette in bits 0 to 4, the flip bits in bits 10 and 11 and the tile's number in the segment in bits 16 to 31, the cell showing a
 * tile of the PS1's own (the arcade's, cleaned) in place of the arcade's; number 0 means the next of the segment's own tiles after the
 * previous such cell of the list. */
#define ARCADE_SCREEN_LISTS_DROP 0x100u
#define ARCADE_SCREEN_LISTS_OWN_TILE 0x200u
struct arcade_screen_lists_exception { uint32_t list, cell, palette; };

/* A window of the arcade's scroll 2 blocks (blocks of 16 x 16 cells of code and attribute from SCROLL_FIRST, 1,024 bytes each, column by column):
 * w x h cells from cell (c0, r0) of the map whose block at region (rx, ry) = (column / 16, row / 16) is named by the window's blocks; the cell
 * (i, j) is drawn at (x0 + 16 i, y0 + 16 j); cells whose tile is blank are left out. */
struct arcade_screen_lists_window { int16_t x0, y0; uint8_t c0, r0, w, h; uint16_t first_block, block_count; };
struct arcade_screen_lists_win_block { uint8_t rx, ry; uint16_t block; };

struct arcade_screen_lists_pal { uint8_t from, to; };       /* palette map entry: an arcade palette number and the PS1's */
struct arcade_screen_lists_map { uint16_t first, count; };  /* a map: `count` entries from `first` */

struct arcade_screen_lists_chain {
    unsigned rules;
    uint32_t positions_size, lists_size;         /* the bytes of the runs and of the lists, as on the disc */
    uint32_t first_item, item_count, first_exception, exception_count;
};

/* Where a chain lies in a file and which tile segment it numbers into (the k-th compressed segment of the chunk table/slot/ordinal that
 * the screens table has a recipe for). */
struct arcade_screen_lists_use {
    const char *file;                            /* "CDEMO00.PAC" */
    uint16_t module_slot;
    uint32_t positions, lists;                   /* byte offsets in the module chunk */
    uint16_t chain;
    uint16_t pool_table, pool_slot, pool_ordinal, pool_segment;
};

extern const struct arcade_screen_lists_item arcade_screen_lists_items[];
extern const struct arcade_screen_lists_window arcade_screen_lists_windows[];
extern const struct arcade_screen_lists_grid arcade_screen_lists_grids[];
extern const struct arcade_screen_lists_win_block arcade_screen_lists_win_blocks[];
extern const struct arcade_screen_lists_exception arcade_screen_lists_exceptions[];
extern const struct arcade_screen_lists_pal arcade_screen_lists_pals[];
extern const struct arcade_screen_lists_map arcade_screen_lists_maps[];
extern const struct arcade_screen_lists_chain arcade_screen_lists_chains[];
extern const struct arcade_screen_lists_use arcade_screen_lists_uses[];
extern const unsigned arcade_screen_lists_item_count, arcade_screen_lists_window_count, arcade_screen_lists_grid_count, arcade_screen_lists_win_block_count, arcade_screen_lists_map_count, arcade_screen_lists_chain_count, arcade_screen_lists_use_count;

/* A tile of a segment, number k being pool[k]. kind 0: arcade tile `code` stored with `flip` (bit 0 across, bit 1 down); 1: a blank tile;
 * 2: not the arcade's (the PS1's own; never matched); 3: window `code` of the screens table (a tile cut from a frame of the arcade, matched by its pixels). */
struct arcade_screen_lists_tile { uint32_t code; uint8_t flip, kind; };

/* Everything a chain needs but the program and the graphics. */
struct arcade_screen_lists_spec {
    unsigned rules;
    uint32_t positions_size, lists_size;
    const struct arcade_screen_lists_item *items; unsigned item_count;
    const struct arcade_screen_lists_exception *exceptions; unsigned exception_count;
    const struct arcade_screen_lists_window *windows; unsigned window_count;
    const struct arcade_screen_lists_grid *grids; unsigned grid_count;
    const struct arcade_screen_lists_win_block *win_blocks; unsigned win_block_count;
    const struct arcade_screen_lists_pal *pals;
    const struct arcade_screen_lists_map *maps; unsigned map_count;
};

struct arcade_screen_lists_out {
    uint8_t *positions, *positions_known; uint32_t positions_size;   /* the position runs; known[i] = 1: built */
    uint8_t *lists, *lists_known; uint32_t lists_size;               /* the lists one after another */
};

/* Build one chain from the arcade program (the 68000's view) and graphics (128 bytes a tile) with the segment `pool` (count tiles). Returns 0
 * with out filled (arcade_screen_lists_free it), or -1 with a message in err (a record or tile outside its data, a tile the segment lacks,
 * sizes that differ from the spec's); then nothing stays allocated. */
int arcade_screen_lists_build_with(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                                   const struct arcade_screen_lists_tile *pool, unsigned pool_count, const struct arcade_screen_lists_spec *spec,
                                   struct arcade_screen_lists_out *out, char *err, size_t errsize);

/* The same for a row of arcade_screen_lists_uses: the spec from the generated table, the pool from the screens table's recipe of the use's file. */
int arcade_screen_lists_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                              const struct arcade_screen_lists_use *use, struct arcade_screen_lists_out *out, char *err, size_t errsize);

/* The tiles of the k-th compressed segment of a chunk of a screens file (a new array, free() it), or NULL with a message in err. */
struct arcade_screen_lists_tile *arcade_screen_lists_pool(const char *file, unsigned table, unsigned slot, unsigned ordinal, unsigned segment,
                                                          unsigned *count, char *err, size_t errsize);

void arcade_screen_lists_free(struct arcade_screen_lists_out *out);

/* For speed, optional: from begin to end the builds of these tiles keep each tile's class (its pixels up to a flip) for the next build, so
 * the caller must not change the tiles meanwhile, nor run builds on other threads. -1 when out of memory (the builds then work it out each
 * time). */
int arcade_screen_lists_begin(const uint8_t *tiles, size_t tiles_size);
void arcade_screen_lists_end(void);

/* The cell map of a screen module's backdrop (SELECTA's chunk 0x26: 10,240 cells of a texture word and a palette word), a window of the arcade's
 * scroll 1 block sets: `rows` rows of `columns` cells; row r is row y0 + r % band of band r / band, where the band's columns come from runs: a run of
 * `columns` cells from the set `set + step * band` (32 x 32 cells) starting at its column x. A cell's piece (8 x 8 pixels of the graphics) is looked up
 * among the 8 x 8 units of the backdrop's picture (the chunk the screens table builds: 3 pages of 32 x 32 units) for the texture word
 * ((first_page + page) << 10 | row << 5 | column), and the palette word is clut + 64 * attribute (bits 0 to 4), or clut for a blank piece. */
struct arcade_screen_lists_backdrop_run { uint16_t columns, set, step, x; };
struct arcade_screen_lists_backdrop {
    const char *file;
    uint16_t slot, picture_slot;                 /* table 0 chunks of the file: the cell map and the picture (ordinal 0) */
    uint32_t picture_offset;                     /* where the picture starts in its chunk (the chunk is a TIM: header and palettes first) */
    uint16_t columns, rows, band, y0, first_page, clut;
    unsigned run_count;
    struct arcade_screen_lists_backdrop_run runs[4];
};
extern const struct arcade_screen_lists_backdrop arcade_screen_lists_backdrops[];
extern const unsigned arcade_screen_lists_backdrop_count;

/* Build the map into out[0 .. columns * rows * 4) with the mask `known` (1: built; a cell whose piece the picture does not hold, or whose unit is
 * not all known, is 0 and its bytes zero). picture and picture_known are the picture chunk's bytes (the picture starts at +24608 of the TIM chunk:
 * pass the chunk's picture bytes, `picture_size` of them, 384 bytes a row) and its mask. Returns 0, or -1 with a message in err. */
int arcade_screen_lists_backdrop_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                                       const struct arcade_screen_lists_backdrop *bd, const uint8_t *picture, const uint8_t *picture_known,
                                       size_t picture_size, uint8_t *out, uint8_t *known, size_t out_size, char *err, size_t errsize);

#endif
