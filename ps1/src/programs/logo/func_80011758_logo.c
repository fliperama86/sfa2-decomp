/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011758_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): looks up an entry in the
 * program's table of the disc's files. The table starts at 0x8001a678 and
 * its entries are 12 bytes long; the first word of the entry is returned
 * and the second is stored through the pointer. What the words stand for
 * is not known here.
 *
 * Contract:
 *   Arguments: a0 = the index of the entry (any int whose entry lies in
 *     RAM), a1 = pointer to a word that receives the second word of the
 *     entry. Returns the first word of the entry in v0.
 *   Reads: the first two words of the entry at 0x8001a678 + 12 * index.
 *     The program's data is not in the setup's memory: the setup writes
 *     the entry itself.
 *   Writes: the word at a1. No callee.
 *   Slots no input can reach: none known.
 */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u32 data_8001a678_logo[][3];

u32 func_80011758_logo(int index, u32 *size) {
    *size = data_8001a678_logo[index][1];
    return data_8001a678_logo[index][0];
}
