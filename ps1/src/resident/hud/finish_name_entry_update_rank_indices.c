/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void finish_name_entry_update_rank_indices(void *a, Object *object)
{
  u8 high;
  object->step_index = object->step_index + 1;
  high = object->score_rank;
  object->phase_index = 0;
  object->countdown_timer = 0;
  object->field_b2 = 0;
  object->countdown_timer_b = 0;
  if (game_state.left_rank_index >= high)
  {
    game_state.left_rank_index = high;
    game_state.field_2bf = object->side;
  }
  high = object->field_121;
  if (game_state.right_rank_index >= high)
  {
    game_state.right_rank_index = high;
  }
  object->score_rank = 0;
  object->field_121 = 0;
}
