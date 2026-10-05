/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966[];

/* Form found by automatic permutation search. */
void func_8014e7ec(Object *object)
{
  u16 v = *(data_80189460++);
  s16 new_var;
  new_var = ((s16) v) >> 8;
  data_80189464 = v;
  object->field_210 = new_var;
  object->field_211 = v;
}
