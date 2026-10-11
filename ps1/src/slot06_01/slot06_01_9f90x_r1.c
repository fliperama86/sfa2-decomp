/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801fa57c_slot06_01[2][108];

/* What it does (inferred, not an original name): the stage 01 relative of
   func_801e9080_slot06_00, draws a tile map as sprite records. The object
   must be of kind field_0e 8: its position is then taken relative to the
   layer block data_801aa5d4 (field_12 and field_16), with a parallax term
   ((layer field_12 - layer field_0a) * object field_26 / 128) added to x.
   A header reached through the object's sequence describes a grid of
   16-bit cells. For each non-zero cell (low 14 bits) it takes a record from
   the table, fills in position, texture coordinates, texture page and a
   command word, and links the record into the list head selected by the
   object. Differences from stage 00: one layer only, the parallax term, the
   cell value is raised by 0x80 before it is split, other record table and
   command constant, 108 records per buffer.

   For a value of field_0e that has no case here (any value but 8), x and y
   are read without a first value. Nothing is claimed here about whether such
   a call happens, or about what the original's registers hold then. */
void func_801e9f90_slot06_01(Object *object) {
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
    int hi;
    s16 lo;
    u16 ox;
    u16 oy;
    int tpage;
    u8 sel;
    int nib;
    int nib2;
    u32 cw;
    int tu;
    int tv;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12 +
            ((s16)layer->field_12 - (s16)layer->field_0a) * (s16)((Slot06Obj *)object)->field_26 / 128;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
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
                cell += 0x80;
                lo = cell % 256;
                nib = lo / 16;
                nib2 = lo - nib * 16;
                prim = &data_801fa57c_slot06_01[sel][(u16)cnt];
                hi = cell / 256;
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.u = tu;
                prim->sprt.clut = tpage;
                prim->sprt.y = y + row * 16 - oy;
                /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
                /* Without it this function differs from the original in 13 instruction slots. */
                do {
                    prim->sprt.v = tv;
                } while (0);
                cw = hi + 0xe100001c;
                cnt++;
                prim->mode[0] = cw;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
