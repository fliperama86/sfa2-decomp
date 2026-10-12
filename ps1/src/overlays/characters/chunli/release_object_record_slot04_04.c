/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c45bc_slot04_04[])(Object *);

void release_object_record_slot04_04(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot04_04(Object *obj) {
    data_801c45bc_slot04_04[obj->state_0](obj);
}
