/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search. */
void func_8012da2c(Object *object)
{
  int v;
  int w;
  int t;
  unsigned short new_var;
  u8 index;
  if (((s16) object->step_flags) >= 0)
  {
    update_sequence(object);
    return;
  }
  if (object->field_163 == 0)
  {
    object->state_0 = 1;
    object->state_1 = 1;
    object->state_2 = 2;
    object->state_3 = 0;
    object->field_15b = 0;
    object->step_applied_flag = 0;
    if ((game_state.versus_flag != 0) && (game_state.mode != (object->side + 1)))
    {
      object->field_1a2 = 0;
    }
    return;
  }
  w = object->count_limit;
  object->state_0 = 1;
  object->state_1 = 0;
  object->state_2 = 9;
  object->state_3 = 0;
  object->countdown_timer_2 = 0;
  object->accum_count = 0;
  object->field_163 = 0;
  object->count_limit = w + 5;
  add_round_score_to_other_side(object);
  object->field_69 = 0;
  object->round_count = 0;
  if ((game_state.versus_flag != 0) && (game_state.mode != (object->side + 1)))
  {
    v = object->field_1a1 + 0xff;
    object->field_1a1 = v;
    if (v & 0x80)
    {
      object->field_19f = 0;
    }
  }
  object->field_170 = 0;
  object->field_157 = 0;
  t = get_random_byte() & 0x1e;
  index = t;
  new_var = index;
  play_side_sound_at(object, object->side, 2);
  if (object->kind == 9)
  {
    object->countdown = table_80171194[new_var];
  }
  else
  {
    object->countdown = table_80171174[new_var];
  }
  set_sequence_by_side_and_mode_2(object, 0x1a);
}
