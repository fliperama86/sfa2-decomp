/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011fc00();

void func_80145b60(Object *object) {
  if (game_state.field_65 == 0 && game_state.field_74 == 0 && data_8018f598 == 0) {
    if ((object->field_3a & 0x80) == 0) {
      func_8011fc00();
    } else {
      object->field_04++;
    }
  }
}
