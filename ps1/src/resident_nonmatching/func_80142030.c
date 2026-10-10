/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has the original's size but differs in which registers hold
 * three locals (read from the original's listing, not tested). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (func_80142030.py, run by difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): looks the object up in a
 * coarse grid around the reference object. The grid column comes from the
 * object's field_21e plus 0x10 (32 units a column), the row from the
 * absolute vertical distance to ref_other (16 units a row, 4 columns to a
 * row of the table). If the column is above 0x100, or the distance is 0x80
 * or more (as a signed 16-bit number), or the cell's index byte has bit 7
 * set, it returns 0; a column or row below zero is not refused. Otherwise
 * it copies the four bytes of the indexed record to field_128, field_129,
 * field_12a and field_219 and returns 1.
 *
 * Contract:
 *   Argument: a0 = object. Result: v0 = 0 or 1 (a byte).
 *   Reads: object->pos_y, object->field_21e, ref_other.p->pos_y,
 *     table_8017ac54 (cell index bytes) and table_8017ac7c (4-byte records,
 *     indexed by a cell byte below 0x80).
 *   Writes: object->field_128, field_129, field_12a, field_219 (only when
 *     returning 1).
 *   The 16-bit quantities wrap as the original's do: a vertical distance of
 *     exactly 0x8000 becomes -0x8000 after the absolute value, which passes
 *     the `< 0x80` test, and a column or row below zero indexes before the
 *     table. The setup includes such inputs; the table then holds whatever
 *     the data before it holds (the resident image's real contents).
 *   No callee. Aliasing: the object and ref_other's object are distinct
 *     blocks.
 *   Exclusions: none. All instruction slots are reachable (inferred).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80142030(Object *object) {
    s16 dy = object->pos_y - ref_other.p->pos_y;
    s16 col = (u16)object->field_21e + 0x10;
    u8 cell;

    if (dy < 0) {
        dy = -dy;
    }
    if (col > 0x100 || dy >= 0x80) {
        return 0;
    }
    cell = table_8017ac54[(col >> 5) + (dy >> 4) * 4];
    if (cell & 0x80) {
        return 0;
    }
    object->field_128 = table_8017ac7c[cell][0];
    object->field_129 = table_8017ac7c[cell][1];
    object->field_12a = table_8017ac7c[cell][2];
    object->field_219 = table_8017ac7c[cell][3];
    return 1;
}
