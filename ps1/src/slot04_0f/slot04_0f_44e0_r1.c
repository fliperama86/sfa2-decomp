/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5be4_slot04_0f[];
extern ObjectFn data_801c5bf0_slot04_0f[];
extern s8 data_801c5c0c_slot04_0f[];
extern u8 data_801c5c1c_slot04_0f[];

void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void set_sequence_by_side(Object *object, int arg);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
int match_pressed_direction_input(Object *object);
int try_match_input_direction_slot04_0f(Object *obj);
u16 *get_input_direction_table_entry_slot04_0f(Object *obj);
int test_input_mask_with_direction_slot04_0f(Object *obj, u16 *mask, BytePair dir);
void call_add_one_to_ec4_wrapper_3(GameState *state, Object *object);
void count_down_and_set_side_bit_slot04_0f(Object *obj);

void update_sequence_check_other_health_input_slot04_0f(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            if (obj->input_bits_2 & 0x1000) {
                enter_state_1_3_with_input_variant_and_sequence(obj);
            }
        }
    }
}

void update_sequence_or_finish_7_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_18_slot04_0f(Object *obj) {
    data_801c5be4_slot04_0f[obj->state_3](obj);
}

void advance_state_add_meter_set_facing_from_input_slot04_0f(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->other->side, 0x31a);
    obj->facing_side = 0;
    if ((obj->input_bits & 0x2000) == 0) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x19);
}

void start_hit_on_step_flag_byte_slot04_0f(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0x12, 0, 1, 0);
    }
    update_sequence(obj);
}

void flip_facing_on_sequence_end_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_reset_object_to_state_1_0_sequence_0_slot04_0f(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void call_reset_object_to_state_1_0_0_2_sequence_1_slot04_0f(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_handler_by_state_2_2_slot04_0f(Object *obj) {
    data_801c5bf0_slot04_0f[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_slot04_0f(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_unless_config_hold_slot04_0f(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void pick_random_action_set_sequence_slot04_0f(Object *obj) {
    Config *c;
    int r;

    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    c = game_state.config;
    if (c->countdown_2 == 0) {
        c->countdown_2 = 0x1e;
    }
    r = get_input_mapped_value_or_default(obj, data_801c5c0c_slot04_0f[get_random_byte() & 0xf]);
    obj->state_2 = data_801c5c1c_slot04_0f[(u8)r];
    set_sequence_by_side(obj, (r & 0xff) + 0x23);
}

void set_rise_velocity_on_step_flag_then_count_down_slot04_0f(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->vel_y = 0x18000;
        obj->accel_y = -0x2000;
        obj->state_2 = obj->state_2 + 1;
    }
    count_down_and_set_side_bit_slot04_0f(obj);
}

void count_down_and_set_side_bit_slot04_0f(Object *obj) {
    s16 t = obj->countdown;
    if (t != 0) {
        t = t - 1;
        obj->countdown = t;
        if (t == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}

void apply_vel_y_flip_to_rise_state_2_inc_slot04_0f(Object *obj) {
    *(u32 *)&obj->pos_y_frac = *(u32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y >= 0) {
        obj->vel_y = 0x18000;
        obj->accel_y = -0x2000;
        obj->state_2 = obj->state_2 + 1;
    }
    count_down_and_set_side_bit_slot04_0f(obj);
}

void apply_vel_y_flip_to_fall_state_2_dec_slot04_0f(Object *obj) {
    *(u32 *)&obj->pos_y_frac = *(u32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->vel_y = -0x18000;
        obj->accel_y = 0x2000;
        obj->state_2--;
    }
    count_down_and_set_side_bit_slot04_0f(obj);
}

void update_sequence_check_input_or_reset_slot04_0f(Object *obj) {
    int t = (s16)obj->step_flags;

    if (t >= 0) {
        if ((s16)t < 0 && match_pressed_direction_input(obj) != 0 || try_match_input_direction_slot04_0f(obj) != 0) {
            obj->state_3 = 0;
            obj->field_25f = 0xff;
        }
        update_sequence(obj);
    } else {
        if (obj->input_dir_0 == 0) {
            reset_object_to_state_1_0_sequence_0(obj);
        } else {
            reset_object_to_state_1_0_0_2_sequence_1(obj);
        }
    }
}

/* The call of call_add_one_to_ec4_wrapper_3 passes two arguments although the callee takes none: the original sets the first two argument registers before it. Written without the arguments, this function differs from the original in 9 instruction slots. */
int try_match_input_direction_slot04_0f(Object *obj) {
    BytePair dir;
    u16 *a;
    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_a) != 0) {
        return 0;
    }
    if ((is_other_object_state_0_and_1_six_one(obj) & 0xff) != 0 || obj->active_flag == 0 || (obj->pressed_bits & 0xfc) == 0) {
        return 0;
    }
    a = get_input_direction_table_entry_slot04_0f(obj);
    dir = get_direction_pair_from_pressed_bits(obj);
    if (obj->field_7e == 0) {
        if (test_input_mask_with_direction_slot04_0f(obj, a, dir) == 0) return 0;
    }
    dir.first = obj->direction_index;
    dir.second = obj->input_dir_second;
    obj->input_dir_0 = 0;
    if (obj->input_bits_2 & 0x4000) obj->input_dir_0 = 2;
    set_dir_match_flag(obj, dir);
    obj->dir_first = dir.first;
    obj->dir_second = dir.second;
    obj->field_17e = obj->input_dir_0;
    ((void (*)(Config *, Object *))call_add_one_to_ec4_wrapper_3)(game_state.config, obj);
    reset_hold_flags_and_other_hit_count(obj);
    return 1;
}
