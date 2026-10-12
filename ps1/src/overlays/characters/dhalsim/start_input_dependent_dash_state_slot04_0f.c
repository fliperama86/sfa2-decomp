/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c59f8_slot04_0f[];
extern s32 data_801c5a00_slot04_0f[];
extern ObjectFn data_801c5a0c_slot04_0f[];
extern s32 data_801c5a18_slot04_0f[];

void set_sequence_and_tables_from_direction(Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);

/* functions of other units of this module */
void update_sequence_check_input_or_reset_slot04_0f(Object *obj);

void select_sequence_by_input_and_direction_a_slot04_0f(Object *obj);
void select_sequence_by_input_and_direction_b_slot04_0f(Object *obj);

void start_input_dependent_dash_state_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int i;

    o->active_flag_3 = 1;
    obj->field_337 = 0;
    o->state_3++;
    if (o->input_bits_2 & 0x2000) {
        reset_direction_and_action_fields(o);
        add_to_meter_clamped(o, o->direction_index >> 1);
        i = o->direction_index >> 2;
        o->kind_table_value = data_801c59f8_slot04_0f[i];
        o->countdown_5 = data_801c59f8_slot04_0f[i + 4];
        set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x23);
    } else {
        o->state_3 = 2;
        obj->field_337 = 0xff;
        set_sequence_and_tables_from_direction(o);
    }
}

void call_update_sequence_check_input_or_reset_5_slot04_0f(Object *obj) {
    update_sequence_check_input_or_reset_slot04_0f(obj);
}

void start_horizontal_slide_from_table_slot04_0f(Object *obj) {
    s32 x;
    s32 y = -0x10000;

    obj->state_3++;
    obj->accel_y = 0;
    obj->vel_y = 0;
    x = data_801c5a00_slot04_0f[obj->direction_index >> 1];
    if (obj->facing_side == 0) {
        x = -x;
        y = 0x10000;
    }
    obj->vel_x = x;
    obj->accel_x = y;
    update_sequence(obj);
}

void advance_state_3_on_negative_flags_with_sound_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_3++;
        play_sound_at(obj, obj->side, 0x324);
    }
    update_sequence(obj);
}

void move_x_until_velocity_zero_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x36);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_call_finish_on_flag_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void select_sequence_by_input_dir_second_slot04_0f(Object *obj) {
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    obj->input_dir_0 = 4;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->input_dir_second == 0) {
        select_sequence_by_input_and_direction_a_slot04_0f(obj);
    } else {
        select_sequence_by_input_and_direction_b_slot04_0f(obj);
    }
}

void select_sequence_by_input_and_direction_a_slot04_0f(Object *obj) {
    int a = 0xc;

    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (!(obj->input_bits_2 & 0x1000)) {
        if (obj->input_bits_2 & 0x4000) {
            if (obj->direction_index == 4) {
                if (obj->pos_y < obj->ground_y - 0x38) {
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 5;
                    obj->state_3 = 0;
                    add_to_meter_clamped(obj, 2);
                    set_sequence_by_side_and_mode(obj, 0x32);
                    return;
                }
            }
        } else if (obj->input_bits_2 & 0x2000) {
            a += 0x1a;
        }
    }
    set_sequence_by_side_and_mode(obj, a + (obj->direction_index >> 1));
    add_to_meter_clamped(obj, obj->direction_index >> 1);
}

void select_sequence_by_input_and_direction_b_slot04_0f(Object *obj) {
    int a = 0xf;

    if (obj->variant_index != 0) {
        a = 0x15;
    }
    if (!(obj->input_bits_2 & 0x1000)) {
        if (obj->input_bits_2 & 0x4000) {
            if (obj->ground_y - 0x38 >= obj->pos_y) {
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 5;
                obj->state_3 = 0;
                add_to_meter_clamped(obj, 2);
                set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x33);
                return;
            }
        } else if (obj->input_bits_2 & 0x2000) {
            a += 0x1a;
        }
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
}

void call_table_function_by_state_3_5_slot04_0f(Object *obj) {
    data_801c5a0c_slot04_0f[obj->state_3](obj);
}

void set_velocity_from_table_by_direction_slot04_0f(Object *obj) {
    int i = 0;
    s32 x;

    obj->state_3++;
    if (obj->input_dir_second != 0) {
        i = obj->direction_index;
    }
    x = data_801c5a18_slot04_0f[i];
    obj->vel_y = data_801c5a18_slot04_0f[i + 1];
    if (obj->facing_side == 0) {
        x = -x;
    }
    obj->vel_x = x;
}
