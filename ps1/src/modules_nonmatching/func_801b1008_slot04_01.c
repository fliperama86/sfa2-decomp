/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original keeps the constant 1 in a register across the stores of 7 and 8
 * and stores it afterwards. The PS1 build keeps the raw bytes of the module
 * image and does not use this file. The differential test next to it
 * (difftest.py, with func_801b1008_slot04_01.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a state entry of a
 * character. It sets the state bytes (field_04 = 1, field_05 = 0,
 * field_06 = 7, field_07 = 0), copies field_158 to field_0b, sets
 * field_159 = 1, field_15a = 8, clears field_157 and field_6b, writes 0x19
 * into field_6b of the object pointed to by field_40 (the other object) and
 * 0x1d into field_27b, then starts sequence 0x41 with func_801307e0.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_158, field_40 (pointer to another object).
 *   Writes: the fields above of the object, field_6b of the other object.
 *   Callee: func_801307e0 (2 arguments) is replaced by a recorder returning
 *     a random word, in the original and in this C alike; it reaches the
 *     sequence code of the resident image.
 *   Aliasing: the other object is a separate block.
 *   Watched by the recorder: the whole object (0x394 bytes) and the whole
 *     other object, at the call, so the stores before the call are tested.
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Inputs excluded: none. Slots not reached: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1008_slot04_01(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_0b = obj->field_158;
    obj->field_159 = 1;
    obj->field_15a = 8;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x19;
    obj->field_27b = 0x1d;
    func_801307e0(obj, 0x41);
}
