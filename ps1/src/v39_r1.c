/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
extern u16 data_801a6984;

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_801227d4(Attacker *a, int side)
{
  Object *p;
  Object *q;
  Object *t;
  Object *new_var2;
  u8 n;
  short new_var;
  u8 new_var3;
  p = &player_left;
  q = p + 1;
  new_var2 = ref_first.p = p;
  if (((u8) side) == 2)
  {
    t = new_var2;
    p = q;
    q = t;
    ref_first.p = p;
  }
  a->field_78 = p;
  a->field_7c = q;
  if (q->field_cd == 0)
  {
    q->field_db = 0xff;
  }
  p->field_ce++;
  n = a->field_48;
  if (p->field_ce == n)
  {
    a->field_45 = 0xff;
    a->field_60 = 0;
    n = n - 1;
    new_var = (new_var3 = (u8) n);
    if (a->field_42 == new_var)
    {
      a->field_60 = 0xff;
      a->field_77++;
    }
    if (a->field_1b == 3)
    {
      u8 v = p->field_d7;
      if (v < 99)
      {
        v++;
      }
      p->field_d7 = v;
      p->field_124 = v;
      q->field_d7 = 0;
      if (a->field_2f != 0)
      {
        table_801aa4e8[0][0][(p->side * 42) + p->kind]++;
        table_801aa4e8[q->side][1][q->kind]++;
        data_801aa53c[p->side]++;
      }
    }
    else
      if (p->field_cd == 0)
    {
      p->field_fc = p->field_5c;
      p->field_fe = p->field_c6;
      if ((a->field_af == 0) && (a->field_54 == a->field_a4))
      {
        a->field_90 = 0xff;
        a->field_2c = 0xff;
      }
    }
  }
}
