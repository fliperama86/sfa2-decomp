/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
int is_other_in_state_01010101_with_flags(Object *object)
{
  Object *other;
  int ok;
  int result;
  ok = is_state_one_and_state_two_allowed(object);
  result = 0;
  if (ok != 0)
  {
    other = object->other;
    if (*((u32 *) (&other->state_0)) == 0x1010101)
    {
      if (other->field_60 == 2)
      {
        if (other->field_15b != 0) return 1;
      }
    }
  }
  return result;
}
