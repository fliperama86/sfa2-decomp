/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c6378_slot04_14[];
extern s16 data_801c6390_slot04_14[];
extern ObjectFn data_801c6398_slot04_14[];
extern ObjectFn data_801c63a8_slot04_14[];

void set_sequence_by_side(Object *object, int arg);
void call_side_func_then_queue_job(Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_three_child_objects(Object *object);
void update_meter_by_mode_or_table(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_child_object_with_offsets(Object *parent, int x, int y);
int apply_velocity_and_check_ground(Object *object);
void move_x_until_slow_then_reset_slot04_14(Object *obj);
void func_801b3654_slot04_14(Object *obj);

void start_slide_with_velocity_and_spawn_children_slot04_14(Object *obj) {
    int n;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->field_29c = 2;
    obj->field_252 = 0xff;
    if (obj->mode_flag_2 != 0) {
        obj->variant_index = obj->facing_side ^ obj->direction_flag;
    }
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->accel_x = -0x3000;
    obj->vel_x = 0xb8000;
    if (obj->input_dir_second != 0) {
        obj->vel_x = 0x80000;
    }
    n = 0x2d;
    if (obj->field_49 != 0) {
        n = 0x64;
        obj->field_252 = 0;
    }
    set_sequence_by_side_and_mode(obj, n);
    if (obj->field_7e == 0) {
        spawn_three_child_objects(obj);
        func_801b3654_slot04_14(obj);
    }
}

void update_sequence_with_signed_x_step_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        play_side_sound_at(obj, obj->side, 0xc);
        move_x_until_slow_then_reset_slot04_14(obj);
    } else {
        obj->pos_x = obj->pos_x + (s8)*(u8 *)&obj->step_flags;
        update_sequence(obj);
    }
}

void move_x_until_slow_then_reset_slot04_14(Object *obj) {
    Object *other = obj->other;
    int t;

    if (obj->variant_index != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    obj->facing_side = obj->saved_facing;
    if (((obj->bound_push_flag >> obj->variant_index) & 1) == 0) {
        t = 0x30000;
        if (obj->input_dir_second != 0) {
            t = 0x24000;
        }
        if (t < obj->vel_x) {
            update_sequence(obj);
            return;
        }
    }
    obj->facing_side = obj->saved_facing;
    other->other_state_code = 5;
    reset_object_to_state_1_0_sequence_0(obj);
    if (obj->field_7e == 0) {
        call_side_func_then_queue_job(obj);
    }
}

void func_801b3654_slot04_14(Object *obj) {
    if (obj->field_49 == 0) {
        store_image_rows_by_mode_and_queue_job(obj, 1);
    }
}

void call_table_function_by_state_3_12_slot04_14(Object *obj) {
    data_801c6378_slot04_14[obj->state_3](obj);
}

void init_state_3_set_sequence_3c_by_direction_slot04_14(Object *obj) {
    obj->field_225 = 1;
    obj->state_3 = 1;
    if (obj->field_7e == 0) {
        spawn_three_child_objects_at_object_spawn(obj);
    }
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3c);
}

void update_sequence_spawn_from_table_on_flag_slot04_14(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    Object *other = obj->other;
    s16 t;
    int i;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        ((Slot04bObj *)obj)->table_value = 1;
        obj->field_165 = 0;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_4b != 0) {
            other->field_6b = 0xa;
            ((Slot04bObj *)obj)->table_value = 0;
        }
    } else if ((t & 0xff) != 0) {
        if (obj->field_165 == 0) {
            obj->field_165 = 0xff;
            if (obj->field_4b != 0) {
                obj->field_165 = 1;
            }
        }
        play_sound_at(obj, obj->side, 0x31c);
        i = (((u8)obj->step_flags - 1) << 2) & 0x1fc;
        obj->step_flags = obj->step_flags & 0xff00;
        spawn_child_object_with_offsets(obj, *(s16 *)((u8 *)data_801c6390_slot04_14 + i), *(s16 *)((u8 *)data_801c6390_slot04_14 + i + 2));
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void spawn_pooled_child_at_step_flag_one_slot04_14(Object *obj) {
    Object *p;
    u16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 1) {
        obj->state_3 = obj->state_3 + 1;
        obj->countdown = *(u8 *)&obj->countdown | 0x800;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->table_index = 8;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->vel_table_index = (obj->direction_index >> 1) + 0xc;
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
            obj->link_flags = obj->link_flags + 1;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void update_delayed_hop_arc_and_landing_slot04_14(Object *obj) {
    s16 t = obj->countdown;
    int n;

    if ((t & 0xff00) != 0) {
        t = t - 0x100;
        obj->countdown = t;
        if ((t & 0xff00) == 0) {
            obj->vel_y = 0x74000;
            obj->accel_y = -0x5000;
            obj->vel_x = 0x10000;
            obj->accel_x = -0x280;
            if (obj->facing_side != 0) {
                obj->vel_x = -0x10000;
                obj->accel_x = 0x280;
            }
        } else {
            update_sequence(obj);
        }
    } else if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if ((s16)obj->step_flags < 0) {
            obj->active_flag_3 = 0;
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        n = 0x39;
        if (obj->variant_index == 1) {
            n = 0x38;
        }
        set_sequence_by_side_and_mode(obj, n);
    }
}

void update_airborne_until_landing_slot04_14(Object *obj) {
    int n;

    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        n = 0x39;
        if (obj->variant_index == 1) {
            n = 0x38;
        }
        set_sequence_by_side_and_mode(obj, n);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_13_slot04_14(Object *obj) {
    data_801c6398_slot04_14[obj->state_3](obj);
}

void start_sequence_with_meter_gain_four_slot04_14(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    set_sequence_by_side_and_mode(obj, obj->field_49 != 0 ? 0x68 : 0x37);
}

void advance_to_directional_sequence_at_end_slot04_14(Object *obj) {
    u16 n = 0x32;

    if ((s16)obj->step_flags < 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_49 != 0) {
            n = 0x69;
        }
        n = (obj->direction_index >> 1) + n;
        set_sequence_by_side_and_mode(obj, n);
    } else {
        update_sequence(obj);
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void spawn_child_object_table_4_on_step_flag_slot04_14(Object *obj) {
    Object *p;
    u16 y;
    int x;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->countdown = *(u8 *)&obj->countdown | 0x600;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->table_index = 4;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            x = obj->direction_index >> 1;
            p->active_flag_2 = 0;
            p->vel_table_index = x + 6;
            p->health = x;
            p->facing_side = obj->facing_side;
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
            obj->link_flags = obj->link_flags + 1;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void call_table_function_by_state_3_14_slot04_14(Object *obj) {
    data_801c63a8_slot04_14[obj->state_3](obj);
}
