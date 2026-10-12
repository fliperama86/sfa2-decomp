/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd264_slot05_06[];
extern ObjectFn data_801dd26c_slot05_06[];
extern ObjectFn data_801dd278_slot05_06[];
extern ObjectFn data_801dd280_slot05_06[];

void func_801c86b4_slot05_06(Object *obj) {
    data_801dd264_slot05_06[obj->state_3](obj);
}

void advance_state_3_and_start_sequence_slot05_06(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void load_velocities_from_table_3_slot05_06(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_handler_by_direction_index_4_slot05_06(Object *obj) {
    data_801dd26c_slot05_06[obj->direction_index >> 1](obj);
}

void func_801c8788_slot05_06(Object *obj) {
    data_801dd278_slot05_06[obj->state_3](obj);
}

void advance_state_3_and_start_sequence_2_slot05_06(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void load_velocities_from_table_4_slot05_06(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801c8818_slot05_06(Object *obj) {
    data_801dd280_slot05_06[obj->state_3](obj);
}
