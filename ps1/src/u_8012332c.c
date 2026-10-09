/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;

/* Form found by automatic permutation search. */
void func_8012332c(GameState *state)
{
  Entity *entity = (Entity *)state;
  int s0;
  int t;
  u8 v;
  if ((u8)func_80125394() == 0)
  {
    t = entity->field_ca;
    t--;
    entity->field_ca = t;
    s0 = ((short) t) >= 0;
    if (s0)
    {
      return;
    }
  }
  data_8018f5a0->field_4e = 0;
  entity->field_c8 = 0;
  data_8018f5a0->field_50 = 0;
  entity->field_ca = 0;
  data_8018f5a0->field_52 = 0;
  entity->field_cc = 0;
  entity->field_4d = 0;
  entity->field_4e = 0;
  entity->field_4b = 0;
  entity->field_27 = 0;
  entity->field_09 = 1;
  entity->field_6a = 1;
  entity->field_4a = 0x3c;
  entity->field_49 = entity->field_13;
  func_80120444(0, 3);
  func_80120444(1, 3);
  func_80120408();
  player_left.field_01 = 0;
  player_right.field_01 = 0;
  if (entity->field_45 != 0)
  {
    v = entity->field_40;
    data_801a6938 = 0;
    data_801abf08 = v;
    func_80123534(entity);
  }
  else
    if ((entity->field_a9 != 0) || (entity->field_87 != 0))
  {
    if (entity->field_2f == 0)
    {
      func_80123748(entity);
    }
    else
    {
      func_80123534(entity);
    }
  }
  else
  {
    s0 = entity->field_48 - 1;
    if (entity->field_42 == (s0 * 2))
    {
      func_80123ac4(&player_left, s0);
      func_80123ac4((&player_left) + 1, s0);
    }
    func_8011eae4();
    func_80136c8c();
    entity->field_4f = 1;
    entity->field_42++;
    if (data_80190568 != 0)
    {
      func_80135ef0(entity);
    }
    func_8013245c();
    func_801285e0();
  }
}
