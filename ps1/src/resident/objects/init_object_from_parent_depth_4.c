/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_object_from_parent_depth_4(Object *object) {
    object->ot_depth = 4;
    object->field_0f = 1;
    object->state_0++;
    game_state.other_object = object->parent;
    object->field_1c = game_state.other_object->field_1c;
}
