/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06TileW data_801f51dc_slot06_0d[2][0x5a];

/* Draws the layer of data_801aa544[2]: for 9 rows of 13 cells of 32 pixels,
   a tile record for each cell that is not blank, linked into the primitive
   list (inferred). The locals t, sx, sy, mask and cc are 16-bit, as in
   func_801e82c8_slot06_00. */
void func_801e9438_slot06_0d(void) {
    Chan *c = &data_801aa544[2];
    int x;
    int pos;
    s16 y;
    u16 t;
    s16 row;
    u32 col;
    int i;
    int k;
    u16 *line;
    u16 *e;
    Slot06TileW *rec;
    u32 *ot;
    s16 blank;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    u16 cc;
    x = c->field_14 - 0x100000;
    pos = -x;
    t = (pos >> 16) - ((s16)game_state.field_92 >> 1);
    pos &= 0xffff;
    pos |= t << 16;
    rows = c->field_5c >> 5;
    mask = (c->field_58 >> 5) - 1;
    sx = c->field_12 & 0x1f;
    y = pos >> 16;
    sy = y & 0x1f;
    col = ((s16)c->field_12 & (c->field_58 - 1)) >> 5;
    blank = c->field_1e;
    if (y >= 0) {
        row = y >> 5;
    } else {
        row = (y - 31) / 32;
    }
    rec = data_801f51dc_slot06_0d[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 9; i++, row++) {
        cc = col;
        if (row < 0 || rows < row) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 3) * ((c->field_58 >> 9) << 8) + ((row & 7) << 5);
        for (k = 0; k < 13; k++) {
            s16 tile;

            e = line + ((cc >> 4) << 8) + ((cc & 0xf) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 5) - sx;
                rec->sprt.u = (tile << 3) & -8;
                rec->sprt.v = (tile >> 2) & -8;
                rec->sprt.y = (i << 5) - sy;
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
