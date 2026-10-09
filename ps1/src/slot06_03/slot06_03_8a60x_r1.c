/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f0d40_slot06_03[2][0x120];

/* Draws the layer of data_801aa544[1]: for the rows of 25 cells of 16
   pixels that lie between the adjusted position and the layer's field_4c
   (at most 16), a tile record for each cell that is not blank, linked into
   the primitive list; the adjusted position goes back into the layer first
   (inferred). The locals t, sx, sy, mask and cc are 16-bit, as in
   func_801e82c8_slot06_00. The local f12 holds field_12 sign-extended, as
   in the model func_801e86d8_slot06_00. */
void func_801e8a60_slot06_03(void) {
    Chan *c = &data_801aa544[1];
    int x;
    int pos;
    s16 y;
    u16 t;
    s16 row;
    u32 col;
    int i;
    int k;
    s16 tile;
    u16 *line;
    u16 *e;
    Slot06Tile *rec;
    u32 *ot;
    s16 blank;
    s16 rows;
    s16 nr;
    s16 sy;
    s16 sx;
    u16 mask;
    u16 cc;
    int f12;
    x = c->field_14 - 0x100000;
    pos = -x;
    t = (pos >> 16) - game_state.field_92;
    pos &= 0xffff;
    pos |= t << 16;
    c->field_3c = pos;
    y = pos >> 16;
    sy = y & 0xf;
    f12 = (s16)c->field_12;
    sx = f12 & 0xf;
    col = (f12 & (c->field_58 - 1)) >> 4;
    blank = c->field_1e;
    if (y >= 0) {
        row = y >> 4;
    } else {
        row = (y - 15) / 16;
    }
    mask = (c->field_58 >> 4) - 1;
    rows = c->field_5c >> 4;
    rec = data_801f0d40_slot06_03[data_801a27d0];
    nr = ((u32)(c->field_4c - ((pos >> 16) & -16)) >> 4) + 1;
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < nr; i++, row++) {
        cc = col;
        if (row < 0 || row >= rows) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 4) - sx;
                rec->sprt.y = (i << 4) - sy;
                rec->sprt.u = (tile << 3) & -8;
                rec->sprt.v = (tile >> 2) & -8;
                rec->sprt.clut = e[1];
                rec->mode[0] = 0xe1000000 + (tile >> 10);
                ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)rec;
                rec++;
            }
            cc = mask & (cc + 1);
        }
    }
}
