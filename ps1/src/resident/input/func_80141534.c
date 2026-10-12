/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

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
    /* The entry is read here, before the count is complete; read in the return statement, this function differs from the original in 17 instruction lines. */
    bits = *mask;
    if (dir.second != 0) {
        shift += 6;
    }
    if ((flags & 0x8000) == 0) {
        shift += 1;
    }
    /* The count is not masked: it can reach 134, and the original shifts by it as it is (the console takes the count modulo 32). With (shift & 31) written, this function differs from the original in 2 instruction lines. The port has its own C for this function, port/overrides/func_80141534.c, which writes the mask. */
    return ((bits >> shift) ^ 1) & 1;
}
