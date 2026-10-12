/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014afd4(Object *object)
{
  func_8014e7ec(object);
  data_80189464 = (data_80189464 - object->compare_limit) + 3;
  if (data_80189464 < 6)
  {
    func_8014c914(object);
  }
  else
  {
    u16 limit = object->limit_lo;
    limit |= object->limit_hi << 8;
    object->field_208 = 1;
    object->field_209 = 2;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 2;
    object->state_3 = 0;
    object->variant_index = 0;
    object->direction_flag = 0;
    if (limit >= object->compare_limit)
    {
      object->variant_index = 1;
      object->direction_flag = 1;
    }
  }
}
