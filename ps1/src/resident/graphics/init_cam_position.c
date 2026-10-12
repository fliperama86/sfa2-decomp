/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void init_cam_position(Cam *cam, int x, int y)
{
  int a;
  short new_var2;
  int b;
  a = x & 0x7ff;
  cam->prev_pos_x = x;
  cam->init_x = x;
  new_var2 = a - 0x40;
  cam->pos_x = x;
  game_state.cam_param_d = new_var2;
  game_state.cam_param_a = a;
  b = y & 0x7ff;
  cam->prev_pos_y = y;
  cam->init_y = y;
  cam->pos_y = y;
  game_state.cam_param_b = b;
  new_var2 = 0x700;
  new_var2 = new_var2 - b;
  game_state.cam_offset_b = new_var2;
  *(s32 *)&cam->field_34 = 0;
  *(s32 *)&cam->field_38 = 0;
  cam->offset_a = 0;
  cam->offset_b = 0;
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
  cam->pending_flag = 0xff;
}
