#include "fighter.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The conversion of the arcade's art records to the PS1's cell lists. The rules, and why, are in fighters.py (the
 * docstring of the tool and Converter / model_build, which this follows step by step). In short:
 *  - a record's blocks expand to cells, flips included; the cells are ordered by (y, x) within parts, the parts last
 *    first (a part starts where a block's x is less than the previous block's);
 *  - the file's tiles are numbered from 1 in the order of first use, each stored with the flip of its first use;
 *  - cells of horizontally adjacent tiles form position runs; identical streams are stored once;
 *  - a list is a header and a body in the shorter of two encodings, ties to the run encoding. */

#define COORD_TABLE 0x9f260u   /* arcade: table of pointers to coordinate blocks, indexed by the record's coordinate index */
#define MAX_CELLS 512u         /* a list longer than this is not a list */
#define MAX_NUMBER 0xffffu     /* tile numbers are 16-bit */
#define MAP_BITS 17
#define FLIPS 0x60u

struct cell {
    uint32_t code;
    int32_t x, y;
    uint16_t flags, part, idx;
};

struct posent {
    uint32_t off, len, hash;
};

struct tslot {
    uint32_t code, num;   /* num 0: empty */
};

struct runrec {
    size_t start;
    unsigned count;
};

struct unit {
    uint32_t rank, rec, part;
};

struct builder {
    const uint8_t *prog;
    size_t prog_size;
    int bad;
    unsigned psplit, by_run;
    uint8_t *lists;
    uint32_t lists_size, lists_cap;
    uint8_t *pos;
    uint32_t pos_size, pos_cap;
    struct posent *ents;
    size_t nents, ents_cap;
    struct tslot *tmap;
    uint32_t *tcode;
    uint8_t *tflip;
    size_t tcap;
    uint32_t next;
    uint32_t *used;
    size_t nused, used_cap;
    struct cell *run_cells;
    size_t run_n, run_cap;
    struct runrec *runrecs;
    size_t nrec, rec_cap;
    struct unit *units;
    size_t units_cap;
    char *err;
    size_t errsize;
};

static int fail(struct builder *b, const char *fmt, ...)
{
    if (b->err && b->errsize) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(b->err, b->errsize, fmt, ap);
        va_end(ap);
    }
    return -1;
}

/* Grow an array so that it holds `need` elements. */
#define GROW(b, p, cap, need) \
    do { \
        if ((size_t)(need) > (cap)) { \
            size_t n_ = (cap) ? (cap) * 2 : 64; \
            void *q_; \
            while (n_ < (size_t)(need)) n_ *= 2; \
            q_ = realloc((p), n_ * sizeof *(p)); \
            if (!q_) return fail((b), "out of memory"); \
            (p) = q_; \
            (cap) = n_; \
        } \
    } while (0)

static uint16_t rd16(struct builder *b, size_t o)
{
    if (o > b->prog_size || b->prog_size - o < 2) {
        b->bad = 1;
        return 0;
    }
    return (uint16_t)(b->prog[o] << 8 | b->prog[o + 1]);
}

static uint32_t rd32(struct builder *b, size_t o)
{
    return (uint32_t)rd16(b, o) << 16 | rd16(b, o + 2);
}

static int cmp_cell(const void *pa, const void *pb)
{
    const struct cell *a = pa, *c = pb;
    if (a->part != c->part) return a->part > c->part ? -1 : 1;   /* the parts last first */
    if (a->y != c->y) return a->y < c->y ? -1 : 1;
    if (a->x != c->x) return a->x < c->x ? -1 : 1;
    return a->idx < c->idx ? -1 : a->idx > c->idx;               /* equal positions keep the record's order */
}

static int cmp_unit(const void *pa, const void *pb)
{
    const struct unit *a = pa, *c = pb;
    if (a->rank != c->rank) return a->rank < c->rank ? -1 : 1;
    return a->rec < c->rec ? -1 : a->rec > c->rec;
}

/* The art record at a (mode 2, or mode 0 with one code a cell) as cells in record order, with their parts. */
static int read_record(struct builder *b, uint32_t a, uint32_t bank, struct cell *cells, unsigned *count, uint32_t *end)
{
    unsigned mode, declared, nb, k, n = 0, g = 0, gp = 0, stride;
    uint16_t shared = 0, prev_attr = 0;
    int32_t prev_x = 0, idx;
    int64_t coord;
    uint32_t ptr;

    b->bad = 0;
    if ((a & 1) || a > b->prog_size || b->prog_size - a < 8) return fail(b, "record %#x: outside the program", (unsigned)a);
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
    if ((size_t)a + 8 + (size_t)stride * nb > b->prog_size) return fail(b, "record %#x: blocks outside the program", (unsigned)a);
    coord = (int64_t)COORD_TABLE + idx;
    if (coord < 0 || (uint64_t)coord + 4 > b->prog_size) return fail(b, "record %#x: coordinate index %d outside the program", (unsigned)a, (int)idx);
    ptr = rd32(b, (size_t)coord);
    if ((ptr & 1) || (uint64_t)ptr + 4u * nb > b->prog_size) return fail(b, "record %#x: coordinates at %#x outside the program", (unsigned)a, (unsigned)ptr);
    for (k = 0; k < nb; k++) {
        uint32_t code = rd16(b, (size_t)a + 8 + (size_t)stride * k);
        uint16_t attr = mode == 2 ? rd16(b, (size_t)a + 10 + 4 * (size_t)k) : shared;
        int32_t x = (int16_t)rd16(b, (size_t)ptr + 4 * (size_t)k), y = (int16_t)rd16(b, (size_t)ptr + 4 * (size_t)k + 2);
        unsigned w = ((attr >> 8) & 15u) + 1u, h = ((attr >> 12) & 15u) + 1u, i, j;
        if (n + w * h > declared) return fail(b, "record %#x: blocks add up to more than its %u cells", (unsigned)a, declared);
        if (k && x < prev_x) g++;
        if (k && (x < prev_x || ((attr ^ prev_attr) & 0x1fu))) gp++;
        for (j = 0; j < h; j++)
            for (i = 0; i < w; i++) {
                unsigned ci = (attr & 0x20u) ? w - 1 - i : i, cj = (attr & 0x40u) ? h - 1 - j : j;
                struct cell *c = &cells[n];
                c->code = code + ci + 16u * cj + bank;
                c->x = x + 16 * (int32_t)i;
                c->y = y + 16 * (int32_t)j;
                c->flags = (uint16_t)(attr & 0x7fu);
                c->part = (uint16_t)(b->psplit ? gp : g);
                c->idx = (uint16_t)n;
                n++;
            }
        prev_x = x;
        prev_attr = attr;
    }
    if (b->bad) return fail(b, "record %#x: read outside the program", (unsigned)a);
    if (n != declared) return fail(b, "record %#x: blocks add up to %u of its %u cells", (unsigned)a, n, declared);
    *count = n;
    *end = a + 8 + stride * nb;
    return 0;
}

/* ---- output blocks */

static int lists_put(struct builder *b, const uint8_t *p, size_t n)
{
    if (n > b->lists_cap - b->lists_size) return fail(b, "the lists need more than their room of %u bytes", (unsigned)b->lists_cap);
    memcpy(b->lists + b->lists_size, p, n);
    b->lists_size += (uint32_t)n;
    return 0;
}

static void put16(uint8_t *p, unsigned v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

/* Offset of the stream in the positions: an identical stream stored before, else appended (always appended when
 * `fresh`, and then not found by later streams). */
static int pos_add(struct builder *b, const uint8_t *s, uint32_t len, int fresh, uint32_t *off)
{
    uint32_t h = 2166136261u, i;
    size_t e;
    for (i = 0; i < len; i++) h = (h ^ s[i]) * 16777619u;
    if (!fresh)
        for (e = 0; e < b->nents; e++)
            if (b->ents[e].hash == h && b->ents[e].len == len && memcmp(b->pos + b->ents[e].off, s, len) == 0) {
                *off = b->ents[e].off;
                return 0;
            }
    if (len > b->pos_cap - b->pos_size) return fail(b, "the positions need more than their room of %u bytes", (unsigned)b->pos_cap);
    memcpy(b->pos + b->pos_size, s, len);
    *off = b->pos_size;
    b->pos_size += len;
    if (!fresh) {
        struct posent *pe;
        GROW(b, b->ents, b->ents_cap, b->nents + 1);
        pe = &b->ents[b->nents++];
        pe->off = *off;
        pe->len = len;
        pe->hash = h;
    }
    return 0;
}

/* The list for numbered cells (number, flags) in order and its position offset. Plain (no flip bit, one flags value
 * for all): mode 0 (a word a cell) or mode 4 (first number, run - 1), the shorter wins, a tie goes to 4, the third
 * header word is the palette. Otherwise mode 2 (number, flags) or mode 6 (first number, flags, run - 1), the shorter
 * wins, a tie goes to 6, the third word is cells - 1. */
static int encode_list(struct builder *b, const uint16_t *t, const uint16_t *f, unsigned n, uint32_t pos)
{
    uint8_t buf[10 + 6 * MAX_CELLS];
    uint16_t rt[MAX_CELLS], rf[MAX_CELLS], rr[MAX_CELLS];
    unsigned i, nr = 0, plain = 1, mode, len;

    if (pos > 0xffffu) return fail(b, "a position offset of %u does not fit 16 bits", (unsigned)pos);
    for (i = 0; i < n; i++)
        if ((f[i] & FLIPS) || f[i] != f[0]) plain = 0;
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
        put16(buf + 2, mode);
        put16(buf + 4, f[0] & 0x1fu);
    } else {
        mode = 2 * n < 3 * nr ? 2 : 6;
        put16(buf + 2, mode);
        put16(buf + 4, n - 1);
    }
    put16(buf, n);
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
    return lists_put(b, buf, len);
}

/* ---- tiles */

static uint32_t tile_find(const struct builder *b, uint32_t code)
{
    size_t m = ((size_t)1 << MAP_BITS) - 1, i = (size_t)((code * 2654435761u) >> (32 - MAP_BITS));
    while (b->tmap[i].num) {
        if (b->tmap[i].code == code) return b->tmap[i].num;
        i = (i + 1) & m;
    }
    return 0;
}

/* The number of the code, new (the next one, stored with the flip of this use) when not seen before. */
static int tile_assign(struct builder *b, uint32_t code, unsigned flags)
{
    size_t m = ((size_t)1 << MAP_BITS) - 1, i = (size_t)((code * 2654435761u) >> (32 - MAP_BITS));
    while (b->tmap[i].num) {
        if (b->tmap[i].code == code) return 0;
        i = (i + 1) & m;
    }
    if (b->next > MAX_NUMBER) return fail(b, "more than %u tiles", MAX_NUMBER);
    if ((size_t)b->next + 1 > b->tcap) {
        size_t cap = b->tcap * 2;
        uint32_t *nc = realloc(b->tcode, cap * sizeof *nc);
        if (nc) b->tcode = nc;
        if (nc) {
            uint8_t *nf = realloc(b->tflip, cap);
            if (nf) {
                b->tflip = nf;
                b->tcap = cap;
            }
        }
        if ((size_t)b->next + 1 > b->tcap) return fail(b, "out of memory");
    }
    b->tcode[b->next] = code;
    b->tflip[b->next] = (uint8_t)(((flags & 0x20u) ? 1 : 0) | ((flags & 0x40u) ? 2 : 0));
    b->tmap[i].code = code;
    b->tmap[i].num = b->next++;
    return 0;
}

/* ---- records to lists */

static int emit(struct builder *b, const struct cell *cells, unsigned n, int fresh)
{
    uint16_t t[MAX_CELLS], f[MAX_CELLS];
    uint8_t stream[6 * MAX_CELLS];
    unsigned i, len = 0;
    int32_t rx = 0, ry = 0, lx = 0, ly = 0;
    unsigned rn = 0, have = 0;
    uint32_t off = 0;

    for (i = 0; i < n; i++) {
        uint32_t num = tile_find(b, cells[i].code);
        unsigned bake;
        if (!num) return fail(b, "internal: tile %#x not numbered", (unsigned)cells[i].code);
        bake = ((b->tflip[num] & 1u) ? 0x20u : 0) | ((b->tflip[num] & 2u) ? 0x40u : 0);
        t[i] = (uint16_t)num;
        f[i] = (uint16_t)(cells[i].flags ^ bake);
    }
    /* position runs: horizontally adjacent cells (the cells are in order); a run is x, y, length - 1 */
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
    if (pos_add(b, stream, len, fresh, &off)) return -1;
    return encode_list(b, t, f, n, off);
}

static int empty_list(struct builder *b)
{
    uint8_t e[12] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (b->pos_size > 0xffffu) return fail(b, "a position offset of %u does not fit 16 bits", (unsigned)b->pos_size);
    put16(e + 8, b->pos_size);
    return lists_put(b, e, sizeof e);
}

/* The tiles of the held records, unit by unit (PL0E): a unit is one part of one record; the parts of a record are ranked
 * by cell count, largest first (then by part); the units go by rank, then by record. Then the lists. */
static int flush(struct builder *b)
{
    size_t i, nu = 0, u;
    if (!b->nrec) return 0;
    for (i = 0; i < b->nrec; i++) {
        const struct cell *cells = b->run_cells + b->runrecs[i].start;
        unsigned n = b->runrecs[i].count, cnt[MAX_CELLS], nparts = 0, p, q, c;
        memset(cnt, 0, sizeof cnt);
        for (c = 0; c < n; c++) {
            cnt[cells[c].part]++;
            if (cells[c].part + 1u > nparts) nparts = cells[c].part + 1u;
        }
        GROW(b, b->units, b->units_cap, nu + nparts);
        for (p = 0; p < nparts; p++) {
            unsigned rank = 0;
            for (q = 0; q < nparts; q++)
                if (cnt[q] > cnt[p] || (cnt[q] == cnt[p] && q < p)) rank++;
            b->units[nu].rank = rank;
            b->units[nu].rec = (uint32_t)i;
            b->units[nu].part = p;
            nu++;
        }
    }
    qsort(b->units, nu, sizeof *b->units, cmp_unit);
    for (u = 0; u < nu; u++) {
        const struct cell *cells = b->run_cells + b->runrecs[b->units[u].rec].start;
        unsigned n = b->runrecs[b->units[u].rec].count, c;
        for (c = 0; c < n; c++)
            if (cells[c].part == b->units[u].part && tile_assign(b, cells[c].code, cells[c].flags)) return -1;
    }
    for (i = 0; i < b->nrec; i++)
        if (emit(b, b->run_cells + b->runrecs[i].start, b->runrecs[i].count, 0)) return -1;
    b->nrec = 0;
    b->run_n = 0;
    return 0;
}

static int record(struct builder *b, uint32_t at, struct cell *cells, unsigned n)
{
    int32_t left = cells[0].x;
    unsigned i, again, parts = 1;
    size_t e;

    qsort(cells, n, sizeof *cells, cmp_cell);
    for (i = 0; i < n; i++)
        if (cells[i].x < left) left = cells[i].x;
    again = 0;
    for (e = 0; e < b->nused; e++)
        if (b->used[e] == at) again = left < -256;
    if (again)   /* a record drawn a second time and starting far left: its leftmost cell goes to x = 0, its stream apart */
        for (i = 0; i < n; i++) cells[i].x -= left;
    GROW(b, b->used, b->used_cap, b->nused + 1);
    b->used[b->nused++] = at;
    for (i = 1; i < n; i++)
        if (cells[i].part != cells[i - 1].part) parts++;
    if (b->by_run && parts > 1) {
        GROW(b, b->run_cells, b->run_cap, b->run_n + n);
        GROW(b, b->runrecs, b->rec_cap, b->nrec + 1);
        memcpy(b->run_cells + b->run_n, cells, n * sizeof *cells);
        b->runrecs[b->nrec].start = b->run_n;
        b->runrecs[b->nrec].count = n;
        b->nrec++;
        b->run_n += n;
        return 0;
    }
    if (flush(b)) return -1;
    for (i = 0; i < n; i++)
        if (tile_assign(b, cells[i].code, cells[i].flags)) return -1;
    return emit(b, cells, n, again);
}

static void release(struct builder *b)
{
    free(b->ents);
    free(b->tmap);
    free(b->used);
    free(b->run_cells);
    free(b->runrecs);
    free(b->units);
}

void arcade_fighter_free(struct arcade_fighter_out *out)
{
    free(out->positions);
    free(out->lists);
    free(out->tile_code);
    free(out->tile_flip);
    memset(out, 0, sizeof *out);
}

int arcade_fighter_build(const uint8_t *program, size_t program_size, const struct arcade_fighter *f,
                         struct arcade_fighter_out *out, char *err, size_t errsize)
{
    struct builder b;
    unsigned s, c;
    int rc = -1;

    memset(out, 0, sizeof *out);
    memset(&b, 0, sizeof b);
    b.prog = program;
    b.prog_size = program_size;
    b.err = err;
    b.errsize = errsize;
    b.psplit = (f->rules & ARCADE_FIGHTER_PALETTE_PARTS) != 0;
    b.by_run = (f->rules & ARCADE_FIGHTER_NUMBER_BY_RUN) != 0;
    if (err && errsize) err[0] = 0;
    if (f->segment_count && !f->segments) return fail(&b, "%s: segments missing", f->file);
    b.lists_cap = f->lists_room;
    b.pos_cap = f->positions_room;
    b.lists = malloc((size_t)b.lists_cap + 1);
    b.pos = malloc((size_t)b.pos_cap + 1);
    b.tmap = calloc((size_t)1 << MAP_BITS, sizeof *b.tmap);
    if (!b.lists || !b.pos || !b.tmap) {
        fail(&b, "out of memory");
        goto done;
    }
    b.tcap = 1024;
    b.tcode = calloc(b.tcap, sizeof *b.tcode);
    b.tflip = calloc(b.tcap, 1);
    if (!b.tcode || !b.tflip) {
        fail(&b, "out of memory");
        goto done;
    }
    b.next = 1;
    for (s = 0; s < f->segment_count; s++) {
        const struct arcade_fighter_segment *seg = &f->segments[s];
        uint32_t a = seg->address;
        for (c = 0; c < seg->count; c++) {
            if (!seg->address) {
                if (flush(&b) || empty_list(&b)) goto done;
            } else {
                struct cell cells[MAX_CELLS];
                unsigned n = 0;
                uint32_t end;
                if (read_record(&b, a, seg->bank, cells, &n, &end) || record(&b, a, cells, n)) goto done;
                a = end;
            }
        }
    }
    if (flush(&b)) goto done;
    if ((f->rules & ARCADE_FIGHTER_TERMINATOR) && empty_list(&b)) goto done;
    out->positions = b.pos;
    out->positions_size = b.pos_size;
    out->lists = b.lists;
    out->lists_size = b.lists_size;
    out->tile_count = b.next;
    out->tile_code = b.tcode;
    out->tile_flip = b.tflip;
    b.pos = b.lists = NULL;
    b.tcode = NULL;
    b.tflip = NULL;
    rc = 0;
done:
    free(b.pos);
    free(b.lists);
    free(b.tcode);
    free(b.tflip);
    release(&b);
    return rc;
}
