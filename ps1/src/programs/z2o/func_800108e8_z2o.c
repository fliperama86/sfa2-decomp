/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800108e8_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the program's main, called
 * from the program's entry code. In order:
 *   1. func_80011afc_z2o() (the library's empty function), ResetCallback(),
 *      ResetGraph(0), func_80011a24_z2o() (the disc start), PadInit(0),
 *      InitGeom(), func_80010ae8_z2o() (the graphics set-up); stores 0 in the
 *      word at 0x80031e6c;
 *   2. func_800119b8_z2o(0), func_800115a8_z2o(0x80), func_80011630_z2o(1),
 *      func_800116c8_z2o(1);
 *   3. func_80011018_z2o(0xff, 0, 0x334, 0x80049ed4, 0x1e, 0x80031e84,
 *      0x8003de8c, 0x800876f8, 1, 1) (open the movie; the fifth to tenth
 *      arguments go on the stack);
 *   4. a loop, each turn of which: r = func_800110d0_z2o(0, y) with y 0xf0
 *      when the index word at 0x80031e58 is not 0, else 0; then
 *      func_80011428_z2o() (wait for the strips), VSync(0),
 *      func_80010be4_z2o() (flip); p = PadRead(1); old = the word at
 *      0x80031e6c; stores p in the word at 0x80031e70; n = (p ^ old) & p
 *      (the buttons newly pressed); stores n & 0xffff at 0x8003de88, n at
 *      0x80031e74 and n >> 16 (logical) at 0x80049e8c. When n & 0x820 is not
 *      0 or (n >> 16) & 0x820 is not 0 the function stores 1 in the word
 *      0x1f80000c (the scratchpad) and leaves the loop; otherwise it goes on
 *      while r is 0 and leaves the loop when r is not 0 (without the
 *      store). The word at 0x80031e6c is not written again, so old is the 0
 *      stored in step 1 unless a callee changes it;
 *   5. func_8001139c_z2o() (the end), then twice: sets the rectangle at
 *      0x80049ea4 to (0, y, 0x1e0, 0xf0) with y as in the loop (read from the
 *      index word each time), ClearImage(&rect, 0, 0, 0), DrawSync(0),
 *      VSync(0), func_80010be4_z2o(); then PadStop() and StopCallback().
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the word at 0x80031e58, the word at 0x80031e6c, the callees'
 *     results.
 *   Writes: the words at 0x80031e6c, 0x80031e70, 0x80031e74, 0x8003de88 and
 *     0x80049e8c, the rectangle at 0x80049ea4, the scratchpad word at
 *     0x1f80000c (only when a button pressed ends the loop).
 *   Callees, all recorders in the test, never run: func_80011afc_z2o(),
 *     ResetCallback(), ResetGraph(mode) (one argument), func_80011a24_z2o(),
 *     PadInit(mode) (one argument), InitGeom(), func_80010ae8_z2o(),
 *     func_800119b8_z2o(v), func_800115a8_z2o(v), func_80011630_z2o(v),
 *     func_800116c8_z2o(v) (one argument each), func_80011018_z2o (ten
 *     arguments, the last six read from the caller's stack),
 *     func_800110d0_z2o(a, y) (two arguments; its results are scripted per
 *     call), func_80011428_z2o(), VSync(mode) (one argument),
 *     func_80010be4_z2o() (the flip: a model written by the contract's
 *     author stores the new value of the index word at 0x80031e58 at each of
 *     its calls, a flip of 0 and not 0), PadRead(id) (one argument, its
 *     results scripted per call), func_8001139c_z2o(), ClearImage(rect, r, g,
 *     b) (four arguments; the rectangle's two words behind the pointer are
 *     logged), DrawSync(mode), PadStop(), StopCallback(). The index word,
 *     the three words from 0x80031e6c, the words at 0x8003de88 and
 *     0x80049e8c, the rectangle and the scratchpad word are watched, so that
 *     the log shows their values at every call.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_800108e8_z2o(void) {
    int r;
    u32 pad;
    u32 old;
    u32 pressed;
    int i;

    func_80011afc_z2o();
    ResetCallback();
    ResetGraph(0);
    func_80011a24_z2o();
    PadInit(0);
    InitGeom();
    func_80010ae8_z2o();
    data_80031e6c_z2o = 0;
    func_800119b8_z2o(0);
    func_800115a8_z2o(0x80);
    func_80011630_z2o(1);
    func_800116c8_z2o(1);
    func_80011018_z2o(0xff, 0, 0x334, data_80049ed4_z2o, 0x1e, data_80031e84_z2o, data_8003de8c_z2o, data_800876f8_z2o, 1,
                      1);
    do {
        r = func_800110d0_z2o(0, data_80031e58_z2o != 0 ? 0xf0 : 0);
        func_80011428_z2o();
        VSync(0);
        func_80010be4_z2o();
        pad = PadRead(1);
        old = data_80031e6c_z2o;
        data_80031e70_z2o = pad;
        pressed = (pad ^ old) & pad;
        data_8003de88_z2o = pressed & 0xffff;
        data_80031e74_z2o = pressed;
        data_80049e8c_z2o = pressed >> 16;
        if ((pressed & 0x820) != 0 || ((pressed >> 16) & 0x820) != 0) {
            *(int *)0x1f80000c = 1;
            break;
        }
    } while (r == 0);
    func_8001139c_z2o();
    i = 0;
    do {
        i++;
        data_80049ea4_z2o.w = 0x1e0;
        data_80049ea4_z2o.x = 0;
        data_80049ea4_z2o.h = 0xf0;
        data_80049ea4_z2o.y = data_80031e58_z2o != 0 ? 0xf0 : 0;
        ClearImage(&data_80049ea4_z2o, 0, 0, 0);
        DrawSync(0);
        VSync(0);
        func_80010be4_z2o();
    } while (i < 2);
    PadStop();
    StopCallback();
}
