/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void move_x_until_countdown_then_clear_config_mode(Object *object) {
    update_position_x_with_accel(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        advance_state_0_and_clear_config_mode(object);
    }
}

void call_handler_by_state_2_b61c_and_advance(Object *object) {
    handlers_b61c[object->state_2](object);
    advance_sequence_checked(object);
    set_flag_and_push_if_mode_8(object);
}

void init_move_left_from_right_sequence_a(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->countdown = 0x1f;
    object->vel_x = -0x20000;
    object->accel_x = -0x8000;
    object->state_2 = object->state_2 + 1;
    set_sequence_from_effect_table(object, 0xa);
}

void accelerate_until_countdown_set_accel_x(Object *object) {
    update_position_x_with_accel(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        object->countdown = 0xf;
        object->accel_x = 0x40000;
        object->state_2 = object->state_2 + 1;
    }
}

void accelerate_until_countdown_set_pos_x(Object *object) {
    update_position_x_with_accel(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        object->pos_x = 0xc0;
        object->state_2 = object->state_2 + 1;
    }
}
