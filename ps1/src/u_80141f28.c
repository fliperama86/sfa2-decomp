/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void add_to_meter_clamped(Object *object, short delta)
{
  s16 old;
  if (object->mode_flag != 0)
  {
    return;
  }
  if ((object->cursor_timer_b != 0) && (((s16) delta) >= 0))
  {
    return;
  }
  if (((s16) delta) == 0)
  {
    return;
  }
  if (((s16) delta) > 0)
  {
    if (game_state.pause_flag_2 != 0)
    {
      return;
    }
    if (object->field_d8 != 0)
    {
      delta = ((s16) delta) >> 2;
      if (delta == 0)
      {
        delta = 1;
      }
    }
  }
  old = object->meter;
  object->meter = delta + object->meter;
  if (((s16) object->meter) < 0)
  {
    object->meter = 0;
  }
  else
    if (object->field_d8 != 0)
  {
    if (((s16) object->meter) >= 0x30)
    {
      object->meter = 0x30;
    }
  }
  else
    if (((s16) object->meter) >= 0x90)
  {
    object->meter = 0x90;
  }
  if ((((s16) object->meter) != old) && (((s16) object->meter) >= 0x90))
  {
    play_sound_at(0, 0, 0x325);
  }
}
