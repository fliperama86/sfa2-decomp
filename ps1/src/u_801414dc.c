/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
u16 *func_801414dc(Object *object)
{
  u8 x = object->input_dir_0;
  u8 y = object->input_dir_second;
  int a = (x) ? (6) : (0);
  u16 *p = table_80171748[object->kind];
  int b = (y) ? (a - -3) : (a);
  a = b;
  return p + ((object->direction_index >> 1) + a);
}
