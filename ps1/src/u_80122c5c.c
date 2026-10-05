/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int func_80122c5c(Entity *entity)
{
  Object *s0;
  int new_var;
  if ((entity->field_4d == 0) && (entity->field_116 == 0))
  {
    entity->field_117--;
    if (entity->field_117 == 0)
    {
      if (entity->field_a6 == 0)
      {
        func_80123b20(entity->field_7c, entity);
      }
      else
      {
        func_80123b20(s0 = &player_left, entity);
        s0 = (&player_left) + 1;
        func_80123b20(s0, entity);
      }
    }
  }
  if ((entity->field_76 != 0) && (entity->field_76 == 0x1e))
  {
    entity->field_76--;
    if (entity->field_a6 == 0)
    {
      if (entity->field_30 == 0)
      {
        data_80190474 = 1;
      }
      if (entity->field_4d == 0)
      {
        new_var = 0;
        s0 = entity->field_78;
        if (s0->field_cd == new_var)
        {
          func_80120554(0, 0, 0x208);
        }
        else
        {
          func_80120554(new_var, new_var, 0x209);
        }
      }
    }
  }
}
