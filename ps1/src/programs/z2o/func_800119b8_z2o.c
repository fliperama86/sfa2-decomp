/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800119b8_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): stores its argument at the word 0x8002f870 (a parameter word; inferred,
 * not an original name).
 *
 * Contract:
 *   Arguments: a0 = the value (any int). No result.
 *   Reads: nothing.
 *   Writes: the word data_8002f870_z2o. No callee.
 *   Slots no input can reach: none; all four slots of the original run.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_800119b8_z2o(int value) {
    data_8002f870_z2o = value;
}
