/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original loads the constant 1 first and this build loads it later (two
 * instruction slots differ in position). The exact owner of the bytes in
 * the PS1 build stays the raw bytes of the module image; the build does
 * not use this file. The differential test next to it (difftest.py, with
 * func_80013ee8_slot01.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): puts an object into a
 * start state. It sets position (0x1e0, 0x54), advances field_04 by one,
 * clears several fields, sets field_0c to 1 and field_46 to 0x1f, and
 * when bit 0 of game_state.mode is clear it sets field_48 to 1 and moves
 * the x position to 0x194. It then calls the function stored in
 * data_80015cdc_slot01 with the object.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (0x394 bytes). No return value.
 *   Reads: the object's field_04; game_state.mode (byte); the function
 *     pointer data_80015cdc_slot01.
 *   Writes: the object's pos_x, pos_y, field_04, field_09, field_0c,
 *     field_0d, field_20, field_22, field_24, field_46 and field_48.
 *   Callee: the function pointer data_80015cdc_slot01 is set by the setup
 *     to a block with a recorder (one argument, the object, result 0). The
 *     recorder copies the whole object (0x394 bytes) into the log at the
 *     call, so every field the function writes before the call is shown
 *     as the callee would see it. game_state is not written.
 *   Excluded inputs: none.
 *   Not reached by any input: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015cdc_slot01;

void func_80013ee8_slot01(Object *obj) {
    obj->pos_x = 0x1e0;
    obj->pos_y = 0x54;
    obj->field_04++;
    obj->field_09 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->field_0c = 1;
    obj->field_0d = 0;
    obj->field_48 = 0;
    obj->field_46 = 0x1f;
    if ((game_state.mode & 1) == 0) {
        obj->field_48 = 1;
        obj->pos_x = 0x194;
    }
    data_80015cdc_slot01(obj);
}
