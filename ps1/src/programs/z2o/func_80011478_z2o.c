/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011478_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * No call of this function was found in the program (inferred from a scan of
 * the program's file for a jal or j to 0x80011478, for the address as a 32 bit data
 * word, and for a lui and addiu or ori pair that builds it; none of the
 * three was found). It is C for completeness.
 *
 * What it does (inferred, not an original name): draws bands. It builds a
 * rectangle (a local: x and y from its second and third arguments, the width
 * step = 0x18 when the sixth argument is not 0, else 0x10, and the height from
 * the fifth argument, each stored as a halfword), takes the stride
 * bytes = ((step * height) >> 1) << 2 (the product as a signed 32 bit number,
 * the shift logical), and the count of bands = total >> 4 (logical). Then, for
 * each band in turn: LoadImage (the game's name func_80157fc4) with the rectangle and the
 * source pointer, DrawSync(0), the rectangle's x grows by step (a halfword),
 * the source pointer by bytes. Nothing is drawn when the count is 0. No
 * result (inferred: the picture is sent in strips of the movie's width).
 *
 * Contract:
 *   Arguments: a0 = the source pointer, a1 = x, a2 = y, a3 = the total
 *     (only total >> 4 is used); the fifth argument (the height) and the
 *     sixth (the wide flag) are on the caller's stack at sp + 0x10 and
 *     sp + 0x14. No result.
 *   Reads: the two stack words. Nothing of the program's memory.
 *   Writes: nothing of the program's memory (the local rectangle aside).
 *   Callees, both recorders in the test, never run: func_80157fc4(rect, src)
 *     (2 arguments; the rectangle's address is the address of a local and is
 *     masked, the two words behind it are logged), DrawSync(mode) (1
 *     argument).
 *   Slots no input can reach: none known. The setup keeps the count of bands
 *     to 8 or fewer (a count of millions would be as many calls), so the
 *     loop's unsigned comparison is not told from a signed one by any case.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011478_z2o(u8 *src, int x, int y, u32 total, int height, int wide) {
    Rect rect;
    int step;
    int bytes;
    u32 count;
    u32 i;

    step = 0x10;
    if (wide != 0) {
        step = 0x18;
    }
    rect.x = x;
    rect.y = y;
    rect.w = step;
    rect.h = height;
    bytes = ((u32)(step * height) >> 1) << 2;
    count = total >> 4;
    for (i = 0; i < count; i++) {
        func_80157fc4(&rect, src);
        DrawSync(0);
        rect.x += step;
        src += bytes;
    }
}
