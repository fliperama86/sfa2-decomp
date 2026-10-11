/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801440e4(Object *object) {
    object->step_timer = 0xfff;
    if (game_state.config->field_76 != 0) {
        object->step_timer = 0x20;
        object->state_2 = object->state_2 + 1;
    }
}
