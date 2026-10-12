/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800efa20_slot0f[])(Object *);

void call_handler_by_state_0_slot0f(Object *object) {
    data_800efa20_slot0f[object->state_0](object);
}
