/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c5b98_slot04_0f[];
extern ObjectFn data_801c5ba0_slot04_0f[];
extern ObjectFn data_801c5bb4_slot04_0f[];
extern ObjectFn data_801c5bbc_slot04_0f[];
extern ObjectFn data_801c5bc4_slot04_0f[];
extern ObjectFn data_801c5bd4_slot04_0f[];

void set_sequence_by_side(Object *object, int index);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void set_other_hit_reaction_state_8(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int unused);
void spawn_two_effects_at_other_ground(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void update_sequence_by_pressed_bytes(Object *object);
int drain_gauge_by_other_input(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void apply_vertical_motion_clamp_ground_slot04_0f(Object *obj);
void call_table_function_by_state_3_18_slot04_0f(Object *obj);
void move_with_facing_flipped_x_slot04_0f(Object *obj);

void update_multi_hit_step_slot04_0f(Object *obj) {
    u16 t;
    int m;
    int i;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        m = -0x100;
        obj->step_flags = t & m;
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown < 0) {
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side_and_mode(obj, 0x53);
        } else {
            spawn_two_effects_at_other_ground(obj);
            i = *(u8 *)&obj->countdown * 2;
            if (obj->direction_index == 4) {
                i += 2;
            }
            apply_damage_to_other_return_defeated(obj, (s16)(*(u16 *)((u8 *)data_801c5b98_slot04_0f + i) | m), 0);
            play_side_sound_at(obj, obj->side, 0xb);
            play_sound_at(obj, obj->side, 0x319);
        }
    }
}

void start_hit_launch_on_other_slot04_0f(Object *obj) {
    Object *o;
    int one;

    obj->state_3 = obj->state_3 + 1;
    spawn_two_effects_at_other_ground(obj);
    one = 1;
    o = obj->other;
    o->field_15b = one;
    set_other_hit_reaction_state_8(obj, 0, 5, 0x1f, 0, 0, 0);
    obj->vel_x = 0x24000;
    obj->vel_y = -0x60000;
    obj->accel_x = -0x500;
    obj->active_state = one;
    obj->accel_y = 0x3000;
    if ((s16)o->health < 0) {
        play_sound_at(obj, obj->side, 0x319);
        obj->field_167 = (obj->direction_index >> 1) + 0xc;
        game_state.config->countdown = 4;
        spawn_game_state_kind_object(obj);
    } else {
        play_sound_at(obj, obj->side, 0x319);
    }
}

void update_fall_land_on_ground_y_slot04_0f(Object *obj) {
    move_with_facing_flipped_x_slot04_0f(obj);
    if (obj->ground_y <= obj->pos_y) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        obj->pos_y_frac = 0;
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_finish_4_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_15_slot04_0f(Object *obj) {
    data_801c5ba0_slot04_0f[obj->state_3](obj);
}

void call_effect_on_step_flag_then_update_slot04_0f(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}

void start_horizontal_slide_on_sequence_end_slot04_0f(Object *obj) {
    s32 a = 0xc0000;
    s32 b;

    if ((obj->step_flags << 16) < 0) {
        obj->state_3 = obj->state_3 + 1;
        b = -0x10000;
        obj->vel_y = 0;
        obj->accel_y = 0;
        if (obj->facing_side == 0) {
            a = -0xc0000;
            b = 0x10000;
        }
        obj->accel_x = b;
        obj->vel_x = a;
        set_sequence_by_side_and_mode(obj, 0x46);
    } else {
        update_sequence(obj);
    }
}

void play_sound_324_on_step_flag_byte_slot04_0f(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        play_sound_at(obj, obj->side, 0x324);
    }
    update_sequence(obj);
}

void slide_x_until_velocity_zero_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x38);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_finish_5_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_16_slot04_0f(Object *obj) {
    data_801c5bb4_slot04_0f[obj->state_3](obj);
}

void advance_state_decrement_remaining_start_sequence_slot04_0f(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x63);
}

void update_sequence_or_finish_6_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_17_slot04_0f(Object *obj) {
    data_801c5bbc_slot04_0f[obj->state_3](obj);
}

void start_fall_with_countdown_decrement_remaining_slot04_0f(Object *obj) {
    obj->active_flag_4 = 1;
    obj->vel_x = 0;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->accel_y = -0x200;
    obj->state_3++;
    apply_vertical_motion_clamp_ground_slot04_0f(obj);
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    ((Slot04bObj *)obj)->countdown = 0x78;
    set_sequence_by_side_and_mode(obj, 0x64);
}

void update_countdown_then_set_states_slot04_0f(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;

    apply_vertical_motion_clamp_ground_slot04_0f(obj);
    o->countdown--;
    if (o->countdown == 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 3;
        obj->state_3 = 1;
        obj->accel_y = -0x3000;
        set_sequence_by_side(obj, 0x21);
    } else {
        update_sequence(obj);
    }
}

void apply_vertical_motion_clamp_ground_slot04_0f(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    t = obj->ground_y;
    if (obj->pos_y >= t) {
        obj->pos_y = t;
        obj->pos_y_frac = 0;
    }
}

void call_handler_by_state_3_unless_direction_slot04_0f(Object *obj) {
    if (obj->direction_index != 2) {
        call_table_function_by_state_3_18_slot04_0f(obj);
    } else {
        data_801c5bc4_slot04_0f[obj->state_3](obj);
    }
}

void call_handler_by_state_3_unless_direction_2_slot04_0f(Object *obj) {
    if (obj->direction_index != 2) {
        call_table_function_by_state_3_18_slot04_0f(obj);
    } else {
        data_801c5bd4_slot04_0f[obj->state_3](obj);
    }
}

void advance_state_add_meter_set_gauge_start_sequence_slot04_0f(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->gauge = 0xb4;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->field_336 = 0;
    set_sequence_by_side_and_mode(obj, 0x18);
}

void update_hit_step_with_damage_slot04_0f(Object *obj) {
    Object *o;
    u16 t;
    int k;

    o = obj->other;
    ref_other.p = o;
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        spawn_effect_at_object_offset(obj, 1, -0x2e, 0x49);
        k = 0;
        if (((Slot04bObj *)obj)->field_336 == 0) {
            ((Slot04bObj *)obj)->field_336 = 0xff;
            k = 10;
        }
        if (apply_damage_to_other_return_defeated(obj, k, 0) != 0) {
            set_other_hit_reaction_state_0(obj, 1, 5, -0x200, 0, 0, 0);
            play_sound_at(o, o->side, 0x333);
            goto tail;
        }
        obj->countdown = obj->countdown + 1;
        play_sound_at(o, o->side, 0x302);
    }
    update_sequence_by_pressed_bytes(obj);
    if ((s16)obj->countdown != 0 && (o->field_15b == 0 || drain_gauge_by_other_input(obj) != 0)) {
        spawn_effect_at_object_offset(obj, 1, -0x2e, 0x49);
        set_other_hit_reaction_state_0(obj, 1, 5, 0, 0, 0, 1);
tail:
        obj->state_3 = obj->state_3 + 1;
        play_sound_at(o, o->side, 0x306);
        set_sequence_by_side_and_mode(obj, 0x54);
    } else {
        update_sequence(obj);
    }
}
