/*
 * The port's C for func_801212e8. It runs in the PC program in place of the
 * C of the unit s120f40_r2.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_800136b0 is written
 * with the argument `unused`, the function's own parameter. The unit calls
 * it with none (and names the parameter `unused`).
 *
 * Why: func_800136b0 reads a0 at its start (`sb zero,0x1(a0)` at 800136b8,
 * read in the original code's listing). On the console a0 still holds this
 * function's first parameter at the call, because the original code does
 * not write a0 between its entry (801212e8) and the jal at 801212f0.
 * Compiled for a PC, the callee would read something else.
 *
 * What the function does (inferred): it calls func_800136b0 with its
 * parameter.
 *
 * Contract:
 *   Argument: unused (a0), a pointer to a state record; it is not written
 *     by this function.
 *   Reads and writes: nothing itself. No result.
 *   Callees: func_800136b0 (1 argument) is replaced in the test by a
 *     recorder that logs its argument and returns 0. What it does (it
 *     writes through a0 and calls a table entry) is outside the test. The
 *     test watches the block a0 points at.
 *   Excluded inputs: a null parameter (the callee writes through it).
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

void func_801212e8(GameState *unused) {
    ((void (*)(GameState *))func_800136b0)(unused);
}
