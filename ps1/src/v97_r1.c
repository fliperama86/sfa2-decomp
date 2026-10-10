/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* The parameters are masked in the function, the index at the top and the
   argument in each path before the shared call. Also in this form from the
   search: the early goto to the shared call when the mask is zero, and entry
   declared unsigned. */
void func_8013e2b0(Object *object, int index, int arg)
{
  unsigned entry;
  int mask;
  index &= 0xff;
  object->slots[index].field_04--;
  if (object->slots[index].field_04 == 0)
  {
    arg &= 0xff;
  call_a8:
    func_8013f2a8(object, index, arg);
    return;
  }
  arg &= 0xff;
  entry = table_8017a8cc[(arg * 7) + object->slots[index].field_01];
  mask = object->field_134 & 0xf0ff;
  if (mask == 0)
    goto call_c8;
  if (!(entry & 0x400))
  {
    if (((mask & entry) == 0) || (((s16) entry) != ((s16) (object->field_130 & 0xf0ff))))
      goto call_a8;
    if (entry & 1)
      func_8013f2d8(object, index, arg);
    else
      func_8013f0c8(object, index, arg);
    return;
  }
  if (!(mask & entry))
  {
  call_c8:
    func_8013f2c8(object);
    return;
  }
  if (entry & 1)
    func_8013f2d8(object, index, arg);
  else
    func_8013f0c8(object, index, arg);
}
