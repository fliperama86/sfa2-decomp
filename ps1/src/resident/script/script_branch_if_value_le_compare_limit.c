/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void script_branch_if_value_le_compare_limit(Object *object)
{
  u16 *p = script_cursor;
  u16 v;
  s16 w;
  u16 t;
  script_cursor = p + 1;
  v = *p;
  w = v;
  t = w >> 8;
  object->script_byte_hi = t;
  object->script_byte_lo = v;
  if (v <= object->compare_limit)
  {
    read_script_word_and_dispatch(object);
  }
  else
  {
    skip_script_block_then_read_opcode(object);
  }
}
