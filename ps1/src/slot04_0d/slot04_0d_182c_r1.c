/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a28_slot04_0d[];
extern ObjectFn data_801c2a38_slot04_0d[];
extern ObjectFn data_801c2a40_slot04_0d[];
extern s32 data_801c2774_slot04_0d[];

void spawn_offset_effect_and_play_sounds(Object *object);
void push_other_by_vel_x(Object *object);
void func_80142c70(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void start_action_if_buttons_pressed(Object *object);
void func_801b3b6c_slot04_0d(Object *object);
void tick_countdown_or_save_counter_slot04_0d(Object *object);
void apply_gravity_vertical_motion_slot04_0d(Object *object);

void update_sequence_spawn_effects_on_flag_slot04_0d(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state == 0) {
            a = 1;
            b = 0x49;
        } else {
            a = -5;
            b = 0x44;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        func_801b3b6c_slot04_0d(obj);
    }
    update_sequence(obj);
}

void count_down_countdown_high_byte_slot04_0d(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void update_counter_and_countdown_low_byte_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->counter++;
    update_sequence(o);
    o->countdown = t = (o->countdown & 0xff00) | ((o->countdown & 0xff) - 1);
    if ((t & 0xff) != 0) {
        tick_countdown_or_save_counter_slot04_0d(o);
    } else {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    }
}

void tick_countdown_or_save_counter_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->saved_count == 0) {
            obj->saved_count = obj->counter;
        }
    } else if (*(u8 *)&o->countdown != 0) {
        (*(u8 *)&o->countdown)--;
    }
    push_other_by_vel_x(o);
}

void func_801b1a64_slot04_0d(Object *obj) {
    data_801c2a28_slot04_0d[obj->state_3](obj);
}

void func_801b1aa4_slot04_0d(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x2f);
    }
}

void launch_arc_on_mode_flag_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if (obj->mode != 0) {
        o->active_state = 1;
        o->state_3++;
        o->vel_x = 0x18000;
        o->vel_y = 0xfff88000;
        o->accel_y = 0x6000;
    }
}

void update_airborne_until_landing_fixed_vel_slot04_0d(Object *obj) {
    int d;

    apply_gravity_vertical_motion_slot04_0d(obj);
    if (obj->ground_y > obj->pos_y) {
        d = obj->vel_x;
        if (obj->facing_side == 0) {
            d = -d;
        }
        *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x26);
    }
}

void update_sequence_or_finish_notify_other_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1c5c_slot04_0d(Object *obj) {
    data_801c2a38_slot04_0d[obj->state_3](obj);
}

void func_801b1c9c_slot04_0d(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x30);
    }
}

void update_sequence_or_finish_sequence_2_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b1d4c_slot04_0d(Object *obj) {
    data_801c2a40_slot04_0d[obj->state_3](obj);
}

void start_dash_sequence_by_direction_index_slot04_0d(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 7);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->facing_side != 0) {
        obj->vel_x = 0x60000;
    } else {
        obj->vel_x = -0x60000;
    }
    a = 0x23;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}

void move_or_launch_from_table_by_mode_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *t;
    s32 *p;
    u8 g;

    t = data_801c2774_slot04_0d;
    update_sequence(o);
    g = obj->mode;
    if (g & 0x80) {
        *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    } else if (g != 0) {
        o->active_state = 1;
        o->step_flags = o->step_flags & 0xff00;
        o->state_3++;
        p = t + o->direction_index * 2;
        o->vel_x = p[0];
        o->vel_y = p[1];
        o->accel_x = p[2];
        o->accel_y = p[3];
        o->vel_y = -o->vel_y;
        o->accel_y = -o->accel_y;
    }
}

void func_801b1ef8_slot04_0d(Object *obj) {
    int d;

    apply_gravity_vertical_motion_slot04_0d(obj);
    if (obj->ground_y <= obj->pos_y) {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x26);
    } else {
        if (obj->facing_side != 0) {
            d = obj->vel_x;
        } else {
            d = -obj->vel_x;
        }
        *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
        update_sequence(obj);
    }
}

void update_sequence_or_finish_set_other_code_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
