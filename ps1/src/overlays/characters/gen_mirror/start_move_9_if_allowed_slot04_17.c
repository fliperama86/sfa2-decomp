/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 try_action_clearing_both_flags(Object *object);
int try_spend_meter_for_action(Object *object);
int is_not_state_2_five(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void start_action_if_buttons_pressed(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);

extern ObjectFnInt data_801ce260_slot04_17[];
extern ObjectFn data_801ce288_slot04_17[];
extern ObjectFn data_801ce2bc_slot04_17[];
extern u8 data_801ce2cc_slot04_17[];
extern s16 data_801ce2d0_slot04_17[];
extern ObjectFn data_801ce2d8_slot04_17[];
extern s32 data_801ce2f4_slot04_17[];
extern u8 data_801ce324_slot04_17[];
extern u8 data_801cdf50_slot04_17[];

void start_move_10_lock_other_slot04_17(Object *obj);
void start_move_11_lock_other_slot04_17(Object *obj);
int is_kind_seventeen_slot04_17(Object *obj);
int is_kind_nineteen_slot04_17(Object *obj);
void update_direction_held_sequence_slot04_17(Object *obj);
void advance_state_3_set_direction_sequence_slot04_17(Object *obj);
void update_sequence_or_end_on_conditions_slot04_17(Object *obj);
void count_down_then_advance_state_3_slot04_17(Object *obj);
void set_fall_accel_y_start_sequence_slot04_17(Object *obj);
void set_sequence_from_table_plus_step_limit_slot04_17(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_slot04_17(Object *obj);
int apply_facing_velocity_with_x_stop_slot04_17(Object *obj);
void copy_kind_table_to_scratch_by_side_slot04_17(Object *obj);

int start_move_9_if_allowed_slot04_17(Object *obj) {
    if (obj->field_7e != 0) goto c;
    if (obj->remaining_count == 0) goto z;
c:
    if (try_action_clearing_both_flags(obj)) goto b;
z:
    return 0;
b:
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->facing_side = obj->saved_facing;
    return 1;
}

int try_start_move_ten_spending_meter_slot04_17(Object *obj) {
    if (try_spend_meter_for_action(obj)) {
        start_move_10_lock_other_slot04_17(obj);
        return 1;
    }
    return 0;
}

void start_move_10_lock_other_slot04_17(Object *obj) {
    int a = 0x10;
    int b = 0x14;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0xa;
    obj->active_flag_3 = 1;
    obj->direction_index = 4;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x13;
        b = 0x17;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->other_lock = b;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x1e);
}

int try_start_move_eleven_spending_meter_slot04_17(Object *obj) {
    if (try_spend_meter_for_action(obj)) {
        start_move_11_lock_other_slot04_17(obj);
        return 1;
    }
    return 0;
}

void start_move_11_lock_other_slot04_17(Object *obj) {
    int a = 0x13;
    int b = 0x17;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0xb;
    obj->active_flag_3 = 1;
    obj->direction_index = 4;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x18;
        b = 0x1c;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->other_lock = b;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x1f);
}

int start_move_12_if_meter_ok_slot04_17(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state == 0) {
        if (!is_not_state_2_five(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->state_0 = 1;
        obj->state_2 = 8;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 0xc;
        obj->facing_side = obj->saved_facing;
        return 1;
    }
    if (obj->field_7e != 0) return 0;
    if (!(u8)try_action_unless_state_bytes_match(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 0xc;
    return 1;
}

void sync_kind_then_call_move_handler_slot04_17(Object *obj) {
    u8 k;

    if (obj->field_15a < 8) {
        k = 0x11;
        if (obj->attack_param_d != 0) {
            k = 0x13;
        }
        if (obj->kind != k) {
            obj->kind = k;
            copy_kind_table_to_scratch_by_side_slot04_17(obj);
            select_box_tables(obj);
            build_metrics(obj);
        }
    }
    script_condition_result = data_801ce260_slot04_17[obj->field_15a](obj);
}

int is_kind_seventeen_slot04_17(Object *obj) {
    return obj->kind == 0x11;
}

int check_kind_equals_seventeen_slot04_17(Object *obj) {
    return is_kind_seventeen_slot04_17(obj);
}

int is_kind_nineteen_slot04_17(Object *obj) {
    return obj->kind == 0x13;
}

int check_kind_equals_seventeen_2_slot04_17(Object *obj) {
    return is_kind_nineteen_slot04_17(obj);
}

int func_801b138c_slot04_17(Object *obj) {
    if (obj->kind != 0x11) return 0;
    return (s16)obj->meter >= 0x30;
}

int func_801b13bc_slot04_17(Object *obj) {
    if (obj->kind == 0x11) {
        return (s16)obj->meter >= 0x30;
    }
    return 0;
}

int func_801b13e4_slot04_17(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->meter >= 0x30;
    }
    return 0;
}

int is_kind_nineteen_with_small_meter_slot04_17(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->meter >= 6;
    }
    return 0;
}

int return_true_always_slot04_17(Object *obj) {
    return 1;
}

int has_remaining_count_slot04_17(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1448_slot04_17(Object *obj) {
    data_801ce288_slot04_17[obj->field_15a](obj);
}

void call_table_function_by_state_3_slot04_17(Object *obj) {
    data_801ce2bc_slot04_17[obj->state_3](obj);
}

void start_direction_sequence_add_meter_4_slot04_17(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->field_12d = 0xb4;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->input_matched_flag = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->direction_index;
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_or_advance_by_mode_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode != 0) {
        o->countdown = 0xf;
        o->state_3++;
        if (o->mode_flag_2 == 0) {
            update_direction_held_sequence_slot04_17(o);
        } else {
            o->countdown = data_801ce2cc_slot04_17[o->input_dir_second];
            update_sequence_or_end_on_conditions_slot04_17(o);
        }
    } else {
        update_sequence(o);
    }
}

void update_direction_held_sequence_slot04_17(Object *obj) {
    s16 x;
    int h;
    u8 s;

    h = (s16)obj->step_flags >> 8;
    if (obj->mode_flag_2 != 0) {
        update_sequence_or_end_on_conditions_slot04_17(obj);
    } else if ((*(u32 *)&game_state.leader_bits & 0xffff00) != 0 || game_state.field_04 != 0 ||
               (is_other_object_state_0_and_1_six_one(obj) & 0xff) != 0) {
        advance_state_3_set_direction_sequence_slot04_17(obj);
    } else {
        if ((h & 0x7f) != 0) {
            obj->step_flags = obj->step_flags & 0x80ff;
            reset_hold_flags_and_other_hit_count(obj);
            add_one_to_ec4_if_object_unflagged(&game_state, obj);
            if (obj->input_matched_flag == 0) {
                obj->input_matched_flag = 0xff;
            }
        }
        if ((obj->pressed_bits & 0x94) == 0) {
            count_down_then_advance_state_3_slot04_17(obj);
        } else {
            if ((obj->pressed_bits & 0x80) != 0) {
                x = 0;
            } else if ((obj->pressed_bits & 0x10) != 0) {
                x = 2;
            } else {
                x = 4;
            }
            if ((s16)obj->countdown <= data_801ce2d0_slot04_17[x >> 1]) {
                count_down_then_advance_state_3_slot04_17(obj);
            } else {
                obj->countdown = 0xf;
                if (obj->direction_index == x) {
                    update_sequence(obj);
                } else {
                    obj->direction_index = x;
                    s = ((Slot04bObj *)obj)->mode;
                    if (obj->field_49 != 0) {
                        x += 0x40;
                    } else {
                        x += 0x21;
                    }
                    set_sequence_by_side_and_mode(obj, x);
                    obj->sequence = obj->sequence + s;
                    obj->step_timer = obj->sequence->duration;
                    obj->frame = obj->frames + obj->sequence->frame_index;
                    obj->frame_sound_index = obj->frame->sequence_param;
                    obj->sequence_changed_flag = 1;
                }
            }
        }
    }
}

void count_down_then_advance_state_3_slot04_17(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        advance_state_3_set_direction_sequence_slot04_17(obj);
    }
}

void advance_state_3_set_direction_sequence_slot04_17(Object *obj) {
    int a;

    obj->state_3++;
    obj->active_flag_4 = 0;
    a = 0x22;
    if (obj->field_49 != 0) {
        a = 0x41;
    }
    a += obj->direction_index;
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_or_end_on_conditions_slot04_17(Object *obj) {
    u16 t;

    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) != 0 ||
        (is_other_object_state_0_and_1_six_one(obj) & 0xff) != 0) {
        goto tail;
    }
    t = obj->step_flags;
    if (((t >> 8) & 0x7f) != 0 && (t & 0x7f00) == 0x100) {
        obj->step_flags = t & 0x80ff;
        reset_hold_flags_and_other_hit_count(obj);
        add_one_to_ec4_if_object_unflagged(&game_state, obj);
        if (obj->input_matched_flag == 0) {
            obj->input_matched_flag = 0xff;
        }
        if (obj->input_dir_second != 0) {
            obj->countdown = (s16)obj->countdown - 1;
            if ((s16)obj->countdown == 0) {
                goto tail;
            }
        } else if ((test_random_bit_in_table(obj, (u32 *)data_801cdf50_slot04_17) & 0xff) != 0) {
            goto tail;
        }
    }
    update_sequence(obj);
    return;
tail:
    obj->state_3++;
    obj->active_flag_4 = 0;
    set_sequence_by_side_and_mode(obj, obj->direction_index + 0x22);
}

void update_sequence_or_set_other_state_code_5_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_17(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_2_slot04_17(Object *obj) {
    data_801ce2d8_slot04_17[obj->state_3](obj);
}

void start_table_velocity_move_with_meter_gain_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int d;

    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    o->active_flag = 0;
    o->step_limit = 0;
    o->vel_x = data_801ce2f4_slot04_17[o->direction_index * 2];
    o->accel_x = data_801ce2f4_slot04_17[o->direction_index * 2 + 1];
    o->vel_y = data_801ce2f4_slot04_17[o->direction_index * 2 + 2];
    o->accel_y = data_801ce2f4_slot04_17[o->direction_index * 2 + 3];
    d = 0x10;
    if (o->facing_side == 0) {
        d = -0x10;
    }
    o->pos_x = o->pos_x + d;
    play_side_sound_at(o, obj->field_a6, 5);
    if (o->field_49 == 0) {
        set_sequence_from_table_plus_step_limit_slot04_17(o);
    } else {
        set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x46);
    }
}

void enter_next_state_3_if_mode_zero_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
        o->step_count_2 = data_801ce324_slot04_17[o->direction_index & 0xfe];
    }
    update_sequence(o);
}

void launch_on_active_flag_then_move_or_fall_slot04_17(Object *obj) {
    if (obj->active_flag != 0) {
        obj->vel_x = 0xc000;
        obj->vel_y = 0x19000;
        obj->accel_x = 0;
        obj->accel_y = -0x1000;
        obj->countdown = 0x1801;
        obj->state_3++;
    }
    if (apply_facing_velocity_with_x_stop_slot04_17(obj) < 0) {
        set_fall_accel_y_start_sequence_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void set_fall_accel_y_start_sequence_slot04_17(Object *obj) {
    int a = -0x6000;

    obj->state_3 = 5;
    if (obj->field_49 != 0) {
        a = -0x12000;
    }
    obj->accel_y = a;
    set_sequence_by_side_and_mode(obj, 0x3c);
}
