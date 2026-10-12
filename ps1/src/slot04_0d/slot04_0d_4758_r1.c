/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3648_slot04_0d[];
extern ObjectFn data_801c3654_slot04_0d[];
extern ObjectFn data_801c36dc_slot04_0d[];
extern ObjectFn data_801c36ec_slot04_0d[];
extern SequenceStep *data_801c0a60_slot04_0d[];

void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void clear_or_set_flag_by_pos_x_limit(Object *o);
void reset_and_push_object_to_pool_table(Slab172 *s);
void call_handler_by_state_1_unless_gate_slot04_0d(Object *obj);
void set_sequence_and_tables_from_direction(Object *obj);

void restore_facing_check_range_or_start_sequence_slot04_0d(Object *obj) {
    u8 one;

    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if (try_force_other_state_if_box_in_range(obj, -0x11, 0x14) & 0xff) {
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

void update_sequence_reset_state_3_on_check_2_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void update_sequence_with_flagged_x_frac_nudge_slot04_0d(Object *obj) {
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

void call_handler_by_half_direction_index_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    data_801c3648_slot04_0d[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_17_slot04_0d(Object *obj) {
    data_801c3654_slot04_0d[obj->state_3](obj);
}

void advance_state_3_restore_facing_set_sequence_slot04_0d(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_reset_on_negative_flags_2_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void start_sequence_by_variant_with_meter_sound_slot04_0d(Object *obj) {
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
    play_sound_from_table_by_direction_index(obj);
}

void call_handler_by_state_0_2_slot04_0d(Object *obj) {
    data_801c36dc_slot04_0d[obj->state_0](obj);
}

void init_rise_toward_parent_then_call_handler_slot04_0d(Object *obj) {
    obj->vel_y = 0xfffe0000;
    obj->accel_y = 0x1000;
    obj->state_0 = obj->state_0 + 1;
    obj->vel_x = (*(s32 *)&obj->parent->pos_x_frac - *(s32 *)&obj->pos_x_frac) >> 6;
    set_sequence_from_table_5(obj, 0, data_801c0a60_slot04_0d);
    call_handler_by_state_1_unless_gate_slot04_0d(obj);
}

void call_handler_by_state_1_unless_gate_slot04_0d(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0 = 2;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        data_801c36ec_slot04_0d[obj->state_1](obj);
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}

void update_rise_until_parent_ground_offset_slot04_0d(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    t = (u16)obj->parent->ground_y;
    t -= 0x55;
    if (obj->pos_y < t) {
        advance_sequence_checked(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->countdown = (obj->countdown & 0xff00) | 4;
    }
}

void count_down_then_set_upward_velocity_state_1_slot04_0d(Object *obj) {
    u16 t = obj->countdown;
    t -= 1;
    obj->countdown = t;
    if ((u8)t == 0) {
        obj->vel_y = 0xffff0000;
        obj->accel_y = 0x2000;
        obj->state_1 = obj->state_1 + 1;
    }
}

void update_fall_until_parent_ground_slot04_0d(Object *obj) {
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
        set_sequence_from_table_5(obj, 1, data_801c0a60_slot04_0d);
    }
}

void call_advance_sequence_checked_slot04_0d(Object *obj) {
    advance_sequence_checked(obj);
}

void increment_state_0_2_slot04_0d(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_record_slot04_0d(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
