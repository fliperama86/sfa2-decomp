/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80136668(Cam *cam, short step)
{
  if (step >= 5)
  {
    step = 5;
  }
  cam->pos_x += step;
  func_80136898(cam, (short) ((short) cam->pos_x < cam->max_x ? cam->pos_x : cam->max_x));
}
