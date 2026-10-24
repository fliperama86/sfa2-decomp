/* Driver of test_arcade_stage.py: made-up arcade programs and graphics, and the generated descriptor table only.
 *   stage_test build PROGRAM TILES SPEC OUTDIR   build one stage from a descriptor in text
 *   stage_test runs TILES SPEC OUTDIR            build a tile run list (shared tiles style)
 *   stage_test pal PROGRAM SPEC OUTDIR           build a palette chunk from runs (the second palette chunk's way)
 *   stage_test table                             sanity of the checked-in descriptor table
 * SPEC (build) is text, one item a line:
 *   stage N | size MODULE PAGES PALETTES OBJECTS | pal LO HI
 *   layer N MAP ACROSS DOWN START FLAGS ID_COUNT ADDRESS...
 *   extra SLOT COUNT FIRST FLIP | hole SLOT COUNT | object FIRST COUNT FLIP   (FIRST may be ZERO or DISC)
 *   exc LAYER CELL SLOT   (SLOT may be NEW) | cknown LAYER WORD... | tknown LAYER WORD...   (64-bit words)
 *   more K COUNT STEP KIND ARG   (KIND 0 rows from ARG, 1 filler)
 *   xmap LAYER MAP BLOCKS ID...   (an extra map of layer 1 or 2: BLOCKS arcade block numbers, two to a page)
 * SPEC (runs) is "TOTAL" then "object" lines; SPEC (pal) is "total BYTES" then "more" lines (the chunk is palettes.bin). OUTDIR gets module.bin pages.bin palettes.bin objects.bin objpal.bin and a
 * .known file for each; a line "error: ..." on stdout and status 1 when the build refuses. */
#include "../stage.h"
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

static int dump(const char *dir, const char *name, const char *ext, const void *p, size_t n)
{
    char path[1024];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s%s", dir, name, ext);
    f = fopen(path, "wb");
    if (!f || (n && fwrite(p, 1, n, f) != n)) return -1;
    return fclose(f);
}

static int dump_piece(const char *dir, const char *name, const struct arcade_stage_piece *p)
{
    return dump(dir, name, ".bin", p->data, p->size) || dump(dir, name, ".known", p->known, p->size);
}

static uint32_t first_word(const char *w)
{
    if (!strcmp(w, "ZERO")) return ARCADE_STAGE_ZERO;
    if (!strcmp(w, "DISC")) return ARCADE_STAGE_DISC;
    return (uint32_t)strtoul(w, NULL, 0);
}

struct spec {
    struct arcade_stage s;
    uint32_t *ids[3];
    struct arcade_stage_slots *extras;
    struct arcade_stage_hole *holes;
    struct arcade_stage_run *objects;
    struct arcade_stage_exc *exc[3];
    unsigned nexc[3];
    uint64_t *known[3][2];
    struct arcade_stage_more *more;
    struct arcade_stage_xmap xm[3][4];
    uint8_t *xids[3][4];
    unsigned nxm[3];
    uint32_t total;
};

static int read_spec(const char *path, struct spec *sp)
{
    FILE *f = fopen(path, "r");
    char word[64], first[64];
    unsigned long a, b, c, d, e, g, h, i;
    unsigned ne = 0, nh = 0, no = 0, nm = 0;
    memset(sp, 0, sizeof *sp);
    sp->more = calloc(256, sizeof *sp->more);
    sp->extras = calloc(4096, sizeof *sp->extras);
    sp->holes = calloc(4096, sizeof *sp->holes);
    sp->objects = calloc(4096, sizeof *sp->objects);
    if (!f) return -1;
    while (fscanf(f, "%63s", word) == 1) {
        if (!strcmp(word, "stage")) { if (fscanf(f, "%lu", &a) != 1) return -3; sp->s.stage = (unsigned)a; }
        else if (!strcmp(word, "total")) { if (fscanf(f, "%lu", &a) != 1) return -3; sp->total = (uint32_t)a; }
        else if (!strcmp(word, "size")) {
            if (fscanf(f, "%lu %lu %lu %lu", &a, &b, &c, &d) != 4) return -3;
            sp->s.size.module = (uint32_t)a; sp->s.size.pages = (uint32_t)b; sp->s.size.palettes = (uint32_t)c; sp->s.size.object_tiles = (uint32_t)d;
        } else if (!strcmp(word, "pal")) {
            if (fscanf(f, "%lu %lu", &a, &b) != 2) return -3;
            sp->s.pal_exact[0] = a; sp->s.pal_exact[1] = b;
        } else if (!strcmp(word, "layer")) {
            if (fscanf(f, "%lu %lu %lu %lu %lu %lu %lu", &a, &b, &c, &d, &e, &g, &h) != 7 || a > 2) return -3;
            sp->s.layer[a].map = (uint32_t)b; sp->s.layer[a].across = (uint16_t)c; sp->s.layer[a].down = (uint16_t)d;
            sp->s.layer[a].start = (uint32_t)e; sp->s.layer[a].flags = (unsigned)g; sp->s.layer[a].ids = (uint16_t)h;
            sp->ids[a] = calloc(h ? h : 1, sizeof(uint32_t));
            for (i = 0; i < h; i++) {
                unsigned long v;
                if (fscanf(f, "%lu", &v) != 1) return -3;
                sp->ids[a][i] = (uint32_t)v;
            }
            sp->s.layer[a].id_at = sp->ids[a];
        } else if (!strcmp(word, "extra")) {
            if (ne >= 4096 || fscanf(f, "%lu %lu %lu %lu", &a, &b, &c, &d) != 4) return -3;
            sp->extras[ne].slot = (uint32_t)a; sp->extras[ne].count = (uint32_t)b; sp->extras[ne].first = (uint32_t)c; sp->extras[ne].flip = (uint32_t)d; ne++;
        } else if (!strcmp(word, "hole")) {
            if (nh >= 4096 || fscanf(f, "%lu %lu", &a, &b) != 2) return -3;
            sp->holes[nh].slot = (uint32_t)a; sp->holes[nh].count = (uint32_t)b; nh++;
        } else if (!strcmp(word, "exc")) {
            if (fscanf(f, "%lu %lu %63s", &a, &b, first) != 3 || a > 2) return -3;
            if (!sp->exc[a]) sp->exc[a] = calloc(4096, sizeof *sp->exc[a]);
            if (sp->nexc[a] >= 4096) return -3;
            sp->exc[a][sp->nexc[a]].cell = (uint32_t)b;
            sp->exc[a][sp->nexc[a]].slot = !strcmp(first, "NEW") ? ARCADE_STAGE_FORCE_NEW : (uint32_t)strtoul(first, NULL, 0);
            sp->nexc[a]++;
        } else if (!strcmp(word, "cknown") || !strcmp(word, "tknown")) {
            int which = word[0] == 't';
            unsigned long long v;
            unsigned w = 0;
            if (fscanf(f, "%lu", &a) != 1 || a > 2) return -3;
            sp->known[a][which] = calloc(1024, sizeof(uint64_t));
            while (fscanf(f, "%llu", &v) == 1 && w < 1024) sp->known[a][which][w++] = v;
        } else if (!strcmp(word, "more")) {
            if (nm >= 256 || fscanf(f, "%lu %lu %lu %lu %lu", &a, &b, &c, &d, &e) != 5) return -3;
            sp->more[nm].k = (uint16_t)a; sp->more[nm].count = (uint16_t)b; sp->more[nm].step = (uint8_t)c; sp->more[nm].kind = (uint8_t)d; sp->more[nm].arg = (uint32_t)e; nm++;
        } else if (!strcmp(word, "xmap")) {
            if (fscanf(f, "%lu %lu %lu", &a, &b, &c) != 3 || a > 2 || sp->nxm[a] >= 4) return -3;
            sp->xids[a][sp->nxm[a]] = calloc(c ? c : 1, 1);
            for (i = 0; i < c; i++) {
                unsigned long v;
                if (fscanf(f, "%lu", &v) != 1) return -3;
                sp->xids[a][sp->nxm[a]][i] = (uint8_t)v;
            }
            sp->xm[a][sp->nxm[a]].map = (uint32_t)b; sp->xm[a][sp->nxm[a]].blocks = (uint16_t)c; sp->xm[a][sp->nxm[a]].ids = sp->xids[a][sp->nxm[a]];
            sp->nxm[a]++;
        } else if (!strcmp(word, "object")) {
            if (no >= 4096 || fscanf(f, "%63s %lu %lu", first, &b, &c) != 3) return -3;
            sp->objects[no].first = first_word(first); sp->objects[no].count = (uint32_t)b; sp->objects[no].flip = (uint32_t)c; no++;
        } else return -3;
    }
    fclose(f);
    sp->s.file = "TEST.PAC";
    sp->s.extras = sp->extras; sp->s.extra_count = ne;
    sp->s.holes = sp->holes; sp->s.hole_count = nh;
    sp->s.objects = sp->objects; sp->s.object_count = no;
    sp->s.more = sp->more; sp->s.more_count = nm;
    for (a = 0; a < 3; a++) {
        sp->s.layer[a].exc = sp->exc[a]; sp->s.layer[a].exc_count = sp->nexc[a];
        sp->s.layer[a].cell_known = sp->known[a][0]; sp->s.layer[a].tile_known = sp->known[a][1];
        sp->s.layer[a].xmaps = sp->nxm[a] ? sp->xm[a] : NULL; sp->s.layer[a].xmap_count = sp->nxm[a];
    }
    return 0;
}

static void free_spec(struct spec *sp)
{
    unsigned n, k;
    for (n = 0; n < 3; n++) {
        free(sp->ids[n]); free(sp->exc[n]); free(sp->known[n][0]); free(sp->known[n][1]);
        for (k = 0; k < 4; k++) free(sp->xids[n][k]);
    }
    free(sp->more);
    free(sp->extras);
    free(sp->holes);
    free(sp->objects);
}

static int build(const char *prog_path, const char *tiles_path, const char *spec_path, const char *dir)
{
    size_t psize = 0, tsize = 0;
    uint8_t *prog = slurp(prog_path, &psize), *tiles = slurp(tiles_path, &tsize);
    struct spec sp;
    struct arcade_stage_out out;
    char err[256];
    int rc = 0;
    if (!prog || !tiles || read_spec(spec_path, &sp)) {
        free(prog);
        free(tiles);
        return 3;
    }
    if (arcade_stage_build(prog, psize, tiles, tsize, &sp.s, &out, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
    } else {
        if (dump_piece(dir, "module", &out.module) || dump_piece(dir, "pages", &out.pages) || dump_piece(dir, "palettes", &out.palettes) ||
            dump_piece(dir, "objects", &out.object_tiles) || dump_piece(dir, "objpal", &out.object_palettes))
            rc = 4;
        arcade_stage_free(&out);
    }
    free_spec(&sp);
    free(prog);
    free(tiles);
    return rc;
}

static int runs(const char *tiles_path, const char *spec_path, const char *dir)
{
    size_t tsize = 0;
    uint8_t *tiles = slurp(tiles_path, &tsize);
    struct spec sp;
    struct arcade_stage_piece p;
    char err[256];
    int rc = 0;
    if (!tiles || read_spec(spec_path, &sp)) {
        free(tiles);
        return 3;
    }
    if (arcade_stage_build_runs(tiles, tsize, sp.s.objects, sp.s.object_count, sp.total, &p, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
    } else {
        if (dump_piece(dir, "runs", &p)) rc = 4;
        arcade_stage_piece_free(&p);
    }
    free_spec(&sp);
    free(tiles);
    return rc;
}

static int pal(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t psize = 0;
    uint8_t *prog = slurp(prog_path, &psize);
    struct spec sp;
    struct arcade_stage_piece p;
    char err[256];
    int rc = 0;
    if (!prog || read_spec(spec_path, &sp)) {
        free(prog);
        return 3;
    }
    if (arcade_stage_build_palette_runs(prog, psize, sp.s.more, sp.s.more_count, sp.total, &p, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
    } else {
        if (dump_piece(dir, "palettes", &p)) rc = 4;
        arcade_stage_piece_free(&p);
    }
    free_spec(&sp);
    free(prog);
    return rc;
}

/* Sanity of the checked-in table: every row has three layers whose ids lie in the program, the sizes are whole tiles and
 * palettes, the layer starts lie on the page grid, and the shared runs add up to a whole number of tiles. */
static int table(void)
{
    unsigned i, n, k;
    uint64_t shared = 0;
    for (i = 0; i < arcade_stage_count; i++) {
        const struct arcade_stage *s = &arcade_stages[i];
        if (!s->file || strncmp(s->file, "STAGE", 5) || s->stage > 0x13u) { printf("bad name/stage at %u\n", i); return 1; }
        if (s->size.palettes % 32u || s->size.object_tiles % 128u || s->size.pages % 32768u || !s->size.module) { printf("bad sizes at %u\n", i); return 1; }
        for (n = 0; n < 3; n++) {
            const struct arcade_stage_layer *l = &s->layer[n];
            unsigned bc = n < 2 ? 16u : 8u;
            if (!l->ids || l->ids != (l->across / bc) * (l->down / bc) || !l->id_at) { printf("bad layer %u at %u\n", n, i); return 1; }
            for (k = 0; k < l->ids; k++)
                if (l->id_at[k] && (l->id_at[k] < 0x1d000u || l->id_at[k] > 0x1e000u)) { printf("id address %#x out of range at %u\n", l->id_at[k], i); return 1; }
            if (l->flags > 7u || ((l->flags & ARCADE_STAGE_START_RULE) && (n == 0 || l->start))) { printf("bad flags at %u\n", i); return 1; }
            if (l->xmap_count && (n == 0 || !l->xmaps)) { printf("bad extra maps at %u\n", i); return 1; }
            for (k = 0; k < l->xmap_count; k++) {
                unsigned h;
                if (!l->xmaps[k].blocks || (l->xmaps[k].blocks & 1u) || !l->xmaps[k].ids) { printf("bad extra map %u at %u\n", k, i); return 1; }
                for (h = 0; h < l->xmaps[k].blocks; h++)
                    if (l->xmaps[k].ids[h] >= (n == 1 ? 154u : 101u)) { printf("extra block out of range at %u\n", i); return 1; }
            }
            for (k = 0; k < l->exc_count; k++)
                if (l->exc[k].cell >= (uint32_t)l->across * l->down || (k && l->exc[k].cell <= l->exc[k - 1].cell)) { printf("bad exception at %u\n", i); return 1; }
        }
        if (!(s->layer[1].flags & ARCADE_STAGE_START_RULE) && s->layer[1].start < s->layer[0].start) { printf("layers out of order at %u\n", i); return 1; }
        for (k = 0; k < s->more_count; k++)
            if (!s->more[k].count || !s->more[k].step || s->more[k].kind > ARCADE_STAGE_MORE_FILL) { printf("bad palette run at %u\n", i); return 1; }
        for (k = 0; k < s->extra_count; k++)
            if (!s->extras[k].count) { printf("empty extra run at %u\n", i); return 1; }
    }
    for (k = 0; k < arcade_stage_shared_count; k++) shared += arcade_stage_shared[k].count;
    if (!arcade_stage_count || !shared) { printf("empty table\n"); return 1; }
    if (arcade_stage_shared_pal_bytes % 32u || (arcade_stage_shared_pal_count && !arcade_stage_shared_pal)) { printf("bad shared palettes\n"); return 1; }
    for (k = 0; k < arcade_stage_shared_pal_count; k++)
        if (!arcade_stage_shared_pal[k].count || !arcade_stage_shared_pal[k].step || arcade_stage_shared_pal[k].k >= arcade_stage_shared_pal_bytes / 32u) { printf("bad shared palette run %u\n", k); return 1; }
    if (!arcade_stage_find("STAGE00.PAC") || arcade_stage_find("NOPE.PAC")) { printf("find fails\n"); return 1; }
    printf("%u stages, %u shared runs of %llu tiles\n", arcade_stage_count, arcade_stage_shared_count, (unsigned long long)shared);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 6 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4], argv[5]);
    if (argc == 5 && !strcmp(argv[1], "runs")) return runs(argv[2], argv[3], argv[4]);
    if (argc == 5 && !strcmp(argv[1], "pal")) return pal(argv[2], argv[3], argv[4]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    return 2;
}
