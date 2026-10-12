/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void update_slot_stage_pressed_bits_any_match(Object *object, u8 index, u8 arg)
{
  u16 entry;
  int new_var;
  int mask;
  int new_var2;
  object->slots[index].timer--;
  if (object->slots[index].timer == 0)
  {
    reset_slot_stage_clear_result(object, index, arg);
  }
  else
  {
    new_var = 0;
    entry = table_8017a8cc[(arg * 7) + object->slots[index].entry_index];
    new_var2 = entry & 0xf0ff;
    mask = 0xf0ff;
    mask = object->pressed_bits & mask;
    if ((mask == new_var) || ((mask & new_var2) == new_var))
    {
      func_8013f2c8(object);
    }
    else
      if (entry & 1)
    {
      reset_slot_stage_set_result(object, index, arg);
    }
    else
    {
      set_slot_random_timer_and_advance(object, index, arg);
    }
  }
}
