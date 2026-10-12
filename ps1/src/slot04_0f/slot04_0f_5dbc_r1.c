/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c62b8_slot04_0f[];
extern ObjectFn data_801c62c4_slot04_0f[];
extern ObjectFn data_801c62d0_slot04_0f[];
extern ObjectFn data_801c62d8_slot04_0f[];
extern ObjectFn data_801c62e4_slot04_0f[];
extern ObjectFn data_801c62ec_slot04_0f[];
extern ObjectFn data_801c62f8_slot04_0f[];
extern ObjectFn data_801c6300_slot04_0f[];
extern ObjectFn data_801c630c_slot04_0f[];
extern s32 data_801c6324_slot04_0f[];

void set_sequence_and_tables_from_direction(Object *obj);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void func_801b0e68_slot04_0f(Object *obj);
void func_801b1cc4_slot04_0f(Object *obj);
void enter_state_3_pick_sequence_by_input_dir_slot04_0f(Object *obj);
void call_handler_by_direction_index_2_slot04_0f(Object *obj);
void func_801b5e84_slot04_0f(Object *obj);
void call_handler_by_direction_index_3_slot04_0f(Object *obj);
void func_801b6060_slot04_0f(Object *obj);
void call_handler_by_direction_index_4_slot04_0f(Object *obj);
void func_801b6238_slot04_0f(Object *obj);
void call_handler_by_direction_index_5_slot04_0f(Object *obj);
void func_801b63d0_slot04_0f(Object *obj);
void func_801b66ac_slot04_0f(Object *obj);

void call_handler_by_input_dir_0_2_slot04_0f(Object *obj) {
    data_801c62b8_slot04_0f[obj->input_dir_0 >> 1](obj);
}

void call_handler_by_input_dir_second_2_slot04_0f(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_3_slot04_0f(obj);
    } else {
        call_handler_by_direction_index_2_slot04_0f(obj);
    }
}

void call_handler_by_direction_index_2_slot04_0f(Object *obj) {
    data_801c62c4_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b5e84_slot04_0f(Object *obj) {
    data_801c62d0_slot04_0f[obj->state_3](obj);
}

void func_801b5ec4_slot04_0f(Object *obj) {
    func_801b5e84_slot04_0f(obj);
}

void func_801b5ee4_slot04_0f(Object *obj) {
    func_801b5e84_slot04_0f(obj);
}

void func_801b5f04_slot04_0f(Object *obj) {
    u8 t;

    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->hit_flag_a == 0 || !func_8013f8c4(obj, -0x14, 0x14)) {
        obj->active_flag_3 = 1;
        if (obj->attack_param_d != 0) {
            t = obj->direction_index >> 1;
            reset_direction_and_action_fields(obj);
            set_sequence_by_side_and_mode(obj, t + 0x1a);
        } else {
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void update_sequence_or_reset_on_end_flag_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_direction_index_3_slot04_0f(Object *obj) {
    data_801c62d8_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b6060_slot04_0f(Object *obj) {
    data_801c62e4_slot04_0f[obj->state_3](obj);
}

void func_801b60a0_slot04_0f(Object *obj) {
    func_801b6060_slot04_0f(obj);
}

void func_801b60c0_slot04_0f(Object *obj) {
    func_801b6060_slot04_0f(obj);
}

void func_801b60e0_slot04_0f(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d != 0) {
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x1d);
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_or_reset_on_end_flag_2_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b61b4_slot04_0f(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_5_slot04_0f(obj);
    } else {
        call_handler_by_direction_index_4_slot04_0f(obj);
    }
}

void call_handler_by_direction_index_4_slot04_0f(Object *obj) {
    data_801c62ec_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b6238_slot04_0f(Object *obj) {
    data_801c62f8_slot04_0f[obj->state_3](obj);
}

void func_801b6278_slot04_0f(Object *obj) {
    func_801b6238_slot04_0f(obj);
}

void func_801b6298_slot04_0f(Object *obj) {
    func_801b6238_slot04_0f(obj);
}

void func_801b62b8_slot04_0f(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d == 0) {
        set_sequence_and_tables_from_direction(obj);
    } else {
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x20);
    }
}

void update_sequence_or_reset_on_end_flag_3_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void call_handler_by_direction_index_5_slot04_0f(Object *obj) {
    data_801c6300_slot04_0f[obj->direction_index >> 1](obj);
}

void func_801b63d0_slot04_0f(Object *obj) {
    data_801c630c_slot04_0f[obj->state_3](obj);
}

void func_801b6410_slot04_0f(Object *obj) {
    func_801b63d0_slot04_0f(obj);
}

void func_801b6430_slot04_0f(Object *obj) {
    func_801b63d0_slot04_0f(obj);
}

void func_801b6450_slot04_0f(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d == 0) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_and_tables_from_direction(obj);
    } else {
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x23);
    }
}

void update_sequence_or_reset_on_end_flag_4_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj)) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void func_801b6530_slot04_0f(Object *obj) {
    int a;
    int b;

    obj->vel_y = 0;
    obj->accel_y = 0;
    obj->state_3 = obj->state_3 + 1;
    a = data_801c6324_slot04_0f[obj->direction_index >> 1];
    b = 0xffff0000;
    if (obj->facing_side == 0) {
        a = -a;
        b = 0x10000;
    }
    obj->vel_x = a;
    obj->accel_x = b;
    update_sequence(obj);
}

void func_801b6598_slot04_0f(Object *obj) {
    if (((Slot04bObj *)obj)->mode != 0) {
        obj->state_3 = obj->state_3 + 1;
    } else {
        update_sequence(obj);
    }
}

void slide_x_until_stop_seq_by_direction_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x36);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_finish_8_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b668c_slot04_0f(Object *obj) {
    func_801b0e68_slot04_0f(obj);
}

void func_801b66ac_slot04_0f(Object *obj) {
    if (obj->active_state != 0 && obj->hit_flag_a != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->variant_index = 0;
        obj->facing_side = obj->saved_facing;
        if (obj->input_dir_second != 0) {
            obj->variant_index = 2;
        }
        if (obj->direction_flag != 0) {
            obj->variant_index = obj->variant_index + 4;
        }
        func_801b1cc4_slot04_0f(obj);
    } else {
        obj->script_opcode_high = obj->script_opcode_high & 0xfe;
        obj->limit_lo = obj->limit_lo | 0x40;
        enter_state_3_pick_sequence_by_input_dir_slot04_0f(obj);
    }
}

void call_handler_by_limit_lo_bit_slot04_0f(Object *obj) {
    if (obj->limit_lo & 1) {
        func_801b66ac_slot04_0f(obj);
    } else {
        enter_state_3_pick_sequence_by_input_dir_slot04_0f(obj);
    }
}
