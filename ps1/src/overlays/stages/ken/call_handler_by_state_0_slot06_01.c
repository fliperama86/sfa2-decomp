/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ea930_slot06_01[];

void call_handler_by_state_0_slot06_01(Object *obj) {
    data_801ea930_slot06_01[obj->state_0](obj);
}
