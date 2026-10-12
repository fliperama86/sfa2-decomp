/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801cdfd0_slot04_17[];
extern u8 data_801ce32c_slot04_17[];
extern u8 data_801ce330_slot04_17[];
extern ObjectFn data_801ce338_slot04_17[];
extern s32 data_801ce348_slot04_17[];
extern ObjectFn data_801ce354_slot04_17[];
extern u8 data_801ce378_slot04_17[];
extern u8 data_801ce380_slot04_17[];

void start_action_if_buttons_pressed(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void set_fall_accel_y_start_sequence_slot04_17(Object *obj);
void set_sequence_from_table_plus_step_limit_slot04_17(Object *obj);
int apply_facing_velocity_with_x_stop_slot04_17(Object *obj);
int apply_velocity_and_acceleration_slot04_17(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_slot04_17(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_slot04_17(Object *obj);
Object *get_table_object_by_field_slot04_17(Object *obj);

void func_801b1c88_slot04_17(Object *o) {
    u8 t;

    if (apply_facing_velocity_with_x_stop_slot04_17(o) < 0 || ((Slot04bObj *)o)->field_47 == 0) {
        set_fall_accel_y_start_sequence_slot04_17(o);
    } else {
        ((Slot04bObj *)o)->field_47--;
        ((Slot04bObj *)o)->countdown++;
        if (o->active_flag != 0) {
            if (o->mode_flag_2 != 0 ? (((Slot04bObj *)o)->field_47 < 6 &&
                                    (test_random_bit_in_table(o, (u32 *)data_801cdfd0_slot04_17) & 0xff) == 0)
                                 : ((o->pressed_bits & data_801ce32c_slot04_17[o->direction_index >> 1]) != 0)) {
                o->step_limit++;
                t = ((Slot04bObj *)o)->countdown;
                if (t != 0) {
                    t--;
                }
                ((Slot04bObj *)o)->field_47 = t;
                play_side_sound_at(o, o->side, 6);
                ((Slot04bObj *)o)->countdown = 0;
                o->active_flag = 0;
                if (o->step_limit == o->step_count_2) {
                    o->accel_x = -0x2000;
                    o->accel_y = -0x5000;
                    o->state_3++;
                }
                o->vel_x = 0xc000;
                o->vel_y = 0x19000;
                set_sequence_from_table_plus_step_limit_slot04_17(o);
            }
        }
        update_sequence(o);
    }
}

void move_and_set_fall_accel_when_vel_y_negative_slot04_17(Object *obj) {
    int a;

    if (apply_facing_velocity_with_x_stop_slot04_17(obj) < 0) {
        a = -0x7000;
        obj->state_3++;
        if (obj->field_49 != 0) {
            a = -0x15000;
        }
        obj->accel_y = a;
    }
    update_sequence(obj);
}

void update_motion_land_with_direction_sequence_slot04_17(Object *obj) {
    apply_facing_velocity_with_x_stop_slot04_17(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3d);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_set_other_state_code_5_2_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_17(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void set_sequence_from_table_plus_step_limit_slot04_17(Object *obj) {
    u16 t = *(u16 *)(data_801ce330_slot04_17 + (obj->direction_index & 0xfe));

    t = obj->step_limit + t;
    set_sequence_by_side_and_mode(obj, t);
}

void call_table_function_by_state_3_3_slot04_17(Object *obj) {
    data_801ce338_slot04_17[obj->state_3](obj);
}

void start_direction_sequence_restore_facing_slot04_17(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->field_29c = 2;
    obj->state_3++;
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->facing_side = obj->saved_facing;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x3a;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}

void set_vel_x_from_table_on_step_flag_slot04_17(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->vel_x = data_801ce348_slot04_17[obj->direction_index >> 1];
    }
    update_sequence(obj);
}

void move_by_vel_x_until_step_flags_clear_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 d = o->vel_x;

    if (o->facing_side == 0) {
        d = -d;
    }
    *(s32 *)&o->pos_x_frac += d;
    if (*(u8 *)&o->step_flags == 0) {
        o->accel_x = -0xc000;
        obj->field_47 = 3;
        o->field_29c = 0;
        o->state_3++;
    }
    update_sequence(o);
}

void update_slide_with_decelerating_vel_x_slot04_17(Object *o) {
    s32 d;
    u16 t;

    if (((Slot04bObj *)o)->field_47 != 0) {
        ((Slot04bObj *)o)->field_47--;
        if (((Slot04bObj *)o)->field_47 == 0) {
            ((Slot04bObj *)o)->field_47 = 3;
            spawn_random_offset_object_variant_zero(o);
        }
    }
    if ((o->step_flags << 16) < 0) {
        ref_other.p = o->other;
        ref_other.p->other_state_code = 5;
        o->active_flag_4 = 0;
        call_reset_object_to_state_1_0_0_2_sequence_1_slot04_17(o);
    } else {
        d = o->vel_x;
        if (o->facing_side == 0) {
            d = -d;
        }
        *(s32 *)&o->pos_x_frac += d;
        o->vel_x += o->accel_x;
        if (o->vel_x < 0) {
            o->vel_x = 0;
            o->accel_x = 0;
            ((Slot04bObj *)o)->field_47 = 0;
        }
        t = o->step_flags;
        if ((t & 0xff) != 0) {
            o->step_flags = t & 0xff00;
            o->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(o);
        update_sequence(o);
    }
}

void call_table_function_by_state_3_4_slot04_17(Object *obj) {
    data_801ce354_slot04_17[obj->state_3](obj);
}

void add_meter_start_sequence_by_field_flag_slot04_17(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x24;
    if (obj->field_49 != 0) {
        a = 0x3d;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void start_jump_toward_target_object_slot04_17(Object *obj) {
    u16 t;
    s32 d;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        t = obj->input_bits_2;
        obj->active_state = 1;
        obj->state_3++;
        obj->variant_index = 0;
        obj->active_flag = 0;
        if (obj->mode_flag_2 != 0) {
            t = get_random_byte();
        }
        if (t & 0x8000) {
            obj->facing_side ^= 1;
        }
        d = *(s32 *)&get_table_object_by_field_slot04_17(obj)->pos_x_frac;
        if (obj->facing_side == 0) {
            d += 0x1800000;
        }
        d -= *(s32 *)&obj->pos_x_frac;
        d /= *(u16 *)(data_801ce378_slot04_17 + (obj->direction_index & 0xfe));
        if (obj->field_49 != 0) {
            d <<= 2;
        }
        obj->vel_y = 0xa5000;
        obj->vel_x = d;
        obj->accel_x = 0;
        obj->accel_y = -0x8000;
    }
}

void update_air_land_or_wall_stick_slot04_17(Object *obj) {
    int r = apply_velocity_and_acceleration_slot04_17(obj);

    if (r < 0 && obj->pos_y >= obj->ground_y) {
        play_object_sound_by_kind(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->state_3 = 8;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        set_sequence_by_side_and_mode(obj, 0x37);
    } else if (obj->pos_y < obj->ground_y - 0x48 && (data_801ce380_slot04_17[obj->facing_side] & obj->bound_push_flag)) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->vel_y = 0;
        obj->accel_y = 0;
        set_sequence_by_side_and_mode(obj, 0x25);
    } else {
        update_sequence(obj);
    }
}
