/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80026a7c_slot27[])(Object *);

void call_handler_by_state_2_clear_flag_2_slot27(Object *obj) {
    data_80026a7c_slot27[obj->state_2](obj);
    obj->field_01 = 0;
}
