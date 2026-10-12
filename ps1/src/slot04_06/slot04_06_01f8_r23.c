/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5540_slot04_06[];

void call_handler_by_state_2_2_slot04_06(Object *obj) {
    data_801c5540_slot04_06[obj->state_2](obj);
}
