/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void move_by_velocity_until_countdown_state_2(Object *object) {
    update_position_by_velocity(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) object->state_2 = object->state_2 + 1;
}

void on_sequence_end_choose_next_action(Object *object) {
    if ((s16)object->step_flags < 0) {
        if ((u8)is_config_object_health_full(object) == 0) advance_state_0_and_clear_config_mode(object);
        else init_move_left_from_right_sequence_c(object);
    }
}

void init_move_left_from_right_sequence_c(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->countdown = 0xf;
    object->vel_x = 0xfff40000;
    object->accel_x = -0x8000;
    object->state_2 = object->state_2 + 1;
    func_80144ee4(object, 0xc);
}
