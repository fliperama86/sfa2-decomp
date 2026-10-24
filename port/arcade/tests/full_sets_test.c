/* Driver of test_arcade_full_sets.py and full_sets.py --compare-c.
 *   full_sets_test table                                     print the linked table (arcade_full_sets), one row each, in the SPEC form
 *   full_sets_test row INDEX MODULE SOURCE LISTS OUT CHANGED build row INDEX of the linked table
 *   full_sets_test spec SPECFILE MODULE SOURCE LISTS OUT CHANGED   build the row the file describes
 *   full_sets_test tiles SOURCE ADDRESS BASE ROOM OUT         arcade_full_tiles; prints the count
 * LISTS, ADDRESS, BASE and ROOM are numbers (0x allowed); OUT and CHANGED are written (size bytes). A refusal prints "refused: message" on
 * standard output and exits with 3; any other failure exits with 1.
 * SPEC: one line `file source` and 13 numbers (slot source_slot tiles_slot source_tiles_slot kind load source_load source_positions
 * positions_size source_lists lists_size positions lists), then the block count and the outside count; then for each block a line of 9 numbers
 * (source_steps step_count source_table_entries steps table table_entries source_frames frames frame_count) and its run count, followed by a
 * line `first count source_first` per run; then a line `target source` per outside step. */
#include "../full_sets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RUNS 512
#define MAX_OUTSIDE 512
#define MAX_BLOCKS 8

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
    unsigned k, b;
    printf("%s %s %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", s->file, s->source, s->slot, s->source_slot, s->tiles_slot, s->source_tiles_slot,
           s->kind, (unsigned)s->load, (unsigned)s->source_load, (unsigned)s->source_positions, (unsigned)s->positions_size,
           (unsigned)s->source_lists, (unsigned)s->lists_size, (unsigned)s->positions, (unsigned)s->lists, s->block_count, s->outside_count);
    for (b = 0; b < s->block_count; b++) {
        const struct arcade_full_block *u = &s->blocks[b];
        printf("%u %u %u %u %u %u %u %u %u %u\n", (unsigned)u->source_steps, u->step_count, u->source_table_entries, (unsigned)u->steps,
               (unsigned)u->table, u->table_entries, (unsigned)u->source_frames, (unsigned)u->frames, u->frame_count, u->run_count);
        for (k = 0; k < u->run_count; k++) printf("%u %u %u\n", u->runs[k].first, u->runs[k].count, u->runs[k].source_first);
    }
    for (k = 0; k < s->outside_count; k++) printf("%u %u\n", (unsigned)s->outside[k].target, (unsigned)s->outside[k].source);
}

struct spec_store {
    struct arcade_full_block blocks[MAX_BLOCKS];
    struct arcade_full_run runs[MAX_BLOCKS][MAX_RUNS];
    struct arcade_full_step outside[MAX_OUTSIDE];
    char file[64], source[64];
};

static int read_spec(const char *path, struct arcade_full_set *s, struct spec_store *st)
{
    FILE *f = fopen(path, "r");
    unsigned long v[15], w[10];
    unsigned k, b;
    if (!f) return -1;
    memset(s, 0, sizeof *s);
    if (fscanf(f, "%63s %63s", st->file, st->source) != 2) goto bad;
    for (k = 0; k < 15; k++)
        if (fscanf(f, "%lu", &v[k]) != 1) goto bad;
    if (v[13] > MAX_BLOCKS || v[14] > MAX_OUTSIDE) goto bad;
    for (b = 0; b < v[13]; b++) {
        struct arcade_full_block *u = &st->blocks[b];
        for (k = 0; k < 10; k++)
            if (fscanf(f, "%lu", &w[k]) != 1) goto bad;
        if (w[9] > MAX_RUNS) goto bad;
        for (k = 0; k < w[9]; k++) {
            unsigned long x, y, z;
            if (fscanf(f, "%lu %lu %lu", &x, &y, &z) != 3) goto bad;
            st->runs[b][k].first = (uint16_t)x;
            st->runs[b][k].count = (uint16_t)y;
            st->runs[b][k].source_first = (uint16_t)z;
        }
        u->source_steps = (uint32_t)w[0]; u->step_count = (uint16_t)w[1]; u->source_table_entries = (uint16_t)w[2]; u->steps = (uint32_t)w[3];
        u->table = (uint32_t)w[4]; u->table_entries = (uint16_t)w[5]; u->source_frames = (uint32_t)w[6]; u->frames = (uint32_t)w[7];
        u->frame_count = (uint16_t)w[8];
        u->runs = st->runs[b];
        u->run_count = (unsigned)w[9];
    }
    for (k = 0; k < v[14]; k++) {
        unsigned long a, b2;
        if (fscanf(f, "%lu %lu", &a, &b2) != 2) goto bad;
        st->outside[k].target = (uint32_t)a;
        st->outside[k].source = (uint32_t)b2;
    }
    fclose(f);
    s->file = st->file;
    s->source = st->source;
    s->slot = (uint16_t)v[0]; s->source_slot = (uint16_t)v[1]; s->tiles_slot = (uint16_t)v[2]; s->source_tiles_slot = (uint16_t)v[3];
    s->kind = (uint16_t)v[4]; s->load = (uint32_t)v[5]; s->source_load = (uint32_t)v[6];
    s->source_positions = (uint32_t)v[7]; s->positions_size = (uint32_t)v[8]; s->source_lists = (uint32_t)v[9]; s->lists_size = (uint32_t)v[10];
    s->positions = (uint32_t)v[11]; s->lists = (uint32_t)v[12];
    s->blocks = st->blocks;
    s->block_count = (unsigned)v[13];
    s->outside = st->outside;
    s->outside_count = (unsigned)v[14];
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
        static struct spec_store store;
        const struct arcade_full_set *s = NULL;
        uint32_t size = 0, source_size = 0;
        uint8_t *module = NULL, *src = NULL, *out = NULL, *changed = NULL;
        int status = 1;
        if (!strcmp(argv[1], "row")) {
            unsigned long i = num(argv[2]);
            if (i < arcade_full_set_count) s = &arcade_full_sets[i];
        } else if (!read_spec(argv[2], &spec, &store)) {
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
