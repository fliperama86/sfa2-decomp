/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int update_entity_countdowns_and_sounds(Entity *entity)
{
  Object *s0;
  int new_var;
  if ((entity->field_4d == 0) && (entity->field_116 == 0))
  {
    entity->countdown_timer_2--;
    if (entity->countdown_timer_2 == 0)
    {
      if (entity->nonzero_flag == 0)
      {
        set_other_marker_and_play_side_sound_3(entity->field_7c, entity);
      }
      else
      {
        set_other_marker_and_play_side_sound_3(s0 = &player_left, entity);
        s0 = (&player_left) + 1;
        set_other_marker_and_play_side_sound_3(s0, entity);
      }
    }
  }
  if ((entity->countdown_2 != 0) && (entity->countdown_2 == 0x1e))
  {
    entity->countdown_2--;
    if (entity->nonzero_flag == 0)
    {
      if (entity->mode_flag_b == 0)
      {
        data_80190474 = 1;
      }
      if (entity->field_4d == 0)
      {
        new_var = 0;
        s0 = entity->field_78;
        if (s0->mode_flag_2 == new_var)
        {
          play_sound_at(0, 0, 0x208);
        }
        else
        {
          play_sound_at(new_var, new_var, 0x209);
        }
      }
    }
  }
}
