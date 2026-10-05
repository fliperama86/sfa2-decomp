/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
s16 func_801415c0(Object *object, u16 *mask, BytePair dir)
{
  u16 m = *mask;
  unsigned short new_var;
  int a = (object->field_130 & 0x4000) ? (6) : (0);
  int b = a;
  if (dir.second != 0)
  {
    b = a + 3;
  }
  new_var = m;
  a = b;
  return new_var & (1 << ((dir.first >> 1) + a));
}
