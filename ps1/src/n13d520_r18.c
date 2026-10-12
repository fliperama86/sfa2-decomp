/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144e18(Object *object) {
    if (game_state.config->field_64 == 0 && game_state.config->mode == 5) {
        object->state_1 = 2;
        if (game_state.config->mode_flag_b == 0) {
            object->state_1 = 3;
            if (game_state.config->table_index == 0) object->state_1 = 1;
        }
    }
}
