/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code stores field_225 and field_4c later than the original does
 * and is 4 bytes longer. The PS1 build keeps the raw bytes of the module image and does not use this file. The
 * differential test next to it (difftest.py, with
 * func_801b20b0_slot04_06.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): enters a character state.
 * It clears the bytes field_12c to field_12f, sets field_17b to 1, adds 1
 * to field_07, calls func_80141f28 (object, 7) and func_80138ae8
 * (&game_state, object), sets field_225 to 1, loads field_4c and field_54
 * from two tables indexed by the byte field_12a (the word tables at
 * data_801c5398_slot04_06 and data_801c539c_slot04_06, which are one array
 * read one word apart), and starts sequence (field_12a >> 1) + 0x1b.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_07, field_12a, the words data_801c5398_slot04_06[field_12a]
 *     and data_801c539c_slot04_06[field_12a].
 *   Writes: field_12c to field_12f, field_17b, field_07, field_225,
 *     field_4c, field_54, plus what the callees write.
 *   Callees: func_80141f28 (2 arguments), func_80138ae8 (2 arguments) and
 *     func_801307e0 (2 arguments) are replaced by recorders returning a
 *     random word, in the original and in this C alike; they reach the
 *     sequence code and the shared state code of the resident image (read
 *     from the original's listing, not tested). The
 *     second argument of func_80138ae8 is the object; the first is
 *     game_state, which this function does not write.
 *   Aliasing: the object is one block; the two tables are the module's own
 *     data, which the setup fills with random words for indices 0 to 256.
 *   Watched by the recorders: the whole object (0x394 bytes) at every
 *     call, so the order of this function's stores against the calls is
 *     tested. No recorded callee gets a pointer to memory filled for the
 *     call.
 *   Inputs excluded: none.
 *   Slots not reached: none (all 46 slots are executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c5398_slot04_06[];
extern s32 data_801c539c_slot04_06[];

void func_801b20b0_slot04_06(Object *obj) {
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->field_225 = 1;
    obj->field_4c = data_801c5398_slot04_06[obj->field_12a];
    obj->field_54 = data_801c539c_slot04_06[obj->field_12a];
    func_801307e0(obj, (obj->field_12a >> 1) + 0x1b);
}
