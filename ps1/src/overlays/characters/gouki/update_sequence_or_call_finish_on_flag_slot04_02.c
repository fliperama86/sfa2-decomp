/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c61c8_slot04_02[])(Object *);

void update_sequence_or_call_finish_on_flag_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_slot04_02(Object *obj) {
    obj->field_157 = 1;
    data_801c61c8_slot04_02[obj->state_3](obj);
}
