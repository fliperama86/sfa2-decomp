/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_game_state_table_handler(void) {
    table_8017ac34[game_state.handler_index]();
}
