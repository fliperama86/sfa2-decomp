/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011eae4(void) {
    clear_four_144_byte_arrays();
    init_object_pools();
    init_sprite_tables();
}

void func_8011eb14(void) {
    clear_players();
    clear_four_144_byte_arrays();
    clear_game_state_fields();
    init_object_pools();
}
