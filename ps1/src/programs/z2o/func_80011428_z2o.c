/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011428_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): waits while the flag word 0x8002f890 is not 0, then copies the word
 * 0x8002f81c to the word 0x8002f818 (inferred: waits until the decoder's
 * strips are done, then takes over their count; not original names). Nothing
 * in the function changes the flag: on the console an interrupt does
 * (inferred).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the flag data_8002f890_z2o and the word data_8002f81c_z2o.
 *   Writes: the word data_8002f818_z2o. No callee.
 *   Limit of the test: the flag is 0 in every case. With any other value
 *     the function does not end unless an interrupt changes the flag, which
 *     the test tool does not model, so the test shows the exit, the read of
 *     the flag and the copy, not the waiting.
 *   Slots no input can reach: none; all twelve slots of the original run
 *     (the branch back is not taken with the flag 0, its delay slot runs).
 *   The flag is declared volatile in z2o.h so that the C reads it at every
 *     turn of the wait.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011428_z2o(void) {
    while (data_8002f890_z2o != 0) {
    }
    data_8002f818_z2o = data_8002f81c_z2o;
}
