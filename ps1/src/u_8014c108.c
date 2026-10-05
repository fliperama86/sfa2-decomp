/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014c108(Object *object)
{
  u16 v = *(data_80189460++);
  Object *other = object->other;
  u16 new_var;
  Object *new_var2;
  new_var2 = other;
  v = (u16) (other->field_70 - v);
  new_var = v;
  if (new_var <= ((u16) new_var2->pos_y))
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
