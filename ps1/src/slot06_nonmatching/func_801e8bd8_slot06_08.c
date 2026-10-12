/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in the row computation and in
 * instruction scheduling. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py) compares the behavior of this C
 * with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): draws a tile layer in
 * perspective, as quads. The layer block data_801aa5d4 holds a tile map
 * (tile_map_ptr, map_width wide, scale high, in pixels) and a scroll position.
 * The first pass draws a few rows (3 to 6, by the layer's scroll_y plus
 * game_state.scroll_y), starting at the row of the layer's signed_param, with
 * x offsets of the quads interpolated between the layer's bound_x_a and
 * bound_x_b along a ramp of the row. The second pass draws the rows from the
 * layer's field_4c down, with the offsets interpolated by the layer's
 * field_1c and rect_right. Each pass takes 33 columns, wrapping at the map's
 * width; a tile equal to the layer's packed_param is blank and skipped. Every
 * other tile becomes a record of the table: four corners, texture
 * coordinates, a texture page word and a clut word from the second half of
 * the tile cell, linked into the list head selected by the layer's field_8a.
 *
 * Contract (what the code reads and writes; the roles are inferred):
 *   No argument, no result.
 *   Reads: the layer block data_801aa5d4 (fields 08, 10, 12, 16, 1c, 1e,
 *     3e, 4c, 4e, 50, 58, 5c, 8a), game_state.scroll_y, the buffer selector
 *     data_801a27d0 (0 or 1), the list base pointer data_801987c8 and the
 *     list head words it points at, the tile map through tile_map_ptr.
 *   Record table data_801f2504_slot06_08: 2 buffers of 264 records of 40
 *     bytes. Writes, for each drawn tile, the record's fields 08 to 25 that
 *     the code names and the link word (low 24 bits) of the record and of
 *     the list head; the records start at the first of the selected buffer
 *     on every call, and no counter is kept. Nothing else is written.
 *   Aliasing: the layer block, the tile map, the list array and the record
 *     table are distinct blocks.
 *   Excluded inputs (the original cannot survive them): a rect_right minus
 *     field_1c whose low 16 bits are 0 (a division by zero in the second
 *     pass, the original traps); a run past the end of the
 *     record table. The original does not check the number of drawn tiles
 *     against the 264 records of the buffer, and with too many tiles the records
 *     go on into the next data symbol past the table; that is not part of the
 *     contract and this C is not tested on it. The setup counts the tiles
 *     the original will draw and keeps the last record inside the table
 *     (buffer 1 only when at most 264 fit; the last record of the table
 *     itself is a tested case).
 *   Slots no input reaches: eight `break` instructions of the original: two
 *     per division, two divisions per row of the second pass, in each of its
 *     two loop copies (division by zero, and the most negative number
 *     divided by -1). The first is excluded above; the second needs rect_right - field_1c equal to -1 and a
 *     numerator of 0x80000000, and the original traps on it. The original
 *     holds two copies of the second pass's loop (it tests a flag that is
 *     set after the first row); both copies compute the same, this C has
 *     one, and the test reaches both.
 *   Layer field_8a is not in the published field list of Slot06Layer; the
 *     tree's types.fields needs the line `0x08a u8 field_8a` there.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_801f2504_slot06_08[2][0x108];

/* Draws the 33 tiles of one map row as quads. `top` and `bottom` are the
   quad's y values, `sx` the x scroll within a tile, `left` and `right` the
   16.16 offsets of the quad's two sides. Returns the next free record.
   Inferred from the code. */
static Poly28 *draw_row(Poly28 *rec, u32 *ot, u16 *line, u16 col, u16 mask,
                        s16 blank, int top, int bottom, int sx, int left,
                        int right);

void func_801e8bd8_slot06_08(void) {
    Slot06Layer *c = (Slot06Layer *)data_801aa5d4;
    s16 blank;
    u16 sx;
    u16 sy;
    u16 col;
    u16 mask;
    u16 d1;
    u16 d2;
    u16 y;
    int n;
    int sum;
    int count;
    int row;
    int rows;
    int i;
    int t;
    int a;
    int ya;
    int lim;
    u16 *line;
    Poly28 *rec;
    u32 *ot;

    n = (s16)c->signed_param;
    blank = c->packed_param;
    d1 = c->field_1c - n + 0x40;
    y = c->field_4c - n + 0x40;
    d2 = c->rect_right - c->field_1c;
    sx = ((s16)c->scroll_x - 0x60) & 0xf;
    sy = y & 0xf;
    col = (((s16)c->scroll_x - 0x60) & (c->map_width - 1)) >> 4;
    mask = (c->map_width >> 4) - 1;
    row = (n - 0x40) >> 4;
    rows = (s16)(c->scale >> 4);

    sum = (s16)c->scroll_y + (s16)game_state.scroll_y;
    if (sum == 0) {
        count = 3;
    } else if (sum < 0x10) {
        count = 4;
    } else if (sum < 0x20) {
        count = 5;
    } else {
        count = 6;
    }

    rec = data_801f2504_slot06_08[data_801a27d0];
    ot = (u32 *)data_801987c8 + c->field_8a;

    for (i = 0; i < count; i++, row++) {
        if ((s16)row < 0 || (s16)row >= rows) {
            continue;
        }
        line = c->tile_map_ptr + ((u16)row >> 4) * ((c->map_width >> 9) << 10) + ((row & 0xf) << 6);
        t = (u16)((0x10 - (s16)row) << 4);
        a = *(s32 *)&c->bound_x_b - *(s32 *)&c->bound_x_a;
        ya = (i << 4) - (n & 0xf);
        rec = draw_row(rec, ot, line, col, mask, blank, ya, ya + 16, sx,
                       a - a * (t - 0x80) / 88, a - a * (t - 0x90) / 88);
    }

    row = (s16)c->field_4c / 16;
    lim = (s16)y / 16 + 15 - (s16)c->field_4c / 16;
    for (i = (s16)y / 16; i < lim; i++, row++) {
        line = c->tile_map_ptr + ((u16)row >> 4) * ((c->map_width >> 9) << 10) + ((row & 0xf) << 6);
        ya = sy + (i << 4);
        a = *(s32 *)&c->bound_x_b - *(s32 *)&c->bound_x_a;
        rec = draw_row(rec, ot, line, col, mask, blank, ya, ya + 16, sx,
                       a - a * ((u16)ya - (s16)d1) / (s16)d2,
                       a - a * ((u16)ya - ((s16)d1 - 16)) / (s16)d2);
    }
}

/* Draws the 33 tiles of one map row as quads. `top` and `bottom` are the
   quad's y values, `sx` the x scroll within a tile, `left` and `right` the
   16.16 offsets of the quad's two sides. Returns the next free record.
   Inferred from the code. */
static Poly28 *draw_row(Poly28 *rec, u32 *ot, u16 *line, u16 col, u16 mask,
                        s16 blank, int top, int bottom, int sx, int left,
                        int right) {
    int k;
    u16 tile;
    u16 *e;

    for (k = -6; k < 27; k++) {
        e = line + ((col >> 5) << 10) + ((col & 0x1f) << 1);
        tile = e[0];
        if ((s16)tile != blank) {
            rec->tpage = (s16)tile >> 10;
            rec->clut = e[1];
            rec->x0 = (k << 4) - sx + (left >> 16);
            rec->x1 = rec->x0 + 16;
            rec->x2 = (k << 4) - sx + (right >> 16);
            rec->x3 = rec->x2 + 16;
            rec->y0 = rec->y1 = top;
            rec->y2 = rec->y3 = bottom;
            rec->u0 = rec->u2 = tile << 3;
            rec->u1 = rec->u3 = (tile << 3) + 15;
            rec->v0 = rec->v1 = ((s16)tile >> 2) & -16;
            rec->v2 = rec->v3 = (((s16)tile >> 2) & -16) | 15;
            ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
            ((PrimTag *)ot)->addr = (u32)rec;
            rec++;
        }
        col = mask & (col + 1);
    }
    return rec;
}
