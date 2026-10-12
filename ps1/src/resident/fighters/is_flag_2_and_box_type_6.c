/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
int is_flag_2_and_box_type_6(Object *a0, Object *a1, Box32 *a2)
{
  if ((data_80188f30 & 2) && a2->box_type == 6)
  {
    return 1;
  }
  return 0;
}
