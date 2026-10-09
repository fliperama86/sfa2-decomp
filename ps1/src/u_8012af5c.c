/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8012af5c(Object *object)
{
  s32 b;
  int new_var;
  s32 a;
  if ((object->field_3a << 16) >= 0)
  {
    func_80130efc(object);
  }
  else
  {
    object->field_07 = 1;
    a = motion_recs[object->kind].field_00;
    object->field_50 = motion_recs[object->kind].field_04;
    b = motion_recs[object->kind].field_08;
    object->field_58 = motion_recs[object->kind].field_0c;
    new_var = object->field_0b ^ 1;
    object->field_0b = new_var;
    if (object->field_0b != 0)
    {
      a = -a;
      b = -b;
    }
    object->field_4c = a;
    a = b;
    object->field_54 = a;
    func_80130678(object, 0x13);
  }
}
