/* A PS1 stage file's pieces built from the arcade program and graphics alone: the three layer maps of the stage module, the
 * texture pages, the layer palettes, the object tiles and palettes, and the shared effect tiles. The rules are those of the
 * model in stages.py (its model_stage function is the specification; tools/test_arcade_stage.py and `stages.py --compare-c`
 * check this code against it and against the disc). Only addresses, counts and rules are here, no byte of either game.
 *
 * A piece is built whole, and `known` says which of its bytes the examined disc proved equal to the build (the descriptor's
 * flags, known bits, holes, exact-palette bits and runs of palettes): the rest of the piece is what the model makes of it, and the
 * disc gives the real bytes. In the pages every byte is known but the holes and the tiles that are not known. */
#ifndef ARCADE_STAGE_H
#define ARCADE_STAGE_H

#include <stddef.h>
#include <stdint.h>

/* struct arcade_stage_run.first: the arcade graphics tile (128 bytes) the run starts at, or one of these. */
#define ARCADE_STAGE_ZERO 0xfffffffeu   /* count transparent tiles (all zero) */
#define ARCADE_STAGE_DISC 0xffffffffu   /* count tiles the arcade does not have: the disc's */

/* Flags of a layer. Without MAP_EXACT / TILES_EXACT the layer's cell_known / tile_known bits say which cells and tiles are known. */
#define ARCADE_STAGE_MAP_EXACT 1u       /* the layer's cells in the module equal the disc's */
#define ARCADE_STAGE_TILES_EXACT 2u     /* the layer's tiles in the pages equal the disc's */
#define ARCADE_STAGE_START_RULE 4u      /* the layer (1 or 2) starts where the packing rule puts it: its start field is not used */

/* struct arcade_stage_exc.slot: the cell takes a new slot though an equal tile exists. */
#define ARCADE_STAGE_FORCE_NEW 0xffffffffu

/* Kinds of run of struct arcade_stage_more. */
#define ARCADE_STAGE_MORE_ROW 0u        /* arcade palette rows from the program address `arg` on, 0x20 apart */
#define ARCADE_STAGE_MORE_FILL 1u       /* filler rows: all fifteen colours the same fixed colour */

/* Consecutive arcade tiles first, first + 1, ...; flip bit 0 mirrors across, bit 1 down (the PS1 stores each flip as a tile). */
struct arcade_stage_run { uint32_t first, count, flip; };
/* Page slots (16 pixel tiles of the pages, column by column) the disc fills with tiles the arcade does not have. */
struct arcade_stage_hole { uint32_t slot, count; };
/* The same at page slots (16 pixel slots of the pages, column by column) from `slot`. */
struct arcade_stage_slots { uint32_t slot, count, first, flip; };

/* Where the disc's cell (counted in the scan order: blocks row by row, the cells of a block row by row) differs from the first-use
 * packing: the cell takes page slot start + slot (or ARCADE_STAGE_FORCE_NEW). Ascending by cell. */
struct arcade_stage_exc { uint32_t cell, slot; };

/* Palettes k, k + step, ... of the palette chunk (the ones no layer table names) are arcade rows or filler rows. */
struct arcade_stage_more { uint16_t k, count; uint8_t step, kind; uint32_t arg; };

struct arcade_stage_layer {
    uint32_t map;                /* byte offset of the layer's map in the module chunk */
    uint16_t across, down;       /* cells */
    uint16_t ids;                /* block ids (blocks across times blocks down, row by row) */
    uint32_t start;              /* page slot of the layer's blank tile; its tiles follow (unless ARCADE_STAGE_START_RULE) */
    unsigned flags;              /* ARCADE_STAGE_MAP_EXACT, ARCADE_STAGE_TILES_EXACT, ARCADE_STAGE_START_RULE */
    const uint32_t *id_at;       /* for each id the arcade program address of the byte that holds it (0: the constant 0) */
    const struct arcade_stage_exc *exc; unsigned exc_count;
    const uint64_t *cell_known;  /* bit i: cell i (scan order) equals the disc's; (across * down + 63) / 64 words, or NULL: none */
    const uint64_t *tile_known;  /* bit j: the layer's tile j equals the disc's; (across * down + 64) / 64 words, or NULL: none */
};

struct arcade_stage {
    const char *file;            /* "STAGE00.PAC" */
    unsigned stage;              /* the stage number the arcade's palette tables are indexed by */
    struct { uint32_t module, pages, palettes, object_tiles; } size;   /* bytes of the disc's chunks */
    uint64_t pal_exact[2];       /* bit k: layer palette k of the palette chunk equals the disc's */
    struct arcade_stage_layer layer[3];
    const struct arcade_stage_slots *extras; unsigned extra_count;   /* page slots holding arcade tiles apart from the layers */
    const struct arcade_stage_hole *holes; unsigned hole_count;      /* page slots the build cannot make (not known) */
    const struct arcade_stage_run *objects; unsigned object_count;   /* the object tile chunk */
    const struct arcade_stage_more *more; unsigned more_count;       /* the palettes that no layer table names */
};

extern const struct arcade_stage arcade_stages[];
extern const unsigned arcade_stage_count;
extern const struct arcade_stage_run *const arcade_stage_shared;     /* the shared effect tiles (the same in every stage file) */
extern const unsigned arcade_stage_shared_count;

const struct arcade_stage *arcade_stage_find(const char *file);

struct arcade_stage_piece { uint8_t *data, *known; uint32_t size; };   /* known[i] = 1: data[i] equals the disc's */

struct arcade_stage_out {
    struct arcade_stage_piece module;          /* only the three layer maps are built */
    struct arcade_stage_piece pages;
    struct arcade_stage_piece palettes;        /* the layer palettes and the runs of the others (the rest of the chunk is not built) */
    struct arcade_stage_piece object_tiles;
    struct arcade_stage_piece object_palettes;
};

/* Build the pieces of `s` from the arcade program (the 68000's view: words big-endian) and graphics. 0, or -1 with a message in
 * err (an id that names no block, an address or tile outside the arcade views, a layer that does not fit the module or the
 * pages, runs that do not add up to the chunk): then nothing stays allocated and `out` is zeroed. */
int arcade_stage_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                       const struct arcade_stage *s, struct arcade_stage_out *out, char *err, size_t errsize);
void arcade_stage_free(struct arcade_stage_out *out);

/* The shared effect tile chunk (slot 7 of every stage file), from arcade_stage_shared. Free it with arcade_stage_piece_free. */
int arcade_stage_build_shared(const uint8_t *tiles, size_t tiles_size, struct arcade_stage_piece *out, char *err, size_t errsize);
/* The same for any list of runs (tests). */
int arcade_stage_build_runs(const uint8_t *tiles, size_t tiles_size, const struct arcade_stage_run *runs, unsigned count,
                            uint32_t total, struct arcade_stage_piece *out, char *err, size_t errsize);
void arcade_stage_piece_free(struct arcade_stage_piece *p);

#endif
