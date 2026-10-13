/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void advance_substep_when_wait_flag_clear_13_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        screen_state->substep_index += 1;
    }
}
