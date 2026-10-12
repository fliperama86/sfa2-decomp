/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (2 slots): the second load of box->field_11 is scheduled after the load of
   field_7e here and before it in the original. */
/* Form found by automatic permutation search. */
void apply_hit_box_to_target(Object *a, Object *b)
{
  Box32 *box;
  u8 t;
  s16 i;
  if (a->mode_id == 0)
  {
    box = &a->wide_boxes[a->frame->active];
  }
  else
  {
    box = ptr_8019040c;
  }
  if (a->mode_id == 0)
  {
    a->hit_skip_flag = 0;
  }
  i = box->box_type;
  if (a->active_state != 0)
  {
    a->table_bits = table_80176fe0[i];
  }
  else
  {
    a->table_bits = table_80176fc0[i];
  }
  if (a->mode_id == 0)
  {
    a->kind_timer = table_80177000[a->kind];
  }
  a->active_flag = 1;
  data_80188f34 = 0;
  b->state_0 = 1;
  b->state_1 = 1;
  b->state_2 = 0;
  b->state_3 = 0;
  b->field_62 = box->hit_param_low & 0xf;
  if (b->field_295 == 0)
  {
    b->field_295 = a->field_49;
  }
  if (((box->box_type != 0x1a) && ((box->box_flags & 0x80) == 0)) && (a->active_state != 0))
  {
    b->field_62 = 0;
  }
  b->vec_table_index = box->field_0a;
  if (a->field_49 != 0)
  {
    b->vec_table_index = box->field_19;
  }
  b->facing_flag = a->facing_side;
  if (box->facing_flags & 0x80)
  {
    b->facing_flag = a->pos_x < b->pos_x;
  }
  else
  {
    b->facing_flag = box->facing_flags ^ b->facing_flag;
  }
  if (a->sound_side != 0)
  {
    b->hit_box_index_b = box->hit_flags;
  }
  else
  {
    b->hit_box_index_a = box->hit_flags;
  }
  if (a->mode_id != 8)
  {
    if (a->sound_side != 0)
    {
      b->hit_count_b = b->hit_count_b + 1;
    }
    else
    {
      b->hit_count_a = b->hit_count_a + 1;
    }
  }
  if (a->mode_flag == 0)
  {
    i = box->field_11;
    b->field_6b = box->field_11 & 0x1f;
    t = i;
    if (a->field_7e != 0)
    {
      t = t >> 2;
    }
    a->field_6b = t;
  }
  if ((((s16) b->health) < 0) || (b->frame->handler_index == 0))
  {
    dispatch_hit_handler_by_box_type(a, b, box);
  }
  else
  {
    call_frame_handler_clear_action_timer(a, b, box);
  }
}
