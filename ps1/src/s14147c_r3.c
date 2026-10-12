/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int is_state_01010101_and_ready(Object *object) {
    if (*(u32 *)&object->state_0 == 0x1010101 && object->active_state != 0 && object->field_163 == 0 &&
        *(s16 *)&object->health >= 0 &&
        (game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) == 0 &&
        is_other_object_state_0_and_1_six_one(object) == 0) {
        return 1;
    }
    return 0;
}
