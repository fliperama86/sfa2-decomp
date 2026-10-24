/* The object grids of the stage modules, built from the arcade's art records (stage_objects.h; the specification is build_grid in stage_objects.py).
 * Project-authored, plain C99. No byte of either game is here. */
#include "stage_objects.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COORD_TABLE 0x9f260u   /* arcade: table of pointers to coordinate blocks, indexed by the record's coordinate index */
#define MAX_CELLS 512u
#define BANK 0x10000u          /* added to a record's tile codes: the graphics' tile */

struct cell { uint32_t code; int x, y; unsigned attr; };

static unsigned be16(const uint8_t *p) { return (unsigned)p[0] << 8 | p[1]; }
static int sbe16(const uint8_t *p) { unsigned v = be16(p); return v & 0x8000u ? (int)v - 65536 : (int)v; }
static uint32_t be32(const uint8_t *p) { return (uint32_t)be16(p) << 16 | be16(p + 2); }

const struct arcade_stage_objects *arcade_stage_objects_find(const char *file)
{
    unsigned i;
    if (!file) return NULL;
    for (i = 0; i < arcade_stage_objects_count; i++)
        if (!strcmp(arcade_stage_objects[i].file, file)) return &arcade_stage_objects[i];
    return NULL;
}

static int msg(char *err, size_t errsize, const char *what, unsigned a, unsigned b)
{
    if (err && errsize) snprintf(err, errsize, what, a, b);
    return -1;
}

/* The cells of the art record at a (mode 2: blocks of tiles; mode 0: one tile a cell), as fighters.py reads them (read_record, read_record0). */
static int read_cells(const uint8_t *prog, size_t n, uint32_t a, struct cell *out, unsigned *count)
{
    unsigned mode, declared, nb, i, j, k = 0, w, h, attr, coords, col;
    int idx;
    uint32_t ptr;
    if ((a & 1u) || a > n || n - a < 8) return -1;
    mode = be16(prog + a);
    declared = be16(prog + a + 2);
    idx = sbe16(prog + a + 6);
    if (mode != 0 && mode != 2) return -1;
    nb = mode == 2 ? be16(prog + a + 4) + 1u : declared;
    if (declared < 1 || declared > MAX_CELLS || (size_t)a + 8u + (mode == 2 ? 4u : 2u) * nb > n) return -1;
    if ((int64_t)COORD_TABLE + idx < 0 || (int64_t)COORD_TABLE + idx > (int64_t)n - 4) return -1;
    ptr = be32(prog + COORD_TABLE + idx);
    if ((ptr & 1u) || (size_t)ptr + 4u * nb > n) return -1;
    coords = ptr;
    for (i = 0; i < nb; i++) {
        uint32_t code;
        int x = sbe16(prog + coords + 4u * i), y = sbe16(prog + coords + 4u * i + 2u);
        if (mode == 2) {
            code = be16(prog + a + 8u + 4u * i);
            attr = be16(prog + a + 10u + 4u * i);
            w = ((attr >> 8) & 15u) + 1u;
            h = ((attr >> 12) & 15u) + 1u;
        } else {
            code = be16(prog + a + 8u + 2u * i);
            attr = be16(prog + a + 4);
            w = h = 1;
            attr &= 0x7fu;
        }
        for (j = 0; j < h; j++)
            for (col = 0; col < w; col++) {
                unsigned ci = attr & 0x20u ? w - 1u - col : col, cj = attr & 0x40u ? h - 1u - j : j;
                if (k >= MAX_CELLS) return -1;
                out[k].code = code + ci + 16u * cj;
                out[k].x = x + 16 * (int)col;
                out[k].y = y + 16 * (int)j;
                out[k].attr = attr & 0x7fu;
                k++;
            }
    }
    *count = k;
    return 0;
}

/* Is graphics tile `index` transparent (every plane bit set: the arcade colour 15 everywhere)? -1 when it lies outside the graphics. */
static int transparent(const uint8_t *tiles, size_t tsize, uint32_t index)
{
    unsigned i;
    if ((uint64_t)index * 128u + 128u > tsize) return -1;
    for (i = 0; i < 128; i++)
        if (tiles[(size_t)index * 128u + i] != 0xff) return 0;
    return 1;
}

int arcade_stage_objects_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                               const struct arcade_stage_run *objects, unsigned object_count,
                               const struct arcade_stage_run *shared, unsigned shared_count,
                               const struct arcade_stage_objects *row, uint8_t *module, uint8_t *known, size_t module_size, char *err, size_t errsize)
{
    uint32_t total = 0, *keys = NULL, at = 0;
    unsigned r, g, rc = 0, i;
    struct cell cells[MAX_CELLS];
    uint32_t *grid = NULL;
    if (err && errsize) err[0] = 0;
    if (!row || !row->grid_count) return 0;
    for (r = 0; r < object_count; r++) total += objects[r].count;
    for (r = 0; r < shared_count; r++) total += shared[r].count;
    keys = malloc(((size_t)total + 1u) * sizeof *keys);
    if (!keys) return msg(err, errsize, "out of memory in the grids (%u)", 0, 0);
    for (r = 0; r < object_count + shared_count; r++) {
        const struct arcade_stage_run *run = r < object_count ? &objects[r] : &shared[r - object_count];
        for (i = 0; i < run->count; i++)
            keys[at++] = run->first == ARCADE_STAGE_ZERO || run->first == ARCADE_STAGE_DISC || run->first == ARCADE_STAGE_BLANK
                             ? 0xffffffffu : (run->first + i) << 2 | (run->flip & 3u);
    }
    for (g = 0; g < row->grid_count; g++) {
        const struct arcade_stage_grid *gr = &row->grids[g];
        unsigned n, vis = 0, cols, rows, pal = 0, palset = 0, c, bad = 0;
        int minx = 0, miny = 0, maxx = 0, maxy = 0;
        uint8_t *dst;
        size_t size;
        if (read_cells(program, program_size, gr->art, cells, &n)) {
            rc = (unsigned)msg(err, errsize, "grid %u: the art record at %#x cannot be read", g, gr->art);
            continue;
        }
        for (c = 0; c < n; c++) {
            int t = transparent(tiles, tiles_size, cells[c].code + BANK);
            if (t < 0) { bad = 1; break; }
            if (t) { cells[c].attr |= 0x8000u; continue; }          /* a cell that shows nothing */
            if (!vis || cells[c].x < minx) minx = cells[c].x;
            if (!vis || cells[c].y < miny) miny = cells[c].y;
            if (!vis || cells[c].x > maxx) maxx = cells[c].x;
            if (!vis || cells[c].y > maxy) maxy = cells[c].y;
            vis++;
        }
        if (bad || !vis) { rc = (unsigned)msg(err, errsize, "grid %u: no visible cell or a tile outside the graphics (%u)", g, 0); continue; }
        cols = (unsigned)(maxx - minx) / 16u + 1u;
        rows = (unsigned)(maxy - miny) / 16u + 1u;
        size = 8u + 2u * cols * rows;
        if (cols > 255 || rows > 255 || gr->offset > module_size || size > module_size - gr->offset) {
            rc = (unsigned)msg(err, errsize, "grid %u does not fit the module (%u)", g, gr->offset);
            continue;
        }
        free(grid);
        grid = calloc((size_t)cols * rows, sizeof *grid);
        if (!grid) { rc = (unsigned)msg(err, errsize, "out of memory in the grids (%u)", 0, 0); continue; }
        for (c = 0; c < n && !bad; c++) {
            uint32_t key, found = 0, number = 0, k;
            unsigned place, ncol, nrow;
            if (cells[c].attr & 0x8000u) continue;
            if ((cells[c].x - minx) % 16 || (cells[c].y - miny) % 16) { bad = 1; break; }
            if (!palset++) pal = cells[c].attr & 31u;
            else if ((cells[c].attr & 31u) != pal) { bad = 1; break; }
            key = (cells[c].code + BANK) << 2 | ((cells[c].attr >> 5) & 1u) | (((cells[c].attr >> 6) & 1u) << 1);
            for (k = gr->lo; k <= gr->hi && k < total; k++)
                if (keys[k] == key) { found++; number = k; }
            ncol = (unsigned)(cells[c].x - minx) / 16u;
            nrow = (unsigned)(cells[c].y - miny) / 16u;
            place = nrow * cols + ncol;
            if (found != 1 || grid[place]) { bad = 1; break; }
            grid[place] = number;
        }
        if (bad) { rc = (unsigned)msg(err, errsize, "grid %u: a cell names no single tile, or two cells share a place (%u)", g, 0); continue; }
        dst = module + gr->offset;
        dst[0] = (uint8_t)cols; dst[1] = 0; dst[2] = (uint8_t)rows; dst[3] = (uint8_t)pal;
        dst[4] = (uint8_t)(-minx); dst[5] = (uint8_t)((-minx) >> 8);
        dst[6] = (uint8_t)(-miny); dst[7] = (uint8_t)((-miny) >> 8);
        for (i = 0; i < cols * rows; i++) {
            dst[8 + 2 * i] = (uint8_t)grid[i];
            dst[9 + 2 * i] = (uint8_t)(grid[i] >> 8);
        }
        if (known) memset(known + gr->offset, 1, size);
    }
    free(grid);
    free(keys);
    return rc ? -1 : 0;
}
