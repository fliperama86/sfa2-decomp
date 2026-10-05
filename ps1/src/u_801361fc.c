/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search. */
void func_801361fc(Sprite *sprite, int x, int y)
{
  int xm = x & 0x1ff;
  int new_var;
  int xd = xm - 0x40;
  int ym;
  sprite->field_12 = x;
  sprite->field_0a = x;
  sprite->field_22 = x;
  game_state.field_de = xd;
  game_state.field_ce = xm;
  xd = 0x100;
  ym = y & 0x1ff;
  xd = xd - ym;
  x = xd;
  sprite->field_16 = y;
  sprite->field_0e = y;
  sprite->field_26 = y;
  game_state.field_d0 = ym;
  game_state.field_e0 = x;
  sprite->field_40 = -1;
  sprite->field_42 = -1;
  new_var = 1;
  sprite->field_44 = -1;
  sprite->field_46 = -new_var;
  sprite->field_34 = 0;
  sprite->field_38 = 0;
  sprite->field_48 = 0;
  sprite->field_4a = 0;
  sprite->field_10 = 0;
  sprite->field_14 = 0;
  sprite->field_08 = 0;
  sprite->field_0c = 0;
  sprite->field_20 = 0;
  sprite->field_24 = 0;
  sprite->pending = 0xff;
}
