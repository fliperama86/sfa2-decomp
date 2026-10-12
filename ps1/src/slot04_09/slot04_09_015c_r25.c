/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7dc4_slot04_09[];

void update_sequence_or_call_finish_on_flag_3_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_18_slot04_09(Object *obj) {
    data_801c7dc4_slot04_09[obj->state_3](obj);
}
