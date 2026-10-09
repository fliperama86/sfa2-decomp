/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_801412a4(Object *object);
int func_80141618(Object *object);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (196 bytes vs 200): the original puts `move a0,s0` in the delay slots
 * of the branches before the shared tail and stores through a0; this form
 * keeps the move in the delay slot of the final call. Same cause as in
 * v2_r3. */
void func_80142a14(Object *object)
{
  s16 state = object->field_3a;
  if (state < 0)
  {
    if (object->field_128 != 0)
    {
      func_80131468(object);
    }
    else
    {
      func_801312b8(object);
    }
    return;
  }
  if ((state & 0x80) == 0)
  {
    if ((u8)func_801412a4(object) == 0)
    {
      func_80130efc(object);
      return;
    }
  }
  else
  {
    if ((u8)func_80141618(object) == 0 && (u8)func_801412a4(object) == 0)
    {
      func_80130efc(object);
      return;
    }
  }
  object->field_07 = 0;
  object->field_25f = 0xff;
  func_80130efc(object);
}
