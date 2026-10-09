/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80136898(Cam *cam, int delta);

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80136668(Cam *cam, short step)
{
  if (step >= 5)
  {
    step = 5;
  }
  cam->field_22 += step;
  func_80136898(cam, (short) ((short) cam->field_22 < cam->field_46 ? cam->field_22 : cam->field_46));
}
