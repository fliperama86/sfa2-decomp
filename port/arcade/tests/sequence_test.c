/* Driver of test_arcade_sequence.py and of `sequences.py --compare-c`: made-up programs and the generated tables only.
 *   sequence_test build PROGRAM SPEC OUTDIR   build one file's steps, frames and table from a made-up program and row
 *   sequence_test lists PROGRAM SPEC OUTDIR   build a made-up fighter, then map its art records to list offsets
 *   sequence_test file PROGRAM NAME OUTDIR    build a file of the checked-in tables from the arcade program (fighter first)
 *   sequence_test table                       sanity of the checked-in table
 * SPEC of build is text:
 *   load steps frames step_count frame_count table_entries rules scale_first scale_count
 *   then a count and that many lines each of: groups "address count", runs "record step count", own "first count",
 *   tables "address count", slices "entry slot count", overrides "step number", lists "record offset".
 * SPEC of lists is the one of fighter_test ("rules positions_room lists_room segment_count", then "address count bank"). */
#include "../sequence.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static int dump_out(const char *dir, const struct arcade_sequence_out *o)
{
    return dump(dir, "steps.bin", o->steps, (size_t)o->step_count * ARCADE_SEQUENCE_STEP) || dump(dir, "steps_built.bin", o->steps_built, o->step_count) ||
           dump(dir, "frames.bin", o->frames, (size_t)o->frame_count * ARCADE_SEQUENCE_FRAME) || dump(dir, "frames_built.bin", o->frames_built, o->frame_count) ||
           dump(dir, "table.bin", o->table, (size_t)o->table_entries * 4) || dump(dir, "table_built.bin", o->table_built, o->table_entries);
}

static int build(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t size = 0, i;
    uint8_t *prog = slurp(prog_path, &size);
    FILE *sp = fopen(spec_path, "r");
    struct arcade_sequence s;
    struct arcade_sequence_group *groups = NULL;
    struct arcade_sequence_run *runs = NULL;
    struct arcade_sequence_span *own = NULL;
    struct arcade_sequence_table *tables = NULL;
    struct arcade_sequence_slice *slices = NULL;
    struct arcade_sequence_frame *over = NULL;
    struct arcade_sequence_lists lists;
    struct arcade_sequence_out out;
    unsigned long v[9], a, b, c;
    unsigned n;
    char err[256];
    int rc = 2;

    if (!prog || !sp) return 3;
    for (i = 0; i < 9; i++)
        if (fscanf(sp, "%lu", &v[i]) != 1) return 3;
    memset(&s, 0, sizeof s);
    memset(&lists, 0, sizeof lists);
    s.file = "TEST.PAC";
    s.load = (uint32_t)v[0];
    s.steps = (uint32_t)v[1];
    s.frames = (uint32_t)v[2];
    s.step_count = (uint16_t)v[3];
    s.frame_count = (uint16_t)v[4];
    s.table_entries = (uint16_t)v[5];
    s.rules = (unsigned)v[6];
    s.scale_first = (uint16_t)v[7];
    s.scale_count = (uint16_t)v[8];
    if (fscanf(sp, "%u", &n) != 1) return 3;
    groups = calloc(n ? n : 1, sizeof *groups);
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu", &a, &b) != 2) return 3;
        groups[i].address = (uint32_t)a;
        groups[i].count = (uint16_t)b;
    }
    s.groups = groups;
    s.group_count = n;
    if (fscanf(sp, "%u", &n) != 1) return 3;
    runs = calloc(n ? n : 1, sizeof *runs);
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu %lu", &a, &b, &c) != 3) return 3;
        runs[i].record = (uint16_t)a;
        runs[i].step = (uint16_t)b;
        runs[i].count = (uint16_t)c;
    }
    s.runs = runs;
    s.run_count = n;
    if (fscanf(sp, "%u", &n) != 1) return 3;
    own = calloc(n ? n : 1, sizeof *own);
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu", &a, &b) != 2) return 3;
        own[i].first = (uint16_t)a;
        own[i].count = (uint16_t)b;
    }
    s.own = own;
    s.own_count = n;
    if (fscanf(sp, "%u", &n) != 1) return 3;
    tables = calloc(n ? n : 1, sizeof *tables);
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu", &a, &b) != 2) return 3;
        tables[i].address = (uint32_t)a;
        tables[i].count = (uint16_t)b;
    }
    s.tables = tables;
    s.table_count = n;
    if (fscanf(sp, "%u", &n) != 1) return 3;
    slices = calloc(n ? n : 1, sizeof *slices);
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu %lu", &a, &b, &c) != 3) return 3;
        slices[i].entry = (uint16_t)a;
        slices[i].slot = (uint16_t)b;
        slices[i].count = (uint16_t)c;
    }
    s.slices = slices;
    s.slice_count = n;
    if (fscanf(sp, "%u", &n) != 1) return 3;
    over = calloc(n ? n : 1, sizeof *over);
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu", &a, &b) != 2) return 3;
        over[i].step = (uint16_t)a;
        over[i].number = (uint16_t)b;
    }
    s.overrides = over;
    s.override_count = n;
    if (fscanf(sp, "%u", &n) != 1) return 3;
    lists.item = calloc(n ? n : 1, sizeof *lists.item);
    lists.count = n;
    for (i = 0; i < n; i++) {
        if (fscanf(sp, "%lu %lu", &a, &b) != 2) return 3;
        lists.item[i].record = (uint32_t)a;
        lists.item[i].offset = (uint32_t)b;
    }
    /* the lists must be sorted by record for the C; the spec gives them so */
    if (arcade_sequence_build(prog, size, &s, &lists, &out, err, sizeof err) != 0) {
        printf("error: %s\n", err);
        if (out.steps || out.frames || out.table || out.steps_built) printf("leak: output left allocated\n");
        rc = 1;
    } else {
        if (dump_out(dir, &out)) return 3;
        printf("ok\n");
        arcade_sequence_free(&out);
        if (out.steps || out.frames_built) printf("leak: free did not clear\n");
        rc = 0;
    }
    free(groups);
    free(runs);
    free(own);
    free(tables);
    free(slices);
    free(over);
    free(lists.item);
    free(prog);
    fclose(sp);
    return rc;
}

static int lists_mode(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t size = 0, i;
    uint8_t *prog = slurp(prog_path, &size);
    FILE *sp = fopen(spec_path, "r");
    struct arcade_fighter f;
    struct arcade_fighter_segment *segs;
    struct arcade_fighter_out fo;
    struct arcade_sequence_lists lists;
    unsigned rules, pr, lr, n;
    char err[256], path[1024];
    FILE *t;
    int rc = 2;

    if (!prog || !sp || fscanf(sp, "%u %u %u %u", &rules, &pr, &lr, &n) != 4) return 3;
    segs = calloc(n ? n : 1, sizeof *segs);
    for (i = 0; i < n; i++) {
        unsigned long a, b;
        unsigned c;
        if (fscanf(sp, "%lu %u %lu", &a, &c, &b) != 3) return 3;
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
    if (arcade_fighter_build(prog, size, &f, &fo, err, sizeof err) != 0) {
        printf("error: fighter: %s\n", err);
        rc = 1;
    } else if (arcade_sequence_lists(prog, size, &f, &fo, &lists, err, sizeof err) != 0) {
        printf("error: %s\n", err);
        if (lists.item) printf("leak: lists left allocated\n");
        arcade_fighter_free(&fo);
        rc = 1;
    } else {
        snprintf(path, sizeof path, "%s/lists.txt", dir);
        t = fopen(path, "w");
        if (!t) return 3;
        for (i = 0; i < lists.count; i++) fprintf(t, "%u %u\n", (unsigned)lists.item[i].record, (unsigned)lists.item[i].offset);
        fclose(t);
        printf("ok\n");
        arcade_sequence_lists_free(&lists);
        arcade_fighter_free(&fo);
        rc = 0;
    }
    free(segs);
    free(prog);
    fclose(sp);
    return rc;
}

static int file_mode(const char *prog_path, const char *name, const char *dir)
{
    size_t size = 0, i;
    uint8_t *prog = slurp(prog_path, &size);
    const struct arcade_sequence *s = arcade_sequence_find(name);
    const struct arcade_fighter *f = NULL;
    struct arcade_fighter_out fo;
    struct arcade_sequence_lists lists;
    struct arcade_sequence_out out;
    char err[256], path[1024];
    clock_t t0, t1;
    FILE *t;

    if (!prog || !s) {
        printf("error: no table row for %s\n", name);
        return 2;
    }
    for (i = 0; i < arcade_fighter_count; i++)
        if (!strcmp(arcade_fighters[i].file, name)) f = &arcade_fighters[i];
    if (!f) {
        printf("error: no fighter row for %s\n", name);
        return 2;
    }
    if (arcade_fighter_build(prog, size, f, &fo, err, sizeof err)) {
        printf("error: fighter: %s\n", err);
        return 1;
    }
    t0 = clock();
    if (arcade_sequence_lists(prog, size, f, &fo, &lists, err, sizeof err) || arcade_sequence_build(prog, size, s, &lists, &out, err, sizeof err)) {
        printf("error: %s\n", err);
        return 1;
    }
    t1 = clock();
    if (dump_out(dir, &out)) return 3;
    snprintf(path, sizeof path, "%s/lists.txt", dir);
    t = fopen(path, "w");
    if (!t) return 3;
    for (i = 0; i < lists.count; i++) fprintf(t, "%u %u\n", (unsigned)lists.item[i].record, (unsigned)lists.item[i].offset);
    fclose(t);
    printf("ok %.3f\n", 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC);
    arcade_sequence_free(&out);
    arcade_sequence_lists_free(&lists);
    arcade_fighter_free(&fo);
    free(prog);
    return 0;
}

static int table(void)
{
    unsigned i, j, k;
    if (!arcade_sequence_count) return 1;
    for (i = 0; i < arcade_sequence_count; i++) {
        const struct arcade_sequence *s = &arcade_sequences[i];
        unsigned covered = 0, end = 0, records = 0;
        if (!s->file || strncmp(s->file, "PL", 2) || !arcade_fighter_count) return 1;
        for (j = 0; j < i; j++)
            if (!strcmp(arcade_sequences[j].file, s->file)) return 1;
        for (j = 0; j < arcade_fighter_count && strcmp(arcade_fighters[j].file, s->file); j++) {}
        if (j == arcade_fighter_count) return 1;                          /* every file has its fighter row */
        if (s->load != 0x801b0000u && s->load != 0x801c8000u) return 1;
        if ((s->load == 0x801b0000u) != (arcade_fighters[j].module_slot == 4)) return 1;
        if (!s->step_count || !s->frame_count || !s->group_count || !s->run_count) return 1;
        if ((s->steps & 3) || (s->frames & 3)) return 1;
        if (s->rules & ~1u) return 1;
        for (k = 0; k < s->group_count; k++) {                             /* ascending, not overlapping (24 bytes a record at least) */
            if (!s->groups[k].count || (s->groups[k].address & 1)) return 1;
            if (k && s->groups[k].address < s->groups[k - 1].address + 24u * s->groups[k - 1].count) return 1;
            records += s->groups[k].count;
        }
        if ((unsigned)s->scale_first + s->scale_count > records) return 1;
        for (k = 0; k < s->run_count; k++) {                               /* ascending in both, inside, disjoint */
            const struct arcade_sequence_run *r = &s->runs[k];
            if (!r->count || (unsigned)r->record + r->count > records || (unsigned)r->step + r->count > s->step_count) return 1;
            if (k && (r->record < s->runs[k - 1].record + s->runs[k - 1].count || r->step < s->runs[k - 1].step + s->runs[k - 1].count)) return 1;
            covered += r->count;
        }
        for (k = 0; k < s->own_count; k++) {                               /* the own spans are the steps no run makes */
            if (!s->own[k].count || (unsigned)s->own[k].first + s->own[k].count > s->step_count) return 1;
            if (k && s->own[k].first < end) return 1;
            end = (unsigned)s->own[k].first + s->own[k].count;
            covered += s->own[k].count;
            for (j = 0; j < s->run_count; j++)
                if (s->own[k].first < (unsigned)s->runs[j].step + s->runs[j].count && s->runs[j].step < (unsigned)s->own[k].first + s->own[k].count) return 1;
        }
        if (covered != s->step_count) return 1;
        for (k = 0; k < s->slice_count; k++)
            if (!s->slices[k].count || (unsigned)s->slices[k].slot + s->slices[k].count > s->table_entries) return 1;
        for (k = 0; k < s->override_count; k++)
            if (s->overrides[k].step >= s->step_count || s->overrides[k].number >= s->frame_count) return 1;
        for (k = 0; k < s->table_count; k++)
            if (!s->tables[k].count || (s->tables[k].address & 1)) return 1;
    }
    printf("table: %u files\n", arcade_sequence_count);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 5 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4]);
    if (argc == 5 && !strcmp(argv[1], "lists")) return lists_mode(argv[2], argv[3], argv[4]);
    if (argc == 5 && !strcmp(argv[1], "file")) return file_mode(argv[2], argv[3], argv[4]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    fprintf(stderr, "usage: sequence_test build|lists PROGRAM SPEC OUTDIR | file PROGRAM NAME OUTDIR | table\n");
    return 64;
}
