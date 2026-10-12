/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144e90(Object *object) {
    object->state_0++;
    game_state.config->mode = 0;
}
