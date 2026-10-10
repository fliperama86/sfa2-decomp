/*
 * The port's C for func_8012126c. It runs in the PC program in place of the
 * C of the unit s120f40_r2.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_80013834 in the
 * second arm is written with the argument `state`. The unit calls it with
 * none.
 *
 * Why: the function that the module slot28 has at that address
 * (func_80013834_slot28; inferred: this arm runs while that module is
 * loaded) hands its a0 on, unchanged, to the function after it (jal 0x80013854
 * at 8001383c, a0 not written before it), and that one hands it on again to
 * an entry of a table (jalr at 80013884, a0 not written before it). On the
 * console a0 still holds this function's first parameter at the call,
 * because the second arm of the original code does not write a0 (a0 is
 * written at 8012129c, in the first arm only, which then calls
 * func_8014efa8). Compiled for a PC, the chain would pass on something else.
 * Whether the entries of that table read a0 is not checked here (inferred:
 * they take the record); the test shows that the callee gets the same a0
 * as in the original code.
 *
 * What the function does (inferred): when the u16 field_4c of the record
 * data_8018f5a0 points at is 0, it raises that field by one and calls
 * func_8014efa8 with the low seven bits of state->field_70; otherwise it
 * calls func_80013834 with the state.
 *
 * Contract:
 *   Argument: state (a0), a pointer to a state record; it is not written.
 *   Reads: data_8018f5a0 (a pointer), the halfword at 0x4c of the record it
 *     points at, and the byte at 0x70 of the state in the first arm.
 *   Result: none.
 *   Writes: the halfword at 0x4c of the record data_8018f5a0 points at
 *     (the first arm).
 *   Callees: func_8014efa8 (1 argument) and func_80013834 are replaced in
 *     the test by recorders that log their arguments and return 0. What they
 *     do is outside the test. The test watches the state and the record at
 *     every call, so the write to field_4c is seen at the call.
 *   Aliasing: the state and the record are separate blocks.
 *   Excluded inputs: a null state or a null data_8018f5a0 (the original
 *     reads through them).
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

void func_8012126c(GameState *state) {
    if (data_8018f5a0->field_4c == 0) {
        data_8018f5a0->field_4c++;
        func_8014efa8(state->field_70 & 0x7f);
    } else {
        ((void (*)(GameState *))func_80013834)(state);
    }
}
