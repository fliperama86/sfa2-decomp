/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2124_slot04_07[])(Object *);

void call_handler_by_state_0_slot04_07(Object *obj) {
    data_801c2124_slot04_07[obj->state_0](obj);
}
