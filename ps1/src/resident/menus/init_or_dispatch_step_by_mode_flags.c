/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void init_or_dispatch_step_by_mode_flags(GameState *state)
{
  Entity *entity = (Entity *)state;
  char new_var;
  if (data_8018f5a0->step_index == 0)
  {
    init_sprite_tables();
    data_8018f5a0->step_index++;
    new_var = 1;
    if (((data_8016e68b != 0) || (entity->mode_flag_a != 0)) || (entity->mode_flag_b != 0))
    {
      close_sounds_and_load_command_4(new_var);
    }
    else
    {
      close_sounds_and_load_command_4(0);
    }
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    data_8018f5a0->field_54 = 0;
  }
  else
  {
    if ((entity->mode_flag_a == 0) && (entity->mode_flag_b == 0))
    {
      if (data_8016e68b != 0)
      {
        func_801b0000(entity);
      }
      else
      {
        func_80010000(entity);
      }
    }
    if ((entity->mode_flag_a != 0) && (entity->mode_flag_b == 0))
    {
      func_801b2194(entity);
    }
    if ((entity->mode_flag_a == 0) && (entity->mode_flag_b != 0))
    {
      func_801b4a88(entity);
    }
  }
  if (data_8018f5a0->step_index == 2)
  {
    data_8018f5a0->step_index = 0;
    data_8018f5a0->phase_index++;
  }
}
