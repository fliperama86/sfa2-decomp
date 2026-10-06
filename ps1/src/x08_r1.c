/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_801199ec(void)
{
  u8 fifteen;
  Rect r;
  int i;
  int base;
  int lim;
  u8 x;
  u8 y;
  func_8014f3b8(0, 0);
  x = 0;
  y = 0;
  i = 0;
  lim = 0x10;
  base = 0x340;
  r.w = 4;
  r.h = 0x10;
  do
  {
    r.x = (x * 4) + base;
    r.y = y << 4;
    func_80157fc4(&r, (u8 *) 0x80010000 + (i << 7));
    x++;
    if (x == lim)
    {
      y++;
    }
    if (y == lim)
    {
      base += 0x40;
    }
    x = x & (fifteen = 0xf);
    y = y % 16;
    i++;
  }
  while (i < 0x2fb);
  r.x = 0x160;
  r.y = 0x1f0;
  r.w = 0x10;
  r.h = fifteen;
  func_80157fc4(&r, (u8 *) 0x80050000);
  r.x = 0x70;
  r.y = 0x1f0;
  r.w = 0x10;
  r.h = fifteen;
  func_80157fc4(&r, (u8 *) 0x80050000);
  r.w = 0x40;
  r.h = 0x100;
  r.x = 0x3c0;
  r.y = 0x100;
  func_80157fc4(&r, (u8 *) 0x80030000);
}
