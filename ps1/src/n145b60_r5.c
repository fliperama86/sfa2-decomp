/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_child_orbit_while_parent_active(Object *object) {
  Object *other;
  if (game_state.field_65 == 0) {
    other = object->parent;
    if ((s16) other->health >= 0
        && (other->field_163 != 0 || (((u32 *) other)[1] & 0xffffff) == 0x90001)) {
      object->variant_index = (object->variant_index + 1) & 0x3f;
      place_object_at_parent_with_two_tables(object);
      set_sequence_by_variant_index_half(object);
    } else {
      object->state_0++;
      return;
    }
  }
  set_flag_and_push_if_mode_8(object);
}
