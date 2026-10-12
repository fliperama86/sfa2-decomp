/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7b88_slot04_09[];

void call_handler_by_state_2_2_slot04_09(Object *obj) {
    data_801c7b88_slot04_09[obj->state_2](obj);
}
