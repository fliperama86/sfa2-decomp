/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153fb8(void) {
    Object *p = &player_left;
    func_8015406c(p);
    if (game_state.mode & 1) {
        func_801541e0(p);
        func_801540ec(p);
        func_801542e4(p);
    }
    p++;
    func_8015406c(p);
    if (game_state.mode & 2) {
        func_801541e0(p);
        func_801540ec(p);
        func_801542e4(p);
    }
}
