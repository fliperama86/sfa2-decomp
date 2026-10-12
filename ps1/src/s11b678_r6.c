/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clear_arrays_and_init_pools_and_sprite_tables(void) {
    clear_four_144_byte_arrays();
    init_object_pools();
    init_sprite_tables();
}

void clear_players_arrays_state_and_init_pools(void) {
    clear_players();
    clear_four_144_byte_arrays();
    clear_game_state_fields();
    init_object_pools();
}
