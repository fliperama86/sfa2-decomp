/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the high-byte shift (original sll 16 / sra 24 via a move; ours srl 8)
 * and, in r2, the resulting register allocation. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search. */
void script_branch_if_linked_distance_ge_value(Object *object)
{
  int value = *(script_cursor++);
  s16 new_var;
  Object *other = object->other;
  new_var = ((s16) value) >> 8;
  object->script_byte_hi = new_var;
  object->script_byte_lo = value;
  if (((other->linked_object_3 == 0) || ((ref_other.p = (Object *) other->linked_object_3, ref_other.p->state_0 == 0))) || ((ref_second.p = (Object *) other->linked_object_3, store_box_distance_to_other_x(object), object->script_byte_lo <= ((s16) script_work_value))))
  {
    read_script_word_and_dispatch(object);
  }
  else
  {
    skip_script_block_then_read_opcode(object);
  }
}
