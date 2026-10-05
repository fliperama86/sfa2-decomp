/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_8014b870, func_8014b8d8 (and r2 func_8014be78, func_8014bee8): the original computes
   field_212 as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014b870(Object *object)
{
  short new_var;
  short new_var2;
  u16 v = *(data_80189460++);
  new_var = v;
  new_var2 = new_var >> 8;
  object->field_212 = new_var2;
  object->field_213 = v;
  if (v >= object->field_21e)
  {
    func_8014c914(object);
  }
  else
  {
    func_8014c528(object);
  }
}
