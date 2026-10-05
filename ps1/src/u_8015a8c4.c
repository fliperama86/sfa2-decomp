/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
int func_8015a8c4(u32 *p, u32 *out)
{
  int new_var;
  u32 n;
  if ((*(p++)) != 0x10)
  {
    return -1;
  }
  out[0] = *(p++);
  if (func_80157c94() == 2)
  {
    func_8015a550(str_8016dcec, 0x10);
  }
  if (func_80157c94() == 2)
  {
    func_8015a550(str_8016dcf8, out[0]);
  }
  if (func_80157c94() == 2)
  {
    func_8015a550(str_8016dd04, p);
  }
  if (out[0] & 8)
  {
    n = p[0] >> 2;
    out[1] = (u32) (p + 1);
    out[2] = (u32) (p + 3);
    p += n;
  }
  else
  {
    n = 0;
    out[1] = 0;
    out[2] = 0;
  }
  new_var = (p[0] >> 2) + 2;
  out[3] = (u32) (p + 1);
  out[4] = (u32) (p + 3);
  return n + new_var;
}
