/* The tiles, palettes and pictures of the select, demo, continue, ending, boss and character-demo screens, built from the arcade
 * program and graphics. The rules are those of the model in screens.py (its docstring says what a recipe is; model_chunk is the
 * specification; tools/test_arcade_screens.py and `other_files.py --compare-c` check this code against it and against the disc).
 * Only addresses, counts and layouts are here, no byte of either game.
 *
 * A chunk of a disc file (table, slot, ordinal) has a recipe: a list of regions, byte ranges of the chunk that are rebuilt. What is
 * outside the regions (headers, code, the PS1's own art) is not built, and a region's own parts (tiles and cells the arcade lacks)
 * are marked unknown: the caller serves a built byte in place of the disc's only where `known` says so.
 *
 * Compressed tile segments (kind SEG) are packed by tilepack.c into the room the disc's segment has, so their bytes never equal the
 * disc's; they are marked 2 ("decodes to the disc's tiles") when every tile is known. A tile the arcade does not have is taken from
 * the disc's segment (the `disc` argument of arcade_screens_build); without it the segment is not built. */
#ifndef ARCADE_SCREENS_H
#define ARCADE_SCREENS_H

#include <stddef.h>
#include <stdint.h>

/* struct arcade_screens_item.first: an arcade tile (or palette address), or one of these. */
#define ARCADE_SCREENS_ZERO 0xfffffffeu     /* count blank tiles or palettes */
#define ARCADE_SCREENS_DISC 0xffffffffu     /* count tiles or palettes the arcade does not have */
#define ARCADE_SCREENS_PIECES 0xfffffffdu   /* one tile of four pieces: flip indexes arcade_screens_pieces */
#define ARCADE_SCREENS_FLAT 0xfffffff0u     /* a piece id at or above this is a flat piece of value id & 15 */

#define ARCADE_SCREENS_BLACK_ZERO 1u        /* palette flag: a black colour stays 0 (else it is 0x8000) */
#define ARCADE_SCREENS_BIT15 2u             /* palette flag: bit 15 of colours 1 to 15 is set */

/* Tile items: count consecutive arcade tiles first, first + 1, ... with the flip (bit 0 across, bit 1 down) of every one.
 * Palette items: count arcade palettes at address first, first + 32, ... with the flags as `flip`.
 *
 * By design: an item whose flip carries this bit differs from the disc's on purpose, and verify does not compare it with the disc. A ZERO item is
 * blank on purpose, not because the disc is blank there. A tile item (a run of arcade tiles, a WINDOW) is the arcade's tile where the PS1 edited its
 * own (the arcade's art unclipped, say). The build takes none of these tiles from the disc. No other use of the flip reaches this bit (the tile flip
 * is 2 bits, the palette flags 2, a PIECES index is below the piece count, a WINDOW index below the window count); the bit on a DISC or PIECES item,
 * and on a palette item that is not ZERO, is refused.
 *
 * WINDOW: one tile (count 1) cut from a frame of the arcade: flip (without the by-design bit) indexes arcade_screens_windows. */
#define ARCADE_SCREENS_BY_DESIGN 0x80000000u
#define ARCADE_SCREENS_WINDOW 0xfffffffcu   /* a tile item: one tile cut from a drawn arcade frame */
struct arcade_screens_item { uint32_t first, count, flip; };

/* A frame of the arcade drawn into a canvas (compose.h): an art record (mode 0, 2, 4 or 10) at `address` with `bank` added to its tile codes, or
 * `blocks` (1 or 2) scroll 2 blocks from block `address` side by side. The PS1 cut some frames on its own 16-pixel grid: a window is the tile with
 * its top left at (x, y) of the canvas's coordinates (a record's, or the blocks' from the first one's top left), the rows of it kept (0 all, 1 odd,
 * 2 even), and the stored flip (bit 0 across, bit 1 down) the PS1 keeps the tile in. */
#define ARCADE_SCREENS_SRC_RECORD 1u
#define ARCADE_SCREENS_SRC_SCROLL2 2u
struct arcade_screens_source { uint32_t address, bank; uint16_t blocks; uint8_t kind; };
struct arcade_screens_window { uint16_t source; int16_t x, y; uint8_t flip, rows; };

/* Picture ops (x, y in units of 8 pixels; w, h in tiles or pieces; the unit at (i, j) of a rect is first + i * stepx + j * stepy). */
#define ARCADE_SCREENS_OP_TILE 1u    /* 16 x 16 arcade tiles; (x, y) is a multiple of 2 for a cell grid but need not be */
#define ARCADE_SCREENS_OP_PIECE 2u   /* 8 x 8 pieces of scroll 1: piece k is the left half of arcade tile k >> 1, rows 0-7 for even k, 8-15 for odd k */
#define ARCADE_SCREENS_OP_FLAT 3u    /* pieces of the single pixel value `first` */
#define ARCADE_SCREENS_OP_DISC 4u    /* units that stay the disc's (zero and unknown) */
#define ARCADE_SCREENS_OP_BLANK 5u   /* units (8 x 8 pixels, w * h of them) blank by design: zero, differing from the disc's on purpose */
struct arcade_screens_op { uint32_t first; int32_t stepx, stepy; uint16_t x, y, w, h; uint8_t kind, flip; };

#define ARCADE_SCREENS_SEG 1u        /* a compressed tile segment (offset table, then a block a tile) */
#define ARCADE_SCREENS_RAW 2u        /* a plain list of 128-byte tiles */
#define ARCADE_SCREENS_PALS 3u       /* palettes of 32 bytes, one every `width` slots (the slots between are zero) */
#define ARCADE_SCREENS_PIC 4u        /* a picture of 4 bits a pixel, `width` pixels wide */
struct arcade_screens_region {
    uint8_t kind;
    uint32_t offset, size;           /* the byte range of the chunk */
    uint32_t width;                  /* PIC: pixels a row; PALS: slots from one palette to the next */
    uint32_t first, count;           /* the region's items (SEG, RAW, PALS) or ops (PIC) in the tables */
};

struct arcade_screens_recipe { uint32_t size; uint16_t first, count; };     /* the chunk's size, its regions */
struct arcade_screens_use { uint16_t table, slot, ordinal, recipe; };
struct arcade_screens_file { const char *file; uint16_t first, count; };    /* the chunks of "BOSS00.PAC" that have recipes */

extern const struct arcade_screens_item arcade_screens_items[];
extern const struct arcade_screens_op arcade_screens_ops[];
extern const uint32_t arcade_screens_pieces[][4];
extern const struct arcade_screens_source arcade_screens_sources[];
extern const struct arcade_screens_window arcade_screens_windows[];
extern const struct arcade_screens_region arcade_screens_regions[];
extern const struct arcade_screens_recipe arcade_screens_recipes[];
extern const struct arcade_screens_use arcade_screens_uses[];
extern const struct arcade_screens_file arcade_screens_files[];
extern const unsigned arcade_screens_file_count, arcade_screens_recipe_count, arcade_screens_piece_count, arcade_screens_source_count, arcade_screens_window_count;

/* The index in arcade_screens_sources of the frame (kind, address, bank, blocks), or -1: for a builder that needs the frame that the tiles of a window were cut
 * from (the sources are keyed by what they draw, not by a number that a regeneration of the table may change). */
int arcade_screens_source_index(unsigned kind, uint32_t address, uint32_t bank, unsigned blocks);

const struct arcade_screens_file *arcade_screens_find(const char *file);
/* The recipe of a chunk of a file (NULL when it has none). */
const struct arcade_screens_recipe *arcade_screens_recipe_of(const struct arcade_screens_file *f, unsigned table, unsigned slot, unsigned ordinal);

struct arcade_screens_out {
    uint8_t *data, *known;           /* the chunk, and for each byte 1 (built, equal to the disc's), 2 (served as built: a packed segment whose tiles
                                      * are the disc's but for the by-design ones, or bytes blank by design in a list of tiles, palettes or picture,
                                      * which differ from the disc's on purpose and are not compared with it) or 0 */
    uint32_t size;
    unsigned unbuilt;                /* regions left to the disc: a segment with tiles missing and no disc, or that does not pack into its room */
    unsigned design;                 /* units blank by design: tiles and palettes of by-design items, 8 x 8 pieces of OP_BLANK ops (counted whether or not the region could be built) */
};

/* Build the regions of `r` into a zeroed chunk of r->size bytes. `program` is the arcade's 68000 view, `tiles` its graphics (128 bytes a
 * tile). `disc` (may be NULL) is the disc's chunk, read only for the tiles of a segment that the arcade lacks. Returns 0, or -1 with
 * a message in err (a code, address or op outside its range, items that do not fit their region); then nothing stays allocated. */
int arcade_screens_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                         const struct arcade_screens_recipe *r, const uint8_t *disc, size_t disc_size,
                         struct arcade_screens_out *out, char *err, size_t errsize);
void arcade_screens_free(struct arcade_screens_out *out);

/* The stored tiles (128 bytes each, as the build puts them in a segment) of windows w[0], ... w[n - 1] of the checked-in table (indexes into arcade_screens_windows),
 * for a builder that numbers cells by the pixels of the tiles (screen_lists.c). The frames are drawn once each for windows that follow one another. 0, or -1 with a
 * message in err (a window or source outside the table, a frame that does not draw). */
int arcade_screens_window_tiles(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size, const uint32_t *w, unsigned n,
                                uint8_t (*out)[128], char *err, size_t errsize);

/* The first word of a chunk. The game's disc stream compares the first word of each chunk it loads with the first word of the chunk's record in the archive
 * header and reads the file again when they differ, so a built chunk must start with the header's word (the chunk's first word on the disc). A chunk whose first
 * bytes are built by design (blank, or an arcade tile in place of the PS1's) takes `word` (little-endian) there: the bytes 0 to 3 that are marked 2 and are not in
 * a packed segment (whose first word, the offset table's, is the disc's) get the word and the mark 1. The caller reads the word from the archive header
 * (the entry's first word, 8 bytes into the record) when it builds. A packed segment at the chunk's start keeps its offset table's first word; if that is not
 * the header's word the segment is left to the disc (nothing built, `unbuilt` counts it): the stream would otherwise read the file again. */
void arcade_screens_head_word_regions(const struct arcade_screens_region *regions, unsigned count, struct arcade_screens_out *out, uint32_t word);
void arcade_screens_head_word(const struct arcade_screens_recipe *r, struct arcade_screens_out *out, uint32_t word);

/* Check a built chunk against the disc's: every byte marked 1 must equal the disc's, and every packed segment (marked 2) must decode to the
 * disc's tiles, the by-design tiles excepted (bytes marked 2 outside a segment are by design and not compared). Returns 0 when they agree; else -1 with a message in err that names the first region or byte that differs (a disc that is
 * not the examined one). `r` and `out` come from arcade_screens_build for the same chunk. */
int arcade_screens_verify(const struct arcade_screens_recipe *r, const struct arcade_screens_out *out, const uint8_t *disc, size_t disc_size,
                          char *err, size_t errsize);

/* The same for any list of regions (tests); `items` is the item table their SEG regions index (NULL: no by-design tiles). */
int arcade_screens_verify_regions(const struct arcade_screens_region *regions, unsigned count, const struct arcade_screens_item *items,
                                  const struct arcade_screens_out *out, const uint8_t *disc, size_t disc_size, char *err, size_t errsize);

/* The same for one made-up region (tests): the region's items and ops are the caller's, `pieces` the quads its PIECES items index, `sources` and
 * `windows` what its WINDOW items index. */
int arcade_screens_build_region(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                                const struct arcade_screens_region *g, const struct arcade_screens_item *items,
                                const struct arcade_screens_op *ops, const uint32_t (*pieces)[4], unsigned piece_count,
                                const struct arcade_screens_source *sources, unsigned source_count, const struct arcade_screens_window *windows, unsigned window_count,
                                const uint8_t *disc, size_t disc_size, struct arcade_screens_out *out, char *err, size_t errsize);

#endif
