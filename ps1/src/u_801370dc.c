/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_801370dc(void)
{
  Rect r;
  u8 *new_var;
  u8 *p = data_801a27e4 + 0xc00;
  r.x = 0xf0;
  new_var = p;
  r.y = 0x1e0;
  r.w = 0x10;
  r.h = 0x20;
  func_80158028(&r, new_var);
  p = new_var + 0x1400;
  func_80158028(&r, p);
  data_801aa4dc[3] = 0x1f;
}
