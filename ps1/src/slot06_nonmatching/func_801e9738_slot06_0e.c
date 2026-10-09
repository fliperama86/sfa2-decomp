/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and register choice.
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test next
 * to it (difftest.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws the third tile layer
 * of the stage, data_801aa544[2], as sprite records. The layer's field_14
 * (high half) minus game_state.field_92 gives a vertical position y; its
 * field_12 gives a horizontal position. The window is 13 columns by 9
 * rows of 32-pixel cells, starting at the cell row (y & 0x1f0) / 32 and the
 * cell column (field_12 masked by field_58 - 1) / 32, offset by the
 * sub-cell remainders. The cells are 16-bit pairs (tile, palette) in the
 * layer's map (field_50); which half of the map a row lies in depends on
 * the layer's field_16 and on bit 3 of the row (a map of 8 rows per half).
 * Every cell whose tile equals the layer's blank value (field_1e) is
 * skipped; every other gets one record of the table (position, texture
 * coordinates, palette, command word), linked into the list head chosen by
 * the layer's field_8a. There is no test of the row against a row count.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   No argument, no return value.
 *   Reads: data_801aa544[2] field_12, field_14, field_16, field_1e,
 *     field_50, field_58, field_8a; game_state.field_92; the buffer selector
 *     data_801a27d0 (0 or 1); the list base pointer data_801987c8 and the
 *     list head word it points at; the map cells it visits.
 *   Map, at field_50: the row half is 0 or 0x200 halfwords in (see below),
 *     row r (r & 7) 32 halfwords on, the column c at (c >> 4) * 256 +
 *     (c & 0xf) * 2 halfwords, a (tile, palette) pair per cell. The half
 *     is 0x200 when field_16 is 0x100 to 0x5ff or when bit 3 of the row
 *     is set for field_16 below 0x100, and when it is clear for field_16 of
 *     0x600 and above; it is 0 otherwise. The setup keeps every visited
 *     cell inside a map block it allocated.
 *   Record table data_801f6734_slot06_0e: 2 buffers of 117 records of 32
 *     bytes; at most 9 * 13 = 117 records are written, which fits a buffer.
 *   Writes: for each drawn cell one record (offsets 0, 4, 0x18 to 0x23 as
 *     the layout of Slot06TileW has them) and the list head word. Nothing
 *     else.
 *   Aliasing: the layer, map, list array and record table are distinct
 *     blocks.
 *   Instruction slots no input can reach: one, at +0x94 of the original; it
 *     is a fix-up of the sign of y & 0x1f0 before its division by 32, a
 *     value that is never negative. The original
 *     also forms a term of the row's bit 15 (the map half) that is always 0
 *     because the row never exceeds 46; it is not written here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06TileW data_801f6734_slot06_0e[2][0x75];

void func_801e9738_slot06_0e(void) {
    Chan *c = &data_801aa544[2];
    int pos;
    s16 y;
    s16 px;
    u32 w;
    u32 col;
    u16 mask;
    u16 cc;
    s16 blank;
    int sx;
    int sy;
    int row;
    int i;
    int k;
    u16 *line;
    u16 *e;
    s16 tile;
    Slot06TileW *rec;
    u32 *ot;

    pos = -(c->field_14 - 0x100000);
    y = (pos >> 16) - game_state.field_92;
    sy = y & 0x1f;
    row = (y & 0x1f0) / 32;
    w = c->field_58;
    mask = (w >> 5) - 1;
    px = c->field_12;
    sx = px & 0x1f;
    col = (px & (w - 1)) >> 5;
    blank = c->field_1e;
    rec = data_801f6734_slot06_0e[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 9; i++, row++) {
        cc = col;
        if (((Slot06Layer *)c)->field_16 < 0x100) {
            line = c->field_50 + ((row & 8) ? 0x200 : 0);
        } else if (((Slot06Layer *)c)->field_16 < 0x600) {
            line = c->field_50 + 0x200;
        } else {
            line = c->field_50 + ((row & 8) ? 0 : 0x200);
        }
        line += (row & 7) << 5;
        for (k = 0; k < 13; k++) {
            e = line + ((cc >> 4) << 8) + ((cc & 0xf) << 1);
            tile = *e;
            if (tile != blank) {
                rec->sprt.x = (k << 5) - sx;
                rec->sprt.y = (i << 5) - sy;
                rec->sprt.u = (tile << 3) & -8;
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
