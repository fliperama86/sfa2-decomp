/* The arcade's art records and scroll 2 blocks as cells, and the cells drawn into a canvas of PS1 pixels. One parser of the records
 * for the loader: screen_lists.c reads its lists' cells through arcade_compose_record, and the screens' windows (a 16 x 16 tile cut from
 * a drawn record or from drawn scroll 2 blocks) are cut from the canvas. The model is compose.py (it reads the records through
 * screen_lists.py's record_blocks and block_parts and draws with its pixel functions); tools/test_arcade_compose.py checks this code
 * against it. Only addresses, counts and rules are here, no byte of either game. */
#ifndef ARCADE_COMPOSE_H
#define ARCADE_COMPOSE_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_COMPOSE_COORD_TABLE 0x9f260u   /* arcade: table of pointers to coordinate blocks, indexed by the record's coordinate index */
#define ARCADE_COMPOSE_SCROLL_ADD 0xa400u     /* modes 4 and 10, scroll 2: the tile is ((code + SCROLL_ADD) & 0xffff) + 0x10000 */
#define ARCADE_COMPOSE_SCROLL_FIRST 0x2c0300u /* the arcade's scroll 2 blocks: 222 blocks of 1,024 bytes (16 x 16 cells of code and attribute, column by column) */
#define ARCADE_COMPOSE_SCROLL_BLOCK 1024u
#define ARCADE_COMPOSE_SCROLL_BLOCKS 222u
#define ARCADE_COMPOSE_MAX_DIM 512u           /* the largest canvas side in pixels */

/* A cell of a record: the tile code with the bank added, the position, the attribute's low 7 bits (palette 0 to 4, flip across 0x20, flip
 * down 0x40) and the part (blocks start a new part when their x differs from the block before by other than a multiple of 16, or, with
 * the palette rule, when their palette differs). */
struct arcade_compose_cell {
    uint32_t code;
    int32_t x, y;
    uint16_t flags, part;
};

struct arcade_compose_info {
    unsigned mode;        /* 0, 2, 4 or 10 */
    unsigned declared;    /* the cells the record says it has (equal to count) */
    unsigned count;       /* cells returned */
    uint32_t end;         /* the address after the record's own bytes (not its coordinate blocks) */
};

/* The art record at `address` of the 68000's view as cells, all of them (blank ones too), in record order: block by block, row by row and left to
 * right inside a block, the codes mirrored where the block's attribute flips it (so the cell at a position holds the code the flipped block puts there). Mode 2: blocks of code and attribute, coordinates through the index. Mode 0: a
 * shared attribute and a code a cell, each a block of 1 x 1. Mode 4 (the portraits): blocks of code, attribute, x, y with the tile code
 * plus ARCADE_COMPOSE_SCROLL_ADD (16 bits). Mode 10 (a grid, height and width first): pairs of code and attribute column by column, the
 * lower left corner at (0, 0), the same tile rule. `bank` is added to every code. `pal_split` applies the palette rule to the parts of
 * modes 0 and 2. At most `max` cells (a record declaring more is refused). Returns 0, or -1 with a line in err ("record 0x..: ..."). */
int arcade_compose_record(const uint8_t *prog, size_t psize, uint32_t address, uint32_t bank, unsigned pal_split, struct arcade_compose_cell *cells,
                          unsigned max, struct arcade_compose_info *info, char *err, size_t errsize);

/* The code and attribute of cell (column c, row r) of scroll 2 block `block`, and its tile code ((code + SCROLL_ADD) & 0xffff) + 0x10000.
 * Returns -1 when the block does not exist or lies outside the program. */
int arcade_compose_scroll2_cell(const uint8_t *prog, size_t psize, unsigned block, unsigned c, unsigned r, uint32_t *tile, uint16_t *attr);

/* The 256 PS1 pixels of the 128 bytes of an arcade tile (value plus one, modulo 16; four planes, 8 pixels to a byte, 4 bytes to 8 pixels). */
void arcade_compose_tile_pixels(const uint8_t *t, uint8_t px[256]);

/* The PS1 pixels (one byte each, 0 transparent) of arcade tile `code` of `tiles` flipped across (flip bit 0) and/or down (bit 1). -1 when the
 * code lies outside the graphics. */
int arcade_compose_tile(const uint8_t *tiles, size_t tsize, uint32_t code, unsigned flip, uint8_t px[256]);

/* A canvas: w x h pixels one byte each, whose top left pixel is (x0, y0) in the source's coordinates (a record's, or a block run's from
 * its first block's top left). Cells drawn later cover earlier ones where their pixel is not 0. */
struct arcade_canvas { int32_t x0, y0; uint32_t w, h; uint8_t *px; };

/* Draw cells (tile of `tiles`, flips from flags) into a canvas that is their bounding box; no cell leaves w = h = 0 and px NULL. -1 with a
 * line in err for a tile outside the graphics or a box wider or taller than ARCADE_COMPOSE_MAX_DIM. */
int arcade_compose_draw(const uint8_t *tiles, size_t tsize, const struct arcade_compose_cell *cells, unsigned count, struct arcade_canvas *out,
                        char *err, size_t errsize);

/* The record drawn (arcade_compose_record then arcade_compose_draw), cells in record order. */
int arcade_compose_record_canvas(const uint8_t *prog, size_t psize, const uint8_t *tiles, size_t tsize, uint32_t address, uint32_t bank,
                                 struct arcade_canvas *out, char *err, size_t errsize);

/* `blocks` (1 or 2) scroll 2 blocks from `first_block` drawn side by side, every cell: 256 x 256 pixels a block, origin (0, 0). */
int arcade_compose_scroll2(const uint8_t *prog, size_t psize, const uint8_t *tiles, size_t tsize, unsigned first_block, unsigned blocks,
                           struct arcade_canvas *out, char *err, size_t errsize);

/* The 16 x 16 tile with its top left at (x, y) of the canvas's coordinates as 128 bytes (8 a row, the left pixel of a pair in the low
 * nibble); pixels outside the canvas are 0. rows: 0 every row, 1 only the odd rows of the tile (the others 0), 2 only the even rows. */
void arcade_compose_cut(const struct arcade_canvas *c, int32_t x, int32_t y, unsigned rows, uint8_t out[128]);

void arcade_compose_free(struct arcade_canvas *c);

#endif
