/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c2fd4_slot04_0b[])(Object *);

void call_handler_by_state_0_with_parent_2_slot04_0b(Object *obj) {
    ref_other.p = obj->parent;
    data_801c2fd4_slot04_0b[obj->state_0](obj);
}
