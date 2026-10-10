/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and register choice.
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test next
 * to it (difftest.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws the first tile layer
 * of the stage, data_801aa544[0], as sprite records. The layer's field_14
 * (high half) minus game_state.field_92 gives a vertical position y; its
 * field_12 gives a horizontal position. The window is 25 columns by 16
 * rows of 16-pixel cells, starting at the cell row y / 16 and the cell
 * column (field_12 masked by field_58 - 1) / 16, offset by the sub-cell
 * remainders. The cells are 16-bit pairs (tile, palette) in the layer's
 * map (field_50). Every cell whose tile differs from the layer's blank
 * value (field_1e, signed; the tile is compared unsigned) gets one record
 * of the table: position, texture coordinates, palette and a command
 * word; the record is linked into the list head chosen by the layer's
 * field_8a. Unlike the flat models of slot06_00, there is no test of the
 * row against a row count.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   No argument, no return value.
 *   Reads: data_801aa544[0] field_12, field_14, field_1e, field_50,
 *     field_58, field_8a; game_state.field_92; the buffer selector
 *     data_801a27d0 (0 or 1); the list base pointer data_801987c8 and the
 *     list head word it points at; the map words of the cells it visits.
 *   Map, at field_50: for map row r (r & 0xf) the row starts 64 cells
 *     (128 bytes) in; the cell column c lies at (c >> 5) * 1024 + (c & 0x1f)
 *     * 2 halfwords, so one cell of the map is a (tile, palette) pair.
 *     The setup keeps every visited cell inside a map block it allocated.
 *   Record table data_801ef554_slot06_0e: 2 buffers of 208 records of 28
 *     bytes. Up to 16 * 25 = 400 records can be written and the original
 *     does not check the count; the setup keeps buffer start plus records
 *     written within the two buffers (a run past the table is excluded).
 *   Writes: for each drawn cell one record (offsets 0, 4, 0x18 to 0x23 as
 *     the layout of Slot06Tile has them), the list head word. Nothing else.
 *   Aliasing: the layer, map, list array and record table are distinct
 *     blocks.
 *   Instruction slots no input can reach: one, at +0x90 of the original; it
 *     is a fix-up of the sign of y & 0x1f0 before its division by 16, a
 *     value that is never negative. The original
 *     also forms a term of the row's bit 15 (the map half) that is always 0
 *     because the row never exceeds 46; it is not written here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ef554_slot06_0e[2][0xd0];

void func_801e84cc_slot06_0e(void) {
    Chan *c = data_801aa544;
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
    Slot06Tile *rec;
    u32 *ot;

    pos = -(c->field_14 - 0x100000);
    y = (pos >> 16) - game_state.field_92;
    sy = y & 0xf;
    row = (y & 0x1f0) / 16;
    w = c->field_58;
    mask = (w >> 4) - 1;
    px = c->field_12;
    sx = px & 0xf;
    col = (px & (w - 1)) >> 4;
    blank = c->field_1e;
    rec = data_801ef554_slot06_0e[data_801a27d0];
    ot = (u32 *)((u8 *)data_801987c8 + c->field_8a * 4);
    for (i = 0; i < 16; i++, row++) {
        cc = col;
        line = c->field_50 + ((row & 0xf) << 6);
        for (k = 0; k < 25; k++) {
            e = line + ((cc >> 5) << 10) + ((cc & 0x1f) << 1);
            if (*e != blank) {
                tile = *e;
                rec->sprt.x = (k << 4) - sx;
                rec->sprt.y = (i << 4) - sy;
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
