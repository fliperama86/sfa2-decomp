/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_80010928_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets up the graphics and
 * the program's two display environments (20 bytes each from 0x8002eb8c; the
 * game's DispEnv). In order:
 *   1. ResetGraph(0), SetDispMask(0);
 *   2. SetVideoMode(w) with the video mode word w at 0x8002eb38 (the
 *      routine is Sony's SetVideoMode, inferred from its listing);
 *   3. SetDefDispEnv(env0, 0, 0, 320, 240) and SetDefDispEnv(env1, 0, 240,
 *      320, 240) (the fifth argument goes on the stack);
 *   4. sets isrgb24 of env1 and of env0 to 1, and the word at 0x8002eb50
 *      (the index of the environment shown, as in func_80010a38_logo) to 0;
 *   5. when the video mode word at 0x8002eb38 is not 0 (read again here): sets the
 *      screen y of env0 to 0x18, its screen x to 0, then the same of env1;
 *   6. makes the rectangle (0, 0, 0x3c0, 0x1e0) in a local and calls
 *      ClearImage(&rect, 0, 0, 0);
 *   7. DrawSync(0), VSync(0), func_80158470(env1) (the game's name for the
 *      program's copy of the library routine that sets the display
 *      environment, as in func_80010a38_logo), SetDispMask(1).
 *
 * Contract:
 *   Arguments: none. No result.
 *   Reads: the video mode word at 0x8002eb38 (twice).
 *   Writes: isrgb24 of both environments (a byte each), the index word, and
 *     when the video mode word is not 0 the two halfwords of each
 *     environment's screen x and y.
 *   Callees, all recorders in the test, never run: ResetGraph(mode),
 *     SetDispMask(mask), SetVideoMode(mode), SetDefDispEnv(env, x, y, w, h)
 *     (five arguments), ClearImage(rect, r, g, b) (four arguments; the
 *     pointer is a local: the recorder logs its value and the two words
 *     behind it, so the test also compares the local's address, which the
 *     two codes share in the test), DrawSync(mode), VSync(mode), func_80158470(env) (one
 *     argument; the five words behind it are logged). The two
 *     environments (ten words), the video mode word and the index word are
 *     watched, so that their values at every call are compared: the order
 *     of the stores against the calls shows.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_80010928_logo(void) {
    Rect rect;
    DispEnv *env;

    env = (DispEnv *)data_8002eb8c_logo;
    ResetGraph(0);
    SetDispMask(0);
    SetVideoMode(data_8002eb38_logo);
    SetDefDispEnv(&env[0], 0, 0, 0x140, 0xf0);
    SetDefDispEnv(&env[1], 0, 0xf0, 0x140, 0xf0);
    env[1].isrgb24 = 1;
    env[0].isrgb24 = 1;
    data_8002eb50_logo = 0;
    if (data_8002eb38_logo != 0) {
        env[0].screen_y = 0x18;
        env[0].screen_x = 0;
        env[1].screen_y = 0x18;
        env[1].screen_x = 0;
    }
    rect.w = 0x3c0;
    rect.x = 0;
    rect.y = 0;
    rect.h = 0x1e0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
    VSync(0);
    func_80158470(&env[1]);
    SetDispMask(1);
}
