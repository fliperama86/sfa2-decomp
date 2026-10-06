/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"

void func_801485c8(Object *object) {
  object->field_09 = 4;
  object->field_0f = 1;
  object->field_04++;
  ref_other.p = object->field_3c;
  object->field_1c = ref_other.p->field_1c;
}
