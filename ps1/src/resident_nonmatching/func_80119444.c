/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original reloads the new counter value into v0 and passes nothing in a0
 * (the callee takes no argument); the code built from this C differs in
 * that register use. The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the resident image; the build does not use this file. The
 * differential test next to it (difftest.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): the handler that an
 * interrupt event calls once a frame. It calls func_80119718, adds one to a
 * frame counter (16 bits, wrapping) and calls func_80150638.
 *
 * Contract:
 *   No argument, no result.
 *   Reads and writes: the 16-bit counter data_801ac310 (any value, it
 *     wraps at 0x10000).
 *   Watched at every call (copied into the log): the word at data_801ac310.
 *   Callees replaced by recorders, the same in both runs: func_80119718
 *     and func_80150638, each taking no argument and returning 0. They are
 *     called in this order, the counter is incremented between them. What
 *     they do themselves (they reach Sony's library and the hardware) is
 *     outside the test.
 *   Every instruction slot of the original is executed by every case.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; not original ones. */
extern u16 data_801ac310;
void func_80119718(void);
void func_80150638(void);

void func_80119444(void) {
    func_80119718();
    data_801ac310++;
    func_80150638();
}
