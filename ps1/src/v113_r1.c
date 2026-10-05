/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Written by hand in the form of func_801395f8, whose form the automatic permutation search had found. */
void func_8013950c(s32 a, Object *object)
{
  u16 x;
  long new_var;
  u16 y;
  if (func_801397d0(a, object, object->boxes_a + object->frame->box_a) == 0)
  {
    x = data_80188ed0.out_3c;
    data_80188ed0.f_0c = x;
    data_80188f30++;
    new_var = data_80188ed0.out_40;
    y = new_var;
    data_80188ed0.f_24 = y;
    x = x - y;
    if (((s16) x) < 0)
    {
      x = -x;
    }
    data_80188ed0.in_00 = x;
    x = data_80188ed0.out_44;
    y = data_80188ed0.out_48;
    data_80188ed0.f_10 = x;
    data_80188ed0.f_28 = y;
    x = x - y;
    if (((s16) x) < 0)
    {
      x = -x;
    }
    data_80188ed0.in_00 = x + data_80188ed0.in_00;
  }
}
