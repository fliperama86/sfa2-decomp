/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f8458_slot06_06[2][0xd0];

/* What it does (inferred, not an original name): a relative of
   func_801e9080_slot06_00 in the module of stage 06. It draws a tile map as
   sprite records. The object selects a layer by its field_0e (8, 4 or 0xc pick
   one of three layer blocks, any other value uses no layer); the layer's
   scroll_x and scroll_y offset the object's position. For layer 8 the x
   position is also moved by a parallax term: (layer scroll_x - layer ref_x)
   * object parallax_factor / 72, signed, rounded toward zero. A header reached
   through the object's sequence describes a grid of 16-bit cells; for each
   non-zero cell (low 14 bits) a record is filled and linked into the list head
   selected by the object. Differences from stage 00: the record table has 208
   records per buffer, the command word is 0xe100001c plus the cell's high
   part, and an object whose field_02 is 0x47 and table_index is 0 draws the cells
   0x39, 0x3a, 0x40, 0x41, 0x46, 0x47, 0x4b, 0x4c, 0x4d, 0x53, 0x54 and 0x55
   with another texture page (offset 0x7846 instead of 0x7806). */
void func_801e9798_slot06_06(Object *object) {
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
    int lo;
    int off;
    int tu;
    int tv;
    int nib;
    int nib2;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x +
            ((s16)layer->scroll_x - (s16)layer->ref_x) * (s16)((Slot06Obj *)object)->parallax_factor / 72;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else {
        x = ((Slot06Obj *)object)->pos_x;
        y = ((Slot06Obj *)object)->pos_y;
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
            cell = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                lo = cell % 256;
                nib2 = lo - (lo / 16) * 16;
                nib = lo / 16;
                prim = &data_801f8458_slot06_06[sel][(u16)cnt];
                if (object->field_02 == 0x47 && object->table_index == 0) {
                    if (cell == 0x39) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x3a) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x40) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x41) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x46) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x47) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x4b) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x4c) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x4d) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x53) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x54) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else if (cell == 0x55) { tpage = ((object->image_row + hdr[3]) << 6) + 0x7846; }
                    else { tpage = ((object->image_row + hdr[3]) << 6) + 0x7806; }
                }
                prim->sprt.clut = tpage;
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.y = y + row * 16 - oy;
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.u = tu;
                prim->sprt.v = tv;
                prim->mode[0] = cell / 256 + 0xe100001c;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
