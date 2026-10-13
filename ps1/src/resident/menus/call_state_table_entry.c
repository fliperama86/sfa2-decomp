/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void call_state_table_entry(GameState *state) {
    table_8016e7bc[((HudBig *)screen_state)->state_index](state);
}
