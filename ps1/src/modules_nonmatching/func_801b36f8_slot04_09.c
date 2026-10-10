/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 24 bytes shorter: the original starts sequence 0x33 once in
 * each arm of the field_a3 test, this C once after both (read from the
 * original's listing, not tested). The PS1 build keeps the raw bytes of the
 * module
 * image and does not use this file. The differential test next to it
 * (difftest.py, with func_801b36f8_slot04_09.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that has another object (field_40, "other") in view. It
 * first runs func_801b2858_slot04_09 (the move: it subtracts field_50 from
 * the word at field_14, adds field_58 to field_50, adds field_4c to the
 * word at field_10 and field_54 to field_4c; read from the original's
 * listing, not tested). Then, when field_3a is
 * negative as a halfword: it sets field_45 to 1, field_50 to 0x90000 and
 * field_58 to -0x6000, adds 1 to field_07, and sets a velocity (field_4c,
 * field_54): when field_a3 is 0 a fixed one whose sign follows field_0b
 * (0xe0000 and -0x8000 when field_0b is not 0, 0xfff20000 and 0x8000
 * otherwise); when field_a3 is not 0 a velocity toward other, field_4c =
 * ((other's pos_x - pos_x) >> 4) << 16 and field_54 = 0. It starts
 * sequence 0x33 in both cases. When field_3a is not negative: if field_67
 * is not 0 and other's field_61 is not 0xff it sets field_a3 to 0x80 and
 * clears field_67; if other's pos_x - pos_x + 0x24 is below 0x48 (signed)
 * it clears field_4c and field_54; then it hands over to func_80130efc.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_3a, field_a3, field_0b, field_67, field_07, pos_x, the
 *     pointer field_40 (other), and of other: pos_x, field_61.
 *   Writes: field_45, field_50, field_58, field_07, field_4c, field_54,
 *     field_a3, field_67, plus what the callees write (the move writes
 *     the words at field_14, field_50, field_10 and field_4c, read from the
 *     original's listing, not tested).
 *   Callees: func_801b2858_slot04_09 (1 argument) runs as the original
 *     code in both runs; it touches only the object (read from the
 *     original's listing, not tested). func_801307e0 (2
 *     arguments) and func_80130efc (1 argument) are replaced by recorders
 *     returning a random word, in the original and in this C alike; they
 *     reach the sequence code and the shared state code of the resident
 *     image (read from the original's listing, not tested).
 *   Aliasing: the object and other are distinct blocks of 0x394 bytes.
 *   Watched by the recorders: the whole object and the whole of other at
 *     every call, so the order of this function's stores against the calls
 *     is tested. No recorded callee gets a pointer to memory filled for the
 *     call.
 *   Inputs excluded: none. The sums of the move are plain 32-bit additions
 *     in both codes (read from the original's listing, not tested).
 *   Slots not reached: none (all 78 slots are executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *obj);

void func_801b36f8_slot04_09(Object *obj) {
    Object *other;

    func_801b2858_slot04_09(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_45 = 1;
        obj->field_50 = 0x90000;
        obj->field_58 = -0x6000;
        obj->field_07++;
        if (obj->field_a3 == 0) {
            if (obj->field_0b != 0) {
                obj->field_4c = 0xe0000;
                obj->field_54 = -0x8000;
            } else {
                obj->field_4c = 0xfff20000;
                obj->field_54 = 0x8000;
            }
        } else {
            other = obj->other;
            obj->field_4c = ((other->pos_x - obj->pos_x) >> 4) << 16;
            obj->field_54 = 0;
        }
        func_801307e0(obj, 0x33);
    } else {
        other = obj->other;
        if (obj->field_67 != 0 && other->field_61 != 0xff) {
            obj->field_a3 = 0x80;
            obj->field_67 = 0;
        }
        if (other->pos_x - obj->pos_x + 0x24 < 0x48) {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
        func_80130efc(obj);
    }
}
