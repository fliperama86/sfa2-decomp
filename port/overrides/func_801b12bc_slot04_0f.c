/*
 * The port's C for func_801b12bc_slot04_0f. It runs in the PC program in
 * place of the C of the unit slot04_0f_09e4_r1_b.c; the PS1 build does not
 * use this file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_80141c4c is written
 * with the argument `obj`. The unit calls it with none.
 *
 * Why: func_80141c4c reads a0 (its second instruction, `move a1,a0` at
 * 80141c50, and everything after it goes through a1). On the console no
 * call lies before the call (jal at 801b12dc): a0 is this function's own
 * first parameter, because no instruction from the entry (801b12bc) to the
 * jal writes a0 (the object is copied to s0 at 801b12c4; the others write
 * sp, ra and v0). Compiled for a PC, the callee would read something else.
 *
 * What the function does (inferred): when the u8 field_260 is 0 and
 * func_80141c4c returns a value that is not 0, it sets a few fields of the
 * object (field_04 = 1, field_06 = 7, field_05 = 0, field_07 = 0,
 * field_15a = 6, field_159 = 0) and returns 1; otherwise it returns 0.
 *
 * Contract:
 *   Argument: obj (a0), a pointer to an object; result in v0 (0 or 1).
 *   Reads: the u8 field_260. Writes: the six fields above, in the one arm.
 *   Callees: func_80141c4c (1 argument) is replaced in the test by a recorder
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
    if (obj->field_260 != 0) {
        return 0;
    }
    if (((int (*)(Object *))func_80141c4c)(obj)) {
        obj->field_04 = 1;
        obj->field_06 = 7;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_15a = 6;
        obj->field_159 = 0;
        return 1;
    }
    return 0;
}
