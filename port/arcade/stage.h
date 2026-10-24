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
#define ARCADE_STAGE_BLANK 0xfffffffdu  /* count tiles the arcade does not have and does not show: blank by the owner's rule ("rebuild
                                         * arcade-style"), by design not equal to the disc's. A chunk whose runs have BLANK ones and no DISC
                                         * one is built whole and `owned` (see arcade_stage_piece). */

/* Flags of a layer. Without MAP_EXACT / TILES_EXACT the layer's cell_known / tile_known bits say which cells and tiles are known. */
#define ARCADE_STAGE_MAP_EXACT 1u       /* the layer's cells in the module equal the disc's */
#define ARCADE_STAGE_TILES_EXACT 2u     /* the layer's tiles in the pages equal the disc's */
#define ARCADE_STAGE_START_RULE 4u      /* the layer (1 or 2) starts where the packing rule puts it: its start field is not used */
#define ARCADE_STAGE_L0_OWN 8u          /* layer 0 only: built by the own layer 0 rule (stages.py, "our own layer 0"), nothing of it known: its cells,
                                         * tiles (the pages' first slots up to layer 1's start, but for l0_reserved) and 32 palette rows are owned */

/* struct arcade_stage_exc.slot: the cell takes a new slot though an equal tile exists. */
#define ARCADE_STAGE_FORCE_NEW 0xffffffffu

/* Kinds of run of struct arcade_stage_more. */
#define ARCADE_STAGE_MORE_ROW 0u        /* arcade palette rows from the program address `arg` on, 0x20 apart */
#define ARCADE_STAGE_MORE_FILL 1u       /* filler rows: all fifteen colours the same fixed colour */
#define ARCADE_STAGE_MORE_APPROX 2u     /* the arcade row at `arg` standing for a row of the disc that differs from it: built, not equal to the disc's, `own` */
#define ARCADE_STAGE_MORE_BLANK 3u      /* a transparent row (all zero): the disc's row has no arcade source and the arcade shows nothing like it, `own` */

/* Consecutive arcade tiles first, first + 1, ...; flip bit 0 mirrors across, bit 1 down (the PS1 stores each flip as a tile). */
struct arcade_stage_run { uint32_t first, count, flip; };
/* Bits of an extras run's flip beyond the flips (bit 0 across, bit 1 down): the run's codes are tiles of the arcade's 32 x 32 pixel
 * scroll 3 graphics (512 bytes each) and each page slot holds one quarter of such a tile, as sprite_tiles.h ARCADE_SPRITE_SCROLL3. */
#define ARCADE_STAGE_SCROLL3 8u
#define ARCADE_STAGE_QUARTER_SHIFT 4u      /* bits 4 and 5: the quarter, 0 top left, 1 top right, 2 bottom left, 3 bottom right (taken, then flipped) */

/* Page slots (16 pixel tiles of the pages, column by column) the disc fills with tiles the arcade does not have. */
struct arcade_stage_hole { uint32_t slot, count; };
/* The same at page slots (16 pixel slots of the pages, column by column) from `slot`; first is an arcade tile (16 pixel), or with
 * ARCADE_STAGE_SCROLL3 in flip a 32 pixel one, consecutive tiles to consecutive slots. */
struct arcade_stage_slots { uint32_t slot, count, first, flip; };

/* Where the disc's cell (counted in the scan order: blocks row by row, the cells of a block row by row) differs from the first-use
 * packing: the cell takes page slot start + slot (or ARCADE_STAGE_FORCE_NEW). Ascending by cell. */
struct arcade_stage_exc { uint32_t cell, slot; };

/* Palettes k, k + step, ... of the palette chunk (the ones no layer table names) are arcade rows or filler rows. */
struct arcade_stage_more { uint16_t k, count; uint8_t step, kind; uint32_t arg; };

/* An extra map of a layer (1 or 2): the arcade's animation frames of the layer, kept by the PS1 as further maps in the stage module. It is
 * a stack of pages of two blocks side by side (block h is page h / 2, left or right half; 32 x 16 cells of 16 pixels or 16 x 8 of
 * 32 pixels a page), each block one of the arcade's. The cells follow the layer's own in the packing scan (a block's cells row by row,
 * the blocks in order), so they share the layer's tiles; the layer's cell_known and tile_known bits go on counting through them. */
struct arcade_stage_xmap {
    uint32_t map;                /* byte offset of the extra map in the module chunk */
    uint16_t blocks;             /* number of blocks (an even number: whole pages) */
    const uint8_t *ids;          /* the arcade block numbers */
};

/* Runs of consecutive indexes (cells in scan order, or tiles of the layer). */
struct arcade_stage_span { uint32_t first, count; };

struct arcade_stage_layer {
    uint32_t map;                /* byte offset of the layer's map in the module chunk */
    uint16_t across, down;       /* cells */
    uint16_t ids;                /* block ids (blocks across times blocks down, row by row) */
    uint32_t start;              /* page slot of the layer's blank tile; its tiles follow (unless ARCADE_STAGE_START_RULE) */
    unsigned flags;              /* ARCADE_STAGE_MAP_EXACT, ARCADE_STAGE_TILES_EXACT, ARCADE_STAGE_START_RULE */
    const uint32_t *id_at;       /* for each id the arcade program address of the byte that holds it (0: the constant 0) */
    const struct arcade_stage_exc *exc; unsigned exc_count;
    const uint64_t *cell_known;  /* bit i: cell i (scan order) equals the disc's; (cells + 63) / 64 words, or NULL: none */
    const uint64_t *tile_known;  /* bit j: the layer's tile j equals the disc's; (cells + 64) / 64 words, or NULL: none */
    const struct arcade_stage_xmap *xmaps; unsigned xmap_count;   /* layers 1 and 2 only; `cells` above counts them (across * down plus theirs) */
    /* By design (the owner's rule "rebuild arcade-style"), built as the arcade has them where the PS1 differs, served owned: `own` is set for
     * the cells' four bytes in the module and the tiles' slots in the pages, `known` is not. Cells are scan indexes of the layer's own cells. */
    const struct arcade_stage_span *own_cells; unsigned own_cell_count;
    const struct arcade_stage_span *own_tiles; unsigned own_tile_count;   /* tiles of the layer (the packing's order) */
    /* Layer 0 with ARCADE_STAGE_L0_OWN: cells of the layer's own blocks (scan indexes) that lie in another layer's map and are not built, and the
     * page slots before layer 1's start that lists of the module name and that stay the disc's. xmaps of layer 0 are lists of scroll 1 set numbers. */
    const struct arcade_stage_span *l0_skip; unsigned l0_skip_count;
    const struct arcade_stage_span *l0_reserved; unsigned l0_reserved_count;
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
/* The second palette chunk of a stage file (the same in every one): runs of palettes that are arcade rows or filler rows; the other
 * palettes of the chunk are not built. arcade_stage_shared_pal_bytes is the chunk's size (0 and no runs: not described). */
extern const struct arcade_stage_more *const arcade_stage_shared_pal;
extern const unsigned arcade_stage_shared_pal_count;
extern const unsigned arcade_stage_shared_pal_bytes;

const struct arcade_stage *arcade_stage_find(const char *file);

/* known[i] = 1: data[i] equals the disc's. owned = 1: every byte of data is built from the arcade alone and is to be served to every
 * read, whether or not it equals the disc's (known still marks the bytes that do, which a check compares). owned is set only by the
 * object tile chunk, when its runs have BLANK ones and no DISC one. */
struct arcade_stage_piece { uint8_t *data, *known; uint32_t size; int owned; uint8_t *own; };
/* own[i] = 1: data[i] is built from the arcade alone by design (an APPROX or BLANK palette run), differs from the disc's and is to be served to
 * every read, as a whole piece that is `owned` is; the palette pieces have them. known and own are never both set. */

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
/* The second palette chunk of every stage file, from arcade_stage_shared_pal: only the program is read. known marks the palettes built. */
int arcade_stage_build_shared_palettes(const uint8_t *program, size_t program_size, struct arcade_stage_piece *out, char *err, size_t errsize);
/* The same for any list of palette runs, into a chunk of `total` bytes (tests). */
int arcade_stage_build_palette_runs(const uint8_t *program, size_t program_size, const struct arcade_stage_more *runs, unsigned count,
                                    uint32_t total, struct arcade_stage_piece *out, char *err, size_t errsize);
/* The same for any list of runs (tests). */
int arcade_stage_build_runs(const uint8_t *tiles, size_t tiles_size, const struct arcade_stage_run *runs, unsigned count,
                            uint32_t total, struct arcade_stage_piece *out, char *err, size_t errsize);
void arcade_stage_piece_free(struct arcade_stage_piece *p);

#endif
