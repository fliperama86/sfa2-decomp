/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (not exact, 204 bytes, 7 slots differ): the original loads the halfword into $v1 and
 * copies it to $v0 (stored as the low byte), shifting from $v0; here the load goes to $v0 and the
 * copy to $v1. Pure register-assignment swap of the loaded value and its copy. Forms tried: see
 * u16 t reused for load and later script_mode read, s16 w copy; int/u16 copies (copy folded away). */
/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int read_script_word_and_run_opcode(Object *object)
{
  u16 *p = (u16 *) object->script_ptr;
  u16 t;
  s16 w;
  data_80189460 = p + 1;
  t = *p;
  w = t;
  t = w >> 8;
  object->script_opcode_high = t;
  object->script_arg = w;
  w = object->script_opcode_high;
  if (w == 0xff)
  {
    call_script_switch_by_arg(object);
  }
  func_8014a5c0(object);
  object->script_ptr = (s32) data_80189460;
  t = object->script_mode;
  if (t == 0)
  {
    object->script_pointer = (s32) data_80189460;
    return;
  }
  if (t == 1)
  {
    object->script_ptr_1 = (s32) data_80189460;
    return;
  }
  if (t == 2)
  {
    object->script_ptr_2 = (s32) data_80189460;
    return;
  }
  if (t == 3)
  {
    object->script_ptr_3 = (s32) data_80189460;
  }
}
