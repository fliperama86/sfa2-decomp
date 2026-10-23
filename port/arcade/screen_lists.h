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

/* REC: `count` lists from the records one after another from `address`, tile codes plus `bank`, palettes through map `map` (index + 1 of
 * arcade_screen_lists_maps, 0 for none). EMPTY: `count` empty lists (one cell, tile 0; the position offset is the runs' size so far).
 * OWN: one list of list_bytes bytes that adds stream_bytes to the position runs. */
struct arcade_screen_lists_item {
    uint8_t kind;
    uint16_t count;
    uint32_t address, bank;
    uint16_t map;
    uint32_t list_bytes, stream_bytes;
};

/* A cell of a REC item's list that keeps another palette than its map gives: the list's index in the chain, the cell's index in the PS1's order. */
struct arcade_screen_lists_exception { uint32_t list, cell, palette; };

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
extern const struct arcade_screen_lists_exception arcade_screen_lists_exceptions[];
extern const struct arcade_screen_lists_pal arcade_screen_lists_pals[];
extern const struct arcade_screen_lists_map arcade_screen_lists_maps[];
extern const struct arcade_screen_lists_chain arcade_screen_lists_chains[];
extern const struct arcade_screen_lists_use arcade_screen_lists_uses[];
extern const unsigned arcade_screen_lists_item_count, arcade_screen_lists_map_count, arcade_screen_lists_chain_count, arcade_screen_lists_use_count;

/* A tile of a segment, number k being pool[k]. kind 0: arcade tile `code` stored with `flip` (bit 0 across, bit 1 down); 1: a blank tile;
 * 2: not the arcade's (the PS1's own; never matched). */
struct arcade_screen_lists_tile { uint32_t code; uint8_t flip, kind; };

/* Everything a chain needs but the program and the graphics. */
struct arcade_screen_lists_spec {
    unsigned rules;
    uint32_t positions_size, lists_size;
    const struct arcade_screen_lists_item *items; unsigned item_count;
    const struct arcade_screen_lists_exception *exceptions; unsigned exception_count;
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

#endif
