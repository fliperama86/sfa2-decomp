/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_801f2504_slot06_08[2][0x108];

/*
 * What it does (inferred, not original names): draws a tile layer in
 * perspective, as quads. The layer block data_801aa5d4 holds a tile map
 * (field_50, field_58 wide, field_5c high, in pixels) and a scroll position.
 * The first pass draws a few rows (3 to 6, by the layer's field_16 plus
 * game_state.field_92), starting at the row of the layer's field_3e, with
 * x offsets of the quads interpolated between the layer's field_08 and
 * field_10 along a ramp of the row. The second pass draws the rows from the
 * layer's field_4c down, with the offsets interpolated by the layer's
 * field_1c and field_4e. Each pass takes 33 columns, wrapping at the map's
 * width; a tile equal to the layer's field_1e is blank and skipped. Every
 * other tile becomes a record of the table: four corners, texture
 * coordinates, a texture page word and a clut word from the second half of
 * the tile cell, linked into the list head selected by the layer's field_8a.
 */
void func_801e8bd8_slot06_08(void) {
    Slot06Layer *c = (Slot06Layer *)data_801aa5d4;
    /* Plain C: v holds the layer's field_1c here and a row's y value in the second pass (one variable for two values; the second use is labelled below). */
    int v;
    /* Plain C: rw is a variable of its own, set first after the count (set after mask it gives 5 differing instruction slots). */
    u32 rw;
    /* Plain C: the second loop holds two copies of the row drawer, the first for its first row and the second for the others (one copy for both gives 270 differing instruction slots, size 1672). */
    int flag;
    u16 mask;
    u16 sx;
    u16 sy;
    u16 nlow;
    u16 col;
    u16 y;
    u16 d1;
    u16 d2;
    s16 blank;
    int n;
    int sum;
    int xx;
    int count;
    short row;
    u16 ix;
    int sxi;
    /* Plain C: L and R are declared for the whole function (declared in the drawer blocks they give 32 differing instruction slots). */
    int L;
    int R;
    int r;
    int i;
    int t;
    int a;
    /* Plain C: ya is a u16 (an int gives 54 differing instruction slots, size 2136). */
    u16 ya;
    int rowsx;
    int d1s;
    int d2s;
    int e1;
    int lo;
    int A15;
    u16 *line;
    Poly28 *rec;
    u32 *ot;
    n = (s16)c->signed_param;
    v = c->field_1c;
    d1 = v - n + 0x40;
    /* Stand-in, not recovered source: the local sxi, which only holds the value on its way to sx, reproduces the original's order of instructions. Without it this function differs from the original in 23 instruction slots. */
    sxi = (s16)c->scroll_x - 0x60;
    sxi &= 0xf;
    sx = sxi;
    nlow = n & 0xf;
    y = c->field_4c - n + 0x40;
    sy = y & 0xf;
    blank = c->packed_param;
    col = (((s16)c->scroll_x - 0x60) & (c->map_width - 1)) >> 4;
    d2 = c->rect_right - v;
    xx = n - 0x40;
    if (xx < 0) {
        /* Stand-in, not recovered source: this hand-written expansion of the signed division (n - 0x4f) / 16 reproduces the original's order of instructions. Without it this function differs from the original in 24 instruction slots. */
        n -= 0x4f;
        if (n < 0) {
            n = xx;
        }
        row = n >> 4;
    } else {
        row = xx >> 4;
    }
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
    rw = c->scale >> 4;
    /* Stand-in, not recovered source: the local lo, which only holds the constant -6, reproduces the original's choice of registers. Without it this function differs from the original in 100 instruction slots. */
    lo = -6;
    mask = (c->map_width >> 4) - 1;
    rec = data_801f2504_slot06_08[data_801a27d0];
    ot = (u32 *)data_801987c8 + c->field_8a;
    for (i = 0; i < count; i++, row++) {
        ix = col;
        /* Stand-in, not recovered source: the local rowsx, which is only a copy of rw as an s16, reproduces the original's choice of registers. Without it this function differs from the original in 8 instruction slots. */
        rowsx = (s16)rw;
        /* Stand-in, not recovered source: the local r, which is only a copy of row as an int, reproduces the original's choice of registers. Without it this function differs from the original in 4 instruction slots. */
        r = (s16)row;
        if (r < 0 || r >= rowsx) {
            continue;
        }
        line = c->tile_map_ptr + ((u16)row >> 4) * ((c->map_width >> 9) << 10) + ((row & 0xf) << 6);
        t = (u16)((u32)(0x10 - r) << 4);
        a = *(s32 *)&c->bound_x_b - *(s32 *)&c->bound_x_a;
        ya = (i << 4) - nlow;
        {
            int k;
            u16 tile;
            u16 *e;
            L = a - a * (t - 0x80) / 88;
            R = a - a * (t - 0x90) / 88;
            for (k = lo; k < 27; k++) {
                e = line + ((ix >> 5) << 10) + ((ix & 0x1f) << 1);
                tile = e[0];
                if ((s16)tile != blank) {
                    rec->tpage = (s16)tile >> 10;
                    rec->clut = e[1];
                    rec->x0 = (k << 4) - sx + (L >> 16);
                    rec->x1 = rec->x0 + 16;
                    rec->x2 = (k << 4) - sx + (R >> 16);
                    rec->x3 = rec->x2 + 16;
                    rec->y0 = rec->y1 = ya;
                    rec->y2 = rec->y3 = ya + 16;
                    rec->u0 = rec->u2 = tile << 3;
                    rec->u1 = rec->u3 = (tile << 3) + 15;
                    rec->v0 = rec->v1 = ((s16)tile >> 2) & -16;
                    rec->v2 = rec->v3 = (((s16)tile >> 2) & -16) | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                ix = mask & (ix + 1);
            }
        }
    }
    row = (s16)c->field_4c / 16;
    flag = 0;
    i = (s16)y / 16;
    /* Stand-in, not recovered source: the local d2s, which is only a copy of d2 as an s16, reproduces the original's choice of registers. Without it this function differs from the original in 8 instruction slots. */
    d2s = (s16)d2;
    /* Stand-in, not recovered source: the local d1s, which is only a copy of d1 as an s16, reproduces the original's choice of registers. Without it this function differs from the original in 6 instruction slots. */
    d1s = (s16)d1;
    /* Stand-in, not recovered source: the local e1, which only names the value d1s - 16, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
    e1 = d1s - 16;
    /* Stand-in, not recovered source: the local A15, which only names the value (s16)y / 16 + 15, reproduces the original's order of instructions. Without it this function differs from the original in 4 instruction slots. */
    for (; A15 = (s16)y / 16 + 15, i < A15 - (s16)c->field_4c / 16; i++, row++) {
        ix = col;
        line = c->tile_map_ptr + ((u16)row >> 4) * ((c->map_width >> 9) << 10) + ((row & 0xf) << 6);
        /* Stand-in, not recovered source: the variable v, which holds this value only to copy it to ya, reproduces the original's choice of registers. Without it this function differs from the original in 15 instruction slots. */
        v = sy + (i << 4);
        ya = v;
        if (flag == 0) {
            {
                /* Plain C: a second a of the block (using the function's a gives 19 differing instruction slots). */
                int a = *(s32 *)&c->bound_x_b - *(s32 *)&c->bound_x_a;
                int k;
                u16 tile;
                u16 *e;
                L = a - a * ((u16)ya - d1s) / d2s;
                R = a - a * ((u16)ya - e1) / d2s;
                for (k = lo; k < 27; k++) {
                    e = line + ((ix >> 5) << 10) + ((ix & 0x1f) << 1);
                    tile = e[0];
                    if ((s16)tile != blank) {
                        rec->tpage = (s16)tile >> 10;
                        rec->clut = e[1];
                        rec->x0 = (k << 4) - sx + (L >> 16);
                        rec->x1 = rec->x0 + 16;
                        rec->x2 = (k << 4) - sx + (R >> 16);
                        rec->x3 = rec->x2 + 16;
                        rec->y0 = rec->y1 = ya;
                        rec->y2 = rec->y3 = ya + 16;
                        rec->u0 = rec->u2 = tile << 3;
                        rec->u1 = rec->u3 = (tile << 3) + 15;
                        rec->v0 = rec->v1 = ((s16)tile >> 2) & -16;
                        rec->v2 = rec->v3 = (((s16)tile >> 2) & -16) | 15;
                        ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                        ((PrimTag *)ot)->addr = (u32)rec;
                        rec++;
                    }
                    ix = mask & (ix + 1);
                }
            }
            /* Plain C: flag is set to 0x88 and not to 1 (flag = 1 gives 1 differing instruction slot). */
            flag = 0x88;
        } else {
            {
                /* Plain C: a second a of the block (using the function's a gives 19 differing instruction slots). */
                int a = *(s32 *)&c->bound_x_b - *(s32 *)&c->bound_x_a;
                int k;
                u16 tile;
                u16 *e;
                L = a - a * ((u16)ya - d1s) / d2s;
                R = a - a * ((u16)ya - e1) / d2s;
                for (k = lo; k < 27; k++) {
                    e = line + ((ix >> 5) << 10) + ((ix & 0x1f) << 1);
                    tile = e[0];
                    if ((s16)tile != blank) {
                        rec->tpage = (s16)tile >> 10;
                        rec->clut = e[1];
                        rec->x0 = (k << 4) - sx + (L >> 16);
                        rec->x1 = rec->x0 + 16;
                        rec->x2 = (k << 4) - sx + (R >> 16);
                        rec->x3 = rec->x2 + 16;
                        rec->y0 = rec->y1 = ya;
                        rec->y2 = rec->y3 = ya + 16;
                        rec->u0 = rec->u2 = tile << 3;
                        rec->u1 = rec->u3 = (tile << 3) + 15;
                        rec->v0 = rec->v1 = ((s16)tile >> 2) & -16;
                        rec->v2 = rec->v3 = (((s16)tile >> 2) & -16) | 15;
                        ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                        ((PrimTag *)ot)->addr = (u32)rec;
                        rec++;
                    }
                    ix = mask & (ix + 1);
                }
            }
        }
    }
}
