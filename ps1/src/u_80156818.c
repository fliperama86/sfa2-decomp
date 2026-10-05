/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_80156818(void *a, Object *object)
{
  u8 high;
  object->field_a9 = object->field_a9 + 1;
  high = object->field_120;
  object->field_aa = 0;
  object->field_b0 = 0;
  object->field_b2 = 0;
  object->field_b4 = 0;
  if (game_state.field_2b8 >= high)
  {
    game_state.field_2b8 = high;
    game_state.field_2bf = object->side;
  }
  high = object->field_121;
  if (game_state.field_2ba >= high)
  {
    game_state.field_2ba = high;
  }
  object->field_120 = 0;
  object->field_121 = 0;
}
