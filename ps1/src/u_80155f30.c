/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_80155f30(Object *o)
{
  struct Object *new_var;
  new_var = o;
  if (new_var->field_cd == 0)
  {
    u32 i = new_var->field_6a;
    if (i >= 100)
    {
      i = 99;
    }
    new_var = new_var->other;
    func_80155cc8(table_80181380[i], (s8) new_var->side);
  }
}
