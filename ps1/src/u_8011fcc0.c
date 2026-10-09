/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_8011fcc0(SeqRec *rec)
{
  u8 f0;
  u8 f1;
  u8 f2;
  u8 f3;
  u16 *p;
  Rect r;
  u16 a;
  short v;
  short kind;
  u16 b;
  if (data_8018f598 != 0)
  {
    return;
  }
  f0 = 0;
  f3 = 0;
  f2 = 0;
  f1 = 0;
  p = rec->data;
  r.w = 0x10;
  r.h = 1;
  do
  {
    r.x = *(p++);
    r.y = *(p++);
    a = *(p++);
    b = *(p++);
    v = b;
    b = (kind = v & 0xff);
    v &= 0x8000;
    if (kind == 0)
    {
      f0 = 1;
    }
    else
      if (kind == 1)
    {
      f1 = 1;
    }
    else
      if (kind == 2)
    {
      f2 = 1;
    }
    else
      if (kind == 3)
    {
      f3 = 1;
    }
    func_80137640(r, a, b);
  }
  while (v == 0);
  if (f1)
  {
    func_80137220(1, 0);
  }
  if (f2)
  {
    func_80137220(2, 1);
  }
  if (f3)
  {
    func_80137220(3, 2);
  }
  if (f0)
  {
    func_80137220(0, 6);
  }
}
