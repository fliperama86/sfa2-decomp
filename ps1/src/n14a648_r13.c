/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_state_index_if_not_looping(GameState *state) {
    if (state->loop_mode == 0) {
        data_8018f5a0->state_index++;
        state->step_counter = 0x1f;
    }
}
