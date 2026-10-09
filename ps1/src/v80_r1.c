/* Reconstruction. Names/roles inferred, not original symbols.
   The int return type (v0 live at the exit) keeps the delay slot of the
   early-out branch empty, as in the original. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138838(GameState *state, int arg);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
int func_801384c4(GameState *state)
{
  u8 step;
  u8 *new_var;
  if (state->field_80 != 0)
  {
    func_80138588();
  }
  else
  {
    new_var = &player_right.field_ce;
    player_right.field_cf = (player_left.field_cf = 0x10);
    step = table_6cf8[func_80151184() & 0xf];
    if ((*new_var) != player_left.field_ce)
    {
      if (player_right.field_ce < player_left.field_ce)
      {
        player_right.field_cf += step;
      }
      else
      {
        player_left.field_cf += step;
      }
    }
  }
}
