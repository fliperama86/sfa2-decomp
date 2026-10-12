/*
 * The port's C for func_80141534. It runs in the PC program in place of the
 * C of the unit e41534_r1.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the shift count is masked with 31 in
 * the return statement. The unit shifts by the count as it is.
 *
 * Why: the count is the first byte of `dir` halved, plus 6 when the second
 * byte is not 0, plus 1 when flag 0x8000 is clear, so it can reach 134
 * (127 + 6 + 1). The original shifts with a variable shift instruction
 * (srav at 801415ac), and the console takes the count modulo 32 (read from
 * the original's listing, not tested here). In C a shift by 32 or more has
 * no defined result, so compiled for a PC nothing ties the unit's C to the
 * console's result for such a count. With the mask written the count is
 * defined, and for counts below 32 this file and the unit do the same. The
 * readable C that stood for this function before it was exact wrote the
 * mask too. Whether the game calls this function with a count of 32 or more
 * is not known.
 *
 * What the function does (inferred): see the comment above the function.
 *
 * Contract:
 *   Arguments: a0 = object; a1 = the byte pair `dir`, passed in the upper
 *     half of a1 (first byte in bits 16 to 23, second in bits 24 to 31; the
 *     lower half of a1 is not read). Result: v0 = 0 or 1.
 *   Reads: object->input_bits_2 and object->kind; table_8017179c[kind] (a
 *     pointer to two 16-bit entries) and the entry.
 *   Writes: nothing. No callee.
 *   Aliasing: none that matters (nothing is written).
 *   Exclusions: kind is limited by the setup to the 8 table entries the
 *     setup fills with its own blocks. All instruction slots are reachable.
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

/* What it does (inferred): tests one bit of a 16-bit mask. The object's
   input_bits_2 flags say whether a mask applies at all (bits 0xa000); if not,
   the result is 1. Otherwise the mask is the 16-bit entry (the second one
   when flag 0x4000 is set) of a table selected by the object's kind, and
   the bit tested is chosen by the pair dir: its first byte halved, plus 6
   when the second byte is not 0, plus 1 when flag 0x8000 is clear. The
   result is 1 when that bit is clear, 0 when set. */
s16 func_80141534(Object *object, BytePair dir) {
    int flags = object->input_bits_2;
    u16 *mask;
    int shift;
    s16 bits;

    if ((flags & 0xa000) == 0) {
        return 1;
    }
    mask = table_8017179c[object->kind];
    if (flags & 0x4000) {
        mask++;
    }
    shift = dir.first >> 1;
    bits = *mask;
    if (dir.second != 0) {
        shift += 6;
    }
    if ((flags & 0x8000) == 0) {
        shift += 1;
    }
    /* The override: the count is masked, as the console's shift does it. */
    return ((bits >> (shift & 31)) ^ 1) & 1;
}
