/* The win quotes in the arcade's English; see quotes.h. The specification is quotes.py (the derivation of the table, the model the
 * --compare-c rendering follows). Project-authored, plain C99. No byte of either game is here. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quotes.h"
#include "tilepack.h"

#define TILE_BYTES 128u
#define MAX_RUN 20u                                   /* tiles a line may have (a screen's width of 16 pixel tiles) */
#define MAX_PAIRS (ARCADE_QUOTES_PER_FIGHTER * ARCADE_QUOTE_LINES * MAX_RUN)
#define WORD_BYTES 2u
#define LINE_BYTES (ARCADE_QUOTE_LINE * WORD_BYTES)
#define FONT_FIRST 0x21u
#define FONT_LAST 0x7eu
#define FONT_TILES 0x10000u                           /* the arcade tile of scroll 1 piece 0 */
#define MAX_TILES 0x3fffu                             /* the game's cell numbers are 14 bits */

struct line {
    unsigned n;                                       /* characters after trimming */
    uint8_t ch[ARCADE_QUOTE_LINE];
};

struct quote {
    struct line line[ARCADE_QUOTE_LINES];
    unsigned cells;                                   /* tiles of the quote */
    uint16_t cell[MAX_RUN * ARCADE_QUOTE_LINES];
    unsigned runs;                                    /* stream triples */
    uint16_t stream[3u * ARCADE_QUOTE_LINES];
    uint32_t list_at, stream_at;                      /* offsets in the lists and in the streams area */
};

static int fail(char *err, size_t errsize, const char *fmt, unsigned a, unsigned b)
{
    if (err && errsize)
        snprintf(err, errsize, fmt, a, b);
    return -1;
}

/* ---- the arcade's font */

/* The arcade value of pixel (x, y) of a 16 x 16 tile: four planes, 8 pixels to a byte, 4 bytes to 8 pixels, 8 bytes a row. */
static unsigned arcade_px(const uint8_t *t, unsigned x, unsigned y)
{
    const uint8_t *p = t + y * 8u + (x >> 3) * 4u;
    unsigned sh = 7u - (x & 7u);
    return ((p[0] >> sh) & 1u) | (((p[1] >> sh) & 1u) << 1) | (((p[2] >> sh) & 1u) << 2) | (((p[3] >> sh) & 1u) << 3);
}

/* The PS1 pixel (x, y), x and y in 0..7 and 0..15, of the glyph of character c: scroll 1 piece TOP (rows 0 to 7) and TOP + 0x10 (rows 8 to
 * 15), a piece k being the left half of arcade tile FONT_TILES + (k >> 1), rows 0 to 7 for an even k and 8 to 15 for an odd one. A space is
 * blank. The PS1's colour index is the arcade's plus one. -1 when the tile lies outside the graphics. */
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

/* The 128-byte tile of two characters side by side. */
static int make_tile(const uint8_t *tiles, size_t tsize, unsigned c1, unsigned c2, uint8_t out[TILE_BYTES])
{
    unsigned x, y;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x += 2) {
            int a = glyph_px(tiles, tsize, x < 8 ? c1 : c2, x & 7u, y);
            int b = glyph_px(tiles, tsize, (x + 1u) < 8 ? c1 : c2, (x + 1u) & 7u, y);
            if (a < 0 || b < 0)
                return -1;
            out[y * 8u + x / 2u] = (uint8_t)(a | b << 4);
        }
    return 0;
}

/* ---- the arcade's text */

/* The character of one 16-bit word of a line (big-endian in the program): 0x20 for a space, 0x21..0x7e for a font character, or -1.
 * 0x0000 and 0x8000 are spaces; the high byte is 0x00 or 0x80 (the arcade's own flag) and the low byte the ASCII code with bit 7 set. */
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

static int read_line(const uint8_t *program, uint32_t at, struct line *l, char *err, size_t errsize)
{
    uint8_t raw[ARCADE_QUOTE_LINE];
    unsigned i, first = 0, last = 0;
    for (i = 0; i < ARCADE_QUOTE_LINE; i++) {
        int c = word_char(program + at + i * WORD_BYTES);
        if (c < 0)
            return fail(err, errsize, "character %u of the line at %#x is outside the font", i, at);
        raw[i] = (uint8_t)c;
    }
    while (first < ARCADE_QUOTE_LINE && raw[first] == 0x20)
        first++;
    for (last = ARCADE_QUOTE_LINE; last > first && raw[last - 1] == 0x20; last--)
        ;
    l->n = last - first;
    memcpy(l->ch, raw + first, l->n);
    if ((l->n + 1u) / 2u > MAX_RUN)
        return fail(err, errsize, "the line at %#x has more than %u tiles", at, MAX_RUN);
    return 0;
}

/* ---- tiles: the distinct pairs of characters, numbered from 1 in order of first use; a pair of spaces is tile 0 */

struct pairs {
    uint16_t key[MAX_PAIRS + 1u];                     /* c1 << 8 | c2 of tile n + 1 */
    unsigned count;
};

static unsigned tile_number(struct pairs *p, unsigned c1, unsigned c2)
{
    unsigned i, key = c1 << 8 | c2;
    if (c1 == 0x20u && c2 == 0x20u)
        return 0;
    for (i = 0; i < p->count; i++)
        if (p->key[i] == key)
            return i + 1u;
    p->key[p->count++] = (uint16_t)key;
    return p->count;
}

/* ---- the layout of a quote */

static void put16(uint8_t *p, unsigned v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

/* The cells and the stream of a quote. A line is one run of tiles whose first tile's left edge is centre - 16 * ceil(tiles / 2), as the PS1's
 * own quotes are; the first run's dx counts from 0, the next one's from where the run before it ended (its last tile's left edge), dy from
 * the line before. Lines are on rows by their place in the arcade's quote; a quote of one line is drawn on the second row, as the PS1's own
 * one-line quote is. */
static void layout(const struct arcade_quotes *q, struct pairs *pairs, struct quote *z)
{
    unsigned l, rows[ARCADE_QUOTE_LINES], shown = 0, i, k, prev_row = 0;
    int x_end = 0;
    z->cells = z->runs = 0;
    for (l = 0; l < ARCADE_QUOTE_LINES; l++)
        shown += z->line[l].n != 0;
    for (l = 0; l < ARCADE_QUOTE_LINES; l++)
        rows[l] = shown == 1 ? 1u : l;
    for (l = 0, k = 0; l < ARCADE_QUOTE_LINES; l++) {
        const struct line *ln = &z->line[l];
        unsigned tiles = (ln->n + 1u) / 2u;
        int xs;
        if (!ln->n)
            continue;
        xs = q->centre - 16 * (int)((tiles + 1u) / 2u);
        for (i = 0; i < tiles; i++) {
            unsigned c1 = ln->ch[2u * i], c2 = 2u * i + 1u < ln->n ? ln->ch[2u * i + 1u] : 0x20u;
            z->cell[z->cells++] = (uint16_t)tile_number(pairs, c1, c2);
        }
        z->stream[3u * k] = (uint16_t)(k ? xs - x_end : xs);
        z->stream[3u * k + 1u] = (uint16_t)((k ? rows[l] - prev_row : rows[l]) * q->line_height);
        z->stream[3u * k + 2u] = (uint16_t)(tiles - 1u);
        x_end = xs + 16 * (int)tiles - 16;
        prev_row = rows[l];
        k++;
    }
    z->runs = k;
}

/* ---- the build */

static int overlaps(uint32_t a, uint32_t an, uint32_t b, uint32_t bn)
{
    return a < b + bn && b < a + an;
}

int arcade_quotes_build(const struct arcade_quotes *q, const struct arcade_quote_fighter *f, const uint8_t *program, size_t program_size,
                        const uint8_t *tiles, size_t tiles_size, uint8_t *module, uint8_t *changed, size_t msize, uint8_t *segment,
                        uint32_t *segment_size, char *err, size_t errsize)
{
    struct pairs *pairs = NULL;
    struct quote *quote = NULL;
    uint8_t (*seg_tiles)[TILE_BYTES] = NULL;
    uint8_t *lists = NULL, *streams = NULL;
    uint32_t list_used = 0, stream_used = 0, packed = 0;
    unsigned i, j, n;
    int status = -1;
    char perr[160];

    if (err && errsize)
        err[0] = 0;
    if (!q || !f || !program || !tiles || !module || !changed || !segment || !segment_size)
        return fail(err, errsize, "a null argument (%u, %u)", 0, 0);
    if (!f->offset_count)
        return fail(err, errsize, "the quote offset table has no entries (%u, %u)", 0, 0);
    if ((uint64_t)f->offsets + 4ull * f->offset_count > msize)
        return fail(err, errsize, "the quote offset table at %#x lies outside the module (%u)", f->offsets, (unsigned)msize);
    if ((uint64_t)f->lists + f->lists_room > msize)
        return fail(err, errsize, "the lists base at %#x lies outside the module (%u)", f->lists, (unsigned)msize);
    if ((uint64_t)f->streams + f->streams_room > msize)
        return fail(err, errsize, "the streams base at %#x lies outside the module (%u)", f->streams, (unsigned)msize);
    if (overlaps(f->offsets, 4u * f->offset_count, f->lists, f->lists_room) ||
        overlaps(f->offsets, 4u * f->offset_count, f->streams, f->streams_room) || overlaps(f->lists, f->lists_room, f->streams, f->streams_room))
        return fail(err, errsize, "the offset table, the lists and the streams overlap (%u, %u)", 0, 0);
    if ((uint64_t)f->text + (uint64_t)ARCADE_QUOTES_PER_FIGHTER * ARCADE_QUOTE_LINES * LINE_BYTES > program_size)
        return fail(err, errsize, "the quotes at %#x lie outside the program (%u)", f->text, (unsigned)program_size);

    pairs = (struct pairs *)calloc(1, sizeof *pairs);
    quote = (struct quote *)calloc(ARCADE_QUOTES_PER_FIGHTER, sizeof *quote);
    seg_tiles = (uint8_t (*)[TILE_BYTES])calloc(MAX_PAIRS + 2u, TILE_BYTES);
    lists = (uint8_t *)malloc((size_t)f->lists_room + 1u);
    streams = (uint8_t *)malloc((size_t)f->streams_room + 1u);
    if (!pairs || !quote || !seg_tiles || !lists || !streams) {
        fail(err, errsize, "out of memory (%u, %u)", 0, 0);
        goto out;
    }

    for (i = 0; i < ARCADE_QUOTES_PER_FIGHTER; i++) {
        for (j = 0; j < ARCADE_QUOTE_LINES; j++)
            if (read_line(program, f->text + (i * ARCADE_QUOTE_LINES + j) * LINE_BYTES, &quote[i].line[j], err, errsize))
                goto out;
        layout(q, pairs, &quote[i]);
        if (!quote[i].cells) {
            fail(err, errsize, "quote %u of the fighter at %#x has no text", i, f->text);
            goto out;
        }
    }

    /* the tiles: 0 is blank, then the pairs in order */
    if (pairs->count + 1u > MAX_TILES) {
        fail(err, errsize, "%u tiles are more than the cell numbers hold (%u)", pairs->count + 1u, MAX_TILES);
        goto out;
    }
    for (n = 0; n < pairs->count; n++)
        if (make_tile(tiles, tiles_size, pairs->key[n] >> 8, pairs->key[n] & 0xffu, seg_tiles[n + 1u])) {
            fail(err, errsize, "a font glyph of the pair %u lies outside the graphics (%u)", n, (unsigned)tiles_size);
            goto out;
        }

    /* the streams, then the lists, each stored once however many quotes are alike */
    for (i = 0; i < ARCADE_QUOTES_PER_FIGHTER; i++) {
        struct quote *z = &quote[i];
        uint8_t body[2u * 3u * ARCADE_QUOTE_LINES];
        uint32_t size = 6u * z->runs;
        int same = 0;
        for (j = 0; j < 3u * z->runs; j++)
            put16(body + 2u * j, z->stream[j]);
        for (n = 0; n < i && !same; n++)
            if (6u * quote[n].runs == size && !memcmp(streams + quote[n].stream_at, body, size)) {
                z->stream_at = quote[n].stream_at;
                same = 1;
            }
        if (same)
            continue;
        if ((uint64_t)stream_used + size > f->streams_room) {
            fail(err, errsize, "the streams need more than their room (%u bytes at least, room %u)", stream_used + size, f->streams_room);
            goto out;
        }
        memcpy(streams + stream_used, body, size);
        z->stream_at = stream_used;
        stream_used += size;
    }
    for (i = 0; i < ARCADE_QUOTES_PER_FIGHTER; i++) {
        struct quote *z = &quote[i];
        uint32_t size = 10u + 2u * z->cells;
        uint8_t body[10u + 2u * MAX_RUN * ARCADE_QUOTE_LINES];
        int same = 0;
        if (z->stream_at > 0xfffeu) {
            fail(err, errsize, "the stream offset %u of quote %u does not fit 16 bits", z->stream_at, i);
            goto out;
        }
        put16(body, z->cells);
        put16(body + 2, 0);                                 /* mode 0: n plain cells */
        put16(body + 4, 0);                                 /* the palette row: the quote palette's own */
        put16(body + 6, 0);
        put16(body + 8, z->stream_at);
        for (j = 0; j < z->cells; j++)
            put16(body + 10u + 2u * j, z->cell[j]);
        for (n = 0; n < i && !same; n++)
            if (10u + 2u * quote[n].cells == size && !memcmp(lists + quote[n].list_at, body, size)) {
                z->list_at = quote[n].list_at;
                same = 1;
            }
        if (same)
            continue;
        if ((uint64_t)list_used + size > f->lists_room) {
            fail(err, errsize, "the lists need more than their room (%u bytes at least, room %u)", list_used + size, f->lists_room);
            goto out;
        }
        memcpy(lists + list_used, body, size);
        z->list_at = list_used;
        list_used += size;
    }

    /* the glyph segment */
    if (arcade_tilepack((const uint8_t (*)[128])seg_tiles, pairs->count + 1u, segment, f->segment_room, &packed, perr, sizeof perr)) {
        fail(err, errsize, "the glyph tiles do not pack in the segment's room (%u tiles, room %u)", pairs->count + 1u, f->segment_room);
        goto out;
    }

    /* committed only now: nothing of the module changes on a failure */
    memcpy(module + f->streams, streams, stream_used);
    memset(changed + f->streams, 1, stream_used);
    memcpy(module + f->lists, lists, list_used);
    memset(changed + f->lists, 1, list_used);
    for (i = 0; i < f->offset_count; i++) {
        uint32_t v = quote[i % ARCADE_QUOTES_PER_FIGHTER].list_at;
        put16(module + f->offsets + 4u * i, v & 0xffffu);
        put16(module + f->offsets + 4u * i + 2u, v >> 16);
        memset(changed + f->offsets + 4u * i, 1, 4);
    }
    *segment_size = packed;
    status = 0;
out:
    free(pairs);
    free(quote);
    free(seg_tiles);
    free(lists);
    free(streams);
    return status;
}
