/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_801dd224_slot05_06[];
extern ObjectFn data_801dd230_slot05_06[];
extern ObjectFn data_801dd238_slot05_06[];

void call_handler_by_direction_index_slot05_06(Object *obj) {
    data_801dd224_slot05_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_slot05_06(Object *obj) {
    data_801dd230_slot05_06[obj->state_3](obj);
}

void enter_state_1_2_on_button_range_check_wide_slot05_06(Object *obj) {
    obj->state_3++;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x16) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void load_velocities_from_table_slot05_06(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_handler_by_direction_index_2_slot05_06(Object *obj) {
    data_801dd238_slot05_06[obj->direction_index >> 1](obj);
}
