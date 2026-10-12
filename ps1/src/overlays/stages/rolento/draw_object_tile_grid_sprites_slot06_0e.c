/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f8478_slot06_0e[2][0xd0];

/* What it does (inferred, not an original name): draws a tile map as sprite
   records, like draw_object_tile_grid_sprites_slot06_00, with two differences: the layer of
   field_0e 8 scales part of the offset by the object's scroll_factor (divided by
   79), and objects whose field_02 is 0x41 or 0x16 take another texture page
   for some cell values. The object selects a layer by its field_0e (8, 4 or
   0xc; any other value uses no layer); the layer's scroll_x and scroll_y
   offset the object's position. A header reached through the object's sequence
   describes a grid of 16-bit cells. For each non-zero cell (low 14 bits) it
   takes a record from the table, fills in position, texture coordinates,
   texture page and a command word, and links the record into the list head
   selected by the object. */
void draw_object_tile_grid_sprites_slot06_0e(Object *object) {
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
    int tu;
    int tv;
    u16 ox;
    u16 oy;
    int tpage;
    u8 sel;
    int nib;
    int nib2;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        x += ((s16)layer->scroll_x - (s16)layer->ref_x) * (s16)object->scroll_factor / 79;
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
                prim = &data_801f8478_slot06_0e[sel][(u16)cnt];
                if (object->field_02 == 0x41 || object->field_02 == 0x16) {
                    if (cell == 0x65) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x66) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x6b) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x6c) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x70) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x71) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x67) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x68) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x6d) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x6e) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x75) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x76) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x77) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x7b) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x7c) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x7d) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x82) { tpage = (object->image_row << 6) + 0x7906; }
                    else if (cell == 0x7e) { tpage = (object->image_row << 6) + 0x78c6; }
                    else if (cell == 0x7f) { tpage = (object->image_row << 6) + 0x78c6; }
                    else if (cell == 0x83) { tpage = (object->image_row << 6) + 0x78c6; }
                    else if (cell == 0x84) { tpage = (object->image_row << 6) + 0x78c6; }
                    else { tpage = ((object->image_row + hdr[3]) << 6) + 0x7806; }
                }
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.u = tu;
                prim->sprt.clut = tpage;
                prim->sprt.v = tv;
                prim->mode[0] = cell / 256 + 0xe100001d;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
