/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has the same size but differs in which registers hold the table
 * index and its temporaries, and the order of some instructions. The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the module
 * image; the build does not use this file. The differential test next to it
 * (difftest.py, with func_801b1a00_slot04_14.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a state entry of a
 * character. It sets active_flag_4 and state_3 to 1, clears substate_index to
 * step_limit, spends 9 points (func_80141f28), calls func_80138ae8 with the
 * game state, copies four consecutive words of a table, selected by
 * direction_index, into vel_x, vel_y, accel_x and accel_y, and starts
 * sequence 0x1b (0x61 when field_49 is not 0) plus direction_index / 2.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: direction_index, field_49; the table data_801c6288_slot04_14 at word
 *     indexes 2 * direction_index + 0 to 3, each taken modulo 256 (the index is a
 *     byte); game_state.mode and game_state.field_2f, object.field_cd and the
 *     counter data_80188ec4 through func_80138ae8.
 *   Writes: active_flag_4 = 1, state_3 = 1, substate_index to step_limit = 0, vel_x,
 *     vel_y, accel_x, accel_y (the four table words), and what the
 *     callees write.
 *   Callees: func_80141f28 (2 arguments) and func_801307e0 (2 arguments) are
 *     replaced by recorders returning a random word, in the original and in
 *     this C alike; they reach the sound and sequence code of the resident
 *     image. func_80138ae8 runs as the original code: it reads the game
 *     state's mode and field_2f and the object's field_cd, and adds to
 *     data_80188ec4 through func_80138838 and func_8013886c.
 *   Aliasing: only the object is a block; the table and game state are the
 *     game's own data.
 *   Watched by the recorders: the whole object (0x394 bytes) at every call of the two recorders,
 *     so the order of this function's stores against the calls is tested.
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Inputs excluded: none. Slots not reached: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6288_slot04_14[];

void func_801b1a00_slot04_14(Object *obj) {
    int idx;
    int a;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->step_limit = 0;
    obj->step_count_2 = 0;
    obj->field_12d = 0;
    obj->substate_index = 0;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    idx = obj->direction_index * 2;
    obj->vel_x = data_801c6288_slot04_14[(u8)idx];
    obj->vel_y = data_801c6288_slot04_14[(u8)(idx | 1)];
    obj->accel_x = data_801c6288_slot04_14[(u8)(idx + 2)];
    obj->accel_y = data_801c6288_slot04_14[(u8)(idx + 3)];
    a = 0x1b;
    if (obj->field_49 != 0) {
        a = 0x61;
    }
    a += obj->direction_index >> 1;
    func_801307e0(obj, a);
}
