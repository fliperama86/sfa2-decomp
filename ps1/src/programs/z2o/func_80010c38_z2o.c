/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010c38_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): opens the movie. It starts
 * the stream at a sector, stores the buffers and the sizes it is given, and
 * takes the first frame. Ten arguments, the first four in a0 to a3 and the
 * others on the caller's stack (sector, stream, frames, ring, ring_size,
 * buf0, buf1, strip, wide, words). In order:
 *   1. stores frames in the word at 0x8002f814; 0 in the words at
 *      0x8002f820, 0x8002f890, 0x8002f894 and 0x8002f86c; buf0, buf1 and
 *      strip in the words at 0x8002f804, 0x8002f808 and 0x8002f80c; words in
 *      the words at 0x8002f818 and 0x8002f81c; wide in the word at
 *      0x8001b348;
 *   2. DecDCTReset(0), DecDCToutCallback(func_80011708_z2o),
 *      StSetRing(ring, ring_size), StSetStream(1, stream, 0x0fffffff,
 *      func_80011810_z2o, 0) (five arguments, the fifth on the stack),
 *      func_800118f0_z2o();
 *   3. stores 1 in the byte at 0x8002f8a8 and calls CdIntToPos(sector,
 *      0x8002f8ac);
 *   4. CdControl(0x0d, 0x8002f8a8, 0) until it returns a value that is not
 *      0, then CdControl(2, 0x8002f8ac, 0) until it returns a value that is
 *      not 0;
 *   5. a local mode word is 0x1e8 when the word at 0x8001b348 (read again
 *      here) is not 0, else 0x168; CdControlB(0x0e, &mode, 0) until it
 *      returns a value that is not 0;
 *   6. VSync(0), 51 times (a counter from 0x32 down to -1);
 *   7. CdRead2(mode) until it returns a value that is not 0; after each
 *      return of 0, CdControl(2, 0x8002f8ac, 0) until it returns a value
 *      that is not 0;
 *   8. CdControlB(1, 0, 0x8002f8c4) until it returns a value that is not 0,
 *      and the same again while bit 0x20 of the byte at 0x8002f8c4 is clear;
 *   9. func_80011830_z2o(); StGetNext(&addr, &hdr) until it returns 0 (addr
 *      and hdr are locals, a pointer each, which the callee fills), no
 *      time-out;
 *  10. subtracts 1 from the word at 0x8002f86c (read again here); copies the
 *      32 bytes at hdr to 0x8002f84c; sets the halfwords at 0x8002f85c and
 *      0x8002f85e to (x + 8) / 16 * 16 each (x the halfword's value, 16 bits
 *      kept);
 *  11. DecDCTvlc(addr, b) with b the pointer in the word at 0x8002f804 + 4 *
 *      i, i the word at 0x8002f820 (read again here); StFreeRing(addr);
 *  12. returns 0.
 *
 * Contract:
 *   Arguments: a0 = sector, a1 = stream, a2 = frames, a3 = ring, and the
 *     stack words at sp + 0x10 to sp + 0x24: ring_size, buf0, buf1, strip,
 *     wide, words. Result in v0: 0.
 *   Reads: the word at 0x8001b348 (after the calls of steps 2 to 4, which
 *     may change it as an interrupt could), the byte at 0x8002f8c4 (the
 *     recorder of CdControlB fills it), the words at 0x8002f86c and
 *     0x8002f820 (after StGetNext, which may change them), the halfwords at
 *     0x8002f85c and 0x8002f85e (after the copy), the 32 bytes at hdr, the
 *     results of the callees.
 *   Writes: the words at 0x8002f804 to 0x8002f81c, 0x8002f820, 0x8002f86c,
 *     0x8002f890, 0x8002f894, 0x8001b348, the byte at 0x8002f8a8, the 32
 *     bytes at 0x8002f84c, the locals.
 *   Callees, all recorders in the test, never run: DecDCTReset(mode),
 *     DecDCToutCallback(f), StSetRing(r, n), StSetStream (five arguments),
 *     func_800118f0_z2o(), CdIntToPos(i, p) (the fill stores a word of the
 *     setup's through p), CdControl(com, param, result) (three arguments,
 *     one word behind the second logged, results scripted), CdControlB(com,
 *     param, result) (three arguments; the pointer of the first call is the
 *     address of a local and is masked, the word behind it logged; its
 *     results and the byte it stores in res[0] are scripted per call),
 *     VSync(mode), CdRead2(mode) (results scripted), func_80011830_z2o(),
 *     StGetNext(a, h) (two arguments, both the address of a local, masked
 *     to 0; the fills store the pointers through them), DecDCTvlc(bs, buf),
 *     StFreeRing(a). Watched at every call: the words at 0x8002f804 to
 *     0x8002f820, the 32 bytes at 0x8002f84c, the word at 0x8002f86c and the
 *     word at 0x8001b348.
 *   Slots no input can reach: the two at +0x27c and +0x2a0 (the `addiu` of
 *     0x17 that the division by 16 takes when a halfword plus 8 is
 *     negative): the halfwords are loaded unsigned (`lhu`), so the sum is
 *     never negative.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

int func_80010c38_z2o(int sector, int stream, int frames, u32 *ring, int ring_size, u32 *buf0, u32 *buf1,
                      u32 *strip, int wide, int words) {
    int mode;
    int count;
    u32 *addr;
    u32 *hdr;

    data_8002f814_z2o = frames;
    data_8002f820_z2o = 0;
    data_8002f890_z2o = 0;
    data_8002f894_z2o = 0;
    data_8002f86c_z2o = 0;
    data_8002f804_z2o = buf0;
    data_8002f808_z2o = buf1;
    data_8002f80c_z2o = strip;
    data_8002f818_z2o = words;
    data_8002f81c_z2o = words;
    data_8001b348_z2o = wide;
    DecDCTReset(0);
    DecDCToutCallback(func_80011708_z2o);
    StSetRing(ring, ring_size);
    StSetStream(1, stream, 0x0fffffff, func_80011810_z2o, 0);
    func_800118f0_z2o();
    data_8002f8a8_z2o = 1;
    CdIntToPos(sector, data_8002f8ac_z2o);
    while (CdControl(0x0d, &data_8002f8a8_z2o, 0) == 0) {
    }
    while (CdControl(2, data_8002f8ac_z2o, 0) == 0) {
    }
    mode = data_8001b348_z2o != 0 ? 0x1e8 : 0x168;
    while (CdControlB(0x0e, (u8 *)&mode, 0) == 0) {
    }
    count = 0x32;
    do {
        VSync(0);
        count--;
    } while (count >= 0);
    while (CdRead2(mode) == 0) {
        while (CdControl(2, data_8002f8ac_z2o, 0) == 0) {
        }
    }
    do {
        while (CdControlB(1, 0, data_8002f8c4_z2o) == 0) {
        }
    } while ((data_8002f8c4_z2o[0] & 0x20) == 0);
    func_80011830_z2o();
    while (StGetNext(&addr, &hdr) != 0) {
    }
    data_8002f86c_z2o = data_8002f86c_z2o - 1;
    data_8002f84c_z2o = *(Z2oFrameInfo *)hdr;
    data_8002f84c_z2o.width = (data_8002f84c_z2o.width + 8) / 16 * 16;
    data_8002f84c_z2o.height = (data_8002f84c_z2o.height + 8) / 16 * 16;
    DecDCTvlc(addr, (&data_8002f804_z2o)[data_8002f820_z2o]);
    StFreeRing(addr);
    return 0;
}
