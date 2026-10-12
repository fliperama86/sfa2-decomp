/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void decode_input_word_into_object_fields(Object *object)
{
  s16 v;
  u16 *new_var;
  object->hit_flag_a = 0;
  object->attack_param_d = 0;
  object->direction_flag = 0;
  if (data_80189464 & 2)
  {
    object->hit_flag_a = 1;
  }
  if (data_80189464 & 4)
  {
    object->attack_param_d = 1;
  }
  if (data_80189464 & 8)
  {
    object->direction_flag = 1;
  }
  if (data_80189464 & 1)
  {
    func_8014de20(object);
    return;
  }
  new_var = &data_80189464;
  v = ((s16) (*new_var)) >> 4;
  data_80189464 = v;
  object->input_dir_0 = v & 0xf;
  v = ((s16) data_80189464) >> 4;
  data_80189464 = v;
  object->input_dir_second = v & 0xf;
  v = ((s16) data_80189464) >> 4;
  data_80189464 = v;
  object->direction_index = v & 0xf;
}
