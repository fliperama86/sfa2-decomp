/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012e600(Object *object)
{
  Box6 *box;
  SequenceStep *next;
  int t;
  set_side_flag(object);
  object->field_7e = 0;
  object->countdown_4 = 0;
  object->other_lock = 0;
  object->once_flag_2 = 0;
  object->done_flag = 0;
  object->flag_25a_set = 0;
  if (game_state.config->field_75 == 0)
  {
    game_state.config->field_75 = 0xff;
    ref_other.p = object->other;
    ref_other.p->effect_request = 8;
    add_table_bcd_score_to_player(0x28, (s8) ref_other.p->side);
  }
  if ((*((s16 *) (&object->countdown))) != 0)
  {
    if (((--(*((s16 *) (&object->countdown)))) == 0) && (object->mode_flag_2 != 0))
    {
      if (test_random_bit_in_table(object, table_8017d718))
      {
        object->field_15b = 0;
        object->field_2a3 = 1;
      }
    }
    else
      if ((object->input_bits_2 & 0xa000) && (object->pressed_bits & 0x3c))
    {
      object->field_15b = 0;
      object->field_2a3 = 1;
    }
  }
  if (object->side == 0)
  {
    box = boxes_124_right[object->kind];
  }
  else
  {
    box = boxes_74_left[object->kind];
  }
  ref_other.p = object->other;
  box += ref_other.p->frame->box_table_index;
  if (object->field_261 == 0)
  {
    t = box->origin;
    if (ref_other.p->facing_side != 0)
    {
      t = -t;
    }
    object->pos_x = ref_other.p->pos_x + t;
    object->pos_y = ref_other.p->pos_y - box->origin_y;
  }
  object->field_17a = 0;
  t = box->extent;
  if (t & 0x80)
  {
    object->field_17a = 0xff;
  }
  t = t & 3;
  object->facing_side = t ^ ref_other.p->facing_side;
  if (object->side != 0)
    next = seqs_118_right[0];
  else
    next = seqs_68_left[0];
  next += box->extent_y;
  if (next != object->sequence)
  {
    object->sequence_changed_flag = 1;
    object->sequence = next;
  }
  object->frame = object->frames + object->sequence->frame_index;
  object->step_flags = 0;
  push_both_objects_inside_bounds(object);
}
