/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c2fe4_slot04_0b[])(Object *);
extern void (*data_801c2ff0_slot04_0b[])(Object *);

void call_handler_by_state_index_slot04_0b(Object *o) {
    ModObj *obj = (ModObj *)o;
    data_801c2fe4_slot04_0b[obj->state_index](o);
}

void call_handler_by_state_1_2_slot04_0b(Object *obj) {
    data_801c2ff0_slot04_0b[obj->state_1](obj);
}
