/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c63c4_slot04_14[];
extern ObjectFn data_801c63cc_slot04_14[];
extern ObjectFn data_801c63d4_slot04_14[];
extern ObjectFn data_801c6420_slot04_14[];
extern s32 data_801c646c_slot04_14[];

void spawn_three_child_objects_at_fixed_spawn(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void fill_palette_banks_white(void);
void restore_palette_banks(void);
void add_table_bcd_score_to_player(int idx, int side);
void add_table_bcd_to_player_total(int idx, int side);
void set_other_hit_reaction_state_8(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int unused);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
u8 tick_countdown_spawn_random_effect_slot04_14(Object *obj);
void start_sequence_3b_with_other_sound_slot04_14(Object *obj);
void hit_other_and_spawn_on_defeat_slot04_14(Object *obj);

void start_meter_spend_spawn_three_children_slot04_14(Object *obj) {
    obj->direction_index = 4;
    obj->field_255 = 4;
    obj->active_flag_3 = 0;
    obj->state_3++;
    obj->countdown = (u8)obj->countdown | 0x3200;
    obj->variant_index = obj->facing_side;
    spawn_three_child_objects_at_fixed_spawn(obj);
    add_to_meter_clamped(obj, -0x90);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x3a);
}

void update_sequence_dash_spawn_on_flag_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        obj->vel_x = -0x80000;
        obj->countdown = (obj->countdown & 0xff00) | 0x30;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x80000;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x10, 0x58);
    }
    update_sequence(obj);
}

void count_down_high_byte_then_advance_state_3_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if (obj->countdown & 0x8000) {
        obj->field_165 = 0;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xc);
    }
    update_sequence(obj);
}

void move_by_vel_x_until_countdown_or_contact_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) != 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        if (((obj->bound_push_flag >> obj->variant_index) & 1) == 0) {
            if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
                start_sequence_3b_with_other_sound_slot04_14(obj);
            } else {
                update_sequence(obj);
            }
            return;
        }
    }
    reset_object_to_state_1_0_sequence_0(obj);
}

void start_sequence_3b_with_other_sound_slot04_14(Object *obj) {
    obj->state_3++;
    obj->countdown = (obj->countdown & 0xff00) | 0x10;
    play_sound_at(obj->other, obj->other->side, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x3b);
}

void count_down_low_byte_then_advance_state_3_slot04_14(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) == 0) {
        obj->state_3++;
        obj->countdown = (obj->countdown & 0xff00) | 0x3b;
        update_sequence(obj);
    }
}

void update_white_flash_countdown_then_hit_other_slot04_14(Object *obj) {
    Object *p = obj->other;

    fill_palette_banks_white();
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) == 0) {
        obj->state_3++;
        play_sound_at(p, p->side, 0x306);
        restore_palette_banks();
        hit_other_and_spawn_on_defeat_slot04_14(obj);
    } else if (tick_countdown_spawn_random_effect_slot04_14(obj) != 0) {
        if (obj->mode_flag_2 == 0) {
            obj->sound_index = 0;
            obj->field_bf = 0xff;
            add_table_bcd_score_to_player(4, (s8)obj->side);
            add_table_bcd_to_player_total(4, obj->side);
        }
        p->round_count++;
    }
}

void update_sequence_or_call_finish_on_flag_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void hit_other_and_spawn_on_defeat_slot04_14(Object *obj) {
    Object *p = obj->other;

    p->field_15b = 1;
    set_other_hit_reaction_state_8(obj, 0, 3, 0x1f, 0x1c, 0, 0);
    if ((s16)p->health < 0) {
        obj->field_167 = 0xf;
        game_state.spawn_kind = 6;
        spawn_game_state_kind_object(obj);
    }
}

void call_table_function_by_state_3_15_slot04_14(Object *obj) {
    data_801c63c4_slot04_14[obj->state_3](obj);
}

void start_sequence_3f_decrement_remaining_slot04_14(Object *obj) {
    obj->field_157 = 0;
    obj->active_flag_3 = 0;
    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    set_sequence_by_side_and_mode(obj, 0x3f);
}

void update_sequence_or_reset_state_on_end_slot04_14(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            game_state.state_param = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_16_slot04_14(Object *obj) {
    data_801c63cc_slot04_14[obj->state_3](obj);
}

void init_state_backward_velocity_sequence_slot04_14(Object *obj) {
    obj->active_flag_4 = 1;
    obj->active_state = 1;
    obj->state_3 = 1;
    obj->field_29c = 2;
    obj->active_flag_3 = 0;
    add_to_meter_clamped(obj, 1);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = -0x40000;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x42);
}

void move_x_update_sequence_or_finish_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_17_slot04_14(Object *obj) {
    data_801c63d4_slot04_14[obj->state_3](obj);
}

void call_table_function_by_state_3_18_slot04_14(Object *obj) {
    data_801c6420_slot04_14[obj->state_3](obj);
}

void func_801b44e4_slot04_14(Object *obj) {
    int one = 1;
    int a;

    obj->active_flag_4 = one;
    obj->state_3 = one;
    obj->field_157 = 0;
    add_to_meter_clamped(obj, 1);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x6c;
        obj->field_225 = one;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void load_velocities_from_table_on_step_flag_slot04_14(Object *obj) {
    int k;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
        k = obj->direction_index << 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = data_801c646c_slot04_14[(u8)k];
        obj->accel_x = data_801c646c_slot04_14[(u8)(k | 1)];
        obj->vel_y = data_801c646c_slot04_14[(u8)(k + 2)];
        obj->accel_y = data_801c646c_slot04_14[(u8)(k + 3)];
    }
}

void move_with_accel_advance_state_3_on_step_flag_slot04_14(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->pos_y = obj->pos_y - 0x28;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->countdown = (u8)obj->countdown | 0xa00;
    }
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac -= obj->vel_x;
    }
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}
