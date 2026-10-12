/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_state_3_handler_from_table_a(Object *object) {
    handlers_1a6c[object->state_3](object);
}

void update_airborne_or_bounce_land(Object *object) {
    int value = object->vel_y;
    int a1;

    if ((u8)apply_velocity_and_check_ground(object)) {
        if (value < 0) {
            update_sequence(object);
        }
    } else {
        a1 = 1;
        value = -0x60000;
        object->field_60 = 2;
        object->state_3 = 7;
        object->countdown = 4;
        object->active_state = 0;
        object->flag_166_set = 1;
        object->pos_y = (u16)object->ground_y;
        if (object->vel_x >= 0) {
            a1 = 0;
            value = 0x60000;
        }
        object->facing_side = a1;
        object->vel_x = value;
        set_sequence_by_side_and_mode_2(object, 0x1f);
    }
}

void call_state_3_handler_from_table_b(Object *object) {
    handlers_1a74[object->state_3](object);
}

void advance_state_3_and_call_step_2(Object *object) {
    object->state_3 = object->state_3 + 1;
    update_pos_x_table_step_then_pick_sequence(object);
}
