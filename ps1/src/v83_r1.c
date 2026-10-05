/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80132f90(Bars *o, Object *a, Object *b)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[16];
  s16 ha;
  s16 hb;
  int i;
  int ta;
  int tb;
  ta = (s16) a->field_5c;
  ha = ta;
  if (ta < 0)
  {
    ha = 0;
  }
  tb = (s16) b->field_5c;
  hb = tb;
  if (tb < 0)
  {
    hb = 0;
  }
  if (((s16) ha) < ((s16) a->field_5e))
  {
    if ((game_state.field_32 & 3) == 0)
    {
      a->field_5e = a->field_5e - 1;
    }
  }
  if (((s16) hb) < ((s16) b->field_5e))
  {
    if ((game_state.field_32 & 3) == 0)
    {
      b->field_5e = b->field_5e - 1;
    }
  }
  if (((s16) ha) > ((s16) a->field_5e))
  {
    if ((game_state.field_32 & 3) == 0)
    {
      a->field_5e = a->field_5e + 1;
    }
  }
  if (((s16) hb) > ((s16) b->field_5e))
  {
    if ((game_state.field_32 & 3) == 0)
    {
      b->field_5e = b->field_5e + 1;
    }
  }
  for (i = 0; i < 2; i++)
  {
    Bars *out = (Bars *) (((u8 *) o) + (i * 0x10));
    out->field_0c = 0x98 - ha;
    out->field_10 = ha;
    out->field_4c = 0x98 - a->field_5e;
    out->field_50 = a->field_5e - ha;
    out->field_2c = 0xd8;
    out->field_30 = hb;
    out->field_6c = hb + 0xd8;
    out->field_70 = b->field_5e - hb;
  }

}
