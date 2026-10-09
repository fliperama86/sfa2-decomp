/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs in layout and size (the sequence start, written twice
 * in the original, is a helper here). The PS1 build keeps the raw bytes of the module image and does not use
 * this file. The differential test next to it (difftest.py, with
 * func_801b33b8_slot04_06.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that moves along a velocity. When field_4c is negative it
 * adds 1 to field_07 and starts sequence (field_12a >> 1) + 0x45. Otherwise
 * it calls func_801b4818_slot04_06 (the move: it adds field_4c, with the
 * sign set by field_0b, to the word at field_10 and field_54 to field_4c);
 * then looks at the object that field_40 points to ("other"). If other's
 * field_6b is non-zero and its field_61 is 1, then a negative halfword
 * field_5c of other starts the same sequence as above, and a non-negative
 * one adds 3 to field_07 and sets field_159 to 1. Every path except the
 * start of the sequence ends by handing over to func_80130efc.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_4c, field_07, field_12a, the pointer field_40 (other),
 *     and of other: field_6b, field_61, field_5c (halfword).
 *   Writes: field_07, field_159, and what the callees write (the move
 *     writes the word at field_10 and field_4c of the object).
 *   Callees: func_801b4818_slot04_06 (1 argument) runs as the original
 *     code in both runs; it touches only the object. func_801307e0 (2
 *     arguments) and func_80130efc (1 argument) are replaced by recorders
 *     returning a random word, in the original and in this C alike; they
 *     reach the sequence code and the shared state code of the resident
 *     image.
 *   Aliasing: the object and other are distinct blocks of 0x394 bytes.
 *   Watched by the recorders: the whole object and the whole of other at
 *     every call, so the order of this function's stores against the calls
 *     is tested. No recorded callee gets a pointer to memory filled for the
 *     call.
 *   Inputs excluded: none. field_4c and field_54 are kept within 30 bits so
 *     that the move's sum does not overflow.
 *   Slots not reached: none (all 45 slots are executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);

/* Starts the turn sequence: the same two steps in both places of the
   original (inferred role). */
static void start_sequence(Object *obj) {
    obj->field_07++;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x45);
}

void func_801b33b8_slot04_06(Object *obj) {
    Object *other;

    if (obj->field_4c < 0) {
        start_sequence(obj);
        return;
    }
    func_801b4818_slot04_06(obj);
    other = obj->other;
    if (other->field_6b != 0 && other->field_61 == 1) {
        if ((s16)other->field_5c < 0) {
            start_sequence(obj);
            return;
        }
        obj->field_07 += 3;
        obj->field_159 = 1;
    }
    func_80130efc(obj);
}
