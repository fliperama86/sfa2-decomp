/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801a27e4[];
extern s16 data_801aa4dc[];

/* Form found by automatic permutation search. */
void func_80137004(void)
{
  u8 *p;
  Rect r;
  int new_var2;
  new_var2 = 0x1e0;
  r.x = 0xd0;
  p = data_801a27e4 + 0x400;
  r.y = new_var2;
  r.w = 0x10;
  (&r)->h = 0x20;
  func_80158028(&r, p);
  func_80158028(&r, p = p + 0x1400);
  data_801aa4dc[1] = 0x1f;
}
