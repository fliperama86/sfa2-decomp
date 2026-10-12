/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f5290_slot06_0b[2][0x80];

/* What it does (inferred, not an original name): draws a tile map as sprite
   records, like func_801e9080_slot06_00, for the stage image 0b. The object
   selects a layer by its field_0e (8, 4 or 0xc pick one of three layer
   blocks, any other value uses no layer). For 4 and 0xc, and for no layer,
   the position is as in the stage 00 function. For 8 the x position also
   gets a parallax term: the layer's field_12 minus its field_0a, times the
   object's field_26, divided by 40. A header reached through the object's
   sequence describes a grid of 16-bit cells. For each non-zero cell (low 14
   bits) it takes a record from the table, fills in position, texture
   coordinates, texture page and a command word, and links the record into
   the list head selected by the object. The texture page depends on the
   cell value: a fixed set of cell values (2, 3, 5, 6, 0x11 to 0x15, 0x2b to
   0x2e, 0x39 to 0x3c, 0x47 to 0x49) uses a page 0x40 above the usual one,
   and the cell value 0x3d a page 0x40 below it. */
void func_801e96fc_slot06_0b(Object *object) {
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
    int nib;
    int nib2;
    int off;
    int tu;
    int tv;
    u16 ox;
    u16 oy;
    int tpage;
    u8 sel;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12 +
            ((s16)layer->field_12 - (s16)layer->field_0a) * (s16)((Slot06Obj *)object)->field_26 / 40;
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
    ot = (u32 *)data_801987c8 + object->field_09;
    sel = *(u8 *)&data_801a27d0;
    for (row = 0; row < hdr[2]; row++) {
        for (col = 0; col < hdr[0]; col++) {
            /* off is a local; written in place, the function differs in 1 instruction slot. */
            off = (col + row * hdr[0]) * 2;
            cell = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                nib2 = (cell % 256) - ((cell % 256) / 16) * 16;
                nib = (cell % 256) / 16;

                /* One branch per cell value; one combined test, or a switch, builds smaller code and
                   differs from the original in 143 instruction slots. */
                if (cell == 0x2) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x3) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x5) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x6) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x11) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x12) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x13) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x14) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x15) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x2b) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x2c) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x2d) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x2e) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x39) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x3a) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x3b) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x3c) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x3d) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x77c6;
                } else if (cell == 0x47) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x48) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else if (cell == 0x49) {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                } else {
                    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7806;
                }

                prim = &data_801f5290_slot06_0b[sel][(u16)cnt];
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.y = y + row * 16 - oy;
                /* The two locals tu and tv hold the texture coordinates; written in place of them,
                   this function differs from the original in 30 instruction slots. */
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                /* The stores run u, clut, v; with clut last the function differs in 4 instruction slots. */
                prim->sprt.u = tu;
                prim->sprt.clut = tpage;
                prim->sprt.v = tv;
                prim->mode[0] = cell / 256 + 0xe100001c;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                /* The list head is written as mask and OR; as ((PrimTag *)ot)->addr = (u32)prim the
                   function differs from the original in 101 instruction slots. */
                *(u32 *)ot = (*(u32 *)ot & 0xff000000) | ((u32)prim & 0x00ffffff);
            }
        }
    }
    data_801adfe4 = cnt;
}
