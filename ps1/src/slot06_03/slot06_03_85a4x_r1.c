/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ee810_slot06_03[2][0xaa];
extern u16 data_801f8000_slot06_03;

/* Draws the first layer: for 16 rows of 25 cells of 16 pixels, a tile
   record for each cell that is neither of the two blank values (the layer's
   and the next layer's), linked into the primitive list; the count of
   records drawn is kept in a global and ends the drawing at 0xaa
   (inferred). The locals t, sx, mask and cc are 16-bit and x holds the
   position before the sign change, as in the model func_801e82c8_slot06_00,
   where the effect of each is measured. */
void func_801e85a4_slot06_03(void) {
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
    s16 other;
    s16 n;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    u16 cc;
    data_801f8000_slot06_03 = 0;
    other = data_801aa544[1].field_1e;
    x = c->field_14 - 0x100000;
    pos = -x;
    t = (pos >> 16) - game_state.field_92;
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
    rec = data_801ee810_slot06_03[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 16; i++, row++) {
        cc = col;
        if (row < 0 || row >= rows) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            if (*e != blank && *e != other) {
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
                n = ++data_801f8000_slot06_03;
                if (n >= 0xaa) {
                    return;
                }
            }
            cc = mask & (cc + 1);
        }
    }
}

