/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ed498_slot06_0a[2][0xc1];

/* Draws the first layer: for 16 rows of 25 cells of 16 pixels, a tile record
   for each cell that is not blank, linked into the primitive list (inferred).
   The vertical scroll is the position minus half of game_state.field_92.
   The locals t, sx, mask and cc are 16-bit, as in the model
   func_801e82c8_slot06_00. */
void func_801e8330_slot06_0a(void) {
    Chan *c = data_801aa544;
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
    Slot06Tile *rec;
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
    rows = c->field_5c >> 4;
    mask = (c->field_58 >> 4) - 1;
    sx = c->field_12 & 0xf;
    y = pos >> 16;
    sy = y & 0xf;
    col = ((s16)c->field_12 & (c->field_58 - 1)) >> 4;
    blank = c->field_1e;
    if (y >= 0) {
        row = y >> 4;
    } else {
        row = (y - 15) / 16;
    }
    rec = data_801ed498_slot06_0a[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 16; i++, row++) {
        cc = col;
        if (row < 0 || row >= rows) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            if (*e != blank) {
                s16 tile;
                tile = *e;
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

