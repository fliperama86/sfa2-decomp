/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register use (the
 * original keeps the object and the frame record in the other two
 * temporaries; read from the original's listing, not tested). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (difftest.py, with func_8013fc18.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): tests whether the object's
 * "other" object (the one the object's field_40 points at, which it also
 * stores in the global ref_other) is within reach horizontally, and if so
 * calls func_8013fab4. It returns 0 (no) when: the other object's field_249
 * or field_27b is set, its halfword at offset 4 is not 1, the configuration
 * block's field_4e is set, the other object's field_45 is set, or its
 * frame record has no box flag (box_a, box_b and box_c all 0). Then it
 * takes the distance of the two objects' x positions: the object's x plus a
 * (negated when the object's field_0b is set), minus the other object's x
 * plus the half-width of its box (box origin minus extent, negated when the
 * other's field_158 is set), as a signed 16-bit number made positive. A
 * distance greater than c returns 0. When the frame record's field_06 is
 * neither 0 nor 4 and the other object's field_d8 or field_157 is set it
 * returns -1. Otherwise it calls func_8013fab4(object) and returns 1.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = a (s16), a2 = c (s16), passed
 *     sign-extended. Result in v0: 0, -1 or 1.
 *   Reads: the object's field_40, field_0b and pos_x; the other object's
 *     field_249, field_27b, halfword at 4, field_45, frame, box pointer
 *     (offset 0x148), field_158, pos_x, field_d8, field_157; game_state's
 *     config pointer and its field_4e; the frame record's box_a, box_b,
 *     box_c and field_06; the box's origin and extent.
 *   Writes: ref_other.p (always).
 *   Callee: func_8013fab4 (takes the object; its result is unused) is
 *     replaced by a recorder with result 0. Its real code writes into the
 *     other object and calls further code (read from the original's
 *     listing, not tested). Watched at every call: the
 *     global ref_other, the object and the other object, whole.
 *   Aliasing: the object, the other object, the frame record, the box and
 *     the configuration block are distinct blocks.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 * The tree declares func_8013fc18 with an s16 result and int arguments; the arguments are narrowed to s16 at entry.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

s16 func_8013fc18(Object *object, int arg_a, int arg_c) {
    s16 a = arg_a;
    s16 c = arg_c;
    Object *other;
    FrameRecord *frame;
    Box6 *box;
    s16 half_width;
    s16 distance;

    ref_other.p = object->other;
    other = ref_other.p;
    frame = other->frame;
    if (other->field_249 != 0) {
        return 0;
    }
    if (other->field_27b != 0) {
        return 0;
    }
    if (*(u16 *)&other->field_04 != 1) { /* inferred: a halfword at offset 4 */
        return 0;
    }
    if (game_state.config->field_4e != 0) {
        return 0;
    }
    if (other->field_45 != 0) {
        return 0;
    }
    if ((frame->box_a | frame->box_b | frame->box_c) == 0) {
        return 0;
    }
    if (object->field_0b != 0) {
        a = -a;
    }
    box = (Box6 *)other->unknown_148;
    half_width = box->origin - box->extent;
    if (other->field_158 != 0) {
        half_width = -half_width;
    }
    distance = a + object->pos_x - (half_width + other->pos_x);
    if (distance < 0) {
        distance = -distance;
    }
    if (distance > c) {
        return 0;
    }
    if (frame->field_06 != 0 && frame->field_06 != 4) {
        if (other->field_d8 != 0) {
            return -1;
        }
        if (other->field_157 != 0) {
            return -1;
        }
    }
    func_8013fab4(object);
    return 1;
}
