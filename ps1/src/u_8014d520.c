/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void check_script_wait_approach_limit_set_direction(Object *object)
{
  unsigned int new_var;
  Object *new_var2;
  int lo;
  int hi;
  unsigned int sum;
  if ((u8)roll_random_bit_check_from_kind_table(object))
  {
    init_object_mode_2(object);
  }
  else
  {
    lo = object->state_2 == 0;
    if (lo || (object->bound_push_flag != 0))
    {
      clear_three_script_bytes(object);
    }
    else
    {
      hi = object->limit_hi;
      lo = object->limit_lo;
      sum = (hi << 8) + lo;
      new_var = 3;
      ;
      if (((u16) (new_var + (sum - object->compare_limit))) < 6)
      {
        clear_three_script_bytes(object);
      }
      else
      {
        new_var2 = object;
        new_var2->direction_flag = 0;
        hi = ((u16) sum) < object->compare_limit;
        if (!hi)
        {
          new_var2->direction_flag = 1;
        }
      }
    }
  }
}
