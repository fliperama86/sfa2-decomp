/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register use (the
 * original copies the arguments into temporaries and keeps the absolute
 * distance apart from the difference). The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the resident executable; the build does
 * not use this file. The differential test next to it (difftest.py, with
 * func_8013f474.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not original names): a sibling of func_8013fc18.
 * It tests whether the object's "other" object (field_40, also stored in
 * the global ref_other) is within horizontal reach of a, and if so returns
 * what func_8013fab4(object) returns, truncated to a byte; every refusal
 * returns 0. Refusals, in order: the object's field_49 is 0; the object's
 * field_7e is 0 and the configuration block's field_4e is set; the other
 * object's field_27b is set; its frame record has no box flag (box_a, box_b
 * and box_c all 0); the frame record's field_06 is not 0; the distance is
 * greater than limit; the other object's field_45 is set. The distance is
 * as in func_8013fc18, except that the box is the entry number field_07 of
 * the other object's array of 6-byte boxes (its field_148): the object's x
 * plus a (negated when the object's field_0b is set), minus the other
 * object's x plus the box's half-width (origin minus extent, negated when
 * the other's field_158 is set), as a signed 16-bit number made positive.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = a (s16), a2 = limit (s16), passed
 *     sign-extended. Result in v0: 0 or the low byte of func_8013fab4's.
 *   Reads: the object's field_49, field_7e, field_40, field_0b, pos_x; the
 *     config pointer of game_state and its field_4e; the other object's
 *     field_27b, frame, box array pointer, field_158, pos_x, field_45; the
 *     frame record's box_a, box_b, box_c, field_06, field_07; the box's
 *     origin and extent.
 *   Writes: ref_other.p, once the first two checks pass.
 *   Callee: func_8013fab4 (takes the object) is replaced by a recorder
 *     whose result is a random 32-bit number per case (the function keeps
 *     its low byte). Watched at every call: ref_other, the object and the
 *     other object, whole.
 *   Aliasing: the object, the other object, the frame record, the box
 *     array and the configuration block are distinct blocks.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8013f474(Object *object, s16 a, s16 limit) {
    Object *other;
    FrameRecord *frame;
    Box6 *box;
    s16 half_width;
    s16 distance;

    if (object->field_49 == 0) {
        return 0;
    }
    if (object->field_7e == 0 && game_state.config->field_4e != 0) {
        return 0;
    }
    ref_other.p = object->other;
    other = ref_other.p;
    if (other->field_27b != 0) {
        return 0;
    }
    frame = other->frame;
    if (frame->box_a == 0 && frame->box_b == 0 && frame->box_c == 0) {
        return 0;
    }
    if (frame->field_06 != 0) {
        return 0;
    }
    if (object->field_0b != 0) {
        a = -a;
    }
    box = (Box6 *)other->unknown_148 + frame->field_07;
    half_width = box->origin - box->extent;
    if (other->field_158 != 0) {
        half_width = -half_width;
    }
    distance = a + object->pos_x - (half_width + other->pos_x);
    if (distance < 0) {
        distance = -distance;
    }
    if (limit < distance) {
        return 0;
    }
    if (other->field_45 != 0) {
        return 0;
    }
    return (u8)func_8013fab4(object);
}
