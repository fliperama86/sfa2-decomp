/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011f70c(Object *object);

void func_8011f6b4(Object *object) {
    if (player_right.field_165 == 0) {
        func_8011f70c(object);
    }
}

void func_8011f6e0(Object *object) {
    if (player_left.field_165 == 0) {
        func_8011f70c(object);
    }
}
