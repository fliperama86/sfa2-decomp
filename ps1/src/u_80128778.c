/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Not exact. Residual: original has `sll $v0,$v0,16` after the or of the two
   status_flag bytes (a 16-bit truncation the compiler elides for u8 operands in
   every form tried), and loads left.status_flag into $v1 first, right into $v0
   second; this build has no sll (152 vs 156 bytes) and the loads in $v0/$v1. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Form found by automatic permutation search. */
void move_object_x_by_table_value(Object *object)
{
  int d;
  s16 x;
  x = player_right.field_165;
  if ((x | player_left.field_165) == 0)
  {
    x = player_left.status_flag;
    x |= player_right.status_flag;
    if ((x == 0) && (object->field_7e != 0))
    {
      d = get_direction_table_value(object);
      if (object->facing_side == 0)
      {
        d = -d;
      }
      *((s32 *) (&object->pos_x_frac)) = d + (*((s32 *) (&object->pos_x_frac)));
    }
  }
}
