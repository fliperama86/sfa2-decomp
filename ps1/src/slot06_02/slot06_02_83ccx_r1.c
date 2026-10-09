/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ef0e8_slot06_02[2][0xd4];
extern u16 data_801f8978_slot06_02;

/* Draws the first layer: for 13 rows of 25 cells of 16 pixels, a tile record
   for each cell that is neither of the two blank words (the layers' field_1e),
   linked into the primitive list (inferred). A counter at data_801f8978 is
   cleared on entry and counted per record; the function returns when it
   reaches 0xd4, the number of records per buffer (inferred). The locals t,
   sx, mask and cc are 16-bit, as in the model func_801e82c8_slot06_00. */
void func_801e83cc_slot06_02(void) {
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
    s16 blank2;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    u16 cc;
    data_801f8978_slot06_02 = 0;
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
    blank2 = data_801aa544[1].field_1e;
    if (y >= 0) {
        row = y >> 4;
    } else {
        row = (y - 15) / 16;
    }
    rec = data_801ef0e8_slot06_02[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 13; i++, row++) {
        cc = col;
        if (row < 0 || row >= rows) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            if (*e != blank && *e != blank2) {
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
                data_801f8978_slot06_02++;
                if ((s16)data_801f8978_slot06_02 >= 0xd4) {
                    return;
                }
            }
            cc = mask & (cc + 1);
        }
    }
}

