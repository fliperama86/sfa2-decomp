/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_801259a8(Select *sel, Object *a, Object *b)
{
  int flags;
  s16 i;
  u8 j;
  u8 count;
  u8 flag;
  u32 mask;
  if (a->field_cd == 0)
  {
    flags = sel->field_50;
    if (b->field_cd != 0)
    {
      count = table_8016f4f0[b->kind];
      j = 0;
      mask = 1;
      for (; j < count; j++)
      {
        mask <<= 1;
      }

      flags |= mask;
      sel->field_50 = flags;
    }
    sel->field_54 = -1;
    func_80125938(sel, a, b);
    i = (mask = sel->field_54);
    again:
    i++;

    flag = sel->field_120[i];
    if (!(i < sel->field_a4))
    {
      goto out;
    }
    j = 0;
    mask = 1;
    for (; j < flag; j++)
    {
      mask <<= 1;
    }

    mask &= flags;
    if (mask)
    {
      goto again;
    }
    out:
    sel->field_54 = i;

    if (i > sel->field_a4)
    {
      return;
    }
    b->kind = flag;
    sel->field_40 = (u8) func_80125afc(flag);
    func_80125b34(sel, a);
  }
}
