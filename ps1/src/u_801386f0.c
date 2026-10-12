/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_801386f0(GameState *state)
{
  s16 t;
  s16 index;
  if (data_80188ecc != 0)
  {
    data_80188ecc = data_80188ecc - 1;
  }
  else
  {
    index = state->table_index_2;
    index = (index * 8) + state->sub_index;
    t = table_6d30[index];
    t <<= 3;
    if (state->table_index_2 < 4)
    {
      if (t < (s16) data_80188ec4)
      {
        data_80188ec4 = t;
      }
      if ((((s16) state->sub_index != 7) && ((s16) state->sub_index != 2)) && ((s16) state->sub_index != 5))
      {
        return;
      }
    }
    if (t >= (s16) data_80188ec4)
    {
      data_80188ec4 = t;
    }
  }
}
