/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014e758(Object *object);

/* Form found by automatic permutation search. */
void func_8014bee8(Object *object)
{
  short new_var2;
  u16 v = *(data_80189460++);
  u16 new_var;
  new_var2 = v;
  new_var = new_var2 >> 8;
  object->field_212 = new_var;
  object->field_213 = v;
  if ((*object).field_cf >= (new_var2 & 0x1f))
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
