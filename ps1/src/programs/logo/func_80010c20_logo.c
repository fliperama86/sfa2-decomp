/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010c20_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): one frame of the movie.
 * It takes the next frame from the stream's ring, hands the frame before it
 * to the decoder, and draws a still when the frame number says so. In order:
 *   1. y = 0xf0 when the index word at 0x8002eb50 is not 0, else 0;
 *   2. calls StGetNext(&addr, &hdr) (addr and hdr are locals, a pointer
 *      each, which the callee fills) up to 0x100000 times, until it
 *      returns 0; after 0x100000 returns that are not 0 the function
 *      returns 1 and nothing else is done (the time-out);
 *   3. moves the frame numbers: the word at 0x8002eb2c gets the word at
 *      0x8002eb30 (the frame before), the word at 0x8002eb30 gets hdr[2]
 *      (the new frame's number); sets the rectangle at 0x8002eb24 to
 *      (0, y, 0x18, 0xf0); calls DecDCTReset(1);
 *   4. let p be the word at 0x8002eb2c (read again). When p - 0xad is below
 *      4 (unsigned), expand with the entry p - 0xac of the two tables
 *      (the pointers in the words at 0x8002eb48 and 0x8002eb4c): calls
 *      func_800110f4_logo(t, c) with t = table1[p - 0xac], c = table2[p -
 *      0xac], then func_800111d0_logo(0, y). Else, when p - 0xb1 is below
 *      0x43, the same with entry 0. Else, when p - 0xf4 is below 0xb, the
 *      same with entry p - 0xef. Else (not a still): calls
 *      DecDCTin(b, 1) with b the pointer data_8002eb08_logo[i], i the word
 *      at 0x8002eb20, and DecDCTout(s, 0xb40) with s the pointer in the
 *      word at 0x8002eb00;
 *   5. stores in the word at 0x8002eb20 1 when it is 0 and else 0;
 *      when the word at 0x8002eb30 minus 0xad is not below 0x52 (unsigned)
 *      calls DecDCTvlc(addr, b) with b the pointer of the table at
 *      0x8002eb08 named by the word at 0x8002eb20 (new value); calls
 *      StFreeRing(addr);
 *   6. when the word at 0x8002eb2c minus 0xad is not below 0x52 (unsigned)
 *      calls func_80010f20_logo() (the wait for the strips);
 *   7. calls VSync(0) when the word at 0x8002eb38 is not 0 and else
 *      VSync(2), then func_80010a38_logo() (the flip);
 *   8. returns 1 when the frame number the function moved into 0x8002eb2c
 *      is at least 0x118, else 0.
 *
 * Contract:
 *   Arguments: none. Result in v0: the value of step 8 (or 1 at the time-out).
 *   Reads: the words at 0x8002eb50, 0x8002eb30, 0x8002eb2c, 0x8002eb20,
 *     0x8002eb38, 0x8002eb00, 0x8002eb48, 0x8002eb4c, the table of two
 *     pointers at 0x8002eb08, the entries 0 to 15 of the two tables, the
 *     header's word 2 through hdr, the results of the callees.
 *   Writes: the words at 0x8002eb2c, 0x8002eb30, 0x8002eb20, the rectangle
 *     at 0x8002eb24, the locals.
 *   Callees, all recorders in the test, never run: StGetNext(a, h) (two
 *     arguments, both the address of a local, masked to 0; the recorder's
 *     results are scripted, 0 to 8 returns that are not 0 and then 0, and
 *     its fills store the pointers through the two arguments: garbage at
 *     the calls before the last and at the last the pointer addr and the
 *     pointer to a header block of the setup's), DecDCTReset(1),
 *     func_800110f4_logo(t, c), func_800111d0_logo(x, y), DecDCTin(b, n),
 *     DecDCTout(s, n), DecDCTvlc(bs, buf), StFreeRing(a),
 *     func_80010f20_logo(), VSync(mode), func_80010a38_logo(). The words
 *     at 0x8002eb20, the rectangle and the words at 0x8002eb2c and
 *     0x8002eb30 are watched, so that their values at every call are
 *     compared.
 *   Limit of the test: the time-out needs 0x100000 returns of StGetNext that
 *     are not 0. The tool's instruction budget (BUDGET in difftest.py,
 *     2,000,000 instructions) is below what that count needs: every return
 *     runs the loop (a call with its delay slot, a test, a decrement, a
 *     branch) and the recorder's code, more than two instructions, and
 *     0x100000 times two is more than the budget. So no case reaches the
 *     time-out, and the two instruction slots of the exit, 0x80010c68 and
 *     0x80010c6c (the jump and the load of 1), stay unreached (the coverage
 *     line of the tool names them: not executed +0x48..+0x4c). Nothing here
 *     tests the counter's start value or the value 1 of the exit.
 *   Slots no input can reach: those two (see the limit).
 */
#include "logo.h"

int func_80010c20_logo(void) {
    u32 *addr;
    u32 *hdr;
    int y;
    int count;
    int ret;
    u32 prev;
    int i;

    y = data_8002eb50_logo != 0 ? 0xf0 : 0;
    count = 0x100000;
    while (StGetNext(&addr, &hdr) != 0) {
        count--;
        if (count == 0) {
            return 1;
        }
    }
    prev = data_8002eb30_logo;
    data_8002eb24_logo.w = 0x18;
    data_8002eb2c_logo = prev;
    data_8002eb24_logo.x = 0;
    data_8002eb24_logo.y = y;
    data_8002eb24_logo.h = 0xf0;
    data_8002eb30_logo = hdr[2];
    DecDCTReset(1);
    ret = prev >= 0x118;
    if ((u32)(data_8002eb2c_logo - 0xad) < 4) {
        i = data_8002eb2c_logo - 0xac;
    } else if ((u32)(data_8002eb2c_logo - 0xb1) < 0x43) {
        i = 0;
    } else if ((u32)(data_8002eb2c_logo - 0xf4) < 0xb) {
        i = data_8002eb2c_logo - 0xef;
    } else {
        i = -1;
    }
    if (i >= 0) {
        func_800110f4_logo(data_8002eb48_logo[i], data_8002eb4c_logo[i]);
        func_800111d0_logo(0, y);
    } else {
        DecDCTin(data_8002eb08_logo[data_8002eb20_logo], 1);
        DecDCTout(data_8002eb00_logo, 0xb40);
    }
    data_8002eb20_logo = data_8002eb20_logo == 0;
    if ((u32)(data_8002eb30_logo - 0xad) >= 0x52) {
        DecDCTvlc(addr, data_8002eb08_logo[data_8002eb20_logo]);
    }
    StFreeRing(addr);
    if ((u32)(data_8002eb2c_logo - 0xad) >= 0x52) {
        func_80010f20_logo();
    }
    VSync(data_8002eb38_logo != 0 ? 0 : 2);
    func_80010a38_logo();
    return ret;
}
