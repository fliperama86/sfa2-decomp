/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801cea2c_slot04_17[];
extern u8 data_801cea34_slot04_17[];

void set_sequence_and_tables_from_direction(Object *obj);
void reset_object_to_state_1_0_0_2_sequence_1(Object *obj);
void func_80131638(Object *obj);
void func_801b1448_slot04_17(Object *obj);
void copy_kind_table_to_scratch_by_side_slot04_17(Object *obj);
void func_801b6fa0_slot04_17(Object *obj);
void start_direction_sequence_slot04_17(Object *obj);
void func_801b75dc_slot04_17(Object *obj);
void func_801b7684_slot04_17(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_17(Object *obj);
int apply_velocity_and_acceleration_3_slot04_17(Object *obj);
void func_801b772c_slot04_17(Object *obj);
void func_801b774c_slot04_17(Object *obj);
void func_801b776c_slot04_17(Object *obj);

void func_801b7244_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b772c_slot04_17(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
        shift_pos_x_by_step_flags_low_bits_slot04_17(obj);
    }
}

void call_handler_by_state_3_set_flag_4_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801cea2c_slot04_17[obj->state_3](obj);
}

void advance_state_3_and_restore_facing_slot04_17(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->field_29c = 1;
    }
    func_801b7684_slot04_17(obj);
    shift_pos_x_by_step_flags_low_bits_slot04_17(obj);
}

void func_801b7358_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b774c_slot04_17(obj);
    }
    if (consume_active_frame_flag(obj) & 0xff) {
        obj->state_3 = 0;
    }
    update_sequence(obj);
    shift_pos_x_by_step_flags_low_bits_slot04_17(obj);
}

void update_airborne_motion_or_land_slot04_17(Object *obj) {
    int n;

    if (obj->active_flag != 0 && *(u8 *)&obj->step_flags != 0 && obj->hit_flag_a != 0) {
        n = 0x18;
        if (obj->variant_index != 0) {
            n = 0x19;
        }
        set_sequence_by_side_and_mode(obj, n);
    } else if (apply_velocity_and_acceleration_3_slot04_17(obj) < 0 && obj->pos_y >= obj->ground_y) {
        play_object_sound_by_kind(obj);
        func_801b776c_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void init_kind_11_or_start_direction_sequence_slot04_17(Object *obj) {
    if (obj->attack_param_d == 0) {
        obj->kind = 0x11;
        copy_kind_table_to_scratch_by_side_slot04_17(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b6fa0_slot04_17(obj);
    } else if (obj->limit_lo != 0) {
        func_801b75dc_slot04_17(obj);
    } else {
        start_direction_sequence_slot04_17(obj);
    }
}

void start_direction_sequence_slot04_17(Object *obj) {
    s16 n;
    s16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, *(s16 *)((u8 *)data_801cea34_slot04_17 + (obj->direction_index & 0xfe)));
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

void func_801b75dc_slot04_17(Object *obj) {
    obj->input_dir_second = 2;
    if (obj->kind != 0x13 || (s16)obj->meter < 0x30 || obj->active_state != 0 || obj->ground_y - 0x18 < obj->pos_y) {
        start_direction_sequence_slot04_17(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_801b1448_slot04_17(obj);
    }
}

void func_801b7684_slot04_17(Object *obj) {
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void shift_pos_x_by_step_flags_low_bits_slot04_17(Object *obj) {
    u16 t = obj->step_flags;
    int d = t & 0x7f;

    if (d != 0) {
        obj->step_flags = t & 0xff80;
        if (obj->facing_side == 0) {
            d = -d;
        }
        obj->pos_x = obj->pos_x + d;
    }
}

int apply_velocity_and_acceleration_3_slot04_17(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    return obj->vel_y += obj->accel_y;
}

void func_801b772c_slot04_17(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void func_801b774c_slot04_17(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void func_801b776c_slot04_17(Object *obj) {
    func_80131638(obj);
}
