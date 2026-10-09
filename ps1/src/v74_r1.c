/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014e758(Object *object);
int func_8014e718(Object *object);

void func_8014afd4(Object *object)
{
  func_8014e7ec(object);
  data_80189464 = (data_80189464 - object->field_21e) + 3;
  if (data_80189464 < 6)
  {
    func_8014c914(object);
  }
  else
  {
    u16 limit = object->field_211;
    limit |= object->field_210 << 8;
    object->field_208 = 1;
    object->field_209 = 2;
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 2;
    object->field_07 = 0;
    object->field_48 = 0;
    object->field_21a = 0;
    if (limit >= object->field_21e)
    {
      object->field_48 = 1;
      object->field_21a = 1;
    }
  }
}
