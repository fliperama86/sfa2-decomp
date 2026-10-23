/* Driver of test_arcade_screen_lists.py: synthetic programs only (and the generated table's sanity).
 *   screen_lists_test build PROGRAM TILES SPEC OUTDIR   build one chain
 *   screen_lists_test table                             sanity of the checked-in table
 * SPEC is text: "rules positions_size lists_size pool_count item_count exception_count pair_count map_count", then pool_count lines "kind code flip",
 * item_count lines "kind count address bank map list_bytes stream_bytes", exception_count lines "list cell palette", pair_count lines "from to",
 * map_count lines "first count". OUTDIR gets lists.bin, lists_known.bin, positions.bin and positions_known.bin. */
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
    struct arcade_screen_lists_out out;
    unsigned rules, ps, ls, np, ni, ne, nq, nm, i;
    char err[256];
    int rc;

    if (!prog || !tiles || !sf || fscanf(sf, "%u %u %u %u %u %u %u %u", &rules, &ps, &ls, &np, &ni, &ne, &nq, &nm) != 8) return 3;
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
    memset(&spec, 0, sizeof spec);
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
    free(pool); free(items); free(exc); free(pals); free(maps); free(prog); free(tiles); fclose(sf);
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

static int table(void)
{
    unsigned i, bad = 0;
    for (i = 0; i < arcade_screen_lists_chain_count; i++) {
        const struct arcade_screen_lists_chain *c = &arcade_screen_lists_chains[i];
        unsigned k;
        if (c->first_item + c->item_count > arcade_screen_lists_item_count) { printf("chain %u: items outside the table\n", i); bad++; }
        for (k = 0; k < c->item_count && c->first_item + k < arcade_screen_lists_item_count; k++) {
            const struct arcade_screen_lists_item *m = &arcade_screen_lists_items[c->first_item + k];
            if (m->kind > ARCADE_SCREEN_LISTS_OWN || m->map > arcade_screen_lists_map_count) { printf("chain %u item %u: kind or map\n", i, k); bad++; }
            if (m->kind == ARCADE_SCREEN_LISTS_REC && (!m->count || !m->address || (m->bank & 0xffffu))) { printf("chain %u item %u: record run\n", i, k); bad++; }
        }
    }
    for (i = 0; i < arcade_screen_lists_use_count; i++) {
        const struct arcade_screen_lists_use *u = &arcade_screen_lists_uses[i];
        if (u->chain >= arcade_screen_lists_chain_count || !u->file || !strstr(u->file, ".PAC")) { printf("use %u: chain or file\n", i); bad++; }
    }
    for (i = 0; i < arcade_screen_lists_map_count; i++)
        if ((unsigned)arcade_screen_lists_maps[i].first + arcade_screen_lists_maps[i].count > 0xffffu) { printf("map %u\n", i); bad++; }
    printf("%u chains, %u uses, %u problems\n", arcade_screen_lists_chain_count, arcade_screen_lists_use_count, bad);
    return bad != 0;
}

int main(int argc, char **argv)
{
    if (argc == 6 && !strcmp(argv[1], "build")) return build(argv[2], argv[3], argv[4], argv[5]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    fprintf(stderr, "usage: screen_lists_test build PROGRAM TILES SPEC OUTDIR | table\n");
    return 2;
}
