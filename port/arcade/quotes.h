/* Win quotes in the arcade's English (port/arcade/README.md, "Win quotes"). The PS1's win screen (CDEMOnn.PAC, the winner's file)
 * draws its quote as a sprite: a cell list of 16 x 16 glyph tiles from a tile segment of the file's sprite blob, placed by a position
 * stream, the list chosen through the winner kind's quote offset table. This code writes, for one winner kind, the arcade's four
 * quotes of that fighter in their place, with data only:
 *   - each quote's two lines (42 characters a line, centred with spaces) as runs of tiles, a tile two characters of the arcade's
 *     8 x 16 font side by side (a character's glyph is the arcade's scroll 1 pieces TOP and TOP + 0x10, TOP = 0x80 + (c & 15) +
 *     0x20 * ((c - 0x20) >> 4), for c from 0x21 to 0x7e; a space is blank);
 *   - the distinct tiles packed (tilepack.c) as the glyph segment, which must fit its room;
 *   - the cell lists (n, mode 0, 0, 0, stream offset, then the n tile numbers) and position streams (dx, dy, run - 1 a line) at the
 *     kind's lists and streams bases, and every entry of the kind's quote offset table pointing to quote (entry % 4).
 * Only addresses, offsets, counts and the font rule are here and in the generated table (quotes_table.c, from quotes.py --c-table):
 * no byte of either game. */
#ifndef ARCADE_QUOTES_H
#define ARCADE_QUOTES_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_QUOTE_LINE 42u          /* characters a line in the arcade's table */
#define ARCADE_QUOTE_LINES 2u          /* lines a quote */
#define ARCADE_QUOTES_PER_FIGHTER 4u

struct arcade_quote_fighter {
    const char *file;          /* "CDEMO0F.PAC": the winner's file, whose module copy and blob are written */
    uint16_t kind;             /* the winner kind (selects the module's per-kind tables) */
    uint32_t text;             /* arcade program address of the fighter's first quote (its 4 quotes follow, 2 lines of 42 words each) */
    uint32_t offsets;          /* module offset of the kind's quote offset table (u32 entries, each an offset from the lists base) */
    uint16_t offset_count;     /* its entries */
    uint32_t lists, lists_room;      /* module offset of the kind's lists base and the bytes that may be written from there */
    uint32_t streams, streams_room;  /* the same for the position streams */
    uint32_t segment, segment_room;  /* blob offset of the glyph segment and the bytes it may take */
};

struct arcade_quotes {
    uint16_t module_slot, blob_slot;   /* table 0 slots of the module and of the sprite blob */
    int16_t centre;                    /* the x a line is centred on, relative to where its first run's dx counts from */
    uint16_t line_height;              /* dy from one line to the next */
    const struct arcade_quote_fighter *fighters;
    unsigned count;
};

extern const struct arcade_quotes arcade_quotes;

/* Build fighter f's quotes: into module (msize bytes, the module chunk as read; changed gets a 1 at each byte written) and into
 * segment (room f->segment_room bytes: the packed glyph segment, *segment_size bytes of it). The program is the 68000's view, tiles
 * the graphics (128 bytes a 16 x 16 tile, the PS1's colour index one higher, as everywhere in this folder). -1 with a message in err
 * when a character is outside the font, a line has more than 20 tiles, a table lies outside the module, the lists or streams do not
 * fit their room, or the tiles do not pack in theirs; then nothing in module, changed or segment is valid. */
int arcade_quotes_build(const struct arcade_quotes *q, const struct arcade_quote_fighter *f, const uint8_t *program, size_t program_size,
                        const uint8_t *tiles, size_t tiles_size, uint8_t *module, uint8_t *changed, size_t msize, uint8_t *segment,
                        uint32_t *segment_size, char *err, size_t errsize);

#endif
