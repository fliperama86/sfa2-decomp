/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010a80_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): starts the movie's stream
 * at a sector and takes the first frame from it. With the sector in a0:
 *   1. stores 0 in the words at 0x8002eb20 (the index of the buffer),
 *      0x8002eb3c (the flag the decoder's callback clears) and 0x8002eb2c;
 *   2. DecDCTReset(0), DecDCToutCallback(func_80010f38_logo), StSetRing(r,
 *      15) with r the pointer in the word at 0x8002eb04, StSetStream(1, 1,
 *      0x0fffffff, 0, 0) (five arguments, the fifth on the stack);
 *   3. sets the two filter bytes at 0x8002eb44 to 1 and calls
 *      CdIntToPos(sector, 0x8002ebb4);
 *   4. a loop; each time round it:
 *        - calls CdControl(0x0d, 0x8002eb44, 0); a return of 0 starts the
 *          loop again, and so does a 0 from each of the next two calls,
 *          CdControl(0x15, 0x8002ebb4, 0) and CdRead2(mode);
 *        - calls CdControlB(1, 0, res) (res is a local of eight bytes) until
 *          it returns a value that is not 0, and the same again while bit
 *          0x20 of res[0] is clear;
 *        - calls CdControlB(0x11, 0, res) until it returns a value that is
 *          not 0 and pos = CdPosToInt(res + 5);
 *        - starts again when pos is below sector - 100 (signed) or when
 *          sector + 5 is below pos (unsigned); otherwise the loop ends;
 *   5. StGetNext(&addr, &hdr) until it returns 0 (addr and hdr are locals,
 *      a pointer each, which the callee fills), no time-out;
 *   6. stores word 2 of the header, hdr[2], in the word at 0x8002eb30 (the
 *      frame number), then DecDCTvlc(addr, b) with b the pointer
 *      data_8002eb08_logo[i], i being the word at 0x8002eb20 read again
 *      here, and StFreeRing(addr);
 *   7. returns 0.
 *
 * Contract:
 *   Arguments: a0 = sector (any int in the test, see the .py). Result in
 *     v0: 0.
 *   Reads: the pointer word at 0x8002eb04, the two pointers at 0x8002eb08
 *     (the index word at 0x8002eb20, read after the call to StGetNext, is
 *     0 or 1 in the test), the header's word 2 through the pointer that
 *     StGetNext leaves in hdr, the byte res[0], the results of the callees.
 *   Writes: the words at 0x8002eb20, 0x8002eb3c, 0x8002eb2c and 0x8002eb30,
 *     the two bytes at 0x8002eb44 (the 1s), the locals.
 *   Callees, all recorders in the test, never run: DecDCTReset(mode),
 *     DecDCToutCallback(f), StSetRing(r, n), StSetStream (five arguments),
 *     CdIntToPos(i, p) (two arguments; a fill stores four bytes of the
 *     setup's through p, the model of what the library writes: the
 *     recorder of CdControl logs the word behind its second argument, so
 *     the bytes at 0x8002ebb4 are compared at the call with command 0x15),
 *     CdControl(com, param, result) (three arguments, one word behind the
 *     second logged), CdRead2(mode), CdControlB(com, param, result) (three
 *     arguments; its result and the byte its fill stores in res[0] are
 *     scripted per call), CdPosToInt(p) (one argument, scripted results;
 *     the pointer is logged as it is, so the test also compares where res
 *     lies: it passes only while the C's frame puts res at the address the
 *     original uses, which it does because the C keeps the number 0x1e8 of
 *     CdRead2 in a local, as the original keeps it in a saved register, so
 *     that the two frames have the same size), StGetNext(a, h) (two arguments, both
 *     the address of a local, masked to 0; the recorder's fills store the
 *     pointers through them, and the final call's differ from those of the
 *     calls before it), DecDCTvlc(bs, buf) (two arguments), StFreeRing(a).
 *     The words at 0x8002eb20, 0x8002eb2c and 0x8002eb30, 0x8002eb3c and the
 *     bytes at 0x8002eb44 are watched, so that their values at every call
 *     are compared; a store of the recorder of StGetNext changes the
 *     index word (as an interrupt could), so that the re-read shows.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

int func_80010a80_logo(int sector) {
    u8 res[8];
    u32 *addr;
    u32 *hdr;
    int pos;
    int mode;

    mode = 0x1e8;
    data_8002eb20_logo = 0;
    data_8002eb3c_logo = 0;
    data_8002eb2c_logo = 0;
    DecDCTReset(0);
    DecDCToutCallback(func_80010f38_logo);
    StSetRing(data_8002eb04_logo, 15);
    StSetStream(1, 1, 0x0fffffff, 0, 0);
    data_8002eb44_logo[0] = 1;
    data_8002eb44_logo[1] = 1;
    CdIntToPos(sector, data_8002ebb4_logo);
    for (;;) {
        if (CdControl(0x0d, data_8002eb44_logo, 0) == 0) {
            continue;
        }
        if (CdControl(0x15, data_8002ebb4_logo, 0) == 0) {
            continue;
        }
        if (CdRead2(mode) == 0) {
            continue;
        }
        do {
            while (CdControlB(1, 0, res) == 0) {
            }
        } while ((res[0] & 0x20) == 0);
        while (CdControlB(0x11, 0, res) == 0) {
        }
        pos = CdPosToInt(res + 5);
        if (pos < sector - 100) {
            continue;
        }
        if ((u32)(sector + 5) < (u32)pos) {
            continue;
        }
        break;
    }
    while (StGetNext(&addr, &hdr) != 0) {
    }
    data_8002eb30_logo = hdr[2];
    DecDCTvlc(addr, data_8002eb08_logo[data_8002eb20_logo]);
    StFreeRing(addr);
    return 0;
}
