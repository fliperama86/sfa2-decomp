/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code keeps the sum of field_4c and field_54 and its negation in
 * other registers. The PS1 build keeps the raw bytes of the module image
 * and does not use this file. The differential test next to it
 * (difftest.py, with func_801b1c28_slot04_08.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * (Same code as func_801b1884_slot04_08, at another address of the image.)
 *
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that moves the object by a velocity. It counts down the
 * byte at 0x1c8 and clears field_17b when the count reaches 0; adds field_4c
 * to the 32-bit word at field_10; adds field_54 to field_4c; adds 1 to
 * field_07 when the new field_4c is negative (field_0b is 0 and the sign is
 * tested on the negated value, so then when it is positive). It ends by
 * handing over to func_80130efc.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the byte at 0x1c8, field_10 (as a 32-bit word), field_4c,
 *     field_54, field_0b, field_07.
 *   Writes: the byte at 0x1c8, field_17b (only when the count is 0 after
 *     the decrement), the word at field_10, field_4c, field_07 (only when
 *     the sign test holds), plus what the callee writes.
 *   Callee: func_80130efc (1 argument) is replaced by a recorder returning
 *     a random word, in the original and in this C alike; it reaches the
 *     shared state code of the resident image.
 *   Aliasing: only the object is a block; nothing else is written.
 *   Watched by the recorder: the whole object (0x394 bytes) at the call,
 *     so the order of this function's stores against the call is tested.
 *     The callee gets no pointer to memory filled for the call.
 *   Inputs excluded: none. The sums are plain 32-bit additions in both
 *     codes; field_4c and field_54 are kept within 30 bits so that the
 *     sums do not overflow.
 *   Slots not reached: none (all 36 slots are executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c28_slot04_08(Object *obj) {
    s32 v;

    ((Slot04aObj *)obj)->field_1c8--;
    if (((Slot04aObj *)obj)->field_1c8 == 0) {
        obj->field_17b = 0;
    }
    /* The word at field_10 is a 32-bit value (inferred); the header
       declares only its low half. */
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    v = obj->field_4c;
    if (obj->field_0b == 0) {
        v = -v;
    }
    if (v < 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}
