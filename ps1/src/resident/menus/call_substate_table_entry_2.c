/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void call_substate_table_entry_2(void) {
    table_8018134c[((HudBig *)screen_state)->substate_index]();
}
