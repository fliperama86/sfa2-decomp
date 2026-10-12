/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011eae4(void) {
    func_8011f010();
    init_object_pools();
    func_8011a744();
}

void func_8011eb14(void) {
    func_8011ef00();
    func_8011f010();
    clear_game_state_fields();
    init_object_pools();
}
