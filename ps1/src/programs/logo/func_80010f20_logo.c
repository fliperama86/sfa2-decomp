/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010f20_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): waits while a flag word
 * is not 0. The flag is the word at 0x8002eb3c (gp + 0x40); the function
 * reads it again and again until it is 0, then returns. Nothing in the
 * function changes the flag: on the console an interrupt does (the
 * decoder's output routine, func_80010f38_logo, sets it to 1 and its
 * successor routine sets it to 0; inferred).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the flag word, data_8002eb3c_logo.
 *   Writes: nothing. No callee.
 *   Limit of the test: the flag is 0 in every case. With any other value
 *     the function does not end unless an interrupt changes the flag, which
 *     the test tool does not model, so a case with another value would be
 *     discarded as one that exceeds its budget. The test therefore shows
 *     the exit and the read of the flag, not the waiting.
 *   Slots no input can reach: none; all six slots of the original run
 *     (the branch back is not taken with the flag 0, its delay slot runs).
 *   The flag is declared volatile in logo.h so that the C reads it at
 *     every turn of the wait, as the original does; without it the
 *     compiler reads it once.
 */
#include "logo.h"

void func_80010f20_logo(void) {
    while (data_8002eb3c_logo != 0) {
    }
}
