/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (52 slots, 704 vs 716 bytes): the original keeps copies of a and b
   in t0 and t2 (move at entry) and the pointer ref_other.p in t1, and reuses a1
   for delta; this build coalesces x/bb with the parameters, so registers differ
   and the frame of moves is 12 bytes short. The 7/8 test as two separate ifs
   now matches. Tried: locals x/bb/t, in-place a/b, ternary, local pointer to
   ref_other.p (CSEs to the stored value), switch, || versus && forms. */
/* Form found by automatic permutation search. */
int func_8013f5f8(Object *object, int a, int b)
{
  FrameRecord *rec;
  Box6 *box;
  int delta;
  short diff;
  int bb;
  diff = a;
  bb = b;
  ref_other.p = object->other;
  rec = ref_other.p->frame;
  if (rec->handler_index != 0)
  {
    return 0;
  }
  if (ref_other.p->other_lock != 0)
  {
    return 0;
  }
  if (game_state.config->mode_flag_c != 0)
  {
    return 0;
  }
  if (ref_other.p->active_state != 0)
  {
    return 0;
  }
  if (((rec->box_a | rec->box_b) | rec->box_c) == 0)
  {
    return 0;
  }
  if (object->facing_side != 0)
  {
    diff = -a;
  }
  diff = diff + ((u16) object->pos_x);
  box = &((Box6 *) ref_other.p->unknown_148)[rec->box_index];
  delta = ((u16) box->origin) - box->extent;
  if ((*((u16 *) (&object->state_0))) == 1)
  {
    if (object->state_2 == 7)
    {
      goto skip;
    }
    if (object->state_2 == 8)
    {
      goto skip;
    }
  }
  if (object->hold_timer != 0)
  {
    bb = 4;
    bb = b + bb;
  }
  skip:
  if (ref_other.p->facing_side != 0)
  {
    delta = -delta;
  }

  diff = diff - (delta + ((u16) ref_other.p->pos_x));
  if (((s16) diff) < 0)
  {
    diff = -diff;
  }
  if (((s16) bb) < ((s16) diff))
  {
    return 0;
  }
  if (ref_other.p->field_7e != 0)
  {
    ref_other.p->meter = ref_other.p->meter - 0x30;
    if (((s16) ref_other.p->meter) < 0)
    {
      ref_other.p->meter = 0;
    }
  }
  ref_other.p->state_0 = 1;
  ref_other.p->state_1 = 3;
  ref_other.p->state_2 = 0;
  ref_other.p->state_3 = 0;
  ref_other.p->status_flag = 0xff;
  object->status_flag = 1;
  ref_other.p->field_15b = 1;
  ref_other.p->countdown = 4;
  object->field_28c = 0;
  object->damage_countdown = 0;
  object->reference_count = 0;
  ref_other.p->other = object;
  func_80138ac8((GameState *)game_state.config, object);
  object->active_flag_3 = 0;
  ref_other.p->active_flag_3 = 0;
  object->field_225 = 0;
  ref_other.p->field_225 = 0;
  ref_other.p->field_180 = 0;
  ref_other.p->cursor_timer_a = 0;
  ref_other.p->cursor_timer_b = 0;
  return 1;
}
