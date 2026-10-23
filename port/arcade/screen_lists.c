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

#include "screens.h"

#define COORD_TABLE 0x9f260u   /* arcade: table of pointers to coordinate blocks, indexed by the record's coordinate index */
#define MAX_CELLS 512u
#define TILE_BYTES 128u

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
    int bad;
    unsigned rules;
    struct slot *map;
    size_t mask;
    uint64_t zero_key;
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

static unsigned be16(const uint8_t *p)
{
    return (unsigned)p[0] << 8 | p[1];
}

static uint16_t rd16(struct builder *b, size_t o)
{
    if (o > b->psize || b->psize - o < 2) {
        b->bad = 1;
        return 0;
    }
    return (uint16_t)be16(b->prog + o);
}

static uint32_t rd32(struct builder *b, size_t o)
{
    return (uint32_t)rd16(b, o) << 16 | rd16(b, o + 2);
}

/* ---- tile classes */

/* The PS1 pixels (the arcade value plus one, modulo 16) of the arcade tile at t: four planes, 8 pixels to a byte, 4 bytes to 8 pixels. */
static void tile_pixels(const uint8_t *t, uint8_t px[256])
{
    unsigned x, y;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            const uint8_t *p = t + y * 8u + (x >> 3) * 4u;
            unsigned sh = 7u - (x & 7u), v = ((p[0] >> sh) & 1u) | (((p[1] >> sh) & 1u) << 1) | (((p[2] >> sh) & 1u) << 2) | (((p[3] >> sh) & 1u) << 3);
            px[y * 16u + x] = (uint8_t)((v + 1u) & 15u);
        }
}

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

static int tile_class(const struct builder *b, uint32_t code, uint64_t *key, unsigned *d)
{
    uint8_t px[256];
    uint64_t at = (uint64_t)code * TILE_BYTES;
    if (at + TILE_BYTES > b->tsize)
        return -1;
    tile_pixels(b->tiles + at, px);
    pixel_class(px, key, d);
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
    for (k = 1; k < count; k++) {
        uint64_t key;
        unsigned d, e;
        struct slot *s;
        if (pool[k].kind == 2)
            continue;
        if (pool[k].kind == 1) {
            key = b->zero_key;
            e = 0;
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

/* ---- records */

static int cmp_cell(const void *pa, const void *pb)
{
    const struct cell *a = pa, *c = pb;
    if (a->part != c->part) return a->part > c->part ? -1 : 1;   /* the parts last first */
    if (a->y != c->y) return a->y < c->y ? -1 : 1;
    if (a->x != c->x) return a->x < c->x ? -1 : 1;
    return a->idx < c->idx ? -1 : a->idx > c->idx;               /* equal positions keep the record's order */
}

/* The art record at a (mode 2, or mode 0 with a shared attribute) as cells in the PS1's order, tile codes plus bank. */
static int read_record(struct builder *b, uint32_t a, uint32_t bank, struct cell *cells, unsigned *count, uint32_t *end)
{
    unsigned mode, declared, nb, k, n = 0, stride, part = 0;
    uint16_t shared = 0, prev_attr = 0;
    int32_t prev_x = 0, idx;
    int64_t coord;
    uint32_t ptr;

    b->bad = 0;
    if ((a & 1) || a > b->psize || b->psize - a < 8) return fail(b, "record %#x: outside the program", (unsigned)a);
    mode = rd16(b, a);
    if (mode != 0 && mode != 2) return fail(b, "record %#x: not an art record (mode %u)", (unsigned)a, mode);
    declared = rd16(b, a + 2);
    idx = (int16_t)rd16(b, (size_t)a + 6);
    if (declared < 1 || declared > MAX_CELLS) return fail(b, "record %#x: %u cells", (unsigned)a, declared);
    if (mode == 2) {
        nb = rd16(b, (size_t)a + 4) + 1u;
        stride = 4;
    } else {
        shared = (uint16_t)(rd16(b, (size_t)a + 4) & 0x7f);
        nb = declared;
        stride = 2;
    }
    if ((size_t)a + 8 + (size_t)stride * nb > b->psize) return fail(b, "record %#x: blocks outside the program", (unsigned)a);
    coord = (int64_t)COORD_TABLE + idx;
    if (coord < 0 || (uint64_t)coord + 4 > b->psize) return fail(b, "record %#x: coordinate index %d outside the program", (unsigned)a, (int)idx);
    ptr = rd32(b, (size_t)coord);
    if ((ptr & 1) || (uint64_t)ptr + 4u * nb > b->psize) return fail(b, "record %#x: coordinates at %#x outside the program", (unsigned)a, (unsigned)ptr);
    for (k = 0; k < nb; k++) {
        uint32_t code = rd16(b, (size_t)a + 8 + (size_t)stride * k);
        uint16_t attr = mode == 2 ? rd16(b, (size_t)a + 10 + 4 * (size_t)k) : shared;
        int32_t x = (int16_t)rd16(b, (size_t)ptr + 4 * (size_t)k), y = (int16_t)rd16(b, (size_t)ptr + 4 * (size_t)k + 2);
        unsigned w = ((attr >> 8) & 15u) + 1u, h = ((attr >> 12) & 15u) + 1u, i, j;
        if (n + w * h > declared) return fail(b, "record %#x: blocks add up to more than its %u cells", (unsigned)a, declared);
        if (k && (((x - prev_x) & 15) || ((b->rules & ARCADE_SCREEN_LISTS_PAL_SPLIT) && ((attr ^ prev_attr) & 0x1fu)))) part++;
        for (j = 0; j < h; j++)
            for (i = 0; i < w; i++, n++) {
                unsigned ci = (attr & 0x20u) ? w - 1 - i : i, cj = (attr & 0x40u) ? h - 1 - j : j;
                struct cell *c = &cells[n];
                c->code = code + ci + 16u * cj + bank;
                c->x = x + 16 * (int32_t)i;
                c->y = y + 16 * (int32_t)j;
                c->flags = (uint16_t)(attr & 0x7fu);
                c->part = (uint16_t)part;
                c->idx = (uint16_t)n;
            }
        prev_x = x;
        prev_attr = attr;
    }
    if (b->bad) return fail(b, "record %#x: read outside the program", (unsigned)a);
    if (n != declared) return fail(b, "record %#x: blocks add up to %u of its %u cells", (unsigned)a, n, declared);
    qsort(cells, n, sizeof *cells, cmp_cell);
    *count = n;
    *end = a + 8 + stride * nb;
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

/* One list of the record at `a`; returns the record's end in *end. */
static int record_list(struct builder *b, const struct arcade_screen_lists_spec *s, uint32_t a, uint32_t bank, const struct arcade_screen_lists_pal *pairs,
                       unsigned npairs, unsigned index, unsigned *ex, uint32_t *end)
{
    struct cell cells[MAX_CELLS];
    uint16_t t[MAX_CELLS], f[MAX_CELLS];
    uint8_t stream[6 * MAX_CELLS];
    unsigned n = 0, k, len = 0;
    uint32_t off = 0;

    if (read_record(b, a, bank, cells, &n, end)) return -1;
    for (k = 0; k < n; k++) {
        uint32_t number;
        unsigned g, pal = cells[k].flags & 0x1fu, p;
        if (lookup(b, cells[k].code, (cells[k].flags >> 5) & 3u, &number, &g))
            return fail(b, "list %u: tile %#x is not in the segment", index, (unsigned)cells[k].code);
        for (p = 0; p < npairs; p++)
            if (pairs[p].from == pal) {
                pal = pairs[p].to;
                break;
            }
        while (*ex < s->exception_count && (s->exceptions[*ex].list < index || (s->exceptions[*ex].list == index && s->exceptions[*ex].cell < k))) (*ex)++;
        if (*ex < s->exception_count && s->exceptions[*ex].list == index && s->exceptions[*ex].cell == k) pal = s->exceptions[*ex].palette & 0x1fu;
        t[k] = (uint16_t)number;
        f[k] = (uint16_t)(pal | g << 5);
    }
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
        } else if (m->kind == ARCADE_SCREEN_LISTS_REC) {
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
            for (c = 0; c < m->count; c++, index++)
                if (record_list(&b, spec, a, m->bank, pairs, np, index, &ex, &a)) goto done;
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
            if (it->first == ARCADE_SCREENS_ZERO) {
                pool[n].kind = 1;
            } else if (it->first == ARCADE_SCREENS_DISC || it->first == ARCADE_SCREENS_PIECES) {
                pool[n].kind = 2;
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
    spec.pals = arcade_screen_lists_pals;
    spec.maps = arcade_screen_lists_maps;
    spec.map_count = arcade_screen_lists_map_count;
    rc = arcade_screen_lists_build_with(program, program_size, tiles, tiles_size, pool, count, &spec, out, err, errsize);
    free(pool);
    return rc;
}
