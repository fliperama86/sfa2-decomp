/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800110d0_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): one frame of the movie. It
 * takes the next frame from the stream's ring, hands the size values to the
 * decoder, starts the decoder on the bitstream buffer of the frame before,
 * and decodes the new frame's bitstream. The listing has no wait and no
 * flip: those are not in this function. With x in a0 and y in a1:
 *   1. calls StGetNext(&addr, &hdr) (addr and hdr are locals, a pointer
 *      each, which the callee fills) up to 0x100000 times, until it returns
 *      0; after 0x100000 returns that are not 0 the function returns 1 and
 *      nothing else is done (the time-out);
 *   2. let prev be the frame number in the header copy (word 2 of the block
 *      at 0x8002f84c), w and h the halfwords at 0x8002f85c and 0x8002f85e.
 *      Stores x and y in the halfwords at 0x8002f874 and 0x8002f876; prev
 *      in the word at 0x8002f894; w and h in the words at 0x8002f898 and
 *      0x8002f89c; subtracts 1 from the word at 0x8002f86c; the result
 *      value is 1 when prev is not below the word at 0x8002f814 (unsigned),
 *      else 0;
 *   3. the halfword at 0x8002f878 is 0x18 when the word at 0x8002f818 is not
 *      0, else 0x10; the word at 0x8002f888 is 0; the word at 0x8002f88c is
 *      w >> 4; the halfword at 0x8002f87a is h; the word at 0x8002f87c is
 *      the pointer in the word at 0x8002f80c; the word at 0x8002f884 is
 *      12 * h when the word at 0x8002f818 is not 0, else 8 * h; the word at
 *      0x8002f880 is 0 when the word at 0x8002f870 is 0, else the same
 *      value as 0x8002f884 (computed from the word at 0x8002f89c, read
 *      again);
 *   4. DecDCTReset(1); DecDCTin(b, n) with b the pointer in the word at
 *      0x8002f804 + 4 * i, i the word at 0x8002f820, and n the word at
 *      0x8002f818 (both read again); DecDCTout(s, n2) with s the word at
 *      0x8002f80c and n2 the word at 0x8002f884 (both read again);
 *   5. the word at 0x8002f820 becomes 1 when it is 0 and else 0; copies the
 *      32 bytes at hdr to 0x8002f84c; sets the halfwords at 0x8002f85c and
 *      0x8002f85e to (x + 8) / 16 * 16 each (x the halfword's value, 16 bits
 *      kept); DecDCTvlc(addr, b2) with b2 the pointer at 0x8002f804 + 4 * i2,
 *      i2 the new word at 0x8002f820; StFreeRing(addr);
 *   6. returns the value of step 2.
 *
 * Contract:
 *   Arguments: a0 = x, a1 = y (the low 16 bits are stored). Result in v0:
 *     the value of step 2 (or 1 at the time-out).
 *   Reads: the words at 0x8002f86c, 0x8002f814, 0x8002f818, 0x8002f870,
 *     0x8002f80c, 0x8002f820, 0x8002f804 and 0x8002f808, the header copy at
 *     0x8002f84c (word 2 and the two halfwords), the 32 bytes at hdr, the
 *     results of the callees.
 *   Writes: the halfwords at 0x8002f874 to 0x8002f87a, the words at
 *     0x8002f86c, 0x8002f87c, 0x8002f880, 0x8002f884, 0x8002f888, 0x8002f88c,
 *     0x8002f894, 0x8002f898, 0x8002f89c, 0x8002f820, the 32 bytes at
 *     0x8002f84c, the locals.
 *   Callees, all recorders in the test, never run: StGetNext(a, h) (two
 *     arguments, both the address of a local, masked to 0; its results are
 *     scripted and its fills store the pointers through the two arguments:
 *     garbage at the calls before the last and at the last the pointer addr
 *     and the pointer to a 32 byte header block of the setup's),
 *     DecDCTReset(1), DecDCTin(b, n), DecDCTout(s, n), DecDCTvlc(bs, buf),
 *     StFreeRing(a). Watched at every call: the words at 0x8002f804 to
 *     0x8002f820, the 32 bytes at 0x8002f84c and the word at 0x8002f86c.
 *   Limit of the test: the time-out needs 0x100000 returns of StGetNext that
 *     are not 0. The tool's instruction budget (BUDGET in difftest.py,
 *     2,000,000 instructions) is below what that count needs (every return
 *     runs the loop's code and the recorder's, more than two instructions),
 *     so no case reaches the time-out, and its two instruction slots (the
 *     jump and the load of 1) stay unreached (the coverage line of the tool
 *     names them). Nothing here tests the counter's start value or the value
 *     1 of the exit.
 *   Slots no input can reach: those two (see the limit), and the five of
 *     the corrections for a negative value that the signed divisions take
 *     (the `addiu` of 3 with its jump and the shift after it at +0x114 to
 *     +0x11c, for 48 * h / 4; the `addiu` of 0x17 at +0x24c and at +0x270,
 *     for the rounding of the two halfwords): the values come from `lhu`
 *     loads, so the products and sums are never negative.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

int func_800110d0_z2o(int x, int y) {
    u32 *addr;
    u32 *hdr;
    int count;
    int ret;
    u32 prev;
    u16 w;
    u16 h;

    count = 0x100000;
    while (StGetNext(&addr, &hdr) != 0) {
        count--;
        if (count == 0) {
            return 1;
        }
    }
    prev = data_8002f84c_z2o.frame;
    w = data_8002f84c_z2o.width;
    h = data_8002f84c_z2o.height;
    data_8002f874_z2o.x = x;
    data_8002f874_z2o.y = y;
    data_8002f894_z2o = prev;
    data_8002f86c_z2o = data_8002f86c_z2o - 1;
    data_8002f898_z2o = w;
    data_8002f89c_z2o = h;
    ret = prev >= (u32)data_8002f814_z2o;
    data_8002f874_z2o.w = data_8002f818_z2o != 0 ? 0x18 : 0x10;
    data_8002f888_z2o = 0;
    data_8002f88c_z2o = w >> 4;
    data_8002f874_z2o.h = data_8002f89c_z2o;
    data_8002f87c_z2o = data_8002f80c_z2o;
    data_8002f884_z2o = data_8002f818_z2o != 0 ? h * 12 : h * 8;
    if (data_8002f870_z2o == 0) {
        data_8002f880_z2o = 0;
    } else {
        data_8002f880_z2o = data_8002f818_z2o != 0 ? data_8002f89c_z2o * 12 : data_8002f89c_z2o * 8;
    }
    DecDCTReset(1);
    DecDCTin(data_8002f804_z2o[data_8002f820_z2o], data_8002f818_z2o);
    DecDCTout(data_8002f80c_z2o, data_8002f884_z2o);
    data_8002f820_z2o = data_8002f820_z2o == 0;
    data_8002f84c_z2o = *(Z2oFrameInfo *)hdr;
    data_8002f84c_z2o.width = (data_8002f84c_z2o.width + 8) / 16 * 16;
    data_8002f84c_z2o.height = (data_8002f84c_z2o.height + 8) / 16 * 16;
    DecDCTvlc(addr, data_8002f804_z2o[data_8002f820_z2o]);
    StFreeRing(addr);
    return ret;
}
