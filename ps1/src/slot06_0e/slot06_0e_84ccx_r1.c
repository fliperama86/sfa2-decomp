/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ef554_slot06_0e[2][0xd0];

/*
 * What it does (inferred, not an original name): draws the first tile layer
 * of the stage, data_801aa544[0], as sprite records. The layer's scroll_y
 * (high half) minus game_state.scroll_y gives a vertical position y; its
 * scroll_x gives a horizontal position. The window is 25 columns by 16
 * rows of 16-pixel cells, starting at the cell row y / 16 and the cell
 * column (scroll_x masked by map_width - 1) / 16, offset by the sub-cell
 * remainders. The cells are 16-bit pairs (tile, palette) in the layer's
 * map (map_data). Every cell whose tile differs from the layer's blank
 * value (field_1e, signed; the tile is compared unsigned) gets one record
 * of the table: position, texture coordinates, palette and a command
 * word; the record is linked into the list head chosen by the layer's
 * ot_index. Unlike the flat models of slot06_00, there is no test of the
 * row against a row count.
 */
void func_801e84cc_slot06_0e(void) {
    Chan *c;
    u32 w;
    int y;
    u16 row;
    int col;
    int i;
    int k;
    u16 *line;
    u16 *e;
    Slot06Tile *rec;
    u32 *ot;
    s16 blank;
    u16 sy;
    u16 sx;
    u16 mask;
    u16 cc;
    s16 px;
    int pos;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    /* Holds the row's vertical offset once per row for the inner loop; computing (i << 4) - sy at the store gives 2 differing slots. */
    int yy;

    c = data_801aa544;
    px = c->scroll_x;
    blank = c->field_1e;
    pos = c->scroll_y;
    w = c->map_width;
    pos += 0xfff00000;
    pos = -pos;
    pos >>= 16;
    pos -= game_state.scroll_y;
    mask = (w >> 4) - 1;
    sx = px & 0xf;
    y = (s16)pos;
    sy = y & 0xf;
    col = (px & (w - 1)) >> 4;
    row = (y & 0x1f0) / 16;
    rec = data_801ef554_slot06_0e[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->ot_index * 4);
    for (i = 0; i < 16; i++, row++) {
        cc = col;
        /* Stand-in, not recovered source: the three-way test below, whose three arms are the same statement and whose two conditions are those of the sibling func_801e9738_slot06_0e, reproduces the original's choice of registers. Without it this function differs from the original in 56 instruction slots. */
        /* Inferred: the compiler joins arms that came out as the same code after it has given the registers, and deletes the tests with what computed them, so the original's instructions show no test; the references in the arms still counted when the registers were given. Which tests the original source had cannot be read from the listing; the sibling's are the nearest evidence. */
        if (((Slot06Layer *)c)->scroll_y < 0x100) {
            line = c->map_data + ((row >> 15) & 1) + ((row & 0xf) << 6);
        } else if (((Slot06Layer *)c)->scroll_y >= 0x100 && ((Slot06Layer *)c)->scroll_y < 0x600) {
            line = c->map_data + ((row >> 15) & 1) + ((row & 0xf) << 6);
        } else {
            line = c->map_data + ((row >> 15) & 1) + ((row & 0xf) << 6);
        }
        yy = (i << 4) - sy;
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            if (*e != blank) {
                u16 tile;
                tile = *e;
                rec->sprt.x = (k << 4) - sx;
                rec->sprt.y = yy;
                rec->sprt.u = (tile << 3) & (-8);
                rec->sprt.v = (((s16)tile) >> 2) & (-8);
                rec->sprt.clut = e[1];
                rec->mode[0] = 0xe1000000 + (((s16)tile) >> 10);
                ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)rec;
                rec++;
            }
            cc = mask & (cc + 1);
        }
    }
}
