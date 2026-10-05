/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual in func_8014ca7c: the original has the same move between the load and the shifts, but with
 * the two temporaries in swapped registers ($v1 loaded, $v0 copy); ours is $v0 / $v1, and the store of
 * field_20f therefore schedules before the reload. Calls without an argument (nop in the delay slot)
 * are written as unprototyped declarations. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014de94(Object *object);

/* Form found by automatic permutation search. */
void func_8014ca7c(Object *object)
{
  u16 v;
  u16 t;
  if (object->field_20a != 0)
  {
    func_8014cb1c();
  }
  else
  {
    func_8014d9ac(object);
    data_80189460 = (u16 *) object->field_228;
    object->field_238 = (s32) data_80189460;
    t = *(data_80189460++);
    v = t;
    t = ((s16) v) >> 8;
    object->field_20e = t;
    object->field_20f = v;
    if ((v = object->field_20e) == 0xff)
    {
      func_8014c994(object);
    }
  }
}
