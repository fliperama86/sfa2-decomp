/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8004ea10_slot28[])(Object *);

void call_handler_by_hud_step_index_2_17_slot28(Object *obj) {
    data_8004ea10_slot28[screen_state->step_index_2](obj);
}
