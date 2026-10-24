/* Driver of test_arcade_plates.py: made-up programs, graphics and pages only.
 *   plates_test build PROGRAM TILES PAGE KNOWN BAND OUTDIR   build one map: OUTDIR/map.bin (8,192 bytes), OUTDIR/page.out (the page with the new units written) and
 *   OUTDIR/units.bin (1,024 bytes, 1 for each unit written); stdout "filled N", or "error: ..." and status 1
 * PAGE is the 32,768 bytes of the page, KNOWN one byte a page byte (0: not built). */
#include "../plates.h"
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

int main(int argc, char **argv)
{
    size_t psize = 0, tsize = 0, gsize = 0, ksize = 0;
    uint8_t *prog, *tiles, *page, *known, *map, units[1024];
    unsigned filled = 0;
    char err[200], path[1024];
    int rc;
    FILE *f;
    if (argc != 8 || strcmp(argv[1], "build")) return 2;
    prog = slurp(argv[2], &psize);
    tiles = slurp(argv[3], &tsize);
    page = slurp(argv[4], &gsize);
    known = slurp(argv[5], &ksize);
    map = (uint8_t *)malloc(ARCADE_PLATE_MAP_BYTES);
    if (!prog || !tiles || !page || !known || !map || gsize != ARCADE_PLATE_PAGE_BYTES || ksize != ARCADE_PLATE_PAGE_BYTES) return 3;
    rc = arcade_plate_build(prog, psize, tiles, tsize, (unsigned)atoi(argv[6]), page, known, map, units, &filled, err, sizeof err);
    if (rc) {
        printf("error: %s\n", err);
    } else {
        printf("filled %u\n", filled);
        snprintf(path, sizeof path, "%s/map.bin", argv[7]);
        f = fopen(path, "wb");
        if (!f || fwrite(map, 1, ARCADE_PLATE_MAP_BYTES, f) != ARCADE_PLATE_MAP_BYTES) return 3;
        fclose(f);
        snprintf(path, sizeof path, "%s/page.out", argv[7]);
        f = fopen(path, "wb");
        if (!f || fwrite(page, 1, ARCADE_PLATE_PAGE_BYTES, f) != ARCADE_PLATE_PAGE_BYTES) return 3;
        fclose(f);
        snprintf(path, sizeof path, "%s/units.bin", argv[7]);
        f = fopen(path, "wb");
        if (!f || fwrite(units, 1, sizeof units, f) != sizeof units) return 3;
        fclose(f);
    }
    free(prog); free(tiles); free(page); free(known); free(map);
    return rc ? 1 : 0;
}
