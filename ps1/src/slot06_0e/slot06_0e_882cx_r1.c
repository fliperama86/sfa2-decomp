/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f22d4_slot06_0e[2][0x7c];

/* Draws the layer of data_801aa5d4: tile records of the visible 16-pixel
   cells into the primitive list, one row of 25 cells per pass, and a last
   partial row when the vertical position is not a multiple of 16 (inferred).
   The tile mask and the column counter cc are 16-bit locals, and the locals r
   (a copy of row) and yy (the row's screen offset) are kept, as in the model
   func_801e86bc_slot06_04. */
void func_801e882c_slot06_0e(void) {
    Slot06Draw *c = (Slot06Draw *)data_801aa5d4;
    int q;
    int x;
    int pos;
    int x12;
    s16 y;
    s16 t;
    s16 row;
    u32 col;
    int i;
    int k;
    s16 n;
    u16 *line;
    u16 *e;
    Slot06Tile *rec;
    Slot06Tile *tp;
    u32 *ot;
    s16 blank;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    s16 yy;
    s16 r;
    s16 tile;
    q = (c->field_10 - c->field_08)
        * (c->field_4c - c->field_1c)
        / (c->field_4e - c->field_1c);
    blank = c->field_1e;
    x = c->field_14;
    x -= 0x100000;
    pos = -x;
    t = (pos >> 16) - game_state.field_92;
    pos &= 0xffff;
    pos |= t << 16;
    c->field_3c = pos;
    y = pos >> 16;
    sy = y & 0xf;
    x12 = (q + c->field_08) >> 16;
    sx = x12 & 0xf;
    col = (x12 & (c->field_58 - 1)) >> 4;
    if (y >= 0) {
        row = y >> 4;
    } else {
        row = (y - 15) / 16;
    }
    rows = c->field_5c >> 4;
    mask = (c->field_58 >> 4) - 1;
    rec = data_801f22d4_slot06_0e[data_801a27d0];
    n = (u32)(c->field_4c - ((pos >> 16) & -16)) >> 4;
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < n; i++, row++) {
        u16 cc = col;
        if (row < 0 || row > rows) {
            continue;
        }
        r = row;
        line = c->field_50 + ((u16)r >> 4) * ((c->field_58 >> 9) << 10) + ((r & 0xf) << 6);
        yy = (i << 4) - sy;
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            tile = *e;
            if (tile != blank) {
                tp = rec;
                tp->sprt.x = (k << 4) - sx;
                tp->sprt.y = yy;
                tp->sprt.u = (tile << 3) & -8;
                tp->sprt.v = (tile >> 2) & -8;
                tp->sprt.clut = e[1];
                tp->mode[0] = 0xe1000000 + (tile >> 10);
                ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)rec;
                rec++;
            }
            cc = mask & (cc + 1);
        }
    }
    if (c->field_4c & 0xf) {
        u16 cc = col;
        if (row > 0 || row <= rows) {
            r = row;
            line = c->field_50 + ((u16)r >> 4) * ((c->field_58 >> 9) << 10) + ((r & 0xf) << 6);
            yy = (i << 4) - sy;
            for (k = 0; k < 25; k++) {
                e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
                tile = *e;
                if (tile != blank) {
                    tp = rec;
                    tp->sprt.x = (k << 4) - sx;
                    tp->sprt.y = yy;
                    tp->sprt.u = (tile << 3) & -8;
                    tp->sprt.v = (tile >> 2) & -8;
                    tp->sprt.clut = e[1];
                    tp->mode[0] = 0xe1000000 + (tile >> 10);
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
        }
    }
}
