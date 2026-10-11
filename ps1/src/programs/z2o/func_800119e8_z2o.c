/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800119e8_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): looks an entry up in the program's table of 12 byte entries that starts
 * at 0x8001b354 (inferred: the disc's files). The first word of the entry is
 * returned and the second is stored through the pointer. What the words stand
 * for is not known here.
 *
 * Contract:
 *   Arguments: a0 = the index of the entry (any int whose entry lies in
 *     RAM), a1 = pointer to a word that receives the second word of the
 *     entry. Returns the first word of the entry in v0.
 *   Reads: the first two words of the entry at 0x8001b354 + 12 * index. The
 *     program's data is not in the setup's memory: the setup writes the
 *     entry itself.
 *   Writes: the word at a1. No callee.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

u32 func_800119e8_z2o(int index, u32 *size) {
    *size = data_8001b354_z2o[index][1];
    return data_8001b354_z2o[index][0];
}
