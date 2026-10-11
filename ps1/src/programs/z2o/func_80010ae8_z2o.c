/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010ae8_z2o.py) compares the
 * behavior of this C with the original code of the opening movie's program
 * (PAC/Z2O.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets up the graphics and the
 * program's two display environments (20 bytes each from 0x80049eac; the
 * game's DispEnv). In order:
 *   1. ResetGraph(0), SetDispMask(0), CdReadCallback(0);
 *   2. SetDefDispEnv(env0, 0, 0, 320, 240) and SetDefDispEnv(env1, 0, 240,
 *      320, 240) (the fifth argument goes on the stack);
 *   3. sets isrgb24 of env1 and of env0 to 1, the screen width of env0 and of
 *      env1 to 0xfa, and the index word at 0x80031e58 (the index of the
 *      environment current) to 0;
 *   4. makes the rectangle (0, 0, 0x3c0, 0x1e0) in a local and calls
 *      ClearImage(&rect, 0, 0, 0);
 *   5. DrawSync(0), VSync(0), func_80158470(env1) (the game's name for the
 *      program's copy of the library routine that sets the display
 *      environment, as in func_80010be4_z2o), SetDispMask(1).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: nothing of the program's memory.
 *   Writes: isrgb24 (a byte) and screen_w (a halfword) of both environments,
 *     the index word.
 *   Callees, all recorders in the test, never run: ResetGraph(mode),
 *     SetDispMask(mask), CdReadCallback(func), SetDefDispEnv(env, x, y, w, h)
 *     (five arguments), ClearImage(rect, r, g, b) (four arguments; the
 *     pointer is the address of a local and is masked, the two words behind
 *     it logged), DrawSync(mode), VSync(mode), func_80158470(env) (one
 *     argument; the five words behind it are logged). The two environments
 *     (ten words) and the index word are watched, so that the log shows what
 *     the function has stored by each call.
 *   Slots no input can reach: none known.
 * The global pointer register: the function does not use it (no instruction
 * of it reads or writes the register or reaches data through it plus an
 * offset); the tool begins its runs with it at 0.
 */
#include "z2o.h"

void func_80010ae8_z2o(void) {
    Rect rect;
    DispEnv *env;

    env = (DispEnv *)data_80049eac_z2o;
    ResetGraph(0);
    SetDispMask(0);
    CdReadCallback(0);
    SetDefDispEnv(&env[0], 0, 0, 0x140, 0xf0);
    SetDefDispEnv(&env[1], 0, 0xf0, 0x140, 0xf0);
    env[1].isrgb24 = 1;
    env[0].isrgb24 = 1;
    env[0].screen_w = 0xfa;
    env[1].screen_w = 0xfa;
    rect.w = 0x3c0;
    data_80031e58_z2o = 0;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1e0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
    VSync(0);
    func_80158470(&env[1]);
    SetDispMask(1);
}
