/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80037480_slot28[])(Object *);

void call_handler_by_state_1_7_slot28(Object *object) {
    data_80037480_slot28[object->state_1](object);
}
