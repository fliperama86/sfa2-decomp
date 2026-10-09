/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80145a20(Object *object) {
    if (game_state.field_65 == 0 && game_state.field_74 == 0) {
        if (object->field_0e == 4) {
            func_8011f7e8(object);
        } else if (object->field_0e == 8) {
            func_8011f83c(object);
        } else if (object->field_0e == 0xc) {
            func_8011f890(object);
        }
    }
}
