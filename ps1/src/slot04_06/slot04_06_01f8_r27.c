/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_direction_index_7_slot04_06(Object *obj);
void call_handler_by_direction_index_8_slot04_06(Object *obj);
extern ObjectFn data_801c5584_slot04_06[];
extern ObjectFn data_801c5590_slot04_06[];

void update_sequence_or_finish_sequence_4_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_direction_handler_by_input_dir_second_2_slot04_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_8_slot04_06(obj);
    } else {
        call_handler_by_direction_index_7_slot04_06(obj);
    }
}

void call_handler_by_direction_index_7_slot04_06(Object *obj) {
    data_801c5584_slot04_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_23_slot04_06(Object *obj) {
    data_801c5590_slot04_06[obj->state_3](obj);
}

void advance_state_3_restore_facing_set_sequence_slot04_06(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}
