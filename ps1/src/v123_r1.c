/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int func_80130184(Object *object);
void func_80130678(Object *object, int index);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012b024(Object *object)
{
  if ((u8) func_80130184(object))
  {
    if ((s16) object->field_3a >= 0)
    {
      func_80130efc(object);
    }
    else
    {
      object->field_07 = 1;
      object->field_159 = 0;
      func_80130efc(object);
    }
  }
  else
  {
    object->pos_y = object->field_70;
    object->field_45 = 0;
    object->field_159 = 0;
    func_801209c4(object);
    if (object->field_cd == 0 && func_8012f970(object))
    {
      func_8012fe60(object);
    }
    else
    {
      object->field_07 = 2;
      func_80130678(object, 0x11);
    }
  }
}
