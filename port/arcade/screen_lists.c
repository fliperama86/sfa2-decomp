/* The screens' cell lists and position runs built from the arcade's art records; see screen_lists.h. The specification is screen_lists.py
 * (model_chain and what it calls). Project-authored, plain C99. No byte of either game is here.
 *
 * Collision note: a tile class is the 64-bit FNV-1a hash of the lowest of a tile's four flipped versions. screen_lists.py checks, for the
 * examined graphics (their digest is pinned by rom.c), that no two tiles share a hash without sharing pixels; the segment's tiles are
 * tiles of those graphics, so lookups here are exact for that set. */
#include "screen_lists.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compose.h"
#include "screens.h"

#define MAX_CELLS 512u
#define TILE_BYTES 128u
#define SHADOW_X (-32)         /* mode 4: the cells from here on right and down are the PS1's shadow cells, not in its lists */
#define SHADOW_Y 64

struct cell {
    uint32_t code;
    int32_t x, y;
    uint16_t flags, part, idx;
};

struct slot {
    uint64_t key;
    uint32_t number;   /* 0: empty */
    uint8_t e;         /* the stored flip relative to the class */
};

struct posent {
    uint32_t off, len, hash;
};

struct builder {
    const uint8_t *prog, *tiles;
    size_t psize, tsize;
    unsigned rules;
    const struct arcade_screen_lists_tile *pool;
    unsigned pool_count;
    struct slot *map;
    size_t mask;
    uint64_t zero_key;
    uint8_t *wtiles;       /* the stored tiles of the pool's windows, in the order of their numbers; wnum[k] is 1 + the index of pool tile k's */
    unsigned *wnum;
    uint8_t *lists, *lists_known, *pos, *pos_known;
    uint32_t lists_size, lists_cap, pos_size, pos_cap;
    struct posent *ents;
    size_t nents, ents_cap;
    char *err;
    size_t errsize;
};

static int fail(struct builder *b, const char *fmt, ...)
{
    if (b->err && b->errsize && !b->err[0]) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(b->err, b->errsize, fmt, ap);
        va_end(ap);
    }
    return -1;
}

/* ---- tile classes */

/* Key and d of 256 pixels: the hash of the lowest of the pixels, flipped across (bit 0) and down (bit 1), and the lowest flip giving it. */
static void pixel_class(const uint8_t px[256], uint64_t *key, unsigned *d)
{
    uint8_t v[4][256];
    unsigned f, x, y, best = 0, i;
    uint64_t h = 14695981039346656037ull;
    for (f = 0; f < 4; f++)
        for (y = 0; y < 16; y++)
            for (x = 0; x < 16; x++)
                v[f][y * 16u + x] = px[((f & 2u) ? 15u - y : y) * 16u + ((f & 1u) ? 15u - x : x)];
    for (f = 1; f < 4; f++)
        if (memcmp(v[f], v[best], 256) < 0)
            best = f;
    for (i = 0; i < 256; i++)
        h = (h ^ v[best][i]) * 1099511628211ull;
    *key = h;
    *d = best;
}

/* The classes worked out so far, from arcade_screen_lists_begin to arcade_screen_lists_end (the caller keeps the tiles unchanged meanwhile):
 * known[code] is 1 + d once key[code] is set. */
static struct {
    const uint8_t *tiles;
    size_t tsize;
    uint64_t *key;
    uint8_t *known;
} classes;

int arcade_screen_lists_begin(const uint8_t *tiles, size_t tiles_size)
{
    size_t n = tiles_size / TILE_BYTES;
    arcade_screen_lists_end();
    classes.key = malloc(n * sizeof *classes.key);
    classes.known = calloc(n, 1);
    if (!classes.key || !classes.known) {
        arcade_screen_lists_end();
        return -1;
    }
    classes.tiles = tiles;
    classes.tsize = tiles_size;
    return 0;
}

void arcade_screen_lists_end(void)
{
    free(classes.key);
    free(classes.known);
    memset(&classes, 0, sizeof classes);
}

static int tile_class(const struct builder *b, uint32_t code, uint64_t *key, unsigned *d)
{
    uint8_t px[256];
    uint64_t at = (uint64_t)code * TILE_BYTES;
    int kept = classes.tiles && classes.tiles == b->tiles && classes.tsize == b->tsize;
    if (at + TILE_BYTES > b->tsize)
        return -1;
    if (kept && classes.known[code]) {
        *key = classes.key[code];
        *d = classes.known[code] - 1u;
        return 0;
    }
    arcade_compose_tile_pixels(b->tiles + at, px);
    pixel_class(px, key, d);
    if (kept) {
        classes.key[code] = *key;
        classes.known[code] = (uint8_t)(*d + 1u);
    }
    return 0;
}

static struct slot *slot_of(struct builder *b, uint64_t key)
{
    size_t i = (size_t)((key >> 11) ^ key) & b->mask;
    while (b->map[i].number && b->map[i].key != key)
        i = (i + 1) & b->mask;
    return &b->map[i];
}

/* The pool: the lowest number (from 1) of each class, with the tile's stored flip relative to the class. */
static int pool_make(struct builder *b, const struct arcade_screen_lists_tile *pool, unsigned count)
{
    size_t cap = 16;
    unsigned k;
    uint8_t zero[256];
    unsigned zd;
    while (cap < 2u * (size_t)count + 2u)
        cap *= 2;
    b->map = calloc(cap, sizeof *b->map);
    if (!b->map)
        return fail(b, "out of memory");
    b->mask = cap - 1;
    memset(zero, 0, sizeof zero);
    pixel_class(zero, &b->zero_key, &zd);
    {   /* the windows' stored tiles, all at once (the frames are drawn once each for windows that follow one another) */
        uint32_t *w = NULL;
        unsigned nw = 0, *at = NULL;
        for (k = 1; k < count; k++)
            if (pool[k].kind == 3) nw++;
        if (nw) {
            w = malloc(nw * sizeof *w);
            at = malloc(nw * sizeof *at);
            b->wtiles = malloc((size_t)nw * TILE_BYTES);
            b->wnum = calloc(count, sizeof *b->wnum);
            if (!w || !at || !b->wtiles || !b->wnum) {
                free(w);
                free(at);
                return fail(b, "out of memory");
            }
            nw = 0;
            for (k = 1; k < count; k++)
                if (pool[k].kind == 3) {
                    w[nw] = pool[k].code;
                    at[nw] = k;
                    nw++;
                }
            if (arcade_screens_window_tiles(b->prog, b->psize, b->tiles, b->tsize, w, nw, (uint8_t (*)[TILE_BYTES])b->wtiles, b->err, b->errsize)) {
                free(w);
                free(at);
                return -1;
            }
            for (k = 0; k < nw; k++) b->wnum[at[k]] = k + 1u;
        }
        free(w);
        free(at);
    }
    for (k = 1; k < count; k++) {
        uint64_t key;
        unsigned d, e;
        struct slot *s;
        if (pool[k].kind == 2)
            continue;
        if (pool[k].kind == 1) {
            key = b->zero_key;
            e = 0;
        } else if (pool[k].kind == 3) {     /* a window: the class of its stored pixels, which are the tile as stored */
            uint8_t px[256];
            unsigned i;
            const uint8_t *t = b->wtiles + (size_t)(b->wnum[k] - 1u) * TILE_BYTES;
            for (i = 0; i < TILE_BYTES; i++) {
                px[2 * i] = t[i] & 15u;
                px[2 * i + 1] = (uint8_t)(t[i] >> 4);
            }
            pixel_class(px, &key, &d);
            e = d;
        } else {
            if (tile_class(b, pool[k].code, &key, &d))
                return fail(b, "segment tile %u: arcade tile %#x lies outside the graphics", k, (unsigned)pool[k].code);
            e = (pool[k].flip & 3u) ^ d;
        }
        s = slot_of(b, key);
        if (!s->number) {
            s->key = key;
            s->number = k;
            s->e = (uint8_t)e;
        }
    }
    return 0;
}

/* Number and flip bits of a cell of arcade tile `code` drawn with flip bits u; -1 when the segment lacks the class (or the tile). */
static int lookup(struct builder *b, uint32_t code, unsigned u, uint32_t *number, unsigned *g)
{
    uint64_t key;
    unsigned d;
    const struct slot *s;
    if (tile_class(b, code, &key, &d))
        return -1;
    s = slot_of(b, key);
    if (!s->number)
        return -1;
    *number = s->number;
    *g = (u ^ d ^ s->e) & 3u;
    return 0;
}

/* Number and flip bits of a window of 256 pixels; -1 when the segment lacks its class. */
static int lookup_pixels(struct builder *b, const uint8_t px[256], uint32_t *number, unsigned *g)
{
    uint64_t key;
    unsigned d;
    const struct slot *s;
    pixel_class(px, &key, &d);
    s = slot_of(b, key);
    if (!s->number) return -1;
    *number = s->number;
    *g = (d ^ s->e) & 3u;
    return 0;
}

/* ---- records */

static int cmp_cell(const void *pa, const void *pb)
{
    const struct cell *a = pa, *c = pb;
    if (a->part != c->part) return a->part > c->part ? -1 : 1;   /* the parts last first */
    if (a->y != c->y) return a->y < c->y ? -1 : 1;
    if (a->x != c->x) return a->x < c->x ? -1 : 1;
    return a->idx < c->idx ? -1 : a->idx > c->idx;               /* equal positions keep the record's order */
}

/* True when arcade tile `code` has no opaque pixel (the class of the blank tile). */
static int is_blank(struct builder *b, uint32_t code)
{
    uint64_t key;
    unsigned d;
    return tile_class(b, code, &key, &d) == 0 && key == b->zero_key;
}

/* The art record at a as cells in the PS1's order, tile codes plus `bank`. The record is parsed by compose.c (the one parser: mode 2 (blocks of code and attribute, coordinates through the index),
 * mode 0 (a shared attribute, a code a cell), mode 4 (the portraits: blocks of code, attribute, x, y; tile code plus SCROLL_ADD) and mode 10 (a grid of
 * code and attribute pairs, column by column, the lower left corner at 0, 0; tile code plus SCROLL_ADD). The grids drop their blank cells, the portraits
 * their blank cells and the shadow cells (dropped here, after the parse); the cells are then sorted into the PS1's order. */
static int read_record(struct builder *b, uint32_t a, uint32_t bank, struct cell *cells, unsigned *count, uint32_t *end)
{
    struct arcade_compose_cell all[MAX_CELLS];
    struct arcade_compose_info info;
    char line[160];
    unsigned i, n = 0;

    if (arcade_compose_record(b->prog, b->psize, a, bank, b->rules & ARCADE_SCREEN_LISTS_PAL_SPLIT, all, MAX_CELLS, &info, line, sizeof line))
        return fail(b, "%s", line);
    for (i = 0; i < info.count; i++) {
        const struct arcade_compose_cell *c = &all[i];
        if (info.mode == 10 || info.mode == 4) {
            int shadow = info.mode == 4 && c->x >= SHADOW_X && c->y >= SHADOW_Y;
            if (shadow || is_blank(b, c->code)) continue;
        }
        cells[n].code = c->code;
        cells[n].x = c->x;
        cells[n].y = c->y;
        cells[n].flags = c->flags;
        cells[n].part = c->part;
        cells[n].idx = (uint16_t)n;
        n++;
    }
    qsort(cells, n, sizeof *cells, cmp_cell);
    *count = n;
    *end = info.end;
    return 0;
}

/* The cells (tile, x, y, flags) of window `w` in raster order, blank tiles left out. */
static int read_window(struct builder *b, const struct arcade_screen_lists_spec *s, const struct arcade_screen_lists_window *w,
                       struct cell *cells, unsigned *count)
{
    unsigned i, j, n = 0, e;
    if ((size_t)w->first_block + w->block_count > s->win_block_count) return fail(b, "window: its blocks are outside the table");
    if ((unsigned)w->w * w->h > MAX_CELLS * 4u) return fail(b, "window: %u x %u cells", w->w, w->h);
    for (j = 0; j < w->h; j++)
        for (i = 0; i < w->w; i++) {
            unsigned mc = w->c0 + i, mr = w->r0 + j, blk = ARCADE_COMPOSE_SCROLL_BLOCKS;
            uint32_t tile;
            uint16_t attr;
            for (e = 0; e < w->block_count; e++) {
                const struct arcade_screen_lists_win_block *wb = &s->win_blocks[w->first_block + e];
                if (wb->rx == mc / 16 && wb->ry == mr / 16) {
                    blk = wb->block;
                    break;
                }
            }
            if (blk >= ARCADE_COMPOSE_SCROLL_BLOCKS) return fail(b, "window: no block for the cell (%u, %u)", mc, mr);
            if (arcade_compose_scroll2_cell(b->prog, b->psize, blk, mc % 16, mr % 16, &tile, &attr)) return fail(b, "window: read outside the program");
            if (is_blank(b, tile)) continue;
            if (n == MAX_CELLS) return fail(b, "window: more than %u cells", MAX_CELLS);
            cells[n].code = tile;
            cells[n].x = w->x0 + 16 * (int32_t)i;
            cells[n].y = w->y0 + 16 * (int32_t)j;
            cells[n].flags = (uint16_t)(attr & 0x7fu);
            cells[n].part = 0;
            cells[n].idx = (uint16_t)n;
            n++;
        }
    *count = n;
    return 0;
}

/* ---- output */

static void put16(uint8_t *p, unsigned v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

static int lists_put(struct builder *b, const uint8_t *p, size_t n, int known)
{
    if (n > b->lists_cap - b->lists_size) return fail(b, "the lists need more than their %u bytes", (unsigned)b->lists_cap);
    if (p) memcpy(b->lists + b->lists_size, p, n);
    if (known) memset(b->lists_known + b->lists_size, 1, n);
    b->lists_size += (uint32_t)n;
    return 0;
}

static int pos_reserve(struct builder *b, uint32_t n, int known, uint32_t *off)
{
    if (n > b->pos_cap - b->pos_size) return fail(b, "the position runs need more than their %u bytes", (unsigned)b->pos_cap);
    if (known) memset(b->pos_known + b->pos_size, 1, n);
    *off = b->pos_size;
    b->pos_size += n;
    return 0;
}

/* Offset of the stream in the runs: an identical stream built before, else appended. Own lists' streams are not looked up. */
static int pos_add(struct builder *b, const uint8_t *s, uint32_t len, uint32_t *off)
{
    uint32_t h = 2166136261u, i;
    size_t e;
    for (i = 0; i < len; i++) h = (h ^ s[i]) * 16777619u;
    for (e = 0; e < b->nents; e++)
        if (b->ents[e].hash == h && b->ents[e].len == len && memcmp(b->pos + b->ents[e].off, s, len) == 0) {
            *off = b->ents[e].off;
            return 0;
        }
    if (pos_reserve(b, len, 1, off)) return -1;
    memcpy(b->pos + *off, s, len);
    if (b->nents == b->ents_cap) {
        size_t cap = b->ents_cap ? b->ents_cap * 2 : 64;
        struct posent *q = realloc(b->ents, cap * sizeof *q);
        if (!q) return fail(b, "out of memory");
        b->ents = q;
        b->ents_cap = cap;
    }
    b->ents[b->nents].off = *off;
    b->ents[b->nents].len = len;
    b->ents[b->nents].hash = h;
    b->nents++;
    return 0;
}

/* The list for numbered cells (number, flags) and its position offset: plain (no flip bit, one flags value): mode 0 (a word a cell) or 4 (first
 * number, run - 1), the shorter wins, a tie goes to 4, the third header word is the palette; otherwise mode 2 (number, flags) or 6 (first number,
 * flags, run - 1), the shorter wins, a tie goes to 6, the third word is cells - 1. */
static int encode_list(struct builder *b, const uint16_t *t, const uint16_t *f, unsigned n, uint32_t pos)
{
    uint8_t buf[10 + 6 * MAX_CELLS];
    uint16_t rt[MAX_CELLS], rf[MAX_CELLS], rr[MAX_CELLS];
    unsigned i, nr = 0, plain = 1, mode, len;

    if (pos > 0xffffu) return fail(b, "a position offset of %u does not fit 16 bits", (unsigned)pos);
    for (i = 0; i < n; i++)
        if ((f[i] & 0x60u) || f[i] != f[0]) plain = 0;
    for (i = 0; i < n; i++) {
        if (nr && (uint32_t)rt[nr - 1] + rr[nr - 1] + 1u == t[i] && (plain || rf[nr - 1] == f[i])) {
            rr[nr - 1]++;
        } else {
            rt[nr] = t[i];
            rf[nr] = f[i];
            rr[nr] = 0;
            nr++;
        }
    }
    if (plain) {
        mode = n < 2 * nr ? 0 : 4;
        put16(buf + 4, f[0] & 0x1fu);
    } else {
        mode = 2 * n < 3 * nr ? 2 : 6;
        put16(buf + 4, n - 1);
    }
    put16(buf, n);
    put16(buf + 2, mode);
    put16(buf + 6, 0);
    put16(buf + 8, pos);
    len = 10;
    if (mode == 0) {
        for (i = 0; i < n; i++, len += 2) put16(buf + len, t[i]);
    } else if (mode == 4) {
        for (i = 0; i < nr; i++, len += 4) {
            put16(buf + len, rt[i]);
            put16(buf + len + 2, rr[i]);
        }
    } else if (mode == 2) {
        for (i = 0; i < n; i++, len += 4) {
            put16(buf + len, t[i]);
            put16(buf + len + 2, f[i]);
        }
    } else {
        for (i = 0; i < nr; i++, len += 6) {
            put16(buf + len, rt[i]);
            put16(buf + len + 2, rf[i]);
            put16(buf + len + 4, rr[i]);
        }
    }
    return lists_put(b, buf, len, 1);
}

/* The position runs of the cells (in order): horizontally adjacent cells form a run of x, y, length - 1, stored as the step from the previous
 * run's last tile and y. */
static int stream_of(struct builder *b, const struct cell *cells, unsigned n, uint8_t *stream, unsigned *len_out)
{
    unsigned i, len = 0, rn = 0, have = 0;
    int32_t rx = 0, ry = 0, lx = 0, ly = 0;
    for (i = 0; i <= n; i++) {
        if (i < n && have && cells[i].y == ry && cells[i].x == rx + 16 * (int32_t)(rn + 1)) {
            rn++;
            continue;
        }
        if (have) {
            int32_t dx = rx - lx, dy = ry - ly;
            if (dx < -32768 || dx > 32767 || dy < -32768 || dy > 32767) return fail(b, "a position step of (%d, %d) does not fit 16 bits", (int)dx, (int)dy);
            put16(stream + len, (unsigned)dx & 0xffffu);
            put16(stream + len + 2, (unsigned)dy & 0xffffu);
            put16(stream + len + 4, rn);
            len += 6;
            lx = rx + 16 * (int32_t)rn;
            ly = ry;
        }
        if (i < n) {
            rx = cells[i].x;
            ry = cells[i].y;
            rn = 0;
            have = 1;
        }
    }
    *len_out = len;
    return 0;
}

/* The next of the segment's own tiles (kind 2) after number n, or 0. */
static uint32_t own_successor(const struct builder *b, uint32_t n)
{
    uint32_t k;
    if (n == 0 || n >= b->pool_count || (b->pool[n].kind != 2 && b->pool[n].kind != 3)) return 0;
    for (k = n + 1; k < b->pool_count; k++)
        if (b->pool[k].kind == 2 || b->pool[k].kind == 3) return k;
    return 0;
}

/* One list from `n` cells (in the PS1's order): the exceptions of the list, the palettes through `pairs`, the numbers by looking the tiles up
 * in the segment (the cells that show tiles of the PS1's own take their numbers from the exception), the position runs and the list. */
static int list_from_cells(struct builder *b, const struct arcade_screen_lists_spec *s, struct cell *cells, unsigned n,
                           const struct arcade_screen_lists_pal *pairs, unsigned npairs, unsigned index, unsigned *ex)
{
    struct cell kept[MAX_CELLS];
    uint16_t t[MAX_CELLS], f[MAX_CELLS];
    uint8_t stream[6 * MAX_CELLS];
    unsigned k, m = 0, len = 0;
    uint32_t off = 0, prev = 0;

    for (k = 0; k < n; k++) {
        uint32_t number, v = 0;
        unsigned g, pal = cells[k].flags & 0x1fu, p, have = 0;
        while (*ex < s->exception_count && (s->exceptions[*ex].list < index || (s->exceptions[*ex].list == index && s->exceptions[*ex].cell < k))) (*ex)++;
        if (*ex < s->exception_count && s->exceptions[*ex].list == index && s->exceptions[*ex].cell == k) {
            v = s->exceptions[*ex].palette;
            have = 1;
        }
        if (have && v == ARCADE_SCREEN_LISTS_DROP) continue;
        if (have && (v & ARCADE_SCREEN_LISTS_OWN_TILE)) {
            number = v >> 16;
            if (!number) number = own_successor(b, prev);
            if (!number) return fail(b, "list %u: no number follows the previous tile of the PS1's own", index);
            if (number >= b->pool_count) return fail(b, "list %u: tile number %u is outside the segment", index, (unsigned)number);
            g = (v >> 10) & 3u;
            prev = number;
        } else if (lookup(b, cells[k].code, (cells[k].flags >> 5) & 3u, &number, &g)) {
            return fail(b, "list %u: tile %#x is not in the segment", index, (unsigned)cells[k].code);
        }
        if (have) {
            pal = v & 0x1fu;
        } else {
            for (p = 0; p < npairs; p++)
                if (pairs[p].from == pal) {
                    pal = pairs[p].to;
                    break;
                }
        }
        kept[m] = cells[k];
        t[m] = (uint16_t)number;
        f[m] = (uint16_t)(pal | g << 5);
        m++;
    }
    if (!m) return fail(b, "list %u: no cells", index);
    if (stream_of(b, kept, m, stream, &len) || pos_add(b, stream, len, &off)) return -1;
    return encode_list(b, t, f, m, off);
}

static int record_list(struct builder *b, const struct arcade_screen_lists_spec *s, uint32_t a, uint32_t bank, const struct arcade_screen_lists_pal *pairs,
                       unsigned npairs, unsigned index, unsigned *ex, uint32_t *end)
{
    struct cell cells[MAX_CELLS];
    unsigned n = 0;
    if (read_record(b, a, bank, cells, &n, end)) return -1;
    return list_from_cells(b, s, cells, n, pairs, npairs, index, ex);
}

static int window_list(struct builder *b, const struct arcade_screen_lists_spec *s, unsigned window, const struct arcade_screen_lists_pal *pairs,
                       unsigned npairs, unsigned index, unsigned *ex)
{
    struct cell cells[MAX_CELLS];
    unsigned n = 0;
    if (window >= s->window_count) return fail(b, "window %u is not in the table", window);
    if (read_window(b, s, &s->windows[window], cells, &n)) return -1;
    return list_from_cells(b, s, cells, n, pairs, npairs, index, ex);
}

/* One list cut from an arcade frame: see ARCADE_SCREEN_LISTS_GRID. */
static int grid_list(struct builder *b, const struct arcade_screen_lists_spec *s, unsigned grid, unsigned index)
{
    const struct arcade_screen_lists_grid *g;
    struct arcade_canvas cv;
    struct cell cells[MAX_CELLS];
    uint16_t t[MAX_CELLS], f[MAX_CELLS];
    uint8_t stream[6 * MAX_CELLS], tile[TILE_BYTES], px[256];
    char e[160];
    unsigned n = 0, len = 0, i;
    int32_t x0, y0, gx, gy;
    uint32_t off = 0;
    if (grid >= s->grid_count) return fail(b, "grid %u is not in the table", grid);
    g = &s->grids[grid];
    e[0] = 0;
    if (arcade_compose_record_canvas(b->prog, b->psize, b->tiles, b->tsize, g->address, g->bank, &cv, e, sizeof e)) return fail(b, "list %u: %s", index, e);
    if (!cv.px) return fail(b, "list %u: the frame at %#x is empty", index, (unsigned)g->address);
    x0 = cv.x0 + g->ox;
    y0 = cv.y0 + g->oy;
    for (gy = y0 - (((y0 - g->qy) % 16) + 16) % 16; gy < y0 + (int32_t)cv.h; gy += 16)
        for (gx = x0 - (((x0 - g->qx) % 16) + 16) % 16; gx < x0 + (int32_t)cv.w; gx += 16) {
            uint32_t number;
            unsigned flip, any = 0;
            arcade_compose_cut(&cv, gx - g->ox, gy - g->oy, 0, tile);
            for (i = 0; i < TILE_BYTES; i++) any |= tile[i];
            if (!any) continue;
            if (n >= MAX_CELLS) {
                arcade_compose_free(&cv);
                return fail(b, "list %u: more than %u cells", index, MAX_CELLS);
            }
            for (i = 0; i < TILE_BYTES; i++) {
                px[2 * i] = tile[i] & 15u;
                px[2 * i + 1] = (uint8_t)(tile[i] >> 4);
            }
            if (lookup_pixels(b, px, &number, &flip)) {
                arcade_compose_free(&cv);
                return fail(b, "list %u: a window of the frame at %#x is not in the segment", index, (unsigned)g->address);
            }
            cells[n].x = gx;
            cells[n].y = gy;
            t[n] = (uint16_t)number;
            f[n] = (uint16_t)(g->palette | flip << 5);
            n++;
        }
    arcade_compose_free(&cv);
    if (!n) return fail(b, "list %u: no cells", index);
    if (stream_of(b, cells, n, stream, &len) || pos_add(b, stream, len, &off)) return -1;
    return encode_list(b, t, f, n, off);
}

void arcade_screen_lists_free(struct arcade_screen_lists_out *out)
{
    free(out->positions);
    free(out->positions_known);
    free(out->lists);
    free(out->lists_known);
    memset(out, 0, sizeof *out);
}

int arcade_screen_lists_build_with(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                                   const struct arcade_screen_lists_tile *pool, unsigned pool_count, const struct arcade_screen_lists_spec *spec,
                                   struct arcade_screen_lists_out *out, char *err, size_t errsize)
{
    struct builder b;
    unsigned it, ex = 0, index = 0, c;
    int rc = -1;

    memset(out, 0, sizeof *out);
    memset(&b, 0, sizeof b);
    b.prog = program;
    b.psize = program_size;
    b.tiles = tiles;
    b.tsize = tiles_size;
    b.rules = spec->rules;
    b.pool = pool;
    b.pool_count = pool_count;
    b.err = err;
    b.errsize = errsize;
    if (err && errsize) err[0] = 0;
    b.lists_cap = spec->lists_size;
    b.pos_cap = spec->positions_size;
    b.lists = calloc((size_t)b.lists_cap + 1, 1);
    b.lists_known = calloc((size_t)b.lists_cap + 1, 1);
    b.pos = calloc((size_t)b.pos_cap + 1, 1);
    b.pos_known = calloc((size_t)b.pos_cap + 1, 1);
    if (!b.lists || !b.lists_known || !b.pos || !b.pos_known) {
        fail(&b, "out of memory");
        goto done;
    }
    if (pool_make(&b, pool, pool_count)) goto done;
    for (it = 0; it < spec->item_count; it++) {
        const struct arcade_screen_lists_item *m = &spec->items[it];
        if (m->kind == ARCADE_SCREEN_LISTS_OWN) {
            uint32_t off = 0;
            if (lists_put(&b, NULL, m->list_bytes, 0) || pos_reserve(&b, m->stream_bytes, 0, &off)) goto done;
            index++;
        } else if (m->kind == ARCADE_SCREEN_LISTS_EMPTY) {
            for (c = 0; c < m->count; c++, index++) {
                uint8_t e[12] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                if (b.pos_size > 0xffffu) {
                    fail(&b, "a position offset of %u does not fit 16 bits", (unsigned)b.pos_size);
                    goto done;
                }
                put16(e + 8, b.pos_size);
                if (lists_put(&b, e, sizeof e, 1)) goto done;
            }
        } else if (m->kind == ARCADE_SCREEN_LISTS_GRID) {
            if (grid_list(&b, spec, m->address, index)) goto done;
            index++;
        } else if (m->kind == ARCADE_SCREEN_LISTS_REC || m->kind == ARCADE_SCREEN_LISTS_WIN) {
            const struct arcade_screen_lists_pal *pairs = NULL;
            unsigned np = 0;
            uint32_t a = m->address;
            if (m->map) {
                if (m->map > spec->map_count) {
                    fail(&b, "item %u: palette map %u is not in the table", it, m->map);
                    goto done;
                }
                pairs = spec->pals + spec->maps[m->map - 1].first;
                np = spec->maps[m->map - 1].count;
            }
            if (m->kind == ARCADE_SCREEN_LISTS_WIN) {
                if (window_list(&b, spec, a, pairs, np, index, &ex)) goto done;
                index++;
            } else {
                for (c = 0; c < m->count; c++, index++)
                    if (record_list(&b, spec, a, m->bank, pairs, np, index, &ex, &a)) goto done;
            }
        } else {
            fail(&b, "item %u: kind %u", it, m->kind);
            goto done;
        }
    }
    if (b.lists_size != spec->lists_size || b.pos_size != spec->positions_size) {
        fail(&b, "the chain builds %u list bytes and %u position bytes, the table says %u and %u", (unsigned)b.lists_size, (unsigned)b.pos_size,
             (unsigned)spec->lists_size, (unsigned)spec->positions_size);
        goto done;
    }
    out->lists = b.lists;
    out->lists_known = b.lists_known;
    out->lists_size = b.lists_size;
    out->positions = b.pos;
    out->positions_known = b.pos_known;
    out->positions_size = b.pos_size;
    b.lists = b.lists_known = b.pos = b.pos_known = NULL;
    rc = 0;
done:
    free(b.lists);
    free(b.lists_known);
    free(b.pos);
    free(b.pos_known);
    free(b.map);
    free(b.ents);
    free(b.wtiles);
    free(b.wnum);
    return rc;
}

struct arcade_screen_lists_tile *arcade_screen_lists_pool(const char *file, unsigned table, unsigned slot, unsigned ordinal, unsigned segment,
                                                          unsigned *count, char *err, size_t errsize)
{
    const struct arcade_screens_file *f = arcade_screens_find(file);
    const struct arcade_screens_recipe *r;
    struct arcade_screen_lists_tile *pool;
    uint64_t total = 0;
    unsigned i, k = 0, j, n = 0;
    const struct arcade_screens_region *g = NULL;

    if (err && errsize) err[0] = 0;
    if (!f || !(r = arcade_screens_recipe_of(f, table, slot, ordinal))) {
        if (err && errsize) snprintf(err, errsize, "%s: the screens table has no recipe for chunk %u/%#x/%u", file, table, slot, ordinal);
        return NULL;
    }
    for (i = 0; i < r->count; i++) {
        const struct arcade_screens_region *h = &arcade_screens_regions[r->first + i];
        if (h->kind != ARCADE_SCREENS_SEG) continue;
        if (k++ == segment) {
            g = h;
            break;
        }
    }
    if (!g) {
        if (err && errsize) snprintf(err, errsize, "%s: chunk %u/%#x/%u has no tile segment %u", file, table, slot, ordinal, segment);
        return NULL;
    }
    for (i = 0; i < g->count; i++) total += arcade_screens_items[g->first + i].count;
    if (total == 0 || total > 0x100000u) {
        if (err && errsize) snprintf(err, errsize, "%s: a segment of %u tiles", file, (unsigned)total);
        return NULL;
    }
    pool = calloc((size_t)total, sizeof *pool);
    if (!pool) {
        if (err && errsize) snprintf(err, errsize, "out of memory");
        return NULL;
    }
    for (i = 0; i < g->count; i++) {
        const struct arcade_screens_item *it = &arcade_screens_items[g->first + i];
        for (j = 0; j < it->count; j++, n++) {
            if (it->first == ARCADE_SCREENS_ZERO && !(it->flip & ARCADE_SCREENS_BY_DESIGN)) {
                pool[n].kind = 1;
            } else if (it->first == ARCADE_SCREENS_WINDOW && !(it->flip & ARCADE_SCREENS_BY_DESIGN)) {
                pool[n].kind = 3;   /* a window is a tile of the PS1's own, found by its pixels */
                pool[n].code = it->flip;
            } else if (it->first == ARCADE_SCREENS_ZERO || it->first == ARCADE_SCREENS_DISC || it->first == ARCADE_SCREENS_PIECES || it->first == ARCADE_SCREENS_WINDOW ||
                       (it->flip & ARCADE_SCREENS_BY_DESIGN)) {
                pool[n].kind = 2;   /* a by-design tile is the PS1's own tile for the lists' numbering */
            } else {
                pool[n].code = it->first + j;
                pool[n].flip = (uint8_t)(it->flip & 3u);
            }
        }
    }
    *count = n;
    return pool;
}

int arcade_screen_lists_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                              const struct arcade_screen_lists_use *use, struct arcade_screen_lists_out *out, char *err, size_t errsize)
{
    const struct arcade_screen_lists_chain *c;
    struct arcade_screen_lists_spec spec;
    struct arcade_screen_lists_tile *pool;
    unsigned count = 0;
    int rc;

    memset(out, 0, sizeof *out);
    if (use->chain >= arcade_screen_lists_chain_count) {
        if (err && errsize) snprintf(err, errsize, "%s: chain %u is not in the table", use->file, use->chain);
        return -1;
    }
    c = &arcade_screen_lists_chains[use->chain];
    pool = arcade_screen_lists_pool(use->file, use->pool_table, use->pool_slot, use->pool_ordinal, use->pool_segment, &count, err, errsize);
    if (!pool) return -1;
    memset(&spec, 0, sizeof spec);
    spec.rules = c->rules;
    spec.positions_size = c->positions_size;
    spec.lists_size = c->lists_size;
    spec.items = arcade_screen_lists_items + c->first_item;
    spec.item_count = c->item_count;
    spec.exceptions = arcade_screen_lists_exceptions + c->first_exception;
    spec.exception_count = c->exception_count;
    spec.windows = arcade_screen_lists_windows;
    spec.window_count = arcade_screen_lists_window_count;
    spec.grids = arcade_screen_lists_grids;
    spec.grid_count = arcade_screen_lists_grid_count;
    spec.win_blocks = arcade_screen_lists_win_blocks;
    spec.win_block_count = arcade_screen_lists_win_block_count;
    spec.pals = arcade_screen_lists_pals;
    spec.maps = arcade_screen_lists_maps;
    spec.map_count = arcade_screen_lists_map_count;
    rc = arcade_screen_lists_build_with(program, program_size, tiles, tiles_size, pool, count, &spec, out, err, errsize);
    free(pool);
    return rc;
}

/* ---- the backdrop cell map */

#define SETS1 0x2ad300u        /* arcade: scroll 1 block sets, 128 bytes each: 8 x 8 ids of 4 x 4 cell groups */
#define GROUPS1 0x2b1a80u      /* the groups, 64 bytes each: 4 x 4 cells of code and attribute, column by column */
#define PICTURE_ROW 384u       /* bytes a row of the 768 pixel wide picture */
#define UNIT_BYTES 32u

/* The 32 bytes of the 8 x 8 piece of scroll 1 cell (code, attr): the left half of the top (even) or bottom (odd) eight rows of graphics tile
 * 0x10000 + code / 2, colour plus one, flipped by attribute bits 5 (across) and 6 (down), the left pixel of a pair in the low nibble. */
static int piece_bytes(const uint8_t *tiles, size_t tsize, unsigned code, unsigned attr, uint8_t out[UNIT_BYTES])
{
    uint8_t px[256], rows[8][8];
    unsigned tile = 0x10000u + (code >> 1), x, y;
    if (((uint64_t)tile + 1) * TILE_BYTES > tsize) return -1;
    arcade_compose_tile_pixels(tiles + (size_t)tile * TILE_BYTES, px);
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++)
            rows[(attr & 0x40u) ? 7u - y : y][(attr & 0x20u) ? 7u - x : x] = px[(y + (code & 1u) * 8u) * 16u + x];
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x += 2) out[y * 4u + x / 2] = (uint8_t)(rows[y][x] | rows[y][x + 1] << 4);
    return 0;
}

struct unit_slot {
    uint32_t hash;
    uint16_t texture;
    uint8_t used;
    uint8_t bytes[UNIT_BYTES];
};

static uint32_t fnv32(const uint8_t *p, size_t n)
{
    uint32_t h = 2166136261u;
    size_t i;
    for (i = 0; i < n; i++) h = (h ^ p[i]) * 16777619u;
    return h;
}

int arcade_screen_lists_backdrop_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                                       const struct arcade_screen_lists_backdrop *bd, const uint8_t *picture, const uint8_t *picture_known,
                                       size_t picture_size, uint8_t *out, uint8_t *known, size_t out_size, char *err, size_t errsize)
{
    enum { SLOTS = 8192 };
    struct unit_slot *table;
    unsigned page, u, v, yy, r, c, k;
    uint8_t blank[UNIT_BYTES];
    int rc = -1;
    size_t cells = (size_t)bd->columns * bd->rows;

    if (err && errsize) err[0] = 0;
#define BD_FAIL(...) do { if (err && errsize) snprintf(err, errsize, __VA_ARGS__); goto done; } while (0)
    table = calloc(SLOTS, sizeof *table);
    if (!table) BD_FAIL("out of memory");
    if (out_size < cells * 4 || !bd->band || !bd->run_count || bd->run_count > 4) BD_FAIL("%s: the backdrop's description does not fit", bd->file);
    memset(out, 0, cells * 4);
    memset(known, 0, cells * 4);
    memset(blank, 0, sizeof blank);
    for (page = 0; page < 3; page++)
        for (u = 0; u < 32; u++)
            for (v = 0; v < 32; v++) {
                uint8_t unit[UNIT_BYTES];
                int whole = 1;
                uint32_t h;
                for (yy = 0; yy < 8 && whole; yy++) {
                    size_t at = (size_t)(v * 8 + yy) * PICTURE_ROW + page * 128u + u * 4u;
                    if (at + 4 > picture_size || !picture_known[at] || !picture_known[at + 1] || !picture_known[at + 2] || !picture_known[at + 3]) whole = 0;
                    else memcpy(unit + yy * 4, picture + at, 4);
                }
                if (!whole) continue;
                h = fnv32(unit, UNIT_BYTES);
                for (k = h & (SLOTS - 1); table[k].used; k = (k + 1) & (SLOTS - 1))
                    if (table[k].hash == h && memcmp(table[k].bytes, unit, UNIT_BYTES) == 0) break;
                if (!table[k].used) {
                    table[k].used = 1;
                    table[k].hash = h;
                    table[k].texture = (uint16_t)(((bd->first_page + page) << 10) | (v << 5) | u);
                    memcpy(table[k].bytes, unit, UNIT_BYTES);
                }
            }
    for (r = 0; r < bd->rows; r++) {
        unsigned band = r / bd->band;
        for (c = 0; c < bd->columns; c++) {
            unsigned run = 0, edge = 0, set, x, y, id, code, attr, texture, clut;
            uint8_t piece[UNIT_BYTES];
            uint32_t h;
            size_t at, g;
            while (run < bd->run_count && c >= edge + bd->runs[run].columns) edge += bd->runs[run++].columns;
            if (run == bd->run_count) BD_FAIL("%s: the runs do not cover %u columns", bd->file, bd->columns);
            set = bd->runs[run].set + band * bd->runs[run].step;
            x = bd->runs[run].x + c - edge;
            y = bd->y0 + r % bd->band;
            if (x > 31 || y > 31) BD_FAIL("%s: cell (%u, %u) is outside its set", bd->file, x, y);
            at = SETS1 + 128u * set + ((x / 4) * 8u + y / 4) * 2u;
            if (at + 2 > program_size) BD_FAIL("%s: set %u is outside the program", bd->file, set);
            id = (unsigned)program[at] << 8 | program[at + 1];
            g = GROUPS1 + 64u * id + 4u * ((x % 4) * 4u + y % 4);
            if (g + 4 > program_size) BD_FAIL("%s: group %u is outside the program", bd->file, id);
            code = (unsigned)program[g] << 8 | program[g + 1];
            attr = (unsigned)program[g + 2] << 8 | program[g + 3];
            if (piece_bytes(tiles, tiles_size, code, attr, piece)) continue;
            h = fnv32(piece, UNIT_BYTES);
            for (k = h & (SLOTS - 1); table[k].used; k = (k + 1) & (SLOTS - 1))
                if (table[k].hash == h && memcmp(table[k].bytes, piece, UNIT_BYTES) == 0) break;
            if (!table[k].used) continue;
            texture = table[k].texture;
            clut = memcmp(piece, blank, UNIT_BYTES) == 0 ? bd->clut : bd->clut + 64u * (attr & 31u);
            at = ((size_t)r * bd->columns + c) * 4;
            out[at] = (uint8_t)texture;
            out[at + 1] = (uint8_t)(texture >> 8);
            out[at + 2] = (uint8_t)clut;
            out[at + 3] = (uint8_t)(clut >> 8);
            memset(known + at, 1, 4);
        }
    }
    rc = 0;
done:
    free(table);
    return rc;
#undef BD_FAIL
}
