/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void place_object_at_parent_with_offset_table(Object *object)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  Object *parent;
  s32 x;
  Object *new_var3;
  s32 *new_var2;
  s32 y;
  if (object->variant_index_2 != 0)
  {
    object->variant_index_2--;
  }
  parent = object->parent;
  x = (s16) object->health;
  object->facing_side = parent->facing_side;
  y = (s16) object->y_offset;
  if (object->facing_side != 0)
  {
    x = -x;
  }
  x += parent->pos_x;
  new_var3 = object;
  y = parent->pos_y - y;
  x = (((u32) x) >> 16) + (x << 16);
  new_var2 = (s32 *) (&object->pos_x_frac);
  *new_var2 = x + offsets_cc28[new_var3->variant_index_2].x;
  y = (((u32) y) >> 16) + (y << 16);
  *((s32 *) (&object->pos_y_frac)) = y + offsets_cc28[object->variant_index_2].y;
}
