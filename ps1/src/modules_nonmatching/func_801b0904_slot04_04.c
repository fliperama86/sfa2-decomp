/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 4 bytes shorter and differs in the placement of one move and
 * a padding nop. The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py, with func_801b0904_slot04_04.py)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * What it does (inferred, not an original name): a state entry of a
 * character. When the object's field_12a is 2 and the top nibble of its
 * field_130 is 4, it sets the four state bytes at 4..7, spends one point
 * (func_80141f28 with 1), clears field_12f and field_67 and starts sequence
 * 0x1d (field_48 not 0) or 0x1c. Otherwise it hands over to the module's
 * function func_801b09ac_slot04_04.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_12a, field_130, field_48.
 *   Writes (then arm): field_04 = 1, field_05 = 0, field_06 = 5, field_07 = 0,
 *     field_12f = 0, field_67 = 0, and whatever the callees write.
 *   Callees: func_80141f28 (2 arguments) and func_801307e0 (2 arguments) are
 *     replaced by recorders returning a random word, in the original and in
 *     this C alike; they reach the sound and sequence code of the resident
 *     image (read from the original's listing, not tested).
 *     func_801b09ac_slot04_04 (the else arm) runs as the original code; it
 *     reads field_12a, field_48, field_129 and the table data_801c4180 of
 *     the module, and calls the two recorded functions (read from the
 *     original's listing, not tested).
 *   Aliasing: only the object is a block; nothing else is written.
 *   Watched by the recorders: the whole object (0x394 bytes) at every call of the two recorders,
 *     so the order of this function's stores against the calls is tested.
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Inputs excluded: none. Slots not reached: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack it (inferred). */
void func_801b09ac_slot04_04(Object *obj);

void func_801b0904_slot04_04(Object *obj) {
    if (obj->field_12a == 2 && (obj->field_130 & 0xf000) == 0x4000) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 5;
        obj->field_07 = 0;
        func_80141f28(obj, 1);
        obj->field_12f = 0;
        obj->field_67 = 0;
        if (obj->field_48 != 0) {
            func_801307e0(obj, 0x1d);
        } else {
            func_801307e0(obj, 0x1c);
        }
    } else {
        func_801b09ac_slot04_04(obj);
    }
}
