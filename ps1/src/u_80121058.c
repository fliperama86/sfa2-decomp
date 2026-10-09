/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

/* Form found by automatic permutation search. */
void func_80121058(GameState *state)
{
  Entity *entity = (Entity *)state;
  char new_var;
  if (data_8018f5a0->field_4c == 0)
  {
    func_8011a744();
    data_8018f5a0->field_4c++;
    new_var = 1;
    if (((data_8016e68b != 0) || (entity->field_2f != 0)) || (entity->field_30 != 0))
    {
      func_8014edac(new_var);
    }
    else
    {
      func_8014edac(0);
    }
    data_8018f5a0->field_4e = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    data_8018f5a0->field_54 = 0;
  }
  else
  {
    if ((entity->field_2f == 0) && (entity->field_30 == 0))
    {
      if (data_8016e68b != 0)
      {
        func_801b0000(entity);
      }
      else
      {
        func_80010000(entity);
      }
    }
    if ((entity->field_2f != 0) && (entity->field_30 == 0))
    {
      func_801b2194(entity);
    }
    if ((entity->field_2f == 0) && (entity->field_30 != 0))
    {
      func_801b4a88(entity);
    }
  }
  if (data_8018f5a0->field_4c == 2)
  {
    data_8018f5a0->field_4c = 0;
    data_8018f5a0->field_4a++;
  }
}
