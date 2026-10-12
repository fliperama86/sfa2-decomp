/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6ab0_slot04_10[];
extern s16 data_801c6abc_slot04_10[];
extern u8 data_801c6adc_slot04_10[];
extern s32 data_801c6ae4_slot04_10[];
extern ObjectFn data_801c6b14_slot04_10[];
extern s32 data_801c6b3c_slot04_10[];
extern s32 data_801c6b6c_slot04_10[];
extern s32 data_801c6b9c_slot04_10[];
extern s16 data_801c6ba8_slot04_10[];

int apply_velocity_and_check_ground(Object *object);
void start_action_if_buttons_pressed(Object *object);
void spawn_two_effects_at_other_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void load_velocities_from_table_slot04_10(Object *obj);
void move_x_decelerate_to_zero_slot04_10(Object *obj);

void start_fall_sequence_when_vel_y_negative_slot04_10(Object *obj) {
    apply_velocity_and_check_ground(obj);
    if ((s32)obj->vel_y < 0) {
        s32 v = data_801c6ab0_slot04_10[obj->direction_index >> 1];
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = v;
        set_sequence_by_side_and_mode(obj, 0x23);
    }
}

void update_airborne_until_landing_with_damage_a_slot04_10(Object *obj) {
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
    if ((u8)apply_damage_to_other_return_defeated(obj, data_801c6abc_slot04_10[i + (k << 2)], 0xf) != 0) {
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

void start_table_hit_launch_on_step_flag_1_slot04_10(Object *obj) {
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
            set_other_hit_reaction_state_0(obj, data_801c6adc_slot04_10[obj->direction_index & 0xfe], 0xf, -0x200, 0, 0, 0);
            ref_other.p = obj->other;
            if ((s16)ref_other.p->health < 0) {
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x20;
                    obj->field_255 = 6;
                }
            }
            obj->vel_x = data_801c6ae4_slot04_10[obj->direction_index * 2];
            obj->vel_y = data_801c6ae4_slot04_10[obj->direction_index * 2 + 1];
            obj->accel_y = data_801c6ae4_slot04_10[obj->direction_index * 2 + 3];
            obj->accel_x = 0;
            if (obj->facing_side == 0) {
                obj->vel_x = -obj->vel_x;
            }
        }
    }
}

void update_fall_land_with_sound_variant_b_slot04_10(Object *obj) {
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

void update_or_end_flipping_facing_side_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->facing_side = obj->facing_side ^ 1;
        load_velocities_from_table_slot04_10(obj);
    }
}

void call_table_function_by_state_3_16_slot04_10(Object *obj) {
    data_801c6b14_slot04_10[obj->state_3](obj);
}

void increment_state_3_slot04_10(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
}

void set_vel_x_accel_x_and_countdown_on_flag_slot04_10(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = data_801c6b3c_slot04_10[obj->direction_index * 2];
        obj->accel_x = data_801c6b3c_slot04_10[obj->direction_index * 2 + 1];
        obj->countdown = 0x10;
    }
}

void slide_and_check_range_or_countdown_slot04_10(Object *obj) {
    move_x_decelerate_to_zero_slot04_10(obj);
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

void launch_from_table_on_step_flag_1_slot04_10(Object *obj) {
    s32 a;
    s32 b;

    if (*(u8 *)&obj->step_flags != 1) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        a = data_801c6b6c_slot04_10[obj->direction_index * 2];
        obj->vel_y = data_801c6b6c_slot04_10[obj->direction_index * 2 + 1];
        b = data_801c6b6c_slot04_10[obj->direction_index * 2 + 2];
        obj->accel_y = data_801c6b6c_slot04_10[obj->direction_index * 2 + 3];
        if (obj->facing_side == 0) {
            a = -a;
            b = -b;
        }
        obj->vel_x = a;
        obj->accel_x = b;
    }
}

void move_set_accel_y_when_vel_y_negative_slot04_10(Object *obj) {
    apply_velocity_and_check_ground(obj);
    if ((s32)obj->vel_y < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = data_801c6b9c_slot04_10[obj->direction_index >> 1];
    }
    update_sequence(obj);
}

void update_airborne_until_landing_with_damage_b_slot04_10(Object *obj) {
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
    if ((u8)apply_damage_to_other_return_defeated(obj, data_801c6ba8_slot04_10[i + (k << 2)], 0xf) != 0) {
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
