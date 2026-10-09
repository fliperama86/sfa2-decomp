/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_801371b4(void)
{
  Rect r;
  Rect *new_var;
  int new_var3;
  int new_var2;
  u8 *p = data_801a27e4 + 0x1200;
  new_var3 = 4;
  new_var2 = 0x160;
  r.x = new_var2;
  r.y = 0x1f0;
  r.w = 0x10;
  r.h = 0xf;
  func_80158028(&r, data_801a27e4 + 0x1200);
  new_var = &r;
  p = p + 0x1400;
  func_80158028(new_var, p);
  data_801aa4dc[new_var3] = 0x1f;
}
