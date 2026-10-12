/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Looks the object up in a coarse grid around the reference object (inferred).
   The column comes from compare_limit plus 0x10 (32 units a column), the row from
   the absolute vertical distance to ref_other (16 units a row, 4 columns to a
   row of the table). If the column is above 0x100, the distance is 0x80 or
   more, or the cell's index byte has bit 7 set, it returns 0. Otherwise it
   copies the four bytes of the indexed record to input_dir_0, input_dir_second,
   direction_index and attack_param_d and returns 1. */
u8 func_80142030(Object *object)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  /* Without it this function differs from the original in 7 instruction slots. */
  u8 unused[24];
  /* Stand-in, not recovered source: the local x holds the column and, after the column is used, the cell's index byte (a second meaning in one local) reproduces the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
  u16 x;
  s16 dy;
  s16 col;
  x = object->compare_limit;
  dy = object->pos_y;
  dy = dy - ref_other.p->pos_y;
  if (dy < 0)
  {
    dy = -dy;
  }
  col = x + 0x10;
  if ((col > 0x100) || (dy >= 0x80))
  {
    return 0;
  }
  x = table_8017ac54[(col >> 5) + ((dy >> 4) * 4)];
  if (x & 0x80)
  {
    return 0;
  }
  object->input_dir_0 = table_8017ac7c[x][0];
  object->input_dir_second = table_8017ac7c[x][1];
  object->direction_index = table_8017ac7c[x][2];
  object->attack_param_d = table_8017ac7c[x][3];
  return 1;
}
