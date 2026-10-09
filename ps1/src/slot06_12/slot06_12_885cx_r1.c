/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f11e0_slot06_12[2][0x10e];
extern s16 data_8019054e;

/* Draws the second layer: tile records of 16 rows of 25 cells of 16 pixels,
   the first 8 rows with the horizontal scroll from data_8019054e and the
   last 8 with the layer's own offset (inferred).
   The row count nr is an s16 local and the blank tile braw a word; rows and
   blank are s16 copies of them: with nr as an int the function is 4 bytes
   shorter and 153 instruction slots differ. yy, the row's screen offset, is
   an s16 local: as an int the function is 28 bytes longer. h is a local
   apart from g: with one local for both, 17 instruction slots differ. The
   second half starts at i = 8 explicitly: with i continuing, the function is
   8 bytes longer. The offset at 0x12 is read as the high half of
   field_10, which the layout has as 32 bits. */
void func_801e885c_slot06_12(void) {
    Slot06Draw *c = (Slot06Draw *)data_801aa5d4;
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
    int braw;
    s16 nr;
    s16 blank;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    u16 cc;
    int g;
    int h;
    s16 tile;
    s16 yy;
    braw = c->field_1e;
    x = c->field_14 - 0x100000;
    pos = -x;
    t = (pos >> 16) - game_state.field_92;
    pos &= 0xffff;
    pos |= t << 16;
    y = pos >> 16;
    sy = y & 0xf;
    c->field_3c = pos;
    if (y >= 0) {
        row = y >> 4;
    } else {
        row = (y - 15) / 16;
    }
    nr = c->field_5c >> 4;
    mask = (c->field_58 >> 4) - 1;
    g = data_8019054e;
    sx = g & 0xf;
    col = (g & (c->field_58 - 1)) >> 4;
    blank = braw;
    rows = nr;
    rec = data_801f11e0_slot06_12[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 8; i++, row++) {
        cc = col;
        if (row < 0 || row >= rows) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        yy = (i << 4) - sy;
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 4) - sx;
                rec->sprt.y = yy;
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
    rows = nr;
    blank = braw;
    h = ((s16 *)&c->field_10)[1];
    sx = h & 0xf;
    col = (h & (c->field_58 - 1)) >> 4;
    for (i = 8; i < 16; i++, row++) {
        cc = col;
        if (row < 0 || row >= rows) {
            continue;
        }
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        yy = (i << 4) - sy;
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 4) - sx;
                rec->sprt.y = yy;
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
