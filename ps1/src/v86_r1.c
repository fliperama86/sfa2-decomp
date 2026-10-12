/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the original computes both 0x30/0x18 masks branch-free and keeps
   the index sum in v1/a2; here the 0x18 term compiles to a branch (4 bytes
   longer). Forcing it branch-free moves object out of a0. */
int clear_mask_bits_below_target_position(Object *object, s16 mask)
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
    game_state.row_ptr = (Row *) metrics_right;
  }
  else
  {
    game_state.row_ptr = (Row *) metrics_left;
  }
  t = game_state.other_object->direction_index >> 2;
  idx = (-(game_state.other_object->input_dir_second != 0)) & 0x30;
  i = (-(game_state.other_object->input_dir_0 != 0)) & 0x18;
  idx = idx + i;
  d0 = game_state.row_ptr->values[t + idx];
  if (!game_state.other_object->facing_side)
  {
    d0 = -d0;
  }
  pos = (s16) (d0 + game_state.other_object->pos_x);
  i = 0;
  while (i < 12)
  {
    shifted = mask >> i;
    mask = shifted;
    if (shifted)
    {
      delta = game_state.pair_ptr[2].first;
      game_state.pair_ptr++;
      if (!object->facing_side)
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
