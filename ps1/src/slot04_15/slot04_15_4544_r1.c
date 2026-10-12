/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3cb0_slot04_15[];
extern ObjectFn data_801c3cbc_slot04_15[];
extern ObjectFn data_801c3cc4_slot04_15[];
extern ObjectFn data_801c3cd0_slot04_15[];
extern ObjectFn data_801c3cdc_slot04_15[];
extern ObjectFn data_801c3ce8_slot04_15[];
extern ObjectFn data_801c3cf4_slot04_15[];
extern SequenceStep *data_801c0fc4_slot04_15[];

void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void set_sequence_and_tables_from_direction(Object *object);
void reset_and_push_object_to_pool_table(Slab172 *s);
void call_handler_by_half_direction_index_slot04_15(Object *obj);
void call_handler_by_input_dir_second_3_slot04_15(Object *obj);
void call_handler_by_direction_index_4_slot04_15(Object *obj);
void call_handler_by_direction_index_3_slot04_15(Object *obj);

void update_fall_until_parent_ground_slot04_15(Object *obj) {
    Object *p;
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    p = obj->parent;
    t = p->ground_y;
    if (t > obj->pos_y) {
        advance_sequence_checked(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->pos_y = (u16)p->ground_y;
        set_sequence_from_table_5(obj, 1, data_801c0fc4_slot04_15);
    }
}

void call_advance_sequence_checked_slot04_15(Object *obj) {
    advance_sequence_checked(obj);
}

void increment_state_0_2_slot04_15(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_record_slot04_15(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_input_dir_0_nonzero_2_slot04_15(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_handler_by_half_direction_index_slot04_15(obj);
    } else {
        call_handler_by_input_dir_second_3_slot04_15(obj);
    }
}

void call_handler_by_input_dir_second_3_slot04_15(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_4_slot04_15(obj);
    } else {
        call_handler_by_direction_index_3_slot04_15(obj);
    }
}

void call_handler_by_direction_index_3_slot04_15(Object *obj) {
    data_801c3cb0_slot04_15[obj->direction_index >> 1](obj);
}

void func_801b46fc_slot04_15(Object *object) {
    data_801c3cbc_slot04_15[object->state_3](object);
}

void func_801b473c_slot04_15(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_reset_state_3_on_check_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4830_slot04_15(Object *object) {
    data_801c3cc4_slot04_15[object->state_3](object);
}

void func_801b4870_slot04_15(Object *obj) {
    u8 one;

    obj->state_3++;
    if (obj->hit_flag_a != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        one = 1;
        obj->active_flag_3 = one;
        set_sequence_and_tables_from_direction(obj);
        ((Slot04bObj *)obj)->field_278 = one;
    }
}

void update_sequence_with_facing_x_nudge_slot04_15(Object *obj) {
    s16 d;

    if (obj->step_flags & 0xff) {
        obj->state_3++;
        d = -0x18;
        if (obj->facing_side != 0) {
            d = 0x18;
        }
        *(s32 *)&obj->pos_x_frac += d;
    }
    if (consume_active_frame_flag(obj) & 0xff) {
        obj->state_3 = 0;
    }
    update_sequence(obj);
}

void func_801b4974_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_direction_index_4_slot04_15(Object *obj) {
    data_801c3cd0_slot04_15[obj->direction_index >> 1](obj);
}

void func_801b4a1c_slot04_15(Object *object) {
    data_801c3cdc_slot04_15[object->state_3](object);
}

void func_801b4a5c_slot04_15(Object *obj) {
    u8 one;

    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        one = 1;
        obj->active_flag_3 = one;
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->state_3 = 2;
            obj->field_157 = 0;
            reset_direction_and_action_fields(obj);
            ((Slot04bObj *)obj)->field_278 = one;
            set_sequence_by_side_and_mode(obj, 0x1f);
        } else {
            set_sequence_and_tables_from_direction(obj);
        }
    }
}

void update_sequence_reset_state_3_on_check_2_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4b9c_slot04_15(Object *obj) {
    s16 t = obj->step_flags;
    s16 d;

    if (t >= 0) {
        if ((u8)t != 0) {
            d = -2;
            if (obj->facing_side != 0) {
                d = 2;
            }
            *(s32 *)&obj->pos_x_frac += d;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_half_direction_index_slot04_15(Object *obj) {
    obj->field_157 = 1;
    data_801c3ce8_slot04_15[obj->direction_index >> 1](obj);
}

void func_801b4c58_slot04_15(Object *obj) {
    data_801c3cf4_slot04_15[obj->state_3](obj);
}

void func_801b4c98_slot04_15(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void func_801b4cd0_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void func_801b4d34_slot04_15(Object *obj) {
    u16 t;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    t = 0x12;
    if (obj->variant_index != 0) {
        t = 0xc;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    t = (obj->direction_index >> 1) + t;
    set_sequence_by_side_and_mode(obj, t);
    func_80120af8(obj);
}
