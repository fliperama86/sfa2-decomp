/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800118f0_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): brings the disc's volume down
 * in steps. The program keeps a volume word at 0x8001b34c, a stereo flag word
 * at 0x8001b350 and four volume bytes at 0x8002f8a4 (handed to CdMix). The
 * flag is read once, first, then the volume word once into a counter v.
 *   - flag not 0: while v >= 0 (a signed comparison; nothing runs when v is
 *     negative), sets the four bytes to v, 0, v, 0 (in that order), calls
 *     CdMix(bytes), then subtracts 2 from v;
 *   - flag 0: the same, the four bytes set to v / 2 (a signed division by
 *     two).
 * Each step stores a byte of v (the low eight bits of an int).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the flag word, once (any word in the test, 0 in half the cases);
 *     the volume word, once (from -6 to 40 in most cases, up to 300 in others,
 *     and about one case in ten negative: the loop runs v / 2 + 1 times, so
 *     the test keeps it small).
 *   Writes: the four volume bytes, at each step. The volume word is not
 *     written.
 *   Callee: CdMix(bytes), the library routine (the program's copy), one
 *     argument, a recorder in the test and never run: it logs the pointer and
 *     the word behind it, so that the four bytes at every call are compared.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_800118f0_z2o(void) {
    int v;

    if (data_8001b350_z2o != 0) {
        v = data_8001b34c_z2o;
        if (v >= 0) {
            do {
                data_8002f8a4_z2o[0] = v;
                data_8002f8a4_z2o[1] = 0;
                data_8002f8a4_z2o[2] = v;
                data_8002f8a4_z2o[3] = 0;
                CdMix(data_8002f8a4_z2o);
                v -= 2;
            } while (v >= 0);
        }
    } else {
        v = data_8001b34c_z2o;
        if (v >= 0) {
            do {
                data_8002f8a4_z2o[0] = v / 2;
                data_8002f8a4_z2o[1] = v / 2;
                data_8002f8a4_z2o[2] = v / 2;
                data_8002f8a4_z2o[3] = v / 2;
                CdMix(data_8002f8a4_z2o);
                v -= 2;
            } while (v >= 0);
        }
    }
}
