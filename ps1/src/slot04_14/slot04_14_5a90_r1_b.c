/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6548_slot04_14[];
extern ObjectFn data_801c6554_slot04_14[];

void call_table_function_by_state_3_22_slot04_14(Object *obj);
void call_table_function_by_state_3_23_slot04_14(Object *obj);
void set_object_states_0_to_3_2_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void set_sequence_and_tables_from_direction(Object *object);

void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void call_handler_by_input_dir_0_2_slot04_14(Object *obj) {
    data_801c6548_slot04_14[obj->input_dir_0 >> 1](obj);
}

void call_handler_by_input_dir_second_2_slot04_14(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_23_slot04_14(obj);
    } else {
        call_table_function_by_state_3_22_slot04_14(obj);
    }
}

void call_table_function_by_state_3_22_slot04_14(Object *obj) {
    data_801c6554_slot04_14[obj->state_3](obj);
}

void start_state_choose_hit_or_jump_sequence_slot04_14(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            set_object_states_0_to_3_2_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
    }
    if (obj->direction_index == 2 && obj->attack_param_d != 0) {
        obj->active_flag_3 = 1;
        obj->state_3 = 2;
        add_to_meter_clamped(obj, 1);
        set_sequence_by_side_and_mode(obj, 0x27);
        return;
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
