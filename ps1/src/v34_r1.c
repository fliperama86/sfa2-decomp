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
  if (a->mode_flag_2 == 0)
  {
    flags = sel->excluded_mask;
    if (b->mode_flag_2 != 0)
    {
      count = table_8016f4f0[b->kind];
      j = 0;
      mask = 1;
      for (; j < count; j++)
      {
        mask <<= 1;
      }

      flags |= mask;
      sel->excluded_mask = flags;
    }
    sel->selected_index = -1;
    load_select_entries(sel, a, b);
    i = (mask = sel->selected_index);
    again:
    i++;

    flag = sel->entries[i];
    if (!(i < sel->index_limit))
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
    sel->selected_index = i;

    if (i > sel->index_limit)
    {
      return;
    }
    b->kind = flag;
    sel->kind_value = (u8) func_80125afc(flag);
    func_80125b34(sel, a);
  }
}
