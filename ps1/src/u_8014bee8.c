/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_8014bee8(Object *object)
{
  short new_var2;
  u16 v = *(data_80189460++);
  u16 new_var;
  new_var2 = v;
  new_var = new_var2 >> 8;
  object->script_byte_hi = new_var;
  object->script_byte_lo = v;
  if ((*object).table_column >= (new_var2 & 0x1f))
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
