/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800287a8_slot12[])(Object *);

void call_handler_by_state_0_6_slot12(Object *obj) {
    data_800287a8_slot12[obj->state_0](obj);
}
