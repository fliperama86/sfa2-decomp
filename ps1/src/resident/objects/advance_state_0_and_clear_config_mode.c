/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_state_0_and_clear_config_mode(Object *object) {
    object->state_0++;
    game_state.config->mode = 0;
}
