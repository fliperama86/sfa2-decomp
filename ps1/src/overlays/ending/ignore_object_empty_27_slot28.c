/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80046988_slot28[])(Object *);

void ignore_object_empty_27_slot28(Object *obj) {
}

void call_handler_by_hud_step_index_2_13_slot28(Object *obj) {
    data_80046988_slot28[screen_state->step_index_2](obj);
}
