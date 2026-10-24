/* The screens' tiles, palettes and pictures built from the arcade program and graphics; see screens.h. The specification is
 * screens.py (model_chunk and what it calls). Project-authored, plain C99. No byte of either game is here. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compose.h"
#include "screens.h"
#include "tilepack.h"

#define TILE_BYTES 128u
#define PIECE_BYTES 32u
#define PAL_BYTES 32u

struct ctx {
    const uint8_t *prog, *tiles;
    size_t psize, tsize;
    char *err;
    size_t errsize;
    struct arcade_canvas canvas;     /* the frame of the source last cut from (windows of one source come together) */
    int canvas_source;               /* its source's index, or -1 */
};

static int fail(struct ctx *c, const char *what, unsigned a, unsigned b)
{
    if (c->err && c->errsize && !c->err[0])
        snprintf(c->err, c->errsize, what, a, b);
    return -1;
}

/* ---- pixels */

static const uint8_t *tile_ptr(const struct ctx *c, uint32_t code)
{
    uint64_t at = (uint64_t)code * TILE_BYTES;
    return at + TILE_BYTES <= c->tsize ? c->tiles + at : NULL;
}

/* The arcade value of pixel (x, y) of a 16 x 16 tile: four planes, 8 pixels to a byte, 4 bytes to 8 pixels, 8 bytes a row. */
static unsigned arcade_px(const uint8_t *t, unsigned x, unsigned y)
{
    const uint8_t *p = t + y * 8u + (x >> 3) * 4u;
    unsigned sh = 7u - (x & 7u);
    return ((p[0] >> sh) & 1u) | (((p[1] >> sh) & 1u) << 1) | (((p[2] >> sh) & 1u) << 2) | (((p[3] >> sh) & 1u) << 3);
}

/* The PS1 pixel (colour one higher) of pixel (x, y) of the arcade tile t, flipped across (flip bit 0) and/or down (bit 1). */
static unsigned ps1_px(const uint8_t *t, unsigned x, unsigned y, unsigned flip, unsigned side)
{
    return (arcade_px(t, (flip & 1u) ? side - 1u - x : x, (flip & 2u) ? side - 1u - y : y) + 1u) & 15u;
}

static int tile_ps1(const struct ctx *c, uint32_t code, unsigned flip, uint8_t out[TILE_BYTES])
{
    const uint8_t *t = tile_ptr(c, code);
    unsigned x, y;
    if (!t)
        return -1;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x += 2)
            out[y * 8u + x / 2u] = (uint8_t)(ps1_px(t, x, y, flip, 16) | ps1_px(t, x + 1u, y, flip, 16) << 4);
    return 0;
}

/* Scroll 1 piece k with a flip: the left half of arcade tile k >> 1, rows 0 to 7 (k even) or 8 to 15 (k odd). 32 bytes, 8 rows of 4. */
static int piece_ps1(const struct ctx *c, uint32_t k, unsigned flip, uint8_t out[PIECE_BYTES])
{
    const uint8_t *t = tile_ptr(c, k >> 1);
    unsigned x, y, base = (k & 1u) * 8u;
    if (!t)
        return -1;
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x += 2) {
            unsigned sx0 = (flip & 1u) ? 7u - x : x, sx1 = (flip & 1u) ? 6u - x : x + 1u, sy = base + ((flip & 2u) ? 7u - y : y);
            out[y * 4u + x / 2u] = (uint8_t)(((arcade_px(t, sx0, sy) + 1u) & 15u) | ((arcade_px(t, sx1, sy) + 1u) & 15u) << 4);
        }
    return 0;
}

/* A piece by id (k << 2 | flip), or a flat piece. */
static int piece_id(const struct ctx *c, uint32_t id, uint8_t out[PIECE_BYTES])
{
    if (id >= ARCADE_SCREENS_FLAT) {
        memset(out, (int)((id & 15u) * 17u), PIECE_BYTES);
        return 0;
    }
    return piece_ps1(c, id >> 2, id & 3u, out);
}

static void tile_of_pieces(const uint8_t p[4][PIECE_BYTES], uint8_t out[TILE_BYTES])
{
    unsigned y;
    for (y = 0; y < 16; y++) {
        memcpy(out + y * 8u, p[(y >> 3) * 2u] + (y & 7u) * 4u, 4);
        memcpy(out + y * 8u + 4u, p[(y >> 3) * 2u + 1u] + (y & 7u) * 4u, 4);
    }
}

static unsigned be16(const uint8_t *p)
{
    return (unsigned)p[0] << 8 | p[1];
}

/* The 32 bytes of the PS1 palette of the arcade palette at address a (15 words): colour 0 is 0; each 12-bit colour's channels doubled. */
static int palette_ps1(struct ctx *c, uint32_t a, unsigned flags, uint8_t out[PAL_BYTES])
{
    unsigned i;
    if ((uint64_t)a + 30u > c->psize)
        return fail(c, "palette at %#x lies outside the program (%u)", a, 0);
    out[0] = out[1] = 0;
    for (i = 0; i < 15; i++) {
        unsigned w = be16(c->prog + a + 2u * i), col = ((w >> 8) & 15u) << 1 | ((w >> 4) & 15u) << 6 | (w & 15u) << 11;
        if (!col && !(flags & ARCADE_SCREENS_BLACK_ZERO))
            col = 0x8000u;
        if (flags & ARCADE_SCREENS_BIT15)
            col |= 0x8000u;
        out[2u * (i + 1u)] = (uint8_t)col;
        out[2u * (i + 1u) + 1u] = (uint8_t)(col >> 8);
    }
    return 0;
}

/* ---- regions */

struct tables {
    const struct arcade_screens_item *items;
    const struct arcade_screens_op *ops;
    const uint32_t (*pieces)[4];
    unsigned piece_count;
    const struct arcade_screens_source *sources;
    const struct arcade_screens_window *windows;
    unsigned source_count, window_count;
};

static int in_chunk(const struct arcade_screens_out *o, const struct arcade_screens_region *g)
{
    return (uint64_t)g->offset + g->size <= o->size;
}

/* The by-design mark is for ZERO, run and WINDOW items of a tile list and for ZERO items of a palette list: on any other item the build is refused. */
static int design_item(struct ctx *c, const struct arcade_screens_item *it, int palettes)
{
    if (!(it->flip & ARCADE_SCREENS_BY_DESIGN))
        return 0;
    if (it->first == ARCADE_SCREENS_DISC || it->first == ARCADE_SCREENS_PIECES || (palettes && it->first != ARCADE_SCREENS_ZERO))
        return fail(c, "the by-design mark is on an item that is not blank or an arcade tile (%#x)", it->first, 0);
    return 0;
}

/* The PS1 tile (128 bytes, 8 a row, the left pixel of a pair in the low nibble) flipped across (bit 0) and/or down (bit 1). */
static void flip_bytes(const uint8_t *in, unsigned flip, uint8_t out[TILE_BYTES])
{
    unsigned x, y;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            unsigned sx = (flip & 1u) ? 15u - x : x, sy = (flip & 2u) ? 15u - y : y;
            unsigned v = (in[sy * 8u + sx / 2u] >> ((sx & 1u) * 4u)) & 15u;
            if (x & 1u)
                out[y * 8u + x / 2u] |= (uint8_t)(v << 4);
            else
                out[y * 8u + x / 2u] = (uint8_t)v;
        }
}

/* The tile of window `w` (an index): the source drawn once and kept, the 16 x 16 cut, the stored flip. */
static int window_tile(struct ctx *c, const struct tables *t, uint32_t w, uint8_t out[TILE_BYTES])
{
    const struct arcade_screens_window *win;
    const struct arcade_screens_source *src;
    uint8_t cut[TILE_BYTES];
    char e[160];
    if (w >= t->window_count)
        return fail(c, "window %u is not in the table (%u)", w, 0);
    win = &t->windows[w];
    if (win->source >= t->source_count)
        return fail(c, "source %u of a window is not in the table (%u)", win->source, 0);
    if (win->rows > 2u)
        return fail(c, "window %u keeps rows %u", w, win->rows);
    src = &t->sources[win->source];
    if (c->canvas_source != (int)win->source) {
        int rc;
        arcade_compose_free(&c->canvas);
        c->canvas_source = -1;
        e[0] = 0;
        if (src->kind == ARCADE_SCREENS_SRC_RECORD)
            rc = arcade_compose_record_canvas(c->prog, c->psize, c->tiles, c->tsize, src->address, src->bank, &c->canvas, e, sizeof e);
        else if (src->kind == ARCADE_SCREENS_SRC_SCROLL2)
            rc = arcade_compose_scroll2(c->prog, c->psize, c->tiles, c->tsize, src->address, src->blocks, &c->canvas, e, sizeof e);
        else
            return fail(c, "source %u has kind %u", win->source, src->kind);
        if (rc) {
            if (c->err && c->errsize && !c->err[0])
                snprintf(c->err, c->errsize, "source %u: %s", win->source, e);
            return -1;
        }
        c->canvas_source = (int)win->source;
    }
    arcade_compose_cut(&c->canvas, win->x, win->y, win->rows, cut);
    flip_bytes(cut, win->flip & 3u, out);
    return 0;
}

/* The tiles of a list of items, in `tiles` (count of them) with `have` = 1 for the ones built (DISC ones are left zero and 0) and `design` = 1
 * for the ones blank by design. */
static int tile_list(struct ctx *c, const struct tables *t, const struct arcade_screens_region *g, uint8_t (*tiles)[TILE_BYTES], uint8_t *have, uint8_t *design,
                     uint32_t total)
{
    uint32_t n = 0, i, j;
    for (i = 0; i < g->count; i++) {
        const struct arcade_screens_item *it = &t->items[g->first + i];
        int by_design = (it->flip & ARCADE_SCREENS_BY_DESIGN) != 0;
        if (design_item(c, it, 0))
            return -1;
        if (it->first == ARCADE_SCREENS_DISC || it->first == ARCADE_SCREENS_ZERO) {
            for (j = 0; j < it->count; j++, n++) {
                have[n] = it->first == ARCADE_SCREENS_ZERO;
                design[n] = it->first == ARCADE_SCREENS_ZERO && by_design;
                memset(tiles[n], 0, TILE_BYTES);
            }
        } else if (it->first == ARCADE_SCREENS_PIECES) {
            uint8_t p[4][PIECE_BYTES];
            unsigned q;
            if (it->count != 1 || it->flip >= t->piece_count)
                return fail(c, "tile of pieces %u is not in the table (%u)", it->flip, 0);
            for (q = 0; q < 4; q++)
                if (piece_id(c, t->pieces[it->flip][q], p[q]))
                    return fail(c, "piece %#x lies outside the graphics (%u)", t->pieces[it->flip][q], 0);
            tile_of_pieces((const uint8_t (*)[PIECE_BYTES])p, tiles[n]);
            have[n++] = 1;
        } else if (it->first == ARCADE_SCREENS_WINDOW) {
            if (it->count != 1 || n >= total)
                return fail(c, "a window item holds %u tiles (%u)", it->count, 0);
            if (window_tile(c, t, it->flip & ~ARCADE_SCREENS_BY_DESIGN, tiles[n]))
                return -1;
            design[n] = (uint8_t)by_design;
            have[n++] = 1;
        } else {
            for (j = 0; j < it->count; j++, n++) {
                if (n >= total || tile_ps1(c, it->first + j, it->flip & 3u, tiles[n]))
                    return fail(c, "tile %#x lies outside the graphics (%u)", it->first + j, 0);
                design[n] = (uint8_t)by_design;
                have[n] = 1;
            }
        }
    }
    return 0;
}

static uint32_t tile_total(const struct tables *t, const struct arcade_screens_region *g)
{
    uint64_t n = 0;
    uint32_t i;
    for (i = 0; i < g->count; i++)
        n += t->items[g->first + i].count;
    return n > 0x100000u ? 0x100000u : (uint32_t)n;
}

static int build_segment(struct ctx *c, const struct tables *t, const struct arcade_screens_region *g, const uint8_t *disc, size_t disc_size,
                         struct arcade_screens_out *out)
{
    uint32_t total = tile_total(t, g), i;
    uint8_t (*tiles)[TILE_BYTES] = NULL;
    uint8_t *have = NULL, *design = NULL;
    int complete = 1, rc = -1;
    char perr[96];
    uint32_t packed = 0;
    if (total == 0 || total >= 0x100000u)
        return fail(c, "a segment holds %u tiles (%u)", total, 0);
    tiles = (uint8_t (*)[TILE_BYTES])calloc(total, TILE_BYTES);
    have = (uint8_t *)calloc(total, 1);
    design = (uint8_t *)calloc(total, 1);
    if (!tiles || !have || !design) {
        fail(c, "out of memory (%u)", 0, 0);
        goto done;
    }
    if (tile_list(c, t, g, tiles, have, design, total))
        goto done;
    for (i = 0; i < total; i++)
        out->design += design[i];
    for (i = 0; i < total; i++) {
        if (have[i])
            continue;
        if (disc && (uint64_t)g->offset + g->size <= disc_size && arcade_tilepack_count(disc + g->offset, g->size) == total &&
            arcade_tilepack_decode(disc + g->offset, g->size, i, tiles[i], perr, sizeof perr) == 0)
            continue;
        complete = 0;
        break;
    }
    if (!complete) {
        out->unbuilt++;
        rc = 0;
        goto done;
    }
    if (arcade_tilepack((const uint8_t (*)[128])tiles, total, out->data + g->offset, g->size, &packed, perr, sizeof perr) != 0) {
        out->unbuilt++;
        rc = 0;
        goto done;
    }
    memset(out->known + g->offset, 2, g->size);
    rc = 0;
done:
    free(tiles);
    free(have);
    free(design);
    return rc;
}

static int build_raw(struct ctx *c, const struct tables *t, const struct arcade_screens_region *g, struct arcade_screens_out *out)
{
    uint32_t total = tile_total(t, g), i;
    uint8_t (*tiles)[TILE_BYTES] = NULL;
    uint8_t *have = NULL, *design = NULL;
    int rc = -1;
    if ((uint64_t)total * TILE_BYTES > g->size)
        return fail(c, "raw tiles are longer than their region (%u)", g->size, 0);
    if (total == 0)
        return 0;
    tiles = (uint8_t (*)[TILE_BYTES])calloc(total, TILE_BYTES);
    have = (uint8_t *)calloc(total, 1);
    design = (uint8_t *)calloc(total, 1);
    if (!tiles || !have || !design) {
        fail(c, "out of memory (%u)", 0, 0);
        goto done;
    }
    if (tile_list(c, t, g, tiles, have, design, total))
        goto done;
    for (i = 0; i < total; i++)
        if (have[i]) {
            memcpy(out->data + g->offset + (size_t)i * TILE_BYTES, tiles[i], TILE_BYTES);
            memset(out->known + g->offset + (size_t)i * TILE_BYTES, design[i] ? 2 : 1, TILE_BYTES);
            out->design += design[i];
        }
    rc = 0;
done:
    free(tiles);
    free(have);
    free(design);
    return rc;
}

static int build_palettes(struct ctx *c, const struct tables *t, const struct arcade_screens_region *g, struct arcade_screens_out *out)
{
    uint32_t i, j, n = 0, stride = g->width ? g->width : 1u;
    uint8_t *base = out->data + g->offset;
    memset(out->known + g->offset, 1, g->size);
    for (i = 0; i < g->count; i++) {
        const struct arcade_screens_item *it = &t->items[g->first + i];
        if (design_item(c, it, 1))
            return -1;
        for (j = 0; j < it->count; j++, n++) {
            uint64_t at = (uint64_t)n * stride * PAL_BYTES;
            if (at + PAL_BYTES > g->size)
                return fail(c, "palettes are longer than their region (%u)", g->size, 0);
            if (it->first == ARCADE_SCREENS_DISC) {
                memset(out->known + g->offset + at, 0, PAL_BYTES);
            } else if (it->first == ARCADE_SCREENS_ZERO) {
                if (it->flip & ARCADE_SCREENS_BY_DESIGN) {
                    memset(out->known + g->offset + at, 2, PAL_BYTES);
                    out->design++;
                }
            } else {
                if (palette_ps1(c, it->first + PAL_BYTES * j, it->flip, base + at))
                    return -1;
            }
        }
    }
    return 0;
}

static void blit(uint8_t *buf, size_t row, unsigned x_px, unsigned y_px, const uint8_t *src, unsigned side)
{
    unsigned y;
    for (y = 0; y < side; y++)
        memcpy(buf + (size_t)(y_px + y) * row + x_px / 2u, src + (size_t)y * (side / 2u), side / 2u);
}

static int build_picture(struct ctx *c, const struct tables *t, const struct arcade_screens_region *g, struct arcade_screens_out *out)
{
    size_t row;
    uint32_t rows, i;
    unsigned x, y;
    uint8_t *base = out->data + g->offset;
    if (!g->width || g->width % 16u || g->size % (g->width / 2u))
        return fail(c, "picture width %u does not fit %u bytes", g->width, g->size);
    row = g->width / 2u;
    rows = (uint32_t)(g->size / row);
    memset(out->known + g->offset, 1, g->size);
    for (i = 0; i < g->count; i++) {
        const struct arcade_screens_op *o = &t->ops[g->first + i];
        unsigned unit = o->kind == ARCADE_SCREENS_OP_TILE ? 16u : 8u;
        if (o->kind < ARCADE_SCREENS_OP_TILE || o->kind > ARCADE_SCREENS_OP_BLANK)
            return fail(c, "picture op kind %u is unknown (%u)", o->kind, 0);
        if ((uint64_t)o->x * 8u + (uint64_t)unit * o->w > g->width || (uint64_t)o->y * 8u + (uint64_t)unit * o->h > rows)
            return fail(c, "picture op %u lies outside the picture (%u)", i, 0);
        if (o->kind == ARCADE_SCREENS_OP_DISC || o->kind == ARCADE_SCREENS_OP_BLANK) {
            int by_design = o->kind == ARCADE_SCREENS_OP_BLANK;
            for (y = 0; y < (unsigned)o->h * 8u; y++) {
                size_t at = ((size_t)o->y * 8u + y) * row + (size_t)o->x * 4u;
                memset(base + at, 0, (size_t)o->w * 4u);
                memset(out->known + g->offset + at, by_design ? 2 : 0, (size_t)o->w * 4u);
            }
            if (by_design)
                out->design += (unsigned)o->w * o->h;
            continue;
        }
        for (y = 0; y < o->h; y++)
            for (x = 0; x < o->w; x++) {
                uint32_t code = o->first + (uint32_t)((int64_t)x * o->stepx + (int64_t)y * o->stepy);
                if (o->kind == ARCADE_SCREENS_OP_TILE) {
                    uint8_t tile[TILE_BYTES];
                    if (tile_ps1(c, code, o->flip & 3u, tile))
                        return fail(c, "tile %#x lies outside the graphics (%u)", code, 0);
                    blit(base, row, o->x * 8u + 16u * x, o->y * 8u + 16u * y, tile, 16);
                } else {
                    uint8_t piece[PIECE_BYTES];
                    if (o->kind == ARCADE_SCREENS_OP_FLAT)
                        memset(piece, (int)((o->first & 15u) * 17u), PIECE_BYTES);
                    else if (piece_ps1(c, code, o->flip & 3u, piece))
                        return fail(c, "piece %#x lies outside the graphics (%u)", code, 0);
                    blit(base, row, o->x * 8u + 8u * x, o->y * 8u + 8u * y, piece, 8);
                }
            }
    }
    return 0;
}

static int build_one(struct ctx *c, const struct tables *t, const struct arcade_screens_region *g, const uint8_t *disc, size_t disc_size,
                     struct arcade_screens_out *out)
{
    if (!in_chunk(out, g))
        return fail(c, "a region lies outside its chunk (%u)", g->offset, 0);
    switch (g->kind) {
    case ARCADE_SCREENS_SEG: return build_segment(c, t, g, disc, disc_size, out);
    case ARCADE_SCREENS_RAW: return build_raw(c, t, g, out);
    case ARCADE_SCREENS_PALS: return build_palettes(c, t, g, out);
    case ARCADE_SCREENS_PIC: return build_picture(c, t, g, out);
    default: return fail(c, "region kind %u is unknown (%u)", g->kind, 0);
    }
}

static int out_open(struct arcade_screens_out *out, uint32_t size)
{
    memset(out, 0, sizeof *out);
    out->size = size;
    out->data = (uint8_t *)calloc(size ? size : 1u, 1);
    out->known = (uint8_t *)calloc(size ? size : 1u, 1);
    return out->data && out->known ? 0 : -1;
}

void arcade_screens_free(struct arcade_screens_out *out)
{
    free(out->data);
    free(out->known);
    memset(out, 0, sizeof *out);
}

int arcade_screens_source_index(unsigned kind, uint32_t address, uint32_t bank, unsigned blocks)
{
    unsigned i;
    for (i = 0; i < arcade_screens_source_count; i++) {
        const struct arcade_screens_source *s = &arcade_screens_sources[i];
        if (s->kind == kind && s->address == address && s->bank == bank && s->blocks == blocks)
            return (int)i;
    }
    return -1;
}

const struct arcade_screens_file *arcade_screens_find(const char *file)
{
    unsigned i;
    for (i = 0; i < arcade_screens_file_count; i++)
        if (!strcmp(arcade_screens_files[i].file, file))
            return &arcade_screens_files[i];
    return NULL;
}

const struct arcade_screens_recipe *arcade_screens_recipe_of(const struct arcade_screens_file *f, unsigned table, unsigned slot, unsigned ordinal)
{
    unsigned i;
    for (i = 0; i < f->count; i++) {
        const struct arcade_screens_use *u = &arcade_screens_uses[f->first + i];
        if (u->table == table && u->slot == slot && u->ordinal == ordinal)
            return &arcade_screens_recipes[u->recipe];
    }
    return NULL;
}

void arcade_screens_head_word_regions(const struct arcade_screens_region *regions, unsigned count, struct arcade_screens_out *out, uint32_t word)
{
    unsigned i, k;
    if (out->size < 4)
        return;
    for (i = 0; i < count; i++) {   /* a packed segment that starts the chunk keeps the offset table's first word, which is the disc's: if it is not the header's the segment is the disc's */
        const struct arcade_screens_region *g = &regions[i];
        if (g->kind == ARCADE_SCREENS_SEG && g->offset == 0 && (uint64_t)g->size <= out->size && out->known[0] == 2 &&
            (out->data[0] | out->data[1] << 8 | out->data[2] << 16 | (uint32_t)out->data[3] << 24) != word) {
            memset(out->data, 0, g->size);
            memset(out->known, 0, g->size);
            out->unbuilt++;
        }
    }
    for (k = 0; k < 4; k++) {
        int in_segment = 0;
        if (out->known[k] != 2)
            continue;
        for (i = 0; i < count; i++)
            if (regions[i].kind == ARCADE_SCREENS_SEG && regions[i].offset <= k && k < (uint64_t)regions[i].offset + regions[i].size)
                in_segment = 1;
        if (in_segment)
            continue;
        out->data[k] = (uint8_t)(word >> (8 * k));
        out->known[k] = 1;
    }
}

void arcade_screens_head_word(const struct arcade_screens_recipe *r, struct arcade_screens_out *out, uint32_t word)
{
    arcade_screens_head_word_regions(arcade_screens_regions + r->first, r->count, out, word);
}

/* The tiles of the SEG region g that are by design: 1 for a blank tile (it must be blank), 2 for an arcade tile (not compared), in a zeroed array of
 * its tile count; NULL when no items or no memory. */
static uint8_t *design_tiles(const struct arcade_screens_region *g, const struct arcade_screens_item *items, uint32_t n)
{
    uint8_t *mark;
    uint32_t i, j, at = 0;
    if (!items || !(mark = (uint8_t *)calloc(n, 1)))
        return NULL;
    for (i = 0; i < g->count; i++) {
        const struct arcade_screens_item *it = &items[g->first + i];
        for (j = 0; j < it->count && at < n; j++, at++)
            mark[at] = (it->flip & ARCADE_SCREENS_BY_DESIGN) ? (it->first == ARCADE_SCREENS_ZERO ? 1 : 2) : 0;
    }
    return mark;
}

int arcade_screens_verify_regions(const struct arcade_screens_region *regions, unsigned count, const struct arcade_screens_item *items,
                                  const struct arcade_screens_out *out, const uint8_t *disc, size_t disc_size, char *err, size_t errsize)
{
    uint32_t i, k;
    if (err && errsize)
        err[0] = 0;
    if (!disc || disc_size < out->size) {
        if (err && errsize)
            snprintf(err, errsize, "the disc's chunk is shorter than the recipe's (%u of %u bytes)", (unsigned)disc_size, out->size);
        return -1;
    }
    for (i = 0; i < out->size; i++)
        if (out->known[i] == 1 && out->data[i] != disc[i]) {
            if (err && errsize)
                snprintf(err, errsize, "byte %#x differs from the disc's", i);
            return -1;
        }
    for (i = 0; i < count; i++) {
        const struct arcade_screens_region *g = &regions[i];
        uint32_t n;
        uint8_t *mark, blank[TILE_BYTES];
        memset(blank, 0, sizeof blank);
        if (g->kind != ARCADE_SCREENS_SEG || (uint64_t)g->offset + g->size > out->size || out->known[g->offset] != 2)
            continue;
        n = arcade_tilepack_count(out->data + g->offset, g->size);
        if (!n || n != arcade_tilepack_count(disc + g->offset, g->size)) {
            if (err && errsize)
                snprintf(err, errsize, "segment at %#x holds another number of tiles than the disc's", g->offset);
            return -1;
        }
        mark = design_tiles(g, items, n);
        for (k = 0; k < n; k++) {
            uint8_t a[TILE_BYTES], b[TILE_BYTES];
            char e2[64];
            if (mark && mark[k] == 2)   /* an arcade tile by design: the disc's is not compared */
                continue;
            if (mark && mark[k]) {   /* blank by design: it must be blank, and the disc's is not compared */
                if (arcade_tilepack_decode(out->data + g->offset, g->size, k, a, e2, sizeof e2) || memcmp(a, blank, TILE_BYTES)) {
                    if (err && errsize)
                        snprintf(err, errsize, "tile %u of the segment at %#x is not blank as designed", k, g->offset);
                    free(mark);
                    return -1;
                }
                continue;
            }
            if (arcade_tilepack_decode(out->data + g->offset, g->size, k, a, e2, sizeof e2) || arcade_tilepack_decode(disc + g->offset, g->size, k, b, e2, sizeof e2) ||
                memcmp(a, b, TILE_BYTES)) {
                if (err && errsize)
                    snprintf(err, errsize, "tile %u of the segment at %#x differs from the disc's", k, g->offset);
                free(mark);
                return -1;
            }
        }
        free(mark);
    }
    return 0;
}

int arcade_screens_verify(const struct arcade_screens_recipe *r, const struct arcade_screens_out *out, const uint8_t *disc, size_t disc_size,
                          char *err, size_t errsize)
{
    if (r->size != out->size) {
        if (err && errsize)
            snprintf(err, errsize, "the built chunk is not the recipe's (%u of %u bytes)", out->size, r->size);
        return -1;
    }
    return arcade_screens_verify_regions(arcade_screens_regions + r->first, r->count, arcade_screens_items, out, disc, disc_size, err, errsize);
}

int arcade_screens_build_region(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                                const struct arcade_screens_region *g, const struct arcade_screens_item *items,
                                const struct arcade_screens_op *ops, const uint32_t (*pieces)[4], unsigned piece_count,
                                const struct arcade_screens_source *sources, unsigned source_count, const struct arcade_screens_window *windows, unsigned window_count,
                                const uint8_t *disc, size_t disc_size, struct arcade_screens_out *out, char *err, size_t errsize)
{
    struct ctx c;
    struct tables t;
    memset(&c, 0, sizeof c);
    c.prog = program; c.tiles = tiles; c.psize = program_size; c.tsize = tiles_size; c.err = err; c.errsize = errsize; c.canvas_source = -1;
    t.items = items; t.ops = ops; t.pieces = pieces; t.piece_count = piece_count;
    t.sources = sources; t.source_count = source_count; t.windows = windows; t.window_count = window_count;
    if (err && errsize)
        err[0] = 0;
    if (out_open(out, g->offset + g->size) || build_one(&c, &t, g, disc, disc_size, out)) {
        if (out->data && err && errsize && !err[0])
            snprintf(err, errsize, "out of memory");
        arcade_compose_free(&c.canvas);
        arcade_screens_free(out);
        return -1;
    }
    arcade_compose_free(&c.canvas);
    return 0;
}

int arcade_screens_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                         const struct arcade_screens_recipe *r, const uint8_t *disc, size_t disc_size,
                         struct arcade_screens_out *out, char *err, size_t errsize)
{
    struct ctx c;
    struct tables t;
    unsigned i;
    memset(&c, 0, sizeof c);
    c.prog = program; c.tiles = tiles; c.psize = program_size; c.tsize = tiles_size; c.err = err; c.errsize = errsize; c.canvas_source = -1;
    t.items = arcade_screens_items; t.ops = arcade_screens_ops; t.pieces = arcade_screens_pieces; t.piece_count = arcade_screens_piece_count;
    t.sources = arcade_screens_sources; t.source_count = arcade_screens_source_count; t.windows = arcade_screens_windows; t.window_count = arcade_screens_window_count;
    if (err && errsize)
        err[0] = 0;
    if (out_open(out, r->size)) {
        fail(&c, "out of memory (%u)", 0, 0);
        arcade_screens_free(out);
        return -1;
    }
    for (i = 0; i < r->count; i++)
        if (build_one(&c, &t, &arcade_screens_regions[r->first + i], disc, disc_size, out)) {
            arcade_compose_free(&c.canvas);
            arcade_screens_free(out);
            return -1;
        }
    arcade_compose_free(&c.canvas);
    return 0;
}
