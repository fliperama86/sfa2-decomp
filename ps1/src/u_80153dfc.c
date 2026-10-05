/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern TextObj *table_8018049c[];

/* Form found by automatic permutation search. */
void func_80153dfc(Actor *a)
{
  int new_var;
  u8 d[7];
  int i;
  u8 *q;
  unsigned v;
  u8 nz;
  TextObj *t;
  v = a->field_10;
  i = 0;
  do
  {
    d[i] = v & 0xf;
    v = v >> 4;
    i++;
  }
  while (i < 7);
  nz = 0;
  i = 0;
  new_var = 0x20;
  t = table_8018049c[a->field_01];
  q = d + 6;
  do
  {
    u8 c = *q;
    v = c;
    if (v != 0)
    {
      nz = 1;
    }
    if (nz)
    {
      t->buf[i] = c + 0x30;
    }
    else
    {
      t->buf[i] = new_var;
    }
    i++;
    q--;
  }
  while (i < 6);
  t->buf[i] = d[6 - i] + 0x30;
  func_801519b4(t);
}
