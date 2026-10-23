/* A fighter module's position runs and cell lists, and the PS1's tile numbering, built from the arcade program
 * alone (the art records of the arcade's sprite lists). The rules are those of the model in fighters.py (its
 * model_build function is the specification; tools/test_arcade_fighter.py and `fighters.py --compare-c` check this
 * code against it). Only addresses, counts and rules are here, no byte of either game. */
#ifndef ARCADE_FIGHTER_H
#define ARCADE_FIGHTER_H

#include <stddef.h>
#include <stdint.h>

/* The per-file rules (the rules field of struct arcade_fighter). */
#define ARCADE_FIGHTER_PALETTE_PARTS 1u   /* a change of palette between blocks of a record also starts a part (PL0E) */
#define ARCADE_FIGHTER_NUMBER_BY_RUN 2u   /* tiles of consecutive records of several parts are numbered unit by unit (PL0E) */
#define ARCADE_FIGHTER_TERMINATOR 4u      /* the lists end with an empty list whose position offset is the positions' size */
/* Not a flag, because the model has it for every file: a record drawn a second time in the file, whose leftmost cell is
 * left of x = -256, has that cell moved to x = 0 and its position stream stored apart. */

#define ARCADE_FIGHTER_SAME_CHUNK 0xffffu /* positions_slot: the positions lie in the chunk of module_slot */

/* A run of consecutive lists made from the arcade's art records. The records follow one another in the arcade
 * program from `address` (mode 2, or mode 0 with one tile code a cell), each list one record; their tile codes get
 * `bank` added (the video chip's code high bits, 0 to 3 times 0x10000). address 0 stands for `count` empty lists
 * (one cell, tile 0), which have no record. */
struct arcade_fighter_segment {
    uint32_t address;
    uint16_t count;
    uint32_t bank;
};

/* A cell of a record that the PS1 leaves out: the record's address and the cell's index in the record's own order (blocks in
 * order, each block row by row). Lists are sorted by record, then cell. */
struct arcade_fighter_drop {
    uint32_t record;
    uint16_t cell;
};

struct arcade_fighter {
    const char *file;                   /* "PL00.PAC" */
    uint16_t module_slot;               /* table 0 slot of the chunk holding the lists (and the positions, see below) */
    uint16_t positions_slot;            /* table 0 slot of the chunk holding the positions, or ARCADE_FIGHTER_SAME_CHUNK */
    uint32_t positions, lists;          /* byte offsets of the two blocks in their chunks */
    uint32_t positions_room, lists_room; /* bytes the PS1 has for each (the disc's sizes) */
    unsigned rules;                     /* ARCADE_FIGHTER_* flags */
    const struct arcade_fighter_segment *segments;
    unsigned segment_count;
    const struct arcade_fighter_drop *drops;   /* cells of those records left out of the lists (none for a fighter file: NULL, 0) */
    unsigned drop_count;
};

extern const struct arcade_fighter arcade_fighters[];
extern const unsigned arcade_fighter_count;

struct arcade_fighter_out {
    uint8_t *positions; uint32_t positions_size;   /* the position runs (three little-endian 16-bit values a run) */
    uint8_t *lists; uint32_t lists_size;           /* the cell lists, one after another, as the PS1 reads them */
    uint32_t tile_count;                           /* entries of the two arrays below: index 0 is reserved, 1 .. tile_count - 1 are tiles */
    uint32_t *tile_code;                           /* the arcade tile code (bank included) that PS1 tile number i holds */
    uint8_t *tile_flip;                            /* how the PS1 stores it: bit 0 flipped across, bit 1 flipped down */
};

/* Build the blocks of `f` from the arcade program (the 68000's view: words big-endian). Fills `out` (allocated; free it
 * with arcade_fighter_free); -1, with a message in err, on malformed input (a record outside the program, blocks that
 * do not add up to the record's cells, a value that does not fit its field) or when a block needs more than its room.
 * On failure nothing stays allocated and `out` is zeroed. */
int arcade_fighter_build(const uint8_t *program, size_t program_size, const struct arcade_fighter *f,
                         struct arcade_fighter_out *out, char *err, size_t errsize);
void arcade_fighter_free(struct arcade_fighter_out *out);

#endif
