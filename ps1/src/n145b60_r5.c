/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014894c(Object *object) {
  Object *other;
  if (game_state.field_65 == 0) {
    other = object->parent;
    if ((s16) other->health >= 0
        && (other->field_163 != 0 || (((u32 *) other)[1] & 0xffffff) == 0x90001)) {
      object->variant_index = (object->variant_index + 1) & 0x3f;
      func_80148a74(object);
      func_801488ec(object);
    } else {
      object->state_0++;
      return;
    }
  }
  func_80120028(object);
}
