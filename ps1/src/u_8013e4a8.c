/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, u8 index, u8 arg);
void func_8013f2d8(Object *object, u8 index, u8 arg);

/* Form found by automatic permutation search. */
void func_8013e4a8(Object *object, u8 index, u8 arg)
{
  u16 entry;
  int new_var;
  int mask;
  int new_var2;
  object->slots[index].field_04--;
  if (object->slots[index].field_04 == 0)
  {
    func_8013f2a8(object, index, arg);
  }
  else
  {
    new_var = 0;
    entry = table_8017a8cc[(arg * 7) + object->slots[index].field_01];
    new_var2 = entry & 0xf0ff;
    mask = 0xf0ff;
    mask = object->field_134 & mask;
    if ((mask == new_var) || ((mask & new_var2) == new_var))
    {
      func_8013f2c8(object, index, arg);
    }
    else
      if (entry & 1)
    {
      func_8013f2d8(object, index, arg);
    }
    else
    {
      func_8013f0c8(object, index, arg);
    }
  }
}
