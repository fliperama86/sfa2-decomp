/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Not exact. Residual: original has `sll $v0,$v0,16` after the or of the two
   field_73 bytes (a 16-bit truncation the compiler elides for u8 operands in
   every form tried), and loads left.field_73 into $v1 first, right into $v0
   second; this build has no sll (152 vs 156 bytes) and the loads in $v0/$v1. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Form found by automatic permutation search. */
void func_80128778(Object *object)
{
  int d;
  s16 x;
  x = player_right.field_165;
  if ((x | player_left.field_165) == 0)
  {
    x = player_left.field_73;
    x |= player_right.field_73;
    if ((x == 0) && (object->field_7e != 0))
    {
      d = func_80128814(object);
      if (object->field_0b == 0)
      {
        d = -d;
      }
      *((s32 *) (&object->field_10)) = d + (*((s32 *) (&object->field_10)));
    }
  }
}
