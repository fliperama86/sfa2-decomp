/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80011794_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): starts the disc. In order:
 *   1. makes a mode byte, 0x80, in a local (inferred to be the speed bit of
 *      the library's CdlSetmode);
 *   2. calls CdInit() up to four times, until it returns a value that is
 *      not 0 (then the function's result is 1); after four returns of 0 the
 *      result is 0 and nothing else is done;
 *   3. with the init done: sends command 0x0e (CdlSetmode inferred) with
 *      the pointer to the mode byte through CdControlB(0x0e, &mode, 0) until
 *      it returns a value that is not 0;
 *   4. makes a disc position with CdIntToPos(0x10, pos) (a four byte
 *      local, filled by the library) and sends command 0x15 (CdlSeekL
 *      inferred) with it through CdControlB(0x15, pos, 0) until it returns
 *      a value that is not 0;
 *   5. returns 1.
 *
 * Contract:
 *   Arguments: none. Result in v0: 1 when an init succeeded, else 0.
 *   Reads: none of the program's data; the results of the callees.
 *   Writes: none to the program's data; the two locals only.
 *   Callees, all recorders in the test, never run: CdInit() (no argument;
 *     the setup scripts its results: 0 up to three times then a value that is
 *     not 0, or four times 0), CdControlB(com, param, result) (three
 *     arguments; the param pointer is a local's address, so its value is not
 *     logged and the word behind it is: the mode byte with the three bytes
 *     after it in the frame, then the position; the recorder's results are
 *     scripted in turn for both loops), CdIntToPos(i, p) (two arguments; the
 *     recorder stands for the library by a fill that stores a word of the
 *     setup's at p, which is the model of what the library does to the
 *     position: written by the contract's author, and the test cannot show
 *     that the library does it).
 *   Slots no input can reach: none known.
 */
#include "logo.h"

int func_80011794_logo(void) {
    u8 mode;
    u8 pos[4];
    int tries;
    int ok;

    ok = 0;
    tries = 4;
    mode = 0x80;
    do {
        if (CdInit() != 0) {
            ok = 1;
            break;
        }
        tries--;
    } while (tries != 0);
    if (ok == 0) {
        return ok;
    }
    while (CdControlB(0x0e, &mode, 0) == 0) {
    }
    CdIntToPos(0x10, pos);
    while (CdControlB(0x15, pos, 0) == 0) {
    }
    return ok;
}
