/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_8014be78(Object *object)
{
  short v = *(data_80189460++);
  u16 new_var;
  new_var = v >> 8;
  object->script_byte_hi = new_var;
  object->script_byte_lo = v;
  if ((v & 0x1f) >= object->table_column)
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
