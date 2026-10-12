/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801bfd70_slot04_00[])(Object *);

void call_handler_by_state_3_set_flag_slot04_00(Object *obj) {
    obj->field_157 = 1;
    data_801bfd70_slot04_00[obj->state_3](obj);
}
