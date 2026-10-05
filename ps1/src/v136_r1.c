/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
int func_8014e368(Object *object)
{
  Object *other;
  int ok;
  int result;
  ok = func_8014e310(object);
  result = 0;
  if (ok != 0)
  {
    other = object->other;
    if (*((u32 *) (&other->field_04)) == 0x1010101)
    {
      if (other->field_60 == 2)
      {
        if (other->field_15b != 0) return 1;
      }
    }
  }
  return result;
}
