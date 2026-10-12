/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015310_slot01[];

void call_handler_by_state_0_slot01(Object *object) {
    data_80015310_slot01[object->state_0](object);
}
