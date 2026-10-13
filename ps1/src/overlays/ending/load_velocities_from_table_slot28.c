/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800289d4_slot28[])(Object *);

void call_handler_by_substep_index_slot28(Object *obj);

void load_velocities_from_table_slot28(Object *obj) {
    call_handler_by_substep_index_slot28(obj);
}

void call_handler_by_substep_index_slot28(Object *obj) {
    data_800289d4_slot28[screen_state->substep_index](obj);
}
