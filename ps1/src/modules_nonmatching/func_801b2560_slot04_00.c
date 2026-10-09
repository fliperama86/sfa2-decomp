/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code loads the two copies of field_12a in the other order. The PS1
 * build keeps the raw bytes of the module image and does not use this file.
 * The differential test next to it (difftest.py, with
 * func_801b2560_slot04_00.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that moves the object along field_14 with a velocity
 * (field_50) and an acceleration (field_58). When the low byte of field_3a is
 * 0 it sets field_45 to 1, subtracts field_50 from the 32-bit value at
 * field_14, adds field_58 to field_50, and if field_50 is then negative it
 * adds 1 to field_07, stores field_12a + 1 in field_1a4 and starts sequence
 * (field_12a >> 1) + 0x32. In every other case it hands over to
 * func_80130efc.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the low byte of field_3a (the byte at 0x3a), the 32-bit word at
 *     field_14, field_50, field_58, field_07, field_12a.
 *   Writes (first arm): field_45, the word at field_14, field_50, and when
 *     field_50 is negative field_07 and field_1a4, plus what the callees
 *     write.
 *   Callees: func_801307e0 (2 arguments) and func_80130efc (1 argument) are
 *     replaced by recorders returning a random word, in the original and in
 *     this C alike; they reach the sequence code and the shared state code
 *     of the resident image.
 *   Aliasing: only the object is a block; nothing else is written.
 *   Watched by the recorders: the whole object (0x394 bytes) at every call,
 *     so the order of this function's stores against the calls is tested.
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Inputs excluded: none (field_50 + field_58 is kept within 31 bits so the
 *     sum does not overflow; the sum is a plain add in both codes).
 *   Slots not reached: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2560_slot04_00(Object *obj) {
    if ((u8)obj->field_3a == 0) {
        obj->field_45 = 1;
        /* The word at field_14 is a 32-bit value (inferred); the header
           declares only its low half. */
        *(s32 *)&obj->field_14 -= obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->field_50 < 0) {
            obj->field_07++;
            ((Slot04aObj *)obj)->field_1a4 = obj->field_12a + 1;
            func_801307e0(obj, (obj->field_12a >> 1) + 0x32);
            return;
        }
    }
    func_80130efc(obj);
}
