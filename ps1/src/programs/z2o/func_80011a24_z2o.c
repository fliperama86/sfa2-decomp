/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011a24_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): starts the disc. It sets a
 * mode byte (a local) to 0x80 and calls CdInit up to 4 times until one call
 * returns a value that is not 0. When none does, it returns 0. Otherwise it
 * calls CdControlB(0x0e, &mode, 0) until that returns a value that is not 0,
 * then CdIntToPos(0x10, pos) (pos is a local of four bytes), then
 * CdControlB(0x15, pos, 0) until it returns a value that is not 0, and
 * returns 1 (the success flag of the first loop, set to 1).
 *
 * Contract:
 *   Arguments: none. Result in v0: 1 when a CdInit call succeeded, else 0.
 *   Reads: nothing of the program's memory.
 *   Writes: nothing of the program's memory (the locals aside).
 *   Callees, all recorders in the test, never run: CdInit() (0 arguments;
 *     its results are scripted), CdControlB(com, param, result) (3
 *     arguments; the pointer is the address of a local and is masked, the
 *     word behind it logged; results scripted), CdIntToPos(i, p) (2
 *     arguments; the pointer is the address of a local and is masked; the
 *     recorder fills the position with a word of the setup, a model of what
 *     the library does).
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

int func_80011a24_z2o(void) {
    u8 mode;
    u8 pos[4];
    int tries;
    int ok;

    ok = 0;
    tries = 4;
    mode = 0x80;
    do {
        if (CdInit() != 0) {
            ok = 1;
            break;
        }
        tries--;
    } while (tries != 0);
    if (ok == 0) {
        return ok;
    }
    while (CdControlB(0x0e, &mode, 0) == 0) {
    }
    CdIntToPos(0x10, pos);
    while (CdControlB(0x15, pos, 0) == 0) {
    }
    return ok;
}
