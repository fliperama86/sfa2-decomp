/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010fc4_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): brings the volume of the
 * disc's sound up in steps. The program keeps four volume bytes at
 * 0x8002eb40 (handed to CdMix, whose argument is the library's CdlATV of
 * four bytes) and a word at 0x8002eb34. The word is read once, first.
 *   - when the word is not 0: for i = 0, 2, 4, ... 128 (65 steps), sets the
 *     four bytes to i, 0, i, 0 (in that order) and calls CdMix(bytes);
 *   - when the word is 0: for the same i, sets all four bytes to i / 2
 *     (a signed division by two) and calls CdMix(bytes).
 * Each step stores a byte of i (the low eight bits of an int); at i = 128 the
 * byte is 0x80.
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the word at 0x8002eb34 (any int in the test), once.
 *   Writes: the four bytes at 0x8002eb40, at each step.
 *   Callee: CdMix(bytes), the program's copy of the library routine, one
 *     argument (the address of the four bytes), a recorder in the test and
 *     never run: it logs the pointer and the word behind it, so that the
 *     values of the four bytes at every call are compared (65 calls).
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_80010fc4_logo(void) {
    int i;

    if (data_8002eb34_logo != 0) {
        for (i = 0; i < 0x81; i += 2) {
            data_8002eb40_logo[0] = i;
            data_8002eb40_logo[1] = 0;
            data_8002eb40_logo[2] = i;
            data_8002eb40_logo[3] = 0;
            CdMix(data_8002eb40_logo);
        }
    } else {
        for (i = 0; i < 0x81; i += 2) {
            data_8002eb40_logo[0] = i / 2;
            data_8002eb40_logo[1] = i / 2;
            data_8002eb40_logo[2] = i / 2;
            data_8002eb40_logo[3] = i / 2;
            CdMix(data_8002eb40_logo);
        }
    }
}
