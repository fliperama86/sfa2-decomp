/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079834_slot2b[];
extern ObjectFn data_80079840_slot2b[];
void call_handler_by_direction_index_3_slot2b(Object *obj);
void call_handler_by_direction_index_4_slot2b(Object *obj);

void update_sequence_reset_state_3_on_check_2_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_80077404_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->active_state = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_80077444_slot2b(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_4_slot2b(obj);
    } else {
        call_handler_by_direction_index_3_slot2b(obj);
    }
}

void call_handler_by_direction_index_3_slot2b(Object *obj) {
    data_80079834_slot2b[obj->direction_index >> 1](obj);
}

void call_handler_by_state_3_3_slot2b(Object *obj) {
    data_80079840_slot2b[obj->state_3](obj);
}
