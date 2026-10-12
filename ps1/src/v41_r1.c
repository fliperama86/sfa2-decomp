/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Exact. The count is read into a local before the subtraction so the load order matches. */


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80129678(Object *object)
{
  unsigned short new_var;
  int cnt;
  new_var = 0;
  game_state.other_object = object->other;
  if (((((((game_state.config->mode_flag_b | game_state.config->mode_flag_c) | game_state.config->mode_flag_a) == new_var) && ((*((u16 *) (&object->state_0))) != 0x301)) && ((*((u16 *) (&object->state_0))) != 0x201)) && (object->field_165 == new_var)) && ((object->damage_countdown & 0x80) == new_var))
  {
    object->damage_countdown--;
    if (object->damage_countdown == new_var)
    {
      func_801297b8(object);
      return;
    }
    if (object->field_28c == new_var && (cnt = object->damage_countdown, (object->reference_count - cnt) >= 0x18))
    {
      object->field_28c = new_var;
      object->damage_countdown = new_var;
      object->reference_count = new_var;
      object->state_0 = 1;
      object->state_1 = 1;
      object->state_2 = new_var;
      object->state_3 = new_var;
      object->field_62 = 2;
      object->hit_type = 3;
      object->field_15b = 1;
      object->field_163 = 1;
      object->countdown_timer_2 = new_var;
      object->accum_count = new_var;
      object->facing_flag = game_state.other_object->facing_side;
      object->vec_table_index = new_var;
      object->field_01 = 1;
      play_sound_at(object, object->side ^ 1, 0x306);
    }
  }
}
