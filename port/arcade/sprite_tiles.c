/* Every tile of a fighter file's sprite chunk, built from the arcade graphics (sprite_tiles.h has the contract). Project-authored,
 * plain C99; the table that drives it (sprite_tiles_table.c) holds codes, flips and counts only. */
#include "sprite_tiles.h"

#include <stdio.h>
#include <string.h>

const struct arcade_sprite_chunk *arcade_sprite_chunk_find(const char *file)
{
    unsigned i;
    if (!file) return NULL;
    for (i = 0; i < arcade_sprite_chunk_count; i++)
        if (!strcmp(arcade_sprite_chunks[i].file, file)) return &arcade_sprite_chunks[i];
    return NULL;
}

static void fail(char *err, size_t errsize, const char *file, const char *what, unsigned long a, unsigned long b)
{
    if (err && errsize) snprintf(err, errsize, "%s: %s (%lu, %lu)", file, what, a, b);
}

/* The PS1 tile that arcade tile `code` stored with `flip` makes: the same rule as the loader's arcade_tile (port/src/arcade.c): 4 bits
 * a pixel from the 4 bitplanes, the colour index one higher (modulo 16, so the arcade's transparent 15 is the PS1's 0), the left
 * pixel of a pair in the low nibble, flipped across (bit 0) and down (bit 1). */
static void convert(const uint8_t *t, unsigned flip, uint8_t out[128])
{
    unsigned x, y, plane;
    memset(out, 0, 128);
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            unsigned sx = flip & 1u ? 15u - x : x, sy = flip & 2u ? 15u - y : y, v = 0;
            for (plane = 0; plane < 4; plane++) v |= ((t[sy * 8u + (sx / 8u) * 4u + plane] >> (7u - sx % 8u)) & 1u) << plane;
            out[y * 8u + x / 2u] |= (uint8_t)(((v + 1u) & 15u) << (x & 1u ? 4 : 0));
        }
}

int arcade_sprite_tiles_build(const struct arcade_sprite_chunk *c, const uint8_t *tiles, size_t tiles_size, uint8_t (*out)[128],
                              char *err, size_t errsize)
{
    const size_t graphics = tiles_size / 128u;
    uint32_t at = 0;
    unsigned r;
    if (err && errsize) err[0] = 0;
    if (!c || !out || (!tiles && tiles_size)) {
        fail(err, errsize, c && c->file ? c->file : "?", "bad argument", 0, 0);
        return -1;
    }
    for (r = 0; r < c->run_count; r++) {
        const struct arcade_sprite_run *run = &c->runs[r];
        uint32_t i;
        if (run->count > c->tiles - at) {
            fail(err, errsize, c->file, "the runs add up to more tiles than the chunk has", r, c->tiles);
            return -1;
        }
        if (run->code == ARCADE_SPRITE_RUN_OWN) {
            fail(err, errsize, c->file, "a run of tiles the arcade does not have", r, at);
            return -1;
        }
        if (run->code == ARCADE_SPRITE_RUN_BLANK) {
            memset(out[at], 0, (size_t)run->count * 128u);
            at += run->count;
            continue;
        }
        if (run->step < -1 || run->step > 1 || (run->flip & ~(3u | ARCADE_SPRITE_REDRAWN))) {
            fail(err, errsize, c->file, "bad step or flip in run", r, at);
            return -1;
        }
        for (i = 0; i < run->count; i++) {
            long long code = (long long)run->code + (long long)run->step * (long long)i;
            if (code < 0 || (unsigned long long)code >= graphics) {
                fail(err, errsize, c->file, "a code lies outside the graphics", (unsigned long)(code < 0 ? 0 : code), (unsigned long)graphics);
                return -1;
            }
            convert(tiles + (size_t)code * 128u, run->flip, out[at + i]);
        }
        at += run->count;
    }
    if (at != c->tiles) {
        fail(err, errsize, c->file, "the runs add up to fewer tiles than the chunk has", at, c->tiles);
        return -1;
    }
    return 0;
}
