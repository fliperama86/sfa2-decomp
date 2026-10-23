/* A PS1 stage file's pieces built from the arcade program and graphics; see stage.h. The specification is stages.py's model
 * (Arc, layer_cells, place_layer, model_stage ...). Project-authored, plain C99. No byte of either game is here. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stage.h"

#define FIRST_PAGE 22u
#define PAGE_BYTES 32768u
#define SETS1 0x2ad300u            /* scroll 1 block sets: 143 of 128 bytes (8 x 8 big-endian ids of 4 x 4 cell groups) */
#define GROUPS1 0x2b1a80u          /* the 4 x 4 cell groups: 64 bytes, column by column */
#define TABLE16 0x2d1f00u          /* 154 blocks of 16 x 16 cells, 1,024 bytes */
#define TABLE32 0x2f8700u          /* 101 blocks of 8 x 8 cells, 256 bytes */
#define SET_COUNT 143u
#define BLOCK16_COUNT 154u
#define BLOCK32_COUNT 101u
#define BRIGHT 43u                 /* 0x0f + 2 * 14 */
#define OBJECT_PALETTES_BYTES 224u

static const uint32_t palette_tables[3] = {0x27c1c0u, 0x2865c0u, 0x2909c0u};
#define PALETTE_OBJECTS 0x275c40u
#define FILL_COLOUR 0x322u         /* the arcade word of a filler palette's fifteen colours */

struct ctx {
    const uint8_t *prog, *tiles;
    size_t psize, tsize;
    char *err;
    size_t errsize;
};

static int fail(struct ctx *c, const char *what, unsigned a, unsigned b)
{
    if (c->err && c->errsize)
        snprintf(c->err, c->errsize, what, a, b);
    return -1;
}

/* ---- tiles */

static const uint8_t *tile_ptr(const struct ctx *c, uint32_t index, unsigned size)
{
    uint64_t at = (uint64_t)index * size;
    return at + size <= c->tsize ? c->tiles + at : NULL;
}

/* The arcade value of pixel (x, y) of a tile of `side` pixels: four planes, 8 pixels to a byte, 4 bytes to 8 pixels. */
static unsigned arcade_px(const uint8_t *t, unsigned side, unsigned x, unsigned y)
{
    const uint8_t *p = t + y * side / 2 + (x >> 3) * 4;
    unsigned sh = 7u - (x & 7u);
    return ((p[0] >> sh) & 1u) | (((p[1] >> sh) & 1u) << 1) | (((p[2] >> sh) & 1u) << 2) | (((p[3] >> sh) & 1u) << 3);
}

/* The PS1 bytes of a tile of `side` pixels (side rows of side / 2 bytes, the left pixel low): colour one higher, flipped.
 * A missing tile (t NULL) is transparent. */
static void ps1_tile(const uint8_t *t, unsigned side, int fx, int fy, uint8_t *out)
{
    unsigned x, y;
    for (y = 0; y < side; y++) {
        unsigned sy = fy ? side - 1u - y : y;
        for (x = 0; x < side; x += 2) {
            unsigned a = 0, b = 0;
            if (t) {
                a = (arcade_px(t, side, fx ? side - 1u - x : x, sy) + 1u) & 15u;
                b = (arcade_px(t, side, fx ? side - 2u - x : x + 1u, sy) + 1u) & 15u;
            }
            out[y * side / 2 + x / 2] = (uint8_t)(a | b << 4);
        }
    }
}

/* Pixel (x, y) of a scroll 1 cell (8 x 8, the left half of the top or bottom eight rows of a 16 x 16 tile), flipped as its attribute says. */
static unsigned piece_px(const struct ctx *c, unsigned code, unsigned attr, unsigned x, unsigned y)
{
    const uint8_t *t = tile_ptr(c, 0x10000u + (code >> 1), 128);
    unsigned sx = (attr & 0x20u) ? 7u - x : x, sy = (attr & 0x40u) ? 7u - y : y;
    if (!t)
        return 0;
    return (arcade_px(t, 16, sx, (code & 1u) * 8u + sy) + 1u) & 15u;
}

static unsigned be16(const uint8_t *p)
{
    return (unsigned)p[0] << 8 | p[1];
}

/* ---- slots and maps */

/* The tile code of a page slot (16 pixel tiles 16 to a column, 16 columns to a page, column by column; 32 pixel tiles 8 and 8). */
static int slot_code(uint32_t slot, unsigned side, uint32_t *code)
{
    uint32_t per = side == 16 ? 256u : 64u, rest = slot % per, page = slot / per + FIRST_PAGE;
    if (page > 63u)
        return -1;
    *code = side == 16 ? page << 10 | ((rest % 16u) * 2u) << 5 | (rest / 16u) * 2u
                       : page << 10 | ((rest % 8u) * 4u) << 5 | (rest / 8u) * 4u;
    return 0;
}

/* The byte offset in the pages of a slot's first row, or -1 when the tile does not lie inside `size` bytes of pages. */
static int64_t slot_place(uint32_t slot, unsigned side, uint32_t size)
{
    uint32_t per = side == 16 ? 256u : 64u, cols = side == 16 ? 16u : 8u, rest = slot % per;
    uint64_t at = (uint64_t)(slot / per) * PAGE_BYTES + (uint64_t)(rest % cols) * side * 128u + (uint64_t)(rest / cols) * side / 2u;
    uint32_t code;
    if (slot_code(slot, side, &code) || at + (uint64_t)(side - 1u) * 128u + side / 2u > size)
        return -1;
    return (int64_t)at;
}

static void mark_known(uint8_t *known, int64_t at, unsigned side, int value)
{
    unsigned y;
    for (y = 0; y < side; y++)
        memset(known + at + y * 128, value, side / 2);
}

static void put_slot(uint8_t *pages, int64_t at, unsigned side, const uint8_t *tile)
{
    unsigned y, per = side / 2;
    for (y = 0; y < side; y++)
        memcpy(pages + at + y * 128, tile + y * per, per);
}

static uint32_t map_word_index(unsigned cx, unsigned cy, unsigned across, unsigned side)
{
    if (side == 16)
        return (cy >> 4) * ((across >> 5) << 10) + ((cx >> 5) << 10) + ((cy & 15u) << 6) + ((cx & 31u) << 1);
    return (cy >> 3) * ((across >> 4) << 8) + ((cx >> 4) << 8) + ((cy & 7u) << 5) + ((cx & 15u) << 1);
}

/* ---- layers */

struct layer_set {
    uint8_t *store;        /* the different tiles in the order they were made, the blank first */
    uint32_t *slot;        /* the slot offset of each entry of the store */
    uint32_t *table;       /* open addressing: store index + 1 */
    uint32_t count;        /* entries of the store */
    uint32_t next;         /* the next free slot offset (the layer's slots so far, gaps included) */
    uint32_t cap, tsize;
    unsigned bytes;
};

static uint64_t hash_bytes(const uint8_t *p, unsigned n)
{
    uint64_t h = 1469598103934665603ull;
    while (n--)
        h = (h ^ *p++) * 1099511628211ull;
    return h;
}

static int set_open(struct layer_set *l, uint32_t cells, unsigned bytes)
{
    l->bytes = bytes;
    l->cap = cells + 1u;
    for (l->tsize = 16; l->tsize < 2u * l->cap; l->tsize <<= 1)
        ;
    l->store = calloc(l->cap, bytes);
    l->slot = calloc(l->cap, sizeof *l->slot);
    l->table = calloc(l->tsize, sizeof *l->table);
    l->count = 1;          /* the blank tile (all zero) is entry 0, on slot 0 */
    l->next = 1;
    if (!l->store || !l->slot || !l->table)
        return -1;
    l->table[hash_bytes(l->store, bytes) & (l->tsize - 1u)] = 1;
    return 0;
}

static void set_close(struct layer_set *l)
{
    free(l->store);
    free(l->slot);
    free(l->table);
    memset(l, 0, sizeof *l);
}

/* The table position of a tile that is in the layer, or of the empty position where it would go. */
static uint32_t set_find(const struct layer_set *l, const uint8_t *tile)
{
    uint32_t i = (uint32_t)(hash_bytes(tile, l->bytes) & (l->tsize - 1u));
    while (l->table[i] && memcmp(l->store + (size_t)(l->table[i] - 1u) * l->bytes, tile, l->bytes))
        i = (i + 1u) & (l->tsize - 1u);
    return i;
}

static void set_add(struct layer_set *l, const uint8_t *tile, uint32_t slot, int index)
{
    memcpy(l->store + (size_t)l->count * l->bytes, tile, l->bytes);
    l->slot[l->count] = slot;
    l->count++;
    if (index)
        l->table[set_find(l, tile)] = l->count;       /* a tile that is there already is replaced */
}

/* The slot offset of a tile among the layer's different tiles, adding it on the next slot when new. */
static uint32_t set_index(struct layer_set *l, const uint8_t *tile)
{
    uint32_t i = set_find(l, tile);
    if (l->table[i])
        return l->slot[l->table[i] - 1u];
    set_add(l, tile, l->next, 1);
    return l->next++;
}

/* The disc started a new slot for a tile that already exists: a new entry that later cells do not find. */
static uint32_t set_force_new(struct layer_set *l, const uint8_t *tile)
{
    set_add(l, tile, l->next, 0);
    return l->next++;
}

/* The disc put a new tile on slot `at` beyond the next free one: the slots between stay without a tile. */
static uint32_t set_new_at(struct layer_set *l, const uint8_t *tile, uint32_t at)
{
    set_add(l, tile, at, 1);
    l->next = at + 1u;
    return at;
}

static int set_cell(struct ctx *c, unsigned set, unsigned x, unsigned y, unsigned *code, unsigned *attr)
{
    unsigned id;
    size_t at;
    if (SETS1 + 128u * (set + 1u) > c->psize)
        return fail(c, "scroll 1 set %u lies outside the program (%u)", set, 0);
    id = be16(c->prog + SETS1 + 128u * set + 2u * ((x >> 2) * 8u + (y >> 2)));
    at = GROUPS1 + 64u * (size_t)id + 4u * ((x & 3u) * 4u + (y & 3u));
    if (at + 4 > c->psize)
        return fail(c, "scroll 1 group %u lies outside the program (set %u)", id, set);
    *code = be16(c->prog + at);
    *attr = be16(c->prog + at + 2);
    return 0;
}

/* The PS1 colours of the stage's 32 layer 0 palette rows (entry 0 transparent, black the opaque 0x8000). */
static int layer0_palettes(struct ctx *c, const struct arcade_stage *s, uint16_t pal[32][16]);

/* What layer 0's cell (x, y) of set `set` is: its tile bytes (16 x 16, 8 bytes a row), the row of its first piece with an opaque
 * pixel (rows_mask: all such rows, as bits), and what the opaque pixels ask of the palette (need[j] valid where bit j of need_mask is
 * set; *conflict when two pieces ask for different colours of one index). */
struct l0_info {
    uint8_t tile[128];
    unsigned first_row, rows_mask, need_mask, conflict;
    uint16_t need[16];
};

static int l0_cell(struct ctx *c, unsigned set, unsigned x, unsigned y, uint16_t pal[32][16], struct l0_info *o)
{
    unsigned q, pc[4], pa[4], px, py;
    o->first_row = o->rows_mask = o->need_mask = o->conflict = 0;
    for (q = 0; q < 4; q++)
        if (set_cell(c, set, 2u * x + (q & 1u), 2u * y + (q >> 1), &pc[q], &pa[q]))
            return -1;
    for (q = 0; q < 4; q++) {
        unsigned used = 0, j, row = pa[q] & 31u;
        for (py = 0; py < 8; py++)
            for (px = 0; px < 8; px++) {
                unsigned v = piece_px(c, pc[q], pa[q], px, py);
                unsigned ox = (q & 1u) * 8u + px, oy = (q >> 1) * 8u + py;
                used |= 1u << v;
                if (ox & 1u)
                    o->tile[oy * 8u + ox / 2u] |= (uint8_t)(v << 4);
                else
                    o->tile[oy * 8u + ox / 2u] = (uint8_t)v;
            }
        used &= ~1u;
        if (!used)
            continue;
        if (!o->rows_mask)
            o->first_row = row;
        o->rows_mask |= 1u << row;
        for (j = 1; j < 16; j++) {
            if (!(used >> j & 1u))
                continue;
            if (o->need_mask >> j & 1u) {
                if (o->need[j] != pal[row][j])
                    o->conflict = 1;
            } else {
                o->need_mask |= 1u << j;
                o->need[j] = pal[row][j];
            }
        }
    }
    return 0;
}

/* The palette row of a layer 0 cell: the first row of `order` (rows by the number of cells with a piece in them, most first) whose
 * colours the cell asks for; else the row of its first opaque piece; a blank cell row 0. */
static unsigned l0_row(const struct l0_info *o, const unsigned char *order, unsigned norder, uint16_t pal[32][16])
{
    unsigned i, j;
    if (!o->rows_mask)
        return 0;
    if (!o->conflict) {
        for (i = 0; i < norder; i++) {
            for (j = 1; j < 16; j++)
                if ((o->need_mask >> j & 1u) && pal[order[i]][j] != o->need[j])
                    break;
            if (j == 16)
                return order[i];
        }
    }
    return o->first_row;
}

static void l0_order(const unsigned count[32], unsigned char order[32], unsigned *norder)
{
    unsigned n = 0, r, i;
    for (r = 0; r < 32; r++)
        if (count[r])
            order[n++] = (unsigned char)r;
    for (i = 1; i < n; i++) {                       /* insertion sort: most cells first, the lower row among equals */
        unsigned char v = order[i];
        unsigned k = i;
        while (k > 0 && (count[order[k - 1]] < count[v] || (count[order[k - 1]] == count[v] && order[k - 1] > v))) {
            order[k] = order[k - 1];
            k--;
        }
        order[k] = v;
    }
    *norder = n;
}

/* Build layer n from its descriptor: the cell words into `module`, the different tiles into `ls`. start: the layer's first page slot. */
static int build_layer(struct ctx *c, const struct arcade_stage *s, unsigned n, uint32_t start, struct layer_set *ls,
                       struct arcade_stage_piece *module)
{
    const struct arcade_stage_layer *l = &s->layer[n];
    unsigned side = n < 2 ? 16u : 32u, bc = n < 2 ? 16u : 8u, nba, nbd, bx, by, x, y, cell = 0, ex = 0, norder = 0;
    unsigned count[32];
    unsigned char order[32];
    uint16_t pal[32][16];
    struct l0_info info;
    uint8_t tile[512];
    if (start > 0xffffu)
        return fail(c, "layer %u starts at slot %u, beyond the pages", n, start);
    if (!l->across || !l->down || l->across % bc || l->down % bc)
        return fail(c, "layer %u has a bad size (%u)", n, l->across);
    nba = l->across / bc;
    nbd = l->down / bc;
    if (l->ids != nba * nbd || !l->id_at)
        return fail(c, "layer %u names %u block ids", n, l->ids);
    if (set_open(ls, (uint32_t)l->across * l->down, side * side / 2))
        return fail(c, "out of memory in layer %u (%u)", n, 0);
    memset(count, 0, sizeof count);
    memset(&info, 0, sizeof info);
    if (n == 0) {                        /* how many cells have a piece in each row, to order the rows */
        if (layer0_palettes(c, s, pal))
            return -1;
        for (by = 0; by < nbd; by++)
            for (bx = 0; bx < nba; bx++) {
                uint32_t a = l->id_at[by * nba + bx];
                unsigned b, r;
                if (a >= c->psize)
                    return fail(c, "layer %u id address %#x is outside the program", n, a);
                b = a ? c->prog[a] : 0u;
                if (b >= SET_COUNT)
                    return fail(c, "layer %u block %u does not exist", n, b);
                for (y = 0; y < 16; y++)
                    for (x = 0; x < 16; x++) {
                        if (l0_cell(c, b, x, y, pal, &info))
                            return -1;
                        for (r = 0; r < 32; r++)
                            count[r] += info.rows_mask >> r & 1u;
                    }
            }
        l0_order(count, order, &norder);
    }
    for (by = 0; by < nbd; by++) {
        for (bx = 0; bx < nba; bx++) {
            unsigned id_index = by * nba + bx, b;
            uint32_t a = l->id_at[id_index];
            if (a >= c->psize)
                return fail(c, "layer %u id address %#x is outside the program", n, a);
            b = a ? c->prog[a] : 0u;
            if (b >= (n == 0 ? SET_COUNT : n == 1 ? BLOCK16_COUNT : BLOCK32_COUNT))
                return fail(c, "layer %u block %u does not exist", n, b);
            for (y = 0; y < bc; y++) {
                for (x = 0; x < bc; x++, cell++) {
                    unsigned cx = bx * bc + x, cy = by * bc + y, row = 0;
                    uint32_t k, code, at;
                    if (n == 0) {
                        if (l0_cell(c, b, x, y, pal, &info))
                            return -1;
                        memcpy(tile, info.tile, 128);
                        row = l0_row(&info, order, norder, pal);
                    } else {
                        unsigned code16, attr16, fx, fy;
                        size_t base = n == 1 ? TABLE16 + 1024u * (size_t)b : TABLE32 + 256u * (size_t)b;
                        size_t cellat = base + 4u * (x * bc + y);
                        uint32_t idx;
                        if (cellat + 4 > c->psize)
                            return fail(c, "layer %u cell at %#x lies outside the program", n, (unsigned)cellat);
                        code16 = be16(c->prog + cellat);
                        attr16 = be16(c->prog + cellat + 2);
                        fx = (attr16 & 0x20u) != 0;
                        fy = (attr16 & 0x40u) != 0;
                        row = attr16 & 31u;
                        idx = n == 1 ? ((code16 + 0xa400u) & 0xffffu) + 0x10000u : ((code16 + 0x900u) & 0x3fffu) + 0x4000u;
                        ps1_tile(tile_ptr(c, idx, side * side / 2), side, (int)fx, (int)fy, tile);
                    }
                    if (ex < l->exc_count && l->exc[ex].cell == cell) {
                        uint32_t e = l->exc[ex].slot;
                        if (e == ARCADE_STAGE_FORCE_NEW)
                            k = set_force_new(ls, tile);
                        else if (e < ls->next)
                            k = e;                           /* a slot made already is shared */
                        else if (e > 0xffffu)
                            return fail(c, "layer %u cell %u is given slot offset beyond the pages", n, cell);
                        else
                            k = set_new_at(ls, tile, e);
                        ex++;
                    } else {
                        k = set_index(ls, tile);
                    }
                    if (k > 0xffffu || slot_code(start + k, side, &code))
                        return fail(c, "layer %u tile %u is beyond the codes", n, k);
                    at = l->map + 2u * map_word_index(cx, cy, l->across, side);
                    if ((size_t)at + 4 > module->size)
                        return fail(c, "layer %u map does not fit the module (%u)", n, at);
                    module->data[at] = (uint8_t)code;
                    module->data[at + 1] = (uint8_t)(code >> 8);
                    {
                        unsigned clut = 0x7800u + n + 64u * (k == 0 ? 0u : row);
                        module->data[at + 2] = (uint8_t)clut;
                        module->data[at + 3] = (uint8_t)(clut >> 8);
                    }
                    if ((l->flags & ARCADE_STAGE_MAP_EXACT) || (l->cell_known && (l->cell_known[cell >> 6] >> (cell & 63u) & 1u)))
                        memset(module->known + at, 1, 4);
                }
            }
        }
    }
    return 0;
}

/* ---- palettes */

static unsigned arcade_color_to_ps1(unsigned w)
{
    unsigned r = ((w >> 8) & 15u) * 0x11u * BRIGHT / 0x2du, g = ((w >> 4) & 15u) * 0x11u * BRIGHT / 0x2du, b = (w & 15u) * 0x11u * BRIGHT / 0x2du;
    return (r >> 3) | (g >> 3) << 5 | (b >> 3) << 10;
}

/* Palette k of a stage's palette chunk of `count` palettes: 1 and (layer, row) when it is a row of a layer table. */
static int palette_group(unsigned k, unsigned count, unsigned *layer, unsigned *row)
{
    if (k < 64u && k < count) {
        *layer = k & 1u;
        *row = k >> 1;
        return 1;
    }
    if (k >= 64u && k < 96u && count < 128u) {
        *layer = 2;
        *row = k - 64u;
        return 1;
    }
    if (k >= 64u && k < 128u && count >= 128u && !(k & 1u)) {
        *layer = 2;
        *row = (k - 64u) >> 1;
        return 1;
    }
    return 0;
}

static int row_words(struct ctx *c, uint32_t at, unsigned *w)
{
    unsigned i;
    if ((size_t)at + 32 > c->psize)
        return fail(c, "palette row at %#x lies outside the program (%u)", at, 0);
    for (i = 0; i < 16; i++)
        w[i] = be16(c->prog + at + 2u * i);
    return 0;
}

static int layer0_palettes(struct ctx *c, const struct arcade_stage *s, uint16_t pal[32][16])
{
    unsigned row, i, w[16];
    for (row = 0; row < 32; row++) {
        if (row_words(c, palette_tables[0] + 0x400u * s->stage + 0x20u * row, w))
            return -1;
        pal[row][0] = 0;
        for (i = 1; i < 16; i++) {
            unsigned col = arcade_color_to_ps1(w[i - 1u]);
            pal[row][i] = (uint16_t)(col ? col : 0x8000u);
        }
    }
    return 0;
}

static void put_palette(struct arcade_stage_piece *p, unsigned k, const unsigned *w)
{
    unsigned i;
    for (i = 0; i < 16; i++) {
        unsigned col = i == 0 ? 0u : arcade_color_to_ps1(w[i - 1u]);
        if (i && !col)
            col = 0x8000u;                           /* black is the opaque black; 0 is transparent */
        p->data[32u * k + 2u * i] = (uint8_t)col;
        p->data[32u * k + 2u * i + 1u] = (uint8_t)(col >> 8);
    }
}

static int build_palettes(struct ctx *c, const struct arcade_stage *s, struct arcade_stage_piece *p)
{
    unsigned count = p->size / 32u, k, i, layer, row, w[16], r;
    for (k = 0; k < count && k < 128u; k++) {
        if (!palette_group(k, count, &layer, &row))
            continue;
        if (row_words(c, palette_tables[layer] + 0x400u * s->stage + 0x20u * row, w))
            return -1;
        put_palette(p, k, w);
        if ((s->pal_exact[k >> 6] >> (k & 63u)) & 1u)
            memset(p->known + 32u * k, 1, 32);
    }
    for (r = 0; r < s->more_count; r++) {
        const struct arcade_stage_more *m = &s->more[r];
        for (i = 0; i < m->count; i++) {
            k = (unsigned)m->k + (unsigned)m->step * i;
            if (k >= count)
                return fail(c, "a palette run leaves the chunk (palette %u)", k, 0);
            if (m->kind == ARCADE_STAGE_MORE_FILL) {
                unsigned j;
                for (j = 0; j < 16; j++)
                    w[j] = FILL_COLOUR;
            } else if (row_words(c, m->arg + 0x20u * i, w)) {
                return -1;
            }
            put_palette(p, k, w);
            memset(p->known + 32u * k, 1, 32);
        }
    }
    return 0;
}

static int build_object_palettes(struct ctx *c, const struct arcade_stage *s, struct arcade_stage_piece *p)
{
    unsigned k, i, w[16];
    for (k = 0; k < 7; k++) {
        if (row_words(c, PALETTE_OBJECTS + 7u * 0x20u * s->stage + 0x20u * k, w))
            return -1;
        for (i = 0; i < 16; i++) {
            unsigned col = i == 0 ? 0u : arcade_color_to_ps1(w[i - 1u]);
            p->data[32u * k + 2u * i] = (uint8_t)col;
            p->data[32u * k + 2u * i + 1u] = (uint8_t)(col >> 8);
        }
    }
    memset(p->known, 1, p->size);
    return 0;
}

/* ---- tile runs */

static int build_runs(struct ctx *c, const struct arcade_stage_run *runs, unsigned count, struct arcade_stage_piece *p)
{
    uint64_t at = 0;
    unsigned r;
    uint32_t k;
    for (r = 0; r < count; r++) {
        if (at + 128ull * runs[r].count > p->size)
            return fail(c, "tile runs are longer than the chunk (%u bytes)", p->size, 0);
        if (runs[r].first == ARCADE_STAGE_DISC) {
            at += 128ull * runs[r].count;
            continue;
        }
        for (k = 0; k < runs[r].count; k++) {
            if (runs[r].first != ARCADE_STAGE_ZERO) {
                const uint8_t *t = tile_ptr(c, runs[r].first + k, 128);
                if (!t)
                    return fail(c, "tile %u lies outside the arcade graphics (%u)", runs[r].first + k, 0);
                ps1_tile(t, 16, (int)(runs[r].flip & 1u), (int)((runs[r].flip >> 1) & 1u), p->data + at);
            }
            memset(p->known + at, 1, 128);
            at += 128;
        }
    }
    if (at != p->size)
        return fail(c, "tile runs are shorter than the chunk (%u of %u bytes)", (unsigned)at, p->size);
    return 0;
}

/* ---- the pieces */

static int piece_open(struct arcade_stage_piece *p, uint32_t size)
{
    p->size = size;
    p->data = calloc(size ? size : 1u, 1);
    p->known = calloc(size ? size : 1u, 1);
    return p->data && p->known ? 0 : -1;
}

void arcade_stage_piece_free(struct arcade_stage_piece *p)
{
    free(p->data);
    free(p->known);
    memset(p, 0, sizeof *p);
}

void arcade_stage_free(struct arcade_stage_out *out)
{
    arcade_stage_piece_free(&out->module);
    arcade_stage_piece_free(&out->pages);
    arcade_stage_piece_free(&out->palettes);
    arcade_stage_piece_free(&out->object_tiles);
    arcade_stage_piece_free(&out->object_palettes);
}

const struct arcade_stage *arcade_stage_find(const char *file)
{
    unsigned i;
    for (i = 0; i < arcade_stage_count; i++)
        if (!strcmp(arcade_stages[i].file, file))
            return &arcade_stages[i];
    return NULL;
}

/* The end of the run of page slots that starts at `end` and goes on through the extras and holes that touch it one after the other. */
static uint32_t chain_end(uint32_t end, const struct arcade_stage *s)
{
    int changed = 1;
    unsigned e;
    while (changed) {
        changed = 0;
        for (e = 0; e < s->extra_count; e++)
            if (s->extras[e].slot <= end && end < (uint64_t)s->extras[e].slot + s->extras[e].count) {
                end = s->extras[e].slot + s->extras[e].count;
                changed = 1;
            }
        for (e = 0; e < s->hole_count; e++)
            if (s->holes[e].slot <= end && end < (uint64_t)s->holes[e].slot + s->holes[e].count) {
                end = s->holes[e].slot + s->holes[e].count;
                changed = 1;
            }
    }
    return end;
}

/* The first slot of 32 pixel tiles that lies wholly after 16 pixel slot `end16`: the next free 32 pixel column of the pages. */
static uint32_t l2_start_after(uint32_t end16)
{
    uint32_t page = end16 / 256u, rest = end16 % 256u;
    return page * 64u + (((rest + 15u) / 16u + 1u) / 2u) * 8u;
}

int arcade_stage_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                       const struct arcade_stage *s, struct arcade_stage_out *out, char *err, size_t errsize)
{
    struct ctx c;
    struct layer_set ls;
    unsigned n, e;
    uint32_t j, k, start = 0, start1 = 0, count1 = 0;
    c.prog = program; c.tiles = tiles; c.psize = program_size; c.tsize = tiles_size; c.err = err; c.errsize = errsize;
    memset(out, 0, sizeof *out);
    memset(&ls, 0, sizeof ls);
    if (err && errsize)
        err[0] = 0;
    if (piece_open(&out->module, s->size.module) || piece_open(&out->pages, s->size.pages) ||
        piece_open(&out->palettes, s->size.palettes) || piece_open(&out->object_tiles, s->size.object_tiles) ||
        piece_open(&out->object_palettes, OBJECT_PALETTES_BYTES)) {
        fail(&c, "out of memory (%u)", 0, 0);
        goto bad;
    }
    memset(out->pages.known, 1, out->pages.size);
    for (n = 0; n < 3; n++) {
        const struct arcade_stage_layer *l = &s->layer[n];
        unsigned side = n < 2 ? 16u : 32u;
        uint32_t cells = (uint32_t)l->across * l->down;
        start = l->start;
        if (n == 1 && (l->flags & ARCADE_STAGE_START_RULE))
            start = (chain_end(count1, s) + 15u) / 16u * 16u;                  /* count1 holds layer 0's tile count here */
        if (n == 2 && (l->flags & ARCADE_STAGE_START_RULE))
            start = l2_start_after(chain_end(start1 + count1, s));             /* start1 and count1 are layer 1's */
        if (build_layer(&c, s, n, start, &ls, &out->module))
            goto bad;
        for (j = 0; j < ls.count; j++) {
            uint32_t off = ls.slot[j];
            int64_t at = slot_place(start + off, side, out->pages.size);
            if (at < 0) {
                fail(&c, "layer %u tile %u does not fit the pages", n, off);
                goto bad;
            }
            put_slot(out->pages.data, at, side, ls.store + (size_t)j * ls.bytes);
            if (!(l->flags & ARCADE_STAGE_TILES_EXACT) &&
                !(l->tile_known && off <= cells && (l->tile_known[off >> 6] >> (off & 63u) & 1u)))
                mark_known(out->pages.known, at, side, 0);
        }
        if (n == 1)
            start1 = start;
        count1 = ls.next;
        set_close(&ls);
    }
    for (e = 0; e < s->extra_count; e++) {
        if (s->extras[e].slot > 0xffffu || s->extras[e].count > 0xffffu) {
            fail(&c, "extra run %u is beyond the pages (%u)", e, s->extras[e].slot);
            goto bad;
        }
        for (k = 0; k < s->extras[e].count; k++) {
            uint8_t tile[128];
            int64_t at = slot_place(s->extras[e].slot + k, 16, out->pages.size);
            const uint8_t *t = tile_ptr(&c, s->extras[e].first + k, 128);
            if (at < 0 || !t) {
                fail(&c, "an extra tile %u does not fit (%u)", e, k);
                goto bad;
            }
            ps1_tile(t, 16, (int)(s->extras[e].flip & 1u), (int)((s->extras[e].flip >> 1) & 1u), tile);
            put_slot(out->pages.data, at, 16, tile);
        }
    }
    for (e = 0; e < s->hole_count; e++) {
        for (k = 0; k < s->holes[e].count && s->holes[e].slot < 0x10000u; k++) {
            int64_t at = slot_place(s->holes[e].slot + k, 16, out->pages.size);
            if (at < 0)
                break;                  /* later slots lie further out */
            mark_known(out->pages.known, at, 16, 0);
        }
    }
    if (build_palettes(&c, s, &out->palettes) || build_object_palettes(&c, s, &out->object_palettes) ||
        build_runs(&c, s->objects, s->object_count, &out->object_tiles))
        goto bad;
    return 0;
bad:
    set_close(&ls);
    arcade_stage_free(out);
    return -1;
}

int arcade_stage_build_runs(const uint8_t *tiles, size_t tiles_size, const struct arcade_stage_run *runs, unsigned count,
                            uint32_t total, struct arcade_stage_piece *out, char *err, size_t errsize)
{
    struct ctx c;
    c.prog = NULL; c.tiles = tiles; c.psize = 0; c.tsize = tiles_size; c.err = err; c.errsize = errsize;
    memset(out, 0, sizeof *out);
    if (piece_open(out, total)) {
        arcade_stage_piece_free(out);
        return fail(&c, "out of memory (%u)", 0, 0);
    }
    if (build_runs(&c, runs, count, out)) {
        arcade_stage_piece_free(out);
        return -1;
    }
    return 0;
}

int arcade_stage_build_shared(const uint8_t *tiles, size_t tiles_size, struct arcade_stage_piece *out, char *err, size_t errsize)
{
    uint64_t total = 0;
    unsigned r;
    for (r = 0; r < arcade_stage_shared_count; r++)
        total += 128ull * arcade_stage_shared[r].count;
    return arcade_stage_build_runs(tiles, tiles_size, arcade_stage_shared, arcade_stage_shared_count, (uint32_t)total, out, err, errsize);
}
