/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f6ce0_slot06_04[2][0x130];

/* What it does (inferred, not an original name): a relative of
   func_801e9080_slot06_00 in the module of stage 04. It draws a tile map as
   sprite records. The object selects a layer by its field_0e (8, 4 or 0xc pick
   one of three layer blocks). For layer 8 the x position is moved by a
   parallax term: (layer scroll_x - layer ref_x) * object parallax_factor / 64,
   signed, rounded toward zero. The layer's scroll_x and scroll_y offset the
   object's position. A header reached through the object's sequence describes
   a grid of 16-bit cells; for each non-zero cell (low 14 bits) a record is
   filled and linked into the list head selected by the object. Two things
   differ from stage 00: the header byte 1 (a flip or palette mode, 5 or 6)
   switches the texture page from row 2 on, and the command word is 0xe100001b
   plus the cell's high part.

   For a value of field_0e that has no case here (any value but 8, 4 and
   0xc), x and y are read without a first value. Nothing is claimed here
   about whether such a call happens, or about what the original's registers
   hold then. */
void func_801e99b4_slot06_04(Object *object) {
    Slot06Layer *layer;
    u8 *hdr;
    Slot06Tile *prim;
    u32 *ot;
    s16 cnt;
    int row;
    int col;
    u16 x;
    s16 y;
    int cell;
    u16 ox;
    u16 oy;
    u16 tpage;
    u8 sel;
    s16 r;
    s16 hi;
    int off;
    int nib;
    int nib2;
    int tu;
    int tv;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x +
            ((s16)layer->scroll_x - (s16)layer->ref_x) * (s16)((Slot06Obj *)object)->parallax_factor / 64;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    }
    if (object->field_0f != 0) y = y + game_state.scroll_y;
    cnt = data_801adfe4;
    hdr = (u8 *)object->sequence->data_ptr;
    ox = *(u16 *)(hdr + 4);
    oy = *(u16 *)(hdr + 6);
    tpage = ((object->image_row + hdr[3]) << 6) + 0x7806;
    ot = (u32 *)data_801987c8 + object->ot_depth;
    sel = *(u8 *)&data_801a27d0;
    for (row = 0; row < hdr[2]; row++) {
        for (col = 0; col < hdr[0]; col++) {
            off = (col + row * hdr[0]) * 2;
            cell = hi = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                r = cell % 256;
                nib2 = r - (r / 16) * 16;
                nib = r / 16;
                if (hdr[1] == 5 && row >= 2) {
                    tpage = 0x7846;
                } else if (hdr[1] == 6 && row >= 2) {
                    tpage = 0x78c6;
                }
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim = &data_801f6ce0_slot06_04[sel][(u16)cnt];
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.u = tu;
                prim->sprt.clut = tpage;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.v = tv;
                prim->mode[0] = (hi / 256) + 0xe100001b;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
