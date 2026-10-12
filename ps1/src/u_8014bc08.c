/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the high-byte shift (original sll 16 / sra 24 via a move; ours srl 8)
 * and, in r2, the resulting register allocation. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014bc08(Object *object)
{
  int value = *(data_80189460++);
  s16 new_var;
  Object *other = object->other;
  new_var = ((s16) value) >> 8;
  object->script_byte_hi = new_var;
  object->script_byte_lo = value;
  if (((other->linked_object_3 == 0) || ((ref_other.p = (Object *) other->linked_object_3, ref_other.p->state_0 == 0))) || ((ref_second.p = (Object *) other->linked_object_3, func_8014c4a8(object), object->script_byte_lo <= ((s16) data_80189464))))
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
