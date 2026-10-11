/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011630_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets stereo or mono for the
 * disc's volume. It stores its argument in the stereo flag word at 0x8001b350
 * and then, from the volume word at 0x8001b34c:
 *   - argument not 0: the four volume bytes at 0x8002f8a4 become (v, 0, v, 0)
 *     with v the low byte of the volume word (a byte load);
 *   - argument 0: all four bytes become the volume word >> 1 (a logical
 *     shift, the low byte);
 * then calls CdMix(bytes).
 *
 * Contract:
 *   Arguments: a0 = the flag (any 32 bit word in the test, 0 in half the
 *     cases). No result.
 *   Reads: the volume word (any word in the test), once.
 *   Writes: the stereo flag word; the four volume bytes.
 *   Callee: CdMix(bytes), the library routine (the program's copy), one
 *     argument, a recorder in the test and never run: it logs the pointer and
 *     the word behind it, so that the four bytes at the call are compared.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011630_z2o(int stereo) {
    u32 half;

    data_8001b350_z2o = stereo;
    if (stereo != 0) {
        data_8002f8a4_z2o[1] = 0;
        data_8002f8a4_z2o[3] = 0;
        data_8002f8a4_z2o[0] = (u8)data_8001b34c_z2o;
        data_8002f8a4_z2o[2] = (u8)data_8001b34c_z2o;
    } else {
        half = data_8001b34c_z2o >> 1;
        data_8002f8a4_z2o[0] = half;
        data_8002f8a4_z2o[1] = half;
        data_8002f8a4_z2o[2] = half;
        data_8002f8a4_z2o[3] = half;
    }
    CdMix(data_8002f8a4_z2o);
}
