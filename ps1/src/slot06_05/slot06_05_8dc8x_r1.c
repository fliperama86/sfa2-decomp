/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f4aa4_slot06_05[2][0x70];

/* What it does (inferred, not an original name): a relative of
   func_801e9080_slot06_00 in the module of stage 05. It draws a tile map as
   sprite records. The object selects a layer by its field_0e (8, 4 or 0xc pick
   one of three layer blocks, any other value uses no layer); the layer's
   field_12 and field_16 offset the object's position. A header reached through
   the object's sequence describes a grid of 16-bit cells; for each non-zero
   cell (low 14 bits) a record is filled and linked into the list head selected
   by the object. Differences from stage 00: the record table has 112 records
   per buffer and the command word is 0xe100001c plus the cell's high part; and
   an object whose field_02 is 0x45 draws the cells 0x47, 0x48, 0x4d, 0x4e,
   0x4f and 0x50 with another texture page (offset 0x7846 instead of 0x7806). */
void func_801e8dc8_slot06_05(Object *object) {
    Slot06Layer *layer;
    u8 *hdr;
    Slot06Tile *prim;
    u32 *ot;
    s16 cnt;
    int row;
    int col;
    u16 x;
    s16 y;
    s16 cell;
    u16 ox;
    u16 oy;
    int tpage;
    u8 sel;
    s16 lo;
    int off;
    int tu;
    int tv;
    int nib;
    int nib2;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else {
        x = ((Slot06Obj *)object)->field_12;
        y = ((Slot06Obj *)object)->field_16;
    }
    if (object->field_0f != 0) y = y + game_state.field_92;
    cnt = data_801adfe4;
    hdr = (u8 *)object->sequence->field_04;
    ox = *(u16 *)(hdr + 4);
    oy = *(u16 *)(hdr + 6);
    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7806;
    ot = (u32 *)data_801987c8 + object->field_09;
    sel = *(u8 *)&data_801a27d0;
    for (row = 0; row < hdr[2]; row++) {
        for (col = 0; col < hdr[0]; col++) {
            off = (col + row * hdr[0]) * 2;
            cell = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                lo = cell % 256;
                nib2 = lo - (lo / 16) * 16;
                nib = lo / 16;
                prim = &data_801f4aa4_slot06_05[sel][(u16)cnt];
                if (object->field_02 == 0x45) {
                    if (cell == 0x47) { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x48) { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x4d) { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x4e) { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x4f) { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x50) { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846; }
                    else { tpage = ((object->field_0d + hdr[3]) << 6) + 0x7806; }
                }
                prim->sprt.clut = tpage;
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.y = y + row * 16 - oy;
                tu = (nib & 0xff) << 4;
                prim->sprt.u = tu;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.v = tv;

                prim->mode[0] = 0xe100001c + cell / 256;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
