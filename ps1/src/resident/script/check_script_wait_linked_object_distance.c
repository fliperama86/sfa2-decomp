/* Reconstruction. Names/roles inferred, not original symbols.
 * The low byte is assigned first and the shifted high byte is ORed in after it;
 * the other operand order swaps the operands of the final or. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void check_script_wait_linked_object_distance(Object *object)
{
  Object *new_var;
  if ((u8) roll_random_bit_check_from_kind_table(object))
  {
    init_object_mode_2(object);
  }
  else
  {
    Object *other = object->other;
    Object *target;
    unsigned x;
    new_var = object;
    if ((((other->link_flags == 0) || ((target = (Object *) other->linked_object_3) == 0)) || ((ref_other.p = target, ref_other.p->state_0 == 0))) || ((ref_second.p = target, store_box_distance_to_other_x(new_var), (x = new_var->limit_lo, x |= (new_var->limit_hi << 8), data_80189464 < x))))
    {
      clear_three_script_bytes(object);
    }
  }
}
