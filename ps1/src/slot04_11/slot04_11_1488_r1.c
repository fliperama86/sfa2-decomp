/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7388_slot04_11[];
extern u8 data_801c7398_slot04_11[];
extern u16 data_801c739c_slot04_11[];
extern ObjectFn data_801c73a4_slot04_11[];
extern s32 data_801c73c0_slot04_11[];
extern u8 data_801c73f0_slot04_11[];
extern u8 data_801c73f8_slot04_11[];
extern u8 data_801c701c_slot04_11[];
extern u8 data_801c709c_slot04_11[];
void update_direction_hold_sequence_slot04_11(Object *obj);
void advance_state_3_set_direction_sequence_slot04_11(Object *obj);
void update_hold_sequence_or_end_to_release_slot04_11(Object *obj);
void count_down_then_advance_state_3_slot04_11(Object *obj);
void set_fall_accel_y_start_sequence_slot04_11(Object *obj);
void set_sequence_from_table_plus_step_limit_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_slot04_11(Object *obj);
void start_action_if_buttons_pressed(Object *object);
int apply_facing_velocity_with_x_stop_slot04_11(Object *obj);

void call_table_function_by_state_3_slot04_11(Object *obj) {
    data_801c7388_slot04_11[obj->state_3](obj);
}

void start_direction_sequence_add_meter_4_slot04_11(Object *obj) {
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

void update_sequence_or_advance_by_mode_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode != 0) {
        o->countdown = 0xf;
        o->state_3++;
        if (o->mode_flag_2 == 0) {
            update_direction_hold_sequence_slot04_11(o);
        } else {
            o->countdown = data_801c7398_slot04_11[o->input_dir_second];
            update_hold_sequence_or_end_to_release_slot04_11(o);
        }
    } else {
        update_sequence(o);
    }
}

void update_direction_hold_sequence_slot04_11(Object *obj) {
    s16 x;
    int h;
    u8 s;

    h = (s16)obj->step_flags >> 8;
    if (obj->mode_flag_2 != 0) {
        update_hold_sequence_or_end_to_release_slot04_11(obj);
    } else if ((*(u32 *)&game_state.leader_bits & 0xffff00) != 0 || game_state.field_04 != 0 ||
               (is_other_object_state_0_and_1_six_one(obj) & 0xff) != 0) {
        advance_state_3_set_direction_sequence_slot04_11(obj);
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
            count_down_then_advance_state_3_slot04_11(obj);
        } else {
            if ((obj->pressed_bits & 0x80) != 0) {
                x = 0;
            } else if ((obj->pressed_bits & 0x10) != 0) {
                x = 2;
            } else {
                x = 4;
            }
            if (obj->countdown <= data_801c739c_slot04_11[x >> 1]) {
                count_down_then_advance_state_3_slot04_11(obj);
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
                    obj->step_flags = obj->sequence->flags;
                    obj->step_timer = obj->sequence->duration;
                    obj->frame = obj->frames + obj->sequence->frame_index;
                    obj->frame_sound_index = obj->frame->sequence_param;
                    obj->sequence_changed_flag = 1;
                }
            }
        }
    }
}

void count_down_then_advance_state_3_slot04_11(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        advance_state_3_set_direction_sequence_slot04_11(obj);
    }
}

void advance_state_3_set_direction_sequence_slot04_11(Object *obj) {
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

void update_hold_sequence_or_end_to_release_slot04_11(Object *obj) {
    u16 t;

    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) != 0 ||
        (is_other_object_state_0_and_1_six_one(obj) & 0xff) != 0) {
        goto tail;
    }
    t = obj->step_flags;
    if (((t >> 8) & 0x7f) != 0 && (s16)obj->step_timer == 1) {
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
        } else if ((test_random_bit_in_table(obj, (u32 *)data_801c701c_slot04_11) & 0xff) != 0) {
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

void update_sequence_or_set_other_state_code_5_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_2_slot04_11(Object *obj) {
    data_801c73a4_slot04_11[obj->state_3](obj);
}

void start_table_velocity_move_with_meter_gain_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int d;

    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    o->active_flag = 0;
    o->step_limit = 0;
    o->vel_x = data_801c73c0_slot04_11[o->direction_index * 2];
    o->accel_x = data_801c73c0_slot04_11[o->direction_index * 2 + 1];
    o->vel_y = data_801c73c0_slot04_11[o->direction_index * 2 + 2];
    o->accel_y = data_801c73c0_slot04_11[o->direction_index * 2 + 3];
    d = 0x10;
    if (o->facing_side == 0) {
        d = -0x10;
    }
    o->pos_x = o->pos_x + d;
    play_side_sound_at(o, obj->field_a6, 5);
    if (o->field_49 == 0) {
        set_sequence_from_table_plus_step_limit_slot04_11(o);
    } else {
        set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x46);
    }
}

void enter_next_state_3_if_mode_zero_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
        o->step_count_2 = data_801c73f0_slot04_11[o->direction_index & 0xfe];
    }
    update_sequence(o);
}

void func_801b1bcc_slot04_11(Object *obj) {
    if (obj->active_flag != 0) {
        obj->vel_x = 0xc000;
        obj->vel_y = 0x19000;
        obj->accel_x = 0;
        obj->accel_y = -0x1000;
        obj->countdown = 0x1801;
        obj->state_3++;
    }
    if (apply_facing_velocity_with_x_stop_slot04_11(obj) < 0) {
        set_fall_accel_y_start_sequence_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void set_fall_accel_y_start_sequence_slot04_11(Object *obj) {
    int a = -0x6000;

    obj->state_3 = 5;
    if (obj->field_49 != 0) {
        a = -0x12000;
    }
    obj->accel_y = a;
    set_sequence_by_side_and_mode(obj, 0x3c);
}

void func_801b1c98_slot04_11(Object *o) {
    u8 t;

    if (apply_facing_velocity_with_x_stop_slot04_11(o) < 0 || ((Slot04bObj *)o)->field_47 == 0) {
        set_fall_accel_y_start_sequence_slot04_11(o);
    } else {
        ((Slot04bObj *)o)->field_47--;
        ((Slot04bObj *)o)->countdown++;
        if (o->active_flag != 0) {
            if (o->mode_flag_2 != 0 ? (((Slot04bObj *)o)->field_47 < 6 &&
                                    (test_random_bit_in_table(o, (u32 *)data_801c709c_slot04_11) & 0xff) == 0)
                                 : ((o->pressed_bits & data_801c73f8_slot04_11[o->direction_index >> 1]) != 0)) {
                o->step_limit++;
                t = ((Slot04bObj *)o)->countdown;
                if (t != 0) {
                    t--;
                }
                ((Slot04bObj *)o)->field_47 = t;
                play_side_sound_at(o, o->side, 6);
                ((Slot04bObj *)o)->countdown = 0;
                o->active_flag = 0;
                if (o->step_limit == o->step_count_2) {
                    o->accel_x = -0x2000;
                    o->accel_y = -0x5000;
                    o->state_3++;
                }
                o->vel_x = 0xc000;
                o->vel_y = 0x19000;
                set_sequence_from_table_plus_step_limit_slot04_11(o);
            }
        }
        update_sequence(o);
    }
}

void move_and_set_fall_accel_when_vel_y_negative_slot04_11(Object *obj) {
    int a;

    if (apply_facing_velocity_with_x_stop_slot04_11(obj) < 0) {
        a = -0x7000;
        obj->state_3++;
        if (obj->field_49 != 0) {
            a = -0x15000;
        }
        obj->accel_y = a;
    }
    update_sequence(obj);
}
