/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b8cb0_slot04_sel[])(void);

void call_handler_by_step_index_2_table_b_7_slot04_sel(Object *unused) {
    data_801b8cb0_slot04_sel[screen_state->step_index_2]();
}
