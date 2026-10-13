/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800e841c_slot0f[])(Object *);
extern void (*data_800e8428_slot0f[])(Object *);

void call_object_handler_by_phase_index_slot0f(Object *obj) {
    data_800e841c_slot0f[screen_state->phase_index](obj);
}

void call_object_handler_by_step_index_slot0f(Object *obj) {
    data_800e8428_slot0f[screen_state->step_index](obj);
}
