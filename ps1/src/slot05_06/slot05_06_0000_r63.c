/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_direction_index_7_slot05_06(Object *obj);
void call_handler_by_direction_index_8_slot05_06(Object *obj);
extern ObjectFn data_801dd580_slot05_06[];
extern ObjectFn data_801dd58c_slot05_06[];

void update_sequence_or_finish_sequence_4_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801cd6d8_slot05_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_8_slot05_06(obj);
    } else {
        call_handler_by_direction_index_7_slot05_06(obj);
    }
}

void call_handler_by_direction_index_7_slot05_06(Object *obj) {
    data_801dd580_slot05_06[obj->direction_index >> 1](obj);
}

void func_801cd75c_slot05_06(Object *obj) {
    data_801dd58c_slot05_06[obj->state_3](obj);
}

void func_801cd79c_slot05_06(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void func_801cd7d4_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}
