/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice, saved
 * register order and instruction order. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): draws a tile map as
 * sprite records. A header reached through the object's sequence describes
 * a grid of 16-bit cells: byte 0 columns, byte 2 rows, byte 3 a texture
 * page offset, u16 at 4 and 6 the x and y origin, then rows * columns cells
 * from offset 8. For each cell whose low 14 bits are not 0 it initialises
 * a 0x1c-byte record through func_80136d1c, fills in the position (x:
 * the object's pos_x less box_margin, 16 pixels per column, less the x
 * origin; y: the object's pos_y plus data_801aa5ea less 8, 16 pixels per
 * row, less the y origin), the texture coordinates and command word from
 * the cell, grey colour 0x80 and the texture page, and links the record
 * into the list head selected by the object. Records are taken in order
 * from one of two buffers chosen by data_801a27d0.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: object field_09, field_0d, pos_x, pos_y, sequence->field_04 (the
 *     header and cells), box_margin and data_801aa5ea (halfwords), the
 *     low byte of data_801a27d0, the list base pointer data_801987c8 and
 *     the list head word.
 *   Writes: data_801e7668_slot0b (set to 0); for each drawn cell one record
 *     (offsets 4 to 7, 0x10 to 0x12, 0x14 to 0x1b; the first word is the
 *     link);
 *     the list head word.
 *   Callee replaced by a recorder (same in both runs): func_80136d1c (one
 *     argument, the record; it reaches Sony's library, so it is not run;
 *     read from the
 *     original's listing, not tested).
 *     The log copies at every call the 7 words of the record passed, the
 *     first 24 records of the buffer in use whole, the 16 list words and
 *     data_801e7668_slot0b.
 *   Aliasing: the object, the sequence step, the header, the list array and
 *     the record buffers are distinct blocks.
 *   Excluded inputs: grids of more than 24 drawn cells (the setup keeps
 *     rows * columns to 24; the record buffers hold 336 records each).
 *   Not reached by any input: two instruction slots of the original, at
 *     offsets 0x124 and 0x144, which correct the division by 256 and by 16
 *     for a negative value (read from the original's listing, not tested);
 *     a cell is masked to 14 bits and never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8019d7cc;
extern u32 data_801e7668_slot0b;

void func_801e1d6c_slot0b(Object *obj) {
    Tilemap *map = (Tilemap *)obj->sequence->field_04;
    u32 *ot = (u32 *)data_801987c8 + obj->field_09;
    u32 *p = (u32 *)&data_8019d7cc;
    TilePrim *q;
    s16 x0;
    s16 tpage;
    int y;
    int row;
    int col;
    int cell;

    tpage = ((obj->field_0d + map->field_03) << 6) + 0x7808;
    x0 = obj->pos_x - box_margin[0];
    y = obj->pos_y + data_801aa5ea[0] - 8;
    data_801e7668_slot0b = 0;
    if (*(u8 *)&data_801a27d0 != 0) {
        p = (u32 *)((u8 *)p + 0x24c0);
    }
    for (row = 0; row < map->rows; row++, y += 16) {
        for (col = 0; col < map->cols; col++) {
            cell = map->cells[row * map->cols + col] & 0x3fff;
            if (cell == 0) continue;
            func_80136d1c((Tx *)p);
            q = (TilePrim *)(p + 1);
            q->tpage = tpage;
            q->x = x0 + col * 16 - map->field_04;
            q->u = cell & 0xf0;
            q->v = (cell & 0xf) << 4;
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            q->y = y - map->field_06;
            q->cmd = 0xe1000006 + (cell >> 8);
            ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
            ((PrimTag *)ot)->addr = (u32)p;
            p += 7;
        }
    }
}
