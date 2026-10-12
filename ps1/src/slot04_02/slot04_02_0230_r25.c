/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6584_slot04_02[];

void call_handler_by_state_2_2_slot04_02(Object *obj) {
    data_801c6584_slot04_02[obj->state_2](obj);
}
