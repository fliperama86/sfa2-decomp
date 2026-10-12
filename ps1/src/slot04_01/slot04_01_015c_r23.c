/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801bf31c_slot04_01[])(Object *);

void call_handler_by_state_0_2_slot04_01(Object *obj) {
    data_801bf31c_slot04_01[obj->state_0](obj);
}
