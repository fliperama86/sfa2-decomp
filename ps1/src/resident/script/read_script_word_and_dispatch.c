/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the s16 copy, >> 8 into a u16 temporary form now gives the original
 * sll 16 / sra 24. Still missing: the original has a move of the loaded halfword
 * into a second register before the shifts (and stores that copy as the low byte);
 * 3 forms tried, the compiler propagates the copy away. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void read_script_word_and_dispatch(Object *object)
{
  u16 v = *(script_cursor++);
  s16 w;
  w = v;
  v = w >> 8;
  object->script_opcode_high = v;
  object->script_arg = w;
  object->script_ptr = (s32) script_cursor;
  w = object->script_opcode_high;
  if (w != 0xff)
  {
    call_script_opcode_table_by_high_byte(object);
    return;
  }
  call_script_switch_by_arg(object);
  call_script_opcode_table_by_high_byte(object);
}
