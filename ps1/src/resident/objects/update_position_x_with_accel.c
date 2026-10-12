/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"




void update_position_x_with_accel(Object *object) {
    *(s32 *)&object->pos_x_frac += object->vel_x;
    object->vel_x = object->vel_x + object->accel_x;
}

void update_position_by_velocity(Object *object) {
    *(s32 *)&object->pos_x_frac += object->vel_x;
    *(s32 *)&object->pos_y_frac -= object->vel_y;
}

void call_handler_by_state_2_b5f4_and_advance(Object *object) {
    handlers_b5f4[object->state_2](object);
    advance_sequence_checked(object);
    set_flag_and_push_if_mode_8(object);
}

void init_slide_in_from_left_sequence_8(Object *object) {
    object->pos_x = -0x40;
    object->pos_y = 0x70;
    object->countdown = 0xf;
    object->vel_x = 0xc0000;
    object->accel_x = 0x8000;
    object->state_2 = object->state_2 + 1;
    set_sequence_from_table_5(object, 8, table_8017b50c);
}
