/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_801384c4 (original keeps only the left cf address in a
   register), func_80138628 (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_80138628(GameState *state)
{
  unsigned int new_var;
  new_var = table_6d08[state->field_2d];
  counter_c -= new_var;
  new_var = table_6d10[func_80151184() & 0x1f];
  counter_c += 8 - new_var;
}
