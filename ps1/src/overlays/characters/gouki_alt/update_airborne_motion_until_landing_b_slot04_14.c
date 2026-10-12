/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c649c_slot04_14[];
extern u16 data_801c64a8_slot04_14[];
extern u16 data_801c64ac_slot04_14[];
extern u8 data_801c64b4_slot04_14[];
extern u8 data_801c64bc_slot04_14[];

int try_set_other_state_if_in_range(Object *object, s16 a, s16 b, s16 c, u16 d);
void set_position_from_other_box(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void choose_air_attack_branch_by_button_bits_slot04_14(Object *obj);
void choose_air_attack_branch_by_distance_slot04_14(Object *obj);
void enter_state_6_launch_from_other_box_slot04_14(Object *obj);
void set_state_3_and_directional_sequence_slot04_14(Object *obj);
void enter_state_12_launch_from_other_box_slot04_14(Object *obj);
void set_state_3_9_direction_sequence_slot04_14(Object *obj);
void nudge_other_x_by_table_every_fourth_tick_slot04_14(Object *obj);

void update_airborne_motion_until_landing_b_slot04_14(Object *obj) {
    u16 a;

    update_sequence(obj);
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->ground_y) {
        obj->field_157 = 1;
        obj->active_flag_3 = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        a = obj->direction_index >> 1;
        obj->accel_x = data_801c649c_slot04_14[a];
        if (obj->field_49 != 0) {
            a += 0x6d;
        } else {
            a += 0x4d;
        }
        set_sequence_by_side_and_mode(obj, a);
        play_sound_at(obj, obj->side, 0x324);
    } else if (obj->mode_flag_2 != 0) {
        choose_air_attack_branch_by_distance_slot04_14(obj);
    } else {
        choose_air_attack_branch_by_button_bits_slot04_14(obj);
    }
}

void choose_air_attack_branch_by_button_bits_slot04_14(Object *obj) {
    if (obj->vel_y < 0) {
        if ((obj->pressed_bits | obj->released_bits) & 0x95) {
            if (obj->other->bound_push_flag == 0 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x54, 0x18)) {
                enter_state_6_launch_from_other_box_slot04_14(obj);
                return;
            }
            set_state_3_and_directional_sequence_slot04_14(obj);
        }
        if (obj->vel_y < 0) {
            if ((obj->pressed_bits | obj->released_bits) & 0x6a) {
                if (obj->other->bound_push_flag == 0 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x30, 0x18)) {
                    enter_state_12_launch_from_other_box_slot04_14(obj);
                } else {
                    set_state_3_9_direction_sequence_slot04_14(obj);
                }
            }
        }
    }
}

void choose_air_attack_branch_by_distance_slot04_14(Object *obj) {
    s16 d;

    if (obj->vel_y < 0 && obj->input_dir_second == 0) {
        if (obj->other->bound_push_flag == 0) {
            d = obj->pos_x - obj->other->pos_x;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x19 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x54, 0x18)) {
                enter_state_6_launch_from_other_box_slot04_14(obj);
                return;
            }
            obj->countdown = obj->countdown - 0x100;
            set_state_3_and_directional_sequence_slot04_14(obj);
        }
        if (obj->vel_y < 0 && obj->input_dir_second == 0 && obj->other->bound_push_flag == 0) {
            d = obj->pos_x - obj->other->pos_x;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x19 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x30, 0x18)) {
                enter_state_12_launch_from_other_box_slot04_14(obj);
            } else {
                obj->countdown = obj->countdown - 0x100;
                set_state_3_9_direction_sequence_slot04_14(obj);
            }
        }
    }
}

void enter_state_6_launch_from_other_box_slot04_14(Object *obj) {
    Object *p;

    obj->state_3 = 6;
    obj->vel_y = 0x40000;
    obj->accel_y = -0x5800;
    obj->vel_x = -0x20000;
    p = obj->other;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x20000;
    }
    obj->accel_x = 0;
    add_to_meter_clamped(obj, 8);
    play_sound_at(p, p->side, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x4a);
    set_position_from_other_box(obj);
}

void set_state_3_and_directional_sequence_slot04_14(Object *obj) {
    obj->state_3 = 0x11;
    obj->active_flag_3 = 1;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x5a);
}

void enter_state_12_launch_from_other_box_slot04_14(Object *obj) {
    Object *p;

    obj->vel_y = 0x58000;
    obj->accel_y = -0x1800;
    obj->vel_x = -0x20000;
    p = obj->other;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x20000;
    }
    obj->accel_x = 0;
    obj->state_3 = 0xc;
    add_to_meter_clamped(obj, 8);
    play_sound_at(p, p->side, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x57);
    set_position_from_other_box(obj);
}

void set_state_3_9_direction_sequence_slot04_14(Object *obj) {
    obj->state_3 = 9;
    obj->active_flag_3 = 1;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x52);
}

void slide_with_decaying_vel_x_slot04_14(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_flag_4 = 0;
        obj->step_timer = 1;
        set_sequence_by_side_and_mode(obj, 0x50);
    } else {
        if ((u8)obj->step_flags == 0) {
            update_sequence(obj);
        }
        nudge_other_x_by_table_every_fourth_tick_slot04_14(obj);
    }
}

void nudge_other_x_by_table_every_fourth_tick_slot04_14(Object *obj) {
    int i;

    if ((game_state.tick_count & 3) == 0) {
        i = 1;
        if ((u8)obj->step_flags != 0) {
            i = 2;
        }
        for (; i >= 0; i--) {
            u16 a;

            if (!spawn_random_offset_object_variant_zero(obj)) {
                break;
            }
            if ((u8)obj->step_flags != 0) {
                a = data_801c64ac_slot04_14[i];
            } else {
                a = data_801c64a8_slot04_14[i];
            }
            if (obj->facing_side != 0) {
                a = -a;
            }
            ref_other.p->pos_x = a + ref_other.p->pos_x;
        }
    }
}

void update_sequence_or_call_finish_on_flag_2_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void update_arc_until_landing_then_set_sequence_slot04_14(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->state_3 = obj->state_3 + 1;
            obj->active_state = 0;
            obj->pos_y = obj->ground_y;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, 0x4b);
        }
    }
}

void update_hit_sequence_with_effects_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    update_sequence(obj);
    p = obj->other;
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_flag_4 = 0;
        p->field_15b = 1;
        set_other_hit_reaction_state_0(obj, 0, 0xf, *(s16 *)(data_801c64b4_slot04_14 + (obj->direction_index & 0xfe)), 4, 0, 1);
        set_sequence_by_side_and_mode(obj, 0x51);
        if (((Slot04bObj *)p)->field_5c < 0) {
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.spawn_kind = 0;
                spawn_game_state_kind_object(obj);
            } else {
                obj->field_167 = 2;
            }
        }
    }
    t = obj->step_flags;
    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        spawn_effect_at_object_offset(obj, 2, -0x17, 0x26);
        play_sound_at(p, p->side, 0x306);
        if (game_state.countdown_timer == 0) {
            game_state.countdown_timer = 0xa;
            game_state.countdown_timer_b = 2;
            game_state.reload_value = 2;
        }
        game_state.state_param = data_801c64bc_slot04_14[obj->direction_index >> 1];
    }
}
