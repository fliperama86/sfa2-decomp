/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800111d0_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): draws a still picture at a
 * place (x, y), given as the two arguments. The program keeps a pointer p at
 * 0x8002eb10 (to a block of at least 0x24 bytes) and a pointer q at
 * 0x8002eb14 (to a block of 0x500 * 12 bytes, the program's picture buffer,
 * inferred). In order:
 *   1. reads the words p[0], p[4], p[8] (word indexes: the byte offsets 0,
 *      0x10 and 0x20) once, and writes them in that order, over and over,
 *      0x500 times, to q (3 words each time, from q on, 0x500 * 12 bytes);
 *   2. sets the rectangle at 0x8002eb24 (x, y, w, h as halfwords) to
 *      (x, y, 0x1e0, 0x10);
 *   3. fifteen times (the rectangle's y runs from y to y + 0xe0): calls
 *      LoadImage(&rect, q) (the game's name for the program's copy is
 *      func_80157fc4), then DrawSync(0), then adds 0x10 to the rectangle's y;
 *   4. sets the rectangle to (x + 0x48, y + 0x60, 0x150, 0x30) (x is the
 *      halfword read back from the rectangle, so it is cut to 16 bits) and
 *      calls LoadImage(&rect, p) once.
 * All arithmetic on the rectangle is on halfwords (it wraps at 16 bits).
 *
 * Contract:
 *   Arguments: a0 = x, a1 = y (in the test random: 16-bit values, values
 *     with garbage in the upper half and values at the edges of the
 *     halfword; only the low 16 bits are stored).
 *   Result: none.
 *   Reads: the pointers p and q, the three words behind p.
 *   Writes: the 0x500 * 12 bytes behind q; the four halfwords of the
 *     rectangle.
 *   Callees, recorders in the test, never run: func_80157fc4(rect, src)
 *     (two arguments; the recorder logs both and the two words behind rect,
 *     so that the halfwords are compared at every call), DrawSync(mode)
 *     (one argument). 16 + 15 calls.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_800111d0_logo(int x, int y) {
    u32 *src;
    u32 *dst;
    u32 a, b, c;
    int n;
    int i;

    src = (u32 *)data_8002eb10_logo;
    dst = data_8002eb14_logo;
    a = src[0];
    b = src[4];
    c = src[8];
    for (n = 0; n < 0x500; n++) {
        *dst++ = a;
        *dst++ = b;
        *dst++ = c;
    }
    data_8002eb24_logo.w = 0x1e0;
    data_8002eb24_logo.x = x;
    data_8002eb24_logo.y = y;
    data_8002eb24_logo.h = 0x10;
    for (i = 0; i < 0xf0; i += 0x10) {
        func_80157fc4(&data_8002eb24_logo, (u8 *)data_8002eb14_logo);
        DrawSync(0);
        data_8002eb24_logo.y += 0x10;
    }
    data_8002eb24_logo.y = y + 0x60;
    data_8002eb24_logo.w = 0x150;
    data_8002eb24_logo.h = 0x30;
    data_8002eb24_logo.x += 0x48;
    func_80157fc4(&data_8002eb24_logo, data_8002eb10_logo);
}
