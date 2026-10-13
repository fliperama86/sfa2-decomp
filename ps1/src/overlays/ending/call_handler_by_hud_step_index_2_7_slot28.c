/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8003a00c_slot28[])(Object *);

void call_handler_by_hud_step_index_2_7_slot28(Object *obj) {
    data_8003a00c_slot28[screen_state->step_index_2](obj);
}
