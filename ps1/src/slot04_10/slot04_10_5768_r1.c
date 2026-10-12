/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6c6c_slot04_10[];
extern u8 data_801c6c9c_slot04_10[];
extern u16 data_801c6ca4_slot04_10[];
extern Slot04_10Rec6cb0 data_801c6cb0_slot04_10[];

int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_three_child_records_at_other(Object *object);
void spawn_two_effects_at_other_ground(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void func_801b5abc_slot04_10(Object *obj);
void set_vertical_motion_from_table_slot04_10(Object *obj);

void update_launch_other_hit_with_velocity_slot04_10(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        update_sequence(o);
    } else {
        o->active_state = 1;
        o->state_3++;
        o->step_flags = o->step_flags & 0xff00;
        ref_other.p = o->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(o, 0x22, 0xf, -0x200, 0, 0, 0);
        ref_other.p = o->other;
        if ((s16)ref_other.p->health < 0) {
            o->field_167 = (o->direction_index >> 1) + 0xc;
        }
        o->vel_x = 0x48000;
        o->vel_y = 0x80000;
        o->accel_y = 0xffff7800;
        o->accel_x = 0;
        if (o->facing_side != 0) {
            o->vel_x = -o->vel_x;
        }
    }
}

void update_airborne_or_land_set_state_3_to_8_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = 8;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x33);
    }
}

void call_table_function_by_state_3_18_slot04_10(Object *obj) {
    data_801c6c6c_slot04_10[obj->state_3](obj);
}

void start_sequence_29_with_effects_slot04_10(Object *obj) {
    ((Slot04bObj *)obj)->table_index_2 = 0;
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter((GameState *)game_state.config, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, 0x29);
}

void update_spawn_children_on_step_flag_high_byte_slot04_10(Object *obj) {
    int t = -1;

    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            t = 1;
        }
        obj->field_165 = t;
        spawn_children_per_direction_index(obj, -0x13, 0x53);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void release_other_when_step_flag_high_byte_clear_slot04_10(Object *obj) {
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        obj->other_lock = 0;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 0;
        set_sequence_by_side_and_mode(obj, 0x3f);
        func_801b5abc_slot04_10(obj);
    }
}

void func_801b5abc_slot04_10(Object *obj) {
    s16 t = *(u16 *)(data_801c6c9c_slot04_10 + (obj->direction_index & 0xfe));

    if ((u8)try_force_other_state_if_box_in_range(obj, -0x26, (s16)(t - 0x26)) == 0) {
        obj->state_3 = 0xa;
        update_sequence(obj);
    } else {
        obj->state_3++;
        if (obj->direction_index != 0) {
            play_side_sound_at(obj, obj->side, 8);
        } else {
            play_side_sound_at(obj, obj->side, 4);
        }
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        spawn_three_child_records_at_other(obj);
        set_sequence_by_side_and_mode(obj, 0x2a);
    }
}

void update_multi_step_attack_with_flag_codes_slot04_10(Object *o) {
    s16 t;
    u16 x;
    int one;
    int m;
    int d;

    update_sequence(o);
    t = o->step_flags;
    if (t < 0) {
        if (o->direction_index != 0 && ((Slot04bObj *)o)->table_index_2 == 0) {
            ((Slot04bObj *)o)->table_index_2 = 1;
            set_sequence_by_side_and_mode(o, 0x2b);
        } else {
            one = 1;
            o->active_state = one;
            ((Slot04bObj *)o)->field_298 = 0;
            o->state_3++;
            set_sequence_by_side_and_mode(o, 0x2c);
            ((Slot04bObj *)o)->table_index_2 = 0;
            if (o->direction_index == 4) {
                ((Slot04bObj *)o)->table_index_2 = one;
            }
            set_vertical_motion_from_table_slot04_10(o);
        }
        return;
    }
    if ((t & 0xff00) != 0) {
        if ((t & 0xff00) != 0x100) {
            o->step_flags = t & 0xff;
            if (o->direction_index == 2) {
                play_side_sound_at(o, o->side, 4);
            }
        } else {
            o->step_flags = t & 0xff;
            if (o->direction_index == 0) {
                play_side_sound_at(o, o->side, 5);
            }
        }
    }
    x = o->step_flags;
    if ((x & 0xff) == 0) {
        return;
    }
    if ((x & 0xff) == 1) {
        m = -0x100;
        apply_damage_to_other_return_defeated(o, (s16)(*(u16 *)((u8 *)data_801c6ca4_slot04_10 + ((((Slot04bObj *)o)->table_index_2 + o->direction_index) << 1)) | m), 0);
        o->step_flags = o->step_flags & m;
        game_state.config->setup_value = 0x3c;
        play_sound_at(o, o->side, 0x319);
        if (o->direction_index != 0) {
            if (((Slot04bObj *)o)->table_index_2 == 0) {
                if (o->direction_index != 4) {
                    return;
                }
                play_side_sound_at(o, o->side, 4);
            } else {
                play_side_sound_at(o, o->side, 5);
            }
        }
        spawn_two_effects_at_other_ground(o);
    } else if ((x & 0xff) == 2) {
        d = 0x58;
        o->step_flags = x & 0xff00;
        if (o->facing_side != 0) {
            d = -0x58;
        }
        o->pos_x = o->pos_x - d;
    } else if ((x & 0xff) == 3) {
        o->step_flags = x & 0xff00;
        if (o->direction_index == 0) {
            set_sequence_by_side_and_mode(o, 0x2b);
        }
    }
}

void set_vertical_motion_from_table_slot04_10(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[4];

    obj->vel_y = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->table_index_2].a;
    obj->accel_y = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->table_index_2].b;
    obj->field_f4 = data_801c6cb0_slot04_10[((Slot04bObj *)obj)->table_index_2].c;
    obj->vel_x = 0;
    obj->accel_x = 0;
}

void update_rise_then_set_accel_y_from_field_slot04_10(Object *obj) {
    apply_velocity_and_check_ground(obj);
    if (obj->vel_y >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->accel_y = obj->field_f4;
        update_sequence(obj);
    }
}

void update_fall_land_choose_sequence_by_direction_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else if (obj->direction_index == 4 && ((Slot04bObj *)obj)->table_index_2 != 2) {
        obj->state_3 = 0xb;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff00;
        set_sequence_by_side_and_mode(obj, 0x2d);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        set_sequence_by_side_and_mode(obj, 0x2e);
    }
}
