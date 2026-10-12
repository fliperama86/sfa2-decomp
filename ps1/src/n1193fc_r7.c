/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void call_counter_update_if_right_field_zero(Object *object) {
    if (player_right.field_165 == 0) {
        func_8011f70c(object);
    }
}

void call_counter_update_if_left_field_zero(Object *object) {
    if (player_left.field_165 == 0) {
        func_8011f70c(object);
    }
}
