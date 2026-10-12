/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void noop_slot00_slot04_03(Object *obj);
extern ObjectFn data_801c0530_slot04_03[];

void call_state_3_handler_then_noop_2_slot04_03(Object *obj) {
    data_801c0530_slot04_03[obj->state_3](obj);
    noop_slot00_slot04_03(obj);
}
