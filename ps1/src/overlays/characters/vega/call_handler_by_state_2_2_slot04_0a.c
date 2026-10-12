/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c04fc_slot04_0a[];

void call_handler_by_state_2_2_slot04_0a(Object *obj) {
    data_801c04fc_slot04_0a[obj->state_2](obj);
}
