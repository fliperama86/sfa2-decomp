/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f5bc0_slot06_0c[200][2];

/* What it does (inferred, not an original name): draws a tile map as sprite
   records, like draw_object_tile_grid_sprites_slot06_00, for the stage image 0c. It differs
   in the gate: it returns at once only when game_state.field_64 is not 0
   and game_state.field_226 is 0. The object selects a layer by its field_0e
   (8, 4 or 0xc pick one of three layer blocks, any other value uses no
   layer). For 4 and 0xc, and for no layer, the position is as in the stage
   00 function. For 8 the x position also gets a parallax term: the layer's
   scroll_x minus its ref_x, times the object's parallax_factor, divided by 41.
   A header reached through the object's sequence describes a grid of 16-bit
   cells. For each non-zero cell (low 14 bits) it takes a record from the
   table, fills in position, texture coordinates, texture page and a command
   word, and links the record into the list head selected by the object. */
void draw_tile_map_sprites_parallax_slot06_0c(Object *object) {
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

    if (game_state.field_64 != 0 && game_state.field_226 == 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)main_layer;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x +
            ((s16)layer->scroll_x - (s16)layer->ref_x) * (s16)((Slot06Obj *)object)->parallax_factor / 41;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)background_layers;
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
            cell = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                lo = cell % 256;
                nib = lo / 16;
                nib2 = lo - nib * 16;
                prim = &data_801f5bc0_slot06_0c[(u16)cnt][sel];
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
                cw = hi + 0xe100001b;
                cnt++;
                prim->mode[0] = cw;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    tile_sprite_count = cnt;
}
