/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void update_health_bars(Bars *o, Object *a, Object *b)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[16];
  s16 ha;
  s16 hb;
  int i;
  int ta;
  int tb;
  ta = (s16) a->health;
  ha = ta;
  if (ta < 0)
  {
    ha = 0;
  }
  tb = (s16) b->health;
  hb = tb;
  if (tb < 0)
  {
    hb = 0;
  }
  if (((s16) ha) < ((s16) a->y_offset))
  {
    if ((game_state.tick_count & 3) == 0)
    {
      a->y_offset = a->y_offset - 1;
    }
  }
  if (((s16) hb) < ((s16) b->y_offset))
  {
    if ((game_state.tick_count & 3) == 0)
    {
      b->y_offset = b->y_offset - 1;
    }
  }
  if (((s16) ha) > ((s16) a->y_offset))
  {
    if ((game_state.tick_count & 3) == 0)
    {
      a->y_offset = a->y_offset + 1;
    }
  }
  if (((s16) hb) > ((s16) b->y_offset))
  {
    if ((game_state.tick_count & 3) == 0)
    {
      b->y_offset = b->y_offset + 1;
    }
  }
  for (i = 0; i < 2; i++)
  {
    Bars *out = (Bars *) (((u8 *) o) + (i * 0x10));
    out->first_bar_rest = 0x98 - ha;
    out->first_bar_value = ha;
    out->first_bar_trail_rest = 0x98 - a->y_offset;
    out->first_bar_trail_width = a->y_offset - ha;
    out->second_bar_origin = 0xd8;
    out->second_bar_value = hb;
    out->second_bar_end = hb + 0xd8;
    out->second_bar_trail_width = b->y_offset - hb;
  }

}
