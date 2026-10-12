/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_airborne_or_land(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object) != 0) {
        update_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
        object->active_state = 0;
        object->pos_y = object->ground_y;
        *(u32 *)&object->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(object);
    }
}

void move_by_vel_x_then_return_to_state_1_1_2_0(Object *object) {
    if ((s16)object->step_flags >= 0 || (object->countdown = (s16)object->countdown - 1, (s16)object->countdown != 0)) {
        *(s32 *)&object->pos_x_frac += object->vel_x;
        update_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
        object->flag_166_set = 0;
        if ((s16)object->health < 0) {
            enter_state_1_6_and_set_sequence(object);
        } else {
            object->field_157 = 1;
            object->facing_side = object->saved_facing;
        }
    }
}

void count_down_then_set_state_3_to_5(Object *object) {
    object->countdown = (s16)object->countdown - 1;
    if ((s16)object->countdown == 0) {
        object->state_3 = 5;
        object->flag_166_set = 0;
        if ((s16)object->health < 0) {
            enter_state_1_6_and_set_sequence(object);
        } else {
            set_sequence_by_side_and_mode_2(object, 0x19);
        }
    }
}

void enter_state_1_8_if_check_passes(Object *object) {
    if (object->hit_flag == 0) {
        if ((u8)call_slot_stage_handler_a(object, 0xf, 0xd) == 0) {
            enter_state_1_8_if_kind_6_and_ready(object);
        } else if (is_state_01010101_and_ready(object) != 0) {
            object->state_0 = 1;
            object->state_1 = 8;
            object->state_2 = 0;
            object->state_3 = 0;
            object->state_change_flag = 0;
            set_direction_index_from_buttons_group_a(object);
        }
    }
}

void enter_state_1_8_if_kind_6_and_ready(Object *object) {
    if (object->kind == 6 && (u8)call_slot_stage_handler_a(object, 0xe, 8) != 0 && is_state_01010101_and_ready(object) != 0) {
        object->state_0 = 1;
        object->state_1 = 8;
        object->state_2 = 0;
        object->state_3 = 0;
        object->state_change_flag = 1;
        set_direction_index_from_buttons_group_a(object);
    }
}

void call_state_3_table_function_5(Object *object) {
    table_80171a60[object->state_3](object);
}
