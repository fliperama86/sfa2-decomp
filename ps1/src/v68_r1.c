/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Exact. k is an s16 copy of the loaded byte m: the copy keeps the compare register apart from the index register. */
/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012c100(Object *object)
{
  s16 k;
  int m;
  Object *new_var;
  if ((object->kind == 10) && (object->kind10_flag != 0))
  {
    object->kind10_flag = 0;
    if (object->side != 0)
    {
      func_80136ed0();
    }
    else
    {
      func_80136e90();
    }
    func_801372c8(object);
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
    func_8012c7b4(object);
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
    func_80138be4(&game_state, new_var);
    ref_other.p = new_var->other;
    ref_other.p->field_28c = 0;
    ref_other.p->damage_countdown = 0;
    ref_other.p->reference_count = 0;
    if (game_state.config->field_75 == 0)
    {
      game_state.config->field_75 = 0xff;
      ref_other.p = new_var->other;
      ref_other.p->effect_request = 8;
      func_80155d4c(0x28, (s8) ref_other.p->side);
    }
    m = new_var->hit_type;
    k = m;
    if ((((k <= (3 - 1)) || (k == 6)) || (((unsigned) (m - 8)) < 2)) || (k == 0xe))
    {
      func_8012c5cc(new_var);
    }
    else
      if (k == 0x1a)
    {
      func_8012c6a4(new_var);
    }
    else
      if ((k == 0x12) || (k == 0x18))
    {
      func_8012c72c(new_var);
    }
    else
      if (k == 0x19)
    {
      func_8012cc00(new_var);
    }
    else
    {
      new_var->pair_value = pairs_80170074[m].first;
      new_var->field_60 = pairs_80170074[m].second;
      if ((k == 0x14) || (k == 0x16))
      {
        func_8012ca04(new_var);
      }
      else
        if ((k == 0x15) || (k == 0x17))
      {
        func_8012cb3c(new_var);
      }
      else
        if (k == 0xb)
      {
        new_var->active_state = 0xff;
        new_var->field_69 = new_var->field_69 + 1;
        func_801308c4(new_var, new_var->pair_value);
      }
      else
      {
        new_var->active_state = 0xff;
        new_var->field_69 += 1;
        if ((new_var->field_60 == 2) && (k == 0xc))
        {
          func_8012c990(new_var);
        }
        else
          if ((new_var->field_60 == 2) && (k == 0xd))
        {
          func_8012cac8(new_var);
        }
        else
          if ((new_var->field_60 == 2) && (k == 0x11))
        {
          func_8012cc6c(new_var);
        }
        else
        {
          func_801308c4(new_var, new_var->pair_value);
        }
      }
    }
  }
}
