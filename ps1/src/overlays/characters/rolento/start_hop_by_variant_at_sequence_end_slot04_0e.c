/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c6064_slot04_0e[];
extern ObjectFn data_801c606c_slot04_0e[];
extern ObjectFn data_801c6074_slot04_0e[];

int apply_velocity_and_check_ground(Object *object);
void update_meter_by_mode_or_table(Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);

void start_hop_by_variant_at_sequence_end_slot04_0e(Object *obj) {
    int a;
    int x;
    int y;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->vel_y = 0x50000;
        obj->accel_y = -0x5000;
        obj->state_3++;
        if (obj->variant_index == 1) {
            a = 0x37;
            x = -0x38000;
            y = 0x500;
        } else {
            a = 0x38;
            x = 0x40000;
            y = -0x500;
        }
        if (obj->facing_side != 0) {
            x = -x;
            y = -y;
        }
        obj->vel_x = x;
        obj->accel_x = y;
        set_sequence_by_side_and_mode(obj, a);
        play_side_sound_at(obj, obj->side, 9);
    }
}

void update_airborne_until_ground_then_land_sequence_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((u8)apply_velocity_and_check_ground(o) == 0) {
        o->pos_y_frac = 0;
        o->active_state = 0;
        o->active_flag_3 = 0;
        o->active_flag_4 = 0;
        o->state_3++;
        o->pos_y = obj->ground_y;
        play_object_sound_by_kind(o);
        set_sequence_by_side_and_mode(o, 0x39);
    } else {
        update_sequence(o);
    }
}

void update_sequence_or_finish_sequence_2_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_9_slot04_0e(Object *obj) {
    data_801c6064_slot04_0e[obj->state_3](obj);
}

void enter_next_state_add_meter_set_sequence_by_field_slot04_0e(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->mode_flag_2 != 0) {
        obj->variant_index = obj->direction_flag;
    }
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x61;
    if (obj->field_49 == 0) {
        a = 0x3c;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void update_or_launch_by_variant_bit_slot04_0e(Object *obj) {
    int a;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 3;
        obj->state_3 = 1;
        obj->active_state = 1;
        obj->field_157 = 0;
        obj->active_flag_4 = 0;
        if (obj->variant_index & 0x80) {
            obj->vel_x = 0x78000;
            obj->vel_y = 0x74000;
            obj->accel_x = -0x500;
            obj->accel_y = -0x5a00;
            a = 0x3f;
        } else {
            obj->vel_x = -0x78000;
            obj->vel_y = 0x70000;
            obj->accel_x = 0x500;
            obj->accel_y = -0x5a00;
            a = 0x3e;
        }
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        set_sequence_by_side_and_mode(obj, a);
    }
}

void call_table_function_by_state_3_10_slot04_0e(Object *obj) {
    data_801c606c_slot04_0e[obj->state_3](obj);
}

void advance_state_apply_meter_seq_fifty_slot04_0e(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x32);
}

void update_sequence_check_mode_spawn_effect_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if (obj->mode == 0) {
        o->field_165 = 0;
        reset_object_to_state_1_0_sequence_0(o);
    } else if (obj->mode != 2) {
        obj->mode = 2;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, 0xe, 0x50);
    }
}

void call_table_function_by_state_3_11_slot04_0e(Object *obj) {
    data_801c6074_slot04_0e[obj->state_3](obj);
}

void advance_state_spawn_seq_fifty_one_slot04_0e(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (((Slot04bObj *)obj)->field_1da == 0) {
        spawn_three_child_objects_at_object_spawn(obj);
    }
    set_sequence_by_side_and_mode(obj, 0x33);
}
