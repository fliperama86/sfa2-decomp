/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void init_or_dispatch_step_by_mode_flags(GameState *state)
{
  Entity *entity = (Entity *)state;
  char new_var;
  if (screen_state->step_index == 0)
  {
    init_sprite_tables();
    screen_state->step_index++;
    new_var = 1;
    if (((data_8016e68b != 0) || (entity->mode_flag_a != 0)) || (entity->mode_flag_b != 0))
    {
      close_sounds_and_load_command_4(new_var);
    }
    else
    {
      close_sounds_and_load_command_4(0);
    }
    screen_state->substep_index = 0;
    screen_state->step_index_2 = 0;
    screen_state->pass_index = 0;
    screen_state->field_54 = 0;
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
  if (screen_state->step_index == 2)
  {
    screen_state->step_index = 0;
    screen_state->phase_index++;
  }
}
