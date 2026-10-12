/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c6324_slot04_14[];
extern ObjectFn data_801c6334_slot04_14[];
extern s32 data_801c6350_slot04_14[];
extern s32 data_801c6354_slot04_14[];
extern u8 data_801c6368_slot04_14[];
extern ObjectFn data_801c636c_slot04_14[];
int apply_velocity_and_check_ground(Object *object);
void set_sequence_by_side(Object *object, int index);
void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void spawn_child_object_with_offsets(Object *object, int a, int b);
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void land_set_ground_states_and_sequence_slot04_14(Object *object);

void update_airborne_motion_or_land_slot04_14(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->active_flag_3 = 0;
            obj->state_3++;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        land_set_ground_states_and_sequence_slot04_14(obj);
    }
}

void move_airborne_or_land_slot04_14(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        land_set_ground_states_and_sequence_slot04_14(obj);
    } else {
        update_sequence(obj);
    }
}

void land_set_ground_states_and_sequence_slot04_14(Object *obj) {
    obj->active_flag_3 = 0;
    obj->active_state = 0;
    obj->pos_y = obj->ground_y;
    set_object_states_0_to_3_slot04_14(obj, 1, 0, 3, 2);
    set_sequence_by_side(obj, 0x11);
}

void call_table_function_by_state_3_9_slot04_14(Object *obj) {
    data_801c6324_slot04_14[obj->state_3](obj);
}

void init_countdown_with_effects_and_sequence_slot04_14(Object *obj) {
    obj->state_3++;
    obj->countdown = *(u8 *)&obj->countdown | 0x3200;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x28);
}

void update_sequence_spawn_effects_on_flag_byte_slot04_14(Object *obj) {
    obj->countdown -= 0x100;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0x1e, 0x30);
        spawn_child_object_with_offsets(obj, -0x2c, 0x30);
    }
    update_sequence(obj);
}

void update_countdown_then_start_next_sequence_slot04_14(Object *obj) {
    int a;

    obj->countdown = (s16)obj->countdown - 0x100;
    if ((s16)obj->countdown < 0) {
        a = 0;
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            a = 6;
            obj->other->field_6b = 0xa;
        }
        obj->other_lock = a;
        set_sequence_by_side_and_mode(obj, 0x46);
    } else {
        update_sequence(obj);
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 5 instruction slots. */
void update_sequence_spawn_linked_child_slot04_14(Object *obj) {
    Object *p;
    s16 t;
    u16 y;

    update_sequence(obj);
    t = obj->step_flags;
    if (!(t & 0x8000)) {
        if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->table_index = 6;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->vel_table_index = (obj->direction_index >> 1) + 9;
            p->health = obj->direction_index + 3;
            p->active_flag_2 = 1;
            p->facing_side = obj->facing_side;
            p->field_0e = obj->field_0e;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            y = obj->ground_y;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->ground_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0xb);
        }
        }
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void update_sequence_effect_on_flag_or_finish_slot04_14(Object *obj) {
    s16 t;

    t = obj->step_flags;
    if (t < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        spawn_offset_effect_and_play_sounds(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_10_slot04_14(Object *obj) {
    data_801c6334_slot04_14[obj->state_3](obj);
}

void start_forward_motion_and_direction_sequence_slot04_14(Object *obj) {
    *(s32 *)&obj->vel_x = 0x80000;
    obj->accel_x = -0x8000;
    obj->state_3++;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x2a);
}

void update_sequence_spawn_pooled_set_on_flag_slot04_14(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x12, 0x30);
    }
    update_sequence(obj);
}

void update_sliding_motion_with_step_flags_slot04_14(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    u8 i = 0;
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        obj->vel_x = data_801c6350_slot04_14[obj->direction_index];
        obj->vel_y = data_801c6354_slot04_14[obj->direction_index];
    } else {
        if ((t & 0xff) == 2) {
            obj->step_flags = t & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if (*(u8 *)&obj->step_flags == 0) {
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->direction_index >> 1) + 1;
                }
                obj->other_lock = data_801c6368_slot04_14[i];
            } else {
                if (obj->facing_side != 0) {
                    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
                } else {
                    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
                }
                obj->vel_x = obj->vel_x + obj->accel_x;
                if (obj->vel_x < 0) {
                    obj->accel_x = 0;
                    obj->vel_x = 0;
                }
            }
        }
    }
}

void update_move_until_vel_x_negative_slot04_14(Object *obj) {
    if (obj->vel_x >= 0) {
        if (*(u8 *)&obj->step_flags == 0) {
            obj->active_state = 1;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
            obj->vel_y = obj->vel_y + obj->accel_y;
        }
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_vertical_motion_advance_at_negative_vel_y_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_fall_until_landing_or_step_flag_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}

void finish_sequence_or_set_other_state_5_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_11_slot04_14(Object *obj) {
    data_801c636c_slot04_14[obj->state_3](obj);
}
