/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice (172 bytes against the original's 196). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the module image; the
 * build does not use this file. The differential test next to it
 * (func_80079144_slot2b.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): places a follower object
 * relative to a target. The object's field_3c points at a parent; the
 * parent's `other` is the target. From the table of box records of the
 * parent's side (the left table in scratchpad when the parent's side is 0,
 * the right one otherwise), selected by the target's kind, it takes the
 * 6-byte record numbered by the parent's current frame's field_0b. The
 * object copies the target's field_0b; its pos_x becomes the target's
 * pos_x plus the record's first half plus the object's field_4e, with that
 * sum negated when the target's field_0b is not 0; its pos_y becomes the
 * target's pos_y minus the record's second half plus the high half of the
 * object's field_50. All position arithmetic is modulo 2^16.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_3c, field_4e, field_50; the parent's side
 *     (field_a6), other (field_40) and frame (field_88); the frame's
 *     field_0b; the target's kind (field_a7), field_0b, pos_x and pos_y;
 *     the scratchpad words boxes_74_left and boxes_124_right (pointers to
 *     arrays indexed by kind of pointers to record arrays); the first two
 *     halves of the selected record.
 *   Writes: the object's field_0b, pos_x and pos_y.
 *   Callees: none.
 *   Aliasing: the object, parent, target and frame record are distinct
 *     blocks. The pointer arrays and record arrays are distinct from them.
 *   Excluded inputs: none; kind and frame field_0b may be any byte, the
 *     setup fills 256 pointers per side and records for 256 indices.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80079144_slot2b(Object *obj) {
    Object *parent = obj->field_3c;
    Object *target = parent->other;
    Box6 **table;
    Box6 *record;
    int dx;

    if (parent->side == 0) {
        table = boxes_74_left;
    } else {
        table = boxes_124_right;
    }
    record = table[target->kind] + parent->frame->field_0b;
    obj->field_0b = target->field_0b;
    dx = record->origin + ((Slot2bObj *)obj)->field_4e;
    if (target->field_0b != 0) {
        dx = -dx;
    }
    obj->pos_x = dx + (u16)target->pos_x;
    obj->pos_y = (u16)target->pos_y - (record->field_02 + ((u32)obj->field_50 >> 16));
}
