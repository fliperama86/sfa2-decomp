/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2ff8_slot04_0b[])(Object *);

void call_handler_by_state_1_3_slot04_0b(Object *obj) {
    data_801c2ff8_slot04_0b[obj->state_1](obj);
}
