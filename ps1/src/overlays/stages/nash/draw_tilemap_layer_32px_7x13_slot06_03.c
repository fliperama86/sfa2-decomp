/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot06TileW data_801f62c0_slot06_03[2][0x75];

/* Draws the layer of background_layers[2]: for 7 rows of 13 cells of 32 pixels,
   a tile record for each cell that is not blank, linked into the primitive
   list (inferred). The locals t, sx, sy, mask and cc are 16-bit, as in
   draw_tile_layer_0_16px_slot06_00. */
void draw_tilemap_layer_32px_7x13_slot06_03(void) {
    Chan *c = &background_layers[2];
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
    Slot06TileW *rec;
    u32 *ot;
    s16 blank;
    s16 rows;
    s16 sy;
    s16 sx;
    u16 mask;
    u16 cc;
    x = c->scroll_y - 0x100000;
    pos = -x;
    t = (pos >> 16) - game_state.scroll_y;
    pos &= 0xffff;
    pos |= t << 16;
    rows = c->map_height >> 5;
    mask = (c->map_width >> 5) - 1;
    sx = c->scroll_x & 0x1f;
    y = pos >> 16;
    sy = y & 0x1f;
    col = ((s16)c->scroll_x & (c->map_width - 1)) >> 5;
    blank = c->field_1e;
    if (y >= 0) {
        row = y >> 5;
    } else {
        row = (y - 31) / 32;
    }
    rec = data_801f62c0_slot06_03[draw_buffer_index];
    ot = (u32 *)((u8 *)current_ordering_table + c->ot_index * 4);
    for (i = 0; i < 7; i++, row++) {
        cc = col;
        if (row < 0 || rows < row) {
            continue;
        }
        line = c->map_data + ((u16)row >> 3) * ((c->map_width >> 9) << 8) + ((row & 7) << 5);
        for (k = 0; k < 13; k++) {
            s16 tile;

            e = line + ((cc >> 4) << 8) + ((cc & 0xf) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 5) - sx;
                rec->sprt.u = (tile << 3) & -8;
                rec->sprt.v = (tile >> 2) & -8;
                rec->sprt.y = (i << 5) - sy;
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
