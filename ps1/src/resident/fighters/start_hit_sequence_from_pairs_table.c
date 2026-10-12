/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search. */
void start_hit_sequence_from_pairs_table(Object *object)
{
  int k = object->hit_type;
  unsigned char new_var;
  object->pair_value = pairs_80170074[k].first;
  object->field_60 = pairs_80170074[k].second;
  new_var = k;
  if ((new_var == 0x14) || (new_var == 0x16))
  {
    update_hit_reaction_type_16_sound_spawn(object);
  }
  else
    if ((new_var == 0x15) || (new_var == 0x17))
  {
    update_hit_reaction_type_17_sound_spawn(object);
  }
  else
    if (new_var == 0xb)
  {
    object->active_state = 0xff;
    object->field_69++;
    set_sequence_by_side_and_mode_2(object, object->pair_value);
  }
  else
  {
    object->active_state = 0xff;
    object->field_69++;
    if (object->field_60 == 2)
    {
      if (new_var == 0xc)
      {
        start_sequence_with_variant_zero_spawns_once(object);
        return;
      }
      if (new_var == 0xd)
      {
        start_sequence_with_variant_two_spawns_once(object);
        return;
      }
      if (new_var == 0x11)
      {
        spawn_three_objects_and_set_sequence_x21(object);
        return;
      }
    }
    set_sequence_by_side_and_mode_2(object, object->pair_value);
  }
}
