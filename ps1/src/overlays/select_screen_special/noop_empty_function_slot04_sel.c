/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b6e88_slot04_sel[])(void);

void noop_empty_function_slot04_sel(void) {
}

void noop_slot00_slot04_sel(Object *obj) {
}

void call_handler_by_step_index_2_table_b_3_slot04_sel(Object *unused) {
    data_801b6e88_slot04_sel[screen_state->step_index_2]();
}
