/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010be4_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the program keeps two display environments of 20 bytes each from
 * 0x80049eac on and the index of the current one at 0x80031e58. The function
 * hands the environment of the current index to the library routine that
 * sets the display environment (the game's name for the program's copy is
 * func_80158470; inferred to be the library's PutDispEnv from its use, as in
 * the logo's program), then flips the index (xor 1).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the index word data_80031e58_z2o (any int in the test, see the
 *     .py); the original reads it before the call and again after it.
 *   Writes: the index word, after the call, to the index xor 1.
 *   Callee: func_80158470(env) with env = 0x80049eac + 20 * index, one
 *     argument, a recorder in the test: it is never run. The recorder logs
 *     the pointer and the five words behind it, and the index word as it is
 *     at the call (watched), so that a flip made before the call would show.
 *     The index is not changed by the callee (as far as the call is
 *     inferred to be PutDispEnv; the test cannot show that).
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80010be4_z2o(void) {
    func_80158470(data_80049eac_z2o[data_80031e58_z2o]);
    data_80031e58_z2o = data_80031e58_z2o ^ 1;
}
