/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Not exact: 2 slots differ. In the second block the original loads the
   constant 1 (ori v1) into the branch delay slot and then the constant 2
   (ori v0); this build emits them in the opposite order. Source reorderings
   of the stores moved the stores but not the two loads. */
void apply_damage_and_check_defeat(Object *object)
{
  int t;
  int new_var;
  t = object->damage_2;
  object->health = object->health - t;
  if (((s16) object->health) < 0)
  {
    object->state_0 = 1;
    object->state_1 = 1;
    new_var = 0;
    object->state_2 = new_var;
    object->state_3 = 0;
    t = 2;
    object->field_62 = t;
    object->hit_type = 7;
    object->field_15b = 1;
    game_state.other_object->field_167 = object->table_param;
    if (object->status_flag != 0)
    {
      game_state.other_object->state_0 = 1;
      game_state.other_object->state_1 = 1;
      game_state.other_object->state_2 = 1;
      game_state.other_object->state_3 = 0;
      game_state.other_object->vec_table_index = 0;
      game_state.other_object->field_60 = t;
      game_state.other_object->hit_type = 5;
      object->status_flag = 0;
      game_state.other_object->status_flag = 0;
      game_state.other_object->field_261 = 0;
      game_state.other_object->facing_flag = object->facing_side;
      game_state.other_object->facing_flag = game_state.other_object->facing_flag ^ 1;
      game_state.other_object->field_243 = 1;
    }
    t = object->table_param;
    t = (t - 12) * 2;
    game_state.other_object->field_255 = t;
    game_state.spawn_kind = 0;
    spawn_game_state_kind_object(object);
  }
  object->field_28c = object->field_28c - 1;
  if (object->field_28c & 0x80)
  {
    object->field_28c = 0;
    object->damage_countdown = 0;
    object->reference_count = 0;
    object->state_0 = 1;
    object->field_62 = 2;
    object->state_1 = 1;
    object->state_2 = 0;
    object->state_3 = 0;
    object->hit_type = 3;
    object->field_15b = 1;
    object->field_163 = 1;
    object->countdown_timer_2 = 0;
    object->accum_count = 0;
    object->facing_flag = game_state.other_object->facing_side;
    object->vec_table_index = 0;
    object->field_01 = 1;
    play_side_sound_at(object, object->side ^ 1, 0xf);
  }
  object->damage_countdown = object->reference_count;
}
