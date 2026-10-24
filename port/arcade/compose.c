/* The arcade's art records and scroll 2 blocks as cells, drawn into canvases; see compose.h. The model is compose.py. Project-authored,
 * plain C99. No byte of either game is here. */
#include "compose.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TILE_BYTES 128u

struct reader {
    const uint8_t *prog;
    size_t psize;
    int bad;
};

static void fail(char *err, size_t errsize, const char *fmt, ...)
{
    if (err && errsize && !err[0]) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(err, errsize, fmt, ap);
        va_end(ap);
    }
}

static unsigned rd16(struct reader *r, size_t o)
{
    if (o > r->psize || r->psize - o < 2) {
        r->bad = 1;
        return 0;
    }
    return (unsigned)r->prog[o] << 8 | r->prog[o + 1];
}

static uint32_t rd32(struct reader *r, size_t o)
{
    return (uint32_t)rd16(r, o) << 16 | rd16(r, o + 2);
}

int arcade_compose_record(const uint8_t *prog, size_t psize, uint32_t a, uint32_t bank, unsigned pal_split, struct arcade_compose_cell *cells,
                          unsigned max, struct arcade_compose_info *info, char *err, size_t errsize)
{
    struct reader rd;
    unsigned mode, declared, nb, k, n = 0, total = 0, stride = 0, part = 0, gh = 0, split;
    unsigned prev_attr = 0;
    int32_t prev_x = 0, idx = 0;
    uint32_t ptr = 0, first = 0, shared = 0;

    rd.prog = prog;
    rd.psize = psize;
    rd.bad = 0;
    if (err && errsize) err[0] = 0;
    if ((a & 1) || a > psize || psize - a < 8) {
        fail(err, errsize, "record %#x: outside the program", (unsigned)a);
        return -1;
    }
    mode = rd16(&rd, a);
    if (mode != 0 && mode != 2 && mode != 4 && mode != 10) {
        fail(err, errsize, "record %#x: not an art record (mode %u)", (unsigned)a, mode);
        return -1;
    }
    if (mode == 10) {
        gh = rd16(&rd, (size_t)a + 2) + 1u;
        nb = rd16(&rd, (size_t)a + 4) + 1u;
        if (nb > 64 || gh > 64 || nb * gh > max) {
            fail(err, errsize, "record %#x: a grid of %u x %u", (unsigned)a, nb, gh);
            return -1;
        }
        declared = nb * gh;
        nb = declared;
        stride = 4;
        first = a + 6;
    } else if (mode == 4) {
        declared = rd16(&rd, (size_t)a + 2);
        nb = rd16(&rd, (size_t)a + 4) + 1u;
        stride = 8;
        first = a + 6;
    } else {
        declared = rd16(&rd, (size_t)a + 2);
        idx = (int16_t)rd16(&rd, (size_t)a + 6);
        if (mode == 2) {
            nb = rd16(&rd, (size_t)a + 4) + 1u;
            stride = 4;
        } else {
            shared = rd16(&rd, (size_t)a + 4) & 0x7fu;
            nb = declared;
            stride = 2;
        }
        first = a + 8;
    }
    if (declared < 1 || declared > max) {
        fail(err, errsize, "record %#x: %u cells", (unsigned)a, declared);
        return -1;
    }
    if ((size_t)first + (size_t)stride * nb > psize) {
        fail(err, errsize, "record %#x: blocks outside the program", (unsigned)a);
        return -1;
    }
    if (mode == 0 || mode == 2) {
        int64_t coord = (int64_t)ARCADE_COMPOSE_COORD_TABLE + idx;
        if (coord < 0 || (uint64_t)coord + 4 > psize) {
            fail(err, errsize, "record %#x: coordinate index %d outside the program", (unsigned)a, (int)idx);
            return -1;
        }
        ptr = rd32(&rd, (size_t)coord);
        if ((ptr & 1) || (uint64_t)ptr + 4u * nb > psize) {
            fail(err, errsize, "record %#x: coordinates at %#x outside the program", (unsigned)a, (unsigned)ptr);
            return -1;
        }
    }
    split = pal_split && (mode == 0 || mode == 2);
    for (k = 0; k < nb; k++) {
        uint32_t code;
        unsigned attr, w, h, i, j;
        int32_t x, y;
        if (mode == 2) {
            code = rd16(&rd, (size_t)first + 4 * (size_t)k);
            attr = rd16(&rd, (size_t)first + 4 * (size_t)k + 2);
        } else if (mode == 0) {
            code = rd16(&rd, (size_t)first + 2 * (size_t)k);
            attr = shared;
        } else {
            code = (rd16(&rd, (size_t)first + (size_t)stride * k) + ARCADE_COMPOSE_SCROLL_ADD) & 0xffffu;
            attr = rd16(&rd, (size_t)first + (size_t)stride * k + 2);
        }
        if (mode == 4) {
            x = (int16_t)rd16(&rd, (size_t)first + 8 * (size_t)k + 4);
            y = (int16_t)rd16(&rd, (size_t)first + 8 * (size_t)k + 6);
        } else if (mode == 10) {
            if (attr & 0xff80u) {
                fail(err, errsize, "record %#x: a block of a grid with a size", (unsigned)a);
                return -1;
            }
            x = 16 * (int32_t)(k / gh);
            y = 16 * (int32_t)(k % gh) - 16 * (int32_t)gh;
        } else {
            x = (int16_t)rd16(&rd, (size_t)ptr + 4 * (size_t)k);
            y = (int16_t)rd16(&rd, (size_t)ptr + 4 * (size_t)k + 2);
        }
        w = ((attr >> 8) & 15u) + 1u;
        h = ((attr >> 12) & 15u) + 1u;
        if (total + w * h > declared) {
            fail(err, errsize, "record %#x: blocks add up to more than its %u cells", (unsigned)a, declared);
            return -1;
        }
        if (k && (((x - prev_x) & 15) || (split && ((attr ^ prev_attr) & 0x1fu)))) part++;
        for (j = 0; j < h; j++)
            for (i = 0; i < w; i++) {
                unsigned ci = (attr & 0x20u) ? w - 1 - i : i, cj = (attr & 0x40u) ? h - 1 - j : j;
                struct arcade_compose_cell *c = &cells[n++];
                total++;
                c->code = code + ci + 16u * cj + bank;
                c->x = x + 16 * (int32_t)i;
                c->y = y + 16 * (int32_t)j;
                c->flags = (uint16_t)(attr & 0x7fu);
                c->part = (uint16_t)part;
            }
        prev_x = x;
        prev_attr = attr;
    }
    if (rd.bad) {
        fail(err, errsize, "record %#x: read outside the program", (unsigned)a);
        return -1;
    }
    if (total != declared) {
        fail(err, errsize, "record %#x: blocks add up to %u of its %u cells", (unsigned)a, total, declared);
        return -1;
    }
    if (info) {
        info->mode = mode;
        info->declared = declared;
        info->count = n;
        info->end = (uint32_t)(first + stride * nb);
    }
    return 0;
}

int arcade_compose_scroll2_cell(const uint8_t *prog, size_t psize, unsigned block, unsigned c, unsigned r, uint32_t *tile, uint16_t *attr)
{
    struct reader rd;
    size_t at;
    unsigned code;
    if (block >= ARCADE_COMPOSE_SCROLL_BLOCKS || c > 15 || r > 15) return -1;
    rd.prog = prog;
    rd.psize = psize;
    rd.bad = 0;
    at = ARCADE_COMPOSE_SCROLL_FIRST + (size_t)ARCADE_COMPOSE_SCROLL_BLOCK * block + 64u * c + 4u * r;
    code = rd16(&rd, at);
    if (attr) *attr = (uint16_t)rd16(&rd, at + 2);
    if (rd.bad) return -1;
    if (tile) *tile = ((code + ARCADE_COMPOSE_SCROLL_ADD) & 0xffffu) + 0x10000u;
    return 0;
}

int arcade_compose_scroll1_cell(const uint8_t *prog, size_t psize, unsigned set, unsigned x, unsigned y, unsigned *code, unsigned *attr)
{
    struct reader rd;
    size_t at, g;
    unsigned id;
    if (x > 31 || y > 31)
        return -1;
    rd.prog = prog;
    rd.psize = psize;
    rd.bad = 0;
    at = ARCADE_COMPOSE_SETS1 + 128u * (size_t)set + ((x / 4) * 8u + y / 4) * 2u;
    id = rd16(&rd, at);
    g = ARCADE_COMPOSE_GROUPS1 + 64u * (size_t)id + 4u * ((x % 4) * 4u + y % 4);
    *code = rd16(&rd, g);
    *attr = rd16(&rd, g + 2);
    return rd.bad ? -1 : 0;
}

int arcade_compose_piece(const uint8_t *tiles, size_t tsize, unsigned code, unsigned attr, uint8_t out[32])
{
    uint8_t px[256], rows[8][8];
    unsigned tile = 0x10000u + (code >> 1), x, y;
    if (((uint64_t)tile + 1) * TILE_BYTES > tsize)
        return -1;
    arcade_compose_tile_pixels(tiles + (size_t)tile * TILE_BYTES, px);
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++)
            rows[(attr & 0x40u) ? 7u - y : y][(attr & 0x20u) ? 7u - x : x] = px[(y + (code & 1u) * 8u) * 16u + x];
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x += 2)
            out[y * 4u + x / 2] = (uint8_t)(rows[y][x] | rows[y][x + 1] << 4);
    return 0;
}

void arcade_compose_tile_pixels(const uint8_t *t, uint8_t px[256])
{
    unsigned x, y;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            const uint8_t *p = t + y * 8u + (x >> 3) * 4u;
            unsigned sh = 7u - (x & 7u), v = ((p[0] >> sh) & 1u) | (((p[1] >> sh) & 1u) << 1) | (((p[2] >> sh) & 1u) << 2) | (((p[3] >> sh) & 1u) << 3);
            px[y * 16u + x] = (uint8_t)((v + 1u) & 15u);
        }
}

int arcade_compose_tile(const uint8_t *tiles, size_t tsize, uint32_t code, unsigned flip, uint8_t px[256])
{
    uint8_t raw[256];
    unsigned x, y;
    uint64_t at = (uint64_t)code * TILE_BYTES;
    if (at + TILE_BYTES > tsize) return -1;
    arcade_compose_tile_pixels(tiles + at, raw);
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) px[y * 16u + x] = raw[((flip & 2u) ? 15u - y : y) * 16u + ((flip & 1u) ? 15u - x : x)];
    return 0;
}

int arcade_compose_draw(const uint8_t *tiles, size_t tsize, const struct arcade_compose_cell *cells, unsigned count, struct arcade_canvas *out,
                        char *err, size_t errsize)
{
    int32_t x0, y0, x1, y1;
    unsigned i, x, y;
    memset(out, 0, sizeof *out);
    if (err && errsize) err[0] = 0;
    if (!count) return 0;
    x0 = x1 = cells[0].x;
    y0 = y1 = cells[0].y;
    for (i = 1; i < count; i++) {
        if (cells[i].x < x0) x0 = cells[i].x;
        if (cells[i].x > x1) x1 = cells[i].x;
        if (cells[i].y < y0) y0 = cells[i].y;
        if (cells[i].y > y1) y1 = cells[i].y;
    }
    if ((int64_t)x1 + 16 - x0 > (int64_t)ARCADE_COMPOSE_MAX_DIM || (int64_t)y1 + 16 - y0 > (int64_t)ARCADE_COMPOSE_MAX_DIM) {
        fail(err, errsize, "compose: the cells span more than %u pixels", ARCADE_COMPOSE_MAX_DIM);
        return -1;
    }
    out->x0 = x0;
    out->y0 = y0;
    out->w = (uint32_t)(x1 + 16 - x0);
    out->h = (uint32_t)(y1 + 16 - y0);
    out->px = (uint8_t *)calloc((size_t)out->w * out->h, 1);
    if (!out->px) {
        fail(err, errsize, "compose: out of memory");
        memset(out, 0, sizeof *out);
        return -1;
    }
    for (i = 0; i < count; i++) {
        uint8_t t[256];
        if (arcade_compose_tile(tiles, tsize, cells[i].code, (cells[i].flags >> 5) & 3u, t)) {
            fail(err, errsize, "compose: tile %#x lies outside the graphics", (unsigned)cells[i].code);
            arcade_compose_free(out);
            return -1;
        }
        for (y = 0; y < 16; y++)
            for (x = 0; x < 16; x++)
                if (t[y * 16u + x])
                    out->px[(size_t)(cells[i].y - y0 + (int32_t)y) * out->w + (size_t)(cells[i].x - x0 + (int32_t)x)] = t[y * 16u + x];
    }
    return 0;
}

int arcade_compose_record_canvas(const uint8_t *prog, size_t psize, const uint8_t *tiles, size_t tsize, uint32_t address, uint32_t bank,
                                 struct arcade_canvas *out, char *err, size_t errsize)
{
    struct arcade_compose_cell *cells = (struct arcade_compose_cell *)malloc(512u * sizeof *cells);
    struct arcade_compose_info info;
    int rc;
    memset(out, 0, sizeof *out);
    if (!cells) {
        if (err && errsize) snprintf(err, errsize, "compose: out of memory");
        return -1;
    }
    rc = arcade_compose_record(prog, psize, address, bank, 0, cells, 512u, &info, err, errsize);
    if (rc == 0) rc = arcade_compose_draw(tiles, tsize, cells, info.count, out, err, errsize);
    free(cells);
    return rc;
}

int arcade_compose_scroll2(const uint8_t *prog, size_t psize, const uint8_t *tiles, size_t tsize, unsigned first_block, unsigned blocks,
                           struct arcade_canvas *out, char *err, size_t errsize)
{
    struct arcade_compose_cell *cells;
    unsigned b, c, r, n = 0;
    int rc;
    memset(out, 0, sizeof *out);
    if (err && errsize) err[0] = 0;
    if (blocks < 1 || blocks > ARCADE_COMPOSE_MAX_DIM / 256u) {
        fail(err, errsize, "compose: %u scroll 2 blocks side by side", blocks);
        return -1;
    }
    cells = (struct arcade_compose_cell *)malloc((size_t)blocks * 256u * sizeof *cells);
    if (!cells) {
        fail(err, errsize, "compose: out of memory");
        return -1;
    }
    for (b = 0; b < blocks; b++)
        for (c = 0; c < 16; c++)
            for (r = 0; r < 16; r++) {
                uint32_t tile;
                uint16_t attr;
                if (arcade_compose_scroll2_cell(prog, psize, first_block + b, c, r, &tile, &attr)) {
                    fail(err, errsize, "compose: scroll 2 block %u is not in the program", first_block + b);
                    free(cells);
                    return -1;
                }
                cells[n].code = tile;
                cells[n].x = (int32_t)(256u * b + 16u * c);
                cells[n].y = (int32_t)(16u * r);
                cells[n].flags = (uint16_t)(attr & 0x7fu);
                cells[n].part = 0;
                n++;
            }
    rc = arcade_compose_draw(tiles, tsize, cells, n, out, err, errsize);
    free(cells);
    return rc;
}

void arcade_compose_cut(const struct arcade_canvas *c, int32_t x, int32_t y, unsigned rows, uint8_t out[128])
{
    unsigned px, py;
    memset(out, 0, 128);
    if (!c->px) return;
    for (py = 0; py < 16; py++) {
        int64_t cy = (int64_t)y + py - c->y0;
        if ((rows == 1 && !(py & 1u)) || (rows == 2 && (py & 1u)) || cy < 0 || cy >= (int64_t)c->h) continue;
        for (px = 0; px < 16; px++) {
            int64_t cx = (int64_t)x + px - c->x0;
            unsigned v;
            if (cx < 0 || cx >= (int64_t)c->w) continue;
            v = c->px[(size_t)cy * c->w + (size_t)cx];
            out[py * 8u + px / 2u] |= (uint8_t)(v << ((px & 1u) * 4u));
        }
    }
}

void arcade_compose_free(struct arcade_canvas *c)
{
    free(c->px);
    memset(c, 0, sizeof *c);
}
