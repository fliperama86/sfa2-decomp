/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_direction_index_5_slot05_06(Object *obj);
void call_handler_by_direction_index_6_slot05_06(Object *obj);
extern ObjectFn data_801dd54c_slot05_06[];
extern ObjectFn data_801dd558_slot05_06[];

void call_handler_by_input_dir_second_2_slot05_06(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_6_slot05_06(obj);
    } else {
        call_handler_by_direction_index_5_slot05_06(obj);
    }
}

void call_handler_by_direction_index_5_slot05_06(Object *obj) {
    data_801dd54c_slot05_06[obj->direction_index >> 1](obj);
}

void func_801cd2dc_slot05_06(Object *obj) {
    data_801dd558_slot05_06[obj->state_3](obj);
}

void func_801cd31c_slot05_06(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
