/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012bbac(Object *object)
{
  object->facing_side = object->saved_facing;
  ref_other.p = object->other;
  if (ref_other.p->field_157 == 0)
  {
    object->state_3 = 0;
    set_sequence_by_side(object, 0x15);
    return;
  }
  if (ref_other.p->active_state != 0)
  {
    object->state_3 = 0;
    set_sequence_by_side(object, 0x15);
    return;
  }
  object->state_3 = 2;
  set_sequence_by_side(object, 0x18);
}
