/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80149b80(Object *object) {
  if (game_state.field_4d != 0) {
    return 0;
  }
  if (game_state.field_04 != 0) {
    return 0;
  }
  if (game_state.field_4e != 0) {
    return 0;
  }
  if (func_8012f56c(object) != 0) {
    return 0;
  }
  if (object->field_21b == 0) {
    return 0;
  }
  if (object->field_67 == 0) {
    return 0;
  }
  if (object->frame->active != 0) {
    object->field_21b = 0;
    return 1;
  }
  return 0;
}
