/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014e758(Object *object);

/* Form found by automatic permutation search. */
void func_8014be78(Object *object)
{
  short v = *(data_80189460++);
  u16 new_var;
  new_var = v >> 8;
  object->field_212 = new_var;
  object->field_213 = v;
  if ((v & 0x1f) >= object->field_cf)
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
