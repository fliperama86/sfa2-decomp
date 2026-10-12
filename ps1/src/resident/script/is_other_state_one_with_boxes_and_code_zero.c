/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

int is_other_state_one_with_boxes_and_code_zero(Object *object)
{
  object = object->other;
  if (object->other_state_code == 0) {
    return is_state_one_with_frame_boxes(object) != 0;
  }
  return 0;
}
