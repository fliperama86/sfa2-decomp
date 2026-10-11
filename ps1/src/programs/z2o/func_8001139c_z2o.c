/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_8001139c_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): ends the movie. In order:
 *   1. func_800118f0_z2o() (the volume down in steps);
 *   2. StUnSetRing(), CdReady(0, 0);
 *   3. CdGetSector(p, 0x200) with p the pointer word at 0x8002f804;
 *   4. stores 0x80 in the mode byte at 0x8002f800;
 *   5. calls CdControlB(0x0e, &mode byte, 0) until it returns a value that is
 *      not 0, then CdControlB(9, 0, 0) until it returns a value that is not 0;
 *   6. DecDCToutCallback(0).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the pointer word at 0x8002f804 (any word in the test), once.
 *   Writes: the mode byte at 0x8002f800.
 *   Callees, all recorders in the test, never run: func_800118f0_z2o (0
 *     arguments), StUnSetRing (0), CdReady (2), CdGetSector (2), CdControlB
 *     (3; one recorder for both loops, its results scripted in turn),
 *     DecDCToutCallback (1). The mode byte's word is watched, so that the
 *     log shows what the function has stored by each call.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_8001139c_z2o(void) {
    func_800118f0_z2o();
    StUnSetRing();
    CdReady(0, 0);
    CdGetSector(data_8002f804_z2o, 0x200);
    data_8002f800_z2o = 0x80;
    while (CdControlB(0x0e, &data_8002f800_z2o, 0) == 0) {
    }
    while (CdControlB(9, 0, 0) == 0) {
    }
    DecDCToutCallback(0);
}
