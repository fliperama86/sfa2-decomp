/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and in how a constant is folded. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py, with
 * func_80012990_slot01.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fills textured-quad
 * primitives for a set of sprites, twice over the same set (the second
 * pass continues in the primitive array where the first stopped). The
 * object's sequence step points at a set: an s16 count, a list of codes
 * (bytes) and a list of offsets (pairs of u16). For each sprite it
 * initialises the primitive through func_8015c09c, sets the three colour
 * bytes to 0x80, the texture page, a clut-like halfword (base plus the
 * code times 64) and the four corner texture coordinates: the offset
 * plus (0x60, 0x50), a square of 16. When the flag is not 0 the square is
 * mirrored in x (the x coordinates are exchanged: the left pair gets x +
 * 0x6f and the right pair x + 0x60). A set with a count of 0 or less
 * still runs its loop once (the loop is a do-while).
 *
 * Contract:
 *   Arguments: a0 = pointer to an object, a1 = pointer to the primitive
 *     array (Poly28, 0x28 bytes each), a2 = the texture page (low 16 bits
 *     used), a3 = the base of the clut-like halfword, and the fifth
 *     argument, the flag, on the stack at sp + 0x10. No return value.
 *   Reads: the object's sequence; the step's field_04 (the set); the set's
 *     s16 count at offset 0, codes pointer at 4 and offsets pointer at 8.
 *   Writes: for each sprite of each pass the primitive's offsets 0x04,
 *     0x05, 0x06, 0x0c, 0x0d, 0x0e, 0x14, 0x15, 0x16, 0x1c, 0x1d, 0x24,
 *     0x25. Nothing else (the object, the set and its lists are not
 *     written).
 *   Callee: func_8015c09c (one argument, the primitive) is replaced by a
 *     recorder, result 0. The recorder copies the whole primitive array
 *     into the log at every call (a watched block), so the writes that
 *     happen before a call are shown as the callee would see them.
 *   Aliasing: the object, the step, the set, its lists and the primitive
 *     array are distinct blocks.
 *   Excluded inputs: none; the setup keeps the count to at most 12 so that
 *     the log has room.
 *   Not reached by any input: none (every instruction slot of the original
 *     is executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8015c09c(void *prim);

void func_80012990_slot01(Object *obj, Poly28 *prim, u16 tpage, int base, int flag) {
    SpriteSet *set = (SpriteSet *)obj->sequence->field_04;
    int pass;
    int n;
    u8 *codes;
    u16 *offs;
    int x;
    int y;

    for (pass = 0; pass < 2; pass++) {
        n = set->count;
        codes = set->codes;
        offs = set->offsets;
        do {
            func_8015c09c(prim);
            prim->field_04 = 0x80;
            prim->field_05 = 0x80;
            prim->field_06 = 0x80;
            prim->field_16 = tpage;
            x = *offs++ + 0x60;
            y = *offs++ + 0x50;
            if (flag == 0) {
                prim->field_0c = x;
                prim->field_1c = x;
                prim->field_14 = x + 0x10;
                prim->field_24 = x + 0x10;
            } else {
                prim->field_0c = x + 0xf;
                prim->field_1c = x + 0xf;
                prim->field_14 = x;
                prim->field_24 = x;
            }
            prim->field_0d = y;
            prim->field_15 = y;
            prim->field_1d = y + 0x10;
            prim->field_25 = y + 0x10;
            prim->field_0e = base + (*codes++ << 6);
            prim++;
            n--;
        } while (n > 0);
    }
}
