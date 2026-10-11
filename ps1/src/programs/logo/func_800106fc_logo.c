/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800106fc_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the program's main, called
 * from the program's entry code. In order:
 *   1. func_80011838_logo() (the empty function), ResetCallback(),
 *      func_80011794_logo() (the disc start), PadInit(0);
 *   2. looks up two entries of the table of the disc's files with
 *      func_80011758_logo(0x23, &size) (the result is the stream's sector,
 *      str) and func_80011758_logo(0x22, &size) (bin, the stills' file's
 *      sector); size is a local that the callee fills and this function
 *      does not read;
 *   3. calls func_800107e8_logo(0x8002ede0, str, bin, 0, 1) (the fifth
 *      argument on the stack);
 *   4. a loop, each turn of which: r = func_80010c20_logo() (the frame);
 *      p = PadRead(1); old = the word at 0x8002eb80; stores p in the word
 *      at 0x8002eb84 and then in the word at 0x8002eb80; n = (p ^ old) & p
 *      (the buttons newly pressed) and stores it in the word at 0x8002eb88;
 *      adds 1 to r when n & 0x820 is not 0 and 1 more when n & 0x08200000 is
 *      not 0; the loop ends when r is not 0;
 *   5. calls func_80010e28_logo() (the end), PadStop() and StopCallback().
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the word at 0x8002eb80 (any word in the test), the results of the
 *     callees.
 *   Writes: the words at 0x8002eb80, 0x8002eb84 and 0x8002eb88, the local
 *     size (through the callee).
 *   Callees, all recorders in the test, never run: func_80011838_logo(),
 *     ResetCallback(), func_80011794_logo(), PadInit(mode) (one argument),
 *     func_80011758_logo(i, p) (two arguments; p is the address of a local,
 *     masked to 0, and the function does not read what the callee would
 *     write there), func_800107e8_logo (five arguments, the fifth read from
 *     the caller's stack), func_80010c20_logo() (its results are scripted
 *     per call: 0 until the last turn of the loop), PadRead(id) (one
 *     argument, its results scripted per call), func_80010e28_logo(),
 *     PadStop(), StopCallback(). The three pad words are watched, so that
 *     their values at every call are compared.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_800106fc_logo(void) {
    u32 size;
    u32 str;
    u32 bin;
    u32 pad;
    u32 old;
    u32 pressed;
    int r;

    func_80011838_logo();
    ResetCallback();
    func_80011794_logo();
    PadInit(0);
    str = func_80011758_logo(0x23, &size);
    bin = func_80011758_logo(0x22, &size);
    func_800107e8_logo(data_8002ede0_logo, str, bin, 0, 1);
    do {
        r = func_80010c20_logo();
        pad = PadRead(1);
        old = data_8002eb80_logo;
        data_8002eb84_logo = pad;
        data_8002eb80_logo = pad;
        pressed = (pad ^ old) & pad;
        data_8002eb88_logo = pressed;
        if ((pressed & 0x820) != 0) {
            r++;
        }
        if ((pressed & 0x08200000) != 0) {
            r++;
        }
    } while (r == 0);
    func_80010e28_logo();
    PadStop();
    StopCallback();
}
