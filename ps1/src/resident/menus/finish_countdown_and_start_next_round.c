/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void finish_countdown_and_start_next_round(GameState *state)
{
  Entity *entity = (Entity *)state;
  int s0;
  int t;
  u8 v;
  if ((u8)is_pad_pressed_for_leader_bits_or_mode() == 0)
  {
    t = entity->countdown_timer;
    t--;
    entity->countdown_timer = t;
    s0 = ((short) t) >= 0;
    if (s0)
    {
      return;
    }
  }
  ((HudBig *)screen_state)->step_count = 0;
  entity->field_c8 = 0;
  ((HudBig *)screen_state)->state_index = 0;
  entity->countdown_timer = 0;
  ((HudBig *)screen_state)->substate_index = 0;
  entity->field_cc = 0;
  entity->field_4d = 0;
  entity->field_4e = 0;
  entity->table_compare_value = 0;
  entity->ready_flag = 0;
  entity->active_flag = 1;
  entity->field_6a = 1;
  entity->field_4a = 0x3c;
  entity->field_49 = entity->field_13;
  stop_sep_by_index(0, 3);
  stop_sep_by_index(1, 3);
  stop_all_keys_clear_table();
  player_left.field_01 = 0;
  player_right.field_01 = 0;
  if (entity->field_45 != 0)
  {
    v = entity->field_40;
    data_801a6938 = 0;
    data_801abf08 = v;
    update_pair_selection_or_reset_by_mode_flag_a(entity);
  }
  else
    if ((entity->a9_flag != 0) || (entity->field_87 != 0))
  {
    if (entity->mode_flag_a == 0)
    {
      advance_hud_step_and_reset_entity_fields(entity);
    }
    else
    {
      update_pair_selection_or_reset_by_mode_flag_a(entity);
    }
  }
  else
  {
    s0 = entity->field_48 - 1;
    if (entity->pass_count == (s0 * 2))
    {
      fill_zero_entries_with_8_up_to_count(&player_left, s0);
      fill_zero_entries_with_8_up_to_count((&player_left) + 1, s0);
    }
    clear_arrays_and_init_pools_and_sprite_tables();
    call_handler_by_kind_index_a();
    entity->active_flag_2 = 1;
    entity->pass_count++;
    if (kind_handlers_enabled != 0)
    {
      set_capped_value_and_ot_indices(entity);
    }
    init_hud_primitives();
    spawn_position_table_objects_and_list_objects();
  }
}
