/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_state_0_2(Object *object) {
    object->state_0++;
}

void call_handler_by_state_2_b5d4_and_advance_2(Object *object) {
    table_8017b654[object->state_2](object);
    advance_sequence_checked(object);
    set_flag_and_push_if_mode_8(object);
}

void init_move_left_from_right_sequence_9(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->countdown = 0xf;
    object->vel_x = 0xfff40000;
    object->accel_x = -0x8000;
    object->state_2++;
    set_sequence_from_effect_table(object, 9);
}

void accelerate_until_countdown_set_pos_x_b(Object *object) {
    int v;
    update_position_x_with_accel(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        object->pos_x = 0xc0;
        object->state_2++;
    }
}

void choose_next_state_when_sequence_ends(Object *object) {
    if ((s16)object->step_flags < 0) {
        if ((u8)is_config_object_health_full(object) != 0 || game_state.config->table_index == 0) {
            advance_state_0_2(object);
        } else {
            object->state_2 = 3;
            set_sequence_from_effect_table(object, 0xe);
        }
    }
}
