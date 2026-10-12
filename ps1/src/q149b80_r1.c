/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int consume_active_frame_flag(Object *object) {
  if (game_state.field_4d != 0) {
    return 0;
  }
  if (game_state.field_04 != 0) {
    return 0;
  }
  if (game_state.field_4e != 0) {
    return 0;
  }
  if (is_other_object_state_0_and_1_six_one(object) != 0) {
    return 0;
  }
  if (object->active_flag_5 == 0) {
    return 0;
  }
  if (object->active_flag == 0) {
    return 0;
  }
  if (object->frame->active != 0) {
    object->active_flag_5 = 0;
    return 1;
  }
  return 0;
}
