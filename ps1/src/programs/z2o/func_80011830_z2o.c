/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011830_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): brings the disc's volume up
 * in steps. The program keeps a volume word at 0x8001b34c, a stereo flag word
 * at 0x8001b350 and four volume bytes at 0x8002f8a4 (handed to CdMix). The
 * flag is read once, first.
 *   - flag not 0: for i = 0, 2, 4, ... while i <= the volume word (an
 *     unsigned comparison, the word read again at the end of each step; the
 *     first step always runs), sets the four bytes to i, 0, i, 0 (in that
 *     order) and calls CdMix(bytes);
 *   - flag 0: the same loop, all four bytes set to i / 2 (a signed division
 *     by two) before each call.
 * Each step stores a byte of i (the low eight bits of an int).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the flag word, once (any word in the test, 0 in half the cases);
 *     the volume word at the end of every step (its value is from 0 to 40 in
 *     most cases and up to 300 in others; the test keeps it small because a
 *     huge value would run for billions of steps: the loop is bounded by the
 *     setup, not by the function).
 *   Writes: the four volume bytes, at each step.
 *   Callee: CdMix(bytes), the library routine (the program's copy), one
 *     argument, a recorder in the test and never run: it logs the pointer and
 *     the word behind it, so that the four bytes at every call are compared.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011830_z2o(void) {
    int i;

    if (data_8001b350_z2o != 0) {
        i = 0;
        do {
            data_8002f8a4_z2o[0] = i;
            data_8002f8a4_z2o[1] = 0;
            data_8002f8a4_z2o[2] = i;
            data_8002f8a4_z2o[3] = 0;
            CdMix(data_8002f8a4_z2o);
            i += 2;
        } while (data_8001b34c_z2o >= (u32)i);
    } else {
        i = 0;
        do {
            data_8002f8a4_z2o[0] = i / 2;
            data_8002f8a4_z2o[1] = i / 2;
            data_8002f8a4_z2o[2] = i / 2;
            data_8002f8a4_z2o[3] = i / 2;
            CdMix(data_8002f8a4_z2o);
            i += 2;
        } while (data_8001b34c_z2o >= (u32)i);
    }
}
