/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80152ee8(void) {
    GameState *state = &game_state;

    func_80136dc4();
    game_state.loop_mode = 1;
    func_80152f80(state);
    *(u32 *)0x1f800014 = *(u32 *)0x1f800014 + 1;
    if (*(u32 *)0x1f800014 == 6) {
        *(u32 *)0x1f800014 = 0;
    }
    game_state.loop_mode = 0;
    if (game_state.wait_flag != 0) {
        do {
            func_801192bc(1);
        } while (state->wait_flag != 0);
    }
    func_80119198(3);
}
