/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011018_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): opens a movie by its index
 * in the program's table of the disc's files. It looks the index up with
 * func_800119e8_z2o(index, &size) (size is a local that the callee fills and
 * this function does not read) and hands the result, the table entry's first
 * word, as the first argument of func_80010c38_z2o (the open of a movie;
 * inferred) together with its own other nine arguments, unchanged and in
 * order, and returns what that returns.
 *
 * Contract:
 *   Arguments: a0 = the index; a1, a2, a3 and six words on the caller's
 *     stack (at sp + 0x10 to sp + 0x24 of the call, the fifth to the tenth
 *     arguments) are passed on to the open. Result in v0: the open's result.
 *   Reads: the six stack words. The index's table entry is read by the
 *     callee, which is a recorder; nothing of the program's memory is read.
 *   Writes: nothing of the program's memory (the locals aside).
 *   Callees, all recorders in the test, never run: func_800119e8_z2o(index,
 *     p) (two arguments; p is the address of a local and is masked to 0;
 *     the result is scripted), func_80010c38_z2o (ten arguments: four in
 *     registers, six read from the caller's stack; the result is scripted).
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

int func_80011018_z2o(int index, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9) {
    u32 size;

    return func_80010c38_z2o(func_800119e8_z2o(index, &size), a1, a2, a3, a4, a5, a6, a7, a8, a9);
}
