/* The name plates' cell maps built from the arcade's scroll 1; see plates.h. The model is plates.py. Project-authored, plain C99. No byte of either game is here. */
#include "plates.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compose.h"

#define COLUMNS 64u
#define ROWS 32u
#define FIRST_ROW 3u            /* the strip's rows in the map, and its rows in the scroll 1 sets */
#define BAND_ROWS 8u
#define FIRST_PAGE 22u          /* a texture word is (FIRST_PAGE << 10) | (unit row << 5) | unit column */
#define CLUT 0x7800u
#define FILL_TEXTURE 0x5800u
#define UNIT_BYTES 32u
#define PAGE_ROW 128u           /* bytes a row of the page */
#define SLOTS 4096u

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
    for (i = 0; i < n; i++)
        h = (h ^ p[i]) * 16777619u;
    return h;
}

static void put_cell(uint8_t *map, unsigned r, unsigned c, unsigned texture, unsigned clut)
{
    uint8_t *p = map + ((size_t)r * COLUMNS + c) * 4u;
    p[0] = (uint8_t)texture;
    p[1] = (uint8_t)(texture >> 8);
    p[2] = (uint8_t)clut;
    p[3] = (uint8_t)(clut >> 8);
}

struct miss {
    uint8_t r, c;
    uint16_t clut;
    uint8_t piece[UNIT_BYTES];
};

int arcade_plate_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size, unsigned band, uint8_t *page,
                       const uint8_t *page_known, uint8_t *map, uint8_t *units, unsigned *filled, char *err, size_t errsize)
{
    struct unit_slot *table = (struct unit_slot *)calloc(SLOTS, sizeof *table);
    struct miss *misses = (struct miss *)calloc(BAND_ROWS * COLUMNS, sizeof *misses);
    uint8_t referenced[1024];
    unsigned u, v, yy, r, c, k, fill = 0, nmiss = 0, next = 0;
    int rc = -1;
    if (err && errsize)
        err[0] = 0;
    if (!table || !misses) {
        free(table);
        free(misses);
        if (err && errsize) snprintf(err, errsize, "out of memory");
        return -1;
    }
    memset(units, 0, 1024);
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLUMNS; c++)
            put_cell(map, r, c, FILL_TEXTURE, CLUT);
    for (v = 0; v < 32; v++)
        for (u = 0; u < 32; u++) {
            uint8_t unit[UNIT_BYTES];
            int whole = 1;
            uint32_t h;
            for (yy = 0; yy < 8 && whole; yy++) {
                size_t at = (size_t)(v * 8 + yy) * PAGE_ROW + u * 4u;
                if (!page_known[at] || !page_known[at + 1] || !page_known[at + 2] || !page_known[at + 3]) whole = 0;
                else memcpy(unit + yy * 4, page + at, 4);
            }
            if (!whole) continue;
            h = fnv32(unit, UNIT_BYTES);
            for (k = h & (SLOTS - 1); table[k].used; k = (k + 1) & (SLOTS - 1))
                if (table[k].hash == h && memcmp(table[k].bytes, unit, UNIT_BYTES) == 0) break;
            if (!table[k].used) {
                table[k].used = 1;
                table[k].hash = h;
                table[k].texture = (uint16_t)((FIRST_PAGE << 10) | (v << 5) | u);
                memcpy(table[k].bytes, unit, UNIT_BYTES);
            }
        }
    for (r = 0; r < BAND_ROWS; r++)
        for (c = 0; c < COLUMNS; c++) {
            unsigned set, x, code, attr, clut;
            uint8_t piece[UNIT_BYTES], blank[UNIT_BYTES];
            uint32_t h;
            if (c < 7) { set = 0; x = c; }
            else if (c < 32) { set = 64 + band; x = c; }
            else { set = 84; x = c - 32; }
            if (arcade_compose_scroll1_cell(program, program_size, set, x, FIRST_ROW + r, &code, &attr)) {
                if (err && errsize) snprintf(err, errsize, "plate: scroll 1 set %u is outside the program", set);
                goto done;
            }
            if (arcade_compose_piece(tiles, tiles_size, code, attr, piece)) {
                fill++;
                continue;
            }
            memset(blank, 0, sizeof blank);
            clut = memcmp(piece, blank, UNIT_BYTES) == 0 ? CLUT : CLUT + 64u * (attr & 31u);
            h = fnv32(piece, UNIT_BYTES);
            for (k = h & (SLOTS - 1); table[k].used; k = (k + 1) & (SLOTS - 1))
                if (table[k].hash == h && memcmp(table[k].bytes, piece, UNIT_BYTES) == 0) break;
            if (!table[k].used) {   /* the page lacks this piece: it goes to a free unit below */
                misses[nmiss].r = (uint8_t)(FIRST_ROW + r);
                misses[nmiss].c = (uint8_t)c;
                misses[nmiss].clut = (uint16_t)clut;
                memcpy(misses[nmiss].piece, piece, UNIT_BYTES);
                nmiss++;
                continue;
            }
            put_cell(map, FIRST_ROW + r, c, table[k].texture, clut);
        }
    /* The units that no cell names and that are blank and built are free: the pieces the page lacks are written there (the PS1 kept those cells some other way). */
    memset(referenced, 0, sizeof referenced);
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLUMNS; c++) {
            const uint8_t *q = map + ((size_t)r * COLUMNS + c) * 4u;
            unsigned tex = q[0] | q[1] << 8;
            if ((tex >> 10) == FIRST_PAGE) referenced[tex & 1023u] = 1;
        }
    for (k = 0; k < nmiss; k++) {
        uint32_t h = fnv32(misses[k].piece, UNIT_BYTES);
        unsigned slot;
        for (slot = h & (SLOTS - 1); table[slot].used; slot = (slot + 1) & (SLOTS - 1))
            if (table[slot].hash == h && memcmp(table[slot].bytes, misses[k].piece, UNIT_BYTES) == 0) break;
        if (!table[slot].used) {   /* the first of its kind takes the next free unit */
            int found = 0;
            while (next < 1024) {
                unsigned i = next++, uu = i & 31u, vv = i >> 5;
                int blank = 1;
                if (referenced[i]) continue;
                for (yy = 0; yy < 8 && blank; yy++) {
                    size_t at = (size_t)(vv * 8 + yy) * PAGE_ROW + uu * 4u;
                    if (!page_known[at] || !page_known[at + 1] || !page_known[at + 2] || !page_known[at + 3] || page[at] || page[at + 1] || page[at + 2] || page[at + 3]) blank = 0;
                }
                if (!blank) continue;
                for (yy = 0; yy < 8; yy++) memcpy(page + (size_t)(vv * 8 + yy) * PAGE_ROW + uu * 4u, misses[k].piece + yy * 4, 4);
                units[i] = 1;
                referenced[i] = 1;
                table[slot].used = 1;
                table[slot].hash = h;
                table[slot].texture = (uint16_t)((FIRST_PAGE << 10) | (vv << 5) | uu);
                memcpy(table[slot].bytes, misses[k].piece, UNIT_BYTES);
                found = 1;
                break;
            }
            if (!found) {
                fill++;
                continue;
            }
        }
        put_cell(map, misses[k].r, misses[k].c, table[slot].texture, misses[k].clut);
    }
    if (filled)
        *filled = fill;
    rc = 0;
done:
    free(table);
    free(misses);
    return rc;
}
