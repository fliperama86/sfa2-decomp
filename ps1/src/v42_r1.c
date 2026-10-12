/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void init_cam_position(Cam *cam, int x, int y)
{
  int a;
  short new_var2;
  int b;
  a = x & 0x7ff;
  cam->field_12 = x;
  cam->field_0a = x;
  new_var2 = a - 0x40;
  cam->pos_x = x;
  game_state.field_e6 = new_var2;
  game_state.field_d6 = a;
  b = y & 0x7ff;
  cam->field_16 = y;
  cam->field_0e = y;
  cam->pos_y = y;
  game_state.field_d8 = b;
  new_var2 = 0x700;
  new_var2 = new_var2 - b;
  game_state.field_e8 = new_var2;
  *(s32 *)&cam->field_34 = 0;
  *(s32 *)&cam->field_38 = 0;
  cam->field_48 = 0;
  cam->field_4a = 0;
  cam->field_10 = 0;
  cam->field_14 = 0;
  cam->field_08 = 0;
  cam->field_0c = 0;
  cam->field_20 = 0;
  cam->field_24 = 0;
  cam->field_40 = 0;
  cam->field_42 = 0;
  cam->min_x = 0;
  cam->max_x = 0;
  cam->field_01 = 0xff;
}
