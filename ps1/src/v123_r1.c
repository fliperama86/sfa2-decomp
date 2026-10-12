/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void update_airborne_motion_simple_or_land(Object *object)
{
  if ((u8) apply_velocity_and_check_ground(object))
  {
    if ((s16) object->step_flags >= 0)
    {
      update_sequence(object);
    }
    else
    {
      object->state_3 = 1;
      object->active_flag_3 = 0;
      update_sequence(object);
    }
  }
  else
  {
    object->pos_y = object->ground_y;
    object->active_state = 0;
    object->active_flag_3 = 0;
    play_object_sound_by_kind(object);
    if (object->mode_flag_2 == 0 && find_nearby_enemy_and_get_flag(object))
    {
      enter_state_2_6_by_button_input(object);
    }
    else
    {
      object->state_3 = 2;
      set_sequence_by_side(object, 0x11);
    }
  }
}
