/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80130678(Object *object, int index);

/* Form found by automatic permutation search. */
void func_80130088(Object *object)
{
  u16 index;
  s32 a;
  s32 b;
  b = object->field_cd == 0;
  if (b)
  {
    b = object->field_130 & 0xa000;
    index = 0;
    if (b != 0)
    {
      if (b & 0x8000)
      {
        index = 2;
      }
      else
      {
        index = 3;
      }
    }
  }
  else
  {
    index = 0;
    if (object->field_21a != 0)
    {
      if (!(object->field_21a & 0x80))
      {
        index = 2;
      }
      else
      {
        index = 3;
      }
    }
  }
  object->field_50 = motion_table[object->kind][index][1];
  object->field_58 = motion_table[object->kind][index][3];
  b = motion_table[object->kind][index][2];
  a = motion_table[object->kind][index][0];
  if (object->field_0b != 0)
  {
    a = -a;
    b = -b;
  }
  object->field_4c = a;
  object->field_54 = b;
}
