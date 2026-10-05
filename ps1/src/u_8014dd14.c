/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014dd14(Object *object)
{
  s16 v;
  u16 *new_var;
  object->field_218 = 0;
  object->field_219 = 0;
  object->field_21a = 0;
  if (data_80189464 & 2)
  {
    object->field_218 = 1;
  }
  if (data_80189464 & 4)
  {
    object->field_219 = 1;
  }
  if (data_80189464 & 8)
  {
    object->field_21a = 1;
  }
  if (data_80189464 & 1)
  {
    func_8014de20(object);
    return;
  }
  new_var = &data_80189464;
  v = ((s16) (*new_var)) >> 4;
  data_80189464 = v;
  object->field_128 = v & 0xf;
  v = ((s16) data_80189464) >> 4;
  data_80189464 = v;
  object->field_129 = v & 0xf;
  v = ((s16) data_80189464) >> 4;
  data_80189464 = v;
  object->field_12a = v & 0xf;
}
