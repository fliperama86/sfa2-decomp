/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150f7c(GameState *state) {
    int n = state->step_delay_timer - 1;
    state->step_delay_timer = n;
    if ((s16)n == 0) {
        int m;
        state->step_delay_timer = 1;
        func_80125dc0(4, 0x20, state->step_counter, 0);
        func_80137220(4, 7);
        m = state->step_counter - 1;
        state->step_counter = m;
        if ((s16)m < 0) {
            ((Hud *)data_8018f5a0)->stage++;
        }
    }
}
