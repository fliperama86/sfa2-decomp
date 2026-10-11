/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011530_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * No call of this function was found in the program (inferred from a scan of
 * the program's file for a jal or j to 0x80011530, for the address as a 32 bit data
 * word, and for a lui and addiu or ori pair that builds it; none of the
 * three was found). It is C for completeness.
 *
 * What it does (inferred, not an original name): pauses or resumes the disc.
 * With the argument 1 it calls func_800118f0_z2o (the volume down) and then
 * CdControlB(9, 0, 0) until that returns a value that is not 0 (the disc
 * command 9 is the library's CdlPause; inferred). With any other argument it
 * picks the mode word 0x1e8 when the word at 0x8001b348 is not 0, else 0x168,
 * and calls CdRead2(mode) until that returns a value that is not 0 (inferred:
 * resumes the stream). It returns 0 in both cases.
 *
 * Contract:
 *   Arguments: a0 = the choice (any 32 bit word; only the value 1 pauses).
 *     Result in v0: 0.
 *   Reads: the word data_8001b348_z2o (only when a0 is not 1).
 *   Writes: nothing of the program's memory. The stack frame aside.
 *   Callees, all recorders in the test, never run: func_800118f0_z2o() (0
 *     arguments), CdControlB(com, param, result) (3 arguments; the setup
 *     scripts its results: 0 up to three times, then a value that is not 0),
 *     CdRead2(mode) (1 argument; results scripted the same way).
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

int func_80011530_z2o(int pause) {
    int mode;

    if (pause == 1) {
        func_800118f0_z2o();
        while (CdControlB(9, 0, 0) == 0) {
        }
        return 0;
    }
    mode = 0x168;
    if (data_8001b348_z2o != 0) {
        mode = 0x1e8;
    }
    while (CdRead2(mode) == 0) {
    }
    return 0;
}
