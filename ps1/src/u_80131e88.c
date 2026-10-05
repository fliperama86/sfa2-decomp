/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s8 data_801a6984;
extern u16 data_801a6966;


/* Form found by automatic permutation search. */
void func_80131e88(Object *object)
{
  Pair *new_var;
  u16 d;
  if ((object->field_61 == 0) && (object->field_62 >= 2))
  {
    u8 hit;
    if (!(func_80151184() & 1))
    {
      hit = func_80146840(object);
    }
    else
    {
      hit = func_80146864(object);
    }
    if (hit)
    {
      new_var = table_80171b48;
      game_state.field_358->pos_y -= new_var[object->kind].second;
      d = new_var[object->kind].first;
      if (object->field_0b != 0)
      {
        d = -d;
      }
      game_state.field_358->pos_x += d;
    }
  }
}
