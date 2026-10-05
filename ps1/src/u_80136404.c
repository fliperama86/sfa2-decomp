/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search. */
void func_80136404(Sprite *sprite, int x, int y)
{
  int xm = x & 0x3ff;
  int xd = xm - 0x100;
  int ym;
  sprite->field_12 = x;
  sprite->field_0a = x;
  sprite->field_22 = x;
  game_state.field_e2 = xd;
  game_state.field_d2 = xm;
  ym = y & 0x3ff;
  xd = 0x300;
  xd = xd - ym;
  sprite->field_16 = y;
  sprite->field_0e = y;
  sprite->field_26 = y;
  game_state.field_d4 = ym;
  game_state.field_e4 = xd;
  sprite->field_40 = 0x10;
  sprite->field_44 = 0x100;
  sprite->field_46 = 0x280;
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
  sprite->field_42 = 0;
  sprite->pending = 0xff;
}
