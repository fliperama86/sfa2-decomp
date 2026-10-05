/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_801396e4(s32 a, Object *object)
{
  u16 x;
  u16 y;
  int new_var;
  if (func_801397d0(a, object, object->boxes_c + (*object).frame->box_c) == 0)
  {
    new_var = data_80188ed0.out_40;
    x = data_80188ed0.out_3c;
    y = new_var;
    data_80188ed0.f_1c = x;
    data_80188f30 = data_80188f30 + 4;
    data_80188ed0.f_34 = y;
    x = x - y;
    if (((s16) x) < 0)
    {
      x = -x;
    }
    data_80188ed0.in_08 = x;
    x = data_80188ed0.out_44;
    y = data_80188ed0.out_48;
    data_80188ed0.f_20 = x;
    data_80188ed0.f_38 = y;
    x = x - y;
    if (((s16) x) < 0)
    {
      x = -x;
    }
    data_80188ed0.in_08 = x + data_80188ed0.in_08;
  }
}
