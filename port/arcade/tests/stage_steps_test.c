/* Driver of test_arcade_stage_steps.py: made-up arcade programs, and the checked-in table's sanity.
 *   stage_steps_test build PROGRAM SPEC OUTDIR   build the steps of a made-up row into a zero module; writes module.bin and module.known
 *   stage_steps_test table                        sanity of the checked-in table
 * SPEC: size MODULE | run OFFSET RECORD COUNT | art ART GRID
 * A line "error: ..." on stdout and status 1 when the build says -1 (the runs that can be built are in the output all the same). */
#include "../stage_steps.h"
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

static int dump(const char *dir, const char *name, const void *p, size_t n)
{
    char path[1024];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f || (n && fwrite(p, 1, n, f) != n)) return -1;
    return fclose(f);
}

static int build(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t psize = 0, msize = 0;
    uint8_t *prog = slurp(prog_path, &psize), *module, *known;
    struct arcade_stage_step_run runs[256];
    struct arcade_stage_step_art arts[256];
    struct arcade_stage_steps row;
    unsigned nr = 0, na = 0, rc = 0;
    char word[64], err[256];
    unsigned long a, b, c;
    FILE *f = fopen(spec_path, "r");
    if (!prog || !f) return 3;
    while (fscanf(f, "%63s", word) == 1) {
        if (!strcmp(word, "size")) { if (fscanf(f, "%lu", &a) != 1) return 3; msize = (size_t)a; }
        else if (!strcmp(word, "run")) {
            if (fscanf(f, "%lu %lu %lu", &a, &b, &c) != 3 || nr >= 256) return 3;
            runs[nr].offset = (uint32_t)a; runs[nr].record = (uint32_t)b; runs[nr].count = (uint16_t)c; nr++;
        } else if (!strcmp(word, "art")) {
            if (fscanf(f, "%lu %lu", &a, &b) != 2 || na >= 256) return 3;
            arts[na].art = (uint32_t)a; arts[na].grid = (uint32_t)b; na++;
        } else return 3;
    }
    fclose(f);
    module = calloc(msize ? msize : 1, 1);
    known = calloc(msize ? msize : 1, 1);
    row.file = "TEST.PAC"; row.runs = runs; row.run_count = nr; row.arts = arts; row.art_count = na;
    if (arcade_stage_steps_build(prog, psize, &row, module, known, msize, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
    }
    if (dump(dir, "module.bin", module, msize) || dump(dir, "module.known", known, msize)) rc = 4;
    free(prog); free(module); free(known);
    return (int)rc;
}

static int table(void)
{
    unsigned i, k, steps = 0;
    for (i = 0; i < arcade_stage_steps_count; i++) {
        const struct arcade_stage_steps *r = &arcade_stage_steps[i];
        if (!r->file || strncmp(r->file, "STAGE", 5)) { printf("bad name at %u\n", i); return 1; }
        for (k = 0; k < r->run_count; k++) {
            const struct arcade_stage_step_run *s = &r->runs[k];
            if ((s->offset & 1u) || (s->record & 1u) || !s->count || (k && s->offset < r->runs[k - 1].offset + 12u * r->runs[k - 1].count)) { printf("bad run %u of %u\n", k, i); return 1; }
            steps += s->count;
        }
        for (k = 0; k < r->art_count; k++)
            if ((r->arts[k].art & 1u) || (r->arts[k].grid & 1u) || (k && r->arts[k].art <= r->arts[k - 1].art)) { printf("bad art %u of %u\n", k, i); return 1; }
    }
    if (!arcade_stage_steps_find("STAGE00.PAC") || arcade_stage_steps_find("NOPE.PAC") || !steps) { printf("find fails\n"); return 1; }
    printf("%u stage files, %u steps\n", arcade_stage_steps_count, steps);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 5 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    return 2;
}
