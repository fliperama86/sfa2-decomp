/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residuals: func_8014edac (original keeps game_state base in a saved
 * register from before the third close_vab_and_sep call, and tests/clears through
 * it; tried a local g set at top, set mid-function, ternary and if/else for
 * the argument). func_8014f038 (original derives both player addresses from
 * the field_cd address in one register; tried if/else, ternary, p++,
 * default-then-override). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014ee5c(int a) {
    GameState *g = &game_state;
    g->loop_mode = 1;
    func_80119144(2, 7);
    func_801192bc(1);
    func_8014f3b8(5, a);
    func_8014f3b8(5, game_state.kind_index + 0x15);
    g->loop_mode = 0;
    while (g->wait_flag) {
        func_801192bc(1);
    }
}

void func_8014eef8(int a, int b) {
    GameState *g = &game_state;
    g->loop_mode = 1;
    func_80119144(2, 7);
    func_801192bc(1);
    b ^= 1;
    close_vab_and_sep(b);
    func_8014f3b8(6, b * 21 + a);
    g->loop_mode = 0;
    while (g->wait_flag) {
        func_801192bc(1);
    }
}

void func_8014efa8(int a) {
    GameState *g = &game_state;
    g->loop_mode = 1;
    func_80119144(2, 7);
    func_801192bc(1);
    close_vab_and_sep(3);
    func_8014f3b8(7, a);
    g->loop_mode = 0;
    while (g->wait_flag) {
        func_801192bc(1);
    }
}
