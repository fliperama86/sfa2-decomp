/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void add_round_score_to_other_side(Object *o)
{
  struct Object *new_var;
  new_var = o;
  if (new_var->mode_flag_2 == 0)
  {
    u32 i = new_var->round_count;
    if (i >= 100)
    {
      i = 99;
    }
    new_var = new_var->other;
    add_bcd_score_to_player(table_80181380[i], (s8) new_var->side);
  }
}
