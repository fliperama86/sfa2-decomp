/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80051828_slot28[])(Object *);

void call_handler_by_state_0_22_slot28(Object *object) {
    data_80051828_slot28[object->state_0](object);
}
