/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 20 bytes shorter and uses other registers (the original
 * copies the shifted direction value to two registers before the call). The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the module
 * image; the build does not use this file. The differential test next to it
 * (difftest.py, with the contract func_801b1b40_slot04_0e.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): enters a character
 * action. It sets the state byte field_07 to 3 and field_159 to 1, then
 * func_80130504 derives the direction pair field_12a, field_129 from the
 * input word field_134. When func_801b6a7c_slot04_0e reports a condition
 * (field_129 not 0 and field_12a equal to 2, and then either field_219 or
 * bit 14 of field_130 depending on field_cd), func_801b6afc_slot04_0e
 * runs. Otherwise the counter function func_80141f28 gets t = field_12a / 2
 * and sequence 12 + t (field_48 equal to 0) or 18 + t (otherwise) is
 * selected, 3 more when field_129 is not 0.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_48, field_129, field_12a (the last two
 *     written by func_80130504 first), and what the callees read.
 *   Writes: field_07 (3), field_159 (1), and what the callees write.
 *   Callees run as original code: func_80130504 (reads field_134, writes
 *     field_129 and field_12a), func_801b6a7c_slot04_0e (reads field_129,
 *     field_12a, field_cd, field_219, field_130), func_801b6afc_slot04_0e
 *     (writes field_4c), func_80141f28 (reads field_74, field_2a2, field_d8,
 *     field_c6 and game_state.field_47, writes field_c6), func_801307e0 (the
 *     sequence tables in the scratchpad, the steps and frame records; writes
 *     the object's sequence, field_38, field_3a, field_80, frame, field_4a).
 *   Callee replaced by a recorder (no result, log in RAM): func_80120554
 *     (3 arguments), reached from func_80141f28; it ends in sound routines
 *     of Sony's library.
 *   Watched by the recorders: the whole object (0x394 bytes), copied at every
 *     recorded call. No recorded callee takes a pointer to memory the
 *     function fills.
 *   Aliasing: the object, the sequence steps, the frame records and the
 *     pointer tables are distinct blocks.
 *   Not reached by any input: none known; see the test lines for the
 *     coverage.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Functions of the same module, declared here because the published headers
   lack them. Inferred from the code; not original declarations. */
u8 func_801b6a7c_slot04_0e(Object *obj);
void func_801b6afc_slot04_0e(Object *obj);

void func_801b1b40_slot04_0e(Object *obj) {
    int t;
    int index;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    if (func_801b6a7c_slot04_0e(obj) != 0) {
        func_801b6afc_slot04_0e(obj);
    } else {
        t = obj->field_12a >> 1;
        func_80141f28(obj, t);
        index = obj->field_48 == 0 ? 0xc : 0x12;
        if (obj->field_129 != 0) {
            index += 3;
        }
        func_801307e0(obj, index + t);
    }
}
