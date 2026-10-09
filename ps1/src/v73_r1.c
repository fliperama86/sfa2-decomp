/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80148220(Object *object)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  Object *parent;
  s32 x;
  Object *new_var3;
  s32 *new_var2;
  s32 y;
  if (object->field_a0 != 0)
  {
    object->field_a0--;
  }
  parent = object->field_3c;
  x = (s16) object->field_5c;
  object->field_0b = parent->field_0b;
  y = (s16) object->field_5e;
  if (object->field_0b != 0)
  {
    x = -x;
  }
  x += parent->pos_x;
  new_var3 = object;
  y = parent->pos_y - y;
  x = (((u32) x) >> 16) + (x << 16);
  new_var2 = (s32 *) (&object->field_10);
  *new_var2 = x + offsets_cc28[new_var3->field_a0].x;
  y = (((u32) y) >> 16) + (y << 16);
  *((s32 *) (&object->field_14)) = y + offsets_cc28[object->field_a0].y;
}
