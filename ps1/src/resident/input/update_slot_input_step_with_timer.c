/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* The parameters are masked in the function, the index at the top and the
   argument in each path before the shared call. Also in this form from the
   search: the early goto to the shared call when the mask is zero, and entry
   declared unsigned. */
void update_slot_input_step_with_timer(Object *object, int index, int arg)
{
  unsigned entry;
  int mask;
  index &= 0xff;
  object->slots[index].timer--;
  if (object->slots[index].timer == 0)
  {
    arg &= 0xff;
  call_a8:
    reset_slot_stage_clear_result(object, index, arg);
    return;
  }
  arg &= 0xff;
  entry = table_8017a8cc[(arg * 7) + object->slots[index].entry_index];
  mask = object->pressed_bits & 0xf0ff;
  if (mask == 0)
    goto call_c8;
  if (!(entry & 0x400))
  {
    if (((mask & entry) == 0) || (((s16) entry) != ((s16) (object->input_bits_2 & 0xf0ff))))
      goto call_a8;
    if (entry & 1)
      reset_slot_stage_set_result(object, index, arg);
    else
      set_slot_random_timer_and_advance(object, index, arg);
    return;
  }
  if (!(mask & entry))
  {
  call_c8:
    clear_global_flag_ignoring_object(object);
    return;
  }
  if (entry & 1)
    reset_slot_stage_set_result(object, index, arg);
  else
    set_slot_random_timer_and_advance(object, index, arg);
}
