/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801beea0_slot04_01[];

void call_handler_by_state_2_slot04_01(Object *obj) {
    data_801beea0_slot04_01[obj->state_2](obj);
}
