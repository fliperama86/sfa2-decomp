/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int func_80149d6c(Object *object)
{
  u16 new_var;
  s32 i;
  for (i = 0; i < 16; i++)
  {
    new_var = units_2c20[i].field_00;
    if ((((new_var == 1) && ((units_2c20[i].field_74 & 0x80) == 0)) && (object->field_65 != units_2c20[i].field_65)) && ((new_var = (u16) (((new_var = units_2c20[i].field_12) - object->pos_x) + 0x50)) < 0xa0))
    {
      return 1;
    }
  }

  return func_80149e3c(object, object->other) != 0;
}
