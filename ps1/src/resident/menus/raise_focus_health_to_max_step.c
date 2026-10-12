/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void raise_focus_health_to_max_step(GameState *g)
{
  u16 new_var;
  Object *o;
  s16 v;
  u16 n;
  if ((u8)is_pad_pressed_for_leader_bits_or_mode() == 0)
  {
    v = g->countdown_timer_2;
    v--;
    g->countdown_timer_2 = v;
    if (v >= 0)
    {
      return;
    }
  }
  if ((g->frame_counter & 3) == 0)
  {
    if ((((--g->countdown_3) & 0x80) == 0) && (((s16) (new_var = (o = g->focus_object)->health)) != 0x90))
    {
      new_var = o->health + 1;
      n = new_var;
      o->health = n;
      o->y_offset = n;
    }
    else
    {
      data_8018f5a0->step_index_2++;
      g->countdown_timer_2 = 0x5a;
      o = g->focus_object;
      o->saved_health = o->health;
    }
  }
}
