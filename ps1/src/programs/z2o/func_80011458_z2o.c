/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011458_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * No call of this function was found in the program (inferred from a scan of
 * the program's file for a jal or j to 0x80011458, for the address as a 32 bit data
 * word, and for a lui and addiu or ori pair that builds it; none of the
 * three was found). It is C for completeness.
 *
 * What it does (inferred, not an original name): a test of the movie's end.
 * Returns 1 when the word at 0x8002f894 (the latest frame number that the
 * frame routine stores) is not below the word at 0x8002f814 (the number of
 * frames of the movie), compared as unsigned numbers, else 0.
 *
 * Contract:
 *   Arguments: none. Result in v0: 1 or 0 as above.
 *   Reads: the words data_8002f894_z2o and data_8002f814_z2o.
 *   Writes: nothing. No callee.
 *   Slots no input can reach: none; all eight slots of the original run.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

int func_80011458_z2o(void) {
    return data_8002f894_z2o >= (u32)data_8002f814_z2o;
}
