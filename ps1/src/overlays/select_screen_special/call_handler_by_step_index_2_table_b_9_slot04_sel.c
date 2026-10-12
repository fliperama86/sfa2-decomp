/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b8df4_slot04_sel[])(void);

void call_handler_by_step_index_2_table_b_9_slot04_sel(Object *unused) {
    data_801b8df4_slot04_sel[data_8018f5a0->step_index_2]();
}
