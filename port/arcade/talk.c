/* The talk before a fight in the arcade's English; see talk.h. The specification is talk.py (the derivation of the table, the model the
 * --compare-c drawing follows). Project-authored, plain C99. No byte of either game is here. */
#include <stdio.h>
#include <string.h>

#include "talk.h"

#define TILE_BYTES 128u
#define WORD_BYTES 2u
#define LINE_BYTES (ARCADE_TALK_COLUMNS * WORD_BYTES)
#define BOX_BYTES (ARCADE_TALK_LINES * LINE_BYTES)
#define PAGE_ROW_BYTES 128u                /* 256 pixels of 4 bits */
#define BOX_ROWS 48u
#define LINE_ROWS 16u
#define FONT_FIRST 0x21u
#define FONT_LAST 0x7eu
#define FONT_TILES 0x10000u                /* the arcade tile of scroll 1 piece 0 */
#define RECORD_BYTES 6u
#define END_BIT 0x8000u

static int fail(char *err, size_t errsize, const char *fmt, unsigned a, unsigned b)
{
    if (err && errsize)
        snprintf(err, errsize, fmt, a, b);
    return -1;
}

/* The arcade value of pixel (x, y) of a 16 x 16 tile: four planes, 8 pixels to a byte, 4 bytes to 8 pixels, 8 bytes a row. */
static unsigned arcade_px(const uint8_t *t, unsigned x, unsigned y)
{
    const uint8_t *p = t + y * 8u + (x >> 3) * 4u;
    unsigned sh = 7u - (x & 7u);
    return ((p[0] >> sh) & 1u) | (((p[1] >> sh) & 1u) << 1) | (((p[2] >> sh) & 1u) << 2) | (((p[3] >> sh) & 1u) << 3);
}

/* The PS1 pixel (x in 0..7, y in 0..15) of the glyph of character c (the arcade's scroll 1 pieces TOP, rows 0 to 7, and TOP + 0x10, rows 8
 * to 15, a piece k being the left half of arcade tile FONT_TILES + (k >> 1), rows 0 to 7 for an even k and 8 to 15 for an odd one); a space
 * is blank; the colour is the arcade's plus one. -1 when the tile lies outside the graphics. */
static int glyph_px(const uint8_t *tiles, size_t tsize, unsigned c, unsigned x, unsigned y)
{
    unsigned k, row;
    uint64_t at;
    if (c == 0x20u)
        return 0;
    k = 0x80u + (c & 15u) + 0x20u * ((c - 0x20u) >> 4) + (y >= 8u ? 0x10u : 0u);
    row = (k & 1u) * 8u + (y & 7u);
    at = ((uint64_t)FONT_TILES + (k >> 1)) * TILE_BYTES;
    if (at + TILE_BYTES > tsize)
        return -1;
    return (int)((arcade_px(tiles + at, x, row) + 1u) & 15u);
}

/* The character of one 16-bit big-endian word of a line: 0x20 for a space (0x0000, 0x8000, or a space code), 0x21..0x7e, or -1. The high
 * byte is 0x00 or 0x80 (the arcade's own flag), the low byte the ASCII code with bit 7 set. */
static int word_char(const uint8_t *p)
{
    unsigned hi = p[0], lo = p[1], c;
    if (hi == 0 && lo == 0)
        return 0x20;
    if (hi != 0 && hi != 0x80)
        return -1;
    if (lo == 0)
        return hi == 0x80 ? 0x20 : -1;
    if (lo < 0x80)
        return -1;
    c = lo & 0x7fu;
    if (c == 0x20u)
        return 0x20;
    return c >= FONT_FIRST && c <= FONT_LAST ? (int)c : -1;
}

/* The arcade box that page box b draws: box_of[b], a box beyond the talk's last one repeating that one. */
static unsigned arcade_box(const struct arcade_talk_page *p, unsigned b)
{
    unsigned a = p->box_of[b];
    return a >= p->boxes ? (unsigned)p->boxes - 1u : a;
}

static int check_page(const struct arcade_talk_page *p, size_t program_size, char *err, size_t errsize)
{
    unsigned b;
    if (!p->boxes || p->boxes > ARCADE_TALK_BOXES)
        return fail(err, errsize, "a talk has %u boxes (1 to %u)", p->boxes, ARCADE_TALK_BOXES);
    if ((uint64_t)p->text + (uint64_t)p->boxes * BOX_BYTES > program_size)
        return fail(err, errsize, "the talk at %#x lies outside the program (%u)", p->text, (unsigned)program_size);
    for (b = 0; b < ARCADE_TALK_BOXES; b++)
        if (p->box_of[b] >= ARCADE_TALK_BOXES)
            return fail(err, errsize, "box %u of the page draws an arcade box past 3 (%u)", b, p->box_of[b]);
    return 0;
}

int arcade_talk_page_build(const struct arcade_talk_page *p, const uint8_t *program, size_t program_size, const uint8_t *tiles,
                           size_t tiles_size, uint8_t *out, char *err, size_t errsize)
{
    unsigned b, l, k, x, y;
    if (err && errsize)
        err[0] = 0;
    if (!p || !program || !tiles || !out)
        return fail(err, errsize, "a null argument (%u, %u)", 0, 0);
    if (check_page(p, program_size, err, errsize))
        return -1;
    /* read every character first, so that nothing is written when one is outside the font */
    {
        uint8_t chars[ARCADE_TALK_BOXES][ARCADE_TALK_LINES][ARCADE_TALK_COLUMNS];
        for (b = 0; b < ARCADE_TALK_BOXES; b++)
            for (l = 0; l < ARCADE_TALK_LINES; l++)
                for (k = 0; k < ARCADE_TALK_COLUMNS; k++) {
                    uint32_t at = p->text + arcade_box(p, b) * BOX_BYTES + l * LINE_BYTES + k * WORD_BYTES;
                    int c = word_char(program + at);
                    if (c < 0)
                        return fail(err, errsize, "the word at %#x is outside the font (%u)", at, 0);
                    chars[b][l][k] = (uint8_t)c;
                    for (y = 0; y < LINE_ROWS && c != 0x20; y++)
                        for (x = 0; x < 8; x++)
                            if (glyph_px(tiles, tiles_size, (unsigned)c, x, y) < 0)
                                return fail(err, errsize, "the glyph of %#x lies outside the graphics (%u)", (unsigned)c, 0);
                }
        memset(out, 0, ARCADE_TALK_PAGE);
        for (b = 0; b < ARCADE_TALK_BOXES; b++)
            for (l = 0; l < ARCADE_TALK_LINES; l++)
                for (k = 0; k < ARCADE_TALK_COLUMNS; k++) {
                    unsigned c = chars[b][l][k];
                    if (c == 0x20u)
                        continue;
                    for (y = 0; y < LINE_ROWS; y++)
                        for (x = 0; x < 8; x += 2) {
                            unsigned a = (unsigned)glyph_px(tiles, tiles_size, c, x, y), d = (unsigned)glyph_px(tiles, tiles_size, c, x + 1u, y);
                            out[(BOX_ROWS * b + LINE_ROWS * l + y) * PAGE_ROW_BYTES + (8u * k + x) / 2u] = (uint8_t)(a | d << 4);
                        }
                }
    }
    return 0;
}

/* The lines of arcade box a of the talk: up to and including the last line that has a character. 0 for a blank box. */
static int box_lines(const struct arcade_talk_page *p, const uint8_t *program, unsigned a, char *err, size_t errsize)
{
    unsigned l, k, last = 0;
    for (l = 0; l < ARCADE_TALK_LINES; l++)
        for (k = 0; k < ARCADE_TALK_COLUMNS; k++) {
            uint32_t at = p->text + a * BOX_BYTES + l * LINE_BYTES + k * WORD_BYTES;
            int c = word_char(program + at);
            if (c < 0)
                return fail(err, errsize, "the word at %#x is outside the font (%u)", at, 0);
            if (c != 0x20)
                last = l + 1u;
        }
    return (int)last;
}

int arcade_talk_script_build(const struct arcade_talk_page *p, const uint8_t *program, size_t program_size, uint8_t *module,
                             uint8_t *changed, size_t msize, char *err, size_t errsize)
{
    uint8_t lines[ARCADE_TALK_BOXES];
    unsigned i, b;
    if (err && errsize)
        err[0] = 0;
    if (!p || !program || !module || !changed)
        return fail(err, errsize, "a null argument (%u, %u)", 0, 0);
    if (check_page(p, program_size, err, errsize))
        return -1;
    if (!p->records || (uint64_t)p->script + (uint64_t)p->records * RECORD_BYTES > msize)
        return fail(err, errsize, "the script at %#x lies outside the module (%u)", p->script, (unsigned)msize);
    /* validate the whole script before writing a byte */
    for (i = 0; i < p->records; i++) {
        const uint8_t *r = module + p->script + i * RECORD_BYTES;
        unsigned w0 = r[0] | (unsigned)r[1] << 8;
        int last = i + 1u == p->records;
        if (last && !(w0 & END_BIT))
            return fail(err, errsize, "the last record of the script at %#x has no end bit (%u)", p->script, 0);
        if (!last && (w0 & END_BIT))
            return fail(err, errsize, "record %u of the script ends it before the last one (%u)", i, 0);
        if (!last && (w0 & 0xffu) >= ARCADE_TALK_BOXES)
            return fail(err, errsize, "record %u names box %u, past 3", i, w0 & 0xffu);
    }
    for (b = 0; b < ARCADE_TALK_BOXES; b++) {
        int n = box_lines(p, program, arcade_box(p, b), err, errsize);
        if (n < 0)
            return -1;
        lines[b] = (uint8_t)(n ? n : 1);
    }
    for (i = 0; i + 1u < p->records; i++) {
        uint8_t *r = module + p->script + i * RECORD_BYTES;
        unsigned box = r[0];
        if (p->box_of[box] >= p->boxes) {   /* the arcade's talk has no such box: the script ends here (only bit 15 of w0 is read then) */
            r[1] |= (uint8_t)(END_BIT >> 8);
            changed[p->script + i * RECORD_BYTES + 1u] = 1;
            break;
        }
        r[4] = lines[box];
        r[5] = 0;
        changed[p->script + i * RECORD_BYTES + 4u] = 1;
        changed[p->script + i * RECORD_BYTES + 5u] = 1;
    }
    return 0;
}
