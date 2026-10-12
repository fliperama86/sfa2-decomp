/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_state_2_handler_then_bound_x(Object *object) {
    object->field_16a = 0;
    table_801719f4[object->state_2](object);
    clamp_pos_x_to_bounds_and_push_other(object);
}

void call_state_3_table_function(Object *object) {
    table_80171a00[object->state_3](object);
}
