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
 * character. It sets field_17b and field_07 to 1, clears field_12c to
 * field_12f, spends 9 points (func_80141f28), calls func_80138ae8 with the
 * game state, copies four consecutive words of a table, selected by
 * field_12a, into field_4c, field_50, field_54 and field_58, and starts
 * sequence 0x1b (0x61 when field_49 is not 0) plus field_12a / 2.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_12a, field_49; the table data_801c6288_slot04_14 at word
 *     indexes 2 * field_12a + 0 to 3, each taken modulo 256 (the index is a
 *     byte); game_state.mode and game_state.field_2f, object.field_cd and the
 *     counter data_80188ec4 through func_80138ae8.
 *   Writes: field_17b = 1, field_07 = 1, field_12c to field_12f = 0, field_4c,
 *     field_50, field_54, field_58 (the four table words), and what the
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

    obj->field_17b = 1;
    obj->field_07 = 1;
    obj->field_12f = 0;
    obj->field_12e = 0;
    obj->field_12d = 0;
    obj->field_12c = 0;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    idx = obj->field_12a * 2;
    obj->field_4c = data_801c6288_slot04_14[(u8)idx];
    obj->field_50 = data_801c6288_slot04_14[(u8)(idx | 1)];
    obj->field_54 = data_801c6288_slot04_14[(u8)(idx + 2)];
    obj->field_58 = data_801c6288_slot04_14[(u8)(idx + 3)];
    a = 0x1b;
    if (obj->field_49 != 0) {
        a = 0x61;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}
