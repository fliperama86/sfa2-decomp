/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: 5 slots. The original stores the constant 6 (in $v1) before the two loads of
   field_20f; the build hoists the loads above the stores and uses $v0. */


/* Form found by automatic permutation search. */
void func_8014c644(Object *object)
{
  u32 *table;
  if (func_80149d48(object))
  {
    object->field_208 = 6;
    object->field_04 = 1;
    object->field_06 = 6;
    object->field_209 = 0;
    object->field_05 = 0;
    object->field_07 = 4;
    object->field_221 = object->field_20f;
    if (object->field_20f != 0)
    {
      if ((object->field_220 != 0) && (object->field_257 != 0))
      {
        if ((player_left.kind == 0xd) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[21];
        }
        else
          if ((player_left.kind == 0x10) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[22];
        }
        else
          if ((player_left.kind == 0x11) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[23];
        }
        else
          if ((player_left.kind == 0x13) && (player_right.kind == player_left.kind))
        {
          table = table_8017d4d0[24];
        }
        else
          if (object->side == 0)
        {
          table = table_8017d4d0[object->kind];
        }
        else
        {
          table = table_8017d534[object->kind];
        }
        if (func_8014a170(object, table) != 0)
        {
          return;
        }
      }
      object->field_221 = 0;
    }
  }
  else
  {
    func_8014c914(object);
  }
}
