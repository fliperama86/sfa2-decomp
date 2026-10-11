/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Looks the object up in a coarse grid around the reference object (inferred).
   The column comes from field_21e plus 0x10 (32 units a column), the row from
   the absolute vertical distance to ref_other (16 units a row, 4 columns to a
   row of the table). If the column is above 0x100, the distance is 0x80 or
   more, or the cell's index byte has bit 7 set, it returns 0. Otherwise it
   copies the four bytes of the indexed record to field_128, field_129,
   field_12a and field_219 and returns 1. */
u8 func_80142030(Object *object)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. Without it this function differs from the original in 7 instruction slots. */
  u8 unused[24];
  /* Stand-in, not recovered source: the local x holds the column and, after the column is used, the cell's index byte (a second meaning in one local) reproduces the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
  u16 x;
  s16 dy;
  s16 col;
  x = object->field_21e;
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
  object->field_128 = table_8017ac7c[x][0];
  object->field_129 = table_8017ac7c[x][1];
  object->field_12a = table_8017ac7c[x][2];
  object->field_219 = table_8017ac7c[x][3];
  return 1;
}
