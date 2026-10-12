/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079848_slot2b[];
extern ObjectFn data_80079854_slot2b[];

void func_80077508_slot2b(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_reset_state_3_on_check_3_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_direction_index_4_slot2b(Object *obj) {
    data_80079848_slot2b[obj->direction_index >> 1](obj);
}

void call_handler_by_state_3_4_slot2b(Object *obj) {
    data_80079854_slot2b[obj->state_3](obj);
}

void func_80077628_slot2b(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}
