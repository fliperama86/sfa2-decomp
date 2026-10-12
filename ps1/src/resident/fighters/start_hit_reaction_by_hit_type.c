/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Exact. k is an s16 copy of the loaded byte m: the copy keeps the compare register apart from the index register. */
/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void start_hit_reaction_by_hit_type(Object *object)
{
  s16 k;
  int m;
  Object *new_var;
  if ((object->kind == 10) && (object->kind10_flag != 0))
  {
    object->kind10_flag = 0;
    if (object->side != 0)
    {
      store_player_right_image_rows_from_field();
    }
    else
    {
      store_player_left_image_rows_from_field();
    }
    queue_image_row_upload_job(object);
  }
  object->active_flag_3 = 0;
  object->field_243 = 0;
  object->step_applied_flag = 0;
  object->field_225 = 0;
  object->state_3 = object->state_3 + 1;
  data_80198226 = 0;
  data_801985ba = 0;
  object->field_7e = 0;
  object->countdown_4 = 0;
  object->other_lock = 0;
  new_var = object;
  object->field_29c = 0;
  object->cursor_timer_b = 0;
  object->cursor_timer_a = 0;
  object->frame_count = 0;
  object->field_180 = 0;
  set_side_field_0d(object);
  if (object->hit_type == 0xff)
  {
    select_sequence_by_action_timer_and_active_state(object);
  }
  else
  {
    if ((game_state.versus_flag != 0) && (game_state.mode != (object->side + 1)))
    {
      object->field_19f = object->round_count;
      if (object->field_19f == 0)
      {
        new_var->hit_count_2 = 0;
      }
      new_var->field_1a1 = 0x13;
      new_var->field_1a2 = 1;
    }
    new_var->round_count = new_var->round_count + 1;
    add_one_to_ec4_if_other_in_state(&game_state, new_var);
    ref_other.p = new_var->other;
    ref_other.p->field_28c = 0;
    ref_other.p->damage_countdown = 0;
    ref_other.p->reference_count = 0;
    if (game_state.config->field_75 == 0)
    {
      game_state.config->field_75 = 0xff;
      ref_other.p = new_var->other;
      ref_other.p->effect_request = 8;
      add_table_bcd_score_to_player(0x28, (s8) ref_other.p->side);
    }
    m = new_var->hit_type;
    k = m;
    if ((((k <= (3 - 1)) || (k == 6)) || (((unsigned) (m - 8)) < 2)) || (k == 0xe))
    {
      set_hit_sequence_by_hit_type_and_move_other(new_var);
    }
    else
      if (k == 0x1a)
    {
      start_hit_sequence_base_7_or_4(new_var);
    }
    else
      if ((k == 0x12) || (k == 0x18))
    {
      set_sequence_from_hit_type_base_and_capped_step(new_var);
    }
    else
      if (k == 0x19)
    {
      start_hit_reaction_with_three_spawns(new_var);
    }
    else
    {
      new_var->pair_value = pairs_80170074[m].first;
      new_var->field_60 = pairs_80170074[m].second;
      if ((k == 0x14) || (k == 0x16))
      {
        update_hit_reaction_type_16_sound_spawn(new_var);
      }
      else
        if ((k == 0x15) || (k == 0x17))
      {
        update_hit_reaction_type_17_sound_spawn(new_var);
      }
      else
        if (k == 0xb)
      {
        new_var->active_state = 0xff;
        new_var->field_69 = new_var->field_69 + 1;
        set_sequence_by_side_and_mode_2(new_var, new_var->pair_value);
      }
      else
      {
        new_var->active_state = 0xff;
        new_var->field_69 += 1;
        if ((new_var->field_60 == 2) && (k == 0xc))
        {
          start_sequence_with_variant_zero_spawns_once(new_var);
        }
        else
          if ((new_var->field_60 == 2) && (k == 0xd))
        {
          start_sequence_with_variant_two_spawns_once(new_var);
        }
        else
          if ((new_var->field_60 == 2) && (k == 0x11))
        {
          spawn_three_objects_and_set_sequence_x21(new_var);
        }
        else
        {
          set_sequence_by_side_and_mode_2(new_var, new_var->pair_value);
        }
      }
    }
  }
}
