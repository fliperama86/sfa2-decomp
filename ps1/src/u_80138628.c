/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: set_table_columns_by_win_counts (original keeps only the left cf address in a
   register), adjust_counter_c_by_table_and_random (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void adjust_counter_c_by_table_and_random(GameState *state)
{
  unsigned int new_var;
  new_var = table_6d08[state->table_index_2];
  counter_c -= new_var;
  new_var = table_6d10[get_random_byte() & 0x1f];
  counter_c += 8 - new_var;
}
