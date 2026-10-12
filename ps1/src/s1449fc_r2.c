/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void move_x_until_countdown_then_advance_state_0(Object *object) {
    int v;
    update_position_x_with_accel(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        advance_state_0_2(object);
    }
}

void call_handler_by_state_2_b5d4_and_advance_3(Object *object) {
    table_8017b668[object->state_2](object);
    advance_sequence_checked(object);
    set_flag_and_push_if_mode_8(object);
}

void init_move_right_from_left_sequence_b(Object *object) {
    object->pos_x = -0x40;
    object->pos_y = 0x70;
    object->countdown = 0x1f;
    object->vel_x = 0x20000;
    object->accel_x = 0x8000;
    object->state_2++;
    set_sequence_from_effect_table(object, 0xb);
}

void move_x_until_countdown_then_set_negative_accel(Object *object) {
    int v;
    update_position_x_with_accel(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        object->countdown = 0xf;
        object->accel_x = 0xfffc0000;
        object->state_2++;
    }
}

void accelerate_until_countdown_set_pos_x_b_2(Object *object) {
    int v;
    update_position_x_with_accel(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        object->pos_x = 0xc0;
        object->state_2++;
    }
}
