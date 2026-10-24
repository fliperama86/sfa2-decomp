/* Driver of test_arcade_screen_lists.py: synthetic programs only (and the generated table's sanity).
 *   screen_lists_test build PROGRAM TILES SPEC OUTDIR   build one chain
 *   screen_lists_test backdrop PROGRAM TILES SPEC OUTDIR   build one backdrop cell map
 *   screen_lists_test table                             sanity of the checked-in table
 * SPEC of build is text: "rules positions_size lists_size pool_count item_count exception_count pair_count map_count window_count win_block_count",
 * then pool_count lines "kind code flip", item_count lines "kind count address bank map list_bytes stream_bytes", exception_count lines
 * "list cell value", pair_count lines "from to", map_count lines "first count", window_count lines "x0 y0 c0 r0 w h first_block block_count" and
 * win_block_count lines "rx ry block". OUTDIR gets lists.bin, lists_known.bin, positions.bin and positions_known.bin.
 * SPEC of backdrop is text: "columns rows band y0 first_page clut run_count", then run_count lines "columns set step x"; the picture
 * is the file OUTDIR/picture.bin with the mask OUTDIR/picture_known.bin; OUTDIR gets map.bin and map_known.bin. */
#include "../screen_lists.h"
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

static int build(const char *prog_path, const char *tiles_path, const char *spec_path, const char *dir)
{
    size_t psize = 0, tsize = 0;
    uint8_t *prog = slurp(prog_path, &psize), *tiles = slurp(tiles_path, &tsize);
    FILE *sf = fopen(spec_path, "r");
    struct arcade_screen_lists_spec spec;
    struct arcade_screen_lists_tile *pool;
    struct arcade_screen_lists_item *items;
    struct arcade_screen_lists_exception *exc;
    struct arcade_screen_lists_pal *pals;
    struct arcade_screen_lists_map *maps;
    struct arcade_screen_lists_window *wins;
    struct arcade_screen_lists_win_block *wbs;
    struct arcade_screen_lists_out out;
    unsigned rules, ps, ls, np, ni, ne, nq, nm, nw, nb, i;
    char err[256];
    int rc;

    if (!prog || !tiles || !sf || fscanf(sf, "%u %u %u %u %u %u %u %u %u %u", &rules, &ps, &ls, &np, &ni, &ne, &nq, &nm, &nw, &nb) != 10) return 3;
    wins = calloc(nw + 1, sizeof *wins);
    wbs = calloc(nb + 1, sizeof *wbs);
    pool = calloc(np + 1, sizeof *pool);
    items = calloc(ni + 1, sizeof *items);
    exc = calloc(ne + 1, sizeof *exc);
    pals = calloc(nq + 1, sizeof *pals);
    maps = calloc(nm + 1, sizeof *maps);
    for (i = 0; i < np; i++) {
        unsigned k, c, f;
        if (fscanf(sf, "%u %u %u", &k, &c, &f) != 3) return 3;
        pool[i].kind = (uint8_t)k;
        pool[i].code = c;
        pool[i].flip = (uint8_t)f;
    }
    for (i = 0; i < ni; i++) {
        unsigned k, c, a, b, m, lb, sb;
        if (fscanf(sf, "%u %u %u %u %u %u %u", &k, &c, &a, &b, &m, &lb, &sb) != 7) return 3;
        items[i].kind = (uint8_t)k;
        items[i].count = (uint16_t)c;
        items[i].address = a;
        items[i].bank = b;
        items[i].map = (uint16_t)m;
        items[i].list_bytes = lb;
        items[i].stream_bytes = sb;
    }
    for (i = 0; i < ne; i++)
        if (fscanf(sf, "%u %u %u", &exc[i].list, &exc[i].cell, &exc[i].palette) != 3) return 3;
    for (i = 0; i < nq; i++) {
        unsigned a, b;
        if (fscanf(sf, "%u %u", &a, &b) != 2) return 3;
        pals[i].from = (uint8_t)a;
        pals[i].to = (uint8_t)b;
    }
    for (i = 0; i < nm; i++) {
        unsigned a, b;
        if (fscanf(sf, "%u %u", &a, &b) != 2) return 3;
        maps[i].first = (uint16_t)a;
        maps[i].count = (uint16_t)b;
    }
    for (i = 0; i < nw; i++) {
        int x, y;
        unsigned c, r, w, h, f, n;
        if (fscanf(sf, "%d %d %u %u %u %u %u %u", &x, &y, &c, &r, &w, &h, &f, &n) != 8) return 3;
        wins[i].x0 = (int16_t)x;
        wins[i].y0 = (int16_t)y;
        wins[i].c0 = (uint8_t)c;
        wins[i].r0 = (uint8_t)r;
        wins[i].w = (uint8_t)w;
        wins[i].h = (uint8_t)h;
        wins[i].first_block = (uint16_t)f;
        wins[i].block_count = (uint16_t)n;
    }
    for (i = 0; i < nb; i++) {
        unsigned rx, ry, b;
        if (fscanf(sf, "%u %u %u", &rx, &ry, &b) != 3) return 3;
        wbs[i].rx = (uint8_t)rx;
        wbs[i].ry = (uint8_t)ry;
        wbs[i].block = (uint16_t)b;
    }
    memset(&spec, 0, sizeof spec);
    spec.windows = wins;
    spec.window_count = nw;
    spec.win_blocks = wbs;
    spec.win_block_count = nb;
    spec.rules = rules;
    spec.positions_size = ps;
    spec.lists_size = ls;
    spec.items = items;
    spec.item_count = ni;
    spec.exceptions = exc;
    spec.exception_count = ne;
    spec.pals = pals;
    spec.maps = maps;
    spec.map_count = nm;
    rc = arcade_screen_lists_build_with(prog, psize, tiles, tsize, pool, np, &spec, &out, err, sizeof err);
    free(pool); free(items); free(exc); free(pals); free(maps); free(wins); free(wbs); free(prog); free(tiles); fclose(sf);
    if (rc) {
        printf("refused: %s\n", err);
        return 1;
    }
    if (dump(dir, "lists.bin", out.lists, out.lists_size) || dump(dir, "lists_known.bin", out.lists_known, out.lists_size) ||
        dump(dir, "positions.bin", out.positions, out.positions_size) || dump(dir, "positions_known.bin", out.positions_known, out.positions_size))
        return 4;
    arcade_screen_lists_free(&out);
    return 0;
}

static int backdrop(const char *prog_path, const char *tiles_path, const char *spec_path, const char *dir)
{
    size_t psize = 0, tsize = 0, picsize = 0, knownsize = 0;
    char path[1024];
    uint8_t *prog = slurp(prog_path, &psize), *tiles = slurp(tiles_path, &tsize), *pic, *pk, *out, *known;
    FILE *sf = fopen(spec_path, "r");
    struct arcade_screen_lists_backdrop bd;
    unsigned run_count, i;
    char err[256];
    int rc;

    if (!prog || !tiles || !sf) return 3;
    memset(&bd, 0, sizeof bd);
    bd.file = "TEST.PAC";
    {
        unsigned co, ro, ba, y0, fp, cl;
        if (fscanf(sf, "%u %u %u %u %u %u %u", &co, &ro, &ba, &y0, &fp, &cl, &run_count) != 7 || run_count > 4) return 3;
        bd.columns = (uint16_t)co; bd.rows = (uint16_t)ro; bd.band = (uint16_t)ba; bd.y0 = (uint16_t)y0; bd.first_page = (uint16_t)fp; bd.clut = (uint16_t)cl;
    }
    bd.run_count = run_count;
    for (i = 0; i < run_count; i++) {
        unsigned c, s, t, x;
        if (fscanf(sf, "%u %u %u %u", &c, &s, &t, &x) != 4) return 3;
        bd.runs[i].columns = (uint16_t)c; bd.runs[i].set = (uint16_t)s; bd.runs[i].step = (uint16_t)t; bd.runs[i].x = (uint16_t)x;
    }
    snprintf(path, sizeof path, "%s/picture.bin", dir);
    pic = slurp(path, &picsize);
    snprintf(path, sizeof path, "%s/picture_known.bin", dir);
    pk = slurp(path, &knownsize);
    if (!pic || !pk || picsize != knownsize) return 3;
    out = calloc((size_t)bd.columns * bd.rows * 4 + 1, 1);
    known = calloc((size_t)bd.columns * bd.rows * 4 + 1, 1);
    rc = arcade_screen_lists_backdrop_build(prog, psize, tiles, tsize, &bd, pic, pk, picsize, out, known, (size_t)bd.columns * bd.rows * 4, err, sizeof err);
    if (rc) {
        printf("refused: %s\n", err);
        return 1;
    }
    if (dump(dir, "map.bin", out, (size_t)bd.columns * bd.rows * 4) || dump(dir, "map_known.bin", known, (size_t)bd.columns * bd.rows * 4)) return 4;
    free(out); free(known); free(pic); free(pk); free(prog); free(tiles); fclose(sf);
    return 0;
}

static int table(void)
{
    unsigned i, bad = 0;
    for (i = 0; i < arcade_screen_lists_chain_count; i++) {
        const struct arcade_screen_lists_chain *c = &arcade_screen_lists_chains[i];
        unsigned k;
        if (c->first_item + c->item_count > arcade_screen_lists_item_count) { printf("chain %u: items outside the table\n", i); bad++; }
        for (k = 0; k < c->item_count && c->first_item + k < arcade_screen_lists_item_count; k++) {
            const struct arcade_screen_lists_item *m = &arcade_screen_lists_items[c->first_item + k];
            if (m->kind > ARCADE_SCREEN_LISTS_WIN || m->map > arcade_screen_lists_map_count) { printf("chain %u item %u: kind or map\n", i, k); bad++; }
            if (m->kind == ARCADE_SCREEN_LISTS_WIN && (m->address >= arcade_screen_lists_window_count || m->bank != 0x10000u)) { printf("chain %u item %u: window\n", i, k); bad++; }
            if (m->kind == ARCADE_SCREEN_LISTS_REC && (!m->count || !m->address || (m->bank & 0xffffu))) { printf("chain %u item %u: record run\n", i, k); bad++; }
        }
    }
    for (i = 0; i < arcade_screen_lists_use_count; i++) {
        const struct arcade_screen_lists_use *u = &arcade_screen_lists_uses[i];
        if (u->chain >= arcade_screen_lists_chain_count || !u->file || !strstr(u->file, ".PAC")) { printf("use %u: chain or file\n", i); bad++; }
    }
    for (i = 0; i < arcade_screen_lists_window_count; i++) {
        const struct arcade_screen_lists_window *w = &arcade_screen_lists_windows[i];
        unsigned e;
        if (!w->w || !w->h || w->c0 > 15 || w->r0 > 15 || (unsigned)w->first_block + w->block_count > arcade_screen_lists_win_block_count) { printf("window %u\n", i); bad++; }
        for (e = 0; e < w->block_count && (unsigned)w->first_block + e < arcade_screen_lists_win_block_count; e++)
            if (arcade_screen_lists_win_blocks[w->first_block + e].block >= 222u) { printf("window %u: block\n", i); bad++; }
    }
    for (i = 0; i < arcade_screen_lists_backdrop_count; i++) {
        const struct arcade_screen_lists_backdrop *b = &arcade_screen_lists_backdrops[i];
        if (!b->file || !b->columns || !b->rows || !b->band || b->run_count == 0 || b->run_count > 4) { printf("backdrop %u\n", i); bad++; }
    }
    for (i = 0; i < arcade_screen_lists_map_count; i++)
        if ((unsigned)arcade_screen_lists_maps[i].first + arcade_screen_lists_maps[i].count > 0xffffu) { printf("map %u\n", i); bad++; }
    printf("%u chains, %u uses, %u problems\n", arcade_screen_lists_chain_count, arcade_screen_lists_use_count, bad);
    return bad != 0;
}

int main(int argc, char **argv)
{
    if (argc == 6 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4], argv[5]);
    if (argc == 6 && !strcmp(argv[1], "backdrop")) return backdrop(argv[2], argv[3], argv[4], argv[5]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    fprintf(stderr, "usage: screen_lists_test build|backdrop PROGRAM TILES SPEC OUTDIR | table\n");
    return 2;
}
