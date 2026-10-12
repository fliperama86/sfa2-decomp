/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801ce4c4_slot04_17[];
extern s16 data_801ce4f4_slot04_17[];
extern ObjectFn data_801ce504_slot04_17[];
extern u8 data_801ce530_slot04_17[];
extern u8 data_801ce531_slot04_17[];
extern s32 data_801ce538_slot04_17[];
extern u8 data_801ce578_slot04_17[];
extern ObjectRef data_80190468;

void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_two_effects_at_other_ground(Object *object);
void set_position_from_other_box(Object *object);
int check_range_set_other_state_if_boxed(Object *object, s16 a, s16 b, s16 c, u16 d);
void spawn_children_per_direction_index(Object *object, int a, int b);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void call_reset_object_to_state_1_0_sequence_0_slot04_17(Object *obj);
int apply_facing_velocity_with_x_stop_slot04_17(Object *obj);
int apply_velocity_and_acceleration_slot04_17(Object *obj);
void latch_pressed_bits_into_input_bits_slot04_17(Object *obj);
u8 get_random_table_byte_slot04_17(Object *obj);

void advance_state_on_step_flag_spawn_and_sounds_slot04_17(Object *obj) {
    int a;

    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        obj->facing_side = obj->saved_facing;
        obj->pos_x_frac = 0;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        spawn_children_per_direction_index(obj, 0xc, 0x4d);
        play_sound_at(obj, obj->side, 0x31c);
        play_side_sound_at(obj, obj->side, 0xa);
    }
}

void load_velocities_from_table_on_step_end_slot04_17(Object *obj) {
    int i;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
        }
        i = obj->direction_index;
        obj->other_lock = 0;
        obj->field_165 = 0;
        obj->vel_x = data_801ce4c4_slot04_17[i * 2];
        obj->accel_x = data_801ce4c4_slot04_17[i * 2 + 1];
        obj->vel_y = data_801ce4c4_slot04_17[i * 2 + 2];
        obj->accel_y = data_801ce4c4_slot04_17[i * 2 + 3];
        obj->active_state = 1;
    }
}

void update_move_check_hit_on_other_slot04_17(Object *obj) {
    if (apply_facing_velocity_with_x_stop_slot04_17(obj) < 0) {
        obj->state_3 = 6;
        obj->variant_index = 0xff;
        set_sequence_by_side_and_mode(obj, 0x41);
    } else {
        ref_other.p = obj->other;
        if (ref_other.p->field_163 == 0) {
            if ((u8)check_range_set_other_state_if_boxed(obj, -0x20, 0x20, 0x1a, 0x1a) != 0) {
                obj->state_3++;
                play_sound_at(obj, obj->other->side, 0x31a);
                play_side_sound_at(obj, obj->side, 0xd);
                set_sequence_by_side_and_mode(obj, obj->direction_index + 0x43);
            }
        }
        update_sequence(obj);
    }
}

void update_move_flip_facing_at_ground_offset_slot04_17(Object *obj) {
    if ((u8)obj->step_flags == 0 && apply_facing_velocity_with_x_stop_slot04_17(obj) < 0) {
        obj->accel_y = -0x3800;
        if (obj->ground_y - 8 <= obj->pos_y) {
            obj->state_3++;
            obj->pos_y_frac = 0;
            obj->pos_y = (u16)((Slot04bObj *)obj)->ground_y - 8;
            obj->facing_side = obj->facing_side ^ 1;
            set_sequence_by_side_and_mode(obj, obj->direction_index + 0x44);
            return;
        }
    }
    update_sequence(obj);
}

void update_hit_sequence_with_launch_on_flag_slot04_17(Object *obj) {
    int k = obj->step_flags & 0x7f;
    int j;
    int a;

    if (k != 0) {
        data_80190468.p->vec_table_index = 0x20;
        obj->step_flags = obj->step_flags & 0xff80;
        spawn_two_effects_at_other_ground(obj);
        play_side_sound_at(obj, obj->side, 8);
        j = k - 1;
        if (apply_damage_to_other_return_defeated(obj, data_801ce4f4_slot04_17[j * 2], data_801ce4f4_slot04_17[j * 2 + 1]) != 0) {
            play_sound_at(obj, obj->side, 0x319);
            data_80190468.p->field_6b = 4;
            spawn_game_state_kind_object(obj);
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
    }
    if (obj->step_flags & 0x80) {
        obj->state_3++;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        set_other_hit_reaction_state_0(obj, 0, 0xa, -0x200, 0, 0, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        a = 0x40000;
        if (obj->facing_side != 0) {
            a = -0x40000;
        }
        obj->vel_y = 0x60000;
        obj->vel_x = a;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
        obj->variant_index = 0;
    } else {
        update_sequence(obj);
        set_position_from_other_box(obj);
    }
}

void update_fall_by_variant_and_land_with_sound_slot04_17(Object *obj) {
    int r;

    if (obj->variant_index == 0) {
        r = apply_velocity_and_acceleration_slot04_17(obj);
    } else {
        r = apply_facing_velocity_with_x_stop_slot04_17(obj);
    }
    if (r < 0 && obj->pos_y >= obj->ground_y) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        set_sequence_by_side_and_mode(obj, 0x42);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_end_with_other_code_5_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_8_slot04_17(Object *obj) {
    data_801ce504_slot04_17[obj->state_3](obj);
}

void advance_state_3_and_start_direction_sequence_slot04_17(Object *obj) {
    obj->state_3++;
    obj->field_225 = 1;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->field_49 == 0) {
        spawn_three_child_objects_at_object_spawn(obj);
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x49);
}

void advance_state_on_step_flag_spawn_sound_slot04_17(Object *obj) {
    int a;

    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        spawn_children_per_direction_index(obj, -0x35, 0x37);
        play_sound_at(obj, obj->side, 0x31c);
    }
}

void load_motion_from_tables_on_sequence_end_slot04_17(Object *obj) {
    int i;
    int a;
    int b;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        i = 6;
        obj->state_3++;
        obj->step_count_2 = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            i = obj->direction_index;
        }
        obj->step_limit = data_801ce530_slot04_17[i];
        obj->other_lock = data_801ce531_slot04_17[i];
        a = data_801ce538_slot04_17[i * 2];
        b = data_801ce538_slot04_17[i * 2 + 1];
        obj->vel_y = data_801ce538_slot04_17[i * 2 + 2];
        obj->accel_y = data_801ce538_slot04_17[i * 2 + 3];
        if (obj->facing_side == 0) {
            a = -a;
            b = -b;
        }
        obj->vel_x = a;
        obj->accel_x = b;
        *(u16 *)&((Slot04bObj *)obj)->input_bits = 0;
    }
}

void update_airborne_land_choose_sequence_slot04_17(Object *obj) {
    Config *c;

    latch_pressed_bits_into_input_bits_slot04_17(obj);
    apply_velocity_and_acceleration_slot04_17(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        c = game_state.config;
        if ((c->mode_flag_b | c->mode_flag_c | c->mode_flag_a) == 0 && (u8)is_other_object_state_0_and_1_six_one(obj) == 0) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x4c);
        } else {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x6f);
        }
    } else {
        update_sequence(obj);
    }
}

void update_choose_jump_direction_by_input_slot04_17(Object *o) {
    u16 a;
    u8 t = 1;
    s32 d;

    latch_pressed_bits_into_input_bits_slot04_17(o);
    if ((o->step_flags << 16) < 0) {
        a = *(u16 *)&((Slot04bObj *)o)->input_bits;
        o->active_state = t;
        o->facing_side ^= 1;
        if (o->mode_flag_2 != 0) {
            a = get_random_table_byte_slot04_17(o);
        }
        if (a != 0) {
            o->facing_side = t;
            if ((a & 0x40) == 0) {
                o->facing_side = 0;
                if ((a & 8) == 0) {
                    o->state_3 += 3;
                    *(u16 *)&((Slot04bObj *)o)->input_bits = 0;
                    ref_other.p = o->other;
                    o->vel_x = (*(s32 *)&ref_other.p->pos_x_frac - *(s32 *)&o->pos_x_frac) / 26;
                    o->accel_x = 0;
                    o->vel_y = 0xa0000;
                    o->accel_y = -0x6000;
                    d = (u32)~o->vel_x >> 31;
                    o->facing_side = d;
                    set_sequence_by_side_and_mode(o, 0x4f);
                    return;
                }
            }
        }
        d = 0x90000;
        *(u16 *)&((Slot04bObj *)o)->input_bits = 0;
        o->state_3++;
        if (o->facing_side != 0) {
            d = -0x90000;
        }
        o->vel_y = 0xa0000;
        o->vel_x = d;
        o->accel_x = 0;
        o->accel_y = -0x6000;
        set_sequence_by_side_and_mode(o, 0x4d);
    } else {
        update_sequence(o);
    }
}

void update_airborne_land_or_wall_contact_slot04_17(Object *obj) {
    latch_pressed_bits_into_input_bits_slot04_17(obj);
    if (apply_velocity_and_acceleration_slot04_17(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->state_3 = 0xa;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x6f);
    } else if (obj->ground_y + 0x48 >= obj->pos_y && (data_801ce578_slot04_17[obj->facing_side] & obj->bound_push_flag) != 0) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->vel_y = 0;
        obj->accel_y = 0;
        set_sequence_by_side_and_mode(obj, 0x4e);
    } else {
        update_sequence(obj);
    }
}
