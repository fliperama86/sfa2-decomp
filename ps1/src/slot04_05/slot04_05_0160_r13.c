/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c14fc_slot04_05[];
extern ObjectFn data_801c1508_slot04_05[];

void update_sequence_or_call_finish_on_flag_2_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_direction_index_slot04_05(Object *obj) {
    data_801c14fc_slot04_05[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_10_slot04_05(Object *obj) {
    data_801c1508_slot04_05[obj->state_3](obj);
}
