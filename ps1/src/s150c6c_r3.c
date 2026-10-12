/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150e4c(GameState *state) {
    ((Hud *)data_8018f5a0)->stage++;
    state->step_delay_timer = 1;
    state->step_counter = 0;
    state->wait_flag = 1;
    func_80150e94(state);
}

void func_80150e94(GameState *state) {
    int n = state->step_delay_timer - 1;
    state->step_delay_timer = n;
    if ((s16)n == 0) {
        state->step_delay_timer = 1;
        func_80125dc0(4, 0x20, state->step_counter, 0);
        func_80137220(4, 7);
        state->step_counter++;
        if ((s16)state->step_counter >= 0x20) {
            ((Hud *)data_8018f5a0)->stage++;
            state->wait_flag = 0x80;
        }
    }
}
