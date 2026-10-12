/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clear_game_state_flag_and_call_palette_setup(void) {
    game_state.field_31 = 0;
    apply_palette_mode_from_game_state_flag(1);
}
