/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c5f40_slot04_0e[];
extern ObjectFn data_801c5f4c_slot04_0e[];
extern ObjectFn data_801c5f58_slot04_0e[];
extern ObjectFn data_801c5f64_slot04_0e[];
extern ObjectFn data_801c5f6c_slot04_0e[];
extern ObjectFn data_801c5f78_slot04_0e[];
extern ObjectFn data_801c5f80_slot04_0e[];

int match_pressed_direction_input(Object *object);
int try_set_direction_from_pressed_bits(Object *object);
void set_sequence_and_tables_from_direction(Object *object);
void call_handler_by_direction_index_3_slot04_0e(Object *obj);
void call_handler_by_direction_index_4_slot04_0e(Object *obj);

void update_sequence_or_finish_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_direction_index_2_slot04_0e(Object *obj) {
    data_801c5f40_slot04_0e[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_2_slot04_0e(Object *obj) {
    data_801c5f4c_slot04_0e[obj->state_3](obj);
}

void choose_direction_state_or_sequence_slot04_0e(Object *obj) {
    obj->state_3++;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x21, 7) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0) {
            obj->state_3 = 2;
            obj->active_flag_3 = 1;
            obj->field_157 = 0;
            obj->active_state = 1;
            add_to_meter_clamped(obj, 1);
            reset_direction_and_action_fields(obj);
            set_sequence_by_side_and_mode(obj, 0x1c);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_reset_state_3_on_input_b_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((((Slot04bObj *)obj)->mode & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void update_sequence_or_clear_active_state_finish_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->active_state = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_direction_handler_by_input_dir_second_slot04_0e(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_4_slot04_0e(obj);
    } else {
        call_handler_by_direction_index_3_slot04_0e(obj);
    }
}

void call_handler_by_direction_index_3_slot04_0e(Object *obj) {
    data_801c5f58_slot04_0e[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_3_slot04_0e(Object *obj) {
    data_801c5f64_slot04_0e[obj->state_3](obj);
}

void advance_state_3_and_start_sequence_slot04_0e(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_reset_state_3_on_input_b_2_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((((Slot04bObj *)obj)->mode & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_direction_index_4_slot04_0e(Object *obj) {
    data_801c5f6c_slot04_0e[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_4_slot04_0e(Object *obj) {
    data_801c5f78_slot04_0e[obj->state_3](obj);
}

void advance_state_3_and_start_sequence_2_slot04_0e(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_reset_state_3_on_input_b_3_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((((Slot04bObj *)obj)->mode & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_5_slot04_0e(Object *obj) {
    data_801c5f80_slot04_0e[obj->state_3](obj);
}

void start_sideways_motion_set_flags_slot04_0e(Object *obj) {
    int one;

    *(s32 *)&obj->vel_x = 0xa0000;
    obj->state_3++;
    *(s32 *)&obj->accel_x = 0xffff4000;
    one = 1;
    obj->active_flag_3 = one;
    if (obj->facing_side == 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    set_sequence_and_tables_from_direction(obj);
    obj->field_278 = one;
}

void advance_state_3_on_mode_with_sound_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    if (obj->mode != 0) {
        o->state_3++;
        play_sound_at(o, o->field_02, 0x324);
    }
    update_sequence(o);
}

void move_x_until_mode_clear_then_advance_slot04_0e(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (o->mode == 0) {
        obj->state_3++;
        obj->pos_x_frac = 0;
    }
    update_sequence(obj);
}

void update_sequence_or_finish_sequence_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}
