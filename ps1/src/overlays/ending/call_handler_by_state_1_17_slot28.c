/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8004c734_slot28[])(Object *);

void call_handler_by_state_1_17_slot28(Object *object) {
    data_8004c734_slot28[object->state_1](object);
}
