/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c63b0_slot04_0f[];
extern ObjectFn data_801c63c0_slot04_0f[];
extern ObjectFn data_801c63cc_slot04_0f[];
extern SequenceStep **data_1f8000b0;
extern SequenceStep **data_1f800160;

void clear_or_set_flag_by_pos_x_limit(Object *o);
void reset_and_push_object_to_pool_table(Slab172 *s);
void set_sequence_by_variant_and_direction_a_slot04_0f(Object *obj);
void set_sequence_by_variant_and_direction_b_slot04_0f(Object *obj);
void count_down_then_switch_sequence_by_other_side_slot04_0f(Object *obj);
void add_velocity_to_position_slot04_0f(Object *obj);

void enter_state_3_pick_sequence_by_input_dir_slot04_0f(Object *obj) {
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    obj->input_dir_0 = 4;
    play_sound_from_table_by_direction_index(obj);
    if (obj->input_dir_second != 0) {
        set_sequence_by_variant_and_direction_b_slot04_0f(obj);
    } else {
        set_sequence_by_variant_and_direction_a_slot04_0f(obj);
    }
}

void set_sequence_by_variant_and_direction_a_slot04_0f(Object *obj) {
    int t;

    if (obj->direction_index != 0) {
        add_to_meter_clamped(obj, 1);
    }
    t = 0xc;
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->attack_param_d == 0) {
        if (obj->hit_flag_a != 0 && obj->pos_y < obj->ground_y - 0x20) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 5;
            obj->state_3 = 0;
            t = 0x32;
            goto call;
        }
    } else {
        t += 0x1a;
    }
    t += obj->direction_index >> 1;
call:
    set_sequence_by_side_and_mode(obj, t);
}

void set_sequence_by_variant_and_direction_b_slot04_0f(Object *obj) {
    int t;

    if (obj->direction_index != 0) {
        add_to_meter_clamped(obj, 1);
    }
    t = 0xf;
    if (obj->variant_index != 0) {
        t = 0x15;
    }
    if (obj->attack_param_d != 0) {
        t += 0x1a;
    } else if (obj->hit_flag_a != 0 && obj->pos_y < obj->ground_y - 0x20) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        t = 0x33;
    }
    set_sequence_by_side_and_mode(obj, t + (obj->direction_index >> 1));
}

void call_handler_by_state_0_with_parent_3_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    data_801c63b0_slot04_0f[obj->state_0](obj);
}

void set_sequence_0_from_other_side_table_slot04_0f(Object *obj) {
    SequenceStep **t;

    obj->state_0++;
    if (ref_other.p->side == 0) {
        t = data_1f8000b0;
    } else {
        t = data_1f800160;
    }
    set_sequence_from_table_5(obj, 0, t);
}

void call_handler_by_state_1_3_slot04_0f(Object *obj) {
    data_801c63c0_slot04_0f[obj->state_1](obj);
}

void advance_state_1_on_parent_step_flag_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    if ((u8)ref_other.p->step_flags != 0) {
        obj->state_1 = obj->state_1 + 1;
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}

void launch_with_velocity_at_sequence_end_slot04_0f(Object *obj) {
    int a;

    advance_sequence_checked(obj);
    if ((s16)obj->step_flags < 0) {
        obj->countdown = 0x1a;
        obj->state_1 = obj->state_1 + 1;
        a = 0x8000;
        if (obj->facing_side != 0) {
            a = -0x8000;
        }
        obj->vel_x = a;
        obj->vel_y = 0;
        obj->accel_y = 0x1000;
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void move_with_gravity_and_blink_countdown_slot04_0f(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    obj->countdown = (s16)obj->countdown - 1;
    t = obj->countdown;
    if (t == 0) {
        obj->state_0 = obj->state_0 + 1;
    } else {
        if (t < 0x12 && (t & 1) == 0) {
            obj->field_01 = 0;
        } else {
            obj->field_01 = 1;
            clear_or_set_flag_by_pos_x_limit(obj);
        }
    }
}

void release_object_record_3_slot04_0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void noop_slot00_slot04_0f(Object *obj) {
}

void call_handler_by_state_0_with_parent_4_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    data_801c63cc_slot04_0f[obj->state_0](obj);
}

void init_countdown_sequence_by_other_side_slot04_0f(Object *obj) {
    obj->countdown = 0x20;
    obj->spawn_flag = 1;
    obj->state_0 = obj->state_0 + 1;
    if (ref_other.p->side == 0) {
        set_sequence_from_table_5(obj, 0xc, seqs_a4_left);
    } else {
        set_sequence_from_table_5(obj, 0xc, seqs_154_right);
    }
    count_down_then_switch_sequence_by_other_side_slot04_0f(obj);
}

void count_down_then_switch_sequence_by_other_side_slot04_0f(Object *obj) {
    if (game_state.config->pause_flag_a == 0 && ref_other.p->field_6b == 0) {
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown == 0) {
            obj->state_0 = obj->state_0 + 1;
            if (ref_other.p->side == 0) {
                set_sequence_from_table_5(obj, 0x11, seqs_a4_left);
            } else {
                set_sequence_from_table_5(obj, 0x11, seqs_154_right);
            }
        }
        add_velocity_to_position_slot04_0f(obj);
        advance_sequence_checked(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void move_and_advance_state_0_at_sequence_end_slot04_0f(Object *obj) {
    if (game_state.config->pause_flag_a == 0 && ref_other.p->field_6b == 0) {
        advance_sequence_checked(obj);
        add_velocity_to_position_slot04_0f(obj);
        if ((s16)obj->step_flags < 0) {
            obj->state_0 = obj->state_0 + 1;
        }
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void release_object_record_4_slot04_0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void add_velocity_to_position_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
}
