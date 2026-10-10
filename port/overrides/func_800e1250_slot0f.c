/*
 * The port's C for func_800e1250_slot0f. It runs in the PC program in place of
 * the C of the unit slot0f_119c_r1.c (under ps1/src/slot0f/); the PS1 build
 * does not use this file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: this function returns the result of
 * `read`, a long. The unit defines it `void`; the loop is the same.
 *
 * Why: the original's callers use the result register after the call
 * (func_800e0a1c tests it with `bltz v0,0x800e0b2c` at 800e0b00, right after
 * the jal at 800e0af8). In the original code of the console that register
 * still holds what the last call of the loop, `read` (the jal to 0x8015784c at
 * 800e1290), returned: from the return of that call to the function's `jr ra`
 * at 800e12c4 no instruction writes v0 (read in the listing of the module
 * image slot0f with the project's Ghidra client). Compiled for a PC, a
 * function defined `void` hands its caller nothing, so the caller would read
 * something else.
 *
 * What the function does (inferred): it calls func_8015fb30(0) and then
 * `read`, up to 0x78 times (`ori s0,zero,0x78` at 800e1270), and stops at the
 * first result of `read` that is not -1; the test `bne v0,...` at 800e1298
 * compares the whole word. It returns the last result.
 *
 * Contract:
 *   Arguments: fd (a0), buf (a1) and n (a2); the function only passes them on
 *     to `read`.
 *   Result: v0, a long; it is compared.
 *   Reads and writes: nothing itself.
 *   Callees, both replaced by recorders: func_8015fb30 (1 argument, result 0)
 *     and `read` (3 arguments, logged as whole words). The recorder of `read`
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

long read(long fd, void *buf, long n);

long func_800e1250_slot0f(long fd, void *buf, long n) {
    int i = 0x78;
    long r;
    do {
        func_8015fb30(0);
        r = read(fd, buf, n);
        if (r != -1) {
            break;
        }
    } while (--i != 0);
    return r;
}
