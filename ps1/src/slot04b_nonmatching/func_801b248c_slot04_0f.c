/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code keeps one constant in a different register. The exact owner of
 * the bytes in the PS1 build stays the raw bytes of the module image; the
 * build does not use this file. The differential test next to it
 * (difftest.py, with func_801b248c_slot04_0f.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a state routine of a
 * character object. It counts the object's field_07 up by one, sets field_17b
 * to 1, applies a change of 4 to the object's field_c6 (func_80141f28), lets
 * the state of the game react to the object (func_80138ae8, passing the
 * pointer at game_state.config), and selects a sequence for the object
 * (func_801307e0) by an index made of half of field_12a plus 0x3b, or plus
 * 0x57 when field_49 is not 0.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_07, field_12a and field_49; game_state.config.
 *   Writes: the object's field_07 and field_17b, and what func_80141f28 and
 *     func_80138ae8 write.
 *   Callees:
 *     func_80141f28 (object, 4) runs as the original code. It reads and
 *       writes the object's field_c6 and reads field_74, field_2a2, field_d8
 *       and game_state.field_47. Its call of func_80120554 (a sound request)
 *       is replaced by a recorder that takes 3 arguments and returns 0.
 *     func_80138ae8 (game_state.config, object) runs as the original code.
 *       It reads the mode and field_2f of the block at game_state.config,
 *       the object's field_cd, and updates data_80188ec4.
 *     func_801307e0 (object, index) is replaced by a recorder that takes 2
 *       arguments and returns 0. It selects a sequence through tables and
 *       would need a contract of its own.
 *   Watched at every recorded call: the whole object (0x394 bytes) and the
 *     word at data_80188ec4 (the globals the callees write). No recorder
 *     is given a pointee: the arguments are values.
 *   Aliasing: the object and the block at game_state.config are distinct.
 *   Excluded inputs: none. Slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b248c_slot04_0f(Object *o) {
    o->field_07 = o->field_07 + 1;
    o->field_17b = 1;
    func_80141f28(o, 4);
    func_80138ae8((GameState *)game_state.config, o);
    if (o->field_49 != 0) {
        func_801307e0(o, (o->field_12a >> 1) + 0x57);
    } else {
        func_801307e0(o, (o->field_12a >> 1) + 0x3b);
    }
}
