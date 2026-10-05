/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8012c494(Object *object)
{
  int k = object->field_61;
  unsigned char new_var;
  object->field_68 = pairs_80170074[k].first;
  object->field_60 = pairs_80170074[k].second;
  new_var = k;
  if ((new_var == 0x14) || (new_var == 0x16))
  {
    func_8012ca04(object);
  }
  else
    if ((new_var == 0x15) || (new_var == 0x17))
  {
    func_8012cb3c(object);
  }
  else
    if (new_var == 0xb)
  {
    object->field_45 = 0xff;
    object->field_69++;
    func_801308c4(object, object->field_68);
  }
  else
  {
    object->field_45 = 0xff;
    object->field_69++;
    if (object->field_60 == 2)
    {
      if (new_var == 0xc)
      {
        func_8012c990(object);
        return;
      }
      if (new_var == 0xd)
      {
        func_8012cac8(object);
        return;
      }
      if (new_var == 0x11)
      {
        func_8012cc6c(object);
        return;
      }
    }
    func_801308c4(object, object->field_68);
  }
}
