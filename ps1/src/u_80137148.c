/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801a27e4[];
extern s16 data_801aa4dc[];
Job *func_8011f4a4(void);

/* Form found by automatic permutation search. */
void func_80137148(void)
{
  int new_var3;
  Rect r;
  int new_var;
  int new_var4;
  u8 *p = data_801a27e4 + 0x0;
  (&r)->x = 0xc0;
  new_var = 0x10;
  r.y = 0x1e0;
  new_var4 = 0x1400;
  r.w = new_var;
  r.h = 0x7;
  func_80158028(&r, p);
  new_var3 = 0;
  p = p + new_var4;
  new_var = new_var3;
  func_80158028(&r, p);
  data_801aa4dc[new_var] = 0x1f;
}
