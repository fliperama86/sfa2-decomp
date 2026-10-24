/* Driver of test_arcade_module_data.py: made-up programs and rows, and the generated tables only.
 *   module_data_test piece PROGRAM SOURCE SIZE LAYOUT OUT   build one piece (exit 1 when the C refuses it; OUT gets SIZE bytes)
 *   module_data_test same PROGRAM SOURCE SIZE LAYOUT UNITS  build a flat piece twice, with this code and with program.c's
 *                                                           arcade_piece_build (UNITS is its layout letters): prints equal or differ
 *   module_data_test row PROGRAM SPEC OUTDIR                build a made-up row (OUTDIR gets module.bin and known.bin)
 *   module_data_test valid SPEC                             arcade_module_data_valid of a made-up row: prints 0 or -1
 *   module_data_test tables                                 sanity of the checked-in table (the fighter rows against fighters_table.c and sequences_table.c)
 * SPEC is text: "load size piece_count reloc_count", then per piece "offset size source layout", then per relocation "offset target". */
#include "../module_data.h"
#include "../program.h"
#include "../fighter.h"
#include "../sequence.h"
#include <assert.h>
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

/* A guard of 8 bytes after the output: the builder must not touch it. */
static int piece(const char *prog_path, unsigned long source, unsigned long size, const char *layout, const char *out_path)
{
    size_t psize = 0;
    uint8_t *prog = slurp(prog_path, &psize);
    uint8_t *out = malloc(size + 8);
    struct arcade_module_piece p;
    int rc;
    unsigned k;
    if (!prog || !out) return 3;
    memset(out, 0xa5, size + 8);
    p.offset = 0;
    p.size = (uint32_t)size;
    p.source = (uint32_t)source;
    p.layout = layout;
    rc = arcade_module_piece_build(prog, psize, &p, out);
    for (k = 0; k < 8; k++)
        if (out[size + k] != 0xa5) { printf("wrote past the piece\n"); rc = 3; }
    if (rc == 0) {
        FILE *f = fopen(out_path, "wb");
        if (!f || (size && fwrite(out, 1, size, f) != size)) rc = 3;
        if (f) fclose(f);
    }
    free(prog);
    free(out);
    return rc ? (rc == -1 ? 1 : rc) : 0;
}

static int same(const char *prog_path, unsigned long source, unsigned long size, const char *layout, const char *units)
{
    size_t psize = 0;
    uint8_t *prog = slurp(prog_path, &psize);
    uint8_t *a = calloc(size + 1, 1), *b = calloc(size + 1, 1);
    struct arcade_module_piece p;
    struct arcade_piece q;
    int ra, rb;
    if (!prog || !a || !b) return 3;
    p.offset = 0;
    p.size = (uint32_t)size;
    p.source = (uint32_t)source;
    p.layout = layout;
    q.address = 0;
    q.size = (unsigned)size;
    q.source = (unsigned)source;
    q.units = units;
    q.name = "x";
    ra = arcade_module_piece_build(prog, psize, &p, a);
    rb = arcade_piece_build(prog, psize, &q, b);
    if (ra == 0 && rb == 0 && !memcmp(a, b, size)) puts("equal");
    else if (ra && rb) puts("equal");   /* both refuse */
    else puts("differ");
    free(prog);
    free(a);
    free(b);
    return 0;
}

static int load_spec(const char *spec_path, struct arcade_module_data *d, struct arcade_module_piece **pieces, struct arcade_module_reloc **relocs,
                     char (**layouts)[256])
{
    FILE *f = fopen(spec_path, "r");
    unsigned long load, size, np, nr, i;
    if (!f || fscanf(f, "%lu %lu %lu %lu", &load, &size, &np, &nr) != 4) { if (f) fclose(f); return -1; }
    *pieces = calloc(np ? np : 1, sizeof **pieces);
    *relocs = calloc(nr ? nr : 1, sizeof **relocs);
    *layouts = calloc(np ? np : 1, sizeof **layouts);
    if (!*pieces || !*relocs || !*layouts) { fclose(f); return -1; }
    for (i = 0; i < np; i++) {
        unsigned long o, s, src;
        if (fscanf(f, "%lu %lu %lu %255s", &o, &s, &src, (*layouts)[i]) != 4) { fclose(f); return -1; }
        (*pieces)[i].offset = (uint32_t)o;
        (*pieces)[i].size = (uint32_t)s;
        (*pieces)[i].source = (uint32_t)src;
        (*pieces)[i].layout = (*layouts)[i];
    }
    for (i = 0; i < nr; i++) {
        unsigned long o, t;
        if (fscanf(f, "%lu %lu", &o, &t) != 2) { fclose(f); return -1; }
        (*relocs)[i].offset = (uint32_t)o;
        (*relocs)[i].target = (uint32_t)t;
    }
    fclose(f);
    memset(d, 0, sizeof *d);
    d->file = "TEST.PAC";
    d->slot = 4;
    d->load = (uint32_t)load;
    d->size = (uint32_t)size;
    d->pieces = *pieces;
    d->piece_count = (unsigned)np;
    d->relocs = *relocs;
    d->reloc_count = (unsigned)nr;
    return 0;
}

static int row(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t psize = 0;
    uint8_t *prog = slurp(prog_path, &psize);
    struct arcade_module_data d;
    struct arcade_module_piece *pieces;
    struct arcade_module_reloc *relocs;
    char (*layouts)[256];
    uint8_t *module, *known;
    int rc = 3;
    if (!prog || load_spec(spec_path, &d, &pieces, &relocs, &layouts)) return 3;
    module = calloc(d.size ? d.size : 1, 1);
    known = calloc(d.size ? d.size : 1, 1);
    if (module && known) {
        int r = arcade_module_data_build(prog, psize, &d, module, known, d.size);
        if (dump(dir, "module.bin", module, d.size) || dump(dir, "known.bin", known, d.size)) rc = 3;
        else rc = r ? 1 : 0;
        /* a module of another size is refused and nothing is written */
        if (d.size && arcade_module_data_build(prog, psize, &d, module, known, d.size - 1) == 0) rc = 3;
    }
    free(prog); free(pieces); free(relocs); free(layouts); free(module); free(known);
    return rc;
}

static int valid(const char *spec_path)
{
    struct arcade_module_data d;
    struct arcade_module_piece *pieces;
    struct arcade_module_reloc *relocs;
    char (*layouts)[256];
    if (load_spec(spec_path, &d, &pieces, &relocs, &layouts)) return 3;
    printf("%d\n", arcade_module_data_valid(&d));
    free(pieces); free(relocs); free(layouts);
    return 0;
}

struct span { uint32_t a, b; };

static int hits(const struct arcade_module_data *d, const struct span *s, unsigned ns)
{
    unsigned i, k;
    for (k = 0; k < ns; k++) {
        for (i = 0; i < d->piece_count; i++)
            if (d->pieces[i].offset < s[k].b && d->pieces[i].offset + d->pieces[i].size > s[k].a) return 1;
        for (i = 0; i < d->reloc_count; i++)
            if (d->relocs[i].offset < s[k].b && d->relocs[i].offset + 4u > s[k].a) return 1;
    }
    return 0;
}

static int tables(void)
{
    unsigned i, j, mains = 0, extras = 0, screens = 0;
    assert(arcade_module_data_count >= 77);
    for (i = 0; i < arcade_module_data_count; i++) {
        const struct arcade_module_data *d = &arcade_module_data[i];
        const struct arcade_module_data *again = arcade_module_data_find(d->file, d->slot);
        const struct arcade_fighter *f = NULL;
        const struct arcade_sequence *q = NULL;
        struct span s[5];
        unsigned ns = 0;
        int fighter = !strncmp(d->file, "PL", 2);
        assert(arcade_module_data_valid(d) == 0);
        assert(again == d);
        assert(!(d->load & 3u));
        assert(d->piece_count > 0 || d->reloc_count > 0);
        if (!fighter) {   /* the screen and stage rows follow the fighter rows, sorted by file and slot among themselves */
            screens++;
            assert(d->load >= 0x80010000u && d->load < 0x80200000u);
            assert(i);
            if (strncmp(arcade_module_data[i - 1].file, "PL", 2)) assert(strcmp(d->file, arcade_module_data[i - 1].file) >= 0);
            for (j = 0; j < d->reloc_count; j++) {
                uint32_t t = d->relocs[j].target;
                if (t < ARCADE_MODULE_RELATIVE) assert(t < d->size);
                else assert(t < d->load || t >= d->load + d->size);
            }
            continue;
        }
        assert(d->load >= 0x80070000u);
        assert(d->piece_count > 0 && d->reloc_count > 0);
        assert(!screens);
        if (i) assert(strcmp(d->file, arcade_module_data[i - 1].file) >= 0);
        for (j = 0; j < arcade_fighter_count; j++)
            if (!strcmp(arcade_fighters[j].file, d->file)) f = &arcade_fighters[j];
        for (j = 0; j < arcade_sequence_count; j++)
            if (!strcmp(arcade_sequences[j].file, d->file)) q = &arcade_sequences[j];
        if (d->slot == 4 || d->slot == 5) {
            mains++;
            assert(d->load == (d->slot == 4 ? 0x801b0000u : 0x801c8000u));
        } else {
            extras++;
        }
        /* nothing here may lie in a block that fighter.c or sequence.c builds */
        if (f && f->positions_slot == ARCADE_FIGHTER_SAME_CHUNK && (d->slot == 4 || d->slot == 5)) {
            s[ns].a = f->positions; s[ns].b = f->positions + f->positions_room; ns++;
        }
        if (f && f->positions_slot == d->slot) { s[ns].a = f->positions; s[ns].b = f->positions + f->positions_room; ns++; }
        if (f && (d->slot == 4 || d->slot == 5)) { s[ns].a = f->lists; s[ns].b = f->lists + f->lists_room; ns++; }
        if (q && (d->slot == 4 || d->slot == 5)) {
            s[ns].a = q->steps; s[ns].b = q->steps + 12u * q->step_count + 4u * q->table_entries; ns++;
            s[ns].a = q->frames; s[ns].b = q->frames + 16u * q->frame_count; ns++;
        }
        assert(!hits(d, s, ns));
        /* the words that name the module itself point inside it */
        for (j = 0; j < d->reloc_count; j++) {
            uint32_t t = d->relocs[j].target;
            if (t < ARCADE_MODULE_RELATIVE) assert(t < d->size);
            else assert(t < d->load || t >= d->load + d->size);
        }
    }
    assert(mains == 45);
    assert(extras >= 30);
    assert(screens >= 100);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 7 && !strcmp(argv[1], "piece")) return piece(argv[2], strtoul(argv[3], NULL, 10), strtoul(argv[4], NULL, 10), argv[5], argv[6]);
    if (argc == 7 && !strcmp(argv[1], "same")) return same(argv[2], strtoul(argv[3], NULL, 10), strtoul(argv[4], NULL, 10), argv[5], argv[6]);
    if (argc == 5 && !strcmp(argv[1], "row")) return row(argv[2], argv[3], argv[4]);
    if (argc == 3 && !strcmp(argv[1], "valid")) return valid(argv[2]);
    if (argc == 2 && !strcmp(argv[1], "tables")) return tables();
    fprintf(stderr, "usage: see the top of this file\n");
    return 2;
}
