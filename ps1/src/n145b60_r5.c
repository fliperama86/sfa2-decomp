/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014894c(Object *object) {
  Object *other;
  if (game_state.field_65 == 0) {
    other = object->field_3c;
    if ((s16) other->field_5c >= 0
        && (other->field_163 != 0 || (((u32 *) other)[1] & 0xffffff) == 0x90001)) {
      object->field_48 = (object->field_48 + 1) & 0x3f;
      func_80148a74(object);
      func_801488ec(object);
    } else {
      object->field_04++;
      return;
    }
  }
  func_80120028(object);
}
