/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int func_8014e310(Object *object)
{
  int new_var;
  if ((*((u16 *) (&object->state_0))) != 1)
  {
    return 0;
  }
  if (object->state_2 == 3)
  {
    return 0;
  }
  if (object->state_2 == 5)
  {
    return 0;
  }
  if (((u8) (object->state_2 - 7)) < 2)
  {
    return 0;
  }
  new_var = 9;
  if (object->state_2 == new_var)
  {
    return 0;
  }
  new_var = 1;
  return new_var;
}
