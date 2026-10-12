/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80037478_slot28[])(Object *);

void call_state_1_handler_d_and_show_3_slot28(Object *obj) {
    data_80037478_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
