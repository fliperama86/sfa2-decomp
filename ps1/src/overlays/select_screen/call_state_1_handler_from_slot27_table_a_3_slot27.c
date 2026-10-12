/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80026940_slot27[])(Object *);
extern void (*data_80026950_slot27[])(Object *);

void call_state_1_handler_from_slot27_table_a_3_slot27(Object *object) {
    data_80026940_slot27[object->state_1](object);
}

void call_handler_by_state_2_set_flag_one_slot27(Object *obj) {
    data_80026950_slot27[obj->state_2](obj);
    obj->field_01 = 1;
}
