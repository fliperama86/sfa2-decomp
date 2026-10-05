/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search, then cleaned by hand. */
void func_8011fae0(u16 *src, u16 *dst)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[4];
  int new_var;
  u16 w = *(src++);
  s16 h = *(src++);
  int row;
  int col;
  for (row = 0; row < h; row++)
  {
    new_var = 4 - (((s16) w) * 128);
    for (col = 0; col < ((s16) w); col++)
    {
      *(dst++) = *(src++);
      *dst = *(src++);
      dst += 63;
    }

    dst = (u16 *) (((u8 *) dst) + new_var);
  }

}
