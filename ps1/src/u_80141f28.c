/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_80141f28(Object *object, short delta)
{
  s16 old;
  if (object->field_74 != 0)
  {
    return;
  }
  if ((object->field_2a2 != 0) && (((s16) delta) >= 0))
  {
    return;
  }
  if (((s16) delta) == 0)
  {
    return;
  }
  if (((s16) delta) > 0)
  {
    if (game_state.field_47 != 0)
    {
      return;
    }
    if (object->field_d8 != 0)
    {
      delta = ((s16) delta) >> 2;
      if (delta == 0)
      {
        delta = 1;
      }
    }
  }
  old = object->field_c6;
  object->field_c6 = delta + object->field_c6;
  if (((s16) object->field_c6) < 0)
  {
    object->field_c6 = 0;
  }
  else
    if (object->field_d8 != 0)
  {
    if (((s16) object->field_c6) >= 0x30)
    {
      object->field_c6 = 0x30;
    }
  }
  else
    if (((s16) object->field_c6) >= 0x90)
  {
    object->field_c6 = 0x90;
  }
  if ((((s16) object->field_c6) != old) && (((s16) object->field_c6) >= 0x90))
  {
    func_80120554(0, 0, 0x325);
  }
}
