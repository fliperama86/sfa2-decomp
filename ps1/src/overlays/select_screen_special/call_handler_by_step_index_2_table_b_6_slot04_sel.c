/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b7eb8_slot04_sel[])(void);

void call_handler_by_step_index_2_table_b_6_slot04_sel(Object *unused) {
    data_801b7eb8_slot04_sel[screen_state->step_index_2]();
}
