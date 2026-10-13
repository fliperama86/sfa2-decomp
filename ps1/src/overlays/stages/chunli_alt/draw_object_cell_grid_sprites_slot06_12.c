/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f6070_slot06_12[2][0x100];

/* What it does (inferred, not an original name): draws a tile map as sprite
   records, a relative of draw_object_tile_grid_sprites_slot06_00 with another record table, a
   parallax term in the first layer case and, for objects whose field_02 is
   0x9f, a texture page that depends on the row. The object selects a layer by
   its field_0e (8, 4 or 0xc pick one of three layer blocks; any other value
   uses no layer and keeps the object's position); the layer's scroll_x and
   scroll_y offset the object's position, and for 8 the position also moves by
   (layer scroll_x - layer ref_x) times the object's scroll_factor divided by
   128. When the object's field_02 is 0x9f the texture page is recomputed for
   each cell: the base page for row 0 and the page 0x40 higher for the other
   rows. A header reached through the object's sequence describes a grid of
   16-bit cells. For each non-zero cell (low 14 bits) it takes a record from
   the table, fills in position, texture coordinates, texture page and a
   command word, and links the record into the list head selected by the
   object. */
void draw_object_cell_grid_sprites_slot06_12(Object *object) {
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
    int tpage;
    u8 sel;
    s16 r;
    int off;
    int nib;
    int nib2;
    int tu;
    int tv;
    s16 hi;
    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)main_layer;
        x = object->pos_x - layer->scroll_x +
            ((s16)layer->scroll_x - (s16)layer->ref_x) * (s16)object->scroll_factor / 128;
        y = object->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)background_layers;
        x = object->pos_x - layer->scroll_x;
        y = object->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = object->pos_x - layer->scroll_x;
        y = object->pos_y + layer->scroll_y - 8;
    } else {
        x = object->pos_x;
        y = object->pos_y;
    }
    if (object->field_0f != 0) y = y + game_state.scroll_y;
    cnt = tile_sprite_count;
    hdr = (u8 *)object->sequence->data_ptr;
    ox = *(u16 *)(hdr + 4);
    oy = *(u16 *)(hdr + 6);
    tpage = ((object->image_row + hdr[3]) << 6) + 0x7806;
    ot = (u32 *)current_ordering_table + object->ot_depth;
    sel = *(u8 *)&draw_buffer_index;
    for (row = 0; row < hdr[2]; row++) {
        for (col = 0; col < hdr[0]; col++) {
            off = (col + row * hdr[0]) * 2;
            cell = hi = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                r = cell % 256;
                nib2 = r - (r / 16) * 16;
                nib = r / 16;
                prim = &data_801f6070_slot06_12[sel][(u16)cnt];
                if (object->field_02 == 0x9f) {
                    if (row == 0) {
                        tpage = ((object->image_row + hdr[3]) << 6) + 0x7806;
                    } else {
                        tpage = ((object->image_row + hdr[3]) << 6) + 0x7846;
                    }
                }
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.u = tu;
                prim->sprt.clut = tpage;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.v = tv;
                prim->mode[0] = (hi / 256) + 0xe100001c;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    tile_sprite_count = cnt;
}
