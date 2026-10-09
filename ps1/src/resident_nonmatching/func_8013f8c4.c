/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register use (the
 * original keeps the two arguments and a halfword in other temporaries).
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (difftest.py, with func_8013f8c4.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not original names): a relative of
 * func_8013f474 and func_8013fc18, for objects of two kinds told apart by
 * their field_49.
 *   field_49 non-zero: when the object's halfword at offset 4 is 1 and its
 *     field_06 is 7, it returns func_8013f474(object, a, b) as a byte; when
 *     the halfword is 1 and field_06 is not 7 it returns 0; when the
 *     halfword is not 1 it returns 1 (see the judgment call below).
 *   field_49 zero: the "other" object (field_40, stored in the global
 *     ref_other) is tested like in func_8013fc18: refusals (return 0) when
 *     its field_249 or field_27b is set, its halfword at offset 4 is not 1,
 *     the configuration block's field_4e is set, its field_45 is set, or
 *     its frame record has no box flag. The distance is the object's x plus
 *     a (a negated when the object's field_0b is set), minus the other's x
 *     plus the half-width of the frame's box number field_07 (origin minus
 *     extent, negated when the OTHER's field_0b is set), as a signed 16-bit
 *     number made positive. b is the limit; it grows by 4 when the
 *     object's field_25e is non-zero, except when the object's halfword at
 *     offset 4 is 1 and its field_06 is 7 or 8. A distance greater than b
 *     returns 0; otherwise the result is func_8013fab4(object) as a byte.
 *
 * Judgment call (flagged): in the first kind with the halfword not 1, the
 * original returns whatever v0 holds, which is the constant 1 the compare
 * has just loaded. The C returns 1 explicitly. Inferred from the listing;
 * the true source may have fallen off the end.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = a (s16), a2 = b (s16), passed
 *     sign-extended. Result in v0 (a byte, or 0 or 1).
 *   Reads: as listed above; the object's field_49, halfword at 4, field_06,
 *     field_40, field_0b, field_25e, pos_x; for the other object field_249,
 *     field_27b, halfword at 4, field_45, frame, box array pointer (0x148),
 *     field_0b, pos_x; game_state's config pointer and its field_4e; the
 *     frame record's box flags and field_07; the box's origin and extent.
 *   Writes: ref_other.p (in the field_49 zero kind), and whatever
 *     func_8013f474 writes.
 *   Callees: func_8013f474 runs as the original code in both runs;
 *     func_8013fab4 (takes the object, returns a random 32-bit number per
 *     case) is replaced by a recorder. Watched at every call: ref_other,
 *     the object and the other object, whole.
 *   Aliasing: the object, the other object, the frame record, the box
 *     array and the configuration block are distinct blocks.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8013f8c4(Object *object, s16 a, s16 b) {
    Object *other;
    FrameRecord *frame;
    Box6 *box;
    s16 half_width;
    s16 distance;

    if (object->field_49 != 0) {
        if (*(u16 *)&object->field_04 != 1) { /* inferred: a halfword at offset 4 */
            return 1;
        }
        if (object->field_06 != 7) {
            return 0;
        }
        return (u8)func_8013f474(object, a, b);
    }
    ref_other.p = object->other;
    other = ref_other.p;
    if (other->field_249 != 0) {
        return 0;
    }
    if (other->field_27b != 0) {
        return 0;
    }
    if (*(u16 *)&other->field_04 != 1) {
        return 0;
    }
    if (game_state.config->field_4e != 0) {
        return 0;
    }
    if (other->field_45 != 0) {
        return 0;
    }
    frame = other->frame;
    if ((frame->box_a | frame->box_b | frame->box_c) == 0) {
        return 0;
    }
    if (object->field_0b != 0) {
        a = -a;
    }
    box = (Box6 *)other->unknown_148 + frame->field_07;
    half_width = box->origin - box->extent;
    if (!(*(u16 *)&object->field_04 == 1 && (object->field_06 == 7 || object->field_06 == 8))) {
        if (object->field_25e != 0) {
            b = b + 4;
        }
    }
    if (other->field_0b != 0) {
        half_width = -half_width;
    }
    distance = a + object->pos_x - (half_width + other->pos_x);
    if (distance < 0) {
        distance = -distance;
    }
    if (b < distance) {
        return 0;
    }
    return (u8)func_8013fab4(object);
}
