/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010e28_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): ends the movie. In order:
 *   1. func_80011060_logo() (the volume down), StUnSetRing(), CdReady(0, 0);
 *   2. CdGetSector(p, 0x200) with p the pointer in the word at 0x8002eb08;
 *   3. stores the byte 0x80 at 0x8002eafc (the disc mode byte) and sends
 *      CdControlB(0x0e, 0x8002eafc, 0) until it returns a value that is
 *      not 0;
 *   4. sends CdControlB(9, 0, 0) until it returns a value that is not 0;
 *   5. DecDCToutCallback(0);
 *   6. twice: makes the rectangle (0, y, 0x1e0, 0xf0) in a local, where y
 *      is 0xf0 when the index word at 0x8002eb50 is not 0 (read at each
 *      turn) and else 0, calls ClearImage(&rect, 0, 0, 0), DrawSync(0),
 *      VSync(0) and func_80010a38_logo() (the flip of the display
 *      environment, which changes the index word).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the word at 0x8002eb08 (any word in the test, only passed on),
 *     the index word at 0x8002eb50 (once at each turn of the last loop),
 *     the results of the callees.
 *   Writes: the byte at 0x8002eafc (0x80), the local rectangle.
 *   Callees, all recorders in the test, never run: func_80011060_logo(),
 *     StUnSetRing(), CdReady(mode, result) (two arguments),
 *     CdGetSector(p, size) (two arguments), CdControlB(com, param, result)
 *     (three arguments; its results are scripted in turn for both loops:
 *     0 up to three times, then a value that is not 0, for each loop),
 *     DecDCToutCallback(f), ClearImage(rect, r, g, b) (four arguments; the
 *     pointer is a local's address, so it is masked to 0 and the two words
 *     behind it are logged), DrawSync(mode), VSync(mode),
 *     func_80010a38_logo() (its recorder stores a new value in the index
 *     word at its first call, as the flip does: the second turn then
 *     reads it). The word at 0x8002eafc and the index word are watched, so
 *     that their values at every call are compared.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_80010e28_logo(void) {
    Rect rect;
    int i;

    func_80011060_logo();
    StUnSetRing();
    CdReady(0, 0);
    CdGetSector(data_8002eb08_logo, 0x200);
    data_8002eafc_logo = 0x80;
    while (CdControlB(0x0e, &data_8002eafc_logo, 0) == 0) {
    }
    while (CdControlB(9, 0, 0) == 0) {
    }
    DecDCToutCallback(0);
    for (i = 0; i < 2; i++) {
        rect.x = 0;
        rect.y = data_8002eb50_logo != 0 ? 0xf0 : 0;
        rect.w = 0x1e0;
        rect.h = 0xf0;
        ClearImage(&rect, 0, 0, 0);
        DrawSync(0);
        VSync(0);
        func_80010a38_logo();
    }
}
