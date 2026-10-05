/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (not exact, 204 bytes, 7 slots differ): the original loads the halfword into $v1 and
 * copies it to $v0 (stored as the low byte), shifting from $v0; here the load goes to $v0 and the
 * copy to $v1. Pure register-assignment swap of the loaded value and its copy. Forms tried: see
 * u16 t reused for load and later field_20a read, s16 w copy; int/u16 copies (copy folded away). */
/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int func_8014a4f4(Object *object)
{
  u16 *p = (u16 *) object->field_238;
  u16 t;
  s16 w;
  data_80189460 = p + 1;
  t = *p;
  w = t;
  t = w >> 8;
  object->field_20e = t;
  object->field_20f = w;
  w = object->field_20e;
  if (w == 0xff)
  {
    func_8014c994();
  }
  func_8014a5c0(object);
  object->field_238 = (s32) data_80189460;
  t = object->field_20a;
  if (t == 0)
  {
    object->field_228 = (s32) data_80189460;
    return;
  }
  if (t == 1)
  {
    object->field_22c = (s32) data_80189460;
    return;
  }
  if (t == 2)
  {
    object->field_230 = (s32) data_80189460;
    return;
  }
  if (t == 3)
  {
    object->field_234 = (s32) data_80189460;
  }
}
