/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_801395f8(s32 a, Object *object)
{
  u16 x;
  long new_var;
  u16 y;
  if (func_801397d0(a, object, object->boxes_b + object->frame->box_b) == 0)
  {
    x = data_80188ed0.out_3c;
    new_var = data_80188ed0.out_40;
    y = new_var;
    data_80188ed0.f_14 = x;
    data_80188f30 = data_80188f30 + 2;
    data_80188ed0.f_2c = y;
    x = x - y;
    if (((s16) x) < 0)
    {
      x = -x;
    }
    data_80188ed0.in_04 = x;
    x = data_80188ed0.out_44;
    y = data_80188ed0.out_48;
    data_80188ed0.f_18 = x;
    data_80188ed0.f_30 = y;
    x = x - y;
    if (((s16) x) < 0)
    {
      x = -x;
    }
    data_80188ed0.in_04 = x + data_80188ed0.in_04;
  }
}
