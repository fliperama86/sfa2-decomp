/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void set_limits_from_script_word(Object *object)
{
  u16 v = *(script_cursor++);
  s16 new_var;
  new_var = ((s16) v) >> 8;
  script_work_value = v;
  object->limit_hi = new_var;
  object->limit_lo = v;
}
