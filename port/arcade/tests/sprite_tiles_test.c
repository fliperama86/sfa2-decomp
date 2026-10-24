/* Driver of sprite_tiles.c (port/arcade/sprite_tiles.py --compare-c and port/tools/test_arcade_sprite_tiles.py): it links whatever table it is given.
 *   list                       "count N", then per row: file slot size tiles run_count
 *   find FILE                  exit 0 when the row exists, 1 when not
 *   build FILE GFX OUT         arcade_sprite_tiles_build over the bytes of file GFX; writes the tiles to OUT; exit 3 (message on stderr) when the
 *                              call refuses, 4 when there is no row, 2 on a file error
 *   check FILE GFX DISC        compares with the tiles in file DISC (128 bytes each) what the build makes, and packs them with arcade_tilepack
 *                              within the row's size. An OWN run (which the build refuses) is built as a copy with the disc's tiles in its
 *                              place and counted; the other tiles are compared. Prints "built OWN DIFFER FIRST PACKED" (FIRST is the first
 *                              differing tile or -; PACKED is the packed size, or "unpacked" and the message); exit 3 when the build refuses,
 *                              4 when there is no row, 2 on a file error */
#include "sprite_tiles.h"
#include "tilepack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *slurp(const char *path, size_t *size)
{
    FILE *f = fopen(path, "rb");
    unsigned char *buf;
    long n;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(n > 0 ? (size_t)n : 1);
    if (!buf || fread(buf, 1, (size_t)n, f) != (size_t)n) {
        free(buf);
        fclose(f);
        return NULL;
    }
    fclose(f);
    *size = (size_t)n;
    return buf;
}

int main(int argc, char **argv)
{
    unsigned i;
    const struct arcade_sprite_chunk *c;
    if (argc >= 2 && !strcmp(argv[1], "list")) {
        printf("count %u\n", arcade_sprite_chunk_count);
        for (i = 0; i < arcade_sprite_chunk_count; i++) {
            c = &arcade_sprite_chunks[i];
            printf("%s %u %u %u %u\n", c->file, (unsigned)c->slot, (unsigned)c->size, (unsigned)c->tiles, c->run_count);
        }
        return 0;
    }
    if (argc >= 3 && !strcmp(argv[1], "find")) return arcade_sprite_chunk_find(argv[2]) ? 0 : 1;
    if (argc >= 5 && (!strcmp(argv[1], "build") || !strcmp(argv[1], "check"))) {
        size_t gsize = 0, dsize = 0;
        unsigned char *gfx, (*tiles)[128], *disc = NULL;
        char err[256];
        int check = !strcmp(argv[1], "check"), rc = 0;
        c = arcade_sprite_chunk_find(argv[2]);
        if (!c) return 4;
        gfx = slurp(argv[3], &gsize);
        if (!gfx) return 2;
        if (check && !(disc = slurp(argv[4], &dsize))) return 2;
        tiles = malloc((size_t)(c->tiles ? c->tiles : 1) * 128u);
        if (!tiles) return 2;
        if (check) {
            /* a copy of the row with each OWN run as a BLANK one; those tiles are then the disc's own */
            struct arcade_sprite_chunk copy = *c;
            struct arcade_sprite_run *runs = malloc((size_t)(c->run_count ? c->run_count : 1) * sizeof *runs);
            uint32_t differ = 0, own = 0, t, at = 0, size = 0;
            long first = -1;
            uint8_t *packed;
            char perr[256];
            if (!runs || dsize != (size_t)c->tiles * 128u) return 2;
            memcpy(runs, c->runs, (size_t)c->run_count * sizeof *runs);
            copy.runs = runs;
            if (arcade_sprite_tiles_build(c, gfx, gsize, tiles, err, sizeof err) != 0 && strstr(err, "does not have") == NULL) {
                fprintf(stderr, "%s\n", err);
                rc = 3;
            } else {
                for (t = 0; t < c->run_count; t++) {
                    if (runs[t].code == ARCADE_SPRITE_RUN_OWN) {
                        runs[t].code = ARCADE_SPRITE_RUN_BLANK;
                        own += runs[t].count;
                    }
                }
                if (arcade_sprite_tiles_build(&copy, gfx, gsize, tiles, err, sizeof err) != 0) {
                    fprintf(stderr, "%s\n", err);
                    rc = 3;
                } else {
                    for (t = 0; t < c->run_count; t++) {
                        if (c->runs[t].code == ARCADE_SPRITE_RUN_OWN) memcpy(tiles[at], disc + (size_t)at * 128u, (size_t)c->runs[t].count * 128u);
                        at += c->runs[t].count;
                    }
                    for (t = 0; t < c->tiles; t++)
                        if (memcmp(tiles[t], disc + (size_t)t * 128u, 128) != 0) {
                            if (first < 0) first = (long)t;
                            differ++;
                        }
                    packed = malloc(c->size ? c->size : 1);
                    if (!packed) rc = 2;
                    else {
                        printf("built %u %u ", (unsigned)own, (unsigned)differ);
                        if (first < 0) printf("- "); else printf("%ld ", first);
                        if (arcade_tilepack((const uint8_t (*)[128])tiles, c->tiles, packed, c->size, &size, perr, sizeof perr) == 0) printf("%u\n", (unsigned)size);
                        else printf("unpacked %s\n", perr);
                        free(packed);
                    }
                }
            }
            free(runs);
        } else if (arcade_sprite_tiles_build(c, gfx, gsize, tiles, err, sizeof err) != 0) {
            fprintf(stderr, "%s\n", err);
            rc = 3;
        } else {
            FILE *f = fopen(argv[4], "wb");
            if (!f || fwrite(tiles, 128, c->tiles, f) != c->tiles) rc = 2;
            if (f) fclose(f);
        }
        free(tiles);
        free(gfx);
        free(disc);
        return rc;
    }
    return 2;
}
