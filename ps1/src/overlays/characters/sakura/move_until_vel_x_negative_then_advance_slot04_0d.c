/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c2824_slot04_0d[];
extern ObjectFn data_801c2a90_slot04_0d[];
extern ObjectFn data_801c2aa4_slot04_0d[];
extern ObjectFn data_801c2aac_slot04_0d[];

void spawn_children_per_direction_index(Object *object, int a, int b);
void set_position_from_other_box(Object *object);
void update_meter_by_mode_or_table(Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void move_x_by_facing_with_accel_slot04_0d(Object *object);
void call_table_function_by_state_3_15_slot04_0d(Object *object);
void call_table_function_by_state_3_16_slot04_0d(Object *object);

void move_until_vel_x_negative_then_advance_slot04_0d(Object *obj) {
    int v;

    obj->other_state_code = 2;
    if (obj->vel_x >= 0) {
        if ((u8)obj->step_flags == 0) {
            obj->active_state = 1;
            *(s32 *)&obj->pos_y_frac += obj->vel_y;
            obj->vel_y += obj->accel_y;
        }
        v = obj->vel_x;
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
    }
    update_sequence(obj);
}

void rise_until_vel_y_positive_slot04_0d(Object *obj) {
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y > 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_fall_landing_snap_to_ground_slot04_0d(Object *obj) {
    s16 y;

    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    y = obj->ground_y;
    if (obj->pos_y >= y) {
        obj->active_state = 0;
        obj->pos_y = y;
        obj->pos_y_frac = 0;
        obj->active_flag_3 = 0;
        obj->state_3++;
        play_object_sound_by_kind(obj);
    } else if ((u8)obj->step_flags != 0) {
        return;
    }
    update_sequence(obj);
}

void update_sequence_or_end_set_other_code_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_13_slot04_0d(Object *obj) {
    data_801c2a90_slot04_0d[obj->state_3](obj);
}

void start_effects_and_sequence_by_direction_index_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    obj->state_3++;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3b);
}

void update_sequence_then_spawn_children_on_flag_slot04_0d(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        int a = -1;

        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x1a, 0x44);
    }
}

void start_dash_on_step_end_slot04_0d(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        u8 i = 0;

        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        ((Slot04bObj *)obj)->table_value = data_801c2824_slot04_0d[i];
        obj->vel_x = 0x30000;
        obj->accel_x = 0;
    }
}

void update_sequence_set_accel_x_then_move_x_slot04_0d(Object *obj) {
    u16 a;

    update_sequence(obj);
    a = obj->step_flags;
    if ((a & 0xff) != 0) {
        obj->step_flags = a & 0xff00;
        obj->state_3++;
        obj->accel_x = -0x4000;
    }
    move_x_by_facing_with_accel_slot04_0d(obj);
}

void update_sequence_move_or_finish_notify_other_slot04_0d(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags >= 0) {
        move_x_by_facing_with_accel_slot04_0d(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void move_x_by_facing_with_accel_slot04_0d(Object *obj) {
    int v = obj->vel_x;

    if (v >= 0) {
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
}

void call_table_function_by_state_3_14_slot04_0d(Object *obj) {
    data_801c2aa4_slot04_0d[obj->state_3](obj);
}

void start_sequence_1c_decrement_remaining_count_slot04_0d(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x1c);
}

void update_sequence_or_finish_sequence_4_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_input_dir_second_2_slot04_0d(Object *obj) {
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_16_slot04_0d(obj);
    } else {
        call_table_function_by_state_3_15_slot04_0d(obj);
    }
}

void call_table_function_by_state_3_15_slot04_0d(Object *obj) {
    data_801c2aac_slot04_0d[obj->state_3](obj);
}

void start_state_gauge_sound_and_snap_to_other_box_slot04_0d(Object *obj) {
    obj->gauge = 0xb4;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->field_1cd = 0;
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x18);
    set_position_from_other_box(obj);
}
