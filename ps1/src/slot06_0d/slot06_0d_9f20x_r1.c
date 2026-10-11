/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f685c_slot06_0d[0x78][2];

/* What it does (inferred, not an original name): draws a tile map as sprite
   records, like func_801e9080_slot06_00, with three differences: the object
   may be drawn mirrored (field_0b not 0), the layer of field_0e 8 scales part
   of the offset by the object's field_26, and the records take their place in
   a table that interleaves the two buffers. The object selects a layer by its
   field_0e (8, 4 or 0xc pick one of three layer blocks, any other value has
   no case here); the layer's field_12 and field_16 offset the object's
   position. A header reached through the object's sequence describes a grid
   of 16-bit cells. For each non-zero cell (low 14 bits) it takes a record
   from the table, fills in position, texture coordinates, texture page and a
   command word, and links the record into the list head selected by the
   object.

   For a value of field_0e that has no case here (any value but 8, 4 and
   0xc), x and y are read without a first value. Nothing is claimed here
   about whether such a call happens, or about what the original's registers
   hold then. */
void func_801e9f20_slot06_0d(Object *object) {
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
    int off;
    s16 lo;
    u16 ox;
    u16 oy;
    u8 sel;
    int tu;
    int tv;
    int tpage;
    int nib;
    int nib2;

    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        x += ((s16)layer->field_12 - (s16)layer->field_0a) * (s16)object->field_26 / 128;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    }
    if (object->field_0f != 0) y = y + game_state.field_92;
    cnt = data_801adfe4;
    hdr = (u8 *)object->sequence->field_04;
    ox = *(u16 *)(hdr + 4);
    oy = *(u16 *)(hdr + 6);
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
                prim = &data_801f685c_slot06_0d[(u16)cnt][sel];
                tpage = ((object->field_0d + hdr[3]) << 6) + 0x7806;
                prim->sprt.clut = tpage;
                if (object->field_0b == 0) {
                    prim->sprt.x = x + col * 16 - ox;
                } else {
                    prim->sprt.x = x - col * 16 + ox;
                }
                prim->sprt.y = y + row * 16 - oy;
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.u = tu;
                prim->sprt.v = tv;
                prim->mode[0] = cell / 256 + 0xe100001b;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
