/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int check_range_set_other_state_if_unlocked(Object *object, s16 a, s16 b, s16 c, u16 d)
{
  FrameRecord *frame;
  u8 new_var;
  unsigned short y;
  u8 x;
  int z;
  u8 new_var2;
  u8 w;
  ref_other.p = object->other;
  frame = ref_other.p->frame;
  new_var2 = frame->handler_index;
  y = ref_other.p->other_lock;
  new_var = (x = new_var2);
  w = ref_other.p->active_state;
  z = ref_other.p->field_163;
  if ((w == 0) ? (((new_var | (y | 1)) | z) != 0) : (((y | new_var) | z) != 0))
  {
    return 0;
  }
  return check_range_and_set_other_state(object, a, b, c, (s16) d) & 0xff;
}
