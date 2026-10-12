/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void spawn_two_effects_at_other_ground(Object *object);
void start_action_if_buttons_pressed(Object *object);
void load_velocities_from_table_slot04_16(Object *obj);
void call_table_function_by_state_3_16_slot04_16(Object *obj);
void check_range_then_enter_hit_or_fallback_slot04_16(Object *obj);
extern ObjectFn data_801ca218_slot04_16[];
extern u16 data_801ca23c_slot04_16[];
extern s32 data_801ca244_slot04_16[];
extern s16 data_801ca250_slot04_16[];
extern u8 data_801ca270_slot04_16[];
extern s32 data_801ca278_slot04_16[];
extern ObjectFn data_801ca2a8_slot04_16[];
extern s32 data_801ca2d0_slot04_16[];
extern s32 data_801ca300_slot04_16[];
extern s32 data_801ca330_slot04_16[];
extern s16 data_801ca33c_slot04_16[];
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void move_x_decelerate_to_zero_slot04_16(Object *obj);
extern u8 data_801ca34c_slot04_16[];
extern s32 data_801ca354_slot04_16[];

void update_fall_land_with_sound_variant_a_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 0;
        obj->state_3++;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x1f);
    }
}

void update_or_end_with_other_state_code_5_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        game_state.other_object = obj->other;
        game_state.other_object->other_state_code = 5;
        load_velocities_from_table_slot04_16(obj);
    }
}

void call_table_function_by_state_3_15_slot04_16(Object *obj) {
    data_801ca218_slot04_16[obj->state_3](obj);
}

void start_direction_sequence_add_meter_check_range_slot04_16(Object *obj) {
    obj->active_flag_4 = 1;
    game_state.other_object = obj->other;
    game_state.other_object->other_state_code = 0;
    obj->state_3++;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x39);
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(game_state.config, obj);
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    check_range_then_enter_hit_or_fallback_slot04_16(obj);
}

void check_range_then_enter_hit_or_fallback_slot04_16(Object *obj) {
    s16 t = *(u16 *)((u8 *)data_801ca23c_slot04_16 + (obj->direction_index & 0xfe));
    t -= 0x26;
    if ((u8)try_force_other_state_if_box_in_range(obj, -0x26, t) == 0) {
        obj->field_15a = 9;
        call_table_function_by_state_3_16_slot04_16(obj);
    } else {
        obj->state_3++;
        add_to_meter_clamped(obj, 0x14);
        add_one_to_ec4_if_object_unflagged(game_state.config, obj);
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        set_sequence_by_side_and_mode(obj, 0x22);
    }
}

void update_hit_and_shift_by_step_flag_table_slot04_16(Object *obj) {
    u16 tbl[8] = { 4, 5, 6, 0, 6, 8, 9, 0 };
    int d;
    int k;
    u8 i;
    int m;
    u16 *p;
    u8 st;

    update_sequence(obj);
    st = obj->step_flags;
    p = tbl;
    if (st != 0) {
        if (st == 1) {
            m = -0x100;
            i = obj->direction_index >> 1;
            k = (obj->field_49 != 0) << 2;
            i += k;
            apply_damage_to_other_return_defeated(obj, (s16)(p[i] | m), 0);
            obj->step_flags &= m;
            game_state.config->setup_value = 0x3c;
            play_sound_at(obj, obj->side, 0x319);
            spawn_two_effects_at_other_ground(obj);
        } else if (st == 2) {
            obj->state_3++;
            obj->step_flags &= 0xff00;
            d = 0x60;
            if (obj->facing_side != 0) {
                d = -0x60;
            }
            obj->pos_x -= d;
        }
    }
}

void launch_from_table_on_step_flag_3_slot04_16(Object *obj) {
    s32 tbl[12] = { 0x24000, 0x90000, 0, -0x8000, 0x24000, 0xa0000, 0, -0x8400, 0x24000, 0xb0000, 0, -0x8800 };

    update_sequence(obj);
    if ((u8)obj->step_flags == 3) {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        obj->vel_x = tbl[obj->direction_index * 2];
        obj->vel_y = tbl[obj->direction_index * 2 + 1];
        obj->accel_y = tbl[obj->direction_index * 2 + 3];
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void start_fall_sequence_when_vel_y_negative_slot04_16(Object *obj) {
    apply_velocity_and_check_ground(obj);
    if ((s32)obj->vel_y < 0) {
        s32 v = data_801ca244_slot04_16[obj->direction_index >> 1];
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = v;
        set_sequence_by_side_and_mode(obj, 0x23);
    }
}

void update_airborne_until_landing_with_damage_a_slot04_16(Object *obj) {
    int k;
    u32 i;

    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    obj->active_state = 0;
    obj->pos_y = (u16)obj->ground_y;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    game_state.config->setup_value = (obj->direction_index << 2) + 0x3c;
    spawn_two_effects_at_other_ground(obj);
    set_sequence_by_side_and_mode(obj, 0x24);
    i = obj->direction_index >> 1;
    k = obj->field_49 != 0;
    if ((u8)apply_damage_to_other_return_defeated(obj, data_801ca250_slot04_16[i + (k << 2)], 0xf) != 0) {
        obj->field_167 = 2;
        if (obj->field_49 == 0) {
            return;
        }
        obj->field_167 = 0x20;
        obj->field_255 = 6;
        game_state.config->countdown = 0;
        spawn_game_state_kind_object(obj);
    }
    play_sound_at(obj, obj->side, 0x319);
}

void start_table_hit_launch_on_step_flag_1_slot04_16(Object *obj) {
    u8 t;
    s32 a;

    update_sequence(obj);
    t = obj->step_flags;
    if (t != 0) {
        if (t == 1) {
            obj->state_3 = obj->state_3 + 1;
            obj->active_state = 1;
            obj->step_flags = obj->step_flags & 0xff00;
            ref_other.p = obj->other;
            ref_other.p->field_15b = 1;
            set_other_hit_reaction_state_0(obj, data_801ca270_slot04_16[obj->direction_index & 0xfe], 0xf, -0x200, 0, 0, 0);
            ref_other.p = obj->other;
            if ((s16)ref_other.p->health < 0) {
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x20;
                    obj->field_255 = 6;
                }
            }
            obj->vel_x = data_801ca278_slot04_16[obj->direction_index * 2];
            obj->vel_y = data_801ca278_slot04_16[obj->direction_index * 2 + 1];
            obj->accel_y = data_801ca278_slot04_16[obj->direction_index * 2 + 3];
            obj->accel_x = 0;
            if (obj->facing_side == 0) {
                obj->vel_x = -obj->vel_x;
            }
        }
    }
}

void update_fall_land_with_sound_variant_b_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x25);
    }
}

void update_or_end_flipping_facing_side_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->facing_side = obj->facing_side ^ 1;
        load_velocities_from_table_slot04_16(obj);
    }
}

void call_table_function_by_state_3_16_slot04_16(Object *obj) {
    data_801ca2a8_slot04_16[obj->state_3](obj);
}

void increment_state_3_slot04_16(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
}

void set_vel_x_accel_x_and_countdown_on_flag_slot04_16(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = data_801ca2d0_slot04_16[obj->direction_index * 2];
        obj->accel_x = data_801ca2d0_slot04_16[obj->direction_index * 2 + 1];
        obj->countdown = 0x10;
    }
}

void slide_and_check_range_or_countdown_slot04_16(Object *obj) {
    move_x_decelerate_to_zero_slot04_16(obj);
    if ((u8)try_force_other_state_if_box_in_range(obj, -0x26, 0x1a) == 0) {
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown == 0) {
            obj->state_3 = 9;
            set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3c);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 3;
        add_to_meter_clamped(obj, 0xc);
        add_one_to_ec4_if_object_unflagged((GameState *)game_state.config, obj);
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        set_sequence_by_side_and_mode(obj, 0x26);
    }
}

void launch_from_table_on_step_flag_1_slot04_16(Object *obj) {
    s32 a;
    s32 b;

    if (*(u8 *)&obj->step_flags != 1) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        a = data_801ca300_slot04_16[obj->direction_index * 2];
        obj->vel_y = data_801ca300_slot04_16[obj->direction_index * 2 + 1];
        b = data_801ca300_slot04_16[obj->direction_index * 2 + 2];
        obj->accel_y = data_801ca300_slot04_16[obj->direction_index * 2 + 3];
        if (obj->facing_side == 0) {
            a = -a;
            b = -b;
        }
        obj->vel_x = a;
        obj->accel_x = b;
    }
}

void move_set_accel_y_when_vel_y_negative_slot04_16(Object *obj) {
    apply_velocity_and_check_ground(obj);
    if ((s32)obj->vel_y < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = data_801ca330_slot04_16[obj->direction_index >> 1];
    }
    update_sequence(obj);
}

void update_airborne_until_landing_with_damage_b_slot04_16(Object *obj) {
    int k;
    u32 i;

    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    obj->active_state = 0;
    obj->pos_y = (u16)obj->ground_y;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    game_state.config->setup_value = (obj->direction_index << 2) + 0x3c;
    spawn_two_effects_at_other_ground(obj);
    set_sequence_by_side_and_mode(obj, 0x27);
    i = obj->direction_index >> 1;
    k = obj->field_49 != 0;
    if ((u8)apply_damage_to_other_return_defeated(obj, data_801ca33c_slot04_16[i + (k << 2)], 0xf) != 0) {
        obj->field_167 = 2;
        if (obj->field_49 != 0) {
            obj->field_167 = 0x20;
            obj->field_255 = 6;
            game_state.config->countdown = 0;
            spawn_game_state_kind_object(obj);
        }
    }
    play_sound_at(obj, obj->side, 0x319);
}

void start_table_launch_hit_unless_flag_zero_slot04_16(Object *obj) {
    s32 a;

    if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        set_other_hit_reaction_state_0(obj, data_801ca34c_slot04_16[obj->direction_index & 0xfe], 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if (((Slot04bObj *)ref_other.p)->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        a = data_801ca354_slot04_16[obj->direction_index * 2];
        obj->vel_y = data_801ca354_slot04_16[obj->direction_index * 2 + 1];
        obj->accel_y = data_801ca354_slot04_16[obj->direction_index * 2 + 3];
        obj->accel_x = 0;
        if (obj->facing_side != 0) {
            a = -a;
        }
        obj->vel_x = a;
    }
}

