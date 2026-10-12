/* Reconstruction. Names/roles inferred, not original symbols.
   The int return type (v0 live at the exit) keeps the delay slot of the
   early-out branch empty, as in the original. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
int set_table_columns_by_win_counts(GameState *state)
{
  u8 step;
  u8 *new_var;
  if (state->field_80 != 0)
  {
    clear_player_field_cf();
  }
  else
  {
    new_var = &player_right.win_count;
    player_right.table_column = (player_left.table_column = 0x10);
    step = table_6cf8[get_random_byte() & 0xf];
    if ((*new_var) != player_left.win_count)
    {
      if (player_right.win_count < player_left.win_count)
      {
        player_right.table_column += step;
      }
      else
      {
        player_left.table_column += step;
      }
    }
  }
}
