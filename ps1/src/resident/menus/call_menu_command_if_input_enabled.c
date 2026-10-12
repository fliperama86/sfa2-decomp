/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_menu_command_if_input_enabled(GameState *unused) {
    if (game_state.input_enable_bits != 0) {
        set_menu_command_by_code(6, 2);
    }
}
