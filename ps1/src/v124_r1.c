/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012eb18(Object *o)
{
  int z = 0;
  if (game_state.config->field_4e == 0)
  {
    goto tail;
  }
  switch (o->kind)
  {
    case 3:

    case 4:

    case 5:

    case 8:

    case 17:

    case 18:

    case 19:
      if (((s16) o->step_flags) < 0)
    {
      goto tail;
    }
      update_sequence(o);
      return;

    default:
      if (o->side == 0)
    {
      (*((void (**)(Object *, int)) 0x1f800078))(o, z);
    }
    else
    {
      (*((void (**)(Object *, int)) 0x1f800128))(o, z);
    }
      return;

  }

  tail:
  o->state_0 = 1;

  o->state_1 = 0;
  o->state_2 = 0;
  o->state_3 = 0;
  func_80130678(o, 0);
}
