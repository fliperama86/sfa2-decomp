/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_801529f4(Object *object)
{
  int idx;
  int v;
  ref_other.p = object->field_3c;
  idx = ref_other.p->frame->field_0a;
  idx = (object->field_03 >> 1) + (idx & 1);
  if (ref_other.p->side != 0)
  {
    ref_first.p = *((Object **) 0x1f80012c);
  }
  else
  {
    ref_first.p = *((Object **) 0x1f80007c);
  }
  v = ((u16 *) ref_first.p)[idx];
  object->field_0b = ref_other.p->field_0b;
  if (object->field_0b != 0)
  {
    v = -v;
  }
  object->pos_x = v + ref_other.p->pos_x;
  v = ((u16 *) ref_first.p)[idx + 1];
  object->pos_y = ref_other.p->pos_y - v;
}
