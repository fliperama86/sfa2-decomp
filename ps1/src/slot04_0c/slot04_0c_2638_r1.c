/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bda6c_slot04_0c[];
extern ObjectFn data_801bda74_slot04_0c[];
extern ObjectFn data_801bda84_slot04_0c[];
extern u8 data_801bdaa4_slot04_0c[];
extern s32 data_801bdaa8_slot04_0c[];

int apply_velocity_and_check_ground(Object *object);
void func_80131638(Object *object);
void func_80142c70(Object *object);
void push_other_by_vel_x(Object *object);
void add_to_meter_clamped(Object *object, short delta);
void func_801428e4(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void func_80145d20(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void apply_velocities_or_land_slot04_0c(Object *object);

void update_countdown_state_with_counter_slot04_0c(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->counter++;
    update_sequence(o);
    o->countdown = t = (o->countdown & 0xff00) | ((o->countdown & 0xff) - 1);
    if ((t & 0xff) == 0) {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    } else {
        if (obj->field_134 != 0) {
            if (obj->saved_count == 0) {
                obj->saved_count = obj->counter;
            }
        } else if ((t & 0xff00) != 0) {
            o->countdown = t - 0x100;
        }
    }
    push_other_by_vel_x(o);
}

void func_801b2708_slot04_0c(Object *obj) {
    data_801bda6c_slot04_0c[obj->state_3](obj);
}

void func_801b2748_slot04_0c(Object *obj) {
    obj->active_flag_3 = 0;
    obj->active_flag_4 = 1;
    obj->state_3++;
    set_sequence_by_side_and_mode(obj, 0x35);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    apply_velocities_or_land_slot04_0c(obj);
}

void update_sequence_and_apply_velocity_slot04_0c(Object *obj) {
    update_sequence(obj);
    apply_velocities_or_land_slot04_0c(obj);
}

void apply_velocities_or_land_slot04_0c(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        func_80131638(obj);
    }
}

void func_801b2810_slot04_0c(Object *obj) {
    data_801bda74_slot04_0c[obj->state_3](obj);
}

void func_801b2850_slot04_0c(Object *obj) {
    obj->active_flag_4 = 1;
    obj->active_state = 1;
    obj->state_3++;
    obj->field_157 = 0;
    if (obj->facing_side == 0) {
        obj->vel_x = -0x40000;
    } else {
        obj->vel_x = 0x40000;
    }
    set_sequence_by_side_and_mode(obj, 0x2a);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
}

void finish_move_or_advance_with_vel_x_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->countdown = 0x48;
        obj->active_state = 0;
        obj->state_3++;
        obj->facing_side = obj->saved_facing;
        add_to_meter_clamped(obj, 5);
        set_sequence_by_side_and_mode(obj, 0x2c);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags != 0) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
    }
}

void func_801b2960_slot04_0c(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void start_move_forward_with_vel_x_slot04_0c(Object *obj) {
    obj->state_3 = 1;
    obj->active_state = 1;
    obj->field_157 = 0;
    if (obj->facing_side == 0) {
        obj->vel_x = 0x30000;
    } else {
        obj->vel_x = -0x30000;
    }
    set_sequence_by_side_and_mode(obj, 0x2b);
}

void func_801b29f4_slot04_0c(Object *obj) {
    data_801bda84_slot04_0c[obj->state_3](obj);
}

void func_801b2a34_slot04_0c(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    func_80145d20(obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x2d);
}

void func_801b2a98_slot04_0c(Object *o) {
    if ((u8)o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        spawn_children_per_direction_index(o, 3, 0x55);
        play_sound_at(o, o->side, 0x31c);
    }
    update_sequence(o);
}

void func_801b2b18_slot04_0c(Object *o) {    int i = 0;

    update_sequence(o);
    if ((u8)o->step_flags == 0) {
        o->state_3++;
        o->field_165 = 0;
        if (o->field_4b == 0) {
            o->other->field_6b = 0xa;
            i = (o->direction_index >> 1) + 1;
        }
        ((Slot04bObj *)o)->table_value = data_801bdaa4_slot04_0c[i];
    }
}

void advance_state_3_on_low_flag_slot04_0c(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
    }
}

void func_801b2bec_slot04_0c(Object *o) {
    u16 a;
    int v;

    update_sequence(o);
    a = o->step_flags;
    if ((a & 0xff) == 2) {
        o->step_flags = (a & 0xff00) | 1;
        if (((Slot04bObj *)o->other)->field_a7 == 0xb) {
            play_side_sound_at(o, o->side, 0x12);
            o->frame_sound_index = 0;
        }
    }
    if ((u8)o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
        v = data_801bdaa8_slot04_0c[o->direction_index >> 1];
        o->vel_y = 0x90000;
        o->accel_x = -0x8000;
        o->accel_y = -0x6000;
        o->vel_x = v;
    }
}

void move_arc_until_vel_x_negative_advance_slot04_0c(Object *obj) {
    if (obj->vel_x < 0) {
        obj->state_3++;
    } else {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac -= obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        obj->vel_x += obj->accel_x;
    }
    update_sequence(obj);
}

void fall_until_landing_or_step_flag_slot04_0c(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->state_3++;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}

void update_sequence_or_call_finish_on_flag_2_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
