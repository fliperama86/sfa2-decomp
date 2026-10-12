/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void clear_flag_call_state_3_handler_table_a(Object *object) {
    object->field_16a = 0;
    handler_table_19a8[object->state_3](object);
}

void start_motion_sequence_with_table_speed(Object *object) {
    int index;
    s32 speed;
    object->field_157 = 0;
    object->state_3 = object->state_3 + 1;
    object->facing_side = object->saved_facing;
    index = 2;
    if (object->variant_index != 0) {
        index = 3;
    }
    set_sequence_by_side(object, index);
    speed = motion_speed_table[object->kind * 2 + (object->variant_index & 1)];
    if (object->facing_side != 0) {
        speed = -speed;
    }
    object->vel_x = speed;
    choose_idle_state_transition_or_update(object);
}
