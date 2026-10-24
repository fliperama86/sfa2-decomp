/* Driver of test_arcade_compose.py: made-up programs and graphics only.
 *   compose_test record PROGRAM ADDRESS BANK SPLIT      the cells of a record, one line each: "code x y flags part"; first line "info mode declared count end"
 *   compose_test canvas PROGRAM TILES ADDRESS BANK OUTDIR [X Y ROWS]...   the record drawn: OUTDIR/canvas.bin (w * h bytes), stdout "canvas x0 y0 w h"; then the cuts
 *   compose_test scroll2 PROGRAM TILES FIRST BLOCKS OUTDIR [X Y ROWS]...  the scroll 2 blocks drawn, the same output
 *   compose_test cell PROGRAM BLOCK C R                 "cell tile attribute"
 *   compose_test tile TILES CODE FLIP OUTDIR            OUTDIR/tile.bin (256 pixels)
 * Each cut X Y ROWS appends 128 bytes to OUTDIR/cuts.bin. A failure prints "error: ..." (or "refused") and exits 1. */
#include "../compose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *slurp(const char *path, size_t *n)
{
    FILE *f = fopen(path, "rb");
    uint8_t *p;
    long len;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = (uint8_t *)malloc((size_t)len + 1);
    if (!p || fread(p, 1, (size_t)len, f) != (size_t)len) {
        fclose(f);
        free(p);
        return NULL;
    }
    fclose(f);
    *n = (size_t)len;
    return p;
}

static int dump(const char *dir, const char *name, const void *p, size_t n)
{
    char path[1024];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f || (n && fwrite(p, 1, n, f) != n)) return -1;
    return fclose(f);
}

static int finish(struct arcade_canvas *cv, const char *dir, int argc, char **argv)
{
    uint8_t *cuts = (uint8_t *)malloc(128u * (size_t)(argc / 3 + 1));
    int i, n = 0;
    if (!cuts) return 1;
    printf("canvas %d %d %u %u\n", (int)cv->x0, (int)cv->y0, cv->w, cv->h);
    for (i = 0; i + 2 < argc; i += 3, n++)
        arcade_compose_cut(cv, atoi(argv[i]), atoi(argv[i + 1]), (unsigned)atoi(argv[i + 2]), cuts + 128u * n);
    if (dump(dir, "canvas.bin", cv->px, (size_t)cv->w * cv->h) || dump(dir, "cuts.bin", cuts, 128u * (size_t)n)) {
        free(cuts);
        arcade_compose_free(cv);
        return 1;
    }
    free(cuts);
    arcade_compose_free(cv);
    return 0;
}

int main(int argc, char **argv)
{
    char err[160];
    size_t psize = 0, tsize = 0;
    uint8_t *prog = NULL, *tiles = NULL;
    int rc = 1;
    err[0] = 0;
    if (argc < 3) return 2;
    if (!strcmp(argv[1], "record") && argc == 6) {
        struct arcade_compose_cell *cells = (struct arcade_compose_cell *)malloc(512u * sizeof *cells);
        struct arcade_compose_info info;
        unsigned i;
        prog = slurp(argv[2], &psize);
        if (!prog || !cells) return 2;
        if (arcade_compose_record(prog, psize, (uint32_t)strtoul(argv[3], NULL, 0), (uint32_t)strtoul(argv[4], NULL, 0), (unsigned)atoi(argv[5]), cells, 512u, &info,
                                  err, sizeof err)) {
            printf("error: %s\n", err);
        } else {
            printf("info %u %u %u %u\n", info.mode, info.declared, info.count, (unsigned)info.end);
            for (i = 0; i < info.count; i++)
                printf("%u %d %d %u %u\n", (unsigned)cells[i].code, (int)cells[i].x, (int)cells[i].y, (unsigned)cells[i].flags, (unsigned)cells[i].part);
            rc = 0;
        }
        free(cells);
    } else if (!strcmp(argv[1], "canvas") && argc >= 7) {
        struct arcade_canvas cv;
        prog = slurp(argv[2], &psize);
        tiles = slurp(argv[3], &tsize);
        if (!prog || !tiles) return 2;
        if (arcade_compose_record_canvas(prog, psize, tiles, tsize, (uint32_t)strtoul(argv[4], NULL, 0), (uint32_t)strtoul(argv[5], NULL, 0), &cv, err, sizeof err))
            printf("error: %s\n", err);
        else
            rc = finish(&cv, argv[6], argc - 7, argv + 7);
    } else if (!strcmp(argv[1], "scroll2") && argc >= 7) {
        struct arcade_canvas cv;
        prog = slurp(argv[2], &psize);
        tiles = slurp(argv[3], &tsize);
        if (!prog || !tiles) return 2;
        if (arcade_compose_scroll2(prog, psize, tiles, tsize, (unsigned)atoi(argv[4]), (unsigned)atoi(argv[5]), &cv, err, sizeof err))
            printf("error: %s\n", err);
        else
            rc = finish(&cv, argv[6], argc - 7, argv + 7);
    } else if (!strcmp(argv[1], "cell") && argc == 6) {
        uint32_t tile = 0;
        uint16_t attr = 0;
        prog = slurp(argv[2], &psize);
        if (!prog) return 2;
        if (arcade_compose_scroll2_cell(prog, psize, (unsigned)atoi(argv[3]), (unsigned)atoi(argv[4]), (unsigned)atoi(argv[5]), &tile, &attr)) {
            printf("refused\n");
        } else {
            printf("cell %u %u\n", (unsigned)tile, (unsigned)attr);
            rc = 0;
        }
    } else if (!strcmp(argv[1], "tile") && argc == 6) {
        uint8_t px[256];
        tiles = slurp(argv[2], &tsize);
        if (!tiles) return 2;
        if (arcade_compose_tile(tiles, tsize, (uint32_t)strtoul(argv[3], NULL, 0), (unsigned)atoi(argv[4]), px)) {
            printf("refused\n");
        } else {
            rc = dump(argv[5], "tile.bin", px, sizeof px) ? 1 : 0;
        }
    } else {
        return 2;
    }
    free(prog);
    free(tiles);
    return rc;
}
