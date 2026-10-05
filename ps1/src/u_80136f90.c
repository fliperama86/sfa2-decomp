/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s16 data_801aa4dc;


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (not exact, 108 of 108 bytes, 2 slots): the original places
   addiu a0,sp,0x10 after the address setup of the table in s0; this build
   puts it first, one slot earlier. Tried: local pointer for the rect, table
   local versus direct argument, assignment order of the rect fields and of
   the table pointer. */
/* Form found by automatic permutation search. */
void func_80136f90(void)
{
  Rect rect;
  u8 *new_var;
  rect.x = 0xb0;
  rect.y = 0x1f0;
  rect.w = 0x10;
  rect.h = 0xa;
  new_var = table_801a2a84;
  func_80158028(&rect, new_var);
  func_80158028(&rect, new_var = &table_801a2a84[0x1400]);
  data_801aa4dc = 0x1f;
}
