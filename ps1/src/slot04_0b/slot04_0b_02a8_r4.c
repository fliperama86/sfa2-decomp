/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c24a8_slot04_0b[];

void noop_slot00_slot04_0b(Object *obj) {
}

void noop_slot00_2_slot04_0b(Object *obj) {
}

void call_handler_by_state_2_2_slot04_0b(Object *obj) {
    data_801c24a8_slot04_0b[obj->state_2](obj);
}
