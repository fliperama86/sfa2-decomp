/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c30d4_slot04_15[];
extern ObjectFn data_801c30e4_slot04_15[];
extern ObjectFn data_801c30f8_slot04_15[];
extern s32 data_801c2df8_slot04_15[];
extern s32 data_801c2e58_slot04_15[];
extern u8 data_801c2e88_slot04_15[];
void start_action_if_buttons_pressed(Object *object);
void update_meter_by_mode_or_table(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);

void start_dash_sequence_by_direction_index_slot04_15(Object *obj) {
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

void move_or_launch_from_table_by_mode_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *t;
    s32 *p;
    u8 g;

    t = data_801c2df8_slot04_15;
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

void apply_gravity_vertical_motion_slot04_15(Object *obj);

void update_airborne_with_x_accel_until_landing_slot04_15(Object *obj) {
    int d;

    apply_gravity_vertical_motion_slot04_15(obj);
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

void update_sequence_or_finish_set_other_code_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void apply_gravity_vertical_motion_slot04_15(Object *obj) {
    s16 t = ((s16 *)&obj->vel_y)[1];

    if (t > 0) {
        obj->accel_y = 0x5000;
    }
    if (obj->field_49 != 0 && t > 0) {
        obj->accel_y = 0x10000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}

void call_table_function_by_state_3_10_slot04_15(Object *obj) {
    data_801c30d4_slot04_15[obj->state_3](obj);
}

void start_sequence_from_direction_index_add_meter_slot04_15(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->field_157 = 0;
    a = 0x43;
    if (obj->field_49 == 0) {
        a = 0x31;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}

void launch_from_table_on_step_flag_slot04_15(Object *obj) {
    s32 *t = data_801c2e58_slot04_15;
    s32 *p;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
        p = t + obj->direction_index * 2;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = p[0];
        obj->vel_y = p[1];
        obj->accel_x = p[2];
        obj->accel_y = p[3];
        obj->vel_y = -obj->vel_y;
        obj->accel_y = -obj->accel_y;
    }
}

void update_airborne_arc_until_landing_sequence_slot04_15(Object *obj) {
    s32 a;

    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->ground_y <= obj->pos_y) {
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->state_3++;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x34);
    } else {
        if (obj->facing_side != 0) {
            a = obj->vel_x;
        } else {
            a = -obj->vel_x;
        }
        *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
        update_sequence(obj);
    }
}

void update_sequence_or_finish_set_other_code_2_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_11_slot04_15(Object *obj) {
    data_801c30e4_slot04_15[obj->state_3](obj);
}

void advance_state_3_set_countdown_start_sequence_slot04_15(Object *obj) {
    obj->state_3++;
    *(u8 *)&obj->countdown = 0x32;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x3e);
}

void count_down_update_sequence_spawn_on_flag_slot04_15(Object *obj) {
    int a;

    (*(u8 *)&obj->countdown)--;
    if (*(u8 *)&obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0x11, 0x40);
    }
    update_sequence(obj);
}

void count_down_then_set_other_lock_from_table_slot04_15(Object *obj) {
    u8 a = 0;

    *(u8 *)&obj->countdown -= 1;
    if (*(u8 *)&obj->countdown & 0x80) {
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c2e88_slot04_15[a];
        set_sequence_by_side_and_mode(obj, 0x35);
    } else {
        update_sequence(obj);
    }
}

void spawn_projectile_child_table_index_2_slot04_15(Object *obj) {
    Object *p;
    s8 t;
    s32 w;
    int one;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            one = 1;
            p->active_count = one;
            p->field_02 = 0x1f;
            p->table_index = 2;
            p->sound_side = obj->sound_side;
            p->field_4b = obj->field_4b;
            p->team_id = obj->team_id;
            t = obj->direction_index;
            p->active_flag_2 = one;
            p->vel_table_index = t + 6;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            w = obj->data_ptr_b;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->data_ptr_b = w;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->health = (t >> 1) + 2;
            p->parent = obj;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x14);
        }
    }
}

void update_sequence_or_finish_sequence_3_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_12_slot04_15(Object *obj) {
    data_801c30f8_slot04_15[obj->state_3](obj);
}

void start_slide_sequence_by_direction_index_slot04_15(Object *obj) {
    obj->other_state_code = 2;
    *(s32 *)&obj->vel_x = 0x80000;
    obj->accel_x = -0x8000;
    obj->state_3++;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x38);
}

void set_other_state_code_spawn_children_on_flag_slot04_15(Object *obj) {
    int a;

    obj->other_state_code = 2;
    if (*(u8 *)&obj->step_flags != 0) {
        a = 1;
        obj->state_3++;
        if (obj->field_4b == 0) {
            a = -1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0xf, 0x44);
    }
    update_sequence(obj);
}
