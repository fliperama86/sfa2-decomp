/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd594_slot05_06[];
extern ObjectFn data_801dd5a0_slot05_06[];
extern ObjectFn data_801dd5a8_slot05_06[];

void call_handler_by_direction_index_8_slot05_06(Object *obj) {
    data_801dd594_slot05_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_24_slot05_06(Object *obj) {
    data_801dd5a0_slot05_06[obj->state_3](obj);
}

void advance_state_3_restore_facing_set_sequence_2_slot05_06(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_reset_on_negative_flags_4_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void call_table_function_by_state_3_25_slot05_06(Object *obj) {
    data_801dd5a8_slot05_06[obj->state_3](obj);
}
