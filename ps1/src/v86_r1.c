/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the original computes both 0x30/0x18 masks branch-free and keeps
   the index sum in v1/a2; here the 0x18 term compiles to a branch (4 bytes
   longer). Forcing it branch-free moves object out of a0. */
int func_801425d8(Object *object, s16 mask)
{
  s16 i;
  int delta;
  int d0;
  int shifted;
  int pos;
  int idx;
  int t;
  if (object->side)
  {
    game_state.field_308 = (Row *) metrics_right;
  }
  else
  {
    game_state.field_308 = (Row *) metrics_left;
  }
  t = game_state.field_358->field_12a >> 2;
  idx = (-(game_state.field_358->field_129 != 0)) & 0x30;
  i = (-(game_state.field_358->field_128 != 0)) & 0x18;
  idx = idx + i;
  d0 = game_state.field_308->values[t + idx];
  if (!game_state.field_358->field_0b)
  {
    d0 = -d0;
  }
  pos = (s16) (d0 + game_state.field_358->pos_x);
  i = 0;
  while (i < 12)
  {
    shifted = mask >> i;
    mask = shifted;
    if (shifted)
    {
      delta = game_state.field_30c[2].first;
      game_state.field_30c++;
      if (!object->field_0b)
      {
        delta = -delta;
      }
      if (((s16) (delta + object->pos_x)) < pos)
      {
        t = shifted;
        mask = t & (~(1 << i));
      }
    }
    i++;
  }

}
