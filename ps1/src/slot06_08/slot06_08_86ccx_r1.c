/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f085c_slot06_08[2][0x83];

/* A variant of func_801e86bc_slot06_04: the row counter starts at 4, the
   loop counter at a value derived from field_14, and the tile table has
   0x83 records per buffer (inferred). The local p2 holds x - 0x100000:
   written into the expression, the function is 8 bytes shorter and 133
   instruction slots differ. */
void func_801e86cc_slot06_08(void) {
    Slot06Draw *c = (Slot06Draw *)data_801aa5d4;
    int q;
    int x;
    int pos;
    int p2;
    int x12;
    int y;
    u16 t;
    s16 row;
    u16 col;
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
    u16 sx;
    u16 mask;
    s16 yy;
    s16 r;
    s16 tile;
    x = c->field_14;
    t = (x >> 16) + game_state.field_92;
    row = 4;
    x &= 0xffff;
    rows = c->field_5c >> 4;
    x |= t << 16;
    p2 = x - 0x100000;
    pos = 0x400000 - p2;
    y = pos >> 16;
    sy = y & 0xf;
    q = (c->field_10 - c->field_08)
        * (c->field_4c - c->field_1c)
        / (c->field_4e - c->field_1c);
    x12 = (q + c->field_08) >> 16;
    n = (u32)(c->field_4c - ((y - 0x40) & -16)) >> 4;
    sx = x12 & 0xf;
    mask = (c->field_58 >> 4) - 1;
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    c->field_3c = pos;
    rec = data_801f085c_slot06_08[data_801a27d0];
    blank = c->field_1e;
    col = (x12 & (c->field_58 - 1)) >> 4;
    i = ((x >> 16) + 0x3f) / 16;
    for (; i < n; i++, row++) {
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
