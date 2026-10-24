/* The name plates' cell maps (NDEMOnn.PAC, table 0 slot 0x11), built from the arcade's scroll 1 block sets. Every file of the win screen's name plate holds a map of
 * 64 x 32 cells (a texture word and a palette word each: 0x2000 bytes), 32 palettes and one 256 x 256 page of 8 x 8 units. The plate is a window of the select
 * backdrop's strips (screen_lists.h): a stage's thumbnail and the ribbon beside it, 8 rows of the 64 columns from row 3, taken from the arcade's scroll 1:
 * columns 0 to 6 from set 0, 7 to 31 from set 64 + band (the thumbnail), 32 to 63 from set 84 (the ribbon), rows 3 to 10 of each. A cell's 8 x 8 piece is looked up among
 * the page's units for its texture word ((22 << 10) | (row << 5) | column of the first unit, rows then columns, that equals the piece), and its palette word is
 * 0x7800 + 64 * the attribute's palette (0x7800 for a blank piece). The cells outside the strip, and the cells whose piece the page lacks (or holds in a part that is not
 * built) get a free unit of the page, written with the piece; only a cell with no free unit or no graphics tile is the constant cell (texture 0x5800, palette 0x7800: a blank unit). The model is plates.py; tools/test_arcade_plates.py and `plates.py --compare-c` check
 * this code against it and against the disc. Only addresses and rules are here, no byte of either game. */
#ifndef ARCADE_PLATES_H
#define ARCADE_PLATES_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_PLATE_MAP_BYTES 0x2000u   /* 64 x 32 cells of 4 bytes */
#define ARCADE_PLATE_PAGE_AT 0x2400u     /* the page's offset in the chunk (after the map and the 32 palettes) */
#define ARCADE_PLATE_PAGE_BYTES 32768u   /* 256 x 256 pixels of 4 bits */
#define ARCADE_PLATE_SLOT 0x11u          /* the chunk's slot in table 0 */

struct arcade_plate { const char *file; uint8_t band; };   /* "NDEMO0F.PAC", the strip of the stage that the file shows (set 64 + band) */
extern const struct arcade_plate arcade_plates[];
extern const unsigned arcade_plate_count;

/* The map of the strip `band` into map[ARCADE_PLATE_MAP_BYTES], from the arcade's program and graphics and the chunk's page (page_known[i] = 1 for the page bytes that
 * are built: a unit with an unbuilt byte is not looked up). A piece that the page lacks (the PS1 kept those few cells some other way) is written into a free unit of
 * the page, in place: a unit that no cell names and that is built and blank, the first by rows then columns; units[i] (1,024 bytes) gets 1 for each unit i (row * 32 +
 * column) written, so that the caller serves it. *filled gets the cells of the strip that stayed the constant cell (no free unit, or no graphics tile for the piece).
 * Returns 0, or -1 with a message in err. */
int arcade_plate_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size, unsigned band, uint8_t *page,
                       const uint8_t *page_known, uint8_t *map, uint8_t *units, unsigned *filled, char *err, size_t errsize);

#endif
