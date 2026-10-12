/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void set_position_from_parent_offset_table(Object *object)
{
  int idx;
  int v;
  ref_other.p = object->parent;
  idx = ref_other.p->frame->offset_index;
  idx = (object->table_index >> 1) + (idx & 1);
  if (ref_other.p->side != 0)
  {
    ref_first.p = *((Object **) 0x1f80012c);
  }
  else
  {
    ref_first.p = *((Object **) 0x1f80007c);
  }
  v = ((u16 *) ref_first.p)[idx];
  object->facing_side = ref_other.p->facing_side;
  if (object->facing_side != 0)
  {
    v = -v;
  }
  object->pos_x = v + ref_other.p->pos_x;
  v = ((u16 *) ref_first.p)[idx + 1];
  object->pos_y = ref_other.p->pos_y - v;
}
