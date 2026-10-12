/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_object_and_push_on_free_stack(Object *o);

void increment_state_0_slot04_12(Object *obj);
void call_handler_by_input_dir_second_2_slot04_12(Object *obj);
void call_by_state_3_zero_check_range_or_update_slot04_12(Object *obj);
void check_range_hit_or_continue_sequence_slot04_12(Object *obj);
void update_sequence_or_finish_on_flag_slot04_12(Object *obj);
void call_handler_by_state_3_slot04_12(Object *obj);
void advance_state_3_play_sound_on_direction_4_slot04_12(Object *obj);
void update_sequence_or_finish_on_flag_2_slot04_12(Object *obj);
void call_state_3_dispatch_by_input_dir_second_a_slot04_12(Object *obj);
void update_airborne_motion_or_land_slot04_12(Object *obj);
void enable_active_flag_3_and_select_sequence_slot04_12(Object *obj);

void increment_state_0_slot04_12(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_and_clear_parent_link_slot04_12(Object *obj) {
    Object *p = obj->parent;

    if (obj->table_index == 0) {
        *(s32 *)&p->linked_object_2 = 0;
    } else {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(obj);
}

void dispatch_by_input_dir_0_2_slot04_12(Object *obj) {
    if (obj->input_dir_0 == 4) {
        update_airborne_motion_or_land_slot04_12(obj);
    } else if (obj->input_dir_0 != 0) {
        call_state_3_dispatch_by_input_dir_second_a_slot04_12(obj);
    } else {
        call_handler_by_input_dir_second_2_slot04_12(obj);
    }
}

void call_handler_by_input_dir_second_2_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_state_3_slot04_12(obj);
    } else {
        call_by_state_3_zero_check_range_or_update_slot04_12(obj);
    }
}

void call_by_state_3_zero_check_range_or_update_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        check_range_hit_or_continue_sequence_slot04_12(obj);
    } else {
        update_sequence_or_finish_on_flag_slot04_12(obj);
    }
}

void check_range_hit_or_continue_sequence_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        enable_active_flag_3_and_select_sequence_slot04_12(obj);
    }
}

void update_sequence_or_finish_on_flag_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        advance_state_3_play_sound_on_direction_4_slot04_12(obj);
    } else {
        update_sequence_or_finish_on_flag_2_slot04_12(obj);
    }
}
