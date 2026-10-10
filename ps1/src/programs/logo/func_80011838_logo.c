/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011838_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): nothing. The original is
 * a return and its delay slot, 8 bytes.
 *
 * Contract:
 *   Arguments: none. The test gives v0 a value before the call and
 *     compares it after (see the .py), so the C must leave it alone.
 *   Reads: nothing. Writes: nothing. No callee.
 *   Slots no input can reach: none; both slots of the original run.
 */
#include "logo.h"

void func_80011838_logo(void) {
}
