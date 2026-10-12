/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8004ab40_slot28[])(Object *);

void call_handler_by_state_0_16_slot28(Object *object) {
    data_8004ab40_slot28[object->state_0](object);
}
