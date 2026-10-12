/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2ac8_slot04_0d[];
extern ObjectFn data_801c2adc_slot04_0d[];

void update_sequence_by_pressed_bytes(Object *object);
void set_position_from_other_box(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
int drain_gauge_by_other_input(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void set_sequence_by_side(Object *object, int arg);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void flip_facing_launch_and_hit_other_slot04_0d(Object *obj, s16 a);
void release_or_drain_gauge_then_advance_slot04_0d(Object *obj);
void apply_velocity_with_gravity_slot04_0d(Object *obj);

void update_hold_sequence_damage_other_on_flag_slot04_0d(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;
    u16 t;
    int k;

    update_sequence_by_pressed_bytes(obj);
    update_sequence(obj);
    set_position_from_other_box(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        obj->active_state = 1;
        play_side_sound_at(obj, obj->side, 0x15);
        spawn_effect_at_object_offset(obj, 1, 0, 0x4e);
        k = 0;
        if (o->field_1cd == 0) {
            o->field_1cd = 0xff;
            k = 6;
        }
        if ((u8)apply_damage_to_other_return_defeated(obj, k, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x16);
            flip_facing_launch_and_hit_other_slot04_0d(obj, -0x200);
        } else {
            o->countdown = o->countdown + 1;
            release_or_drain_gauge_then_advance_slot04_0d(obj);
        }
    } else {
        release_or_drain_gauge_then_advance_slot04_0d(obj);
    }
}

void release_or_drain_gauge_then_advance_slot04_0d(Object *obj) {
    if (*(u8 *)&obj->countdown != 0) {
        if (obj->other->field_15b == 0) {
            flip_facing_launch_and_hit_other_slot04_0d(obj, 2);
        } else if ((u8)drain_gauge_by_other_input(obj) != 0) {
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side_and_mode(obj, 0x19);
            set_position_from_other_box(obj);
        }
    }
}

void flip_facing_launch_and_hit_other_slot04_0d(Object *obj, s16 a) {
    int x = 0x40000;
    int t;

    obj->state_3 = 6;
    t = obj->facing_side ^ 1;
    obj->facing_side = t;
    if (t != 0) {
        x = -0x40000;
    }
    obj->vel_x = x;
    obj->vel_y = -0x40000;
    obj->accel_y = 0x6000;
    set_other_hit_reaction_state_0(obj, 0x1e, 0xf, a, 0, 0, 0);
    set_sequence_by_side(obj, 0x14);
}

void follow_other_box_then_spawn_hit_effect_slot04_0d(Object *obj) {
    Object *o;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
        set_position_from_other_box(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        spawn_effect_at_object_offset(obj, 1, 0x18, 0x47);
        o = obj->other;
        ref_other.p = o;
        play_sound_at(o, ref_other.p->side, 0x306);
        set_sequence_by_side_and_mode(obj, 0x1a);
    }
}

void update_sequence_stop_and_hit_other_on_flag_slot04_0d(Object *obj) {
    Object *o;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->vel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0x6000;
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0x1e, 0xf, 1, 0, 1, 1);
        o = obj->other;
        if (o->field_15b != 0) {
            o->field_243 = 0;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void fall_until_ground_then_land_sequence_a_slot04_0d(Object *obj) {
    apply_velocity_with_gravity_slot04_0d(obj);
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x1b);
    }
}

void update_sequence_or_flip_facing_finish_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void update_airborne_sequence_land_on_ground_slot04_0d(Object *obj) {
    s16 y;

    apply_velocity_with_gravity_slot04_0d(obj);
    if (((Slot04bObj *)obj)->saved_pos_x <= 0 || obj->pos_y < (y = obj->ground_y)) {
        update_sequence(obj);
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->active_state = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_16_slot04_0d(Object *obj) {
    data_801c2ac8_slot04_0d[obj->state_3](obj);
}

void start_state_sound_input_facing_snap_to_other_box_slot04_0d(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x20);
    set_position_from_other_box(obj);
}

void update_sequence_launch_other_on_flag_slot04_0d(Object *obj) {
    Object *o;
    int x;

    update_sequence(obj);
    set_position_from_other_box(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        spawn_effect_at_object_offset(obj, 2, -0x49, 0x30);
        x = 0x36000;
        if (obj->facing_side != 0) {
            x = -0x36000;
        }
        obj->vel_y = -0x40000;
        obj->vel_x = x;
        obj->accel_y = 0x6000;
        set_other_hit_reaction_state_0(obj, 0x1d, 5, 0xf, 0, 0, 0);
        o = obj->other;
        ref_other.p = o;
        if ((s16)obj->health >= 0) {
            play_sound_at(o, ref_other.p->side, 0x30f);
        } else {
            play_sound_at(o, ref_other.p->side, 0x346);
        }
    }
}

void fall_until_near_ground_then_start_sequence_slot04_0d(Object *obj) {
    apply_velocity_with_gravity_slot04_0d(obj);
    if (((Slot04bObj *)obj)->saved_pos_x < 0 || obj->pos_y < (s16)(obj->ground_y - 0x50)) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x21);
    }
}

void fall_until_ground_then_land_sequence_b_slot04_0d(Object *obj) {
    apply_velocity_with_gravity_slot04_0d(obj);
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x22);
    }
}

void update_sequence_or_finish_sequence_5_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void apply_velocity_with_gravity_slot04_0d(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void load_velocities_from_table_4_slot04_0d(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void load_velocities_from_table_5_slot04_0d(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_handler_by_state_2_2_slot04_0d(Object *obj) {
    data_801c2adc_slot04_0d[obj->state_2](obj);
}
