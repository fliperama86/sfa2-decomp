/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ed4c4_slot06_08[];

void call_handler_by_state_0_2_slot06_08(Object *obj) {
    data_801ed4c4_slot06_08[obj->state_0](obj);
}
