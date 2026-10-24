/* Driver of test_arcade_stage_objects.py: made-up arcade programs and graphics, and the checked-in table's sanity.
 *   stage_objects_test build PROGRAM TILES SPEC OUTDIR   build the grids of a made-up row into a zero module; writes module.bin and module.known
 *   stage_objects_test table                              sanity of the checked-in table
 * SPEC: size MODULE | object FIRST COUNT FLIP | shared FIRST COUNT FLIP (FIRST may be ZERO, DISC or BLANK) | grid OFFSET ART LO HI
 * A line "error: ..." on stdout and status 1 when the build says -1 (the grids that can be built are in the output all the same). */
#include "../stage_objects.h"
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
    p = malloc((size_t)len + 1);
    if (!p || fread(p, 1, (size_t)len, f) != (size_t)len) { fclose(f); free(p); return NULL; }
    fclose(f);
    *n = (size_t)len;
    return p;
}

static uint32_t first_word(const char *w)
{
    if (!strcmp(w, "ZERO")) return ARCADE_STAGE_ZERO;
    if (!strcmp(w, "DISC")) return ARCADE_STAGE_DISC;
    if (!strcmp(w, "BLANK")) return ARCADE_STAGE_BLANK;
    return (uint32_t)strtoul(w, NULL, 0);
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

static int build(const char *prog_path, const char *tiles_path, const char *spec_path, const char *dir)
{
    size_t psize = 0, tsize = 0, msize = 0;
    uint8_t *prog = slurp(prog_path, &psize), *tiles = slurp(tiles_path, &tsize), *module, *known;
    struct arcade_stage_run objects[64], shared[64];
    struct arcade_stage_grid grids[256];
    struct arcade_stage_objects row;
    unsigned no = 0, ns = 0, ng = 0, rc = 0;
    char word[64], first[64], err[256];
    unsigned long a, b, c, d;
    FILE *f = fopen(spec_path, "r");
    if (!prog || !tiles || !f) return 3;
    while (fscanf(f, "%63s", word) == 1) {
        if (!strcmp(word, "size")) { if (fscanf(f, "%lu", &a) != 1) return 3; msize = (size_t)a; }
        else if (!strcmp(word, "object") || !strcmp(word, "shared")) {
            struct arcade_stage_run *r = word[0] == 'o' ? &objects[no] : &shared[ns];
            if (fscanf(f, "%63s %lu %lu", first, &b, &c) != 3 || no >= 64 || ns >= 64) return 3;
            r->first = first_word(first); r->count = (uint32_t)b; r->flip = (uint32_t)c;
            if (word[0] == 'o') no++; else ns++;
        } else if (!strcmp(word, "grid")) {
            if (fscanf(f, "%lu %lu %lu %lu", &a, &b, &c, &d) != 4 || ng >= 256) return 3;
            grids[ng].offset = (uint32_t)a; grids[ng].art = (uint32_t)b; grids[ng].lo = (uint16_t)c; grids[ng].hi = (uint16_t)d; ng++;
        } else return 3;
    }
    fclose(f);
    module = calloc(msize ? msize : 1, 1);
    known = calloc(msize ? msize : 1, 1);
    row.file = "TEST.PAC"; row.grids = grids; row.grid_count = ng;
    if (arcade_stage_objects_build(prog, psize, tiles, tsize, objects, no, shared, ns, &row, module, known, msize, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
    }
    if (dump(dir, "module.bin", module, msize) || dump(dir, "module.known", known, msize)) rc = 4;
    free(prog); free(tiles); free(module); free(known);
    return (int)rc;
}

static int table(void)
{
    unsigned i, k, grids = 0;
    for (i = 0; i < arcade_stage_objects_count; i++) {
        const struct arcade_stage_objects *r = &arcade_stage_objects[i];
        if (!r->file || strncmp(r->file, "STAGE", 5)) { printf("bad name at %u\n", i); return 1; }
        for (k = 0; k < r->grid_count; k++) {
            const struct arcade_stage_grid *g = &r->grids[k];
            if ((g->offset & 1u) || (g->art & 1u) || g->lo > g->hi || !g->lo || (k && g->offset < r->grids[k - 1].offset + 8u)) { printf("bad grid %u of %u\n", k, i); return 1; }
        }
        grids += r->grid_count;
    }
    if (!arcade_stage_objects_find("STAGE00.PAC") || arcade_stage_objects_find("NOPE.PAC") || !grids) { printf("find fails\n"); return 1; }
    printf("%u stage files, %u grids\n", arcade_stage_objects_count, grids);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 6 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4], argv[5]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    return 2;
}
