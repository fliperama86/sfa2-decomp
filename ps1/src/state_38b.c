/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Second name for game_state. The original loads this address on its own,
   apart from the store through game_state; with one name this compiler
   derives it from the store address. */


void check_collisions_unless_paused(void) {
    game_state.config = &game_state_second;
    if (game_state.config->pause_flag_b == 0 &&
        (player_left.field_165 | player_right.field_165) == 0) {
        check_player_hits_and_set_defeat_bits();
        check_object_pair_box_collisions();
        func_8013bfa4();
        if (game_state.versus_flag != 0 && *(u16 *)data_801a6984 == 0) {
            player_left.health = 0x90;
            player_right.health = 0x90;
        }
    }
}
