/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012b024(Object *object)
{
  if ((u8) func_80130184(object))
  {
    if ((s16) object->step_flags >= 0)
    {
      update_sequence(object);
    }
    else
    {
      object->state_3 = 1;
      object->active_flag_3 = 0;
      update_sequence(object);
    }
  }
  else
  {
    object->pos_y = object->ground_y;
    object->active_state = 0;
    object->active_flag_3 = 0;
    func_801209c4(object);
    if (object->mode_flag_2 == 0 && func_8012f970(object))
    {
      func_8012fe60(object);
    }
    else
    {
      object->state_3 = 2;
      set_sequence_by_side(object, 0x11);
    }
  }
}
