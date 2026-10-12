/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_800f7264_slot0f;

/* What it does (inferred, not an original name): draws a window of a tile
map as sprite records. The map layer c has a pixel width (field_58), a
pixel height (field_5c), a data block (field_50) and a scroll position
(field_12 horizontal, field_14 a 16.16 vertical value, which is
subtracted from 0x100000 before use). The map is stored in blocks of 32 by 16 cells of 4 bytes (a cell
word, then a texture-page word). The function walks 15 rows and `count`
columns from the scroll position; a row outside the map is skipped, and
the column index wraps with the map's width. For every cell whose first
halfword is not field_1e (compared as a signed halfword, so a field_1e
of 0x8000 or more never equals any cell) it fills the next record: x and
y from the grid position minus the scroll remainder, u and v from bits of
the cell, the command word from its top bits (arithmetic shift), the
texture page from the second halfword, increments the record counter
data_800f7264_slot0f (16 bits), and links the record into the list head
selected by c->field_8a in the array data_801987c8 points at. */
/* count is an int: the original does not narrow it at entry. With an s16 parameter this function is 604 bytes and differs from the original in 43 instruction slots. */
void func_800e4ab0_slot0f(Chan *c, Strip1c *buf, int count) {
    int row;
    int n;
    int yy;
    int col;
    u16 ty;
    u16 ix;
    int xoff;
    int yoff;
    int fy;
    int wmask;
    u32 *ot;
    u16 *p;
    u16 cell;
    u16 *base;
    int x;
    s16 key;
    s16 h;

    h = c->field_5c >> 4;
    wmask = (c->field_58 >> 4) - 1;
    xoff = c->scroll_x & 0xf;
    /* fy is written as the negation of field_14 minus 0x100000; written as (0x100000 - c->field_14) this function differs from the original in 28 instruction slots. */
    fy = (-(c->field_14 + (-0x100000))) >> 16;
    yoff = fy & 0xf;
    x = ((s16)c->scroll_x & (c->field_58 - 1)) >> 4;
    key = c->field_1e;
    /* Stand-in, not recovered source: the local n, which is only a copy of count, reproduces the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
    n = count;
    /* The two arms are the division by 16 of a signed value; written as ty = (s16)fy >> 4 this function differs from the original in 67 instruction slots. */
    if ((s16)fy >= 0) {
        ty = (s16)fy / 16;
    } else {
        ty = ((s16)fy - 15) / 16;
    }
    ot = ((u32 *)data_801987c8) + c->ot_index;
    for (row = 0; row < 0xf; row++, ty++) {
        ix = x;
        if ((s16)ty < 0 || (s16)ty >= h) {
            continue;
        }
        /* yy holds the row's y offset, which the original computes once per row; written at its use this function differs from the original in 8 instruction slots. */
        yy = (row * 16) - yoff;
        base = (u16 *)((u8 *)c->map_data + ((ty >> 4) * ((c->field_58 >> 9) << 10)) * 2 + (ty & 0xf) * 128);
        for (col = 0; col < ((s16)n); col++) {
            p = (u16 *)((u8 *)base + ((ix >> 5) << 11) + ((ix & 0x1f) << 2));
            if ((*p) != key) {
                cell = *p;
                buf->pos_x = col * 16 - xoff;
                buf->field_16 = yy;
                buf->field_18 = (cell << 3) & (-8);
                buf->field_19 = ((s16)cell >> 2) & (-8);
                buf->field_1a = p[1];
                buf->field_04 = ((s16)cell >> 10) + 0xe1000000;
                data_800f7264_slot0f = data_800f7264_slot0f + 1;
                ((PrimTag *)buf)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)buf;
                buf++;
            }
            ix = wmask & (ix + 1);
        }
    }
}
