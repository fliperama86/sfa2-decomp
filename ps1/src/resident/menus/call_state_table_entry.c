/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void call_state_table_entry(GameState *state) {
    table_8016e7bc[((HudBig *)data_8018f5a0)->state_index](state);
}
