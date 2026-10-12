/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c7e04_slot04_02[])(Object *);

void call_handler_by_state_0_4_slot04_02(Object *obj) {
    data_801c7e04_slot04_02[obj->state_0](obj);
}
