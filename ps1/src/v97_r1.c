/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, int index, int unused);
void func_8013f2d8(Object *object, int index, int unused);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* Exact. Decided by: params taken as u8 (the table's type) and copied to int locals that are masked (index at the top,
   arg in each path before the shared f2a8 label), the mask == 0 early goto to
   the shared f2c8 call, and entry declared unsigned (fixes the and operand order). */
void func_8013e2b0(Object *object, u8 index_in, u8 arg_in)
{
  unsigned entry;
  int mask;
  int arg;
  int index = index_in;
  index &= 0xff;
  object->slots[index].field_04--;
  if (object->slots[index].field_04 == 0)
  {
    arg = arg_in & 0xff;
  call_a8:
    func_8013f2a8(object, index, arg);
    return;
  }
  arg = arg_in & 0xff;
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
