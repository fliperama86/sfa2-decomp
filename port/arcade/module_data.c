#include "module_data.h"
#include <stdlib.h>
#include <string.h>

const struct arcade_module_data *arcade_module_data_find(const char *file, unsigned slot)
{
    unsigned i;
    if (!file) return NULL;
    for (i = 0; i < arcade_module_data_count; i++)
        if (arcade_module_data[i].slot == slot && !strcmp(arcade_module_data[i].file, file)) return &arcade_module_data[i];
    return NULL;
}

/* Bytes of one field of the letter: PS1 and arcade. 0 for an unknown letter. */
static int field_sizes(char c, unsigned *ps1, unsigned *arc)
{
    *ps1 = *arc = 0;
    switch (c) {
    case 'b': case 'w': *ps1 = 2; *arc = 2; return 1;
    case 'l': *ps1 = 4; *arc = 4; return 1;
    case 'n': case 'f': case 'q': *ps1 = 1; *arc = 2; return 1;
    case 'e': *ps1 = 4; *arc = 1; return 1;
    case 'j': *ps1 = 4; *arc = 8; return 1;
    case 'k': case 'z': case 'K': case 'Z': *ps1 = 32; *arc = 32; return 1;
    default: return 0;
    }
}

/* A palette row (letters k, z, K, Z; the rule of screens.py's palette_bytes): colour 0 is 0, colours 1 to 15 are the first 15 big-endian words
 * of a (the brightness nibble ignored) as R * 2 | (G * 2) << 5 | (B * 2) << 10; a black result is 0x8000 unless black_zero; bit 15 is set on
 * colours 1 to 15 with bit15. */
static void palette_build(const uint8_t *a, int black_zero, int bit15, uint8_t *o)
{
    unsigned k;
    o[0] = o[1] = 0;
    for (k = 0; k < 15; k++) {
        unsigned w = ((unsigned)a[2 * k] << 8) | a[2 * k + 1];
        unsigned c = (w >> 8 & 15u) << 1 | (w >> 4 & 15u) << 6 | (w & 15u) << 11;
        if (!c && !black_zero) c = 0x8000u;
        if (bit15) c |= 0x8000u;
        o[2 + 2 * k] = (uint8_t)c;
        o[3 + 2 * k] = (uint8_t)(c >> 8);
    }
}

/* One field: the PS1 bytes into o from the arcade bytes a. -1 when the value does not fit the letter. */
static int field_build(char c, const uint8_t *a, uint8_t *o)
{
    unsigned v = c == 'e' || c == 'j' ? 0u : ((unsigned)a[0] << 8) | a[1];
    switch (c) {
    case 'b': o[0] = a[0]; o[1] = a[1]; return 0;
    case 'w': o[0] = a[1]; o[1] = a[0]; return 0;
    case 'l': o[0] = a[3]; o[1] = a[2]; o[2] = a[1]; o[3] = a[0]; return 0;
    case 'e': o[0] = a[0]; o[1] = o[2] = o[3] = 0; return 0;
    case 'j': o[0] = a[5]; o[1] = a[4]; o[2] = a[1]; o[3] = a[0]; return 0;
    case 'k': palette_build(a, 0, 0, o); return 0;
    case 'z': palette_build(a, 1, 0, o); return 0;
    case 'K': palette_build(a, 0, 1, o); return 0;
    case 'Z': palette_build(a, 1, 1, o); return 0;
    case 'n': if (v > 255u) return -1; o[0] = (uint8_t)v; return 0;
    case 'f': if (v & 0x7f80u) return -1; o[0] = (uint8_t)(((v >> 8) & 0x80u) | (v & 0x7fu)); return 0;
    case 'q': if (v % 24u || v / 24u > 255u) return -1; o[0] = (uint8_t)(v / 24u); return 0;
    default: return -1;
    }
}

/* A decimal number at *s (at least one digit, at most 9): advances *s. -1 when there is none. */
static long number(const char **s)
{
    long v = 0;
    int n = 0;
    while (**s >= '0' && **s <= '9' && n < 9) { v = v * 10 + (**s - '0'); (*s)++; n++; }
    return n ? v : -1;
}

/* One token (a letter and its count) at *s. */
static int token(const char **s, char *letter, long *count)
{
    unsigned a, b;
    if (!field_sizes(**s, &a, &b)) return -1;
    *letter = **s;
    (*s)++;
    *count = number(s);
    return *count > 0 ? 0 : -1;
}

/* Walk the layout. With out == NULL only the totals: *ps1 PS1 bytes and *arc arcade bytes it makes/reads (a malformed layout is -1).
 * With out, build the fields from the arcade bytes at program + source (the caller has checked the totals fit) into out[0..outsize). */
static int walk(const char *s, const uint8_t *program, uint32_t source, uint8_t *out, uint32_t outsize, uint64_t *ps1, uint64_t *arc)
{
    uint64_t tp = 0, ta = 0;
    uint32_t at = 0;
    if (!s || !*s) return -1;
    while (*s) {
        char letter;
        long count, reps = 1;
        char letters[16];
        long counts[16];
        int n = 0, k;
        if (*s == '(') {
            s++;
            while (*s && *s != ')') {
                if (n >= 16 || token(&s, &letters[n], &counts[n])) return -1;
                n++;
            }
            if (*s != ')' || n == 0) return -1;
            s++;
            reps = number(&s);
            if (reps <= 0) return -1;
        } else {
            if (token(&s, &letter, &count)) return -1;
            letters[0] = letter;
            counts[0] = count;
            n = 1;
        }
        {
            uint64_t sp = 0, sa = 0;
            for (k = 0; k < n; k++) {
                unsigned p, a;
                field_sizes(letters[k], &p, &a);
                sp += (uint64_t)p * (uint64_t)counts[k];
                sa += (uint64_t)a * (uint64_t)counts[k];
            }
            if (sp == 0 || sa == 0 || (uint64_t)reps > 0xffffffffull / sp || (uint64_t)reps > 0xffffffffull / sa) return -1;
            tp += sp * (uint64_t)reps;
            ta += sa * (uint64_t)reps;
            if (tp > 0xffffffffull || ta > 0xffffffffull) return -1;
        }
        if (out) {
            long r, c;
            for (r = 0; r < reps; r++)
                for (k = 0; k < n; k++)
                    for (c = 0; c < counts[k]; c++) {
                        unsigned p, a;
                        field_sizes(letters[k], &p, &a);
                        if (at + p > outsize) return -1;
                        if (field_build(letters[k], program + source, out + at)) return -1;
                        at += p;
                        source += a;
                    }
        }
    }
    if (ps1) *ps1 = tp;
    if (arc) *arc = ta;
    return 0;
}

int arcade_module_piece_build(const uint8_t *program, size_t program_size, const struct arcade_module_piece *p, uint8_t *out)
{
    uint64_t ps1 = 0, arc = 0;
    if (!program || !p || !out || !p->layout || (p->source & 1u)) return -1;
    if (walk(p->layout, NULL, 0, NULL, 0, &ps1, &arc) || ps1 != p->size || p->source > program_size || arc > program_size - p->source) return -1;
    return walk(p->layout, program, p->source, out, p->size, NULL, NULL);
}

int arcade_module_data_valid(const struct arcade_module_data *d)
{
    unsigned i, j;
    uint64_t ps1, arc;
    if (!d || !d->file || (d->load & 3u) || (d->piece_count && !d->pieces) || (d->reloc_count && !d->relocs)) return -1;
    for (i = 0; i < d->piece_count; i++) {
        const struct arcade_module_piece *p = &d->pieces[i];
        if (p->size == 0 || p->offset > d->size || p->size > d->size - p->offset || (p->source & 1u)) return -1;
        if (!p->layout || walk(p->layout, NULL, 0, NULL, 0, &ps1, &arc) || ps1 != p->size) return -1;
        if (i && p->offset < d->pieces[i - 1].offset + d->pieces[i - 1].size) return -1;
    }
    for (i = 0; i < d->reloc_count; i++) {
        const struct arcade_module_reloc *r = &d->relocs[i];
        if ((r->offset & 3u) || r->offset > d->size || d->size - r->offset < 4u) return -1;
        if (i && r->offset < d->relocs[i - 1].offset + 4u) return -1;
    }
    if ((d->name_count && !d->names) || (d->shape_count && !d->shapes)) return -1;
    for (i = 0; i < d->name_count; i++) {
        const struct arcade_module_names *n = &d->names[i];
        if (!n->count || !n->size || n->offset > d->size || n->size > d->size - n->offset || n->stride < 8u) return -1;
    }
    for (i = 0; i < d->shape_count; i++) {
        const struct arcade_module_shapes *s = &d->shapes[i];
        if (!s->count || (s->steps & 1u) || (s->list_at & 3u) || (s->data_at & 3u) || (s->coords_at & 1u)) return -1;
        if (s->list_at > d->size || 12ull * s->count > d->size - s->list_at || s->data_at > d->size || s->coords_at > d->size) return -1;
    }
    for (i = 0, j = 0; i < d->piece_count && j < d->reloc_count;) {   /* a word inside a piece */
        uint32_t pa = d->pieces[i].offset, pb = pa + d->pieces[i].size, ra = d->relocs[j].offset;
        if (ra + 4u <= pa) j++;
        else if (ra >= pb) i++;
        else return -1;
    }
    return 0;
}

/* The names' records into module[offset ..) (known set for the bytes written). -1 when the names do not fit the program or the room. */
static int names_build(const uint8_t *program, size_t program_size, const struct arcade_module_names *n, uint8_t *module, uint8_t *known, size_t module_size)
{
    uint32_t at = 0;
    unsigned i;
    if (n->offset > module_size || n->size > module_size - n->offset) return -1;
    memset(module + n->offset, 0, n->size);
    for (i = 0; i < n->count; i++) {
        unsigned k = n->reverse ? n->count - 1u - i : i, len = 8, left, free_, w;
        uint64_t a = (uint64_t)n->source + (uint64_t)n->stride * k;
        const uint8_t *t;
        if (a > program_size || program_size - a < 8) return -1;
        t = program + a;
        while (len && t[len - 1] == ' ') len--;
        if (!len || len > 7) return -1;
        free_ = 7 - len;
        left = (free_ + 1) / 2;
        w = left + len + 2;
        if (at + w > n->size) return -1;
        memset(module + n->offset + at, ' ', left);
        memcpy(module + n->offset + at + left, t, len);
        module[n->offset + at + left + len] = '@';
        module[n->offset + at + left + len + 1] = '@';
        at += (w + 4u) & ~3u;
        if (at > n->size) return -1;
    }
    if (at != n->size) return -1;
    if (known) memset(known + n->offset, 1, n->size);
    return 0;
}

/* The cells of a portrait record (a mode 4 art record): cell k of the result is (x, y, attribute), in order of y, then x, a later block over an
 * earlier at the same place. Returns the count, or -1. */
#define SHAPE_CELLS 512
struct shape_cell { int16_t x, y; uint16_t attr; };

static int cmp_cell(const void *pa, const void *pb)
{
    const struct shape_cell *a = pa, *b = pb;
    if (a->y != b->y) return a->y < b->y ? -1 : 1;
    return a->x < b->x ? -1 : a->x > b->x;
}

static int shape_cells(const uint8_t *prog, size_t size, uint32_t a, struct shape_cell *out)
{
    unsigned nb, b, n = 0, i, j, w, h, k, m;
    if ((a & 1u) || a > size || size - a < 6 || (((unsigned)prog[a] << 8) | prog[a + 1]) != 4u) return -1;
    nb = (((unsigned)prog[a + 4] << 8) | prog[a + 5]) + 1u;
    if (size - a < 6u + 8u * nb) return -1;
    for (b = 0; b < nb; b++) {
        const uint8_t *r = prog + a + 6 + 8 * (size_t)b;
        unsigned attr = ((unsigned)r[2] << 8) | r[3];
        int x = (int16_t)(((unsigned)r[4] << 8) | r[5]), y = (int16_t)(((unsigned)r[6] << 8) | r[7]);
        w = ((attr >> 8) & 15u) + 1u;
        h = (attr >> 12) + 1u;
        for (j = 0; j < h; j++)
            for (i = 0; i < w; i++) {
                int cx = x + 16 * (int)i, cy = y + 16 * (int)j;
                for (k = 0; k < n; k++)
                    if (out[k].x == cx && out[k].y == cy) break;
                if (k == n) {
                    if (n >= SHAPE_CELLS || cx < -32768 || cx > 32767 || cy < -32768 || cy > 32767) return -1;
                    n++;
                }
                out[k].x = (int16_t)cx;
                out[k].y = (int16_t)cy;
                out[k].attr = (uint16_t)attr;
            }
    }
    for (m = 0; m < n; m++)
        if ((out[m].attr & 0x1fu) < 2u) return -1;   /* the palettes of a portrait are 2 and up: the shape's byte is the palette less 2 */
    qsort(out, n, sizeof *out, cmp_cell);
    return (int)n;
}

static int shapes_build(const uint8_t *program, size_t program_size, const struct arcade_module_shapes *s, uint8_t *module, uint8_t *known, size_t module_size)
{
    uint32_t data = s->data_at, coords = s->coords_at;
    unsigned i, k;
    for (i = 0; i < s->count; i++) {
        struct shape_cell cells[SHAPE_CELLS];
        uint64_t sp = (uint64_t)s->steps + (uint64_t)s->stride * i;
        uint32_t a, list = s->list_at + 12u * i;
        int n;
        if (sp > program_size || program_size - sp < 4) return -1;
        a = ((uint32_t)program[sp] << 24) | ((uint32_t)program[sp + 1] << 16) | ((uint32_t)program[sp + 2] << 8) | program[sp + 3];
        n = shape_cells(program, program_size, a, cells);
        if (n < 1) return -1;
        if (list > module_size || module_size - list < 4u || data > module_size || (uint32_t)n > module_size - data ||
            coords > module_size || 4ull * (unsigned)n > module_size - coords)
            return -1;
        module[list] = (uint8_t)n;
        module[list + 1] = (uint8_t)((unsigned)n >> 8);
        module[list + 2] = module[list + 3] = 0;
        if (known) memset(known + list, 1, 4);
        for (k = 0; k < (unsigned)n; k++) module[data + k] = (uint8_t)((cells[k].attr & 0x1fu) - 2u);
        if (known) memset(known + data, 1, (size_t)n);
        {   /* the pad to the next multiple of 4 is zero */
            uint32_t end = (data + (uint32_t)n + 3u) & ~3u;
            if (end > module_size) return -1;
            memset(module + data + n, 0, end - data - (uint32_t)n);
            if (known) memset(known + data + n, 1, end - data - (uint32_t)n);
            data = end;
        }
        for (k = 0; k < (unsigned)n; k++) {
            module[coords + 4 * k] = (uint8_t)cells[k].x;
            module[coords + 4 * k + 1] = (uint8_t)((uint16_t)cells[k].x >> 8);
            module[coords + 4 * k + 2] = (uint8_t)cells[k].y;
            module[coords + 4 * k + 3] = (uint8_t)((uint16_t)cells[k].y >> 8);
        }
        if (known) memset(known + coords, 1, 4u * (unsigned)n);
        coords += 4u * (unsigned)n;
    }
    return 0;
}

int arcade_module_data_build(const uint8_t *program, size_t program_size, const struct arcade_module_data *d,
                             uint8_t *module, uint8_t *known, size_t module_size)
{
    unsigned i;
    int rc = 0;
    if (!program || !d || !module || module_size != d->size) return -1;
    for (i = 0; i < d->piece_count; i++) {
        const struct arcade_module_piece *p = &d->pieces[i];
        if (p->offset > module_size || p->size > module_size - p->offset) { rc = -1; continue; }
        if (arcade_module_piece_build(program, program_size, p, module + p->offset)) {
            memset(module + p->offset, 0, p->size);
            rc = -1;
            continue;
        }
        if (known) memset(known + p->offset, 1, p->size);
    }
    for (i = 0; i < d->name_count; i++)
        if (names_build(program, program_size, &d->names[i], module, known, module_size)) rc = -1;
    for (i = 0; i < d->shape_count; i++)
        if (shapes_build(program, program_size, &d->shapes[i], module, known, module_size)) rc = -1;
    for (i = 0; i < d->reloc_count; i++) {
        const struct arcade_module_reloc *r = &d->relocs[i];
        uint32_t w = r->target < ARCADE_MODULE_RELATIVE ? d->load + r->target : r->target;
        if ((r->offset & 3u) || r->offset > module_size || module_size - r->offset < 4u) { rc = -1; continue; }
        module[r->offset] = (uint8_t)w;
        module[r->offset + 1] = (uint8_t)(w >> 8);
        module[r->offset + 2] = (uint8_t)(w >> 16);
        module[r->offset + 3] = (uint8_t)(w >> 24);
        if (known) memset(known + r->offset, 1, 4);
    }
    return rc;
}
