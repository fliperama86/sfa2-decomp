/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8014b8d8(Object *object)
{
  u16 *p = data_80189460;
  u16 v;
  s16 w;
  u16 t;
  data_80189460 = p + 1;
  v = *p;
  w = v;
  t = w >> 8;
  object->field_212 = t;
  object->field_213 = v;
  if (v <= object->field_21e)
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
