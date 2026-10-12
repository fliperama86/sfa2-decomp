/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_follow_parent_sequence_check(Object *object) {
  if (game_state.field_65 == 0) {
    ref_other.p = object->parent;
    if ((((u32 *) ref_other.p)[1] & 0xffffff) == 0x80001) {
      advance_sequence_checked(object);
    } else {
      object->state_1 = 0;
      object->state_0++;
      return;
    }
  }
  func_8011ffdc(object);
}
