/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5e58_slot04_0f[];

void call_handler_by_state_1_slot04_0f(Object *obj);

void call_handler_by_state_1_slot04_0f(Object *obj) {
    data_801c5e58_slot04_0f[obj->state_1](obj);
}
