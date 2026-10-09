/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual in func_8014cb1c: same swapped-register pair as func_8014ca7c (move present, registers
 * reversed, field_20f store scheduled one slot earlier). Calls without an argument (nop in the delay
 * slot) are written as unprototyped declarations. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014de94(Object *object);

/* Form found by automatic permutation search. */
void func_8014cb1c(Object *object)
{
  Object *new_var2;
  u16 v;
  u16 t;
  object->field_20a = 0;
  new_var2 = object;
  new_var2->field_20c = new_var2->field_222;
  data_80189460 = (u16 *) new_var2->field_228;
  new_var2->field_238 = (s32) data_80189460;
  t = *(data_80189460++);
  v = t;
  t = ((s16) v) >> 8;
  new_var2->field_20e = t;
  new_var2->field_20f = v;
  v = new_var2->field_20e;
  if (v == 0xff)
  {
    func_8014d9ac(object);
    func_8014c994(new_var2);
  }
}
