/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c4154_slot04_04[];

void call_handler_by_state_2_slot04_04(Object *obj) {
    data_801c4154_slot04_04[obj->state_2](obj);
}
