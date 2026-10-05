/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_801560d4(void)
{
  u16 w;
  s16 *p;
  unsigned t;
  u8 *c;
  s16 *new_var;
  c = &data_8018d268;
  *c = (*c) + 1;
  w = game_state.field_78->field_5c;
  if (w == 0x90)
  {
    w = 300;
  }
  data_8018d26c = func_80156454(w);
  data_8018d26a = game_state.field_60;
  data_8018d269 = func_80156454(table_801815d4[game_state.field_49]);
  t = game_state.field_77;
  if (t > 10)
  {
    t = 10;
  }
  new_var = table_80181638;
  p = new_var;
  p += t;
  data_8018d274 = func_80156454(*p);
  w = w * table_801815d4[game_state.field_49];
  if (data_8018d26a != 0)
  {
    w = ((u16) (*p)) + w;
  }
  data_8018d270 = func_80156454(w);
  game_state.field_104 = data_8018d270;
  game_state.field_ca = 0x1e;
}
