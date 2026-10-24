/* The rest of a module's data (after the code, and for a fighter module the blocks that fighter.c and sequence.c build; for a screen
 * or stage module the blocks that screen_lists.c, texts.c, quotes.c, talk.c and stage.c build): tables that are runs of the arcade
 * program, tables that are the arcade's records with their fields narrowed, the PS1 palette rows of the arcade's palettes, and the
 * pointer words, built from the arcade program alone. The model is module_data.py (its model_build is the specification; tools/test_arcade_module_data.py and
 * `module_data.py --compare-c` check this code against it). The generated table (module_data_table.c) has, per fighter file,
 * the pieces (module offset, size, arcade address, layout) and the pointer words (module offset, target): only offsets,
 * addresses and layouts, no byte of either game. The same table has a row for each file of the screen modules (END, CDEMO, CONT, DEMO,
 * SELECT, SELECTA, BOSS, RDM, GDEMO, STAGE) and its slot, with the link address of the image, which this code does not care about
 * except for the pointer words that name the module itself; rows of files that carry one module share their arrays.
 *
 * A piece's layout is text. A token is a letter and a field count, 'b16w4'; a group is tokens in parentheses and a repeat count,
 * '(w2n2)410' (a record repeated; no group inside a group). The letters, with the PS1 bytes a field makes and the arcade bytes
 * it is made from (the arcade program is the 68000's view: words big-endian):
 *   b  2 from 2   the two bytes as they are            w  2 from 2   the two bytes turned
 *   l  4 from 4   the four bytes turned                n  1 from 2   a byte, the 16-bit value if it is at most 255
 *   f  1 from 2   a flags byte: bit 15 becomes bit 7, bits 0 to 6 stay, no other bit may be set
 *   q  1 from 2   a byte that is the 16-bit value divided by 0x18 (it has to be a multiple, at most 255 times that)
 *   e  4 from 1   a 32-bit little-endian value from a byte
 *   j  4 from 8   a (cosine, sine) pair of the arcade's rotation table: two big-endian 32-bit values (sine first, cosine second), each
 *                 turned into the 16-bit little-endian value of its high half, cosine first (the PS1's Slot0bRot)
 *   k z K Z  32 from 32   a PS1 palette row from an arcade palette (15 big-endian words at the field's start; the 32 bytes of the field
 *                 are the arcade palette's stride, its last word is not read). Colour 0 is 0x0000 (transparent); colours 1 to 15 are
 *                 the words 0 to 14: brightness nibble (bits 12 to 15) ignored, R bits 8 to 11, G bits 4 to 7, B bits 0 to 3, the PS1
 *                 colour R * 2 | (G * 2) << 5 | (B * 2) << 10. k: a black colour becomes 0x8000; z: a black colour stays 0; K and Z:
 *                 the same with bit 15 set on all of colours 1 to 15 (screens.py's PAL_BIT15; a black colour is then 0x8000 either
 *                 way, so K and Z build the same bytes). Consecutive rows of an arcade run are a count: 'K17' is 17 palettes.
 * The first five are the letters of program.h's pieces (the walker here is equal to arcade_piece_build for them). */
#ifndef ARCADE_MODULE_DATA_H
#define ARCADE_MODULE_DATA_H

#include <stddef.h>
#include <stdint.h>

/* A relocation target below this is an offset in the module (the module's link address is added); any other is an address. */
#define ARCADE_MODULE_RELATIVE 0x80000000u

struct arcade_module_piece {
    uint32_t offset;       /* in the module */
    uint32_t size;         /* PS1 bytes the layout makes */
    uint32_t source;       /* address in the arcade program, even */
    const char *layout;
};

/* The 32-bit word at `offset` (4-aligned) is `target`, as ARCADE_MODULE_RELATIVE says. */
struct arcade_module_reloc {
    uint32_t offset;
    uint32_t target;
};

/* The names of the fighters that the PS1 keeps as text at the start of the shared module (slot 0xb): `count` names from the arcade's list of
 * 8-character names (12 bytes each, the first at `source`, an address that may be odd), taken last to first when `reverse`. A name is its
 * characters without the trailing spaces, centred in 7 characters (the spaces left of it are half of the free ones, rounded up), then "@@",
 * then a zero and zeros up to the next multiple of 4 of the record's start; the records follow one another from `offset` for `size` bytes. */
struct arcade_module_names {
    uint32_t offset, size, source;
    uint16_t count, stride, reverse;
};

/* The portrait shapes of the shared module (slot 0xb): for each of `count` fighters the arcade's portrait record (a mode 4 art record, blocks
 * of code, attribute, x, y; its address is the long at `steps` + `stride` * fighter) as cells of 16 x 16: each block makes its attribute's
 * width x height cells, a later block over an earlier at the same place, and the cells go in order of y, then x. Shape i has, in the module:
 * at `list_at` + 12 * i the cell count (a 16-bit value and a zero halfword), the cells' palettes (the attribute's low 5 bits less 2; one
 * byte a cell, each array starting on a multiple of 4 from `data_at`) and the cells' x and y (two 16-bit values a cell, the arrays one after
 * another from `coords_at`). */
struct arcade_module_shapes {
    uint32_t steps, stride, list_at, data_at, coords_at;
    uint16_t count;
};

struct arcade_module_data {
    const char *file;                        /* "PL00.PAC": the same files as arcade_fighters */
    uint16_t slot;                           /* table 0 slot of the module chunk (4 first side, 5 second side, or another code chunk's) */
    uint32_t load;                           /* its link address */
    uint32_t size;                           /* bytes of the module chunk */
    const struct arcade_module_piece *pieces;
    unsigned piece_count;
    const struct arcade_module_reloc *relocs;
    unsigned reloc_count;
    const struct arcade_module_names *names;     /* NULL, 0 for the modules that have none */
    unsigned name_count;
    const struct arcade_module_shapes *shapes;   /* NULL, 0 for the modules that have none */
    unsigned shape_count;
};

extern const struct arcade_module_data arcade_module_data[];
extern const unsigned arcade_module_data_count;

/* The row for a fighter file name ("PL00.PAC") and the table 0 slot of the module chunk (4 or 5 for the fighter module; the other code
 * chunks that some files carry have their own slots), or NULL. */
const struct arcade_module_data *arcade_module_data_find(const char *file, unsigned slot);

/* The piece's bytes into out[0..p->size): 0, or -1 when the layout is malformed or does not make exactly p->size bytes, the piece
 * does not fit the arcade program, or a value does not fit its letter. Nothing is written past out[p->size). */
int arcade_module_piece_build(const uint8_t *program, size_t program_size, const struct arcade_module_piece *p, uint8_t *out);

/* The row's own order and bounds: pieces sorted by offset, not overlapping, inside the module, even source, a well formed layout
 * that makes the piece's size; relocations sorted, 4-aligned, inside the module, not overlapping each other or a piece. 0 or -1. */
int arcade_module_data_valid(const struct arcade_module_data *d);

/* Build the row into `module` (module_size == d->size, the chunk as it will be read; bytes not built are left as they are, so
 * pass zeros) and set known[i] = 1 for every byte built (known may be NULL). A piece or word that cannot be built is skipped and
 * makes the result -1; the others are still built. 0 when all were. */
int arcade_module_data_build(const uint8_t *program, size_t program_size, const struct arcade_module_data *d,
                             uint8_t *module, uint8_t *known, size_t module_size);

#endif
