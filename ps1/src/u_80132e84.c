/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801ac6a8[];

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the original keeps the stores in source order between the table
   loads; the compiler here hoists the flag loads above them (19 slots). */
/* Form found by automatic permutation search. */
void func_80132e84(void)
{
  u8 *p = data_801ac6a8;
  u32 *new_var;
  Object *l;
  data_801ac870 = table_80172144[data_80171c5d];
  data_801ac878 = *(new_var = &table_80172144[data_80171c5c]);
  if (data_80171c5e)
  {
    data_801ac878 = table_80172144[11];
  }
  if (data_80171c5f)
  {
    data_801ac870 = table_80172144[12];
  }
  l = &player_left;
  func_80132f90(p, l, l + 1);
  func_80133108(p, l, l + 1);
  func_80135858();
  func_80135a10();
}
