/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size (4 bytes more) and register use (the
 * original keeps the object in two registers, this build in one). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the module
 * image; the build does not use this file. The differential test next to it
 * (func_80078628_slot00.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a handler of an object's
 * state that either runs three of the module's update steps and then the
 * shared func_80131094, or, when it must not, counts the object's field_06
 * up by one and calls func_80078458_slot00 with 8. The first arm is taken
 * when game_state.field_4e is not 0 and the byte at offset 0x3a of the
 * object that field_3c points at is 0.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: game_state.field_4e; the object's field_3c; the byte at offset
 *     0x3a of the block field_3c points at (only when field_4e is not 0);
 *     the object's field_06 (second arm).
 *   Writes: the object's field_06 (second arm: old value plus one, a byte).
 *   Callees, all replaced by recorders that return 0 (their results are
 *     not used): func_80078b50_slot00, func_80078b94_slot00 and
 *     func_80078d78_slot00 (1 argument each: the object),
 *     func_80131094 (1 argument: the object), func_80078458_slot00
 *     (2 arguments: the object and 8). The log watches the whole object
 *     (0x394 bytes), the only block the function writes, at every call.
 *   Aliasing: the object and the block of field_3c are distinct blocks.
 *   Excluded inputs: none.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void func_80078458_slot00(Object *obj, int index);
extern void func_80078b50_slot00(Object *obj);
extern void func_80078b94_slot00(Object *obj);
extern void func_80078d78_slot00(Object *obj);

void func_80078628_slot00(Object *obj) {
    Slot00Obj *target = (Slot00Obj *)obj->field_3c;

    if (game_state.field_4e != 0 && target->field_3a == 0) {
        func_80078b50_slot00(obj);
        func_80078b94_slot00(obj);
        func_80078d78_slot00(obj);
        func_80131094(obj);
    } else {
        obj->field_06 = obj->field_06 + 1;
        func_80078458_slot00(obj, 8);
    }
}
