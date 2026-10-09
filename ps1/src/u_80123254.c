/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

int func_80125394(void);

/* Form found by automatic permutation search. */
void func_80123254(GameState *g)
{
  u16 new_var;
  Object *o;
  s16 v;
  u16 n;
  if ((u8)func_80125394() == 0)
  {
    v = g->field_ca;
    v--;
    g->field_ca = v;
    if (v >= 0)
    {
      return;
    }
  }
  if ((g->field_1d & 3) == 0)
  {
    if ((((--g->field_115) & 0x80) == 0) && (((s16) (new_var = (o = g->field_78)->field_5c)) != 0x90))
    {
      new_var = o->field_5c + 1;
      n = new_var;
      o->field_5c = n;
      o->field_5e = n;
    }
    else
    {
      data_8018f5a0->field_50++;
      g->field_ca = 0x5a;
      o = g->field_78;
      o->field_fc = o->field_5c;
    }
  }
}
