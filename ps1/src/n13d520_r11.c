/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void hold_step_timer_until_config_countdown(Object *object) {
    object->step_timer = 0xfff;
    if (game_state.config->countdown_2 != 0) {
        object->step_timer = 0x20;
        object->state_2 = object->state_2 + 1;
    }
}
