/* Driver of test_arcade_fighter.py: synthetic programs and the generated descriptor table only.
 *   fighter_test build PROGRAM SPEC OUTDIR   build one fighter from the made-up arcade program
 *   fighter_test table                       sanity of the checked-in descriptor table
 * SPEC is text: "rules positions_room lists_room segment_count" then "address count bank" per segment.
 * OUTDIR gets lists.bin, positions.bin and tiles.txt (a line "number code flip" per tile). */
#include "../fighter.h"
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

static int build(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t size = 0;
    uint8_t *prog = slurp(prog_path, &size);
    FILE *spec = fopen(spec_path, "r");
    struct arcade_fighter f;
    struct arcade_fighter_segment *segs = NULL;
    struct arcade_fighter_out out;
    char err[256], line[64];
    unsigned rules, pr, lr, n, i;
    FILE *t;
    int rc = 2;

    if (!prog || !spec || fscanf(spec, "%u %u %u %u", &rules, &pr, &lr, &n) != 4) return 3;
    segs = calloc(n ? n : 1, sizeof *segs);
    for (i = 0; i < n; i++) {
        unsigned long a, b;
        unsigned c;
        if (fscanf(spec, "%lu %u %lu", &a, &c, &b) != 3) return 3;
        segs[i].address = (uint32_t)a;
        segs[i].count = (uint16_t)c;
        segs[i].bank = (uint32_t)b;
    }
    memset(&f, 0, sizeof f);
    f.file = "TEST.PAC";
    f.rules = rules;
    f.positions_room = pr;
    f.lists_room = lr;
    f.segments = segs;
    f.segment_count = n;
    if (arcade_fighter_build(prog, size, &f, &out, err, sizeof err) != 0) {
        printf("error: %s\n", err);
        if (out.lists || out.positions || out.tile_code || out.tile_flip) printf("leak: output left allocated\n");
        rc = 1;
    } else {
        snprintf(line, sizeof line, "%s/tiles.txt", dir);
        t = fopen(line, "w");
        if (dump(dir, "lists.bin", out.lists, out.lists_size) || dump(dir, "positions.bin", out.positions, out.positions_size) || !t) return 3;
        for (i = 1; i < out.tile_count; i++) fprintf(t, "%u %u %u\n", i, (unsigned)out.tile_code[i], (unsigned)out.tile_flip[i]);
        fclose(t);
        printf("ok\n");
        arcade_fighter_free(&out);
        if (out.lists || out.tile_code) printf("leak: free did not clear\n");
        rc = 0;
    }
    free(segs);
    free(prog);
    fclose(spec);
    return rc;
}

static int table(void)
{
    unsigned i, j, s;
    if (!arcade_fighter_count) return 1;
    for (i = 0; i < arcade_fighter_count; i++) {
        const struct arcade_fighter *f = &arcade_fighters[i];
        if (!f->file || strncmp(f->file, "PL", 2) || !f->segments || !f->segment_count || !f->positions_room || !f->lists_room) return 1;
        if (f->rules & ~7u) return 1;
        if (f->module_slot != 4 && f->module_slot != 5) return 1;
        if ((f->positions & 1) || (f->lists & 1)) return 1;
        for (j = 0; j < i; j++)
            if (!strcmp(arcade_fighters[j].file, f->file)) return 1;
        for (s = 0; s < f->segment_count; s++)
            if (!f->segments[s].count || (f->segments[s].address & 1)) return 1;
    }
    printf("table: %u files\n", arcade_fighter_count);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 5 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    fprintf(stderr, "usage: fighter_test build PROGRAM SPEC OUTDIR | table\n");
    return 64;
}
