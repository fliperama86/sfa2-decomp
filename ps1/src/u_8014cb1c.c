/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual in func_8014cb1c: same swapped-register pair as func_8014ca7c (move present, registers
 * reversed, script_arg store scheduled one slot earlier). Calls without an argument (nop in the delay
 * slot) are written as unprototyped declarations. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_8014cb1c(Object *object)
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
    func_8014d9ac(object);
    func_8014c994(new_var2);
  }
}
