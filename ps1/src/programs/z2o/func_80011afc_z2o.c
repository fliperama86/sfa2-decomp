/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011afc_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): nothing. The listing of
 * the original is a return and its delay slot (a nop), 8 bytes. Inferred: it
 * is a routine of the program's own copy of the library that does nothing;
 * the main calls it first.
 *
 * Contract:
 *   Arguments: none. The test gives a0 to a3 random values; v0 is 0 at the
 *     start of every run, as the tool sets it, and v0 and memory are compared
 *     after the call (see the .py). The argument registers are not compared
 *     after the call. No result.
 *   Reads: nothing. Writes: nothing. No callee. The test fills the bytes
 *     around the program's word 0x8002f86c (data_8002f86c_z2o, which this
 *     function does not use) with random values, so that a store would
 *     show.
 *   Slots no input can reach: none; both slots of the original run.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011afc_z2o(void) {
}
