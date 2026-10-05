/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80130678(Object *object, int index);

/* Form found by automatic permutation search. */
int func_80130258(Object *object)
{
  int new_var4;
  int held;
  int new_var3;
  unsigned short new_var2;
  Object *new_var;
  int result = 0;
  new_var = object;
  held = new_var->field_7e;
  new_var3 = held == 0;
  new_var2 = 0x1000;
  if (new_var3)
  {
    held = new_var2;
    result = object->field_130;
    new_var2 = result;
    held &= new_var2;
    new_var3 = (new_var4 = 0);
    new_var2 = held;
    result = new_var2 != new_var3;
  }
  return result;
}
