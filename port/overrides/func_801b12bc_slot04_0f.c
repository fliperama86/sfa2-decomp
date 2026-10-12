/*
 * The port's C for func_801b12bc_slot04_0f. It runs in the PC program in
 * place of the C of the unit count_down_then_reset_or_update_sequence_slot04_0f.c; the PS1 build does not
 * use this file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: the call of is_state_01010101_and_ready is written
 * with the argument `obj`. The unit calls it with none.
 *
 * Why: is_state_01010101_and_ready reads a0 (its second instruction, `move a1,a0` at
 * 80141c50, and everything after it goes through a1). On the console no
 * call lies before the call (jal at 801b12dc): a0 is this function's own
 * first parameter, because no instruction from the entry (801b12bc) to the
 * jal writes a0 (the object is copied to s0 at 801b12c4; the others write
 * sp, ra and v0). Compiled for a PC, the callee would read something else.
 *
 * What the function does (inferred): when the u8 hit_flag is 0 and
 * is_state_01010101_and_ready returns a value that is not 0, it sets a few fields of the
 * object (state_0 = 1, state_2 = 7, state_1 = 0, state_3 = 0,
 * field_15a = 6, active_flag_3 = 0) and returns 1; otherwise it returns 0.
 *
 * Contract:
 *   Argument: obj (a0), a pointer to an object; result in v0 (0 or 1).
 *   Reads: the u8 hit_flag. Writes: the six fields above, in the one arm.
 *   Callees: is_state_01010101_and_ready (1 argument) is replaced in the test by a recorder
 *     that logs its argument and returns 0 or 1 (the setup chooses per case,
 *     so that both arms are reached). What it does is outside the test. The
 *     test watches the whole object at the recorder's call.
 *   Excluded inputs: a null obj.
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

int func_801b12bc_slot04_0f(Object *obj) {
    if (obj->hit_flag != 0) {
        return 0;
    }
    if (((int (*)(Object *))is_state_01010101_and_ready)(obj)) {
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 0;
        return 1;
    }
    return 0;
}
