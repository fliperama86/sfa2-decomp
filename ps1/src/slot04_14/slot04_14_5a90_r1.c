/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c64ec_slot04_14[];
extern ObjectFn data_801c64f8_slot04_14[];
extern ObjectFn data_801c650c_slot04_14[];
extern u8 data_801c651c_slot04_14[];
extern u16 data_801c652c_slot04_14[];
extern ObjectFn data_801c6538_slot04_14[];
extern u16 box_margin[];

void hit_other_on_flag_then_update_sequence_slot04_14(Object *obj);
u8 move_by_velocity_return_vel_y_negative_slot04_14(Object *obj);
u8 is_pos_x_beyond_box_margin_slot04_14(Object *obj);
void set_sequence_by_side(Object *object, int arg);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void call_table_function_by_state_3_20_slot04_14(Object *obj) {
    data_801c64ec_slot04_14[obj->state_3](obj);
}

void start_sidestep_set_facing_from_input_slot04_14(Object *obj) {
    Object *o = obj->other;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (is_pos_x_beyond_box_margin_slot04_14(obj) != 0) {
            obj->facing_side = obj->facing_side + 1;
        }
    } else if (obj->input_bits & 0x8000) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, (obj->input_dir_second >> 1) + 0x18);
}

void hit_other_on_flag_then_update_sequence_slot04_14(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0xf, 0, 1, 0);
    }
    update_sequence(obj);
}

void update_sequence_or_flip_facing_and_reset_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_21_slot04_14(Object *obj) {
    data_801c64f8_slot04_14[obj->state_3](obj);
}

void start_arc_move_facing_by_input_or_position_slot04_14(Object *obj) {
    Object *o = obj->other;
    int t;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    obj->vel_x = 0x48000;
    if (obj->mode_flag_2 != 0) {
        t = is_pos_x_beyond_box_margin_slot04_14(obj);
    } else {
        t = obj->input_bits & 0x8000;
    }
    if (t) {
        obj->vel_x = -0x48000;
        obj->facing_side = obj->facing_side + 1;
    }
    obj->vel_y = 0x20000;
    obj->accel_y = -0x2000;
    set_sequence_by_side_and_mode(obj, 0x45);
}

void advance_state_3_when_step_flags_zero_slot04_14(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
    }
    update_sequence(obj);
}

void update_arc_landing_set_state_param_slot04_14(Object *obj) {
    if (move_by_velocity_return_vel_y_negative_slot04_14(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
        }
    }
    hit_other_on_flag_then_update_sequence_slot04_14(obj);
}

void check_landing_set_state_param_slot04_14(Object *obj) {
    if (move_by_velocity_return_vel_y_negative_slot04_14(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
            obj->state_3 = obj->state_3 + 1;
        }
    }
}

u8 move_by_velocity_return_vel_y_negative_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->vel_y < 0;
}

u8 is_pos_x_beyond_box_margin_slot04_14(Object *obj) {
    return (s16)(box_margin[0] + 0xc0) < obj->pos_x;
}

void call_handler_by_state_2_2_slot04_14(Object *obj) {
    data_801c650c_slot04_14[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_slot04_14(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_when_timers_clear_slot04_14(Object *obj) {
    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void set_random_sequence_from_table_slot04_14(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    game_state.countdown = 0x1e;
    set_sequence_by_side(obj, data_801c652c_slot04_14[(u8)get_input_mapped_value_or_default(obj, data_801c651c_slot04_14[get_random_byte() & 0xf])]);
}

void update_sequence_with_countdown_side_bit_slot04_14(Object *obj) {
    u16 t = obj->step_flags;
    s16 c;
    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        game_state.state_param = 0x18;
        play_side_sound_at(obj, obj->side, 0xe);
    }
    c = obj->countdown;
    if (c != 0) {
        c = c - 1;
        obj->countdown = c;
        if (c == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}

void call_handler_by_state_2_3_slot04_14(Object *obj) {
    data_801c6538_slot04_14[obj->state_2](obj);
}

void advance_state_2_when_countdown_timer_zero_slot04_14(Object *obj) {
    if (game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void start_sequence_by_side_with_countdown_slot04_14(Object *obj) {
    int idx = 0x28;
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->other->facing_side;
    obj->countdown = 0x78;
    if (game_state.field_a6 != 0) {
        idx = 0x29;
    }
    set_sequence_by_side(obj, idx);
}

void count_down_and_set_side_bit_slot04_14(Object *obj) {
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

void clear_object_byte_block_slot04_14(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}
