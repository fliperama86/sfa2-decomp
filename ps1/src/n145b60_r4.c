/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801486a4(Object *object) {
  if (game_state.field_65 == 0) {
    ref_other.p = object->field_3c;
    if ((((u32 *) ref_other.p)[1] & 0xffffff) == 0x80001) {
      func_80131094(object);
    } else {
      object->field_05 = 0;
      object->field_04++;
      return;
    }
  }
  func_8011ffdc(object);
}
