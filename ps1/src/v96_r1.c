/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014e718(Object *object)
{
  object = object->other;
  if (object->field_249 == 0) {
    return func_8014e784(object) != 0;
  }
  return 0;
}
