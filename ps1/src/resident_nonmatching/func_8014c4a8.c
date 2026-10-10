/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice. The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (func_8014c4a8.py, run by difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the same distance as
 * func_8014362c, for the object's partner (object->other) and a box chosen
 * by the animation frame of the object that ref_second points at. The
 * result goes to the global data_80189464 instead of a field. The partner's
 * field_0b selects the mirrored arm. The distance: the object's x minus the
 * sum of the partner's x and the box half-width (origin minus the extent
 * byte, negated when the partner's field_0b is non-zero), as a 16-bit
 * number made positive.
 *
 * Contract:
 *   Argument: a0 = object. No return value.
 *   Reads: ref_second.p (an object) and from it frame->active (an index) and
 *     box_tables (read here as a table of 6-byte box records, origin at 0,
 *     extent byte at 4; the declared type is BoxTables*, the code indexes it
 *     directly); object->other, other->field_0b, other->pos_x,
 *     object->pos_x.
 *   Writes: data_80189464 (16 bits) only.
 *   Aliasing: object, other, the reference object, its frame record and
 *     the box table are distinct blocks.
 *   Exclusions: none. No callee. All instruction slots are reachable
 *     (inferred).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8014c4a8(Object *object) {
    Object *other = object->other;
    Box6 *box = (Box6 *)ref_second.p->box_tables + ref_second.p->frame->active;
    int origin = (u16)box->origin;
    int extent = -box->extent;
    int diff;

    if (other->field_0b != 0) {
        origin = -origin;
        extent = -extent;
    }
    diff = (u16)object->pos_x - (origin + extent + (u16)other->pos_x);
    if ((s16)diff < 0) {
        diff = -diff;
    }
    data_80189464 = diff;
}
