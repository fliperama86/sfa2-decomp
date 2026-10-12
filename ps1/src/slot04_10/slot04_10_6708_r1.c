/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c6e70_slot04_10[];

void call_handler_by_step_limit_slot04_10(Object *obj);
void set_states_step_limit_zero_add_meter_sequence_slot04_10(Object *obj);
void set_states_step_limit_one_add_meter_sequence_slot04_10(Object *obj);
void set_states_step_limit_two_add_meter_sequence_slot04_10(Object *obj);
void clear_object_and_push_on_free_stack(Object *o);
void set_sequence_and_tables_from_direction(Object *obj);
void reset_object_to_state_1_0_0_2_sequence_1(Object *obj);
void increment_state_0_2_slot04_10(Object *obj);
void clear_variant_index_slot04_10(Object *obj);
void set_variant_index_and_sequence_slot04_10(Object *obj, s16 idx);
void call_handler_by_input_dir_second_2_slot04_10(Object *obj);
void call_handler_by_state_3_0_1_slot04_10(Object *obj);
void restore_facing_force_other_wide_or_set_sequence_slot04_10(Object *obj);
void set_states_1_2_0_0_2_slot04_10(Object *obj);
void update_sequence_nudge_x_or_reset_slot04_10(Object *obj);
void call_handler_by_state_3_0_1_2_slot04_10(Object *obj);
void restore_facing_force_other_near_or_set_sequence_slot04_10(Object *obj);
void update_sequence_reset_state_3_on_check_slot04_10(Object *obj);
void set_flag_and_call_function_by_state_3_slot04_10(Object *obj);
void update_state_restore_facing_no_second_dir_slot04_10(Object *obj);
void update_sequence_reset_state_3_on_check_2_slot04_10(Object *obj);

void follow_other_frame_param_sequence_slot04_10(Object *obj) {
    obj->field_01 = 0;
    if (ref_other.p->kind != obj->table_index) {
        increment_state_0_2_slot04_10(obj);
    } else if (ref_other.p->frame->frame_param == 0) {
        clear_variant_index_slot04_10(obj);
    } else {
        obj->pos_x = ref_other.p->pos_x;
        obj->pos_y = ref_other.p->pos_y;
        obj->facing_side = ref_other.p->facing_side;
        obj->field_01 = 1;
        if (ref_other.p->frame->frame_param != obj->variant_index) {
            set_variant_index_and_sequence_slot04_10(obj, ref_other.p->frame->frame_param);
        } else {
            advance_sequence_checked(obj);
        }
    }
}

void increment_state_0_2_slot04_10(Object *obj) {
    obj->state_0++;
}

void release_object_or_clear_link_slot04_10(Object *o) {
    Object *p = ref_other.p;

    if (p->linked_object != (s32)o) {
        clear_object_and_push_on_free_stack(o);
    } else {
        p->linked_object = 0;
    }
}

void clear_variant_index_slot04_10(Object *obj) {
    obj->variant_index = 0;
}

void set_variant_index_and_sequence_slot04_10(Object *obj, s16 idx) {
    obj->variant_index = idx;
    set_sequence_from_table_5(obj, idx, data_801c6e70_slot04_10);
}

void dispatch_by_input_dir_0_2_slot04_10(Object *obj) {
    if (obj->input_dir_0 == 4) {
        call_handler_by_step_limit_slot04_10(obj);
    } else if (obj->input_dir_0 != 0) {
        set_flag_and_call_function_by_state_3_slot04_10(obj);
    } else {
        call_handler_by_input_dir_second_2_slot04_10(obj);
    }
}

void call_handler_by_input_dir_second_2_slot04_10(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_state_3_0_1_2_slot04_10(obj);
    } else {
        call_handler_by_state_3_0_1_slot04_10(obj);
    }
}

void call_handler_by_state_3_0_1_slot04_10(Object *obj) {
    if (obj->state_3 == 0) {
        restore_facing_force_other_wide_or_set_sequence_slot04_10(obj);
    } else if (obj->state_3 == 1) {
        update_sequence_nudge_x_or_reset_slot04_10(obj);
    }
}

void restore_facing_force_other_wide_or_set_sequence_slot04_10(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x26, 0x14) != 0) {
        set_states_1_2_0_0_2_slot04_10(obj);
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void set_states_1_2_0_0_2_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void update_sequence_nudge_x_or_reset_slot04_10(Object *obj) {
    s16 t = obj->step_flags;
    s16 n;

    if (t < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0x100) {
            n = 3;
            if (obj->facing_side == 0) {
                n = -3;
            }
            obj->pos_x = n + obj->pos_x;
        }
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_0_1_2_slot04_10(Object *obj) {
    if (obj->state_3 == 0) {
        restore_facing_force_other_near_or_set_sequence_slot04_10(obj);
    } else if (obj->state_3 == 1) {
        update_sequence_reset_state_3_on_check_slot04_10(obj);
    }
}

void restore_facing_force_other_near_or_set_sequence_slot04_10(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x24, 0x14) != 0) {
        set_states_1_2_0_0_2_slot04_10(obj);
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_reset_state_3_on_check_slot04_10(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void set_flag_and_call_function_by_state_3_slot04_10(Object *obj) {
    obj->field_157 = 1;
    if (obj->state_3 == 0) {
        update_state_restore_facing_no_second_dir_slot04_10(obj);
    } else if (obj->state_3 == 1) {
        update_sequence_reset_state_3_on_check_2_slot04_10(obj);
    }
}

void update_state_restore_facing_no_second_dir_slot04_10(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->input_dir_second == 0 && obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x26, 0x14) != 0) {
        set_states_1_2_0_0_2_slot04_10(obj);
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_reset_state_3_on_check_2_slot04_10(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void start_attack_sequence_by_inputs_slot04_10(Object *obj) {
    s16 n;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->attack_param_d != 0) {
        if (obj->input_dir_second != 0) {
            set_states_step_limit_one_add_meter_sequence_slot04_10(obj);
        } else if (obj->variant_index == 0) {
            set_states_step_limit_two_add_meter_sequence_slot04_10(obj);
        } else {
            set_states_step_limit_zero_add_meter_sequence_slot04_10(obj);
        }
    } else {
        n = 0xc;
        if (obj->variant_index != 0) {
            n = 0x12;
        }
        if (obj->input_dir_second != 0) {
            n += 3;
        }
        n += obj->direction_index >> 1;
        set_sequence_by_side_and_mode(obj, n);
    }
}
