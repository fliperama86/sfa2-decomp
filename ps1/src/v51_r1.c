/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 func_80139f84(Object *a0, Object *a1, Box32 *a2);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
int func_80139f54(Object *a0, Object *a1, Box32 *a2)
{
  if ((data_80188f30 & 2) && a2->field_0d == 6)
  {
    return 1;
  }
  return 0;
}
