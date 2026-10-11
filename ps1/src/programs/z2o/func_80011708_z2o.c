/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011708_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the routine that runs when
 * the movie decoder has a strip ready (the role is inferred from its calls:
 * DecDCTout asks the decoder for the next strip). In order:
 *   1. adds 1 to the word at 0x8002f888 (a count of strips; inferred);
 *   2. if the word at 0x80031e68 is not 0, calls StCdInterrupt() and clears
 *      the word;
 *   3. if the word at 0x8002f870 is 0, copies the strip to the picture with
 *      the game's name for the program's LoadImage, func_80157fc4(&rect,
 *      buffer), the rectangle at 0x8002f874 and the buffer pointer at
 *      0x8002f87c;
 *   4. if the count (after the add) is below the word at 0x8002f88c (an
 *      unsigned comparison): adds 0x18 to the rectangle's x when the word at
 *      0x8002f818 is not 0, else 0x10 (a halfword store; x is a signed
 *      halfword), adds the word at 0x8002f880 (in words) to the buffer
 *      pointer, asks for the next strip with DecDCTout(buffer,
 *      word at 0x8002f884) and sets the word at 0x8002f890 to 1; otherwise
 *      sets that word to 0 (func_80011428_z2o waits for it to become 0;
 *      inferred).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the words at 0x8002f888, 0x80031e68, 0x8002f870, 0x8002f88c,
 *     0x8002f818, 0x8002f880, 0x8002f884, the pointer at 0x8002f87c and the
 *     halfword x of the rectangle at 0x8002f874.
 *   Writes: 0x8002f888, 0x80031e68 (to 0 only when it was not 0), the
 *     halfword x of the rectangle, 0x8002f87c, 0x8002f890 (1 or 0).
 *   Callees, all recorders in the test, never run: StCdInterrupt() (no
 *     argument), func_80157fc4(rect, buffer) (two arguments; the recorder
 *     logs the pointer and the two words behind it, and the buffer
 *     pointer), DecDCTout(buffer, size) (two arguments). The words from
 *     0x8002f874 up to and including 0x8002f890 (eight words) and the word
 *     at 0x80031e68 are watched, so that the log shows their values at every
 *     call and an order of the stores that differs from the original's
 *     would show.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80011708_z2o(void) {
    data_8002f888_z2o++;
    if (data_80031e68_z2o != 0) {
        StCdInterrupt();
        data_80031e68_z2o = 0;
    }
    if (data_8002f870_z2o == 0) {
        func_80157fc4(&data_8002f874_z2o, (u8 *)data_8002f87c_z2o);
    }
    if (data_8002f888_z2o < data_8002f88c_z2o) {
        if (data_8002f818_z2o != 0) {
            data_8002f874_z2o.x += 0x18;
        } else {
            data_8002f874_z2o.x += 0x10;
        }
        data_8002f87c_z2o += data_8002f880_z2o;
        DecDCTout(data_8002f87c_z2o, data_8002f884_z2o);
        data_8002f890_z2o = 1;
    } else {
        data_8002f890_z2o = 0;
    }
}
