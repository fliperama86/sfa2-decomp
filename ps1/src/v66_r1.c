/* Reconstruction. Names/roles inferred, not original symbols. */
/* Exact. The table load shares a local with limit, so the compiler keeps a copy into entry; u16 new_var and unsigned hit fix the registers; the check read adds the object first. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void check_slot_input_with_level_limit(Object *object, int index, int arg)
{
  unsigned entry;
  unsigned pad;
  u16 dir;
  u16 new_var;
  unsigned hit;
  unsigned limit;
  limit = table_8017aaf8[(((u8) arg) * 7) + object->slots[(u8) index].entry_index];
  entry = limit;
  pad = object->pressed_bits | object->released_bits;
  hit = pad & ((entry & 0xff) + 3);
  dir = pad;
  if (hit != 0)
  {
    limit = 2;
    if (hit & 0x80)
    {
      goto check;
    }
    new_var = hit;
    if (new_var & 0x40)
    {
      goto check;
    }
    limit = 4;
    if (hit & 0x10)
    {
      goto check;
    }
    if (hit & 0x20)
    {
      goto check;
    }
    limit = 6;
    if (hit & 4)
    {
      goto check;
    }
    if (hit & 8)
    {
      goto check;
    }
    if ((entry & 0x194) && (dir == 1))
    {
      goto check;
    }
    if (!(entry & 0x868))
    {
      goto tail;
    }
    if (dir != 2)
    {
      goto tail;
    }
    check:
    if ((*((u8 *) ((u8 *) object + ((u8) index << 3)) + 0x2b2)) >= limit)
    {
      reset_slot_stage_set_result(object, (u8) index, (u8) arg);
      return;
    }

  }
  tail:
  object->slots[(u8) index].timer--;

  if (object->slots[(u8) index].timer == 0)
  {
    object->slots[(u8) index].repeat_count -= 2;
    if (object->slots[(u8) index].repeat_count == 0)
    {
      reset_slot_stage_clear_result(object, (u8) index, (u8) arg);
    }
    object->slots[(u8) index].timer = 2;
  }
  func_8013f2c8(object);
}
