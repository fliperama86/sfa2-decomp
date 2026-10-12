/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



/* Form found by automatic permutation search. */
void init_sprite_position(Sprite *sprite, int x, int y)
{
  int xm = x & 0x1ff;
  int new_var;
  int xd = xm - 0x40;
  int ym;
  sprite->x_b = x;
  sprite->x_a = x;
  sprite->field_22 = x;
  game_state.sprite_offset_x = xd;
  game_state.cam_x = xm;
  xd = 0x100;
  ym = y & 0x1ff;
  xd = xd - ym;
  x = xd;
  sprite->y_b = y;
  sprite->y_a = y;
  sprite->field_26 = y;
  game_state.cam_y = ym;
  game_state.sprite_x = x;
  sprite->max_step_y = -1;
  sprite->move_limit_b = -1;
  new_var = 1;
  sprite->init_param_a = -1;
  sprite->init_param_b = -new_var;
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
