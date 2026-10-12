/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80152404(Object *object) {
    object->ot_depth = 4;
    object->field_0f = 1;
    object->state_0++;
    game_state.other_object = object->field_3c;
    object->field_1c = game_state.other_object->field_1c;
}
