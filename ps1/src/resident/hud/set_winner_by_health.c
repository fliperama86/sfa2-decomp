/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: same code, but the original keeps the second 16-bit read in $a1 and the kind in $v1 (registers swapped). */
/* Form found by automatic permutation search. */
void set_winner_by_health(GameState *state)
{
  int a;
  int b;
  int who;
  int side;
  if (state->field_80 == 0)
  {
    who = *((s16 *) (&player_right.health));
    a = who;
    b = *((s16 *) (&player_left.health));
    who = player_left.kind;
    side = 1;
    if (a == b)
    {
      state->leader_bits = 3;
    }
    else
    {
      if (a >= b)
      {
        side = 2;
        who = player_right.kind;
      }
      state->leader_bits = side;
    }
    state->winner = who;
  }
}
