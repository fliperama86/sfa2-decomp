/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search. */
void decode_input_word_into_object_fields(Object *object)
{
  s16 v;
  u16 *new_var;
  object->hit_flag_a = 0;
  object->attack_param_d = 0;
  object->direction_flag = 0;
  if (script_work_value & 2)
  {
    object->hit_flag_a = 1;
  }
  if (script_work_value & 4)
  {
    object->attack_param_d = 1;
  }
  if (script_work_value & 8)
  {
    object->direction_flag = 1;
  }
  if (script_work_value & 1)
  {
    unpack_shifted_nibbles_into_direction_fields(object);
    return;
  }
  new_var = &script_work_value;
  v = ((s16) (*new_var)) >> 4;
  script_work_value = v;
  object->input_dir_0 = v & 0xf;
  v = ((s16) script_work_value) >> 4;
  script_work_value = v;
  object->input_dir_second = v & 0xf;
  v = ((s16) script_work_value) >> 4;
  script_work_value = v;
  object->direction_index = v & 0xf;
}
