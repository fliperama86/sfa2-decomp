/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Block172 *ref_third;

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT: original keeps the address of the cursor global in a3 (copied from
   a temp) plus a direct access for the field_08 store, and allocates i/slot
   to a2/a1. Mine differs in register allocation and hoisting (16 slots). */
/* Form found by automatic permutation search. */
void func_8011ee64(void)
{
  unsigned int i;
  unsigned int j;
  u8 *p;
  Block172 **slot;
  Block172 **cursor = &ref_third;
  Block172 *cur;
  i = 0;
  data_801a27d4 = 8;
  data_801a27cc = 8;
  data_80197f10 = 0x27;
  *cursor = data_801a89f4;
  do
  {
    slot = cursor;
    p = (u8 *) (*slot);
    for (j = 0; j < 0xac; j++)
    {
      *(p++) = 0;
    }

    ref_third->field_08 = 0xc;
    cur = *slot;
    *slot = cur + 1;
    table_80197f20[i] = cur;
    i++;
  }
  while (i < 40);
}
