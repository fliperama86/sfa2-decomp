/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801e9f80_slot06_00[])(Object *);

void call_handler_by_state_0_2_slot06_00(Object *object) {
    data_801e9f80_slot06_00[object->state_0](object);
}
