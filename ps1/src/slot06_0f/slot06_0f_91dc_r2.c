/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eb4a0_slot06_0f[];

void call_handler_by_state_0_slot06_0f(Object *obj) {
    data_801eb4a0_slot06_0f[obj->state_0](obj);
}
