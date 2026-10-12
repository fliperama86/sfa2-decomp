/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual in restore_script_pointer_and_read_word: same swapped-register pair as read_script_word_or_restore_script (move present, registers
 * reversed, script_arg store scheduled one slot earlier). Calls without an argument (nop in the delay
 * slot) are written as unprototyped declarations. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void restore_script_pointer_and_read_word(Object *object)
{
  Object *new_var2;
  u16 v;
  u16 t;
  object->script_mode = 0;
  new_var2 = object;
  new_var2->field_20c = new_var2->saved_script_word;
  data_80189460 = (u16 *) new_var2->script_pointer;
  new_var2->script_ptr = (s32) data_80189460;
  t = *(data_80189460++);
  v = t;
  t = ((s16) v) >> 8;
  new_var2->script_opcode_high = t;
  new_var2->script_arg = v;
  v = new_var2->script_opcode_high;
  if (v == 0xff)
  {
    init_script_from_table_index(object);
    call_script_switch_by_arg(new_var2);
  }
}
