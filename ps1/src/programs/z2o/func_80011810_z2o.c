/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011810_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): adds 1 to the word 0x8002f86c (inferred: the count of completed frames;
 * the table of the private study calls the routine the stream's frame routine).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the word data_8002f86c_z2o (any int).
 *   Writes: the same word, to its value plus 1 (wrapping). No callee.
 *   Slots no input can reach: none; all eight slots of the original run.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011810_z2o(void) {
    data_8002f86c_z2o = data_8002f86c_z2o + 1;
}
