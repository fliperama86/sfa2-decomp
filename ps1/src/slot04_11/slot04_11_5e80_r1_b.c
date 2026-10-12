/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7a34_slot04_11[];
extern ObjectFn data_801c7a40_slot04_11[];
extern ObjectFn data_801c7a48_slot04_11[];
extern u8 data_801c7a50_slot04_11[];

void reset_object_to_state_1_0_0_2_sequence_1(Object *obj);
void land_reset_fields_and_choose_next_state(Object *obj);
void func_8007bc00(Object *obj);
void func_8007be8c(Object *obj);
void copy_kind_table_to_scratch_by_side_slot04_11(Object *obj);
void enable_active_flag_3_and_select_sequence_3_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_11(Object *obj);
void call_state_3_handler_wrapper_3_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);
void enable_active_flag_3_and_select_sequence_3_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_11(Object *obj);
void call_state_3_handler_wrapper_3_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);
void start_special_or_direction_sequence_slot04_11(Object *obj);

void call_reset_object_to_state_1_0_sequence_0_2_slot04_11(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void call_reset_object_to_state_1_0_0_2_sequence_1_2_slot04_11(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_state_3_handler_wrapper_2_slot04_11(Object *obj) {
    land_reset_fields_and_choose_next_state(obj);
}

void set_kind_11_or_call_by_input_dir_slot04_11(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        copy_kind_table_to_scratch_by_side_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_8007bc00(obj);
    } else {
        data_801c7a34_slot04_11[obj->input_dir_0 >> 1](obj);
    }
}

void call_handler_by_state_3_clear_field_3_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7a40_slot04_11[obj->state_3](obj);
}

void update_state_or_switch_to_state_1_2_slot04_11(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x17, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        enable_active_flag_3_and_select_sequence_3_slot04_11(obj);
        shift_pos_x_by_step_flags_low_bits_slot04_11(obj);
    }
}

void update_sequence_shift_x_or_reset_on_end_slot04_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        call_reset_object_to_state_1_0_sequence_0_3_slot04_11(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
        shift_pos_x_by_step_flags_low_bits_slot04_11(obj);
    }
}

void call_handler_by_state_3_set_flag_3_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c7a48_slot04_11[obj->state_3](obj);
}

void advance_state_3_and_restore_facing_slot04_11(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->field_29c = 1;
    }
    enable_active_flag_3_and_select_sequence_3_slot04_11(obj);
    shift_pos_x_by_step_flags_low_bits_slot04_11(obj);
}

void reset_on_negative_flags_then_update_and_shift_slot04_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_11(obj);
    }
    if (consume_active_frame_flag(obj) & 0xff) {
        obj->state_3 = 0;
    }
    update_sequence(obj);
    shift_pos_x_by_step_flags_low_bits_slot04_11(obj);
}

void update_airborne_motion_or_land_slot04_11(Object *obj) {
    int n;

    if (obj->active_flag != 0 && *(u8 *)&obj->step_flags != 0 && obj->hit_flag_a != 0) {
        n = 0x18;
        if (obj->variant_index != 0) {
            n = 0x19;
        }
        set_sequence_by_side_and_mode(obj, n);
    } else if (apply_velocity_and_acceleration_3_slot04_11(obj) < 0 && obj->pos_y >= obj->ground_y) {
        play_object_sound_by_kind(obj);
        call_state_3_handler_wrapper_3_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void init_kind_11_or_start_direction_sequence_slot04_11(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        copy_kind_table_to_scratch_by_side_slot04_11(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_8007be8c(obj);
    } else if (obj->limit_lo != 0) {
        start_special_or_direction_sequence_slot04_11(obj);
    } else {
        start_direction_sequence_slot04_11(obj);
    }
}

void start_direction_sequence_slot04_11(Object *obj) {
    s16 n;
    s16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, *(s16 *)((u8 *)data_801c7a50_slot04_11 + (obj->direction_index & 0xfe)));
    n = 0xc;
    if (obj->variant_index != 0) {
        n = 0x12;
    }
    if (obj->input_dir_second != 0) {
        n += 3;
    }
    t = obj->direction_index >> 1;
    t += n;
    set_sequence_by_side_and_mode(obj, t);
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        obj->active_flag = 0;
    }
}
