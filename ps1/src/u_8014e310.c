/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int func_8014e310(Object *object)
{
  int new_var;
  if ((*((u16 *) (&object->field_04))) != 1)
  {
    return 0;
  }
  if (object->field_06 == 3)
  {
    return 0;
  }
  if (object->field_06 == 5)
  {
    return 0;
  }
  if (((u8) (object->field_06 - 7)) < 2)
  {
    return 0;
  }
  new_var = 9;
  if (object->field_06 == new_var)
  {
    return 0;
  }
  new_var = 1;
  return new_var;
}
