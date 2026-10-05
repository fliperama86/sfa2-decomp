/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: same code, but the original keeps the second 16-bit read in $a1 and the kind in $v1 (registers swapped). */
/* Form found by automatic permutation search. */
void func_80123968(GameState *state)
{
  int a;
  int b;
  int who;
  int side;
  if (state->field_80 == 0)
  {
    who = *((s16 *) (&player_right.field_5c));
    a = who;
    b = *((s16 *) (&player_left.field_5c));
    who = player_left.kind;
    side = 1;
    if (a == b)
    {
      state->field_4c = 3;
    }
    else
    {
      if (a >= b)
      {
        side = 2;
        who = player_right.kind;
      }
      state->field_4c = side;
    }
    state->field_8f = who;
  }
}
