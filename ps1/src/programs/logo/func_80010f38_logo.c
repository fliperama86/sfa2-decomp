/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010f38_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the output routine of the
 * movie decoder, run when the decoder has a strip ready (the name of the
 * hook is inferred from its calls: DecDCTout asks the decoder for the next
 * strip). In order:
 *   1. if the word at 0x8002edb8 is not 0, calls StCdInterrupt() and
 *      clears the word;
 *   2. copies the strip in the buffer (the pointer at 0x8002eb00) to the
 *      picture, with the game's name for the program's LoadImage,
 *      func_80157fc4(&rect, buffer), the rectangle at 0x8002eb24;
 *   3. adds 0x18 to the rectangle's x (24 words of video memory; a
 *      halfword store; x is a signed halfword);
 *   4. if the new x is below 480 (0x1e0): asks for the next strip with
 *      DecDCTout(buffer, 0xb40) and sets the word at 0x8002eb3c to 1;
 *      otherwise sets that word to 0 (the waiting function
 *      func_80010f20_logo waits for it to become 0; inferred).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the words at 0x8002edb8 and 0x8002eb00, the rectangle at
 *     0x8002eb24 (x is read after the call of the library routine).
 *   Writes: 0x8002edb8 (to 0 only when it was not 0), the halfword x of the
 *     rectangle (x + 0x18), the word at 0x8002eb3c (1 or 0).
 *   Callees, all recorders in the test, never run: StCdInterrupt() (no
 *     argument), func_80157fc4(rect, buffer) (two arguments; the recorder
 *     logs the pointer and the two words behind it, and the buffer
 *     pointer), DecDCTout(buffer, size) (two arguments). The words at
 *     0x8002edb8, 0x8002eb24 (two words) and 0x8002eb3c are watched, so
 *     that the log shows their values at every call and an order of
 *     the stores that differs from the original's would show.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_80010f38_logo(void) {
    if (data_8002edb8_logo != 0) {
        StCdInterrupt();
        data_8002edb8_logo = 0;
    }
    func_80157fc4(&data_8002eb24_logo, (u8 *)data_8002eb00_logo);
    data_8002eb24_logo.x += 0x18;
    if (data_8002eb24_logo.x < 0x1e0) {
        DecDCTout(data_8002eb00_logo, 0xb40);
        data_8002eb3c_logo = 1;
    } else {
        data_8002eb3c_logo = 0;
    }
}
