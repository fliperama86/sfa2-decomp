/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_8014d520(Object *object)
{
  unsigned int new_var;
  Object *new_var2;
  int lo;
  int hi;
  unsigned int sum;
  if ((u8)func_8014de94(object))
  {
    func_8014e080(object);
  }
  else
  {
    lo = object->field_06 == 0;
    if (lo || (object->field_164 != 0))
    {
      func_8014d99c(object);
    }
    else
    {
      hi = object->field_210;
      lo = object->field_211;
      sum = (hi << 8) + lo;
      new_var = 3;
      ;
      if (((u16) (new_var + (sum - object->field_21e))) < 6)
      {
        func_8014d99c(object);
      }
      else
      {
        new_var2 = object;
        new_var2->field_21a = 0;
        hi = ((u16) sum) < object->field_21e;
        if (!hi)
        {
          new_var2->field_21a = 1;
        }
      }
    }
  }
}
