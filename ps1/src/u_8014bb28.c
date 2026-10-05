/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the high-byte shift (original sll 16 / sra 24 via a move; ours srl 8)
 * and, in r2, the resulting register allocation. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014bb28(Object *object)
{
  s16 new_var;
  int value = *(data_80189460++);
  Object *other = object->other;
  new_var = ((s16) value) >> 8;
  object->field_212 = new_var;
  object->field_213 = value;
  if (((other->field_240 != 0) && (other->field_14c != 0)) && ((ref_other.p = (Object *) other->field_14c, ref_other.p->field_04 != 0)))
  {
    ref_second.p = (Object *) other->field_14c;
    func_8014c4a8(object);
    if (object->field_213 >= ((s16) data_80189464))
    {
      func_8014c914(object);
    }
    else
    {
      func_8014c528(object);
    }
  }
  else
  {
    func_8014c528(object);
  }
}
