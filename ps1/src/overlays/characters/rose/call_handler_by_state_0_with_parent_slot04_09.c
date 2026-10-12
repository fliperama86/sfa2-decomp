/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c7e14_slot04_09[])(Object *, Object *);

void call_handler_by_state_0_with_parent_slot04_09(Object *obj) {
    data_801c7e14_slot04_09[obj->state_0](obj, obj->parent);
}
