/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80121ef0(Object *unused) {
    if (game_state.busy_flag == 0) {
        game_state.countdown_timer_2 = (s16)game_state.countdown_timer_2 - 1;
        if ((s16)game_state.countdown_timer_2 < 0) {
            data_8018f5a0->step_index_2 = 0xd;
            func_80124ecc();
        }
    }
}

void func_80121f48(Object *unused) {
    if (game_state.busy_flag == 0) {
        data_8018f5a0->step_index_2 = 0xb;
    }
}
