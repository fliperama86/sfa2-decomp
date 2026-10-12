/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c61a0_slot04_14[];
extern ObjectFn data_801c61a8_slot04_14[];

void set_sequence_and_tables_from_direction(Object *object);
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void move_by_velocity_until_ground_then_reset_slot04_14(Object *obj);

void update_sequence_or_call_reset_state_slot04_14(Object *obj) {
    s16 t = obj->step_flags;

    if ((t & 0x8000) != 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_slot04_14(Object *obj) {
    obj->field_157 = 1;
    data_801c61a0_slot04_14[obj->state_3](obj);
}

void set_flag_3_and_state_3_call_direction_start_slot04_14(Object *obj) {
    int one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = one;
    set_sequence_and_tables_from_direction(obj);
}

void start_directional_sequence_with_special_state_slot04_14(Object *obj) {
    u16 x;

    obj->input_dir_0 = 4;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    x = 0xc;
    if (obj->variant_index != 0) {
        x = 0x12;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    x += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, x);
    if ((u16)obj->pos_y < (u16)(obj->ground_y - 0x50) && obj->variant_index != 0 && (obj->variant_index & 0x80) == 0 && obj->input_dir_second != 0 && obj->direction_index == 2 && (obj->input_bits_2 & 0x4000) != 0) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 5, 0);
        set_sequence_by_side_and_mode(obj, 0x29);
    }
}

void call_table_function_by_state_3_3_slot04_14(Object *obj) {
    data_801c61a8_slot04_14[obj->state_3](obj);
}

void set_launch_velocity_by_facing_and_move_slot04_14(Object *obj) {
    obj->vel_y = -0x4c000;
    obj->vel_x = -0x40000;
    obj->state_3 = obj->state_3 + 1;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    }
    move_by_velocity_until_ground_then_reset_slot04_14(obj);
}
