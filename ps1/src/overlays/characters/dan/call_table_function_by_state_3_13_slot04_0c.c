/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801bdab4_slot04_0c[];
extern ObjectFn data_801bdb0c_slot04_0c[];
extern ObjectFn data_801bdb18_slot04_0c[];
extern u8 data_801bdafc_slot04_0c[];
extern s8 data_801bdb04_slot04_0c[];
extern s16 data_801bd7fc_slot04_0c[];

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void set_sequence_by_side(Object *object, int arg);
void add_vel_x_on_step_flag_then_update_slot04_0c(Object *obj);

void call_table_function_by_state_3_13_slot04_0c(Object *obj) {
    data_801bdab4_slot04_0c[obj->state_3](obj);
}

void start_meter_cost_sequence_clear_direction_slot04_0c(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, -0x30);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->active_flag_3 = 0;
    obj->field_157 = 1;
    obj->field_255 = 0;
    obj->direction_index = 0;
    set_sequence_by_side_and_mode(obj, 0x1b);
}

void spawn_children_and_set_countdown_on_step_flag_slot04_0c(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->countdown = 0x32;
        obj->state_3 = obj->state_3 + 1;
        spawn_children_per_direction_index(obj, -0x25, 0x41);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void count_down_then_advance_state_3_slot04_0c(Object *obj) {
    update_sequence(obj);
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->countdown = 1;
        obj->field_165 = 0;
        obj->state_3 = obj->state_3 + 1;
    }
}

void count_down_then_set_vel_x_by_facing_slot04_0c(Object *obj) {
    int v = obj->countdown;
    int w = (v & 0xff00) | ((v & 0xff) - 1);
    obj->countdown = w;
    if ((w & 0xff) == 0) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->field_157 = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -0x30000;
        } else {
            obj->vel_x = 0x30000;
        }
        set_sequence_by_side_and_mode(obj, 0x3f);
    }
    update_sequence(obj);
}

void finish_or_continue_moving_sequence_slot04_0c(Object *obj) {
    Config *c;
    s16 x;
    u8 t;
    int idx;

    if ((s16)obj->step_flags & 0x8000) {
        c = game_state.config;
        if ((c->mode_flag_b | c->mode_flag_c | c->mode_flag_a) != 0) {
            reset_object_to_state_1_0_0_2_sequence_1(obj);
        } else {
            obj->active_state = 0;
            obj->field_157 = 0;
            x = obj->countdown;
            t = obj->state_3 + 1;
            idx = (x & 0xff00) >> 8;
            obj->state_3 = t;
            obj->facing_side = obj->saved_facing;
            obj->countdown = (x & -0x100) | (s8)data_801bdafc_slot04_0c[idx];
            idx = data_801bdb04_slot04_0c[idx];
            play_side_sound_at(obj, obj->side, data_801bd7fc_slot04_0c[idx]);
            obj->countdown = obj->countdown + 0x100;
            set_sequence_by_side_and_mode(obj, 0x3c);
        }
    } else {
        add_vel_x_on_step_flag_then_update_slot04_0c(obj);
    }
}

void end_sequence_set_countdown_or_move_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->countdown = 0x30;
        obj->active_state = 0;
        obj->field_157 = 1;
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x3d);
    } else {
        add_vel_x_on_step_flag_then_update_slot04_0c(obj);
    }
}

void launch_on_step_flag_by_facing_slot04_0c(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->field_157 = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -0x1c000;
        } else {
            obj->vel_x = 0x1c000;
        }
        obj->vel_y = 0x80000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
    }
}

void update_airborne_until_landing_slot04_0c(Object *obj) {
    Config *c;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if (*(u8 *)&obj->step_flags == 0) {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        c = game_state.config;
        if ((c->mode_flag_b | c->mode_flag_c | c->mode_flag_a) != 0) {
            reset_object_to_state_1_0_sequence_0(obj);
        } else {
            obj->countdown = 0x70;
            obj->facing_side = obj->saved_facing;
            obj->field_29c = 2;
            set_sequence_by_side_and_mode(obj, 0x3e);
        }
    }
}

void count_down_then_reset_object_else_update_3_slot04_0c(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void add_vel_x_on_step_flag_then_update_slot04_0c(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
    }
    update_sequence(obj);
}

void call_table_function_by_state_3_14_slot04_0c(Object *obj) {
    data_801bdb0c_slot04_0c[obj->state_3](obj);
}

void start_sequence_18_with_sound_and_facing_slot04_0c(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 == 0) {
        if (obj->input_bits & 0x8000) {
            obj->facing_side = 1;
        }
    }
    set_sequence_by_side_and_mode(obj, 0x18);
}

void hit_other_on_flag_then_update_sequence_slot04_0c(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0xf, 0, 1, 0);
    }
    update_sequence(obj);
}

void update_sequence_or_flip_facing_and_reset_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_state_2_2_slot04_0c(Object *obj) {
    data_801bdb18_slot04_0c[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_slot04_0c(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_unless_config_hold_slot04_0c(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}
