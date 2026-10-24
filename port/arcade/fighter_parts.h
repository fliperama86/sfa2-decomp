/* The fighter files' palettes (table 1, 64 sprite palettes of 16 colors) and picture pages (table 2, 256 x 256 pictures of
 * 4 bits a pixel) built from the arcade program and graphics. The rules are those of the model in fighter_parts.py (its
 * docstring says what each is and how it was found; tools/test_arcade_fighter_parts_c.py and `fighter_parts.py
 * --compare-c` check this code against it). Only addresses, counts and rules are here, no byte of either game.
 *
 * What the arcade does not give stays the disc's: the caller serves a built palette or page in place of the disc's only where
 * it is a whole (a palette kept by `keep` is left zero and is the disc's; a page flagged ARCADE_PARTS_PAGE_DISC is not built). */
#ifndef ARCADE_FIGHTER_PARTS_H
#define ARCADE_FIGHTER_PARTS_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_PARTS_PALETTE_BYTES 2048u   /* a table 1 chunk: 64 palettes of 32 bytes */
#define ARCADE_PARTS_PAGE_BYTES 32768u     /* a table 2 chunk: 256 x 256 pixels, 4 bits each, low nibble first */
#define ARCADE_PARTS_RAMPS 18u             /* filler ramp slots of a file (see ramps below) */
#define ARCADE_PARTS_NO_RAMP 0xffffu

#define ARCADE_PARTS_FLAT 0u               /* layout of the second block of odd palettes: palettes in a row */
#define ARCADE_PARTS_FIVES 1u              /* runs of 5 palettes (one per button) of which a run's first 2 are used */

#define ARCADE_PARTS_PAGE_DISC 1u          /* flags: the file's portrait page is the PS1's own, it stays the disc's */
#define ARCADE_PARTS_PAGE_OWNED 2u         /* flags: the portrait page is built whole the arcade's way and differs from the disc's by design (the
                                            * 18 portrait palettes are the fighter's arcade ones where the PS1 has its own colours): served owned */

/* A cell of the shared (VS) page that a portrait page holds besides its own: the tile at cell (vx, vy) of the shared page, placed at (cx, cy). */
struct arcade_parts_cell {
    uint8_t cx, cy, vx, vy;
};

struct arcade_parts_file {
    const char *file;          /* "PL00.PAC" */
    uint8_t fighter;           /* the arcade's fighter number (0 Ryu ... 0x12) whose palettes and portrait the file holds */
    uint8_t layout;            /* ARCADE_PARTS_FLAT or ARCADE_PARTS_FIVES */
    uint8_t split;             /* odd palettes p = n >> 1 below it are at 0x272ae0 + 0x200 * fighter + 0x20 * p; from it on in the second block */
    uint8_t palette_chunks;    /* table 1 chunks in the file, all alike (1, or 2 in PL15 to PL17) */
    uint32_t base;             /* arcade address of the second block of odd palettes */
    uint64_t keep;             /* bit n: palette n of the chunk stays the disc's (the PS1's own colors) */
    const uint16_t *ramps;     /* NULL, or 18 start words of the PS1's filler ramps (ARCADE_PARTS_NO_RAMP: the slot is the arcade's palette) */
    const char *pages;         /* the file's table 2 chunks in order: 'P' the fighter's portrait page, 'V' the shared page */
    uint16_t shadow;           /* bit i: the portrait page holds shadow cell i (the PS1's own choice, 12 cells) */
    uint8_t flags;             /* ARCADE_PARTS_* */
    int8_t drop_x, drop_y;     /* the one cell of the portrait the PS1 left blank, or -1 */
    const struct arcade_parts_cell *cells;   /* cells of the shared page the portrait page holds besides its shadow cells (NULL, 0 for most) */
    unsigned cell_count;
};

extern const struct arcade_parts_file arcade_parts_files[];
extern const unsigned arcade_parts_count;

/* Build the palette chunk of `f` into out[ARCADE_PARTS_PALETTE_BYTES] from the arcade program (the 68000's view). Palette n is
 * 16 little-endian halfwords: 0, then the arcade palette's first 15 colors doubled per channel (black as 0x8000), or a filler
 * ramp. Palettes of f->keep are left zero. On success returns 0 and *kept (if not NULL) is f->keep. On a palette outside the
 * program or without an address returns -1 with a line in err (and out is not trusted). */
int arcade_parts_palettes(const uint8_t *program, size_t program_size, const struct arcade_parts_file *f,
                          uint8_t *out, uint64_t *kept, char *err, size_t errsize);

/* Build the fighter's portrait page of `f` into out[ARCADE_PARTS_PAGE_BYTES] from the arcade program and the graphics (128-byte
 * tiles). Returns 0 when built, 1 when the page stays the disc's (nothing written), -1 on malformed input (a record or tile
 * outside its view, a flipped block, a cell outside the page) with a line in err. */
int arcade_parts_portrait(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                          const struct arcade_parts_file *f, uint8_t *out, char *err, size_t errsize);

/* The same for the shared page (the same in every file). Returns 0, or -1 with a line in err. */
int arcade_parts_shared(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                        uint8_t *out, char *err, size_t errsize);

#endif
