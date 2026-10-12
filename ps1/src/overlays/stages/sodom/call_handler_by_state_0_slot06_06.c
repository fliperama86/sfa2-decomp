/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ee1c8_slot06_06[];

void call_handler_by_state_0_slot06_06(Object *obj) {
    data_801ee1c8_slot06_06[obj->state_0](obj);
}
