/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has the original's size but differs in where one shift is
 * scheduled around a branch (a measured difference of the instruction
 * order, not of meaning). The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the resident executable; the build does not use this file.
 * The differential test next to it (func_80141534.py, run by difftest.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): tests one bit of a 16-bit
 * mask. The object's field_130 flags say whether a mask applies at all
 * (bits 0xa000); if not, the result is 1. Otherwise the mask is the 16-bit
 * entry (the second one when flag 0x4000 is set) of a table selected by the
 * object's kind, and the bit tested is chosen by the pair `dir`: its first
 * byte halved, plus 6 when the second byte is not 0, plus 1 when flag
 * 0x8000 is clear. The result is 1 when that bit is clear, 0 when set.
 *
 * Contract:
 *   Arguments: a0 = object; a1 = the byte pair `dir`, passed in the upper
 *     half of a1 (first byte in bits 16 to 23, second in bits 24 to 31; the
 *     lower half of a1 is not read). Result: v0 = 0 or 1.
 *   Reads: object->field_130 and object->kind; table_8017179c[kind] (a
 *     pointer to two 16-bit entries) and the entry.
 *   Writes: nothing. No callee.
 *   The shift count is taken modulo 32 as the machine does (the original
 *     uses a variable shift); the C states the mask so the count is
 *     defined. Counts reach at most 133.
 *   Aliasing: none that matters (nothing is written).
 *   Exclusions: kind is limited by the setup to the 8 table entries the
 *     setup fills with its own blocks. All instruction slots are reachable.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

s16 func_80141534(Object *object, BytePair dir) {
    int flags = object->field_130;
    u16 *mask;
    int shift;

    if ((flags & 0xa000) == 0) {
        return 1;
    }
    mask = table_8017179c[object->kind];
    if (flags & 0x4000) {
        mask++;
    }
    shift = dir.first >> 1;
    if (dir.second != 0) {
        shift += 6;
    }
    if ((flags & 0x8000) == 0) {
        shift += 1;
    }
    return (((s16)*mask >> (shift & 31)) ^ 1) & 1;
}
