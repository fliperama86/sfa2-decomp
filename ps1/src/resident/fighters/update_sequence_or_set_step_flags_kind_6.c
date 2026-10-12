/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_sequence_or_set_step_flags_kind_6(Object *object) {
    if (object->kind != 6 || *(u8 *)&object->step_flags == 0) {
        update_sequence(object);
    } else {
        *(s16 *)&object->step_flags = -0x100;
    }
}

void clear_flag_call_state_3_handler_set_a(Object *object) {
    object->field_16a = 0;
    handlers_07[object->state_3](object);
}
