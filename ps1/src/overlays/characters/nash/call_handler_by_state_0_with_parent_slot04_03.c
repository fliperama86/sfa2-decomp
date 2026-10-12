/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c09d8_slot04_03[])(Object *, Object *);

void call_handler_by_state_0_with_parent_slot04_03(Object *obj) {
    data_801c09d8_slot04_03[obj->state_0](obj, obj->parent);
}
