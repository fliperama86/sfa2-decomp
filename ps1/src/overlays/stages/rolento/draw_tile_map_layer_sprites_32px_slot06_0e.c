/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot06TileW data_801f6734_slot06_0e[2][0x75];

/*
 * What it does (inferred, not an original name): draws the third tile layer
 * of the stage, data_801aa544[2], as sprite records. The layer's scroll_y
 * (high half) minus game_state.scroll_y gives a vertical position y; its
 * scroll_x gives a horizontal position. The window is 13 columns by 9
 * rows of 32-pixel cells, starting at the cell row (y & 0x1f0) / 32 and the
 * cell column (scroll_x masked by map_width - 1) / 32, offset by the
 * sub-cell remainders. The cells are 16-bit pairs (tile, palette) in the
 * layer's map (map_data); which half of the map a row lies in depends on
 * the layer's scroll_y and on bit 3 of the row (a map of 8 rows per half).
 * Every cell whose tile equals the layer's blank value (field_1e) is
 * skipped; every other gets one record of the table (position, texture
 * coordinates, palette, command word), linked into the list head chosen by
 * the layer's ot_index. There is no test of the row against a row count.
 */
void draw_tile_map_layer_sprites_32px_slot06_0e(void) {
    Chan *c = &data_801aa544[2];
    int pos;
    int y;
    s16 px;
    u32 w;
    u32 col;
    u16 mask;
    u16 cc;
    s16 blank;
    u16 sx;
    u16 sy;
    u16 row;
    int i;
    int k;
    u16 *line;
    u16 *e;
    s16 tile;
    Slot06TileW *rec;
    u32 *ot;
    u16 f16;
    int yy;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    px = c->scroll_x;
    blank = c->field_1e;
    pos = c->scroll_y;
    w = c->map_width;
    /* The shift and the subtraction are done in pos, in two statements, and y is cast in a statement of its own behind mask and sx. Written as y = (s16)((pos >> 16) - game_state.scroll_y); in one statement, this function differs from the original in 16 instruction slots. Inferred, not shown by a dump: a value that is assigned several times is not moved next to its reader by the first scheduling pass, so the sets of mask and sx stand before the cast and their lives are longer by the few instructions that decide the rank of three saved registers. */
    pos += 0xfff00000;
    pos = -pos;
    pos >>= 16;
    pos -= game_state.scroll_y;
    mask = (w >> 5) - 1;
    sx = px & 0x1f;
    y = (s16)pos;
    sy = y & 0x1f;
    col = (px & (w - 1)) >> 5;
    row = (y & 0x1f0) / 32;
    rec = data_801f6734_slot06_0e[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->ot_index * 4);
    for (i = 0; i < 9; i++, row++) {
        cc = col;
        /* The term (row >> 12) & 0xf in every arm and the test f16 >= 0x100 are instructions of the original (a shift by 11 with the mask 0x1e, and 0x100 taken off before the compare). row is at most 15 + 8 here, so the term is 0, and the test is true where it is reached. */
        f16 = ((Slot06Layer *)c)->scroll_y;
        if (f16 < 0x100) {
            if (row & 8) {
                line = c->map_data + (((row >> 12) & 0xf) + 0x200) + ((row & 7) << 5);
            } else {
                line = c->map_data + ((row >> 12) & 0xf) + ((row & 7) << 5);
            }
        } else if (f16 >= 0x100 && f16 < 0x600) {
            line = c->map_data + (((row >> 12) & 0xf) + 0x200) + ((row & 7) << 5);
        } else {
            if (row & 8) {
                line = c->map_data + ((row >> 12) & 0xf) + ((row & 7) << 5);
            } else {
                line = c->map_data + (((row >> 12) & 0xf) + 0x200) + ((row & 7) << 5);
            }
        }
        /* The row offset is computed once per row; written in place of yy at the store, this function differs from the original in 2 instruction slots. */
        yy = (i << 5) - sy;
        for (k = 0; k < 13; k++) {
            e = line + ((cc >> 4) << 8) + ((cc & 0xf) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 5) - sx;
                rec->sprt.y = yy;
                rec->sprt.u = ((u16)tile << 3) & -8;
                rec->sprt.v = (tile >> 2) & -8;
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
