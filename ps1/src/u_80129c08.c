/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_80129c08(Object *object)
{
  int new_var;
  u16 new_var2;
  if (object->field_7e != 0)
  {
    object->field_c6--;
    new_var2 = object->field_c6;
    if (new_var = ((s16) (new_var = new_var2)) < 0)
    {
      object->field_c6 = 0;
    }
  }
}
