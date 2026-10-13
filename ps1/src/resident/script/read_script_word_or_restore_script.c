/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual in read_script_word_or_restore_script: the original has the same move between the load and the shifts, but with
 * the two temporaries in swapped registers ($v1 loaded, $v0 copy); ours is $v0 / $v1, and the store of
 * script_arg therefore schedules before the reload. Calls without an argument (nop in the delay slot)
 * are written as unprototyped declarations. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void read_script_word_or_restore_script(Object *object)
{
  u16 v;
  u16 t;
  if (object->script_mode != 0)
  {
    restore_script_pointer_and_read_word(object);
  }
  else
  {
    init_script_from_table_index(object);
    script_cursor = (u16 *) object->script_pointer;
    object->script_ptr = (s32) script_cursor;
    t = *(script_cursor++);
    v = t;
    t = ((s16) v) >> 8;
    object->script_opcode_high = t;
    object->script_arg = v;
    if ((v = object->script_opcode_high) == 0xff)
    {
      call_script_switch_by_arg(object);
    }
  }
}
