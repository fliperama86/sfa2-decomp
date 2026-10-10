/*
 * The port's C for func_800e11e4_slot0f. It runs in the PC program in place of
 * the C of the unit slot0f_119c_r1.c (under ps1/src/slot0f/); the PS1 build
 * does not use this file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: this function returns the result of
 * `open`, an int. The unit defines it `void`; the loop is the same.
 *
 * Why: the original's callers use the result register after the call
 * (func_800e0850 moves v0 into s0 right after the jal at 800e091c (at
 * 800e0924), after the jal at 800e093c (at 800e0944) and after the jal at
 * 800e0960 (at 800e0968); func_800e0a1c does so at 800e0adc, after the jal at
 * 800e0ad4). In the original code of the console that register still holds
 * what the last call of the loop, `open` (the jal to 0x8015787c at 800e1218),
 * returned: from the return of that call to the function's `jr ra` at 800e1248
 * no instruction writes v0 (read in the listing of the module image slot0f
 * with the project's Ghidra client). Compiled for a PC, a function defined
 * `void` hands its caller nothing, so the caller would read something else.
 *
 * What the function does (inferred): it calls func_8015fb30(0) and then
 * `open`, up to 0x78 times (`ori s0,zero,0x78` at 800e11fc), and stops at the
 * first result of `open` that is not -1; the test `bne v0,...` at 800e1220
 * compares the whole word. It returns the last result.
 *
 * Contract:
 *   Arguments: name (a0), a pointer to a path, and flags (a1); the function
 *     only passes both on to `open`.
 *   Result: v0, an int; it is compared.
 *   Reads and writes: nothing itself.
 *   Callees, both replaced by recorders: func_8015fb30 (1 argument, result 0)
 *     and `open` (2 arguments, logged as whole words). The recorder of `open`
 *     answers from a script, one result per call in turn and the last one for
 *     any further call. The script ends the loop at the first try, at a later
 *     try (the k-th, k = 2 to 0x78) and never (0x78 answers of -1). The
 *     answers are whole words; the ones other than -1 include the near misses
 *     of a test against -1 (0xffff, 0xff, 0xfffffffe, 0 and others).
 *   Excluded inputs: none.
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

int open(const char *devname, int flags);

int func_800e11e4_slot0f(const char *name, int flags) {
    int i = 0x78;
    int r;
    do {
        func_8015fb30(0);
        r = open(name, flags);
        if (r != -1) {
            break;
        }
    } while (--i != 0);
    return r;
}
