/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search. */
void set_motion_from_table(Object *object)
{
  u16 index;
  s32 a;
  s32 b;
  b = object->mode_flag_2 == 0;
  if (b)
  {
    b = object->input_bits_2 & 0xa000;
    index = 0;
    if (b != 0)
    {
      if (b & 0x8000)
      {
        index = 2;
      }
      else
      {
        index = 3;
      }
    }
  }
  else
  {
    index = 0;
    if (object->direction_flag != 0)
    {
      if (!(object->direction_flag & 0x80))
      {
        index = 2;
      }
      else
      {
        index = 3;
      }
    }
  }
  object->vel_y = motion_table[object->kind][index][1];
  object->accel_y = motion_table[object->kind][index][3];
  b = motion_table[object->kind][index][2];
  a = motion_table[object->kind][index][0];
  if (object->facing_side != 0)
  {
    a = -a;
    b = -b;
  }
  object->vel_x = a;
  object->accel_x = b;
}
