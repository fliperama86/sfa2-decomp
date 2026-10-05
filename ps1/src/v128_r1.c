/* Reconstruction. Names/roles inferred, not original symbols.
 * The low byte is assigned first and the shifted high byte is ORed in after it;
 * the other operand order swaps the operands of the final or. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014de94(Object *object);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8014d03c(Object *object)
{
  Object *new_var;
  if (func_8014de94(object))
  {
    func_8014e080(object);
  }
  else
  {
    Object *other = object->other;
    Object *target;
    unsigned x;
    new_var = object;
    if ((((other->field_240 == 0) || ((target = (Object *) other->field_14c) == 0)) || ((ref_other.p = target, ref_other.p->field_04 == 0))) || ((ref_second.p = target, func_8014c4a8(new_var), (x = new_var->field_211, x |= (new_var->field_210 << 8), data_80189464 < x))))
    {
      func_8014d99c(object);
    }
  }
}
