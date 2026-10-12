/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c0924_slot04_0a[])(Object *);

void noop_slot00_2_slot04_0a(Object *obj) {
}

void noop_slot00_3_slot04_0a(Object *obj) {
}

void noop_slot00_4_slot04_0a(Object *obj) {
}

void call_handler_by_state_0_3_slot04_0a(Object *obj) {
    data_801c0924_slot04_0a[obj->state_0](obj);
}
