/* Driver of test_arcade_screens.py: made-up arcade programs and graphics, and the generated recipe table only.
 *   screens_test build PROGRAM TILES SPEC OUTDIR [DISC]   build one region of a made-up chunk from a spec in text
 *   screens_test table                                    sanity of the checked-in recipe table
 *   screens_test all                                      build every recipe of the table with a blank arcade of the real sizes
 * SPEC is text, one item a line:
 *   region KIND OFFSET SIZE WIDTH      (KIND is SEG, RAW, PALS or PIC; the chunk is OFFSET + SIZE bytes)
 *   item FIRST COUNT FLIP              (FIRST may be ZERO, DISC or PIECES; FLIP 2147483648 on a ZERO item is by design)
 *   op KIND X Y W H FIRST STEPX STEPY FLIP   (KIND is TILE, PIECE, FLAT, DISC or BLANK)
 *   piece A B C D
 *   head WORD                          (the archive header's first word of the chunk: bytes 0 to 3 built by design take it, known 1)
 * OUTDIR gets out.bin and out.known; stdout gets "unbuilt N design M" on success, or "error: ..." and status 1 when the build refuses. */
#include "../screens.h"
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

static uint32_t word(const char *w)
{
    if (!strcmp(w, "ZERO")) return ARCADE_SCREENS_ZERO;
    if (!strcmp(w, "DISC")) return ARCADE_SCREENS_DISC;
    if (!strcmp(w, "PIECES")) return ARCADE_SCREENS_PIECES;
    if (!strcmp(w, "WINDOW")) return ARCADE_SCREENS_WINDOW;
    return (uint32_t)strtoul(w, NULL, 0);
}

static unsigned kind_of(const char *w)
{
    static const char *const names[] = {"SEG", "RAW", "PALS", "PIC"};
    unsigned i;
    for (i = 0; i < 4; i++)
        if (!strcmp(w, names[i])) return i + 1;
    return 99;
}

static unsigned op_of(const char *w)
{
    static const char *const names[] = {"TILE", "PIECE", "FLAT", "DISC", "BLANK"};
    unsigned i;
    for (i = 0; i < 5; i++)
        if (!strcmp(w, names[i])) return i + 1;
    return 99;
}

static int build(int argc, char **argv)
{
    size_t psize, tsize, dsize = 0;
    uint8_t *prog = slurp(argv[2], &psize), *tiles = slurp(argv[3], &tsize), *disc = argc > 6 ? slurp(argv[6], &dsize) : NULL;
    FILE *f = fopen(argv[4], "r");
    struct arcade_screens_region g;
    struct arcade_screens_item *items = calloc(8192, sizeof *items);
    struct arcade_screens_op *ops = calloc(8192, sizeof *ops);
    uint32_t (*pieces)[4] = calloc(1024, sizeof *pieces);
    struct arcade_screens_source *sources = calloc(256, sizeof *sources);
    struct arcade_screens_window *windows = calloc(1024, sizeof *windows);
    unsigned ni = 0, no = 0, np = 0, ns = 0, nw = 0, rc = 0, has_head = 0;
    uint32_t head = 0;
    char w[64], w2[64];
    unsigned long a, b, c, d, e;
    struct arcade_screens_out out;
    char err[256];
    if (!prog || !tiles || !f || !items || !ops || !pieces || !sources || !windows) return 3;
    memset(&g, 0, sizeof g);
    while (fscanf(f, "%63s", w) == 1) {
        if (!strcmp(w, "region")) {
            if (fscanf(f, "%63s %lu %lu %lu", w2, &a, &b, &c) != 4) return 3;
            g.kind = (uint8_t)kind_of(w2); g.offset = (uint32_t)a; g.size = (uint32_t)b; g.width = (uint32_t)c;
        } else if (!strcmp(w, "item")) {
            if (ni >= 8192 || fscanf(f, "%63s %lu %lu", w2, &a, &b) != 3) return 3;
            items[ni].first = word(w2); items[ni].count = (uint32_t)a; items[ni].flip = (uint32_t)b; ni++;
        } else if (!strcmp(w, "op")) {
            long sx, sy;
            if (no >= 8192 || fscanf(f, "%63s %lu %lu %lu %lu %63s %ld %ld %lu", w2, &a, &b, &c, &d, w, &sx, &sy, &e) != 9) return 3;
            ops[no].kind = (uint8_t)op_of(w2); ops[no].x = (uint16_t)a; ops[no].y = (uint16_t)b; ops[no].w = (uint16_t)c; ops[no].h = (uint16_t)d;
            ops[no].first = word(w); ops[no].stepx = (int32_t)sx; ops[no].stepy = (int32_t)sy; ops[no].flip = (uint8_t)e; no++;
        } else if (!strcmp(w, "piece")) {
            if (np >= 1024 || fscanf(f, "%lu %lu %lu %lu", &a, &b, &c, &d) != 4) return 3;
            pieces[np][0] = (uint32_t)a; pieces[np][1] = (uint32_t)b; pieces[np][2] = (uint32_t)c; pieces[np][3] = (uint32_t)d; np++;
        } else if (!strcmp(w, "head")) {
            if (fscanf(f, "%lu", &a) != 1) return 3;
            head = (uint32_t)a; has_head = 1;
        } else if (!strcmp(w, "source")) {
            if (ns >= 256 || fscanf(f, "%63s %lu %lu %lu", w2, &a, &b, &c) != 4) return 3;
            sources[ns].kind = !strcmp(w2, "RECORD") ? ARCADE_SCREENS_SRC_RECORD : !strcmp(w2, "SCROLL2") ? ARCADE_SCREENS_SRC_SCROLL2 : 9;
            sources[ns].address = (uint32_t)a; sources[ns].bank = (uint32_t)b; sources[ns].blocks = (uint16_t)c; ns++;
        } else if (!strcmp(w, "window")) {
            long x, y;
            if (nw >= 1024 || fscanf(f, "%lu %ld %ld %lu %lu", &a, &x, &y, &b, &c) != 5) return 3;
            windows[nw].source = (uint16_t)a; windows[nw].x = (int16_t)x; windows[nw].y = (int16_t)y; windows[nw].flip = (uint8_t)b; windows[nw].rows = (uint8_t)c; nw++;
        } else return 3;
    }
    fclose(f);
    g.first = 0;
    g.count = g.kind == ARCADE_SCREENS_PIC ? no : ni;
    if (arcade_screens_build_region(prog, psize, tiles, tsize, &g, items, ops, (const uint32_t (*)[4])pieces, np, sources, ns, windows, nw, disc, dsize, &out, err, sizeof err) != 0) {
        printf("error: %s\n", err);
        rc = 1;
    } else {
        if (has_head) arcade_screens_head_word_regions(&g, 1, &out, head);
        printf("unbuilt %u design %u\n", out.unbuilt, out.design);
        if (disc) {
            struct arcade_screens_out o2 = out;
            char e2[256];
            if (arcade_screens_verify_regions(&g, 1, items, &o2, disc, dsize, e2, sizeof e2) == 0)
                printf("verify ok\n");
            else
                printf("verify: %s\n", e2);
        }
        if (dump(argv[5], "out.bin", out.data, out.size) || dump(argv[5], "out.known", out.known, out.size)) rc = 3;
        arcade_screens_free(&out);
    }
    free(prog); free(tiles); free(disc); free(items); free(ops); free(pieces); free(sources); free(windows);
    return (int)rc;
}

/* The checked-in table: every use names a recipe, every recipe its regions, every region its items or ops, and everything fits. */
static int table(void)
{
    unsigned i, j, bad = 0, chunks = 0;
    for (i = 0; i < arcade_screens_file_count; i++) {
        const struct arcade_screens_file *f = &arcade_screens_files[i];
        if (!f->file || !f->count) { printf("file %u has no name or chunks\n", i); bad++; continue; }
        for (j = i + 1; j < arcade_screens_file_count; j++)
            if (!strcmp(f->file, arcade_screens_files[j].file)) { printf("file %s twice\n", f->file); bad++; }
        for (j = 0; j < f->count; j++) {
            const struct arcade_screens_use *u = &arcade_screens_uses[f->first + j];
            if (u->recipe >= arcade_screens_recipe_count) { printf("%s: recipe %u\n", f->file, u->recipe); bad++; continue; }
            if (arcade_screens_find(f->file) != f || arcade_screens_recipe_of(f, u->table, u->slot, u->ordinal) != &arcade_screens_recipes[u->recipe]) {
                printf("%s: lookup of table %u slot %#x fails\n", f->file, u->table, u->slot);
                bad++;
            }
            chunks++;
        }
    }
    for (i = 0; i < arcade_screens_recipe_count; i++) {
        const struct arcade_screens_recipe *r = &arcade_screens_recipes[i];
        for (j = 0; j < r->count; j++) {
            const struct arcade_screens_region *g = &arcade_screens_regions[r->first + j];
            if ((uint64_t)g->offset + g->size > r->size || g->kind < 1 || g->kind > 4) { printf("recipe %u region %u\n", i, j); bad++; }
        }
    }
    if (arcade_screens_find("NO-SUCH-FILE.PAC")) bad++;
    for (i = 0; i < arcade_screens_window_count; i++)
        if (arcade_screens_windows[i].source >= arcade_screens_source_count || arcade_screens_windows[i].rows > 2) { printf("window %u\n", i); bad++; }
    printf("%u files, %u chunks, %u recipes, %u pieces, %u windows of %u sources: %s\n", arcade_screens_file_count, chunks, arcade_screens_recipe_count, arcade_screens_piece_count,
           arcade_screens_window_count, arcade_screens_source_count, bad ? "BAD" : "ok");
    return bad ? 1 : 0;
}

/* Every recipe of the checked-in table built with a blank arcade of the real views' sizes and no disc: nothing may be refused or overrun. */
static int all(void)
{
    enum { PROGRAM = 0x300000, TILES = 0x1400000 };
    uint8_t *prog = calloc(PROGRAM, 1), *tiles = calloc(TILES, 1);
    unsigned i, j, built = 0, unbuilt = 0, bad = 0;
    char err[256];
    if (!prog || !tiles) return 3;
    /* a record source of the table is a made-up record of one cell of tile code 0 at (0, 0): mode 0, 1 cell, coordinates at 0x1000 */
    for (i = 0; i < arcade_screens_source_count; i++) {
        const struct arcade_screens_source *s = &arcade_screens_sources[i];
        if (s->kind == ARCADE_SCREENS_SRC_RECORD && (uint64_t)s->address + 10 < PROGRAM) {
            prog[s->address + 3] = 1;
            prog[0x9f260 + 2] = 0x10;
        }
    }
    for (i = 0; i < arcade_screens_file_count; i++) {
        const struct arcade_screens_file *f = &arcade_screens_files[i];
        for (j = 0; j < f->count; j++) {
            const struct arcade_screens_use *u = &arcade_screens_uses[f->first + j];
            struct arcade_screens_out out;
            if (arcade_screens_build(prog, PROGRAM, tiles, TILES, &arcade_screens_recipes[u->recipe], NULL, 0, &out, err, sizeof err)) {
                printf("%s table %u slot %#x: %s\n", f->file, u->table, u->slot, err);
                bad++;
                continue;
            }
            built++;
            unbuilt += out.unbuilt;
            arcade_screens_free(&out);
        }
    }
    printf("%u chunks built, %u segments left (tiles missing without a disc), %u refused\n", built, unbuilt, bad);
    free(prog);
    free(tiles);
    return bad ? 1 : 0;
}

int main(int argc, char **argv)
{
    if (argc >= 6 && !strcmp(argv[1], "build")) return build(argc, argv);
    if (argc >= 2 && !strcmp(argv[1], "table")) return table();
    if (argc >= 2 && !strcmp(argv[1], "all")) return all();
    fprintf(stderr, "usage: screens_test build PROGRAM TILES SPEC OUTDIR [DISC] | table\n");
    return 2;
}
