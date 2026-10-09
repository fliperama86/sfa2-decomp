/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice. The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (func_8014362c.py, run by difftest.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): measures the horizontal
 * distance between two objects, allowing for a box of the second one. It
 * takes the second box record of the other object's box table, places the
 * box at the other object's x, mirrored when the object's field_158 is not 1,
 * and stores the absolute value of (object x - box edge) in field_21e.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = other. No return value.
 *   Reads: other->unknown_148 (a pointer to a table of 6-byte box records;
 *     the record at index 1 is used: origin at 0, extent byte at 4),
 *     other->pos_x, object->pos_x, object->field_158.
 *   Writes: object->field_21e (16 bits) only.
 *   Aliasing: object, other and the box table are distinct blocks.
 *   Exclusions: none. No callee. All instruction slots are reachable.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8014362c(Object *object, Object *other) {
    Box6 *box = (Box6 *)other->unknown_148 + 1;
    int origin = (u16)box->origin;
    int extent = -box->extent;
    int diff;

    if (object->field_158 != 1) {
        origin = -origin;
        extent = -extent;
    }
    diff = (u16)object->pos_x - (origin + extent + (u16)other->pos_x);
    if ((s16)diff < 0) {
        diff = -diff;
    }
    object->field_21e = diff;
}
