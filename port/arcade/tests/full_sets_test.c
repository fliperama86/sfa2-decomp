/* Driver of test_arcade_full_sets.py and full_sets.py --compare-c.
 *   full_sets_test table                                     print the linked table (arcade_full_sets), one row each, in the SPEC form
 *   full_sets_test row INDEX MODULE SOURCE LISTS OUT CHANGED build row INDEX of the linked table
 *   full_sets_test spec SPECFILE MODULE SOURCE LISTS OUT CHANGED   build the row the file describes
 *   full_sets_test tiles SOURCE ADDRESS BASE ROOM OUT         arcade_full_tiles; prints the count
 * LISTS, ADDRESS, BASE and ROOM are numbers (0x allowed); OUT and CHANGED are written (size bytes). A refusal prints "refused: message" on
 * standard output and exits with 3; any other failure exits with 1.
 * SPEC: one line `file source` and 19 numbers (slot source_slot tiles_slot source_tiles_slot kind load source_load source_positions
 * positions_size source_lists lists_size source_steps step_count source_table_entries positions lists steps table table_entries), then the run
 * count and the outside count; then a line `first count source_first` per run and a line `target source` per outside step. */
#include "../full_sets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RUNS 512
#define MAX_OUTSIDE 512

static uint8_t *slurp(const char *path, uint32_t *n)
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
    *n = (uint32_t)len;
    return p;
}

static int dump(const char *path, const void *p, size_t n)
{
    FILE *f = fopen(path, "wb");
    int ok;
    if (!f) return -1;
    ok = fwrite(p, 1, n, f) == n;
    return fclose(f) == 0 && ok ? 0 : -1;
}

static unsigned long num(const char *s)
{
    return strtoul(s, NULL, 0);
}

static void print_row(const struct arcade_full_set *s)
{
    unsigned k;
    printf("%s %s %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", s->file, s->source, s->slot, s->source_slot, s->tiles_slot,
           s->source_tiles_slot, s->kind, (unsigned)s->load, (unsigned)s->source_load, (unsigned)s->source_positions, (unsigned)s->positions_size,
           (unsigned)s->source_lists, (unsigned)s->lists_size, (unsigned)s->source_steps, s->step_count, s->source_table_entries,
           (unsigned)s->positions, (unsigned)s->lists, (unsigned)s->steps, (unsigned)s->table, s->table_entries, s->run_count, s->outside_count);
    for (k = 0; k < s->run_count; k++) printf("%u %u %u\n", s->runs[k].first, s->runs[k].count, s->runs[k].source_first);
    for (k = 0; k < s->outside_count; k++) printf("%u %u\n", (unsigned)s->outside[k].target, (unsigned)s->outside[k].source);
}

static int read_spec(const char *path, struct arcade_full_set *s, struct arcade_full_run *runs, struct arcade_full_step *outside,
                     char *file, char *source)
{
    FILE *f = fopen(path, "r");
    unsigned long v[21];
    unsigned k;
    if (!f) return -1;
    if (fscanf(f, "%63s %63s", file, source) != 2) goto bad;
    for (k = 0; k < 21; k++)
        if (fscanf(f, "%lu", &v[k]) != 1) goto bad;
    if (v[19] > MAX_RUNS || v[20] > MAX_OUTSIDE) goto bad;
    for (k = 0; k < v[19]; k++) {
        unsigned long a, b, c;
        if (fscanf(f, "%lu %lu %lu", &a, &b, &c) != 3) goto bad;
        runs[k].first = (uint16_t)a;
        runs[k].count = (uint16_t)b;
        runs[k].source_first = (uint16_t)c;
    }
    for (k = 0; k < v[20]; k++) {
        unsigned long a, b;
        if (fscanf(f, "%lu %lu", &a, &b) != 2) goto bad;
        outside[k].target = (uint32_t)a;
        outside[k].source = (uint32_t)b;
    }
    fclose(f);
    memset(s, 0, sizeof *s);
    s->file = file;
    s->source = source;
    s->slot = (uint16_t)v[0]; s->source_slot = (uint16_t)v[1]; s->tiles_slot = (uint16_t)v[2]; s->source_tiles_slot = (uint16_t)v[3];
    s->kind = (uint16_t)v[4]; s->load = (uint32_t)v[5]; s->source_load = (uint32_t)v[6];
    s->source_positions = (uint32_t)v[7]; s->positions_size = (uint32_t)v[8]; s->source_lists = (uint32_t)v[9]; s->lists_size = (uint32_t)v[10];
    s->source_steps = (uint32_t)v[11]; s->step_count = (uint16_t)v[12]; s->source_table_entries = (uint16_t)v[13];
    s->positions = (uint32_t)v[14]; s->lists = (uint32_t)v[15]; s->steps = (uint32_t)v[16]; s->table = (uint32_t)v[17];
    s->table_entries = (uint16_t)v[18];
    s->runs = runs;
    s->run_count = (unsigned)v[19];
    s->outside = outside;
    s->outside_count = (unsigned)v[20];
    return 0;
bad:
    fclose(f);
    return -1;
}

int main(int argc, char **argv)
{
    char err[256] = "";
    if (argc == 2 && !strcmp(argv[1], "table")) {
        unsigned k;
        for (k = 0; k < arcade_full_set_count; k++) print_row(&arcade_full_sets[k]);
        return 0;
    }
    if (argc == 8 && (!strcmp(argv[1], "row") || !strcmp(argv[1], "spec"))) {
        struct arcade_full_set spec;
        struct arcade_full_run runs[MAX_RUNS];
        struct arcade_full_step outside[MAX_OUTSIDE];
        char file[64], source[64];
        const struct arcade_full_set *s = NULL;
        uint32_t size = 0, source_size = 0;
        uint8_t *module = NULL, *src = NULL, *out = NULL, *changed = NULL;
        int status = 1;
        if (!strcmp(argv[1], "row")) {
            unsigned long i = num(argv[2]);
            if (i < arcade_full_set_count) s = &arcade_full_sets[i];
        } else if (!read_spec(argv[2], &spec, runs, outside, file, source)) {
            s = &spec;
        }
        module = s ? slurp(argv[3], &size) : NULL;
        src = module ? slurp(argv[4], &source_size) : NULL;
        out = src ? malloc(size + 1u) : NULL;
        changed = out ? malloc(size + 1u) : NULL;
        if (changed) {
            memset(out, 0xee, size);
            memset(changed, 0xee, size);
            if (arcade_full_build(s, module, size, src, source_size, (uint32_t)num(argv[5]), out, changed, err, sizeof err)) {
                printf("refused: %s\n", err);
                status = 3;
            } else if (!dump(argv[6], out, size) && !dump(argv[7], changed, size)) {
                status = 0;
            }
        }
        free(module);
        free(src);
        free(out);
        free(changed);
        return status;
    }
    if (argc == 7 && !strcmp(argv[1], "tiles")) {
        uint32_t source_size = 0, count = 0, room = (uint32_t)num(argv[5]);
        uint8_t *src = slurp(argv[2], &source_size), *out = src ? malloc((size_t)room + 4u) : NULL;
        int status = 1;
        if (out) {
            if (arcade_full_tiles(src, source_size, (uint32_t)num(argv[3]), (uint32_t)num(argv[4]), out, room, &count, err, sizeof err)) {
                printf("refused: %s\n", err);
                status = 3;
            } else if (!dump(argv[6], out, 4u * (size_t)count)) {
                printf("%u\n", count);
                status = 0;
            }
        }
        free(src);
        free(out);
        return status;
    }
    fprintf(stderr, "usage: see the head of this file\n");
    return 1;
}
