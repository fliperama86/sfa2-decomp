/*
 * The port's C for func_8014dcc0. It runs in the PC program in place of the
 * C of the unit s14d8a4_r2.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the call through the scratchpad
 * pointer (scr_d4_left for the object's side 0, scr_184_right otherwise) is
 * written with the argument `object`. The unit declares those pointers as
 * functions without a parameter and calls them without one.
 *
 * Why: the functions that the two pointers can hold read their first
 * parameter, and on the console it is still in a0 at the call, because this
 * function has not changed a0 since its entry (a0 is the caller's own first
 * parameter on every path of the original code, read from its listing: a0 is
 * used at 8014dcc8 and not written before the call at 8014dcf4). Compiled for
 * a PC, the callee would read something else.
 *
 * Evidence that the callees read a0, read in the original code's listing:
 * the functions that the start-up code of the character modules stores in
 * these two pointers read a0 at their start, in every one that a one-off
 * search of 2026-10-10 found (a private search, not a command of this
 * repository). Two of them: func_801b0c88_slot04_0a has `lbu v0,0x15a(a0)` at
 * 801b0c90, and func_801b1070_slot04_01 has the same instruction at 801b1078.
 * Which function a pointer holds at a given time is not decided here
 * (inferred: the module that is loaded for a side sets that side's pointer).
 *
 * Contract:
 *   Argument: object (a0), a pointer to an object record; it is not written.
 *   Reads: byte at 0xa6 of the object (the side), the two scratchpad
 *     pointers scr_d4_left and scr_184_right, and the byte data_801ad398.
 *   Result: data_801ad398 (a byte).
 *   Writes: nothing itself.
 *   Callees: the two pointers' targets are replaced in the test by one
 *     recorder each (placed in a block that the setup stores in the
 *     pointer's cell), which logs its first argument and returns 0. What
 *     those functions do is outside the test. The test watches the object
 *     and data_801ad398 at each call.
 *   Aliasing: the object and the two recorder blocks are separate.
 *   Excluded inputs: a null object (the original reads through it).
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

u8 func_8014dcc0(Object *object) {
    if (object->side == 0) ((u8 (*)(Object *))scr_d4_left)(object);
    else ((u8 (*)(Object *))scr_184_right)(object);
    return data_801ad398;
}
