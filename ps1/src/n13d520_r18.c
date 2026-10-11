/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144e18(Object *object) {
    if (game_state.config->field_64 == 0 && game_state.config->field_6c == 5) {
        object->state_1 = 2;
        if (game_state.config->field_4d == 0) {
            object->state_1 = 3;
            if (game_state.config->field_a6 == 0) object->state_1 = 1;
        }
    }
}
