/* The rest of a fighter module's data (after the code and the blocks that fighter.c and sequence.c build): tables that are runs
 * of the arcade program, tables that are the arcade's records with their fields narrowed, and the pointer words, built from the
 * arcade program alone. The model is module_data.py (its model_build is the specification; tools/test_arcade_module_data.py and
 * `module_data.py --compare-c` check this code against it). The generated table (module_data_table.c) has, per fighter file,
 * the pieces (module offset, size, arcade address, layout) and the pointer words (module offset, target): only offsets,
 * addresses and layouts, no byte of either game.
 *
 * A piece's layout is text. A token is a letter and a field count, 'b16w4'; a group is tokens in parentheses and a repeat count,
 * '(w2n2)410' (a record repeated; no group inside a group). The letters, with the PS1 bytes a field makes and the arcade bytes
 * it is made from (the arcade program is the 68000's view: words big-endian):
 *   b  2 from 2   the two bytes as they are            w  2 from 2   the two bytes turned
 *   l  4 from 4   the four bytes turned                n  1 from 2   a byte, the 16-bit value if it is at most 255
 *   f  1 from 2   a flags byte: bit 15 becomes bit 7, bits 0 to 6 stay, no other bit may be set
 *   q  1 from 2   a byte that is the 16-bit value divided by 0x18 (it has to be a multiple, at most 255 times that)
 *   e  4 from 1   a 32-bit little-endian value from a byte
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

struct arcade_module_data {
    const char *file;                        /* "PL00.PAC": the same files as arcade_fighters */
    uint16_t slot;                           /* table 0 slot of the module chunk (4 first side, 5 second side, or another code chunk's) */
    uint32_t load;                           /* its link address */
    uint32_t size;                           /* bytes of the module chunk */
    const struct arcade_module_piece *pieces;
    unsigned piece_count;
    const struct arcade_module_reloc *relocs;
    unsigned reloc_count;
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
