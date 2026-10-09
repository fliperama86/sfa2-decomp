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
  func_8013786c(object);
  object->field_7e = 0;
  object->field_263 = 0;
  object->field_27b = 0;
  object->field_258 = 0;
  object->field_259 = 0;
  object->field_25a = 0;
  if (game_state.config->field_75 == 0)
  {
    game_state.config->field_75 = 0xff;
    ref_other.p = object->other;
    ref_other.p->field_bd = 8;
    func_80155d4c(0x28, (s8) ref_other.p->side);
  }
  if ((*((s16 *) (&object->field_46))) != 0)
  {
    if (((--(*((s16 *) (&object->field_46)))) == 0) && (object->field_cd != 0))
    {
      if (func_8014a170(object, table_8017d718))
      {
        object->field_15b = 0;
        object->field_2a3 = 1;
      }
    }
    else
      if ((object->field_130 & 0xa000) && (object->field_134 & 0x3c))
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
  box += ref_other.p->frame->field_0b;
  if (object->field_261 == 0)
  {
    t = box->origin;
    if (ref_other.p->field_0b != 0)
    {
      t = -t;
    }
    object->pos_x = ref_other.p->pos_x + t;
    object->pos_y = ref_other.p->pos_y - box->field_02;
  }
  object->field_17a = 0;
  t = box->extent;
  if (t & 0x80)
  {
    object->field_17a = 0xff;
  }
  t = t & 3;
  object->field_0b = t ^ ref_other.p->field_0b;
  if (object->side != 0)
    next = seqs_118_right[0];
  else
    next = seqs_68_left[0];
  next += box->field_05;
  if (next != object->sequence)
  {
    object->field_80 = 1;
    object->sequence = next;
  }
  object->frame = object->frames + object->sequence->frame_index;
  object->field_3a = 0;
  func_80130c80(object);
}
